#include "VideoMediaData.h"

using namespace fst;

VideoMediaData::VideoMediaData(VideoMediaData::TYPE type) : type(type) {}

VideoMediaData::Ptr fst::VideoMediaData::create(VideoMediaData::TYPE type)
{
	return VideoMediaData::Ptr(new VideoMediaData(type));
}

VideoMediaData::TYPE VideoMediaData::GetVideoType() const
{
	return type;
}

MediaData::TYPE VideoMediaData::GetType() const
{
	return MediaData::TYPE::VIDEO;
}
