#include "stdafx.h"

extern HWND main_hwnd;

static void insert_to_editor(TCHAR *buffer, HWND hwnd_editor) {
	HWND hwnd_hex;
	INT data_format;

	hwnd_hex = GetDlgItem(main_hwnd, IDC_RADIO1);
	data_format = Button_GetCheck(hwnd_hex);
	
	if (1 == data_format) {//接收16进制数据
		convent_to_hex(buffer, strlen(buffer));
	}
	/* insert in the bottom of editor*/
	SendMessage(hwnd_editor, EM_SETSEL, -1, -1);
	/* slide in the bottom of editor*/
	SendMessage(hwnd_editor, WM_VSCROLL, SB_BOTTOM, 0);
	/* insert the buffer to the bottom of editor*/
	SendMessage(hwnd_editor, EM_REPLACESEL, 0, (LPARAM)buffer);
	/* insert the \n */
	SendMessage(hwnd_editor, EM_REPLACESEL, 0, (LPARAM)L"\r\n");
}

DWORD WINAPI receive_serial_port_thread(LPVOID lpParam)//用于读线程的函数
{
	HANDLE     sp_hdr;      /* serial port handler */
	BOOL       rd_stat;     /* read status */
	DWORD      err_type;    /* error type*/
	COMSTAT    sp_stat;     /* serial port status */
	HWND       hwnd_editor;
	OVERLAPPED asy_io;
	TCHAR      buffer[MAX_BUFFER_SIZE]; //设置读缓冲区
	DWORD      len = MAX_BUFFER_SIZE;   //读取到的字节数

	sp_hdr      = (HANDLE)lpParam;
	hwnd_editor = GetDlgItem(main_hwnd, IDC_RICHEDIT22);

	while (1) {
		asy_io.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		ZeroMemory(&asy_io, sizeof(asy_io));
		ZeroMemory(buffer, sizeof(buffer));
		ClearCommError(sp_hdr, &err_type, &sp_stat);

		if (0 == sp_stat.cbInQue) {//none in buffer
			Sleep(30);
			continue;
		}
		else {/* there is data in buffer，0！=ComStat.cbInQue */
			len = sp_stat.cbInQue;/* the byte of data in buffer */
			rd_stat = ReadFile(sp_hdr, buffer, len, &len, &asy_io);//进行读串口处理
			
			if (!rd_stat && (ERROR_IO_PENDING == GetLastError())) { //如果ReadFile函数返回FALSE
				GetOverlappedResult(sp_hdr, &asy_io, &len, TRUE);
			}
			insert_to_editor(buffer, hwnd_editor);
			PurgeComm(sp_hdr, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
			CloseHandle(asy_io.hEvent);
		}
	}
	return 0;
}