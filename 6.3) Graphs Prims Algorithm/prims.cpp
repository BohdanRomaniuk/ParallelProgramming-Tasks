#include <iostream>
#include <thread>
#include <ctime>
using namespace std;

int findMinIndex(int* ways, bool* visited, unsigned size)
{
	int min = INT_MAX;
	int minIndex = 0;
	for (unsigned i = 0; i < size; ++i)
	{
		if (visited[i] == false && ways[i] < min)
		{
			min = ways[i];
			minIndex = i;
		}
	}
	return minIndex;
}

void searching(int** matrix, int* ways, int* parent, bool* visited, int minWay, unsigned from, unsigned to)
{
	for (unsigned j = from; j < to; ++j)
	{
		if (matrix[minWay][j] && visited[j] == false && matrix[minWay][j] < ways[j])
		{
			parent[j] = minWay;
			ways[j] = matrix[minWay][j];
		}
	}
}

void prims(int** matrix, unsigned size, unsigned threadCount=1)
{
	int* parent = new int[size];
	int* ways = new int[size];
	bool* visited = new bool[size];

	for (int i = 0; i < size; i++)
	{
		ways[i] = INT_MAX;
		visited[i] = false;
	}

	ways[0] = 0;
	parent[0] = -1;

	
	for (unsigned i = 0; i < size - 1; ++i)
	{
		int minWay = findMinIndex(ways, visited, size);
		visited[minWay] = true;

		thread* threadArray = new thread[threadCount];
		unsigned from = 0;
		unsigned threadStep = size / threadCount;
		unsigned to = threadStep;
		for (unsigned j = 0; j < threadCount; ++j)
		{
			threadArray[j] = thread(searching, matrix, ways, parent, visited, minWay, from, to);
			from += threadStep;
			to += threadStep;
			threadArray[j].join();
		}
	}
}

void main()
{
	const unsigned size = 100;
	srand(time(NULL));
	int** matrix = new int*[size];
	for (unsigned i = 0; i < size; ++i)
	{
		matrix[i] = new int[size];
		for (unsigned j = 0; j < size; ++j)
		{
			matrix[i][j] = rand() % 100;
		}
	}

	clock_t beginTime = clock();
	prims(matrix, size);
	cout << "1 thread time: " << (float)(clock() - beginTime) / CLOCKS_PER_SEC << " s" << endl;

	beginTime = clock();
	prims(matrix, size, 2);
	cout << "2 threads time: " << (float)(clock() - beginTime) / CLOCKS_PER_SEC << " s" << endl;

	beginTime = clock();
	prims(matrix, size, 4);
	cout << "4 threads time: " << (float)(clock() - beginTime) / CLOCKS_PER_SEC << " s" << endl;

	beginTime = clock();
	prims(matrix, size, 10);
	cout << "10 threads time: " << (float)(clock() - beginTime) / CLOCKS_PER_SEC << " s" << endl;

	beginTime = clock();
	prims(matrix, size, 10);
	cout << "50 threads time: " << (float)(clock() - beginTime) / CLOCKS_PER_SEC << " s" << endl;
	system("pause");
}