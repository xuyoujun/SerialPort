////#pragma once

//#include "resource.h"

#ifndef _MAIN_H
#define _MAIN_H

#include <windows.h>

BOOL WINAPI Main_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL Main_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
void Main_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void Main_OnSize(HWND hwnd, UINT state, INT cx, INT cy);
void Main_OnClose(HWND hwnd);
DWORD WINAPI Receive_Serial_Port_Thread( LPVOID lpParam );

#endif