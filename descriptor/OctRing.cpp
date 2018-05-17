#include "OctRing.h"
#include <iostream>

void OctRing::setRing(int row, int col, int r){
	//we round (sqrt(2)-1)*r -> that is why we add 0.5
	int qr = (int)((cv::sqrt(2.0) - 1)* r-1 + 0.5) ;
	

	cv::Point2i verts[8];
	verts[0] = cv::Point2i(col - r , row + qr);
	verts[1] = cv::Point2i(col - r , row - qr);
	verts[2] = cv::Point2i(col - qr, row - r);
	verts[3] = cv::Point2i(col + qr, row - r);
	verts[4] = cv::Point2i(col + r , row - qr);
	verts[5] = cv::Point2i(col + r , row + qr);
	verts[6] = cv::Point2i(col + qr, row + r);
	verts[7] = cv::Point2i(col - qr, row + r);

	int x, y ;
	_ring.clear() ;
	// drawing the octan 
	for ( x = verts[0].x, y = verts[0].y ; y > verts[1].y; y-- )            
		_ring.push_back(cv::Point2i(x, y));  

	for ( x = verts[1].x, y = verts[1].y ; x < verts[2].x; x++, y-- )
		_ring.push_back(cv::Point2i(x, y));

	for ( x = verts[2].x, y = verts[2].y ; x < verts[3].x; x++ )
		_ring.push_back(cv::Point2i(x, y));

	for ( x = verts[3].x, y = verts[3].y ; x < verts[4].x; x++, y++ )
		_ring.push_back(cv::Point2i(x, y));

	for ( x = verts[4].x, y = verts[4].y ; y < verts[5].y; y++ )
		_ring.push_back(cv::Point2i(x, y));

	for ( x = verts[5].x, y = verts[5].y ; y > verts[6].y; x--, y++ )
		_ring.push_back(cv::Point2i(x, y));

	for ( x = verts[6].x, y = verts[6].y ; y < verts[7].y; x-- )
		_ring.push_back(cv::Point2i(x, y));

	for ( x = verts[7].x, y = verts[7].y ; y < verts[0].y; x--, y-- )
		_ring.push_back(cv::Point2i(x, y));
}
