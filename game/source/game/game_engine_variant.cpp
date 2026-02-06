#include "game/game_engine_variant.hpp"

#include "game/game_engine.hpp"
#include "main/main_game.hpp"
#include "memory/bitstream.hpp"
#include "memory/byte_swapping.hpp"

c_game_variant::c_game_variant()
{
	m_game_engine_index = _game_engine_type_none;
	csmemset(m_variant_storage, 0, sizeof(m_variant_storage));
}

void c_game_variant::encode_to_mcc(c_bitstream* packet) const
{
	ASSERT(packet);

	packet->write_integer("variant-game-engine-index", get_game_engine_index(), 4);
	if (m_game_engine_index)
	{
		switch (get_game_engine_index())
		{
		case _game_engine_type_none:
			m_base_variant.encode_to_mcc(packet);
			break;
		case _game_engine_type_ctf:
			m_ctf_variant.encode_to_mcc(packet);
			break;
		case _game_engine_type_slayer:
			m_slayer_variant.encode_to_mcc(packet);
			break;
		case _game_engine_type_oddball:
			m_oddball_variant.encode_to_mcc(packet);
			break;
		case _game_engine_type_king:
			m_king_variant.encode_to_mcc(packet);
			break;
		case _game_engine_type_sandbox:
			m_sandbox_variant.encode_to_mcc(packet);
			break;
		case _game_engine_type_vip:
			m_vip_variant.encode_to_mcc(packet);
			break;
		case _game_engine_type_juggernaut:
			m_juggernaut_variant.encode_to_mcc(packet);
			break;
		case _game_engine_type_territories:
			m_territories_variant.encode_to_mcc(packet);
			break;
		case _game_engine_type_assault:
			m_assault_variant.encode_to_mcc(packet);
			break;
		case _game_engine_type_infection:
			m_infection_variant.encode_to_mcc(packet);
			break;
		}
	}
}

bool c_game_variant::decode_from_mcc(c_bitstream* packet)
{
	ASSERT(packet);

	e_game_engine_type game_engine_index = packet->read_enum<e_game_engine_type, 4>("variant-game-engine-index");
	set_game_engine_index(game_engine_index);

	//if (m_game_engine_index == _game_engine_type_none)
	//	return false;

	m_base_variant.set_built_in(false);
	switch (game_engine_index)
	{
	case _game_engine_type_none:
		m_base_variant.decode_from_mcc(packet);
		break;
	case _game_engine_type_ctf:
		m_ctf_variant.decode_from_mcc(packet);
		break;
	case _game_engine_type_slayer:
		m_slayer_variant.decode_from_mcc(packet);
		break;
	case _game_engine_type_oddball:
		m_oddball_variant.decode_from_mcc(packet);
		break;
	case _game_engine_type_king:
		m_king_variant.decode_from_mcc(packet);
		break;
	case _game_engine_type_sandbox:
		m_sandbox_variant.decode_from_mcc(packet);
		break;
	case _game_engine_type_vip:
		m_vip_variant.decode_from_mcc(packet);
		break;
	case _game_engine_type_juggernaut:
		m_juggernaut_variant.decode_from_mcc(packet);
		break;
	case _game_engine_type_territories:
		m_territories_variant.decode_from_mcc(packet);
		break;
	case _game_engine_type_assault:
		m_assault_variant.decode_from_mcc(packet);
		break;
	case _game_engine_type_infection:
		m_infection_variant.decode_from_mcc(packet);
		break;
	}

	return true;
}

void c_game_variant::copy_from_and_validate(const c_game_variant* other)
{
	INVOKE_CLASS_MEMBER(0x00573030, c_game_variant, copy_from_and_validate, other);
}

void c_game_variant::copy_from_unsafe(const c_game_variant* other)
{
	INVOKE_CLASS_MEMBER(0x005730F0, c_game_variant, copy_from_unsafe, other);
}

