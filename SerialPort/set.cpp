#include "stdafx.h"

static void set_hide_close_show_open(HWND hwnd) {

	HWND hwnd_close;
	HWND hwnd_open;
	hwnd_open = GetDlgItem(hwnd, ID_OPEN);
	EnableWindow(hwnd_open, FALSE);
	hwnd_close = GetDlgItem(hwnd, ID_CLOSE);
	EnableWindow(hwnd_close, TRUE);
}

static void set_timeout(HANDLE sp_hdr,COMMTIMEOUTS *timeout) {
	if (NULL == timeout) {
		return;
	}

	timeout->ReadIntervalTimeout = 1000;//设定读超时
	timeout->ReadTotalTimeoutMultiplier = 500;
	timeout->ReadTotalTimeoutConstant = 5000;
	timeout->WriteTotalTimeoutMultiplier = 500;//设定写超时
	timeout->WriteTotalTimeoutConstant = 2000;
	SetCommTimeouts(sp_hdr, timeout);    //设置超时
}

INT get_data_bit(HWND hwnd) {
	INT i;
	HWND hwnd_db;
	TCHAR buffer[32] = {0};

	hwnd_db = GetDlgItem(hwnd, IDC_DATA_BIT);
	i = ComboBox_GetCurSel(hwnd_db);
	ComboBox_GetLBText(hwnd_db, i, buffer);
	return  atoi(buffer);
}

INT get_stop_bit(HWND hwnd) {
	INT   ret;
	HWND  hwnd_sb;  //停止位 handler
	TCHAR buffer[32] = { 0 };

	hwnd_sb = GetDlgItem(hwnd, IDC_STOP_BIT);
	ret = ComboBox_GetCurSel(hwnd_sb);
	ComboBox_GetLBText(hwnd_sb, ret, buffer);
	return  atoi(buffer);
}

INT get_boud_rate(HWND hwnd) {
	INT   ret;
	HWND  hwnd_br;
	TCHAR buffer[32] = { 0 };

	hwnd_br = GetDlgItem(hwnd, IDC_BOUD_RATE);
	ret = ComboBox_GetCurSel(hwnd_br);
	ComboBox_GetLBText(hwnd_br, ret, buffer);
	return atoi(buffer);
}


void get_check_bit(HWND hwnd, TCHAR *buffer) {
	INT ret;
	HWND hwnd_cb;
	if (NULL == buffer) {
		return;
	}
	ZeroMemory(buffer, sizeof(buffer));
	hwnd_cb = GetDlgItem(hwnd, IDC_CHECK_BIT);
	ret = ComboBox_GetCurSel(hwnd_cb);
	ComboBox_GetLBText(hwnd_cb, ret, buffer);
}

BOOL set_serial_port(HWND hwnd, HANDLE sp_hdr) {
	
	DCB   dcb;
	TCHAR buffer[10] = { 0 };
	COMMTIMEOUTS TimeOuts;

	if (INVALID_HANDLE_VALUE == sp_hdr) {
		return FALSE;
	}

	/*set the buffer's size of receive and send*/
	SetupComm(sp_hdr, RECEIVE_BUFFER_SIZE, SEND_BUFFER_SIZE);
	set_timeout(sp_hdr, &TimeOuts);
	
	/* Get current serial port state */
	GetCommState(sp_hdr, &dcb);
	dcb.DCBlength = sizeof(dcb);
	dcb.ByteSize = get_data_bit(hwnd);
	dcb.StopBits = get_stop_bit(hwnd);
	dcb.BaudRate = get_boud_rate(hwnd); 
	get_check_bit(hwnd, buffer);
	dcb.Parity = *buffer;
	/* Set current serial port state */
	SetCommState(sp_hdr, &dcb);

	/*clear receive and send buffer*/
	PurgeComm(sp_hdr,PURGE_TXCLEAR|PURGE_RXCLEAR);
	/*Set the event serial port monitor */
	//SetCommMask(sp_hdr, EV_RXCHAR | EV_TXEMPTY);
	SetCommMask(sp_hdr, EV_RXCHAR);
	/* Create a thread to receive data */
	CreateThread(NULL, 0, receive_serial_port_thread, sp_hdr, 0, 0);//开始读线程
	/* hide the open botton */
	set_hide_close_show_open(hwnd);
	return true;
}
