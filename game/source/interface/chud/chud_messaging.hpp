#pragma once

enum e_chud_message_context
{
	_chud_message_context_self = 0,
	_chud_message_context_friendly,
	_chud_message_context_enemy,
	_chud_message_context_neutral,

	k_chud_message_context_count
};

extern void __cdecl chud_messaging_post(long user_index, wchar_t const* message_text, e_chud_message_context chud_message_context);
extern void __cdecl chud_messaging_special_autosave(bool end);
extern void __cdecl chud_messaging_special_load(bool end);
extern void __cdecl chud_messaging_special_saving(bool end);
extern void __cdecl chud_post_action_response(long user_index);
extern bool __cdecl sub_A964E0(long user_index, long message, long a3, long placement);

