/** 
 * 
 * A2Mouse
 * 
 * Copyright (C) 2021 Ciprian Stingu
 * ciprian dot stingu at gmail dot com
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111 USA
 * or see <http://www.gnu.org/licenses/>
 *
 * File:   ps2.c
 * Author: Ciprian Stingu
 *
 * Created on April 14, 2021, 11:51 PM
 */


#include "main.h"
#include "ps2.h"
//debug on serial port
#ifdef ENABLE_DEBUG
    #include "uart.h"
    char messageBuffer[9];
#endif

void Ps2Init(void)
{
    PS2_DATA_PIN = PIN_HIGH;
    PS2_DATA_IO = IO_IN;
    PS2_CLOCK_PIN = PIN_HIGH;
    PS2_CLOCK_IO = IO_IN;
}

void Ps2DisableCommunication(void)
{
    PS2_CLOCK_IO = IO_OUT;
    PS2_CLOCK_PIN = PIN_LOW;
}

uint8_t Ps2Reset(void)
{
    Ps2Write(PS2_OPERATION_RESET);
    
    uint16_t acknowledge = Ps2Read();
    uint16_t bat = Ps2Read();
    uint16_t id = Ps2Read();
  
    Ps2DisableCommunication();
    
#ifdef ENABLE_DEBUG    
    ByteToAsciiHex(acknowledge, messageBuffer);
    ByteToAsciiHex(bat, &messageBuffer[2]);
    ByteToAsciiHex(id, &messageBuffer[4]);
    
    UartWriteString(messageBuffer);
    UartWriteString("\n");
#endif
    
    return acknowledge == PS2_RESPONSE_ACKNOWLEDGE && bat == PS2_RESPONSE_BAT_SUCCESSFUL && id == 0;
}

uint8_t Ps2SetRemoteMode(void)
{
    Ps2Write(PS2_OPERATION_SET_REMOTE_MODE);
    
    uint16_t acknowledge = Ps2Read();
    
    Ps2DisableCommunication();
    
#ifdef ENABLE_DEBUG    
    ByteToAsciiHex(acknowledge, messageBuffer);
    UartWriteString(messageBuffer);
    UartWriteString("\n");
#endif

    return acknowledge == PS2_RESPONSE_ACKNOWLEDGE;
}

uint8_t Ps2SetSampleRate(uint8_t sampleRate)
{
    Ps2Write(PS2_OPERATION_SET_SAMPLE_RATE);
    uint16_t acknowledge1 = Ps2Read();
    
    Ps2Write(sampleRate);
    uint16_t acknowledge2 = Ps2Read();
    
    Ps2DisableCommunication();
    
#ifdef ENABLE_DEBUG
    ByteToAsciiHex(acknowledge1, messageBuffer);
    ByteToAsciiHex(acknowledge2, &messageBuffer[2]);
    UartWriteString(messageBuffer);
    UartWriteString("\n");
#endif
    
    return acknowledge1 == PS2_RESPONSE_ACKNOWLEDGE && acknowledge2 == PS2_RESPONSE_ACKNOWLEDGE;
}

uint8_t Ps2SetResolution(uint8_t resolution)
{
    Ps2Write(PS2_OPERATION_SET_RESOLUTION);
    uint16_t acknowledge1 = Ps2Read();

    Ps2Write(resolution);
    uint16_t acknowledge2 = Ps2Read();
    
    Ps2DisableCommunication();
    
#ifdef ENABLE_DEBUG
    ByteToAsciiHex(acknowledge1, messageBuffer);
    ByteToAsciiHex(acknowledge2, &messageBuffer[2]);
    UartWriteString(messageBuffer);
    UartWriteString("\n");
#endif
    
    return acknowledge1 == PS2_RESPONSE_ACKNOWLEDGE && acknowledge2 == PS2_RESPONSE_ACKNOWLEDGE;
}

uint8_t Ps2UpdateMouseData(void)
{
    Ps2Write(PS2_OPERATION_READ_DATA);
    
    uint16_t acknowledge = Ps2Read();
    uint16_t flags = Ps2Read();
    uint16_t xMovement = Ps2Read();
    uint16_t yMovement = Ps2Read();
    
    Ps2DisableCommunication();
    
    if(acknowledge != PS2_RESPONSE_ACKNOWLEDGE || flags > 0xFF || xMovement > 0xFF || yMovement > 0xFF) {
        return 0;
    }
    
    MouseDataFlags = flags & 0xFF;
    MouseDataXDelta = xMovement & 0xFF;
    MouseDataYDelta = yMovement & 0xFF;
    
#ifdef ENABLE_DEBUG
    ByteToAsciiHex(acknowledge, messageBuffer);
    ByteToAsciiHex(flags, &messageBuffer[2]);
    ByteToAsciiHex(xMovement, &messageBuffer[4]);
    ByteToAsciiHex(yMovement, &messageBuffer[6]);
    
    UartWriteString("PS2 mouse data: ");
    UartWriteString(messageBuffer);
    UartWriteString("\n");
#endif
    
    return 1;
}

