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

#ifndef UTILS_H
#define UTILS_H

#include <opencv.hpp>
#include "ipoint.h"

#include <vector>


//! Display error message and terminate program
void error(const char *msg);

//! Show the provided image and wait for keypress
void showImage(const cv::Mat& img);

//! Show the provided image in titled window and wait for keypress
void showImage(char *title,const cv::Mat& img);

// Convert image to single channel 32F
void getGray(const cv::Mat& img, cv::Mat& gray);

//! Draw a single feature on the image
void drawIpoint(cv::Mat& img, Ipoint &ipt, int tailSize = 0);

//! Draw all the Ipoints in the provided vector
void drawIpoints(cv::Mat& img, std::vector<Ipoint> &ipts, int tailSize = 0);

//! Draw descriptor windows around Ipoints in the provided vector
void drawWindows(cv::Mat& img, std::vector<Ipoint> &ipts);

// Draw the FPS figure on the image (requires at least 2 calls)
void drawFPS(cv::Mat& img);

//! Draw a Point at feature location
void drawPoint(cv::Mat& img, Ipoint &ipt);

//! Draw a Point at all features
void drawPoints(cv::Mat& img, std::vector<Ipoint> &ipts);

//! Save the SURF features to file
void saveSurf(char *filename, std::vector<Ipoint> &ipts);

//! Load the SURF features from file
void loadSurf(char *filename, std::vector<Ipoint> &ipts);

//! Round float to nearest integer
inline int fRound(float flt)
{
	return (int) floor(flt+0.5f);
}

#endif
