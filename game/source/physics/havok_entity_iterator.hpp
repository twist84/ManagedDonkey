#pragma once

struct hkWorld;
class c_havok_entity_iterator
{
protected:
	bool m_iterating;
	bool m_inactive_simulation_islands;
	int32 m_island_index;
	int32 m_entity_index;
	hkWorld* m_world;
	int32 m_iteration_type;

private:
	static int16& g_num_entity_iterators_in_use;
};
static_assert(sizeof(c_havok_entity_iterator) == 0x14);

