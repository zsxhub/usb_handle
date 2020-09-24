/**
  ******************************************************************************
  * @file    usb_desc.c
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   Descriptors for Joystick Mouse Demo
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include "usb_desc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* USB Standard Device Descriptor */
const uint8_t Joystick_DeviceDescriptor[JOYSTICK_SIZ_DEVICE_DESC] =
  {
    0x12,                       /*bLength */
    USB_DEVICE_DESCRIPTOR_TYPE, /*bDescriptorType*/
    0x00,                       /*bcdUSB */
    0x02,
    0x00,                       /*bDeviceClass*/
    0x00,                       /*bDeviceSubClass*/
    0x00,                       /*bDeviceProtocol*/
    0x40,                       /*bMaxPacketSize 64*/
    0x83,                       /*idVendor (0x0483)*/
    0x04,
    0x10,                       /*idProduct = 0x5710*/
    0x57,
    0x00,                       /*bcdDevice rel. 2.00*/
    0x02,
    1,                          /*Index of string descriptor describing
                                                  manufacturer */
    2,                          /*Index of string descriptor describing
                                                 product*/
    3,                          /*Index of string descriptor describing the
                                                 device serial number */
    0x01                        /*bNumConfigurations*/
  }
  ; /* Joystick_DeviceDescriptor */


/* USB Configuration Descriptor */
/*   All Descriptors (Configuration, Interface, Endpoint, Class, Vendor */
const uint8_t Joystick_ConfigDescriptor[JOYSTICK_SIZ_CONFIG_DESC] =
  {
    0x09, /* bLength: Configuration Descriptor size */
    USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType: Configuration */
    JOYSTICK_SIZ_CONFIG_DESC,
    /* wTotalLength: Bytes returned */
    0x00,
    0x01,         /*bNumInterfaces: 1 interface*/
    0x01,         /*bConfigurationValue: Configuration value*/
    0x00,         /*iConfiguration: Index of string descriptor describing
                                     the configuration*/
    0xE0,         /*bmAttributes: Self powered */
    0x32,         /*MaxPower 100 mA: this current is used for detecting Vbus*/

    /************** Descriptor of Joystick Mouse interface ****************/
    /* 09 */
    0x09,         /*bLength: Interface Descriptor size*/
    USB_INTERFACE_DESCRIPTOR_TYPE,/*bDescriptorType: Interface descriptor type*/
    0x00,         /*bInterfaceNumber: Number of Interface*/
    0x00,         /*bAlternateSetting: Alternate setting*/
    0x01,         /*bNumEndpoints*/
    0x03,         /*bInterfaceClass: HID*/
    0x01,         /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
    0x01,         /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
    0,            /*iInterface: Index of string descriptor*/
    /******************** Descriptor of Joystick Mouse HID ********************/
    /* 18 */
    0x09,         /*bLength: HID Descriptor size*/
    HID_DESCRIPTOR_TYPE, /*bDescriptorType: HID*/
    0x00,         /*bcdHID: HID Class Spec release number*/
    0x01,
    0x00,         /*bCountryCode: Hardware target country*/
    0x01,         /*bNumDescriptors: Number of HID class descriptors to follow*/
    0x22,         /*bDescriptorType*/
    JOYSTICK_SIZ_REPORT_DESC,/*wItemLength: Total length of Report descriptor*/
    0x00,
    /******************** Descriptor of Joystick Mouse endpoint ********************/
    /* 27 */
    0x07,          /*bLength: Endpoint Descriptor size*/
    USB_ENDPOINT_DESCRIPTOR_TYPE, /*bDescriptorType:*/

    0x81,          /*bEndpointAddress: Endpoint Address (IN)*/
    0x03,          /*bmAttributes: Interrupt endpoint*/
    0x08,          /*wMaxPacketSize: 4 Byte max */
    0x00,
    0x20,          /*bInterval: Polling Interval (32 ms)*/
    /* 34 */
  }
  ; /* MOUSE_ConfigDescriptor */
