#include "render.h"
#include "lcd.h"
#include "usart.h"
#include "delay.h"	

/* draw game interface */
void drawInterface(u16 array[4][4], u16 score)
{
	int i,j;
	u16 color;
	
	LCD_ShowxNum(160, 30, score, 4, 24, 1);
	for(i=0; i<4; i++)
	{
		for(j=0; j<4; j++)
		{
			color = getColor(array[i][j]);
			LCD_Fill(5+6*(j+1)+50*j, 85+6*(i+1)+50*i, 5+6*(j+1)+50*(j+1), 85+6*(i+1)+50*(i+1), color);
			POINT_COLOR = BLACK;
			LCD_ShowxNum(5+6*(j+1)+50*j, 85+6*(i+1)+50*i, array[i][j], 4, 24, 1);
		}
	}
}

/* get color for num */
u16 getColor(u16 num)
{
	u16 color;
	switch(num){
		case 0:   color=0x780F;break;
		case 2:   color=0x03E0;break;
		case 4:   color=0x7800;break;
		case 8:   color=0x7BE0;break;
		case 16:  color=0x7BEF;break;
		case 32:  color=0x07E0;break;
		case 64:  color=0xF81F;break;
		case 128: color=0xFFE0;break;
		case 256: color=0xC618;break;
		case 512: color=0x780F;break;
		case 2048:color=0xF74F;break;
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
