#pragma once

enum e_chud_message_context
{
	_chud_message_context_self = 0,
	_chud_message_context_friendly,
	_chud_message_context_enemy,
	_chud_message_context_neutral,

	k_chud_message_context_count
};

extern void _cdecl chud_messaging_post(long user_index, wchar_t const* message_text, e_chud_message_context chud_message_context);

