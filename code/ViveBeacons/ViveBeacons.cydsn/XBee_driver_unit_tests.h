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

#if !defined(XBEE_DRIVER_UNIT_TESTS_H)
#define XBEE_DRIVER_UNIT_TESTS_H

#include <cytypes.h>
#include "project.h"

#include "XBee_driver.h"
    
extern XBee_driver* xBee_driver;

// TX unit test
void xBee_driver_unit_test_TX();

// RX unit test
void xBee_driver_unit_test_RX_callback(uint8_t *frame_buffer);
void xBee_driver_unit_test_RX();

#endif

/* [] END OF FILE */
