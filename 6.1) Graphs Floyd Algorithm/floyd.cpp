#include <iostream>
#include <thread>
#include <ctime>
using namespace std;

void floyd(int** matrix, int** way, unsigned size, unsigned from, unsigned to)
{
	for (unsigned k = from; k < to; ++k)
	{
		for (unsigned i = 0; i < size; ++i)
		{
			for (unsigned j = 0; j < size; ++j)
			{
				if (matrix[i][k] + matrix[k][j] < matrix[i][j] && i != k && j != k)
				{
					matrix[i][j] = matrix[i][k] + matrix[k][j];
					way[i][j] = k - 1;
				}
			}
		}
	}
}

void createThreads(int** matrix, int** way, unsigned size, unsigned threadCount = 1)
{
	thread* threadArray = new thread[threadCount];
	unsigned from = 0;
	unsigned threadStep = size / threadCount;
	unsigned to = threadStep;
	for (unsigned i = 0; i < threadCount; ++i)
	{
		threadArray[i] = thread(floyd, matrix, way, size, from, to);
		from += threadStep;
		to += threadStep;
	}
	for (unsigned i = 0; i < threadCount; ++i)
	{
		if (threadArray[i].joinable())
		{
			threadArray[i].join();
		}
	}
}

void main()
{
	const unsigned size = 1000;
	int** matrix = new  int*[size];
	int** ways = new  int*[size];
	srand(time(NULL));
	for (unsigned i = 0; i < size; ++i)
	{
		matrix[i] = new int[size];
		ways[i] = new int[size];
		for (unsigned j = 0; j < size; ++j)
		{
			if (i == j)
			{
				matrix[i][j] = 0;
			}
			matrix[i][j] = rand() % 100;
		}
	}
	clock_t beginTime = clock();
	floyd(matrix, ways, size, 0, size);
	cout << "1 thread time: " << (float)(clock() - beginTime) / CLOCKS_PER_SEC << endl;

	beginTime = clock();
	createThreads(matrix, ways, size, 2);
	cout << "2 threads time: " << (float)(clock() - beginTime) / CLOCKS_PER_SEC << endl;

	beginTime = clock();
	createThreads(matrix, ways, size, 4);
	cout << "4 threads time: " << (float)(clock() - beginTime) / CLOCKS_PER_SEC << endl;

	beginTime = clock();
	createThreads(matrix, ways, size, 10);
	cout << "10 threads time: " << (float)(clock() - beginTime) / CLOCKS_PER_SEC << endl;

	beginTime = clock();
	createThreads(matrix, ways, size, 50);
	cout << "50 threads time: " << (float)(clock() - beginTime) / CLOCKS_PER_SEC << endl;
	system("pause");
}