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
#include "project.h"
#include "USB_commands_manager.h"

/*
    ---------------------------------------------------------------------------
    This file should be a PSoC version of an Arduino library. It would have
    been trans-coded from https://github.com/kroimon/Arduino-SerialCommand
    
    Unfortunately, because of 2018's project evolution (moving from a main
    robot with a lattepanda to the summberbot using a teensy), this library is
    not finished. The idea is to copy the already working UART_command_manager
    here and replaced UART by USB everywhere in the firmware.
    ---------------------------------------------------------------------------
*/

/*
    --------------
    Public methods
    --------------
*/

USB_commands_manager* USB_commands_manager_create() {
    USB_commands_manager* usb_commands_manager = (USB_commands_manager*) malloc(1*sizeof(USB_commands_manager));
    
    return usb_commands_manager;
}

void USB_commands_manager_init(USB_commands_manager *usb_commands_manager) {
    USB_Serial_Start(0, USB_Serial_3V_OPERATION);
    while(!USB_Serial_GetConfiguration());
    USB_Serial_CDC_Init();
    
    USB_Serial_PutString("START\n");
    while(USB_Serial_CDCIsReady() == 0u);
    CyDelay(1);
    
    CyDelay(3000);
    
    USB_Serial_PutString("INIT\n");
    while(USB_Serial_CDCIsReady() == 0u);
    CyDelay(1);
}


void USB_commands_manager_send_command(USB_commands_manager *usb_commands_manager, char command[USB_COMMAND_MANAGER_MAX_COMMAND_LENGTH + 1]) {

    
}

/* [] END OF FILE */
