#ifndef _PointDescriptor_H_
#define _PointDescriptor_H_

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/ml/ml.hpp>
#include "Sampler.h"
#include "PointRing.h"
#include "PointSignature.h"





class PointDescriptor{
private:

	cv::vector<cv::Mat> fPyramidMats;
	cv::Mat fImageMat;
	cv::Point2i fPoint;
	double fStartingWeight;
	int fNumberOfPyramidLevels;
	cv::vector<PointSignature*> fSignatures;
	//_dominantSet is the subset of _descriptor matrix
	// _descriptorMat includes all vectors retrieved from the _signatures vector
	cv::Mat	fDominantSet; //each row is a PointSignature 
	cv::Mat	fFTDominantSet;
	cv::Mat	fComparedDominantSet;
	cv::Mat	fComparedFTDominantSet;
	cv::Mat	fDescriptorMat;
	cv::Mat fDominantSignal;
	cv::vector<cv::Point2i> fFarthestRing;
	cv::Mat fMellinFourierMat;
	int	fMinRadius;
	int	fMaxRadius;
	cv::Mat FFT(cv::Mat img);
	cv::Mat switchQuadrantsWithoutMiddleRowAndColumnIfOdd(cv::Mat);

	cv::Mat convertToLogPolarUsingIplImage(cv::Mat);
	cv::Mat convertToLogPolar(cv::Mat);
	double calculateEuclideanDistance(cv::Mat, cv::Mat);
	double calculateMahalanobisDistance(cv::Mat, cv::Mat);
	double calculateChiSquaredDistance(cv::Mat, cv::Mat);
	cv::Mat normalizeVectors(cv::Mat);
	cv::Mat cutMatrix(cv::Mat, int);



public: 
	const static int EUCLIDEAN = 1;
	const static int MAHALANOBIS = 2;
	const static int CHI_SQUARED  = 4;
	PointDescriptor() {}
	
	//PointDescriptor(cv::Mat mat, cv::Point2i p, double interpolationValue): fImageMat(mat), fPoint(p),fStartingWeight(interpolationValue){}
	PointDescriptor(cv::Mat mat, cv::Point2i p, double interpolationValue, int numberOfPyramidLevels, cv::vector<cv::Mat> pyramidMats) : fImageMat(mat), fPoint(p), fStartingWeight(interpolationValue), fNumberOfPyramidLevels(numberOfPyramidLevels), fPyramidMats(pyramidMats){}
	~PointDescriptor(void);

	void setMat(cv::Mat mat)      { fImageMat = mat ; }
	void setPoint(cv::Point2i p)  { fPoint = p ;}
	int getMinRadius() { return fMinRadius; }
	int getMaxRadius() { return fMaxRadius; }

	cv::Mat runEM(int nclusters);
	void createRingSignatureSet(int radius); 
	cv::vector<cv::Point2i> getFarthestRing(){ return fFarthestRing;}
	void calculateDescriptorMatrix();//(int sampleSize, int method);
	void findDominantSet();//(int size);
	void calculateDominantSignal();
	void calculateDominantSignal(double);
	cv::Mat getDominantSignal();
	double getAverageVariance();
	cv::Mat getDominantSet() { return fDominantSet; }
	cv::Point2i getCenterPoint() { return fPoint; }
	double calculateDistance(PointDescriptor&, bool = false, int = 0);


	cv::Mat getComparedDominantSet(){ return fComparedDominantSet; };
	cv::Mat getDescriptorMatrix(){ return fDescriptorMat; };	

	void setComparedDominantSet(cv::Mat dominantSet){ fComparedDominantSet = dominantSet; };
	
	void setInterpolationValue(int);


};

#endif 