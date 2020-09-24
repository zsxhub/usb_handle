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
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�жϷ������� 
	delay_init();  //��ʱ��ʼ��
	uart_init(9600); //���� 1 ��ʼ��
	LED_Init(); //LED ��ʼ��
	Adc_Init();
	
	delay_ms(1800);
	USB_Port_Set(0); //USB �ȶϿ�
	delay_ms(300);
	USB_Port_Set(1); //USB �ٴ�����
	USB_Interrupts_Config(); //USB �ж�����
	Set_USBClock(); //USB ʱ������
	USB_Init();  //USB ��ʼ��
	
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
