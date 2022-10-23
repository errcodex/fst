#pragma once

#include "Stream/Stream.h"
#include "libutils/Daemon.h"

#include <mutex>

namespace fst {

class VideoStream : virtual public Stream, public libutils::Daemon, public libutils::IDaemon
{
public:
	using Ptr = std::shared_ptr<VideoStream>;

	virtual ~VideoStream() = default;

public:
	static VideoStream::Ptr create();
	
	// 通过 Stream 继承
	virtual void AddSource(Source::Ptr sourcePtr) override;
	virtual void AddSink(Sink::Ptr sinkPtr) override;

	virtual bool Start(int64_t timestamp);
	virtual bool Stop();

protected:
	VideoStream();

private:
	bool PushMediaData(MediaData::Ptr mediaPtr);
	MediaData::ConstPtr PopMediaData();

	// 通过 IDaemon 继承
	virtual void Job() override;

private:
	std::mutex mediaDataPoolRWMutex;
	size_t mediaDataPoolSize = 5;
	std::list<MediaData::Ptr> mediaDataPool;

	MediaData::Ptr lastMediaData;

	std::mutex sourcePoolRWMutex;
	std::list<Source::Ptr> sourcePool;

	std::mutex sinkPoolRWMutex;
	std::list<Sink::Ptr> sinkPool;

	int64_t startTs = 0;
	int64_t timestampDiff = 0;
	int64_t nextFrameTs = 0;
	size_t framerate = 30;
	size_t pts = 0;
};

}  // namespace fst