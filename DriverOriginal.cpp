#include "RadialDescriptor.hpp"
#include <iostream>
#include <fstream>


void calculateSignals(cv::Mat image, int vectorsCount, int kernelSize, double gaussBlur, int radius, int minVariance, double interpolationValue, int minDistance, int maxSignals, std::string outFile){

	//convert image to grayscale and apply gaussian blur
	cv::vector<cv::Mat> padded = RadialDescriptor::prepareImage(image, vectorsCount, kernelSize, gaussBlur, radius);

	int shift = RadialDescriptor::getShiftValue(vectorsCount, radius);

	cv::vector<SignalDescriptor*> *imageSignalsDescriptors = new cv::vector<SignalDescriptor*>();

	//scan image pixels and create descriptors
	int currentPercentage = 0;
	for (int i = shift; i < padded[0].rows - shift; i++){
		for (int j = shift; j < padded[0].cols - shift; j++){
			SignalDescriptor *sd = RadialDescriptor::createFeature(i, j, padded, interpolationValue, radius, vectorsCount, minVariance);
			if (sd != nullptr)
				imageSignalsDescriptors->push_back(sd);
		}
	}

	std::cout << "totalNumberOfSignals=" << imageSignalsDescriptors->size() << "\n";
	//sort descriptors by variance
	cv::vector<SignalDescriptor*>* sortedSignalDescriptorsPtr = RadialDescriptor::sortFeaturesByVariance(imageSignalsDescriptors);
	cv::vector<SignalDescriptor*>& sortedSignalDescriptors = *sortedSignalDescriptorsPtr;


	// if distance -1, we just take top K signals
	// otherwise we split the descriptors into groups - each group consists of a point and all 
	// its surrounding points of fMinDistnace -
	// and take highest variance from each group
	cv::vector<SignalDescriptor*> *topImageSignalsDescriptors = RadialDescriptor::getTopKFeatures(minDistance, sortedSignalDescriptors, maxSignals);



	//remove unused signals
	for (SignalDescriptor* sd : sortedSignalDescriptors)
		if (sd != nullptr)
			delete sd;

	//store result to file
	std::ofstream outStream;
	outStream.open(outFile);
	std::cout << "topSignals=" << topImageSignalsDescriptors->size() << "\n";
	for (SignalDescriptor* sd : *topImageSignalsDescriptors){
		outStream << sd->getRow() << " " << sd->getCol();// << " " << sd->getSignal() << "\n";
		cv::Mat signal = sd->getSignal();
		for (int i = 0; i < signal.cols; i++)
			outStream << " " << signal.at<double>(0, i);
		outStream << "\n";
	}
	outStream.close();

	//final cleanup
	for (SignalDescriptor* sd : *sortedSignalDescriptorsPtr){
		delete sd;
	}

	
	imageSignalsDescriptors->clear();
	delete imageSignalsDescriptors;
	sortedSignalDescriptorsPtr->clear();
	topImageSignalsDescriptors->clear();
	delete topImageSignalsDescriptors;
	delete sortedSignalDescriptorsPtr;
}

int mainOriginal(int argc, char* argv[]){
	// input: receive image name, number of features
	// open image into cvmat
	
	// scan image for high variance
	// create co-occurrence vector for each high variance point
	// output : store result in file:  x,y,vector  or just return at char*
	

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Constructor. </summary>
	///
	/// <remarks>	Majeek, 30/09/2014. </remarks>
	///
	/// Parameters:
	///		1. image matrix, rgb, untouched
	///		2. how many signals for each feature, 3 levels usually. the signals will be combined into one.  
	///		3. the kernel size of the gaussian blur for the feature, 3 for 3x3 and so on.  
	///		4. gaussBlur, the standard deviation value. 1 is fine.  
	///		5. the radius of the feature, the pixels around that radius will be taken for the feature. size 2-3 is fine.  
	///		6. minimum variance required so the feature is included int the final set, used only to speed up things.  
	///		7. the value for the weights, giving value of 0.5 will give middle signal 0.5 while the first and 3rd get 0.25 weight  
	///		8. number of features required, 200 for 30x30 image, the larger the image the more features are required.  
	///		9. the outfile path and file name, to store the results. each feature line format: X Y [feature, values]
	////////////////////////////////////////////////////////////////////////////////////////////////////
	//void calculateSignals(cv::Mat image, int vectorsCount, int kernelSize, double gaussBlur, int radius,
	//  int minVariance, double interpolationValue, int minDistance, int maxSignals, std::string outFile){
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	
	
	if (argc != 11){
		std::cout << "Wrong number of parameters entered.\n";
		std::cout << "Correct input:\n";
		std::cout << argv[0] << " std::string imageName, int vectorsCount[3], int kernelSize[3-5], double gaussBlur[0-1], int radius[2-3], int minVariance[0-30], double interpolationValue[0.5], int minDistance[1], int maxSignals[200], std::string outFile\n";
		std::string imageName = "D:/majeek/RadialDescriptor/data-all/00-wordpartsSet/a/00-13059004-15-a.png";
		cv::Mat img = cv::imread(imageName);
		std::string outFile = "C:/Users/majeek/Documents/RadialDescriptorConsole/x64/Release/outFile";
		calculateSignals(img, 3, 3, 1, 3, 2, 0.5, 2, 200, outFile);
	}
	else{
		//std::cout << argv[1] << "," << argv[2] << "," << argv[3] << "," << argv[4] << "," << argv[5] << "," << argv[6] << "," <<
		//	argv[7] << "," << argv[8] << "," << argv[9] << "," << argv[10] << std::endl;
		std::string imageName(argv[1]);
		cv::Mat img = cv::imread(imageName);
		std::stringstream ss;

		int vectorsCount;
		int kernelSize;
		double gaussBlur;
		int radius;
		int minVariance;
		double interpolationValue;
		int minDistance;
		int maxSignals;
		std::string outFile(argv[10]);
		ss << argv[2];
		ss >> vectorsCount;
		ss = std::stringstream();
		ss << argv[3];
		ss >> kernelSize;
		ss = std::stringstream();
		ss << argv[4];
		ss >> gaussBlur;
		ss = std::stringstream();
		ss << argv[5];
		ss >> radius;
		ss = std::stringstream();
		ss << argv[6];
		ss >> minVariance;
		ss = std::stringstream();
		ss << argv[7];
		ss >> interpolationValue;
		ss = std::stringstream();
		ss << argv[8];
		ss >> minDistance;
		ss = std::stringstream();
		ss << argv[9];
		ss >> maxSignals;


		//std::cout << imageName << "," << vectorsCount << "," << kernelSize << "," << gaussBlur << "," << radius << "," <<
		//	minVariance << "," << interpolationValue << "," << minDistance << "," << maxSignals << "," << outFile;
		calculateSignals(img, vectorsCount, kernelSize, gaussBlur, radius,
			minVariance, interpolationValue, minDistance, maxSignals, outFile);


		//to run from within application comment code above, and uncomment below. Details of each prameter is explained above.
		// 
		//std::string imageName = "D:/majeek/RadialDescriptor/data-all/00-wordpartsSet/a/00-13059004-15-a.png";
		//cv::Mat img = cv::imread(imageName);
		//calculateSignals(img, 3, 3, 1, 3, 2, 0.5, 2, 200, "outFile");

	}
}


