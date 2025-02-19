#pragma once

#include "cseries/cseries.hpp"

enum e_character_status
{
	_character_status_invalid = 0,
	_character_status_loading,
	_character_status_ready,

	k_character_status_count
};

enum e_font_cache_flags
{
	_font_cache_block_bit = 0,
	_font_cache_load_bit,
	_font_cache_predicted_bit,

	k_font_cache_flag_count
};

enum e_character_data_index;
enum e_font_id;
enum e_utf32 : dword;
struct s_font_header;
struct s_font_character;

// 0165FCB0
struct c_font_cache_base
{
public:
	// HACK: so we don't have to manually construct the class
	struct
	{
		long(__thiscall* character_data_get_hardware_block_index)(c_font_cache_base* _this, e_character_data_index character_data_index);
		void(__thiscall* character_data_set_hardware_block_index)(c_font_cache_base* _this, e_character_data_index character_data_index, long hardware_block_index);
		void(__thiscall* character_data_hardware_block_touched)(c_font_cache_base* _this, e_character_data_index character_data_index);
		bool(__thiscall* get_pixels)(c_font_cache_base* _this, dword a1, e_character_data_index character_data_index, s_font_character const* font_character, c_static_array<word, 16384>* pixels);
		bool(__thiscall* character_predict)(c_font_cache_base* _this, e_font_id font_id, e_utf32 utf32);
		bool(__thiscall* character_load)(c_font_cache_base* _this, e_font_id font_id, e_utf32 utf32, bool a3, dword* a4, e_character_data_index* out_character_data_index, s_font_character const** out_font_character);
	}*__vftable;

	c_font_cache_base();

	long character_data_get_hardware_block_index(e_character_data_index character_data_index)
	{
		return __vftable->character_data_get_hardware_block_index(this, character_data_index);
	}

	void character_data_set_hardware_block_index(e_character_data_index character_data_index, long hardware_block_index)
	{
		__vftable->character_data_set_hardware_block_index(this, character_data_index, hardware_block_index);
	}

	void character_data_hardware_block_touched(e_character_data_index character_data_index)
	{
		__vftable->character_data_hardware_block_touched(this, character_data_index);
	}

	bool get_pixels(dword a1, e_character_data_index character_data_index, s_font_character const* font_character, c_static_array<word, 16384>* pixels)
	{
		return __vftable->get_pixels(this, a1, character_data_index, font_character, pixels);
	}

	bool character_predict(e_font_id font_id, e_utf32 character)
	{
		return __vftable->character_predict(this, font_id, character);
	}

	bool character_load(e_font_id font_id, e_utf32 character, bool a3, dword* a4, e_character_data_index* out_character_data_index, s_font_character const** out_font_character)
	{
		return __vftable->character_load(this, font_id, character, a3, a4, out_character_data_index, out_font_character);
	}

};
static_assert(sizeof(c_font_cache_base) == 0x4);

struct c_font_cache_scope_lock
{
public:
	c_font_cache_scope_lock();
	~c_font_cache_scope_lock();

protected:
	volatile bool m_locked;
};
static_assert(sizeof(c_font_cache_scope_lock) == sizeof(bool));

// 0165FCCC
struct c_font_cache_mt_safe : // thread safe
	c_font_cache_base
{
public:
	c_font_cache_mt_safe();
	~c_font_cache_mt_safe();

protected:
	c_font_cache_scope_lock m_locked;
};
static_assert(sizeof(c_font_cache_mt_safe) == sizeof(c_font_cache_base) + 0x4);

// 0165FCE8
//c_null_font_cache

extern void __cdecl font_cache_delete();
extern void __cdecl font_cache_flush();
extern void __cdecl font_cache_idle();
extern void __cdecl font_cache_new();
extern e_character_status __cdecl font_cache_retrieve_character(dword key, c_flags<e_font_cache_flags, dword, k_font_cache_flag_count> flags, s_font_character const** out_character, void const** out_pixel_data);

