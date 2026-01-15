#pragma once

#include "interface/user_interface_data.hpp"

enum e_skull_state
{
	_skull_locked = 0,
	_skull_enabled,
	_skull_disabled,

	k_skull_state_count,
};

class c_gui_primary_skulls_data :
	public c_gui_ordered_data
{
public:
	virtual ~c_gui_primary_skulls_data() = default;
	virtual void get_column_names(string_id* const column_names, int32* column_count) override;

protected:
	virtual int32 get_current_item_count_internal() override;

public:
	virtual bool get_integer_value(int32 element_handle, string_id value_name, int32* value) override;

public:
	virtual bool get_string_id_value(int32 element_handle, string_id value_name, int32* value) override;
	virtual void update() override;

public:
	c_gui_primary_skulls_data();

public:
	e_skull_state m_skull_state[k_campaign_skull_primary_count];
};
static_assert(sizeof(c_gui_primary_skulls_data) == sizeof(c_gui_ordered_data) + 0x24);

class c_gui_secondary_skulls_data :
	public c_gui_ordered_data
{
public:
	virtual ~c_gui_secondary_skulls_data() = default;
	virtual void get_column_names(string_id* const column_names, int32* column_count) override;

protected:
	virtual int32 get_current_item_count_internal() override;

public:
	virtual bool get_integer_value(int32 element_handle, string_id value_name, int32* value) override;

public:
	virtual bool get_string_id_value(int32 element_handle, string_id value_name, int32* value) override;
	virtual void update() override;


public:
	c_gui_secondary_skulls_data();

public:
	e_skull_state m_skull_state[k_campaign_skull_secondary_count];
};
static_assert(sizeof(c_gui_secondary_skulls_data) == sizeof(c_gui_ordered_data) + 0x1C);


