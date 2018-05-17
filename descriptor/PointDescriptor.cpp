#include "PointDescriptor.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Creates a set of ring signatures from rmin to rmax. </summary>
///
/// <param name="rmin">	The minimum radius. </param>
/// <param name="rmax">	The maximum radius. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

void PointDescriptor::createRingSignatureSet(int radius){

	//clear signaturs in case of their # change.
	for (int i=0; i<fSignatures.size();i++)
		delete fSignatures.at(i);
	fSignatures.clear();
	cv::Mat image = fPyramidMats[1];
	cv::Point2i point(fPoint.x*2,fPoint.y*2);
	fFarthestRing.clear();

	for (int i=0;i<fNumberOfPyramidLevels;i++){
		PointRing ring(image);
		PointSignature* sig;
		ring.setPoint(point);
		ring.setRadius(radius);
		ring.createRingCoordinates();
		cv::vector<cv::Point2i> ringCoordinates = ring.getRing();
		
		sig = new PointSignature(image, radius);
		sig->setSamplePoints(point, ring.getRing(), image);
		sig->setVariance();
		fSignatures.push_back(sig);
		//resize image using gaussian pyramid
		image = fPyramidMats[i + 1];
		point = cv::Point2i(point.x/2,point.y/2);
	}



	//old method 2: pixel, get values on given radius, resize then again, until vectorNum times.
	/* 
	//clear signaturs in case of their # change.
	for (int i=0; i<fSignatures.size();i++)
		delete fSignatures.at(i);
	fSignatures.clear();
	cv::Mat image;
	fImageMat.copyTo(image);
	cv::Point2i point(fPoint.x,fPoint.y);
	fFarthestRing.clear();
	//method 2: new method, scale space (resize) and get radius 2
	for (int i=0;i<vectorNum;i++){
		PointRing ring(image);
		PointSignature* sig;
		ring.setPoint(point);
		ring.setRadius(radius);
		ring.createRingCoordinates();
		cv::vector<cv::Point2i> ringCoordinates = ring.getRing();
		if (fFarthestRing.size()==0)
			fFarthestRing = ring.getRing();
		sig = new PointSignature(image,2);
		sig->setSamplePoints(point, ring.getRing(),image);
		sig->setVariance();
		fSignatures.push_back(sig);
		//resize image
		cv::resize(image,image,cv::Size(image.cols/2,image.rows/2));
		point = cv::Point2i(point.x/2,point.y/2);
	}
	*/




	//method 1: old method, rings from radius 2 to 20
	/*
	fMinRadius=rmin;
	fMaxRadius=rmax;

	fullRingMat.at<unsigned char>(fPoint) = fImageMat.at<unsigned char>(fPoint);
	for (int i=1;i<rmin;i++){
		ring.setRadius(i);
		ring.createRingCoordinates();
		cv::vector<cv::Point2i> r = ring.getRing();
		//std::cout << i << ": ";
		//for (unsigned j = 0;j<r.size();j++)
		//	std::cout << r.at(j) << ",";
		//std::cout << endl;
		sig = new PointSignature(fImageMat,i);
		sig->setRingMat(fullRingMat);
		fullRingMat = sig->getRingMat();
		delete sig;
	}
	for ( int i = rmin; i <= rmax; i++ ){
		ring.setRadius(i);
		ring.createRingCoordinates();
		cv::vector<cv::Point2i> ringCoordinates = ring.getRing();
		//std::cout << i << ": ";
		//for (unsigned j = 0;j<r.size();j++)
		//	std::cout << r.at(j) << ",";
		//std::cout << endl;
		sig = new PointSignature(fImageMat,i);
		sig->setRingMat(fullRingMat);
		sig->setSamplePoints(fPoint, ring.getRing());
		fullRingMat = sig->getRingMat();
		sig->setVariance();
		fSignatures.push_back(sig);
		if (i==rmax)
			fFarthestRing = ringCoordinates;
	}

	/*cv::Mat croppedMat(2*rmax,2*rmax,CV_8UC1);
	for (int row=fPoint.y-rmax;row<fPoint.y+rmax;row++){
		for (int col=fPoint.x-rmax;col<fPoint.x+rmax;col++){
			//cout << "from " << row <<"," << col << " to " << row-fPoint.y+rmax << "," << col-fPoint.x+rmax << endl; 
			croppedMat.at<unsigned char>(row-fPoint.y+rmax,col-fPoint.x+rmax)=fullRingMat.at<unsigned char>(row,col); 
		}
	}
	fMat.release();
	*/
	//return croppedMat;
	//int num = rand();
	//stringstream ss;
	//ss << "C:/RadialDescriptor/segment-" << fPoint << ".png";
	//string str = ss.str();
	//cv::namedWindow(str,CV_WINDOW_NORMAL);
	//cv::imwrite(ss.str(),croppedMat);
	//cv::imshow(str, fullRingMat);
	//cv::waitKey();
}


