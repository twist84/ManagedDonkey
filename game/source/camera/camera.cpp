#include "camera/camera.hpp"

//REFERENCE_DECLARE_ARRAY(0x018ECDA8, const char*, global_camera_mode_names, k_number_of_camera_modes);

const char* const global_camera_mode_names[k_number_of_camera_modes]
{
	"following",
	"orbiting",
	"flying",
	"first_person",
	"dead",
	"static",
	"scripted",
	"authored",
};

const char* camera_mode_get_name(int32 camera_mode)
{
	if (camera_mode < _camera_mode_following || camera_mode >= k_number_of_camera_modes)
		return "<invalid 'camera_mode'>";

	return global_camera_mode_names[camera_mode];
}

e_camera_mode camera_mode_from_string(const char* str)
{
	e_camera_mode camera_mode = k_camera_mode_null;
	for (int32 i = _camera_mode_following; i < k_number_of_camera_modes; i++)
	{
		if (csstricmp(str, global_camera_mode_names[i]) != 0)
			continue;

		camera_mode = e_camera_mode(i);
	}

	return camera_mode;
}

void c_camera::set_next_move_instantly()
{
	m_move_instant_ticks = 5;
	m_flags.set(_move_instantly, true);
}

