#ifndef IMGFILTER_H
#define IMGFILTER_H

#include <opencv2\opencv.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc.hpp>



class imgFilter 
{
public:
	void setOrigBackdrop(cv::Mat img);
	cv::Mat none();
	cv::Mat grayscale();
	
	cv::Mat erode();
	cv::Mat fliph();
	cv::Mat flipv();
	cv::Mat fliphv();
	cv::Mat styliz();
	cv::Mat smooth();

protected:
	
private:
	//Inputs
	cv::Mat origBckdrp;
	cv::Mat currBckdrp;
	
};

#endif // IMGFILTER_H