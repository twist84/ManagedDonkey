#pragma once

template <typename t_type>
struct hkArrayBase
{
public:
	t_type* m_data;
	int m_size;
	unsigned int m_capacityAndFlags;
};
static_assert(sizeof(hkArrayBase<void*>) == 0xC);

template <typename t_type, typename t_allocator = void>
struct hkArray :
	public hkArrayBase<t_type>
{
public:
	hkArray();
	void pushBack(t_type* t);
	int getSize() const;
	//const t_type& operator[](int i) const;
	t_type& operator[](int i);
};
static_assert(sizeof(hkArray<void*>) == 0xC);

template <typename t_type, int t_count, typename t_allocator = void>
struct hkInplaceArray :
	public hkArray<t_type>
{
	t_type m_storage[t_count];
};

