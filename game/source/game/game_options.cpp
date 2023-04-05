#include "game/game_options.hpp"

#include "cseries/console.hpp"

#include <string.h>

c_game_variant::c_game_variant()
{
	m_game_engine_index = _game_engine_base_variant;
	memset(m_variant_storage, 0, sizeof(m_variant_storage));
}

void c_game_variant::copy_from_and_validate(c_game_variant const* other)
{
	DECLFUNC(0x00573030, void, __thiscall, c_game_variant*, c_game_variant const*)(this, other);
}

void c_game_variant::copy_from_unsafe(class c_game_variant const* other)
{
	DECLFUNC(0x005730F0, void, __thiscall, c_game_variant*, c_game_variant const*)(this, other);
}

long c_game_variant::get_variant_size_for_game_engine_index(e_game_engine_type game_engine_index) const
{
	//DECLFUNC(0x0057A2E0, void, __thiscall, c_game_variant const*, e_game_engine_type)(this, game_engine_index);

	if (!VALID_INDEX(game_engine_index, k_game_engine_type_count))
		c_console::write_line("invalid game engine type #%ld requested variant size");

	long size = 0;
	switch (game_engine_index)
	{
	case _game_engine_base_variant:
		size = sizeof(c_game_engine_base_variant);
		break;
	case _game_engine_ctf_variant:
		size = sizeof(c_game_engine_ctf_variant);
		break;
	case _game_engine_slayer_variant:
		size = sizeof(c_game_engine_slayer_variant);
		break;
	case _game_engine_oddball_variant:
		size = sizeof(c_game_engine_oddball_variant);
		break;
	case _game_engine_king_variant:
		size = sizeof(c_game_engine_king_variant);
		break;
	case _game_engine_sandbox_variant:
		size = sizeof(c_game_engine_sandbox_variant);
		break;
	case _game_engine_vip_variant:
		size = sizeof(c_game_engine_vip_variant);
		break;
	case _game_engine_juggernaut_variant:
		size = sizeof(c_game_engine_juggernaut_variant);
		break;
	case _game_engine_territories_variant:
		size = sizeof(c_game_engine_territories_variant);
		break;
	case _game_engine_assault_variant:
		size = sizeof(c_game_engine_assault_variant);
		break;
	case _game_engine_infection_variant:
		size = sizeof(c_game_engine_infection_variant);
		break;
	}
	ASSERT(size <= k_maximum_game_engine_variant_size);

	return size;
}

bool c_game_variant::is_equal_to(c_game_variant const* other) const
{
	return DECLFUNC(0x0057A4C0, bool, __thiscall, c_game_variant const*, c_game_variant const*)(this, other);

	// #TODO
	//ASSERT(other != NULL);
	//
	//if (get_game_engine_index() != other->get_game_engine_index())
	//	return false;
	//
	//long structure_size = get_variant_size_for_game_engine_index(get_game_engine_index()) - 4;
	//ASSERT(structure_size > k_vtable_pointer_size);
}

void c_game_variant::recreate_variant_vtable_for_game_engine_index(e_game_engine_type game_engine_index)
{
	//DECLFUNC(0x0057A570, void, __thiscall, c_game_variant*, e_game_engine_type)(this, game_engine_index);

	if (!VALID_INDEX(game_engine_index, k_game_engine_type_count))
	{
		c_console::write_line("invalid game engine type #%ld, game options given c_game_engine_base_variant vtable in recreate vtable!", game_engine_index);
		game_engine_index = _game_engine_base_variant;
	}

	c_game_engine_base_variant* active_variant = get_active_variant_writeable();
	switch (game_engine_index)
	{
	case _game_engine_base_variant:
		*reinterpret_cast<void**>(active_variant) = reinterpret_cast<void*>(0x01659F38); // c_game_engine_base_variant::`vftable'
		active_variant->m_flags;
		break;
	case _game_engine_ctf_variant:
		*reinterpret_cast<void**>(active_variant) = reinterpret_cast<void*>(0x0165A058); // c_game_engine_ctf_variant::`vftable'
		active_variant->m_flags;
		break;
	case _game_engine_slayer_variant:
		*reinterpret_cast<void**>(active_variant) = reinterpret_cast<void*>(0x01659F68); // c_game_engine_slayer_variant::`vftable'
		active_variant->m_flags;
		break;
	case _game_engine_oddball_variant:
		*reinterpret_cast<void**>(active_variant) = reinterpret_cast<void*>(0x01659F98); // c_game_engine_oddball_variant::`vftable'
		active_variant->m_flags;
		break;
	case _game_engine_king_variant:
		*reinterpret_cast<void**>(active_variant) = reinterpret_cast<void*>(0x01659FC8); // c_game_engine_king_variant::`vftable'
		active_variant->m_flags;
		break;
	case _game_engine_sandbox_variant:
		*reinterpret_cast<void**>(active_variant) = reinterpret_cast<void*>(0x0165A118); // c_game_engine_sandbox_variant::`vftable'
		active_variant->m_flags;
		break;
	case _game_engine_vip_variant:
		*reinterpret_cast<void**>(active_variant) = reinterpret_cast<void*>(0x0165A0B8); // c_game_engine_vip_variant::`vftable'
		active_variant->m_flags;
		break;
	case _game_engine_juggernaut_variant:
		*reinterpret_cast<void**>(active_variant) = reinterpret_cast<void*>(0x0165A0E8); // c_game_engine_juggernaut_variant::`vftable'
		active_variant->m_flags;
		break;
	case _game_engine_territories_variant:
		*reinterpret_cast<void**>(active_variant) = reinterpret_cast<void*>(0x0165A028); // c_game_engine_territories_variant::`vftable'
		active_variant->m_flags;
		break;
	case _game_engine_assault_variant:
		*reinterpret_cast<void**>(active_variant) = reinterpret_cast<void*>(0x0165A088); // c_game_engine_assault_variant::`vftable'
		active_variant->m_flags;
		break;
	case _game_engine_infection_variant:
		*reinterpret_cast<void**>(active_variant) = reinterpret_cast<void*>(0x01659FF8); // c_game_engine_infection_variant::`vftable'
		active_variant->m_flags;
		break;
	}
}

