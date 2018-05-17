#include "RadialDescriptor.hpp"


////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Gets shift value. </summary>
///
/// <remarks>	majeek, 3/15/2014. </remarks>
///
/// <param name="vectorsCount">	Number of vectors. </param>
/// <param name="radius">	   	The radius. </param>
///
/// <returns>	The shift value. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

int RadialDescriptor::getShiftValue(int vectorsCount, int radius){
	// find signals
	int shift = 2;
	for (int i = 0; i<vectorsCount; i++)
		shift = shift * 2;
	shift = shift + radius;

	return shift;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Prepare image, convert to grayscale, pad it and apply gaussian blur. Then generates
///				Gaussian pyramid images (depending on the number of levels given) to improve
///				application performance.
///	</summary>
///
/// <remarks>	majeek, 3/15/2014. </remarks>
///
/// <param name="imageToPrepare">	The image to prepare. </param>
/// <param name="vectorsCount">  	Number of vectors. </param>
/// <param name="kernelSize">	 	Size of the kernel. </param>
/// <param name="gaussBlur">	 	The gauss blur. </param>
/// <param name="radius">		 	The radius. </param>
///
/// <returns>	A cv::Mat. The prepared image. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

cv::vector<cv::Mat> RadialDescriptor::prepareImage(cv::Mat imageToPrepare, int vectorsCount, int kernelSize, double gaussBlur, int radius){
	cv::vector<cv::Mat> result;
	cv::Mat image;

	//convert to grayscale
	cv::cvtColor(imageToPrepare, image, CV_RGB2GRAY);

	int shift = getShiftValue(vectorsCount, radius);

	//padd it with  lines everywhere //background white
	cv::Mat padded = cv::Mat::ones(image.rows + 2 * shift, image.cols + 2 * shift, image.type()) * 255;
	image.copyTo(padded(cv::Rect(shift, shift, image.cols, image.rows)));


	if (gaussBlur != 0.0)
		cv::GaussianBlur(padded, padded, cv::Size(kernelSize, kernelSize), gaussBlur);
	result.push_back(padded);
	cv::Mat pyredImage;
	cv::pyrUp(padded, pyredImage);
	result.push_back(pyredImage.clone());
	for (int i = 1; i < vectorsCount; i++){
		cv::pyrDown(pyredImage, pyredImage);
		result.push_back(pyredImage.clone());
	}

	return result;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Creates a feature of a given pixel co-ordinates for an image. </summary>
///
/// <remarks>	majeek, 3/14/2014. </remarks>
///
/// <param name="i">				 	Zero-based index of the. </param>
/// <param name="j">				 	The int to process. </param>
/// <param name="padded">			 	The padded. </param>
/// <param name="interpolationValue">	The interpolation value. </param>
/// <param name="radius">			 	The radius. </param>
/// <param name="vectorsCount">		 	Number of vectors. </param>
/// <param name="minVariance">		 	The minimum variance. </param>
///
/// <returns>	null if it doesn't pass the average variance threshold, else the new feature. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

SignalDescriptor* RadialDescriptor::createFeature(int i, int j, cv::vector<cv::Mat> padded, double interpolationValue, int radius, int vectorsCount, int minVariance){

	PointDescriptor* pd = new PointDescriptor(padded[0], cv::Point2i(j, i), interpolationValue, vectorsCount, padded);
	pd->createRingSignatureSet(radius);
	pd->calculateDescriptorMatrix();
	//if the option is checked and point descriptor passes variance threshold, it is added.
	SignalDescriptor *sd = nullptr;
	if (pd->getAverageVariance() >= minVariance){
		pd->findDominantSet();
		pd->calculateDominantSignal();
		sd = new SignalDescriptor(pd->getCenterPoint().y, pd->getCenterPoint().x,
			radius, pd->getAverageVariance(), pd->getDominantSignal(), cv::Mat());
		sd->setFarthestRing(pd->getFarthestRing());
	}
	delete pd;
	return sd;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Sort features by variance. </summary>
///
/// <remarks>	majeek, 3/14/2014. </remarks>
///
/// <param name="imageSignalsDescriptors">	[in,out] If non-null, the image signals descriptors. </param>
///
/// <returns>	returns the sorted features by variance. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

cv::vector<SignalDescriptor*>* RadialDescriptor::sortFeaturesByVariance(cv::vector<SignalDescriptor*> *imageSignalsDescriptors){

	cv::vector<SignalDescriptor*> *sortedSignalDescriptors = new cv::vector<SignalDescriptor*>();
	cv::vector<SignalDescriptor*> &descriptorListForImage = *imageSignalsDescriptors;
	int size = (int)imageSignalsDescriptors->size();

	for (int i = 0; i<size; i++){
		int maxIndex = -1;
		double maxVariance = 0;
		for (int j = 0; j<imageSignalsDescriptors->size(); j++){
			if (descriptorListForImage[j]->getVariance()>maxVariance){
				maxIndex = j;
				maxVariance = descriptorListForImage[j]->getVariance();
			}
		}
		sortedSignalDescriptors->push_back(descriptorListForImage[maxIndex]);
		imageSignalsDescriptors->erase(imageSignalsDescriptors->begin() + maxIndex);
	}

	return sortedSignalDescriptors;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Gets top k features. </summary>
///
/// <remarks>	majeek, 3/14/2014. </remarks>
///
/// <param name="minDistance">			  	The minimum distance between two feature co-ordinates. </param>
/// <param name="sortedSignalDescriptors">	[in,out] [in,out] If non-null, the sorted signal
/// 										descriptors. </param>
/// <param name="maxSignals">			  	The number of required signals. </param>
///
/// <returns>	Top K features, where two features are of distance minDistance at least, if possible. Otherwise, just top K features. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

cv::vector<SignalDescriptor*>* RadialDescriptor::getTopKFeatures(int minDistance, cv::vector<SignalDescriptor*> &sortedSignalDescriptors, int maxSignals){

	cv::vector<SignalDescriptor*> *topImageSignalsDescriptors = new cv::vector<SignalDescriptor*>();
	if (minDistance != -1){
		//we split each descriptor with all its surroundings and put them into 1 group
		cv::vector<cv::vector<SignalDescriptor*>> signalGroups;

		while (sortedSignalDescriptors.size()>0){
			//initialize new goup
			cv::vector<SignalDescriptor*> oneSignalGroup;
			SignalDescriptor* frontElement = sortedSignalDescriptors.at(0);
			oneSignalGroup.push_back(frontElement);
			sortedSignalDescriptors.erase(sortedSignalDescriptors.begin());
			//find indexes of new signals around our signal descriptor
			cv::vector<int> indexesToGroup; //indexes added here

			int row = frontElement->getRow();
			int col = frontElement->getCol();
			for (int i = 0; i<sortedSignalDescriptors.size(); i++){
				int diffRow = std::abs(sortedSignalDescriptors.at(i)->getRow() - row);
				int diffCol = std::abs(sortedSignalDescriptors.at(i)->getCol() - col);
				if ((diffRow <= minDistance) && (diffCol <= minDistance)) //if in range
					indexesToGroup.push_back(i);
			}
			//add new signals to same group and remove from older one
			for (int i = 0; i<indexesToGroup.size(); i++)
				oneSignalGroup.push_back(sortedSignalDescriptors[indexesToGroup.at(i)]);
			//remove signals from sortedSignalDescriptors
			for (int i = (int)indexesToGroup.size() - 1; i >= 0; i--)
				sortedSignalDescriptors.erase(sortedSignalDescriptors.begin() + indexesToGroup.at(i));
			//add group to signalGroups
			signalGroups.push_back(oneSignalGroup);
		}

		//we take 1 element from each group hoping to reach number of points required. if not we do another wave

		int i = 0;
		int numOfGroups = signalGroups.size();
		int elementsToAdd = maxSignals;
		bool success = false;
		while (elementsToAdd != 0){
			if (!signalGroups[i].empty()){
				success = true;
				topImageSignalsDescriptors->push_back(signalGroups[i][0]);
				signalGroups[i].erase(signalGroups[i].begin());
				elementsToAdd--;
			}
			i++;
			//if we scanned all possible groups and all are empty then we stop, otherwise we reset.
			if (i == numOfGroups){
				if (!success)
					elementsToAdd = 0;
				else{
					i = 0;
					success = false;
				}
			}
		}
		//free signalGroups
		for (cv::vector<SignalDescriptor*> sdVec : signalGroups)
			for (SignalDescriptor* sd : sdVec)
				delete sd;

	}
	else { //we just take first K elements.
		for (int i = 0; i<maxSignals; i++){
			topImageSignalsDescriptors->push_back(sortedSignalDescriptors[i]);
			sortedSignalDescriptors[i] = nullptr;
		}
	}

	return topImageSignalsDescriptors;
}
