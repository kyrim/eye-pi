#include "streaming-worker.h"

class Simple : public StreamingWorker
{
  public:
	Simple(Callback *data, Callback *complete, Callback *error_callback, v8::Local<v8::Object> &options)
		: StreamingWorker(data, complete, error_callback)
	{
	}

	void Execute(const AsyncProgressWorker::ExecutionProgress &progress)
	{
		for (int i = 0; i < 100; i++)
		{
			Message tosend("integer", std::to_string(i));
			writeToNode(progress, tosend);
		}
	}
};

StreamingWorker *create_worker(Callback *data, Callback *complete, Callback *error_callback, v8::Local<v8::Object> &options)
{
	return new Simple(data, complete, error_callback, options);
}

NODE_MODULE(eye_pi, StreamWorkerWrapper::Init)