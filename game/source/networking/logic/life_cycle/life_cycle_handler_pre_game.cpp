#include "networking/logic/life_cycle/life_cycle_handler_pre_game.hpp"

#include "memory/module.hpp"

bool net_skip_countdown = false;

// #TODO: rewrite `c_life_cycle_state_handler_pre_game::host_update_countdown` 
__declspec(naked) void net_skip_countdown_inline()
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
HOOK_DECLARE(0x0048F66A, net_skip_countdown_inline);

