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
 * File:   ps2.h
 * Author: Ciprian Stingu
 *
 * Created on April 14, 2021, 11:51 PM
 */

#ifndef PS2_H
#define	PS2_H

#ifdef	__cplusplus
extern "C" {
#endif   
 
//PS2 pins
#define PS2_CLOCK_PIN RA0
#define PS2_CLOCK_IO  TRISA0
#define PS2_DATA_PIN  RA1 
#define PS2_DATA_IO   TRISA1

#define PS2_OPERATION_RESET                0xFF
#define PS2_OPERATION_RESEND               0xFE
#define PS2_OPERATION_SET_DEFAULTS         0xF6
#define PS2_OPERATION_DATA_REPORTING_OFF   0xF5
#define PS2_OPERATION_DATA_REPORTING_ON    0xF4
#define PS2_OPERATION_SET_SAMPLE_RATE      0xF3
#define PS2_OPERATION_GET_DEVICE_ID        0xF2
#define PS2_OPERATION_SET_REMOTE_MODE      0xF0
#define PS2_OPERATION_RESET_WRAP_MODE      0xEC
#define PS2_OPERATION_READ_DATA            0xEB
#define PS2_OPERATION_SET_STREAM_MODE      0xEA
#define PS2_OPERATION_STATUS_REQUEST       0xE9
#define PS2_OPERATION_SET_RESOLUTION       0xE8
#define PS2_OPERATION_SET_SCALING_2        0xE7
#define PS2_OPERATION_SET_SCALING_1        0xE6

#define PS2_RESPONSE_ACKNOWLEDGE    0xFA
#define PS2_RESPONSE_BAT_SUCCESSFUL 0xAA
#define PS2_RESPONSE_ERROR          0xFC

#define PS2_READ_ERROR_1 0xFF01
#define PS2_READ_ERROR_2 0xFF02
#define PS2_READ_ERROR_3 0xFF03

uint8_t MouseDataFlags = 0;
uint8_t MouseDataXDelta = 0;
uint8_t MouseDataYDelta = 0;

void Ps2Init(void);
void Ps2Write(uint8_t data);
uint16_t Ps2Read(void);
uint8_t Ps2Reset(void);
uint8_t Ps2SetRemoteMode(void);
uint8_t Ps2UpdateMouseData(void);
uint8_t Ps2SetSampleRate(uint8_t sampleRate);
uint8_t Ps2SetResolution(uint8_t resolution);

#ifdef	__cplusplus
}
#endif

#endif	/* PS2_H */

