#include "multithreading/synchronized_value.hpp"

#include <windows.h>

long c_synchronized_long::peek() const
{
	return m_value;
}

long c_interlocked_long::set_if_equal(long ExChange, long Comperand)
{
	return InterlockedCompareExchange(&m_value, ExChange, Comperand);
}
