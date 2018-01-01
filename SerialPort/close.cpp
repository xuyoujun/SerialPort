#include "stdafx.h"

BOOL close_serial_port(HWND hwnd, HANDLE sp_hdr) {
	BOOL ret        = FALSE;
	HWND hwnd_open  = NULL;
	HWND hwnd_close = NULL;

	/* close serial port */
	ret = CloseHandle(sp_hdr);
	
	if (TRUE == ret) {
		sp_hdr = INVALID_HANDLE_VALUE;

		/* show the serial port botton */
		hwnd_open = GetDlgItem(hwnd, ID_OPEN);
		EnableWindow(hwnd_open, TRUE);
		/* hide the serial port botton */
		hwnd_close = GetDlgItem(hwnd, ID_CLOSE);
		EnableWindow(hwnd_close, FALSE);
	}

	return ret;
}