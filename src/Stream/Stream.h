#pragma once

#include "MediaData/MediaData.h"

#include <list>
#include <memory>

namespace fst {

class Stream
{
public:
	using Ptr = std::shared_ptr<Stream>;

	virtual ~Stream() = default;

public:
	virtual void Write(MediaData::Ptr framePtr) = 0;
	virtual MediaData::Ptr Read() = 0;

protected:
	Stream() = default;
};

}  // namespace fst