#ifndef LOGIC
#define LOGIC
#include "usart.h"
#include "delay.h"
extern u16 game[4][4];
extern u16 sum_score;

void initialize(void);
u16 put_random(void);
void move_right(void);
void move_left(void);
void move_down(void);
void move_up(void);
u16 test_16(void);
u16 test_right(void);
u16 test_left(void);
u16 test_up(void);
u16 test_down(void);
u16 lost_game(void);
u16 win_game(void);
int moveAndJudge(int x);
#endif
