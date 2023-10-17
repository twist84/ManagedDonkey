#include "simulation/simulation_world.hpp"

bool c_simulation_world::exists()
{
	return m_world_type > _simulation_world_type_none;
}

bool c_simulation_world::is_active()
{
	ASSERT(exists());

	return m_world_state == _simulation_world_state_active;
}

void c_simulation_world::debug_render()
{
	ASSERT(m_distributed_world);

	m_distributed_world->m_entity_database.debug_render();
}

