#include "PointRing.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Return true if the coordinate (row, col) inside the ring. </summary>
///
/// <param name="row">   	The row coordinate of the pixel. </param>
/// <param name="col">   	The col coordinate of the pixel. </param>
/// <param name="radius">	The radius of the ring. </param>
///
/// <returns>	true if inside, false if not. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

bool PointRing::isInside(int row, int col, int radius){
	return ((row - radius >= 0 ) &&  (row + radius < _mat.rows ) && 
		    (col - radius >= 0 ) &&  (col + radius < _mat.cols )); 
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Add all the points on the ring to the vector _ring. as octagon </summary>
///
/// <param name="point">	The row coordinate of seed pixel. </param>
/// <param name="r">		The radius of the ring. </param>
///
/// ### <param name="col">	The col coordinate of seed pixel. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

void PointRing::createRingCoordinates(cv::Point2i point, int r){
	int row = point.x ;
	int col = point.y ;


	if (r==2){
		//left side
		_ring.push_back(cv::Point2i(row+1, col-r));  
		_ring.push_back(cv::Point2i(row, col-r));
		_ring.push_back(cv::Point2i(row-1, col-r));
		
		//upper side
		_ring.push_back(cv::Point2i(row-r, col-1));
		_ring.push_back(cv::Point2i(row-r, col));
		_ring.push_back(cv::Point2i(row-r, col+1));

		//right side
		_ring.push_back(cv::Point2i(row-1, col+r));
		_ring.push_back(cv::Point2i(row, col+r));
		_ring.push_back(cv::Point2i(row+1, col+r));

		//bottom side
		_ring.push_back(cv::Point2i(row+r, col+1));
		_ring.push_back(cv::Point2i(row+r, col));
		_ring.push_back(cv::Point2i(row+r, col-1));
	}else{
		//left side
		_ring.push_back(cv::Point2i(row+1, col-r));
		_ring.push_back(cv::Point2i(row, col-r));
		_ring.push_back(cv::Point2i(row-1, col-r));

		//left-to-up diagonal 
		_ring.push_back(cv::Point2i(row-2, col-2));

		//upper side
		_ring.push_back(cv::Point2i(row-r, col-1));
		_ring.push_back(cv::Point2i(row-r, col));
		_ring.push_back(cv::Point2i(row-r, col+1));

		//up-to-right diagonal
		_ring.push_back(cv::Point2i(row-2, col+2));

		//right side
		_ring.push_back(cv::Point2i(row-1, col+r));
		_ring.push_back(cv::Point2i(row, col+r));
		_ring.push_back(cv::Point2i(row+1, col+r));

		//right-to-bottom diagonal
		_ring.push_back(cv::Point2i(row+2, col+2));

		//bottom side
		_ring.push_back(cv::Point2i(row+r, col+1));
		_ring.push_back(cv::Point2i(row+r, col));
		_ring.push_back(cv::Point2i(row+r, col-1));

		//bottom-to-left diagonal
		_ring.push_back(cv::Point2i(row+2, col-2));
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Creates ring coordinates square. </summary>
///
/// <remarks>	Majeek, 11/20/2013. </remarks>
///
/// <param name="point">	The point. </param>
/// <param name="r">		The int to process. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

void PointRing::createRingCoordinatesSquare(cv::Point2i point, int r){
	int row = point.y ;
	int col = point.x ;
	int min_col = col - r ;
	int max_col = col + r ;
	int min_row = row - r ;
	int max_row = row + r ;
	int c ;

	_ring.clear() ;
	if ( isInside(row, col, r) ){
		for( r = row ; r > min_row ; r--)
			_ring.push_back(cv::Point2i(min_col, r));
	
		for( c = min_col; c < max_col; c++)
			_ring.push_back(cv::Point2i(c, min_row));
		
		for( r = min_row ; r < max_row; r++)
			_ring.push_back(cv::Point2i(max_col, r));
		
		for( c = max_col; c > min_col ; c--)
			_ring.push_back(cv::Point2i(c, max_row));
	
		for( r = max_row; r > row; r--)
			_ring.push_back(cv::Point2i(min_col, r));
	}
}