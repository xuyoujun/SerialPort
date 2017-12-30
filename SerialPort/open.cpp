#include "stdafx.h"

static BOOL format_sp_name(TCHAR *sp_name,INT len,HWND hwnd) {
	HWND  hwnd_sp; //获得串口组件的句柄
	INT index = 0;
	*sp_name++ = '\\';
	*sp_name++ = '\\';
	*sp_name++ = '.';
	*sp_name++ = '\\';
	hwnd_sp = GetDlgItem(hwnd, IDC_COMBOSERIAL);//获得串口组件的句柄
	index   = ComboBox_GetCurSel(hwnd_sp);//得到串口组件现在的选项的索引值
	ComboBox_GetLBText(hwnd_sp, index, sp_name);//得到索引值的内容
	return TRUE;
}

HANDLE open_serial_port(HWND hwnd) {
	HANDLE sp_hdr;  /* serial port handler */
	CString str;
	TCHAR sp_name[COMM_NAME_LEN] = { 0 };

	format_sp_name(sp_name, sizeof(sp_name), hwnd);
	sp_hdr = CreateFile(sp_name,      /* serial port name */
		GENERIC_READ | GENERIC_WRITE, //允许读和写
		0,  //独占方式
		NULL,
		OPEN_EXISTING,  //打开而不是创建
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, //重叠方式
		NULL
		);

	if (INVALID_HANDLE_VALUE == sp_hdr) {
		str.Format(_T("Open %s is failure!"), sp_name + strlen("\\\\.\\"));
		MessageBox(NULL, str, TEXT("Error"), MB_OK);
	}
	return sp_hdr;
}