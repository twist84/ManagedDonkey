#include "networking/logic/life_cycle/life_cycle_handler_pre_game.hpp"

#include "game/multiplayer_game_hopper.hpp"
#include "interface/user_interface_error_manager.hpp"
#include "memory/module.hpp"
#include "networking/logic/life_cycle/life_cycle_manager.hpp"
#include "networking/session/network_session.hpp"

bool net_skip_countdown = false;

c_life_cycle_state_handler_pre_game::c_life_cycle_state_handler_pre_game() :
	c_life_cycle_state_handler(),
	m_last_countdown_timestamp(),
	m_last_countdown_timer(),
	m_game_start_timestamp()
{
	//DECLFUNC(0x00454100, void, __thiscall, c_life_cycle_state_handler_pre_game*)(this);
}

const char* c_life_cycle_state_handler_pre_game::get_state_string()
{
	//return INVOKE_CLASS_MEMBER(0x004547D0, c_life_cycle_state_handler_pre_game, get_state_string);

	return "pre-game";
}

void c_life_cycle_state_handler_pre_game::dispose()
{
	//INVOKE_CLASS_MEMBER(0x0048F280, c_life_cycle_state_handler_pre_game, dispose);

	c_life_cycle_state_handler::dispose();
}

void c_life_cycle_state_handler_pre_game::enter(c_life_cycle_state_handler* from, int32 entry_data_size, void* entry_data)
{
	INVOKE_CLASS_MEMBER(0x0048F290, c_life_cycle_state_handler_pre_game, enter, from, entry_data_size, entry_data);
}

void c_life_cycle_state_handler_pre_game::exit(c_life_cycle_state_handler* to)
{
	INVOKE_CLASS_MEMBER(0x0048F340, c_life_cycle_state_handler_pre_game, exit, to);

	//c_network_session* active_squad_session = get_manager()->get_active_squad_session();
	////active_squad_session->m_enable_speculative_host_migration = false;
	//if (active_squad_session->is_host())
	//{
	//	s_network_session_parameter_game_start_status status
	//	{
	//		.game_start_status = _session_game_start_status_not_pregame,
	//		.game_start_error = _session_game_start_error_none,
	//		.player_error_mask = 0,
	//		.map_load_progress = 0,
	//	};
	//	active_squad_session->get_session_parameters()->game_start_status.set(&status);
	//}
	//network_squad_session_request_campaign_quit(_campaign_quit_reason_none, NONE);
	//user_interface_error_manager_get()->clear_error(STRING_ID(gui_alert, player_quit_from_campaign_error), k_any_controller);
	//user_interface_error_manager_get()->clear_error(STRING_ID(gui_alert, player_dropped_from_campaign_error), k_any_controller);
	//c_life_cycle_state_handler::exit(to);
}

//.text:0048F570 ; private: void c_life_cycle_state_handler_pre_game::host_update_countdown()

void c_life_cycle_state_handler_pre_game::initialize(c_life_cycle_state_manager* manager)
{
	//INVOKE_CLASS_MEMBER(0x0048F7E0, c_life_cycle_state_handler_pre_game, initialize, manager);

	c_life_cycle_state_handler_flags parent_flags;
	parent_flags.set(_life_cycle_state_handler_requires_squad_session_bit, true);
	c_life_cycle_state_handler::initialize(manager, _life_cycle_state_pre_game, &parent_flags, 0x0ULL, 0x0ULL);
}

//.text:0048F810 ; 
//.text:0048F820 ; private: void c_life_cycle_state_handler_pre_game::leader_update_countdown()
//.text:0048F8D0 ; private: void c_life_cycle_state_handler_pre_game::matchmaking_host_update()
//.text:0048FD40 ; 
//.text:0048FD50 ; 
//.text:0048FD70 ; 
//.text:0048FD80 ; 
//.text:0048FDA0 ; 
//.text:0048FDB0 ; 
//.text:0048FDC0 ; 
//.text:0048FDE0 ; private: void c_life_cycle_state_handler_pre_game::squad_game_start_status_update()

