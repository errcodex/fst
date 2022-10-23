#pragma once

#include "Sink/Sink.h"
#include "Source/Source.h"

#include <list>
#include <memory>

namespace fst {

class Stream 
{
public:
	using Ptr = std::shared_ptr<Stream>;

	virtual ~Stream() = default;

public:
	virtual void AddSource(Source::Ptr sourcePtr) = 0;
	virtual void AddSink(Sink::Ptr sinkPtr) = 0;

protected:
	Stream() = default;
};

}  // namespace fst