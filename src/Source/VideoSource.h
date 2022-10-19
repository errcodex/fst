#pragma once

#include "Source.h"

#include <memory>
namespace fst {

class VideoSource : virtual public Source
{
public:
	using Ptr = std::shared_ptr<VideoSource>;
	virtual ~VideoSource() = default;

public:
	virtual MediaData::Ptr Read() = 0;

protected:
	VideoSource() = default;
};

}  // namespace fst