#include "Camera/CameraStreamer.hpp"
#include "MotionDetection/MotionDetector.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <nan.h>

using namespace std;

bool HandleRecording(bool newVideo, const std::string &videoFileName,
					 int dWidth, int dHeight, cv::VideoWriter &videoWriter,
					 const cv::Size &frameSize, cv::Mat &currentFrame,
					 double fps)
{
	if (newVideo)
	{
		std::cout << "File has been opened for writing: " << videoFileName
				  << std::endl;
		std::cout << "Frame Size = " << dWidth << "x" << dHeight << std::endl;

		videoWriter = cv::VideoWriter(videoFileName, CV_FOURCC('D', 'I', 'V', '3'),
									  fps, frameSize, true);
	}

	if (!videoWriter.isOpened())
	{
		std::cout << "ERROR: Failed to initialize video writing" << std::endl;
		getchar();

		return false;
	}

	videoWriter.write(currentFrame);

	return true;
}

void handleFrame(cv::UMat &frame, std::unique_ptr<cv::Rect> rect_ptr)
{
	if (rect_ptr != nullptr)
	{
		auto rect = *rect_ptr;

		cv::rectangle(frame, cv::Point(rect.x, rect.y),
					  cv::Point(rect.x + rect.width, rect.y + rect.height), cv::Scalar(0, 255, 0),
					  2);
	}

	cv::imshow("Stream", frame);
}

int main(int argc, char **argv)
{
	CameraStreamer cam(0);
	MotionDetector motion_detector(cam);
	auto textPoint = cv::Point(30, 30);
	auto textScalar = cv::Scalar(0, 0, 255);

	while (true)
	{
		auto start = cv::getTickCount();
		auto m_frame = motion_detector.getNextFrame();

		if (m_frame == nullptr)
			continue;

		auto frame = m_frame->getFrame();
		auto rect = m_frame->getRectangle();
		auto fps = cv::getTickFrequency() / (cv::getTickCount() - start);

		cv::putText(frame, std::to_string(fps), textPoint, cv::FONT_HERSHEY_SIMPLEX,
					0.8, textScalar);

		handleFrame(frame, move(rect));

		if (cv::waitKey(1) == 27)
			return 0;
	}
}

// if (motionDetected)
// {
// 	cout << "Motion detected, recording" << endl;
// 	motionCounter += 1;
// 	if (motionCounter >= 2)
// 	{

// 		if (!recording)
// 		{
// 			videoFileName = GetTime() + ".avi";
// 		}

// 		while (!recordingBuffer.empty())
// 		{
// 			HandleRecording(!recording,
// 							videoFileName,
// 							dWidth,
// 							dHeight, videoWriter,
// 							frameSize,
// 							recordingBuffer.front(),
// 							fps);

// 			recordingBuffer.pop();
// 			recording = true;
// 		}
// 		motionCounter = 1;
// 	}
// }

// capture.release();
