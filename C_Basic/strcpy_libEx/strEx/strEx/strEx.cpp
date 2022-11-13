#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#pragma warning(disable:4996)
using namespace std;

#define BUFFER_SIZE 100


int main() 
{
	char* words[100];
	int n = 0;
	char buffer[BUFFER_SIZE];
	char* copyBuffer = NULL;

	while (n < 4 && scanf("%s", buffer) != EOF)
	{
		//words[n] = strdup(buffer);

		copyBuffer = new char[strlen(buffer) + 1];
		words[n] = strcpy(copyBuffer, buffer);
		n++;
	}

	for (int i = 0; i < 4; i++)
	{
		printf("%s\n", words[i]);
	}

	if (copyBuffer != NULL)
	{
		delete[] copyBuffer;
		copyBuffer = NULL;
	}

	return 0;
}