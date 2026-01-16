#pragma once

#include "hkBase.hpp"

struct hkMonitorStream
{
	hkPadSpu<char*> m_start;
	hkPadSpu<char*> m_end;
	hkPadSpu<char*> m_capacity;
	hkPadSpu<char*> m_capacityMinus16;
	hkBool m_isBufferAllocatedOnTheHeap;
};
COMPILE_ASSERT(sizeof(hkMonitorStream) == 0x14);

