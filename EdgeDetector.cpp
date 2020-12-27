#include "EdgeDetector.h"

int EdgeDetector::DetectEdge(const Mat& sourceImage, Mat& destinationImage, int kWidth, int kHeight, int method)
{
	if (sourceImage.empty())
		return 1;

	switch (method)
	{
	case 1: {
		//Lọc Sobel
		Convolution Convo;
		vector<float> Kernelx, Kernely;

		float Wx[9] = { 1, 0, -1, 2, 0, -2, 1, 0, -1 };
		//		[1  0 -1]
		//	Wx =[2  0 -2] 
		//		[1  0 -1]
		float Wy[9] = { -1, -2, -1, 0, 0, 0, 1, 2, 1 };
		//		[-1  -2  -1]
		//	Wx =[ 0   0   0] 
		//		[ 1   2   1]
		//tạo 2 bộ lọc kernel
		for (int i = 0; i < 9; i++) {
			Kernelx.push_back(Wx[i]);
			Kernely.push_back(Wy[i]);
		}

		Mat fx, fy;
		//Tính tích chập ma trận fx với Kernelx
		Convo.SetKernel(Kernelx, 3, 3);
		Convo.DoConvolution(sourceImage, fx);
		//Tính tích chập ma trận fy với Kernely
		Convo.SetKernel(Kernely, 3, 3);
		Convo.DoConvolution(sourceImage, fy);

		//Tạo ma trận đích có loại giống ma trận nguồn
		destinationImage.create(fx.rows, fx.cols, sourceImage.type());

		//lấy căn của tổng bình phương fx fy và truncate về đoạn [0, 255]
		auto pDes = destinationImage.data;
		auto pX = (float*)fx.data;
		auto pY = (float*)fy.data;

		int num = fx.cols * fx.rows * fx.channels();
		for (int i = 0; i < num; ++i)
		{
			int val = sqrt(pX[i] * pX[i] + pY[i] * pY[i]);
			if (val > 255) val = 255;
			pDes[i] = val;
		}

		return 0;
	}
		  break;
	case 2: {
		//Lọc Prewitt
		Convolution Convo;
		vector<float> Kernelx, Kernely;

		float Wx[9] = { 1, 0, -1, 1, 0, -1, 1, 0, -1 };
		//		[1  0 -1]
		//	Wx =[1  0 -1] 
		//		[1  0 -1]
		float Wy[9] = { -1, -1, -1, 0, 0, 0, 1, 1, 1 };
		//		[-1  -1  -1]
		//	Wx =[ 0   0   0] 
		//		[ 1   1   1]
		//tạo 2 bộ lọc kernel
		for (int i = 0; i < 9; i++) {
			Kernelx.push_back(Wx[i]);
			Kernely.push_back(Wy[i]);
		}

		Mat fx, fy;
		//Tính tích chập ma trận fx với Kernelx
		Convo.SetKernel(Kernelx, 3, 3);
		Convo.DoConvolution(sourceImage, fx);
		//Tính tích chập ma trận fy với Kernely
		Convo.SetKernel(Kernely, 3, 3);
		Convo.DoConvolution(sourceImage, fy);

		//Tạo ma trận đích có loại giống ma trận nguồn
		destinationImage.create(fx.rows, fx.cols, sourceImage.type());

		//lấy căn của tổng bình phương fx fy và truncate về đoạn [0, 255]
		auto pDes = destinationImage.data;
		auto pX = (float*)fx.data;
		auto pY = (float*)fy.data;

		int num = fx.cols * fx.rows * fx.channels();
		for (int i = 0; i < num; ++i)
		{
			int val = sqrt(pX[i] * pX[i] + pY[i] * pY[i]);
			if (val > 255) val = 255;
			pDes[i] = val;
		}

		return 0;
	}
		  break;
	case 3: {
		//Lọc Laplace
		float Laplace[9] = { 1, 1, 1, 1, -8, 1, 1, 1, 1 };
		//			  [1  1  1]
		//	Laplace = [1 -8  1]
		//			  [1  1  1]
		//Tạo Kernel
		vector<float> Kernel;
		for (int i = 0; i < 9; i++)
			Kernel.push_back(Laplace[i]);

		//Tính tích chập của ảnh với ma trận Kernel
		Convolution Convo;
		Convo.SetKernel(Kernel, 3, 3);
		Mat tmp;
		Convo.DoConvolution(sourceImage, tmp);

		//truncate các giá trị về đoạn [0, 255]
		destinationImage.create(tmp.rows, tmp.cols, sourceImage.type());

		auto pDes = destinationImage.data;
		auto pTmp = (float*)tmp.data;

		int num = tmp.cols * tmp.rows * tmp.channels();
		for (int i = 0; i < num; ++i)
		{
			int val = pTmp[i];
			if (val > 255) val = 255;
			if (val < 0) val = 0;
			pDes[i] = val;
		}

		return 0;
	}
		  break;
	default:
		break;
	}
	return 1;
}

EdgeDetector::EdgeDetector()
{
}

EdgeDetector::~EdgeDetector()
{
}
