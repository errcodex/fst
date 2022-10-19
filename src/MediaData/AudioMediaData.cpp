#include "AudioMediaData.h"

using namespace fst;

MediaData::TYPE AudioMediaData::GetType() const
{
	return MediaData::TYPE::AUDIO;
}
