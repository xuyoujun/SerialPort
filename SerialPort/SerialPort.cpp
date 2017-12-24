#include "stdafx.h"
#include <windows.h>
#include <windowsx.h>
#include "resource.h"
#include "SerialPort.h"
#include <commdlg.h>
#include <stdio.h>
#include <atlstr.h>
#include "OpenSerialPort.h"

#define MAX_BUFFER_SIZE        (1024)
#define MAX_COMM_NUM           (32)
						       
#define MAX_BOUD_BUFFER        (16)
#define GAP                    (20)
#define BORDER_GAP             (30)
#define SEND_EDITOR_HEIGHT     (40)
#define HEIGHT_TOTAL_PORTION   (11)

static HANDLE com_handler = INVALID_HANDLE_VALUE;//串口句柄
static TCHAR  visibla[10000] = { 0 };//

BOOL WINAPI Main_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
		
        HANDLE_MSG(hWnd, WM_INITDIALOG, Main_OnInitDialog);
        HANDLE_MSG(hWnd, WM_COMMAND, Main_OnCommand);
		HANDLE_MSG(hWnd, WM_SIZE, Main_OnSize);
		HANDLE_MSG(hWnd, WM_CLOSE, Main_OnClose);
    }

    return FALSE;
}

BOOL get_comm_name(LPBYTE buffer, UINT32 num) {
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


BOOL set_candidate_comm_name(HWND hwnd) {
	UINT32 i = 0;
	HWND  hwnd_comm_sel;
	BOOL result = FALSE;
	BYTE comm_name[32] = { 0 };
	hwnd_comm_sel = GetDlgItem(hwnd, IDC_COMBOSERIAL);//添加串口选项
	for (i = 0; i < MAX_COMM_NUM; i++) {
		result = get_comm_name(comm_name, i);
		if (FALSE == result) {
			break;
		}
		ComboBox_InsertString(hwnd_comm_sel, -1, comm_name);
		ZeroMemory(comm_name, sizeof(comm_name));
	}
	ComboBox_SetCurSel(hwnd_comm_sel, i / 2);
	return TRUE;
}


BOOL set_candidate_boud_rate(HWND hwnd) {
	UINT32 i = 0;
	HWND  hwnd_boud_sel;
	TCHAR  buffer[MAX_BOUD_BUFFER];
	UINT32 boud_rate[] = {600,	1200,	2400,	4800,	9600,	19200,
						38400,	56000,	57600,	115200,	128000,	256000,	1440000};
	hwnd_boud_sel = GetDlgItem(hwnd, IDC_COMBOBOUDRATE);//添加波特率选项
	for (i = 0; i < sizeof(boud_rate) / sizeof(boud_rate[0]); i++) {
		itoa(boud_rate[i], buffer, 10);
		ComboBox_InsertString(hwnd_boud_sel, -1, buffer);
	}
	ComboBox_SetCurSel(hwnd_boud_sel, 4);
	return TRUE;
}


BOOL set_candidate_data_bit(HWND hwnd) {
	HWND hwndDate = GetDlgItem(hwnd, IDC_DATE);//数据位选项
	ComboBox_InsertString(hwndDate, -1, "4");
	ComboBox_InsertString(hwndDate, -1, "5");
	ComboBox_InsertString(hwndDate, -1, "6");
	ComboBox_InsertString(hwndDate, -1, "7");
	ComboBox_InsertString(hwndDate, -1, "8");
	ComboBox_SetCurSel(hwndDate, 4);
	return TRUE;
}

BOOL set_candidate_stop_bit(HWND hwnd) {
	HWND hwndStop = GetDlgItem(hwnd, IDC_STOP);//停止位选项
	ComboBox_InsertString(hwndStop, -1, "1");
	ComboBox_InsertString(hwndStop, -1, "2");
	ComboBox_SetCurSel(hwndStop, 0);
	return TRUE;
}

BOOL set_candidate_check_bit(HWND hwnd) {

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

BOOL set_candidate_data_format(HWND hwnd) {
	
	HWND receiveHex = GetDlgItem(hwnd, IDC_RADIO1);//接收_得到16进制的按钮句柄
	Button_SetCheck(receiveHex, 1);//设置默认选项
	HWND sendHex = GetDlgItem(hwnd, IDC_RADIO3);//发送_得到16进制的按钮句柄
	Button_SetCheck(sendHex, 1);//设置默认选项
	return TRUE;
}

BOOL set_default_icon(HWND hwnd) {
	HINSTANCE hInstance = GetModuleHandle(NULL);
	HICON hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(SerialPort));
	SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
	return TRUE;
}




HANDLE Open_Serial_Port(TCHAR com_name[]){

	HANDLE hCom;  //串口句柄
	hCom = CreateFile(com_name,  //COM口
				GENERIC_READ | GENERIC_WRITE, //允许读和写
				0,  //独占方式
				NULL,
				OPEN_EXISTING,  //打开而不是创建
				FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, //重叠方式
				NULL
			);

	if (INVALID_HANDLE_VALUE == hCom){
		MessageBox(NULL, TEXT("打开COM失败!"), TEXT("Error"), NULL);
		return hCom;
	}
	return hCom;
}



BOOL convent_to_hex(TCHAR *buffer, DWORD len) {

	DWORD i = 0;

	if (NULL == buffer || len < 0 || len > MAX_BUFFER_SIZE) {
		return false;
	}

	for (i = 0; i < len; i++) {
		if ('0' <= buffer[i] && buffer[i] <= '9') {//对0-9之间的数字进行转换
			buffer[i] = buffer[i] - '0';
		}
		else if ('A' <= buffer[i] && buffer[i] <= 'F') {//对A-F之间的数据进行转换
			buffer[i] = buffer[i] - 'A' + 10;
		}
		else if ('a' <= buffer[i] && buffer[i] <= 'f') {//对a-f之间的数据进行转换	
			buffer[i] = buffer[i] - 'a' + 10;
		}
		else {//对有非法字符进行标记
			return false;
		}
	}
	return true;
}

DWORD WINAPI Receive_Serial_Port_Thread(LPVOID lpParam)//用于读线程的函数
{
	HWND    hwnd = (HWND)lpParam; //获得串口句柄
	TCHAR   lpInBuffer[MAX_BUFFER_SIZE]; //设置读缓冲区
	DWORD   dwBytesRead = MAX_BUFFER_SIZE;//读取到的字节数
	BOOL    bReadStatus;//读是否成功
	DWORD   dwErrorFlags;
	COMSTAT ComStat;//对串口具有监视作用
	OVERLAPPED m_osRead;
	HWND hwnd_hex = GetDlgItem(hwnd, IDC_RADIO1);//接收_得到16进制的按钮句柄
	HWND hwnd_richedit = GetDlgItem(hwnd, IDC_RICHEDIT22);

	INT hex_format = 0;//接收_得到句柄的状态
	UINT32 i = 0;
	while (1){
		memset(&m_osRead, 0, sizeof(OVERLAPPED));
		memset(lpInBuffer, 0, sizeof(lpInBuffer));
		m_osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		ClearCommError(com_handler, &dwErrorFlags, &ComStat);
		hex_format = Button_GetCheck(hwnd_hex);

		if (!ComStat.cbInQue){//读缓冲区没有数据，即0==ComStat.cbInQue////////////////////////////////
			Sleep(50);//这个函数很重要，如果没有的话cup的使用率会很高（大约30%），用过之后会降低很多（3%左右）。不过在传输数据很频繁的系统，休眠时间要尽量短，以免影响效率。
			continue;
		}
		else{//读缓冲区有数据，即0！=ComStat.cbInQue
			dwBytesRead = (DWORD)ComStat.cbInQue;//得到读缓冲区的字节数
			bReadStatus = ReadFile(com_handler, lpInBuffer,
							dwBytesRead, &dwBytesRead, &m_osRead);//进行读串口处理
			if (!bReadStatus && (GetLastError() == ERROR_IO_PENDING)){ //如果ReadFile函数返回FALSE
				GetOverlappedResult(com_handler, &m_osRead, &dwBytesRead, TRUE);
				PurgeComm(com_handler,
					PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
				if (1 == hex_format) {//发16进制数据
					convent_to_hex(lpInBuffer, strlen(lpInBuffer));
				}
				
				SendMessage(hwnd_richedit, EM_SETSEL, -1, -1);
				SendMessage(hwnd_richedit, WM_VSCROLL,SB_BOTTOM,0);
				SendMessage(hwnd_richedit, EM_REPLACESEL, 0, (LPARAM)lpInBuffer);
				SendMessage(hwnd_richedit, EM_REPLACESEL, 0, (LPARAM)L"\r\n");
				//SetDlgItemText(hwnd, IDC_RICHEDIT22, strcat(visibla, lpInBuffer));//显示在richedit中
				CloseHandle(m_osRead.hEvent);
			}
			else{// //ReadFile函数返回TRUE
				if (1 == hex_format){//接收16进制数据
					convent_to_hex(lpInBuffer, strlen(lpInBuffer));
				}
				SendMessage(hwnd_richedit, EM_SETSEL, -1, -1);
				SendMessage(hwnd_richedit, WM_VSCROLL, SB_BOTTOM, 0);
				SendMessage(hwnd_richedit, EM_REPLACESEL, 0, (LPARAM)lpInBuffer);
				SendMessage(hwnd_richedit, EM_REPLACESEL, 0, (LPARAM)L"\r\n");
				//SetDlgItemText(hwnd, IDC_RICHEDIT22, strcat(visibla, lpInBuffer));//显示在richedit中
				PurgeComm(com_handler, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
				CloseHandle(m_osRead.hEvent);
			}
		}
	}
	return 0;
}

BOOL Send_Serial_Port(HWND hwnd) {

	HWND  hwnd_hex;
	INT   hex_format;
	TCHAR buffer[MAX_BUFFER_SIZE];//准备写入的数据
	DWORD len = sizeof(buffer);         //要写入数据的字节
	BOOL  check_flag = true;//是否有非法字符的标志
	BOOL  bWriteStat = FALSE;//用于判断写入是否成功
	OVERLAPPED m_osWrite = { 0 };

	if (com_handler == INVALID_HANDLE_VALUE) {
		MessageBox(hwnd, TEXT("Invalid Port Number"), TEXT("Error"), MB_OK);
	}
	else {

		hwnd_hex   = GetDlgItem(hwnd, IDC_RADIO3);//发送_得到16进制的按钮句柄
		hex_format = Button_GetCheck(hwnd_hex);//得到句柄的状态
		PurgeComm(com_handler, PURGE_TXCLEAR);//清空写缓冲区
		ZeroMemory(buffer, sizeof(buffer));
		GetDlgItemText(hwnd, IDC_EDIT2, buffer, sizeof(buffer));//得到要写入的数据
		len = strlen(buffer);//要写入数据的长度
		if (1 == hex_format) {//发送16进制数据
			check_flag = convent_to_hex(buffer, len);
		}
	
		if (!check_flag){//判断有非法字符
			MessageBox(hwnd, TEXT("请输入的16进制在0-9或a-f或A-F之间"), TEXT("Error"), MB_OK);
		}
		else{
			m_osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
			bWriteStat = WriteFile(com_handler, buffer,len, &len, &m_osWrite);
			if (!bWriteStat){
				if (GetLastError() == ERROR_IO_PENDING){
					WaitForSingleObject(m_osWrite.hEvent, 1000);
				}
			}
			CloseHandle(m_osWrite.hEvent);
		}
	}
	return true;
}
BOOL Set_Serial_Port(HWND hwnd) {
	//HWND hwndIDOK    = GetDlgItem(hwnd, IDOK);//隐蔽打开串口按钮
	//ShowWindow(hwndIDOK,SW_HIDE);
	//HWND hwndIDCLOSE = GetDlgItem(hwnd, IDCLOSE);//解除对关闭串口按钮的隐蔽
	//ShowWindow(hwndIDCLOSE,SW_SHOW);
	HWND hwnd_data_bit;  //数据位 handler
	HWND hwnd_baud_rate; //波特率 handler
	HWND hwnd_check_bit; //校验位 handler
	HWND hwnd_stop_bit;  //停止位 handler
	TCHAR buffer[10] = {0};
	INT   index      = 0;
	DCB   dcb;
	COMMTIMEOUTS TimeOuts;

	SetupComm(com_handler, 1024, 1024); //输入缓冲区和输出缓冲区的大小都是1024
	
	TimeOuts.ReadIntervalTimeout         = 1000;//设定读超时
	TimeOuts.ReadTotalTimeoutMultiplier  = 500;
	TimeOuts.ReadTotalTimeoutConstant    = 5000;

	TimeOuts.WriteTotalTimeoutMultiplier = 500;//设定写超时
	TimeOuts.WriteTotalTimeoutConstant   = 2000;
	SetCommTimeouts(com_handler, &TimeOuts);    //设置超时

	/* Get current comm port state */
	GetCommState(com_handler, &dcb);
	dcb.DCBlength = sizeof(dcb);

	/* Get data bit config */
	ZeroMemory(buffer, sizeof(buffer));
	hwnd_data_bit = GetDlgItem(hwnd, IDC_DATA_BIT);
	index         = ComboBox_GetCurSel(hwnd_data_bit);//得到现在的选项的索引值
	ComboBox_GetLBText(hwnd_data_bit, index, buffer);//得到索引值的内容
	dcb.ByteSize  = atoi(buffer);    //每个字节有8位

	/* Get stop bit config */
	ZeroMemory(buffer, sizeof(buffer));
	hwnd_stop_bit = GetDlgItem(hwnd, IDC_STOP_BIT);
	index         = ComboBox_GetCurSel(hwnd_stop_bit);//得到现在的选项的索引值
	ComboBox_GetLBText(hwnd_stop_bit, index, buffer);//得到索引值的内容
	dcb.StopBits  = atoi(buffer) - 1; //停止位

	/* Get boud rate config */
	ZeroMemory(buffer, sizeof(buffer));
	hwnd_baud_rate = GetDlgItem(hwnd, IDC_BOUD_RATE);//设置波特率
	index          = ComboBox_GetCurSel(hwnd_baud_rate);
	ComboBox_GetLBText(hwnd_baud_rate, index, buffer);
	dcb.BaudRate   = atoi(buffer);       //波特率

	/*Get check bit config*/
	ZeroMemory(buffer, sizeof(buffer));
	hwnd_check_bit = GetDlgItem(hwnd, IDC_CHECK_BIT);//设置校检位
	index          = ComboBox_GetCurSel(hwnd_check_bit);
	ComboBox_GetLBText(hwnd_check_bit, index, buffer);
	dcb.Parity     = *buffer; //有无奇偶校验位

	/* Set current comm port state */
	SetCommState(com_handler, &dcb);
	//PurgeComm(com_handler,PURGE_TXCLEAR|PURGE_RXCLEAR);
	//SetCommMask(com_handler,);

	/* Create a thread to receive data */
	CreateThread(NULL, 0, Receive_Serial_Port_Thread, hwnd, 0, 0);//开始读线程
	return true;
}


BOOL Close_Serial_Port(HWND hwnd) {
	BOOL close_flag    = false;
	HWND hwnd_ID_OPEN  = NULL;
	HWND hwnd_ID_CLOSE = NULL;
	close_flag  = CloseHandle(com_handler);//关闭串口
	com_handler = INVALID_HANDLE_VALUE;
	//hwnd_ID_OPEN  = GetDlgItem(hwnd, ID_OPEN);//解除对打开串口按钮的隐蔽
	//ShowWindow(hwnd_ID_OPEN, SW_SHOW);
	//hwnd_ID_CLOSE = GetDlgItem(hwnd, ID_CLOSE);//使关闭串口按钮隐蔽
	//ShowWindow(hwndIDCLOSE,SW_HIDE);
	return close_flag;
	//ShowWindow(hwnd,SW_HIDE);
}


BOOL Main_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	//HWND hwndIDCLOSE = GetDlgItem(hwnd,IDCLOSE);//获得关闭按钮的句柄
	//ShowWindow(hwndIDCLOSE,SW_HIDE);//先隐蔽关闭串口选项
	//ShowWindow(hwndIDOK,SW_);
	BOOL result = FALSE;
	set_candidate_comm_name(hwnd);
	set_candidate_boud_rate(hwnd);
	set_candidate_data_bit(hwnd);
	set_candidate_stop_bit(hwnd);
	set_candidate_check_bit(hwnd);
	set_candidate_data_format(hwnd);
	//加载图标文件...
	set_default_icon(hwnd);
	return TRUE;
}


void Main_OnSize(HWND hwnd, UINT state, INT cx, INT cy)
{

	int left   = 0;
	int top    = 0;
	int right  = 0;
	int bottom = 0;
	int height = 0;
	int width  = 0;

	int receive_left = 0;
	int receive_top = 0;
	int receive_right = 0;
	int receive_bottom = 0;
	int receive_height = 0;
	int receive_width = 0;

	int temp_left   = 0;
	int temp_top    = 0;
	int temp_right  = 0;
	int temp_bottom = 0;
	int temp_height = 0;
	int temp_width  = 0;

	CString s;
	RECT mian_windiw_pos;
	RECT radio1_pos;
	RECT radio2_pos;
	RECT clear_pos;

	RECT rich_edit_pos1;
	RECT rich_edit_pos2;
	HWND hwnd_editor_riceive;
	HWND hwnd_editor_send;
	HWND hwnd_static_receive;
	HWND hwnd_static_send;
	HWND hwnd_radio1;
	HWND hwnd_radio2;
	HWND hwnd_clear_receive;

	hwnd_editor_riceive = GetDlgItem(hwnd, IDC_RICHEDIT22);
	hwnd_static_send    = GetDlgItem(hwnd, IDC_STATIC_SEND);
	hwnd_static_receive = GetDlgItem(hwnd, IDC_STATIC_RECEIVE);
	hwnd_editor_send    = GetDlgItem(hwnd, IDC_EDIT2);
	hwnd_radio1         = GetDlgItem(hwnd, IDC_RADIO1);
	hwnd_radio2         = GetDlgItem(hwnd, IDC_RADIO2);
	hwnd_clear_receive  = GetDlgItem(hwnd, IDC_CLEAR_RECEIVE);

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
	left   = rich_edit_pos1.left - GAP;
	top    = rich_edit_pos1.top  - GAP;
	right  = mian_windiw_pos.right;
	bottom = mian_windiw_pos.bottom;
	height = (bottom - top) - BORDER_GAP;
	width  = (right - left) - BORDER_GAP;

	//set receive data area
	receive_left   = left;
	receive_top    = top;
	receive_height = height * 7/11;
	receive_width  = width;
	MoveWindow(hwnd_static_receive, receive_left, receive_top, receive_width, receive_height , TRUE);
	
	temp_left   = receive_left + GAP;
	temp_top    = receive_top + GAP;
	temp_height = (receive_height - GAP) * 0.85;
	temp_width  = width - 2 * GAP;
	MoveWindow(hwnd_editor_riceive, temp_left, temp_top, temp_width, temp_height, TRUE);
	

	GetClientRect(hwnd_radio1, &radio1_pos);
	GetClientRect(hwnd_radio1, &radio2_pos);
	GetClientRect(hwnd_radio1, &clear_pos);

	int radio1_width = (radio1_pos.right - radio1_pos.left);
	int radio2_width = (radio2_pos.right - radio2_pos.left);
	int clear_width  = (clear_pos.right - clear_pos.left);
	int space_width = temp_width - radio1_width - radio2_width - temp_width / 6;// clear_width;


	temp_left = receive_left + GAP;
	temp_height = (receive_height - GAP) * 0.15;
	temp_top = receive_top + GAP + (receive_height - GAP) * 0.85 + temp_height * 0.1;


	MoveWindow(hwnd_radio1, temp_left, temp_top, radio1_width, temp_height * 0.8, TRUE);

	
	temp_left = receive_left + GAP + radio1_width + space_width / 2;
	//temp_top = top + temp_height + GAP + 10;
	MoveWindow(hwnd_radio2, temp_left, temp_top, radio2_width, temp_height * 0.8, TRUE);

	temp_left = receive_left + GAP + temp_width * 5/6;
	//temp_top = top + temp_height + GAP + 10;
	MoveWindow(hwnd_clear_receive, temp_left, temp_top, temp_width /6, temp_height * 0.8, TRUE);


	//set send data area
	temp_left = left;
	temp_top = top + (height * 8 / 11);
	temp_height = height * 3 / 11;
	temp_width = width;
	MoveWindow(hwnd_static_send, temp_left, temp_top, temp_width, temp_height, TRUE);


	temp_left = temp_left + GAP;
	temp_top  = temp_top + GAP;
	temp_height = SEND_EDITOR_HEIGHT;
	temp_width = temp_width - 2 *GAP;
	MoveWindow(hwnd_editor_send, temp_left, temp_top, temp_width, temp_height, TRUE);
	

	// receive data area
	/*s.Format(_T("top = %d  bottom = %d  left = %d  right = %d  top = %d  bottom = %d  left = %d  right = %d  top = %d  bottom = %d  left = %d  right = %d  "), mian_windiw_pos.top, mian_windiw_pos.bottom, mian_windiw_pos.left, mian_windiw_pos.right,
		
		rich_edit_pos2.top, rich_edit_pos2.bottom, rich_edit_pos2.left, rich_edit_pos2.right,
		rich_edit_pos1.top, rich_edit_pos1.bottom, rich_edit_pos1.left, rich_edit_pos1.right
		);*/

	//SetDlgItemText(hwnd, IDC_RICHEDIT22, s);

	//MessageBox(hwnd, s, TEXT("Error"), MB_OK);
//	MoveWindow(hwnd_static_receive, left, top,  width, height, TRUE);
}

void Main_OnCommand(HWND hwnd, int command, HWND hwndCtl, UINT codeNotify)
{
	HWND  hwnd_comm; //获得串口组件的句柄
	TCHAR comm_name[256];	
	INT   index;
	BOOL  close_flag = false;

	switch(command){
		case ID_OPEN:
			ZeroMemory(comm_name, sizeof(comm_name));
			hwnd_comm   = GetDlgItem(hwnd, IDC_COMBOSERIAL);//获得串口组件的句柄
			index       = ComboBox_GetCurSel(hwnd_comm);//得到串口组件现在的选项的索引值
			/* Get comm name */
			ComboBox_GetLBText(hwnd_comm, index, comm_name);//得到索引值的内容
			com_handler = Open_Serial_Port(comm_name);//打开串口
			
			if (com_handler != INVALID_HANDLE_VALUE) {
				Set_Serial_Port(hwnd);
			}
			break;

		case ID_CLOSE:
			Close_Serial_Port(hwnd);
			break;

		case IDC_SEND:
			//判断端口是否打开
			Send_Serial_Port(hwnd);
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

void Main_OnClose(HWND hwnd)
{
    EndDialog(hwnd,0);
}

