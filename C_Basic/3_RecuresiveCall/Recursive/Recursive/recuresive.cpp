#include <stdio.h>

int recuresive(int iNum)
{
	// for�� ó�� ������ �־���Ѵ�. (�ٽ�)

	if (0 == iNum) // ���ȣ�� ���ߴ� ����
		return 0;
	
	printf("%d\n", iNum);

	return recuresive(iNum - 1);
}

int main(void)
{
	recuresive(5);
	return 0;
}