#include "motor/actions.hpp"

#include "cseries/cseries.hpp"

// action_submit
bool __cdecl action_submit_request(long object_index, action_request const* request)
{
	return INVOKE(0x00B69CF0, action_submit_request, object_index, request);
}

bool __cdecl action_submit(long object_index, e_action action)
{
	return INVOKE(0x00B69CF0, action_submit, object_index, action);
}

