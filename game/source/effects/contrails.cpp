#include "effects/contrails.hpp"

#include "memory/module.hpp"
#include "memory/thread_local.hpp"

//.text:00755540 ; public: static int32 __cdecl c_contrail::create(uns8 priority, int32 parent_index, int32 definition_block_index, uns32 random_seed)
//.text:007556F0 ; public: static int32 __cdecl c_contrail_location::create(uns8 priority, int32 parent_index, const real_matrix4x3* matrix, uns32 random_seed)
//.text:00755840 ; public: static int32 __cdecl c_contrail_profile::create(const c_contrail* parent)
//.text:00755930 ; public: static int32 __cdecl c_contrail_system::create(uns8 priority, c_tag_index definition_index, int32 effect_index, uns16 event_block_index, uns16 event_counter, c_tag_index effect_definition_index, uns8 camera_mode)
//.text:00755A10 ; private: void c_contrail::create_profile(uns8 priority, c_contrail_states* contrail_states, bool temporary, const real_vector3d* direction, real32 distance)
//.text:00755B10 ; 
//.text:00755B40 ; 
//.text:00755B70 ; 
//.text:00755BA0 ; 
//.text:00755BD0 ; public: static void __cdecl c_contrail::destroy(int32 datum_index)
//.text:00755C80 ; public: static void __cdecl c_contrail_location::destroy(int32 datum_index)
//.text:00755CE0 ; public: static void __cdecl c_contrail_profile::destroy(int32 datum_index)
//.text:00755D10 ; public: static void __cdecl c_contrail_system::destroy(int32 datum_index)
//.text:00755D60 ; 
//.text:00755D80 ; 
//.text:00755DA0 ; 
//.text:00755DC0 ; public: static void __cdecl c_contrail_system::dispose_from_game(void)
//.text:00755E00 ; 
//.text:00755E20 ; 
//.text:00755E40 ; 
//.text:00755E60 ; public: static void __cdecl c_contrail_system::dispose_from_map(void)
//.text:00755EB0 ; 
//.text:00755EC0 ; 
//.text:00755ED0 ; 
//.text:00755EE0 ; private: void c_contrail::extend(uns8 priority, c_contrail_states* contrail_states, bool temporary, const real_vector3d* direction, real32 extend_by)
//.text:00756040 ; public: void c_contrail::set_should_frame_advance(bool should_frame_advance)
//.text:00756070 ; 
//.text:00756110 ; public: void c_contrail_system::frame_advance(real32 delta_time)
//.text:007561B0 ; public: static void __cdecl c_contrail_system::frame_advance_all(real32 delta_time)
//.text:007562F0 ; public: static void __cdecl c_contrail_system::frame_advance_all_gpu(real32 delta_time)
//.text:00756310 ; 
//.text:00756340 ; 
//.text:00756370 ; 
//.text:007563A0 ; 

c_contrail* __cdecl c_contrail::get(int32 datum_index)
{
	return INVOKE(0x007563D0, c_contrail::get, datum_index);
}

//.text:00756400 ; 
//.text:00756430 ; 
//.text:00756440 ; 
//.text:00756450 ; 

const c_contrail_definition* c_contrail::get_definition(void) const
{
	return INVOKE_CLASS_MEMBER(0x00756460, c_contrail, get_definition);
}

//.text:007564C0 ; 
//.text:007564D0 ; 
//.text:007564E0 ; 
//.text:007564F0 ; 
//.text:00756500 ; 
//.text:00756510 ; 
//.text:00756520 ; 
//.text:00756530 ; 
//.text:00756540 ; 
//.text:00756550 ; 

const c_contrail_location* c_contrail::get_parent_location(void) const
{
	return INVOKE_CLASS_MEMBER(0x00756560, c_contrail, get_parent_location);
}

const c_contrail_system* c_contrail_location::get_parent_system(void) const
{
	return INVOKE_CLASS_MEMBER(0x00756590, c_contrail_location, get_parent_system);
}

