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
	bool PushVideoData(uint8_t* data, size_t size)
	{
		return VideoSource::Wirte(data, size);
	};

	void Init(VideoMediaData::PIXEL_FORMAT pixFmt, size_t width, size_t height)
	{
		VideoSource::SetPixelFormat(pixFmt);
		VideoSource::SetSize(width, height);
	};
};

CameraDeviceWindowsImpl::CameraDeviceWindowsImpl() : Daemon(this)
{
	videoInput::setComMultiThreaded(true);
	videoInput::setVerbose(true);

	videoInputPtr = std::make_shared<videoInput>();
	videoSourcePtr = std::make_shared<VideoSourceCameraDeviceWindowsImpl>();
	videoInputPtr->setRequestedMediaSubType(VI_MEDIASUBTYPE_MJPG);

	// TODO: 分散到具体功能函数中
	auto list = videoInputPtr->getDeviceList();
	deviceId = 1;									 // 1,3 to local test
	videoInputPtr->setIdealFramerate(deviceId, 60);	 // fps
	width = 1280;
	height = 720;
	
	std::dynamic_pointer_cast<VideoSourceCameraDeviceWindowsImpl>(videoSourcePtr)->Init(VideoMediaData::PIXEL_FORMAT::RGB24, width, height);
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
	return Daemon::Stop();

	videoInputPtr->stopDevice(deviceId);
}

Source::Ptr CameraDeviceWindowsImpl::GetSource()
{
	return videoSourcePtr;
}

void CameraDeviceWindowsImpl::Job()
{
	auto frameSize = videoInputPtr->getSize(deviceId);
	uint8_t* frame = nullptr;
	while (Daemon::IsRunnable())
	{
		if (!videoInputPtr->isFrameNew(deviceId))
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			continue;
		}
		if (!(frame = videoInputPtr->getPixels(deviceId, true, true)))
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			continue;
		}
		std::dynamic_pointer_cast<VideoSourceCameraDeviceWindowsImpl>(videoSourcePtr)->PushVideoData(frame, frameSize);
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}
