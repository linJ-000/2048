#include "render.h"
#include "lcd.h"
#include "usart.h"
#include "delay.h"	

/* draw game interface */
void drawInterface(u16 array[4][4], u16 score)
{
	int i,j;
	u16 color;
	LCD_Fill(120,27,220,60,0xFFFF);
	LCD_ShowxNum(160, 30, score, 4, 24, 1);
	for(i=0; i<4; i++)
	{
		for(j=0; j<4; j++)
		{
			color = getColor(array[i][j]);
			LCD_Fill(5+6*(j+1)+50*j, 85+6*(i+1)+50*i, 5+6*(j+1)+50*(j+1), 85+6*(i+1)+50*(i+1), color);
			POINT_COLOR = BLACK;
			if (array[i][j]!=0)
			{
				if(array[i][j]<10)
				{
					LCD_ShowxNum(5+6*(j+1)+50*j+8, 85+6*(i+1)+50*i+13, array[i][j], 2, 24, 1);
				}
				else if(array[i][j]<100)
				{
					LCD_ShowxNum(5+6*(j+1)+50*j+2, 85+6*(i+1)+50*i+13, array[i][j], 3, 24, 1);
				}
				else if(array[i][j]<1000)
				{
					LCD_ShowxNum(5+6*(j+1)+50*j+8, 85+6*(i+1)+50*i+13, array[i][j], 3, 24, 1);
				}
				else
				{
					LCD_ShowxNum(5+6*(j+1)+50*j+1, 85+6*(i+1)+50*i+13, array[i][j], 4, 24, 1);
			  }
			}
		}
	}
}

/* get color for num */
u16 getColor(u16 num)
{
	u16 color;
	switch(num){
		case 0:   color=0xFFFF;break;
		case 2:   color=0xFFE0;break;
		case 4:   color=0xCFFF;break;
		case 8:   color=0x2EEC;break;
		case 16:  color=0x07FF;break;
		case 32:  color=0xFDEF;break;
		case 64:  color=0xF81F;break;
		case 128: color=0x07E0;break;
		case 256: color=0x7D7C;break;
		case 512: color=0xFBE0;break;
		case 1024: color=0xFA8B;break;
		case 2048:color=0xF807;break;
	}
	return color;
}

/*draw stage*/
void drawStage()
{
	LCD_Clear(WHITE);
	LCD_Fill(5, 85, 235, 315, GRAY);
	POINT_COLOR = BLACK;
	LCD_ShowNum(5,5, 2048, 4, 24);
	POINT_COLOR = GRAY;
	LCD_ShowString(160, 5, 80, 40, 24, "Score");
	
}

/* slide test */
void slideTest(int t)
{	
	LCD_Fill(0,40,100,80,WHITE);
	POINT_COLOR = BLACK;
	LCD_ShowNum(0,40, t, 1, 16);
}
void game_result(int i)
{
	if(i==1)
	{
		LCD_ShowString(10,60,100,30,24,"You win.");
	}
	else if(i==2)
	{
		LCD_ShowString(10,60,100,30,24,"You lose.");
	}
	else
	{
		LCD_Fill(0,60,120,83,0xFFFF);
	}	
}
void start_page()
{
	LCD_Fill(0, 0, 240, 360, 0xFFE0);
	LCD_ShowxNum(100,100, 2048, 4, 24,1);
	LCD_ShowString(45,126,195,152,24,"Slide screen!");
	LCD_ShowString(70,154,170,180,24,"GET 2048!");
	LCD_Fill(70, 250, 170, 290, 0x7D7C);
	LCD_ShowString(94,258,146,282,24,"Start");
}
