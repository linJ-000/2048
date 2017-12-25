#include "logic.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "includes.h"

u16 game[4][4];
u16 sum_score;

void initialize()
{
	u16 i, j;
	sum_score=0;
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			game[i][j] = 0;
		}
	}
	put_random();
	put_random();
	return;
}

u16 put_random()
{
	u16 N, n, ran, ran4;
	u16 i, j;
	OS_ERR err;
	N = 0;
	n = 0;
	srand(OSTimeGet(&err));

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			u16 a = game[i][j];
			if (!a)
				N++;
		}
	}
	if (!N)
		return 0;
	ran = rand() % N + 1;
	ran4 = rand() % 4;
	//ran4 = rand() % 10;
	//ran8 = rand() % 10;
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			u16 a = game[i][j];
			if (!a)
				n++;
			if (n == ran)
			{
				if (ran4)
					game[i][j] = 2;
				if (!ran4)
					game[i][j] = 4;
				return 1;
			}
		}
	}
	return 1;
}
void move_left()
{
	u16 shu, next_shu;
	int i, j, k;
	if (test_left())
	{
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 3; j++)
			{
				shu = game[i][j];
				k = j + 1;
				for (; k < 4; k++)
				{						
					next_shu = game[i][k];
					if (shu&&next_shu)
					{		
							if (shu == next_shu)
							{
								game[i][j] = shu + next_shu;
								game[i][k] = 0;
								sum_score += shu * 2;
							}
							break;
					}
					if (!shu)
					{
						if (next_shu)
						{
							game[i][j] = next_shu;
							game[i][k] = 0;
							j--;
							break;
						}
					}
					
				}
			}
		}
		put_random();
	}
}
void move_right()
{
	int i, j, k;
	u16 shu, next_shu;
	if (test_right())
	{
		for (i = 0; i < 4; i++)
		{
			for (j = 3; j > 0; j--)
			{
				shu = game[i][j];
				k = j - 1;
				for (; k >= 0; k--)
				{
					next_shu = game[i][k];
					if (shu&&next_shu)
					{
						if (shu == next_shu)
						{
							game[i][j] = shu + next_shu;
							game[i][k] = 0;
							sum_score += shu * 2;
						}
						break;
					}
					if (!shu)
					{
						if (next_shu)
						{
							game[i][j] = next_shu;
							game[i][k] = 0;
							j++;
							break;
						}
					}
				}
			}
		}
		put_random();
	}
}
void move_up()
{
	u16 shu, next_shu;
	int	i, j, k;

	if (test_up())
	{
		for (j = 0; j < 4; j++)
		{
			for (i = 0; i < 3; i++)
			{
				shu = game[i][j];
				k = i + 1;
				for (; k < 4; k++)
				{
					next_shu = game[k][j];
						if (shu&&next_shu)
						{
							if (shu == next_shu)
							{
								game[i][j] = shu + next_shu;
								game[k][j] = 0;
								sum_score += shu * 2;
							}
							break;
						}
						if (!shu)
						{
							if (next_shu)
							{
								game[i][j] = next_shu;
								game[k][j] = 0;
								i--;
								break;
							}
						}
					}
			}
		}
		put_random();
	}
}
void move_down()
{
	u16 shu, next_shu;
	int i, j, k;
	if (test_down())
	{
		for (j = 0; j < 4; j++)
		{
			for (i = 3; i > 0; i--)
			{
				shu = game[i][j];
				k = i - 1;
				for (; k >= 0; k--)
				{
					next_shu = game[k][j];
					if (shu&&next_shu)
					{
						if (shu == next_shu)
						{
							game[i][j] = shu + next_shu;
							game[k][j] = 0;
							sum_score += shu * 2;
						}
						break;
					}
					if (!shu)
					{
						if (next_shu)
						{
							game[i][j] = next_shu;
							game[k][j] = 0;
							i++;
							break;
						}
					}
				}
			}
		}
		put_random();
	}
}
u16 test_right()
{
	int i = 0, j;
	for (; i < 4; i++)
	{
		for (j = 0; j < 3; j++)
		{
			if (game[i][j]!=0&&(game[i][j] == game[i][j + 1] || game[i][j + 1] == 0))
				return 1;
		}
	}
	return 0;
}
u16 test_left()
{
	int i = 0, j;
	for (; i < 4; i++)
	{
		for (j = 3; j > 0; j--)
		{
			if (game[i][j] != 0 && (game[i][j] == game[i][j - 1] || game[i][j - 1] == 0))
				return 1;
		}
	}
	return 0;
}
u16 test_up()
{

	int i, j;
	for (j = 0; j < 4; j++)
	{
		for (i = 3; i >0; i--)
		{
			if (game[i][j] != 0 && (game[i][j] == game[i - 1][j] || game[i - 1][j] == 0))
				return 1;
		}
	}
	return 0;
}
u16 test_down()
{
	int i , j;
	for (j = 0; j < 4; j++)
	{
		for (i = 0; i < 3; i++)
		{
			if (game[i][j] != 0 && (game[i][j] == game[i+1][j] || game[i+1][j] == 0))
				return 1;
		}
	}
	return 0;
}
u16 test_16()
{
	u16 N = 0;
	int i, j;
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			if (game[i][j])
				N++;
		}
	}
	if (N == 16)
		return 1;
	else
		return 0;
}
u16 lost_game()
{
	int i, j;
	if (test_16())
	{
		for (i = 0; i < 4; i++)
		{
			for ( j = 0; j < 3; j++)
			{
				if (game[i][j] == game[i][j + 1] || game[j][i] == game[j + 1][i])
				{
					return 0;
				}
			}
		}
		return 1;
	}
	return 0;
}
u16 win_game()
{
	int i, j;
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			if (game[i][j] == 2048)
				return 1;
		}
	}
	return 0;
}
/*win:1; lose:2; else:0*/
int moveAndJudge(int x)
{
	switch(x){
		case 1:move_left();break;
		case 2:move_up();break;
		case 3:move_right();break;
		case 4:move_down();break;
	}
	if(win_game())
		return 1;
	else if(lost_game())
		return 2;
	return 0;
}
