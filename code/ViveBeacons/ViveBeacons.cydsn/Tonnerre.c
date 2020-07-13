/* ============================================================================
 *
 *  Copyright (C) 2019 - Evan Roué
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * ============================================================================
*/

#include <stdlib.h>
#include "main.h"
#include "Tonnerre.h"
#include "Tonnerre_commands.h"

/*
    ---------------------------------------------------------------------------
    A PSoC Library that implements the Tonnerre protocol.
    
    See the documentation in docs/reports
    ---------------------------------------------------------------------------
*/

/*
    --------------
    Public methods
    --------------
*/

Tonnerre* Tonnerre_create() {
    Tonnerre* tonnerre = (Tonnerre *) malloc(1*sizeof(Tonnerre));

    tonnerre->msg_id = 0;
    
    for(int i = 0; i < TONNERRE_MAX_MSG_ID+1; i++) {
        tonnerre->callbacks[i].command = i;
        tonnerre->callbacks[i].callback_function = NULL;
    }
    
    for(int i = 0; i < TONNERRE_MAX_COMMANDS_MANAGEMENT_SYSTEM_ENTRIES; i++)
        tonnerre->commands_tabs[0].valid = false;
    
    return tonnerre;
}

void Tonnerre_init(Tonnerre *tonnerre, XBee_driver *xbee_driver) {
    tonnerre->xbee_driver = xbee_driver;

    // Initializing xBee_driver module
    XBee_driver_init(tonnerre->xbee_driver);
    XBee_driver_register_callback(tonnerre->xbee_driver, RX_16_RESPONSE, Tonnerre_dispatch);
    
    // Tonnerre callback registration
    Tonnerre_register_callback(tonnerre, ACK, Tonnerre_commands_ack_callback);
    Tonnerre_register_callback(tonnerre, POS, Tonnerre_commands_pos_callback);
    Tonnerre_register_callback(tonnerre, PING, Tonnerre_commands_ping_callback);
    Tonnerre_register_callback(tonnerre, PONG, Tonnerre_commands_pong_callback);
    Tonnerre_register_callback(tonnerre, SBPOS, Tonnerre_commands_sbpos_callback);
    Tonnerre_register_callback(tonnerre, SP, Tonnerre_commands_start_protocol_callback);
    Tonnerre_register_callback(tonnerre, EP, Tonnerre_commands_end_protocol_callback);
    Tonnerre_register_callback(tonnerre, SM, Tonnerre_commands_start_match_callback);
    Tonnerre_register_callback(tonnerre, EM, Tonnerre_commands_end_match_callback);
}

void Tonnerre_check_commands(Tonnerre* tonnerre) {
    XBee_driver_check_commands(tonnerre->xbee_driver);
}

void Tonnerre_send_command(Tonnerre* tonnerre, uint8_t command, uint16_t vive_ID, uint8_t data[TONNERRE_MAX_ARG_SIZE], uint8_t data_length, bool require_response) {
    XBee_TX_frame TX_frame;
    uint16_t TX_frame_length;
    uint8_t payload[TONNERRE_MAX_ARG_SIZE + 2];
    
    payload[0] = command;
    payload[1] = tonnerre->msg_id;
    
    if(data_length != 0)
        memcpy(payload + 2, data, data_length);
    
    XBee_driver_create_TX_request(tonnerre->xbee_driver, &TX_frame, &TX_frame_length, vive_ID, payload, data_length + 2);
    XBee_driver_send(tonnerre->xbee_driver, &TX_frame, TX_frame_length);
    
    if(require_response) {
        int index = Tonnerre_commands_management_system_get_next_empty_entry(tonnerre);
        
        if(index != -1) { // If an index was found
            tonnerre->commands_tabs[index].valid = true;
            
            tonnerre->commands_tabs[index].command = command;
            tonnerre->commands_tabs[index].vive_ID = vive_ID;
            tonnerre->commands_tabs[index].msg_id = tonnerre->msg_id;
            
            tonnerre->commands_tabs[index].data_length = data_length;
            
            if(data_length != 0)
                memcpy(tonnerre->commands_tabs[index].data, data, data_length);
        } else {
            // TODO : Don't know what to do, just ignore the ACK sorry :(   
        }
    }
    
    tonnerre->msg_id++;
}

