#include "motor/sync_action.hpp"

REFERENCE_DECLARE(0x05287C15, bool, debug_enable_force_phonebooth_assassinate);

//.text:00BA8360 ; public: __cdecl c_sync_action_iterator::c_sync_action_iterator(int32, int32)
//.text:00BA8390 ; 
//.text:00BA83A0 ; 
//.text:00BA83B0 ; 
//.text:00BA83C0 ; 
//.text:00BA83D0 ; bool __cdecl action_sync_animation_setup(int32, int32, e_action, const real_point3d*, const real_vector3d*, const real_vector3d*, real32, bool, bool)
//.text:00BA8A20 ; bool __cdecl action_sync_animation_setup_on_participant(int32, bool, e_action, c_animation_id, int32, const real_point3d*, const real_vector3d*, const real_vector3d*, bool, real32)
//.text:00BA8C10 ; void __cdecl add_object_to_sync_action_participant_circular_list(int32, int32)
//.text:00BA8C80 ; public: bool __cdecl c_animation_sync_participant_tracker::add_participant(int32)
//.text:00BA8D20 ; bool __cdecl assassination_sync_action_collides_or_falls(int32, int32, const s_animation_sync_action*, const s_animation_sync_action_same_type_participant*, const s_animation_sync_action_same_type_participant*, bool)
//.text:00BA93D0 ; bool __cdecl assassination_sync_action_point_on_slope_or_falls(int32, real32, real32, const real_point3d*, bool, real32*)
//.text:00BA9510 ; 
//.text:00BA96A0 ; public: bool __cdecl c_animation_sync_participant_tracker::clean_up_sync_action(void)
//.text:00BA97B0 ; 
//.text:00BA97C0 ; 
//.text:00BA97D0 ; void __cdecl find_best_assassination_sync_action_in_group(int32, int32, int32, const real_point3d*, const real_vector3d*, const s_animation_sync_action**, const s_animation_sync_action_same_type_participant**, const s_animation_sync_action_same_type_participant**)
//.text:00BA9BD0 ; e_sync_action_search_result __cdecl find_best_assassination_sync_action_to_run(int32, int32, const s_animation_sync_action**)
//.text:00BA9D10 ; suggest_sync_action_to_actor
//.text:00BAA140 ; 
//.text:00BAA180 ; bool __cdecl find_two_compatible_sync_action_participants(int32, int32, const s_animation_sync_action*, real_point3d*, real_vector3d*, const s_animation_sync_action_same_type_participant**, real_point3d*, real_vector3d*, const s_animation_sync_action_same_type_participant**, bool*)
//.text:00BAA5D0 ; 
//.text:00BAA5E0 ; 
//.text:00BAA5F0 ; 
//.text:00BAA600 ; 
//.text:00BAA610 ; 
//.text:00BAA620 ; public: int32 __cdecl c_animation_sync_participant_tracker::get_participant_count()
//.text:00BAA680 ; public: void __cdecl c_sync_action_iterator::initialize(int32, int32)
//.text:00BAA750 ; 
//.text:00BAA810 ; public: const s_animation_sync_action* __cdecl c_sync_action_iterator::next()
//.text:00BAA9E0 ; 
//.text:00BAAA40 ; void __cdecl remove_object_from_sync_action_participant_circular_list(int32)
//.text:00BAAAB0 ; public: void __cdecl c_animation_sync_participant_tracker::report_cast_result(int32, bool)
//.text:00BAAB40 ; 
//.text:00BAAB70 ; 
//.text:00BAABE0 ; 
//.text:00BAAF60 ; 
//.text:00BAAF90 ; 
//.text:00BAB0B0 ; void __cdecl sync_action_compute_world_origin_and_axes(int32, int32, const s_animation_sync_action*, real_point3d*, real_vector3d*, real_vector3d*)
//.text:00BAB0E0 ; void __cdecl sync_action_compute_world_origin_and_axes(int32, int32, const s_animation_sync_action*, real_point3d*, real_vector3d*, real_vector3d*, e_sync_action_origin_calculation_type)
//.text:00BAB500 ; void __cdecl sync_action_handle_deleted_object(int32)
//.text:00BAB520 ; 
//.text:00BAB580 ; 
//.text:00BAB5F0 ; 
//.text:00BAB660 ; 
//.text:00BAB6D0 ; 
//.text:00BAB740 ; bool __cdecl target_satisfies_airborne_state(int32, int32, bool)

