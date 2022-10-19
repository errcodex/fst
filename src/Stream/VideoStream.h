#pragma once

#include "Stream/Stream.h"

namespace fst {

class VideoStream : virtual public Stream
{
public:
	using Ptr = std::shared_ptr<VideoStream>;

	virtual ~VideoStream() = default;

public:
	// 通过 Stream 继承
	virtual void Write(MediaData::Ptr framePtr) override;
	virtual MediaData::Ptr Read() override;

protected:
	VideoStream() = default;

private:
	std::list<MediaData::Ptr> MediaDatas;
};

}  // namespace fst