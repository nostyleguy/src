// ChatServerOnline.h
// Copyright 2000-02, Sony Online Entertainment Inc., all rights reserved. 
// Author: Justin Randall

#ifndef	_INCLUDED_ChatServerOnline_H
#define	_INCLUDED_ChatServerOnline_H

#include "sharedNetworkMessages/GameNetworkMessage.h"

//-----------------------------------------------------------------------

class ChatServerOnline : public GameNetworkMessage
{
public:
	ChatServerOnline(const std::string & address, const uint16_t port);
	ChatServerOnline(Archive::ReadIterator & source);
	~ChatServerOnline();

	const std::string &   getAddress  () const;
	const uint16_t  getPort     () const;

private:
	ChatServerOnline & operator = (const ChatServerOnline & rhs);
	ChatServerOnline(const ChatServerOnline & source);

	Archive::AutoVariable<std::string>     address;
	Archive::AutoVariable<uint16_t>  port;
};

//-----------------------------------------------------------------------

inline const std::string & ChatServerOnline::getAddress() const
{
	return address.get();
}

//-----------------------------------------------------------------------

inline const uint16_t ChatServerOnline::getPort() const
{
	return port.get();
}

//-----------------------------------------------------------------------

#endif	// _INCLUDED_ChatServerOnline_H
