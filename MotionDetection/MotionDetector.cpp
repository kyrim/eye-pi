#include "MotionDetector.hpp"
#include <string>

MotionDetector::MotionDetector(CameraStreamer &camera_streamer)
	: streamer_(camera_streamer), has_average_frame_(false) {}

std::unique_ptr<MotionFrame> MotionDetector::getNextFrame()
{
	if (!streamer_.tryGetNextFrame(current_frame_))
		return std::unique_ptr<MotionFrame>();

	cv::cvtColor(current_frame_, gray_current_frame_, cv::COLOR_BGR2GRAY);
	cv::blur(gray_current_frame_, gray_current_frame_, cv::Size(18, 18));

	if (!has_average_frame_)
	{
		average_frame_ = gray_current_frame_;
		average_frame_.convertTo(average_frame_, CV_32F);

		has_average_frame_ = true;
		return std::unique_ptr<MotionFrame>();
	}

	accumulateWeighted(gray_current_frame_, average_frame_, 0.5);

	cv::Mat scaled_abs, frame_delta, thresh;
	std::vector<std::vector<cv::Point>> contours;

	convertScaleAbs(average_frame_, scaled_abs);
	absdiff(gray_current_frame_, scaled_abs, frame_delta);
	threshold(frame_delta, thresh, 5, 255, cv::THRESH_BINARY);
	dilate(thresh, thresh, cv::Mat(), cv::Point(-1, -1), 2);

	findContours(thresh.clone(), contours, cv::RETR_EXTERNAL,
				 cv::CHAIN_APPROX_SIMPLE);

	cv::Rect motion_rect;

	auto motion_rect_u_ptr = std::unique_ptr<cv::Rect>(
		tryDetectMotion(contours, motion_rect) ? new cv::Rect(motion_rect)
											   : nullptr);

	return std::unique_ptr<MotionFrame>(
		new MotionFrame(current_frame_, move(motion_rect_u_ptr)));
}

bool MotionDetector::tryDetectMotion(
	std::vector<std::vector<cv::Point>> &contours,
	cv::Rect &motion_rect)
{
	for (const auto &contour : contours)
	{
		if (cv::contourArea(contour) < 5000)
			continue;

		motion_rect = boundingRect(contour);

		return true;
	}

	return false;
}