void c_life_cycle_state_handler_pre_game::update()
{
	INVOKE_CLASS_MEMBER(0x004905F0, c_life_cycle_state_handler_pre_game, update);

	//c_network_session* active_squad_session = get_manager()->get_active_squad_session();
	//if (network_squad_session_get_start_mode() == _network_game_start_mode_matchmaking)
	//{
	//	multiplayer_game_hopper_update_in_matchmaking(active_squad_session->is_leader());
	//	if (active_squad_session->is_host())
	//	{
	//		matchmaking_host_update();
	//	}
	//	matchmaking_update_required_files_for_hopper();
	//}
	//
	//const c_network_session_membership* session_membership = active_squad_session->get_session_membership();
	//
	//if (session_membership->is_leader())
	//{
	//	clean_disallowed_parameters();
	//}
	//
	//if (active_squad_session->is_host())
	//{
	//	squad_game_start_status_update();
	//}
	//
	//switch (active_squad_session->session_mode())
	//{
	//case _network_session_mode_idle:
	//{
	//	if (session_membership->is_leader())
	//	{
	//		leader_update_countdown();
	//	}
	//
	//	if (active_squad_session->is_host())
	//	{
	//		host_update_countdown();
	//	}
	//}
	//break;
	//default:
	//{
	//	VASSERT("unreachable");
	//}
	//break;
	//}
}

e_life_cycle_state_transition_type c_life_cycle_state_handler_pre_game::update_for_state_transition()
{
	return INVOKE_CLASS_MEMBER(0x00490680, c_life_cycle_state_handler_pre_game, update_for_state_transition);
}

// $TODO rewrite `c_life_cycle_state_handler_pre_game::host_update_countdown` 
__declspec(naked) void life_cycle_state_handler_pre_game_host_update_countdown_for_net_skip_countdown()
{
	// original instructions
	// loc_48F66A:
	//     test    eax, eax
	//     jnz     loc_48F7C1

	ASM_ADDR(0x0048F672, addr_48F672);
	ASM_ADDR(0x0048F7C1, addr_48F7C1);

	__asm
	{
		// execute our instructions
		push    edx
		mov     dl, net_skip_countdown
		test    dl, dl
		jz      loc_48F66A

		mov     eax, 0

	loc_48F66A:
		pop     edx

		// execute the original instructions
		test    eax, eax
		jnz     loc_48F7C1

		// jump out to after our hook
		jmp     addr_48F672

	loc_48F7C1:
		jmp     addr_48F7C1
	}
}
HOOK_DECLARE(0x0048F66A, life_cycle_state_handler_pre_game_host_update_countdown_for_net_skip_countdown);

// $TODO rewrite `c_life_cycle_state_handler_pre_game::host_update_countdown` 
__declspec(naked) void life_cycle_state_handler_pre_game_host_update_countdown_for_saved_films()
{
	// prevent countdown from stopping
	
	// run cleanup and return
	__asm
	{
		pop     edi
		pop     esi
		pop     ebx
		mov     esp, ebp
		pop     ebp
		retn
	}
}
HOOK_DECLARE(0x0048F704, life_cycle_state_handler_pre_game_host_update_countdown_for_saved_films);

// $TODO rewrite `c_life_cycle_state_handler_pre_game::update` 
__declspec(naked) void life_cycle_state_handler_pre_game_update()
{
	ASM_ADDR(0x00490604, addr_490604);
	ASM_ADDR(0x004391C0, network_squad_session_get_start_mode);

	__asm
	{
		// execute the original instructions
		mov     eax, [esi + 0x0C]
		mov     edi, [eax + 0x50]
		call    network_squad_session_get_start_mode
		cmp     eax, _network_game_start_mode_matchmaking
		jnz     loc_490604

		// execute our instructions
		cmp[edi + 0x28], NONE
		jz      loc_490604

		mov     eax, [edi + 0x1A3D40]
		cmp     eax, [edi + 0x2C]
		setz    al
		test    al, al
		jz      loc_490604

		push    eax
		call    multiplayer_game_hopper_update_in_matchmaking
		pop     eax

		// jump out to after our hook
	loc_490604:
		jmp     addr_490604
	}
}
HOOK_DECLARE(0x004905FA, life_cycle_state_handler_pre_game_update);

