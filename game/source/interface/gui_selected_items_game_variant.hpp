#pragma once

#include "interface/gui_selected_items.hpp"
#include "interface/user_interface_data.hpp"
#include "saved_games/autosave_queue.hpp"
#include "tag_files/files.hpp"

class c_gui_game_variant_selected_item :
	public c_gui_selected_item
{
public:

//protected:
	e_controller_index m_controller_index;
	int32 m_variant_index;
	s_file_reference m_file_reference;
	int32 m_sort_order;
};
static_assert(sizeof(c_gui_game_variant_selected_item) == sizeof(c_gui_selected_item) + 0x120);
static_assert(0x190 == OFFSETOF(c_gui_game_variant_selected_item, m_controller_index));
static_assert(0x194 == OFFSETOF(c_gui_game_variant_selected_item, m_variant_index));
static_assert(0x198 == OFFSETOF(c_gui_game_variant_selected_item, m_file_reference));
static_assert(0x2A8 == OFFSETOF(c_gui_game_variant_selected_item, m_sort_order));

struct c_gui_game_variant_subitem_selectable_item_datasource :
	public c_gui_ordered_data
{
public:
	enum
	{
		k_maximum_game_variants_shown = 151,
		k_saved_game_files_enumeration_batch_size = 5,
	};

public:
	// $TODO: virtual functions

//private:
	e_game_engine_type m_game_engine_type;
	c_static_array<c_gui_game_variant_selected_item, k_maximum_game_variants_shown> m_game_variants;
	int32 m_game_variant_count;
	e_controller_index m_controller_index;
	bool m_enumeration_complete;
	c_autosave_queue_enumerator m_autosave_enumerator;
};
static_assert(sizeof(c_gui_game_variant_subitem_selectable_item_datasource) == 0x1D418);
static_assert(sizeof(c_gui_game_variant_subitem_selectable_item_datasource) == sizeof(c_gui_ordered_data) + 0x1D30C);
static_assert(0x0010C == OFFSETOF(c_gui_game_variant_subitem_selectable_item_datasource, m_game_engine_type));
static_assert(0x00110 == OFFSETOF(c_gui_game_variant_subitem_selectable_item_datasource, m_game_variants));
static_assert(0x196E0 == OFFSETOF(c_gui_game_variant_subitem_selectable_item_datasource, m_game_variant_count));
static_assert(0x196E4 == OFFSETOF(c_gui_game_variant_subitem_selectable_item_datasource, m_controller_index));
static_assert(0x196E8 == OFFSETOF(c_gui_game_variant_subitem_selectable_item_datasource, m_enumeration_complete));
static_assert(0x196F0 == OFFSETOF(c_gui_game_variant_subitem_selectable_item_datasource, m_autosave_enumerator));

