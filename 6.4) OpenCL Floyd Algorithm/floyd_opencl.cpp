#include <iostream>
#include <thread>
#include <ctime>
#define __CL_ENABLE_EXCEPTIONS
#define DATA_SIZE 256
#include <CL/cl.hpp>
using namespace std;

void floyd(int** matrix, int** way, unsigned size, unsigned from, unsigned to)
{
	for (unsigned k = from; k < to; ++k)
	{
		for (unsigned i = 0; i < size; ++i)
		{
			for (unsigned j = 0; j < size; ++j)
			{
				if (matrix[i][k] + matrix[k][j] < matrix[i][j] && i != k && j != k)
				{
					matrix[i][j] = matrix[i][k] + matrix[k][j];
					way[i][j] = k - 1;
				}
			}
		}
	}
}

void createThreads(int** matrix, int** way, unsigned size, unsigned threadCount = 1)
{
	thread* threadArray = new thread[threadCount];
	unsigned from = 0;
	unsigned threadStep = size / threadCount;
	unsigned to = threadStep;
	for (unsigned i = 0; i < threadCount; ++i)
	{
		threadArray[i] = thread(floyd, matrix, way, size, from, to);
		from += threadStep;
		to += threadStep;
	}
	for (unsigned i = 0; i < threadCount; ++i)
	{
		if (threadArray[i].joinable())
		{
			threadArray[i].join();
		}
	}
}

