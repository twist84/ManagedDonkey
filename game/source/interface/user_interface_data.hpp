#pragma once

#include "cseries/cseries.hpp"
#include "game/players.hpp"

struct c_gui_selected_item;
struct c_gui_data
{
public:
	virtual void* destructor(uint32); // void *`scalar deleting destructor'(unsigned int)
	virtual bool initialize(int32 name);
	virtual void dispose();
	virtual void update();
	virtual bool is_busy() const;
	virtual int32 get_current_item_count();
	virtual int32 get_first_element_handle();
	virtual int32 get_next_element_handle(int32 element_handle);
	virtual int32 get_previous_element_handle(int32 element_handle);
	virtual void get_column_names(int32* const column_names, int32* column_count);
	virtual bool get_element(int32 element_handle, void* element, int32 element_size);
	virtual bool get_integer_value(int32 element_handle, int32 value_name, int32* value);
	virtual bool get_text_value(int32 element_handle, int32 value_name, c_static_wchar_string<1024>* buffer);
	virtual bool get_string_id_value(int32 element_handle, int32 value_name, int32* value);
	virtual bool get_qword_value(int32 element_handle, int32 value_name, uint64* value);
	virtual bool get_real_value(int32 element_handle, int32 value_name, real32* value);
	virtual bool get_player_appearance(int32 element_handle, s_player_appearance* appearance);
	virtual c_gui_selected_item const* get_gui_selected_item(int32 element_handle) const;
	virtual int32 get_maximum_item_count();
	virtual int32 add_element();
	virtual void delete_element(int32 element_handle);
	virtual void set_element(int32 element_handle, void const* element, int32 element_size);
	virtual bool get_invoked_control(int32 element_handle, int32* control_name);
	virtual void clear_disabled_elements();
	virtual void set_disabled_element(int32 string_id_column_name, int32 string_id_value);
	virtual void disable_all_elements(int32 string_id_column_name);
	virtual void enable_element(int32 string_id_column_name, int32 string_id_value);

	c_string_id get_name() { return m_name; }

protected:
	virtual int32 get_current_item_count_internal();

	c_string_id m_name;
};
static_assert(sizeof(c_gui_data) == 0x8);

struct c_gui_ordered_data :
	c_gui_data
{
protected:
	enum
	{
		k_maximum_disabled_elements = 32,
	};

	struct s_disabled_element
	{
		int32 name;
		int32 value;
	};
	static_assert(sizeof(s_disabled_element) == 0x8);

	s_disabled_element m_disabled_elements[k_maximum_disabled_elements];
	int32 m_disabled_element_count;
};
static_assert(sizeof(c_gui_ordered_data) == 0x10C);

struct c_gui_tag_datasource :
	c_gui_ordered_data
{
	int32 m_tag_index;
};
static_assert(sizeof(c_gui_tag_datasource) == 0x110);

