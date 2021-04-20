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
 * File:   main.c
 * Author: Ciprian Stingu
 *
 * Created on April 14, 2021, 7:26 PM
 */


#include "main.h"
#include "ps2.h"
#include "quadrature.h"
#include "leds.h"

//debug on serial port
#ifdef ENABLE_DEBUG
    #include "uart.h"
#endif

void main(void) 
{
    // Enable all unmasked global INT
    GIE = 1;
    // Enable Peripheral interrupt
    PEIE = 1;
    // disable comparators
    CM0 = 1;
    CM1 = 1;
    CM2 = 1;

    QuadratureMouseInit();
    LedsInit();
    Ps2Init();
#ifdef ENABLE_DEBUG
    UartInit();
#endif
    
    //enable leds
    __delay_ms(100);
    LedsError(LED_STATUS_ON);
    __delay_ms(250);
    LedsButtons(LED_STATUS_ON);
    __delay_ms(250);
    LedsXAxis(LED_STATUS_ON);
    __delay_ms(250);
    LedsYAxis(LED_STATUS_ON);
    __delay_ms(250);
    //disable leds
    LedsError(LED_STATUS_OFF);
    LedsButtons(LED_STATUS_OFF);
    LedsXAxis(LED_STATUS_OFF);
    LedsYAxis(LED_STATUS_OFF);
    
#ifdef ENABLE_DEBUG    
    UartWriteString("A2M Inited.\n");
#endif

    //reset the device
#ifdef ENABLE_DEBUG
    UartWriteString("PS2 reseting mouse...\n");
#endif
    while(!Ps2Reset()) 
    {
        LedsError(LED_STATUS_ON);
        __delay_ms(1000);
    }
#ifdef ENABLE_DEBUG
    UartWriteString("done\n");
#endif
    
#ifdef ENABLE_DEBUG
    UartWriteString("PS2 setting sample rate...\n");
#endif
    while(!Ps2SetSampleRate(80)) 
    {
        LedsError(LED_STATUS_ON);
        LedsButtons(LED_STATUS_ON);
        __delay_ms(1000);
    }
#ifdef ENABLE_DEBUG
    UartWriteString("done\n");
#endif
    
#ifdef ENABLE_DEBUG
    UartWriteString("PS2 setting resolution...\n");
#endif
    while(!Ps2SetResolution(1))
    {
        LedsError(LED_STATUS_ON);
        LedsXAxis(LED_STATUS_ON);
        __delay_ms(1000);
    }    
#ifdef ENABLE_DEBUG
    UartWriteString("done\n");
#endif
    
    //set remote mode
#ifdef ENABLE_DEBUG
    UartWriteString("PS2 setting remote mode...\n");
#endif
    while(!Ps2SetRemoteMode())
    {
        LedsError(LED_STATUS_ON);
        LedsYAxis(LED_STATUS_ON);
        __delay_ms(1000);
    }
#ifdef ENABLE_DEBUG
    UartWriteString("done\n");
#endif
    
#ifdef ENABLE_DEBUG
    char convBuffer[3];
#endif
    
    //disable leds if any
    LedsError(LED_STATUS_OFF);
    LedsButtons(LED_STATUS_OFF);
    LedsXAxis(LED_STATUS_OFF);
    LedsYAxis(LED_STATUS_OFF);
    
    TMR1IE = 1; // start timer1
    
    uint8_t ps2UpdateCounter = 0;
    while(1)
    {        
        if(QuadratureShouldSendUpdate) 
        {
            QuadratureMouseOutput();
            QuadratureShouldSendUpdate = 0;
            ps2UpdateCounter++;
        }
        
        //read ps2 data at ~80 samples per second
        if(ps2UpdateCounter > 4) 
        {
            if(Ps2UpdateMouseData()) {
                QuadratureMouseUpdate();
            }
            ps2UpdateCounter = 0;
            
#ifdef ENABLE_DEBUG
            UartWriteString("LB -> ");
            UartWriteString(LeftMouseStatus == QUADRATURE_BUTTON_ON ? "ON" : "OFF");
            UartWriteString(", MB -> ");
            UartWriteString(MiddleMouseStatus == QUADRATURE_BUTTON_ON ? "ON" : "OFF");
            UartWriteString(", RB -> ");
            UartWriteString(RightMouseStatus == QUADRATURE_BUTTON_ON ? "ON" : "OFF");
            UartWriteString(", XAxisDistance -> ");
            ByteToAsciiHex(XAxisDistance, convBuffer);
            UartWriteString(convBuffer);
            UartWriteString(", YAxisDistance -> ");
            ByteToAsciiHex(YAxisDistance, convBuffer);
            UartWriteString(convBuffer);
            UartWriteString("\n");
#endif
        }
        
    }
}

void __interrupt() ISR(void)
{
#ifdef ENABLE_DEBUG
    //uart data
    if(RCIF)
    {
        if(OERR) // check for overrun error and reset
        {
            CREN = 0; 
            CREN = 1;
        }
        
        RCIF = 0;
    }
#endif
    
    if(TMR1IF)
    {
        TMR1IE = 0; // stop timer1
        
        QuadratureShouldSendUpdate = 1;
#ifdef ENABLE_DEBUG
        TMR1H = 0; //default value
        TMR1L = 0; //0
#else
        TMR1H = 0xF3; //set 62235 so it will remain 3300 instructions
        TMR1L = 0x1B; //which is ~300 updates per second
#endif
        TMR1IF = 0;
        TMR1IE = 1; // enable timer1
    }
}

#ifdef ENABLE_DEBUG
void ByteToAsciiHex(uint8_t value, char *buffer)
{
    uint8_t lsb = value & 0x0F;
    uint8_t msb = (value >> 4) & 0x0F;
    
    *buffer++ = msb <= 9 ? msb + '0' : (msb - 10) + 'A';
    *buffer++ = lsb <= 9 ? lsb + '0' : (lsb - 10) + 'A';
    *buffer = NULL_CHAR;
}
#endif
