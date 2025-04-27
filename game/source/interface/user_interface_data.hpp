#pragma once

#include "cseries/cseries.hpp"
#include "game/players.hpp"

struct c_gui_selected_item;
struct c_gui_data
{
public:
	virtual void* destructor(uint32); // void *`scalar deleting destructor'(unsigned int)
	virtual bool initialize(long name);
	virtual void dispose();
	virtual void update();
	virtual bool is_busy() const;
	virtual long get_current_item_count();
	virtual long get_first_element_handle();
	virtual long get_next_element_handle(long element_handle);
	virtual long get_previous_element_handle(long element_handle);
	virtual void get_column_names(long* const column_names, long* column_count);
	virtual bool get_element(long element_handle, void* element, long element_size);
	virtual bool get_integer_value(long element_handle, long value_name, long* value);
	virtual bool get_text_value(long element_handle, long value_name, c_static_wchar_string<1024>* buffer);
	virtual bool get_string_id_value(long element_handle, long value_name, long* value);
	virtual bool get_qword_value(long element_handle, long value_name, uint64* value);
	virtual bool get_real_value(long element_handle, long value_name, real32* value);
	virtual bool get_player_appearance(long element_handle, s_player_appearance* appearance);
	virtual c_gui_selected_item const* get_gui_selected_item(long element_handle) const;
	virtual long get_maximum_item_count();
	virtual long add_element();
	virtual void delete_element(long element_handle);
	virtual void set_element(long element_handle, void const* element, long element_size);
	virtual bool get_invoked_control(long element_handle, long* control_name);
	virtual void clear_disabled_elements();
	virtual void set_disabled_element(long string_id_column_name, long string_id_value);
	virtual void disable_all_elements(long string_id_column_name);
	virtual void enable_element(long string_id_column_name, long string_id_value);

	c_string_id get_name() { return m_name; }

protected:
	virtual long get_current_item_count_internal();

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
		long name;
		long value;
	};
	static_assert(sizeof(s_disabled_element) == 0x8);

	s_disabled_element m_disabled_elements[k_maximum_disabled_elements];
	long m_disabled_element_count;
};
static_assert(sizeof(c_gui_ordered_data) == 0x10C);

struct c_gui_tag_datasource :
	c_gui_ordered_data
{
	long m_tag_index;
};
static_assert(sizeof(c_gui_tag_datasource) == 0x110);

