#include "multithreading/synchronized_value.hpp"

#include "cseries/cseries.hpp"

#include <windows.h>

c_synchronized_long::c_synchronized_long(int32 starting_value) :
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

c_synchronized_long::operator int32() const
{
	return m_value;
}

int32 c_synchronized_long::set(int32 value)
{
	int32 result = InterlockedExchange(&m_value, value);
	return result;
}

int32 c_synchronized_long::set_if_equal(int32 value, int32 comperand)
{
	int32 result = InterlockedCompareExchange(&m_value, value, comperand);
	return result;
}

int32 c_synchronized_long::peek() const
{
	return m_value;
}

int32 c_synchronized_long::add(int32 value)
{
	int32 result = InterlockedExchangeAdd(&m_value, value);
	return result;
}

int32 c_synchronized_long::increment()
{
	int32 result = InterlockedIncrement(&m_value);
	return result;
}

int32 c_synchronized_long::decrement()
{
	int32 result = InterlockedDecrement(&m_value);
	return result;
}

int32 c_synchronized_long::and_(int32 value)
{
	InterlockedAdd(&m_value, value);
	m_value &= value;
	return m_value;
}

int32 c_synchronized_long::or_(int32 value)
{
	m_value |= value;
	return m_value;
}

int32 c_synchronized_long::xor_(int32 value)
{
	m_value ^= value;
	return m_value;
}

int32 c_synchronized_long::set_bit(int32 index, bool setting)
{
	SET_BIT(m_value, index, setting);
	return m_value;
}

bool c_synchronized_long::test_bit(int32 index) const
{
	return TEST_BIT(m_value, index);
}

c_synchronized_long& c_synchronized_long::operator+=(int32 value)
{
	add(value);
	return *this;
}

c_synchronized_long& c_synchronized_long::operator-=(int32 value)
{
	add(-value);
	return *this;
}

c_synchronized_long& c_synchronized_long::operator&=(int32 value)
{
	and_(value);
	return *this;
}

c_synchronized_long& c_synchronized_long::operator|=(int32 value)
{
	or_(value);
	return *this;
}

c_synchronized_long& c_synchronized_long::operator=(const c_synchronized_long& other)
{
	set(other.m_value);
	return *this;
}

c_synchronized_long& c_synchronized_long::operator=(bool value)
{
	set(value);
	return *this;
}

c_synchronized_long& c_synchronized_long::operator=(int32 value)
{
	InterlockedExchange(&m_value, value);
	return *this;
}

c_synchronized_int64::c_synchronized_int64(int64 starting_value) :
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

c_synchronized_int64::operator int64() const
{
	return peek();
}

int64 c_synchronized_int64::set(int64 value)
{
	int64 result = InterlockedExchange64(&m_value, value);
	return result;
}

int64 c_synchronized_int64::set_if_equal(int64 value, int64 comperand)
{
	int64 result = InterlockedCompareExchange64(&m_value, value, comperand);
	return result;
}

int64 c_synchronized_int64::peek() const
{
	return m_value;
}

int64 c_synchronized_int64::add(int64 value)
{
	int64 result = InterlockedExchangeAdd64(&m_value, value);
	return result;
}

int64 c_synchronized_int64::increment()
{
	int64 result = InterlockedIncrement64(&m_value);
	return result;
}

int64 c_synchronized_int64::decrement()
{
	int64 result = InterlockedDecrement64(&m_value);
	return result;
}

int64 c_synchronized_int64::and_(int64 value)
{
	m_value &= value;
	return m_value;
}

int64 c_synchronized_int64::or_(int64 value)
{
	m_value |= value;
	return m_value;
}

int64 c_synchronized_int64::xor_(int64 value)
{
	m_value ^= value;
	return m_value;
}

c_synchronized_int64& c_synchronized_int64::operator+=(int64 value)
{
	int64 result = add(value);
	return *this;
}

c_synchronized_int64& c_synchronized_int64::operator-=(int64 value)
{
	int64 result = add(-value);
	return *this;
}

c_synchronized_int64& c_synchronized_int64::operator&=(int64 value)
{
	int64 result = and_(value);
	return *this;
}

c_synchronized_int64& c_synchronized_int64::operator|=(int64 value)
{
	int64 result = or_(value);
	return *this;
}

c_synchronized_int64& c_synchronized_int64::operator=(const c_synchronized_int64& other)
{
	int64 result = set(other.m_value);
	return *this;
}

c_synchronized_int64& c_synchronized_int64::operator=(bool value)
{
	int64 result = set(value);
	return *this;
}

c_synchronized_int64& c_synchronized_int64::operator=(int64 value)
{
	int64 result = set(value);
	return *this;
}

