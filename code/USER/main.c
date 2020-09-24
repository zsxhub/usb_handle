#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h" 
 #include "adc.h"
  #include "stm32f10x_adc.h"
int main(void)
{ 
  u8 i=0;
	u16 adcx,adcx2;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断分组设置 
	delay_init();  //延时初始化
	uart_init(9600); //串口 1 初始化
	LED_Init(); //LED 初始化
	Adc_Init();
	
	delay_ms(1800);
	USB_Port_Set(0); //USB 先断开
	delay_ms(300);
	USB_Port_Set(1); //USB 再次连接
	USB_Interrupts_Config(); //USB 中断配置
	Set_USBClock(); //USB 时钟设置
	USB_Init();  //USB 初始化
	
	while(1)
	{ 
		USB_Send();
		if(bDeviceState==CONFIGURED)
		{
			if(i==200) 
			{ 
				i=0; 
				LED4=!LED4; 
			}
		}
		else 
			LED4=0;
		i++;

	} 
} 
