#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "ball.h"
#include "collisions.h"
#include "sound_effect.h"

// axis zero for col, axis 1 for row
// ball 
int ballPos[2] = {screenWidth-15, screenHeight-20}, nextPos[2] = {screenWidth-14, screenHeight-20};
int colVelocity = -4, rowVelocity = -5;
int colLimits[2] = {5 , screenWidth-10};
int rowLimits[2] = {20 , screenHeight+5}; 

void
draw_ball(int col, int row, unsigned short color)
{
  
  fillRectangle(col-1, row-1, 6, 6, color);
}
void
screen_update_ball()
{
  for (int axis = 0; axis < 2; axis ++)

    if (ballPos[axis] != nextPos[axis]) /* position changed? */

      goto redraw;

  return;/* nothing to do */

 redraw:

  draw_ball(ballPos[0], ballPos[1], COLOR_DARK_GREEN); /* erase */

  for (int axis = 0; axis < 2; axis ++)

    ballPos[axis] = nextPos[axis];

  draw_ball(ballPos[0], ballPos[1], COLOR_WHITE); /* draw */
}
void ball_boundary(){
  int oldCol = nextPos[0];
  int newCol = oldCol + colVelocity;

  int oldRow = nextPos[1];
  int newRow = oldRow + rowVelocity;
  
  if(ball_paddle_collision()){
    collision(); // sound
    rowVelocity = -rowVelocity;
    nextPos[1] = newRow + rowVelocity;
  }
  else if( ball_paddle2_collision()){
    collision(); // sound
    rowVelocity = -rowVelocity;
    
    nextPos[1] = newRow + rowVelocity;
  } 
  else if (newCol <= colLimits[0] || newCol >= colLimits[1]){
    colVelocity = -colVelocity;
  }
   
 
  newCol = oldCol + colVelocity;  // new col result

  newRow = oldRow  + rowVelocity; // new row result
  
  nextPos[0] = newCol;  
  nextPos[1] = newRow;
  
}
