#include "AudioMediaData.h"

using namespace fst;

MediaData::MEDIA_TYPE AudioMediaData::GetType() const
{
	return MediaData::MEDIA_TYPE::AUDIO;
}
