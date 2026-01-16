#pragma once

#include "cseries/cseries.hpp"

enum e_character_data_index;
enum e_utf32 : uns32;
struct s_font_header;
struct s_font_character;

// 0165FCB0
class c_font_cache_base
{
public:
	// HACK: so we don't have to manually construct the class
	struct
	{
		int32(__thiscall* character_data_get_hardware_block_index)(c_font_cache_base* _this, e_character_data_index character_data_index);
		void(__thiscall* character_data_set_hardware_block_index)(c_font_cache_base* _this, e_character_data_index character_data_index, int32 hardware_block_index);
		void(__thiscall* character_data_hardware_block_touched)(c_font_cache_base* _this, e_character_data_index character_data_index);
		bool(__thiscall* get_pixels)(c_font_cache_base* _this, uns32 a1, e_character_data_index character_data_index, const s_font_character* font_character, c_static_array<uns16, 16384>* pixels);
		bool(__thiscall* character_predict)(c_font_cache_base* _this, e_font_id font_id, e_utf32 utf32);
		bool(__thiscall* character_load)(c_font_cache_base* _this, e_font_id font_id, e_utf32 utf32, bool a3, uns32* a4, e_character_data_index* out_character_data_index, const s_font_character** out_font_character);
	}*__vftable;

	c_font_cache_base();

	int32 character_data_get_hardware_block_index(e_character_data_index character_data_index)
	{
		return __vftable->character_data_get_hardware_block_index(this, character_data_index);
	}

	void character_data_set_hardware_block_index(e_character_data_index character_data_index, int32 hardware_block_index)
	{
		__vftable->character_data_set_hardware_block_index(this, character_data_index, hardware_block_index);
	}

	void character_data_hardware_block_touched(e_character_data_index character_data_index)
	{
		__vftable->character_data_hardware_block_touched(this, character_data_index);
	}

	bool get_pixels(uns32 a1, e_character_data_index character_data_index, const s_font_character* font_character, c_static_array<uns16, 16384>* pixels)
	{
		return __vftable->get_pixels(this, a1, character_data_index, font_character, pixels);
	}

	bool character_predict(e_font_id font_id, e_utf32 character)
	{
		return __vftable->character_predict(this, font_id, character);
	}

	bool character_load(e_font_id font_id, e_utf32 character, bool a3, uns32* a4, e_character_data_index* out_character_data_index, const s_font_character** out_font_character)
	{
		return __vftable->character_load(this, font_id, character, a3, a4, out_character_data_index, out_font_character);
	}

};
COMPILE_ASSERT(sizeof(c_font_cache_base) == 0x4);

class c_font_cache_scope_lock
{
public:
	c_font_cache_scope_lock();
	~c_font_cache_scope_lock();

protected:
	volatile bool m_locked;
};
COMPILE_ASSERT(sizeof(c_font_cache_scope_lock) == sizeof(bool));

#define FONT_CACHE_SCOPE_LOCK c_font_cache_scope_lock __local_font_cache_scope_lock

// 0165FCCC
class c_font_cache_mt_safe : // thread safe
	public c_font_cache_base
{
public:
	c_font_cache_mt_safe();
	~c_font_cache_mt_safe();

protected:
	c_font_cache_scope_lock m_locked;
};
COMPILE_ASSERT(sizeof(c_font_cache_mt_safe) == sizeof(c_font_cache_base) + 0x4);

// 0165FCE8
//c_null_font_cache

extern void __cdecl font_cache_delete();
extern void __cdecl font_cache_flush();
extern void __cdecl font_cache_idle();
extern void __cdecl font_cache_new();
extern e_character_status __cdecl font_cache_retrieve_character(uns32 key, c_flags<e_font_cache_flags, uns32, k_font_cache_flag_count> flags, const s_font_character** out_character, const void** out_pixel_data);

