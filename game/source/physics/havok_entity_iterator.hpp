#pragma once

struct hkWorld;
struct c_havok_entity_iterator
{
protected:
	bool m_iterating;
	bool m_inactive_simulation_islands;
	long m_island_index;
	long m_entity_index;
	hkWorld* m_world;
	long m_iteration_type;

private:
	static short& g_num_entity_iterators_in_use;
};
static_assert(sizeof(c_havok_entity_iterator) == 0x14);

