/************************************************************************
This is a library to handling capacitive multitouch sensors using FT5206.
Originally written to work with ER-TFTM070-5 (LCD module) from EastRising.

Written by Helge Langehaug, February 2014

BSD license, all text above must be included in any redistribution
-------------------------------------------------------------------------------
Modified by Max MC Costa from .s.u.m.o.t.o.y. to correctly return coordinates,
fixed initialization, added gesture, touch state and faster operations.
*************************************************************************/

#include <SPI.h>
#include "i2c_t3.h"
#include "FT5206.h"


static volatile boolean _touched = false;


FT5206::FT5206(uint8_t CTP_INT)
{
    _ctpInt = CTP_INT;
	_maxTouch = 5;
}


void FT5206::isr(void)
{
	_touched = true;
}


//in EXTRLN the entire ISR it's not handled by the library!
void FT5206::begin(enum FT5206isr init) 
{
    _isrMode = init;
    Wire.begin();
	#if ARDUINO >= 157
		Wire.setClock(400000UL); // Set I2C frequency to 400kHz
	#else
		TWBR = ((F_CPU / 400000UL) - 16) / 2; // Set I2C frequency to 400kHz
	#endif
	delay(10);
	writeRegister(FT5206_DEVICE_MODE,0);
	if (_isrMode != EXTRLN){
		pinMode(_ctpInt ,INPUT);
#ifdef digitalPinToInterrupt
		attachInterrupt(digitalPinToInterrupt(_ctpInt),isr,FALLING);
#else
		attachInterrupt(0,isr,FALLING);
#endif
	}
}
 
//this rearm ISR only in SAFE mode, any other mode does nothing
void FT5206::rearmISR(void) 
{
	if (_isrMode == SAFE){
#ifdef digitalPinToInterrupt
		attachInterrupt(digitalPinToInterrupt(_ctpInt),isr,FALLING);
#else
		attachInterrupt(0,isr,FALLING);
#endif
		_touched = false;
	}
}

//in safe mode it will also disconnect interrupt!
bool FT5206::touched()
{
    if (_touched){
		if (_isrMode == SAFE){
#ifdef digitalPinToInterrupt
		detachInterrupt(digitalPinToInterrupt(_ctpInt));
#else
		detachInterrupt(0);
#endif
		} else {
			_touched = false;
		}
		return true;
    }
	return false;
}

void FT5206::writeRegister(uint8_t reg,uint8_t val)
{
    Wire.beginTransmission(FT5206_I2C_ADDRESS);
    Wire.write(reg);
    Wire.write(val);
    Wire.endTransmission(FT5206_I2C_ADDRESS);
}
 
 
void FT5206::setTouchLimit(uint8_t limit)
{
	if (limit > 5) limit = 5;
	_maxTouch = limit;
}

uint8_t FT5206::getTouchLimit(void)
{
	return _maxTouch;
}

uint8_t FT5206::getTScoordinates(uint16_t (*touch_coordinates)[2], uint8_t *reg)
{
	uint8_t touches = reg[FT5206_TD_STATUS] & 0xF;
	uint8_t i;
	if (touches < 1) return 0;
 	for (i=1;i<=touches;i++){
		touch_coordinates[i-1][0] = ((reg[coordRegStart[i-1]] & 0x0f) << 8) | reg[coordRegStart[i-1] + 1];
		touch_coordinates[i-1][1] = ((reg[coordRegStart[i-1] + 2] & 0x0f) << 8) | reg[coordRegStart[i-1] + 3];
		if (i == _maxTouch) return i;
	} 
    return touches;
}

void FT5206::getTSregisters(uint8_t *registers) 
{
    Wire.requestFrom(FT5206_I2C_ADDRESS, FT5206_REGISTERS); 
    uint8_t register_number = 0;
    while(Wire.available()) {
      registers[register_number++] = Wire.read();
    }
}
  

uint8_t FT5206::getGesture(uint8_t *reg) 
{
	if (_maxTouch < 2) return 0;
	uint8_t temp = reg[FT5206_GEST_ID];
	switch(temp){
		case 0x00: //not rec
			return 0;
		break;
		case 0x10: //move up
			return 1;
		break;
		case 0x14: //move left
			return 2;
		break;
		case 0x18: //move down
			return 3;
		break;
		case 0x1C: //move right
			return 4;
		break;
		case 0x48: //zoom in
			return 5;
		break;
		case 0x49: //zoom out
			return 6;
		break;
		default:
		return temp;
	}
	return 0;
}

uint8_t FT5206::getTSflag(uint8_t *reg) 
{
	uint8_t temp = reg[FT5206_TOUCH1_XH];
	//if (!bitRead(temp,7) && !bitRead(temp,6)) return 3;//
	if (!bitRead(temp,7) && bitRead(temp,6)) return 1;//finger up
	if (bitRead(temp,7) && !bitRead(temp,6)) return 2;//finger down
	return 0;
}
