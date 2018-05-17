#include "Sampler.h"


void Sampler::bilinear(cv::vector<double>& src, cv::vector<double>& dst, const int size, const double rate){

	double sample = 0;
	int srcSize = src.size();
	for (int i = 0 ; i < size ; i++ ){
		int idx = (int)sample;
		double t = sample - idx ; 
		if ( idx < srcSize - 1)
			dst.push_back(src[idx]*(1-t) + src[idx+1]*t);
		else
			dst.push_back(src[srcSize - 2]*(1-t) + src[srcSize - 1]*t);
		sample += rate ;
	}
	
}

void Sampler::quardic(cv::vector<double>& src, cv::vector<double>& dst, const int size, const double rate){

	double sample = 0 ;
	int dstSize = dst.size();
	int srcSize = src.size();
	for (int i = 0 ; i < dstSize ; i++ ){
		double f0;
		int  idx = (int)sample ;
		double t = sample - idx ;
		if ( t < 0.5 ) {
			t += 1.0 ;
			f0 = (idx == 0 ) ? src[0]: src[idx-1] ;
		}
		else 
			f0 = src[idx]; 

		double a =  (src[idx+1] - f0)/2.0 - (src[idx+2]-f0);
		double b =  (src[idx+1] - f0) - a ;
		dst.push_back((a*t + b)*t) ;
		sample += rate ;
	}
}

void Sampler::cubic(cv::vector<double>& src, cv::vector<double>& dst, const int size, const double rate){

	int srcSize = src.size();

	double sample = 0 ;
	for (int i = 0 ; i < size ; i++ ){
		int  idx = (int)sample ;
		double t = 1.0 + (sample - idx) ;
		double f0 = (idx == 0 ) ? src[0]: src[idx-1] ;

		double f1 = src[idx] - f0 ;
		double f2 = src[idx+1] - f0 ;
		double f3 = src[idx+2] - f0 ;
		double a = (f3 - 3.0*f2 - 3.0*f1)/6.0  ;
		double b = (6.0 * a - 2.0*f1 - f2)/2.0 ;
		double c = f1 - a - b ;

		dst.push_back(((a*t + b)*t + c)*t);
	}
}

void Sampler::sample(cv::vector<double>& src, cv::vector<double>& dst, const int size, const int method){

	double rate = src.size() / (double)size;
	switch(method) {
	case BILINEAR:
		bilinear(src, dst, size, rate);
		break ;

	case QUADRIC:
		quardic(src, dst, size, rate);
		break ;

	case CUBIC:
		cubic(src, dst, size, rate) ;
		break ;

	default:
		bilinear(src, dst, size, rate);
	}
}

void Sampler::observeWeights(const cv::Mat weights){
		cv::MatConstIterator_<double> first = weights.begin<double>();
		cv::MatConstIterator_<double> last = weights.end<double>();
		double s = 0.0;
		while ( first != last ) { // loop over k
				s += *first; // *first means \pi_{k}
				++first;
		}
		//assert(std::abs(s - 1.0) < Epsilon);
}


void Sampler::observeLabelsAndMeans(const cv::Mat labels, const cv::Mat means, int height, int width){
		int dimension = 1;
		cv::Mat rgb_image(height, width, CV_8UC1);
		cv::MatIterator_<unsigned char> rgb_first = rgb_image.begin<unsigned char>();
		cv::MatIterator_<unsigned char> rgb_last = rgb_image.end<unsigned char>();
		cv::MatConstIterator_<int> label_first = labels.begin<int>();
		 
		cv::Mat means_u8;
		means.convertTo(means_u8, CV_8UC1, 255.0);
		cv::Mat means_u8c3 = means_u8.reshape(dimension);
		 
		while ( rgb_first != rgb_last ) {
				const unsigned char rgb = means_u8c3.ptr<unsigned char>(*label_first)[0];
				*rgb_first = rgb;
				++rgb_first;
				++label_first;
		}
		//cv::imshow("tmp", rgb_image);
		//cv::imwrite("/Users/kumada/Data/graph-cut/output/em_result.jpg", rgb_image);
		//cv::waitKey();
}


void Sampler::observeEMProbs(const cv::Mat probs){
		cv::vector<double> t(probs.cols, 0.0);
		for ( int n = 0; n < probs.rows; ++n ) {
				const double* gamma_n = probs.ptr<double>(n);
				double s = 0.0;
				for ( int k = 0; k < probs.cols; ++k ) {
						s += gamma_n[k]; // \gamma_{n,k}
						t[k] += gamma_n[k];
				}
			//	assert(std::abs(s - 1.0) < Epsilon);
		}
		double total = std::accumulate(t.begin(), t.end(), 0.0);
		//assert(std::abs(total - probs.rows) < Epsilon);
}

