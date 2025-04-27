#include "animations/animation_manager.hpp"

//.text:006DA5A0 ; public: __cdecl c_animation_manager::c_animation_manager()
//.text:006DA610 ; 
//.text:006DA630 ; 
//.text:006DA650 ; 
//.text:006DA670 ; 
//.text:006DA6C0 ; 
//.text:006DA6D0 ; 
//.text:006DA6E0 ; 
//.text:006DA6F0 ; 
//.text:006DA700 ; 
//.text:006DA710 ; public: __cdecl t_short_fixed<13>::~t_short_fixed<13>()
//.text:006DA720 ; 
//.text:006DA730 ; 
//.text:006DA750 ; 
//.text:006DA760 ; 
//.text:006DA770 ; 
//.text:006DA780 ; 
//.text:006DA7E0 ; 
//.text:006DA810 ; 
//.text:006DA830 ; 
//.text:006DA840 ; 
//.text:006DA850 ; 
//.text:006DA870 ; 
//.text:006DA890 ; public: void __cdecl c_animation_manager::adjust_node_orientations_for_rendering(render_model_definition const*, real32, c_static_flags<255> const*, long, real_orientation*, void(__cdecl*)(long, c_model_animation_graph const*, c_animation_id, uint32 const*, real32), long)
//.text:006DA8D0 ; public: void __cdecl c_animation_manager::animation_get_root_matrix(c_animation_id, real32, real_matrix4x3*) const
//.text:006DA900 ; public: void __cdecl c_animation_manager::animation_get_root_orientation(c_animation_id, real32, render_model_definition const*, real_orientation*) const
//.text:006DA930 ; public: void __cdecl c_animation_manager::animation_get_root_starting_point(c_animation_id, render_model_definition const*, real_orientation*) const
//.text:006DA960 ; public: void __cdecl c_animation_manager::animation_get_root_velocity(c_animation_id, real32, real32, real_vector3d*) const
//.text:006DA9A0 ; 
//.text:006DA9B0 ; 
//.text:006DA9E0 ; 
//.text:006DA9F0 ; 
//.text:006DAA00 ; private: void __cdecl c_animation_manager::apply_random_permutations_to_settings(c_animation_channel_settings*) const
//.text:006DAA60 ; private: void __cdecl c_animation_manager::apply_transition_delta(real32, real_orientation*)
//.text:006DAAF0 ; 
//.text:006DAB00 ; public: bool __cdecl c_animation_manager::build_model_mapping_table(long, long*, long*, long)
//.text:006DAB20 ; 
//.text:006DAB30 ; 
//.text:006DAB40 ; public: c_animation_id __cdecl c_animation_manager::choose_random_variation(c_animation_id, uint32*) const
//.text:006DABA0 ; 
//.text:006DABB0 ; 
//.text:006DABC0 ; 
//.text:006DABD0 ; 
//.text:006DAC00 ; 
//.text:006DAC50 ; 
//.text:006DAC80 ; 
//.text:006DACA0 ; 
//.text:006DAD40 ; 
//.text:006DAD70 ; 
//.text:006DADD0 ; 
//.text:006DAEA0 ; public: bool __cdecl c_animation_manager::current_state_contains_motion_data() const
//.text:006DAEE0 ; 
//.text:006DAF00 ; 
//.text:006DAF20 ; 
//.text:006DB190 ; public: float __cdecl t_short_fixed<13>::export_real() const
//.text:006DB1E0 ; 
//.text:006DB220 ; 
//.text:006DB2C0 ; 
//.text:006DB2F0 ; public: c_animation_id __cdecl c_animation_manager::find_death_or_damage_animation_id(long, long, short, short, short*, short*) const
//.text:006DB410 ; public: c_animation_id __cdecl c_animation_manager::find_facial_animation_base() const
//.text:006DB440 ; public: c_animation_id __cdecl c_animation_manager::find_facial_animation_cinematic_base() const
//.text:006DB470 ; public: c_animation_id __cdecl c_animation_manager::find_facial_animation_overlay() const
//.text:006DB4A0 ; public: bool __cdecl c_animation_manager::find_goal(long, long, long, long, c_flags<e_animation_state_change_flags, uint16, 16>) const
//.text:006DB4D0 ; public: c_animation_id __cdecl c_animation_manager::find_goal_animation_id(long, long, long, long, long, c_flags<e_animation_state_change_flags, uint16, 16>) const
//.text:006DB510 ; public: c_model_animation const* __cdecl c_animation_manager::find_impulse_or_state_animation(long, long, long) const
//.text:006DB560 ; public: c_animation_id __cdecl c_animation_manager::find_impulse_or_state_animation_id(long, long, long) const
//.text:006DB5E0 ; 
//.text:006DB610 ; 
//.text:006DB650 ; public: bool __cdecl c_animation_manager::find_next_mode_ik_point(ik_point_iterator*) const
//.text:006DB690 ; 
//.text:006DB6B0 ; public: bool __cdecl c_animation_manager::find_next_vehicle_suspension(suspension_animation_iterator*) const
//.text:006DB6D0 ; public: bool __cdecl c_animation_manager::find_next_weapon_ik_point(ik_point_iterator*) const
//.text:006DB710 ; 
//.text:006DB730 ; 
//.text:006DB780 ; public: c_animation_id __cdecl c_animation_manager::find_parallel_impulse_animation_id(long, long, long) const
//.text:006DB820 ; public: c_animation_id __cdecl c_animation_manager::find_state_animation_id(long, long, long) const
//.text:006DB870 ; public: s_animation_sync_action const* __cdecl c_animation_manager::find_sync_action(long, long, long) const
//.text:006DB8D0 ; 
//.text:006DB8E0 ; 
//.text:006DB8F0 ; 
//.text:006DB900 ; 
//.text:006DB910 ; public: c_animation_channel const* __cdecl c_animation_manager::get_active_channel() const
//.text:006DB930 ; public: c_model_animation const* __cdecl c_animation_manager::get_animation(c_animation_id) const
//.text:006DB950 ; public: real32 __cdecl c_animation_manager::get_authored_state_duration() const
//.text:006DB980 ; public: real32 __cdecl c_animation_manager::get_authored_state_time() const
//.text:006DB9B0 ; 
//.text:006DBA30 ; 
//.text:006DBA40 ; 
//.text:006DBA50 ; public: bool __cdecl c_animation_manager::get_current_foot_tracking_state(long, c_enum<e_foot_tracking_state, uint8, 0, 4>*, real32*, short*) const
//.text:006DBE40 ; 
//.text:006DBE80 ; public: e_foot_tracking_state __cdecl c_animation_manager::get_default_foot_tracking_state(long) const
//.text:006DBEB0 ; 

