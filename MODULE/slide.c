#include "slide.h"
#include "24cxx.h" 
#include "myiic.h"
#include "touch.h"
#include "lcd.h"

codin point = 
{
	0,
	{0},
	{0},
};

/* listen screen slide event */
/* return none:0; left:1; top:2; right:3; bottom:4 */
int slide(void)
{
	tp_dev.scan(0);
	if(tp_dev.sta&TP_PRES_DOWN)			//������������
	{	
		if(tp_dev.x[0]<lcddev.width&&tp_dev.y[0]<lcddev.height)
		{	
			if(point.t!=2)
			{
				point.x[point.t] = tp_dev.x[0];
				point.y[point.t] = tp_dev.y[0];
				point.t++;
			}else{
				point.x[1] = tp_dev.x[0];
				point.y[1] = tp_dev.y[0];
			}				
		}
	}else{													//������û������
		if(point.t==2)
		{
			point.t = 0;
			//���������жϻ�������
			if(s_abs(point.x[0], point.x[1]) >= 80 && s_abs(point.y[0], point.y[1]) <= 15 )
			{
				if(point.x[0] < point.x[1])
				{
					return 3;
				}else{
					return 1;
				}
			}else if(s_abs(point.y[0], point.y[1]) >= 80 && s_abs(point.x[0], point.x[1]) <= 15){
				if(point.y[0] < point.y[1])
				{
					return 4;
				}else{
					return 2;
				}
			}else{
				return 0;
			}
		}
		point.t = 0;
	}
	return 0;
}

/* ��������֮��ľ���ֵ */
u16 s_abs(u16 x1,u16 x2)
{			 
	if(x1>x2)return x1-x2;
	else return x2-x1;
}  