#include "camera/camera.hpp"

#include "cseries/console.hpp"

char const* k_camera_mode_names[k_number_of_camera_modes]
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

const char* camera_mode_get_name(long camera_mode)
{
	FUNCTION_BEGIN(true);

	if (camera_mode < _camera_mode_following || camera_mode >= k_number_of_camera_modes)
		return "<invalid 'camera_mode'>";

	return k_camera_mode_names[camera_mode];
}

e_camera_mode camera_mode_from_string(const char* str)
{
	FUNCTION_BEGIN(true);

	e_camera_mode camera_mode = k_camera_mode_null;
	for (long i = _camera_mode_following; i < k_number_of_camera_modes; i++)
	{
		if (csstricmp(str, k_camera_mode_names[i]) != 0)
			continue;

		camera_mode = e_camera_mode(i);
	}

	return camera_mode;
}

void c_camera::set_next_move_instantly()
{
	FUNCTION_BEGIN(true);

	__unknownC = 5;
	m_flags |= (1 << _next_move_instantly_bit);
}