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
 * File:   main.h
 * Author: Ciprian Stingu
 *
 * Created on April 14, 2021, 7:25 PM
 */

#ifndef MAIN_H
#define	MAIN_H

#include <xc.h>
//#include <pic16f628a.h> 
#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif

#pragma config CP    = OFF
#pragma config CPD   = OFF
#pragma config LVP   = OFF
#pragma config BOREN = ON
#pragma config MCLRE = OFF
#pragma config PWRTE = OFF
#pragma config WDTE  = OFF
#pragma config FOSC  = INTOSCIO

#define _XTAL_FREQ 4000000

#define IO_IN    1
#define IO_OUT   0
#define PIN_HIGH 1
#define PIN_LOW  0
    
//Atari pins
#define ATARI_X1 RB7
#define ATARI_X2 RB6
#define ATARI_Y1 RB5
#define ATARI_Y2 RB4
#define ATARI_LB RA2
#define ATARI_MB RA3
#define ATARI_RB RA4
    
//Amiga pins
#define AMIGA_X1 RB7
#define AMIGA_X2 RB4
#define AMIGA_Y1 RB5
#define AMIGA_Y2 RB6
#define AMIGA_LB RA2
#define AMIGA_MB RA3
#define AMIGA_RB RA4

//config
//#define AMIGA_MOUSE
#ifdef AMIGA_MOUSE
    #define AXIS_X1 AMIGA_X1
    #define AXIS_X2 AMIGA_X2
    #define AXIS_Y1 AMIGA_Y1
    #define AXIS_Y2 AMIGA_Y2
    #define BUTTON_L AMIGA_LB
    #define BUTTON_R AMIGA_RB
    #define BUTTON_M AMIGA_MB
#else
    #define AXIS_X1 ATARI_X1
    #define AXIS_X2 ATARI_X2
    #define AXIS_Y1 ATARI_Y1
    #define AXIS_Y2 ATARI_Y2
    #define BUTTON_L ATARI_LB
    #define BUTTON_R ATARI_RB
    #define BUTTON_M ATARI_MB
#endif

void __interrupt() ISR(void);

//#define ENABLE_DEBUG
#ifdef ENABLE_DEBUG
    #define NULL_CHAR 0x00
    void ByteToAsciiHex(uint8_t value, char *buffer);
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_H */

