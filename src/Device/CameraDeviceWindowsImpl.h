#pragma once

#include "CameraDevice.h"
#include "libutils/Daemon.h"

#include <memory>

class videoInput;
namespace fst {

class VideoSource;

class CameraDeviceWindowsImpl : public CameraDevice, private libutils::Daemon, private libutils::IDaemon
{
public:
	using Ptr = std::shared_ptr<CameraDeviceWindowsImpl>;

public:
	virtual ~CameraDeviceWindowsImpl() = default;
	static CameraDeviceWindowsImpl::Ptr create()
	{
		return CameraDeviceWindowsImpl::Ptr(new CameraDeviceWindowsImpl());
	}

	// 通过 CameraDevice 继承
	virtual bool Start() override;
	virtual bool Stop() override;
	virtual Source::Ptr GetSource() override;

protected:
	CameraDeviceWindowsImpl();

private:
	// 通过 IDaemon 继承
	virtual void Job() override;

private:
	std::shared_ptr<videoInput> videoInputPtr;
	std::shared_ptr<VideoSource> videoSourcePtr;
	size_t deviceId = 0;
	size_t width = 0;
	size_t height = 0;
};

};	// namespace fst