e_game_engine_type c_game_variant::get_game_engine_index() const
{
	return m_game_engine_index;
}

void c_game_variant::set_game_engine_index(e_game_engine_type game_engine_index)
{
	DECLFUNC(0x0057AA70, void, __thiscall, c_game_variant*, e_game_engine_type)(this, game_engine_index);
}

c_game_engine_base_variant const* c_game_variant::get_active_variant() const
{
	return reinterpret_cast<c_game_engine_base_variant const*>(m_variant_storage);
}

c_game_engine_base_variant* c_game_variant::get_active_variant_writeable()
{
	return reinterpret_cast<c_game_engine_base_variant*>(m_variant_storage);
}

c_game_engine_ctf_variant const* c_game_variant::get_ctf_variant() const
{
	ASSERT(get_game_engine_index() == _game_engine_ctf_variant, "this variant is not ctf!");
	return static_cast<c_game_engine_ctf_variant const*>(get_active_variant());
}

c_game_engine_ctf_variant* c_game_variant::get_ctf_variant_writeable()
{
	ASSERT(get_game_engine_index() == _game_engine_ctf_variant, "this variant is not ctf!");
	return static_cast<c_game_engine_ctf_variant*>(get_active_variant_writeable());
}

c_game_engine_slayer_variant const* c_game_variant::get_slayer_variant() const
{
	ASSERT(get_game_engine_index() == _game_engine_slayer_variant, "this variant is not slayer!");
	return static_cast<c_game_engine_slayer_variant const*>(get_active_variant());
}

c_game_engine_slayer_variant* c_game_variant::get_slayer_variant_writeable()
{
	ASSERT(get_game_engine_index() == _game_engine_slayer_variant, "this variant is not slayer!");
	return static_cast<c_game_engine_slayer_variant*>(get_active_variant_writeable());
}

c_game_engine_oddball_variant const* c_game_variant::get_oddball_variant() const
{
	ASSERT(get_game_engine_index() == _game_engine_oddball_variant, "this variant is not oddball!");
	return static_cast<c_game_engine_oddball_variant const*>(get_active_variant());
}

c_game_engine_oddball_variant* c_game_variant::get_oddball_variant_writeable()
{
	ASSERT(get_game_engine_index() == _game_engine_oddball_variant, "this variant is not oddball!");
	return static_cast<c_game_engine_oddball_variant*>(get_active_variant_writeable());
}

c_game_engine_king_variant const* c_game_variant::get_king_variant() const
{
	ASSERT(get_game_engine_index() == _game_engine_king_variant, "this variant is not king of the hill!");
	return static_cast<c_game_engine_king_variant const*>(get_active_variant());
}

c_game_engine_king_variant* c_game_variant::get_king_variant_writeable()
{
	ASSERT(get_game_engine_index() == _game_engine_king_variant, "this variant is not king of the hill!");
	return static_cast<c_game_engine_king_variant*>(get_active_variant_writeable());
}

c_game_engine_sandbox_variant const* c_game_variant::get_sandbox_variant() const
{
	ASSERT(get_game_engine_index() == _game_engine_sandbox_variant, "this variant is not sandbox editor!");
	return static_cast<c_game_engine_sandbox_variant const*>(get_active_variant());
}

