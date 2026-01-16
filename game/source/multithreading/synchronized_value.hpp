#pragma once

class c_synchronized_long
{
public:
	c_synchronized_long(int32 starting_value);
	c_synchronized_long();
	~c_synchronized_long();

	operator int32() const;

	int32 set(int32 value);
	int32 set_if_equal(int32 value, int32 comperand);
	int32 peek() const;
	int32 add(int32 value);
	int32 increment();
	int32 decrement();
	int32 and_(int32 value);
	int32 or_(int32 value);
	int32 xor_(int32 value);
	int32 set_bit(int32 index, bool setting);
	bool test_bit(int32 index) const;

	c_synchronized_long& operator+=(int32 value);
	c_synchronized_long& operator-=(int32 value);
	c_synchronized_long& operator&=(int32 value);
	c_synchronized_long& operator|=(int32 value);
	c_synchronized_long& operator=(const c_synchronized_long& other);
	c_synchronized_long& operator=(bool value);
	c_synchronized_long& operator=(int32 value);

private:
	volatile int32 m_value;
};
COMPILE_ASSERT(sizeof(c_synchronized_long) == 0x4);

class c_synchronized_int64
{
public:
	c_synchronized_int64(int64 starting_value);
	c_synchronized_int64();
	~c_synchronized_int64();

	operator int64() const;

	int64 set(int64 value);
	int64 set_if_equal(int64 value, int64 comperand);
	int64 peek() const;
	int64 add(int64 value);
	int64 increment();
	int64 decrement();
	int64 and_(int64 value);
	int64 or_(int64 value);
	int64 xor_(int64 value);

	c_synchronized_int64& operator+=(int64 value);
	c_synchronized_int64& operator-=(int64 value);
	c_synchronized_int64& operator&=(int64 value);
	c_synchronized_int64& operator|=(int64 value);
	c_synchronized_int64& operator=(const c_synchronized_int64& other);
	c_synchronized_int64& operator=(bool value);
	c_synchronized_int64& operator=(int64 value);

private:
	volatile int64 m_value;
};
COMPILE_ASSERT(sizeof(c_synchronized_int64) == 0x8);

class c_interlocked_long
{
public:
	c_interlocked_long(int32 starting_value);
	c_interlocked_long();
	~c_interlocked_long();

	operator int32() const;

	int32 set(int32 value);
	int32 set_if_equal(int32 value, int32 comperand);
	int32 peek() const;
	int32 add(int32 value);
	int32 increment() volatile;
	int32 decrement() volatile;
	int32 and_(int32 value);
	int32 or_(int32 value);
	int32 xor_(int32 value);
	int32 set_bit(int32 index, bool setting);
	bool test_bit(int32 index) const;

	c_interlocked_long& operator+=(int32 value);
	c_interlocked_long& operator-=(int32 value);
	c_interlocked_long& operator&=(int32 value);
	c_interlocked_long& operator|=(int32 value);
	c_interlocked_long& operator=(const c_interlocked_long& other);
	c_interlocked_long& operator=(bool value);
	c_interlocked_long& operator=(int32 value);

private:
	volatile int32 m_value;
};
COMPILE_ASSERT(sizeof(c_interlocked_long) == 0x4);

class c_interlocked_int64
{
public:
	c_interlocked_int64(int64 starting_value);
	c_interlocked_int64();
	~c_interlocked_int64();

	operator int64() const;

	int64 set(int64 value);
	int64 set_if_equal(int64 value, int64 comperand);
	int64 peek() const;
	int64 add(int64 value);
	int64 increment();
	int64 decrement();
	int64 and_(int64 value);
	int64 or_(int64 value);
	int64 xor_(int64 value);

	c_interlocked_int64& operator+=(int64 value);
	c_interlocked_int64& operator-=(int64 value);
	c_interlocked_int64& operator&=(int64 value);
	c_interlocked_int64& operator|=(int64 value);
	c_interlocked_int64& operator=(const c_interlocked_int64& other);
	c_interlocked_int64& operator=(bool value);
	c_interlocked_int64& operator=(int64 value);

private:
	volatile int64 m_value;
};

template<int32 k_count>
class c_synchronized_array
{
public:
//	c_synchronized_array();
//	~c_synchronized_array();
//
//	c_synchronized_long& get_member(int32 index);
//	c_synchronized_long& operator[](int32 index);
//	void set_array(int32 value);
//	c_synchronized_array& operator=(c_synchronized_array& other);
//
//private:
	c_synchronized_long m_value[k_count];
};
COMPILE_ASSERT(sizeof(c_synchronized_array<1>) == (sizeof(c_synchronized_long) * 1));

template<int32 k_bit_count>
class c_synchronized_bitvector :
	public c_synchronized_array<((k_bit_count + (32 - 1)) >> 5)>
{
};
COMPILE_ASSERT(sizeof(c_synchronized_bitvector<256>) == sizeof(c_synchronized_array<8>));

