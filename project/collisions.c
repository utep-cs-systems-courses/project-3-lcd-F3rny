#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "ball.h"
#include "paddles.h"
#include "collisions.h"

char ball_paddle_collision()
{
  int ballLeft = ballPos[0] - 1;  // left edge of ball
  int ballRight = ballPos[0] + 6; // right edge of ball
  int ballTop = ballPos[1] - 1;   // top edge of ball
  int ballBottom = ballPos[1] + 6; // bottom edge of ball
  int paddle1Left = paddlePos1[0] - 1;  // left edge of paddle
  int paddle1Right = paddlePos1[0] + 30; // right edge of paddle
  int paddle1Top = paddlePos1[1] - 1;   // top edge of paddle
  int paddle1Bottom = paddlePos1[1] + 10; // bottom edge of paddle

  // check for intersection of x-coordinate ranges

  if (ballRight >= paddle1Left && ballLeft <= paddle1Right) {
    // check for intersection of y-coordinate ranges
    if (ballBottom >= paddle1Top && ballTop <= paddle1Bottom) {
      return 1; // collision detected
    }
  }
  return 0; // no collision detected
}

char
ball_paddle2_collision()
{
  int ballLeft = ballPos[0] - 1;  // left edge of ball
  int ballRight = ballPos[0] + 6; // right edge of ball
  int ballTop = ballPos[1] - 1;   // top edge of ball
  int ballBottom = ballPos[1] + 6; // bottom edge of ball
  int paddle2Left = paddlePos2[0] - 1;  // left edge of paddle
  int paddle2Right = paddlePos2[0] + 30; // right edge of paddle
  int paddle2Top = paddlePos2[1] - 1;   // top edge of paddle
  int paddle2Bottom = paddlePos2[1] + 10; // bottom edge of paddle

  if(ballRight >= paddle2Left && ballLeft <= paddle2Right){
    if( ballBottom >= paddle2Top && ballTop <= paddle2Bottom) {
      return 1;
    }
  }
  return 0;

}


