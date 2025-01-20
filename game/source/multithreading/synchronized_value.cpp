#include "multithreading/synchronized_value.hpp"

#include "cseries/cseries.hpp"

#include <windows.h>

c_synchronized_long::c_synchronized_long(long starting_value) :
	m_value(starting_value)
{
}

c_synchronized_long::c_synchronized_long() :
	m_value(0)
{
}

c_synchronized_long::~c_synchronized_long()
{
}

c_synchronized_long::operator long() const
{
	return m_value;
}

long c_synchronized_long::set(long value)
{
	long result = InterlockedExchange(&m_value, value);
	return result;
}

long c_synchronized_long::set_if_equal(long value, long comperand)
{
	long result = InterlockedCompareExchange(&m_value, value, comperand);
	return result;
}

long c_synchronized_long::peek() const
{
	return m_value;
}

long c_synchronized_long::add(long value)
{
	long result = InterlockedExchangeAdd(&m_value, value);
	return result;
}

long c_synchronized_long::increment()
{
	long result = InterlockedIncrement(&m_value);
	return result;
}

long c_synchronized_long::decrement()
{
	long result = InterlockedDecrement(&m_value);
	return result;
}

long c_synchronized_long::and_(long value)
{
	InterlockedAdd(&m_value, value);
	m_value &= value;
	return m_value;
}

long c_synchronized_long::or_(long value)
{
	m_value |= value;
	return m_value;
}

long c_synchronized_long::xor_(long value)
{
	m_value ^= value;
	return m_value;
}

long c_synchronized_long::set_bit(long index, bool setting)
{
	SET_BIT(m_value, index, setting);
	return m_value;
}

bool c_synchronized_long::test_bit(long index) const
{
	return TEST_BIT(m_value, index);
}

c_synchronized_long& c_synchronized_long::operator+=(long value)
{
	add(value);
	return *this;
}

c_synchronized_long& c_synchronized_long::operator-=(long value)
{
	add(-value);
	return *this;
}

c_synchronized_long& c_synchronized_long::operator&=(long value)
{
	and_(value);
	return *this;
}

c_synchronized_long& c_synchronized_long::operator|=(long value)
{
	or_(value);
	return *this;
}

c_synchronized_long& c_synchronized_long::operator=(c_synchronized_long const& other)
{
	set(other.m_value);
	return *this;
}

c_synchronized_long& c_synchronized_long::operator=(bool value)
{
	set(value);
	return *this;
}

c_synchronized_long& c_synchronized_long::operator=(long value)
{
	InterlockedExchange(&m_value, value);
	return *this;
}

c_synchronized_int64::c_synchronized_int64(__int64 starting_value) :
	m_value(starting_value)
{
}

c_synchronized_int64::c_synchronized_int64() :
	m_value(0)
{
}

c_synchronized_int64::~c_synchronized_int64()
{
}

c_synchronized_int64::operator __int64() const
{
	return peek();
}

__int64 c_synchronized_int64::set(__int64 value)
{
	__int64 result = InterlockedExchange64(&m_value, value);
	return result;
}

__int64 c_synchronized_int64::set_if_equal(__int64 value, __int64 comperand)
{
	__int64 result = InterlockedCompareExchange64(&m_value, value, comperand);
	return result;
}

__int64 c_synchronized_int64::peek() const
{
	return m_value;
}

__int64 c_synchronized_int64::add(__int64 value)
{
	__int64 result = InterlockedExchangeAdd64(&m_value, value);
	return result;
}

__int64 c_synchronized_int64::increment()
{
	__int64 result = InterlockedIncrement64(&m_value);
	return result;
}

__int64 c_synchronized_int64::decrement()
{
	__int64 result = InterlockedDecrement64(&m_value);
	return result;
}

__int64 c_synchronized_int64::and_(__int64 value)
{
	m_value &= value;
	return m_value;
}

__int64 c_synchronized_int64::or_(__int64 value)
{
	m_value |= value;
	return m_value;
}

__int64 c_synchronized_int64::xor_(__int64 value)
{
	m_value ^= value;
	return m_value;
}

c_synchronized_int64& c_synchronized_int64::operator+=(__int64 value)
{
	__int64 result = add(value);
	return *this;
}

c_synchronized_int64& c_synchronized_int64::operator-=(__int64 value)
{
	__int64 result = add(-value);
	return *this;
}

c_synchronized_int64& c_synchronized_int64::operator&=(__int64 value)
{
	__int64 result = and_(value);
	return *this;
}

c_synchronized_int64& c_synchronized_int64::operator|=(__int64 value)
{
	__int64 result = or_(value);
	return *this;
}

c_synchronized_int64& c_synchronized_int64::operator=(c_synchronized_int64 const& other)
{
	__int64 result = set(other.m_value);
	return *this;
}

c_synchronized_int64& c_synchronized_int64::operator=(bool value)
{
	__int64 result = set(value);
	return *this;
}

