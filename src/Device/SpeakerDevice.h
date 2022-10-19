#pragma once
#include "Device.h"

namespace fst {

class SpeakerDevice : virtual public Device
{
public:
	using Ptr = std::shared_ptr<SpeakerDevice>;

public:
	virtual ~SpeakerDevice() = default;

	virtual Source::Ptr GetSource() final
	{
		return nullptr;
	};

protected:
	SpeakerDevice() = default;
};

}  // namespace fst