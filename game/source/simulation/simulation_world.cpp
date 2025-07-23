#include "simulation/simulation_world.hpp"

#include "simulation/simulation.hpp"

bool c_simulation_world::actor_exists(int32 simulation_actor_index) const
{
	return INVOKE_CLASS_MEMBER(0x00467D40, c_simulation_world, actor_exists, simulation_actor_index);
}

void c_simulation_world::advance_update(const struct simulation_update* update)
{
	INVOKE_CLASS_MEMBER(0x00467D60, c_simulation_world, advance_update, update);
}

bool c_simulation_world::all_client_views_active() const
{
	return INVOKE_CLASS_MEMBER(0x00467D70, c_simulation_world, all_client_views_active);
}

void c_simulation_world::apply_simulation_queue(const c_simulation_queue* simulation_queue)
{
	INVOKE_CLASS_MEMBER(0x00467E00, c_simulation_world, apply_simulation_queue, simulation_queue);
}

void c_simulation_world::attach_simulation_queues_to_update(struct simulation_update* update)
{
	INVOKE_CLASS_MEMBER(0x00467F00, c_simulation_world, attach_simulation_queues_to_update, update);
}

void c_simulation_world::attach_to_map()
{
	INVOKE_CLASS_MEMBER(0x00467F70, c_simulation_world, attach_to_map);
}

int32 c_simulation_world::attach_view(c_simulation_view* view)
{
	return INVOKE_CLASS_MEMBER(0x00467F80, c_simulation_world, attach_view, view);
}

bool c_simulation_world::authority_join_timeout_expired() const
{
	return INVOKE_CLASS_MEMBER(0x00467FC0, c_simulation_world, authority_join_timeout_expired);
}

void c_simulation_world::build_player_actions(struct simulation_update* update)
{
	INVOKE_CLASS_MEMBER(0x00468010, c_simulation_world, build_player_actions, update);
}

void c_simulation_world::build_update(bool build_for_simulation_in_progress, struct simulation_update* update, s_simulation_update_metadata* metadata)
{
	INVOKE_CLASS_MEMBER(0x00468160, c_simulation_world, build_update, build_for_simulation_in_progress, update, metadata);
}

void c_simulation_world::change_state_active()
{
	INVOKE_CLASS_MEMBER(0x00468330, c_simulation_world, change_state_active);
}

void c_simulation_world::change_state_dead()
{
	INVOKE_CLASS_MEMBER(0x00468420, c_simulation_world, change_state_dead);
}

void c_simulation_world::change_state_disconnected()
{
	INVOKE_CLASS_MEMBER(0x004684F0, c_simulation_world, change_state_disconnected);
}

void c_simulation_world::change_state_internal(e_simulation_world_state new_state)
{
	INVOKE_CLASS_MEMBER(0x004685C0, c_simulation_world, change_state_internal, new_state);
}

void c_simulation_world::change_state_joining(uns32 joining_client_machine_mask)
{
	INVOKE_CLASS_MEMBER(0x00468680, c_simulation_world, change_state_joining, joining_client_machine_mask);
}

void c_simulation_world::change_state_leaving()
{
	INVOKE_CLASS_MEMBER(0x00468730, c_simulation_world, change_state_leaving);
}

void c_simulation_world::claim_authority_gameworld()
{
	INVOKE_CLASS_MEMBER(0x004687F0, c_simulation_world, claim_authority_gameworld);
}

int32 c_simulation_world::control_stream_get_unit_index(int32 control_index) const
{
	return INVOKE_CLASS_MEMBER(0x00468860, c_simulation_world, control_stream_get_unit_index, control_index);
}

int32 c_simulation_world::create_actor(int32 simulation_actor_index, int32 unit_index)
{
	return INVOKE_CLASS_MEMBER(0x004688C0, c_simulation_world, create_actor, simulation_actor_index, unit_index);
}

void c_simulation_world::create_player(int32 player_index, e_game_simulation_type simulation_type)
{
	INVOKE_CLASS_MEMBER(0x00468920, c_simulation_world, create_player, player_index, simulation_type);
}

