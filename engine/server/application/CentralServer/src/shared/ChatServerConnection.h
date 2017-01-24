// ChatServerConnection.h
// Copyright 2000-02, Sony Online Entertainment Inc., all rights reserved. 
// Author: Justin Randall

#ifndef	_INCLUDED_ChatServerConnection_H
#define	_INCLUDED_ChatServerConnection_H

//-----------------------------------------------------------------------

#include "serverUtility/ServerConnection.h"

//-----------------------------------------------------------------------

class ChatServerConnection : public ServerConnection
{
public:
	explicit ChatServerConnection(UdpConnectionMT *, TcpClient *);
	~ChatServerConnection();
	const uint16_t  getGameServicePort  () const;
	void                  onConnectionClosed  ();
	void                  onConnectionOpened  ();
	void                  onReceive           (const Archive::ByteStream & message);
	void                  setGameServicePort  (const uint16_t gameServicePort);

private:
	ChatServerConnection & operator = (const ChatServerConnection & rhs);
	ChatServerConnection(const ChatServerConnection & source);
	uint16_t  gameServicePort;

}; //lint !e1712 default constructor not defined

//-----------------------------------------------------------------------

#endif	// _INCLUDED_ChatServerConnection_H
