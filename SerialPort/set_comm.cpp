#include "stdafx.h"
#include <windows.h>
#include <windowsx.h>
#include "resource.h"
#include "SerialPort.h"
#include <commdlg.h>

BOOL Set_Serial_Port(HWND hwnd, HANDLE com_handler) {
	//HWND hwndIDOK    = GetDlgItem(hwnd, IDOK);//隐蔽打开串口按钮
	//ShowWindow(hwndIDOK,SW_HIDE);
	//HWND hwndIDCLOSE = GetDlgItem(hwnd, IDCLOSE);//解除对关闭串口按钮的隐蔽
	//ShowWindow(hwndIDCLOSE,SW_SHOW);
	HWND hwnd_data_bit;  //数据位 handler
	HWND hwnd_baud_rate; //波特率 handler
	HWND hwnd_check_bit; //校验位 handler
	HWND hwnd_stop_bit;  //停止位 handler
	TCHAR buffer[10] = { 0 };
	INT   index = 0;
	DCB   dcb;
	COMMTIMEOUTS TimeOuts;

	SetupComm(com_handler, 1024, 1024); //输入缓冲区和输出缓冲区的大小都是1024

	TimeOuts.ReadIntervalTimeout = 1000;//设定读超时
	TimeOuts.ReadTotalTimeoutMultiplier = 500;
	TimeOuts.ReadTotalTimeoutConstant = 5000;

	TimeOuts.WriteTotalTimeoutMultiplier = 500;//设定写超时
	TimeOuts.WriteTotalTimeoutConstant = 2000;
	SetCommTimeouts(com_handler, &TimeOuts);    //设置超时

												/* Get current comm port state */
	GetCommState(com_handler, &dcb);
	dcb.DCBlength = sizeof(dcb);

	/* Get data bit config */
	ZeroMemory(buffer, sizeof(buffer));
	hwnd_data_bit = GetDlgItem(hwnd, IDC_DATA_BIT);
	index = ComboBox_GetCurSel(hwnd_data_bit);//得到现在的选项的索引值
	ComboBox_GetLBText(hwnd_data_bit, index, buffer);//得到索引值的内容
	dcb.ByteSize = atoi(buffer);    //每个字节有8位

									/* Get stop bit config */
	ZeroMemory(buffer, sizeof(buffer));
	hwnd_stop_bit = GetDlgItem(hwnd, IDC_STOP_BIT);
	index = ComboBox_GetCurSel(hwnd_stop_bit);//得到现在的选项的索引值
	ComboBox_GetLBText(hwnd_stop_bit, index, buffer);//得到索引值的内容
	dcb.StopBits = atoi(buffer) - 1; //停止位

									 /* Get boud rate config */
	ZeroMemory(buffer, sizeof(buffer));
	hwnd_baud_rate = GetDlgItem(hwnd, IDC_BOUD_RATE);//设置波特率
	index = ComboBox_GetCurSel(hwnd_baud_rate);
	ComboBox_GetLBText(hwnd_baud_rate, index, buffer);
	dcb.BaudRate = atoi(buffer);       //波特率

									   /*Get check bit config*/
	ZeroMemory(buffer, sizeof(buffer));
	hwnd_check_bit = GetDlgItem(hwnd, IDC_CHECK_BIT);//设置校检位
	index = ComboBox_GetCurSel(hwnd_check_bit);
	ComboBox_GetLBText(hwnd_check_bit, index, buffer);
	dcb.Parity = *buffer; //有无奇偶校验位

						  /* Set current comm port state */
	SetCommState(com_handler, &dcb);
	//PurgeComm(com_handler,PURGE_TXCLEAR|PURGE_RXCLEAR);
	//SetCommMask(com_handler,);

	/* Create a thread to receive data */
	CreateThread(NULL, 0, Receive_Serial_Port_Thread, hwnd, 0, 0);//开始读线程
	return true;
}

