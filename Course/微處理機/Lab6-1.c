#include <stdio.h>
#include <math.h>
#include "NUC100Series.h"
#include "MCU_init.h"
#include "SYS_init.h"
#include "LCD.h"
#include "Draw2D.h"
#include "Scankey.h"
#include "Seven_Segment.h"

#define CENTER_X 63    // LCD center X coordinate
#define CENTER_Y 31    // LCD center Y coordinate
#define RADIUS 30      // Circle radius in pixels
#define  PI 3.14159265

void Display_7seg(uint16_t value) {
		uint8_t digit;

		if(value >= 10){
				digit = value / 10;
				CloseSevenSegment();
				ShowSevenSegment(1,digit);
				CLK_SysTickDelay(5000);
				value -= digit * 10;
		}

		digit = value;
		CloseSevenSegment();
		ShowSevenSegment(0,digit);
		CLK_SysTickDelay(5000);
}

void draw_polygon(int sides) {
    int x1, y1, x2, y2;
		int i;
    float angle = PI/2;  // Starting from 90 degrees (p/2)
    float angleStep = 2*PI/sides;
   
    // Calculate and draw each side of the polygon
    for(i = 0; i < sides; i++) {
        // Calculate current point
        x1 = CENTER_X + RADIUS * cos(angle);
        y1 = CENTER_Y - RADIUS * sin(angle);  // Subtract because Y axis is inverted
       
        // Calculate next point
        x2 = CENTER_X + RADIUS * cos(angle + angleStep);
        y2 = CENTER_Y - RADIUS * sin(angle + angleStep);
       
        // Draw line between points
        draw_Line(x1, y1, x2, y2, FG_COLOR, BG_COLOR);
       
        angle += angleStep;
    }
}

int main(void) {
		int i;
		int key = 0;
		int flag = 1;
		int side = 6;

		SYS_Init();
		OpenKeyPad();
		OpenSevenSegment();
		init_LCD();
		clear_LCD();
		draw_polygon(side);
		Display_7seg(side);

		while(1) {
				draw_Circle(63,31,30,FG_COLOR, BG_COLOR);
				key = ScanKey();
				if(key != 0 && flag == 1) {
						clear_LCD();
						flag = 0;
						if(key == 4 && side > 3) side--; //-1
						else if(key == 6 && side < 12) side++; //+1
						draw_polygon(side);
				}
				else if(key == 0 && flag == 0) flag = 1;
				Display_7seg(side);
		}
}
