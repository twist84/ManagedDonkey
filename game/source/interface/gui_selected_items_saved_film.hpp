#pragma once

#include "interface/gui_selected_items.hpp"
#include "interface/user_interface_data.hpp"
#include "saved_games/autosave_queue.hpp"
#include "tag_files/files.hpp"

struct game_options;
struct s_saved_game_item_metadata;

class c_gui_saved_film_selected_item :
	public c_gui_selected_item
{
public:
	bool __thiscall get_film_details_(game_options* options, int32* out_length_in_ticks, int32* out_start_tick);

public:
	virtual bool get_file_path(wchar_t* path, int32 maximum_characters) const;

public:
	c_gui_saved_film_selected_item(const s_saved_game_item_metadata* metadata, e_saved_film_category category, e_gui_selected_item_location location, e_controller_index controller_index, const s_file_reference* file_reference, int32 sort_order, bool corrupt, bool is_new);

	c_gui_saved_film_selected_item& operator=(const c_gui_saved_film_selected_item& __that);

public:
	bool get_film_details(game_options* options, int32* out_length_in_ticks, int32* out_start_tick) const;

//protected:
	e_controller_index m_controller_index;
	s_file_reference m_file_reference;
	uns64 m_sort_order;
	e_saved_film_category m_category;
};
static_assert(sizeof(c_gui_saved_film_selected_item) == 0x2B8);
static_assert(sizeof(c_gui_saved_film_selected_item) == sizeof(c_gui_selected_item) + 0x128);
static_assert(0x190 == OFFSETOF(c_gui_saved_film_selected_item, m_controller_index));
static_assert(0x194 == OFFSETOF(c_gui_saved_film_selected_item, m_file_reference));
static_assert(0x2A8 == OFFSETOF(c_gui_saved_film_selected_item, m_sort_order));
static_assert(0x2B0 == OFFSETOF(c_gui_saved_film_selected_item, m_category));

class c_gui_saved_film_subitem_datasource :
	public c_gui_ordered_data
{
public:
	void __thiscall update_autosave_enumeration_();
	void __thiscall update_content_enumeration_();

public:
	enum
	{
		k_maximum_saved_films_shown = 151,
	};

public:
	// $TODO virtual functions

protected:
	bool film_matches_category(s_saved_game_item_metadata* metadata);
	void update_autosave_enumeration();
	void update_content_enumeration();

public:
//private:
	c_static_array<c_gui_saved_film_selected_item, k_maximum_saved_films_shown> m_saved_films;
	int32 m_saved_film_count;
	e_saved_film_category m_category;
	e_controller_index m_controller_index;
	bool m_enumeration_complete;
	c_autosave_queue_enumerator m_autosave_enumerator;
};
static_assert(sizeof(c_gui_saved_film_subitem_datasource) == 0x1D8D0);
static_assert(sizeof(c_gui_saved_film_subitem_datasource) == sizeof(c_gui_ordered_data) + 0x1D7C4);
static_assert(0x00110 == OFFSETOF(c_gui_saved_film_subitem_datasource, m_saved_films));
static_assert(0x19B98 == OFFSETOF(c_gui_saved_film_subitem_datasource, m_saved_film_count));
static_assert(0x19B9C == OFFSETOF(c_gui_saved_film_subitem_datasource, m_category));
static_assert(0x19BA0 == OFFSETOF(c_gui_saved_film_subitem_datasource, m_controller_index));
static_assert(0x19BA4 == OFFSETOF(c_gui_saved_film_subitem_datasource, m_enumeration_complete));
static_assert(0x19BA8 == OFFSETOF(c_gui_saved_film_subitem_datasource, m_autosave_enumerator));

class c_gui_saved_film_category_datasource :
	public c_gui_ordered_data
{
public:
	bool __thiscall initialize_(int32 name);

public:
	struct s_film_category
	{
		wchar_t name[48];
		wchar_t description[256];
		e_saved_film_category category;
		c_gui_selected_item::e_special_item_type special_item_type;
	};
	static_assert(sizeof(s_film_category) == 0x268);

private: 
	void add_category(int32 title, int32 description, e_saved_film_category category, c_gui_selected_item::e_special_item_type special_item_type);

public:
//private:
	c_static_array<s_film_category, k_saved_film_category_count> m_saved_films;
	int32 m_saved_film_count;
	int32 m_owner_screen_index;
};
static_assert(sizeof(c_gui_saved_film_category_datasource) == 0x1454);
//static_assert(sizeof(c_gui_saved_film_category_datasource) == sizeof(c_gui_ordered_data) + 0x1348);

