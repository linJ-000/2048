#include "led.h"
#include "lcd.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "key.h"  
#include "24cxx.h" 
#include "myiic.h"
#include "touch.h" 
#include "remote.h"
#include "includes.h"

#include "render.h"
#include "slide.h"
#include "logic.h"
#include "infrared.h"

int slideDir = 0;				//滑动方向
u8 key;									//按键标示
u16 Lgame[4][4] = {{0,2,16,32},{4,256,512,2048},{4,16,8,32},{0,8,16,32}};

///////事件标志组///////
#define SCAN_FLAG 0x01
#define REMOTE_FLAG 0x01
OS_FLAG_GRP PlayFlags;

#define START_TASK_PRIO		3   	//任务优先级
#define START_STK_SIZE 		640    //任务堆栈大小
OS_TCB StartTaskTCB;    				//任务控制块
CPU_STK START_TASK_STK[START_STK_SIZE];   //任务堆栈	
void start_task(void *p_arg);   //任务函数

#define LED1_TASK_PRIO		5			//任务优先级
#define LED1_STK_SIZE 		128		//任务堆栈大小	
OS_TCB Led1TaskTCB;							//任务控制块
CPU_STK LED1_TASK_STK[LED1_STK_SIZE];			//任务堆栈	
void led1_task(void *p_arg);		//任务函数

//渲染任务
#define LCD1_TASK_PRIO		7			//任务优先级
#define LCD1_STK_SIZE		128			//任务堆栈大小
OS_TCB	Lcd1TaskTCB;						//任务控制块
__align(8) CPU_STK	LCD1_TASK_STK[LCD1_STK_SIZE];		//任务堆栈
void lcd1_task(void *p_arg);		//任务函数

//触摸屏扫描任务
#define SCAN_TASK_PRIO		8			//任务优先级
#define SCAN_STK_SIZE 		128		//任务堆栈大小	
OS_TCB ScanTaskTCB;							//任务控制块
CPU_STK SCAN_TASK_STK[SCAN_STK_SIZE];			//任务堆栈	
void scan_task(void *p_arg);		//任务函数

//红外遥控器任务
#define REMOTE_TASK_PRIO		8			//任务优先级
#define REMOTE_STK_SIZE 		128		//任务堆栈大小	
OS_TCB RemoteTaskTCB;							//任务控制块
CPU_STK REMOTE_TASK_STK[REMOTE_STK_SIZE];			//任务堆栈	
void remote_task(void *p_arg);		//任务函数

int main(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	
	delay_init();       //延时初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //中断分组配置
	uart_init(115200);    //串口波特率设置
	LED_Init();         //LED初始化
	LCD_Init();					//LCD初始化
	KEY_Init();				//按键初始化		 	
	tp_dev.init();			//触摸屏初始化
	Remote_Init();			//红外接收初始化	
	
	OSInit(&err);		//初始化UCOSIII
	OS_CRITICAL_ENTER();//进入临界区
	//创建开始任务
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//任务控制块
				 (CPU_CHAR	* )"start task", 		//任务名字
                 (OS_TASK_PTR )start_task, 			//任务函数
                 (void		* )0,					//传递给任务函数的参数
                 (OS_PRIO	  )START_TASK_PRIO,     //任务优先级
                 (CPU_STK   * )&START_TASK_STK[0],	//任务堆栈基地址
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//任务堆栈深度限位
                 (CPU_STK_SIZE)START_STK_SIZE,		//任务堆栈大小
                 (OS_MSG_QTY  )0,					//任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
                 (OS_TICK	  )0,					//当使能时间片轮转时的时间片长度，为0时为默认长度，
                 (void   	* )0,					//用户补充的存储区
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
                 (OS_ERR 	* )&err);				//存放该函数错误时的返回值
	OS_CRITICAL_EXIT();	//退出临界区	 
	OSStart(&err);  //开启UCOSIII
	while(1);
}

//开始任务函数
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//统计任务                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();	
#endif
	
