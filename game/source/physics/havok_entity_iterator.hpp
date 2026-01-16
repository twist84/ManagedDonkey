#pragma once

struct hkEntity;
struct hkWorld;

class c_havok_entity_iterator
{
public:
	enum e_iteration_type
	{
		_iteration_type_active_entities = 0,
		_iteration_type_inactive_entities,
		_iteration_type_all_entities,

		k_iteration_type_count,
	};

public:
	c_havok_entity_iterator(hkWorld* world, e_iteration_type iteration_type);
	~c_havok_entity_iterator();

	void begin();
	void end();
	hkEntity* get_entity();
	int32 get_object_index();
	bool next();

private:
	bool m_iterating;
	bool m_inactive_simulation_islands;
	int32 m_island_index;
	int32 m_entity_index;
	hkWorld* m_world;
	e_iteration_type m_iteration_type;

	static int16& g_num_entity_iterators_in_use;
};
COMPILE_ASSERT(sizeof(c_havok_entity_iterator) == 0x14);

