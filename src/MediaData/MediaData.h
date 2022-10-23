#pragma once
#include <memory>
namespace fst {

class MediaData
{
public:
	using Ptr = std::shared_ptr<MediaData>;
	using ConstPtr = std::shared_ptr<const MediaData>;
	enum class MEDIA_TYPE
	{
		VIDEO,
		AUDIO
	};

public:
	virtual ~MediaData() = default;
	virtual MediaData::MEDIA_TYPE GetType() const = 0;

protected:
	MediaData() = default;
};
}  // namespace fst