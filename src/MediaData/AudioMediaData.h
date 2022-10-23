#pragma once

#include "MediaData.h"
namespace fst {

class AudioMediaData : public MediaData
{
public:
	using Ptr = std::shared_ptr<AudioMediaData>;

public:
	virtual ~AudioMediaData() = default;
	static AudioMediaData::Ptr create()
	{
		return AudioMediaData::Ptr(new AudioMediaData());
	}

	// 通过 MediaData 继承
	virtual MediaData::MEDIA_TYPE GetType() const override;

protected:
	AudioMediaData() = default;
};

}  // namespace fst