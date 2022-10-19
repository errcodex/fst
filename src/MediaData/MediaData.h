#pragma once
#include <memory>
namespace fst {

class MediaData
{
public:
	using Ptr = std::shared_ptr<MediaData>;
	enum class TYPE
	{
		VIDEO,
		AUDIO
	};

public:
	virtual ~MediaData() = default;
	virtual MediaData::TYPE GetType() const = 0;

protected:
	MediaData() = default;
};
}  // namespace fst