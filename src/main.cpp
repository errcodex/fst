#include "Device/CameraDeviceWindowsImpl.h"
#include "Device/Device.h"
#include "MediaData/VideoMediaData.h"

#include <cassert>
#include <thread>

using namespace fst;

int main(int, char*[])
{
	Device::Ptr t = CameraDeviceWindowsImpl::create();
	assert(t->Start());

	auto source = t->GetSource();
	FILE* file = fopen("t.yuv", "wb");
	size_t size;
	for (size_t i = 0; i < 60; ++i)
	{
		auto mediaData = std::dynamic_pointer_cast<VideoMediaData>(source->Read());
		if (mediaData->size == 0)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			continue;
		}
		assert(size = fwrite(mediaData->data.get(), 1, mediaData->size * 3, file));
		std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 60));
	}
	fclose(file);

	assert(t->Stop());

	return 0;
}