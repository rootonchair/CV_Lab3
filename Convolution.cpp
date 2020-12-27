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
	if (sourceImage.data == NULL) return 1;

	// kích thước kernel là chẵn
	if (_kernelHeight % 2 == 0 || _kernelWidth % 2 == 0) return 1;

	int topLeftX = _kernelWidth / 2;
	int topLeftY = _kernelHeight / 2;
	int botRightX = sourceImage.cols - topLeftX - 1;
	int botRightY = sourceImage.rows - topLeftY - 1;

	// ảnh đích chứa giá trị float
	if (sourceImage.channels() == 1)
		destinationImage = Mat(botRightY - topLeftY + 1, botRightX - topLeftX + 1, CV_32FC1);
	else if (sourceImage.channels() == 3)
		destinationImage = Mat(botRightY - topLeftY + 1, botRightX - topLeftX + 1, CV_32FC3);
	else
		return 1;

	// Tạo mảng offset pixel
	int* offset = new int[_kernelHeight * _kernelWidth];
	auto ptr = offset;
	for (int y = -_kernelHeight / 2; y <= _kernelHeight / 2; ++y)
		for (int x = -_kernelWidth / 2; x <= _kernelWidth / 2; ++x)
		{
			*ptr = (sourceImage.cols * y + x) * sourceImage.channels();
			++ptr;
		}

	// chập kernel với ảnh
	auto sData = sourceImage.data;
	auto dData = (float*)destinationImage.data;
	for (int i = topLeftY; i <= botRightY; ++i)
		for (int j = topLeftX; j <= botRightX; ++j)
		{
			int sPos = (sourceImage.cols * i + j) * sourceImage.channels();
			for (int c = 0; c < sourceImage.channels(); ++c)
			{
				float val = 0;
				for (int k = 0; k < _kernelHeight * _kernelWidth; ++k)
					val += sData[sPos + offset[k]] * _kernel[k];

				*dData = val;
				++dData;
				++sPos;
			}
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
