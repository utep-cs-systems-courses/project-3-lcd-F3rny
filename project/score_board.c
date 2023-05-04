#include <msp430.h>
#include <libTimer.h>
#include <stdio.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "score_board.h"

int score1 = 0;
void
player1_score(){
  
  char p1[10];
  sprintf(p1 , "%d", score1);
  drawString8x12(10, 2 , "P1" , COLOR_BLACK, COLOR_BLUE);
  drawString8x12(15, 15 , p1 , COLOR_BLACK, COLOR_BLUE);
}
int score2 = 0;
void
player2_score(){
  
  char p2[10];
  sprintf(p2 , "%d", score2);
  drawString8x12(screenWidth-30, 2 , "P2" , COLOR_BLACK, COLOR_BLUE);
  drawString8x12(screenWidth-25, 15 , p2 , COLOR_BLACK, COLOR_BLUE);
}

