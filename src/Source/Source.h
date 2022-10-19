#pragma once

#include <MediaData/MediaData.h>
#include <memory>

namespace fst {

class Source
{
public:
	using Ptr = std::shared_ptr<Source>;
	virtual ~Source() = default;

public:
	virtual MediaData::Ptr Read() = 0;

protected:
	Source() = default;
};

}  // namespace fst