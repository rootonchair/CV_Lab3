#include <iostream>
#include <cstring>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "Blur.h"
#include "EdgeDetector.h"
#include "TypeConvert.h"

using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
	try
	{
		if (argc < 3)
			throw std::string("Invalid Syntax");

		char* command = argv[1];
		char* inputPath = argv[2];

		Mat orgImage = imread(inputPath, CV_LOAD_IMAGE_ANYCOLOR);
		Mat resultImage;
		int result;
		Blur blur;
		EdgeDetector edgeDetector;

		if (strcmp(command, "--mean") == 0)
		{
			if (argc < 5)
				throw std::string("Invalid Syntax");
			int kW = ParseInt(argv[3]);
			int kH = ParseInt(argv[4]);
			result = blur.BlurImage(orgImage, resultImage, kW, kH, 0);
		}
		else if (strcmp(command, "--median") == 0)
		{
			if (argc < 5)
				throw std::string("Invalid Syntax");
			int kW = ParseInt(argv[3]);
			int kH = ParseInt(argv[4]);
			result = blur.BlurImage(orgImage, resultImage, kW, kH, 1);
		}
		else if (strcmp(command, "--gauss") == 0)
		{
			if (argc < 5)
				throw std::string("Invalid Syntax");
			int kW = ParseInt(argv[3]);
			int kH = ParseInt(argv[4]);
			result = blur.BlurImage(orgImage, resultImage, kW, kH, 2);
		}
		else if (strcmp(command, "--sobel") == 0)
		{
			result = edgeDetector.DetectEdge(orgImage, resultImage, 3, 3, 1);
		}
		else if (strcmp(command, "--prewitt") == 0)
		{
			result = edgeDetector.DetectEdge(orgImage, resultImage, 3, 3, 2);
		}
		else if (strcmp(command, "--laplace") == 0)
		{
			result = edgeDetector.DetectEdge(orgImage, resultImage, 3, 3, 3);
		}
		else
		{
			throw std::string("No such command ") + command;
		}

		if (result != 0) {
			throw std::string("Cannot perform operation. Maybe due to unexisted image or invalid command arguments");
		}

		namedWindow("Original Image");
		imshow("Original Image", orgImage);
		namedWindow("Result");
		imshow("Result", resultImage);
		waitKey(0);
	}
	catch (std::string errMsg) {
		std::cout << errMsg;
	}

	return 0;
}