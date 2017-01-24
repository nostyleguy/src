
#ifndef AUCTIONSTRANFERCLIENT_H
#define AUCTIONSTRANFERCLIENT_H

#ifndef _WIN32
#include "AuctionTransferGameAPI/AuctionTransferAPI.h"
#include <stdint.h>
#else
namespace AuctionTransfer
{
	class AuctionTransferAPI
	{
	public:
		AuctionTransferAPI( const char *[], int16_t [], int , const char *[], unsigned )
		{};
		virtual void process() {};
	};

};
#endif

class AuctionTransferClient : public AuctionTransfer::AuctionTransferAPI
{
public:
    AuctionTransferClient(const char *hostName[], int16_t port[], int count, const char *identifier[], unsigned identifierCount);
    virtual ~AuctionTransferClient();


    // Connection status callbacks
    virtual void onConnect(const char* host, uint16_t port, int16_t current, int16_t max);
    virtual void onDisconnect(const char *host, int16_t port, int16_t current, int16_t max);


    // Callbacks that are responses
    void onSendPrepareTransaction(uint32_t, uint32_t, void *) {};
    void onSendPrepareTransactionCompressed(uint32_t, uint32_t, void *) {};
    void onSendCommitTransaction(uint32_t, uint32_t, void *) {};
    void onSendAbortTransaction(uint32_t, uint32_t, void *) {};
    void onSendAuditAssetTransfer(uint32_t, uint32_t, void *) {};

    void onGetNewTransactionID(uint32_t, uint32_t, int64, void *) {};

    // Responses to reply requests
    void onReplyReceivePrepareTransaction(uint32_t, uint32_t, void *) {};
    void onReplyReceiveCommitTransaction(uint32_t, uint32_t, void *) {};
    void onReplyReceiveAbortTransaction(uint32_t, uint32_t, void *) {};
    void onReplyReceiveGetCharacterList(uint32_t, uint32_t, void *) {};

    // house keeping
    void onIdentifyHost(uint32_t, uint32_t, void *) {};

    // callbacks initiated by auction
    void onReceivePrepareTransaction(uint32_t, int64, uint32_t, uint32_t, int64, const char *) {};
    void onReceiveCommitTransaction(uint32_t, int64) {};
    void onReceiveAbortTransaction(uint32_t, int64) {};
    void onReceiveGetCharacterList(uint32_t, uint32_t, const char *) {};

private:

/*************************************************
    struct AuctionAssetDetails
    {
        enum eAssetTypes
        {
            TYPE_ITEM,
            TYPE_COIN,
            TYPE_CHARACTER,
            TYPE_FOR_IMAGE_UPLOAD
        };

        AuctionAssetDetails()
            : u8Type( TYPE_ITEM ),
              u32AccountNum( 0 ),
              u32CharDBID( 0 ),
              i64ItemDBID( INVALID_EQUID ),
              u32ItemStackCount( 0 ),
              i64AssetID( 0 )
        {}

        uns8 u8Type;
        uns32 u32AccountNum;
        uns32 u32CharDBID;
        int64 i64ItemDBID;
        uns32 u32ItemStackCount;
        int64 i64Copper;
        CharString sAssetXML;

        int64 i64AssetID;
    };

    typedef std::map< unsigned, AuctionAssetDetails > PendingRequestMap;
    PendingRequestMap m_mPendingRequestDetails;





    struct IncomingPrepareDetails
    {
        int64 i64TransactionID;
        int64 i64AssetID;
        uns32 u32DestStationID;
        uns32 u32DestCharDBID;
    };
    typedef std::map< unsigned, IncomingPrepareDetails > IncomingPrepareMap;//key=tracking
    IncomingPrepareMap m_mPendingIncomingPrepareDetails;



    typedef std::map< unsigned, int64 > IncomingCommitMap;//key=tracking, val=transaction
    IncomingCommitMap m_mPendingIncomingCommitDetails;



    struct TransferDetails
    {
        TransferDetails()
            : i64AssetID( 0 ),
              u32DestStationID( 0 ),
              u32DestCharDBID( INVALID_EQUID )
        {}

        int64 i64AssetID;
        uns32 u32DestStationID;
        uns32 u32DestCharDBID;

        //used when committing
        std::vector< uns8 > aAssetSerialData;
        int32 i32Copper;
    };
    typedef std::map< int64, TransferDetails > PendingTransferMap;
    PendingTransferMap m_mPendingTransferDetails;
**********************************************/


};


#endif
