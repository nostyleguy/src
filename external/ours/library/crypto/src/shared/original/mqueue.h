#ifndef CRYPTOPP_MQUEUE_H
#define CRYPTOPP_MQUEUE_H

#include "queue.h"
#include "filters.h"
#include <deque>

#define ULONG_SIZE sizeof(uint32_t)

NAMESPACE_BEGIN(CryptoPP)

//! Message Queue
class MessageQueue : public BufferedTransformationWithAutoSignal
{
public:
	MessageQueue(uint32_t nodeSize=256);

	void Put(byte inByte)
		{m_queue.Put(inByte); m_lengths.back()++;}
	void Put(const byte *inString, uint32_t length)
		{m_queue.Put(inString, length); m_lengths.back()+=length;}

	uint32_t MaxRetrievable() const
		{return m_lengths.front();}
	bool AnyRetrievable() const
		{return m_lengths.front() > 0;}

	uint32_t TransferTo(BufferedTransformation &target, uint32_t transferMax=ULONG_SIZE)
		{return Got(m_queue.TransferTo(target, STDMIN(MaxRetrievable(), transferMax)));}
	uint32_t CopyTo(BufferedTransformation &target, uint32_t copyMax=ULONG_SIZE) const
		{return m_queue.CopyTo(target, STDMIN(MaxRetrievable(), copyMax));}

	void MessageEnd(int=-1)
		{m_lengths.push_back(0);}

	uint32_t TotalBytesRetrievable() const
		{return m_queue.MaxRetrievable();}
	uint32_t NumberOfMessages() const
		{return m_lengths.size()-1;}
	bool RetrieveNextMessage();

	uint32_t CopyMessagesTo(BufferedTransformation &target, uint32_t count=UINT_MAX) const;

	void swap(MessageQueue &rhs);

private:
	uint32_t Got(uint32_t length)
		{assert(m_lengths.front() >= length); m_lengths.front() -= length; return length;}

	ByteQueue m_queue;
	std::deque<uint32_t> m_lengths;
};

NAMESPACE_END

NAMESPACE_BEGIN(std)
template<> inline void swap(CryptoPP::MessageQueue &a, CryptoPP::MessageQueue &b)
{
	a.swap(b);
}
NAMESPACE_END

#endif
