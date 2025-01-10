#pragma once

struct c_font_cache_base;
union point2d;

class c_debug_menu_item;
class c_debug_menu
{
	enum
	{
		// Halo 3
		k_max_items = 64

		// Reach
		//k_max_items = 128
	};

public:
	virtual ~c_debug_menu();
	virtual void update();
	virtual void render(c_font_cache_base* font_cache, point2d const& point);
	virtual void game_render();
	virtual void notify_selected(short selected_value);
	virtual void open();
	virtual char const* get_caption();
	virtual void notify_closed();
	virtual void notify_activated();

protected:
	virtual long get_num_items_to_render();
	virtual void close(bool closed); // closed ? m_parent->notify_selection_closed() : m_parent->notify_selection_exited()
	virtual short get_value_width();
	virtual void notify_selection_closed();
	virtual void notify_selection_exited();
	virtual void notify_up();
	virtual void notify_down();
	virtual void notify_left();
	virtual void notify_right();

	bool is_active_menu();
	short get_menu_rate();
	short get_max_active_captions();

	void render_background(c_font_cache_base* font_cache, point2d const& point);
	void render_title(c_font_cache_base* font_cache, point2d const& point);
	void render_caption(c_font_cache_base* font_cache, point2d const& point);
	void render_global_caption(c_font_cache_base* font_cache, point2d const& point);
	void render_items(c_font_cache_base* font_cache, point2d const& point, short start_index, short end_index);

private:
	void try_left();
	void try_right();

public:
	c_debug_menu(c_debug_menu* parent, char const* name);

	void clear();

	short get_num_items();
	void set_num_items(short num_items);

	short get_selection();
	void set_selection(short selection);

	c_debug_menu_item* get_item(short item_index);
	void add_item(c_debug_menu_item* item);

	char const* get_name();
	void set_name(char const* name);

	void set_caption(char const* caption);

	c_debug_menu* get_parent();

	bool get_enabled();
	void set_enabled(bool enable);

	short get_title_height();
	short get_item_indent();
	short get_item_height();

protected:
	short m_num_items;
	short m_selection;
	long m_up_time;
	long m_down_time;
	long m_left_time;
	long m_right_time;
	c_debug_menu_item* m_items[64];
	char* m_name;
	char* m_caption;
	c_debug_menu* m_parent;
	bool m_enabled;
};

