#pragma once

#include "cseries/cseries.hpp"
#include "text/unicode.hpp"

class c_gui_selected_item;
struct s_data_array;
struct s_player_appearance;

class c_gui_data
{
public:
	virtual ~c_gui_data();
	virtual bool initialize(string_id name);
	virtual void dispose();
	virtual void update();
	virtual bool is_busy() const;
	virtual int32 get_current_item_count() = 0;
	virtual int32 get_first_element_handle() = 0;
	virtual int32 get_next_element_handle(int32 element_handle) = 0;
	virtual int32 get_previous_element_handle(int32 element_handle) = 0;
	virtual void get_column_names(string_id* const column_names, int32* column_count) = 0;
	virtual bool get_element(int32 element_handle, void* element, int32 element_size);
	virtual bool get_integer_value(int32 element_handle, string_id value_name, int32* value);
	virtual bool get_text_value(int32 element_handle, string_id value_name, c_static_wchar_string<1024>* buffer);
	virtual bool get_string_id_value(int32 element_handle, string_id value_name, int32* value);
	virtual bool get_qword_value(int32 element_handle, string_id value_name, uns64* value);
	virtual bool get_real_value(int32 element_handle, string_id value_name, real32* value);
	virtual bool get_player_appearance(int32 element_handle, s_player_appearance* appearance);
	virtual const c_gui_selected_item* get_gui_selected_item(int32 element_handle) const;
	virtual int32 get_maximum_item_count();
	virtual int32 add_element();
	virtual void delete_element(int32 element_handle);
	virtual void set_element(int32 element_handle, const void* element, int32 element_size);
	virtual bool get_invoked_control(int32 element_handle, string_id* control_name);
	virtual void clear_disabled_elements();
	virtual void set_disabled_element(string_id string_id_column_name, string_id string_id_value);
	virtual void disable_all_elements(string_id string_id_column_name);
	virtual void enable_element(string_id string_id_column_name, string_id string_id_value);

public:
	c_gui_data();
	bool contains(int32 element_handle);
	int32 find_element_handle_from_string_id_value(string_id column_name, int32 value);
	string_id get_name();
	void set_name(int32 name);

protected:
	string_id m_name;
};
static_assert(sizeof(c_gui_data) == 0x8);

class c_gui_data_array :
	public c_gui_data
{
public:
	virtual ~c_gui_data_array();
	virtual bool initialize(string_id name) override;
	virtual int32 get_current_item_count() override;
	virtual int32 get_first_element_handle() override;
	virtual int32 get_next_element_handle(int32 element_handle) override;
	virtual int32 get_previous_element_handle(int32 element_handle) override;
	virtual bool get_element(int32 element_handle, void* element, int32 element_size) override;
	virtual int32 get_maximum_item_count() override;
	virtual int32 add_element() override;
	virtual void delete_element(int32 element_handle) override;
	virtual void set_element(int32 element_handle, const void* element, int32 element_size) override;

public:
	c_gui_data_array();

protected:
	s_data_array* m_list_data;
};
static_assert(sizeof(c_gui_data_array) == 0xC);
static_assert(sizeof(c_gui_data_array) == sizeof(c_gui_data) + 0x4);

class c_gui_data_array_test:
	public c_gui_data_array
{
public:
	enum
	{
		k_test_datum_count = 101,
	};

	struct s_test_datum :
		s_datum_header
	{
		int16 pad;
	};

public:
	virtual ~c_gui_data_array_test() = default;
	virtual bool initialize(string_id name) override;
	virtual void get_column_names(string_id* const column_names, int32* column_count) override;
	virtual bool get_integer_value(int32 element_handle, string_id value_name, int32* value) override;
	virtual bool get_text_value(int32 element_handle, string_id value_name, c_static_wchar_string<1024>* buffer) override;
	virtual bool get_string_id_value(int32 element_handle, string_id value_name, int32* value) override;

public:
	c_gui_data_array_test();

protected:
	s_data_array* m_list_data;
};
static_assert(sizeof(c_gui_data_array) == 0xC);
static_assert(sizeof(c_gui_data_array) == sizeof(c_gui_data) + 0x4);

class c_gui_ordered_data :
	public c_gui_data
{
public:
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

public:
	virtual ~c_gui_ordered_data() = default;
	virtual int32 get_current_item_count() override;
	virtual int32 get_first_element_handle() override;
	virtual int32 get_next_element_handle(int32 element_handle) override;
	virtual int32 get_previous_element_handle(int32 element_handle) override;
	virtual void clear_disabled_elements() override;
	virtual void set_disabled_element(string_id string_id_column_name, string_id string_id_value) override;
	virtual void disable_all_elements(string_id string_id_column_name) override;
	virtual void enable_element(string_id string_id_column_name, string_id string_id_value) override;

protected:
	virtual int32 get_current_item_count_internal() = 0;

public:
	c_gui_ordered_data();

protected:
	s_disabled_element m_disabled_elements[k_maximum_disabled_elements];
	int32 m_disabled_element_count;
};
static_assert(sizeof(c_gui_ordered_data) == 0x10C);
static_assert(sizeof(c_gui_ordered_data) == sizeof(c_gui_data) + 0x104);

class c_gui_tag_datasource :
	public c_gui_ordered_data
{
public:
	virtual ~c_gui_tag_datasource() = default;
	virtual bool initialize(string_id name) override;
	virtual void get_column_names(string_id* const column_names, int32* column_count) override;
	virtual bool get_element(int32 element_handle, void* element, int32 element_size) override;
	virtual bool get_integer_value(int32 element_handle, string_id value_name, int32* value) override;
	virtual bool get_text_value(int32 element_handle, string_id value_name, c_static_wchar_string<1024>* buffer) override;
	virtual bool get_string_id_value(int32 element_handle, string_id value_name, int32* value) override;
	virtual bool get_invoked_control(int32 element_handle, string_id* control_name) override;
	virtual int32 get_current_item_count_internal() override;

public:
	c_gui_tag_datasource(int32 tag_index);
	c_gui_tag_datasource();

protected:
	int32 m_tag_index;
};
static_assert(sizeof(c_gui_tag_datasource) == 0x110);
static_assert(sizeof(c_gui_tag_datasource) == sizeof(c_gui_ordered_data) + 0x4);

