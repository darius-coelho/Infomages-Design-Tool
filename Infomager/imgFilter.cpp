#include "imgFilter.h"

void imgFilter::setOrigBackdrop(cv::Mat img)
{
	img.copyTo(origBckdrp);	
	img.copyTo(currBckdrp);
}

cv::Mat imgFilter::none()
{
	cv::Mat result_image;
	origBckdrp.copyTo(currBckdrp);
	origBckdrp.copyTo(result_image);
	return result_image;
}

cv::Mat imgFilter::grayscale()
{
	cv::Mat result_image;
	cvtColor(currBckdrp, result_image, CV_BGR2GRAY);
	cvtColor(result_image, result_image, CV_GRAY2BGR);
	result_image.copyTo(currBckdrp);
	return result_image;
}

cv::Mat imgFilter::erode()
{
	cv::Mat result_image;
	int dilation_size = 3;
	cv::Mat element = getStructuringElement(cv::MORPH_ELLIPSE,
		cv::Size(2 * dilation_size + 1, 2 * dilation_size + 1),
		cv::Point(dilation_size, dilation_size));
	/// Apply the dilation operation
	cv::erode(currBckdrp, result_image, element);
	result_image.copyTo(currBckdrp);
	return result_image;
}

cv::Mat imgFilter::styliz()
{
	cv::Mat result_image;
	cv::stylization(currBckdrp, result_image, 15, 0.7);
	result_image.copyTo(currBckdrp);
	return result_image;
}

cv::Mat imgFilter::smooth()
{
	cv::Mat result_image;
	cv::edgePreservingFilter(currBckdrp, result_image, 2);
	result_image.copyTo(currBckdrp);
	return result_image;
}

cv::Mat imgFilter::fliph()
{
	cv::Mat result_image;
	cv::flip(currBckdrp, result_image, 0);
	return result_image;
}

cv::Mat imgFilter::flipv()
{
	cv::Mat result_image;
	cv::flip(currBckdrp, result_image, 1);
	return result_image;
}

cv::Mat imgFilter::fliphv()
{
	cv::Mat result_image;
	cv::flip(currBckdrp, result_image, -1);
	return result_image;
}

