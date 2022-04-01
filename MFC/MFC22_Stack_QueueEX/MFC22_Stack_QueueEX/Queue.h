

#pragma once

class CQueue
{

public:
	CQueue();
	~CQueue();


	void Push(int _iValue, int _iSize);
	int Pop();

};