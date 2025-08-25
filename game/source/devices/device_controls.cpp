#include "devices/device_controls.hpp"

#include "cache/cache_files.hpp"
#include "devices/device_definitions.hpp"

void __cdecl control_destroyed(int32 control_index)
{
	INVOKE(0x00BBFB20, control_destroyed, control_index);

	//control_datum* control = CONTROL_GET(control_index);
	//struct control_definition* control_definition = TAG_GET(DEVICE_CONTROL_TAG, struct control_definition, control->definition_index);
	//if (control_definition->control.trigger == _control_trigger_destruction)
	//{
	//	control_toggle(control_index);
	//}
}

int32 __cdecl control_get_action_string(int32 control_index)
{
	return INVOKE(0x00BBFB70, control_get_action_string, control_index);

	//control_datum* control = CONTROL_GET(control_index);
	//struct control_definition* control_definition = TAG_GET(DEVICE_CONTROL_TAG, struct control_definition, control->definition_index);
	//return control_definition->control.action_string;
}

void __cdecl control_place(int32 control_index, const s_scenario_control* scenario_control)
{
	INVOKE(0x00BBFBB0, control_place, control_index, scenario_control);

	//control_datum* control = CONTROL_GET(control_index);
	//if (TEST_BIT(scenario_control->control.flags, 0))
	//	control->control.flags |= FLAG(_control_usable_from_both_sides_bit);
	//if (TEST_BIT(scenario_control->control.flags, 4))
	//	control->control.flags |= FLAG(_control_toggled_bit);
	//control->control.flags &= ~FLAG(_control_toggled_bit);
	//SET_BIT(control->control.flags, _control_toggle_only_once_bit, TEST_BIT(scenario_control->device.flags, 2));
	//control->control.hud_override_index = scenario_control->control.hud_override_string_list_index - 1;
}

void __cdecl control_toggle(int32 control_index)
{
	INVOKE(0x00BBFC30, control_toggle, control_index);

	//control_datum* control = CONTROL_GET(control_index);
	//struct control_definition* control_definition = TAG_GET(DEVICE_CONTROL_TAG, struct control_definition, control->definition_index)
	//int32 position_group_index = control->device.position_group_index;
	//// $IMPLEMENT
}

void __cdecl control_touched(int32 control_index, int32 unit_index)
{
	INVOKE(0x00BBFD50, control_touched, control_index, unit_index);

	//control_datum* control = CONTROL_GET(control_index);
	//struct control_definition* control_definition = TAG_GET(DEVICE_CONTROL_TAG, struct control_definition, control->definition_index);
	//if (control_definition->control.trigger == _control_trigger_player)
	//{
	//	control_toggle(control_index);
	//}
}

