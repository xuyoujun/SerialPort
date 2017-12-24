// main.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Resource.h"
#include "SerialPort.h"
#include <commctrl.h>
#include <commdlg.h>
//#define MAX_LOADSTRING 100
//
// 全局变量:
//HINSTANCE hInst;								// 当前实例
//TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
//TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

// 此代码模块中包含的函数的前向声明:

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	//InitCommonControls();

	LoadLibrary( "riched20.dll" );
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAIN),NULL,Main_Proc);
	return 0;
}