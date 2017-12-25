#include "infrared.h"
#include "remote.h"
#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "lcd.h"
#include "includes.h"
   
u8 preKey;	//上一次按下的键值
OS_TICK lastTime;	//上一次按下按键的时间
OS_TICK time;
 int infrared(void)
 { 
	u8 key;
	OS_ERR err;
	int diff;
	key=Remote_Scan();
	 
	if(key==48 || key==152 || key==122 || key==56)
	{			
		time = OSTimeGet(&err);
		if(!lastTime)
			lastTime = time;
		if(!preKey)
			preKey = key;
		diff = time - lastTime;
		if(preKey == key && diff < 180)
		{
			preKey = key;
			lastTime = time;
			return 0;
		}
		preKey = key;
		lastTime = time;
	}
	 
	 
	 if(key==48)	
		{
		 return 1;
		}
		else if(key==152)
		{
		 return 2;
		}
		else if(key==122)
		{
		 return 3;
		}
		else if (key==56)
		{
		 return 4;
		}
		else
		{
		 return 0;
		}
		
	}	
 