#ifndef ball_included
#define ball_included

extern int ballPos[2] , nextPos[2];
extern int colVelocity, rowVelocity;
extern int colLimits[2], rowLimits[2]; 

void draw_ball(int col, int row, unsigned short color);
void screen_update_ball();
void ball_boundary();


#endif // included
