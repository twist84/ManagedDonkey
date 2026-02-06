#include "physics/havok_entity_iterator.hpp"

#include "cseries/cseries.hpp"

REFERENCE_DECLARE(0x04FC48E8, int16, c_havok_entity_iterator::g_num_entity_iterators_in_use);

c_havok_entity_iterator::c_havok_entity_iterator(hkWorld* world, e_iteration_type iteration_type) //:
	//m_world(world),
	//m_iteration_type(iteration_type)
{
	DECLFUNC(0x00760740, void, __thiscall, c_havok_entity_iterator*, hkWorld*, e_iteration_type)(this, world, iteration_type);

	//ASSERT(m_world != nullptr);
	//ASSERT(VALID_INDEX(iteration_type, k_iteration_type_count));
	//
	//m_iterating = false;
}

c_havok_entity_iterator::~c_havok_entity_iterator()
{
	DECLFUNC(0x00760760, void, __thiscall, c_havok_entity_iterator*)(this);

	//VASSERT(!m_iterating, "you must call end before finishing with this iterator");
}

void c_havok_entity_iterator::begin()
{
	INVOKE_CLASS_MEMBER(0x00760770, c_havok_entity_iterator, begin);

	//ASSERT(m_world != nullptr);
	//ASSERT(!m_iterating);
	//
	//m_inactive_simulation_islands = false;
	//m_island_index = NONE;
	//m_entity_index = NONE;
	//
	//m_iterating = true;
	//g_num_entity_iterators_in_use++;
}

void c_havok_entity_iterator::end()
{
	//INVOKE_CLASS_MEMBER(0x00760790, c_havok_entity_iterator, begin);

	//ASSERT(m_iterating);
	//
	//m_iterating = false;
	//g_num_entity_iterators_in_use--;
}

hkEntity* c_havok_entity_iterator::get_entity()
{
	return INVOKE_CLASS_MEMBER(0x007607A0, c_havok_entity_iterator, get_entity);
}

int32 c_havok_entity_iterator::get_object_index()
{
	return INVOKE_CLASS_MEMBER(0x007607E0, c_havok_entity_iterator, get_object_index);
}

bool c_havok_entity_iterator::next()
{
	return INVOKE_CLASS_MEMBER(0x00760830, c_havok_entity_iterator, next);
}

