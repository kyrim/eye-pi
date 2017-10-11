#ifndef MOTIONFRAME_H
#define MOTIONFRAME_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

// TODO: Re-implement pointers to use smart pointers
class MotionFrame
{

  public:
	MotionFrame(cv::UMat &frame, std::unique_ptr<cv::Rect> rectangle);

	cv::UMat getFrame();
	std::unique_ptr<cv::Rect> getRectangle();

  private:
	cv::UMat &frame_;
	std::unique_ptr<cv::Rect> rectangle_;
};

#endif