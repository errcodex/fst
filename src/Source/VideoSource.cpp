#include "VideoSource.h"

#include "MediaData/VideoMediaData.h"
#include "libutils/Time.h"

using namespace fst;

void VideoSource::SetSize(size_t width, size_t height)
{
	this->width = width;
	this->height = height;
}

void VideoSource::SetPixelFormat(VideoMediaData::PIXEL_FORMAT pixelFormat)
{
	this->pixelFormat = pixelFormat;
}

void VideoSource::Push(MediaData::Ptr mediaPtr)
{
	std::lock_guard<std::mutex> lck(mediaPoolRWMutex);
	mediaCache = mediaPtr;
}

MediaData::Ptr VideoSource::Pop()
{
	std::lock_guard<std::mutex> lck(mediaPoolRWMutex);

	return mediaCache;
}

MediaData::Ptr VideoSource::Read()
{
	return Pop();
}

size_t VideoSource::GetWidth() const
{
	return width;
}

size_t VideoSource::GetHeight() const
{
	return height;
}

VideoMediaData::PIXEL_FORMAT VideoSource::GetPixelFormat() const
{
	return pixelFormat;
}

bool VideoSource::Wirte(uint8_t* data, size_t size)
{
	if (VideoMediaData::calc_video_size(GetPixelFormat(), GetWidth(), GetHeight()) != size) return false;

	VideoMediaData::Ptr videoMediaPtr = VideoMediaData::create();
	videoMediaPtr->Fill(data, GetPixelFormat(), GetWidth(), GetHeight(), libutils::Time::steady_ts());

	Push(videoMediaPtr);

	return true;
}

bool VideoSource::Wirte(std::shared_ptr<uint8_t[]> dataPtr, size_t size)
{
	if (VideoMediaData::calc_video_size(GetPixelFormat(), GetWidth(), GetHeight()) != size) return false;

	VideoMediaData::Ptr videoMediaPtr = VideoMediaData::create();
	videoMediaPtr->Fill(dataPtr, GetPixelFormat(), GetWidth(), GetHeight(), libutils::Time::steady_ts());

	Push(videoMediaPtr);

	return true;
}

bool VideoSource::Write(MediaData::Ptr mediaPtr)
{
	if (!mediaPtr || mediaPtr->GetType() != MediaData::MEDIA_TYPE::VIDEO) return false;

	VideoMediaData::Ptr videoMediaPtr = std::dynamic_pointer_cast<VideoMediaData>(mediaPtr);

	if (!videoMediaPtr || videoMediaPtr->GetWidth() != GetWidth() || videoMediaPtr->GetHeight() != GetHeight()) return false;

	Push(videoMediaPtr);

	return true;
}