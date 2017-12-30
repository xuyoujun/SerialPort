#include "stdafx.h"

BOOL close_serial_port(HWND hwnd, HANDLE sp_hdr) {
	BOOL ret        = FALSE;
	HWND hwnd_open  = NULL;
	HWND hwnd_close = NULL;

	ret = CloseHandle(sp_hdr);//close serial port
	
	if (TRUE == ret) {
		sp_hdr = INVALID_HANDLE_VALUE;
		hwnd_open = GetDlgItem(hwnd, ID_OPEN);//解除对打开串口按钮的隐蔽
		EnableWindow(hwnd_open, TRUE);
		hwnd_close = GetDlgItem(hwnd, ID_CLOSE);//使关闭串口按钮隐蔽
		EnableWindow(hwnd_close, FALSE);
	}

	return ret;
}