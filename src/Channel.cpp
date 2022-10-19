#include "Channel.h"

using namespace fst;

Channel::Ptr Channel::create()
{
	return Channel::Ptr(new Channel());
}

bool Channel::Join(const Channel::ID& channelID, Channel::ROLE role)
{
	if (role != ROLE::VISITOR) return false;

	context.channelID = channelID;
	context.role = role;

	return true;
}

bool Channel::Bind(Stream::Ptr stream)
{
	streams.emplace_back(stream);
	return true;
}
