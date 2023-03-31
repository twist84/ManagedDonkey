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


bool c_map_variant::read_from(c_map_variant const* source)
{
	return DECLFUNC(0x005860F0, bool, __thiscall, c_map_variant*, c_map_variant const*)(this, source);
}

bool __cdecl c_map_variant::validate()
{
	return DECLFUNC(0x00586D00, bool, __thiscall, c_map_variant*)(this);
}

bool c_map_variant::is_valid() const
{
	return DECLFUNC(0x005841D0, bool, __thiscall, c_map_variant const*)(this);
}

//e_map_id c_map_variant::get_map_id() const
long c_map_variant::get_map_id() const
{
	return m_map_id;
}

