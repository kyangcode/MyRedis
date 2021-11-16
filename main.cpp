#include "SDS.h"
#include <iostream>
using namespace std;

int main()
{
	SDS sds("Hello SDS");

	SDS sds1("abc");

	cout << sds.cmp(sds1) << endl;
	return 0;
}
