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
	virtual void AddSource(Source::Ptr sourcePtr) override;
	virtual void AddSink(Sink::Ptr sinkPtr) override;

protected:
	AudioStream() = default;


};

}  // namespace fst