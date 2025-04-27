#include "motor/biped_ground_motor_program.hpp"

#include "game/cheats.hpp"
#include "memory/module.hpp"
#include "objects/objects.hpp"

//.text:00BE1B30 ; bool __cdecl biped_jump(int32)

__declspec(naked) void super_jump_inline()
{
	// original instructions
	//     push    0
	//     lea     eax, [ebp-0x40]
	//     push    eax
	//     push    esi
	//     call    object_get_velocities

	// all this for the following
	//if (cheat.super_jump && biped->unit.player_index != NONE)
	//    jump_velocity = jump_velocity * 4.0f;

	static real32 const velocity_multiplier = 4.0f;
	ASM_ADDR(0x00BE1C39, addr_BE1C39);

	__asm
	{
		// execute our instructions
		cmp     cheat.super_jump, 0
		jz      loc_BE1C2D
		cmp     dword ptr[edi+0x198], NONE
		jz      loc_BE1C2D

		movss   xmm0, [ebp-0x8]
		movss   xmm1, dword ptr[velocity_multiplier]
		mulss   xmm0, xmm1
		movss   [ebp-0x8], xmm0

		// execute the original instructions
	loc_BE1C2D:
		push    0
		lea     eax, [ebp-0x40]
		push    eax
		push    esi
		call    object_get_velocities

		// jump out to after our hook
		jmp     addr_BE1C39
	}
}
HOOK_DECLARE(0x00BE1C2D, super_jump_inline);

