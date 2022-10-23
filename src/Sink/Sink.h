#pragma once

#include <memory>
#include "MediaData/MediaData.h"
namespace fst {

class Sink
{
public:
	using Ptr = std::shared_ptr<Sink>;
	virtual ~Sink() = default;

public:
	virtual bool Write(MediaData::ConstPtr mediaDataPtr) = 0;

protected:
	Sink() = default;
};

}  // namespace fst