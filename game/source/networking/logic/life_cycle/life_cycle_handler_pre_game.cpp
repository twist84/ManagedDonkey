#include "networking/logic/life_cycle/life_cycle_handler_pre_game.hpp"

#include "memory/module.hpp"

bool net_skip_countdown = false;

// #TODO: rewrite `c_life_cycle_state_handler_pre_game::host_update_countdown` 
__declspec(naked) void net_skip_countdown_inline()
{
	// original instructrions
	// loc_48F66A:
	//     test    eax, eax
	//     jnz     loc_48F7C1

	__asm
	{
		push    edx
		mov     dl, net_skip_countdown
		test    dl, dl
		jz      loc_48F66A

		mov     eax, 0

	loc_48F66A:
		pop     edx

		// execute the original instructrions
		test    eax, eax
		jnz     loc_48F7C1

		// jump out to after our hook
		mov     ecx, 0x0048F672
		jmp     ecx

	loc_48F7C1:
		mov     ecx, 0x0048F7C1
		jmp     ecx
	}
}
HOOK_DECLARE(0x0048F66A, net_skip_countdown_inline);