c_synchronized_int64& c_synchronized_int64::operator=(__int64 value)
{
	__int64 result = set(value);
	return *this;
}

c_interlocked_long::c_interlocked_long(long starting_value) :
	m_value(starting_value)
{
}

c_interlocked_long::c_interlocked_long() :
	m_value(0)
{
}

c_interlocked_long::~c_interlocked_long()
{
}

c_interlocked_long::operator long() const
{
	return peek();
}

long c_interlocked_long::set(long value)
{
	long result = InterlockedExchange(&m_value, value);
	return result;
}

long c_interlocked_long::set_if_equal(long value, long comperand)
{
	long result = InterlockedCompareExchange(&m_value, value, comperand);
	return result;
}

long c_interlocked_long::peek() const
{
	return m_value;
}

long c_interlocked_long::add(long value)
{
	long result = InterlockedExchangeAdd(&m_value, value);
	return result;
}

long c_interlocked_long::increment() volatile
{
	long result = InterlockedIncrement(&m_value);
	return result;
}

long c_interlocked_long::decrement() volatile
{
	long result = InterlockedDecrement(&m_value);
	return result;
}

long c_interlocked_long::and_(long value)
{
	m_value &= value;
	return m_value;
}

long c_interlocked_long::or_(long value)
{
	m_value |= value;
	return m_value;
}

long c_interlocked_long::xor_(long value)
{
	m_value ^= value;
	return m_value;
}

long c_interlocked_long::set_bit(long index, bool setting)
{
	SET_BIT(m_value, index, setting);
	return m_value;
}

bool c_interlocked_long::test_bit(long index) const
{
	bool result = TEST_BIT(m_value, index);
	return result;
}

c_interlocked_long& c_interlocked_long::operator+=(long value)
{
	long result = add(value);
	return *this;
}

c_interlocked_long& c_interlocked_long::operator-=(long value)
{
	long result = add(-value);
	return *this;
}

c_interlocked_long& c_interlocked_long::operator&=(long value)
{
	long result = and_(value);
	return *this;
}

c_interlocked_long& c_interlocked_long::operator|=(long value)
{
	long result = or_(value);
	return *this;
}

c_interlocked_long& c_interlocked_long::operator=(c_interlocked_long const& other)
{
	long result = set(other.m_value);
	return *this;
}

c_interlocked_long& c_interlocked_long::operator=(bool value)
{
	long result = set(value);
	return *this;
}

c_interlocked_long& c_interlocked_long::operator=(long value)
{
	long result = set(value);
	return *this;
}

c_interlocked_int64::c_interlocked_int64(__int64 starting_value) :
	m_value(starting_value)
{
}

c_interlocked_int64::c_interlocked_int64() :
	m_value(0)
{
}

c_interlocked_int64::~c_interlocked_int64()
{
}

c_interlocked_int64::operator __int64() const
{
	return peek();
}

__int64 c_interlocked_int64::set(__int64 value)
{
	__int64 result = InterlockedExchange64(&m_value, value);
	return result;
}

__int64 c_interlocked_int64::set_if_equal(__int64 value, __int64 comperand)
{
	__int64 result = InterlockedCompareExchange64(&m_value, value, comperand);
	return result;
}

__int64 c_interlocked_int64::peek() const
{
	return m_value;
}

__int64 c_interlocked_int64::add(__int64 value)
{
	__int64 result = InterlockedExchangeAdd64(&m_value, value);
	return result;
}

__int64 c_interlocked_int64::increment()
{
	__int64 result = InterlockedIncrement64(&m_value);
	return result;
}

__int64 c_interlocked_int64::decrement()
{
	__int64 result = InterlockedDecrement64(&m_value);
	return result;
}

__int64 c_interlocked_int64::and_(__int64 value)
{
	m_value &= value;
	return m_value;
}

__int64 c_interlocked_int64::or_(__int64 value)
{
	m_value |= value;
	return m_value;
}

__int64 c_interlocked_int64::xor_(__int64 value)
{
	m_value ^= value;
	return m_value;
}

c_interlocked_int64& c_interlocked_int64::operator+=(__int64 value)
{
	__int64 result = add(value);
	return *this;
}

c_interlocked_int64& c_interlocked_int64::operator-=(__int64 value)
{
	__int64 result = add(-value);
	return *this;
}

c_interlocked_int64& c_interlocked_int64::operator&=(__int64 value)
{
	__int64 result = and_(value);
	return *this;
}

c_interlocked_int64& c_interlocked_int64::operator|=(__int64 value)
{
	__int64 result = or_(value);
	return *this;
}

c_interlocked_int64& c_interlocked_int64::operator=(c_interlocked_int64 const& other)
{
	__int64 result = set(other.m_value);
	return *this;
}

c_interlocked_int64& c_interlocked_int64::operator=(bool value)
{
	__int64 result = set(value);
	return *this;
}

c_interlocked_int64& c_interlocked_int64::operator=(__int64 value)
{
	__int64 result = set(value);
	return *this;
}

