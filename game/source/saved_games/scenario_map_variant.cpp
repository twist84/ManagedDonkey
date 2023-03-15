#include "saved_games/scenario_map_variant.hpp"

#include <string.h>

c_map_variant::c_map_variant()
{
	memset(this, 0, sizeof(*this));

	DECLFUNC(0x00580B80, void, __thiscall, c_map_variant*)(this);
}

//void __cdecl c_map_variant::create_default(e_map_id map_id)
void __cdecl c_map_variant::create_default(long map_id)
{
	DECLFUNC(0x00581F70, void, __thiscall, c_map_variant*, long)(this, map_id);
}

bool __cdecl c_map_variant::validate()
{
	return DECLFUNC(0x00586D00, bool, __thiscall, c_map_variant*)(this);
}

