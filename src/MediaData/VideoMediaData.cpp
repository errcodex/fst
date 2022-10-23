#include "VideoMediaData.h"

using namespace fst;

VideoMediaData::Ptr VideoMediaData::create()
{
	return VideoMediaData::Ptr(new VideoMediaData());
}

size_t VideoMediaData::calc_video_size(VideoMediaData::PIXEL_FORMAT pixelFormat, size_t width, size_t height)
{
	double sizeRatio = 1;
	switch (pixelFormat)
	{
		case VideoMediaData::PIXEL_FORMAT::YUV420P:
			sizeRatio = 3.f / 2.f;
			break;
		case VideoMediaData::PIXEL_FORMAT::RGB24:
			sizeRatio = 3.f;
			break;
		default:
			sizeRatio = 0;
			break;
	}
	return static_cast<size_t>(width * height * sizeRatio);
}

VideoMediaData::PIXEL_FORMAT VideoMediaData::GetVideoType() const
{
	return pixelFormat;
}

size_t VideoMediaData::GetWidth() const
{
	return width;
}

size_t VideoMediaData::GetHeight() const
{
	return height;
}

size_t VideoMediaData::GetSize() const
{
	return size;
}

int64_t VideoMediaData::GetTimestemp() const
{
	return timestemp;
}

size_t fst::VideoMediaData::GetPts() const
{
	return pts;
}

void VideoMediaData::SetPts(size_t pts)
{
	this->pts = pts;
}

bool VideoMediaData::Fill(const std::shared_ptr<uint8_t[]> dataPtr, VideoMediaData::PIXEL_FORMAT pixelFormat, size_t width, size_t height, int64_t timestemp)
{
	if (!dataPtr) return false;
	if (pixelFormat == VideoMediaData::PIXEL_FORMAT::NONE) return false;
	if (width * height <= 0u) return false;

	this->pixelFormat = pixelFormat;
	this->width = width;
	this->height = height;
	this->size = calc_video_size(pixelFormat, width, height);
	this->timestemp = timestemp;

	if (this->size <= 0) return false;	// pixel format not supported

	this->data = dataPtr;

	return true;
}

bool VideoMediaData::Fill(const uint8_t* data, VideoMediaData::PIXEL_FORMAT pixelFormat, size_t width, size_t height, int64_t timestemp)
{
	if (!data) return false;
	if (pixelFormat == VideoMediaData::PIXEL_FORMAT::NONE) return false;
	if (width * height <= 0u) return false;

	this->pixelFormat = pixelFormat;
	this->width = width;
	this->height = height;
	this->size = calc_video_size(pixelFormat, width, height);
	this->timestemp = timestemp;

	if (this->size <= 0) return false;	// pixel format not supported

	this->data = std::shared_ptr<uint8_t[]>(new uint8_t[size]);
	memcpy(this->data.get(), data, size);

	return true;
}

const std::shared_ptr<uint8_t[]> VideoMediaData::GetData() const
{
	return data;
}

MediaData::MEDIA_TYPE VideoMediaData::GetType() const
{
	return MediaData::MEDIA_TYPE::VIDEO;
}