bool c_game_variant::decode(c_bitstream* packet)
{
	return INVOKE_CLASS_MEMBER(0x00573340, c_game_variant, decode, packet);

	//ASSERT(packet);
	//
	//m_game_engine_index = packet->read_integer("variant-game-engine-index", 4);
	//set_game_engine_index(m_game_engine_index);
	//
	//if (m_game_engine_index == _game_engine_type_none)
	//	return false;
	//
	//get_active_variant_writeable()->set_built_in(false);
	//get_active_variant_writeable()->decode(packet);
	//
	//return true;
}

void c_game_variant::encode(c_bitstream* packet) const
{
	INVOKE_CLASS_MEMBER(0x005733C0, c_game_variant, encode, packet);

	//ASSERT(packet);
	//
	//e_game_engine_type game_engine_index = m_game_engine_index;
	//packet->write_integer("variant-game-engine-index", game_engine_index, 4);
	//
	//if (game_engine_index)
	//	get_active_variant()->encode(packet);
}

int32 c_game_variant::get_variant_size_for_game_engine_index(e_game_engine_type game_engine_index) const
{
	//INVOKE_CLASS_MEMBER(0x0057A2E0, c_game_variant, get_variant_size_for_game_engine_index, game_engine_index);

	if (!VALID_INDEX(game_engine_index, k_game_engine_type_count))
	{
		//event(_event_warning, "invalid game engine type #%ld requested variant size", game_engine_index);
		c_console::write_line("invalid game engine type #%ld requested variant size", game_engine_index);
	}

	int32 size = 0;
	switch (game_engine_index)
	{
	case _game_engine_type_none:
		size = sizeof(c_game_engine_base_variant);
		break;
	case _game_engine_type_ctf:
		size = sizeof(c_game_engine_ctf_variant);
		break;
	case _game_engine_type_slayer:
		size = sizeof(c_game_engine_slayer_variant);
		break;
	case _game_engine_type_oddball:
		size = sizeof(c_game_engine_oddball_variant);
		break;
	case _game_engine_type_king:
		size = sizeof(c_game_engine_king_variant);
		break;
	case _game_engine_type_sandbox:
		size = sizeof(c_game_engine_sandbox_variant);
		break;
	case _game_engine_type_vip:
		size = sizeof(c_game_engine_vip_variant);
		break;
	case _game_engine_type_juggernaut:
		size = sizeof(c_game_engine_juggernaut_variant);
		break;
	case _game_engine_type_territories:
		size = sizeof(c_game_engine_territories_variant);
		break;
	case _game_engine_type_assault:
		size = sizeof(c_game_engine_assault_variant);
		break;
	case _game_engine_type_infection:
		size = sizeof(c_game_engine_infection_variant);
		break;
	}
	ASSERT(size <= k_maximum_game_engine_variant_size);

	return size;
}

bool c_game_variant::is_equal_to(const c_game_variant* other) const
{
	return INVOKE_CLASS_MEMBER(0x0057A4C0, c_game_variant, is_equal_to, other);

	// $TODO
	//ASSERT(other != nullptr);
	//
	//if (get_game_engine_index() != other->get_game_engine_index())
	//	return false;
	//
	//int32 structure_size = get_variant_size_for_game_engine_index(get_game_engine_index()) - 4;
	//ASSERT(structure_size > k_vtable_pointer_size);
}

void c_game_variant::recreate_variant_vtable_for_game_engine_index(e_game_engine_type game_engine_index)
{
	//INVOKE_CLASS_MEMBER(0x0057A570, c_game_variant, recreate_variant_vtable_for_game_engine_index, game_engine_index);

	if (!VALID_INDEX(game_engine_index, k_game_engine_type_count))
	{
		//event(_event_warning, "invalid game engine type #%ld, game options given c_game_engine_base_variant vtable in recreate vtable!", game_engine_index);
		c_console::write_line("invalid game engine type #%ld, game options given c_game_engine_base_variant vtable in recreate vtable!", game_engine_index);
		game_engine_index = _game_engine_type_none;
	}

	switch (game_engine_index)
	{
	case _game_engine_type_none:
		m_base_variant.constructor();
		break;
	case _game_engine_type_ctf:
		m_ctf_variant.constructor();
		break;
	case _game_engine_type_slayer:
		m_slayer_variant.constructor();
		break;
	case _game_engine_type_oddball:
		m_oddball_variant.constructor();
		break;
	case _game_engine_type_king:
		m_king_variant.constructor();
		break;
	case _game_engine_type_sandbox:
		m_sandbox_variant.constructor();
		break;
	case _game_engine_type_vip:
		m_vip_variant.constructor();
		break;
	case _game_engine_type_juggernaut:
		m_juggernaut_variant.constructor();
		break;
	case _game_engine_type_territories:
		m_territories_variant.constructor();
		break;
	case _game_engine_type_assault:
		m_assault_variant.constructor();
		break;
	case _game_engine_type_infection:
		m_infection_variant.constructor();
		break;
	}
}

