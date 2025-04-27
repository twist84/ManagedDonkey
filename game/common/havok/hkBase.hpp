#pragma once

struct hkBaseObject
{
public:
	struct hkBaseObject_vtbl* __vftable /*VFT*/;
};

struct hkReferencedObject :
	public hkBaseObject
{
public:
	int16 m_size;
	int16 m_count;
};

template<typename T>
struct hkPadSpu
{
	T m_storage;
};
static_assert(sizeof(hkPadSpu<char*>) == sizeof(char*));

struct hkBool
{
	char m_bool;
};
static_assert(sizeof(hkBool) == sizeof(char));

