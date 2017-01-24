// filters.cpp - written and placed in the public domain by Wei Dai

#include "FirstCrypto.h"
#include "filters.h"
#include "mqueue.h"
#include <memory>

NAMESPACE_BEGIN(CryptoPP)

BitBucket g_bitBucket;

Filter::Filter(BufferedTransformation *outQ)
	: m_outQueue(outQ ? outQ : new MessageQueue)
{
}

void Filter::Detach(BufferedTransformation *newOut)
{
	m_outQueue.reset(newOut ? newOut : new MessageQueue);
	NotifyAttachmentChange();
}

void Filter::Insert(Filter *filter)
{
	filter->m_outQueue.reset(m_outQueue.release());
	m_outQueue.reset(filter);
	NotifyAttachmentChange();
}

// *************************************************************

FilterWithBufferedInput::BlockQueue::BlockQueue(uint32_t blockSize, uint32_t maxBlocks)
	: m_buffer(blockSize * maxBlocks)
{
	ResetQueue(blockSize, maxBlocks);
}

void FilterWithBufferedInput::BlockQueue::ResetQueue(uint32_t blockSize, uint32_t maxBlocks)
{
	m_buffer.Resize(blockSize * maxBlocks);
	m_blockSize = blockSize;
	m_maxBlocks = maxBlocks;
	m_size = 0;
	m_begin = m_buffer;
}

const byte *FilterWithBufferedInput::BlockQueue::GetBlock()
{
	if (m_size >= m_blockSize)
	{
		const byte *ptr = m_begin;
		if ((m_begin+=m_blockSize) == m_buffer.End())
			m_begin = m_buffer;
		m_size -= m_blockSize;
		return ptr;
	}
	else
		return nullptr;
}

const byte *FilterWithBufferedInput::BlockQueue::GetContigousBlocks(uint32_t &numberOfBlocks)
{
	numberOfBlocks = STDMIN(numberOfBlocks, STDMIN((uint32_t)(m_buffer.End()-m_begin), m_size)/m_blockSize);
	const byte *ptr = m_begin;
	if ((m_begin+=m_blockSize*numberOfBlocks) == m_buffer.End())
		m_begin = m_buffer;
	m_size -= m_blockSize*numberOfBlocks;
	return ptr;
}

uint32_t FilterWithBufferedInput::BlockQueue::GetAll(byte *outString)
{
	uint32_t size = m_size;
	uint32_t numberOfBlocks = m_maxBlocks;
	const byte *ptr = GetContigousBlocks(numberOfBlocks);
	memcpy(outString, ptr, numberOfBlocks*m_blockSize);
	memcpy(outString+numberOfBlocks*m_blockSize, m_begin, m_size);
	m_size = 0;
	return size;
}

void FilterWithBufferedInput::BlockQueue::Put(const byte *inString, uint32_t length)
{
	assert(m_size + length <= m_buffer.size);
	byte *end = (m_size < (unsigned)(m_buffer+m_buffer.size-m_begin)) ? m_begin + m_size : m_begin + m_size - m_buffer.size;
	uint32_t len = STDMIN(length, (uint32_t)(m_buffer+m_buffer.size-end));
	memcpy(end, inString, len);
	if (len < length)
		memcpy(m_buffer, inString+len, length-len);
	m_size += length;
}

FilterWithBufferedInput::FilterWithBufferedInput(uint32_t firstSize, uint32_t blockSize, uint32_t lastSize, BufferedTransformation *outQ)
	: Filter(outQ), m_firstSize(firstSize), m_blockSize(blockSize), m_lastSize(lastSize)
	, m_firstInputDone(false)
	, m_queue(1, m_firstSize)
{
}

void FilterWithBufferedInput::Put(byte inByte)
{
	Put(&inByte, 1);
}

