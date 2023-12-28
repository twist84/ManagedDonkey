#include "text/font_cache.hpp"

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

