#include <iostream>
#include <thread>
#include <ctime>
using namespace std;

const unsigned rows = 4;
const unsigned cols = 6;

void setInputValuesToMatrix(double input[][cols], double** matrix)
{
	for (unsigned i = 0; i < rows; ++i)
	{
		for (unsigned j = 0; j < cols; ++j)
		{
			matrix[i][j] = input[i][j];
		}
	}
}

void printMatrix(double** matrix)
{
	for (unsigned i = 0; i < rows; i++)
	{
		for (unsigned j = 0; j < cols; j++)
		{
			cout << matrix[i][j] << " ";
		}
		cout << endl;
	}
}

bool findPivotElementPos(double** matrix, unsigned& minRowPos, unsigned& minColPos)
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

void simplexMethodLinear(double** matrix)
{
	unsigned minRowPos, minColPos;

	//аби розрізняти si та xi змінні s мають відємні індекси, а x додатні
	//s1=-1, s2=-2,... x1=1, x2=2....
	int* variblesNums = new int[rows - 1];
	int sVaribles = -1;
	for (unsigned i = 0; i < rows - 1; ++i)
	{
		variblesNums[i] = sVaribles--;
	}

	while (findPivotElementPos(matrix, minRowPos, minColPos))
	{
		variblesNums[minRowPos] = minColPos + 1;

		double divider = matrix[minRowPos][minColPos];
		for (unsigned i = 0; i < cols; ++i)
		{
			matrix[minRowPos][i] = matrix[minRowPos][i] / divider;
		}

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

		cout << "\nIteration" << endl;
		printMatrix(matrix);
	}
	cout << endl;
	for (unsigned i = 0; i < rows - 1; ++i)
	{
		if (variblesNums[i] < 0)
		{
			cout << "s" << variblesNums[i] * (-1) << "=" << matrix[i][cols - 1] << endl;
		}
		else
		{
			cout << "x" << variblesNums[i] << "=" << matrix[i][cols - 1] << endl;
		}
	}
	cout << "Maximum=" << matrix[rows - 1][cols - 1] << endl;
}

void subRows(double** matrix, unsigned minRowPos, unsigned minColPos, unsigned from, unsigned to)
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

void simplexMethodParallel(double** matrix, unsigned threadCount=1)
{
	unsigned minRowPos, minColPos;

	//аби розрізняти si та xi змінні s мають відємні індекси, а x додатні
	//s1=-1, s2=-2,... x1=1, x2=2....
	int* variblesNums = new int[rows - 1];
	int sVaribles = -1;
	for (unsigned i = 0; i < rows - 1; ++i)
	{
		variblesNums[i] = sVaribles--;
	}

	thread* threadArray = new thread[threadCount];
	while (findPivotElementPos(matrix, minRowPos, minColPos))
	{
		variblesNums[minRowPos] = minColPos + 1;

		double divider = matrix[minRowPos][minColPos];
		for (unsigned i = 0; i < cols; ++i)
		{
			matrix[minRowPos][i] = matrix[minRowPos][i] / divider;
		}

		//TRUE If count of rows properly divided to threadCount
		bool isThreadDividedProper = (rows%threadCount == 0);
		unsigned from = 0;
		unsigned threadStep = rows / threadCount;
		unsigned to = threadStep;
		for (unsigned i = 0; i < threadCount; ++i)
		{
			if (isThreadDividedProper)
			{
				threadArray[i] = thread(subRows, matrix, minRowPos, minColPos, from, to);
			}
			else
			{
				threadArray[i] = thread(subRows, matrix, minRowPos, minColPos, from, (i == threadCount - 1) ? to + (rows%threadCount) : to);
			}
			from += threadStep;
			to += threadStep;
			threadArray[i].join();
		}

		cout << "\nIteration" << endl;
		printMatrix(matrix);
	}
	cout << endl;
	for (unsigned i = 0; i < rows - 1; ++i)
	{
		if (variblesNums[i] < 0)
		{
			cout << "s" << variblesNums[i] * (-1) << "=" << matrix[i][cols - 1] << endl;
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
	double input[rows][cols] =
	{
		2,3,2,1,0,0,1000,
		1,1,2,0,1,0,800,
		-7,-8,-10,0,0,1,0
	};
	*/
	
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
	}
	setInputValuesToMatrix(input, matrix);
	cout << "Input data:" << endl;
	printMatrix(matrix);

	clock_t beginTime = clock();
	simplexMethodLinear(matrix);
	cout << "1 thread time::::::::" << (float)(clock() - beginTime) / CLOCKS_PER_SEC << " s" << endl;

	setInputValuesToMatrix(input, matrix);
	beginTime = clock();
	simplexMethodParallel(matrix, 2);
	cout << "2 threads time::::::::" << (float)(clock() - beginTime) / CLOCKS_PER_SEC << " s" << endl;

	setInputValuesToMatrix(input, matrix);
	beginTime = clock();
	simplexMethodParallel(matrix, 3);
	cout << "3 threads time::::::::" << (float)(clock() - beginTime) / CLOCKS_PER_SEC << " s" << endl;

	setInputValuesToMatrix(input, matrix);
	beginTime = clock();
	simplexMethodParallel(matrix, 4);
	cout << "4 threads time::::::::" << (float)(clock() - beginTime) / CLOCKS_PER_SEC << " s" << endl;
	system("pause");
}