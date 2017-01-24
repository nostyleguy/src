// specification file for an unlimited queue for storing bytes

#ifndef CRYPTOPP_QUEUE_H
#define CRYPTOPP_QUEUE_H

#include "cryptlib.h"
#include <algorithm>

#define ULONG_SIZE sizeof(uint32_t)

NAMESPACE_BEGIN(CryptoPP)

/** The queue is implemented as a linked list of arrays, but you don't need to
    know about that.  So just ignore this next line. :) */
class ByteQueueNode;

//! Byte Queue
class ByteQueue : public BufferedTransformation
{
public:
	ByteQueue(uint32_t m_nodeSize=256);
	ByteQueue(const ByteQueue &copy);
	~ByteQueue();

	uint32_t MaxRetrievable() const
		{return CurrentSize();}
	bool AnyRetrievable() const
		{return !IsEmpty();}

	void Put(byte inByte);
	void Put(const byte *inString, uint32_t length);

	uint32_t Get(byte &outByte);
	uint32_t Get(byte *outString, uint32_t getMax);

	uint32_t Peek(byte &outByte) const;
	uint32_t Peek(byte *outString, uint32_t peekMax) const;

	uint32_t Skip(uint32_t skipMax=ULONG_SIZE);
	uint32_t TransferTo(BufferedTransformation &target, uint32_t transferMax=ULONG_SIZE);
	uint32_t CopyTo(BufferedTransformation &target, uint32_t copyMax=ULONG_SIZE) const;

	// these member functions are not inherited
	uint32_t CurrentSize() const;
	bool IsEmpty() const;

	void Clear();

	void Unget(byte inByte);
	void Unget(const byte *inString, uint32_t length);

	const byte * Spy(uint32_t &contiguousSize) const;

	byte * MakeNewSpace(uint32_t &contiguousSize);
	void OccupyNewSpace(uint32_t size);

	void LazyPut(const byte *inString, uint32_t size);
	void FinalizeLazyPut();

	ByteQueue & operator=(const ByteQueue &rhs);
	bool operator==(const ByteQueue &rhs) const;
	byte operator[](uint32_t i) const;
	void swap(ByteQueue &rhs);

	class Walker : public BufferedTransformation
	{
	public:
		Walker(const ByteQueue &queue)
			: m_queue(queue), m_node(queue.m_head), m_position(0), m_offset(0)
			, m_lazyString(queue.m_lazyString), m_lazyLength(queue.m_lazyLength) {}

		uint32_t MaxRetrievable() const
			{return m_queue.CurrentSize() - m_position;}

		void Put(byte inByte) {}
		void Put(const byte *inString, uint32_t length) {}

		uint32_t Get(byte &outByte);
		uint32_t Get(byte *outString, uint32_t getMax);

		uint32_t Peek(byte &outByte) const;
		uint32_t Peek(byte *outString, uint32_t peekMax) const;

		uint32_t Skip(uint32_t skipMax=ULONG_SIZE);
		uint32_t TransferTo(BufferedTransformation &target, uint32_t transferMax=ULONG_SIZE);
		uint32_t CopyTo(BufferedTransformation &target, uint32_t copyMax=ULONG_SIZE) const;

	private:
		const ByteQueue &m_queue;
		const ByteQueueNode *m_node;
		uint32_t m_position, m_offset;
		const byte *m_lazyString;
		uint32_t m_lazyLength;
	};

	friend class Walker;

private:
	void CleanupUsedNodes();
	void CopyFrom(const ByteQueue &copy);
	void Destroy();

	uint32_t m_nodeSize;
	ByteQueueNode *m_head, *m_tail;
	const byte *m_lazyString;
	uint32_t m_lazyLength;
};

//! use this to make sure LazyPut is finalized in event of exception
class LazyPutter
{
public:
	LazyPutter(ByteQueue &bq, const byte *inString, uint32_t size)
		: m_bq(bq) {bq.LazyPut(inString, size);}
	~LazyPutter()
		{try {m_bq.FinalizeLazyPut();} catch(...) {}}
private:
	ByteQueue &m_bq;
};

NAMESPACE_END

NAMESPACE_BEGIN(std)
template<> inline void swap(CryptoPP::ByteQueue &a, CryptoPP::ByteQueue &b)
{
	a.swap(b);
}
NAMESPACE_END

#endif
