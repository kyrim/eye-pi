#include "Camera/CameraStreamer.hpp"
#include "MotionDetection/MotionDetector.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>
#include "streaming-worker.h"
#include "Utility/json.hpp"
#include "Utility/base64.hpp"

using namespace std;
using json = nlohmann::json;

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
		auto textPoint = cv::Point(30, 30);
		auto textScalar = cv::Scalar(0, 0, 255);

		json sample;

		while (!closed())
		{
			auto start = cv::getTickCount();
			auto m_frame = motion_detector.getNextFrame();

			if (m_frame == nullptr)
				continue;

			auto frame = m_frame->getFrame();
			auto rect = m_frame->getRectangle();

			handleFrame(frame, move(rect));

			json data;

			cv::Mat mat = frame.getMat(cv::ACCESS_READ);
			int len = mat.total() * mat.elemSize();

			std::vector<uchar> buf;
			imencode(".jpg", frame, buf);
			uchar *enc_msg = new uchar[buf.size()];

			for (int i = 0; i < buf.size(); i++)
				enc_msg[i] = buf[i];

			auto hasMotion = rect != nullptr;
			data["motionRect"]["hasMotion"] = hasMotion;
			data["motionRect"]["x"] = hasMotion ? rect->x : 0;
			data["motionRect"]["y"] = hasMotion ? rect->y : 0;
			data["motionRect"]["width"] = hasMotion ? rect->width : 0;
			data["motionRect"]["height"] = hasMotion ? rect->height : 0;
			data["frame"] = base64_encode(enc_msg, buf.size());

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