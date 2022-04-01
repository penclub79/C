

#pragma once

class CQueue
{

public:
	CQueue(int _iSize);
	~CQueue();


	void Push(int _iValue, int _iSize);
	int Pop();

};