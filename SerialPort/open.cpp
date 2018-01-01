#include "stdafx.h"

static BOOL format_sp_name(TCHAR *sp_name,INT len,HWND hwnd) {
	HWND  hwnd_sp;
	INT index = 0;
	*sp_name++ = '\\';
	*sp_name++ = '\\';
	*sp_name++ = '.';
	*sp_name++ = '\\';
	hwnd_sp = GetDlgItem(hwnd, IDC_COMBOSERIAL);
	index   = ComboBox_GetCurSel(hwnd_sp);
	ComboBox_GetLBText(hwnd_sp, index, sp_name);
	return TRUE;
}

HANDLE open_serial_port(HWND hwnd) {
	HANDLE sp_hdr;  /* serial port handler */
	CString str;
	TCHAR sp_name[COMM_NAME_LEN] = { 0 };

	format_sp_name(sp_name, sizeof(sp_name), hwnd);
	sp_hdr = CreateFile(sp_name,      /* serial port name */
		GENERIC_READ | GENERIC_WRITE, /* read and writ */
		0,                            /* show mode. 0:exclusive 1:share*/
		NULL,
		OPEN_EXISTING,                /* the file must exist */
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, /* overlapped style */
		NULL
		);

	if (INVALID_HANDLE_VALUE == sp_hdr) {
		str.Format(_T("Open %s is failure!"), sp_name + strlen("\\\\.\\"));
		MessageBox(NULL, str, TEXT("Error"), MB_OK);
	}
	return sp_hdr;
}