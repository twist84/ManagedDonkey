#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

enum e_camera_mode : dword
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
	k_camera_mode_null = 0xFFFFFFFF,
};

extern const char* camera_mode_get_name(long camera_mode);
extern e_camera_mode camera_mode_from_string(const char* str);

struct c_camera
{
	virtual e_camera_mode get_type();
	virtual long get_perspective();
	virtual void update(long, real, void*);
	virtual long get_target();
	virtual void set_target(long);
	virtual void set_position(real_point3d const*);
	virtual void set_forward(real_vector3d const*);
	virtual void set_roll(real);
	virtual void enable_orientation(bool);
	virtual void enable_movement(bool);
	virtual void enable_roll(bool);
	virtual void handle_deleted_player(long);
	virtual void handle_deleted_object(long);
	virtual real get_unknown(); // c_flying_camera, c_static_camera, c_scripted_camera

	enum e_flags
	{
		_next_move_instantly_bit = 0
	};

	datum_index m_object_index;
	dword_flags m_flags;
	dword __unknownC;

	void set_next_move_instantly();
};
static_assert(sizeof(c_camera) == 0x10);

struct c_null_camera : public c_camera
{
	byte unused[0x3C];
};
static_assert(sizeof(c_null_camera) == 0x4C);