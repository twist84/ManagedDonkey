#pragma once

struct D3DBaseTexture;
uns32 __stdcall XGSetTextureHeader(uns32 width, uns32 height, uns32 levels, uns32 usage, uns32 format, uns32 pool, uns32 base_offset, uns32 mip_offset, uns32 pitch, D3DBaseTexture* out_texture, uns32* out_base_size, uns32* out_mip_size);
void __stdcall XGOffsetResourceAddress(D3DBaseTexture* out_resource, void* base_address);

