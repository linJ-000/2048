#ifndef SLIDE
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
#endif
