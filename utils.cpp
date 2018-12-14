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

#include <iostream>
#include <fstream>
#include <time.h>
#include <opencv.hpp>
#include "utils.h"

using namespace std;

//-------------------------------------------------------

static const int NCOLOURS = 8;
static const cv::Scalar COLOURS [] = { cv::Scalar(255,0,0), cv::Scalar(0,255,0),
	cv::Scalar(0,0,255), cv::Scalar(255,255,0),
	cv::Scalar(0,255,255), cv::Scalar(255,0,255),
	cv::Scalar(255,255,255), cv::Scalar(0,0,0)};

//-------------------------------------------------------

//! Display error message and terminate program
void error(const char *msg) 
{
	cout << "\nError: " << msg;
	getchar();
	exit(0);
}

//-------------------------------------------------------

//! Show the provided image and wait for keypress
void showImage(const cv::Mat& img)
{
	cv::namedWindow("Surf");
	cv::imshow("Surf", img);
	cv::waitKey(0);
}

//-------------------------------------------------------

//! Show the provided image in titled window and wait for keypress
void showImage(std::string& title,const cv::Mat& img)
{
	cv::namedWindow(title);
	cv::imshow(title, img);
	cv::waitKey(0);
}

//-------------------------------------------------------

// Convert image to single channel 32F
void getGray(const cv::Mat& img, cv::Mat& gray)
{
// 	cv::imshow("test", img);
// 	cv::waitKey(0);
	if (img.empty())
	{
		error("Unable to create gray scale image.  No image supplied");
	}

	cv::Mat gray8;
	gray.create(img.size(), CV_32FC1);

	

	if (img.channels() == 1)
		gray8 = img.clone();
	else 
	{
		cv::cvtColor(img, gray8, cv::COLOR_BGR2GRAY);
	}
	gray8.convertTo(gray, CV_32FC1, 1.0 / 255.0);
	
	
}

//-------------------------------------------------------

//! Draw all the Ipoints in the provided vector
void drawIpoints(cv::Mat& img, vector<Ipoint> &ipts, int tailSize)
{
	Ipoint *ipt;
	float s, o;
	int r1, c1, r2, c2, lap;

	for(unsigned int i = 0; i < ipts.size(); i++) 
	{
		ipt = &ipts.at(i);
		s = (2.5f * ipt->scale);
		o = ipt->orientation;
		lap = ipt->laplacian;
		r1 = fRound(ipt->y);
		c1 = fRound(ipt->x);
		c2 = fRound(s * cos(o)) + c1;
		r2 = fRound(s * sin(o)) + r1;

		if (o) // Green line indicates orientation
			cv::line(img, cv::Point(c1, r1), cv::Point(c2, r2), cv::Scalar(0, 255, 0));
		else  // Green dot if using upright version
			cv::circle(img, cv::Point(c1,r1), 1, cv::Scalar(0, 255, 0),-1);

		if (lap == 1)
		{ // Blue circles indicate dark blobs on light backgrounds
			cv::circle(img, cv::Point(c1,r1), fRound(s), cv::Scalar(255, 0, 0),1);
		}
		else if (lap == 0)
		{ // Red circles indicate light blobs on dark backgrounds
			cv::circle(img, cv::Point(c1,r1), fRound(s), cv::Scalar(0, 0, 255),1);
		}
		else if (lap == 9)
		{ // Red circles indicate light blobs on dark backgrounds
			cv::circle(img, cv::Point(c1,r1), fRound(s), cv::Scalar(0, 255, 0),1);
		}

		// Draw motion from ipoint dx and dy
		if (tailSize)
		{
			cv::line(img, cv::Point(c1,r1),
				cv::Point(int(c1+ipt->dx*tailSize), int(r1+ipt->dy*tailSize)),
				cv::Scalar(255,255,255), 1);
		}
	}
}

//-------------------------------------------------------

//! Draw a single feature on the image
void drawIpoint(cv::Mat& img, Ipoint &ipt, int tailSize)
{
	float s, o;
	int r1, c1, r2, c2, lap;

	s = (2.5f * ipt.scale);
	o = ipt.orientation;
	lap = ipt.laplacian;
	r1 = fRound(ipt.y);
	c1 = fRound(ipt.x);

	// Green line indicates orientation
	if (o) // Green line indicates orientation
	{
		c2 = fRound(s * cos(o)) + c1;
		r2 = fRound(s * sin(o)) + r1;
		cv::line(img, cv::Point(c1, r1), cv::Point(c2, r2), cv::Scalar(0, 255, 0));
	}
	else  // Green dot if using upright version
		cv::circle(img, cv::Point(c1,r1), 1, cv::Scalar(0, 255, 0),-1);

	if (lap >= 0)
	{ // Blue circles indicate light blobs on dark backgrounds
		cv::circle(img, cv::Point(c1,r1), fRound(s), cv::Scalar(255, 0, 0),1);
	}
	else
	{ // Red circles indicate light blobs on dark backgrounds
		cv::circle(img, cv::Point(c1,r1), fRound(s), cv::Scalar(0, 0, 255),1);
	}

	// Draw motion from ipoint dx and dy
	if (tailSize)
	{
		cv::line(img, cv::Point(c1,r1),
			cv::Point(int(c1+ipt.dx*tailSize), int(r1+ipt.dy*tailSize)),
			cv::Scalar(255,255,255), 1);
	}
}

//-------------------------------------------------------

//! Draw a single feature on the image
void drawPoint(cv::Mat& img, Ipoint &ipt)
{
	float s, o;
	int r1, c1;

	s = 3;
	o = ipt.orientation;
	r1 = fRound(ipt.y);
	c1 = fRound(ipt.x);

	cv::circle(img, cv::Point(c1,r1), fRound(s), COLOURS[ipt.clusterIndex%NCOLOURS], -1);
	cv::circle(img, cv::Point(c1,r1), fRound(s+1), COLOURS[(ipt.clusterIndex+1)%NCOLOURS], 2);

}

