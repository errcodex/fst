#include "CameraDeviceWindowsImpl.h"

#include "MediaData/VideoMediaData.h"
#include "Source/VideoSource.h"
#include "videoInput.h"

#include <mutex>
#include <thread>

using namespace fst;
using namespace libutils;

class VideoSourceCameraDeviceWindowsImpl : public VideoSource
{
public:
	void Write(const uint8_t* framePtr, size_t width, size_t height)
	{
		std::lock_guard<std::mutex> lck(mtx);

		this->framePtr = std::shared_ptr<uint8_t[]>(new uint8_t[width * height * 3]);
		this->width = width;
		this->height = height;
		memcpy(this->framePtr.get(), framePtr, width * height * 3);
	};

	void Write(std::shared_ptr<uint8_t[]> framePtr, size_t width, size_t height)
	{
		std::lock_guard<std::mutex> lck(mtx);

		this->framePtr = framePtr;
		this->width = width;
		this->height = height;
	};

	// 通过 VideoSource 继承
	virtual MediaData::Ptr Read() override
	{
		std::lock_guard<std::mutex> lck(mtx);

		auto p = VideoMediaData::create(VideoMediaData::TYPE::RGB24);
		p->data = framePtr;
		p->size = width * height;
		return p;
	}

private:
	std::mutex mtx;
	std::shared_ptr<uint8_t[]> framePtr;
	size_t width = 0;
	size_t height = 0;
};

CameraDeviceWindowsImpl::CameraDeviceWindowsImpl() : Daemon(this)
{
	videoInputPtr = std::make_shared<videoInput>();
	videoSourcePtr = std::make_shared<VideoSourceCameraDeviceWindowsImpl>();

	// TODO: 分散到具体功能函数中
	auto list = videoInputPtr->getDeviceList();
	deviceId = 1;									 // 1,3 to local test
	videoInputPtr->setIdealFramerate(deviceId, 60);	 // fps
	videoInputPtr->setVerbose(true);
	width = 1280;
	height = 720;
}

bool CameraDeviceWindowsImpl::Start()
{
	if (!videoInputPtr->setupDevice(deviceId, width, height, VI_COMPOSITE)) return false;

	width = videoInputPtr->getWidth(deviceId);
	height = videoInputPtr->getHeight(deviceId);

	return Daemon::Start();
}

bool CameraDeviceWindowsImpl::Stop()
{
	videoInputPtr->stopDevice(deviceId);

	return Daemon::Stop();
}

Source::Ptr CameraDeviceWindowsImpl::GetSource()
{
	return videoSourcePtr;
}

void CameraDeviceWindowsImpl::Job()
{
	while (Daemon::IsRunnable())
	{
		if (!videoInputPtr->isFrameNew(deviceId))
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			continue;
		}

		auto framePtr = std::shared_ptr<uint8_t[]>(new uint8_t[width * height * 3]);
		if (!videoInputPtr->getPixels(deviceId, framePtr.get(), true, true))
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			continue;
		}

		std::dynamic_pointer_cast<VideoSourceCameraDeviceWindowsImpl>(videoSourcePtr)->Write(framePtr, width, height);
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}
