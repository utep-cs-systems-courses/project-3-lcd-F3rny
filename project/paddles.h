#ifndef paddles_included
#define paddles_included

extern int paddlePos1[2], futurePP1[2];
extern int paddlePos2[2], futurePP2[2];
extern int paddleVelocity;

void draw_paddle(int col, int row, unsigned short color);
void screen_update_paddle1();
void screen_update_paddle2();

void paddle1_left();
void paddle1_right();
void paddle2_left();
void paddle2_right();

#endif // included
