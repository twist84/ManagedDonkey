#pragma once

class c_font_cache_base;
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
	virtual void render(c_font_cache_base* font_cache, const point2d& point);
	virtual void game_render();
	virtual void notify_selected(int16 selected_value);
	virtual void open();
	virtual const char* get_caption();
	virtual void notify_closed();
	virtual void notify_activated();

protected:
	virtual int32 get_num_items_to_render();
	virtual void close(bool closed); // closed ? m_parent->notify_selection_closed() : m_parent->notify_selection_exited()
	virtual int16 get_value_width();
	virtual void notify_selection_closed();
	virtual void notify_selection_exited();
	virtual void notify_up();
	virtual void notify_down();
	virtual void notify_left();
	virtual void notify_right();

	bool is_active_menu();
	int16 get_menu_rate();
	int16 get_max_active_captions();

	void render_background(c_font_cache_base* font_cache, const point2d& point);
	void render_title(c_font_cache_base* font_cache, const point2d& point);
	void render_caption(c_font_cache_base* font_cache, const point2d& point);
	void render_global_caption(c_font_cache_base* font_cache, const point2d& point);
	void render_items(c_font_cache_base* font_cache, const point2d& point, int16 start_index, int16 end_index);

private:
	void try_left();
	void try_right();

public:
	c_debug_menu(c_debug_menu* parent, const char* name);

	void clear();

	int16 get_num_items();
	void set_num_items(int16 num_items);

	int16 get_selection();
	void set_selection(int16 selection);

	c_debug_menu_item* get_item(int16 item_index);
	void add_item(c_debug_menu_item* item);

	const char* get_name();
	void set_name(const char* name);

	void set_caption(const char* caption);

	c_debug_menu* get_parent();

	bool get_enabled();
	void set_enabled(bool enable);

	int16 get_title_height();
	int16 get_item_indent();
	int16 get_item_height();

protected:
	int16 m_num_items;
	int16 m_selection;
	int32 m_last_up;
	int32 m_last_down;
	int32 m_last_left;
	int32 m_last_right;
	c_debug_menu_item* m_items[k_max_items];
	char* m_name;
	char* m_caption;
	c_debug_menu* m_parent_ref;
	bool m_enabled;
};

