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

#if !defined(MAIN_H)
#define MAIN_H

#include "project.h"
#include "VIVE_sensors.h"
#include "TS4231_driver.h"
#include "Position_finder.h"
#include "configuration.h"
#include "UART_commands_manager.h"
#include "USB_commands_manager.h"
#include "XBee_driver.h"

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

// Handler "objects" pointers
extern UART_commands_manager* uart_commands_manager;
extern VIVE_sensors_data* vive_sensors_data;
extern VIVE_sensors* vive_sensors;
extern Position_finder* position_finder;

// Callback functions
void callback_are_you_ready();
void callback_send_position();
void callback_set_beacon_position();

// Global variables
bool beacon_position_initialized = false;
bool configured = false;
Position3D beacon_position;

#endif

/* [] END OF FILE */
