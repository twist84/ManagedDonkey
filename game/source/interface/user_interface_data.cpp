#include "interface/user_interface_data.hpp"

#include "memory/data.hpp"

c_gui_data::c_gui_data() :
	m_name(0)
{
	//DECLFUNC(0x00AD4500, void, __thiscall, c_gui_data*)(this);
}

c_gui_data_array::c_gui_data_array() :
	c_gui_data(),
	m_list_data(NULL)
{
	//DECLFUNC(0x00AD4510, void, __thiscall, c_gui_data_array*)(this);
}

c_gui_data_array_test::c_gui_data_array_test() :
	c_gui_data_array(),
	m_list_data(NULL)
{
	//DECLFUNC(0x00AD4530, void, __thiscall, c_gui_data_array_test*)(this);
}

c_gui_ordered_data::c_gui_ordered_data() :
	c_gui_data(),
	m_disabled_elements(),
	m_disabled_element_count(0)
{
	//DECLFUNC(0x00AD4550, void, __thiscall, c_gui_ordered_data*)(this);
}

c_gui_tag_datasource::c_gui_tag_datasource(int32 tag_index) :
	c_gui_ordered_data(),
	m_tag_index(tag_index)
{
	//DECLFUNC(0x00AD4570, void, __thiscall, c_gui_tag_datasource*)(this);
}

c_gui_tag_datasource::c_gui_tag_datasource() :
	c_gui_tag_datasource(NONE)
{
	//DECLFUNC(0x00AD45A0, void, __thiscall, c_gui_tag_datasource*)(this);
}

//.text:00AD45D0 ; public: virtual __cdecl c_gui_data::~c_gui_data(void)
c_gui_data::~c_gui_data()
{
	//DECLFUNC(0x00AD45D0, void, __thiscall, c_gui_data*)(this);
}

//.text:00AD45E0 ; public: virtual __cdecl c_gui_data_array::~c_gui_data_array(void)
c_gui_data_array::~c_gui_data_array()
{
	//DECLFUNC(0x00AD45E0, void, __thiscall, c_gui_data_array*)(this);

	if (m_list_data)
	{
		data_dispose(m_list_data);
		m_list_data = NULL;
	}
}

//.text:00AD46A0 ; public: virtual void* c_gui_data::`scalar deleting destructor'(unsigned int)
//.text:00AD46D0 ; public: virtual void* c_gui_data_array::`vector deleting destructor'(unsigned int)

//.text:00AD4740 ; public: virtual void* c_gui_data_array_test::`vector deleting destructor'(unsigned int)
//.text:00AD47B0 ; public: virtual void* c_gui_ordered_data::`vector deleting destructor'(unsigned int)
//.text:00AD47E0 ; public: virtual void* c_gui_tag_datasource::`vector deleting destructor'(unsigned int)

int32 c_gui_data::add_element()
{
	//return INVOKE_CLASS_MEMBER(0x00AD4810, c_gui_data, add_element);

	return NONE;
}

int32 c_gui_data_array::add_element()
{
	return INVOKE_CLASS_MEMBER(0x00AD4820, c_gui_data_array, add_element);
}

void c_gui_data::clear_disabled_elements()
{
	//INVOKE_CLASS_MEMBER(0x00AD4830, c_gui_data, clear_disabled_elements);
}

void c_gui_ordered_data::clear_disabled_elements()
{
	INVOKE_CLASS_MEMBER(0x00AD4840, c_gui_ordered_data, clear_disabled_elements);
}

bool c_gui_data::contains(int32 element_handle)
{
	return INVOKE_CLASS_MEMBER(0x00AD4850, c_gui_data, contains, element_handle);

	//int32 next_handle = get_first_element_handle();
	//if (next_handle == NONE)
	//{
	//	return false;
	//}
	//
	//while (next_handle != element_handle)
	//{
	//	next_handle = get_next_element_handle(next_handle);
	//	if (next_handle == NONE)
	//	{
	//		return false;
	//	}
	//}
	//
	//return true;
}

