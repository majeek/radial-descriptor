#ifndef SignalDescriptor_H_
#define SignalDescriptor_H_

#include <opencv2/core/core.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	A signal descriptor. </summary>
///
/// <remarks>	Majeek, 11/22/2013. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class SignalDescriptor{

public:
	SignalDescriptor(int row, int col, int radius, double variance, cv::Mat signal, cv::Mat signalPCA);
	~SignalDescriptor();

	int getRow();
	int getCol();
	int getRadius();
	double getVariance();
	cv::Mat getSignal();

	void setFarthestRing(cv::vector<cv::Point2i> &ring);
		
	cv::vector<cv::Point2i> getFarthestRing(){ return fFarthestRing;}
	int calculateDistance(SignalDescriptor& other);

	void setCluster(int cluster);
	int  getCluster();


private:
	//point
	int fRow;
	int fCol;
	

	//cluster
	int fCluster;

	//signal
	cv::Mat fSignal;
	
	//other properties
	double fVariance;
	int fRadius;
	cv::vector<cv::Point2i> fFarthestRing;

};
#endif