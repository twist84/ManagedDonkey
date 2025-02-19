#include "text/font_cache.hpp"

#include "memory/module.hpp"
#include "text/font_fallback.hpp"
#include "text/font_loading.hpp"

HOOK_DECLARE(0x0065A960, font_cache_retrieve_character);

c_font_cache_base::c_font_cache_base() :
	__vftable(reinterpret_cast<decltype(__vftable)>(0x0165FCB0))
{
}

c_font_cache_scope_lock::c_font_cache_scope_lock() :
	m_locked(false)
{
	DECLFUNC(0x006596A0, void, __thiscall, c_font_cache_scope_lock*)(this);
}

c_font_cache_scope_lock::~c_font_cache_scope_lock()
{
	DECLFUNC(0x00659770, void, __thiscall, c_font_cache_scope_lock*)(this);
}

c_font_cache_mt_safe::c_font_cache_mt_safe() :
	c_font_cache_base(),
	m_locked()
{
	__vftable = reinterpret_cast<decltype(__vftable)>(0x0165FCCC);
	DECLFUNC(0x00659650, void, __thiscall, c_font_cache_mt_safe*)(this);
}

c_font_cache_mt_safe::~c_font_cache_mt_safe()
{
	DECLFUNC(0x00659720, void, __thiscall, c_font_cache_mt_safe*)(this);
}

void __cdecl font_cache_delete()
{
	INVOKE(0x0065A0E0, font_cache_delete);
}

void __cdecl font_cache_flush()
{
	INVOKE(0x0065A200, font_cache_flush);
}

void __cdecl font_cache_idle()
{
	INVOKE(0x0065A510, font_cache_idle);
}

//.text:0065A670 ; e_character_status __cdecl font_cache_load_internal(c_font_cache_mt_safe*, e_font_id, e_utf32, c_flags<e_font_cache_flags, dword, 3>, dword*, e_character_data_index*, s_font_character const**)

void __cdecl font_cache_new()
{
	INVOKE(0x0065A890, font_cache_new);
}

//.text:0065A950 ; void __cdecl font_cache_precache()

e_character_status __cdecl font_cache_retrieve_character(dword key, c_flags<e_font_cache_flags, dword, k_font_cache_flag_count> flags, s_font_character const** out_character, void const** out_pixel_data)
{
	//return INVOKE(0x0065A960, font_cache_retrieve_character, key, flags, out_character, out_pixel_data);

	if (font_in_emergency_mode() && fallback_font_get_character(e_utf32((word)key), out_character, out_pixel_data))
	{
		return _character_status_ready;
	}

	e_character_status result = _character_status_invalid;
	HOOK_INVOKE(result =, font_cache_retrieve_character, key, flags, out_character, out_pixel_data);
	return result;
}

