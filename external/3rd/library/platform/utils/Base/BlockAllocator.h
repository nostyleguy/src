#if !defined (BLOCKALLOCATOR_H_)
#define BLOCKALLOCATOR_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#if __x86_64__
typedef long unsigned int baType;
#else
typedef uint baType;
#endif

#ifdef EXTERNAL_DISTRO
namespace NAMESPACE 
{

#endif
namespace Base 
{
	class BlockAllocator
	{
	public:
		BlockAllocator();
		~BlockAllocator();
		void *getBlock(baType accum);
		void returnBlock(baType *handle);

	private:
		uintptr_t *m_blocks[31];
	};
};
#ifdef EXTERNAL_DISTRO
};
#endif

#endif
