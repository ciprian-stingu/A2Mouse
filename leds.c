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
 * File:   leds.c
 * Author: Ciprian Stingu
 *
 * Created on April 15, 2021, 12:04 AM
 */


#include "main.h"
#include "leds.h"

void LedsInit(void)
{
    //set leds output
    TRISB0 = 0;
    TRISB3 = 0;
    TRISA6 = 0;
    TRISA7 = 0;
    
    LED_ERROR = LED_STATUS_OFF;
    LED_BUTTONS = LED_STATUS_OFF;
    LED_X_AXIS = LED_STATUS_OFF;
    LED_Y_AXIS = LED_STATUS_OFF;
}

void LedsError(uint8_t status)
{
    LED_ERROR = status;
}

void LedsButtons(uint8_t status)
{
    LED_BUTTONS = status;
}

void LedsXAxis(uint8_t status)
{
    LED_X_AXIS = status;
}

void LedsYAxis(uint8_t status)
{
    LED_Y_AXIS = status;
}
