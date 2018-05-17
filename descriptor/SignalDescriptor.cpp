#include "SignalDescriptor.hpp"



SignalDescriptor::SignalDescriptor(int row, int col, int radius, double variance, cv::Mat signal, cv::Mat signalPCA) : fRow(row), fCol(col), fRadius(radius), fVariance(variance), fSignal(signal){


}

SignalDescriptor::~SignalDescriptor(){

}

int SignalDescriptor::getRadius(){
	return fRadius;
}


int SignalDescriptor::getRow(){
	return fRow;
}

int SignalDescriptor::getCol(){
	return fCol;
}
double SignalDescriptor::getVariance(){
	return fVariance;

}

cv::Mat SignalDescriptor::getSignal(){
	return fSignal;
}


void SignalDescriptor::setFarthestRing(cv::vector<cv::Point2i> &ring){
	fFarthestRing=ring;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Calculates the distance between two points as follows:
/// 			find distance of row coordinates
/// 			find distance of col coordinates
/// 			return largest distance of the two </summary>
///
/// <remarks>	majeek, 12/16/2013. </remarks>
///
/// <param name="other">	[in,out] The other. </param>
///
/// <returns>	The calculated distance. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

int SignalDescriptor::calculateDistance(SignalDescriptor& other){
	int rowDifference = std::abs(fRow-other.fRow);
	int colDifference = std::abs(fCol-other.fCol);
	if (rowDifference > colDifference)
		return rowDifference;
	else return colDifference;
}


void SignalDescriptor::setCluster(int cluster){
	fCluster = cluster;
}


int SignalDescriptor::getCluster(){
	return fCluster;
}



