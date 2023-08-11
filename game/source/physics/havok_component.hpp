#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

template <typename t_type>
struct hkArrayBase
{
public:
	t_type* _data;
	long _size;
	dword _capacityAndFlags;
};
static_assert(sizeof(hkArrayBase<void*>) == 0xC);

template <typename t_type, typename t_allocator = void>
struct hkArray :
	public hkArrayBase<t_type>
{
public:
};
static_assert(sizeof(hkArray<void*>) == 0xC);

struct hkRigidBody;
struct c_havok_component : s_datum_header
{
	struct c_rigid_body
	{
		byte __data0[0x1C];
		real_point3d __point1C;
		vector3d __vector28;
		vector3d __vector34;
		hkRigidBody* m_rigid_body;
		byte_flags m_flags;
	};
	static_assert(sizeof(c_rigid_body) == 0x48);

	struct c_constraint
	{
		byte __data0[0xC];
	};
	static_assert(sizeof(c_constraint) == 0xC);

	dword_flags m_flags;
	long m_object_index;
	short m_object_postion_controlling_rigid_body_index;
	bool __unknownE;
	long __unknown10;
	hkArray<c_rigid_body> m_havok_rigid_bodies;
	c_rigid_body m_rigid_body;
	hkArray<c_constraint> m_constraints;
	void* __unknown60;
	void* m_simple_shape_phantom;
	void* m_aabb_phantom;
};
static_assert(sizeof(c_havok_component) == 0x80);

extern c_smart_data_array<c_havok_component>*& g_havok_component_data;

