#include "networking/logic/life_cycle/life_cycle_handler_matchmaking_prepare_map.hpp"

c_life_cycle_state_handler_matchmaking_prepare_map::c_life_cycle_state_handler_matchmaking_prepare_map() :
	c_life_cycle_state_handler(),
	m_flags(0),
	m_prepare_map_start_time(),
	m_countdown_start_time(),
	m_loading_start_time()
{
	//DECLFUNC(0x00454070, void, __thiscall, c_life_cycle_state_handler_matchmaking_prepare_map*)(this);
}

char const* c_life_cycle_state_handler_matchmaking_prepare_map::get_state_string()
{
	//return INVOKE_CLASS_MEMBER(0x00454780, c_life_cycle_state_handler_matchmaking_prepare_map, get_state_string);

	return "matchmaking-prepare-map";
}

//.text:004983B0 ; public: void c_life_cycle_state_handler_matchmaking_prepare_map::dispose()

void c_life_cycle_state_handler_matchmaking_prepare_map::enter(c_life_cycle_state_handler* from, int32 entry_data_size, void* entry_data)
{
	INVOKE_CLASS_MEMBER(0x004983C0, c_life_cycle_state_handler_matchmaking_prepare_map, enter, from, entry_data_size, entry_data);
}

void c_life_cycle_state_handler_matchmaking_prepare_map::exit(c_life_cycle_state_handler* to)
{
	INVOKE_CLASS_MEMBER(0x00498450, c_life_cycle_state_handler_matchmaking_prepare_map, exit, to);
}

//.text:00498500 ; 
//.text:00498510 ; public: s_game_hopper_picked_game_collection const* c_generic_network_session_parameter<s_game_hopper_picked_game_collection>::get() const
//.text:00498540 ; 
//.text:00498550 ; public: void c_life_cycle_state_handler_matchmaking_prepare_map::get_progress(s_life_cycle_matchmaking_progress*)
//.text:00498760 ; private: void c_life_cycle_state_handler_matchmaking_prepare_map::get_state_from_progress(s_replicated_life_cycle_matchmaking_progress_configuring_match*)

void c_life_cycle_state_handler_matchmaking_prepare_map::handle_missing_required_session_parameter(e_network_session_type session_type)
{
	INVOKE_CLASS_MEMBER(0x00498800, c_life_cycle_state_handler_matchmaking_prepare_map, handle_missing_required_session_parameter, session_type);
}

void c_life_cycle_state_handler_matchmaking_prepare_map::initialize(c_life_cycle_state_manager* manager)
{
	INVOKE_CLASS_MEMBER(0x00498820, c_life_cycle_state_handler_matchmaking_prepare_map, initialize, manager);
}

//.text:00498850 ; 
//.text:00498860 ; 
//.text:00498880 ; 
//.text:00498890 ; 
//.text:004988B0 ; private: void c_life_cycle_state_handler_matchmaking_prepare_map::pick_next_game_and_map(s_replicated_life_cycle_matchmaking_progress_configuring_match*)
//.text:00498900 ; private: void c_life_cycle_state_handler_matchmaking_prepare_map::request_next_game_and_map(int32)
//.text:004989E0 ; 
//.text:00498A00 ; 

void c_life_cycle_state_handler_matchmaking_prepare_map::update()
{
	INVOKE_CLASS_MEMBER(0x00498A20, c_life_cycle_state_handler_matchmaking_prepare_map, update);
}

//.text:00498B30 ; private: void c_life_cycle_state_handler_matchmaking_prepare_map::update_countdown(s_replicated_life_cycle_matchmaking_progress_configuring_match*)

e_life_cycle_state_transition_type c_life_cycle_state_handler_matchmaking_prepare_map::update_for_state_transition()
{
	return INVOKE_CLASS_MEMBER(0x00498B30, c_life_cycle_state_handler_matchmaking_prepare_map, update_for_state_transition);
}

//.text:00498D20 ; private: void c_life_cycle_state_handler_matchmaking_prepare_map::update_game_and_map_download(s_replicated_life_cycle_matchmaking_progress_configuring_match*)
//.text:00498ED0 ; private: void c_life_cycle_state_handler_matchmaking_prepare_map::update_game_variant_download(s_game_set_file const*)
//.text:00498FC0 ; private: void c_life_cycle_state_handler_matchmaking_prepare_map::update_loading(s_replicated_life_cycle_matchmaking_progress_configuring_match*)
//.text:00499350 ; private: bool c_life_cycle_state_handler_matchmaking_prepare_map::update_map_variant_download(int32)
//.text:00499470 ; private: void c_life_cycle_state_handler_matchmaking_prepare_map::update_progress(s_replicated_life_cycle_matchmaking_progress_configuring_match const*)
//.text:004994E0 ; private: void c_life_cycle_state_handler_matchmaking_prepare_map::update_veto(s_replicated_life_cycle_matchmaking_progress_configuring_match*)

