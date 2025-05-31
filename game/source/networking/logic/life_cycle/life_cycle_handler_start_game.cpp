#include "networking/logic/life_cycle/life_cycle_handler_start_game.hpp"

c_life_cycle_state_handler_start_game::c_life_cycle_state_handler_start_game() :
	c_life_cycle_state_handler()
{
	//DECLFUNC(0x00454110, void, __thiscall, c_life_cycle_state_handler_start_game*)(this);
}

const char* c_life_cycle_state_handler_start_game::get_state_string()
{
	//return INVOKE_CLASS_MEMBER(0x004547E0, c_life_cycle_state_handler_start_game, get_state_string);

	return "start-game";
}

//.text:00490C00 ; private: static bool __cdecl c_life_cycle_state_handler_start_game::check_host(c_network_session*)

void c_life_cycle_state_handler_start_game::dispose()
{
	//INVOKE_CLASS_MEMBER(0x00490C20, c_life_cycle_state_handler_start_game, dispose);

	c_life_cycle_state_handler::dispose();
}

void c_life_cycle_state_handler_start_game::enter(c_life_cycle_state_handler* from, int32 entry_data_size, void* entry_data)
{
	INVOKE_CLASS_MEMBER(0x00490C30, c_life_cycle_state_handler_start_game, enter, from, entry_data_size, entry_data);
}

void c_life_cycle_state_handler_start_game::exit(c_life_cycle_state_handler* to)
{
	//INVOKE_CLASS_MEMBER(0x00490CA0, c_life_cycle_state_handler_start_game, exit, to);

	c_life_cycle_state_handler::exit(to);
}

void c_life_cycle_state_handler_start_game::initialize(c_life_cycle_state_manager* manager)
{
	//INVOKE_CLASS_MEMBER(0x00490CB0, c_life_cycle_state_handler_start_game, initialize, manager);

	c_life_cycle_state_handler_flags parent_flags;
	parent_flags.set(_life_cycle_state_handler_requires_squad_session_bit, true);
	c_life_cycle_state_handler::initialize(manager, _life_cycle_state_start_game, &parent_flags, 0x0ULL, 0x0ULL);
}

//.text:00490CE0 ; public: virtual bool c_generic_network_session_parameter<uns32>::set(const uns32*)
//.text:00490D70 ; public: virtual bool c_generic_network_session_parameter<e_language>::set(const e_language*)
//.text:00490E00 ; public: bool c_generic_network_session_parameter<uns64>::set(const uns64*)
//.text:00490EA0 ; private: void c_life_cycle_state_handler_start_game::set_unique_game_parameters()

void c_life_cycle_state_handler_start_game::update()
{
	INVOKE_CLASS_MEMBER(0x00490FB0, c_life_cycle_state_handler_start_game, update);
}

e_life_cycle_state_transition_type c_life_cycle_state_handler_start_game::update_for_state_transition()
{
	return INVOKE_CLASS_MEMBER(0x00491010, c_life_cycle_state_handler_start_game, update_for_state_transition);
}

