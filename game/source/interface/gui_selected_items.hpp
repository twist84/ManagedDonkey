#pragma once

#include "cseries/cseries.hpp"

enum e_gui_selected_item_location
{
	_gui_stored_item_location_none = NONE,
	_gui_stored_item_location_built_in,
	_gui_stored_item_location_saved_game_file,
	_gui_stored_item_location_autosave_queue,

	k_gui_stored_item_location_count
};

enum e_gui_selected_item_type
{
	_gui_selection_type_none = NONE,
	_gui_selection_type_level,
	_gui_selection_type_difficulty,
	_gui_selection_type_map,
	_gui_selection_type_game_variant,
	_gui_selection_type_hopper,
	_gui_selection_type_film,
	_gui_selection_type_screenshot,
	_gui_selection_type_network_mode,

	k_gui_selected_item_type_count
};

enum e_campaign_difficulty_level;
enum e_campaign_id;
enum e_game_engine_type;
enum e_map_id;
enum e_saved_game_file_type;

struct s_ui_saved_game_item_metadata
{
	uint64 unique_id;
	wchar_t display_name[16];
	wchar_t description[128];
	char author[16];
	e_saved_game_file_type file_type;
	bool author_is_xuid_online;
	uint64 author_id;
	uint64 size_in_bytes;
	uint64 date;
	long length_seconds;
	e_campaign_id campaign_id;
	e_map_id map_id;
	e_game_engine_type game_engine_index;
	e_campaign_difficulty_level campaign_difficulty;
	uint8 campaign_insertion_point;
	bool campaign_survival_enabled;
	uint16 pad;
	uint64 game_id;
};
static_assert(sizeof(s_ui_saved_game_item_metadata) == 0x178);
static_assert(0x000 == OFFSETOF(s_ui_saved_game_item_metadata, unique_id));
static_assert(0x008 == OFFSETOF(s_ui_saved_game_item_metadata, display_name));
static_assert(0x028 == OFFSETOF(s_ui_saved_game_item_metadata, description));
static_assert(0x128 == OFFSETOF(s_ui_saved_game_item_metadata, author));
static_assert(0x138 == OFFSETOF(s_ui_saved_game_item_metadata, file_type));
static_assert(0x13C == OFFSETOF(s_ui_saved_game_item_metadata, author_is_xuid_online));
static_assert(0x140 == OFFSETOF(s_ui_saved_game_item_metadata, author_id));
static_assert(0x148 == OFFSETOF(s_ui_saved_game_item_metadata, size_in_bytes));
static_assert(0x150 == OFFSETOF(s_ui_saved_game_item_metadata, date));
static_assert(0x158 == OFFSETOF(s_ui_saved_game_item_metadata, length_seconds));
static_assert(0x15C == OFFSETOF(s_ui_saved_game_item_metadata, campaign_id));
static_assert(0x160 == OFFSETOF(s_ui_saved_game_item_metadata, map_id));
static_assert(0x164 == OFFSETOF(s_ui_saved_game_item_metadata, game_engine_index));
static_assert(0x168 == OFFSETOF(s_ui_saved_game_item_metadata, campaign_difficulty));
static_assert(0x16C == OFFSETOF(s_ui_saved_game_item_metadata, campaign_insertion_point));
static_assert(0x16D == OFFSETOF(s_ui_saved_game_item_metadata, campaign_survival_enabled));
static_assert(0x16E == OFFSETOF(s_ui_saved_game_item_metadata, pad));
static_assert(0x170 == OFFSETOF(s_ui_saved_game_item_metadata, game_id));

struct c_gui_selected_item
{
public:
	enum e_special_item_type
	{
		_special_item_type_none = NONE,
		_special_item_type_recent,
		_special_item_type_my_file_share,
		_special_item_type_category
	};

	virtual bool is_valid();
	virtual e_gui_selected_item_location get_location();
	virtual bool get_file_path(char*, int);
	virtual wchar_t const* get_title();
	virtual utf8 const* get_description(utf8*, int);
	virtual wchar_t const* get_description();
	virtual s_ui_saved_game_item_metadata const* get_metadata(s_ui_saved_game_item_metadata*);
	virtual bool get_enabled();
	virtual bool get_corrupt();
	virtual bool get_is_new();

//protected:
	e_gui_selected_item_type m_selection_type;
	e_special_item_type m_special_item_type;
	e_gui_selected_item_location m_location;
	bool m_enabled;
	bool m_corrupt;
	bool m_is_new;
	s_ui_saved_game_item_metadata m_metadata;
};
static_assert(sizeof(c_gui_selected_item) == 0x190);
#ifdef __INTELLISENSE__
static_assert(0x04 == OFFSETOF(c_gui_selected_item, m_selection_type));
static_assert(0x08 == OFFSETOF(c_gui_selected_item, m_special_item_type));
static_assert(0x0C == OFFSETOF(c_gui_selected_item, m_location));
static_assert(0x10 == OFFSETOF(c_gui_selected_item, m_enabled));
static_assert(0x11 == OFFSETOF(c_gui_selected_item, m_corrupt));
static_assert(0x12 == OFFSETOF(c_gui_selected_item, m_is_new));
static_assert(0x18 == OFFSETOF(c_gui_selected_item, m_metadata));
#else
static_assert(0x08 == OFFSETOF(c_gui_selected_item, m_selection_type));
static_assert(0x0C == OFFSETOF(c_gui_selected_item, m_special_item_type));
static_assert(0x10 == OFFSETOF(c_gui_selected_item, m_location));
static_assert(0x14 == OFFSETOF(c_gui_selected_item, m_enabled));
static_assert(0x15 == OFFSETOF(c_gui_selected_item, m_corrupt));
static_assert(0x16 == OFFSETOF(c_gui_selected_item, m_is_new));
static_assert(0x18 == OFFSETOF(c_gui_selected_item, m_metadata));
#endif // __INTELLENSE__


