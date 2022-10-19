#pragma once

#include "Stream/Stream.h"

namespace fst {

class AudioStream : virtual public Stream
{
public:
	using Ptr = std::shared_ptr<AudioStream>;

public:
	virtual ~AudioStream() = default;

	// 通过 Stream 继承
	virtual void Write(MediaData::Ptr framePtr) override;
	virtual MediaData::Ptr Read() override;

protected:
	AudioStream() = default;
};

}  // namespace fst