#include "Device/CameraDeviceWindowsImpl.h"
#include "Device/Device.h"
#include "FileHelper.h"
#include "MediaData/VideoMediaData.h"
#include "libutils/Time.h"

#include <cassert>
#include <thread>

using namespace fst;

int main(int, char *[])
{
	Device::Ptr t = CameraDeviceWindowsImpl::create();
	VideoStream::Ptr s = VideoStream::create();
	std::shared_ptr<FileHelper> f = std::make_shared<FileHelper>();
	f->init(1280, 720, 30);
	assert(t->Start());

	auto source = t->GetSource();
	s->AddSource(source);
	s->AddSink(f);
	auto ts = libutils::Time::steady_ts();
	s->Start(ts);
	std::this_thread::sleep_for(std::chrono::seconds(5));
	s->Stop();
	
	f->release();
	assert(t->Stop());
	return 0;

	/*
	Device::Ptr t = CameraDeviceWindowsImpl::create();
	FileHelper f;
	f.init(1280, 720, 30);
	assert(t->Start());

	auto source = t->GetSource();
	auto time = libutils::Time::steady_time();
	for (size_t i = 0; i < 300;)
	{
		auto mediaData = std::dynamic_pointer_cast<VideoMediaData>(source->Read());
		if (!mediaData || mediaData->GetSize() == 0)
		{
			std::this_thread::yield();
			// std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 60 / 3));
			continue;
		}
		++i;
		f.push(mediaData);
		std::this_thread::yield();
		// std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 60 / 3));
	}
	f.release();

	assert(t->Stop());
	auto timeUsed = libutils::Time::timing_ms(time);
	return 0;
	*/
}