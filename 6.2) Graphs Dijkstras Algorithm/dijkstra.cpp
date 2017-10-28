#include <iostream>
#include <thread>
#include <ctime>
using namespace std;

void checking(int** matrix, int* ways, bool* visited, int pos, unsigned from, unsigned to)
{
	for (unsigned j = from; j < to; ++j)
	{
		if (!visited[j] && matrix[pos][j] != 0 && ways[pos] != INT_MAX && ways[pos] + matrix[pos][j] < ways[j])
		{
			ways[j] = ways[pos] + matrix[pos][j];
		}
	}
}

void dijkstra(int** matrix, int* ways, unsigned size, unsigned threadsCount=1)
{
	bool* visited = new bool[size];
	for (unsigned i = 0; i<size; ++i)
	{
		ways[i] = INT_MAX;
		visited[i] = false;
	}
	ways[0] = 0;

	for (unsigned i = 0; i < size-1; ++i)
	{
		int min = INT_MAX;
		int pos=0;

		for (unsigned j = 0; j < size; ++j)
		{
			if (!visited[j] && ways[j] <= min)
			{
				min = ways[j];
				pos = j;
			}
		}

		visited[pos] = true;
		thread* threadArray = new thread[threadsCount];
		unsigned from = 0;
		unsigned threadStep = size / threadsCount;
		unsigned to = threadStep;
		for (unsigned i = 0; i < threadsCount; ++i)
		{
			threadArray[i] = thread(checking, matrix, ways, visited, pos, from, to);
			threadArray[i].join();
			from += threadStep;
			to += threadStep;
		}
	}
	delete[] visited;
}

void main()
{
	const unsigned size = 100;
	int** matrix = new int*[size];
	int* ways = new int[size];
	srand(time(NULL));
	for (unsigned i = 0; i < size; ++i)
	{
		matrix[i] = new int[size];
		for (unsigned j = 0; j < size; ++j)
		{
			if (i == j)
			{
				matrix[i][j] = 0;
			}
			else
			{
				matrix[i][j] = rand() % 100;
			}
		}
	}
	clock_t beginTime = clock();
	dijkstra(matrix, ways, size);
	cout << "1 thread time: " << (float)(clock() - beginTime) / CLOCKS_PER_SEC << " s" << endl;
	beginTime = clock();
	dijkstra(matrix, ways, size, 2);
	cout << "2 threads time: " << (float)(clock() - beginTime) / CLOCKS_PER_SEC << " s" << endl;
	beginTime = clock();
	dijkstra(matrix, ways, size, 4);
	cout << "4 threads time: " << (float)(clock() - beginTime) / CLOCKS_PER_SEC << " s" << endl;
	beginTime = clock();
	dijkstra(matrix, ways, size, 10);
	cout << "10 threads time: " << (float)(clock() - beginTime) / CLOCKS_PER_SEC << " s" << endl;
	beginTime = clock();
	dijkstra(matrix, ways, size, 50);
	cout << "50 threads time: " << (float)(clock() - beginTime) / CLOCKS_PER_SEC << " s" << endl;
	system("pause");
}