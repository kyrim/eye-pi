#ifndef CAMERASTREAMER_H
#define CAMERASTREAMER_H

#include <opencv2/opencv.hpp>

// TODO: Re-implement pointers to use smart pointers

class CameraStreamer
{
  public:
	//Constructor for IP Camera capture
	explicit CameraStreamer(std::string stream_source);
	//Constructor for USB Camera capture
	explicit CameraStreamer(int capture_index);
	~CameraStreamer();

	bool tryGetNextFrame(cv::UMat &frame);

  private:
	std::string camera_source;
	int camera_index;

	std::unique_ptr<cv::VideoCapture> camera_capture_;
	bool is_usb_camera_;

	void startCapture();
	void stopCapture();
	cv::Size getFrameSize();
	void createCameraCapture();
};

#endif