#pragma once


class CSearch
{
protected:
	BOOL IsStrMatch();// 글자만 비교하는 함수
public:
	CSearch();
	~CSearch();

	BOOL IsStrFind(char* A_PtrFindStr, int A_FindStrSize, char* A_PtrTextStr, int A_TextSize); // 찾으려는 글자, 찾으려는 글자 사이즈, 파일의 텍스트, 텍스트 사이즈
};