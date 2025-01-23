#include "effects/decals.hpp"

#include "cache/cache_files.hpp"
#include "game/game.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "rasterizer/rasterizer_profile.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00694430, c_decal, render);
HOOK_DECLARE_CLASS(0x00694790, c_decal, render_all);
HOOK_DECLARE(0x006948C0, sub_6948C0);

bool disable_sub_6948C0 = true;

void __cdecl c_decal_system::prepare_for_new_zone_set(dword old_active_structure_bsp_mask, dword new_active_structure_bsp_mask)
{
	INVOKE(0x005302D0, prepare_for_new_zone_set, old_active_structure_bsp_mask, new_active_structure_bsp_mask);
}

void __cdecl c_decal_system::initialize_for_new_non_bsp_zone_set(s_game_non_bsp_zone_set const* new_non_bsp_zone_set)
{
	INVOKE(0x005302E0, initialize_for_new_non_bsp_zone_set, new_non_bsp_zone_set);
}

void __cdecl c_decal_system::dispose_from_old_non_bsp_zone_set(s_game_non_bsp_zone_set const* activating_non_bsp_zone_set)
{
	INVOKE(0x005302F0, dispose_from_old_non_bsp_zone_set, activating_non_bsp_zone_set);
}

void __cdecl c_decal_system::change_pvs(s_game_cluster_bit_vectors const* old_cluster_activation, s_game_cluster_bit_vectors const* new_cluster_activation, bool local)
{
	INVOKE(0x0068FBB0, change_pvs, old_cluster_activation, new_cluster_activation, local);
}

void __cdecl c_decal_system::dispose()
{
	INVOKE(0x00691B00, dispose);
}

void __cdecl c_decal_system::dispose_from_old_map()
{
	INVOKE(0x00691CD0, dispose_from_old_map);
}

void __cdecl c_decal_system::dispose_from_old_structure_bsp(dword deactivating_structure_bsp_mask)
{
	INVOKE(0x00691D70, dispose_from_old_structure_bsp, deactivating_structure_bsp_mask);
}

void __cdecl c_decal_system::initialize()
{
	INVOKE(0x006931F0, initialize);
}

void __cdecl c_decal_system::initialize_for_new_map()
{
	INVOKE(0x006936C0, initialize_for_new_map);
}

void __cdecl c_decal_system::initialize_for_new_structure_bsp(dword activating_structure_bsp_mask)
{
	INVOKE(0x00693740, initialize_for_new_structure_bsp, activating_structure_bsp_mask);
}

void __cdecl c_decal_system::prepare_for_non_bsp_zone_set_switch(s_game_non_bsp_zone_set const* old_non_bsp_zone_set, s_game_non_bsp_zone_set const* new_non_bsp_zone_set, c_scenario_resource_registry* pending_zone_registry)
{
	INVOKE(0x006941B0, prepare_for_non_bsp_zone_set_switch, old_non_bsp_zone_set, new_non_bsp_zone_set, pending_zone_registry);
}

//void __cdecl c_decal::render(c_decal_definition::e_pass) const
void __thiscall c_decal::render(long pass)
{
	//INVOKE_CLASS_MEMBER(0x00694430, c_decal, render, pass);

	static c_static_wchar_string<128> tag_name;
	char const* tag_name_ = "$TODO: tag_name";
	//TLS_DATA_GET_VALUE_REFERENCE(g_decal_system_data_array);
	//REFERENCE_DECLARE(offset_pointer(this, 0x8), long, decal_system_index);
	//void* decal_system = datum_get(g_decal_system_data_array, decal_system_index);
	//REFERENCE_DECLARE(offset_pointer(decal_system, 0x4), long, tag_index);
	//tag_name_ = tag_name_strip_path(tag_get_name(tag_index));
	c_rasterizer_profile_scope _decal(_rasterizer_profile_element_effects, tag_name.print(L"%hs", tag_name_));

	HOOK_INVOKE_CLASS_MEMBER(, c_decal, render, pass);
}

//void __cdecl c_decal::render_all(enum c_decal_definition::e_pass)
void __cdecl c_decal::render_all(long pass)
{
	//INVOKE(0x00694790, c_decal::render_all, pass);

	c_rasterizer_profile_scope _decals(_rasterizer_profile_element_effects, L"decals");

	HOOK_INVOKE_CLASS(, c_decal, render_all, decltype(&c_decal::render_all), pass);
}

//.text:00695030 ; public: void __cdecl c_decal::submit() const
//.text:00695120 ; private: void __cdecl c_decal_system::submit()

void __cdecl c_decal_system::submit_all()
{
	INVOKE(0x006951B0, c_decal_system::submit_all);
}

void __cdecl sub_6948C0(long a1)
{
	if (disable_sub_6948C0)
		return;

	HOOK_INVOKE(, sub_6948C0, a1);
}


