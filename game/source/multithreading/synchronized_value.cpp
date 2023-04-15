#include "multithreading/synchronized_value.hpp"

#include <windows.h>

long c_synchronized_long::increment()
{
	return InterlockedIncrement(&m_value);
}

long c_synchronized_long::peek() const
{
	return m_value;
}

void c_synchronized_long::operator=(long Value)
{
	InterlockedExchange(&m_value, Value);
}

long c_interlocked_long::set_if_equal(long ExChange, long Comperand)
{
	return InterlockedCompareExchange(&m_value, ExChange, Comperand);
}