e_game_engine_type c_game_variant::get_game_engine_index() const
{
	return m_game_engine_index;
}

void c_game_variant::set_game_engine_index(e_game_engine_type game_engine_index)
{
	return INVOKE_CLASS_MEMBER(0x0057AA70, c_game_variant, set_game_engine_index, game_engine_index);
}

const c_game_engine_base_variant* c_game_variant::get_active_variant() const
{
	return reinterpret_cast<const c_game_engine_base_variant*>(m_variant_storage);
}

c_game_engine_base_variant* c_game_variant::get_active_variant_writeable()
{
	return reinterpret_cast<c_game_engine_base_variant*>(m_variant_storage);
}

const c_game_engine_ctf_variant* c_game_variant::get_ctf_variant() const
{
	ASSERT(get_game_engine_index() == _game_engine_type_ctf, "this variant is not ctf!");
	return static_cast<const c_game_engine_ctf_variant*>(get_active_variant());
}

c_game_engine_ctf_variant* c_game_variant::get_ctf_variant_writeable()
{
	ASSERT(get_game_engine_index() == _game_engine_type_ctf, "this variant is not ctf!");
	return static_cast<c_game_engine_ctf_variant*>(get_active_variant_writeable());
}

const c_game_engine_slayer_variant* c_game_variant::get_slayer_variant() const
{
	ASSERT(get_game_engine_index() == _game_engine_type_slayer, "this variant is not slayer!");
	return static_cast<const c_game_engine_slayer_variant*>(get_active_variant());
}

c_game_engine_slayer_variant* c_game_variant::get_slayer_variant_writeable()
{
	ASSERT(get_game_engine_index() == _game_engine_type_slayer, "this variant is not slayer!");
	return static_cast<c_game_engine_slayer_variant*>(get_active_variant_writeable());
}

const c_game_engine_oddball_variant* c_game_variant::get_oddball_variant() const
{
	ASSERT(get_game_engine_index() == _game_engine_type_oddball, "this variant is not oddball!");
	return static_cast<const c_game_engine_oddball_variant*>(get_active_variant());
}

c_game_engine_oddball_variant* c_game_variant::get_oddball_variant_writeable()
{
	ASSERT(get_game_engine_index() == _game_engine_type_oddball, "this variant is not oddball!");
	return static_cast<c_game_engine_oddball_variant*>(get_active_variant_writeable());
}

const c_game_engine_king_variant* c_game_variant::get_king_variant() const
{
	ASSERT(get_game_engine_index() == _game_engine_type_king, "this variant is not king of the hill!");
	return static_cast<const c_game_engine_king_variant*>(get_active_variant());
}

c_game_engine_king_variant* c_game_variant::get_king_variant_writeable()
{
	ASSERT(get_game_engine_index() == _game_engine_type_king, "this variant is not king of the hill!");
	return static_cast<c_game_engine_king_variant*>(get_active_variant_writeable());
}

const c_game_engine_sandbox_variant* c_game_variant::get_sandbox_variant() const
{
	ASSERT(get_game_engine_index() == _game_engine_type_sandbox, "this variant is not sandbox editor!");
	return static_cast<const c_game_engine_sandbox_variant*>(get_active_variant());
}

c_game_engine_sandbox_variant* c_game_variant::get_sandbox_variant_writeable()
{
	ASSERT(get_game_engine_index() == _game_engine_type_sandbox, "this variant is not sandbox editor!");
	return static_cast<c_game_engine_sandbox_variant*>(get_active_variant_writeable());
}

