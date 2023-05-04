#include <msp430.h>
#include <libTimer.h>
#include <stdio.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "ball.h"
#include "paddles.h"
#include "tennis.h"
#include "collisions.h"
#include "score_board.h"
#include "buzzer.h"
#include "sound_effect.h"

#define LED BIT6/* note that bit zero req'd for display */

#define SW1 1
#define SW2 2
#define SW3 4
#define SW4 8

#define SWITCHES 15

unsigned char step = 0;
int state = 0;
char button_pressed = 0;

static char
switch_update_interrupt_sense()
{
  char p2val = P2IN;

  /* update switch interrupt to detect changes from current buttons */

  P2IES |= (p2val & SWITCHES);/* if switch up, sense down */

  P2IES &= (p2val | ~SWITCHES);/* if switch down, sense up */

  return p2val;

}



void

switch_init()/* setup switch */

{

  P2REN |= SWITCHES;/* enables resistors for switches */

  P2IE |= SWITCHES;/* enable interrupts from switches */

  P2OUT |= SWITCHES;/* pull-ups for switches */

  P2DIR &= ~SWITCHES;/* set switches' bits for input */

  switch_update_interrupt_sense();

}
void state_advance();

int switches = 0;

void

switch_interrupt_handler()

{

  char p2val = switch_update_interrupt_sense();

  switches = ~p2val & SWITCHES;

  if( !(P2IN & SW1)){
    button_pressed ++;
    if(button_pressed == 1 && state == 0){
      state = 1;
      // button_pressed = 1;
    }
    if(button_pressed >= 1 && state == 2) {
      paddle1_left();
    }
  }
  
  else if( !(P2IN & SW2)){
      if(button_pressed >= 1){
	paddle1_right();
      }
    }
      
  else if( !(P2IN & SW3)){
      if(button_pressed >= 1){
	paddle2_right();
      }
  }
  else if( !(P2IN & SW4)){
      if(button_pressed >= 1){
	paddle2_left();
      
      }
  }
  else{
    
  }
}


void update_shape();
void
state_advance(){
  switch(state){
  case 0:
    
    tennisCourt();
    drawString5x7(centerCol-20, (centerRow/2)-25, "TENNIS", COLOR_NAVY, COLOR_DARK_GREEN); //Title Card
    drawString5x7(centerCol-15, (centerRow/2)-15, "GAME", COLOR_NAVY, COLOR_DARK_GREEN); //Title Card
    drawString5x7((centerCol-15), (centerRow+45), "START", COLOR_RED, COLOR_BLACK);
   
   button_pressed = 0;
   break;
  case 1:
    clearScreen(COLOR_DARK_GREEN);
    state = 2;
    break;
  case 2:
    border();
    mainCourt();
    
    update_shape();
    
    break;
    
  } 
}

void
reset_game(){

  ballPos[0] = screenWidth-15; 
  ballPos[1] = screenHeight-20;
  nextPos[0] = screenWidth-14;
  nextPos[1] = screenHeight-20;

    
  paddlePos1[0] = (screenWidth/2) + 30;
  paddlePos1[1] = screenHeight-8;
  futurePP1[0] = (screenWidth/2) + 31;
  futurePP1[1] = screenHeight-8;
    
  paddlePos2[0] = 15;
  paddlePos2[1] = 35;

  futurePP2[0] = 16;
  futurePP2[1] = 35;

  colVelocity = -4;
  rowVelocity = -5;

}


short redrawScreen = 1;

void wdt_c_handler()    
{
 
  static int secCount = 0;

  secCount ++;
  if (secCount >= 25) {		/* 10/sec */
   
      /* move ball */
   ball_boundary();
   
   secCount = 0;
   redrawScreen = 1; 
  }
}

void
main()
{
  
  P1DIR |= LED;		/**< Green led on when CPU on */
  P1OUT |= LED;
  configureClocks();
  buzzer_init();
  lcd_init();
  switch_init();
  
  enableWDTInterrupts();      /**< enable periodic interrupt */
  or_sr(0x8);	              /**< GIE (enable interrupts) */
  
  clearScreen(COLOR_DARK_GREEN);
  
  while (1) {			/* forever */
    if (redrawScreen) {
      redrawScreen = 0;
      state_advance();
    }
    P1OUT &= ~LED;	/* led off */
    or_sr(0x10);	/**< CPU OFF */
    P1OUT |= LED;	/* led on */
  }
  
}
void

update_shape()

{
  int ballLeft = ballPos[0] - 1;
  int ballRight = ballPos[0] + 6;
  int ballTop = ballPos[1] - 1;
  int ballBottom = ballPos[1] + 6;
  if(ballTop <= rowLimits[0] || ballBottom >= rowLimits[1] || ballLeft < colLimits[0]-1 || ballRight > colLimits[1] + 2 ){
    if(ballTop <= rowLimits[0]){
      score1++;
      lost();
      player1_score();
    }
    if(ballBottom >= rowLimits[1]){
      score2++;
      lost();
      player2_score();
    }
    
    draw_paddle(paddlePos1[0], paddlePos1[1], COLOR_DARK_GREEN);
    draw_paddle(paddlePos2[0], paddlePos2[1], COLOR_DARK_GREEN);
    
    reset_game();
  }
  
  if(button_pressed > 0){
    screen_update_ball();

    screen_update_paddle1();

    screen_update_paddle2();

    player1_score();
    player2_score();
  }
  
}
void
__interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & SWITCHES) {      /* did a button cause this interrupt? */
    P2IFG &= ~SWITCHES;      /* clear pending sw interrupts */
    switch_interrupt_handler();/* single handler for all switches */
  }

}

