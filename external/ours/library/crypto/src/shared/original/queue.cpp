// queue.cpp - written and placed in the public domain by Wei Dai

#include "FirstCrypto.h"
#include "queue.h"
#include "filters.h"

NAMESPACE_BEGIN(CryptoPP)

// this class for use by ByteQueue only
class ByteQueueNode
{
public:
	ByteQueueNode(uint32_t maxSize)
		: buf(maxSize)
	{
		m_head = m_tail = 0;
		next = 0;
	}

	inline uint32_t MaxSize() const { return buf.size; }

	inline uint32_t CurrentSize() const
	{
		return m_tail - m_head;
	}

	inline bool UsedUp() const
	{
		return (m_head == MaxSize());
	}

	inline void Clear()
	{
		m_head = m_tail = 0;
	}

	inline uint32_t Put(byte inByte)
	{
		if (MaxSize() == m_tail)
			return 0;

		buf[m_tail++] = inByte;
		return 1;
	}

	inline uint32_t Put(const byte *inString, uint32_t length)
	{
		uint32_t l = STDMIN(length, MaxSize() - m_tail);
		memcpy(buf + m_tail, inString, l);
		m_tail += l;
		return l;
	}

	inline uint32_t Peek(byte &outByte) const
	{
		if (m_tail == m_head)
			return 0;

		outByte = buf[m_head];
		return 1;
	}

	inline uint32_t Peek(byte *target, uint32_t copyMax) const
	{
		uint32_t len = STDMIN(copyMax, m_tail - m_head);
		memcpy(target, buf + m_head, len);
		return len;
	}

	inline uint32_t CopyTo(BufferedTransformation &target) const
	{
		uint32_t len = m_tail - m_head;
		target.Put(buf + m_head, len);
		return len;
	}

	inline uint32_t CopyTo(BufferedTransformation &target, uint32_t copyMax) const
	{
		uint32_t len = STDMIN(copyMax, m_tail - m_head);
		target.Put(buf + m_head, len);
		return len;
	}

	inline uint32_t Get(byte &outByte)
	{
		uint32_t len = Peek(outByte);
		m_head += len;
		return len;
	}

	inline uint32_t Get(byte *outString, uint32_t getMax)
	{
		uint32_t len = Peek(outString, getMax);
		m_head += len;
		return len;
	}

	inline uint32_t TransferTo(BufferedTransformation &target)
	{
		uint32_t len = CopyTo(target);
		m_head += len;
		return len;
	}

	inline uint32_t TransferTo(BufferedTransformation &target, uint32_t transferMax)
	{
		uint32_t len = CopyTo(target, transferMax);
		m_head += len;
		return len;
	}

	inline uint32_t Skip(uint32_t skipMax)
	{
		uint32_t len = STDMIN(skipMax, m_tail - m_head);
		m_head += len;
		return len;
	}

	inline byte operator[](uint32_t i) const
	{
		return buf[m_head + i];
	}

	ByteQueueNode *next;

	SecByteBlock buf;
	uint32_t m_head, m_tail;
};

// ********************************************************

ByteQueue::ByteQueue(uint32_t m_nodeSize)
	: m_nodeSize(0), m_lazyLength(0), m_lazyString(nullptr)
{
	m_head = m_tail = new ByteQueueNode(m_nodeSize);
}

ByteQueue::ByteQueue(const ByteQueue &copy)
{
	CopyFrom(copy);
}

void ByteQueue::CopyFrom(const ByteQueue &copy)
{
	m_lazyLength = 0;
	m_nodeSize = copy.m_nodeSize;
	m_head = m_tail = new ByteQueueNode(*copy.m_head);

	for (ByteQueueNode *current = copy.m_head->next; current; current = current->next)
	{
		m_tail->next = new ByteQueueNode(*current);
		m_tail = m_tail->next;
	}

	m_tail->next = nullptr;

	Put(copy.m_lazyString, copy.m_lazyLength);
}

ByteQueue::~ByteQueue()
{
	Destroy();
}

void ByteQueue::Destroy()
{
	ByteQueueNode *next;

	for (ByteQueueNode *current = m_head; current; current = next)
	{
		next = current->next;
		delete current;
	}
}

uint32_t ByteQueue::CurrentSize() const
{
	uint32_t size = 0;

	for (ByteQueueNode *current = m_head; current; current = current->next)
		size += current->CurrentSize();

	return size + m_lazyLength;
}

