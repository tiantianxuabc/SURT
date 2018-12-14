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

#include "surflib.h"
#include "kmeans.h"
#include <ctime>
#include <iostream>

//-------------------------------------------------------
// In order to you use OpenSURF, the following illustrates
// some of the simple tasks you can do.  It takes only 1
// function call to extract described SURF features!
// Define PROCEDURE as:
//  - 1 and supply image path to run on static image
//  - 2 to capture from a webcam
//  - 3 to match find an object in an image (work in progress)
//  - 4 to display moving features (work in progress)
//  - 5 to show matches between static images
#define PROCEDURE 1

#include <opencv.hpp>

#ifdef _DEBUG
#pragma comment(lib, "opencv_world400d.lib")
#else
#pragma comment(lib, "opnecv_world400.lib")
#endif
//-------------------------------------------------------

int mainImage(void)
{
	// Declare Ipoints and other stuff
	IpVec ipts;
	cv::Mat img= cv::imread("..\\imgs\\19.jpg", cv::IMREAD_COLOR);

	// Detect and describe interest points in the image
	clock_t start = clock();
	surfDetDes(img, ipts, false, 5, 4, 2, 0.0004f); 
	clock_t end = clock();

	std::cout<< "OpenSURF found: " << ipts.size() << " interest points" << std::endl;
	std::cout<< "OpenSURF took: " << float(end - start) / CLOCKS_PER_SEC  << " seconds" << std::endl;

	// Draw the detected points
	drawIpoints(img, ipts);

	// Display the result
	showImage(img);

	return 0;
}

//-------------------------------------------------------




//-------------------------------------------------------


//-------------------------------------------------------


//-------------------------------------------------------

int mainStaticMatch()
{
	cv::Mat img1, img2;
	img1 = cv::imread("..\\imgs\\19.jpg");
	img2 = cv::imread("..\\imgs\\20.jpg");

	IpVec ipts1, ipts2;
	surfDetDes(img1,ipts1,false,4,4,2,0.0001f);
	surfDetDes(img2,ipts2,false,4,4,2,0.0001f);

	IpPairVec matches;
	getMatches(ipts1,ipts2,matches);

	for (unsigned int i = 0; i < matches.size(); ++i)
	{
		drawPoint(img1,matches[i].first);
		drawPoint(img2,matches[i].second);

		const int & w = img1.cols;
		cv::line(img1,cv::Point(matches[i].first.x,matches[i].first.y),cv::Point(matches[i].second.x+w,matches[i].second.y), cv::Scalar(255,255,255),1);
		cv::line(img2,cv::Point(matches[i].first.x-w,matches[i].first.y),cv::Point(matches[i].second.x,matches[i].second.y), cv::Scalar(255,255,255),1);
	}

	std::cout<< "Matches: " << matches.size();

	cv::namedWindow("1");
	cv::namedWindow("2");
	cv::imshow("1", img1);
	cv::imshow("2",img2);
	cv::waitKey(0);

	return 0;
}

//-------------------------------------------------------


//-------------------------------------------------------

int main(void) 
{
	if( PROCEDURE == 2)
		mainImage();
	else
		mainStaticMatch();
}
