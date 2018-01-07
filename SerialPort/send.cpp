#include "stdafx.h"


static INT get_data_form_editor(HWND hwnd, TCHAR *buffer) {
	INT   data_format;
	HWND  hwnd_hex;
	DWORD len = MAX_BUFFER_SIZE;
	hwnd_hex = GetDlgItem(hwnd, IDC_RADIO3);
	data_format = Button_GetCheck(hwnd_hex);

	GetDlgItemText(hwnd, IDC_EDIT2, buffer, MAX_BUFFER_SIZE);
	len = strlen(buffer);
	
	if (HEX_DATA == data_format) {
		if (!convent_to_hex(buffer, len)) {
			return 0;
		}
	}
	return len;
}

BOOL send_serial_port(HWND hwnd, HANDLE sp_hdr) {

	BOOL  ret = FALSE;
	BOOL  wr_stat = FALSE;
	DWORD len = MAX_BUFFER_SIZE;
	TCHAR buffer[MAX_BUFFER_SIZE];
	OVERLAPPED asy_io;

	if (sp_hdr == INVALID_HANDLE_VALUE) {
		MessageBox(hwnd, "No serial port is opened.", TEXT("Error"), MB_OK);
		return ret;
	}
	
	asy_io.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (FALSE == asy_io.hEvent) {
		MessageBox(NULL, "Create send event!", TEXT("Error"), MB_OK);
	}

	PurgeComm(sp_hdr, PURGE_TXCLEAR);
	ZeroMemory(buffer, sizeof(buffer));

	len = get_data_form_editor(hwnd, buffer);
	
	if (0 == len) {
		MessageBox(hwnd, TEXT("Data is illegal."), TEXT("Error"), MB_OK);
		return ret;
	}
	/* Event is used to sync */


	wr_stat = WriteFile(sp_hdr, buffer, len, &len, &asy_io);
	
	if (FALSE == wr_stat){
		if (ERROR_IO_PENDING == GetLastError()) {
			//WaitForSingleObject(asy_io.hEvent, 500);
			GetOverlappedResult(sp_hdr, &asy_io, &len, TRUE);
		}
		else {
			MessageBox(NULL, "Send data get trouble!", TEXT("Error"), MB_OK);
		}
	}

	CloseHandle(asy_io.hEvent);
	return ret;
}
