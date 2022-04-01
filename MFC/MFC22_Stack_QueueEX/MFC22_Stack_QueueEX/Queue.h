

#pragma once

class CQueue
{

private:
	int* m_piBuff;
	int m_iRear;
	int m_iFront;
	int m_iMaxSize;

protected:
	// value count üũ
	/*int GetCount();
	int GetAt(int _iIndex);*/
	
public:
	CQueue(int _iSize);
	~CQueue();

	void EnQueue(int _iValue);
	int DeQueue();
	int GetCount();
	int GetAt(int _iIndex);
	int GetFront();

};