#pragma once

#include "interface/gui_selected_items.hpp"
#include "interface/user_interface_data.hpp"
#include "saved_games/autosave_queue.hpp"
#include "tag_files/files.hpp"

class c_map_variant;

class c_gui_map_selected_item :
	public c_gui_selected_item
{
public:
	bool __thiscall get_map_variant_(c_map_variant* map_variant);

public:
	virtual bool get_file_path(wchar_t* path, int32 maximum_characters) const;

public:
	c_gui_map_selected_item(const s_ui_saved_game_item_metadata* metadata, e_gui_selected_item_location location, e_controller_index controller_index, const s_file_reference* file_reference, int32 sort_order, e_special_item_type special_item_type, bool corrupt, bool is_new);

	c_gui_map_selected_item& operator=(const c_gui_map_selected_item& __that);

public:
	bool get_map_variant(c_map_variant* map_variant) const;

//protected:
	e_controller_index m_controller_index;
	s_file_reference m_file_reference;
	int32 m_sort_order;
};
static_assert(sizeof(c_gui_map_selected_item) == sizeof(c_gui_selected_item) + 0x118);
static_assert(0x190 == OFFSETOF(c_gui_map_selected_item, m_controller_index));
static_assert(0x194 == OFFSETOF(c_gui_map_selected_item, m_file_reference));
static_assert(0x2A4 == OFFSETOF(c_gui_map_selected_item, m_sort_order));

struct c_gui_map_subitem_selectable_item_datasource : 
	public c_gui_ordered_data
{
public:
	void __thiscall update_autosave_enumeration_();
	void __thiscall update_content_enumeration_();

public:
	enum
	{
		k_maximum_maps_shown = 151,
		k_saved_game_files_enumeration_batch_size = 5,
	};

public:
	// $TODO: virtual functions

protected:
	void update_autosave_enumeration();
	void update_content_enumeration();
	static bool update_content_enumeration_proc(void* userdata, s_file_reference* found_file);

public:
//private:
	e_map_id m_enumeration_map_id;
	c_static_array<c_gui_map_selected_item, k_maximum_maps_shown> m_maps;
	int32 m_map_count;
	e_controller_index m_controller_index;
	bool m_enumeration_complete;
	c_autosave_queue_enumerator m_autosave_enumerator;
};
static_assert(sizeof(c_gui_map_subitem_selectable_item_datasource) == 0x1CF60);
static_assert(sizeof(c_gui_map_subitem_selectable_item_datasource) == sizeof(c_gui_ordered_data) + 0x1CE54);
static_assert(0x0010C == OFFSETOF(c_gui_map_subitem_selectable_item_datasource, m_enumeration_map_id));
static_assert(0x00110 == OFFSETOF(c_gui_map_subitem_selectable_item_datasource, m_maps));
static_assert(0x19228 == OFFSETOF(c_gui_map_subitem_selectable_item_datasource, m_map_count));
static_assert(0x1922C == OFFSETOF(c_gui_map_subitem_selectable_item_datasource, m_controller_index));
static_assert(0x19230 == OFFSETOF(c_gui_map_subitem_selectable_item_datasource, m_enumeration_complete));
static_assert(0x19238 == OFFSETOF(c_gui_map_subitem_selectable_item_datasource, m_autosave_enumerator));

extern int __cdecl map_selected_item_sort_proc(const void* a, const void* b);