void Ps2Write(uint8_t data)
{
    //set idle
    PS2_DATA_PIN = PIN_HIGH;
    PS2_DATA_IO = IO_IN;
    PS2_CLOCK_PIN = PIN_HIGH;
    PS2_CLOCK_IO = IO_IN;
    __delay_us(150);
    //set inhibit
    PS2_CLOCK_IO = IO_OUT;
    PS2_CLOCK_PIN = PIN_LOW;
    __delay_us(150);
    //request for communication
    PS2_DATA_IO = IO_OUT;
    PS2_DATA_PIN = PIN_LOW;        
    __delay_us(10);
    //release clock
    PS2_CLOCK_PIN = PIN_HIGH;
    PS2_CLOCK_IO = IO_IN;
    //wait for clock to be low
    while(PS2_CLOCK_PIN) {
        asm("nop");
    }
    //send data
    uint8_t parity = 1;
    for(uint8_t i = 0; i < 8; i++) 
    {
        //send bit
        PS2_DATA_PIN = data & 0x01 ? PIN_HIGH : PIN_LOW;
        //wait clock high
        while(!PS2_CLOCK_PIN) {
            asm("nop");
        }
        //compute parity
        parity = parity ^ (data & 0x01);
        //prepare next data bit
        data = data >> 1;
        //wait clock low
        while(PS2_CLOCK_PIN) {
            asm("nop");
        }
    }
    PS2_DATA_PIN = parity ? PIN_HIGH : PIN_LOW;
    //wait clock high
    while(!PS2_CLOCK_PIN) {
        asm("nop");
    }
    //wait clock low
    while(PS2_CLOCK_PIN) {
        asm("nop");
    }
    //release data
    PS2_DATA_PIN = PIN_HIGH;
    PS2_DATA_IO = IO_IN;
    //wait for data low - acknowledge
    while(PS2_DATA_PIN) {
        asm("nop");
    }
    //wait for clock low
    while(PS2_CLOCK_PIN) {
        asm("nop");
    }
    //wait for data & clock high
    while(!PS2_CLOCK_PIN || !PS2_DATA_PIN) {
        asm("nop");
    }
    //inhibit
    PS2_DATA_PIN = PIN_HIGH;
    PS2_DATA_IO = IO_IN;
    PS2_CLOCK_IO = IO_OUT;
    PS2_CLOCK_PIN = PIN_LOW;
}

uint16_t Ps2Read(void)
{
    //set idle
    PS2_DATA_PIN = PIN_HIGH;
    PS2_DATA_IO = IO_IN;
    PS2_CLOCK_PIN = PIN_HIGH;
    PS2_CLOCK_IO = IO_IN;
    __delay_us(50);
    //wait for clock low
    uint16_t maxWait = 0;
    while(PS2_CLOCK_PIN && ++maxWait < 0xFFFE) {
        asm("nop");
    }
    if(PS2_DATA_PIN != PIN_LOW) {
        return PS2_READ_ERROR_1;
    }
    //wait for clock high
    while(!PS2_CLOCK_PIN) {
        asm("nop");
    }
    //read data
    uint8_t data = 0, parity = 1;
    for(uint8_t i = 0; i < 8; i++)
    {
        //wait for clock low
        while(PS2_CLOCK_PIN) {
            asm("nop");
        }
        parity = parity ^ PS2_DATA_PIN;
        data |= (PS2_DATA_PIN << i);
        //wait for clock high
        while(!PS2_CLOCK_PIN) {
            asm("nop");
        }
    }
    //read parity bit
    //wait for clock low
    while(PS2_CLOCK_PIN) {
        asm("nop");
    }
    if(parity != PS2_DATA_PIN)
    {
        return PS2_READ_ERROR_2;
    }
    //wait for clock high
    while(!PS2_CLOCK_PIN) {
        asm("nop");
    }
    //read stop bit
    while(PS2_CLOCK_PIN) {
        asm("nop");
    }
    if(PS2_DATA_PIN != PIN_HIGH)
    {
        return PS2_READ_ERROR_3;
    }
    //wait for clock high
    while(!PS2_CLOCK_PIN) {
        asm("nop");
    }

    return data;
}
