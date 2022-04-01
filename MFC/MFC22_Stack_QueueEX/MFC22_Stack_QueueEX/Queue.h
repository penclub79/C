

#pragma once

class CQueue
{

private:
	int* m_piBuff;
	int m_iIndex;
	int m_iMaxSize;

protected:
	// value count üũ
	/*int GetCount();
	int GetAt(int _iIndex);*/
	
public:
	CQueue(int _iSize);
	~CQueue();

	void Push(int _iValue);
	int Pop();
	int GetCount();
	int GetAt(int _iIndex);

};