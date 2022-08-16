#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>
//#include <openssl/sha.h>


static const char base64code_ascii[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int text2base64_size(const char* pszText)
{
	int len = 0;

	len = strlen(pszText);
	return ((len + 2)/3 * 4);
}

//// 인코딩 부분
//int base64_encoder(char* pszInput, int iInputSize, char* pszOutput, int iOutputSize)
//{
//	int i = 0;
//	int o = 0;
//
//	if (!pszInput || !pszOutput) return 0;
//	if (text2base64_size(pszInput) > iOutputSize) return 0;
//
//	while (i < iInputSize - 2 && o + 4 <= iOutputSize)
//	{
//		pszOutput[o++] = base64code_ascii[(pszInput[i] >> 2) & 0x3F];
//		pszOutput[o++] = base64code_ascii[((pszInput[i] & 0x3) << 4) | ((int)(pszInput[i + 1] & 0xF0) >> 4)];
//		pszOutput[o++] = base64code_ascii[((pszInput[i + 1] & 0xF) << 2) | ((int)(pszInput[i + 2] & 0xC0) >> 6)];
//		pszOutput[o++] = base64code_ascii[pszInput[i + 2] & 0x3F];
//		i += 3;
//	}
//
//	if (i < iInputSize)
//	{
//		pszOutput[o++] = base64code_ascii[(pszInput[i] >> 2) & 0x3F];
//		if (i == (iInputSize - 1))
//		{
//			pszOutput[o++] = base64code_ascii[((pszInput[i] & 0x3) << 4)];
//			pszOutput[o++] = '='; // padding
//		}
//		else
//		{
//			pszOutput[o++] = base64code_ascii[((pszInput[i] & 0x3) << 4) | ((int)(pszInput[i + 1] & 0xF0) >> 4)];
//			pszOutput[o++] = base64code_ascii[((pszInput[i + 1] & 0xF) << 2)];
//		}
//
//		pszOutput[o++] = '='; // padding
//	}
//
//	return o;
//
//
//}

/*------ Base64 Encoding Table ------*/
static const char MimeBase64[] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
	'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
	'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
	'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
	'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
	'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
	'w', 'x', 'y', 'z', '0', '1', '2', '3',
	'4', '5', '6', '7', '8', '9', '+', '/'
};

int base64_encode(char *text, int numBytes, char **encodedText)
{
	unsigned char input[3] = { 0, 0, 0 };
	unsigned char output[4] = { 0, 0, 0, 0 };
	int   index, i, j, size;
	int	iLen = 0;
	char *p, *plen;
	wchar_t strUnicode[256] = { 0, };
	char strUtf8[256] = { 0, };


	plen = text + numBytes - 1;
	size = (4 * (numBytes / 3)) + (numBytes % 3 ? 4 : 0) + 1;
	(*encodedText) = (char*)malloc(size);
	memset(*encodedText, 0, sizeof(char) * size);
	
	//// 유니코드로 변형 
	//iLen = MultiByteToWideChar(CP_ACP, 0, text, strlen(text), NULL, NULL);
	//MultiByteToWideChar(CP_ACP, 0, text, strlen(text), strUnicode, iLen);
	//
	//// 유니코드 -> utf-8
	//iLen = WideCharToMultiByte(CP_UTF8, 0, strUnicode, lstrlenW(strUnicode), NULL, 0, NULL, NULL);
	//WideCharToMultiByte(CP_UTF8, 0, strUnicode, lstrlenW(strUnicode), strUtf8, iLen, NULL, NULL);

	j = 0;
	for (i = 0, p = text; p <= plen; i++, p++) {
		index = i % 3;
		input[index] = *p;
		if (index == 2 || p == plen) {
			output[0] = ((input[0] & 0xFC) >> 2);
			output[1] = ((input[0] & 0x3) << 4) | ((input[1] & 0xF0) >> 4);
			output[2] = ((input[1] & 0xF) << 2) | ((input[2] & 0xC0) >> 6);
			output[3] = (input[2] & 0x3F);
			(*encodedText)[j++] = MimeBase64[output[0]];
			(*encodedText)[j++] = MimeBase64[output[1]];
			(*encodedText)[j++] = index == 0 ? '=' : MimeBase64[output[2]];
			(*encodedText)[j++] = index <  2 ? '=' : MimeBase64[output[3]];
			input[0] = input[1] = input[2] = 0;
		}
	}

	(*encodedText)[j] = '\0';

	return size;
}

/*------ Base64 Decoding Table ------*/
static int DecodeMimeBase64[256] = {
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  /* 00-0F */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  /* 10-1F */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,  /* 20-2F */
	52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,  /* 30-3F */
	-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,  /* 40-4F */
	15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,  /* 50-5F */
	-1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,  /* 60-6F */
	41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,  /* 70-7F */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  /* 80-8F */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  /* 90-9F */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  /* A0-AF */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  /* B0-BF */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  /* C0-CF */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  /* D0-DF */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  /* E0-EF */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1   /* F0-FF */
};


int base64_decode(char * text, unsigned char * dst, int numBytes)
{
	const char* cp;
	int space_idx = 0, phase;
	int d, prev_d = 0;
	unsigned char c;
	space_idx = 0;
	phase = 0;
	for (cp = text; *cp != '\0'; ++cp) {
		d = DecodeMimeBase64[(int)*cp];
		if (d != -1) {
			switch (phase) {
			case 0:
				++phase;
				break;
			case 1:
				c = ((prev_d << 2) | ((d & 0x30) >> 4));
				if (space_idx < numBytes)
					dst[space_idx++] = c;
				++phase;
				break;
			case 2:
				c = (((prev_d & 0xf) << 4) | ((d & 0x3c) >> 2));
				if (space_idx < numBytes)
					dst[space_idx++] = c;
				++phase;
				break;
			case 3:
				c = (((prev_d & 0x03) << 6) | d);
				if (space_idx < numBytes)
					dst[space_idx++] = c;
				phase = 0;
				break;
			}
			prev_d = d;
		}
	}
	return space_idx;
}


// Main
void main() {
	char str[16] = "111111";
	char* base64_code = NULL;
	unsigned char text[128] = { 0, };
	int	ret = 0;
	//SHA_CTX sha;

	//printf("%s\n", str);
	base64_encode(str, strlen(str), &base64_code);
	//ret = base64_encoder(str, strlen(str), base64_code, 128);
	ret = base64_decode(base64_code, &text[0], 128);
	printf("base64 encoding : %s\n", base64_code);

	//unsigned char digest[SHA_DIGEST_LENGTH];
	//char string[] = "hello world";

	//SHA1((unsigned char*)&string, strlen(string), (unsigned char*)&digest);

	//char mdString[SHA_DIGEST_LENGTH * 2 + 1];

	//for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
	//	sprintf(&mdString[i * 2], "%02x", (unsigned int)digest[i]);

	//printf("SHA1 digest: %s\n", mdString);

}