#include "networking/logic/life_cycle/life_cycle_handler_pre_game.hpp"

#include "game/multiplayer_game_hopper.hpp"
#include "memory/module.hpp"
#include "networking/logic/life_cycle/life_cycle_manager.hpp"
#include "networking/session/network_session.hpp"

bool net_skip_countdown = false;

// $TODO: rewrite `c_life_cycle_state_handler_pre_game::host_update_countdown` 
__declspec(naked) void life_cycle_state_handler_pre_game_host_update_countdown()
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
HOOK_DECLARE(0x0048F66A, life_cycle_state_handler_pre_game_host_update_countdown);

// $TODO: rewrite `c_life_cycle_state_handler_pre_game::update` 
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

