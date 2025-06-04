#pragma once

__interface IDirect3DDevice9Ex;
class c_d3d_resource_allocator
{
public:
	static void set_d3d_device(IDirect3DDevice9Ex* d3d_device_ptr);

protected:
	static IDirect3DDevice9Ex*& m_d3d_device_ptr;
};

