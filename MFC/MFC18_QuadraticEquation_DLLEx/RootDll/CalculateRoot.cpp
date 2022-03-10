#include "stdafx.h"
#include "CalculateRoot.h"


CCalculateRoot::CCalculateRoot()
{
}


CCalculateRoot::~CCalculateRoot()
{
}


int CCalculateRoot::Determinant(double a, double b, double c, double* det)
{
	*det = (b * b) - (4 * a * c);
	if (*det > 0)
	{
		return 2;
	}
	else if (*det == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


double CCalculateRoot::CalcRoot1(double a, double b, double c)
{
	return (-b + sqrt((b * b) - (4 * a * c))) / (2 * a);
}


double CCalculateRoot::CalcRoot2(double a, double b, double c)
{
	return (-b - sqrt((b * b) - (4 * a * c))) / (2 * a);
}
