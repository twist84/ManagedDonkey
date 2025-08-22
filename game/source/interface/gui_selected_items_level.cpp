#include "interface/gui_selected_items_level.hpp"

#include "tag_files/string_ids.hpp"

//.text:00B20560 ; public: c_static_array<c_gui_level_selected_item, 64>::c_static_array<c_gui_level_selected_item, 64>(void)
//.text:00B205D0 ; public: c_gui_level_category_datasource::c_gui_level_category_datasource()

c_gui_level_selected_item::c_gui_level_selected_item(const wchar_t* title, const wchar_t* description, e_campaign_id campaign_id, e_map_id map_id, bool level_is_locked, e_special_item_type special_item_type)
{
	DECLFUNC(0x00B20630, void, __thiscall, c_gui_level_selected_item*, const wchar_t*, const wchar_t*, e_campaign_id, e_map_id, bool, e_special_item_type)
		(this, title, description, campaign_id, map_id, level_is_locked, special_item_type);
}

c_gui_level_selected_item::c_gui_level_selected_item() :
	c_gui_level_selected_item(L"", L"", _campaign_id_none, _map_id_none, false, _special_item_type_none)
{
	//DECLFUNC(0x00B20680, void, __thiscall, c_gui_level_selected_item*)(this);
}

c_gui_level_selected_item& c_gui_level_selected_item::operator=(c_gui_level_selected_item const& __that)
{
	c_gui_selected_item::operator=(__that);
	m_campaign_id = __that.m_campaign_id;
	m_map_id = __that.m_map_id;
	m_level_is_locked = __that.m_level_is_locked;
	m_insertion_point = __that.m_insertion_point;

	return *this;
}

c_gui_level_category_datasource::c_gui_level_category_datasource() :
	c_gui_ordered_data(),
	m_levels(),
	m_level_count(0)
{
	//DECLFUNC(0x00B206D0, void, __thiscall, c_gui_level_subitem_selectable_item_datasource*)(this);
}

c_gui_level_subitem_selectable_item_datasource::c_gui_level_subitem_selectable_item_datasource() :
	c_gui_ordered_data(),
	m_levels(),
	m_level_count(0)
{
	//DECLFUNC(0x00B206D0, void, __thiscall, c_gui_level_subitem_selectable_item_datasource*)(this);
}

c_gui_level_subitem_selectable_item_datasource::s_level::s_level()
{
	DECLFUNC(0x00B20730, void, __thiscall, c_gui_level_subitem_selectable_item_datasource::s_level*)(this);
}

//.text:00B20780 ; 
//.text:00B20790 ; 
//.text:00B207A0 ; 
//.text:00B207B0 ; 
//.text:00B207C0 ; 
//.text:00B207E0 ; public: virtual void* c_gui_level_category_datasource::`vector deleting destructor'(unsigned int)
//.text:00B20810 ; public: virtual void* c_gui_level_subitem_selectable_item_datasource::`scalar deleting destructor'(unsigned int)

void c_gui_level_category_datasource::get_column_names(int32* const column_names, int32* column_count)
{
	//INVOKE_CLASS_MEMBER(0x00B20840, c_gui_level_category_datasource, get_column_names, column_names, column_count);

	*column_count = 0;
	column_names[*column_count++] = STRING_ID(global, category);
	column_names[*column_count++] = STRING_ID(global, title);
	column_names[*column_count++] = STRING_ID(global, description);
}

void c_gui_level_subitem_selectable_item_datasource::get_column_names(int32* const column_names, int32* column_count)
{
	//INVOKE_CLASS_MEMBER(0x00B20880, c_gui_level_subitem_selectable_item_datasource,get_column_names, column_names, column_count);

	*column_count = 0;
	column_names[*column_count++] = STRING_ID(global, title);
	column_names[*column_count++] = STRING_ID(global, description);
}

int32 c_gui_level_category_datasource::get_current_item_count_internal()
{
	//INVOKE_CLASS_MEMBER(0x00B208B0, c_gui_level_category_datasource, get_current_item_count_internal);

	return m_level_count;
}

int32 c_gui_level_subitem_selectable_item_datasource::get_current_item_count_internal()
{
	//INVOKE_CLASS_MEMBER(0x00B208C0, c_gui_level_subitem_selectable_item_datasource, get_current_item_count_internal);

	return m_level_count;
}

const c_gui_selected_item* c_gui_level_subitem_selectable_item_datasource::get_gui_selected_item(int32 element_handle) const
{
	//return INVOKE_CLASS_MEMBER(0x00B208D0, c_gui_level_subitem_selectable_item_datasource, get_gui_selected_item, element_handle);

	if (!VALID_INDEX(element_handle, m_level_count))
	{
		return NULL;
	}

	return &m_levels[element_handle].item;
}

bool c_gui_level_category_datasource::get_integer_value(int32 element_handle, int32 value_name, int32* value)
{
	return INVOKE_CLASS_MEMBER(0x00B20900, c_gui_level_category_datasource, get_integer_value, element_handle, value_name, value);
}

bool c_gui_level_subitem_selectable_item_datasource::get_integer_value(int32 element_handle, int32 value_name, int32* value)
{
	return INVOKE_CLASS_MEMBER(0x00B20950, c_gui_level_subitem_selectable_item_datasource, get_integer_value, element_handle, value_name, value);
}

bool c_gui_level_subitem_selectable_item_datasource::get_string_id_value(int32 element_handle, int32 value_name, int32* value)
{
	return INVOKE_CLASS_MEMBER(0x00B209A0, c_gui_level_subitem_selectable_item_datasource, get_string_id_value, element_handle, value_name, value);
}

bool c_gui_level_category_datasource::get_text_value(int32 element_handle, int32 value_name, c_static_wchar_string<1024>* buffer)
{
	return INVOKE_CLASS_MEMBER(0x00B209C0, c_gui_level_category_datasource, get_text_value, element_handle, value_name, buffer);
}

bool c_gui_level_subitem_selectable_item_datasource::get_text_value(int32 element_handle, int32 value_name, c_static_wchar_string<1024>* buffer)
{
	return INVOKE_CLASS_MEMBER(0x00B20A30, c_gui_level_subitem_selectable_item_datasource, get_text_value, element_handle, value_name, buffer);
}

bool c_gui_level_category_datasource::initialize(int32 name)
{
	return INVOKE_CLASS_MEMBER(0x00B20AB0, c_gui_level_category_datasource, initialize, name);
}

bool c_gui_level_subitem_selectable_item_datasource::initialize(e_controller_index controller_index, int32 name, e_campaign_id campaign_id)
{
	return INVOKE_CLASS_MEMBER(0x00B20BF0, c_gui_level_subitem_selectable_item_datasource, initialize, controller_index, name, campaign_id);
}

//.text:00B20E80 ; int32 __cdecl level_selected_item_sort_proc(const void* a, const void* b);
//.text:00B20EF0 ; int32 __cdecl level_selected_sub_item_sort_proc(const void* a, const void* b);
//.text:00B20F40 ; public: void c_static_array<c_gui_level_subitem_selectable_item_datasource::s_level, 64>::sort(int32 element_count, int(__cdecl* comparison_function)(const void*, const void*))
//.text:00B20F60 ; public: void c_static_array<c_gui_level_selected_item, 64>::sort(int32 element_count, int(__cdecl* comparison_function)(const void*, const void*))

