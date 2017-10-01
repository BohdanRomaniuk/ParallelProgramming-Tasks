#include <iostream>
#include <thread>
#include <ctime>
using namespace std;

void multiplyMatrixAndVec(double*)
{

}

void createThreadsForMatrixAndVectorMult()
{


}

void multiplyingMatrices(double** firstMatrix, double** secondMatrix, double** resMatrix, unsigned size, unsigned fromRow, unsigned toRow)
{
	for (unsigned i = fromRow; i < toRow; ++i)
	{
		for (unsigned j = 0; j < size; ++j)
		{
			for (unsigned k = 0; k < size; ++k)
			{
				resMatrix[i][j] += firstMatrix[i][k] * secondMatrix[k][j];
			}
		}
	}
}

void createThreadsForMatricesMult(double** firstMatrix, double** secondMatrix, double** resultMatrix, unsigned size, unsigned threadsCount)
{
	thread* threadsArray = new thread[threadsCount];
	unsigned fromRow = 0;
	unsigned threadStep = size / threadsCount;
	unsigned toRow = threadStep;
	for (unsigned i = 0; i < threadsCount; ++i)
	{
		threadsArray[i] = thread(multiplyingMatrices, firstMatrix, secondMatrix, resultMatrix, size, fromRow, toRow);
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

void gaussianAlgorithm(double** matrixA, double* vectorB, double** result, unsigned size, unsigned threadsCount)
{
	for (unsigned k = 0; k < size; ++k)
	{
		double** matrixT = new double*[size];
		for (unsigned i = 0; i < size; ++i)
		{
			matrixT[i][i] = 1;
		}
		matrixT[k][k] = 1 / matrixA[k][k];
		for (unsigned i = k + 1; i < size; ++i)
		{
			matrixT[i][k] = -matrixA[i][k] / matrixA[k][k];
		}
		
		createThreadsForMatricesMult(matrixT, matrixA, matrixA, size, threadsCount);
		createThreadsForMatrixAndVectorMult(matrixT, vectorB, vectorB, size, threadsCount);
	}
}

void main()
{
	const unsigned size = 1000;
	int** matrixA = new int*[size];
	int* vectorB = new int[size];
	srand(time(NULL));
	for (unsigned i = 0; i < size; ++i)
	{
		matrixA[i] = new int[size];
		vectorB[i] = rand() % 100;
		for (unsigned j = 0; j < size; ++j)
		{
			matrixA[i][j] = rand() % 100;
		}
	}
	system("pause");
}