bool ByteQueue::IsEmpty() const
{
	return m_head == m_tail && m_head->CurrentSize() == 0 && m_lazyLength == 0;
}

void ByteQueue::Clear()
{
	Destroy();
	m_head = m_tail = new ByteQueueNode(m_nodeSize);
	m_lazyLength = 0;
}

void ByteQueue::Put(byte inByte)
{
	if (m_lazyLength > 0)
		FinalizeLazyPut();

	if (!m_tail->Put(inByte))
	{
		m_tail->next = new ByteQueueNode(m_nodeSize);
		m_tail = m_tail->next;
		m_tail->Put(inByte);
	}
}

void ByteQueue::Put(const byte *inString, uint32_t length)
{
	if (m_lazyLength > 0)
		FinalizeLazyPut();

	uint32_t len;
	while ((len = m_tail->Put(inString, length)) < length)
	{
		m_tail->next = new ByteQueueNode(m_nodeSize);
		m_tail = m_tail->next;
		inString += len;
		length -= len;
	}
}

void ByteQueue::CleanupUsedNodes()
{
	while (m_head != m_tail && m_head->UsedUp())
	{
		ByteQueueNode *temp = m_head;
		m_head = m_head->next;
		delete temp;
	}

	if (m_head->CurrentSize() == 0)
		m_head->Clear();
}

void ByteQueue::LazyPut(const byte *inString, uint32_t size)
{
	if (m_lazyLength > 0)
		FinalizeLazyPut();
	m_lazyString = inString;
	m_lazyLength = size;
}

void ByteQueue::FinalizeLazyPut()
{
	uint32_t len = m_lazyLength;
	m_lazyLength = 0;
	Put(m_lazyString, len);
}

uint32_t ByteQueue::Get(byte &outByte)
{
	if (m_head->Get(outByte))
	{
		if (m_head->UsedUp())
			CleanupUsedNodes();
		return 1;
	}
	else if (m_lazyLength > 0)
	{
		outByte = *m_lazyString++;
		m_lazyLength--;
		return 1;
	}
	else
		return 0;
}

uint32_t ByteQueue::Get(byte *outString, uint32_t getMax)
{
	ArraySink sink(outString, getMax);
	return TransferTo(sink, getMax);
}

uint32_t ByteQueue::Peek(byte &outByte) const
{
	if (m_head->Peek(outByte))
		return 1;
	else if (m_lazyLength > 0)
	{
		outByte = *m_lazyString;
		return 1;
	}
	else
		return 0;
}

uint32_t ByteQueue::Peek(byte *outString, uint32_t peekMax) const
{
	ArraySink sink(outString, peekMax);
	return CopyTo(sink, peekMax);
}

uint32_t ByteQueue::Skip(uint32_t skipMax)
{
	return TransferTo(g_bitBucket, skipMax);
}

uint32_t ByteQueue::TransferTo(BufferedTransformation &target, uint32_t transferMax)
{
	uint32_t bytesLeft = transferMax;
	for (ByteQueueNode *current = m_head; bytesLeft && current; current = current->next)
		bytesLeft -= current->TransferTo(target, bytesLeft);
	CleanupUsedNodes();

	uint32_t len = (uint32_t)STDMIN(bytesLeft, (uint32_t)m_lazyLength);
	if (len)
	{
		target.Put(m_lazyString, len);
		m_lazyString += len;
		m_lazyLength -= len;
		bytesLeft -= len;
	}
	return transferMax - bytesLeft;
}

uint32_t ByteQueue::CopyTo(BufferedTransformation &target, uint32_t copyMax) const
{
	uint32_t bytesLeft = copyMax;
	for (ByteQueueNode *current = m_head; bytesLeft && current; current = current->next)
		bytesLeft -= current->CopyTo(target, bytesLeft);
	if (bytesLeft && m_lazyLength)
	{
		uint32_t len = (uint32_t)STDMIN(bytesLeft, (uint32_t)m_lazyLength);
		target.Put(m_lazyString, len);
		bytesLeft -= len;
	}
	return copyMax - bytesLeft;
}

void ByteQueue::Unget(byte inByte)
{
	Unget(&inByte, 1);
}

