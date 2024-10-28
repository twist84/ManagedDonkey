#include "render/render_structure.hpp"

#include "cseries/cseries.hpp"

REFERENCE_DECLARE(0x050E8938, render_structure_globals, g_render_structure_globals);

s_structure_bsp_resources* c_structure_bsp_resource_interface::get_resources() const
{
	return INVOKE_CLASS_MEMBER(0x004E9550, c_structure_bsp_resource_interface, get_resources);

	//if (use_resource_items)
	//{
	//	if (tag_resource_not_empty(&tag_resources))
	//	{
	//		return static_cast<s_structure_bsp_resources*>(tag_resource_get(&tag_resources));
	//	}
	//}
	//
	//return &raw_resources.m_elements[0];
}

void __cdecl c_structure_renderer::dispose()
{
	INVOKE(0x00A2EDA0, dispose);
}

void __cdecl c_structure_renderer::dispose_from_old_map()
{
	INVOKE(0x00A2EDB0, dispose_from_old_map);
}

void __cdecl c_structure_renderer::dispose_from_old_structure_bsp(dword old_structure_bsp_mask)
{
	INVOKE(0x00A2EE00, dispose_from_old_structure_bsp, old_structure_bsp_mask);
}

void __cdecl c_structure_renderer::initialize()
{
	INVOKE(0x00A2EF10, initialize);
}

void __cdecl c_structure_renderer::initialize_for_new_map()
{
	INVOKE(0x00A2EF20, initialize_for_new_map);
}

void __cdecl c_structure_renderer::initialize_for_new_structure_bsp(dword new_structure_bsp_mask)
{
	INVOKE(0x00A2EF70, initialize_for_new_structure_bsp, new_structure_bsp_mask);
}

