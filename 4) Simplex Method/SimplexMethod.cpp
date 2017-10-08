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

bool findPivotElement(double** matrix,unsigned rows,unsigned cols, unsigned& minRowPos, unsigned& minColPos)
{
	bool found = false;
	double min = 0;
	minColPos = 0;
	for (unsigned i = 0; i < cols; ++i)//по стовпц€х цикл
	{
		if (matrix[rows - 1][i] < min)
		{
			found = true;
			min = matrix[rows - 1][i];
			minColPos = i;
			break;
		}
	}
	double* rightColumn = new double[rows - 1];
	for (unsigned i = 0; i < rows - 1; ++i)//по р€дках цикл
	{
		rightColumn[i] = matrix[i][cols - 1] / matrix[i][minColPos];
	}
	double minRightColumn = rightColumn[0];
	minRowPos = 0;
	for (unsigned i = 0; i < rows - 1; ++i)//по р€дках цикл
	{
		if (rightColumn[i] < minRightColumn)
		{
			minRightColumn = rightColumn[i];
			minRowPos = i;
		}
	}
	return found;
}

void simplexMethod(double** matrix, unsigned rows, unsigned cols)
{
	unsigned minRowPos, minColPos;
	while(findPivotElement(matrix,rows,cols, minRowPos, minColPos))
	{
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
		printMatrix(matrix, rows, cols);
	}
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