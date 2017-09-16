#include <iostream>
#include <thread>
#include <future>
using namespace std;

//Second thread
void summing(promise<int*> arr1, int arr2[], unsigned length, int* result)
{
	int* inner_arr = new int[length]{ 1,4,5,6,12 };
	for (unsigned i = 0; i < length; ++i)
	{
		*result += inner_arr[i] + arr2[i];
	}
	arr1.set_value(inner_arr);
}

//First thread - Main Thread
void main()
{
	int sum = 0;
	int mult = 0;
	promise<int*> arr1_promise;
	future<int*> arr1_result = arr1_promise.get_future();
	int* arr2 = new int[5]{ 3,7,8,9,10 };
	thread second(summing, move(arr1_promise), arr2, 5, &sum);
	int* arr1 = arr1_result.get();
	for (unsigned i = 0; i < 5; ++i)
	{
		mult += arr1[i] * arr2[i];
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
	cout << "\nSum=" << sum << "\tMult=" << mult << endl;
	system("pause");
}
