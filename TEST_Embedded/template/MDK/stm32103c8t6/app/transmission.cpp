#include "transmission.h"

Transmission::Transmission()
{
	 WifiName = new char[32];
	 WifiPassword = new char[32];
	 RcvSucceed=false;
	 IsSendWifiInfo=false;
}

Transmission::~Transmission()
{
	delete []WifiName;
	delete []WifiPassword;
}

bool Transmission::Calibration(u8 *data,int lenth,u8 check)
{
	u8 num=0; 
	for(int i=0;i<lenth;i++)
	{
		num+=data[i];
	}
	if(num == check)
		return true;
	else
		return false;
}

//协议组成
//0XFF 0X00 数据长度 WIFI名 0XFF  WIFI密码 0XFF 校验和

bool Transmission::GetData(USART &ListeningCOM)
{
	u8 ch=0;
	ListeningCOM.GetReceivedData(&ch,1);
		if(ch == 0xFF)
		{
			ListeningCOM.GetReceivedData(&ch,1);
			if(ch == 0x00)
			{
				//读取长度
				ListeningCOM.GetReceivedData(&ch,1);
				while(ListeningCOM.ReceiveBufferSize()<ch); //循环等待数据
					ListeningCOM.GetReceivedData(RcvData,ch);
				
				if( Calibration(RcvData, ch-1, RcvData[ch-1]) )
				{
					RcvSucceed=true;//标准读取到一帧新数据
					return 1;
				}
				else
				{
					RcvSucceed=false;//由于上面已经更改暂存数组，之后校验失败，这里必须将标志位置为false
					return false;
				}
				
			}else if(ch == 0x01)
			{
				ListeningCOM.GetReceivedData(&ch,1);
				if(ch == 0x01)
				{
					IsSendWifiInfo=true;
					return 1;
				}
				else
					return false;
			}
			else
				return 0;
		}
		else
			return 0;
}

bool Transmission::UpdataWifiPassword()
{
	int i=0,j=0;
	if(RcvSucceed)
	{
			while(RcvData[i]!=0xff)
			{
				*(WifiName+i)=RcvData[i];
				i++;
				if(i>=70)//防止越界
					return false;
			}
			i++;
			while(RcvData[i]!=0xff)
			{
				*(WifiPassword+j)=RcvData[i];
				i++;
				j++;
				if(i>=70)
					return false;
			}
		RcvSucceed=false;
		return true;
	}
	else
		return false;
}

