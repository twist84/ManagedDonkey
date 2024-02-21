#pragma once

#include "cseries/cseries.hpp"
#include "game/players.hpp"

struct c_gui_selected_item;
struct c_gui_data
{
public:
	virtual void* destructor(dword);
	virtual bool initialize(long);
	virtual void dispose();
	virtual void update();
	virtual void __func4();
	virtual long get_current_item_count();
	virtual long get_first_element_handle();
	virtual long get_next_element_handle(long);
	virtual long get_previous_element_handle(long);
	virtual void get_column_names(long* const, long*);
	virtual bool get_element(long, void*, long);
	virtual bool get_integer_value(long, long, long*);
	virtual bool get_text_value(long, long, c_static_wchar_string<1024>*);
	virtual bool get_string_id_value(long, long, long*);
	virtual bool get_qword_value(long, long, qword*);
	virtual bool get_real_value(long, long, real*);
	virtual bool get_player_appearance(long, s_player_appearance*);
	virtual c_gui_selected_item const* get_gui_selected_item(long) const;
	virtual long get_maximum_item_count();
	virtual long add_element();
	virtual void delete_element(long);
	virtual void set_element(long, void const*, long);
	virtual bool get_invoked_control(long, long*);
	virtual void clear_disabled_elements();
	virtual void set_disabled_element(long, long);
	virtual void disable_all_elements(long);
	virtual void enable_element(long, long);

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
	struct
	{
		long __unknown0;
		long __unknown4;
	} m_disabled_elements[32];
	long m_disabled_element_count;
};
static_assert(sizeof(c_gui_ordered_data) == 0x10C);

struct c_gui_tag_datasource :
	c_gui_ordered_data
{
	long m_gui_datasource_definition_index;
};
static_assert(sizeof(c_gui_tag_datasource) == 0x110);

