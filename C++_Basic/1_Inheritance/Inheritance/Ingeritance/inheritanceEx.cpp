#include <iostream>
#include <stdlib.h>

using namespace std;

class Car
{
private:
	char* pszCarName;

protected:
	int iSpeed;

public:
	Car(){}
	Car(const char* _pszCarName)
	{
		pszCarName = new char[strlen(_pszCarName) + 1];
		strcpy(pszCarName, _pszCarName);
	}
	~Car(){}

	char* getName()
	{
		return pszCarName;
	}

	int getSpeed()
	{
		return iSpeed;
	}

	virtual void speedUp()
	{
		iSpeed += 10;
	}

	virtual void speedDown()
	{
		iSpeed -= 10;
	}
};

class Sedan : public Car
{
public:
	Sedan(const char* _pszCarName, int _iSpeed) : Car(_pszCarName)
	{
		this->iSpeed = _iSpeed;
	}
	~Sedan(){}

	virtual void speedUp()
	{
		this->iSpeed += 20;
	}

	virtual void speedDown()
	{
		this->iSpeed -= 20;
	}
};

class Truck : public Car
{
public:
	Truck(const char* _pszCarName, int _iSpeed) : Car(_pszCarName)
	{
		this->iSpeed = _iSpeed;
	}

	virtual void speedUp()
	{
		this->iSpeed += 5;
	}

	virtual void speedDown()
	{
		this->iSpeed -= 5;
	}
};

int main()
{
	Car* pCarList[5];
	//Car* pCarList = new Car[5];

	pCarList[0] = new Car("K5");
	pCarList[1] = new Truck("포터", 100);
	pCarList[2] = new Sedan("그랜저", 100);
	pCarList[3] = new Truck("볼보", 100);
	pCarList[4] = new Sedan("아반떼", 100);

	for (int i = 0; i < 5; i++)
	{
		Car* pCar = pCarList[i];
		cout << pCar->getName() << endl;
	}

	return 0;
}
