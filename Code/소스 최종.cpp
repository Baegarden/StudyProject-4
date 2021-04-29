#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cstdlib>
#include <cmath>

typedef unsigned char BYTE;
using namespace cv;

void histogram_equalization(Mat &img, Mat &out, int *hist, int *hist_sum, int height, int width);

void main()
{
	
	VideoCapture vcap("C:\\풍경1.avi");

	namedWindow("Video1", WINDOW_NORMAL);
	namedWindow("Video2", WINDOW_NORMAL);
	namedWindow("Video3", WINDOW_NORMAL);

	Mat maFrame;
	Mat img_gray;

	vcap.read(maFrame);

	int width, height;
	int *Histogram = new int[256];
	int *Histogram_Sum = new int[256];

	width = maFrame.cols;
	height = maFrame.rows;

	Mat output_hist(height, width, CV_8UC1);

	clock_t start = clock();
	while (vcap.read(maFrame))
	{
		cvtColor(maFrame, img_gray, CV_BGR2GRAY);
		histogram_equalization(img_gray, output_hist, Histogram, Histogram_Sum, height, width);

		imshow("Video1", maFrame);
		imshow("Video2", img_gray);
		imshow("Video3", output_hist);

		if (waitKey(25) >= 0)
			break;

		clock_t end = clock();

		printf("Elapsed time(CPU) = %u ms\n", end - start);
	}
	

	vcap.release();
	destroyAllWindows();
}

void histogram_equalization(Mat &img, Mat &out, int *hist, int *hist_sum, int height, int width)
{
	int sum = 0;
	for (int i = 0; i < 256; i++)
	{
		hist[i] = 0;
		hist_sum[i] = 0;
	}

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			int value = img.at<uchar>(i, j);
			hist[value] += 1;
		}
	}

	//입력 영상의 누적 히스토그램 계산
	for (int i = 0; i < 256; i++)
	{
		sum += hist[i];
		hist_sum[i] = sum;
	}

	//입력 그레이스케일 영상의 정규화된 누적 히스토그램 계산
	float normalized_Histogram_Sum[256] = { 0.0, };
	int image_size = height * width;
	for (int i = 0; i < 256; i++)
	{
		normalized_Histogram_Sum[i] = hist_sum[i] / (float)image_size;
	}

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			out.at<uchar>(i, j) = (BYTE)(round(normalized_Histogram_Sum[img.at<uchar>(i, j)] * 255));
		}
	}
}
