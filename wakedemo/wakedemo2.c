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
short ballPos[2] = {screenWidth-15, screenHeight-20}, nextPos[2] = {screenWidth-14, screenHeight-20};
short colVelocity = -4, rowVelocity = -5;
short colLimits[2] = {0 , screenWidth};
short rowLimits[2] = {0 , screenHeight}; 

// paddles
short paddlePos1[2] = {(screenWidth/2) + 35, screenHeight-6};
short futurePP1[2] = {(screenWidth/2) + 36, screenHeight-6};

short paddlePos2[2] = {10, 3};
short futurePP2[2] = {11, 3};

short paddleVelocity = 2;

void
draw_ball(int col, int row, unsigned short color)
{
  
  fillRectangle(col-1, row-1, 6, 6, color);
}

void
draw_paddle(int col , int row, u_int color){

  fillRectangle(col-1, row-1, 20, 5, color);

}

void
screen_update_paddle1(){

  draw_paddle(paddlePos1[0], paddlePos1[1], COLOR_BLUE); 

  paddlePos1[1] = futurePP1[1];

  draw_paddle(paddlePos1[0], paddlePos1[1], COLOR_WHITE);


}
void
screen_update_paddle2(){

  draw_paddle(paddlePos2[0], paddlePos2[1], COLOR_BLUE); 

  paddlePos2[1] = futurePP2[1];

  draw_paddle(paddlePos2[0], paddlePos2[1], COLOR_WHITE);


}

void
screen_update_ball()
{
  for (char axis = 0; axis < 2; axis ++)

    if (ballPos[axis] != nextPos[axis]) /* position changed? */

      goto redraw;

  return;/* nothing to do */

 redraw:

  draw_ball(ballPos[0], ballPos[1], COLOR_BLUE); /* erase */

  for (char axis = 0; axis < 2; axis ++)

    ballPos[axis] = nextPos[axis];

  draw_ball(ballPos[0], ballPos[1], COLOR_WHITE); /* draw */
}

void ball_boundary(){
  short oldCol = nextPos[0];
  short newCol = oldCol + colVelocity;

  short oldRow = nextPos[1];
  short newRow = oldRow + rowVelocity;
  
  if (newCol <= colLimits[0] || newCol >= colLimits[1]){
    colVelocity = -colVelocity;
  }
  // check collision with paddle 1
  if (ballPos[1] >= paddlePos1[1] && ballPos[1] <= paddlePos1[1] + 5) {

    if (ballPos[0] >= paddlePos1[0] - 5 && ballPos[0] <= paddlePos1[0] + 10) {

      colVelocity = -colVelocity;

      rowVelocity = -rowVelocity;

    }

  }



  // Check collision with paddle 2

  if (ballPos[1] <= paddlePos2[1] + 5 && ballPos[1] >= paddlePos2[1]) {

    if (ballPos[0] >= paddlePos2[0] - 5 && ballPos[0] <= paddlePos2[0] + 10) {

      colVelocity = -colVelocity;

      rowVelocity = -rowVelocity;

    }
  }

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
   
    {
      /* move ball */
      //detects_collision();
      ball_boundary();
      
    }

    {				
      if (switches & SW2) 
      if (switches & SW1) 
      if (step <= 30)
	step ++;
      else
	step = 0;
      secCount = 0;
    }
    if (switches & SW4) return;
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
