33333333333333333#ifndef SLIDE
#define SLIDE
#include "sys.h"
typedef struct
{
	int t;		//��ȡ��ĸ���
	u16 x[2];
	u16 y[2];
}codin;

u16 s_abs(u16 x1,u16 x2);
int slide(void);
int start(void);
#endif
