#pragma once

#include "../Sink/Sink.h"
#include "../Source/Source.h"

namespace fst {

class Device
{
public:
	using Ptr = std::shared_ptr<Device>;
	enum class STATUS
	{
		OK
	};

public:
	virtual ~Device() = default;

	virtual bool Start() = 0;
	virtual bool Stop() = 0;
	virtual Source::Ptr GetSource() = 0;
	virtual Sink::Ptr GetSink() = 0;
	virtual Device::STATUS GetStatus() final
	{
		return status;
	};

protected:
	Device() = default;
	virtual void SetStatus(Device::STATUS status) final
	{
		this->status = status;
	};

private:
	STATUS status;
};

};	// namespace fst