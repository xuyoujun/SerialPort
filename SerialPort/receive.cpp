#include "stdafx.h"

extern HWND main_hwnd;

static void insert_to_editor(TCHAR *buffer, HWND hwnd_editor) {
	HWND hwnd_hex;
	INT data_format;

	hwnd_hex = GetDlgItem(main_hwnd, IDC_RADIO1);
	data_format = Button_GetCheck(hwnd_hex);
	
	/* hexicadecimal data */
	if (HEX_DATA == data_format) {
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


/*
	The thread read data from serial port
*/
DWORD WINAPI receive_serial_port_thread(LPVOID lpParam)//用于读线程的函数
{
	HANDLE     sp_hdr;      /* serial port handler */
	BOOL       rd_stat;     /* read status */
	DWORD      err_type;    /* error type*/
	DWORD      mask_type;
	COMSTAT    sp_stat;     /* serial port status */
	HWND       hwnd_editor;
	OVERLAPPED asy_io;
	TCHAR      buffer[MAX_BUFFER_SIZE];
	DWORD      len = MAX_BUFFER_SIZE;
	DWORD      read_num;
	sp_hdr      = (HANDLE)lpParam;

	ZeroMemory(&asy_io, sizeof(asy_io));
	ZeroMemory(buffer , sizeof(buffer));
	hwnd_editor = GetDlgItem(main_hwnd, IDC_RICHEDIT22);
	asy_io.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (FALSE == asy_io.hEvent) {
		MessageBox(NULL, "Error:Create event!", TEXT("Error"), MB_OK);
		return 0;
	}

	while (TRUE) {
		ZeroMemory(buffer, sizeof(buffer));

		rd_stat = WaitCommEvent(sp_hdr, &mask_type, &asy_io);
		if (FALSE == rd_stat) {
			if (ERROR_IO_PENDING == GetLastError()) {
				GetOverlappedResult(sp_hdr, &asy_io, &len, TRUE);
			}
		}

		/* Waiting the data is transfered completely */
		Sleep(1);

		ClearCommError(sp_hdr, &err_type, &sp_stat);

		len = sp_stat.cbInQue;
		if (0 == len) {
			continue;
		}

		/* Not receive event */
		if (EV_RXCHAR != (mask_type & EV_RXCHAR)) {
			continue;
		}

		rd_stat = ReadFile(sp_hdr, buffer, len , &read_num, &asy_io);

		if (FALSE == rd_stat){
			break;
		}

		insert_to_editor(buffer, hwnd_editor);
		//PurgeComm(sp_hdr, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	}

	CloseHandle(asy_io.hEvent);
	return 0;
}