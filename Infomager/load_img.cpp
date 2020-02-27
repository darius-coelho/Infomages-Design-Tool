#include <cstdio>
#include <opencv2\opencv.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

cv::Mat load_img(char* fname)
{
	cv::Mat img;
	img = cv::imread(fname, CV_LOAD_IMAGE_UNCHANGED);

	if (!img.data)                              // Check for invalid input
	{
		std::cout << "Could not open or find the image" << std::endl;
	}	
	return img;
}