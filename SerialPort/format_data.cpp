#include "stdafx.h"

BOOL convent_to_hex(TCHAR *p, DWORD len) {

	UINT32 i = 0;

	if (NULL == p || len < 0 || len > MAX_BUFFER_SIZE) {
		return FALSE;
	}

	for (i = 0; i < len; p++,i++) {
		if ('0' <= *p && *p <= '9') {
			*p = *p - '0';
		}
		else if ('A' <= *p && *p <= 'F') {
			*p = *p - 'A' + 10;
		}
		else if ('a' <= *p && *p <= 'f') {
			*p = *p - 'a' + 10;
		}
		else {//对有非法字符进行标记
			return FALSE;
		}
	}
	return TRUE;
}