/// <summary>	Destructor. </summary>
PointDescriptor::~PointDescriptor(void){
	 int size = (int)fSignatures.size();
	for ( int i = 0; i<size; i++)
		if (fSignatures.at(i)!=NULL)
			delete(fSignatures.at(i));
	fSignatures.clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Calculates average variance over all signatures. </summary>
///
/// <returns>	The average variance. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

double PointDescriptor::getAverageVariance(){
	double allVariance =0;
	 int sigSize = fSignatures.size();
	for ( int i = 0; i<sigSize; i++)
		allVariance += fSignatures.at(i)->getVariance();
	return allVariance/sigSize;
}

/// <summary>	Calculates the descriptor matrix. </summary>
void PointDescriptor::calculateDescriptorMatrix(){
	//(int sampleSize, int method){

	//method 1: regular method with sampling of intensity values from raduis 2 to 20
	/*
	int cols = sampleSize;//fSignatures.at(0)->getSamplePointsSize();
	int rows = fSignatures.size();

	fDescriptorMat = cv::Mat::ones(rows,cols,CV_64FC1)*99999;
	//cv::FileStorage fs("rows.yml", cv::FileStorage::WRITE);

	for ( int row = 0; row < rows; row++ ){
	cv::vector<double> allSamples = fSignatures.at(row)->getSamplePoints();

	cv::vector<double> sampledResult;
	Sampler::sample(allSamples,sampledResult,cols,method);

	for ( int col = 0; col < cols; col++ )
	fDescriptorMat.at<double>(row,col) = sampledResult.at(col);
	}
	*/
	//method 2: scale space and taking pixel values of radius 2 or 3
	// the scale space is created when created the signatures. we just combine the signatures here 
	// into one matrix
	int cols = fSignatures.at(0)->getSamplePointsSize();
	 int rows = fSignatures.size();
	fDescriptorMat = cv::Mat::ones(rows, cols, CV_64FC1) * 99999;
	for ( int row = 0; row<rows; row++){
		cv::vector<double> allSamples = fSignatures.at(row)->getSamplePoints();
		for (int col = 0; col < cols; col++)
			fDescriptorMat.at<double>(row, col) = allSamples.at(col);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Selects the dominant set and adds them to vector. </summary>
///
/// <param name="size">	Size of the dominant set. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

void PointDescriptor::findDominantSet(){//int size){
	//method 1 using estimation maximization
	//fDominantSet = runEM(size);
	//cout << "dominant set: " << fDominantSet << endl;
	//system("pause");
	
	//method 2 using regular distance, row 2, row 5, row 8 etc
	/*fDominantSet = cv::Mat();
	for (int i=1;i<20;i+=size)
		fDominantSet.push_back(fDescriptorMat.row(i));
		*/
	//method 3 using scale space of resizing: image, image/2 image/4 image/8 image/16
	// done in sampling !! we just copy descriptormap to dominant set and be done with it.
	//fDominantSet = cv::Mat();
	fDescriptorMat.copyTo(fDominantSet);
	//create average signal of the dominant set signals
	calculateDominantSignal();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Calculates the distance between two dominant sets. </summary>
///
/// <param name="other">	[in,out] The other. </param>
///
/// <returns>	The calculated distance. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

double PointDescriptor::calculateDistance(PointDescriptor& other, bool normalize, int vectorLength){
	cv::Mat firstMat = fDominantSet;
	cv::Mat secondMat = other.getDominantSet();
	if (normalize){
		firstMat = PointDescriptor::normalizeVectors(firstMat);
		secondMat = PointDescriptor::normalizeVectors(secondMat);
	}
	if (vectorLength>0){
		firstMat= PointDescriptor::cutMatrix(firstMat,vectorLength);
		secondMat = PointDescriptor::cutMatrix(secondMat,vectorLength);
	}

	fComparedDominantSet = firstMat;
	other.setComparedDominantSet(secondMat);

	return calculateEuclideanDistance(firstMat, secondMat);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	per row matrix normalization. </summary>
///
/// <remarks>	Majeed Kassis, 8/6/2013. </remarks>
///
/// <param name="vectorsMat">	The vectors matrix. </param>
///
/// <returns>	. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

cv::Mat PointDescriptor::normalizeVectors(cv::Mat vectorsMat){
	cv::Mat result;
	for (int i=0;i<vectorsMat.rows;i++){
		cv::Mat resultRow;
		cv::normalize(vectorsMat.row(i),resultRow,1,0,cv::NORM_MINMAX,vectorsMat.type());
		result.push_back(resultRow);
	}
	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Cut matrix vectors depending on given vector size. </summary>
///
/// <remarks>	Majeed Kassis, 8/6/2013. </remarks>
///
/// <param name="vectorsMat">	The vectors matrix. </param>
/// <param name="vectorSize">	Size of the vector. </param>
///
/// <returns>	. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

cv::Mat PointDescriptor::cutMatrix(cv::Mat vectorsMat, int vectorSize){
	cv::Mat result(vectorsMat.rows,vectorSize,vectorsMat.type());
	for (int i=0;i<vectorsMat.rows;i++)
		for (int j=0;j<vectorSize;j++)
			result.at<double>(i,j) = vectorsMat.at<double>(i,j);
	
	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Calculates the Euclidean distance between two matrices. </summary>
///
/// <remarks>	Majeed Kassis, 6/25/2013. </remarks>
///
/// <param name="firstMat"> 	The first matrix. </param>
/// <param name="secondMat">	The second matrix. </param>
///
/// <returns>	The calculated distance. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

double PointDescriptor::calculateEuclideanDistance(cv::Mat firstMat, cv::Mat secondMat){
	//cout << endl;
	double totalDist = 0;
	for ( int i = 0 ; i < firstMat.rows ; i++ ){
		double dist = 0 ;
		for (int j=0;j< firstMat.cols;j++){
			double d = firstMat.at<double>(i,j) - secondMat.at<double>(i,j) ;
			//cout << d*d << ", ";
			dist += d*d ;
		}
		//cout << endl << "col " << i << ": " << dist/fDominantSet.cols << endl;
		totalDist += dist/firstMat.cols;
	}
	//cout << "totalDist : " << totalDist/fDominantSet.rows << endl;
	return totalDist/firstMat.rows ;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Calculates the chi square distance between the two histograms. </summary>
///
/// <remarks>	Majeed Kassis, 7/31/2013. </remarks>
///
/// <param name="firstMat"> 	The first matrix. </param>
/// <param name="secondMat">	The second matrix. </param>
///
/// <returns>	The calculated chi square distance. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

double PointDescriptor::calculateChiSquaredDistance(cv::Mat firstMat, cv::Mat secondMat){
	cv::Mat A;
	cv::Mat B;
	firstMat.convertTo(A,CV_32FC1);
	secondMat.convertTo(B,CV_32FC1);
	return cv::compareHist(A,B, CV_COMP_CHISQR);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Calculates the Mahalanobis distance between two vector sets. </summary>
///
/// <remarks>	Majeed Kassis, 7/31/2013. </remarks>
///
/// <param name="A">	The cv::Mat to process. </param>
/// <param name="B">	The cv::Mat to process. </param>
///
/// <returns>	The calculated mahalanobis distance. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

double PointDescriptor::calculateMahalanobisDistance(cv::Mat A, cv::Mat B){
	cv::Mat meanA;
	cv::Mat meanB;
	cv::Mat covA;
	cv::Mat covB;
	int n = A.rows + B.rows;
	
	cv::calcCovarMatrix(A,covA,meanA,CV_COVAR_NORMAL+CV_COVAR_ROWS);
	cv::calcCovarMatrix(B,covB,meanB,CV_COVAR_NORMAL+CV_COVAR_ROWS);
	cv::Mat xDiff = meanA - meanB;
	// the covariance is normalized by the matrix row size - this makes OpenCV's covariance behave
	// exactly like Matlab's covariance.
	covA = covA / A.rows;
	covB = covB / B.rows;
	cv::Mat pC = (double)A.rows/n*covA+(double)B.rows/n*covB;
	cv::Mat invpC;
	cv::invert(pC, invpC, cv::DECOMP_SVD);
	cv::Mat resultMat = xDiff*invpC*xDiff.t();
	return cv::sqrt(resultMat.at<double>(0,0));
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Sets interpolation value. </summary>
///
/// <remarks>	Majeek, 11/20/2013. </remarks>
///
/// <param name="value">	The value. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

void PointDescriptor::setInterpolationValue(int value){
	fStartingWeight = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Calculates the dominant signal. </summary>
///
/// <remarks>	Majeek, 11/20/2013. </remarks>
///
/// <param name="interpolationValue">	The interpolation value. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

void PointDescriptor::calculateDominantSignal(double interpolationValue){
	fStartingWeight = interpolationValue;
	calculateDominantSignal();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Calculates the average signal of the descriptor, if there is a weight
/// 			for level 0 signal, we calculate the weights of the rest of the signals
/// 			and calculate, otherwise, the weights are even for all signals. </summary>
///
/// <remarks>	majeek, 11/7/2013. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

void PointDescriptor::calculateDominantSignal(){

	//if value is -1, then we calculate regular average, otherwise we interpolate
	cv::Mat total = cv::Mat::zeros(1, fDominantSet.cols,fDominantSet.type());
	if (fStartingWeight==-1){
		for (int i=0;i<fDominantSet.rows;i++)
			total += fDominantSet.row(i);
		fDominantSignal = total / fDominantSet.rows;
	}else{
		//apply weight of level max signal
		total += fDominantSet.row(fDominantSet.rows-1)*fStartingWeight;
		//apply rest of weight distributed evently among the rest of the signals
		double f = 1-fStartingWeight;
		double iteration = f;
		for (int i=fDominantSet.rows-2;i>0;i--){
			f = f * iteration;
			total += fDominantSet.row(i)*fStartingWeight*f;
		}	
		//last row
		total += fDominantSet.row(0)*fStartingWeight;
		fDominantSignal = total;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	returns the average signal of the descriptor. </summary>
///
/// <remarks>	Majeek, 11/7/2013. </remarks>
///
/// <returns>	The dominant signal interpolated. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

cv::Mat PointDescriptor::getDominantSignal(){
	return fDominantSignal;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Please ignore this function for now </summary>
///
/// <remarks>	Majeed Kassis, 6/26/2013. </remarks>
///
/// <param name="magI">	The mag i. </param>
///
/// <returns>	. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

cv::Mat PointDescriptor::switchQuadrantsWithoutMiddleRowAndColumnIfOdd(cv::Mat magI){
	int cx = magI.cols/2;
	int cy = magI.rows/2;
	
	// four different quadrants
	// if the row size or col size is not even
	// middle row/col stays at its place
	// and move the quadrants without it
	// is that ok fft wise?!
	cv::Mat q0(magI, cv::Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
	cv::Mat q1;
	if (magI.cols % 2 == 0)
		q1 = cv::Mat(magI, cv::Rect(cx, 0, cx, cy));  // Top-Right
	else
		q1 = cv::Mat(magI, cv::Rect(cx+1, 0, cx, cy));  // Top-Right
	cv::Mat q2;
	if (magI.rows % 2 == 0)
		q2 = cv::Mat(magI, cv::Rect(0, cy, cx, cy));  // Bottom-Left
	else
		q2 = cv::Mat(magI, cv::Rect(0, cy+1, cx, cy));  // Bottom-Left
	cv::Mat q3;
	if (magI.cols % 2 == 1 && magI.rows % 2 == 1)
		q3 = cv::Mat(magI, cv::Rect(cx+1, cy+1, cx, cy)); // Bottom-Right
	else if (magI.cols % 2 == 1)
		q3 = cv::Mat(magI, cv::Rect(cx+1, cy, cx, cy)); // Bottom-Right
	else if  (magI.rows % 2 == 1)
		q3 = cv::Mat(magI, cv::Rect(cx, cy+1, cx, cy)); // Bottom-Right
	else
		q3 = cv::Mat(magI, cv::Rect(cx, cy, cx, cy)); // Bottom-Right

	//swap to prepare for viewing. Make the origin of the image to be at (0,0)

	// swap quadrants (Top-Left with Bottom-Right)
    cv::Mat tmp;                           
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
	// swap quadrant (Top-Right with Bottom-Left)
    q1.copyTo(tmp);                    
    q2.copyTo(q1);
    tmp.copyTo(q2);
	return magI;
}