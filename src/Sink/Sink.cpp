#pragma once

#include <MediaData/MediaData.h>
#include <memory>
namespace fst {

class Sink
{
public:
	using Ptr = std::shared_ptr<Sink>;
	virtual ~Sink() = default;

public:
	virtual MediaData::Ptr Write() = 0;

protected:
	Sink() = default;
};

}  // namespace fst