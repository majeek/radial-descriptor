#ifndef _OctRing_H_
#define _OctRing_H_

#include "PointRing.h"
class OctRing : public PointRing{
public:
	OctRing(void): PointRing(){}
	OctRing(cv::Mat mat): PointRing(mat) { }
	OctRing(cv::Mat mat, cv::Point2i p): PointRing(mat, p) {}
	OctRing(cv::Mat mat, cv::Point2i p, int radius): PointRing(mat, p, radius) {}
	void setRing(int row, int col, int r);
};

#endif
