#include <iostream>
#include <thread>
#include <ctime>
using namespace std;

void printMatrix(double** matrix, unsigned rows, unsigned cols)
{
	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < cols; j++)
		{
			cout << matrix[i][j] << " ";
		}
		cout << endl;
	}
}

bool findPivotElementPos(double** matrix, unsigned rows, unsigned cols, unsigned& minRowPos, unsigned& minColPos)
{
	bool found = false;
	double min = 0;
	for (unsigned j = 0; j < cols; ++j)
	{
		if (matrix[rows - 1][j] < min)
		{
			min = matrix[rows - 1][j];
			minColPos = j;
			found = true;
		}
	}

	//end of simplex method because element < 0 at last row not found
	if (!found)
	{
		return false;
	}

	min = DBL_MAX;
	for (unsigned i = 0; i < rows - 1; ++i)
	{
		double dividedElem = matrix[i][cols - 1] / matrix[i][minColPos];
		if (dividedElem < min)
		{
			min = dividedElem;
			minRowPos = i;
		}
	}
	return found;
}

void subRows(double** matrix, unsigned rows, unsigned cols, unsigned minRowPos, unsigned minColPos, unsigned from, unsigned to)
{
	for (unsigned i = from; i < to; ++i)
	{
		if (i == minRowPos)
		{
			continue;
		}
		double mult = -matrix[i][minColPos];
		for (unsigned j = 0; j < cols; ++j)
		{
			matrix[i][j] = mult * matrix[minRowPos][j] + matrix[i][j];
		}
	}
}

void divideRow(double** matrix, unsigned minRowPos, unsigned minColPos, unsigned from, unsigned to, double divider)
{
	for (unsigned i = from; i < to; ++i)
	{
		matrix[minRowPos][i] = matrix[minRowPos][i] / divider;
	}
}

void simplexMethod(double** matrix, unsigned rows, unsigned cols, unsigned threadCount=1)
{
	unsigned minRowPos, minColPos;

	//s1=-1, s2=-2,... x1=1, x2=2....
	//аби розрізняти si та xi змінні s мають відємні індекси, а x додатні
	int* variblesNums = new int[rows - 1];
	int sVaribles = -1;
	for (unsigned i = 0; i < rows - 1; ++i)
	{
		variblesNums[i] = sVaribles--;
	}

	thread* threadArray = new thread[2*threadCount];
	while(findPivotElementPos(matrix,rows,cols, minRowPos, minColPos))
	{
		variblesNums[minRowPos] = minColPos+1;

		unsigned from = 0;
		unsigned threadStep = cols / threadCount;
		unsigned to = threadStep;
		double divider = matrix[minRowPos][minColPos];
		for (unsigned i = 0; i < threadCount; ++i)
		{
			threadArray[i] = thread(divideRow, matrix, minRowPos, minColPos, from, to, divider);
			from += threadStep;
			to += threadStep;
			threadArray[i].join();
		}
		
		/*
		double divider = matrix[minRowPos][minColPos];
		for (unsigned i = 0; i < cols; ++i)
		{
			matrix[minRowPos][i] = matrix[minRowPos][i] / divider;
		}
		*/

		from = 0;
		threadStep = rows / threadCount;
		to = threadStep;
		for (unsigned i = threadCount; i < 2*threadCount; ++i)
		{
			threadArray[i] = thread(subRows, matrix, rows, cols,minRowPos,minColPos, from, to);
			from += threadStep;
			to += threadStep;
			threadArray[i].join();
		}

		/*
		for (unsigned i = 0; i < rows; ++i)
		{
			if (i == minRowPos)
			{
				continue;
			}
			double mult = -matrix[i][minColPos];
			for (unsigned j = 0; j < cols; ++j)
			{
				matrix[i][j] = mult * matrix[minRowPos][j] + matrix[i][j];
			}
		}
		*/
		cout << "\nIteration" << endl;
		printMatrix(matrix, rows, cols);
	}
	cout << endl;
	for (unsigned i = 0; i < rows - 1; ++i)
	{
		if (variblesNums[i] < 0)
		{
			cout << "s" << variblesNums[i]*(-1) << "=" << matrix[i][cols - 1] << endl;
		}
		else
		{
			cout << "x" << variblesNums[i] << "=" << matrix[i][cols - 1] << endl;
		}
	}
	cout << "Maximum=" << matrix[rows - 1][cols - 1] << endl;
}

void main()
{
	/*
	const unsigned rows = 3;
	const unsigned cols = 7;
	double input[rows][cols] =
	{
		2,3,2,1,0,0,1000,
		1,1,2,0,1,0,800,
		-7,-8,-10,0,0,1,0
	};
	*/

	const unsigned rows = 4;
	const unsigned cols = 6;
	double input[rows][cols] =
	{
		5,9,1,0,0,45,
		3,3,0,1,0,19,
		2,1,0,0,1,10,
		-5,-6,0,0,0,0
	};

	double** matrix = new double*[rows];
	for (unsigned i = 0; i < rows; ++i)
	{
		matrix[i] = new double[cols];
		for (unsigned j = 0; j < cols; ++j)
		{
			matrix[i][j] = input[i][j];
		}
	}
	cout << "Input data:" << endl;
	printMatrix(matrix, rows, cols);

	clock_t beginTime = clock();
	simplexMethod(matrix, rows, cols);
	cout << "1 thread time::::::::" << (float)(clock() - beginTime) / CLOCKS_PER_SEC << " s" << endl;

	for (unsigned i = 0; i < rows; ++i)
	{
		matrix[i] = new double[cols];
		for (unsigned j = 0; j < cols; ++j)
		{
			matrix[i][j] = input[i][j];
		}
	}
	beginTime = clock();
	simplexMethod(matrix, rows, cols, 2);
	cout << "2 threads time::::::::" << (float)(clock() - beginTime) / CLOCKS_PER_SEC << " s" << endl;
	system("pause");
}