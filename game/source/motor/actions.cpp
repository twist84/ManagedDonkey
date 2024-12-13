#include "motor/actions.hpp"

#include "cseries/cseries.hpp"

//.text:00B69B20 ; bool __cdecl action_allow_interruption(e_action, e_action, c_flags<e_action_interruption_context, word, 2> const*)
//.text:00B69BA0 ; bool __cdecl action_executing(long, e_action)
//.text:00B69BF0 ; void __cdecl action_finished(long, e_action)
//.text:00B69C10 ; s_action_definition const* __cdecl action_get_definition(e_action)
//.text:00B69C20 ; void __cdecl action_interrupt(long, e_action)

// dummy declaration
bool __cdecl action_submit0(long mover_index, action_request const* request);
bool __cdecl action_submit(long mover_index, action_request const* request)
{
	return INVOKE(0x00B69C50, action_submit0, mover_index, request);
}

// dummy declaration
bool __cdecl action_submit1(long mover_index, e_action action_type);
bool __cdecl action_submit(long mover_index, e_action action_type)
{
	//return INVOKE(0x00B69CF0, action_submit1, mover_index, action_type);

	action_request request{};
	request.type = action_type;
	return action_submit(mover_index, &request);
}

//.text:00B69D20 ; bool __cdecl action_update(long)

