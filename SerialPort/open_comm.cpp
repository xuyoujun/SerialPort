#include "stdafx.h"
#include <windows.h>
#include <windowsx.h>
#include "resource.h"
#include "SerialPort.h"
#include <commdlg.h>
#include "common.h"


BOOL get_comm_name(TCHAR * buffer, UINT32 num) {
	UINT32 i = 0;
	UINT32 r = 0;
	BYTE temp = 0;
	if (NULL == buffer || num >= MAX_COMM_NUM) {
		return FALSE;
	}
	buffer[i++] = 'C';
	buffer[i++] = 'O';
	buffer[i++] = 'M';
	itoa(num, (char *)&buffer[i], 10);
	return TRUE;
}
HANDLE Open_Serial_Port(HWND hwnd) {
	INT   index;
	HANDLE hCom;  //串口句柄
	TCHAR comm_name[256];
	HWND  hwnd_comm; //获得串口组件的句柄
	ZeroMemory(comm_name, sizeof(comm_name));
	hwnd_comm = GetDlgItem(hwnd, IDC_COMBOSERIAL);//获得串口组件的句柄
	index = ComboBox_GetCurSel(hwnd_comm);//得到串口组件现在的选项的索引值
	get_comm_name(comm_name, index);	/* Get comm name */
	ComboBox_GetLBText(hwnd_comm, index, comm_name);//得到索引值的内容

	hCom = CreateFile(comm_name,  //COM口
		GENERIC_READ | GENERIC_WRITE, //允许读和写
		0,  //独占方式
		NULL,
		OPEN_EXISTING,  //打开而不是创建
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, //重叠方式
		NULL
		);

	if (INVALID_HANDLE_VALUE == hCom) {
		MessageBox(NULL, TEXT("打开COM失败!"), TEXT("Error"), NULL);
		return hCom;
	}
	return hCom;
}