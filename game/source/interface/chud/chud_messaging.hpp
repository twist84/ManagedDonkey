#pragma once

enum e_chud_message_context
{
	_chud_message_context_self = 0,
	_chud_message_context_friendly,
	_chud_message_context_enemy,
	_chud_message_context_neutral,

	k_chud_message_context_count
};

class c_chud_messaging_manager
{
public:
	enum
	{
		k_normal_message_count = 5,
		k_medal_count = 4,
		k_normal_message_buffer_size = 96,
	};

	enum e_chud_state_text_placement
	{
		_chud_state_text_right = 0,
		_chud_state_text_left,
		_chud_state_text_bottom_state,
		_chud_state_text_bottom_primary,
		_chud_state_text_unknown4,
		_chud_state_text_unknown5,

		k_chud_state_placement_count
	};

	struct s_medal
	{
		bool valid;
		union
		{
			int32 name;
			int32 medal_index;
		};
		int32 pre_time;
		int32 post_time;
		int32 update_time;
		bool updated;
		real32 desired_slot_index;
		real32 current_slot_index;
		real32 horizontal_offset;
		real32 alpha;
	};
	static_assert(sizeof(s_medal) == 0x28);

	struct s_normal_message
	{
		wchar_t text[k_normal_message_buffer_size];
		bool precached;
		bool valid;
		int32 post_time;
		int32 precached_time;
		real32 desired_line_index;
		real32 current_line_index;
		real32 vertical_offset;
		real32 alpha;
		real32 scale;
		real32 flash;
		int32 context;
	};
	static_assert(sizeof(s_normal_message) == 0xE8);

	struct s_state_message
	{
		wchar_t text[k_normal_message_buffer_size];
		int32 key;
		bool precached;
		int32 precached_time;
		bool valid;
		real32 character_scale;
		real32 alpha;
		real32 flash;
	};
	static_assert(sizeof(s_state_message) == 0xDC);

public:
	static c_chud_messaging_manager* __cdecl get(int32 user_index);

//private:
	c_chud_messaging_manager::s_medal m_medals[4];

	byte __dataA0[0x70];

	c_chud_messaging_manager::s_normal_message m_normal_messages[5];

	byte __data598[5][0xE4];
	byte __dataA0C[6][0xE0];

	c_chud_messaging_manager::s_state_message m_state_messages[5];

	byte __data1398[5][0x14];
	bool __unknown13FC;
	bool __unknown13FD;
	bool __unknown13FE;
};
static_assert(sizeof(c_chud_messaging_manager) == 0x1400);

extern void __cdecl chud_messaging_post(int32 user_index, const wchar_t* message_text, e_chud_message_context chud_message_context);
extern void __cdecl chud_messaging_special_autosave(bool end);
extern void __cdecl chud_messaging_special_load(bool end);
extern void __cdecl chud_messaging_special_saving(bool end);
extern void __cdecl chud_post_action_response(int32 user_index);
extern bool __cdecl chud_set_state_text(int32 user_index, int32 message, int32 a3, c_chud_messaging_manager::e_chud_state_text_placement text_placement);

