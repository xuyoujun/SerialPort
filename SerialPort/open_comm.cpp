#include "stdafx.h"
#include <windows.h>
#include <windowsx.h>
#include <atlstr.h>
#include <commdlg.h>

#include "resource.h"
#include "SerialPort.h"
#include "common.h"

static BOOL get_comm_name(TCHAR *pname,INT len,HWND hwnd) {
	HWND  hwnd_comm; //获得串口组件的句柄
	INT index = 0;
	*pname++ = '\\';
	*pname++ = '\\';
	*pname++ = '.';
	*pname++ = '\\';
	hwnd_comm = GetDlgItem(hwnd, IDC_COMBOSERIAL);//获得串口组件的句柄
	index = ComboBox_GetCurSel(hwnd_comm);//得到串口组件现在的选项的索引值
	ComboBox_GetLBText(hwnd_comm, index, pname);//得到索引值的内容
	return TRUE;
}

HANDLE Open_Serial_Port(HWND hwnd) {
	INT   index;
	HANDLE hCom;  //串口句柄
	TCHAR comm_name[COMM_NAME_LEN] = {0};
	CString str;
	get_comm_name(comm_name, sizeof(comm_name), hwnd);
	hCom = CreateFile(comm_name,  //COM口
		GENERIC_READ | GENERIC_WRITE, //允许读和写
		0,  //独占方式
		NULL,
		OPEN_EXISTING,  //打开而不是创建
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, //重叠方式
		NULL
		);

	if (INVALID_HANDLE_VALUE == hCom) {
		str.Format(_T("打开%s失败!"),comm_name + strlen("\\\\.\\"));
		MessageBox(NULL,str, TEXT("Error"), NULL);
	}
	return hCom;
}