void ByteQueue::Unget(const byte *inString, uint32_t length)
{
	ByteQueueNode *newHead = new ByteQueueNode(length);
	newHead->next = m_head;
	m_head = newHead;
	m_head->Put(inString, length);
}
/*
byte * ByteQueue::Spy(uint32_t &contiguousSize)
{
	contiguousSize = m_head->m_tail - m_head->m_head;
	return m_head->buf + m_head->m_head;
}
*/
const byte * ByteQueue::Spy(uint32_t &contiguousSize) const
{
	contiguousSize = m_head->m_tail - m_head->m_head;
	if (contiguousSize == 0 && m_lazyLength > 0)
	{
		contiguousSize = m_lazyLength;
		return m_lazyString;
	}
	else
		return m_head->buf + m_head->m_head;
}

byte * ByteQueue::MakeNewSpace(uint32_t &contiguousSize)
{
	if (m_lazyLength > 0)
		FinalizeLazyPut();

	if (m_tail->m_tail == m_tail->MaxSize())
	{
		m_tail->next = new ByteQueueNode(m_nodeSize);
		m_tail = m_tail->next;
	}

	contiguousSize = m_tail->MaxSize() - m_tail->m_tail;
	return m_tail->buf + m_tail->m_tail;
}

void ByteQueue::OccupyNewSpace(uint32_t size)
{
	m_tail->m_tail += size;
	assert(m_tail->m_tail <= m_tail->MaxSize());
}

ByteQueue & ByteQueue::operator=(const ByteQueue &rhs)
{
	Destroy();
	CopyFrom(rhs);
	return *this;
}

bool ByteQueue::operator==(const ByteQueue &rhs) const
{
	const uint32_t currentSize = CurrentSize();

	if (currentSize != rhs.CurrentSize())
		return false;

	Walker walker1(*this), walker2(rhs);
	byte b1, b2;

	while (walker1.Get(b1) && walker2.Get(b2))
		if (b1 != b2)
			return false;

	return true;
}

byte ByteQueue::operator[](uint32_t i) const
{
	for (ByteQueueNode *current = m_head; current; current = current->next)
	{
		if (i < current->CurrentSize())
			return (*current)[i];

		i -= current->CurrentSize();
	}

	assert(i < m_lazyLength);
	return m_lazyString[i];
}

void ByteQueue::swap(ByteQueue &rhs)
{
	std::swap(m_nodeSize, rhs.m_nodeSize);
	std::swap(m_head, rhs.m_head);
	std::swap(m_tail, rhs.m_tail);
	std::swap(m_lazyString, rhs.m_lazyString);
	std::swap(m_lazyLength, rhs.m_lazyLength);
}

// ********************************************************

uint32_t ByteQueue::Walker::Get(byte &outByte)
{
	ArraySink sink(&outByte, 1);
	return TransferTo(sink, 1);
}

uint32_t ByteQueue::Walker::Get(byte *outString, uint32_t getMax)
{
	ArraySink sink(outString, getMax);
	return TransferTo(sink, getMax);
}

uint32_t ByteQueue::Walker::Peek(byte &outByte) const
{
	ArraySink sink(&outByte, 1);
	return CopyTo(sink, 1);
}

uint32_t ByteQueue::Walker::Peek(byte *outString, uint32_t peekMax) const
{
	ArraySink sink(outString, peekMax);
	return CopyTo(sink, peekMax);
}

uint32_t ByteQueue::Walker::TransferTo(BufferedTransformation &target, uint32_t transferMax)
{
	uint32_t bytesLeft = transferMax;
	while (m_node)
	{
		uint32_t len = STDMIN(bytesLeft, (uint32_t)m_node->CurrentSize() - m_offset);
		target.Put(m_node->buf + m_node->m_head + m_offset, len);
		m_position += len;
		bytesLeft -= len;

		if (!bytesLeft)
		{
			m_offset += len;
			break;
		}

		m_node = m_node->next;
		m_offset = 0;
	}

	uint32_t len = (uint32_t)STDMIN(bytesLeft, (uint32_t)m_lazyLength);
	if (len)
	{
		target.Put(m_lazyString, len);
		m_lazyString += len;
		m_lazyLength -= len;
		bytesLeft -= len;
	}
	return transferMax - bytesLeft;
}

uint32_t ByteQueue::Walker::Skip(uint32_t skipMax)
{
	return TransferTo(g_bitBucket, skipMax);
}

uint32_t ByteQueue::Walker::CopyTo(BufferedTransformation &target, uint32_t copyMax) const
{
	return Walker(*this).TransferTo(target, copyMax);
}

NAMESPACE_END