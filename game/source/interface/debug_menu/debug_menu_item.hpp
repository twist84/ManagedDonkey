#pragma once

class c_font_cache_base;
union point2d;
union real_argb_color;

class c_debug_menu;
class c_debug_menu_item
{
public:
	virtual ~c_debug_menu_item();
	virtual void update();
	virtual void render(c_font_cache_base* font_cache, const point2d& position);
	virtual void notify_selected();
	virtual void open();
	virtual void notify_left();
	virtual void notify_right();
	virtual const real_argb_color* get_enabled_color();
	virtual const real_argb_color* get_background_color();

	c_debug_menu_item(c_debug_menu* menu, const char* name, c_debug_menu* child, bool active);
	
	const char* get_name();
	void set_name(const char* name);

	c_debug_menu* get_menu();
	c_debug_menu* get_child();

	bool get_active();
	void set_active(bool active);

	bool get_data();
	void set_data(int32 data);

	int16 get_index();
	bool get_is_selection();

protected:
	char* m_name;
	c_debug_menu* m_menu_ref;
	c_debug_menu* m_child_ref;
	bool m_active;
	//bool m_numbered;
	int32 m_data;
};

