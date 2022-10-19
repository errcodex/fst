#pragma once
#include "Channel.h"

#include <list>
#include <memory>
namespace fst {
class Engine
{
public:
	using Ptr = std::shared_ptr<Engine>;

public:
	static Engine::Ptr create()
	{
		return Engine::Ptr(new Engine());
	}

	bool Bind(Channel::Ptr p)
	{
		channels.emplace_back(p);
		return true;
	}

	virtual ~Engine() = default;

private:
	Engine() = default;

private:
	std::list<Channel::Ptr> channels;
};

}  // namespace fst