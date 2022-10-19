#pragma once
#include "Device.h"

namespace fst {

class MicrophoneDevice : virtual public Device
{
public:
	using Ptr = std::shared_ptr<MicrophoneDevice>;

public:
	virtual ~MicrophoneDevice() = default;

	virtual Sink::Ptr GetSink() final
	{
		return nullptr;
	};

protected:
	MicrophoneDevice() = default;
};

}  // namespace fst