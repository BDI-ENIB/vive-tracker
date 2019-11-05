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

#if !defined(XBEE_DRIVER_H)
#define XBEE_DRIVER_H

#include <cytypes.h>
#include "project.h"

// Utils
#define SWAP_ENDIAN_16(n) (n << 8) | (n >> 8)

// API frame type
#define TX_64_REQUEST 0x0
#define TX_16_REQUEST 0x1
#define AT_COMMAND_REQUEST 0x08
#define AT_COMMAND_QUEUE_REQUEST 0x09
#define REMOTE_AT_REQUEST 0x17
#define ZB_TX_REQUEST 0x10
#define ZB_EXPLICIT_TX_REQUEST 0x11
#define RX_64_RESPONSE 0x80
#define RX_16_RESPONSE 0x81
#define RX_64_IO_RESPONSE 0x82
#define RX_16_IO_RESPONSE 0x83
#define AT_RESPONSE 0x88
#define TX_STATUS_RESPONSE 0x89
#define MODEM_STATUS_RESPONSE 0x8a
#define ZB_RX_RESPONSE 0x90
#define ZB_EXPLICIT_RX_RESPONSE 0x91
#define ZB_TX_STATUS_RESPONSE 0x8b
#define ZB_IO_SAMPLE_RESPONSE 0x92
#define ZB_IO_NODE_IDENTIFIER_RESPONSE 0x95
#define AT_COMMAND_RESPONSE 0x88
#define REMOTE_AT_COMMAND_RESPONSE 0x97

// Status
#define	SUCCESS 0x0
#define CCA_FAILURE 0x2
#define INVALID_DESTINATION_ENDPOINT_SUCCESS 0x15
#define	NETWORK_ACK_FAILURE 0x21
#define NOT_JOINED_TO_NETWORK 0x22
#define	SELF_ADDRESSED 0x23
#define ADDRESS_NOT_FOUND 0x24
#define ROUTE_NOT_FOUND 0x25
#define PAYLOAD_TOO_LARGE 0x74

// Data structure
typedef struct xBee_driver_callback xBee_driver_callback;
struct xBee_driver_callback {
    uint8_t frame_type;
    void (*callback_function)(uint8_t *frame_buffer);
};

typedef struct XBee_driver XBee_driver;
struct XBee_driver
{
    xBee_driver_callback *callbacks;
    int callbacks_count;
    char delimiter[2];
};

// Frames

// Special type of frame
// Only used to send / receive other kind of frames
typedef struct XBee_generic_frame XBee_generic_frame;
struct XBee_generic_frame {
    uint8_t type;
    uint8_t *data;
} __attribute__((__packed__));


typedef struct XBee_TX_frame XBee_TX_frame;
struct XBee_TX_frame {
    uint8_t type;
    uint8_t id;
    uint16_t dest_addr;
    uint8_t options;
    uint8_t data[100];
} __attribute__((__packed__));

typedef struct XBee_RX_frame XBee_RX_frame;
struct XBee_RX_frame {
    uint8_t type;
    uint16_t dest_addr;
    uint8_t RSSI;
    uint8_t options;
    uint8_t data[100];
} __attribute__((__packed__));

// Public methods
XBee_driver* XBee_driver_create();
void XBee_driver_init(XBee_driver *xBee_driver);
void XBee_driver_register_callback(XBee_driver *xBee_driver, uint8_t frame_type, void (*callback_function)(uint8_t *frame_buffer));
void XBee_driver_create_TX_request(XBee_driver *xBee_driver, XBee_TX_frame *xBee_TX_frame, uint16_t *xBee_TX_frame_length, const uint16_t dest_address, const uint8_t payload[], const uint8_t payload_length);
void XBee_driver_send(XBee_driver *xBee_driver, const void *xBee_frame, const uint16_t frame_length);
void XBee_driver_check_commands(XBee_driver *xBee_driver);
uint8_t XBee_driver_compute_checksum(const uint8_t *frame, const uint16_t frame_length);

#endif

/* [] END OF FILE */
