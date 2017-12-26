#include "stdafx.h"
#include <windows.h>
#include <windowsx.h>
#include "resource.h"
#include "common.h"
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