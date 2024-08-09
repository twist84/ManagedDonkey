#include "simulation/simulation_view.hpp"

//.text:00465D00 ; void c_simulation_view::attach_observer_channel(c_network_observer*, long, s_transport_secure_address const*)
//.text:00465EC0 ; void c_simulation_view::attach_to_world(c_simulation_world*)
//.text:00465EE0 ; bool c_simulation_view::client_in_game() const
//.text:00465F00 ; bool c_simulation_view::client_join_is_finished() const
//.text:00465F50 ; void c_simulation_view::destroy_view()
//.text:00465F60 ; void c_simulation_view::detach_from_world()
//.text:00465FC0 ; void c_simulation_view::detach_observer_channel()
//.text:00466100 ; void c_simulation_view::dispatch_synchronous_actions(dword, c_static_array<s_player_action, 4> const&)
//.text:00466230 ; void c_simulation_view::dispatch_synchronous_playback_control(e_network_synchronous_playback_control, long, long)
//.text:004662A0 ; void c_simulation_view::dispatch_synchronous_update(struct simulation_update const*, s_simulation_update_metadata const*)
//.text:00466350 ; void c_simulation_view::distributed_join_abort()
//.text:00466370 ; static void c_simulation_view::distributed_join_complete()
//.text:00466380 ; bool c_simulation_view::distributed_join_in_progress() const
//.text:004663F0 ; bool c_simulation_view::distributed_join_initiate()
//.text:00466400 ; bool c_simulation_view::established() const
//.text:00466420 ; bool c_simulation_view::establishment_add_cache_file_signature(long*, long, byte*) const
//.text:00466480 ; bool c_simulation_view::establishment_verify_cache_file_signature(long, byte const*) const
//.text:00466500 ; void c_simulation_view::failed_to_join()
//.text:00466590 ; void c_simulation_view::force_unacknowledge_player(long)
//.text:004665B0 ; dword c_simulation_view::get_acknowledged_player_mask() const
//.text:004665C0 ; c_replication_control_view* c_simulation_view::get_control_view()

e_simulation_view_type c_simulation_view::view_type() const
{
	return DECLFUNC(0x00466600, e_simulation_view_type, __thiscall, c_simulation_view const*)(this);
}

//.text:00466620 ; long c_simulation_view::get_machine_index() const
//.text:00466680 ; void c_simulation_view::get_statistics(s_simulation_view_statistics*)
//.text:00466700 ; c_simulation_view::get_machine_identifier?

char const* c_simulation_view::get_view_description() const
{
	return DECLFUNC(0x00466710, char const*, __thiscall, c_simulation_view const*)(this);

	//return NULL;
}

long c_simulation_view::get_view_establishment_identifier() const
{
	return DECLFUNC(0x00466720, long, __thiscall, c_simulation_view const*)(this);

	//return m_view_establishment_identifier;
}

e_simulation_view_establishment_mode c_simulation_view::get_view_establishment_mode() const
{
	return DECLFUNC(0x00466730, e_simulation_view_establishment_mode, __thiscall, c_simulation_view const*)(this);

	//return m_view_establishment_mode;
}

//.text:00466740 ; void c_simulation_view::go_out_of_sync()

bool c_simulation_view::handle_distributed_game_results(long message_establishment_identifier, long incremental_update_number, s_game_results_incremental_update const* incremental_update)
{
	return DECLFUNC(0x004667A0, bool, __thiscall, c_simulation_view*, long, long, s_game_results_incremental_update const*)(this, message_establishment_identifier, incremental_update_number, incremental_update);

	//ASSERT(exists());
	//ASSERT(m_view_type == _simulation_view_type_distributed_to_remote_authority);
	//ASSERT(m_distributed_view);
	//
	//if (message_establishment_identifier == m_view_establishment_identifier)
	//{
	//	return m_distributed_view->m_control_view.m_game_results_replicator.handle_update();
	//}
	//
	//if (message_establishment_identifier >= m_view_establishment_identifier)
	//{
	//	generate_event(_event_level_warning, "networking:simulation:view: failing to handle incremental game results (message establishment identifier [%d] > current [%d])",
	//		message_establishment_identifier,
	//		m_view_establishment_identifier);
	//	return false;
	//}
	//
	//
	//generate_event(_event_level_message, "networking:simulation:view: ignoring incremental game results (message establishment identifier %d < current %d)",
	//	message_establishment_identifier,
	//	m_view_establishment_identifier);
	//return true;
}

bool c_simulation_view::handle_player_acknowledge(dword player_valid_mask, dword player_in_game_mask, s_player_identifier const* player_identifiers)
{
	return DECLFUNC(0x00466820, bool, __thiscall, c_simulation_view*, dword, dword, s_player_identifier const*)(this, player_valid_mask, player_in_game_mask, player_identifiers);
}

