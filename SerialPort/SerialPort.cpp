#include "stdafx.h"
#include <windows.h>
#include <windowsx.h>
#include "resource.h"
#include "SerialPort.h"
#include <commdlg.h>

#include "open_comm.h"
#include "close_comm.h"
#include "set_comm.h"
#include "send_data.h"
#include "receive_data.h"
#include "set_layout.h"

#include <atlstr.h>


static HANDLE com_handler = INVALID_HANDLE_VALUE;//串口句柄
HWND   main_hwnd;// = { 0 };//

BOOL WINAPI Main_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        HANDLE_MSG(hWnd, WM_INITDIALOG, Main_OnInitDialog);
        HANDLE_MSG(hWnd, WM_COMMAND, Main_OnCommand);
		HANDLE_MSG(hWnd, WM_SIZE, Main_OnSize);
		HANDLE_MSG(hWnd, WM_CLOSE, Main_OnClose);
    }
	main_hwnd = hWnd;
    return FALSE;
}


BOOL Main_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	HWND hwnd_close = GetDlgItem(hwnd,ID_CLOSE);//获得关闭按钮的句柄
	EnableWindow(hwnd_close,FALSE);//先隐蔽关闭串口选项
	//ShowWindow(hwndIDOK,SW_);
	//BOOL result = FALSE;
	init_layout(hwnd);
	return TRUE;
}



void Main_OnCommand(HWND hwnd, int command, HWND hwndCtl, UINT codeNotify)
{
	switch(command){
		case ID_OPEN:
			com_handler = Open_Serial_Port(hwnd);//打开串口
			if (com_handler != INVALID_HANDLE_VALUE) {
				Set_Serial_Port(hwnd, com_handler);
			}
			break;

		case ID_CLOSE:
			Close_Serial_Port(hwnd, com_handler);
			break;

		case IDC_SEND:

			Send_Serial_Port(hwnd, com_handler);
			break;
		case ID_FRESH:
			fresh_serial_port(hwnd);
			break;
		case IDC_CLEAR_RECEIVE:
			SetDlgItemText(hwnd,IDC_RICHEDIT22,NULL);//清空接收区
			break;

		case IDC_CLEAR_SEND:
			SetDlgItemText(hwnd,IDC_EDIT2,NULL);//清空发送区
			break;
        default:
		    break;
    }
}

void Main_OnSize(HWND hwnd, UINT state, INT cx, INT cy)
{
	set_layout(hwnd);
}
void Main_OnClose(HWND hwnd)
{
    EndDialog(hwnd,0);
}

