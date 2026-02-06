#include "networking/logic/life_cycle/life_cycle_state_handler.hpp"

#include "cseries/cseries_events.hpp"
#include "memory/module.hpp"
#include "networking/logic/life_cycle/life_cycle_manager.hpp"
#include "networking/session/network_session.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x0048E130, c_life_cycle_state_handler, setup_initial_participants);

c_life_cycle_state_handler::c_life_cycle_state_handler() :
	m_handler_flags()
{
	//DECLFUNC(0x00453EC0, void, __thiscall, c_life_cycle_state_handler*)(this);
}

bool c_life_cycle_state_handler::all_peers_have_main_menu_ready(const c_network_session* session)
{
	return INVOKE_CLASS_MEMBER(0x0048CDB0, c_life_cycle_state_handler, all_peers_have_main_menu_ready, session);
}

//.text:0048CE20 ; protected: bool c_life_cycle_state_handler::all_squad_peers_established_in_group()
//.text:0048CF70 ; protected: void c_life_cycle_state_handler::build_player_options(const s_network_session_player*, uns32, const s_machine_identifier*, const c_game_variant*, int32, int32, int32, game_player_options*)
//.text:0048D010 ; protected: bool c_life_cycle_state_handler::can_pick_teams_for_group_with_party_exclude_mask(uns32)
//.text:0048D1A0 ; protected: void c_life_cycle_state_handler::disband_group_session()

void c_life_cycle_state_handler::dispose()
{
	INVOKE_CLASS_MEMBER(0x0048D1F0, c_life_cycle_state_handler, dispose);

	//get_manager()->deregister_state_handler(m_state, this);
}

void c_life_cycle_state_handler::enter(c_life_cycle_state_handler* from, int32 entry_data_size, void* entry_data)
{
	INVOKE_CLASS_MEMBER(0x0048D220, c_life_cycle_state_handler, enter, from, entry_data_size, entry_data);

	//ASSERT(entry_data_size == 0);
	//ASSERT(entry_data == nullptr);
	//
	//event(_event_message, "networking:logic:life-cycle: entering state %s",
	//	get_state_string());
	//
	//if (!test_flag(_life_cycle_state_handler_requires_group_session_bit) && !test_flag(_life_cycle_state_handler_allows_group_session_bit))
	//{
	//	c_network_session* group_session = get_manager()->get_group_session();
	//	if (!group_session->disconnected() && !group_session->leaving_session())
	//	{
	//		event(_event_message, "networking:logic:life-cycle: entering state %s which does not require nor allow a group, leaving",
	//			get_state_string());
	//
	//		group_session->leave_session_and_disconnect();
	//	}
	//}
}

void c_life_cycle_state_handler::exit(c_life_cycle_state_handler* to)
{
	//INVOKE_CLASS_MEMBER(0x0048D280, c_life_cycle_state_handler, exit, to);

	event(_event_message, "networking:logic:life-cycle: exiting state %s", get_state_string());
}

c_life_cycle_state_manager* c_life_cycle_state_handler::get_manager() const
{
	//return INVOKE_CLASS_MEMBER(0x0048D350, c_life_cycle_state_handler, get_manager);

	ASSERT(m_manager != nullptr);
	return m_manager;
}

uns64 c_life_cycle_state_handler::get_required_group_session_parameter_mask() const
{
	//return INVOKE_CLASS_MEMBER(0x0048D700, c_life_cycle_state_handler, get_required_group_session_parameter_mask);

	return m_required_group_session_parameter_mask;
}

uns64 c_life_cycle_state_handler::get_required_squad_session_parameter_mask() const
{
	//return INVOKE_CLASS_MEMBER(0x0048D710, c_life_cycle_state_handler, get_required_squad_session_parameter_mask);

	return m_required_squad_session_parameter_mask;
}

e_life_cycle_state c_life_cycle_state_handler::get_state() const
{
	//return INVOKE_CLASS_MEMBER(0x0048D7C0, c_life_cycle_state_handler, get_state);

	return m_state;
}

