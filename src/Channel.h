#pragma once
#include "Stream/Stream.h"

#include <list>
#include <memory>
#include <string>

namespace fst {

class Channel
{
public:
	using Ptr = std::shared_ptr<Channel>;
	using ID = std::string;
	enum class ROLE
	{
		ROOT,
		ADMINISTRATOR,
		VISITOR,
		DEFAULT = VISITOR
	};

public:
	static Channel::Ptr create();

	virtual ~Channel() = default;

	bool Join(const Channel::ID& channelID, Channel::ROLE role = Channel::ROLE::DEFAULT);

	bool Bind(Stream::Ptr stream);

private:
	Channel() = default;
	struct
	{
		Channel::ID channelID;
		Channel::ROLE role = Channel::ROLE::DEFAULT;
	} context;

	std::list<Stream::Ptr> streams;
};

}  // namespace fst