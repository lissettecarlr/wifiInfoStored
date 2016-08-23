#include "stm32f10x.h"
#include "Configuration.h"
#include "TaskManager.h"
#include "USART.h"
#include "flash.h"
#include "WIFI_Memory.h"
#include "transmission.h"


//Timer T1(TIM1,1,2,3); //使用定时器计，溢出时间:1S+2毫秒+3微秒
USART com(1,115200,false);


flash InfoStore(0x08000000+50*MEMORY_PAGE_SIZE,true);     //flash

GPIO ledRedGPIO(GPIOB,0,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);//LED GPIO
GPIO ledBlueGPIO(GPIOB,1,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);//LED GPIO

WifiMemory WifiStore(InfoStore);
Transmission Listen;

int main()
{

	double record_tmgTest=0; //taskmanager时间 测试
	
	char *name = new char[70];
	char *pass = new char[70];
	
	while(1)
	{
		
		if(tskmgr.TimeSlice(record_tmgTest,0.2)) //每0.2秒执行一次
		{
				Listen.GetData(com);
		}
		
		
		if(Listen.RcvSucceed)
		{
			if(Listen.UpdataWifiPassword())
			{
//				com<<"name:"<<Listen.WifiName<<"\n";
//				com<<"password:"<<Listen.WifiPassword<<"\n";
				WifiStore.Save(Listen.WifiName,Listen.WifiPassword);
				com<<"SAVE succed "<<"\n";
				
			}
			else
			{
				com<<"Updata Error \n";
			}	
		}
		if(Listen.IsSendWifiInfo)
		{
			Listen.IsSendWifiInfo=false;
			WifiStore.Load(name,pass);
			com<<"name:"<<name<<"\n";
			com<<"password:"<<pass<<"\n";
			
		}
		
	}
	delete(name);
	delete(pass);
}