void FilterWithBufferedInput::Put(const byte *inString, uint32_t length)
{
	if (length == 0)
		return;

	uint32_t newLength = m_queue.CurrentSize() + length;

	if (!m_firstInputDone && newLength >= m_firstSize)
	{
		uint32_t len = m_firstSize - m_queue.CurrentSize();
		m_queue.Put(inString, len);
		FirstPut(m_queue.GetContigousBlocks(m_firstSize));
		assert(m_queue.CurrentSize() == 0);
		m_queue.ResetQueue(m_blockSize, (2*m_blockSize+m_lastSize-2)/m_blockSize);

		inString += len;
		newLength -= m_firstSize;
		m_firstInputDone = true;
	}

	if (m_firstInputDone)
	{
		if (m_blockSize == 1)
		{
			while (newLength > m_lastSize && m_queue.CurrentSize() > 0)
			{
				uint32_t len = newLength - m_lastSize;
				const byte *ptr = m_queue.GetContigousBlocks(len);
				NextPut(ptr, len);
				newLength -= len;
			}

			if (newLength > m_lastSize)
			{
				uint32_t len = newLength - m_lastSize;
				NextPut(inString, len);
				inString += len;
				newLength -= len;
			}
		}
		else
		{
			while (newLength >= m_blockSize + m_lastSize && m_queue.CurrentSize() >= m_blockSize)
			{
				NextPut(m_queue.GetBlock(), m_blockSize);
				newLength -= m_blockSize;
			}

			if (newLength >= m_blockSize + m_lastSize && m_queue.CurrentSize() > 0)
			{
				assert(m_queue.CurrentSize() < m_blockSize);
				uint32_t len = m_blockSize - m_queue.CurrentSize();
				m_queue.Put(inString, len);
				inString += len;
				NextPut(m_queue.GetBlock(), m_blockSize);
				newLength -= m_blockSize;
			}

			while (newLength >= m_blockSize + m_lastSize)
			{
				NextPut(inString, m_blockSize);
				inString += m_blockSize;
				newLength -= m_blockSize;
			}
		}
	}

	m_queue.Put(inString, newLength - m_queue.CurrentSize());
}

void FilterWithBufferedInput::MessageEnd(int propagation)
{
	if (!m_firstInputDone && m_firstSize==0)
		FirstPut(nullptr);

	SecByteBlock temp(m_queue.CurrentSize());
	m_queue.GetAll(temp);
	LastPut(temp, temp.size);

	m_firstInputDone = false;
	m_queue.ResetQueue(1, m_firstSize);

	Filter::MessageEnd(propagation);
}

void FilterWithBufferedInput::ForceNextPut()
{
	if (m_firstInputDone && m_queue.CurrentSize() >= m_blockSize)
		NextPut(m_queue.GetBlock(), m_blockSize);
}

// *************************************************************



// *************************************************************

ProxyFilter::ProxyFilter(Filter *filter, uint32_t firstSize, uint32_t lastSize, BufferedTransformation *outQ)
	: FilterWithBufferedInput(firstSize, 1, lastSize, outQ), m_filter(filter), m_proxy(nullptr)
{
	if (m_filter.get())
		m_filter->Attach(m_proxy = new OutputProxy(*this, false));
}

void ProxyFilter::Flush(bool completeFlush, int propagation)
{
	if (m_filter.get())
	{
		bool passSignal = m_proxy->GetPassSignal();
		m_proxy->SetPassSignal(false);
		m_filter->Flush(completeFlush, -1);
		m_proxy->SetPassSignal(passSignal);
	}
	Filter::Flush(completeFlush, propagation);
}

void ProxyFilter::SetFilter(Filter *filter)
{
	bool passSignal = m_proxy ? m_proxy->GetPassSignal() : false;
	m_filter.reset(filter);
	if (filter)
	{
		std::unique_ptr<OutputProxy> temp(m_proxy = new OutputProxy(*this, passSignal));
		m_filter->TransferAllTo(*m_proxy);
		m_filter->Attach(temp.release());
	}
	else
		m_proxy=nullptr;
}

void ProxyFilter::NextPut(const byte *s, uint32_t len) 
{
	if (m_filter.get())
		m_filter->Put(s, len);
}

// *************************************************************

void StreamCipherFilter::Put(const byte *inString, uint32_t length)
{
	SecByteBlock temp(length);
	cipher.ProcessString(temp, inString, length);
	AttachedTransformation()->Put(temp, length);
}

void HashFilter::Put(byte inByte)
{
	m_hashModule.Update(&inByte, 1);
	if (m_putMessage)
		AttachedTransformation()->Put(inByte);
}

void HashFilter::Put(const byte *inString, uint32_t length)
{
	m_hashModule.Update(inString, length);
	if (m_putMessage)
		AttachedTransformation()->Put(inString, length);
}

void HashFilter::MessageEnd(int propagation)
{
	SecByteBlock buf(m_hashModule.DigestSize());
	m_hashModule.Final(buf);
	AttachedTransformation()->Put(buf, buf.size);
	Filter::MessageEnd(propagation);
}

// *************************************************************

HashVerifier::HashVerifier(HashModule &hm, BufferedTransformation *outQueue, word32 flags)
	: FilterWithBufferedInput(flags & HASH_AT_BEGIN ? hm.DigestSize() : 0, 1, flags & HASH_AT_BEGIN ? 0 : hm.DigestSize(), outQueue)
	, m_hashModule(hm), m_flags(flags)
	, m_expectedHash(flags & HASH_AT_BEGIN ? hm.DigestSize() : 0), m_verified(false)
{
}

