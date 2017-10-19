#include "Camera/CameraStreamer.hpp"
#include "MotionDetection/MotionDetector.hpp"

#include <iostream>
#include <chrono>

#include <opencv2/opencv.hpp>
#include "streaming-worker.h"
#include "Utility/json.hpp"
#include "Utility/base64.hpp"

using namespace std;
using json = nlohmann::json;

typedef std::chrono::high_resolution_clock Clock;

void handleFrame(cv::UMat &frame, std::unique_ptr<cv::Rect> rect_ptr)
{
	if (rect_ptr != nullptr)
	{
		auto rect = *rect_ptr;

		cv::rectangle(frame, cv::Point(rect.x, rect.y),
					  cv::Point(rect.x + rect.width, rect.y + rect.height), cv::Scalar(0, 255, 0),
					  2);
	}
}

class EyePi : public StreamingWorker
{
  public:
	EyePi(Callback *data, Callback *complete, Callback *error_callback, v8::Local<v8::Object> &options)
		: StreamingWorker(data, complete, error_callback)
	{
	}

	void Execute(const AsyncProgressWorker::ExecutionProgress &progress)
	{
		CameraStreamer cam(0);
		MotionDetector motion_detector(cam);

		auto timesCreatedVideo = 0;
		auto codec = CV_FOURCC('M', 'J', 'P', 'G');
		auto fps = 25.0;

		auto haveFirstFrame = false;

		cv::Size frameSize;
		while (!haveFirstFrame)
		{
			auto m_frame = motion_detector.getNextFrame();
			if (m_frame == nullptr)
				continue;

			auto frame = m_frame->getFrame();
			frameSize = frame.size();

			haveFirstFrame = true;
		}

		while (!closed())
		{
			cv::VideoWriter outputVideo;
			outputVideo.open(("./data/" + to_string(timesCreatedVideo++) + ".mp4"), codec, fps, frameSize, true);
			if (!outputVideo.isOpened())
				return;

			auto t1 = Clock::now();

			while (std::chrono::duration_cast<std::chrono::seconds>(Clock::now() - t1).count() < 1)
			{
				auto m_frame = motion_detector.getNextFrame();

				if (m_frame == nullptr)
					continue;

				outputVideo << m_frame->getFrame().getMat(cv::ACCESS_READ);
			}
			// auto frame = m_frame->getFrame();
			// auto rect = m_frame->getRectangle();

			// handleFrame(frame, move(rect));

			json data;
			data["0"] = 1;
			// auto hasMotion = rect != nullptr;
			// data["motionRect"]["hasMotion"] = hasMotion;
			// data["motionRect"]["x"] = hasMotion ? rect->x : 0;
			// data["motionRect"]["y"] = hasMotion ? rect->y : 0;
			// data["motionRect"]["width"] = hasMotion ? rect->width : 0;
			// data["motionRect"]["height"] = hasMotion ? rect->height : 0;

			// data["frame"]["width"] = frame.cols;
			// data["frame"]["height"] = frame.rows;
			// data["frame"]["data"] = base64_encode(enc_msg, buf.size());

			Message tosend("newframe", data.dump());
			writeToNode(progress, tosend);
		}
	}
};

StreamingWorker *create_worker(Callback *data, Callback *complete, Callback *error_callback, v8::Local<v8::Object> &options)
{
	return new EyePi(data, complete, error_callback, options);
}

NODE_MODULE(eye_pi, StreamWorkerWrapper::Init)