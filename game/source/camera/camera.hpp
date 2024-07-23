#pragma once

#include "cseries/cseries.hpp"

enum e_camera_mode
{
	_camera_mode_following = 0,
	_camera_mode_orbiting,
	_camera_mode_flying,
	_camera_mode_first_person,
	_camera_mode_dead,
	_camera_mode_static,
	_camera_mode_scripted,
	_camera_mode_authored,

	k_number_of_camera_modes,
	k_camera_mode_null = -1,
};

enum e_director_perspective;
enum e_output_user_index;

struct s_observer_command;

struct c_camera
{
	virtual e_camera_mode get_type() const;
	virtual e_director_perspective get_perspective() const;
	virtual void update(e_output_user_index output_user_index, real a2, s_observer_command* result);
	virtual long get_target() const;
	virtual void set_target(long target_index);
	virtual void set_position(real_point3d const* position);
	virtual void set_forward(vector3d const* forward);
	virtual void set_roll(real roll);
	virtual void enable_orientation(bool orientation_enable);
	virtual void enable_movement(bool movement_enable);
	virtual void enable_roll(bool roll_enable);
	virtual void handle_deleted_player(long player);
	virtual void handle_deleted_object(long object_index);
	virtual real get_unknown(); // c_flying_camera, c_static_camera, c_scripted_camera

	enum e_flags
	{
		_next_move_instantly_bit = 0
	};

	long m_object_index;
	dword_flags m_flags;
	dword __unknownC;

	void set_next_move_instantly();
};
static_assert(sizeof(c_camera) == 0x10);

struct c_null_camera :
	public c_camera
{
	byte unused[0x3C];
};
static_assert(sizeof(c_null_camera) == 0x4C);

//extern char const* const (&global_camera_mode_names)[k_number_of_camera_modes];
extern char const* const global_camera_mode_names[k_number_of_camera_modes];

extern char const* camera_mode_get_name(long camera_mode);
extern e_camera_mode camera_mode_from_string(char const* str);

