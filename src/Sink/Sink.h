#pragma once

#include <memory>
namespace fst {

class Sink
{
public:
	using Ptr = std::shared_ptr<Sink>;
	virtual ~Sink() = default;

public:
	virtual bool Write() = 0;

protected:
	Sink() = default;
};

}  // namespace fst