void floydOpenCL(int** matrix, unsigned size)
{
	const char *ProgramSource =
		"__kernel void OpenCLFloyd(__global uint * pathDistanceBuffer, __global uint * pathBuffer, const unsigned int numNodes, const unsigned int pass) \n"\
		"{ \n"\
		"int xValue = get_global_id(0); \n"\
		"int yValue = get_global_id(1); \n"\
		"int k = pass; \n"\
		"int oldWeight = pathDistanceBuffer[yValue * numNodes + xValue]; \n"\
		"int tempWeight = (pathDistanceBuffer[yValue * numNodes + k] + pathDistanceBuffer[k * numNodes + xValue]); \n"\
		"if (tempWeight < oldWeight){ \n"\
		"pathDistanceBuffer[yValue * numNodes + xValue] = tempWeight; \n"\
		" } \n"\
		"} \n"\
		"\n";
	/*
	int path_dis_mat[DATA_SIZE*DATA_SIZE];
	int path_mat[DATA_SIZE*DATA_SIZE];
	int seq_dis_mat[DATA_SIZE][DATA_SIZE];

	for (int i = 0; i<DATA_SIZE*DATA_SIZE; ++i)
	{
		path_dis_mat[i] = rand() % 500 + 1;
	}

	for (int i = 0; i<DATA_SIZE; ++i)
	{
		for (int j = 0; j<DATA_SIZE; j++)
		{
			seq_dis_mat[i][j] = path_dis_mat[i*DATA_SIZE + j];
		}
	}

	for (cl_int i = 0; i < DATA_SIZE; ++i)
	{
		for (cl_int j = 0; j < i; ++j)
		{
			path_mat[i * DATA_SIZE + j] = i;
			path_mat[j * DATA_SIZE + i] = j;
		}
		path_mat[i * DATA_SIZE + i] = i;
	}
	*/
	int* path_dis_mat = new int[size*size];
	int* path_mat = new int[size*size];
	int** seq_dis_mat = new int*[size];
	for (int i = 0; i < size; ++i)
	{
		seq_dis_mat[i] = new int[size];
	}

	for (int i = 0; i<size*size; ++i)
	{
		path_dis_mat[i] = rand() % 500 + 1;
	}

	for (int i = 0; i<size; ++i)
	{
		for (int j = 0; j<size; j++)
		{
			seq_dis_mat[i][j] = path_dis_mat[i*size + j];
		}
	}

	for (cl_int i = 0; i < size; ++i)
	{
		for (cl_int j = 0; j < i; ++j)
		{
			path_mat[i * size + j] = i;
			path_mat[j * size + i] = j;
		}
		path_mat[i * size + i] = i;
	}
	cl_platform_id platform_id;
	cl_device_id device_id;
	cl_uint num_of_platforms = 0;
	cl_uint num_of_devices = 0;
	if (clGetPlatformIDs(1, &platform_id, &num_of_platforms) != CL_SUCCESS)
	{
		cout << "Unable to get platform id" << endl;
	}
	if (clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_CPU, 1, &device_id, &num_of_devices) != CL_SUCCESS)
	{
		cout << "Unable to get device_id" << endl;
	}
	cl_context_properties properties[3];
	properties[0] = CL_CONTEXT_PLATFORM;
	properties[1] = (cl_context_properties)platform_id;
	properties[2] = 0;

	cl_int err;
	cl_context context = clCreateContext(properties, 1, &device_id, NULL, NULL, &err);
	cl_command_queue command_queue = clCreateCommandQueue(context, device_id, 0, &err);
	cl_program program = clCreateProgramWithSource(context, 1, (const char **)&ProgramSource, NULL, &err);

	if (clBuildProgram(program, 0, NULL, NULL, NULL, NULL) != CL_SUCCESS)
	{
		printf("Error building program\n");
	}

	cl_kernel kernel = clCreateKernel(program, "OpenCLFloyd", &err);
	/*
	cl_mem path_dis_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(int) * DATA_SIZE * DATA_SIZE, NULL, NULL);
	cl_mem path_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(int) * DATA_SIZE * DATA_SIZE, NULL, NULL);

	clEnqueueWriteBuffer(command_queue, path_dis_buffer, CL_TRUE, 0, sizeof(int) * DATA_SIZE * DATA_SIZE, path_dis_mat, 0, NULL, NULL);
	clEnqueueWriteBuffer(command_queue, path_buffer, CL_TRUE, 0, sizeof(int) * DATA_SIZE * DATA_SIZE, path_mat, 0, NULL, NULL);
	*/

	cl_mem path_dis_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(int) * size * size, NULL, NULL);
	cl_mem path_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(int) * size * size, NULL, NULL);

	clEnqueueWriteBuffer(command_queue, path_dis_buffer, CL_TRUE, 0, sizeof(int) * size * size, path_dis_mat, 0, NULL, NULL);
	clEnqueueWriteBuffer(command_queue, path_buffer, CL_TRUE, 0, sizeof(int) * size * size, path_mat, 0, NULL, NULL);

	int temp = size;
	clSetKernelArg(kernel, 0, sizeof(cl_mem), &path_dis_buffer);
	clSetKernelArg(kernel, 1, sizeof(cl_mem), &path_buffer);
	clSetKernelArg(kernel, 2, sizeof(int), &temp);
	clSetKernelArg(kernel, 3, sizeof(int), &temp);

	size_t global[2];
	global[0] = size;
	global[1] = size;

	size_t local[2];
	int block_size = 4;
	local[0] = block_size;
	local[1] = block_size;
	int num_passes = DATA_SIZE;
	clock_t beginTime = clock();
	for (int i = 0; i < num_passes; ++i)
	{
		clSetKernelArg(kernel, 3, sizeof(int), &i);
		clEnqueueNDRangeKernel(command_queue, kernel, 2, NULL, global, local, 0, NULL, NULL);
		clFlush(command_queue);
	}
	clFinish(command_queue);
	cout << "OpenCL Floyd time: " << ((float)(clock() - beginTime)) / CLOCKS_PER_SEC << " s" << endl;
	clEnqueueReadBuffer(command_queue, path_dis_buffer, CL_TRUE, 0, sizeof(int) *DATA_SIZE * DATA_SIZE, path_dis_mat, 0, NULL, NULL);
	clEnqueueReadBuffer(command_queue, path_buffer, CL_TRUE, 0, sizeof(int) * DATA_SIZE * DATA_SIZE, path_mat, 0, NULL, NULL);
	clReleaseMemObject(path_dis_buffer);
	clReleaseMemObject(path_buffer);
	clReleaseProgram(program);
	clReleaseKernel(kernel);
	clReleaseCommandQueue(command_queue);
	clReleaseContext(context);
}

void main()
{
	const unsigned size = 600;
	int** matrix = new  int*[size];
	int** ways = new  int*[size];
	srand(time(NULL));
	for (unsigned i = 0; i < size; ++i)
	{
		matrix[i] = new int[size];
		ways[i] = new int[size];
		for (unsigned j = 0; j < size; ++j)
		{
			if (i == j)
			{
				matrix[i][j] = 0;
			}
			matrix[i][j] = rand() % 100;
		}
	}
	clock_t beginTime = clock();
	floyd(matrix, ways, size, 0, size);
	cout << "Simple Floyd time: " << (float)(clock() - beginTime) / CLOCKS_PER_SEC << endl;

	beginTime = clock();
	createThreads(matrix, ways, size, 4);
	cout << "Parallel Floyd time(4 threads): " << (float)(clock() - beginTime) / CLOCKS_PER_SEC << endl;
	
	floydOpenCL(matrix, size);
	system("pause");
}