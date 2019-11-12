/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include <stdlib.h>

#define TONNERRE_HANDLER
#include "Tonnerre.h"
#undef TONNERRE_HANDLER

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
        tonnerre->callbacks->msg_type = i;
        tonnerre->callbacks->callback_function = NULL;
    }
    
    return tonnerre;
}

void Tonnerre_init(Tonnerre *tonnerre, XBee_driver *xbee_driver) {
    tonnerre->xbee_driver = xbee_driver;
    
    XBee_driver_register_callback(tonnerre->xbee_driver, TX_16_REQUEST, Tonnerre_dispatch);
}

/*
    ---------------------------------------------------------------------------
    Name : Dispatch
    Description : This is Tonnerre's dispatcher. This function tries to call the
    right Tonnerre callback associated with the Tonnerre msg id provided. This
    function is itself a callback for the xBee_driver linked to TX_16_REQUEST.
    ---------------------------------------------------------------------------
*/

void Tonnerre_dispatch(uint8_t *xbee_frame_buffer) {
    XBee_RX_frame *rx_frame_received = (XBee_RX_frame *) xbee_frame_buffer;
    
    // Unpack xBee RX frame
    uint8_t *data = rx_frame_received->data;
    uint8_t src_addr = (uint8_t) rx_frame_received->src_addr;
    uint8_t msg_type = data[0];
    
    if(tonnerre->callbacks[msg_type].callback_function == NULL)
        return;
    
    tonnerre->callbacks[msg_type].callback_function(src_addr, data);
}

/* [] END OF FILE */
