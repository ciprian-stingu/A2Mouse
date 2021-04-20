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
 * File:   leds.h
 * Author: Ciprian Stingu
 *
 * Created on April 15, 2021, 12:03 AM
 */

#ifndef LEDS_H
#define	LEDS_H

#ifdef	__cplusplus
extern "C" {
#endif

#define LED_ERROR   RB0
#define LED_BUTTONS RB3
#define LED_X_AXIS  RA7
#define LED_Y_AXIS  RA6

#define LED_STATUS_ON  PIN_LOW
#define LED_STATUS_OFF PIN_HIGH
    
void LedsInit(void);
void LedsError(uint8_t status);
void LedsButtons(uint8_t status);
void LedsXAxis(uint8_t status);
void LedsYAxis(uint8_t status);

#ifdef	__cplusplus
}
#endif

#endif	/* LEDS_H */

