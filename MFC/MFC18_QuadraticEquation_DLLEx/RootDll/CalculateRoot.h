#pragma once
class AFX_EXT_CLASS CCalculateRoot
{
public:
	CCalculateRoot();
	~CCalculateRoot();
	int Determinant(double a, double b, double c, double* det);
	double CalcRoot1(double a, double b, double c);
	double CalcRoot2(double a, double b, double c);
};

