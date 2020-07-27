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

/*
    Units used in all the firmware :
    - distances, coordinates : mm or m
    - angles : rad
    - time : s
    - velocity : m/s
    - acceleration : m/(s^2)
    - temperature : °C
*/

#include <stdlib.h>
#include <stdio.h>
#include "position.h"
#include "main.h"

// Check vive_sensors.h to select the type of sensors that you have

// Global variables
bool beacon_position_initialized = false;
bool configured = false;
Position3D beacon_position;
uint8_t ID; // From 0 to 3. 0 is the coordinator

/*
Position of LEDs in tracker's coordinates (aka relative to its center).
This is a list of couples of coordinates {(X0,Y0), (X1,Y1), ..., (X7,Y7)}
*/

double tracker_led_offset[8][2] = { // 8 for 8 LEDs
    {40, 0},
    {40*SQRT_2, 40*SQRT_2},
    {0, 40},
    {-40*SQRT_2, 40*SQRT_2},
    {-40, 0},
    {-40*SQRT_2, -40*SQRT_2},
    {0, -40},
    {40*SQRT_2, -40*SQRT_2},
};

// Handler "objects" pointers (We are in C ...)
VIVE_sensors_data* vive_sensors_data = NULL;
VIVE_sensors* vive_sensors = NULL;
Position_finder* position_finder = NULL;
XBee_driver* xBee_driver = NULL;
Arsenal* arsenal = NULL;
Tonnerre* tonnerre = NULL;
Neopixel_driver *neopixel_driver = NULL;

int main(void) {
    // Creating handler objects
    vive_sensors = VIVE_sensors_create();
    position_finder = Position_finder_create();
    xBee_driver = XBee_driver_create();
    arsenal = Arsenal_create();
    tonnerre = Tonnerre_create();
    neopixel_driver = Neopixel_driver_create();
    
    /* --- Tracker initialization --- */
    /* ID decoding */
    ISCONNECTED(ID_1) {
        ISCONNECTED(ID_0)
            ID = 3;
        ISDISCONNECTED(ID_0)
            ID = 2;
    }
    ISDISCONNECTED(ID_1) {
        ISCONNECTED(ID_0)
            ID = 1;
        ISDISCONNECTED(ID_0)
            ID = 0;
    }
    
    CyGlobalIntEnable;
    Arsenal_init(arsenal);
    Tonnerre_init(tonnerre, xBee_driver);
    CyGlobalIntDisable;
    
    /* VIVE sensors init : VIVE decoders and TS4231 init (It has to wait for light though).*/
    millis_timer_Start(); //for the timeout during init phase
    VIVE_sensors_init(vive_sensors);
    CyGlobalIntEnable;

    /* --- Main loop --- */
    for(;;) {
        Arsenal_check_commands(arsenal);
        Tonnerre_check_commands(tonnerre);
        
        if(beacon_position_initialized) {
            // Position finder init
            Position_finder_init(position_finder, &beacon_position, LED_COORD_HEIGHT, tracker_led_offset);
            configured = true; // Yay, it's configured !
            /* --- Tracker initialized --- */
        }
        
        if(configured) {
            if(VIVE_pulses_decoded) { // If configured and new pulse received, go process it !
                VIVE_pulses_decoded = 0; // Disarm the flag
                vive_sensors_data = VIVE_sensors_process_pulses(vive_sensors); // Get the angles
                Position_finder_find_position(position_finder, vive_sensors_data); // Compute the position
                
                for(uint16_t i = 0; i < 2; i++) // For every friend xBee (not opponent)
                    if(i != ID) // If it's not our ID ...
                        Tonnerre_commands_send_pos(i, position_finder->current_position); // send pos to everyone
            }
        }
    }
}

/* [] END OF FILE */
