#pragma once

#include "Stream/AudioStream.h"
#include "Stream/VideoStream.h"

namespace fst {

class MixVideoAudioStream : virtual public VideoStream, virtual public AudioStream
{
public:
	using Ptr = std::shared_ptr<MixVideoAudioStream>;

public:
	virtual ~MixVideoAudioStream() = default;
	static MixVideoAudioStream::Ptr create()
	{
		return MixVideoAudioStream::Ptr(new MixVideoAudioStream());
	}

	// 通过 Stream 继承
	virtual void AddSource(Source::Ptr sourcePtr) override;
	virtual void AddSink(Sink::Ptr sinkPtr) override;

protected:
	MixVideoAudioStream() = default;
};

}  // namespace fst