bool c_simulation_view::handle_remote_establishment(e_simulation_view_establishment_mode establishment_mode, long establishment_identifier, long signature_size, byte const* signature_data)
{
	return DECLFUNC(0x00466940, bool, __thiscall, c_simulation_view*, e_simulation_view_establishment_mode, long, long, byte const*)(this, establishment_mode, establishment_identifier, signature_size, signature_data);
}

bool c_simulation_view::handle_synchronous_acknowledge(long current_update_number)
{
	return DECLFUNC(0x00466B10, bool, __thiscall, c_simulation_view*, long)(this, current_update_number);
}

bool c_simulation_view::handle_synchronous_actions(long action_number, long current_action_number, dword user_flags, s_player_action const* actions)
{
	return DECLFUNC(0x00466B30, bool, __thiscall, c_simulation_view*, long, long, dword, s_player_action const*)(this, action_number, current_action_number, user_flags, actions);
}

bool c_simulation_view::handle_synchronous_gamestate(s_network_message_synchronous_gamestate const* synchronous_gamestate, void const* chunk_data)
{
	return DECLFUNC(0x00466BB0, bool, __thiscall, c_simulation_view*, s_network_message_synchronous_gamestate const*, void const*)(this, synchronous_gamestate, chunk_data);
}

bool c_simulation_view::handle_synchronous_playback_control(e_network_synchronous_playback_control type, long identifier, long update_number)
{
	return DECLFUNC(0x00466D10, bool, __thiscall, c_simulation_view*, e_network_synchronous_playback_control, long, long)(this, type, identifier, update_number);
}

bool c_simulation_view::handle_synchronous_update(struct simulation_update const* update)
{
	return DECLFUNC(0x00466D50, bool, __thiscall, c_simulation_view*, struct simulation_update const*)(this, update);
}

//.text:00466DC0 ; void c_game_results_replicator::handle_view_establishment(bool)

//.text:00466EB0 ; void c_simulation_view::initialize_view(long, e_simulation_view_type, c_simulation_distributed_view*, s_machine_identifier const*, long, char const*)

bool c_simulation_view::is_client_view() const
{
	return DECLFUNC(0x00466FC0, bool, __thiscall, c_simulation_view const*)(this);
}

//.text:00466FE0 ; bool c_simulation_view::is_dead(long*) const
//.text:00467000 ; bool c_simulation_view::is_distributed() const
//.text:00467050 ; void c_simulation_view::kill_view(e_simulation_view_reason)
//.text:00467120 ; void c_simulation_view::no_longer_authority()
//.text:00467180 ; void c_simulation_view::notify_closed()
//.text:004671E0 ; bool c_simulation_view::observer_channel_backlogged(e_network_message_type) const
//.text:00467210 ; bool c_simulation_view::observer_channel_get_bandwidth_events(long, long*) const
//.text:00467240 ; void c_simulation_view::observer_channel_set_waiting_on_backlog(e_network_message_type)
//.text:00467270 ; void c_simulation_view::send_establishment_message()

//.text:00467330 ; void c_game_results_replicator::send_game_results_update()

//.text:00467410 ; void c_simulation_view::send_message(e_network_message_type, long, void const*, bool) const
//.text:00467490 ; void c_simulation_view::set_view_establishment(e_simulation_view_establishment_mode, long)

//.text:00467520 ; void c_game_results_replicator::start_receiving_updates()
//.text:00467550 ; void c_game_results_replicator::start_sending_updates()
//.text:00467580 ; void c_game_results_replicator::stop_receiving_updates()
//.text:00467590 ; void c_game_results_replicator::stop_sending_updates()

long c_simulation_view::synchronous_catchup_attempt_count() const
{
	return DECLFUNC(0x004675A0, long, __thiscall, c_simulation_view const*)(this);
}

void c_simulation_view::synchronous_catchup_complete()
{
	DECLFUNC(0x004675B0, void, __thiscall, c_simulation_view*)(this);
}

bool c_simulation_view::synchronous_catchup_in_progress() const
{
	return DECLFUNC(0x00467610, bool, __thiscall, c_simulation_view const*)(this);
}

bool c_simulation_view::synchronous_catchup_initiate()
{
	return DECLFUNC(0x00467620, bool, __thiscall, c_simulation_view*)(this);
}

void c_simulation_view::synchronous_catchup_send_data()
{
	DECLFUNC(0x00467730, void, __thiscall, c_simulation_view*)(this);
}

void c_simulation_view::synchronous_catchup_terminate()
{
	DECLFUNC(0x00467A60, void, __thiscall, c_simulation_view*)(this);
}

long c_simulation_view::synchronous_client_get_acknowledged_update_number()
{
	return DECLFUNC(0x00467AA0, long, __thiscall, c_simulation_view*)(this);
}

//.text:00467AD0 ; void c_game_results_replicator::update()

//.text:00467B80 ; void c_simulation_view::update()
//.text:00467BF0 ; void c_simulation_view::update_view_activation_state()

