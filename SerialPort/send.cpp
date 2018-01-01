#include "stdafx.h"


static BOOL get_data_form_editor(HWND hwnd, TCHAR *buffer) {
	INT   data_format;
	HWND  hwnd_hex;
	DWORD len = MAX_BUFFER_SIZE;
	hwnd_hex = GetDlgItem(hwnd, IDC_RADIO3);
	data_format = Button_GetCheck(hwnd_hex);

	GetDlgItemText(hwnd, IDC_EDIT2, buffer, MAX_BUFFER_SIZE);
	len = strlen(buffer);
	
	if (HEX_DATA == data_format) {
		 return convent_to_hex(buffer, len);
	}
	return TRUE;
}

BOOL send_serial_port(HWND hwnd, HANDLE sp_hdr) {

	BOOL  ret = FALSE;
	BOOL  wr_stat = FALSE;
	DWORD len = MAX_BUFFER_SIZE;
	TCHAR buffer[MAX_BUFFER_SIZE];
	OVERLAPPED asy_io;

	if (sp_hdr == INVALID_HANDLE_VALUE) {
		MessageBox(hwnd, TEXT("No serial port is opened."), TEXT("Error"), MB_OK);
		return ret;
	}
		
	PurgeComm(sp_hdr, PURGE_TXCLEAR);
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
