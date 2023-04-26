/** \file lcddemo.c
 *  \brief A simple demo that draws a string and square
 */

#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"

/** Initializes everything, clears the screen, draws "hello" and a square */
int
main()
{
  configureClocks();
  lcd_init();
  u_char width = screenWidth, height = screenHeight;

  #define centerRow (screenHeight/2)
  #define centerCol (screenWidth/2)

  clearScreen(COLOR_DARK_GREEN);  
  
  border();
  
  
 
  
}
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

void
tennisCourt()
{
   fillRectangle(0,0, 3, screenHeight, COLOR_WHITE); // verticle double line
  fillRectangle(0,0, screenWidth, 3, COLOR_WHITE); // top horizontal
  fillRectangle(15,0, 3, screenHeight, COLOR_WHITE); //  left vertical single line
  fillRectangle(0,screenHeight-3, screenWidth, 3, COLOR_WHITE); // bottom horizontal
  fillRectangle(screenWidth-3,0, 3, screenHeight, COLOR_WHITE); // right vertical double line
  fillRectangle(screenWidth-20,0, 3, screenHeight, COLOR_WHITE); // right vertical single line
  fillRectangle(0,centerRow-3, screenWidth, 3, COLOR_GRAY); // net
  fillRectangle(15,(centerRow/2)-3, (screenWidth-20) - 15, 3, COLOR_WHITE); // service line1
  fillRectangle(15,(centerRow * 1.5)-3, (screenWidth-20) - 15, 3, COLOR_WHITE); // serivce line2
  fillRectangle(centerCol-3, (centerRow/2), 3, (screenHeight/2), COLOR_WHITE); // centre serivce line
  fillRectangle(centerCol-3, 0, 3, 6, COLOR_WHITE); // center mark
  fillRectangle(centerCol-3, screenHeight-6, 3, 6, COLOR_WHITE); // cernter mark
}

void
startingScreen11x16(){
  tennisCourt();
  drawString11x16((centerCol/2)-5, (centerRow/2)-25, "TENNIS", COLOR_NAVY, COLOR_DARK_GREEN); //Title Card
  drawString11x16((centerCol/2)+10, (centerRow/2)-5, "GAME", COLOR_NAVY, COLOR_DARK_GREEN); //Title Card
  drawString11x16((centerCol/2)+2, (centerRow/2)+25, "START", COLOR_DARK_GREEN, COLOR_RED);
  drawString11x16((centerCol/2)+8, (centerRow/2)+50, "QUIT", COLOR_DARK_GREEN, COLOR_RED);
}
void
makeTri(){
  for(int i = 0; i < 20; i++){
    drawPixel(centerCol + i, centerRow - i, COLOR_PINK);
    drawPixel(centerCol - i, centerRow - i, COLOR_GREEN);
    drawPixel(centerCol - 19 + (i*2), centerRow - 19  , COLOR_YELLOW);
  }
}

void
makeSqr(){
  for(int i = 0; i < 20; i++){
    drawPixel(centerCol - 19 + (i*2), centerRow - 19 , COLOR_YELLOW); // top
    drawPixel(centerCol - 19 + (i*2), centerRow + 19 , COLOR_YELLOW); // bottom
    drawPixel(centerCol - 19 , centerRow - 19 + (i*2), COLOR_YELLOW); // left side
    drawPixel(centerCol + 19 , centerRow - 19 + (i*2), COLOR_YELLOW); // right side
    
  }
}

void
makeDmd(){
  for(int i = 0; i < 20; i++){
    drawPixel(centerCol + i, centerRow + 19 - i , COLOR_PINK);  // bottom r
    drawPixel(centerCol - i, centerRow + 19 - i , COLOR_GREEN); // bottom l
    drawPixel(centerCol + 19 - i, centerRow - i , COLOR_PINK);     // top r  
    drawPixel(centerCol - 19 + i, centerRow - i , COLOR_GREEN);    // top l
    
  }
} 
makeRtri(){
  for(int i = 0; i < 20; i++){
    drawPixel(centerCol - 19 + (i*2), centerRow + 9 - (i*2) , COLOR_RED); // diag (c)
    drawPixel(centerCol + 19  , centerRow - 29 + (i*2), COLOR_GREEN); // v line (a)
    drawPixel(centerCol - 19 + (i*2), centerRow + 9, COLOR_PINK); // h line (b)
  }
}
