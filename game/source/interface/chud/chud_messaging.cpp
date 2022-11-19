#include "interface/chud/chud_messaging.hpp"

#include "cseries/cseries.hpp"

void _cdecl chud_messaging_post(long user_index, wchar_t const* message_text, e_chud_message_context chud_message_context)
{
	INVOKE(0x00A95920, chud_messaging_post, user_index, message_text, chud_message_context);
}

