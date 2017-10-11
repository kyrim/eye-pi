#include "MotionFrame.hpp"

MotionFrame::MotionFrame(cv::UMat &frame, std::unique_ptr<cv::Rect> rectangle) : frame_(frame), rectangle_(std::move(rectangle))
{
}

cv::UMat MotionFrame::getFrame()
{
	return frame_;
}

std::unique_ptr<cv::Rect> MotionFrame::getRectangle()
{
	return move(rectangle_);
}