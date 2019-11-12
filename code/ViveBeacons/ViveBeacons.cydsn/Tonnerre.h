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

#if !defined(TONNERRE_H)
#define TONNERRE_H

#include <cytypes.h>
#include "project.h"
#include "XBee_driver.h"

#define TONNERRE_MAX_ARG_SIZE 24 // Max argument size for Tonnerre messages
#define TONNERRE_MAX_MSG_ID 69

// Tonnerre message IDs
// -- Mandatory commands --
#define ACK 0
#define POS 1
#define SBPOS 2
#define PING 3
#define PONG 4
#define SBBOX 5
// -- AI-related "protocol" commands --
#define SP 10 // Start
#define EP 11 // End
// -- AI-related match commands --
#define SM 20 // Start
#define EM 21 // End
// -- Various messages types --
#define MSG 30 // Generic message
// -- Qute commands --
#define QT 69 // ;-) See Qute protocol for more details   

// Data structure
typedef struct Tonnerre_callback Tonnerre_callback;
struct Tonnerre_callback {
    uint8_t msg_type;
    void (*callback_function)(uint8_t src_addr, uint8_t buffer[TONNERRE_MAX_ARG_SIZE]);
};

typedef struct Tonnerre Tonnerre;
struct Tonnerre
{
    XBee_driver *xbee_driver;
    Tonnerre_callback callbacks[TONNERRE_MAX_MSG_ID+1];
    uint8_t msg_id; // Incremented for each message sent
};

// Public methods
Tonnerre* Tonnerre_create();
void Tonnerre_init(Tonnerre *tonnerre, XBee_driver *xbee_driver);

// Private methods
void Tonnerre_dispatch(uint8_t *xbee_frame_buffer);

#if defined(TONNERRE_HANDLER)
Tonnerre* tonnerre = NULL;
#else
extern Tonnerre* tonnerre;
#endif

#endif

/* [] END OF FILE */
