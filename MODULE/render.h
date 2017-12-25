#ifndef RENDER
#define RENDER
#include "usart.h"
#include "delay.h"

void drawInterface(u16 array[4][4], u16 score);	//画游戏界面，4X4的方格
void drawStage(void);			//画不需要随游戏变动的部分，初始化时使用
void slideTest(int t);
u16 getColor(u16 num);		//不同的数字对应的不同背景色
void game_result(int i);
void start_page(void);
#endif