void HashVerifier::FirstPut(const byte *inString)
{
	if (m_flags & HASH_AT_BEGIN)
	{
		memcpy(m_expectedHash, inString, m_expectedHash.size);
		if (m_flags & PUT_HASH)
			AttachedTransformation()->Put(inString, m_expectedHash.size);
	}
}

void HashVerifier::NextPut(const byte *inString, uint32_t length)
{
	m_hashModule.Update(inString, length);
	if (m_flags & PUT_MESSAGE)
		AttachedTransformation()->Put(inString, length);
}

void HashVerifier::LastPut(const byte *inString, uint32_t length)
{
	if (m_flags & HASH_AT_BEGIN)
	{
		assert(length == 0);
		m_verified = m_hashModule.Verify(m_expectedHash);
	}
	else
	{
		m_verified = (length==m_hashModule.DigestSize() && m_hashModule.Verify(inString));
		if (m_flags & PUT_HASH)
			AttachedTransformation()->Put(inString, length);
	}

	if (m_flags & PUT_RESULT)
		AttachedTransformation()->Put(m_verified);

	if ((m_flags & THROW_EXCEPTION) && !m_verified)
		throw HashVerificationFailed();
}

// *************************************************************

void SignerFilter::MessageEnd(int propagation)
{
	SecByteBlock buf(m_signer.SignatureLength());
	m_signer.Sign(m_rng, m_messageAccumulator.release(), buf);
	AttachedTransformation()->Put(buf, buf.size);
	Filter::MessageEnd(propagation);
	m_messageAccumulator.reset(m_signer.NewMessageAccumulator());
}

void VerifierFilter::PutSignature(const byte *sig)
{
	memcpy(m_signature.ptr, sig, m_signature.size);
}

void VerifierFilter::MessageEnd(int propagation)
{
	AttachedTransformation()->Put((byte)m_verifier.Verify(m_messageAccumulator.release(), m_signature));
	Filter::MessageEnd(propagation);
	m_messageAccumulator.reset(m_verifier.NewMessageAccumulator());
}

// *************************************************************

void Source::PumpAll()
{
	while (PumpMessages()) {}
	while (Pump()) {}
}

StringSource::StringSource(const char *string, bool pumpAll, BufferedTransformation *outQueue)
	: Source(outQueue), m_store(string)
{
	if (pumpAll)
		PumpAll();
}

StringSource::StringSource(const byte *string, uint32_t length, bool pumpAll, BufferedTransformation *outQueue)
	: Source(outQueue), m_store(string, length)
{
	if (pumpAll)
		PumpAll();
}

bool Store::GetNextMessage()
{
	if (!m_messageEnd && !AnyRetrievable())
	{
		m_messageEnd=true;
		return true;
	}
	else
		return false;
}

uint32_t Store::CopyMessagesTo(BufferedTransformation &target, uint32_t count) const
{
	if (m_messageEnd || count == 0)
		return 0;
	else
	{
		CopyTo(target);
		if (GetAutoSignalPropagation())
			target.MessageEnd(GetAutoSignalPropagation()-1);
		return 1;
	}
}

uint32_t StringStore::TransferTo(BufferedTransformation &target, uint32_t transferMax)
{
	uint32_t result = CopyTo(target, transferMax);
	m_count += result;
	return result;
}

uint32_t StringStore::CopyTo(BufferedTransformation &target, uint32_t copyMax) const
{
	uint32_t len = (uint32_t)STDMIN((uint32_t)(m_length-m_count), copyMax);
	target.Put(m_store+m_count, len);
	return len;
}

uint32_t RandomNumberStore::CopyTo(BufferedTransformation &target, uint32_t copyMax) const
{
	uint32_t len = (uint32_t)STDMIN((uint32_t)(m_length-m_count), copyMax);
	for (uint32_t i=0; i<len; i++)
		target.Put(m_rng.GenerateByte());
	return len;
}

uint32_t RandomNumberStore::TransferTo(BufferedTransformation &target, uint32_t transferMax)
{
	uint32_t len = RandomNumberStore::CopyTo(target, transferMax);
	m_count += len;
	return len;
}

RandomNumberSource::RandomNumberSource(RandomNumberGenerator &rng, uint32_t length, bool pumpAll, BufferedTransformation *outQueue)
	: Source(outQueue), m_store(rng, length)
{
	if (pumpAll)
		PumpAll();
}

NAMESPACE_END
