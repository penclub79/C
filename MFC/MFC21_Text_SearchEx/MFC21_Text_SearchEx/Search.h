#pragma once


typedef struct St_Test
{
	int test0;
	int test100;
	int test200;
	int test300;
}St_Test;

class CSearch
{
protected:
	BOOL IsStrMatch();// ���ڸ� ���ϴ� �Լ�
public:
	CSearch();
	~CSearch();

	void InputValue(int* _piBuffer, int _iCount, int _iValue);
	void ValueAsc(int* _piBuffer, int _iCount);
	int SumResult(int* _piBuffer, int _iCount);
	void GetValue(int* _piBuffer, int _iCount, struct St_Test* _stResult );
	//BOOL IsStrFind(char* A_PtrFindStr, int A_FindStrSize, char* A_PtrTextStr, int A_TextSize); // ã������ ����, ã������ ���� ������, ������ �ؽ�Ʈ, �ؽ�Ʈ ������
	
};