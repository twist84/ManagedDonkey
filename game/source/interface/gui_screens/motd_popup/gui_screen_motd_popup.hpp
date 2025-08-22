#pragma once

#include "cseries/cseries.hpp"
#include "interface/c_gui_bitmap_widget.hpp"
#include "interface/c_gui_group_widget.hpp"
#include "interface/c_gui_screen_widget.hpp"
#include "interface/c_gui_text_widget.hpp"
#include "interface/user_interface_messages.hpp"
#include "networking/logic/storage/network_http_buffer_downloader.hpp"
#include "text/unicode.hpp"

struct s_message_of_the_day_popup
{
	static int32 const k_chunk_type = 'mtdp';
	static int32 const k_chunk_major_version = 4;
	//static int32 const k_chunk_minor_version = 0;

	// c_player_profile_interface::popup_message_titles[title_index] = message_index;
	int32 message_index;
	int32 prevent_dismissal_duration_milliseconds;

	int32 title_text_length;
	wchar_t title_text[48];

	int32 header_text_length;
	wchar_t header_text[48];

	int32 accept_text_length;
	wchar_t accept_text[48];

	int32 wait_text_length;
	wchar_t wait_text[48];

	int32 message_text_length;
	wchar_t message_text[1024];
};
static_assert(sizeof(s_message_of_the_day_popup) == 0x99C);

class c_motd_popup_screen_message :
	public c_load_screen_message
{
public:
	virtual ~c_motd_popup_screen_message() = default;
	virtual void apply_initial_state(c_gui_screen_widget* screen_widget) const override;

public:
	c_motd_popup_screen_message(e_controller_index controller, e_window_index window, int32 layered_position, const s_message_of_the_day_popup* message, int32 message_index);

public:
	s_message_of_the_day_popup m_message;
	int32 m_message_index;
};
static_assert(sizeof(c_motd_popup_screen_message) == 0x9DC);
static_assert(sizeof(c_motd_popup_screen_message) == sizeof(c_load_screen_message) + 0x9A0);

class c_motd_popup_screen_widget :
	public c_gui_screen_widget
{
public:
	enum
	{
		k_maximum_motd_jpeg_size = 0xF000,
	};

	enum e_color_preset
	{
		_color_preset_white = 0,
		_color_preset_transparent,
		_color_preset_ice,
		_color_preset_hilite,
		_color_preset_unhilite,
		_color_preset_dim,
		_color_preset_semi_dim,
		_color_preset_orange,
		_color_preset_red,
	};

public:
	virtual ~c_motd_popup_screen_widget() = default;
	virtual void post_initialize() override;
	virtual c_gui_bitmap_widget* create_bitmap_widget(const s_runtime_bitmap_widget_definition* definition) override;
	virtual void update(uns32 current_milliseconds) override;
	virtual bool handle_controller_input_message(const c_controller_input_message* message) override;

public:
	c_motd_popup_screen_widget(int32 name);

private:
	void acknowledge_message();
	static void __cdecl add_bitmap_widget(s_bitmap_widget_block* bitmap, s_tag_reference template_reference, int32 flags, int32 name, int16 render_depth_bias, int16 l0, int16 t0, int16 r0, int16 b0, int16 l1, int16 t1, int16 r1, int16 b1, int16 blend_mode, s_tag_reference animation_collection_reference, s_tag_reference bitmap_reference);
	static void __cdecl add_text_widget(s_text_widget_block* text, int32 flags, int32 name, int16 render_depth_bias, int16 l, int16 t, int16 r, int16 b, e_color_preset color_preset, e_font_id font_id);
	static s_tag_reference __cdecl get_bitmap_tag_reference_from_screen(int32 screen_name, int32 group_index, int32 bitmap_block_index);
	static const s_bitmap_widget_block* __cdecl get_bitmap_widget_from_screen(uns32 reference_type, int32 screen_name, int32 group_index, int32 bitmap_block_index);
	static int32 __cdecl get_color_preset(e_color_preset color_preset);

public:
	void set_message(const s_message_of_the_day_popup* message, int32 message_index);

private:
	void set_popup_text(int32 widget_name, wchar_t* text, int32 text_length);
	static void __cdecl set_widget_bounds(rectangle2d* bounds, int16 l, int16 t, int16 r, int16 b);

public:
	void setup_screen_in_code();

//private:
	int32 m_allow_dismissal_milliseconds;
	s_message_of_the_day_popup m_message;
	c_http_stored_buffer_downloader<61440> m_image_downloader;
	int32 m_image_downloader_pad;

	int32 m_image_url_key;
	bool m_motd_displayed;
	int32 m_message_index;
	s_screen_widget_definition m_fake_definition;
	s_group_widget_block m_fake_groups[2];
	s_text_widget_block m_fake_group0_texts[5];
	s_bitmap_widget_block m_fake_group0_bitmaps[12];
	s_bitmap_widget_block m_fake_group1_bitmaps[4];
};
static_assert(sizeof(c_motd_popup_screen_widget) == 0x124A0);
static_assert(sizeof(c_motd_popup_screen_widget) == sizeof(c_gui_screen_widget) + 0x10A00);
static_assert(0x01AA0 == OFFSETOF(c_motd_popup_screen_widget, m_allow_dismissal_milliseconds));
static_assert(0x01AA4 == OFFSETOF(c_motd_popup_screen_widget, m_message));
static_assert(0x02440 == OFFSETOF(c_motd_popup_screen_widget, m_image_downloader));
static_assert(0x11AD8 == OFFSETOF(c_motd_popup_screen_widget, m_image_url_key));
static_assert(0x11ADC == OFFSETOF(c_motd_popup_screen_widget, m_motd_displayed));
static_assert(0x11AE0 == OFFSETOF(c_motd_popup_screen_widget, m_message_index));
static_assert(0x11AE4 == OFFSETOF(c_motd_popup_screen_widget, m_fake_definition));
static_assert(0x11B8C == OFFSETOF(c_motd_popup_screen_widget, m_fake_groups));
static_assert(0x11C64 == OFFSETOF(c_motd_popup_screen_widget, m_fake_group0_texts));
static_assert(0x11DE0 == OFFSETOF(c_motd_popup_screen_widget, m_fake_group0_bitmaps));
static_assert(0x122F0 == OFFSETOF(c_motd_popup_screen_widget, m_fake_group1_bitmaps));

