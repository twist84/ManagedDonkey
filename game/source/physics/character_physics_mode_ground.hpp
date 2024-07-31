#include "cseries/cseries.hpp"

struct s_character_physics_update_input_datum
{
	long mode;
	byte __byte4;
	byte __byte5;
	byte __byte6;
	byte __byte7;
	void* __struct8;
	dword_flags __flagsC;
	long m_local_physics_object;
	long m_accepted_local_physics_object;
	dword __dword18;
	void* __struct1C;
	dword __dword20;
	dword __dword24;
	dword __dword28;
	real_point3d __pointd2C;
	real_point3d __pointd38;
	vector3d __vector44;
	dword __dword50;
	long m_support_havok_component_index;
	long m_support_rigid_body_index;
	real_matrix4x3 m_support_havok_component_matrix;
	vector3d m_ground_plane;
	real __real9C;
	vector3d __vectorA0;
	real m_ground_adhesion_velocity_scale;
	byte __byteB0;
	byte __byteB1;
	byte __byteB2;
	byte __byteB3;
	real __realB4;
	real __realB8;
	dword dwordBC;
	byte __byteC0;
	byte __byteC1;
	byte __byteC2;
	byte __byteC3;
	dword __dwordC4;
	dword __dwordC8;
	dword __dwordCC;
	dword __dwordD0;
	dword __dwordD4;
	dword __dwordD8;
	dword __dwordDC;
	dword __dwordE0;
	dword __dwordE4;
	dword __dwordE8;
	vector3d __vectorEC;
	real_point3d __pointF8;
	vector3d __vector104;
	vector3d __vector110;
	vector3d __vector11C;
	vector3d __vector12C;
	vector3d __vector134;
	vector3d m_early_mover_local_space_velocity;
	real __real14C;
	real __real150;
	real __real154;
	real __real158;
	real walking_speed;
	real __real160;
	real __real164;
	real airborne_acceleration;
};

struct s_havok_contact_point_reference;
struct s_character_physics_move_output_datum;
struct s_character_physics_move_input_datum;
struct s_character_physics_update_output_datum;

struct c_character_physics_mode_ground_datum
{
	vector3d const* __cdecl get_ground_plane() const;
	real __cdecl get_ground_plane_contact_offset() const;
	real __cdecl get_landing_velocity() const;
	long __cdecl get_support_havok_component_index() const;
	real_matrix4x3 const* __cdecl get_support_havok_component_matrix() const;
	long __cdecl get_support_object_index() const;
	long __cdecl get_support_rigid_body_index() const;
	void __cdecl initialize();
	void __cdecl initialize_internal(bool a1);
	void __cdecl move(s_character_physics_move_output_datum* output, s_character_physics_move_input_datum const* input);
	void __cdecl notify_in_slip_surface_volume();
	void __cdecl set_contact(long a1, long a2, vector3d const* a3);
	void __cdecl set_ground_plane(vector3d const* a1);
	void __cdecl update(s_character_physics_update_output_datum* output, struct s_character_physics_update_input_datum const* input);
	void __cdecl update_internal(s_character_physics_update_output_datum* output, s_character_physics_update_input_datum const* input, bool a2, vector3d const* a3);
	bool __cdecl update_localized_physics_space_linear_velocity(s_character_physics_update_input_datum const* input, vector3d* a1, vector3d* a2, matrix3x3* a3);

	bool __thiscall update_ground_velocity(s_character_physics_update_input_datum const* physics_input, vector3d* linear_velocity, matrix3x3* rotation);
};

