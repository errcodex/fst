#pragma once

#include "MediaData.h"

namespace fst {

class VideoMediaData : public MediaData
{
public:
	using Ptr = std::shared_ptr<VideoMediaData>;
	enum class TYPE
	{
		RGB24
	};
	std::shared_ptr<uint8_t[]> data;
	size_t size = 0;

public:
	virtual ~VideoMediaData() = default;
	static VideoMediaData::Ptr create(VideoMediaData::TYPE type);

	VideoMediaData::TYPE GetVideoType() const;

	// 通过 MediaData 继承
	virtual MediaData::TYPE GetType() const override;

protected:
	VideoMediaData(VideoMediaData::TYPE type);

private:
	VideoMediaData::TYPE type;
};

}  // namespace fst