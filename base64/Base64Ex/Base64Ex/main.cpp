#include <stdio.h>
#include <string.h>

static const char base64code_ascii[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int text2base64_size(const char* pszText)
{
	int len = 0;

	len = strlen(pszText);
	return ((len + 2)/3 * 4);
}

// 인코딩 부분
int base64_encoder(char* pszInput, int iInputSize, char* pszOutput, int iOutputSize)
{
	int i = 0;
	int o = 0;

	if (!pszInput || !pszOutput) return 0;
	if (text2base64_size(pszInput) > iOutputSize) return 0;

	while (i < iInputSize - 2 && o + 4 <= iOutputSize)
	{
		pszOutput[o++] = base64code_ascii[(pszInput[i] >> 2) & 0x3F];
		pszOutput[o++] = base64code_ascii[((pszInput[i] & 0x3) << 4) | ((int)(pszInput[i + 1] & 0xF0) >> 4)];
		pszOutput[o++] = base64code_ascii[((pszInput[i + 1] & 0xF) << 2) | ((int)(pszInput[i + 2] & 0xC0) >> 6)];
		pszOutput[o++] = base64code_ascii[pszInput[i + 2] & 0x3F];
		i += 3;
	}

	if (i < iInputSize)
	{
		pszOutput[o++] = base64code_ascii[(pszInput[i] >> 2) & 0x3F];
		if (i == (iInputSize - 1))
		{
			pszOutput[o++] = base64code_ascii[((pszInput[i] & 0x3) << 4)];
			pszOutput[o++] = '='; // padding
		}
		else
		{
			pszOutput[o++] = base64code_ascii[((pszInput[i] & 0x3) << 4) | ((int)(pszInput[i + 1] & 0xF0) >> 4)];
			pszOutput[o++] = base64code_ascii[((pszInput[i + 1] & 0xF) << 2)];
		}

		pszOutput[o++] = '='; // padding
	}

	return o;


}

// Main
void main() {
	
	char *str = "111111";
	char base64_code[128] = { 0, };
	char text[128] = { 0, };
	int ret = 0;

	printf("%s\n", str);

	ret = base64_encoder(str, strlen(str), base64_code, 128);
	printf("base64 encoding : %s\n", base64_code);


}