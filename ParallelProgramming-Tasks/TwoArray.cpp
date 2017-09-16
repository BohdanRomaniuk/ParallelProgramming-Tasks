#include <iostream>
#include <thread>
using namespace std;

//First thread
void summing(int* arr1, int* arr2, unsigned length, int* result)
{
	arr1 = new int[length]{ 1,4,5,6,12 };
	for (unsigned i = 0; i < 5; ++i)
	{
		*result += arr1[i] + arr2[i];
	}
}

//Second thread
void multiplying(int* arr1, int* arr2, unsigned length, int* result)
{
	arr2 = new int[length] {4, 5, 7, 8, 9};
	for (unsigned i = 0; i < length; ++i)
	{
		*result += arr1[i] * arr2[i];
	}
}

void main()
{
	int sum = 0;
	int mult = 0;
	int *arr1, *arr2;
	thread first(summing, arr1, arr2, &sum);
	thread second(multiplying, arr1, arr2, &mult);
	/*if (first.joinable())
	{
		first.join();
	}
	if (second.joinable())
	{
		second.join();
	}*/
	cout << "Sum=" << sum << "\tMult=" << mult << endl;
	system("pause");
}