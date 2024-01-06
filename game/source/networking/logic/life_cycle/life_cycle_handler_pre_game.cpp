#include "networking/logic/life_cycle/life_cycle_handler_pre_game.hpp"

#include "memory/module.hpp"

bool net_skip_countdown = false;

void c_life_cycle_state_handler_pre_game::update()
{
	DECLFUNC(0x004905F0, void, __thiscall, c_life_cycle_state_handler_pre_game*)(this);
}

e_life_cycle_state_transition_type c_life_cycle_state_handler_pre_game::update_for_state_transition()
{
	return DECLFUNC(0x00490680, e_life_cycle_state_transition_type, __thiscall, c_life_cycle_state_handler_pre_game*)(this);
}

void c_life_cycle_state_handler_pre_game::enter(c_life_cycle_state_handler* handler, long entry_data_size, void* entry_data)
{
	DECLFUNC(0x0048F290, void, __thiscall, c_life_cycle_state_handler_pre_game*, c_life_cycle_state_handler*, long, void*)(this, handler, entry_data_size, entry_data);
}

void c_life_cycle_state_handler_pre_game::exit(c_life_cycle_state_handler* handler)
{
	DECLFUNC(0x0048F340, void, __thiscall, c_life_cycle_state_handler_pre_game*, c_life_cycle_state_handler*)(this, handler);
}

char const* c_life_cycle_state_handler_pre_game::get_state_string()
{
	//return DECLFUNC(0x004547D0, char const*, __thiscall, c_life_cycle_state_handler_pre_game*)(this);

	return "pre-game";
}

void c_life_cycle_state_handler_pre_game::initialize(c_life_cycle_state_manager* manager)
{
	//DECLFUNC(0x0048F7E0, void, __thiscall, c_life_cycle_state_handler_pre_game*, c_life_cycle_state_manager*)(this, manager);

	c_flags<e_life_cycle_state_handler_flags, byte, k_life_cycle_state_handler_flags> handler_flags{};
	handler_flags.set(_life_cycle_state_handler_unknown_bit0, true);

	c_life_cycle_state_handler::initialize(manager, _life_cycle_state_pre_game, &handler_flags, 0, 0);
}

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

			loc_48F7C1 :
		mov     ecx, 0x0048F7C1
			jmp     ecx
	}
}
HOOK_DECLARE(0x0048F66A, net_skip_countdown_inline);

