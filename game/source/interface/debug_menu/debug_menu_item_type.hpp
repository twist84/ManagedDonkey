#pragma once

#include "interface/debug_menu/debug_menu_item_numbered.hpp"
#include "interface/debug_menu/debug_menu_value_hs_global_external.hpp"

class c_debug_menu_item_type :
	public c_debug_menu_item_numbered
{
public:
	virtual ~c_debug_menu_item_type() = default;
	virtual void render(c_font_cache_base* font_cache, const point2d& position) override;
	virtual void to_string(char* buffer, int32 buffer_size);
	virtual void render_value(c_font_cache_base* font_cache, const point2d& position);

	c_debug_menu_item_type(c_debug_menu* menu, const char* name, bool readonly);

	bool get_readonly();

protected:
	bool m_readonly;
};

class c_debug_menu_item_type_bool :
	public c_debug_menu_item_type
{
public:
	virtual ~c_debug_menu_item_type_bool() = default;
	virtual void notify_left() override;
	virtual void notify_right() override;

protected:
	virtual void to_string(char* buffer, int32 buffer_size) override;

public:
	c_debug_menu_item_type_bool(c_debug_menu* menu, const char* name, bool readonly, const char* variable);

protected:
	c_debug_menu_value_hs_global_external<bool> m_value;
};

class c_debug_menu_item_type_real :
	public c_debug_menu_item_type
{
public:
	virtual ~c_debug_menu_item_type_real() = default;
	virtual void notify_left() override;
	virtual void notify_right() override;

protected:
	virtual void to_string(char* buffer, int32 buffer_size) override;

public:
	c_debug_menu_item_type_real(c_debug_menu* menu, const char* name, bool readonly, const char* variable, real32 min, real32 max, real32 inc);

protected:
	c_debug_menu_value_hs_global_external<real32> m_value;
	real32 m_min;
	real32 m_max;
	real32 m_inc;
};

class c_debug_menu_item_type_short :
	public c_debug_menu_item_type
{
public:
	virtual ~c_debug_menu_item_type_short() = default;
	virtual void notify_left() override;
	virtual void notify_right() override;

protected:
	virtual void to_string(char* buffer, int32 buffer_size) override;

public:
	c_debug_menu_item_type_short(c_debug_menu* menu, const char* name, bool readonly, const char* variable, int16 min, int16 max, int16 inc);

protected:
	c_debug_menu_value_hs_global_external<int16> m_value;
	int16 m_min;
	int16 m_max;
	int16 m_inc;
};

class c_debug_menu_item_type_long :
	public c_debug_menu_item_type
{
public:
	virtual ~c_debug_menu_item_type_long() = default;
	virtual void notify_left() override;
	virtual void notify_right() override;

protected:
	virtual void to_string(char* buffer, int32 buffer_size) override;

public:
	c_debug_menu_item_type_long(c_debug_menu* menu, const char* name, bool readonly, const char* variable, int32 min, int32 max, int32 inc);

protected:
	c_debug_menu_value_hs_global_external<int32> m_value;
	int32 m_min;
	int32 m_max;
	int32 m_inc;
};

