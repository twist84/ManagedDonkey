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
	virtual void render(c_font_cache_base* font_cache, point2d const& position) override;

	c_debug_menu_item_numbered(c_debug_menu* menu, char const* name, c_debug_menu* child);

protected:
	void render_number(c_font_cache_base* font_cache, point2d const& position);

	short get_indent();
};

struct c_debug_menu_item_type :
	public c_debug_menu_item_numbered
{
public:
	virtual ~c_debug_menu_item_type();
	virtual void render(c_font_cache_base* font_cache, point2d const& position) override;
	virtual void to_string(char* buffer, long buffer_size);
	virtual void render_value(c_font_cache_base* font_cache, point2d const& position);

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
	c_debug_menu_item_type_bool(c_debug_menu* menu, char const* name, bool readonly, char const* variable);

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
	c_debug_menu_item_type_real(c_debug_menu* menu, char const* name, bool readonly, char const* variable, real min, real max, real inc);

protected:
	c_debug_menu_value_hs_global_external<real> m_value;
	real m_min;
	real m_max;
	real m_inc;
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
	c_debug_menu_item_type_short(c_debug_menu* menu, char const* name, bool readonly, char const* variable, short min, short max, short inc);

protected:
	c_debug_menu_value_hs_global_external<short> m_value;
	short m_min;
	short m_max;
	short m_inc;
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
	c_debug_menu_item_type_long(c_debug_menu* menu, char const* name, bool readonly, char const* variable, long min, long max, long inc);

protected:
	c_debug_menu_value_hs_global_external<long> m_value;
	long m_min;
	long m_max;
	long m_inc;
};