const uint8_t Joystick_ReportDescriptor[JOYSTICK_SIZ_REPORT_DESC] =
  {
		
		
	//0x05:0000 01 01 这是个全局条目，用途页选择为普通桌面页
	0x05, 0x01, // USAGE_PAGE (Generic Desktop)
	//0x09:0000 10 01 这是个全局条目，用途选择为键盘
	0x09, 0x06, // USAGE (Keyboard)
	//0xa1:1010 00 01 这是个主条目，选择为应用集合，
	0xa1, 0x01, // COLLECTION (Application)
	//0x05:0000 01 11 这是个全局条目，用途页选择为键盘/按键
	0x05, 0x07, // USAGE_PAGE (Keyboard/Keypad)

	//0x19:0001 10 01 这是个局部条目，用途的最小值为0xe0，对应键盘上的contral键
	0x19, 0xe0, // USAGE_MINIMUM (Keyboard LeftControl)
	//0x29:0010 10 01 这是个局部条目，用途的最大值为0xe7，对应键盘上的有GUI(WIN)键
	0x29, 0xe7, // USAGE_MAXIMUM (Keyboard Right GUI)
	//0x15:0001 01 01 这是个全局条目，说明数据的逻辑值最小值为0
	0x15, 0x00, // LOGICAL_MINIMUM (0)
	//0x25:0010 01 01 这是个全局条目，说明数据的逻辑值最大值为1
	0x25, 0x01, // LOGICAL_MAXIMUM (1)

	//0x95:1001 01 01 这是个全局条目，数据域的数量为8个
	0x95, 0x08, // REPORT_COUNT (8)
	//0x75:0111 01 01 这是个全局条目，每个数据域的长度为1位
	0x75, 0x01, // REPORT_SIZE (1)	   
	//0x81:1000 00 01 这是个主条目，有8*1bit数据域作为输入，属性为:Data,Var,Abs
	0x81, 0x02, // INPUT (Data,Var,Abs)

	//0x95:1001 01 01 这是个全局条目,数据域的数量为1个
	0x95, 0x01, // REPORT_COUNT (1)
	//0x75:0111 01 01 这是个全局条目，每个数据域的长度为8位
	0x75, 0x08, // REPORT_SIZE (8)
	//0x81:1000 00 01 这是个主条目，有1*8bit数据域作为输入，属性为:Cnst,Var,Abs
	0x81, 0x03, // INPUT (Cnst,Var,Abs)

	//0x95:1001 01 01 这是个全局条目，数据域的数量为6个
	0x95, 0x06, // REPORT_COUNT (6)
	//0x75:0111 01 01 这是个全局条目，每个数据域的长度为8位
	0x75, 0x08, // REPORT_SIZE (8)
	//0x25:0010 01 01 这是个全局条目，逻辑最大值为255
	0x25, 0xFF, // LOGICAL_MAXIMUM (255)
	//0x19:0001 10 01 这是个局部条目，用途的最小值为0
	0x19, 0x00, // USAGE_MINIMUM (Reserved (no event indicated))
	//0x29:0010 10 01 这是个局部条目，用途的最大值为0x65
	0x29, 0x65, // USAGE_MAXIMUM (Keyboard Application)
	//0x81:1000 00 01 这是个主条目，有6*8bit的数据域作为输入，属相为属性为:Data,Var,Abs
	0x81, 0x00, // INPUT (Data,Ary,Abs)

	//0x25:0010 01 01 这是个全局条目，逻辑的最大值为1
	0x25, 0x01, // LOGICAL_MAXIMUM (1)
	//0x95:1001 01 01 这是个全局条目，数据域的数量为2
	0x95, 0x02, // REPORT_COUNT (2)
	//0x75:0111 01 01 这是个全局条目，每个数据域的长度为1位
	0x75, 0x01, // REPORT_SIZE (1)
	//0x05:0000 01 01 这是个全局条目，用途页选择为LED页
	0x05, 0x08, // USAGE_PAGE (LEDs)
	//0x19:0001 10 01 这是个局部条目，用途的最小值为0x01,对应键盘上的Num Lock
	0x19, 0x01, // USAGE_MINIMUM (Num Lock)
	//0x29:0010 10 01 这是个局部条目，用途的最大值为0x02,对应键盘上的Caps Lock
	0x29, 0x02, // USAGE_MAXIMUM (Caps Lock)
	//0x91:1001 00 01 这是个主条目，有2*1bit的数据域作为输出，属性为:Data,Var,Abs
	0x91, 0x02, // OUTPUT (Data,Var,Abs)

	//0x95:1001 01 01 这是个全局条目，数据域的数量为1个
	0x95, 0x01, // REPORT_COUNT (1)
	//0x75:0111 01 01 这是个全局条目，每个数据域的长度为6bit,正好与前面的2bit组成1字节
	0x75, 0x06, // REPORT_SIZE (6)
	//0x91:1001 00 01 这是个主条目，有1*6bit数据域最为输出，属性为:Cnst,Var,Abs
	0x91, 0x03, // OUTPUT (Cnst,Var,Abs)

	0xc0        // END_COLLECTION
	
	
  }
  ; /* Joystick_ReportDescriptor */

/* USB String Descriptors (optional) */
const uint8_t Joystick_StringLangID[JOYSTICK_SIZ_STRING_LANGID] =
  {
    JOYSTICK_SIZ_STRING_LANGID,
    USB_STRING_DESCRIPTOR_TYPE,
    0x09,
    0x04
  }
  ; /* LangID = 0x0409: U.S. English */

const uint8_t Joystick_StringVendor[JOYSTICK_SIZ_STRING_VENDOR] =
  {
    JOYSTICK_SIZ_STRING_VENDOR, /* Size of Vendor string */
    USB_STRING_DESCRIPTOR_TYPE,  /* bDescriptorType*/
    /* Manufacturer: "STMicroelectronics" */
    'S', 0, 'T', 0, 'M', 0, 'i', 0, 'c', 0, 'r', 0, 'o', 0, 'e', 0,
    'l', 0, 'e', 0, 'c', 0, 't', 0, 'r', 0, 'o', 0, 'n', 0, 'i', 0,
    'c', 0, 's', 0
  };

const uint8_t Joystick_StringProduct[JOYSTICK_SIZ_STRING_PRODUCT] =
{
	JOYSTICK_SIZ_STRING_PRODUCT,          /* bLength */
	USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
	'A', 0, 'L', 0, 'I', 0, 'E', 0, 'N', 0, 'T', 0, 'E', 0, 'K', 0,//ALIENTEK
	' ', 0, 
	0XE6,0X89,//触
	0XA7,0X63,//控
	'U', 0,
	'S', 0, 
	'B', 0,
	0X20,0X9F,//鼠
	0X07,0X68,//标 
};

uint8_t Joystick_StringSerial[JOYSTICK_SIZ_STRING_SERIAL] =
  {
    JOYSTICK_SIZ_STRING_SERIAL,           /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'S', 0, 'T', 0, 'M', 0, '3', 0, '2', 0
  };

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/



	