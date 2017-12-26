#include "stdafx.h"
#include <windows.h>
#include <windowsx.h>
#include "resource.h"
#include "SerialPort.h"
#include <commdlg.h>
#include "common.h"
#include "data_format.h"

BOOL Send_Serial_Port(HWND hwnd, HANDLE com_handler) {

	HWND  hwnd_hex;
	INT   hex_format;
	TCHAR buffer[MAX_BUFFER_SIZE];//准备写入的数据
	DWORD len = sizeof(buffer);         //要写入数据的字节
	BOOL  check_flag = true;//是否有非法字符的标志
	BOOL  bWriteStat = FALSE;//用于判断写入是否成功
	OVERLAPPED m_osWrite = { 0 };

	if (com_handler == INVALID_HANDLE_VALUE) {
		MessageBox(hwnd, TEXT("Invalid Port Number"), TEXT("Error"), MB_OK);
	}
	else {

		hwnd_hex = GetDlgItem(hwnd, IDC_RADIO3);//发送_得到16进制的按钮句柄
		hex_format = Button_GetCheck(hwnd_hex);//得到句柄的状态
		PurgeComm(com_handler, PURGE_TXCLEAR);//清空写缓冲区
		ZeroMemory(buffer, sizeof(buffer));
		GetDlgItemText(hwnd, IDC_EDIT2, buffer, sizeof(buffer));//得到要写入的数据
		len = strlen(buffer);//要写入数据的长度
		if (1 == hex_format) {//发送16进制数据
			check_flag = convent_to_hex(buffer, len);
		}

		if (!check_flag) {//判断有非法字符
			MessageBox(hwnd, TEXT("请输入的16进制在0-9或a-f或A-F之间"), TEXT("Error"), MB_OK);
		}
		else {
			m_osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
			bWriteStat = WriteFile(com_handler, buffer, len, &len, &m_osWrite);
			if (!bWriteStat) {
				if (GetLastError() == ERROR_IO_PENDING) {
					WaitForSingleObject(m_osWrite.hEvent, 1000);
				}
			}
			CloseHandle(m_osWrite.hEvent);
		}
	}
	return true;
}

