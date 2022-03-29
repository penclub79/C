#pragma once


class CSearch
{
protected:
	BOOL IsStrMatch();// 글자만 비교하는 함수
public:
	CSearch();
	~CSearch();

	void InputValue(int* _piBuffer, int _iCount, int _iValue);
	void ValueAsc(int* _piBuffer, int _iCount);
	int SumResult(int* _piBuffer, int _iCount);
	//void GetValue(int* _piBuffer, struct St_Test st);
	//BOOL IsStrFind(char* A_PtrFindStr, int A_FindStrSize, char* A_PtrTextStr, int A_TextSize); // 찾으려는 글자, 찾으려는 글자 사이즈, 파일의 텍스트, 텍스트 사이즈
	
};