void c_life_cycle_state_handler::handle_missing_required_session_parameter(e_network_session_type session_type)
{
	//INVOKE_CLASS_MEMBER(0x0048D7D0, c_life_cycle_state_handler, handle_missing_required_session_parameter, session_type);
}

void c_life_cycle_state_handler::initialize(c_life_cycle_state_manager* manager, e_life_cycle_state state, const c_life_cycle_state_handler_flags* flags, uns64 required_squad_session_parameter_mask, uns64 required_group_session_parameter_mask)
{
	//INVOKE_CLASS_MEMBER(0x0048D7E0, c_life_cycle_state_handler, initialize, manager, state, flags, required_squad_session_parameter_mask, m_required_group_session_parameter_mask);

	m_manager = manager;
	m_state = state;
	m_handler_flags = *flags;
	m_required_squad_session_parameter_mask = required_squad_session_parameter_mask;
	m_required_group_session_parameter_mask = required_group_session_parameter_mask;
	
	if (m_handler_flags.test(_life_cycle_state_handler_requires_group_session_bit))
	{
		ASSERT(m_handler_flags.test(_life_cycle_state_handler_allows_group_session_bit));
		ASSERT(m_handler_flags.test(_life_cycle_state_handler_group_session_disconnect_leaves_squad_bit) || m_handler_flags.test(_life_cycle_state_handler_group_session_disconnect_recreates_group_bit));
	}
	
	manager->register_state_handler(state, this);
}

//.text:0048D8A0 ; protected: static bool __cdecl c_life_cycle_state_handler::is_map_precached(const c_network_session*, uns32*)
//.text:0048D910 ; protected: void c_life_cycle_state_handler::matchmaking_update_required_files_for_hopper()
//.text:0048DAF0 ; protected: bool c_life_cycle_state_handler::pick_teams_for_group(bool)
//.text:0048DB10 ; private: bool c_life_cycle_state_handler::pick_teams_for_group_internal(bool, uns32)
//.text:0048DE90 ; protected: bool c_life_cycle_state_handler::session_composition_valid(const c_network_session*)
//.text:0048DEE0 ; protected: bool c_life_cycle_state_handler::session_has_minimum_player_count_to_start_game_in_hopper(const c_network_session*)

bool c_life_cycle_state_handler::setup_initial_participants(c_network_session* session)
{
	//return INVOKE_CLASS_MEMBER(0x0048E130, c_life_cycle_state_handler, setup_initial_participants, session);

	c_console::write_line("donkey:matchmaking c_life_cycle_state_handler::setup_initial_participants");
	if (!session->is_host())
	{
		return true;
	}

	bool result = false;
	HOOK_INVOKE_CLASS_MEMBER(result =, c_life_cycle_state_handler, setup_initial_participants, session);
	return result;
}

//.text:0048E3D0 ; protected: void c_life_cycle_state_handler::squad_session_host_abort_matchmaking()

bool c_life_cycle_state_handler::test_flag(e_life_cycle_state_handler_flags flag)
{
	//return INVOKE_CLASS_MEMBER(0x0048E490, c_life_cycle_state_handler, test_flag, flag);

	return m_handler_flags.test(flag);
}

//.text:0048E5D0 ; protected: void c_life_cycle_state_handler::update_advertised_session(const s_matchmaking_session_properties*)
//.text:0048E790 ; protected: void c_life_cycle_state_handler::update_group_qos_listener_data(s_qos_listener_data*)
//.text:0048EC60 ; protected: void c_life_cycle_state_handler::update_main_menu_precaching()
//.text:0048ECB0 ; protected: void c_life_cycle_state_handler::update_matchmaking_search_criteria(bool)
//.text:0048F0B0 ; protected: void c_life_cycle_state_handler::update_search_progress(const c_matchmaking_seeker*, bool, uns32)

