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

int slideDir = 0;				//��������
u8 key;									//������ʾ
u16 game[4][4] = {{0,2,16,32},{4,256,512,2048},{4,16,8,32},{0,8,16,32}};
u16 score = 0;					//��Ϸ�÷�

#define START_TASK_PRIO		3   	//�������ȼ�
#define START_STK_SIZE 		640    //�����ջ��С
OS_TCB StartTaskTCB;    				//������ƿ�
CPU_STK START_TASK_STK[START_STK_SIZE];   //�����ջ	
void start_task(void *p_arg);   //������

#define KEY_TASK_PRIO		4			//�������ȼ�
#define KEY_STK_SIZE 		128		//�����ջ��С	
OS_TCB KeyTaskTCB;							//������ƿ�
CPU_STK KEY_TASK_STK[KEY_STK_SIZE];			//�����ջ	
void key_task(void *p_arg);		//������

#define LED1_TASK_PRIO		5			//�������ȼ�
#define LED1_STK_SIZE 		128		//�����ջ��С	
OS_TCB Led1TaskTCB;							//������ƿ�
CPU_STK LED1_TASK_STK[LED1_STK_SIZE];			//�����ջ	
void led1_task(void *p_arg);		//������

#define LCD1_TASK_PRIO		6			//�������ȼ�
#define LCD1_STK_SIZE		128			//�����ջ��С
OS_TCB	Lcd1TaskTCB;						//������ƿ�
__align(8) CPU_STK	LCD1_TASK_STK[LCD1_STK_SIZE];		//�����ջ
void lcd1_task(void *p_arg);		//������

//������ɨ������
#define SCAN_TASK_PRIO		7			//�������ȼ�
#define SCAN_STK_SIZE 		128		//�����ջ��С	
OS_TCB ScanTaskTCB;							//������ƿ�
CPU_STK SCAN_TASK_STK[SCAN_STK_SIZE];			//�����ջ	
void scan_task(void *p_arg);		//������

int main(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	
	delay_init();       //��ʱ��ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //�жϷ�������
	uart_init(115200);    //���ڲ���������
	LED_Init();         //LED��ʼ��
	LCD_Init();					//LCD��ʼ��
	KEY_Init();				//������ʼ��		 	
	tp_dev.init();			//��������ʼ��
	
	OSInit(&err);		//��ʼ��UCOSIII
	OS_CRITICAL_ENTER();//�����ٽ���
	//������ʼ����
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//������ƿ�
				 (CPU_CHAR	* )"start task", 		//��������
                 (OS_TASK_PTR )start_task, 			//������
                 (void		* )0,					//���ݸ��������Ĳ���
                 (OS_PRIO	  )START_TASK_PRIO,     //�������ȼ�
                 (CPU_STK   * )&START_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//�����ջ�����λ
                 (CPU_STK_SIZE)START_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	  )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	* )0,					//�û�����Ĵ洢��
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
                 (OS_ERR 	* )&err);				//��Ÿú�������ʱ�ķ���ֵ
	OS_CRITICAL_EXIT();	//�˳��ٽ���	 
	OSStart(&err);  //����UCOSIII
	while(1);
}

//��ʼ������
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//ͳ������                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//���ʹ���˲����жϹر�ʱ��
    CPU_IntDisMeasMaxCurReset();	
#endif
	
#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //��ʹ��ʱ��Ƭ��ת��ʱ��
	 //ʹ��ʱ��Ƭ��ת���ȹ���,ʱ��Ƭ����Ϊ1��ϵͳʱ�ӽ��ģ���1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		
	
	OS_CRITICAL_ENTER();	//�����ٽ���		
				 
	//����LED1����
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
	
	//������������
	OSTaskCreate((OS_TCB 	* )&KeyTaskTCB,		
				 (CPU_CHAR	* )"key task", 		
                 (OS_TASK_PTR )key_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )KEY_TASK_PRIO,     	
                 (CPU_STK   * )&KEY_TASK_STK[0],	
                 (CPU_STK_SIZE)KEY_STK_SIZE/10,	
                 (CPU_STK_SIZE)KEY_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);
								 
	//������Ⱦ��Ϸ��������
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
	
	//����������ɨ������
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
								 
	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//����ʼ����			 
	OS_CRITICAL_EXIT();	//�˳��ٽ���	
}

//led1������
void led1_task(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{
		LED1=~LED1;
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ500ms
	}
}

//�������񡪡����¿�ʼ
void key_task(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{
		key=KEY_Scan(0);
		if(key==KEY0_PRES)
		{
			//���¿�ʼ
			slideTest(0);
		}
		OSTimeDly(2, OS_OPT_TIME_HMSM_STRICT, &err);
	}
}

//������ɨ������
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
		}
		OSTimeDly(2, OS_OPT_TIME_HMSM_STRICT, &err);
	}
}

//��Ϸ������Ⱦ����
void lcd1_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	
	/* interface Init */
	OS_CRITICAL_ENTER();
	drawStage();
	drawInterface(game, score);
	OS_CRITICAL_EXIT();
	
	while(1)
	{
		OS_CRITICAL_ENTER();
		if(slideDir!=0)
		{
			slideTest(slideDir);
		}	
		slideDir = 0;
		OS_CRITICAL_EXIT();
		OSTimeDly(2, OS_OPT_TIME_HMSM_STRICT, &err);
	}
}
