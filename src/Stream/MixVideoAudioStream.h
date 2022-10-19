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
	virtual void Write(MediaData::Ptr framePtr) override;
	virtual MediaData::Ptr Read() override;

protected:
	MixVideoAudioStream() = default;
};

}  // namespace fst