#pragma once

#include "cseries/cseries.hpp"
#include "interface/debug_menu/debug_menu_item.hpp"
#include "interface/debug_menu/debug_menu_value_hs_global_external.hpp"

struct c_font_cache_base;
class c_debug_menu_item_numbered :
	public c_debug_menu_item
{
public:
	virtual ~c_debug_menu_item_numbered();
	virtual void render(c_font_cache_base* font_cache, int16_point2d const& point) override;

	c_debug_menu_item_numbered(c_debug_menu* menu, char const* name, c_debug_menu* child);

protected:
	void render_number(c_font_cache_base* font_cache, int16_point2d const& point);

	short get_indent();
};

struct c_debug_menu_item_type :
	public c_debug_menu_item_numbered
{
public:
	virtual ~c_debug_menu_item_type();
	virtual void render(c_font_cache_base* font_cache, int16_point2d const& point) override;
	virtual void to_string(char* buffer, long buffer_size);
	virtual void render_value(c_font_cache_base* font_cache, int16_point2d const& point);

	c_debug_menu_item_type(c_debug_menu* menu, char const* name, bool readonly);

	bool get_readonly();

protected:
	bool m_readonly;
};

class c_debug_menu_item_type_bool :
	public c_debug_menu_item_type
{
public:
	virtual ~c_debug_menu_item_type_bool();
	virtual void notify_left() override;
	virtual void notify_right() override;

protected:
	virtual void to_string(char* buffer, long buffer_size) override;

public:
	c_debug_menu_item_type_bool(c_debug_menu* menu, char const* name, bool readonly, char const* hs_global_name);

protected:
	c_debug_menu_value_hs_global_external<bool> m_value;
};

class c_debug_menu_item_type_real :
	public c_debug_menu_item_type
{
public:
	virtual ~c_debug_menu_item_type_real();
	virtual void notify_left() override;
	virtual void notify_right() override;

protected:
	virtual void to_string(char* buffer, long buffer_size) override;

public:
	c_debug_menu_item_type_real(c_debug_menu* menu, char const* name, bool readonly, char const* hs_global_name, real min_value, real max_value, real inc_value);

protected:
	c_debug_menu_value_hs_global_external<real> m_value;
	real m_min_value;
	real m_max_value;
	real m_inc_value;
};

class c_debug_menu_item_type_short :
	public c_debug_menu_item_type
{
public:
	virtual ~c_debug_menu_item_type_short();
	virtual void notify_left() override;
	virtual void notify_right() override;

protected:
	virtual void to_string(char* buffer, long buffer_size) override;

public:
	c_debug_menu_item_type_short(c_debug_menu* menu, char const* name, bool readonly, char const* hs_global_name, short min_value, short max_value, short inc_value);

protected:
	c_debug_menu_value_hs_global_external<short> m_value;
	short m_min_value;
	short m_max_value;
	short m_inc_value;
};

class c_debug_menu_item_type_long :
	public c_debug_menu_item_type
{
public:
	virtual ~c_debug_menu_item_type_long();
	virtual void notify_left() override;
	virtual void notify_right() override;

protected:
	virtual void to_string(char* buffer, long buffer_size) override;

public:
	c_debug_menu_item_type_long(c_debug_menu* menu, char const* name, bool readonly, char const* hs_global_name, long min_value, long max_value, long inc_value);

protected:
	c_debug_menu_value_hs_global_external<long> m_value;
	long m_min_value;
	long m_max_value;
	long m_inc_value;
};

