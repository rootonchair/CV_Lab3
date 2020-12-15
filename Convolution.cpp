#include "Convolution.h"

vector<float> Convolution::GetKernel()
{
	return _kernel;
}

void Convolution::SetKernel(vector<float> kernel, int kWidth, int kHeight)
{
	_kernelHeight = kHeight;
	_kernelWidth = kWidth;
	_kernel = kernel;
}

int Convolution::DoConvolution(const Mat& sourceImage, Mat& destinationImage)
{
	int topLeftX = _kernelWidth / 2;
	int topLeftY = _kernelHeight / 2;
	int botRightX = sourceImage.cols - topLeftX - 1;
	int botRightY = sourceImage.rows - topLeftY - 1;

	destinationImage = Mat(botRightY - topLeftY + 1, botRightX - topLeftX + 1, CV_8UC1);

	int* offset = new int[_kernelHeight * _kernelWidth];
	auto ptr = offset;
	for (int y = -_kernelHeight / 2; y <= _kernelHeight / 2; ++y)
		for (int x = -_kernelWidth / 2; x <= _kernelWidth / 2; ++x)
		{
			*ptr = sourceImage.cols * y + x;
			++ptr;
		}

	auto sData = sourceImage.data;
	auto dData = destinationImage.data;
	for (int i = topLeftY; i <= botRightY; ++i)
		for (int j = topLeftX; j <= botRightX; ++j)
		{
			float val = 0;
			int sPos = sourceImage.cols * i + j;
			for (int k = 0; k < _kernelHeight * _kernelWidth; ++k)
				val += sData[sPos + offset[k]] * _kernel[k];

			if ((int)val > 255) val = 255;
			if ((int)val < 0) val = 0;
			*dData = val;
			dData += 1;
		}

	delete[] offset;

	return 0;
}

Convolution::Convolution()
{
	_kernelHeight = _kernelWidth = 0;
}

Convolution::~Convolution()
{
}