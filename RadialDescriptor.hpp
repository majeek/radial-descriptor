#ifndef RADIALDESCRIPTOR_HPP_
#define RADIALDESCRIPTOR_HPP_

#include "descriptor/SignalDescriptor.hpp"
#include "descriptor/PointDescriptor.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace RadialDescriptor{
	int getShiftValue(int vectorsCount, int radius);
	cv::vector<cv::Mat> prepareImage(cv::Mat imageToPrepare, int vectorsCount, int kernelSize, double gaussBlur, int radius);
	SignalDescriptor* createFeature(int i, int j, cv::vector<cv::Mat> padded, double interpolationValue, int radius, int vectorsCount, int minVariance);
	cv::vector<SignalDescriptor*>* sortFeaturesByVariance(cv::vector<SignalDescriptor*> *imageSignalsDescriptors);
	cv::vector<SignalDescriptor*>* getTopKFeatures(int minDistance, cv::vector<SignalDescriptor*> &sortedSignalDescriptors, int maxSignals);








	
};
#endif
