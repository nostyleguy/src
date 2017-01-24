// ChatServerConnection.cpp
// Copyright 2000-02, Sony Online Entertainment Inc., all rights reserved. 
// Author: Justin Randall

//-----------------------------------------------------------------------

#include "FirstCentralServer.h"
#include "ChatServerConnection.h"

//-----------------------------------------------------------------------

ChatServerConnection::ChatServerConnection(UdpConnectionMT * u, TcpClient * t) :
ServerConnection(u, t),
gameServicePort(0)
{
}

//-----------------------------------------------------------------------

ChatServerConnection::~ChatServerConnection()
{
}

//-----------------------------------------------------------------------

const uint16_t ChatServerConnection::getGameServicePort() const
{
	return gameServicePort;
}

//-----------------------------------------------------------------------

void ChatServerConnection::onConnectionOpened()
{
	ServerConnection::onConnectionOpened();
	static MessageConnectionCallback m("ChatServerConnectionOpened");
	emitMessage(m);
}

//-----------------------------------------------------------------------

void ChatServerConnection::onConnectionClosed()
{
	ServerConnection::onConnectionClosed();
	static MessageConnectionCallback m("ChatServerConnectionClosed");
	emitMessage(m);
}

//-----------------------------------------------------------------------

void ChatServerConnection::onReceive(const Archive::ByteStream & message)
{
	ServerConnection::onReceive(message);
}

//-----------------------------------------------------------------------

void ChatServerConnection::setGameServicePort(const uint16_t p)
{
	gameServicePort = p;
}

//-----------------------------------------------------------------------

