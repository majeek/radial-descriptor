// ImageToFlag.cpp : Defines the entry point for the console application.
//

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include <math.h>
#include <iostream>
#define MY_PI 3.14159265358979323846
#define MAT_SIZE 4
#define padding 20
#define floatDivide 255.0f
using namespace cv;
using namespace std;
Mat_<Vec3b> dst, srcWithMirror;
Mat_<Vec3f> dst_inter;

double cubicInterpolate(double p[4], double x) {
	return p[1] + 0.5 * x*(p[2] - p[0] + x*(2.0*p[0] - 5.0*p[1] + 4.0*p[2] - p[3] + x*(3.0*(p[1] - p[2]) + p[3] - p[0])));
}

double bicubicInterpolate(double p[4][4], double x, double y) {
	double arr[4];
	arr[0] = cubicInterpolate(p[0], y);
	arr[1] = cubicInterpolate(p[1], y);
	arr[2] = cubicInterpolate(p[2], y);
	arr[3] = cubicInterpolate(p[3], y);
	return cubicInterpolate(arr, x);
}

bool in_Range(double value, int from, int to) {
	return (from <= floor(value)) && (ceil(value) < to);
}
int newRound(double num) {
	if (abs((num - (int)num)) > 0.5)
		return (int)num + 1;
	else
		return (int)num;
}
double uFunc(double d) {
	if (0 <= d && d < 1)
		return (3 / 2) * (pow(d, 3)) - (5 / 2) * (pow(d, 2)) + 1;
	if (1 <= d && d < 2)
		return -(3 / 2) * (pow(d, 3)) + (5 / 2) * (pow(d, 2)) - 4 * d + 2;
	return 0;
}

