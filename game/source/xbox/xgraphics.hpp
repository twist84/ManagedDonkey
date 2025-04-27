#pragma once

struct D3DBaseTexture;
uint32 __stdcall XGSetTextureHeader(uint32 width, uint32 height, uint32 levels, uint32 usage, uint32 format, uint32 pool, uint32 base_offset, uint32 mip_offset, uint32 pitch, D3DBaseTexture* out_texture, uint32* out_base_size, uint32* out_mip_size);
void __stdcall XGOffsetResourceAddress(D3DBaseTexture* out_resource, void* base_address);

