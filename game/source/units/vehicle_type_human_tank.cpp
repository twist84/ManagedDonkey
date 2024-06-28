#include "units/vehicle_type_human_tank.hpp"

//.text:00BC77F0 ; public: void __cdecl c_vehicle_type_human_tank::adjust_gravity(long, vector3d*)
//.text:00BC7840 ; 
//.text:00BC78A0 ; public: bool __cdecl c_vehicle_type_human_tank::compute_function_value(long, long, real*, bool*)
//.text:00BC7C20 ; private: void __cdecl c_vehicle_type_human_tank::compute_tread_speeds(long, real*, real*)
//.text:00BC7CE0 ; public: void __cdecl c_vehicle_type_human_tank::create_effects(long)
//.text:00BC7CF0 ; public: void __cdecl c_vehicle_type_human_tank::deplete_function_variables(long)
//.text:00BC7D00 ; public: bool __cdecl c_vehicle_type_human_tank::effected_by_vehicle_ceiling(long)

s_vehicle_engine* const c_vehicle_type_human_tank::get_engine(long vehicle_index)
{
	//DECLFUNC(0x00BC7D10, s_vehicle_engine* const, __thiscall, c_vehicle_type_human_tank*, long)(this, vehicle_index);

	return &m_engine;
}

//.text:00BC7D20 ; public: bool __cdecl c_vehicle_type_human_tank::is_stopped(long)
//.text:00BC7D50 ; public: bool __cdecl c_vehicle_type_human_tank::kills_riders_at_terminal_velocity(long)
//.text:00BC7D60 ; public: bool __cdecl c_vehicle_type_human_tank::physics_disabled(long)
//.text:00BC7D70 ; public: c_vehicle_type_human_tank::process_animation_channels
//.text:00BC7D80 ; public: void __cdecl c_vehicle_type_human_tank::reset(long)
//.text:00BC7DB0 ; public: bool __cdecl c_vehicle_type_human_tank::should_override_deactivation(long)
//.text:00BC7E30 ; public: void __cdecl c_vehicle_type_human_tank::update_control(long)
//.text:00BC8430 ; public: void __cdecl c_vehicle_type_human_tank::update_physics(long, s_havok_vehicle_physics_instance*)
//.text:00BC8750 ; public: bool __cdecl c_vehicle_type_human_tank::vector_is_upsides_down(long, vector3d const*)

