// test.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <openssl\aes.h>

int _tmain(int argc, _TCHAR* argv[])
{	
	AES_encrypt(NULL, NULL, NULL);
	return 0;
}