void c_simulation_world::deactivate_all_players()
{
	INVOKE_CLASS_MEMBER(0x00468A10, c_simulation_world, deactivate_all_players);
}

void c_simulation_world::debug_render()
{
	if (c_simulation_world::exists() && c_simulation_world::is_distributed())
	{
		ASSERT(m_distributed_world);
		m_distributed_world->m_entity_database.debug_render();
	}
}

void c_simulation_world::delete_actor(int32 simulation_actor_index)
{
	INVOKE_CLASS_MEMBER(0x00468A40, c_simulation_world, delete_actor, simulation_actor_index);
}

void c_simulation_world::delete_all_actors()
{
	INVOKE_CLASS_MEMBER(0x00468A60, c_simulation_world, delete_all_actors);
}

void c_simulation_world::delete_all_players()
{
	INVOKE_CLASS_MEMBER(0x00468A90, c_simulation_world, delete_all_players);
}

void c_simulation_world::delete_player(int32 player_index)
{
	INVOKE_CLASS_MEMBER(0x00468AC0, c_simulation_world, delete_player, player_index);
}

void __cdecl c_simulation_world::destroy_update(struct simulation_update* update)
{
	//INVOKE(0x00468B40, c_simulation_world::destroy_update, update);

	update->bookkeeping_simulation_queue.dispose();
	update->game_simulation_queue.dispose();
}

void c_simulation_world::destroy_world()
{
	INVOKE_CLASS_MEMBER(0x00468B60, c_simulation_world, destroy_world);
}

void c_simulation_world::detach_from_map()
{
	INVOKE_CLASS_MEMBER(0x00468C80, c_simulation_world, detach_from_map);
}

void c_simulation_world::detach_view(c_simulation_view* view, int32 view_index)
{
	INVOKE_CLASS_MEMBER(0x00468C90, c_simulation_world, detach_view, view, view_index);
}

//.text:00468CB0 ; 

void c_simulation_world::disconnect()
{
	INVOKE_CLASS_MEMBER(0x00468CC0, c_simulation_world, disconnect);
}

void c_simulation_world::distribute_update(const struct simulation_update* update, const s_simulation_update_metadata* metadata)
{
	INVOKE_CLASS_MEMBER(0x00468CF0, c_simulation_world, distribute_update, update, metadata);
}

void c_simulation_world::distributed_authority_dispatch_actor_control(uns32 actor_valid_mask, const unit_control_data* actor_control)
{
	INVOKE_CLASS_MEMBER(0x00468D50, c_simulation_world, distributed_authority_dispatch_actor_control, actor_valid_mask, actor_control);
}

void c_simulation_world::distributed_authority_dispatch_player_actions(uns32 player_valid_mask, const player_action* player_actions)
{
	INVOKE_CLASS_MEMBER(0x00468E30, c_simulation_world, distributed_authority_dispatch_player_actions, player_valid_mask, player_actions);
}

void c_simulation_world::drop_simulation_from_active_to_joining()
{
	INVOKE_CLASS_MEMBER(0x00468F80, c_simulation_world, drop_simulation_from_active_to_joining);
}

//.text:00469000 ; 
//.text:00469010 ; 

bool c_simulation_world::exists() const
{
	return m_world_type > _simulation_world_type_none;
}

c_simulation_player* c_simulation_world::find_player_by_machine(const s_machine_identifier* machine_identifier, int32 user_index)
{
	return INVOKE_CLASS_MEMBER(0x00469020, c_simulation_world, find_player_by_machine, machine_identifier, user_index);
}

void c_simulation_world::gamestate_flush()
{
	INVOKE_CLASS_MEMBER(0x004690D0, c_simulation_world, gamestate_flush);
}

//.text:00469100 ; 

uns32 c_simulation_world::get_acknowledged_player_mask() const
{
	return INVOKE_CLASS_MEMBER(0x00469110, c_simulation_world, get_acknowledged_player_mask);
}

c_simulation_view* c_simulation_world::get_authority_view() const
{
	return INVOKE_CLASS_MEMBER(0x004691D0, c_simulation_world, get_authority_view);
}

//.text:00469200 ; 

