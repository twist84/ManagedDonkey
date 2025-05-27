#include "networking/logic/life_cycle/life_cycle_handler_matchmaking_assemble_match.hpp"

c_life_cycle_state_handler_matchmaking_assemble_match::c_life_cycle_state_handler_matchmaking_assemble_match() :
	c_life_cycle_state_handler(),
	m_flags(0),
	m_gather_start_time(),
	m_ready_to_start_time(),
	m_advertised_session_properties(),
	m_qos_listener_stats_query_time(),
	m_last_qos_listener_data_replies_sent(),
	m_qos_listener_data()
{
	//DECLFUNC(0x00453FB0, void, __thiscall, c_life_cycle_state_handler_matchmaking_assemble_match*)(this);
}

char const* c_life_cycle_state_handler_matchmaking_assemble_match::get_state_string()
{
	//return INVOKE_CLASS_MEMBER(0x00454740, c_life_cycle_state_handler_matchmaking_assemble_match, get_state_string);

	return "matchmaking-assemble-match";
}

//.text:00497080 ; private: bool c_life_cycle_state_handler_matchmaking_assemble_match::clients_ready_to_start()
//.text:004970A0 ; private: bool c_life_cycle_state_handler_matchmaking_assemble_match::continue_assembling()
//.text:00497270 ; public: void c_life_cycle_state_handler_matchmaking_assemble_match::dispose()

void c_life_cycle_state_handler_matchmaking_assemble_match::enter(c_life_cycle_state_handler* from, int32 entry_data_size, void* entry_data)
{
	INVOKE_CLASS_MEMBER(0x00497280, c_life_cycle_state_handler_matchmaking_assemble_match, enter, from, entry_data_size, entry_data);
}

void c_life_cycle_state_handler_matchmaking_assemble_match::exit(c_life_cycle_state_handler* to)
{
	INVOKE_CLASS_MEMBER(0x00497370, c_life_cycle_state_handler_matchmaking_assemble_match, exit, to);
}

//.text:00497450 ; 
//.text:00497460 ; public: int32 c_network_session_membership::get_established_player_count() const
//.text:004974C0 ; 
//.text:004974D0 ; public: bool c_life_cycle_state_handler_matchmaking_assemble_match::get_gather_party_properties(s_matchmaking_gather_party_properties*)
//.text:004975C0 ; 
//.text:004975D0 ; private: uns32 c_life_cycle_state_handler_matchmaking_assemble_match::get_peer_mask_to_evict_for_team_balance()
//.text:00497760 ; public: void c_life_cycle_state_handler_matchmaking_assemble_match::get_progress(s_life_cycle_matchmaking_progress*)

void c_life_cycle_state_handler_matchmaking_assemble_match::handle_missing_required_session_parameter(e_network_session_type session_type)
{
	INVOKE_CLASS_MEMBER(0x00497920, c_life_cycle_state_handler_matchmaking_assemble_match, handle_missing_required_session_parameter, session_type);
}

void c_life_cycle_state_handler_matchmaking_assemble_match::initialize(c_life_cycle_state_manager* manager)
{
	INVOKE_CLASS_MEMBER(0x00497960, c_life_cycle_state_handler_matchmaking_assemble_match, initialize, manager);
}

//.text:00497990 ; 
//.text:004979A0 ; 
//.text:004979C0 ; 
//.text:004979D0 ; 
//.text:004979F0 ; public: void c_life_cycle_state_handler_matchmaking_assemble_match::notify_qos_reply_block_set()
//.text:00497A00 ; private: bool c_life_cycle_state_handler_matchmaking_assemble_match::pick_games()
//.text:00497AE0 ; private: bool c_life_cycle_state_handler_matchmaking_assemble_match::ready_to_lockdown()
//.text:00497BD0 ; private: bool c_life_cycle_state_handler_matchmaking_assemble_match::ready_to_start()
//.text:00497C80 ; 
//.text:00497CA0 ; public: bool c_generic_network_session_parameter<s_game_hopper_picked_game_collection>::set(s_game_hopper_picked_game_collection const*)
//.text:00497D50 ; 
//.text:00497D60 ; 

void c_life_cycle_state_handler_matchmaking_assemble_match::update()
{
	INVOKE_CLASS_MEMBER(0x00497D80, c_life_cycle_state_handler_matchmaking_assemble_match, update);
}

e_life_cycle_state_transition_type c_life_cycle_state_handler_matchmaking_assemble_match::update_for_state_transition()
{
	return INVOKE_CLASS_MEMBER(0x00498130, c_life_cycle_state_handler_matchmaking_assemble_match, update_for_state_transition);
}

//.text:004981E0 ; private: void c_life_cycle_state_handler_matchmaking_assemble_match::update_progress()

