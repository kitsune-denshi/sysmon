/********************************************************************
 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the "Company") for its PIC(R) Microcontroller is intended and
 supplied to you, the Company's customer, for use solely and
 exclusively on Microchip PIC Microcontroller products. The
 software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,
 WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *******************************************************************/

/** INCLUDES *******************************************************/
#include "system.h"

#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>

#include <stdlib.h>
#include <stdio.h>

#include "usb.h"

#include "app_device_cdc_basic.h"
#include "usb_config.h"

/** VARIABLES ******************************************************/

static bool buttonPressed;
static char buttonMessage[] = "Buffer overflow!\r\n";
static char echoMessage[] = "You said: ";
static uint8_t readBuffer[CDC_DATA_OUT_EP_SIZE];
static uint8_t writeBuffer[CDC_DATA_IN_EP_SIZE];

#define MSG_BUF_SIZE 8
static uint8_t msg_buffer[MSG_BUF_SIZE];


/*********************************************************************
* Function: void APP_DeviceCDCBasicDemoInitialize(void);
*
* Overview: Initializes the demo code
*
* PreCondition: None
*
* Input: None
*
* Output: None
*
********************************************************************/
void APP_DeviceCDCBasicDemoInitialize()
{
    CDCInitEP();

    
    line_coding.bCharFormat = 0;
    line_coding.bDataBits = 8;
    line_coding.bParityType = 0;
    line_coding.dwDTERate = 9600;

    buttonPressed = false;
}

/*********************************************************************
* Function: void APP_DeviceCDCBasicDemoTasks(void);
*
* Overview: Keeps the demo running.
*
* PreCondition: The demo should have been initialized and started via
*   the APP_DeviceCDCBasicDemoInitialize() and APP_DeviceCDCBasicDemoStart() demos
*   respectively.
*
* Input: None
*
* Output: None
*
********************************************************************/
void APP_DeviceCDCBasicDemoTasks()
{
    static uint8_t msg_buf_idx = 0;
    static bool msg_done = false;
    
    static char msg_type;
    
    
    /* Check to see if there is a transmission in progress, if there isn't, then
     * we can see about performing an echo response to data received.
     */
    if( USBUSARTIsTxTrfReady() == true)
    {
        uint8_t i;
        uint8_t numBytesRead;

        numBytesRead = getsUSBUSART(readBuffer, sizeof(readBuffer));

        /* For every byte that was read... */
        for(i=0; i<numBytesRead; i++)
        {
            if(msg_buf_idx == MSG_BUF_SIZE)
            {
                putrsUSBUSART(buttonMessage);
                msg_buf_idx = 0;
                msg_buffer[0] = 0;
                break;
            }
            else
            {
                if(readBuffer[i] == '\r' || readBuffer[i] == '\n')
                {
                    msg_buffer[msg_buf_idx] = 0;
                    msg_done = true;
                    break;
                }
                
                
                if(readBuffer[i] >= '0' && readBuffer[i] <= '9')
                {
                    msg_buffer[msg_buf_idx] = readBuffer[i];
                    msg_buf_idx++;
                }
                else
                {
                    msg_type = readBuffer[i];
                    msg_buf_idx = 0;
                    msg_buffer[msg_buf_idx] = 0;
                    
                }
                
                msg_done = false;
                

            }
        }

        if(numBytesRead > 0 && msg_done == true && msg_buf_idx > 0)
        {
            /* After processing all of the received data, we need to send out
             * the "echo" data now.
             */
            
            int number;
            
            number = atoi(msg_buffer);
            
            //sprintf(writeBuffer, "Type: %c, number: %d\r\n", msg_type, number);
            
            if(msg_type == 'c' && number > 0 && number < 128)
            {
                PWM1DCH = (uint8_t)number;
            }
            
            if(msg_type == 'm' && number > 0 && number < 128)
            {
                PWM2DCH = (uint8_t)number;
            }
            
            //putrsUSBUSART(writeBuffer);
            msg_buf_idx = 0;
            
        }
    }

    CDCTxService();
}