c_model_animation_graph const* c_animation_manager::get_graph() const
{
    return INVOKE_CLASS_MEMBER(0x006DBEC0, c_animation_manager, get_graph);
}

//.text:006DBED0 ; 
//.text:006DBEE0 ; 
//.text:006DBEF0 ; 
//.text:006DBF00 ; public: c_aiming_channel* __cdecl c_animation_manager::get_mixing_board_aiming_channel(long) const
//.text:006DBF30 ; public: c_animation_channel* __cdecl c_animation_manager::get_mixing_board_animation_channel(long) const
//.text:006DBF60 ; public: c_auxilary_channel* __cdecl c_animation_manager::get_mixing_board_auxilary_channel(long) const
//.text:006DBF90 ; public: c_facial_animation_channel* __cdecl c_animation_manager::get_mixing_board_facial_animation_channel() const
//.text:006DBFD0 ; public: void __cdecl c_animation_manager::get_mixing_board_node_orientations(render_model_definition const*, real32, c_static_flags<255> const*, long, real_orientation*, void(__cdecl*)(long, c_model_animation_graph const*, c_animation_id, uint32 const*, real32), long)
//.text:006DC2C0 ; public: void __cdecl c_animation_manager::get_mixing_board_render_time_node_orientations(render_model_definition const*, real32, c_static_flags<255> const*, long, real_orientation*, void(__cdecl*)(long, c_model_animation_graph const*, c_animation_id, uint32 const*, real32), long)
//.text:006DC3D0 ; public: c_mixing_board_slider* __cdecl c_animation_manager::get_mixing_board_slider(long) const
//.text:006DC400 ; public: long __cdecl c_animation_manager::get_node_count() const
//.text:006DC420 ; public: void __cdecl c_animation_manager::get_node_orientations(render_model_definition const*, real32, c_static_flags<255> const*, long, real_orientation*, void(__cdecl*)(long, c_model_animation_graph const*, c_animation_id, uint32 const*, real32), long)
//.text:006DC4F0 ; 
//.text:006DC500 ; 
//.text:006DC510 ; 
//.text:006DC520 ; 
//.text:006DC530 ; public: bool __cdecl c_animation_manager::get_pill_offset_data(real_point3d*, real_vector3d*) const
//.text:006DC570 ; 
//.text:006DC5E0 ; 
//.text:006DC5F0 ; 
//.text:006DC6A0 ; public: t_short_fixed<13> __cdecl c_animation_manager::get_slider_value_from_chain(long, e_mixing_board_parent_slider_type) const
//.text:006DC7A0 ; public: animation_graph_sound_reference const* __cdecl c_animation_manager::get_sound_reference(long) const
//.text:006DC7C0 ; public: real32 __cdecl c_animation_manager::get_state_playback_ratio() const
//.text:006DC7F0 ; 
//.text:006DC820 ; 
//.text:006DC850 ; 
//.text:006DC880 ; 

bool c_animation_manager::valid_graph() const
{
    return m_graph_tag_index != NONE;
}

long c_animation_manager::get_state_name() const
{
    return m_goal_settings.state;
}