c_simulation_view* c_simulation_world::get_client_view_by_machine_identifier(const s_machine_identifier* remote_machine_identifier)
{
	return INVOKE_CLASS_MEMBER(0x00469210, c_simulation_world, get_client_view_by_machine_identifier, remote_machine_identifier);
}

c_simulation_view* c_simulation_world::get_client_view_by_machine_index(int32 remote_machine_index)
{
	return INVOKE_CLASS_MEMBER(0x004692B0, c_simulation_world, get_client_view_by_machine_index, remote_machine_index);
}

//.text:00469310 ; 
//.text:00469320 ; 
//.text:00469330 ; 
//.text:00469340 ; 
//.text:00469360 ; 
//.text:00469380 ; 

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

void c_simulation_world::get_machine_identifier(s_machine_identifier* identifier) const
{
	INVOKE_CLASS_MEMBER(0x004693A0, c_simulation_world, get_machine_identifier, identifier);
}

int32 c_simulation_world::get_machine_index() const
{
	return INVOKE_CLASS_MEMBER(0x004693C0, c_simulation_world, get_machine_index);
}

int32 c_simulation_world::get_machine_index_by_identifier(const s_machine_identifier* remote_machine_identifier) const
{
	return INVOKE_CLASS_MEMBER(0x004693D0, c_simulation_world, get_machine_index_by_identifier, remote_machine_identifier);
}

uns32 c_simulation_world::get_player_active_mask() const
{
	return INVOKE_CLASS_MEMBER(0x004693E0, c_simulation_world, get_player_active_mask);
}

uns32 c_simulation_world::get_player_exists_mask() const
{
	return INVOKE_CLASS_MEMBER(0x00469450, c_simulation_world, get_player_exists_mask);
}

uns32 c_simulation_world::get_player_in_game_mask() const
{
	return INVOKE_CLASS_MEMBER(0x004694A0, c_simulation_world, get_player_in_game_mask);
}

//.text:004694B0 ; 

