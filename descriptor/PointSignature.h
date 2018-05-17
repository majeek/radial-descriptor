#ifndef _PointSignature_H_
#define _PointSignature_H_

#include <opencv2/core/core.hpp>
#include "OctRing.h"
#include "Sampler.h"


class PointSignature {
protected:

	int      _radius;
	cv::vector<double>  fSamplePoints;
	double _variance;
public:
	PointSignature():  _variance(0){ }
	PointSignature(int radius): _radius(radius), _variance(0){}
	PointSignature(cv::Mat image, int radius):_radius(radius),_variance(0){}

	~PointSignature(void);

	void  setRadius(int r ) { _radius = r ; }
	void  setVariance();

	int   getRadius()          { return _radius ; }
	double getVariance()        { setVariance(); return _variance ; }
	int getSamplePointsSize(){ return (int)fSamplePoints.size(); }

	double getSamplePoint(int i)     { return fSamplePoints.at(i) ; }
	void  setSamplePoints(cv::Point2i, cv::vector<cv::Point2i>&, cv::Mat); 
	
	cv::vector<double>&  getSamplePoints()   { return fSamplePoints ; }
	double alignedDistance(PointSignature& other, const int sampleSize, const int samplingMethod);
	double distance(PointSignature& sig);
	
};

#endif 

