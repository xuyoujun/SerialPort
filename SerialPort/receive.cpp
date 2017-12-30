#include "stdafx.h"

extern HWND main_hwnd;
DWORD WINAPI Receive_Serial_Port_Thread(LPVOID lpParam)//用于读线程的函数
{
	HANDLE    com_handler = (HANDLE)lpParam; //获得串口句柄
	TCHAR   lpInBuffer[MAX_BUFFER_SIZE]; //设置读缓冲区
	DWORD   dwBytesRead = MAX_BUFFER_SIZE;//读取到的字节数
	BOOL    bReadStatus;//读是否成功
	DWORD   dwErrorFlags;
	COMSTAT ComStat;//对串口具有监视作用
	OVERLAPPED m_osRead;
	HWND hwnd_hex = GetDlgItem(main_hwnd, IDC_RADIO1);//接收_得到16进制的按钮句柄
	HWND hwnd_richedit = GetDlgItem(main_hwnd, IDC_RICHEDIT22);

	INT hex_format = 0;//接收_得到句柄的状态
	UINT32 i = 0;
	while (1) {
		memset(&m_osRead, 0, sizeof(OVERLAPPED));
		memset(lpInBuffer, 0, sizeof(lpInBuffer));
		m_osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		ClearCommError(com_handler, &dwErrorFlags, &ComStat);
		hex_format = Button_GetCheck(hwnd_hex);

		if (!ComStat.cbInQue) {//读缓冲区没有数据，即0==ComStat.cbInQue////////////////////////////////
			Sleep(50);//这个函数很重要，如果没有的话cup的使用率会很高（大约30%），用过之后会降低很多（3%左右）。不过在传输数据很频繁的系统，休眠时间要尽量短，以免影响效率。
			continue;
		}
		else {//读缓冲区有数据，即0！=ComStat.cbInQue
			dwBytesRead = (DWORD)ComStat.cbInQue;//得到读缓冲区的字节数
			bReadStatus = ReadFile(com_handler, lpInBuffer,
				dwBytesRead, &dwBytesRead, &m_osRead);//进行读串口处理
			if (!bReadStatus && (GetLastError() == ERROR_IO_PENDING)) { //如果ReadFile函数返回FALSE
				GetOverlappedResult(com_handler, &m_osRead, &dwBytesRead, TRUE);
				PurgeComm(com_handler,
					PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
				if (1 == hex_format) {//发16进制数据
					convent_to_hex(lpInBuffer, strlen(lpInBuffer));
				}

				SendMessage(hwnd_richedit, EM_SETSEL, -1, -1);
				SendMessage(hwnd_richedit, WM_VSCROLL, SB_BOTTOM, 0);
				SendMessage(hwnd_richedit, EM_REPLACESEL, 0, (LPARAM)lpInBuffer);
				SendMessage(hwnd_richedit, EM_REPLACESEL, 0, (LPARAM)L"\r\n");
				//SetDlgItemText(hwnd, IDC_RICHEDIT22, strcat(visibla, lpInBuffer));//显示在richedit中
				CloseHandle(m_osRead.hEvent);
			}
			else {// //ReadFile函数返回TRUE
				if (1 == hex_format) {//接收16进制数据
					convent_to_hex(lpInBuffer, strlen(lpInBuffer));
				}
				SendMessage(hwnd_richedit, EM_SETSEL, -1, -1);
				SendMessage(hwnd_richedit, WM_VSCROLL, SB_BOTTOM, 0);
				SendMessage(hwnd_richedit, EM_REPLACESEL, 0, (LPARAM)lpInBuffer);
				SendMessage(hwnd_richedit, EM_REPLACESEL, 0, (LPARAM)L"\r\n");
				//SetDlgItemText(hwnd, IDC_RICHEDIT22, strcat(visibla, lpInBuffer));//显示在richedit中
				PurgeComm(com_handler, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
				CloseHandle(m_osRead.hEvent);
			}
		}
	}
	return 0;
}