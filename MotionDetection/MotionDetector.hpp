#ifndef MOTIONDETECTOR_H
#define MOTIONDETECTOR_H

#include "../Camera/CameraStreamer.hpp"
#include "MotionFrame.hpp"
#include <iostream>
#include <vector>

// TODO: Re-implement pointers to use smart pointers
class MotionDetector
{
  public:
	explicit MotionDetector(CameraStreamer &camera_streamer);

	std::unique_ptr<MotionFrame> getNextFrame();

  private:
	CameraStreamer &streamer_;

	cv::UMat current_frame_;
	cv::UMat gray_current_frame_;
	cv::UMat average_frame_;

	bool has_average_frame_;

	bool tryDetectMotion(std::vector<std::vector<cv::Point>> &contours,
						 cv::Rect &motion_rect);
};

#endif