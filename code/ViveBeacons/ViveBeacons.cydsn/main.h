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

#if !defined(MAIN_H)
#define MAIN_H

#include "project.h"
#include "VIVE_sensors.h"
#include "TS4231_driver.h"
#include "Position_finder.h"
#include "configuration.h"

// Protocols
#include "UART_commands_manager.h"
#include "USB_commands_manager.h"
#include "XBee_driver.h"
#include "XBee_driver_unit_tests.h"
#include "Arsenal.h"
#include "Arsenal_commands.h"
#include "Tonnerre.h"
#include "Tonnerre_commands.h"

/*
Position of LEDs in tracker's coordinates (aka relative to its center).
This is a list of couples of coordinates {(X0,Y0), (X1,Y1), ..., (X7,Y7)}
*/

extern double tracker_led_offset[8][2]; // 8 for 8 LEDs

// Handler "objects" pointers
extern UART_commands_manager* uart_commands_manager;
extern VIVE_sensors_data* vive_sensors_data;
extern VIVE_sensors* vive_sensors;
extern Position_finder* position_finder;
extern Arsenal* arsenal;
extern Tonnerre* tonnerre;

// Callback functions
/*void callback_are_you_ready();
void callback_send_position();
void callback_set_beacon_position();*/

// Global variables
extern bool beacon_position_initialized;
extern bool configured;
extern Position3D beacon_position;
extern uint8_t ID; // From 0 to 3. 0 is the coordinator

#endif

/* [] END OF FILE */
