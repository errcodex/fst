#pragma once
#include "Device.h"

namespace fst {

class CameraDevice : virtual public Device
{
public:
	using Ptr = std::shared_ptr<CameraDevice>;

public:
	virtual ~CameraDevice() = default;

	virtual Sink::Ptr GetSink() final
	{
		return nullptr;
	};

protected:
	CameraDevice() = default;
};

}  // namespace fst