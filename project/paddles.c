#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "ball.h"
#include "paddles.h"

// paddles
int paddlePos1[2] = {(screenWidth/2) + 30, screenHeight-8};
int futurePP1[2] = {(screenWidth/2) + 31, screenHeight- 8};

int paddlePos2[2] = {15, 35};
int futurePP2[2] = {16, 35};

int paddleVelocity = 15;

void
draw_paddle(int col , int row, unsigned short color)
{
  fillRectangle(col-1, row-1, 20, 3, color);

}

void
screen_update_paddle1(){

  for (int axis = 0; axis < 2; axis ++)

    if (paddlePos1[axis] != futurePP1[axis]) /* position changed? */

      goto redraw;

  return;/* nothing to do */

 redraw:

  draw_paddle(paddlePos1[0], paddlePos1[1], COLOR_DARK_GREEN); 

  for (int axis = 0; axis < 2; axis ++)

    paddlePos1[axis] = futurePP1[axis];

  draw_paddle(paddlePos1[0], paddlePos1[1], COLOR_WHITE);


}
void
screen_update_paddle2(){


  for (int axis = 0; axis < 2; axis ++)

    if (paddlePos2[axis] != futurePP2[axis]) /* position changed? */

      goto redraw;

  return;/* nothing to do */

 redraw:

  draw_paddle(paddlePos2[0], paddlePos2[1], COLOR_DARK_GREEN); 

  for (int axis = 0; axis < 2; axis++)

    paddlePos2[axis] = futurePP2[axis];

  draw_paddle(paddlePos2[0], paddlePos2[1], COLOR_WHITE);


}


void
paddle1_left()
{
  int oldCol = futurePP1[0];
  int newCol = oldCol - paddleVelocity;
  if(newCol >= colLimits[0]){
    futurePP1[0] = newCol;
  }
   
}
void
paddle1_right()
{
  int oldCol = futurePP1[0];
  int newCol = oldCol + paddleVelocity;
  if( newCol <= (colLimits[1])-15)
    futurePP1[0] = newCol;
}
void
paddle2_left()
{
  int oldCol = futurePP2[0];
  int newCol = oldCol + paddleVelocity;
  if(newCol <= (colLimits[1]-15)){
    futurePP2[0] = newCol;
  }
   
}
void
paddle2_right()
{
  int oldCol = futurePP2[0];
  int newCol = oldCol - paddleVelocity;
  if( newCol > (colLimits[0]))
    futurePP2[0] = newCol;
}