void c_gui_data::delete_element(int32 element_handle)
{
	//INVOKE_CLASS_MEMBER(0x00AD48A0, c_gui_data, delete_element, element_handle);

	VASSERT(0, "not implemented");
}

void c_gui_data_array::delete_element(int32 element_handle)
{
	INVOKE_CLASS_MEMBER(0x00AD48B0, c_gui_data_array, delete_element, element_handle);
}

void c_gui_data::disable_all_elements(string_id string_id_column_name)
{
	//INVOKE_CLASS_MEMBER(0x00AD48D0, c_gui_data, disable_all_elements, string_id_column_name);

	VASSERT(0, "datasource doesn't support enabling and disabling");
}

void c_gui_ordered_data::disable_all_elements(string_id string_id_column_name)
{
	INVOKE_CLASS_MEMBER(0x00AD48E0, c_gui_ordered_data, disable_all_elements, string_id_column_name);
}

void c_gui_data::dispose()
{
	//INVOKE_CLASS_MEMBER(0x00AD4950, c_gui_data, dispose);
}

void c_gui_data::enable_element(string_id string_id_column_name, string_id string_id_value)
{
	//INVOKE_CLASS_MEMBER(0x00AD4960, c_gui_data, enable_element, string_id_column_name, string_id_value);

	VASSERT(0, "datasource doesn't support enabling and disabling");
}

void c_gui_ordered_data::enable_element(string_id string_id_column_name, string_id string_id_value)
{
	INVOKE_CLASS_MEMBER(0x00AD4970, c_gui_ordered_data, enable_element, string_id_column_name, string_id_value);
}

int32 c_gui_data::find_element_handle_from_string_id_value(string_id column_name, int32 value)
{
	return INVOKE_CLASS_MEMBER(0x00AD49E0, c_gui_data, find_element_handle_from_string_id_value, column_name, value);
}

void c_gui_data_array_test::get_column_names(string_id* const column_names, int32* column_count)
{
	INVOKE_CLASS_MEMBER(0x00AD4A50, c_gui_data_array_test, get_column_names, column_names, column_count);
}

void c_gui_tag_datasource::get_column_names(string_id* const column_names, int32* column_count)
{
	INVOKE_CLASS_MEMBER(0x00AD4A60, c_gui_tag_datasource, get_column_names, column_names, column_count);
}

int32 c_gui_data_array::get_current_item_count()
{
	return INVOKE_CLASS_MEMBER(0x00AD4BD0, c_gui_data_array, get_current_item_count);
}

int32 c_gui_ordered_data::get_current_item_count()
{
	return INVOKE_CLASS_MEMBER(0x00AD4BE0, c_gui_ordered_data, get_current_item_count);
}

int32 c_gui_tag_datasource::get_current_item_count_internal()
{
	return INVOKE_CLASS_MEMBER(0x00AD4C90, c_gui_tag_datasource, get_current_item_count_internal);
}

//.text:00AD4CC0 ; private: const s_datasource_definition* c_gui_tag_datasource::get_definition()

bool c_gui_data::get_element(int32 element_handle, void* element, int32 element_size)
{
	//return INVOKE_CLASS_MEMBER(0x00AD4CE0, c_gui_data, get_element, element_handle, element, element_size);

	return false;
}

bool c_gui_data_array::get_element(int32 element_handle, void* element, int32 element_size)
{
	return INVOKE_CLASS_MEMBER(0x00AD4CF0, c_gui_data_array, get_element, element_handle, element, element_size);
}

bool c_gui_tag_datasource::get_element(int32 element_handle, void* element, int32 element_size)
{
	return INVOKE_CLASS_MEMBER(0x00AD4D30, c_gui_tag_datasource, get_element, element_handle, element, element_size);
}

int32 c_gui_data_array::get_first_element_handle()
{
	return INVOKE_CLASS_MEMBER(0x00AD4DB0, c_gui_data_array, get_first_element_handle);
}

int32 c_gui_ordered_data::get_first_element_handle()
{
	return INVOKE_CLASS_MEMBER(0x00AD4DD0, c_gui_ordered_data, get_first_element_handle);
}