#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
	 //使能时间片轮转调度功能,时间片长度为1个系统时钟节拍，既1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		
	
	OS_CRITICAL_ENTER();	//进入临界区		
	
	//创建LED1任务
	OSTaskCreate((OS_TCB 	* )&Led1TaskTCB,		
				 (CPU_CHAR	* )"led1 task", 		
                 (OS_TASK_PTR )led1_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )LED1_TASK_PRIO,     	
                 (CPU_STK   * )&LED1_TASK_STK[0],	
                 (CPU_STK_SIZE)LED1_STK_SIZE/10,	
                 (CPU_STK_SIZE)LED1_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);	
								 
	//创建游戏任务
	OSTaskCreate((OS_TCB 	* )&Lcd1TaskTCB,		
				 (CPU_CHAR	* )"lcd1 task", 		
                 (OS_TASK_PTR )lcd1_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )LCD1_TASK_PRIO,     	
                 (CPU_STK   * )&LCD1_TASK_STK[0],	
                 (CPU_STK_SIZE)LCD1_STK_SIZE/10,	
                 (CPU_STK_SIZE)LCD1_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);
	
	//创建触摸屏扫描任务
	OSTaskCreate((OS_TCB 	* )&ScanTaskTCB,		
				 (CPU_CHAR	* )"scan task", 		
                 (OS_TASK_PTR )scan_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )SCAN_TASK_PRIO,     	
                 (CPU_STK   * )&SCAN_TASK_STK[0],	
                 (CPU_STK_SIZE)SCAN_STK_SIZE/10,	
                 (CPU_STK_SIZE)SCAN_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);
								 
	//创建红外遥控器任务
	OSTaskCreate((OS_TCB 	* )&RemoteTaskTCB,		
				 (CPU_CHAR	* )"remote task", 		
                 (OS_TASK_PTR )remote_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )REMOTE_TASK_PRIO,     	
                 (CPU_STK   * )&REMOTE_TASK_STK[0],	
                 (CPU_STK_SIZE)REMOTE_STK_SIZE/10,	
                 (CPU_STK_SIZE)REMOTE_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);
								 
	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//挂起开始任务			 
	OS_CRITICAL_EXIT();	//退出临界区	
}

//led1任务函数
void led1_task(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{
		LED1=~LED1;
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //延时500ms
	}
}

//触摸屏扫描任务
void scan_task(void *p_arg)
{
	int sli;
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	
	while(1)
	{
		sli=slide();
		if(sli!= 0)
		{		
			OS_CRITICAL_ENTER();
			slideDir = sli;
			OS_CRITICAL_EXIT();
			OS_FlagPost(&PlayFlags, SCAN_FLAG, OS_OPT_POST_FLAG_SET, 0, &err);	
		}
		OSTimeDly(20, OS_OPT_TIME_HMSM_STRICT, &err);
	}
}

//红外遥控器任务
void remote_task(void *p_arg)
{
	int rem;
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	
	while(1)
	{
		rem=infrared();
		if(rem!= 0)
		{	
			OS_CRITICAL_ENTER();
			slideDir = rem;
			printf("rem=%d\n", rem);
			OS_CRITICAL_EXIT();
			OS_FlagPost(&PlayFlags, REMOTE_FLAG, OS_OPT_POST_FLAG_SET, 0, &err);
			OSTaskSuspend(&RemoteTaskTCB, &err);
		}
		OSTimeDly(80, OS_OPT_TIME_HMSM_STRICT, &err);
	}
}

//游戏任务
void lcd1_task(void *p_arg)
{
	int res;
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	
	start_page();
	while(1)
	{
		if(start())
			break;
		OSTimeDly(20, OS_OPT_TIME_HMSM_STRICT, &err);
	}
	
	/* Init */
	OSFlagCreate(&PlayFlags, "Play Flags", 0, &err);
	OS_CRITICAL_ENTER();
	initialize();
	drawStage();
	drawInterface(game, sum_score);
	OS_CRITICAL_EXIT();
	
	while(1)
	{
		//等待事件标志组
		OSFlagPend(&PlayFlags, SCAN_FLAG|REMOTE_FLAG, 0, OS_OPT_PEND_FLAG_SET_ANY|OS_OPT_PEND_FLAG_CONSUME, 0, &err);

		OS_CRITICAL_ENTER();
		res = moveAndJudge(slideDir);
		game_result(res);
		drawInterface(game, sum_score);
		slideDir = 0;
		OS_CRITICAL_EXIT();
		printf("move\n");

		OSTimeDly(80, OS_OPT_TIME_HMSM_STRICT, &err);
		OSTaskResume(&RemoteTaskTCB, &err);
	}
}
