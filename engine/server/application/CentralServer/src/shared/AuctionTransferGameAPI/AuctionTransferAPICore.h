#ifndef AUCTIONTRANSFERAPICORE_H
#define AUCTIONTRANSFERAPICORE_H


#include "ATGenericAPI/GenericApiCore.h"
#include "ATGenericAPI/GenericConnection.h"

//////////////////////////////////////////////////////////////////////////////////////
namespace AuctionTransfer
{

class AuctionTransferAPI;

//////////////////////////////////////////////////////////////////////////////////////
class AuctionTransferAPICore : public GenericAPICore
{
public:
	AuctionTransferAPICore(const char *hostName[], const int16_t port[], int count, AuctionTransferAPI *api, const char *identifier[], unsigned identifierCount, unsigned timeout = 60, unsigned maxRecvMessageSizeInKB = 16000);
	virtual ~AuctionTransferAPICore();

	void OnConnect(GenericConnection *connection);
	void OnDisconnect(GenericConnection *connection);
	void responseCallback(GenericResponse *response);
	void responseCallback(int16_t type, Base::ByteStream::ReadIterator &iter, GenericConnection *connection);

private:
	AuctionTransferAPI *m_api;
	unsigned m_mappedServerTrack;
};

}; // namespace AuctionTransfer
//////////////////////////////////////////////////////////////////////////////////////

#endif //AUCTIONTRANSFERAPICORE_H

