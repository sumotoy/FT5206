
#include <SPI.h>
#include <RA8875.h>
#include <Wire.h>
#include <FT5206.h>

#define RA8875_CS         10 
#define RA8875_RESET      9                 
#define CTP_INT           2    // touch data ready for read from FT5206 touch controller

uint8_t registers[FT5206_REGISTERS];
uint16_t new_coordinates[5][2];
uint16_t old_coordinates[5][2]; 
uint8_t current_touches = 0;
uint8_t old_touches = 0;


RA8875 tft = RA8875(RA8875_CS, RA8875_RESET);
FT5206 cts = FT5206(CTP_INT);


void setup() 
{
  Serial.begin(38400);
  long unsigned debug_start = millis ();
  while (!Serial && ((millis () - debug_start) <= 5000)) ;
  tft.begin(RA8875_800x480);
  cts.begin(SAFE);
  tft.setTextColor(0xFFFF,0x0000);
  Serial.println("inited...");
}



void loop() 
{
  //cts.setTouchLimit(1);//from 1 to 5
  if (cts.touched()){
    /*
    In SAFE mode the interrupt it's has been disabled
    from exact this point so you are safe to do your stuff without risk that another 
    touch causes an interrupt and exit the following calculations.
    You NEED to rearm interrupt manually by using rearmISR (when it's safe for you).
    */
    uint8_t i;
    uint16_t x,y;
    cts.getTSregisters(registers);
    current_touches = cts.getTScoordinates(new_coordinates, registers);
    if (current_touches < 1) return;
    
    
    /*
    uint8_t temp1 = cts.getGesture(registers);
    if (temp1 > 0){
      tft.setCursor(0,0);
      tft.print("                  ");
      tft.setCursor(0,0);
      tft.print(temp1);
    }
    */
    /*
    uint8_t temp2 = cts.getTSflag(registers);
    if (temp2 > 0){
      tft.setCursor(0,20);
      tft.print("                  ");
      tft.setCursor(0,20);
      tft.print(temp2);
    }
    */
    
    for (i = 1; i <= old_touches; i++){// remove previous touches on screen
      tft.fillCircle(old_coordinates[i-1][0], old_coordinates[i-1][1], 70, RA8875_BLACK);
    }
    for (i = 1; i <= current_touches; i++){// mark touches on screen
      x = new_coordinates[i-1][0];
      y = new_coordinates[i-1][1];
      // Mark touches on screen
      tft.fillCircle(x, y, 70, RA8875_BLUE);
      tft.fillCircle(x, y, 50, RA8875_WHITE);
      tft.fillCircle(x, y, 30, RA8875_WHITE);
      old_coordinates[i-1][0] = x;
      old_coordinates[i-1][1] = y;
    }
    old_touches = current_touches;
    /*
    Here we rearm the interrupt
    */
    cts.rearmISR();
  }

}