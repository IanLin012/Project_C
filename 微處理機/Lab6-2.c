//
// LCD_Bmp_Keypad: use 3x3 keypad to move bitmap on LCD
//
#include <stdio.h>
#include "NUC100Series.h"
#include "MCU_init.h"
#include "SYS_init.h"
#include "LCD.h"
#include "Scankey.h"

unsigned char bmp16x16[32] = {
	0x00,0x00,0x40,0xE0,0xE0,0xE0,0xE0,0x70,0x7C,0xFE,0xF8,0x80,0x80,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x01,0x03,0x7F,0x3E,0x1E,0x07,0x07,0x07,0x03,0x03,0x02,0x00
};

int main(void)
{
	int8_t x=0,y=0;
	int8_t keyin=0;
	int8_t movX=3, dirX=0;
	int8_t movY=3, dirY=0;
	int i=0;
	
	SYS_Init();
	  
	init_LCD();
	clear_LCD();
	
	OpenKeyPad();
	
  x=64; y=32;
	draw_Bmp16x16(x,y,FG_COLOR,BG_COLOR,bmp16x16);
	
	while(keyin==0) keyin = ScanKey(); // wait till key is pressed

	clear_LCD();
	
	while(1) {
		draw_Bmp16x16(x,y,FG_COLOR,BG_COLOR,bmp16x16); // Draw Object
		for(i=0; i<10000; i++) {
			CLK_SysTickDelay(50);
			keyin = ScanKey();
		}
		draw_Bmp16x16(x,y,BG_COLOR,BG_COLOR,bmp16x16); // Erase Object
		
    switch(keyin){   // input to change direction
			case 2: dirX=0;  dirY=-1; break;
			case 4:	dirX=-1; dirY=0;  break;
			case 6: dirX=+1; dirY=0;  break;
			case 8: dirX=0;  dirY=+1; break;		
			default: break;
		}
		
		x += dirX * movX; // increment/decrement X
		y += dirY * movY; // increment/decrement Y
		
		if (x<0)           x=LCD_Xmax-16; // check X boundary Min
		if (x>LCD_Xmax-16) x=0; // check X boundary Max
		if (y<0)           y=LCD_Ymax-16; // check Y boundary Min
		if (y>LCD_Ymax-16) y=0; // check Y boundary Max
	}
}
