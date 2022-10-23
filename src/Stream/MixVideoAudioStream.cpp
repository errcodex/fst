#include "MixVideoAudioStream.h"

using namespace fst;

void MixVideoAudioStream::Write(MediaData::Ptr framePtr)
{
	if (framePtr->GetType() == MediaData::MEDIA_TYPE::VIDEO)
		VideoStream::Write(framePtr);
	else
		AudioStream::Write(framePtr);
}

MediaData::Ptr MixVideoAudioStream::Read()
{
	return MediaData::Ptr();
}