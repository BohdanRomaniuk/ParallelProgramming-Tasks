#include <iostream>
#include <thread>
#include <ctime>
using namespace std;

void multiplyMatrixAndVec(double** matrix, double* vec, double* result, unsigned size, unsigned fromRow, unsigned toRow)
{
	for (unsigned i = fromRow; i < toRow; ++i)
	{
		for (unsigned j = 0; j < size; ++j)
		{
			result[i] += matrix[i][j] * vec[j];
		}
	}
}

void createThreadsForMatrixAndVecMult(double** matrixT,double* vectorB, double* vectorRes, unsigned size,unsigned threadsCount)
{
	thread* threadsArray = new thread[threadsCount];
	unsigned from = 0;
	unsigned threadStep = size / threadsCount;
	unsigned to = threadStep;
	for (unsigned i = 0; i < threadsCount; ++i)
	{
		threadsArray[i] = thread(multiplyMatrixAndVec, matrixT, vectorB, vectorRes, size, from, to);
		from += threadStep;
		to += threadStep;
	}
	for (unsigned i = 0; i < threadsCount; ++i)
	{
		if (threadsArray[i].joinable())
		{
			threadsArray[i].join();
		}
	}
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

void directCourse(double** matrixA, double* vectorB, unsigned size, unsigned from, unsigned to, unsigned threadsCount)
{
	for (unsigned k = from; k < to; ++k)
	{
		double** matrixT = new double*[size];
		for (unsigned i = 0; i < size; ++i)
		{
			matrixT[i] = new double[size];
			matrixT[i][i] = 1;
		}
		matrixT[k][k] = 1 / matrixA[k][k];
		for (unsigned i = k + 1; i < size; ++i)
		{
			matrixT[i][k] = -matrixA[i][k] / matrixA[k][k];
		}
		createThreadsForMatricesMult(matrixT, matrixA, matrixA, size, threadsCount);
		createThreadsForMatrixAndVecMult(matrixT, vectorB, vectorB, size, threadsCount);
	}
}

void reverseCourse(double** matrixA, double* vectorB, unsigned size, unsigned from, unsigned to, unsigned threadsCount)
{
	for (int k = from; k >= to; --k)
	{
		double** matrixV = new double*[size];
		for (unsigned i = 0; i< size; ++i)
		{
			matrixV[i] = new double[size];
			matrixV[i][i] = 1;
		}
		for (unsigned i = 0; i<k; ++i)
		{
			matrixV[i][k] = -matrixA[i][k];
		}
		createThreadsForMatricesMult(matrixV, matrixA, matrixA, size, threadsCount);
		createThreadsForMatrixAndVecMult(matrixV, vectorB, vectorB, size, threadsCount);
	}
}

void gaussianAlgorithm(double** matrixA, double* vectorB, double* result, unsigned size, unsigned threadsCount = 1)
{
	thread* threadsArray = new thread[2*threadsCount];

	//Direct Course
	unsigned from = 0;
	unsigned threadStep = size / threadsCount;
	unsigned to = threadStep;
	for (unsigned i = 0; i < threadsCount; ++i)
	{
		threadsArray[i] = thread(directCourse, matrixA, vectorB, size, from, to, threadsCount);
		from += threadStep;
		to += threadStep;
	}

	//Reverse Course
	from = size - 1;
	to = from - threadStep;
	for (unsigned i = threadsCount; i < 2*threadsCount; ++i)
	{
		threadsArray[i] = thread(reverseCourse, matrixA, vectorB, size, from, to, threadsCount);
		from -= threadStep;
		to -= threadStep;
	}
	for (unsigned i = 0; i < 2 * threadsCount; ++i)
	{
		if (threadsArray[i].joinable())
		{
			threadsArray[i].join();
		}
	}

	//Result
	for (unsigned i = 0; i < size; ++i)
	{
		result[i] = vectorB[i];
	}
}

void main()
{
	const unsigned size = 100;
	double** matrixA = new double*[size];
	double* vectorB = new double[size];
	double* resultVec = new double[size];
	srand(time(NULL));
	for (unsigned i = 0; i < size; ++i)
	{
		matrixA[i] = new double[size];
		vectorB[i] = rand() % 100;
		for (unsigned j = 0; j < size; ++j)
		{
			matrixA[i][j] = rand() % 100;
		}
	}
	clock_t beginTime = clock();
	gaussianAlgorithm(matrixA, vectorB, resultVec, size);
	cout << "1 thread time: " << (float)(clock() - beginTime)/CLOCKS_PER_SEC << " s" << endl;
	beginTime = clock();
	gaussianAlgorithm(matrixA, vectorB, resultVec, size, 2);
	cout << "2 thread time: " << (float)(clock() - beginTime) / CLOCKS_PER_SEC << " s" << endl;
	beginTime = clock();
	gaussianAlgorithm(matrixA, vectorB, resultVec, size, 5);
	cout << "5 thread time: " << (float)(clock() - beginTime) / CLOCKS_PER_SEC << " s" << endl;
	beginTime = clock();
	gaussianAlgorithm(matrixA, vectorB, resultVec, size, 10);
	cout << "10 thread time: " << (float)(clock() - beginTime) / CLOCKS_PER_SEC << " s" << endl;
	beginTime = clock();
	gaussianAlgorithm(matrixA, vectorB, resultVec, size, 20);
	cout << "20 thread time: " << (float)(clock() - beginTime) / CLOCKS_PER_SEC << " s" << endl;
	beginTime = clock();
	gaussianAlgorithm(matrixA, vectorB, resultVec, size, 50);
	cout << "50 thread time: " << (float)(clock() - beginTime) / CLOCKS_PER_SEC << " s" << endl;
	system("pause");
}