#include "CameraStreamer.hpp"
#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

// Constructor for IP Cameras
CameraStreamer::CameraStreamer(std::string stream_source)
	: is_usb_camera_(false), camera_source(std::move(stream_source))
{
	startCapture();
}

// Constructor for USB Cameras
CameraStreamer::CameraStreamer(int capture_index)
	: is_usb_camera_(true), camera_index(capture_index)
{
	startCapture();
}

CameraStreamer::~CameraStreamer() { stopCapture(); }

void CameraStreamer::startCapture()
{
	// Create the camera capture based on
	// the input specified to the object (USB / IP Camera)
	createCameraCapture();

	getFrameSize();
}

bool CameraStreamer::tryGetNextFrame(cv::UMat &frame)
{

	(*camera_capture_) >> frame;

	return true;
}

cv::Size CameraStreamer::getFrameSize()
{
	auto dWidth = camera_capture_->get(CV_CAP_PROP_FRAME_WIDTH);
	auto dHeight = camera_capture_->get(CV_CAP_PROP_FRAME_HEIGHT);

	return cv::Size(static_cast<int>(dWidth), static_cast<int>(dHeight));
}

void CameraStreamer::createCameraCapture()
{
	camera_capture_ = std::unique_ptr<cv::VideoCapture>(
		is_usb_camera_ ? new cv::VideoCapture(camera_index)
					   : new cv::VideoCapture(camera_source));
}

void CameraStreamer::stopCapture()
{
	if (camera_capture_->isOpened())
	{
		camera_capture_->release();
	}
}