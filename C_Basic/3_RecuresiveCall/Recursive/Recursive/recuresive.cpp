#include <stdio.h>

int recuresive(int iNum)
{
	// for문 처럼 조건을 주어야한다. (핵심)

	if (0 == iNum) // 재귀호출 멈추는 조건
		return 0;
	
	printf("%d\n", iNum);

	return recuresive(iNum - 1);
}

int main(void)
{
	recuresive(5);
	return 0;
}