#include "AudioStream.h"

using namespace fst;

void AudioStream::Write(MediaData::Ptr framePtr) {}

MediaData::Ptr AudioStream::Read()
{
	return MediaData::Ptr();
}
