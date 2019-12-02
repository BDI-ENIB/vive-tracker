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

#include "XBee_driver_unit_tests.h"

/*
    ---------------------------------------------------------------------------
    Test name : TX unit test
    Description : This module should send 6 bytes every 50ms to xBee number 0.
    Test procedure : Monitor data received on xBee number 0.
    Results expected : Get the (same) 6 bytes data on the xBee number 0.
    Probable bad results : 
    - "0x11" byte may not be transmitted correctly. 
      -> Check escaping bytes aspects.
    - Packet losses
      -> Check xBee configurations, UART connections and re-try, if possible,
    in a less RF-noisy environment.
    ---------------------------------------------------------------------------
*/

void xBee_driver_unit_test_TX() {
    uint8_t data[6] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55};
    XBee_TX_frame TX_frame;
    uint16_t TX_frame_length;
    
    XBee_driver_init(xBee_driver);
    XBee_driver_create_TX_request(xBee_driver, &TX_frame, &TX_frame_length, 0x00, data, 6);
    
    while(1)
    {
        XBee_driver_send(xBee_driver, &TX_frame, TX_frame_length);
        CyDelay(50);
    };
}

/*
    ---------------------------------------------------------------------------
    Test name : RX unit test
    Description : This module should send the data received, in a 100 bytes
    packet to the xBee device number 0.
    Test procedure : Send a random TX packet to this module.
    Results expected : Get the same data sent in the random TX packet at the
    beginning, on the xBee device number 0.
    Known issues : If spammed, some packets can be lost. Put, at least, 15 ms
    between each packet (depend on the baudrate defined on the UART link).
    ---------------------------------------------------------------------------
*/

void xBee_driver_unit_test_RX_callback(uint8_t *frame_buffer) {
    XBee_RX_frame *rx_frame_received = (XBee_RX_frame *) frame_buffer;
    
    XBee_TX_frame tx_frame;
    uint16_t tx_frame_length;

    XBee_driver_create_TX_request(xBee_driver, &tx_frame, &tx_frame_length, 0, rx_frame_received->data, 100);
    XBee_driver_send(xBee_driver, &tx_frame, tx_frame_length);
}

void xBee_driver_unit_test_RX() {            
    XBee_driver_init(xBee_driver);
    XBee_driver_register_callback(xBee_driver, RX_16_RESPONSE, xBee_driver_unit_test_RX_callback);
    
    while(1)
    {
        XBee_driver_check_commands(xBee_driver);
    };
}

/* [] END OF FILE */
