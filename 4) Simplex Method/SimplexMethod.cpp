#include <iostream>
#include <thread>
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

bool findPivotElementPos(double** matrix,unsigned rows,unsigned cols, unsigned& minRowPos, unsigned& minColPos)
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

	//element < 0 at last row not found, end of simplex method
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

void simplexMethod(double** matrix, unsigned rows, unsigned cols, unsigned threadsCount=1)
{
	unsigned minRowPos, minColPos;
	unsigned* variblesNums = new unsigned[rows - 1];
	while(findPivotElementPos(matrix,rows,cols, minRowPos, minColPos))
	{
		variblesNums[minRowPos] = minColPos;
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
		printMatrix(matrix, rows, cols);
	}
	cout << endl;
	for (unsigned i = 0; i < rows - 1; ++i)
	{
		cout << "x" << variblesNums[i] + 1 << "=" << matrix[i][cols-1] << endl;
	}
	cout << "Maximum=" << matrix[rows - 1][cols - 1] << endl;
}

void main()
{
	const unsigned rows = 3;
	const unsigned cols = 7;
	double input[3][7] =
	{
		2,3,2,1,0,0,1000,
		1,1,2,0,1,0,800,
		-7,-8,-10,0,0,1,0
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
	simplexMethod(matrix, rows, cols);
	std::system("pause");
}