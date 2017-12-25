#ifndef RENDER
#define RENDER
#include "usart.h"
#include "delay.h"

void drawInterface(u16 array[4][4], u16 score);	//����Ϸ���棬4X4�ķ���
void drawStage(void);			//������Ҫ����Ϸ�䶯�Ĳ��֣���ʼ��ʱʹ��
void slideTest(int t);
u16 getColor(u16 num);		//��ͬ�����ֶ�Ӧ�Ĳ�ͬ����ɫ
void game_result(int i);
void start_page(void);
#endif
