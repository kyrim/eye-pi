#include "Camera/CameraStreamer.hpp"
#include "MotionDetection/MotionDetector.hpp"

#include <iostream>
#include <cstdio>
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

	cv::Size GetFirstFrameSize(MotionDetector &motion_detector)
	{
		while (!closed())
		{
			auto m_frame = motion_detector.getNextFrame();
			if (m_frame == nullptr)
				continue;

			return m_frame->getFrame().size();
		}
	}

	bool CreateBufferFile(MotionDetector &motion_detector,
						  string bufferFilePath,
						  int codec,
						  int fps,
						  cv::Size frameSize,
						  int bufferSizeInSeconds)
	{
		cv::VideoWriter outputVideo;
		outputVideo.open(bufferFilePath, codec, fps, frameSize, true);

		if (!outputVideo.isOpened())
			return false;

		// Buffer frames into a mp4 file of 1 second length
		auto t1 = Clock::now();
		while (std::chrono::duration_cast<std::chrono::seconds>(Clock::now() - t1).count() < 1)
		{
			auto m_frame = motion_detector.getNextFrame();

			if (m_frame == nullptr)
				continue;
			outputVideo << m_frame->getFrame().getMat(cv::ACCESS_READ);
		}

		return true;
	}

	int ReadBufferFile(string bufferFilePath, unsigned char *bufferFile)
	{
		streampos size = 0;
		ifstream file(bufferFilePath, ios::in | ios::binary | ios::ate);

		if (file.is_open())
		{
			size = file.tellg();
			bufferFile = new unsigned char[size];
			file.seekg(0, ios::beg);
			file.read((char *)bufferFile, size);
			file.close();
		}

		return size;
	}

	void Execute(const AsyncProgressWorker::ExecutionProgress &progress)
	{
		CameraStreamer cam(0);
		MotionDetector motion_detector(cam);

		string filename = "buffer";

		string bufferFilePath = "./data/" + filename + ".mp4";
		string bufferFragFilePath = "./data/" + filename + "-frag.mp4";

		std::remove(bufferFilePath.c_str());
		std::remove(bufferFragFilePath.c_str());

		auto fps = 25.0;
		cv::Size frameSize = GetFirstFrameSize(motion_detector);

		while (!closed())
		{
			if (!CreateBufferFile(motion_detector, bufferFilePath, CV_FOURCC('M', 'J', 'P', 'G'), fps, frameSize, 1))
				return;

			// TODO: Compile this instead of having a file?
			auto command = "mp4fragment --fragment-duration 1000 " + bufferFilePath + " " + bufferFragFilePath + "";
			system(command.c_str());
			std::remove(bufferFilePath.c_str());

			unsigned char *mp4FragmentData = nullptr;
			unsigned int mp4FragmentDataSize = ReadBufferFile(bufferFragFilePath, mp4FragmentData);
			std::remove(bufferFragFilePath.c_str());

			if (mp4FragmentData == nullptr)
				continue;
			// auto frame = m_frame->getFrame();
			// auto rect = m_frame->getRectangle();

			// handleFrame(frame, move(rect));

			json data;
			// auto hasMotion = rect != nullptr;
			// data["motionRect"]["hasMotion"] = hasMotion;
			// data["motionRect"]["x"] = hasMotion ? rect->x : 0;
			// data["motionRect"]["y"] = hasMotion ? rect->y : 0;
			// data["motionRect"]["width"] = hasMotion ? rect->width : 0;
			// data["motionRect"]["height"] = hasMotion ? rect->height : 0;

			// data["frame"]["width"] = frame.cols;
			// data["frame"]["height"] = frame.rows;

			data["frame"]["data"] = base64_encode(mp4FragmentData, mp4FragmentDataSize);

			Message tosend("newframe", data.dump());
			writeToNode(progress, tosend);

			delete[] mp4FragmentData;
		}
	}
};

StreamingWorker *create_worker(Callback *data, Callback *complete, Callback *error_callback, v8::Local<v8::Object> &options)
{
	return new EyePi(data, complete, error_callback, options);
}

NODE_MODULE(eye_pi, StreamWorkerWrapper::Init)