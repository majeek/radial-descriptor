#ifndef _Sampler_H_
#define _Sampler_H_

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>
#include <numeric>
#include <cstdio>
#include <cstdlib>
#include <cmath>



namespace Sampler {
	static const int BILINEAR = 0 ;
	static const int QUADRIC  = 1 ;
	static const int CUBIC    = 2 ;
	void bilinear(cv::vector<double>& src, cv::vector<double>& dst, const int size, const double rate) ;
	void quardic(cv::vector<double>& src, cv::vector<double>& dst, const int size, const double rate) ;
	void cubic(cv::vector<double>& src, cv::vector<double>& dst, const int size, const double rate) ;

	void sample(cv::vector<double>& src, cv::vector<double>& dst, const int size, const int method);

	void observeLabelsAndMeans(const cv::Mat labels, const cv::Mat means, int height, int width);
	void observeEMProbs(const cv::Mat probs);
	void observeWeights(const cv::Mat weights);
};

 #endif