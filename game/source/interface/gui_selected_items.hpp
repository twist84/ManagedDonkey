#pragma once

#include "cseries/cseries.hpp"

struct s_saved_game_item_metadata;

struct s_ui_saved_game_item_metadata
{
	void set(const s_saved_game_item_metadata* metadata);

	bool operator==(const s_ui_saved_game_item_metadata& __that) const
	{
		return csmemcmp(this, &__that, sizeof(s_ui_saved_game_item_metadata)) == 0;
	}

	uns64 unique_id;
	wchar_t display_name[16];
	wchar_t description[128];
	char author[16];
	e_saved_game_file_type file_type;
	bool author_is_xuid_online;
	uns64 author_id;
	uns64 size_in_bytes;
	uns64 date;
	int32 length_seconds;
	e_campaign_id campaign_id;
	e_map_id map_id;
	e_game_engine_type game_engine_index;
	e_campaign_difficulty_level campaign_difficulty;
	uns8 campaign_insertion_point;
	bool campaign_survival_enabled;
	uns16 pad;
	uns64 game_id;
};
COMPILE_ASSERT(sizeof(s_ui_saved_game_item_metadata) == 0x178);
COMPILE_ASSERT(0x000 == OFFSETOF(s_ui_saved_game_item_metadata, unique_id));
COMPILE_ASSERT(0x008 == OFFSETOF(s_ui_saved_game_item_metadata, display_name));
COMPILE_ASSERT(0x028 == OFFSETOF(s_ui_saved_game_item_metadata, description));
COMPILE_ASSERT(0x128 == OFFSETOF(s_ui_saved_game_item_metadata, author));
COMPILE_ASSERT(0x138 == OFFSETOF(s_ui_saved_game_item_metadata, file_type));
COMPILE_ASSERT(0x13C == OFFSETOF(s_ui_saved_game_item_metadata, author_is_xuid_online));
COMPILE_ASSERT(0x140 == OFFSETOF(s_ui_saved_game_item_metadata, author_id));
COMPILE_ASSERT(0x148 == OFFSETOF(s_ui_saved_game_item_metadata, size_in_bytes));
COMPILE_ASSERT(0x150 == OFFSETOF(s_ui_saved_game_item_metadata, date));
COMPILE_ASSERT(0x158 == OFFSETOF(s_ui_saved_game_item_metadata, length_seconds));
COMPILE_ASSERT(0x15C == OFFSETOF(s_ui_saved_game_item_metadata, campaign_id));
COMPILE_ASSERT(0x160 == OFFSETOF(s_ui_saved_game_item_metadata, map_id));
COMPILE_ASSERT(0x164 == OFFSETOF(s_ui_saved_game_item_metadata, game_engine_index));
COMPILE_ASSERT(0x168 == OFFSETOF(s_ui_saved_game_item_metadata, campaign_difficulty));
COMPILE_ASSERT(0x16C == OFFSETOF(s_ui_saved_game_item_metadata, campaign_insertion_point));
COMPILE_ASSERT(0x16D == OFFSETOF(s_ui_saved_game_item_metadata, campaign_survival_enabled));
COMPILE_ASSERT(0x16E == OFFSETOF(s_ui_saved_game_item_metadata, pad));
COMPILE_ASSERT(0x170 == OFFSETOF(s_ui_saved_game_item_metadata, game_id));

class c_gui_selected_item
{
public:
	enum e_special_item_type
	{
		_special_item_type_none = NONE,
		_special_item_type_recent,
		_special_item_type_my_file_share,
		_special_item_type_category
	};

	virtual bool is_valid() const;
	virtual e_gui_selected_item_location get_location() const;
	virtual bool get_file_path(wchar_t* path, int32 maximum_characters) const;
	virtual const wchar_t* get_title() const;
	virtual const utf8* get_description(utf8* buffer, int32 buffer_length) const;
	virtual const wchar_t* get_description() const;
	virtual const s_ui_saved_game_item_metadata* get_metadata(s_ui_saved_game_item_metadata* metadata) const;
	virtual bool get_enabled() const;
	virtual bool get_corrupt() const;
	virtual bool get_is_new() const;

//protected:
	e_gui_selected_item_type m_selection_type;
	e_special_item_type m_special_item_type;
	e_gui_selected_item_location m_location;
	bool m_enabled;
	bool m_corrupt;
	bool m_is_new;
	s_ui_saved_game_item_metadata m_metadata;
};
COMPILE_ASSERT(sizeof(c_gui_selected_item) == 0x190);
#ifdef __INTELLISENSE__
COMPILE_ASSERT(0x04 == OFFSETOF(c_gui_selected_item, m_selection_type));
COMPILE_ASSERT(0x08 == OFFSETOF(c_gui_selected_item, m_special_item_type));
COMPILE_ASSERT(0x0C == OFFSETOF(c_gui_selected_item, m_location));
COMPILE_ASSERT(0x10 == OFFSETOF(c_gui_selected_item, m_enabled));
COMPILE_ASSERT(0x11 == OFFSETOF(c_gui_selected_item, m_corrupt));
COMPILE_ASSERT(0x12 == OFFSETOF(c_gui_selected_item, m_is_new));
COMPILE_ASSERT(0x18 == OFFSETOF(c_gui_selected_item, m_metadata));
#else
COMPILE_ASSERT(0x08 == OFFSETOF(c_gui_selected_item, m_selection_type));
COMPILE_ASSERT(0x0C == OFFSETOF(c_gui_selected_item, m_special_item_type));
COMPILE_ASSERT(0x10 == OFFSETOF(c_gui_selected_item, m_location));
COMPILE_ASSERT(0x14 == OFFSETOF(c_gui_selected_item, m_enabled));
COMPILE_ASSERT(0x15 == OFFSETOF(c_gui_selected_item, m_corrupt));
COMPILE_ASSERT(0x16 == OFFSETOF(c_gui_selected_item, m_is_new));
COMPILE_ASSERT(0x18 == OFFSETOF(c_gui_selected_item, m_metadata));
#endif // __INTELLENSE__

extern bool gui_selected_item_get_file_path(c_gui_selected_item const* selected_item, wchar_t* path, int32 maximum_characters);

