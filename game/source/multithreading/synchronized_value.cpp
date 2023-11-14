#include "multithreading/synchronized_value.hpp"

#include "cseries/cseries.hpp"

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

__int64 c_synchronized_int64::increment()
{
	return InterlockedIncrement64(&m_value);
}

__int64 c_synchronized_int64::peek() const
{
	return m_value;
}

void c_synchronized_int64::operator=(__int64 Value)
{
	InterlockedExchange64(&m_value, Value);
}

long c_interlocked_long::peek() const
{
	return m_value;
}

long c_interlocked_long::set(long value)
{
	return InterlockedExchange(&m_value, value);
}

long c_interlocked_long::set_if_equal(long exchange, long comperand)
{
	return InterlockedCompareExchange(&m_value, exchange, comperand);
}

long c_interlocked_long::set_bit(long index, bool enable)
{
	if (enable)
		m_value |= FLAG(index);
	else
		m_value &= ~FLAG(index);

	return m_value;
}

