 #include "adc.h"
 #include "delay.h"
 #include "stm32f10x_adc.h"
 #include "hw_config.h"
 #include "usart.h"
 #include "led.h"
////////////////////////////////// 
	   
		   
//初始化ADC
//这里我们仅以规则通道为例
//我们默认将开启通道0~3																	   
void  Adc_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOB | RCC_APB2Periph_ADC1	, ENABLE );	  //使能ADC1通道时钟
 

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M

	//PA1 作为模拟通道输入引脚                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_0|GPIO_Pin_5|GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	ADC_DeInit(ADC1);  //复位ADC1,将外设 ADC1 的全部寄存器重设为缺省值
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   

  
	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1
	
	ADC_ResetCalibration(ADC1);	//使能复位校准  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
	
	ADC_StartCalibration(ADC1);	 //开启AD校准
 
	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束
 
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能

}				  
//获得ADC值
//ch:通道值 0~3
u16 Get_Adc(u8 ch)   
{
  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
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
	
	KEY_L = Get_Adc_Average(ADC_Channel_5,5);//左侧按键
	KEY_R = Get_Adc_Average(ADC_Channel_7,5);//右侧按键
	///////////////////////////////////////////////////////////////
	if(KEY_L<500)  				//左1
	{
		change_flag = 0;
		LED3 = 0 ;
		LED5 = 1 ;
	}
	else if(KEY_L>2400&&KEY_L<3500)	//左3
	{
		use_flag = 0;
		LED1 = 1 ;
	}
	else if(KEY_L>1500&&KEY_L<2400)								//左2
	{
		use_flag = 1;
		LED1 = 0 ;
	}
	///////////////////////////////////////////////////////////////
	if(KEY_R<500)					//右1
	{
		change_flag = 1;
		LED3 = 1 ;
		LED5 = 0 ;
	}
	else if(KEY_R>2400&&KEY_R<3500)	//右3
	{
	}
	else if(KEY_R>1500&&KEY_R<2400)									//右2
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
	//控制
	if(contral_NM>3100)
	{
		key=1;
		key_date[3]=0x11;// 发送 n
	}
	else	if(contral_NM<1000)
	{
		key_date[3]=0x10;// 发送 m
		key=1;
	}
	///////////////////////////////////////////////////////////////
	if(contral_JK>3500)
	{
		key=1;
		key_date[3]=0x0D;// 发送 J
	}
	else	if(contral_JK<600)
	{
		key_date[3]=0x0E;// 发送 K
		key=1;
	}
	///////////////////////////////////////////////////////////////
	//控制
	if(direction_WS>3500)
	{
		key_date[4]=0x1A; // 发送 w
		key=1;
	}
	else if(direction_WS<600)
	{
		key_date[4]=0x16;// 发送 s
		key=1;
	}
	///////////////////////////////////////////////////////////////
	//控制
		if(direction_AD<1000)
	{
		key_date[5]=0x07;// 发送 D
		key=1;
	}
	else	if(direction_AD>3100)
	{
		key_date[5]=0x04; // 发送 A
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

























