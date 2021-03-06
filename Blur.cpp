﻿
#include <math.h>
#include "Blur.h"
#include "Convolution.h"
#include <algorithm>
#define pi 3.141592653589793238462643383279502884L /* pi */ 

int Blur::BlurImage(const Mat& sourceImage, Mat& destinationImage, int kWidth, int kHeight, int method) {
	if (sourceImage.empty())
		return 1;
	if (kHeight % 2 == 0 || kWidth % 2 == 0) return 1;
	
	Convolution Convo;
	vector<float> kernel;
	vector<int> offset;
	switch (method)
	{
	case 0: {
		//Lọc trung bình
		int n = kWidth*kHeight;
		for (int i = 0; i < n; i++)
			kernel.push_back(1.0 / n);
		Convo.SetKernel(kernel, kWidth, kHeight);
		Mat tmp;
		Convo.DoConvolution(sourceImage, tmp);

		destinationImage.create(tmp.rows, tmp.cols, sourceImage.type());

		// đưa các giá trị float về uchar
		auto pDes = destinationImage.data;
		auto pTmp = (float*)tmp.data;

		int num = tmp.cols * tmp.rows * tmp.channels();
		for (int i = 0; i < num; ++i)
		{
			pDes[i] = pTmp[i];
		}

		return 0;
	}
		break;
	case 2: {
		// Gaussian

		// lấy sigma bằng 1/3 kích thước cửa sổ
		auto kernel = FilterCreator(kWidth / 2, kWidth / 3.0);

		Convo.SetKernel(kernel, kWidth, kHeight);
		Mat tmp;
		Convo.DoConvolution(sourceImage, tmp);

		destinationImage.create(tmp.rows, tmp.cols, sourceImage.type());

		// đưa các giá trị float về uchar
		auto pDes = destinationImage.data;
		auto pTmp = (float*)tmp.data;

		int num = tmp.cols * tmp.rows * tmp.channels();
		for (int i = 0; i < num; ++i)
		{
			pDes[i] = pTmp[i];
		}

		return 0;
	}
	case 1:
	{
		int topLeftX = kWidth / 2;
		int topLeftY = kHeight / 2;
		int botRightX = sourceImage.cols - topLeftX - 1;
		int botRightY = sourceImage.rows - topLeftY - 1;

		if (sourceImage.channels() == 1)
			destinationImage = Mat(botRightY - topLeftY + 1, botRightX - topLeftX + 1, CV_8UC1);
		else if (sourceImage.channels() == 3)
			destinationImage = Mat(botRightY - topLeftY + 1, botRightX - topLeftX + 1, CV_8UC3);
		else
			return 1;

		// tính mảng offset
		offset.assign(kHeight * kWidth, 0);
		auto ptr = offset.data();
		for (int y = -kHeight / 2; y <= kHeight / 2; ++y)
			for (int x = -kWidth / 2; x <= kWidth / 2; ++x)
			{
				*ptr = (sourceImage.cols * y + x) * sourceImage.channels();
				++ptr;
			}

		auto sData = sourceImage.data;
		auto dData = destinationImage.data;
		for (int i = topLeftY; i <= botRightY; ++i)
			for (int j = topLeftX; j <= botRightX; ++j)
			{
				int sPos = (sourceImage.cols * i + j) * sourceImage.channels();
				for (int c = 0; c < sourceImage.channels(); ++c)
				{
					vector<uchar> window(kWidth* kHeight, 0);
					for (int k = 0; k < kWidth * kHeight; ++k)
						window[k] = sData[sPos + offset[k]];
					// sắp xếp và lấy trung vị
					sort(window.begin(), window.end());
					auto val = window[(kWidth * kHeight - 1) / 2];
					*dData = val;
					++dData;
					++sPos;
				}
			}

		return 0;
	}
	default:
		return 1;
	}
	return 0;
}

Blur::Blur() {}
Blur::~Blur() {}

vector<float> Blur::FilterCreator(int R, double sigma)
{
	vector<float> result((R * 2 + 1) * (R * 2 + 1), 0);

	auto ptr = result.data();
	float sum = 0.0;
	for (int x = -R; x <= R; x++)
	{
		for (int y = -R; y <= R; y++)
		{
			*ptr = (exp(-(x * x + y * y) / (2 * sigma * sigma)) / (2 * pi * sigma * sigma));
			sum += *ptr;
			++ptr;
		}
	}

	// chuẩn hóa kernel
	ptr = result.data();
	for (int x = -R; x <= R; x++)
	{
		for (int y = -R; y <= R; y++)
		{
			*ptr /= sum;
			++ptr;
		}
	}

	return result;
}