int main(int argc, char** argv) {
	int wave_frequency = 3; //Default wave frequency
	double amplitude = 0.03; //Defined after load of picture to be 3%
	
	
	char* interpolation = "c";
	Mat src = imread("D:/majeek2/image-3.jpg", IMREAD_ANYCOLOR | IMREAD_ANYDEPTH);   // Read the file


	if ((!src.data) || src.empty())                   // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl;
		waitKey(0);                        // Wait for a keystroke in the window
		return -2;
	}
	amplitude = amplitude * src.rows;   //% of picture height.


	namedWindow("Before", WINDOW_AUTOSIZE);  // Create a window for display.
	imshow("Before", src);  // Show Before Image
	/// Set the dst image the same type as src but in size + amplitude for height.
	Size size(src.cols, (int)ceil(src.rows + 2 * amplitude));
	//Create destination Matrix
	Size sizeMirror(src.cols + 2, src.rows + 2);
	srcWithMirror = Mat::zeros(sizeMirror, src.type());
	dst = Mat::zeros(size, src.type());
	dst_inter = Mat::zeros(size, CV_32F);
	//Set Background to white.
	dst.setTo(Scalar(255, 255, 255));
	dst_inter.setTo(Scalar(255, 255, 255));
	cv::copyMakeBorder(src, srcWithMirror, padding, padding, padding, padding, BORDER_REPLICATE, 0);

	//Distort done here using forward scanning with no interpolation.
	for (int x = 0; x < src.cols; x++) {
		for (int y = 0; y < src.rows; y++) {
			int new_y = (int)(y
				+ amplitude
				* (1 + sin(wave_frequency * MY_PI * x / src.cols)));
			dst.at<Vec3b>(new_y, x) = src.at<Vec3b>(y, x);
		}
	}
	/*namedWindow("No Interpolation", WINDOW_AUTOSIZE); // Create a window for display.
	imshow("No Interpolation", dst);  // Show After  Image
	waitKey();*/

	//Distort done here using backward scanning with interpolation.
	for (int x = 0; x < dst_inter.cols; x++) {
		for (int dst_y = 0; dst_y < dst_inter.rows; dst_y++) {
			double y = dst_y
				- amplitude
				* (1 + sin(wave_frequency * MY_PI * x / src.cols));
			int src_y;
			double src_y1, src_y2;
			switch (interpolation[0]) {
			case 'n':
				src_y = (int)round(y);
				if (in_Range(src_y, 0, src.rows)){
					dst_inter.at<Vec3f>(dst_y, x)(0) = src.at<Vec3b>(src_y, x)(0) / floatDivide;
					dst_inter.at<Vec3f>(dst_y, x)(1) = src.at<Vec3b>(src_y, x)(1) / floatDivide;
					dst_inter.at<Vec3f>(dst_y, x)(2) = src.at<Vec3b>(src_y, x)(2) / floatDivide;
				}
				break;
			case 'b': //Bi-Linear
				src_y1 = floor(y);
				src_y2 = ceil(y);
				if (in_Range(src_y1, 0, src.rows)) {
					if (in_Range(src_y2, 0, src.rows)) {         //Both in range
						for (int i = 0; i < 3; i++) {
							dst_inter.at<Vec3f>(dst_y, x)(i) = ((src.at<Vec3b>(
								(int)src_y1, x)(i)* (src_y2 - y)
								+ src.at<Vec3b>((int)src_y2, x)(i)
								* (y - src_y1))) / floatDivide;
						}
					}
					else {
						// 1 in range
						dst_inter.at<Vec3f>(dst_y, x) = src.at<Vec3b>(
							(int)src_y1, x) / floatDivide;
					}
				}
				else if (in_Range(src_y2, 0, src.rows)) {
					// 2 in range
					dst_inter.at<Vec3f>(dst_y, x) = src.at<Vec3b>((int)src_y2, x) / floatDivide;
				}                              //else None in range
				break;
			case 'c': {
				if (in_Range(y, 0, src.rows)) {
					double distFromSrcY = abs(y - round(y));
					int mirrX = x + 2;
					int mirrY = ((int)newRound(y)) + 2;
					Point p1;
					if (y < round(y)) { //Use bottomRight
						p1 = Point(mirrX - 2 + padding, mirrY - 2 + padding);

						Rect rect(p1, Size(4, 4));
						Mat cropped = srcWithMirror(rect);

						Rect roi(mirrX - 2 + padding, mirrY - 2 + padding, 4, 4);
						cv::Mat croppedRef(srcWithMirror, roi);
						croppedRef.copyTo(cropped);

						for (int i = 0; i < 3; i++) {
							double croppedDouble[MAT_SIZE][MAT_SIZE];
							for (int row = 0; row < MAT_SIZE; row++)
								for (int col = 0; col < MAT_SIZE; col++)
									croppedDouble[row][col] = cropped.at<Vec3b>(row, col)(i);

							dst_inter.at<Vec3f>(dst_y, x)(i) = (bicubicInterpolate(croppedDouble, 1.0, 1.0 - distFromSrcY)) / floatDivide;
						}
					}
					else {  //Use topRight
						p1 = Point(mirrX - 1 + padding, mirrY - 1 + padding);

						Rect rect(p1, Size(4, 4));
						Mat cropped = srcWithMirror(rect);

						Rect roi(mirrX - 1 + padding, mirrY - 1 + padding, 4, 4);
						cv::Mat croppedRef(srcWithMirror, roi);
						croppedRef.copyTo(cropped);

						for (int i = 0; i < 3; i++) {
							double croppedDouble[MAT_SIZE][MAT_SIZE];
							for (int row = 0; row < MAT_SIZE; row++)
								for (int col = 0; col < MAT_SIZE; col++)
									croppedDouble[row][col] = cropped.at<Vec3b>(row, col)(i);
							double tmp = bicubicInterpolate(croppedDouble, 1, distFromSrcY);
							dst_inter.at<Vec3f>(dst_y, x)(i) = (tmp) / floatDivide;
						}
					}

				}
				break;
			}
			default:
				src_y = ((int)round(y));
				if (in_Range(src_y, 0, src.rows))
					dst_inter.at<Vec3f>(dst_y, x) = src.at<Vec3b>(src_y, x) / floatDivide;
				break;

			}

		}
	}
	cout << "\ndone backward \n";



	namedWindow("After", WINDOW_AUTOSIZE); // Create a window for display.
	imshow("After", dst_inter);  // Create a window for display.

	waitKey(0);  // Wait for a keystroke in the window
	return 0;
}