//-------------------------------------------------------

//! Draw a single feature on the image
void drawPoints(cv::Mat& img, vector<Ipoint> &ipts)
{
	float s, o;
	int r1, c1;

	for(unsigned int i = 0; i < ipts.size(); i++) 
	{
		s = 3;
		o = ipts[i].orientation;
		r1 = fRound(ipts[i].y);
		c1 = fRound(ipts[i].x);

		cv::circle(img, cv::Point(c1,r1), fRound(s), COLOURS[ipts[i].clusterIndex%NCOLOURS], -1);
		cv::circle(img, cv::Point(c1,r1), fRound(s+1), COLOURS[(ipts[i].clusterIndex+1)%NCOLOURS], 2);
	}
}

//-------------------------------------------------------

//! Draw descriptor windows around Ipoints in the provided vector
void drawWindows(cv::Mat& img, vector<Ipoint> &ipts)
{
	Ipoint *ipt;
	float s, o, cd, sd;
	int x, y;
	cv::Point2f src[4];

	for(unsigned int i = 0; i < ipts.size(); i++) 
	{
		ipt = &ipts.at(i);
		s = (10 * ipt->scale);
		o = ipt->orientation;
		y = fRound(ipt->y);
		x = fRound(ipt->x);
		cd = cos(o);
		sd = sin(o);

		src[0].x=sd*s+cd*s+x;   src[0].y=-cd*s+sd*s+y;
		src[1].x=sd*s+cd*-s+x;  src[1].y=-cd*s+sd*-s+y;
		src[2].x=sd*-s+cd*-s+x; src[2].y=-cd*-s+sd*-s+y;
		src[3].x=sd*-s+cd*s+x;  src[3].y=-cd*-s+sd*s+y;

		if (o) // Draw orientation line
			cv::line(img, cv::Point(x, y), 
				cv::Point(fRound(s*cd + x), fRound(s*sd + y)), cv::Scalar(0, 255, 0),1);
		else  // Green dot if using upright version
			cv::circle(img, cv::Point(x,y), 1, cv::Scalar(0, 255, 0),-1);


		// Draw box window around the point
		cv::line(img, cv::Point(fRound(src[0].x), fRound(src[0].y)), 
			cv::Point(fRound(src[1].x), fRound(src[1].y)), cv::Scalar(255, 0, 0),2);
		cv::line(img, cv::Point(fRound(src[1].x), fRound(src[1].y)),
			cv::Point(fRound(src[2].x), fRound(src[2].y)), cv::Scalar(255, 0, 0),2);
		cv::line(img, cv::Point(fRound(src[2].x), fRound(src[2].y)),
			cv::Point(fRound(src[3].x), fRound(src[3].y)), cv::Scalar(255, 0, 0),2);
		cv::line(img, cv::Point(fRound(src[3].x), fRound(src[3].y)),
			cv::Point(fRound(src[0].x), fRound(src[0].y)), cv::Scalar(255, 0, 0),2);

	}
}

//-------------------------------------------------------

// Draw the FPS figure on the image (requires at least 2 calls)
void drawFPS(cv::Mat& img)
{
	static int counter = 0;
	static clock_t t;
	static float fps;
	char fps_text[20];
	
	// Add fps figure (every 10 frames)
	if (counter > 10)
	{
		fps = (10.0f/(clock()-t) * CLOCKS_PER_SEC);
		t=clock(); 
		counter = 0;
	}

	// Increment counter
	++counter;

	// Get the figure as a string
	sprintf(fps_text,"FPS: %.2f",fps);

	// Draw the string on the image
	cv::putText (img,fps_text,cv::Point(10,25), cv::FONT_HERSHEY_PLAIN, 1.0, cv::Scalar(255,255,0));
}

//-------------------------------------------------------

//! Save the SURF features to file
void saveSurf(char *filename, vector<Ipoint> &ipts)
{
	ofstream outfile(filename);

	// output descriptor length
	outfile << "64\n";
	outfile << ipts.size() << "\n";

	// create output line as:  scale  x  y  des
	for(unsigned int i=0; i < ipts.size(); i++) 
	{
		outfile << ipts.at(i).scale << "  ";
		outfile << ipts.at(i).x << " ";
		outfile << ipts.at(i).y << " ";
		outfile << ipts.at(i).orientation << " ";
		outfile << ipts.at(i).laplacian << " ";
		outfile << ipts.at(i).scale << " ";
		for(int j=0; j<64; j++)
			outfile << ipts.at(i).descriptor[j] << " ";

		outfile << "\n";
	}

	outfile.close();
}

//-------------------------------------------------------

//! Load the SURF features from file
void loadSurf(char *filename, vector<Ipoint> &ipts)
{
	int descriptorLength, count;
	ifstream infile(filename);

	// clear the ipts vector first
	ipts.clear();

	// read descriptor length/number of ipoints
	infile >> descriptorLength;
	infile >> count;

	// for each ipoint
	for (int i = 0; i < count; i++) 
	{
		Ipoint ipt;

		// read vals
		infile >> ipt.scale; 
		infile >> ipt.x;
		infile >> ipt.y;
		infile >> ipt.orientation;
		infile >> ipt.laplacian;
		infile >> ipt.scale;

		// read descriptor components
		for (int j = 0; j < 64; j++)
			infile >> ipt.descriptor[j];

		ipts.push_back(ipt);

	}
}

//-------------------------------------------------------

//-------------------------------------------------------
