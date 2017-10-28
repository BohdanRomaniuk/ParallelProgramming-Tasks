#include <iostream>
#include <thread>
#include <ctime>
using namespace std;

void dijkstra(int** matrix, int* ways, unsigned size, unsigned from, unsigned to)
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
		for (unsigned j = 0; j < size; ++j)
		{
			if (!visited[j] && matrix[pos][j] != 0 && ways[pos] != INT_MAX && ways[pos] + matrix[pos][j] < ways[j])
			{
				ways[j] = ways[pos] + matrix[pos][j];
			}
		}
	}
	delete[] visited;
}

void createThreads(int** matrix, int* ways, unsigned size, unsigned threadsCount)
{
	thread* threadArray = new thread[threadsCount];
	unsigned from = 0;
	unsigned threadStep = size / threadsCount;
	unsigned to = threadStep;
	for (unsigned i = 0; i < threadsCount; ++i)
	{
		threadArray[i] = thread(dijkstra, matrix, ways, size, from, to);
		if (threadArray[i].joinable())
		{
			threadArray[i].join();
		}
		from += threadStep;
		to += threadStep;
	}
}

void main()
{
	const unsigned size = 5;
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
	dijkstra(matrix, ways, size, 0, size);
	cout << "1 thread time: " << (float)(clock() - beginTime) / CLOCKS_PER_SEC << " s" << endl;
	beginTime = clock();
	createThreads(matrix, ways, size, 2);
	cout << "2 threads time: " << (float)(clock() - beginTime) / CLOCKS_PER_SEC << " s" << endl;
	system("pause");
}