#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/sha.h>

void main() 
{
	char *str = "as7979";
	char* base64_code = NULL;
	char text[128] = { 0, };
	int	ret = 0;
	SHA_CTX sha;


	SHA1_Init(&sha);
	SHA1_Update(&sha, str, 7);
	SHA1_Final((unsigned char*)text, &sha);
}