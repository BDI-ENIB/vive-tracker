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
