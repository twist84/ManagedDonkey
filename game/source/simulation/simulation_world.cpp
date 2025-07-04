#include "simulation/simulation_world.hpp"

#include "simulation/simulation.hpp"

bool c_simulation_world::exists() const
{
	return m_world_type > _simulation_world_type_none;
}

void c_simulation_world::initialize_world(e_game_simulation_type simulation_type, e_game_playback_type playback_type, bool reset_next_update_number, c_simulation_type_collection* type_collection, c_simulation_watcher* watcher, c_simulation_distributed_world* distributed_world)
{
	INVOKE_CLASS_MEMBER(0x00469A00, c_simulation_world, initialize_world, simulation_type, playback_type, reset_next_update_number, type_collection, watcher, distributed_world);
}

bool c_simulation_world::is_active()
{
	ASSERT(exists());

	return m_world_state == _simulation_world_state_active;
}

bool c_simulation_world::is_authority() const
{
	ASSERT(exists());

	bool is_client = m_world_type == _simulation_world_type_synchronous_game_client
		|| m_world_type == _simulation_world_type_synchronous_film_client
		|| m_world_type == _simulation_world_type_distributed_client;
	return !is_client;
}

bool c_simulation_world::is_distributed() const
{
	ASSERT(exists());

	return m_world_type == _simulation_world_type_distributed_server || m_world_type == _simulation_world_type_distributed_client;
}

bool c_simulation_world::is_local() const
{
	ASSERT(exists());

	bool is_local = m_world_type == _simulation_world_type_local || m_world_type == _simulation_world_type_local_playback;
	ASSERT(!is_local || m_view_count == 0);

	return is_local;
}

void __cdecl c_simulation_world::destroy_update(struct simulation_update* update)
{
	//INVOKE(0x00468B40, c_simulation_world::destroy_update, update);

	update->bookkeeping_simulation_queue.dispose();
	update->game_simulation_queue.dispose();
}

void c_simulation_world::debug_render()
{
	if (exists() && is_distributed())
	{
		ASSERT(m_distributed_world);
		m_distributed_world->m_entity_database.debug_render();
	}
}

const char* c_simulation_world::get_state_string(int32 state)
{
	switch (state)
	{
	case _simulation_world_state_none:
		return "none";
	case _simulation_world_state_dead:
		return "dead";
	case _simulation_world_state_disconnected:
		return "disconnected";
	case _simulation_world_state_joining:
		return "joining";
	case _simulation_world_state_active:
		return "active";
	case _simulation_world_state_handoff:
		return "handoff";
	case 6:
		return "leaving"; // if this exists add it to the enum
	}

	return "<unknown>";
}

void c_simulation_world::get_join_status(
	int32* join_time_elapsed,
	int32* join_time_to_abort,
	int32* join_attempt_count,
	int32* join_attempt_maximum,
	int32* join_client_establishing_count,
	int32* join_client_waiting_count,
	int32* join_client_joining_count,
	int32* join_client_complete_count,
	int32* join_client_total_count,
	int32* join_time_to_failure) const
{

}

