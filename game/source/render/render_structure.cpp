#include "render/render_structure.hpp"

#include "cseries/cseries.hpp"
#include "memory/module.hpp"
#include "rasterizer/rasterizer_profile.hpp"

HOOK_DECLARE_CLASS(0x00A2F050, c_structure_renderer, render_albedo);
HOOK_DECLARE_CLASS(0x00A2F340, c_structure_renderer, render_decorators);

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

void __cdecl c_structure_renderer::dispose_from_old_structure_bsp(uns32 deactivating_structure_bsp_mask)
{
	INVOKE(0x00A2EE00, dispose_from_old_structure_bsp, deactivating_structure_bsp_mask);
}

void __cdecl c_structure_renderer::initialize()
{
	INVOKE(0x00A2EF10, initialize);
}

void __cdecl c_structure_renderer::initialize_for_new_map()
{
	INVOKE(0x00A2EF20, initialize_for_new_map);
}

void __cdecl c_structure_renderer::initialize_for_new_structure_bsp(uns32 activating_structure_bsp_mask)
{
	INVOKE(0x00A2EF70, initialize_for_new_structure_bsp, activating_structure_bsp_mask);
}

//.text:00A2EF80 ; bool __cdecl instance_list_sort_proc(int16 one, int16 two, const void* data)

bool __cdecl c_structure_renderer::lightmaps_available(int32 structure_bsp_index)
{
	return INVOKE(0x00A2EFC0, c_structure_renderer::lightmaps_available, structure_bsp_index);
}

bool __cdecl c_structure_renderer::lightmaps_available_on_all_active_bsps()
{
	return INVOKE(0x00A2EFE0, c_structure_renderer::lightmaps_available_on_all_active_bsps);

	//return g_render_structure_globals.lightmaps_available_on_all_active_bsps;
}

void __cdecl c_structure_renderer::pop_marker()
{
	return INVOKE(0x00A2EFF0, c_structure_renderer::pop_marker);
}

void __cdecl c_structure_renderer::push_marker()
{
	return INVOKE(0x00A2F020, c_structure_renderer::push_marker);
}

void __cdecl c_structure_renderer::render_albedo()
{
	//INVOKE(0x00A2F050, c_structure_renderer::render_albedo);

	c_rasterizer_profile_scope _structure_albedo(_rasterizer_profile_element_total, L"structure_albedo");

	HOOK_INVOKE_CLASS(, c_structure_renderer, render_albedo, decltype(&c_structure_renderer::render_albedo));
}

//.text:00A2F0C0 ; 
//.text:00A2F0D0 ; public: static void __cdecl c_structure_renderer::render_cluster_mesh_part(int32, e_entry_point, int32)
//.text:00A2F2A0 ; public: static void __cdecl c_structure_renderer::render_cluster_parts(e_entry_point, int32)

void __cdecl c_structure_renderer::render_decorators()
{
	//INVOKE(0x00A2F340, c_structure_renderer::render_decorators);

	c_rasterizer_profile_scope _decorators(_rasterizer_profile_element_total, L"decorators");

	HOOK_INVOKE_CLASS(, c_structure_renderer, render_decorators, decltype(&c_structure_renderer::render_decorators));
}