c_interlocked_long::c_interlocked_long(int32 starting_value) :
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

c_interlocked_long::operator int32() const
{
	return peek();
}

int32 c_interlocked_long::set(int32 value)
{
	int32 result = InterlockedExchange(&m_value, value);
	return result;
}

int32 c_interlocked_long::set_if_equal(int32 value, int32 comperand)
{
	int32 result = InterlockedCompareExchange(&m_value, value, comperand);
	return result;
}

int32 c_interlocked_long::peek() const
{
	return m_value;
}

int32 c_interlocked_long::add(int32 value)
{
	int32 result = InterlockedExchangeAdd(&m_value, value);
	return result;
}

int32 c_interlocked_long::increment() volatile
{
	int32 result = InterlockedIncrement(&m_value);
	return result;
}

int32 c_interlocked_long::decrement() volatile
{
	int32 result = InterlockedDecrement(&m_value);
	return result;
}

int32 c_interlocked_long::and_(int32 value)
{
	m_value &= value;
	return m_value;
}

int32 c_interlocked_long::or_(int32 value)
{
	m_value |= value;
	return m_value;
}

int32 c_interlocked_long::xor_(int32 value)
{
	m_value ^= value;
	return m_value;
}

int32 c_interlocked_long::set_bit(int32 index, bool setting)
{
	SET_BIT(m_value, index, setting);
	return m_value;
}

bool c_interlocked_long::test_bit(int32 index) const
{
	bool result = TEST_BIT(m_value, index);
	return result;
}

c_interlocked_long& c_interlocked_long::operator+=(int32 value)
{
	int32 result = add(value);
	return *this;
}

c_interlocked_long& c_interlocked_long::operator-=(int32 value)
{
	int32 result = add(-value);
	return *this;
}

c_interlocked_long& c_interlocked_long::operator&=(int32 value)
{
	int32 result = and_(value);
	return *this;
}

c_interlocked_long& c_interlocked_long::operator|=(int32 value)
{
	int32 result = or_(value);
	return *this;
}

c_interlocked_long& c_interlocked_long::operator=(const c_interlocked_long& other)
{
	int32 result = set(other.m_value);
	return *this;
}

c_interlocked_long& c_interlocked_long::operator=(bool value)
{
	int32 result = set(value);
	return *this;
}

c_interlocked_long& c_interlocked_long::operator=(int32 value)
{
	int32 result = set(value);
	return *this;
}

c_interlocked_int64::c_interlocked_int64(int64 starting_value) :
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

c_interlocked_int64::operator int64() const
{
	return peek();
}

int64 c_interlocked_int64::set(int64 value)
{
	int64 result = InterlockedExchange64(&m_value, value);
	return result;
}

int64 c_interlocked_int64::set_if_equal(int64 value, int64 comperand)
{
	int64 result = InterlockedCompareExchange64(&m_value, value, comperand);
	return result;
}

int64 c_interlocked_int64::peek() const
{
	return m_value;
}

int64 c_interlocked_int64::add(int64 value)
{
	int64 result = InterlockedExchangeAdd64(&m_value, value);
	return result;
}

int64 c_interlocked_int64::increment()
{
	int64 result = InterlockedIncrement64(&m_value);
	return result;
}

int64 c_interlocked_int64::decrement()
{
	int64 result = InterlockedDecrement64(&m_value);
	return result;
}

int64 c_interlocked_int64::and_(int64 value)
{
	m_value &= value;
	return m_value;
}

int64 c_interlocked_int64::or_(int64 value)
{
	m_value |= value;
	return m_value;
}

int64 c_interlocked_int64::xor_(int64 value)
{
	m_value ^= value;
	return m_value;
}

c_interlocked_int64& c_interlocked_int64::operator+=(int64 value)
{
	int64 result = add(value);
	return *this;
}

c_interlocked_int64& c_interlocked_int64::operator-=(int64 value)
{
	int64 result = add(-value);
	return *this;
}

c_interlocked_int64& c_interlocked_int64::operator&=(int64 value)
{
	int64 result = and_(value);
	return *this;
}

c_interlocked_int64& c_interlocked_int64::operator|=(int64 value)
{
	int64 result = or_(value);
	return *this;
}

c_interlocked_int64& c_interlocked_int64::operator=(const c_interlocked_int64& other)
{
	int64 result = set(other.m_value);
	return *this;
}

c_interlocked_int64& c_interlocked_int64::operator=(bool value)
{
	int64 result = set(value);
	return *this;
}

c_interlocked_int64& c_interlocked_int64::operator=(int64 value)
{
	int64 result = set(value);
	return *this;
}

