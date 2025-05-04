#include "xbox/xgraphics.hpp"

#include "cseries/cseries.hpp"

uns32 __stdcall XGSetTextureHeader(uns32 width, uns32 height, uns32 levels, uns32 usage, uns32 format, uns32 pool, uns32 base_offset, uns32 mip_offset, uns32 pitch, D3DBaseTexture* out_texture, uns32* out_base_size, uns32* out_mip_size)
{
	return INVOKE(0x00D7AA12, XGSetTextureHeader, width, height, levels, usage, format, pool, base_offset, mip_offset, pitch, out_texture, out_base_size, out_mip_size);
}

void __stdcall XGOffsetResourceAddress(D3DBaseTexture* out_resource, void* base_address)
{
	INVOKE(0x00D7AE34, XGOffsetResourceAddress, out_resource, base_address);
}
