#pragma once

#include "interface/gui_selected_items.hpp"
#include "interface/user_interface_data.hpp"

class c_gui_level_selected_item :
	public c_gui_selected_item
{
public:
	c_gui_level_selected_item(const wchar_t* title, const wchar_t* description, e_campaign_id campaign_id, e_map_id map_id, bool level_is_locked, e_special_item_type special_item_type);
	c_gui_level_selected_item();
	c_gui_level_selected_item& operator=(c_gui_level_selected_item const& __that);

//protected:
	e_campaign_id m_campaign_id;
	e_map_id m_map_id;
	bool m_level_is_locked;
	int16 m_insertion_point;
};
static_assert(sizeof(c_gui_level_selected_item) == sizeof(c_gui_selected_item) + 0x10);
static_assert(0x190 == OFFSETOF(c_gui_level_selected_item, m_campaign_id));
static_assert(0x194 == OFFSETOF(c_gui_level_selected_item, m_map_id));
static_assert(0x198 == OFFSETOF(c_gui_level_selected_item, m_level_is_locked));
static_assert(0x19A == OFFSETOF(c_gui_level_selected_item, m_insertion_point));

class c_gui_level_category_datasource :
	public c_gui_ordered_data
{
public:
	virtual ~c_gui_level_category_datasource() = default;
	virtual bool initialize(string_id name) override;
	virtual void get_column_names(string_id* const column_names, int32* column_count) override;
	virtual bool get_integer_value(int32 element_handle, string_id value_name, int32* value) override;
	virtual bool get_text_value(int32 element_handle, string_id value_name, c_static_wchar_string<1024>* buffer) override;
	virtual int32 get_current_item_count_internal() override;

public:
	c_gui_level_category_datasource();

protected:
	c_static_array<c_gui_level_selected_item, 64> m_levels;
	int32 m_level_count;
};


class c_gui_level_subitem_selectable_item_datasource :
	public c_gui_ordered_data
{
public:
	struct s_level
	{
		s_level();

		wchar_t name[48];
		c_gui_level_selected_item item;
	};
	static_assert(sizeof(s_level) == 0x200);

public:
	virtual ~c_gui_level_subitem_selectable_item_datasource() = default;
	virtual void get_column_names(string_id* const column_names, int32* column_count) override;
	virtual bool get_integer_value(int32 element_handle, string_id value_name, int32* value) override;
	virtual bool get_text_value(int32 element_handle, string_id value_name, c_static_wchar_string<1024>* buffer) override;
	virtual bool get_string_id_value(int32 element_handle, string_id value_name, int32* value) override;
	virtual const c_gui_selected_item* get_gui_selected_item(int32 element_handle) const override;
	virtual int32 get_current_item_count_internal() override;
	virtual bool initialize(e_controller_index controller_index, int32 name, e_campaign_id campaign_id);

public:
	c_gui_level_subitem_selectable_item_datasource();

protected:
	c_static_array<s_level, 64> m_levels;
	int32 m_level_count;
};
//static_assert(sizeof(c_gui_level_subitem_selectable_item_datasource) == sizeof(c_gui_ordered_data) == 0x8008);

