#pragma once

#include "MediaData/MediaData.h"
#include "MediaData/VideoMediaData.h"
#include "Source.h"

#include <list>
#include <memory>
#include <mutex>
namespace fst {

class VideoSource : virtual public Source
{
public:
	using Ptr = std::shared_ptr<VideoSource>;
	virtual ~VideoSource() = default;

public:
	// Í¨¹ý Source ¼Ì³Ð
	virtual MediaData::Ptr Read() override;

	virtual size_t GetWidth() const final;
	virtual size_t GetHeight() const final;
	virtual VideoMediaData::PIXEL_FORMAT GetPixelFormat() const final;

protected:
	VideoSource() = default;
	virtual bool Wirte(uint8_t* data, size_t size) final;
	virtual bool Wirte(std::shared_ptr<uint8_t[]> dataPtr, size_t size) final;
	virtual bool Write(MediaData::Ptr mediaPtr) final;
	virtual void SetSize(size_t width, size_t height) final;
	virtual void SetPixelFormat(VideoMediaData::PIXEL_FORMAT pixelFormat) final;

private:
	void Push(MediaData::Ptr mediaPtr);
	MediaData::Ptr Pop();

private:
	size_t width = 0;
	size_t height = 0;
	VideoMediaData::PIXEL_FORMAT pixelFormat = VideoMediaData::PIXEL_FORMAT::NONE;

	std::mutex mediaPoolRWMutex;
	size_t mediaPoolCacheSize = 10;
	std::list<MediaData::Ptr> mediaPool;
};

}  // namespace fst