void c_simulation_world::get_player_machine(int32 player_index, s_machine_identifier* machine_identifier) const
{
	INVOKE_CLASS_MEMBER(0x004694C0, c_simulation_world, get_player_machine, player_index, machine_identifier);
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

//.text:004694F0 ; 
//.text:00469500 ; 

c_simulation_view* c_simulation_world::get_view_by_channel(const c_network_channel* channel)
{
	return INVOKE_CLASS_MEMBER(0x00469510, c_simulation_world, get_view_by_channel, channel);
}

c_simulation_view* c_simulation_world::get_view_by_observer(int32 observer_channel_index)
{
	return INVOKE_CLASS_MEMBER(0x00469570, c_simulation_world, get_view_by_observer, observer_channel_index);
}

int32 c_simulation_world::get_view_count() const
{
	return INVOKE_CLASS_MEMBER(0x004695D0, c_simulation_world, get_view_count);
}

void c_simulation_world::go_out_of_sync(bool determinism_failure)
{
	INVOKE_CLASS_MEMBER(0x004695E0, c_simulation_world, go_out_of_sync, determinism_failure);
}

bool c_simulation_world::handle_playback_update(const struct simulation_update* update, s_simulation_update_metadata* metadata)
{
	return INVOKE_CLASS_MEMBER(0x00469610, c_simulation_world, handle_playback_update, update, metadata);
}

void c_simulation_world::handle_synchronous_client_actions(const s_machine_identifier* remote_machine_identifier, uns32 valid_user_mask, const player_action* user_actions)
{
	INVOKE_CLASS_MEMBER(0x00469690, c_simulation_world, handle_synchronous_client_actions, remote_machine_identifier, valid_user_mask, user_actions);
}

bool c_simulation_world::handle_synchronous_playback_control(e_network_synchronous_playback_control type, int32 identifier, int32 update_number)
{
	return INVOKE_CLASS_MEMBER(0x00469760, c_simulation_world, handle_synchronous_playback_control, type, identifier, update_number);
}

bool c_simulation_world::handle_synchronous_update(const struct simulation_update* update, const s_simulation_update_metadata* metadata)
{
	return INVOKE_CLASS_MEMBER(0x004697D0, c_simulation_world, handle_synchronous_update, update, metadata);
}

void c_simulation_world::handle_view_activation(c_simulation_view* view, bool active)
{
	INVOKE_CLASS_MEMBER(0x00469900, c_simulation_world, handle_view_activation, view, active);
}

void c_simulation_world::handle_view_establishment(c_simulation_view* view, bool established)
{
	INVOKE_CLASS_MEMBER(0x004699A0, c_simulation_world, handle_view_establishment, view, established);
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

//.text:00469C10 ; 
//.text:00469C30 ; 
//.text:00469C40 ; 
//.text:00469C50 ; 
//.text:00469C60 ; 

void c_simulation_world::iterator_begin(s_simulation_world_view_iterator* iterator, uns32 view_type_mask) const
{
	INVOKE_CLASS_MEMBER(0x00469C80, c_simulation_world, iterator_begin, iterator, view_type_mask);
}

bool c_simulation_world::iterator_next(s_simulation_world_view_iterator* iterator, c_simulation_view** view) const
{
	return INVOKE_CLASS_MEMBER(0x00469CA0, c_simulation_world, iterator_next, iterator, view);
}

void c_simulation_world::mark_player_pending_deletion(int32 player_index)
{
	INVOKE_CLASS_MEMBER(0x00469D10, c_simulation_world, mark_player_pending_deletion, player_index);
}

//.text:00469D50 ; 
//.text:00469D60 ; 
//.text:00469D80 ; 
//.text:00469D90 ; 

void c_simulation_world::notify_gamestate_flush()
{
	INVOKE_CLASS_MEMBER(0x00469DB0, c_simulation_world, notify_gamestate_flush);
}

void c_simulation_world::notify_gamestate_flush_outside_game_tick()
{
	INVOKE_CLASS_MEMBER(0x00469DD0, c_simulation_world, notify_gamestate_flush_outside_game_tick);
}

void c_simulation_world::notify_initial_gamestate_load(int32 update_number)
{
	INVOKE_CLASS_MEMBER(0x00469DE0, c_simulation_world, notify_initial_gamestate_load, update_number);
}

void c_simulation_world::notify_playback_control(e_network_synchronous_playback_control type, int32 identifier, int32 update_number)
{
	INVOKE_CLASS_MEMBER(0x00469E30, c_simulation_world, notify_playback_control, type, identifier, update_number);
}

//.text:00469EA0 ; 

bool c_simulation_world::player_exists(int32 player_index, s_player_identifier* out_player_identifier) const
{
	return INVOKE_CLASS_MEMBER(0x00469EB0, c_simulation_world, player_exists, player_index, out_player_identifier);
}

bool c_simulation_world::player_is_active(int32 player_index) const
{
	return INVOKE_CLASS_MEMBER(0x00469F00, c_simulation_world, player_is_active, player_index);
}

bool c_simulation_world::player_is_in_game(int32 player_index, const s_player_identifier* player_identifier) const
{
	return INVOKE_CLASS_MEMBER(0x00469F30, c_simulation_world, player_is_in_game, player_index, player_identifier);
}

void c_simulation_world::process_actor_control(int32 simulation_actor_index, const unit_control_data* actor_control)
{
	INVOKE_CLASS_MEMBER(0x00469FC0, c_simulation_world, process_actor_control, simulation_actor_index, actor_control);
}

void c_simulation_world::process_input(uns32 user_action_mask, struct player_action* user_actions)
{
	INVOKE_CLASS_MEMBER(0x0046A000, c_simulation_world, process_input, user_action_mask, user_actions);
}

void c_simulation_world::process_pending_updates()
{
	INVOKE_CLASS_MEMBER(0x0046A0A0, c_simulation_world, process_pending_updates);
}

void c_simulation_world::process_playback_events()
{
	INVOKE_CLASS_MEMBER(0x0046A0D0, c_simulation_world, process_playback_events);
}

//.text:0046A140 ; 

void c_simulation_world::recreate_players(e_game_simulation_type simulation_type)
{
	INVOKE_CLASS_MEMBER(0x0046A160, c_simulation_world, recreate_players, simulation_type);
}

void c_simulation_world::remove_all_views()
{
	INVOKE_CLASS_MEMBER(0x0046A1F0, c_simulation_world, remove_all_views);
}

void c_simulation_world::reset_world()
{
	INVOKE_CLASS_MEMBER(0x0046A240, c_simulation_world, reset_world);
}

//.text:0046A300 ; 
//.text:0046A310 ; 

void c_simulation_world::send_player_acknowledgements(bool force_acknowledgement)
{
	INVOKE_CLASS_MEMBER(0x0046A320, c_simulation_world, send_player_acknowledgements, force_acknowledgement);
}

void c_simulation_world::send_synchronous_acknowledgements(bool force_acknowledgement)
{
	INVOKE_CLASS_MEMBER(0x0046A3D0, c_simulation_world, send_synchronous_acknowledgements, force_acknowledgement);
}

//.text:0046A450 ; 

void c_simulation_world::set_machine_identifier(const s_machine_identifier* identifier)
{
	INVOKE_CLASS_MEMBER(0x0046A480, c_simulation_world, set_machine_identifier, identifier);
}

void c_simulation_world::set_machine_index(int32 machine_index)
{
	INVOKE_CLASS_MEMBER(0x0046A4B0, c_simulation_world, set_machine_index, machine_index);
}

void c_simulation_world::simulation_queue_allocate(e_simulation_queue_element_type type, int32 data_size, s_simulation_queue_element** element_out)
{
	INVOKE_CLASS_MEMBER(0x0046A4C0, c_simulation_world, simulation_queue_allocate, type, data_size, element_out);
}

void c_simulation_world::simulation_queue_enqueue(s_simulation_queue_element* element)
{
	INVOKE_CLASS_MEMBER(0x0046A5E0, c_simulation_world, simulation_queue_enqueue, element);
}

void c_simulation_world::simulation_queue_free(s_simulation_queue_element* element)
{
	INVOKE_CLASS_MEMBER(0x0046A620, c_simulation_world, simulation_queue_free, element);
}

bool c_simulation_world::simulation_queues_empty()
{
	return INVOKE_CLASS_MEMBER(0x0046A660, c_simulation_world, simulation_queues_empty);
}

void c_simulation_world::synchronous_authority_dispatch_update(const struct simulation_update* update, const s_simulation_update_metadata* metadata)
{
	INVOKE_CLASS_MEMBER(0x0046A690, c_simulation_world, synchronous_authority_dispatch_update, update, metadata);
}

int32 c_simulation_world::synchronous_authority_get_maximum_update_queue_size()
{
	return INVOKE_CLASS_MEMBER(0x0046A720, c_simulation_world, synchronous_authority_get_maximum_update_queue_size);
}

int32 c_simulation_world::synchronous_authority_get_maximum_updates()
{
	return INVOKE_CLASS_MEMBER(0x0046A7A0, c_simulation_world, synchronous_authority_get_maximum_updates);
}

//.text:0046A820 ; 
//.text:0046A830 ; 
//.text:0046A840 ; 

bool c_simulation_world::synchronous_catchup_in_progress() const
{
	return INVOKE_CLASS_MEMBER(0x0046A850, c_simulation_world, synchronous_catchup_in_progress);
}

//.text:0046A8D0 ; 

int32 c_simulation_world::time_get_available(bool* out_match_remote_time, int32* out_updates_available)
{
	return INVOKE_CLASS_MEMBER(0x0046A8F0, c_simulation_world, time_get_available, out_match_remote_time, out_updates_available);
}

void c_simulation_world::time_set_immediate_update(bool immediate_update)
{
	INVOKE_CLASS_MEMBER(0x0046A9A0, c_simulation_world, time_set_immediate_update, immediate_update);
}

void c_simulation_world::time_set_next_update_number(int32 next_update_number, bool flush_update_queue)
{
	INVOKE_CLASS_MEMBER(0x0046AAC0, c_simulation_world, time_set_next_update_number, next_update_number, flush_update_queue);
}

void c_simulation_world::time_start(int32 next_update_number, bool flush_update_queue)
{
	INVOKE_CLASS_MEMBER(0x0046AB10, c_simulation_world, time_start, next_update_number, flush_update_queue);
}

void c_simulation_world::time_stop()
{
	INVOKE_CLASS_MEMBER(0x0046AB40, c_simulation_world, time_stop);
}

//.text:0046AB50 ; 

void c_simulation_world::update()
{
	INVOKE_CLASS_MEMBER(0x0046AB60, c_simulation_world, update);
}

void c_simulation_world::update_authority_active()
{
	INVOKE_CLASS_MEMBER(0x0046ACA0, c_simulation_world, update_authority_active);
}

void c_simulation_world::update_authority_join_initiate()
{
	INVOKE_CLASS_MEMBER(0x0046B100, c_simulation_world, update_authority_join_initiate);
}

void c_simulation_world::update_authority_join_progress()
{
	INVOKE_CLASS_MEMBER(0x0046B180, c_simulation_world, update_authority_join_progress);
}

void c_simulation_world::update_client_disconnection()
{
	INVOKE_CLASS_MEMBER(0x0046B4A0, c_simulation_world, update_client_disconnection);
}

void c_simulation_world::update_client_failure()
{
	INVOKE_CLASS_MEMBER(0x0046B510, c_simulation_world, update_client_failure);
}

void c_simulation_world::update_client_join_initiate()
{
	INVOKE_CLASS_MEMBER(0x0046B550, c_simulation_world, update_client_join_initiate);
}

void c_simulation_world::update_client_join_progress()
{
	INVOKE_CLASS_MEMBER(0x0046B600, c_simulation_world, update_client_join_progress);
}

void c_simulation_world::update_establishing_view(c_simulation_view* view)
{
	INVOKE_CLASS_MEMBER(0x0046B700, c_simulation_world, update_establishing_view, view);
}

c_simulation_world::e_join_progress c_simulation_world::update_joining_view(c_simulation_view* view)
{
	return INVOKE_CLASS_MEMBER(0x0046B750, c_simulation_world, update_joining_view, view);
}

void c_simulation_world::update_player_activation()
{
	INVOKE_CLASS_MEMBER(0x0046B910, c_simulation_world, update_player_activation);
}

int32 c_simulation_world::update_queue_get_available_updates() const
{
	return INVOKE_CLASS_MEMBER(0x0046B990, c_simulation_world, update_queue_get_available_updates);
}

int32 c_simulation_world::update_queue_get_next_expected_update_number() const
{
	return INVOKE_CLASS_MEMBER(0x0046B9D0, c_simulation_world, update_queue_get_next_expected_update_number);
}

bool c_simulation_world::update_queue_handle_playback_event(e_simulation_playback_event event_type, int32 event_data, int32 event_update_number)
{
	return INVOKE_CLASS_MEMBER(0x0046BA00, c_simulation_world, update_queue_handle_playback_event, event_type, event_data, event_update_number);
}

bool c_simulation_world::update_queue_handle_server_update(const struct simulation_update* update, const s_simulation_update_metadata* metadata)
{
	return INVOKE_CLASS_MEMBER(0x0046BA90, c_simulation_world, update_queue_handle_server_update, update, metadata);
}

void c_simulation_world::update_queue_reset()
{
	INVOKE_CLASS_MEMBER(0x0046BB40, c_simulation_world, update_queue_reset);
}

bool c_simulation_world::update_queue_retrieve_event(e_simulation_playback_event* out_event_type, int32* out_event_data, int32* out_event_update_number)
{
	return INVOKE_CLASS_MEMBER(0x0046BBC0, c_simulation_world, update_queue_retrieve_event, out_event_type, out_event_data, out_event_update_number);
}

void c_simulation_world::update_queue_retrieve_update(struct simulation_update* update, s_simulation_update_metadata* metadata)
{
	INVOKE_CLASS_MEMBER(0x0046BC50, c_simulation_world, update_queue_retrieve_update, update, metadata);
}

void c_simulation_world::update_queue_start(int32 next_update_number, bool flush_update_queue)
{
	INVOKE_CLASS_MEMBER(0x0046BCD0, c_simulation_world, update_queue_start, next_update_number, flush_update_queue);
}

void c_simulation_world::update_queue_stop()
{
	INVOKE_CLASS_MEMBER(0x0046BD40, c_simulation_world, update_queue_stop);
}

