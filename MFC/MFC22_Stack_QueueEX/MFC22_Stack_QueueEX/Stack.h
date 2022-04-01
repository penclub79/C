#pragma once

class CStack 
{

private:
	int* m_piBuff;
	int m_iIndex;
	int m_iMaxSize;

public:
	CStack(int _iSize);
	~CStack();

	void Push(int _iValue);
	int Pop();
	int GetCount();  // value count üũ
	int GetAt(int _iIndex);  // value�� �����´�

};