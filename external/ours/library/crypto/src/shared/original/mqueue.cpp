// mqueue.cpp - written and placed in the public domain by Wei Dai

#include "FirstCrypto.h"
#include "mqueue.h"

NAMESPACE_BEGIN(CryptoPP)

MessageQueue::MessageQueue(uint32_t nodeSize)
	: m_queue(nodeSize), m_lengths(1, 0)
{
}

bool MessageQueue::RetrieveNextMessage()
{
	if (NumberOfMessages() > 0 && !AnyRetrievable())
	{
		m_lengths.pop_front();
		return true;
	}
	else
		return false;
}

uint32_t MessageQueue::CopyMessagesTo(BufferedTransformation &target, uint32_t count) const
{
	ByteQueue::Walker walker(m_queue);
	std::deque<uint32_t>::const_iterator it = m_lengths.begin();
	uint32_t i;
	for (i=0; i<count && it != --m_lengths.end(); ++i, ++it)
	{
		walker.TransferTo(target, *it);
		if (GetAutoSignalPropagation())
			target.MessageEnd(GetAutoSignalPropagation()-1);
	}
	return i;
}

void MessageQueue::swap(MessageQueue &rhs)
{
	m_queue.swap(rhs.m_queue);
	m_lengths.swap(rhs.m_lengths);
}

NAMESPACE_END
