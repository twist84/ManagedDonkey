#include "camera/debug_director.hpp"

#include "camera/camera_globals.hpp"
#include "editor/editor_stubs.hpp"
#include "main/console.hpp"
#include "memory/module.hpp"

#include <cmath>

HOOK_DECLARE_CLASS_MEMBER(0x00726170, c_debug_director, changed_camera);
HOOK_DECLARE_CLASS_MEMBER(0x007262F0, c_debug_director, _update);

byte const cycle_camera_key_code_bytes[] = { _key_code_backspace };
DATA_PATCH_DECLARE(0x007262F6 + 1, cycle_camera_key_code, cycle_camera_key_code_bytes);

//REFERENCE_DECLARE_ARRAY(0x018ECEC4, e_camera_mode, k_debug_camera_modes, 2);
e_camera_mode k_debug_camera_modes[] = { _camera_mode_flying, _camera_mode_following };

void __thiscall c_debug_director::_update(real a1)
{
	if (input_key_frames_down(_key_code_backspace, _input_type_ui) == 1)
		cycle_camera();

	mouse_state* state = input_get_mouse_state(_input_type_ui);
	if (k_debug_camera_modes[m_camera_mode_index] == _camera_mode_flying && state)
	{
		if (state->frames_down[_mouse_button_middle_click] == 1)
		{
			m_biped_control_mode = !m_biped_control_mode;
			console_printf(m_biped_control_mode ? "Biped control mode" : "Flying camera control mode");
		}

		if (g_editor_director_mouse_wheel_speed_enabled && !state->frames_down[_mouse_button_middle_click])
		{
			//g_director_camera_speed_scale = int_pin(powf(1.3f, (real)state->wheel_ticks) * g_director_camera_speed_scale, 0.0009765625f, 256.0f);

			real wheel_value = real(short(state->wheel_delta) / input_globals.mouse_wheel_delta);
			g_director_camera_speed_scale = int_pin(powf(1.3f, wheel_value) * g_director_camera_speed_scale, 0.0009765625f, 256.0f);

			//real wheel_value = real(short(state->wheel_delta) / input_globals.mouse_wheel_delta);
			//if (wheel_value < 0.0f)
			//	g_director_camera_speed_scale = int_pin(g_director_camera_speed_scale / 1.03f, 0.0009765625f, 256.0f);
			//if (wheel_value > 0.0f)
			//	g_director_camera_speed_scale = int_pin(g_director_camera_speed_scale * 1.03f, 0.0009765625f, 256.0f);
		}

		if (m_biped_control_mode || !editor_input_inhibited())
		{
			m_collision_disabled = false;
			set_camera_mode(_camera_mode_static, 0.0f);
		}
		else
		{
			m_collision_disabled = true;
			set_camera_mode(_camera_mode_flying, 0.0f);
			static_cast<c_flying_camera*>(get_camera())->set_collision(false);
		}
	}

	//c_director::update(a1);
	DECLFUNC(0x00593540, void, __thiscall, c_director*, real)(this, a1);
}

void c_debug_director::constructor(long user_index)
{
	changed_camera();
	DECLFUNC(0x007260D0, void, __thiscall, c_director*, long)(this, user_index);
}

void c_debug_director::changed_camera()
{
	set_camera_mode(k_debug_camera_modes[m_camera_mode_index], 0.0f);
	console_printf("%s camera", global_camera_mode_names[k_debug_camera_modes[m_camera_mode_index]]);
	c_camera* camera = get_camera();
	if (camera->get_type() == _camera_mode_flying)
	{
		m_collision_disabled = true;
		static_cast<c_flying_camera*>(get_camera())->set_collision(false);
	}
	else
	{
		m_collision_disabled = false;
	}
}

void c_debug_director::cycle_camera()
{
	m_camera_mode_index = (m_camera_mode_index - 1) % NUMBEROF(k_debug_camera_modes);
	changed_camera();
}

bool c_debug_director::finished_cycle()
{
	return m_camera_mode_index == NUMBEROF(k_debug_camera_modes) - 1;
}