const c_game_engine_vip_variant* c_game_variant::get_vip_variant() const
{
	ASSERT(get_game_engine_index() == _game_engine_type_vip, "this variant is not vip!");
	return static_cast<const c_game_engine_vip_variant*>(get_active_variant());
}

c_game_engine_vip_variant* c_game_variant::get_vip_variant_writeable()
{
	ASSERT(get_game_engine_index() == _game_engine_type_vip, "this variant is not vip!");
	return static_cast<c_game_engine_vip_variant*>(get_active_variant_writeable());
}

const c_game_engine_juggernaut_variant* c_game_variant::get_juggernaut_variant() const
{
	ASSERT(get_game_engine_index() == _game_engine_type_juggernaut, "this variant is not juggernaut!");
	return static_cast<const c_game_engine_juggernaut_variant*>(get_active_variant());
}

c_game_engine_juggernaut_variant* c_game_variant::get_juggernaut_variant_writeable()
{
	ASSERT(get_game_engine_index() == _game_engine_type_juggernaut, "this variant is not juggernaut!");
	return static_cast<c_game_engine_juggernaut_variant*>(get_active_variant_writeable());
}

const c_game_engine_territories_variant* c_game_variant::get_territories_variant() const
{
	ASSERT(get_game_engine_index() == _game_engine_type_territories, "this variant is not territories!");
	return static_cast<const c_game_engine_territories_variant*>(get_active_variant());
}

c_game_engine_territories_variant* c_game_variant::get_territories_variant_writeable()
{
	ASSERT(get_game_engine_index() == _game_engine_type_territories, "this variant is not territories!");
	return static_cast<c_game_engine_territories_variant*>(get_active_variant_writeable());
}

const c_game_engine_assault_variant* c_game_variant::get_assault_variant() const
{
	ASSERT(get_game_engine_index() == _game_engine_type_assault, "this variant is not assault!");
	return static_cast<const c_game_engine_assault_variant*>(get_active_variant());
}

c_game_engine_assault_variant* c_game_variant::get_assault_variant_writeable()
{
	ASSERT(get_game_engine_index() == _game_engine_type_assault, "this variant is not assault!");
	return static_cast<c_game_engine_assault_variant*>(get_active_variant_writeable());
}

const c_game_engine_infection_variant* c_game_variant::get_infection_variant() const
{
	ASSERT(get_game_engine_index() == _game_engine_type_infection, "this variant is not infection!");
	return static_cast<const c_game_engine_infection_variant*>(get_active_variant());
}

c_game_engine_infection_variant* c_game_variant::get_infection_variant_writeable()
{
	ASSERT(get_game_engine_index() == _game_engine_type_infection, "this variant is not infection!");
	return static_cast<c_game_engine_infection_variant*>(get_active_variant_writeable());
}

bool c_game_variant::get_integer_game_engine_setting(e_game_variant_parameter parameter, int32* out_value) const
{
	return get_game_engine_setting(parameter, _integer_value, out_value);
}

bool c_game_variant::set_integer_game_engine_setting(e_game_variant_parameter parameter, int32 value)
{
	return set_game_engine_setting(parameter, _integer_value, value);
}

bool c_game_variant::get_string_id_game_engine_setting(e_game_variant_parameter parameter, int32* out_value) const
{
	return get_game_engine_setting(parameter, _string_id_value, out_value);
}

bool c_game_variant::set_string_id_game_engine_setting(e_game_variant_parameter parameter, int32 value)
{
	return set_game_engine_setting(parameter, _string_id_value, value);
}

bool c_game_variant::get_game_engine_setting(e_game_variant_parameter parameter, e_text_value_pair_parameter_type parameter_type, int32* out_value) const
{
	return INVOKE_CLASS_MEMBER(0x00574530, c_game_variant, get_game_engine_setting, parameter, parameter_type, out_value);
}

bool c_game_variant::set_game_engine_setting(e_game_variant_parameter parameter, e_text_value_pair_parameter_type parameter_type, int32 value)
{
	return INVOKE_CLASS_MEMBER(0x0057AAB0, c_game_variant, set_game_engine_setting, parameter, parameter_type, value);
}

