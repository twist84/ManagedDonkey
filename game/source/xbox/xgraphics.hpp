#pragma once

struct D3DBaseTexture;
unsigned long __stdcall XGSetTextureHeader(unsigned long width, unsigned long height, unsigned long levels, unsigned long usage, unsigned long format, unsigned long pool, unsigned long base_offset, unsigned long mip_offset, unsigned long pitch, D3DBaseTexture* out_texture, unsigned long* out_base_size, unsigned long* out_mip_size);
void __stdcall XGOffsetResourceAddress(D3DBaseTexture* out_resource, void* base_address);

