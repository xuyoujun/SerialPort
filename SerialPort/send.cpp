#include "stdafx.h"


static BOOL get_data_form_editor(HWND hwnd, TCHAR *buffer) {
	INT   data_format;
	HWND  hwnd_hex;
	DWORD len = MAX_BUFFER_SIZE;         //要写入数据的字节
	hwnd_hex = GetDlgItem(hwnd, IDC_RADIO3);//发送_得到16进制的按钮句柄
	data_format = Button_GetCheck(hwnd_hex);//得到句柄的状态

	GetDlgItemText(hwnd, IDC_EDIT2, buffer, MAX_BUFFER_SIZE);//得到要写入的数据
	len = strlen(buffer);  //要写入数据的长度
	
	if (1 == data_format) {//发送16进制数据
		 return convent_to_hex(buffer, len);
	}
	return TRUE;
}

BOOL send_serial_port(HWND hwnd, HANDLE sp_hdr) {

	BOOL  ret = FALSE;//是否有非法字符的标志
	BOOL  wr_stat = FALSE;//用于判断写入是否成功
	DWORD len = MAX_BUFFER_SIZE;
	TCHAR buffer[MAX_BUFFER_SIZE];//准备写入的数据
	OVERLAPPED asy_io;

	if (sp_hdr == INVALID_HANDLE_VALUE) {
		MessageBox(hwnd, TEXT("No serial port is opened."), TEXT("Error"), MB_OK);
		return ret;
	}
		
	PurgeComm(sp_hdr, PURGE_TXCLEAR);//清空写缓冲区
	ZeroMemory(buffer, sizeof(buffer));
	ret = get_data_form_editor(hwnd, buffer);
	
	if (FALSE == ret) {
		MessageBox(hwnd, TEXT("Data is illegal."), TEXT("Error"), MB_OK);
		return ret;
	}

	asy_io.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	wr_stat = WriteFile(sp_hdr, buffer, len, &len, &asy_io);
	
	if ((FALSE == wr_stat) && (ERROR_IO_PENDING == GetLastError())) {
		WaitForSingleObject(asy_io.hEvent, 500);
	}

	CloseHandle(asy_io.hEvent);
	return ret;
}
