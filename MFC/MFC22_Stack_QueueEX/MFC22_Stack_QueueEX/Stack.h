#pragma once

class CStack 
{
	
public:
	CStack(int _iSize);
	~CStack();


	void Push(int _iValue);
	int Pop();
	int GetCount();  // value count 체크
	int GetAt(int _iIndex);  // value를 가져온다
	int GetIndex();

private:
	int* m_piBuff;
	int m_iIndex;
	int m_iCount;
	int m_iMaxSize;

};