/*
    ---------------------------------------------------------------------------
    Name : Send Response
    Description : This method is used to send response-like messages
    (for ACK and PONG).
    ---------------------------------------------------------------------------
*/

void Tonnerre_send_response(Tonnerre* tonnerre, uint8_t command, uint16_t vive_ID, uint8_t msg_id, uint8_t data[TONNERRE_MAX_ARG_SIZE], uint8_t data_length) {
    XBee_TX_frame TX_frame;
    uint16_t TX_frame_length;
    uint8_t payload[TONNERRE_MAX_ARG_SIZE + 2];
    
    payload[0] = command;
    payload[1] = msg_id;
    
    if(data_length != 0)
        memcpy(payload + 2, data, data_length);
    
    XBee_driver_create_TX_request(tonnerre->xbee_driver, &TX_frame, &TX_frame_length, vive_ID, payload, data_length + 2);
    XBee_driver_send(tonnerre->xbee_driver, &TX_frame, TX_frame_length);
}

/*
    ---------------
    Private methods
    ---------------
*/

/*
    ---------------------------------------------------------------------------
    Name : Dispatch
    Description : This is Tonnerre's dispatcher. This function tries to call
    the Tonnerre callback associated with the Tonnerre msg id provided. This
    function is itself a callback for the xBee_driver linked to TX_16_REQUEST.
    ---------------------------------------------------------------------------
*/

void Tonnerre_dispatch(uint8_t *xbee_frame_buffer) {
    XBee_RX_frame *rx_frame_received = (XBee_RX_frame *) xbee_frame_buffer;
    
    // Unpack xBee RX frame
    uint8_t *args = rx_frame_received->data + 2;
    uint16_t src_addr = SWAP_ENDIAN_16(rx_frame_received->src_addr);
    uint8_t msg_type = rx_frame_received->data[0];
    uint8_t msg_id = rx_frame_received->data[1];
    
    if(tonnerre->callbacks[msg_type].callback_function == NULL)
        return;
    
    tonnerre->callbacks[msg_type].callback_function(src_addr, msg_id, args);
}

void Tonnerre_register_callback(Tonnerre* tonnerre, uint8_t msg_type, void (*callback_function)(uint16_t src_addr, uint8_t msg_id, uint8_t args[TONNERRE_MAX_ARG_SIZE])) {
    tonnerre->callbacks[msg_type].callback_function = callback_function;
}

int Tonnerre_commands_management_system_get_next_empty_entry(Tonnerre *tonnerre) {
    for(int i = 0; i < TONNERRE_MAX_COMMANDS_MANAGEMENT_SYSTEM_ENTRIES; i++)
        if(tonnerre->commands_tabs[i].valid == false)
            return i;
    
    return -1; // if no empty entries are found
}

void Tonnerre_commands_management_system_remove_entry(Tonnerre *tonnerre, uint8_t command, uint16_t vive_ID, uint8_t msg_id) {
    for(int i = 0; i < TONNERRE_MAX_COMMANDS_MANAGEMENT_SYSTEM_ENTRIES; i++)
        if(tonnerre->commands_tabs[i].valid == true && \
            ((command == PING && tonnerre->commands_tabs[i].command == PING) || command == ACK) && // If it's an ACK, no command type check is done. Otherwise, the command type is checked.
            tonnerre->commands_tabs[i].vive_ID == vive_ID && \
            tonnerre->commands_tabs[i].msg_id == msg_id)
            tonnerre->commands_tabs[i].valid = false;
}

/* [] END OF FILE */