const c_gui_selected_item* c_gui_data::get_gui_selected_item(int32 element_handle) const
{
	//return INVOKE_CLASS_MEMBER(0x00AD4E70, c_gui_data, get_gui_selected_item, element_handle);

	return NULL;
}

bool c_gui_data::get_integer_value(int32 element_handle, string_id value_name, int32* value)
{
	//return INVOKE_CLASS_MEMBER(0x00AD4E80, c_gui_data, get_integer_value, element_handle, value_name, value);

	return false;
}

bool c_gui_data_array_test::get_integer_value(int32 element_handle, string_id value_name, int32* value)
{
	return INVOKE_CLASS_MEMBER(0x00AD4E90, c_gui_data_array_test, get_integer_value, element_handle, value_name, value);
}

bool c_gui_tag_datasource::get_integer_value(int32 element_handle, string_id value_name, int32* value)
{
	return INVOKE_CLASS_MEMBER(0x00AD4ED0, c_gui_tag_datasource, get_integer_value, element_handle, value_name, value);
}

bool c_gui_data::get_invoked_control(int32 element_handle, string_id* control_name)
{
	//return INVOKE_CLASS_MEMBER(0x00AD4F40, c_gui_data, get_invoked_control, element_handle, control_name);

	return false;
}


bool c_gui_tag_datasource::get_invoked_control(int32 element_handle, string_id* control_name)
{
	return INVOKE_CLASS_MEMBER(0x00AD4F50, c_gui_tag_datasource, get_invoked_control, element_handle, control_name);
}

int32 c_gui_data::get_maximum_item_count()
{
	//return INVOKE_CLASS_MEMBER(0x00AD4F90, c_gui_data, get_maximum_item_count);

	return get_current_item_count();
}

int32 c_gui_data_array::get_maximum_item_count()
{
	return INVOKE_CLASS_MEMBER(0x00AD4FA0, c_gui_data_array, get_maximum_item_count);
}

int32 c_gui_data_array::get_next_element_handle(int32 element_handle)
{
	return INVOKE_CLASS_MEMBER(0x00AD4FB0, c_gui_data_array, get_next_element_handle, element_handle);
}

int32 c_gui_ordered_data::get_next_element_handle(int32 element_handle)
{
	return INVOKE_CLASS_MEMBER(0x00AD4FE0, c_gui_ordered_data, get_next_element_handle, element_handle);
}

bool c_gui_data::get_player_appearance(int32 element_handle, s_player_appearance* appearance)
{
	//return INVOKE_CLASS_MEMBER(0x00AD5080, c_gui_data, get_player_appearance, element_handle, appearance);

	return false;
}

int32 c_gui_data_array::get_previous_element_handle(int32 element_handle)
{
	return INVOKE_CLASS_MEMBER(0x00AD5090, c_gui_data_array, get_previous_element_handle, element_handle);
}

int32 c_gui_ordered_data::get_previous_element_handle(int32 element_handle)
{
	return INVOKE_CLASS_MEMBER(0x00AD50C0, c_gui_ordered_data, get_previous_element_handle, element_handle);
}

bool c_gui_data::get_qword_value(int32 element_handle, string_id value_name, uns64* value)
{
	//return INVOKE_CLASS_MEMBER(0x00AD5160, c_gui_data, get_qword_value, element_handle, value_name, value);

	return false;
}

bool c_gui_data::get_real_value(int32 element_handle, string_id value_name, real32* value)
{
	//return INVOKE_CLASS_MEMBER(0x00AD5170, c_gui_data, get_real_value, element_handle, value_name, value);

	return false;
}

bool c_gui_data::get_string_id_value(int32 element_handle, string_id value_name, int32* value)
{
	//return INVOKE_CLASS_MEMBER(0x00AD5180, c_gui_data, get_string_id_value, element_handle, value_name, value);

	return false;
}

bool c_gui_data_array_test::get_string_id_value(int32 element_handle, string_id value_name, int32* value)
{
	return INVOKE_CLASS_MEMBER(0x00AD5190, c_gui_data_array_test, get_string_id_value, element_handle, value_name, value);
}

