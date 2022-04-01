

#pragma once

class CStack
{
	
public:
	CStack(int _iSize);
	~CStack();

	int* m_piBuff;
	int m_iIndex;

	void Push(int _iValue);
	int Pop();

};