const char* k_game_engine_type_names[k_game_engine_type_count] =
{
	"none",
	"ctf",
	"slayer",
	"oddball",
	"king",
	"sandbox",
	"vip",
	"juggernaut",
	"territories",
	"assault",
	"infection"
};

const char* game_engine_type_get_string(int32 game_engine_index)
{
	if (game_engine_index < _game_engine_type_none || game_engine_index >= k_game_engine_type_count)
		return "<invalid 'game_engine_index'>";

	return k_game_engine_type_names[game_engine_index];
}

c_game_variant* __cdecl build_default_game_variant(c_game_variant* game_variant, e_game_engine_type game_engine_index)
{
	return INVOKE(0x005721C0, build_default_game_variant, game_variant, game_engine_index);
}

bool __cdecl game_engine_tag_defined_variant_get_built_in_variant(e_game_engine_type game_engine_index, int32 variant_index, c_game_variant* game_variant)
{
	return INVOKE(0x00572270, game_engine_tag_defined_variant_get_built_in_variant, game_engine_index, variant_index, game_variant);
}

int32 __cdecl game_engine_tag_defined_variant_get_default_variant_count(e_game_engine_type game_engine_index)
{
	return INVOKE(0x00572560, game_engine_tag_defined_variant_get_default_variant_count, game_engine_index);

	//if (s_game_engine_settings_definition* game_engine_settings = game_engine_settings_try_and_get())
	//{
	//	switch (game_engine_index)
	//	{
	//	case _game_engine_type_ctf:
	//		return game_engine_settings->ctf_variants.count;
	//	case _game_engine_type_slayer:
	//		return game_engine_settings->slayer_variants.count;
	//	case _game_engine_type_oddball:
	//		return game_engine_settings->oddball_variants.count;
	//	case _game_engine_type_king:
	//		return game_engine_settings->koth_variants.count;
	//	case _game_engine_type_sandbox:
	//		return game_engine_settings->sandbox_variants.count;
	//	case _game_engine_type_vip:
	//		return game_engine_settings->vip_variants.count;
	//	case _game_engine_type_juggernaut:
	//		return game_engine_settings->juggernaut_variants.count;
	//	case _game_engine_type_territories:
	//		return game_engine_settings->territories_variants.count;
	//	case _game_engine_type_assault:
	//		return game_engine_settings->assault_variants.count;
	//	case _game_engine_type_infection:
	//		return game_engine_settings->infection_variants.count;
	//	}
	//}
	//return 0;
}

int32 __cdecl game_engine_tag_defined_variant_get_default_variant_index(const c_game_variant* game_variant)
{
	return INVOKE(0x00572600, game_engine_tag_defined_variant_get_default_variant_index, game_variant);
}

bool __cdecl game_engine_tag_defined_variant_get_strings(e_game_engine_type game_engine_index, int32 variant_index, c_static_wchar_string<48>* variant_name, c_static_wchar_string<256>* variant_description)
{
	return INVOKE(0x00572690, game_engine_tag_defined_variant_get_strings, game_engine_index, variant_index, variant_name, variant_description);
}

void __cdecl game_engine_variant_describe_invalidity(const c_game_variant* game_variant)
{
	INVOKE(0x00572830, game_engine_variant_describe_invalidity, game_variant);
}

bool __cdecl game_engine_variant_is_valid(const c_game_variant* variant)
{
	return INVOKE(0x005728B0, game_engine_variant_is_valid, variant);

	//ASSERT(variant != nullptr);
	//
	//if (main_game_loaded_map())
	//{
	//	c_game_variant game_variant;
	//	game_variant.copy_from_unsafe(variant);
	//	return game_engine_variant_validate(&game_variant);
	//}
	//
	//return true;
}

bool game_engine_variant_validate(c_game_variant* variant)
{
	return INVOKE(0x00572920, game_engine_variant_validate, variant);

	//ASSERT(variant != nullptr);
	//
	//c_game_variant game_variant;
	//game_variant.copy_from_unsafe(variant);
	//
	//bool variant_is_equal = game_variant.is_equal_to(variant);
	//csmemcpy(variant, &game_variant, sizeof(c_game_variant));
	//
	//return variant_is_equal;
}

