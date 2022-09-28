#include <iostream>
#include <vector>
#include <string>
using namespace std;

void main()
{
	vector<string>v1;

	// value input 
	v1.push_back("a");
	v1.push_back("b");
	v1.push_back("c");
	for (string x : v1)
	{
		cout << x << ",";
	}
	
	cout << endl;
}