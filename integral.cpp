/*********************************************************** 
*  --- OpenSURF ---                                       *
*  This library is distributed under the GNU GPL. Please   *
*  use the contact form at http://www.chrisevansdev.com    *
*  for more information.                                   *
*                                                          *
*  C. Evans, Research Into Robust Visual Features,         *
*  MSc University of Bristol, 2008.                        *
*                                                          *
************************************************************/

#include "utils.h"

#include "integral.h"

//! Computes the integral image of image img.  Assumes source image to be a 
//! 32-bit floating point.  Returns IplImage of 32-bit float form.
cv::Mat Integral(cv::Mat& source, cv::Mat& dst)
{
	// convert the image to single channel 32f
	cv::Mat img;
	getGray(source, img);
	dst.create(img.size(), CV_32FC1);

	// set up variables for data access
	int height = img.rows;
	int width = img.cols;
	
	
	//std::cout << "W * H " << width << "\t" << height << "\n";
	//std::cout << img.depth() << "\t" << img.type() << img.elemSize()<<std::endl;
	//the first rows
	float rs = 0.0f;
	for(int j=0; j<width; j++) 
	{
		rs += img.at<float>(0, j);
		dst.at<float>(0, j) = rs;
	}

	// remaining cells are sum above and to the left
	for(int i=1; i<height; ++i) 
	{
		rs = 0.0f;
		for(int j=0; j<width; ++j) 
		{
			rs += img.at<float>(i, j);
			dst.at<float>(i, j) = rs + dst.at<float>(i-1, j);
		}
	}


	// return
	return dst;
}

