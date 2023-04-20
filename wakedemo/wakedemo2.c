#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"

// WARNING: LCD DISPLAY USES P1.0.  Do not touch!!! 

#define LED BIT6		/* note that bit zero req'd for display */

#define SW1 1
#define SW2 2
#define SW3 4
#define SW4 8

#define SWITCHES 15

unsigned char step = 0;

static char 
switch_update_interrupt_sense()
{
  char p2val = P2IN;
  /* update switch interrupt to detect changes from current buttons */
  P2IES |= (p2val & SWITCHES);	/* if switch up, sense down */
  P2IES &= (p2val | ~SWITCHES);	/* if switch down, sense up */
  return p2val;
}

void 
switch_init()			/* setup switch */
{  
  P2REN |= SWITCHES;		/* enables resistors for switches */
  P2IE |= SWITCHES;		/* enable interrupts from switches */
  P2OUT |= SWITCHES;		/* pull-ups for switches */
  P2DIR &= ~SWITCHES;		/* set switches' bits for input */
  switch_update_interrupt_sense();
}

int switches = 0;

void
switch_interrupt_handler()
{
  char p2val = switch_update_interrupt_sense();
  switches = ~p2val & SWITCHES;

}


// axis zero for col, axis 1 for row
// ball 
int ballPos[2] = {screenWidth-15, screenHeight-20}, nextPos[2] = {screenWidth-14, screenHeight-20};
int colVelocity = -4, rowVelocity = -5;
int colLimits[2] = {0 , screenWidth};
int rowLimits[2] = {0 , screenHeight}; 

// paddles
int paddlePos1[2] = {(screenWidth/2) + 35, screenHeight-6};
int futurePP1[2] = {(screenWidth/2) + 36, screenHeight-6};

int paddlePos2[2] = {15, 5};
int futurePP2[2] = {16, 5};

int paddleVelocity = 5;

void
draw_ball(int col, int row, unsigned short color)
{
  
  fillRectangle(col-1, row-1, 6, 6, color);
}
void
draw_paddle(int col , int row, unsigned short color)
{
  fillRectangle(col-1, row-1, 20, 5, color);

}

char ball_paddle_collision()
{
  int ballLeft = ballPos[0] - 1;  // left edge of ball

  int ballRight = ballPos[0] + 6; // right edge of ball

  int ballTop = ballPos[1] - 1;   // top edge of ball

  int ballBottom = ballPos[1] + 6; // bottom edge of ball

  int paddle1Left = paddlePos1[0] - 1;  // left edge of paddle

  int paddle1Right = paddlePos1[0] + 20; // right edge of paddle

  int paddle1Top = paddlePos1[1] - 1;   // top edge of paddle

  int paddle1Bottom = paddlePos1[1] + 5; // bottom edge of paddle



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

  int paddle2Right = paddlePos2[0] + 20; // right edge of paddle

  int paddle2Top = paddlePos2[1] - 1;   // top edge of paddle

  int paddle2Bottom = paddlePos2[1] + 5; // bottom edge of paddle

  if(ballRight >= paddle2Left && ballLeft <= paddle2Right){
    
    if( ballBottom <= paddle2Top && ballTop >= paddle2Bottom) {

      return 1;

    }
    
  }
  return 0;
 
}
void
screen_update_paddle1(){

  for (int axis = 0; axis < 2; axis ++)

    if (paddlePos1[axis] != futurePP1[axis]) /* position changed? */

      goto redraw;

  return;/* nothing to do */

 redraw:

  draw_paddle(paddlePos1[0], paddlePos1[1], COLOR_BLUE); 

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

  draw_paddle(paddlePos2[0], paddlePos2[1], COLOR_BLUE); 

  for (int axis = 0; axis < 2; axis++)

    paddlePos2[axis] = futurePP2[axis];

  draw_paddle(paddlePos2[0], paddlePos2[1], COLOR_WHITE);


}

void
screen_update_ball()
{
  for (int axis = 0; axis < 2; axis ++)

    if (ballPos[axis] != nextPos[axis]) /* position changed? */

      goto redraw;

  return;/* nothing to do */

 redraw:

  draw_ball(ballPos[0], ballPos[1], COLOR_BLUE); /* erase */

  for (int axis = 0; axis < 2; axis ++)

    ballPos[axis] = nextPos[axis];

  draw_ball(ballPos[0], ballPos[1], COLOR_WHITE); /* draw */
}
void
paddle1_left()
{
  int oldCol = futurePP1[0];
  int newCol = oldCol - paddleVelocity;
  if(newCol > colLimits[0]){
    futurePP1[0] = newCol;
  }
   
}
void
paddle1_right()
{
  int oldCol = futurePP1[0];
  int newCol = oldCol + paddleVelocity;
  if( newCol < (colLimits[1] - 20))
    futurePP1[0] = newCol;
}
void
paddle2_left()
{
  int oldCol = futurePP2[0];
  int newCol = oldCol + paddleVelocity;
  if(newCol < (colLimits[1]-20)){
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


void ball_boundary(){
  int oldCol = nextPos[0];
  int newCol = oldCol + colVelocity;

  int oldRow = nextPos[1];
  int newRow = oldRow + rowVelocity;
  
  if(ball_paddle_collision()){
    rowVelocity = -rowVelocity;
  }
  if( ball_paddle2_collision()){
    rowVelocity = -rowVelocity;
  }
  if (newCol <= colLimits[0] || newCol >= colLimits[1]){
    colVelocity = -colVelocity;
  }
  //if (newRow <= rowLimits[0] || newRow >= rowLimits[1]){
    //rowVelocity = -rowVelocity;
    //  }
  
 
  newCol = oldCol + colVelocity;  // new col result

  newRow = oldRow  + rowVelocity; // new row result
  
  nextPos[0] = newCol;  
  nextPos[1] = newRow;
  
}

short redrawScreen = 1;
void wdt_c_handler()
{
  
  static int secCount = 0;

  secCount ++;
  if (secCount >= 25) {		/* 10/sec */
   
      /* move ball */
    ball_boundary();
      		
    if (switches & SW1) paddle1_left();
    if (switches & SW2) paddle1_right();
    if (switches & SW3) paddle2_right();
    if (switches & SW4) paddle2_left();

      secCount = 0;
      redrawScreen = 1; 
  }
}
  
void update_shape();

void main()
{
  
  P1DIR |= LED;		/**< Green led on when CPU on */
  P1OUT |= LED;
  configureClocks();
  lcd_init();
  switch_init();
  
  enableWDTInterrupts();      /**< enable periodic interrupt */
  or_sr(0x8);	              /**< GIE (enable interrupts) */
  
  clearScreen(COLOR_DARK_GREEN);
  while (1) {			/* forever */
    if (redrawScreen) {
      redrawScreen = 0;
      update_shape();
 
    }
    P1OUT &= ~LED;	/* led off */
    or_sr(0x10);	/**< CPU OFF */
    P1OUT |= LED;	/* led on */
  }
}


    
void
update_shape()
{
  screen_update_ball();
  screen_update_paddle1();
  screen_update_paddle2();
}
   


void
__interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & SWITCHES) {	      /* did a button cause this interrupt? */
    P2IFG &= ~SWITCHES;		      /* clear pending sw interrupts */
    switch_interrupt_handler();	/* single handler for all switches */
  }
}
