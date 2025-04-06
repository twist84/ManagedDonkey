#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/files.hpp"

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
enum e_controller_index;
enum e_game_engine_type;
enum e_hopper_category_id;
enum e_network_mode_category;
enum e_network_mode_choice;
enum e_saved_film_category;
enum e_saved_game_file_type;

struct s_ui_saved_game_item_metadata
{
	qword unique_id;
	wchar_t display_name[16];
	wchar_t description[128];
	char author[16];
	e_saved_game_file_type file_type;
	bool author_is_xuid_online;
	qword author_id;
	qword size_in_bytes;
	qword date;
	long length_seconds;
	long campaign_id; // e_campaign_id
	long map_id; // e_map_id
	e_game_engine_type game_engine_index;
	e_campaign_difficulty_level campaign_difficulty;
	byte campaign_insertion_point;
	bool campaign_survival_enabled;
	word pad;
	qword game_id;
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

struct s_hopper_information
{
	wchar_t name[32];
	word hopper_identifier;
	e_hopper_category_id category_identifier;
	byte presence_hopper_identifier;
	long image_index;
	bool is_visible;
	bool is_playable;
	bool is_ranked;
	bool has_xp_penalty;
};
static_assert(sizeof(s_hopper_information) == 0x54);
static_assert(0x00 == OFFSETOF(s_hopper_information, name));
static_assert(0x40 == OFFSETOF(s_hopper_information, hopper_identifier));
static_assert(0x44 == OFFSETOF(s_hopper_information, category_identifier));
static_assert(0x48 == OFFSETOF(s_hopper_information, presence_hopper_identifier));
static_assert(0x4C == OFFSETOF(s_hopper_information, image_index));
static_assert(0x50 == OFFSETOF(s_hopper_information, is_visible));
static_assert(0x51 == OFFSETOF(s_hopper_information, is_playable));
static_assert(0x52 == OFFSETOF(s_hopper_information, is_ranked));
static_assert(0x53 == OFFSETOF(s_hopper_information, has_xp_penalty));

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

struct c_gui_difficulty_selected_item :
	public c_gui_selected_item
{
public:

//protected:
	e_campaign_difficulty_level m_difficulty;
};
static_assert(sizeof(c_gui_difficulty_selected_item) == sizeof(c_gui_selected_item) + 0x8);
static_assert(0x190 == OFFSETOF(c_gui_difficulty_selected_item, m_difficulty));

struct c_gui_game_variant_selected_item :
	public c_gui_selected_item
{
public:

//protected:
	e_controller_index m_controller_index;
	long m_variant_index;
	s_file_reference m_file_reference;
	long m_sort_order;
};
static_assert(sizeof(c_gui_game_variant_selected_item) == sizeof(c_gui_selected_item) + 0x120);
static_assert(0x190 == OFFSETOF(c_gui_game_variant_selected_item, m_controller_index));
static_assert(0x194 == OFFSETOF(c_gui_game_variant_selected_item, m_variant_index));
static_assert(0x198 == OFFSETOF(c_gui_game_variant_selected_item, m_file_reference));
static_assert(0x2A8 == OFFSETOF(c_gui_game_variant_selected_item, m_sort_order));

struct c_gui_hopper_selected_item :
	public c_gui_selected_item
{
public:

//protected:
	s_hopper_information m_hopper_information;
};
static_assert(sizeof(c_gui_hopper_selected_item) == sizeof(c_gui_selected_item) + 0x58);
static_assert(0x190 == OFFSETOF(c_gui_hopper_selected_item, m_hopper_information));

struct c_gui_level_selected_item :
	public c_gui_selected_item
{
public:

//protected:
	long m_campaign_id;
	long m_map_id;
	bool m_level_is_locked;
	short m_insertion_point;
};
static_assert(sizeof(c_gui_level_selected_item) == sizeof(c_gui_selected_item) + 0x10);
static_assert(0x190 == OFFSETOF(c_gui_level_selected_item, m_campaign_id));
static_assert(0x194 == OFFSETOF(c_gui_level_selected_item, m_map_id));
static_assert(0x198 == OFFSETOF(c_gui_level_selected_item, m_level_is_locked));
static_assert(0x19A == OFFSETOF(c_gui_level_selected_item, m_insertion_point));

struct c_gui_map_selected_item :
	public c_gui_selected_item
{
public:

//protected:
	e_controller_index m_controller_index;
	s_file_reference m_file_reference;
	long m_sort_order;
};
static_assert(sizeof(c_gui_map_selected_item) == sizeof(c_gui_selected_item) + 0x118);
static_assert(0x190 == OFFSETOF(c_gui_map_selected_item, m_controller_index));
static_assert(0x194 == OFFSETOF(c_gui_map_selected_item, m_file_reference));
static_assert(0x2A4 == OFFSETOF(c_gui_map_selected_item, m_sort_order));

struct c_gui_network_mode_selected_item :
	public c_gui_selected_item
{
public:

//protected:
	e_network_mode_category m_network_mode;
	e_network_mode_choice m_network_choice;
};
static_assert(sizeof(c_gui_network_mode_selected_item) == sizeof(c_gui_selected_item) + 0x8);
static_assert(0x190 == OFFSETOF(c_gui_network_mode_selected_item, m_network_mode));
static_assert(0x194 == OFFSETOF(c_gui_network_mode_selected_item, m_network_choice));

struct c_gui_saved_film_selected_item :
	public c_gui_selected_item
{
public:

//protected:
	e_controller_index m_controller_index;
	s_file_reference m_file_reference;
	qword m_sort_order;
	e_saved_film_category m_category;
};
static_assert(sizeof(c_gui_saved_film_selected_item) == sizeof(c_gui_selected_item) + 0x128);
static_assert(0x190 == OFFSETOF(c_gui_saved_film_selected_item, m_controller_index));
static_assert(0x194 == OFFSETOF(c_gui_saved_film_selected_item, m_file_reference));
static_assert(0x2A8 == OFFSETOF(c_gui_saved_film_selected_item, m_sort_order));
static_assert(0x2B0 == OFFSETOF(c_gui_saved_film_selected_item, m_category));

struct c_gui_saved_screenshot_selected_item :
	public c_gui_selected_item
{
public:

//protected:
	e_controller_index m_controller_index;
	s_file_reference m_file_reference;
	char m_author[16];
	qword m_date;
	qword m_size_in_bytes;
	long m_list_item_index;
};
static_assert(sizeof(c_gui_saved_screenshot_selected_item) == sizeof(c_gui_selected_item) + 0x140);
static_assert(0x190 == OFFSETOF(c_gui_saved_screenshot_selected_item, m_controller_index));
static_assert(0x194 == OFFSETOF(c_gui_saved_screenshot_selected_item, m_file_reference));
static_assert(0x2A4 == OFFSETOF(c_gui_saved_screenshot_selected_item, m_author));
static_assert(0x2B8 == OFFSETOF(c_gui_saved_screenshot_selected_item, m_date));
static_assert(0x2C0 == OFFSETOF(c_gui_saved_screenshot_selected_item, m_size_in_bytes));
static_assert(0x2C8 == OFFSETOF(c_gui_saved_screenshot_selected_item, m_list_item_index));


