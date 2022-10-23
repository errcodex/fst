#pragma once

#include "MediaData.h"

namespace fst {

class VideoMediaData : public MediaData
{
public:
	using Ptr = std::shared_ptr<VideoMediaData>;
	enum class PIXEL_FORMAT
	{
		NONE,
		YUV420P,
		RGB24
	};

public:
	virtual ~VideoMediaData() = default;
	static VideoMediaData::Ptr create();
	static size_t calc_video_size(VideoMediaData::PIXEL_FORMAT pixelFormat, size_t width, size_t height);

	VideoMediaData::PIXEL_FORMAT GetVideoType() const;
	const std::shared_ptr<uint8_t[]> GetData() const;
	size_t GetWidth() const;
	size_t GetHeight() const;
	size_t GetSize() const;
	int64_t GetTimestemp() const;

	bool Fill(const std::shared_ptr<uint8_t[]> dataPtr, VideoMediaData::PIXEL_FORMAT pixelFormat, size_t width, size_t height, int64_t timestemp);

	bool Fill(const uint8_t* data, VideoMediaData::PIXEL_FORMAT pixelFormat, size_t width, size_t height, int64_t timestemp);

	// 通过 MediaData 继承
	virtual MediaData::MEDIA_TYPE GetType() const override;

protected:
	VideoMediaData() = default;

private:
	VideoMediaData::PIXEL_FORMAT pixelFormat;
	std::shared_ptr<uint8_t[]> data;
	size_t width = 0;
	size_t height = 0;
	size_t size = 0;
	int64_t timestemp = 0;
};

}  // namespace fst