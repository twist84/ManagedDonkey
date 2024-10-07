#include "rasterizer/rasterizer_d3d_allocations.hpp"

#include "cseries/cseries.hpp"

REFERENCE_DECLARE(0x0524B6AC, IDirect3DDevice9Ex*, c_d3d_resource_allocator::m_d3d_device_ptr);

void c_d3d_resource_allocator::set_d3d_device(IDirect3DDevice9Ex* d3d_device_ptr)
{
	m_d3d_device_ptr = d3d_device_ptr;
}