bool c_gui_tag_datasource::get_string_id_value(int32 element_handle, string_id value_name, int32* value)
{
	return INVOKE_CLASS_MEMBER(0x00AD51B0, c_gui_tag_datasource, get_string_id_value, element_handle, value_name, value);
}

bool c_gui_data::get_text_value(int32 element_handle, string_id value_name, c_static_wchar_string<1024>* buffer)
{
	//return INVOKE_CLASS_MEMBER(0x00AD5220, c_gui_data, get_text_value, element_handle, value_name, buffer);

	return false;
}

bool c_gui_data_array_test::get_text_value(int32 element_handle, string_id value_name, c_static_wchar_string<1024>* buffer)
{
	return INVOKE_CLASS_MEMBER(0x00AD5230, c_gui_data_array_test, get_text_value, element_handle, value_name, buffer);
}

bool c_gui_tag_datasource::get_text_value(int32 element_handle, string_id value_name, c_static_wchar_string<1024>* buffer)
{
	return INVOKE_CLASS_MEMBER(0x00AD5280, c_gui_tag_datasource, get_text_value, element_handle, value_name, buffer);
}

string_id c_gui_data::get_name()
{
	return m_name;
}

bool c_gui_data::initialize(string_id name)
{
	//return INVOKE_CLASS_MEMBER(0x00AD52F0, c_gui_data, initialize, name);

	m_name = name;
	return true;
}

bool c_gui_data_array::initialize(string_id name)
{
	return INVOKE_CLASS_MEMBER(0x00AD5300, c_gui_data_array, initialize, name);
}

bool c_gui_data_array_test::initialize(string_id name)
{
	return INVOKE_CLASS_MEMBER(0x00AD5330, c_gui_data_array_test, initialize, name);
}

//.text:00AD5370 ; public: virtual bool c_gui_data_proxy::initialize()

bool c_gui_tag_datasource::initialize(string_id name)
{
	return INVOKE_CLASS_MEMBER(0x00AD5390, c_gui_tag_datasource, initialize, name);
}

bool c_gui_data::is_busy() const
{
	//return INVOKE_CLASS_MEMBER(0x00AD53B0, c_gui_data, is_busy);

	return false;
}

//.text:00AD53C0 ; private: bool c_gui_ordered_data::is_enabled(int32 absolute_index)

void c_gui_data::set_disabled_element(string_id string_id_column_name, string_id string_id_value)
{
	//INVOKE_CLASS_MEMBER(0x00AD5430, c_gui_data, set_disabled_element, string_id_column_name, string_id_value);

	VASSERT(0, "datasource doesn't support enabling and disabling");
}

void c_gui_ordered_data::set_disabled_element(string_id string_id_column_name, string_id string_id_value)
{
	//INVOKE_CLASS_MEMBER(0x00AD5440, c_gui_ordered_data, set_disabled_element, string_id_column_name, string_id_value);

	if (VALID_INDEX(m_disabled_element_count + 1, k_maximum_disabled_elements))
	{
		m_disabled_elements[m_disabled_element_count].name = string_id_column_name;
		m_disabled_elements[m_disabled_element_count].value = string_id_value;
		m_disabled_element_count++;
	}
}

void c_gui_data::set_element(int32 element_handle, const void* element, int32 element_size)
{
	//INVOKE_CLASS_MEMBER(0x00AD5480, c_gui_data, set_element, element_handle, element, element_size);
}

void c_gui_data_array::set_element(int32 element_handle, const void* element, int32 element_size)
{
	INVOKE_CLASS_MEMBER(0x00AD5490, c_gui_data_array, set_element, element_handle, element, element_size);
}

void c_gui_data::set_name(int32 name)
{
	//INVOKE_CLASS_MEMBER(0x00AD54D0, c_gui_data, set_name, name);

	m_name = name;
}

void c_gui_data::update()
{
	//INVOKE_CLASS_MEMBER(0x00AD54E0, c_gui_data, update);
}

