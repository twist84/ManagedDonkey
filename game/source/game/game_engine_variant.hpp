#pragma once

#include "cseries/cseries.hpp"
#include "game/game_engine_default.hpp"
#include "game/game_engine_assault.hpp"
#include "game/game_engine_ctf.hpp"
#include "game/game_engine_infection.hpp"
#include "game/game_engine_juggernaut.hpp"
#include "game/game_engine_king.hpp"
#include "game/game_engine_oddball.hpp"
#include "game/game_engine_sandbox.hpp"
#include "game/game_engine_slayer.hpp"
#include "game/game_engine_territories.hpp"
#include "game/game_engine_vip.hpp"
#include "text/unicode.hpp"

#define BUILD_DEFAULT_GAME_VARIANT(_game_variant, _game_engine_index)                            \
if (game_engine_tag_defined_variant_get_default_variant_count(_game_engine_index) > 0)           \
    game_engine_tag_defined_variant_get_built_in_variant(_game_engine_index, 0, &_game_variant); \
else                                                                                             \
    build_default_game_variant(&_game_variant, _game_engine_index);

class c_bitstream;

//#pragma pack(push, 4)
//
//#pragma pack(pop)

int32 const k_maximum_game_engine_variant_size = 0x260;

class c_game_variant
{
public:
	c_game_variant();

	void encode_to_mcc(c_bitstream* packet) const;
	bool decode_from_mcc(c_bitstream* packet);

	void copy_from_and_validate(const c_game_variant* other);
	void copy_from_unsafe(const c_game_variant* other);
	bool decode(c_bitstream* packet);
	void encode(c_bitstream* packet) const;

	int32 get_variant_size_for_game_engine_index(e_game_engine_type game_engine_index) const;
	bool is_equal_to(const c_game_variant* other) const;
	void recreate_variant_vtable_for_game_engine_index(e_game_engine_type game_engine_index);

	e_game_engine_type get_game_engine_index() const;
	void set_game_engine_index(e_game_engine_type game_engine_index);

	const c_game_engine_base_variant* get_active_variant() const;
	c_game_engine_base_variant* get_active_variant_writeable();

	const c_game_engine_ctf_variant* get_ctf_variant() const;
	c_game_engine_ctf_variant* get_ctf_variant_writeable();

	const c_game_engine_slayer_variant* get_slayer_variant() const;
	c_game_engine_slayer_variant* get_slayer_variant_writeable();

	const c_game_engine_oddball_variant* get_oddball_variant() const;
	c_game_engine_oddball_variant* get_oddball_variant_writeable();

	const c_game_engine_king_variant* get_king_variant() const;
	c_game_engine_king_variant* get_king_variant_writeable();

	const c_game_engine_sandbox_variant* get_sandbox_variant() const;
	c_game_engine_sandbox_variant* get_sandbox_variant_writeable();

	const c_game_engine_vip_variant* get_vip_variant() const;
	c_game_engine_vip_variant* get_vip_variant_writeable();

	const c_game_engine_juggernaut_variant* get_juggernaut_variant() const;
	c_game_engine_juggernaut_variant* get_juggernaut_variant_writeable();

	const c_game_engine_territories_variant* get_territories_variant() const;
	c_game_engine_territories_variant* get_territories_variant_writeable();

	const c_game_engine_assault_variant* get_assault_variant() const;
	c_game_engine_assault_variant* get_assault_variant_writeable();

	const c_game_engine_infection_variant* get_infection_variant() const;
	c_game_engine_infection_variant* get_infection_variant_writeable();

	bool get_integer_game_engine_setting(e_game_variant_parameter parameter, int32* out_value) const;
	bool set_integer_game_engine_setting(e_game_variant_parameter parameter, int32 value);

	bool get_string_id_game_engine_setting(e_game_variant_parameter parameter, int32* out_value) const;
	bool set_string_id_game_engine_setting(e_game_variant_parameter parameter, int32 value);

protected:
	bool get_game_engine_setting(e_game_variant_parameter parameter, e_text_value_pair_parameter_type parameter_type, int32* out_value) const;
	bool set_game_engine_setting(e_game_variant_parameter parameter, e_text_value_pair_parameter_type parameter_type, int32 value);

	c_enum<e_game_engine_type, int32, _game_engine_type_none, k_game_engine_type_count> m_game_engine_index;
	union
	{
		c_game_engine_base_variant m_base_variant;
		c_game_engine_ctf_variant m_ctf_variant;
		c_game_engine_slayer_variant m_slayer_variant;
		c_game_engine_oddball_variant m_oddball_variant;
		c_game_engine_king_variant m_king_variant;
		c_game_engine_sandbox_variant m_sandbox_variant;
		c_game_engine_vip_variant m_vip_variant;
		c_game_engine_juggernaut_variant m_juggernaut_variant;
		c_game_engine_territories_variant m_territories_variant;
		c_game_engine_assault_variant m_assault_variant;
		c_game_engine_infection_variant m_infection_variant;
		uns8 m_variant_storage[k_maximum_game_engine_variant_size];
	};
};
COMPILE_ASSERT(sizeof(c_game_variant) == 0x264);

extern const char* game_engine_type_get_string(int32 game_engine_variant);
extern c_game_variant* __cdecl build_default_game_variant(c_game_variant* game_variant, e_game_engine_type game_engine_index);
extern bool __cdecl game_engine_tag_defined_variant_get_built_in_variant(e_game_engine_type game_engine_index, int32 variant_index, c_game_variant* game_variant);
extern int32 __cdecl game_engine_tag_defined_variant_get_default_variant_count(e_game_engine_type game_engine_index);
extern int32 __cdecl game_engine_tag_defined_variant_get_default_variant_index(const c_game_variant* game_variant);
extern bool __cdecl game_engine_tag_defined_variant_get_strings(e_game_engine_type game_engine_index, int32 variant_index, c_static_wchar_string<48>* variant_name, c_static_wchar_string<256>* variant_description);
extern void __cdecl game_engine_variant_describe_invalidity(const c_game_variant* game_variant);
extern bool __cdecl game_engine_variant_is_valid(const c_game_variant* variant);
extern bool game_engine_variant_validate(c_game_variant* variant);

