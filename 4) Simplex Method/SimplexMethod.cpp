#include <iostream>
#include <thread>
using namespace std;

void simplexMethod()
{

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
	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < cols; j++)
		{
			cout << matrix[i][j] << " ";
		}
		cout << endl;
	}
	cout << "First iter:" << endl;

	double min = 0;
	unsigned minColPos = 0;
	for (unsigned i = 0; i < cols; ++i)//по стовпц€х цикл
	{
		if (matrix[rows - 1][i] < min)
		{
			min = matrix[rows - 1][i];
			minColPos = i;
		}
	}
	double* rightColumn = new double[rows - 1];
	for (unsigned i = 0; i < rows - 1; ++i)//по р€дках цикл
	{
		rightColumn[i] = matrix[i][cols - 1] / matrix[i][minColPos];
	}
	double minRightColumn = rightColumn[0];
	unsigned minRowPos = 0;
	for (unsigned i = 0; i < rows - 1; ++i)//по р€дках цикл
	{
		if (rightColumn[i] < minRightColumn)
		{
			minRightColumn = rightColumn[i];
			minRowPos = i;
		}
	}

	double divider = matrix[minRowPos][minColPos];
	for (unsigned i = 0; i < cols; ++i)
	{
		matrix[minRowPos][i] = matrix[minRowPos][i] / divider;
	}

	
	for (unsigned i = 0; i < rows - 1 && i != minRowPos; ++i)
	{
		double mult = -matrix[i][minColPos];
		for (unsigned j = 0; j < cols; ++j)
		{
			matrix[i][j] = mult * matrix[minRowPos][j] + matrix[i][j];
		}
	}

	//Last row
	double mult = -matrix[rows - 1][minColPos];
	for (unsigned j = 0; j < cols; ++j)
	{
		matrix[rows - 1][j] = mult*matrix[minRowPos][j] + matrix[rows - 1][j];
	}

	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < cols; j++)
		{
			cout << matrix[i][j] << " ";
		}
		cout << endl;
	}
	std::system("pause");
}