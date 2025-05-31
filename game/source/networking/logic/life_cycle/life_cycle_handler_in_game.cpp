#include "networking/logic/life_cycle/life_cycle_handler_in_game.hpp"

c_life_cycle_state_handler_in_game::c_life_cycle_state_handler_in_game() :
	c_life_cycle_state_handler(),
	m_flags(0),
	m_handled_simulation_abort(),
	m_time_of_handled_campaign_quit(),
	m_simulation_aborted_timestamp(),
	m_game_instance()
{
	//DECLFUNC(0x00453F10, void, __thiscall, c_life_cycle_state_handler_in_game*)(this);
}

const char* c_life_cycle_state_handler_in_game::get_state_string()
{
	//return INVOKE_CLASS_MEMBER(0x004546F0, c_life_cycle_state_handler_in_game, get_state_string);

	return "in-game";
}

//.text:004914D0 ; 
//.text:004914E0 ; 
//.text:004914F0 ; 
//.text:00491520 ; private: static bool __cdecl c_life_cycle_state_handler_in_game::begin_load_map(c_network_session*, const s_game_matchmaking_options*)

void c_life_cycle_state_handler_in_game::dispose()
{
	//INVOKE_CLASS_MEMBER(0x00491FD0, c_life_cycle_state_handler_in_game, dispose);

	c_life_cycle_state_handler::dispose();
}

//.text:00491FE0 ; private: static uns64 __cdecl c_life_cycle_state_handler_in_game::end_load_map(c_network_session*)

void c_life_cycle_state_handler_in_game::enter(c_life_cycle_state_handler* from, int32 entry_data_size, void* entry_data)
{
	INVOKE_CLASS_MEMBER(0x004920C0, c_life_cycle_state_handler_in_game, enter, from, entry_data_size, entry_data);
}

void c_life_cycle_state_handler_in_game::exit(c_life_cycle_state_handler* to)
{
	INVOKE_CLASS_MEMBER(0x00492140, c_life_cycle_state_handler_in_game, exit, to);
}

//.text:004921D0 ; public: const uns32* c_generic_network_session_parameter<uns32>::get() const
//.text:00492200 ; public: const e_language* c_generic_network_session_parameter<e_language>::get() const
//.text:00492230 ; public: const e_network_game_simulation_protocol* c_generic_network_session_parameter<e_network_game_simulation_protocol>::get() const
//.text:00492260 ; public: const uns64* c_generic_network_session_parameter<uns64>::get() const

void c_life_cycle_state_handler_in_game::initialize(c_life_cycle_state_manager* manager)
{
	//INVOKE_CLASS_MEMBER(0x00492290, c_life_cycle_state_handler_in_game, initialize, manager);

	c_life_cycle_state_handler_flags parent_flags;
	parent_flags.set(_life_cycle_state_handler_requires_squad_session_bit, true);
	c_life_cycle_state_handler::initialize(manager, _life_cycle_state_in_game, &parent_flags, 0x0ULL, 0x0ULL);
	m_game_instance = NONE;
}

bool c_life_cycle_state_handler_in_game::is_map_load_pending() const
{
	return m_flags.test(_life_cycle_state_handler_in_game_map_load);
}

//.text:004922D0 ; network_time_difference_msec
//.text:004922E0 ; network_time_get
//.text:00492300 ; network_time_get_exact
//.text:00492310 ; network_time_since

void c_life_cycle_state_handler_in_game::notify_core_load()
{
	//INVOKE_CLASS_MEMBER(0x00492330, c_life_cycle_state_handler_in_game, notify_core_load);

	m_flags.set(_life_cycle_state_handler_in_game_core_load_pending, true);
}

//.text:00492340 ; 
//.text:00492350 ; 
//.text:00492360 ; 
//.text:00492370 ; 
//.text:00492380 ; 
//.text:004923A0 ; public: void c_network_session::set_mode_based_maximum_player_count(int32)
//.text:004923C0 ; private: bool c_life_cycle_state_handler_in_game::squad_has_required_session_parameters()

void c_life_cycle_state_handler_in_game::update()
{
	INVOKE_CLASS_MEMBER(0x004923E0, c_life_cycle_state_handler_in_game, update);
}

e_life_cycle_state_transition_type c_life_cycle_state_handler_in_game::update_for_state_transition()
{
	return INVOKE_CLASS_MEMBER(0x00492960, c_life_cycle_state_handler_in_game, update_for_state_transition);
}

//.text:004929A0 ; public: static bool __cdecl c_life_cycle_state_handler_in_game::update_simulation_aborted(c_network_session*, bool*, uns32*)

