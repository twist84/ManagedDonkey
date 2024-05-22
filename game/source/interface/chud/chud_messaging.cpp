#include "interface/chud/chud_messaging.hpp"

#include "cseries/cseries.hpp"

//.text:00A95850 ; void __cdecl chud_messaging_picked_up_powerup(long, long)
//.text:00A95890 ; void __cdecl chud_messaging_picked_up_weapon(long, long)
//.text:00A958D0 ; void __cdecl chud_messaging_post(long, long, e_chud_message_context)

void __cdecl chud_messaging_post(long user_index, wchar_t const* message_text, e_chud_message_context chud_message_context)
{
	INVOKE(0x00A95920, chud_messaging_post, user_index, message_text, chud_message_context);
}

//.text:00A95950 ; void __cdecl chud_messaging_post_progression_toast(long, long, real)
//.text:00A95990 ; 
//.text:00A959C0 ; 
//.text:00A95A10 ; 
//.text:00A95A40 ; 
//.text:00A95A90 ; 
//.text:00A95AC0 ; void __cdecl chud_messaging_post_primary(long, wchar_t const*, long, e_chud_message_context)
//.text:00A95B40 ; 

void __cdecl chud_messaging_special_autosave(bool end)
{
	INVOKE(0x00A95B60, chud_messaging_special_autosave, end);
}

void __cdecl chud_messaging_special_load(bool end)
{
	INVOKE(0x00A95BF0, chud_messaging_special_load, end);
}

void __cdecl chud_messaging_special_saving(bool end)
{
	INVOKE(0x00A95C80, chud_messaging_special_saving, end);
}

//.text:00A95D10 ; void __cdecl chud_post_action_response(long)

