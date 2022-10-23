#include "VideoStream.h"

#include "MediaData/VideoMediaData.h"
#include "libutils/Time.h"

#include <limits>

using namespace fst;
using namespace libutils;

VideoStream::VideoStream() : Daemon(this) {}

VideoStream::Ptr fst::VideoStream::create()
{
	return VideoStream::Ptr(new VideoStream());
}

void fst::VideoStream::AddSource(Source::Ptr sourcePtr)
{
	std::lock_guard<std::mutex> lck(sourcePoolRWMutex);
	sourcePool.push_back(sourcePtr);
}

void fst::VideoStream::AddSink(Sink::Ptr sinkPtr)
{
	std::lock_guard<std::mutex> lck(sinkPoolRWMutex);
	sinkPool.push_back(sinkPtr);
}

bool VideoStream::Start(int64_t timestamp)
{
	startTs = Time::steady_ts();
	timestampDiff = startTs - timestamp;
	nextFrameTs = INT64_MIN;
	pts = -1;
	return Daemon::Start();
}

bool VideoStream::Stop()
{
	return Daemon::Stop();
}

bool VideoStream::PushMediaData(MediaData::Ptr mediaPtr)
{
	std::lock_guard<std::mutex> lck(mediaDataPoolRWMutex);

	// check media data type
	if (mediaPtr->GetType() != MediaData::MEDIA_TYPE::VIDEO) return false;
	VideoMediaData::Ptr videoMediaPtr = std::dynamic_pointer_cast<VideoMediaData>(mediaPtr);

	//// outdated
	// if (videoMediaPtr->GetTimestemp() + timestampDiff < libutils::Time::steady_ts())
	//	return false;

	// pool is full
	if (mediaDataPool.size() >= mediaDataPoolSize)
	{
		mediaDataPool.pop_front();
		printf("VideoStream force pop\n");
	}

	mediaDataPool.push_back(mediaPtr);
	return true;
}

MediaData::ConstPtr VideoStream::PopMediaData()
{
	std::lock_guard<std::mutex> lck(mediaDataPoolRWMutex);
	if (mediaDataPool.empty()) return nullptr;

	VideoMediaData::Ptr tmp = nullptr;
	do {
		tmp = std::dynamic_pointer_cast<VideoMediaData>(mediaDataPool.front());
		// if (1 >= mediaDataPool.size()) break;
		mediaDataPool.pop_front();
	} while (!mediaDataPool.empty() && tmp->GetTimestemp() + timestampDiff < libutils::Time::steady_ts());

	tmp->SetPts(pts - 1);
	lastMediaData = tmp;
	return tmp;
}

void fst::VideoStream::Job()
{
	while (Daemon::IsRunnable())
	{
		int64_t ts = Time::steady_ts();
		if (nextFrameTs > ts)
		{
			// sleep 1/3 framerate time
			std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<size_t>(1000.f / framerate / 3)));
			continue;
		}
		++pts;
		nextFrameTs = startTs + (1000.f / framerate) * (pts + 1);

		// get media data from source and push to pool 
		{
			std::lock_guard<std::mutex> lck(sourcePoolRWMutex);
			if (mediaDataPool.empty())
			{
				if (sourcePool.empty()) continue;
				auto tmpMediaDataPtr = sourcePool.front()->Read();
				if (tmpMediaDataPtr == nullptr) continue;
				PushMediaData(tmpMediaDataPtr);
			}
		}

		// pop from pool, use cache if pool is empty
		auto mediaDataPtr = PopMediaData();
		if (!mediaDataPtr)
		{
			if (!lastMediaData)
			{
				++pts;
				continue;
			}
			std::dynamic_pointer_cast<VideoMediaData>(lastMediaData)->SetPts(pts - 1);
			mediaDataPtr = lastMediaData;
		}
		
		// push to sink
		{
			std::lock_guard<std::mutex> lck(sinkPoolRWMutex);
			for (auto sinkPtr : sinkPool)
			{
				sinkPtr->Write(mediaDataPtr);
			}
		}

		// sleep 1/3 framerate time
		std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<size_t>(1000.f / framerate / 3)));
	}
}
