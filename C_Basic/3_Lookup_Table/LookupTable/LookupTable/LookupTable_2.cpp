#include <stdio.h>


int CountCircle(int iCount)
{
	int iIdx = 0;
	const int aiArr[] = { 1, 0, 0, 0, 0, 0, 1, 0, 2, 1 };
	while (iCount > 0)
	{
		iIdx += aiArr[iCount % 10];
		iCount /= 10;
	}
	
	return iIdx;
}

int main()
{
	
	CountCircle(2);

	return 0;
}