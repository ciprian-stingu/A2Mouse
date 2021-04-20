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
 * File:   quadrature.c
 * Author: Ciprian Stingu
 *
 * Created on April 14, 2021, 11:52 PM
 */


#include "main.h"
#include "quadrature.h"
#include "ps2.h"
#include "leds.h"

void QuadratureTimer1Init(void)
{
    TMR1ON = 1;   // Enables Timer1
    TMR1CS = 0;   //Internal clock
    nT1SYNC = 1;  //Do not synchronize external clock input
    T1OSCEN = 0;  //Oscillator is shut off
    T1CKPS1 = 0;  //1:1 Prescale value
    T1CKPS0 = 0;  //1:1 Prescale value

    TMR1H = 0xF2; //default value
    TMR1L = 0x30; //63000
    
    TMR1IF = 0;   //clear int flag
    TMR1IE = 0;   //stop timer1
}

void QuadratureMouseInit(void)
{
    //set quadrature pins as output
    TRISA2 = 0;
    TRISA3 = 0;
    TRISA4 = 0;
    TRISB4 = 0;
    TRISB5 = 0;
    TRISB6 = 0;
    TRISB7 = 0;
    
    //set mouse port pins to high
    AXIS_X1 = PIN_HIGH;
    AXIS_X2 = PIN_HIGH;
    AXIS_Y1 = PIN_HIGH;
    AXIS_Y2 = PIN_HIGH;
    BUTTON_L = PIN_HIGH;
    BUTTON_R = PIN_HIGH;
    BUTTON_M = PIN_HIGH;

    QuadratureTimer1Init();
}

uint8_t QuadratureAdjustDistance(uint8_t distance)
{
#ifndef AMIGA_MOUSE
    distance /= 2;
#endif
    
    //no switch because of code size
    if(distance == 0 || distance == 1 || distance == 2) {
        distance = 1;
    }
    //distance 3 - not changed
    else if(distance == 4 || distance == 5 || distance == 6) {
        distance = 4;
    }
    else if(distance == 7 || distance == 8 || distance == 9) {
        distance = 5;
    }
    else if(distance > 9) {
        distance /= 2;
    }

    //limit distance because my mouse seems a little too spurious
    if(distance > 7) {
        distance = 7;
    }
    
    return distance;
}

void QuadratureMouseUpdate(void)
{
    //update buttons
    LeftMouseStatus = MouseDataFlags & 0x01 ? QUADRATURE_BUTTON_ON : QUADRATURE_BUTTON_OFF;
    MiddleMouseStatus = MouseDataFlags & 0x04 ? QUADRATURE_BUTTON_ON : QUADRATURE_BUTTON_OFF;
    RightMouseStatus = MouseDataFlags & 0x02 ? QUADRATURE_BUTTON_ON : QUADRATURE_BUTTON_OFF;
    
    //update x movement
    if(MouseDataXDelta > 0)
    {
        if(MouseDataFlags & 0x10)
        {
            uint8_t deltaX = QuadratureAdjustDistance(-(~MouseDataXDelta));
            if(XAxisDirection == QADRATURE_DIRECTION_LEFT) {
                XAxisDistance += deltaX;
            }
            else
            {
                XAxisDistance = 0;
                XAxisDirection = QADRATURE_DIRECTION_LEFT;
            }
        }
        else
        {
            uint8_t deltaX = QuadratureAdjustDistance(MouseDataXDelta);
            if(XAxisDirection == QADRATURE_DIRECTION_RIGHT) {
                XAxisDistance += deltaX;
            }
            else
            {
                XAxisDistance = 0;
                XAxisDirection = QADRATURE_DIRECTION_RIGHT;
            }
        }
    }
    if(XAxisDistance > QUADRATURE_MAX_MOVEMENT_LENGTH) {
        XAxisDistance = QUADRATURE_MAX_MOVEMENT_LENGTH;
    }

    //update y movement
    if(MouseDataYDelta > 0)
    {
        if(MouseDataFlags & 0x20)
        {
            uint8_t deltaY = QuadratureAdjustDistance(-(~MouseDataYDelta));
            if(YAxisDirection == QADRATURE_DIRECTION_LEFT) {
                YAxisDistance += deltaY;
            }
            else
            {
                YAxisDistance = 0;
                YAxisDirection = QADRATURE_DIRECTION_LEFT;
            }
        }
        else
        {
            uint8_t deltaY = QuadratureAdjustDistance(MouseDataYDelta);
            if(YAxisDirection == QADRATURE_DIRECTION_RIGHT) {
                YAxisDistance += deltaY;
            }
            else
            {
                YAxisDistance = 0;
                YAxisDirection = QADRATURE_DIRECTION_RIGHT;
            }
        }
    }
    if(YAxisDistance > QUADRATURE_MAX_MOVEMENT_LENGTH) {
        YAxisDistance = QUADRATURE_MAX_MOVEMENT_LENGTH;
    }
}

void QuadratureMouseOutput(void)
{
    //set Atari/Amiga mouse port pins
    
    LedsButtons(LeftMouseStatus == QUADRATURE_BUTTON_ON || MiddleMouseStatus == QUADRATURE_BUTTON_ON || RightMouseStatus == QUADRATURE_BUTTON_ON ? LED_STATUS_ON : LED_STATUS_OFF);
    BUTTON_L = LeftMouseStatus;
    BUTTON_M = MiddleMouseStatus;
    BUTTON_R = RightMouseStatus;

    LedsXAxis(XAxisDistance > 0 ? LED_STATUS_ON : LED_STATUS_OFF);
    if(XAxisDistance > 0)
    {
        if(XAxisDirection == QADRATURE_DIRECTION_LEFT) 
        {
            XAxisTransitionIndex--;
            if(XAxisTransitionIndex < 0) {
                XAxisTransitionIndex = 3;
            }
        }
        else
        {
            XAxisTransitionIndex++;
            if(XAxisTransitionIndex > 3) {
                XAxisTransitionIndex = 0;
            }
        }
        XAxisDistance--;

        //no switch or if/else because code size...
        if(XAxisTransitionIndex == 0) {
            AXIS_X1 = PIN_HIGH;
        }
        if(XAxisTransitionIndex == 1) {
            AXIS_X2 = PIN_HIGH;
        }
        if(XAxisTransitionIndex == 2) {
            AXIS_X1 = PIN_LOW;
        }
        if(XAxisTransitionIndex == 3) {
            AXIS_X2 = PIN_LOW;
        }
    }

    LedsYAxis(YAxisDistance > 0 ? LED_STATUS_ON : LED_STATUS_OFF);
    if(YAxisDistance > 0)
    {
        if(YAxisDirection == QADRATURE_DIRECTION_LEFT) 
        {
            YAxisTransitionIndex--;
            if(YAxisTransitionIndex < 0) {
                YAxisTransitionIndex = 3;
            }
        }
        else
        {
            YAxisTransitionIndex++;
            if(YAxisTransitionIndex > 3) {
                YAxisTransitionIndex = 0;
            }
        }
        YAxisDistance--;

        if(YAxisTransitionIndex == 0) {
            AXIS_Y1 = PIN_HIGH;
        }
        if(YAxisTransitionIndex == 1) {
            AXIS_Y2 = PIN_HIGH;
        }
        if(YAxisTransitionIndex == 2) {
            AXIS_Y1 = PIN_LOW;
        }
        if(YAxisTransitionIndex == 3) {
            AXIS_Y2 = PIN_LOW;
        }
    }
}