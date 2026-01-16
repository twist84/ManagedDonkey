#pragma once

#include "cseries/cseries.hpp"

struct s_observer_command;

class c_camera
{
public:
	virtual e_camera_mode get_type() const;
	virtual e_director_perspective get_perspective() const;
	virtual void update(int32 user_index, real32 dt, s_observer_command* result);
	virtual int32 get_target() const;
	virtual void set_target(int32 object_index);
	virtual void set_position(const real_point3d* position);
	virtual void set_forward(const real_vector3d* forward);
	virtual void set_roll(real32 roll);
	virtual void enable_orientation(bool enabled);
	virtual void enable_movement(bool enabled);
	virtual void enable_roll(bool enabled);
	virtual void handle_deleted_player(int32 player_index);
	virtual void handle_deleted_object(int32 object_index);
	virtual real32 get_unknown(); // c_flying_camera, c_static_camera, c_scripted_camera

	enum e_base_camera_flags
	{
		_move_instantly = 0,

		k_number_of_base_camera_flags
	};

	int32 m_target_object_index;
	c_flags<e_base_camera_flags, int32, k_number_of_base_camera_flags> m_flags;
	int32 m_move_instant_ticks;

	void set_next_move_instantly();
};
COMPILE_ASSERT(sizeof(c_camera) == 0x10);

class c_null_camera :
	public c_camera
{
public:
	byte unused[0x3C];
};
COMPILE_ASSERT(sizeof(c_null_camera) == 0x4C);

//extern const char* const (&global_camera_mode_names)[k_number_of_camera_modes];
extern const char* const global_camera_mode_names[k_number_of_camera_modes];

extern const char* camera_mode_get_name(int32 camera_mode);
extern e_camera_mode camera_mode_from_string(const char* str);