c_game_engine_sandbox_variant* c_game_variant::get_sandbox_variant_writeable()
{
	ASSERT(get_game_engine_index() == _game_engine_sandbox_variant, "this variant is not sandbox editor!");
	return static_cast<c_game_engine_sandbox_variant*>(get_active_variant_writeable());
}

c_game_engine_vip_variant const* c_game_variant::get_vip_variant() const
{
	ASSERT(get_game_engine_index() == _game_engine_vip_variant, "this variant is not vip!");
	return static_cast<c_game_engine_vip_variant const*>(get_active_variant());
}

c_game_engine_vip_variant* c_game_variant::get_vip_variant_writeable()
{
	ASSERT(get_game_engine_index() == _game_engine_vip_variant, "this variant is not vip!");
	return static_cast<c_game_engine_vip_variant*>(get_active_variant_writeable());
}

c_game_engine_juggernaut_variant const* c_game_variant::get_juggernaut_variant() const
{
	ASSERT(get_game_engine_index() == _game_engine_juggernaut_variant, "this variant is not juggernaut!");
	return static_cast<c_game_engine_juggernaut_variant const*>(get_active_variant());
}

c_game_engine_juggernaut_variant* c_game_variant::get_juggernaut_variant_writeable()
{
	ASSERT(get_game_engine_index() == _game_engine_juggernaut_variant, "this variant is not juggernaut!");
	return static_cast<c_game_engine_juggernaut_variant*>(get_active_variant_writeable());
}

c_game_engine_territories_variant const* c_game_variant::get_territories_variant() const
{
	ASSERT(get_game_engine_index() == _game_engine_territories_variant, "this variant is not territories!");
	return static_cast<c_game_engine_territories_variant const*>(get_active_variant());
}

c_game_engine_territories_variant* c_game_variant::get_territories_variant_writeable()
{
	ASSERT(get_game_engine_index() == _game_engine_territories_variant, "this variant is not territories!");
	return static_cast<c_game_engine_territories_variant*>(get_active_variant_writeable());
}

c_game_engine_assault_variant const* c_game_variant::get_assault_variant() const
{
	ASSERT(get_game_engine_index() == _game_engine_assault_variant, "this variant is not assault!");
	return static_cast<c_game_engine_assault_variant const*>(get_active_variant());
}

c_game_engine_assault_variant* c_game_variant::get_assault_variant_writeable()
{
	ASSERT(get_game_engine_index() == _game_engine_assault_variant, "this variant is not assault!");
	return static_cast<c_game_engine_assault_variant*>(get_active_variant_writeable());
}

c_game_engine_infection_variant const* c_game_variant::get_infection_variant() const
{
	ASSERT(get_game_engine_index() == _game_engine_infection_variant, "this variant is not infection!");
	return static_cast<c_game_engine_infection_variant const*>(get_active_variant());
}

c_game_engine_infection_variant* c_game_variant::get_infection_variant_writeable()
{
	ASSERT(get_game_engine_index() == _game_engine_infection_variant, "this variant is not infection!");
	return static_cast<c_game_engine_infection_variant*>(get_active_variant_writeable());
}

bool c_game_variant::get_integer_game_engine_setting(e_game_variant_parameter parameter, long* out_value) const
{
	return get_game_engine_setting(parameter, _text_value_pair_parameter_type_integer, out_value);
}

bool c_game_variant::set_integer_game_engine_setting(e_game_variant_parameter parameter, long value)
{
	return set_game_engine_setting(parameter, _text_value_pair_parameter_type_integer, value);
}

bool c_game_variant::get_string_id_game_engine_setting(e_game_variant_parameter parameter, long* out_value) const
{
	return get_game_engine_setting(parameter, _text_value_pair_parameter_type_string_id, out_value);
}

bool c_game_variant::set_string_id_game_engine_setting(e_game_variant_parameter parameter, long value)
{
	return set_game_engine_setting(parameter, _text_value_pair_parameter_type_string_id, value);
}

bool c_game_variant::get_game_engine_setting(e_game_variant_parameter parameter, e_text_value_pair_parameter_type parameter_type, long* out_value) const
{
	return DECLFUNC(0x00574530, bool, __thiscall, c_game_variant const*, e_game_variant_parameter, e_text_value_pair_parameter_type, long*)(this, parameter, parameter_type, out_value);
}

bool c_game_variant::set_game_engine_setting(e_game_variant_parameter parameter, e_text_value_pair_parameter_type parameter_type, long value)
{
	return DECLFUNC(0x0057AAB0, bool, __thiscall, c_game_variant*, e_game_variant_parameter, e_text_value_pair_parameter_type, long)(this, parameter, parameter_type, value);
}

