#include "camera/camera.hpp"

//REFERENCE_DECLARE_ARRAY(0x018ECDA8, char const*, global_camera_mode_names, k_number_of_camera_modes);

char const* const global_camera_mode_names[k_number_of_camera_modes]
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

char const* camera_mode_get_name(long camera_mode)
{
	if (camera_mode < _camera_mode_following || camera_mode >= k_number_of_camera_modes)
		return "<invalid 'camera_mode'>";

	return global_camera_mode_names[camera_mode];
}

e_camera_mode camera_mode_from_string(char const* str)
{
	e_camera_mode camera_mode = k_camera_mode_null;
	for (long i = _camera_mode_following; i < k_number_of_camera_modes; i++)
	{
		if (csstricmp(str, global_camera_mode_names[i]) != 0)
			continue;

		camera_mode = e_camera_mode(i);
	}

	return camera_mode;
}

void c_camera::set_next_move_instantly()
{
	__unknownC = 5;
	SET_BIT(m_flags, _next_move_instantly_bit, true);
}

