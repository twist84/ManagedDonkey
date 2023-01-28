#include "xbox/xgraphics.hpp"

#include "cseries/cseries.hpp"

unsigned long __stdcall XGSetTextureHeader(unsigned long width, unsigned long height, unsigned long levels, unsigned long usage, unsigned long format, unsigned long pool, unsigned long base_offset, unsigned long mip_offset, unsigned long pitch, D3DBaseTexture* out_texture, unsigned long* out_base_size, unsigned long* out_mip_size)
{
	return INVOKE(0x00D7AA12, XGSetTextureHeader, width, height, levels, usage, format, pool, base_offset, mip_offset, pitch, out_texture, out_base_size, out_mip_size);
}

void __stdcall XGOffsetResourceAddress(D3DBaseTexture* out_resource, void* base_address)
{
	INVOKE(0x00D7AE34, XGOffsetResourceAddress, out_resource, base_address);
}
