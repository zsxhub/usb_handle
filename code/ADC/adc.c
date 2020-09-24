 #include "adc.h"
 #include "delay.h"
 #include "stm32f10x_adc.h"
 #include "hw_config.h"
 #include "usart.h"
 #include "led.h"
////////////////////////////////// 
	   
		   
//��ʼ��ADC
//�������ǽ��Թ���ͨ��Ϊ��
//����Ĭ�Ͻ�����ͨ��0~3																	   
void  Adc_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOB | RCC_APB2Periph_ADC1	, ENABLE );	  //ʹ��ADC1ͨ��ʱ��
 

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M

	//PA1 ��Ϊģ��ͨ����������                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_0|GPIO_Pin_5|GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	ADC_DeInit(ADC1);  //��λADC1,������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   

  
	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1
	
	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	
	ADC_StartCalibration(ADC1);	 //����ADУ׼
 
	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����
 
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������

}				  
//���ADCֵ
//ch:ͨ��ֵ 0~3
u16 Get_Adc(u8 ch)   
{
  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
}

u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(1);
	}
	return temp_val/times;
} 	 

void	USB_Send(void)
{
	static u8 key;
	static u8 send;
	static u8 change_flag=0,use_flag=0;
	u16 contral_JK,direction_WS,direction_AD,contral_NM;
	u16 KEY_L,KEY_R;
	u8 key_date[8]={0,0,0,0,0,0,0,0};
	
	KEY_L = Get_Adc_Average(ADC_Channel_5,5);//��ఴ��
	KEY_R = Get_Adc_Average(ADC_Channel_7,5);//�Ҳఴ��
	///////////////////////////////////////////////////////////////
	if(KEY_L<500)  				//��1
	{
		change_flag = 0;
		LED3 = 0 ;
		LED5 = 1 ;
	}
	else if(KEY_L>2400&&KEY_L<3500)	//��3
	{
		use_flag = 0;
		LED1 = 1 ;
	}
	else if(KEY_L>1500&&KEY_L<2400)								//��2
	{
		use_flag = 1;
		LED1 = 0 ;
	}
	///////////////////////////////////////////////////////////////
	if(KEY_R<500)					//��1
	{
		change_flag = 1;
		LED3 = 1 ;
		LED5 = 0 ;
	}
	else if(KEY_R>2400&&KEY_R<3500)	//��3
	{
	}
	else if(KEY_R>1500&&KEY_R<2400)									//��2
	{
	}	
	
	///////////////////////////////////////////////////////////////
	if(change_flag == 1)
	{
		if(use_flag==1)
			contral_JK = Get_Adc_Average(ADC_Channel_0,5);	//	 j k	contral_JK
		else
			contral_JK = 2000;	
		direction_WS = Get_Adc_Average(ADC_Channel_9,5);	//	 w s	direction_WS
		direction_AD = Get_Adc_Average(ADC_Channel_8,5);	//	 D A	direction_AD
		contral_NM   = Get_Adc_Average(ADC_Channel_1,5);	//	 n m	contral_NM
	}
	else if(change_flag == 0)
	{
		contral_JK   = Get_Adc_Average(ADC_Channel_9,5);	//	 j k	contral_JK   
		if(use_flag==1)
			direction_WS = Get_Adc_Average(ADC_Channel_0,5);	//	 w s	direction_WS
		else
			direction_WS = 2000;
		direction_AD = Get_Adc_Average(ADC_Channel_1,5);	//	 D A	direction_AD
		contral_NM   = Get_Adc_Average(ADC_Channel_8,5);	//	 n m	contral_NM	
	}
	///////////////////////////////////////////////////////////////
	//����
	if(contral_NM>3100)
	{
		key=1;
		key_date[3]=0x11;// ���� n
	}
	else	if(contral_NM<1000)
	{
		key_date[3]=0x10;// ���� m
		key=1;
	}
	///////////////////////////////////////////////////////////////
	if(contral_JK>3500)
	{
		key=1;
		key_date[3]=0x0D;// ���� J
	}
	else	if(contral_JK<600)
	{
		key_date[3]=0x0E;// ���� K
		key=1;
	}
	///////////////////////////////////////////////////////////////
	//����
	if(direction_WS>3500)
	{
		key_date[4]=0x1A; // ���� w
		key=1;
	}
	else if(direction_WS<600)
	{
		key_date[4]=0x16;// ���� s
		key=1;
	}
	///////////////////////////////////////////////////////////////
	//����
		if(direction_AD<1000)
	{
		key_date[5]=0x07;// ���� D
		key=1;
	}
	else	if(direction_AD>3100)
	{
		key_date[5]=0x04; // ���� A
		key=1;

	}	
	///////////////////////////////////////////////////////////////
		if(key)
		{ 	
			send=1;
				Joystick1_Send(key_date);
		}	
		
		else if(send)
		{
			send=0;
			key_date[3]=0;
			key_date[4]=0;
			key_date[5]=0;	
			Joystick1_Send(key_date);
		}	
}

























