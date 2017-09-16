#include <iostream>
#include <thread>
#include <future>
using namespace std;

//First thread
void summing(promise<int*> arr1, int* arr2, unsigned length, int* sum)
{
	int* inner_arr = new int[length]{ 1,4,5,6,12 };
	for (unsigned i = 0; i < length; ++i)
	{
		*sum += inner_arr[i] + arr2[i];
	}
	arr1.set_value(inner_arr);
}

//Second thread
void multiplying(int* arr1, int* arr2, unsigned length, promise<int*> mult)
{
	int* inner_mult = new int[length];
	for (unsigned i = 0; i < length; ++i)
	{
		inner_mult[i] = arr1[i] * arr2[i];
	}
	mult.set_value(inner_mult);
}

void main()
{
	int size = 5;
	int sum = 0;
	int* mult;

	promise<int*> arr1_promise;
	future<int*> arr1_result = arr1_promise.get_future();

	int* arr2 = new int[5]{ 3,7,8,9,10 };
	thread first(summing, move(arr1_promise), arr2, size, &sum);

	promise<int*> mult_promise;
	future<int*> mult_result = mult_promise.get_future();

	int* arr1 = arr1_result.get();
	thread second(multiplying, arr1, arr2, size, move(mult_promise));

	if (first.joinable())
	{
		first.join();
	}
	if (second.joinable())
	{
		second.join();
	}
	cout << "First array: ";
	for (int i = 0; i < 5; ++i)
	{
		cout << arr1[i] << " ";
	}
	cout << "\nSecond array: ";
	for (int i = 0; i < 5; ++i)
	{
		cout << arr2[i] << " ";
	}
	mult = mult_result.get();
	cout << "\nSum=" << sum << "\nMult=";
	for (int i = 0; i < size; ++i)
	{
		cout << mult[i] << " ";
	}
	cout << endl;
	system("pause");
}
