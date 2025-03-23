#pragma once

enum HK_MEMORY_CLASS;
struct hkRigidBody;
struct hkMemory;

struct hkThreadMemory
{
	virtual void* alignedAllocate(int alignment, int nbytes, HK_MEMORY_CLASS cl);
	virtual void alignedDeallocate(void* p);
	virtual void setStackArea(void* buf, int nbytes);
	virtual void releaseCachedMemory(void);
	virtual void destructor(unsigned int);
	virtual void* onStackOverflow(int nbytesin);
	virtual void onStackUnderflow(void* ptr);

	struct Stack
	{
		char* m_current;
		Stack* m_prev;
		char* m_base;
		char* m_end;
	};
	static_assert(sizeof(Stack) == 0x10);

	struct FreeList
	{
		struct FreeElem
		{
			FreeElem* m_next;
		};
		static_assert(sizeof(FreeElem) == 0x4);

		FreeElem* m_head;
		int m_numElem;
	};
	static_assert(sizeof(FreeList) == 0x8);

	unsigned char __data4[0xC];
	hkMemory* m_memory;
	int m_referenceCount;
	unsigned char __data18[0x8];
	Stack m_stack;
	int m_stackSize;
	int m_maxNumElemsOnFreeList;
	FreeList m_free_list[17];
	int m_row_to_size_lut[17];
	char m_small_size_to_row_lut[512 + 1];
	int m_large_size_to_row_lut[10];
};
static_assert(sizeof(hkThreadMemory) == 0x330);
