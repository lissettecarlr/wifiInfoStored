/*
	功能：通过串口接收发送而来的wifi名和密码
	作者：lissettecarlr
	版本：1.0
	说明：通过GetData函数接收一帧数据,将会保存在RcvData数组中，这个时候标志位RcvSucceed将会变为true,UpdataWifiPassword函数
				更新公共成员WifiName和WifiPassword。
*/

#ifndef __TRANSMISSION_H
#define __TRANSMISSION_H

#include "stm32f10x.h"
#include "USART.h"


class Transmission{
	
	private:
		u8 RcvData[70];
		bool Calibration(u8 *data,int lenth,u8 check);
	public:
		bool RcvSucceed;//是否接收成功 ,在更新一次名字密码后自动变为false
		char *WifiName;
		char *WifiPassword;
	
		Transmission();
		~Transmission();
		bool GetData(USART &ListeningCOM);
		bool UpdataWifiPassword();
	
};



#endif
