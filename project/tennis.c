#include <libTimer.h>

#include "lcdutils.h"
#include "tennis.h"
#include "lcddraw.h"

void border()
{
  fillRectangle(0,0, screenWidth, 30, COLOR_CHOCOLATE); //Boarder for Score
  fillRectangle(0,0, 5, screenHeight, COLOR_CHOCOLATE); // left dirt boarder 
  fillRectangle(screenWidth-5,0, 5, screenHeight, COLOR_CHOCOLATE); // right dirt boarder
}

void
mainCourt()
{
  fillRectangle(5,30, 3, screenHeight, COLOR_WHITE); // verticle double line
  fillRectangle(5,30, screenWidth-10, 3, COLOR_WHITE); // top horizontal
  fillRectangle(20,30, 3, screenHeight, COLOR_WHITE); //  left vertical single line
  fillRectangle(5,screenHeight-3, screenWidth-10, 3, COLOR_WHITE); // bottom horizontal
  fillRectangle(screenWidth-8,30, 3, screenHeight, COLOR_WHITE); // right vertical double line
  fillRectangle(screenWidth-25,30, 3, screenHeight, COLOR_WHITE); // right vertical single line
  fillRectangle(20,(centerRow/2)+25, (screenWidth-20) - 25, 3, COLOR_WHITE); // service line1
  fillRectangle(20,(centerRow * 1.5)+3, (screenWidth-20) - 25, 3, COLOR_WHITE); // serivce line2
  fillRectangle(centerCol-3, (centerRow/2)+25, 3, (screenHeight/2)-20, COLOR_WHITE); // centre serivce line
   fillRectangle(5,centerRow+15, screenWidth-10, 3, COLOR_GRAY); // net
  fillRectangle(centerCol-3, 30, 3, 6, COLOR_WHITE); // center mark
  fillRectangle(centerCol-3, screenHeight-6, 3, 6, COLOR_WHITE); // cernter mark
}


void tennisCourt()
{
    fillRectangle(0,0, 3, screenHeight, COLOR_WHITE); // verticle double line
    fillRectangle(0,0, screenWidth, 3, COLOR_WHITE); // top horizontal
    fillRectangle(0,screenHeight-3, screenWidth, 3, COLOR_WHITE); // bottom horizontal
    fillRectangle(screenWidth-3,0, 3, screenHeight, COLOR_WHITE); // right vertical double line
    fillRectangle(15,0, 3, screenHeight, COLOR_WHITE); //  left vertical single line
    fillRectangle(screenWidth-20,0, 3, screenHeight, COLOR_WHITE); // right vertical single line
    fillRectangle(0,centerRow-3, screenWidth, 3, COLOR_GRAY); // net
    fillRectangle(15,(centerRow/2)-3, (screenWidth-20) - 15, 3, COLOR_WHITE); // service line1
    fillRectangle(15,(centerRow * 1.5)-3, (screenWidth-20) - 15, 3, COLOR_WHITE); // serivce line2
    fillRectangle(centerCol-3, (centerRow/2), 3, (screenHeight/2), COLOR_WHITE); // centre serivce line
    fillRectangle(centerCol-3, 0, 3, 6, COLOR_WHITE); // center mark
    fillRectangle(centerCol-3, screenHeight-6, 3, 6, COLOR_WHITE); // cernter mark
}
