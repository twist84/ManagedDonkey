#include "motor/actions.hpp"

#include "cseries/cseries.hpp"

//.text:00B69B20 ; bool __cdecl action_allow_interruption(e_action, e_action, const c_flags<e_action_interruption_context, uns16, 2>*)

bool __cdecl action_executing(int32 mover_index, e_action action_type)
{
	return INVOKE(0x00B69BA0, action_executing, mover_index, action_type);
}

//.text:00B69BF0 ; void __cdecl action_finished(int32, e_action)
//.text:00B69C10 ; const s_action_definition* __cdecl action_get_definition(e_action)
//.text:00B69C20 ; void __cdecl action_interrupt(int32, e_action)

// dummy declaration
bool __cdecl action_submit0(int32 mover_index, const action_request* request);
bool __cdecl action_submit(int32 mover_index, const action_request* request)
{
	return INVOKE(0x00B69C50, action_submit0, mover_index, request);
}

// dummy declaration
bool __cdecl action_submit1(int32 mover_index, e_action action_type);
bool __cdecl action_submit(int32 mover_index, e_action action_type)
{
	//return INVOKE(0x00B69CF0, action_submit1, mover_index, action_type);

	action_request request{};
	request.type = action_type;
	return action_submit(mover_index, &request);
}

//.text:00B69D20 ; bool __cdecl action_update(int32)

