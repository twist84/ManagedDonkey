#include "rasterizer/rasterizer_text.hpp"

#include "cseries/cseries.hpp"

bool __cdecl c_rasterizer_texture_ref::valid()
{
	return m_datum_ref && m_datum_ref != -1;
}

void __cdecl c_rasterizer_texture_ref::release(c_rasterizer_texture_ref& texture_ref)
{
	INVOKE(0x00A6EB10, c_rasterizer_texture_ref::release, texture_ref);
}

