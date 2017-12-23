#include "infrared.h"
#include "remote.h"
#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "lcd.h"
   

 int infrared(void)
 { 
	u8 key;

	key=Remote_Scan();	

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
 