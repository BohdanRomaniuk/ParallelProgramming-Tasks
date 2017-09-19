#include <iostream>
#include <ctime>
#include <thread>
using namespace std;

void setMatrixValuesToDefault(int** matrix, unsigned size)
{
	for (unsigned i = 0; i < size; ++i)
	{
		for (unsigned j = 0; j < size; ++j)
		{
			matrix[i][j] = 0;
		}
	}
}

void summing(int** firstMatrix, int** secondMatrix, int** resMatrix, unsigned size, unsigned fromRow, unsigned toRow)
{
	for (unsigned i = fromRow; i < toRow; ++i)
	{
		for (unsigned j = 0; j < size; ++j)
		{
			resMatrix[i][j] += firstMatrix[i][j] + secondMatrix[i][j];
		}
	}
}

void subtraction(int** firstMatrix, int** secondMatrix, int** resMatrix, unsigned size, unsigned fromRow, unsigned toRow)
{
	for (unsigned i = fromRow; i < toRow; ++i)
	{
		for (unsigned j = 0; j < size; ++j)
		{
			resMatrix[i][j] += firstMatrix[i][j] - secondMatrix[i][j];
		}
	}
}

void createThreads(int** firstMatrix, int** secondMatrix, int** resultMatrix, unsigned size, int threadsCount, bool isToSum=true)
{
	thread* threadsArray = new thread[threadsCount];
	unsigned fromRow = 0;
	unsigned threadStep = size / threadsCount;
	unsigned toRow = threadStep;
	for (unsigned i = 0; i < threadsCount; ++i)
	{
		threadsArray[i] = thread(isToSum?summing:subtraction, firstMatrix, secondMatrix, resultMatrix, size, fromRow, toRow);
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
	const unsigned size = 10000;
	int** firstMatrix = new int*[size];
	int** secondMatrix = new int*[size];
	int** resultMatrix = new int*[size];
	for (unsigned i = 0; i < size; ++i)
	{
		firstMatrix[i] = new int[size];
		secondMatrix[i] = new int[size];
		resultMatrix[i] = new int[size];
	}
	setMatrixValuesToDefault(resultMatrix, size);

	//Generation random matrices
	srand(time(NULL));
	for (unsigned i = 0; i < size; ++i)
	{
		for (unsigned j = 0; j < size; ++j)
		{
			firstMatrix[i][j] = rand() % 100;
			secondMatrix[i][j] = rand() % 100;
		}
	}

	//1 thread
	clock_t beginTime = clock();
	summing(firstMatrix, secondMatrix, resultMatrix, size, 0, size);
	cout << "1 thread summing matrices time: " << (float)(clock() - beginTime) / CLOCKS_PER_SEC << " s" << endl;

	setMatrixValuesToDefault(resultMatrix, size);
	beginTime = clock();
	summing(firstMatrix, secondMatrix, resultMatrix, size, 0, size);
	cout << "1 thread subtraction matrices time: " << (float)(clock() - beginTime) / CLOCKS_PER_SEC << " s" << endl;

	//2 threads
	setMatrixValuesToDefault(resultMatrix, size);
	beginTime = clock();
	createThreads(firstMatrix, secondMatrix, resultMatrix, size, 2);
	cout << "\n2 threads summing matrices time: " << (float)(clock() - beginTime) / CLOCKS_PER_SEC << " s" << endl;

	setMatrixValuesToDefault(resultMatrix, size);
	beginTime = clock();
	createThreads(firstMatrix, secondMatrix, resultMatrix, size, 2, false);
	cout << "2 threads subtraction matrices time: " << (float)(clock() - beginTime) / CLOCKS_PER_SEC << " s" << endl;

	//5 threads
	setMatrixValuesToDefault(resultMatrix, size);
	beginTime = clock();
	createThreads(firstMatrix, secondMatrix, resultMatrix, size, 5);
	cout << "\n5 threads summing matrices time: " << (float)(clock() - beginTime) / CLOCKS_PER_SEC << " s" << endl;

	setMatrixValuesToDefault(resultMatrix, size);
	beginTime = clock();
	createThreads(firstMatrix, secondMatrix, resultMatrix, size, 5, false);
	cout << "5 threads subtraction matrices time: " << (float)(clock() - beginTime) / CLOCKS_PER_SEC << " s" << endl;

	//10 threads
	setMatrixValuesToDefault(resultMatrix, size);
	beginTime = clock();
	createThreads(firstMatrix, secondMatrix, resultMatrix, size, 10);
	cout << "\n10 threads summing matrices time: " << (float)(clock() - beginTime) / CLOCKS_PER_SEC << " s" << endl;

	setMatrixValuesToDefault(resultMatrix, size);
	beginTime = clock();
	createThreads(firstMatrix, secondMatrix, resultMatrix, size, 10, false);
	cout << "10 threads subtraction matrices time: " << (float)(clock() - beginTime) / CLOCKS_PER_SEC << " s" << endl;

	//150 threads
	setMatrixValuesToDefault(resultMatrix, size);
	beginTime = clock();
	createThreads(firstMatrix, secondMatrix, resultMatrix, size, 15);
	cout << "\n15 threads summing matrices time: " << (float)(clock() - beginTime) / CLOCKS_PER_SEC << " s" << endl;

	setMatrixValuesToDefault(resultMatrix, size);
	beginTime = clock();
	createThreads(firstMatrix, secondMatrix, resultMatrix, size, 15, false);
	cout << "15 threads subtraction matrices time: " << (float)(clock() - beginTime) / CLOCKS_PER_SEC << " s" << endl;

	//20 threads
	setMatrixValuesToDefault(resultMatrix, size);
	beginTime = clock();
	createThreads(firstMatrix, secondMatrix, resultMatrix, size, 20);
	cout << "\n20 threads summing matrices time: " << (float)(clock() - beginTime) / CLOCKS_PER_SEC << " s" << endl;

	setMatrixValuesToDefault(resultMatrix, size);
	beginTime = clock();
	createThreads(firstMatrix, secondMatrix, resultMatrix, size, 20, false);
	cout << "20 threads subtraction matrices time: " << (float)(clock() - beginTime) / CLOCKS_PER_SEC << " s" << endl;
	system("pause");
}