#ifndef _PointRing_H_
#define _PointRing_H_

#include <opencv2\core\core.hpp>

// This is the base class to collect the ring pixel. 
class PointRing{
protected:
	cv::Mat              _mat ;  // reference to the image 
	cv::Point2i          _point  ;  // the pixel coordintaes 
	int              _radius ;
	cv::vector<cv::Point2i>  _ring ;

public:
	PointRing(void){ }
	PointRing(cv::Mat mat): _mat(mat){ }
	PointRing(cv::Mat mat, cv::Point2i p):  _mat (mat), _point(p) {}
	PointRing(cv::Mat mat, cv::Point2i p, int radius): _mat(mat), _point(p), _radius(radius) {}

	~PointRing(void){}

	void setMat(cv::Mat mat)     { _mat   = mat ; }
	void setPoint(cv::Point2i p)  { _point = p ; }
	void setRadius(int r)    { _radius = r ;}

	cv::Mat     getImage()         { return _mat ;  }
	int     getRadius()      { return _radius ; }
	cv::Point2i getPoint()       { return _point; }
	cv::vector<cv::Point2i> getRing() {return _ring; }

	bool isInside(int row, int col, int radius) ;

	void createRingCoordinates() { createRingCoordinates(_point, _radius);	}
	// Virtual functions 
	virtual void createRingCoordinates(cv::Point2i point, int radius);
	void createRingCoordinatesSquare(cv::Point2i, int);
};

#endif
