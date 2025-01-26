#pragma once

class c_synchronized_long
{
public:
	c_synchronized_long(long starting_value);
	c_synchronized_long();
	~c_synchronized_long();

	operator long() const;

	long set(long value);
	long set_if_equal(long value, long comperand);
	long peek() const;
	long add(long value);
	long increment();
	long decrement();
	long and_(long value);
	long or_(long value);
	long xor_(long value);
	long set_bit(long index, bool setting);
	bool test_bit(long index) const;

	c_synchronized_long& operator+=(long value);
	c_synchronized_long& operator-=(long value);
	c_synchronized_long& operator&=(long value);
	c_synchronized_long& operator|=(long value);
	c_synchronized_long& operator=(c_synchronized_long const& other);
	c_synchronized_long& operator=(bool value);
	c_synchronized_long& operator=(long value);

private:
	volatile long m_value;
};
static_assert(sizeof(c_synchronized_long) == 0x4);

class c_synchronized_int64
{
public:
	c_synchronized_int64(__int64 starting_value);
	c_synchronized_int64();
	~c_synchronized_int64();

	operator __int64() const;

	__int64 set(__int64 value);
	__int64 set_if_equal(__int64 value, __int64 comperand);
	__int64 peek() const;
	__int64 add(__int64 value);
	__int64 increment();
	__int64 decrement();
	__int64 and_(__int64 value);
	__int64 or_(__int64 value);
	__int64 xor_(__int64 value);

	c_synchronized_int64& operator+=(__int64 value);
	c_synchronized_int64& operator-=(__int64 value);
	c_synchronized_int64& operator&=(__int64 value);
	c_synchronized_int64& operator|=(__int64 value);
	c_synchronized_int64& operator=(c_synchronized_int64 const& other);
	c_synchronized_int64& operator=(bool value);
	c_synchronized_int64& operator=(__int64 value);

private:
	volatile __int64 m_value;
};
static_assert(sizeof(c_synchronized_int64) == 0x8);

class c_interlocked_long
{
public:
	c_interlocked_long(long starting_value);
	c_interlocked_long();
	~c_interlocked_long();

	operator long() const;

	long set(long value);
	long set_if_equal(long value, long comperand);
	long peek() const;
	long add(long value);
	long increment() volatile;
	long decrement() volatile;
	long and_(long value);
	long or_(long value);
	long xor_(long value);
	long set_bit(long index, bool setting);
	bool test_bit(long index) const;

	c_interlocked_long& operator+=(long value);
	c_interlocked_long& operator-=(long value);
	c_interlocked_long& operator&=(long value);
	c_interlocked_long& operator|=(long value);
	c_interlocked_long& operator=(c_interlocked_long const& other);
	c_interlocked_long& operator=(bool value);
	c_interlocked_long& operator=(long value);

private:
	volatile long m_value;
};
static_assert(sizeof(c_interlocked_long) == 0x4);

class c_interlocked_int64
{
public:
	c_interlocked_int64(__int64 starting_value);
	c_interlocked_int64();
	~c_interlocked_int64();

	operator __int64() const;

	__int64 set(__int64 value);
	__int64 set_if_equal(__int64 value, __int64 comperand);
	__int64 peek() const;
	__int64 add(__int64 value);
	__int64 increment();
	__int64 decrement();
	__int64 and_(__int64 value);
	__int64 or_(__int64 value);
	__int64 xor_(__int64 value);

	c_interlocked_int64& operator+=(__int64 value);
	c_interlocked_int64& operator-=(__int64 value);
	c_interlocked_int64& operator&=(__int64 value);
	c_interlocked_int64& operator|=(__int64 value);
	c_interlocked_int64& operator=(c_interlocked_int64 const& other);
	c_interlocked_int64& operator=(bool value);
	c_interlocked_int64& operator=(__int64 value);

private:
	volatile __int64 m_value;
};

template<long k_count>
class c_synchronized_array
{
public:
//	c_synchronized_array();
//	~c_synchronized_array();
//
//	c_synchronized_long& get_member(long index);
//	c_synchronized_long& operator[](long index);
//	void set_array(long value);
//	c_synchronized_array& operator=(c_synchronized_array& other);
//
//private:
	c_synchronized_long m_value[k_count];
};
static_assert(sizeof(c_synchronized_array<1>) == (sizeof(c_synchronized_long) * 1));

template<long k_bit_count>
struct c_synchronized_bitvector :
	public c_synchronized_array<((k_bit_count + (32 - 1)) >> 5)>
{
};
static_assert(sizeof(c_synchronized_bitvector<256>) == sizeof(c_synchronized_array<8>));

