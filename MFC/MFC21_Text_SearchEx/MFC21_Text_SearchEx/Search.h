#pragma once


class CSearch
{
protected:
	BOOL IsStrMatch();// ���ڸ� ���ϴ� �Լ�
public:
	CSearch();
	~CSearch();

	BOOL IsStrFindGet(char* A_PtrFindStr, int A_FindStrSize, char* A_PtrTextStr, int A_TextSize); // ã������ ����, ã������ ���� ������, ������ �ؽ�Ʈ, �ؽ�Ʈ ������
};