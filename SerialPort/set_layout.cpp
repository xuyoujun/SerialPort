#include "stdafx.h"
#include <windows.h>
#include <windowsx.h>
#include "resource.h"
#include <atlstr.h>
#include "set_layout.h"
#include "common.h"


static BOOL get_comm_name(TCHAR * buffer, UINT32 num) {
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

static BOOL set_candidate_comm_name(HWND hwnd) {
	UINT32 i = 0;
	HWND  hwnd_comm_sel;
	BOOL result = FALSE;
	BYTE comm_name[32] = { 0 };
	hwnd_comm_sel = GetDlgItem(hwnd, IDC_COMBOSERIAL);//添加串口选项
	for (i = 0; i < MAX_COMM_NUM; i++) {
		result = get_comm_name((TCHAR *)comm_name, i);
		if (FALSE == result) {
			break;
		}
		ComboBox_InsertString(hwnd_comm_sel, -1, comm_name);
		ZeroMemory(comm_name, sizeof(comm_name));
	}
	ComboBox_SetCurSel(hwnd_comm_sel, i / 2);
	return TRUE;
}


static BOOL set_candidate_boud_rate(HWND hwnd) {
	UINT32 i = 0;
	HWND  hwnd_boud_sel;
	TCHAR  buffer[MAX_BOUD_BUFFER];
	UINT32 boud_rate[] = { 600,	1200,	2400,	4800,	9600,	19200,
		38400,	56000,	57600,	115200,	128000,	256000,	1440000 };
	hwnd_boud_sel = GetDlgItem(hwnd, IDC_COMBOBOUDRATE);//添加波特率选项
	for (i = 0; i < sizeof(boud_rate) / sizeof(boud_rate[0]); i++) {
		itoa(boud_rate[i], buffer, 10);
		ComboBox_InsertString(hwnd_boud_sel, -1, buffer);
	}
	ComboBox_SetCurSel(hwnd_boud_sel, 4);
	return TRUE;
}


static BOOL set_candidate_data_bit(HWND hwnd) {
	HWND hwndDate = GetDlgItem(hwnd, IDC_DATE);//数据位选项
	ComboBox_InsertString(hwndDate, -1, "4");
	ComboBox_InsertString(hwndDate, -1, "5");
	ComboBox_InsertString(hwndDate, -1, "6");
	ComboBox_InsertString(hwndDate, -1, "7");
	ComboBox_InsertString(hwndDate, -1, "8");
	ComboBox_SetCurSel(hwndDate, 4);
	return TRUE;
}

static BOOL set_candidate_stop_bit(HWND hwnd) {
	HWND hwndStop = GetDlgItem(hwnd, IDC_STOP);//停止位选项
	ComboBox_InsertString(hwndStop, -1, "1");
	ComboBox_InsertString(hwndStop, -1, "2");
	ComboBox_SetCurSel(hwndStop, 0);
	return TRUE;
}

static BOOL set_candidate_check_bit(HWND hwnd) {

	HWND hwnd_check_bit = GetDlgItem(hwnd, IDC_COMBO3);//校检位
													   //EVENPARITY 偶校验     NOPARITY 无校验
													   //MARKPARITY 标记校验   ODDPARITY 奇校验
	ComboBox_InsertString(hwnd_check_bit, -1, "NOPARITY");
	ComboBox_InsertString(hwnd_check_bit, -1, "ODDPARITY");
	ComboBox_InsertString(hwnd_check_bit, -1, "EVENPARITY");
	ComboBox_InsertString(hwnd_check_bit, -1, "MARKPARITY");
	ComboBox_SetCurSel(hwnd_check_bit, 0);
	return TRUE;
}

static BOOL set_candidate_data_format(HWND hwnd) {

	HWND receiveHex = GetDlgItem(hwnd, IDC_RADIO1);//接收_得到16进制的按钮句柄
	Button_SetCheck(receiveHex, 1);//设置默认选项
	HWND sendHex = GetDlgItem(hwnd, IDC_RADIO3);//发送_得到16进制的按钮句柄
	Button_SetCheck(sendHex, 1);//设置默认选项
	return TRUE;
}

static BOOL set_default_icon(HWND hwnd) {
	HINSTANCE hInstance = GetModuleHandle(NULL);
	HICON hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(SerialPort));
	SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
	return TRUE;
}
BOOL set_layout(HWND hwnd) {
	int left = 0;
	int top = 0;
	int right = 0;
	int bottom = 0;
	int height = 0;
	int width = 0;

	int receive_left = 0;
	int receive_top = 0;
	int receive_right = 0;
	int receive_bottom = 0;
	int receive_height = 0;
	int receive_width = 0;

	int send_left = 0;
	int send_top = 0;
	int send_right = 0;
	int send_bottom = 0;
	int send_height = 0;
	int send_width = 0;

	int temp_left = 0;
	int temp_top = 0;
	int temp_right = 0;
	int temp_bottom = 0;
	int temp_height = 0;
	int temp_width = 0;

	int radio1_width;
	int radio2_width;
	int radio3_width;
	int	radio4_width;
	int space_width;
	int space_height;

	CString s;
	RECT mian_windiw_pos;
	RECT radio1_pos;
	RECT radio2_pos;
	RECT clear_pos;
	RECT radio3_pos;
	RECT radio4_pos;
	RECT clear_send_pos;
	RECT send_pos;
	RECT comm_open_pos;

	RECT rich_edit_pos1;
	RECT rich_edit_pos2;

	HWND hwnd_editor_riceive;
	HWND hwnd_editor_send;
	HWND hwnd_static_receive;
	HWND hwnd_static_send;
	HWND hwnd_radio1;
	HWND hwnd_radio2;
	HWND hwnd_radio3;
	HWND hwnd_radio4;
	HWND hwnd_clear_receive;
	HWND hwnd_clear_send;
	HWND hwnd_send;
	HWND hwnd_comm_close;
	HWND hwnd_comm_open;
	HWND hwnd_comm_fresh;

	hwnd_editor_riceive = GetDlgItem(hwnd, IDC_RICHEDIT22);
	hwnd_static_send = GetDlgItem(hwnd, IDC_STATIC_SEND);
	hwnd_static_receive = GetDlgItem(hwnd, IDC_STATIC_RECEIVE);
	hwnd_editor_send = GetDlgItem(hwnd, IDC_EDIT2);
	hwnd_radio1 = GetDlgItem(hwnd, IDC_RADIO1);
	hwnd_radio2 = GetDlgItem(hwnd, IDC_RADIO2);
	hwnd_radio3 = GetDlgItem(hwnd, IDC_RADIO3);
	hwnd_radio4 = GetDlgItem(hwnd, IDC_RADIO4);
	hwnd_clear_receive = GetDlgItem(hwnd, IDC_CLEAR_RECEIVE);
	hwnd_clear_send = GetDlgItem(hwnd, IDC_CLEAR_SEND);
	hwnd_send = GetDlgItem(hwnd, IDC_SEND);
	hwnd_comm_close = GetDlgItem(hwnd, ID_CLOSE);
	hwnd_comm_open = GetDlgItem(hwnd, ID_OPEN);
	hwnd_comm_fresh = GetDlgItem(hwnd, ID_FRESH);

	GetClientRect(hwnd, &mian_windiw_pos); // 获取窗口客户区大小
	GetWindowRect(hwnd_editor_riceive, &rich_edit_pos1);
	ScreenToClient(hwnd, (LPPOINT)&rich_edit_pos1);

	GetClientRect(hwnd_editor_riceive, &rich_edit_pos2);
	// 将RichEdit大小调整为客户区大小</span>
	//s.Format(_T("top = %d  bottom = %d  left = %d  right = %d"), rich_edit_rect.top, rich_edit_rect.bottom, rich_edit_rect.left, rich_edit_rect.right);
	/*
	-----------------------------------------
	|   ---------------------------------   |
	|	| ----------------------------  |	|
	|	| |   						  | |	|
	|	| |							  | |	|
	|	| |							  | |	|
	|	| |							  | |	|
	|	| |							  | |	|
	|	| |							  | |	|
	|	| |							  | |	|
	|	| |							  | |	|
	|	| |							  | |	|
	|	| |---------------------------| |	|
	|	|   -       -        ---        |	|
	|	| 	                            |   |
	|	 -------------------------------	|
	|										|
	|	|-------------------------------|	|
	|	| ----------------------------| |	|
	|	| |							  | |	|
	|	| |							  | |	|
	|	| |---------------------------	|	|
	|	| 	-   ----                    |   |
	|	|	-	----					|	|
	|	|								|	|
	|	---------------------------------	|
	-----------------------------------------
	*/

	//range include receive and send data area
	left = rich_edit_pos1.left - GAP;
	top = rich_edit_pos1.top - GAP;
	right = mian_windiw_pos.right;
	bottom = mian_windiw_pos.bottom;
	height = (bottom - top) - BORDER_GAP;
	width = (right - left) - BORDER_GAP;

	//set receive data area
	receive_left = left;
	receive_top = top;
	receive_height = height * HEIGHT_RECEIVE_PORTION / HEIGHT_TOTAL_PORTION;
	receive_width = width;
	MoveWindow(hwnd_static_receive, receive_left, receive_top, receive_width, receive_height, TRUE);

	//set receive data editor
	temp_left = receive_left + GAP;
	temp_top = receive_top + GAP;
	temp_height = (receive_height - GAP) * HEIGHT_RECEIVE_AREA_EDITOR_PORTION / HEIGHT_RECEIVE_AREA_TOTOAL_PORTION;
	temp_width = width - 2 * GAP;
	MoveWindow(hwnd_editor_riceive, temp_left, temp_top, temp_width, temp_height, TRUE);

	//receive data control area
	GetClientRect(hwnd_radio1, &radio1_pos);
	GetClientRect(hwnd_radio2, &radio2_pos);
	radio1_width = (radio1_pos.right - radio1_pos.left);
	radio2_width = (radio2_pos.right - radio2_pos.left);
	space_width = temp_width - radio1_width - radio2_width - temp_width * 2 / 7;// clear_width;

																				//radio1
	temp_height = (receive_height - GAP) * HEIGHT_RECEIVE_AREA_CONTROL_PORTION / HEIGHT_RECEIVE_AREA_TOTOAL_PORTION;
	temp_top = receive_top + GAP + (receive_height - GAP) * HEIGHT_RECEIVE_AREA_EDITOR_PORTION / HEIGHT_RECEIVE_AREA_TOTOAL_PORTION + temp_height * 1 / 10;

	MoveWindow(hwnd_radio1, temp_left, temp_top, radio1_width, temp_height * 8 / 10, TRUE);

	//radio2

	temp_left = receive_left + GAP + radio1_width + space_width / 2;
	MoveWindow(hwnd_radio2, temp_left, temp_top, radio2_width, temp_height * 8 / 10, TRUE);

	//clear command
	temp_left = receive_left + GAP + temp_width * 5 / 7;
	MoveWindow(hwnd_clear_receive, temp_left, temp_top, temp_width * 2 / 7, temp_height * 8 / 10, TRUE);


	//send  data area
	send_left = left;
	send_top = top + height * (HEIGHT_RECEIVE_PORTION + HEIGHT_GAP_PORTION) / HEIGHT_TOTAL_PORTION;
	send_height = height * HEIGHT_SEND_PORTION / HEIGHT_TOTAL_PORTION;
	send_width = width;
	MoveWindow(hwnd_static_send, send_left, send_top, send_width, send_height, TRUE);

	//comm open button
	GetWindowRect(hwnd_comm_open, &comm_open_pos);
	ScreenToClient(hwnd, (LPPOINT)&comm_open_pos);

	temp_left = comm_open_pos.left;
	temp_top = send_top + 10;

	GetClientRect(hwnd_comm_open, &comm_open_pos);
	temp_width = comm_open_pos.right - comm_open_pos.left;
	temp_height = comm_open_pos.bottom - comm_open_pos.top;
	MoveWindow(hwnd_comm_open, temp_left, temp_top, temp_width, temp_height, TRUE);

	//comm close button
	temp_top += 20 + temp_height;
	MoveWindow(hwnd_comm_close, temp_left, temp_top, temp_width, temp_height, TRUE);

	//comm fresh button
	temp_top += 20 + temp_height;
	MoveWindow(hwnd_comm_fresh, temp_left, temp_top, temp_width, temp_height, TRUE);


	//send editor
	temp_left = send_left + GAP;
	temp_top = send_top + GAP;
	temp_height = (send_height - GAP) * HEIGHT_SEND_AREA_EDITOR_PORTION / HEIGHT_SEND_AREA_TOTOAL_PORTION;
	temp_width = send_width - 2 * GAP;
	MoveWindow(hwnd_editor_send, temp_left, temp_top, temp_width, temp_height, TRUE);

	//send data control area
	GetClientRect(hwnd_radio3, &radio3_pos);
	radio3_width = (radio3_pos.right - radio3_pos.left);

	//radio3
	temp_height = (send_height - GAP) * HEIGHT_SEND_AREA_CONTROL1_PORTION / HEIGHT_SEND_AREA_TOTOAL_PORTION;
	temp_top = send_top + GAP + (send_height - GAP) *  HEIGHT_SEND_AREA_EDITOR_PORTION / HEIGHT_SEND_AREA_TOTOAL_PORTION + temp_height * 1 / 10;

	temp_width = send_width - 2 * GAP;
	MoveWindow(hwnd_radio3, temp_left, temp_top, radio3_width, temp_height * 8 / 10, TRUE);

	//send command
	temp_left = send_left + GAP + (send_width - 2 * GAP) * 5 / 7;
	temp_width = (send_width - 2 * GAP) * 2 / 7;
	MoveWindow(hwnd_send, temp_left, temp_top, temp_width, temp_height * 8 / 10, TRUE);

	//radio4
	temp_top = send_top + GAP + (send_height - GAP) * (HEIGHT_SEND_AREA_EDITOR_PORTION + HEIGHT_SEND_AREA_CONTROL1_PORTION) / HEIGHT_SEND_AREA_TOTOAL_PORTION + temp_height * 0.1;
	temp_left = send_left + GAP;
	temp_width = send_width - 2 * GAP;
	MoveWindow(hwnd_radio4, temp_left, temp_top, radio3_width, temp_height * 8 / 10, TRUE);

	//clear command
	temp_left = send_left + GAP + (send_width - 2 * GAP) * 5 / 7;
	temp_width = (send_width - 2 * GAP) * 2 / 7;
	MoveWindow(hwnd_clear_send, temp_left, temp_top, temp_width, temp_height * 8 / 10, TRUE);
	// receive data area
	/*s.Format(_T("top = %d  bottom = %d  left = %d  right = %d  top = %d  bottom = %d  left = %d  right = %d  top = %d  bottom = %d  left = %d  right = %d  "), mian_windiw_pos.top, mian_windiw_pos.bottom, mian_windiw_pos.left, mian_windiw_pos.right,

	rich_edit_pos2.top, rich_edit_pos2.bottom, rich_edit_pos2.left, rich_edit_pos2.right,
	rich_edit_pos1.top, rich_edit_pos1.bottom, rich_edit_pos1.left, rich_edit_pos1.right
	);*/

	//SetDlgItemText(hwnd, IDC_RICHEDIT22, s);

	//MessageBox(hwnd, s, TEXT("Error"), MB_OK);
	//	MoveWindow(hwnd_static_receive, left, top,  width, height, TRUE);

}


void init_layout(HWND hwnd) {
	set_candidate_comm_name(hwnd);
	set_candidate_boud_rate(hwnd);
	set_candidate_data_bit(hwnd);
	set_candidate_stop_bit(hwnd);
	set_candidate_check_bit(hwnd);
	set_candidate_data_format(hwnd);
	//加载图标文件...
	set_default_icon(hwnd);


	set_layout(hwnd);
}