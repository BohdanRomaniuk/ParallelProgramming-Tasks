#pragma once
#include "MatrixProcessing.h"
#include <vector>

Mat Floyd(Mat W, int _k);

Mat FloydParallel(Mat W, int _k, int p);

void _FloydPart(Mat W, Mat Wk, int _k, int k, int from, int to);

std::vector<double> OpenCLFloyd(Mat W, int _k);