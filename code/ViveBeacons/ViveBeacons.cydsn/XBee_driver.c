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
#include "XBee_driver.h"

/*
    ---------------------------------------------------------------------------
    A PSoC Library to communicate with xBee S1
    
    Link to the documentation :
    https://www.digi.com/resources/documentation/digidocs/pdfs/90000982.pdf
    
    NOTE : This library can be used with xBee S3, as it supports IEEE 802.15.4
    ---------------------------------------------------------------------------
*/

/*
    --------------
    Public methods
    --------------
*/

XBee_driver* XBee_driver_create() {
    XBee_driver* xBee_driver = (XBee_driver*) malloc(1*sizeof(XBee_driver));

    xBee_driver->callbacks = NULL;
    xBee_driver->callbacks_count = 0;
    
    return xBee_driver;
}

void XBee_driver_init(XBee_driver *xBee_driver) {
    (void)xBee_driver; // suppress "unused" warning
    UART_XBEE_Start();
}

/*
    ---------------------------------------------------------------------------
    Name : Register callback
    Description : Associate a callback with a specific xBee API frame type.
    ---------------------------------------------------------------------------
*/

void XBee_driver_register_callback(XBee_driver *xBee_driver, uint8_t frame_type, void (*callback_function)(uint8_t *frame_buffer)) {
    xBee_driver->callbacks = (xBee_driver_callback *) realloc(xBee_driver->callbacks, (xBee_driver->callbacks_count + 1)*sizeof(xBee_driver_callback));
    
    (xBee_driver->callbacks[xBee_driver->callbacks_count]).frame_type = frame_type;
    (xBee_driver->callbacks[xBee_driver->callbacks_count]).callback_function = callback_function;
    xBee_driver->callbacks_count++;
}

void XBee_driver_create_TX_request(XBee_driver *xBee_driver, XBee_TX_frame *xBee_TX_frame, uint16_t *xBee_TX_frame_length, const uint16_t dest_address, const uint8_t payload[], const uint8_t payload_length) {
    (void) xBee_driver; // suppress warning
    
    *xBee_TX_frame_length = 1 + 1 + 2 + 1 + payload_length;
    uint8_t frame_id = 0x01; // for future use
    uint8_t frame_options = 0x0; // for future use
    
    // Forge the frame
    xBee_TX_frame->type = TX_16_REQUEST;
    xBee_TX_frame->id = frame_id;
    xBee_TX_frame->dest_addr = SWAP_ENDIAN_16(dest_address);
    xBee_TX_frame->options = frame_options;
    memcpy(xBee_TX_frame->data, payload, payload_length);
}

/*
    ---------------------------------------------------------------------------
    Name : Send
    Description : Send any frame to the local xBee.
    Parameters :
    
    WARNING : This is a blocking function.
    ---------------------------------------------------------------------------
*/

void XBee_driver_send(XBee_driver *xBee_driver, const void *xBee_frame, const uint16_t frame_length) {
    (void) xBee_driver; // suppress warning
    
    uint16_t length = 1 + 2 + frame_length + 1;
    const uint8_t *frame = (const uint8_t *) xBee_frame;
    
    // Create buffer
    uint8_t *buffer = (uint8_t *) malloc(length*sizeof(uint8_t));
    
    // Fill in the buffer
    buffer[0] = 0x7E;
    buffer[1] = (frame_length >> 8);
    buffer[2] = (frame_length & 0xFF);
    
    memcpy(buffer + 3, frame, frame_length);
    
    // Checkum forever !
    buffer[length - 1] = XBee_driver_compute_checksum(frame, frame_length);

    UART_XBEE_PutArray(buffer, length);
    
    free(buffer);
}

/*
    ---------------------------------------------------------------------------
    Name : Check commands
    Description : Process the reception of xBee API frame and call the
    registered callback associated to the frame type received.
    WARNING : This is a blocking function.
    ---------------------------------------------------------------------------
*/

// TODO : Convert to non-blocking, using state machine method ?
void XBee_driver_check_commands(XBee_driver *xBee_driver) {
    uint16_t character = (uint8_t) UART_XBEE_GetByte();
    
    if(character == 0x7E) { // Start of frame
        while(UART_XBEE_GetRxBufferSize() < 2); // Wait until length can be red
        
        // Get length
        uint8_t length_buffer[2];
        length_buffer[0] = UART_XBEE_GetByte();
        length_buffer[1] = UART_XBEE_GetByte();
        uint16 frame_length = (length_buffer[0] << 8) | length_buffer[1];
        
        // Get API frame itself
        uint8_t *frame_buffer = (uint8_t *) malloc(frame_length*sizeof(uint8_t));
        while(UART_XBEE_GetRxBufferSize() < frame_length);
        for(int i = 0; i < frame_length; i++)
            frame_buffer[i] = UART_XBEE_GetByte();
        
        // Get checksum and do the check
        while(UART_XBEE_GetRxBufferSize() < 1); // Wait until checksum can be red
        uint8_t checksum = UART_XBEE_GetByte();
        
        if(XBee_driver_compute_checksum(frame_buffer, frame_length) == checksum) {
            uint8_t frame_type = frame_buffer[0];
            
            // Call the correct callback
            for(int i = 0; i < xBee_driver->callbacks_count; i++) {
                if((xBee_driver->callbacks[i]).frame_type == frame_type) {
                    (*((xBee_driver->callbacks[i]).callback_function))(frame_buffer);
                    break;
                }
            }
        }
        free(frame_buffer);
    }
}

/*
    ---------------------------------------------------------------------------
    Name : Compute checksum
    Description : Compute the checksum for API frames, as indicated in the xBee
    documentation.
    ---------------------------------------------------------------------------
*/

uint8_t XBee_driver_compute_checksum(const uint8_t *frame, const uint16_t frame_length) {
    uint8_t acc = 0x0;
    
    for(int i = 0; i < frame_length; i++) {
        acc = (uint8_t) (acc + frame[i]);
    }
    
    return 0xFF - acc;
}

/* [] END OF FILE */
