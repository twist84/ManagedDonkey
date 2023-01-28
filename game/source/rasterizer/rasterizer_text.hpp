#pragma once

struct c_rasterizer_texture_ref
{
public:
	bool __cdecl valid();
	static void __cdecl release(c_rasterizer_texture_ref& texture_ref);

protected:
	long m_datum_ref;
};
static_assert(sizeof(c_rasterizer_texture_ref) == 0x4);

