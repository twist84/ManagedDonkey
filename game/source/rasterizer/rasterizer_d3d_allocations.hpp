#pragma once

struct IDirect3DDevice9Ex;
struct c_d3d_resource_allocator
{
	static IDirect3DDevice9Ex*& m_d3d_device_ptr;
};

