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

int paddlePos2[2] = {10, 3};
int futurePP2[2] = {11, 3};

int paddleVelocity = 2;

void
draw_ball(int col, int row, unsigned short color)
{
  
  fillRectangle(col-1, row-1, 6, 6, color);
}
int paddle_location[2] = {0,0};
void
draw_paddle(int col , int row, unsigned short color)
{

  paddle_location[0] = col - 1;
  paddle_location[1] = col + 20;
  
  fillRectangle(col-1, row-1, 20, 5, color);

}

char ball_paddle_collision()
{
  if((ballPos[0] >= paddle_location[0]) && (ballPos[0] <= paddle_location[1]))
    if( (ballPos[1] + 6) >= (screenHeight - 6) && (ballPos[1] <= (screenHeight-1)))
      return 1;
  return 0;
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
  int newCol = oldCol - colVelocity;
  if( newCol > (colLimits[0]))
    futurePP1[0] = newCol;
}
void
paddle1_right()
{
  int oldCol = futurePP1[0];
  int newCol = oldCol + colVelocity;
  if( newCol < (colLimits[1] - 20))
    futurePP1[0] = newCol;
}


void ball_boundary(){
  int oldCol = nextPos[0];
  int newCol = oldCol + colVelocity;

  int oldRow = nextPos[1];
  int newRow = oldRow + rowVelocity;
  
  if (newCol <= colLimits[0] || newCol >= colLimits[1]){
    colVelocity = -colVelocity;
  }
  if(ball_paddle_collision()){
    rowVelocity = -rowVelocity;
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
   
      /* move ball */
      secCount = 0;
      ball_boundary();
      redrawScreen = 1;
  }				
  if (switches & SW1) paddle1_left();
  if (switches & SW2) paddle1_right();
      
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