//.text:007565C0 ; 
//.text:007565D0 ; 
//.text:007565E0 ; 
//.text:007565F0 ; 
//.text:00756600 ; 
//.text:00756610 ; 
//.text:00756620 ; 
//.text:00756630 ; 
//.text:00756640 ; 
//.text:00756650 ; 
//.text:00756660 ; 
//.text:00756670 ; 
//.text:00756680 ; 
//.text:00756690 ; 
//.text:007566A0 ; 
//.text:007566B0 ; public: bool c_contrail_definition::get_double_sided(void) const
//.text:007566C0 ; public: static void __cdecl c_contrail::initialize_for_game(void)
//.text:00756700 ; public: static void __cdecl c_contrail_location::initialize_for_game(void)
//.text:00756740 ; public: static void __cdecl c_contrail_profile::initialize_for_game(void)
//.text:00756770 ; public: static void __cdecl c_contrail_system::initialize_for_game(void)
//.text:00756800 ; public: static void __cdecl c_contrail::initialize_for_map(void)
//.text:00756820 ; public: static void __cdecl c_contrail_location::initialize_for_map(void)
//.text:00756840 ; public: static void __cdecl c_contrail_profile::initialize_for_map(void)
//.text:00756860 ; public: static void __cdecl c_contrail_system::initialize_for_map(void)
//.text:007568B0 ; public: static void __cdecl c_contrail_system::initialize_for_new_structure_bsp(void)
//.text:00756900 ; private: void c_contrail::initialize_profile(c_contrail_profile* contrail_profile, c_contrail_states* contrail_states, const real_vector3d* direction, real32 distance)
//.text:00756AB0 ; 
//.text:00756AC0 ; public: void c_contrail_location::move_to(real32 delta_time, const real_matrix4x3* matrix)
//.text:00756B80 ; 
//.text:00756BA0 ; public: static void __cdecl c_contrail_system::prepare_for_non_bsp_zone_set_switch(const s_game_non_bsp_zone_set* old_non_bsp_zone_set, const s_game_non_bsp_zone_set* new_non_bsp_zone_set, c_scenario_resource_registry* pending_zone_registry)
//.text:00756CA0 ; public: void c_contrail::pulse(real32 delta_time, bool force_profile, c_contrail_states* contrail_states)
//.text:00756F10 ; public: void c_contrail_location::pulse(real32 delta_time, bool force_profile, c_contrail_states* contrail_states)
//.text:00756F80 ; public: void c_contrail_system::pulse(real32 delta_time, bool force_profile)
//.text:00757040 ; public: static void __cdecl c_contrail_system::pulse_all(real32 delta_time)
//.text:00757130 ; public: void __cdecl c_contrail_system::query_effect(real32 delta_time)
//.text:007571B0 ; 
//.text:00757210 ; public: void __cdecl c_contrail_system::remove_further_contrail_locations(int32 parent_index)

void c_contrail::render() const
{
	INVOKE_CLASS_MEMBER(0x007572B0, c_contrail, render);
}

void __cdecl c_contrail::render_callback(const void* user_data, int32 user_context)
{
	//INVOKE(0x00757370, c_contrail::render_callback, user_data, user_context);

	const c_contrail* contrail = (const c_contrail*)user_data;
	contrail->render();
}

//.text:00757380 ; 
//.text:007573C0 ; 
//.text:00757400 ; 
//.text:00757440 ; 
//.text:00757480 ; 
//.text:007574A0 ; 
//.text:007574C0 ; 
//.text:007574C0 ; 
//.text:007574E0 ; 
//.text:00757500 ; 
//.text:00757520 ; 
//.text:00757540 ; 
//.text:00757560 ; 
//.text:00757580 ; 
//.text:00757590 ; 
//.text:007575A0 ; 
//.text:007575C0 ; 
//.text:007575E0 ; 
//.text:00757600 ; 
//.text:00757620 ; 
//.text:00757640 ; 
//.text:00757660 ; 
//.text:00757680 ; 
//.text:007576A0 ; 
//.text:007576C0 ; 
//.text:007576E0 ; 
//.text:00757740 ; public: void c_contrail::submit(void) const
//.text:00757810 ; public: void c_contrail_system::submit(int32 first_person_user_index, e_effect_pass effect_pass)

void __cdecl c_contrail_system::submit_all(int32 user_index, e_effect_pass effect_pass)
{
	INVOKE(0x00757890, c_contrail_system::submit_all, user_index, effect_pass);
}

//.text:00757910 ; 

