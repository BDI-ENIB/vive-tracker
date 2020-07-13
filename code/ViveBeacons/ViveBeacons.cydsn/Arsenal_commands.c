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
#include <stdio.h>
#include "main.h"
#include "configuration.h"
#include "position.h"
#include "Arsenal.h"
#include "Arsenal_commands.h"
#include "Tonnerre.h"
#include "Tonnerre_commands.h"

/*
    ---------------------------------------------------------------------------
    Implementation of Arsenal commands
    
    See the documentation in docs/reports
    ---------------------------------------------------------------------------
*/

/*
    -----------
    RX commands
    -----------
*/


void Arsenal_commands_set_beacon_position_callback() {
    beacon_position.x = atof(Arsenal_get_next_token(arsenal));
    beacon_position.y = atof(Arsenal_get_next_token(arsenal));
    beacon_position.z = atof(Arsenal_get_next_token(arsenal));

    beacon_position.alpha = atof(Arsenal_get_next_token(arsenal));
    beacon_position.beta = atof(Arsenal_get_next_token(arsenal));
    beacon_position.gamma = atof(Arsenal_get_next_token(arsenal));
    
    beacon_position_initialized = true;
    
    for(uint16_t i = 0; i < 4; i++) // For every xBee
        if(i != ID) // If it's not our ID ...
            Tonnerre_commands_send_sbpos(i, &beacon_position); // send beacon position
}

void Arsenal_commands_start_match_callback() {
    for(uint16_t i = 0; i < 4; i++) // For every xBee
        if(i != ID) // If it's not our ID ...
            Tonnerre_commands_send_start_match(i); // send start match
}

void Arsenal_commands_end_match_callback() {
    for(uint16_t i = 0; i < 4; i++) // For every xBee
        if(i != ID) // If it's not our ID ...
            Tonnerre_commands_send_end_match(i); // send end match
}

void Arsenal_commands_start_protocol_callback() {
    uint32_t protocol_ID = atoi(Arsenal_get_next_token(arsenal));
    
    for(uint16_t i = 0; i < 4; i++) // For every xBee
        if(i != ID) // If it's not our ID ...
            Tonnerre_commands_send_start_protocol(i, protocol_ID); // send start match
}

void Arsenal_commands_end_protocol_callback() {
    uint32_t protocol_ID = atoi(Arsenal_get_next_token(arsenal));
    
    for(uint16_t i = 0; i < 4; i++) // For every xBee
        if(i != ID) // If it's not our ID ...
            Tonnerre_commands_send_end_protocol(i, protocol_ID); // send end match
}

/*
    -----------
    TX commands 
    -----------
*/

void Arsenal_commands_send_pos(uint16_t ID, Position2D *pos) {
    Position_serialized pos_s;
    pos_s.i = POS_SERIALIZE_DEFAULT(pos);
    
    char buffer[32] = {0};
    //sprintf(buffer, "0x%08X", pos_s.i);
    sprintf(buffer, "%f %f %f", pos->x, pos->y, pos->a);
    
    Arsenal_send_command(arsenal, "POS", ID, buffer);
}

void Arsenal_commands_send_start_match(uint16_t ID) {
    Arsenal_send_command(arsenal, "STMATCH", ID, "");
}

void Arsenal_commands_send_end_match(uint16_t ID) {
    Arsenal_send_command(arsenal, "ENDMATCH", ID, "");
}

void Arsenal_commands_send_start_protocol(uint16_t ID, uint32_t protocol_ID) {
    char buffer[32] = {0};
    sprintf(buffer, "%d", protocol_ID);
    
    Arsenal_send_command(arsenal, "STPROTOCOL", ID, buffer);
}

void Arsenal_commands_send_end_protocol(uint16_t ID, uint32_t protocol_ID) {
    char buffer[32] = {0};
    sprintf(buffer, "%d", protocol_ID);
    
    Arsenal_send_command(arsenal, "ENDPROTOCOL", ID, buffer);
}

/* [] END OF FILE */
