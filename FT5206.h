/************************************************************************
This is a library to handling capacitive multitouch sensors using FT5206.
Originally written to work with ER-TFTM070-5 (LCD module) from EastRising.

Written by Helge Langehaug, February 2014

BSD license, all text above must be included in any redistribution
-------------------------------------------------------------------------------
Modified by Max MC Costa from .s.u.m.o.t.o.y. to correctly return coordinates,
fixed initialization, added gesture, touch state and faster operations.
works with Teensy 3,3.1,LC and Arduino's (dunno DUE)
version:1.0
*************************************************************************/

#ifndef _FT5206_H_
#define _FT5206_H_

#if defined(ENERGIA) // LaunchPad, FraunchPad and StellarPad specific
	#include "Energia.h"
#else
	#include "Arduino.h"
#endif

/* FT5206 definitions */
#define FT5206_I2C_ADDRESS 			0x38
#define FT5206_REGISTERS 	31     // there are more registers, but this is enought to get all 5 touch coordinates.                    
#define FT5206_TOUCH1_XH 			0x03
#define FT5206_GEST_ID 				0x01
#define FT5206_DEVICE_MODE 			0x00
//number of touched points [3:0] , valid 1..5
#define FT5206_TD_STATUS 			0x02//n. of touch points [R]
/*
bit[7:0] the value will be x4 times the register value (280)
*/
#define FT5206_ID_G_THGROUP 		0x80//valid touching detect threshold [R/W]
/*
bit[7:0]  (60)
*/
#define FT5206_ID_G_THPEAK 			0x81//valid touching peak detect threshold [R/W]
/*
bit[7:0]  (16)
*/
#define FT5206_ID_G_THCAL 			0x82//the threshold when calculating the focus of touching[R/W]
/*
bit[7:0] value it's x32 times the register val (20)
*/
#define FT5206_ID_G_THDIFF	 		0x85//the threshold where the coordinatis differs from orig[R/W]
#define FT5206_ID_G_CLTR	 		0x86//power control mode[R/W]

/*
TOUCH - XH
bit[7:6] event flag -> 00:put down
					-> 01:put up
					-> 10:contact
					-> 11: (reserved)
bit[5:4] (reserved)
bit[3:0] touch MSB X pos (pix)
TOUCH - XL
bit[7:0] touch LSB X pos (pix)
TOUCH - YH
bit[7:4] touch ID of Touch Point
bit[3:0] touch MSB Y pos (pix)
TOUCH - YL
bit[7:0] touch LSB Y pos (pix)
*/

enum FT5206isr{ INTRL=0, SAFE, EXTRLN };

class FT5206 {
 public:
	FT5206(uint8_t CTP_INT);
	void 				begin(enum FT5206isr init=INTRL);
	uint8_t 			getTScoordinates(uint16_t (*touch_coordinates)[2], uint8_t *reg);
	void 				getTSregisters(uint8_t *registers);
	bool 				touched();
	uint8_t 			getGesture(uint8_t *reg);
	uint8_t 			getTSflag(uint8_t *reg);
	void 				writeRegister(uint8_t reg,uint8_t val);
	void 				setTouchLimit(uint8_t limit);
	uint8_t 			getTouchLimit(void);
	void 				rearmISR(void);
 private:
	static void 		 isr(void);
	uint8_t 			_ctpInt;
	uint8_t				_maxTouch;
	enum FT5206isr 		_isrMode;
	const uint8_t coordRegStart[5] = {{0x03},{0x09},{0x0F},{0x15},{0x1B}};
};

#endif