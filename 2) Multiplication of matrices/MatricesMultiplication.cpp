#include <iostream>
#include <ctime>
#include <thread>
using namespace std;

void setDefaultValuesToMatrix(int** matrix, unsigned rows, unsigned cols)
{
	for (unsigned i = 0; i < rows; ++i)
	{
		for (unsigned j = 0; j < cols; ++j)
		{
			matrix[i][j] = 0;
		}
	}
}

void multiplyingMatrices(int** firstMatrix, int** secondMatrix, int** resMatrix, unsigned size, unsigned fromRow, unsigned toRow)
{
	for (unsigned i = fromRow; i < toRow; ++i)
	{
		for (unsigned j = 0; j < size; ++j)
		{
			for (unsigned k = 0; k < size; ++k)
			{
				resMatrix[i][j] += firstMatrix[i][k] * secondMatrix[k][j];
			}
		}
	}
}

void createThreads(int** firstMatrix, int** secondMatrix, int** resultMatrix, unsigned size, unsigned threadsCount)
{
	thread* threadsArray = new thread[threadsCount];
	unsigned fromRow = 0;
	unsigned threadStep = size / threadsCount;
	unsigned toRow = threadStep;
	for (unsigned i = 0; i < threadsCount; ++i)
	{
		threadsArray[i] = thread(multiplyingMatrices, firstMatrix, secondMatrix, resultMatrix, size, fromRow, toRow);
		fromRow += threadStep;
		toRow += threadStep;
	}
	for (unsigned i = 0; i < threadsCount; ++i)
	{
		if (threadsArray[i].joinable())
		{
			threadsArray[i].join();
		}
	}
}

void main()
{
	unsigned size = 1000;
	int** firstMatrix = new int*[size];
	int** secondMatrix = new int*[size];
	int** resMatrix = new int*[size];
	for (unsigned i = 0; i < size; ++i)
	{
		firstMatrix[i] = new int[size];
		secondMatrix[i] = new int[size];
		resMatrix[i] = new int[size];
	}
	srand(time(NULL));
	for (unsigned i = 0; i < size; ++i)
	{
		for (unsigned j = 0; j < size; ++j)
		{
			firstMatrix[i][j] = rand() % 100;
			secondMatrix[i][j] = rand() % 100;
		}
	}
	setDefaultValuesToMatrix(resMatrix, size, size);

	//1 thread
	clock_t beginTime = clock();
	multiplyingMatrices(firstMatrix, secondMatrix, resMatrix, size, 0, size);
	cout << "1 thread matrices multiplying time: " << (float)(clock() - beginTime) / CLOCKS_PER_SEC << endl;

	//2 threads
	setDefaultValuesToMatrix(resMatrix, size, size);
	beginTime = clock();
	createThreads(firstMatrix, secondMatrix, resMatrix, size, 2);
	cout << "2 thread matrices multiplying time: " << (float)(clock() - beginTime) / CLOCKS_PER_SEC << endl;

	//4 threads
	setDefaultValuesToMatrix(resMatrix, size, size);
	beginTime = clock();
	createThreads(firstMatrix, secondMatrix, resMatrix, size, 4);
	cout << "4 thread matrices multiplying time: " << (float)(clock() - beginTime) / CLOCKS_PER_SEC << endl;

	//5 threads
	setDefaultValuesToMatrix(resMatrix, size, size);
	beginTime = clock();
	createThreads(firstMatrix, secondMatrix, resMatrix, size, 5);
	cout << "5 thread matrices multiplying time: " << (float)(clock() - beginTime) / CLOCKS_PER_SEC << endl;

	//10 threads
	setDefaultValuesToMatrix(resMatrix, size, size);
	beginTime = clock();
	createThreads(firstMatrix, secondMatrix, resMatrix, size, 10);
	cout << "10 thread matrices multiplying time: " << (float)(clock() - beginTime) / CLOCKS_PER_SEC << endl;

	//20 threads
	setDefaultValuesToMatrix(resMatrix, size, size);
	beginTime = clock();
	createThreads(firstMatrix, secondMatrix, resMatrix, size, 20);
	cout << "20 thread matrices multiplying time: " << (float)(clock() - beginTime) / CLOCKS_PER_SEC << endl;

	//50 threads
	setDefaultValuesToMatrix(resMatrix, size, size);
	beginTime = clock();
	createThreads(firstMatrix, secondMatrix, resMatrix, size, 20);
	cout << "50 thread matrices multiplying time: " << (float)(clock() - beginTime) / CLOCKS_PER_SEC << endl;

	system("pause");
}