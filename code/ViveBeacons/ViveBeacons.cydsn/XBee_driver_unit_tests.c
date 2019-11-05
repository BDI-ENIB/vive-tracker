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

#include "XBee_driver_unit_tests.h"

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