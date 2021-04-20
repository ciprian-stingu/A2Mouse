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
 * File:   quadrature.h
 * Author: Ciprian Stingu
 *
 * Created on April 14, 2021, 11:51 PM
 */

#ifndef QUADRATURE_H
#define	QUADRATURE_H

#ifdef	__cplusplus
extern "C" {
#endif

#define QADRATURE_DIRECTION_RIGHT  1
#define QADRATURE_DIRECTION_LEFT  -1
#define QUADRATURE_BUTTON_ON  PIN_LOW
#define QUADRATURE_BUTTON_OFF PIN_HIGH
#define QUADRATURE_MAX_MOVEMENT_LENGTH 120
    
int8_t XAxisDirection = QADRATURE_DIRECTION_RIGHT;
int8_t YAxisDirection = QADRATURE_DIRECTION_RIGHT;    
int8_t XAxisTransitionIndex = 0;
int8_t YAxisTransitionIndex = 0;
int16_t XAxisDistance = 0;
int16_t YAxisDistance = 0;
uint8_t LeftMouseStatus = QUADRATURE_BUTTON_OFF;
uint8_t MiddleMouseStatus = QUADRATURE_BUTTON_OFF;
uint8_t RightMouseStatus = QUADRATURE_BUTTON_OFF;
uint8_t QuadratureShouldSendUpdate = 0;

void QuadratureMouseInit(void);
void QuadratureMouseUpdate(void);
void QuadratureMouseOutput(void);

#ifdef	__cplusplus
}
#endif

#endif	/* QUADRATURE_H */

