#include "interface/gui_screens/motd_popup/gui_screen_motd_popup.hpp"

#include "cache/cache_files.hpp"
#include "cseries/cseries_events.hpp"
#include "interface/c_controller.hpp"
#include "interface/gui_custom_bitmap_widget.hpp"
#include "interface/user_interface.hpp"
#include "interface/user_interface_memory.hpp"
#include "interface/user_interface_messages.hpp"
#include "interface/user_interface_window_manager.hpp"
#include "memory/module.hpp"
#include "networking/logic/storage/network_http_buffer_downloader.hpp"

using t_motd_popup_data_downloader = c_http_blf_simple_downloader<s_message_of_the_day_popup, 4665>;
e_download_status __thiscall t_motd_popup_data_downloader::get_data_no_update(const s_message_of_the_day_popup** data, int32* data_size)
{
	static s_message_of_the_day_popup static_data{};

	if (static_data.message_index != 85839908)
	{
		static_data.message_index = 85839908;
		static_data.prevent_dismissal_duration_milliseconds = 5000;

#define MAKE_MOTD_TEXT(TEXT, TEXT_LEN, STRING) \
ustrnzcpy(TEXT, STRING, NUMBEROF(TEXT)); \
TEXT_LEN = sizeof(wchar_t) * ustrnlen(TEXT, NUMBEROF(TEXT));

		MAKE_MOTD_TEXT(static_data.title_text, static_data.title_text_length, L"Halo: Reach E3 Trailer!");
		MAKE_MOTD_TEXT(static_data.header_text, static_data.header_text_length, L"Prepare for Launch.");
		MAKE_MOTD_TEXT(static_data.accept_text, static_data.accept_text_length, L"Lift-off!");
		MAKE_MOTD_TEXT(static_data.wait_text, static_data.wait_text_length, L"T-minus 5...");
		MAKE_MOTD_TEXT(static_data.message_text, static_data.message_text_length, L"Going where no man has gone before is standard operating procedure for Noble Team, but even they weren't prepared for Operation Uppercut. Backs against the wall and facing the full might of the Covenant invasion, Six must rely on his unique skill set and previous involvement in ONI's secret Sabre program to launch a surprise attack against the Covenant fleet in the last place they expect to be met by resistance. And since you weren't on hand to witness this trailer's maiden voyage at E3, Bungie brought back some footage so you can relive the launch sequence on Bungie.net and Marketplace.");

#undef MAKE_MOTD_TEXT
	}

	if (data)
	{
		*data = &static_data;
	}

	if (data_size)
	{
		*data_size = sizeof(static_data);
	}

	return _http_download_status_succeeded;
}
//HOOK_DECLARE_CLASS_MEMBER(0x00AE74E0, t_motd_popup_data_downloader, get_data_no_update);

c_motd_popup_screen_message::c_motd_popup_screen_message(e_controller_index controller, e_window_index window, int32 layered_position, const s_message_of_the_day_popup* message, int32 message_index) :
	c_load_screen_message(STRING_ID(gui, message), controller, window, layered_position),
	m_message(*message),
	m_message_index(message_index)
{
	//DECLFUNC(0x00B12F10, void, __thiscall, c_motd_popup_screen_message*, e_controller_index, e_window_index, int32, const s_message_of_the_day_popup*, int32)(this, controller, window, layered_position, message, message_index);
}

c_motd_popup_screen_widget::c_motd_popup_screen_widget(int32 name) :
	c_gui_screen_widget(name),
	m_allow_dismissal_milliseconds(0x7FFFFFFF),
	m_message(),
	m_image_downloader(),
	m_image_downloader_pad(),
	m_image_url_key(0),
	m_motd_displayed(false),
	m_message_index(0),
	m_fake_definition(),
	m_fake_groups(),
	m_fake_group0_texts(),
	m_fake_group0_bitmaps(),
	m_fake_group1_bitmaps()
{
	//DECLFUNC(0x00B12F50, void, __thiscall, c_motd_popup_screen_widget*, int32)(this, name);

	DECLFUNC(0x00AEC040, void, __thiscall, c_http_stored_buffer_downloader<61440>*)(&m_image_downloader);
}

//.text:00B133F0 ; public: s_group_widget_block::s_group_widget_block()
//.text:00B13450 ; public: s_group_widget_definition::s_group_widget_definition()
//.text:00B134D0 ; public: s_screen_widget_definition::s_screen_widget_definition()
//.text:00B135D0 ; public: virtual c_motd_popup_screen_message::~c_motd_popup_screen_message()
//.text:00B135E0 ; public: virtual c_motd_popup_screen_widget::~c_motd_popup_screen_widget()
//.text:00B13630 ; public: virtual void* c_motd_popup_screen_message::`vector deleting destructor'(unsigned int)
//.text:00B13660 ; public: virtual void* c_motd_popup_screen_widget::`scalar deleting destructor'(unsigned int)

void c_motd_popup_screen_widget::acknowledge_message()
{
	INVOKE_CLASS_MEMBER(0x00B136D0, c_motd_popup_screen_widget, acknowledge_message);
}

void __cdecl c_motd_popup_screen_widget::add_bitmap_widget(s_bitmap_widget_block* bitmap, s_tag_reference template_reference, int32 flags, int32 name, int16 render_depth_bias, int16 l0, int16 t0, int16 r0, int16 b0, int16 l1, int16 t1, int16 r1, int16 b1, int16 blend_mode, s_tag_reference animation_collection_reference, s_tag_reference bitmap_reference)
{
	//INVOKE(0x00B13740, c_motd_popup_screen_widget::add_bitmap_widget, bitmap, template_reference, flags, name, render_depth_bias, l0, t0, r0, b0, l1, t1, r1, b1, blend_mode, animation_collection_reference, bitmap_reference);

	bitmap->override_definition.flags = flags;
	bitmap->override_definition.widget_identifier = name;
	bitmap->override_definition.render_depth_bias = render_depth_bias;
	bitmap->widget_template_reference = template_reference;
	c_motd_popup_screen_widget::set_widget_bounds(&bitmap->override_definition.bounds[0], l0, t0, r0, b0);
	c_motd_popup_screen_widget::set_widget_bounds(&bitmap->override_definition.bounds[1], l1, t1, r1, b1);
	bitmap->override_definition.animation_collection_reference = animation_collection_reference;
	bitmap->override_definition.bitmap_reference = bitmap_reference;
}

void __cdecl c_motd_popup_screen_widget::add_text_widget(s_text_widget_block* text, int32 flags, int32 name, int16 render_depth_bias, int16 l, int16 t, int16 r, int16 b, e_color_preset color_preset, e_font_id font_id)
{
	//INVOKE(0x00B137E0, c_motd_popup_screen_widget::add_text_widget, text, flags, name, render_depth_bias, l, t, r, b, color_preset, font_id);

	text->override_definition.flags = flags;
	text->override_definition.widget_identifier = name;
	text->override_definition.render_depth_bias = render_depth_bias;
	c_motd_popup_screen_widget::set_widget_bounds(&text->override_definition.bounds[0], l, t, r, b);
	text->override_definition.text_color_preset = c_motd_popup_screen_widget::get_color_preset(color_preset);
	text->override_definition.font = font_id;
	text->override_definition.animation_collection_reference = c_motd_popup_screen_widget::get_bitmap_widget_from_screen(GUI_WIDGET_ANIMATION_COLLECTION_DEFINITION_TAG, STRING_ID(gui, start_menu), 0, 0)->override_definition.animation_collection_reference;
}

void c_motd_popup_screen_message::apply_initial_state(c_gui_screen_widget* screen_widget) const
{
	//INVOKE_CLASS_MEMBER(0x00B13890, c_motd_popup_screen_message, apply_initial_state, screen_widget);

	ASSERT(screen_widget);

	c_motd_popup_screen_widget* motd_popup = (c_motd_popup_screen_widget*)screen_widget;
	motd_popup->set_message(&m_message, m_message_index);
}

c_gui_bitmap_widget* c_motd_popup_screen_widget::create_bitmap_widget(const s_runtime_bitmap_widget_definition* definition)
{
	//return INVOKE_CLASS_MEMBER(0x00B138C0, c_motd_popup_screen_widget, create_bitmap_widget, definition);

	if (definition->widget_identifier == STRING_ID(gui, flavor_image))
	{
		return UI_MALLOC(c_gui_custom_bitmap_widget);
	}

	return c_gui_widget::create_bitmap_widget(definition);
}

s_tag_reference __cdecl c_motd_popup_screen_widget::get_bitmap_tag_reference_from_screen(int32 screen_name, int32 group_index, int32 bitmap_block_index)
{
	return INVOKE(0x00B13940, c_motd_popup_screen_widget::get_bitmap_tag_reference_from_screen, screen_name, group_index, bitmap_block_index);

	//s_tag_reference bitmap_reference = c_motd_popup_screen_widget::get_bitmap_widget_from_screen(BITMAP_TAG, screen_name, group_index, bitmap_block_index)->override_definition.bitmap_reference;
	//return bitmap_reference;
}

const s_bitmap_widget_block* __cdecl c_motd_popup_screen_widget::get_bitmap_widget_from_screen(uns32 reference_type, int32 screen_name, int32 group_index, int32 bitmap_block_index)
{
	return INVOKE(0x00B139A0, c_motd_popup_screen_widget::get_bitmap_widget_from_screen, reference_type, screen_name, group_index, bitmap_block_index);

	//int32 screen_definition_index = window_manager_get()->locate_screen_definition_tag_by_name(screen_name);
	//s_screen_widget_definition* screen_widget_definition = TAG_GET(GUI_SCREEN_WIDGET_DEFINITION_TAG, s_screen_widget_definition, screen_definition_index);
	//if (!VALID_INDEX(group_index, screen_widget_definition->group_blocks.count))
	//{
	//	return NULL;
	//}
	//
	//s_group_widget_block* group_widget_block = TAG_BLOCK_GET_ELEMENT(&screen_widget_definition->group_blocks, group_index, s_group_widget_block);
	//if (!VALID_INDEX(bitmap_block_index, group_widget_block->override_definition.bitmap_blocks.count))
	//{
	//	return NULL;
	//}
	//
	//s_bitmap_widget_block* bitmap_widget_block = TAG_BLOCK_GET_ELEMENT(&group_widget_block->override_definition.bitmap_blocks, bitmap_block_index, s_bitmap_widget_block);
	//return bitmap_widget_block;
}

int32 __cdecl c_motd_popup_screen_widget::get_color_preset(e_color_preset color_preset)
{
	return INVOKE(0x00B139F0, c_motd_popup_screen_widget::get_color_preset, color_preset);
}

bool c_motd_popup_screen_widget::handle_controller_input_message(const c_controller_input_message* message)
{
	//return INVOKE_CLASS_MEMBER(0x00B13A20, c_motd_popup_screen_widget, handle_controller_input_message, message);

	if (message->get_event_type() == _event_type_button_press &&
		IN_RANGE_INCLUSIVE(message->get_component(), _controller_component_button_a, _controller_component_button_b) &&
		(int32)user_interface_milliseconds() >= m_allow_dismissal_milliseconds)
	{
		event(_event_message, "ui:motd_popup: closed");
		c_motd_popup_screen_widget::acknowledge_message();
		c_gui_screen_widget::transition_out(_transition_out_normal);
		return true;
	}

	return c_gui_screen_widget::handle_controller_input_message(message);
}

void c_motd_popup_screen_widget::post_initialize()
{
	//INVOKE_CLASS_MEMBER(0x00B13A70, c_motd_popup_screen_widget, post_initialize);

	m_allow_dismissal_milliseconds = user_interface_milliseconds() + m_message.prevent_dismissal_duration_milliseconds;
	c_motd_popup_screen_widget::set_popup_text(STRING_ID(global, title), m_message.title_text, m_message.title_text_length);
	c_motd_popup_screen_widget::set_popup_text(STRING_ID(gui, header), m_message.header_text, m_message.header_text_length);
	c_motd_popup_screen_widget::set_popup_text(STRING_ID(gui, button_key_default), m_message.accept_text, m_message.accept_text_length);
	c_motd_popup_screen_widget::set_popup_text(STRING_ID(gui, button_key_wait), m_message.wait_text, m_message.wait_text_length);
	c_motd_popup_screen_widget::set_popup_text(STRING_ID(gui, message), m_message.message_text, m_message.message_text_length);
}

//.text:00B13C80 ; public: void c_static_wchar_string<1024>::set_bounded(const wchar_t*, int32)

void c_motd_popup_screen_widget::set_message(const s_message_of_the_day_popup* message, int32 message_index)
{
	//INVOKE_CLASS_MEMBER(0x00B13CB0, c_motd_popup_screen_widget, set_message, message, message_index);

	m_message = *message;
	m_message_index = message_index;
}

void c_motd_popup_screen_widget::set_popup_text(int32 widget_name, wchar_t* text, int32 text_length)
{
	INVOKE_CLASS_MEMBER(0x00B13CE0, c_motd_popup_screen_widget, set_popup_text, widget_name, text, text_length);

	//c_gui_text_widget* text_widget = c_gui_widget::get_child_text_widget(widget_name);
	//if (text_widget)
	//{
	//	c_static_wchar_string<1024> bounded_text;
	//	bounded_text.set_bounded(text, text_length / sizeof(wchar_t));
	//	text_widget->set_text(bounded_text.get_string());
	//}
}

void __cdecl c_motd_popup_screen_widget::set_widget_bounds(rectangle2d* bounds, int16 l, int16 t, int16 r, int16 b)
{
	//INVOKE(0x00B13D40, c_motd_popup_screen_widget::set_widget_bounds, bounds, l, t, r, b);

	bounds->x0 = l;
	bounds->y0 = t;
	bounds->x1 = r;
	bounds->y1 = b;
}

void c_motd_popup_screen_widget::setup_screen_in_code()
{
	// $REVIEW confirm this implementation is correct

	m_fake_definition.flags = 0x8;
	m_fake_definition.widget_identifier = STRING_ID(gui, message);
	m_fake_definition.initial_button_key_name = k_string_id_empty_string;

	c_motd_popup_screen_widget::set_widget_bounds(&m_fake_definition.bounds[0], -640, -360, 640, 104);
	c_motd_popup_screen_widget::set_widget_bounds(&m_fake_definition.bounds[1], -449, -267, 348, 0);

	tag_block_set_element_count(&m_fake_definition.group_blocks, NUMBEROF(m_fake_groups));
	tag_block_set_elements(&m_fake_definition.group_blocks, m_fake_groups);

	ASSERT(TAG_BLOCK_GET_ELEMENT(&m_fake_definition.group_blocks, 0, s_group_widget_block) == &m_fake_groups[0]);
	ASSERT(TAG_BLOCK_GET_ELEMENT(&m_fake_definition.group_blocks, 1, s_group_widget_block) == &m_fake_groups[1]);

	s_tag_reference empty_bitmap_widget_reference{};
	empty_bitmap_widget_reference.group_tag = GUI_BITMAP_WIDGET_DEFINITION_TAG;
	empty_bitmap_widget_reference.name = "";
	empty_bitmap_widget_reference.name_length = 0;
	empty_bitmap_widget_reference.index = NONE;

	s_tag_reference empty_bitmap_reference{};
	empty_bitmap_reference.group_tag = BITMAP_TAG;
	empty_bitmap_reference.name = "";
	empty_bitmap_reference.name_length = 0;
	empty_bitmap_reference.index = NONE;

	tag_block_set_element_count(&m_fake_groups[0].override_definition.text_blocks, NUMBEROF(m_fake_group0_texts));
	tag_block_set_elements(&m_fake_groups[0].override_definition.text_blocks, m_fake_group0_texts);
	tag_block_set_element_count(&m_fake_groups[0].override_definition.bitmap_blocks, NUMBEROF(m_fake_group0_bitmaps));
	tag_block_set_elements(&m_fake_groups[0].override_definition.bitmap_blocks, m_fake_group0_bitmaps);

	{
		c_motd_popup_screen_widget::add_text_widget(&m_fake_group0_texts[0],
			0x48,
			STRING_ID(global, title),
			20,
			249, 98, 1060, 200,
			_color_preset_hilite,
			_title_font);

		c_motd_popup_screen_widget::add_text_widget(&m_fake_group0_texts[1],
			0x48,
			STRING_ID(gui, header),
			20,
			529, 165, 1015, 228,
			_color_preset_hilite,
			_large_body_text_font);

		c_motd_popup_screen_widget::add_text_widget(&m_fake_group0_texts[2],
			0x10,
			STRING_ID(gui, button_key_default),
			20,
			529, 613, 1025, 645,
			_color_preset_hilite,
			_body_text_font);

		c_motd_popup_screen_widget::add_text_widget(&m_fake_group0_texts[3],
			0x10,
			STRING_ID(gui, button_key_wait),
			20,
			529, 613, 1025, 645,
			_color_preset_hilite,
			_body_text_font);

		c_motd_popup_screen_widget::add_text_widget(&m_fake_group0_texts[4],
			0x4008,
			STRING_ID(gui, message),
			20,
			529, 210, 1015, 575,
			_color_preset_ice,
			_body_text_font);


		c_motd_popup_screen_widget::add_bitmap_widget(&m_fake_group0_bitmaps[0],
			empty_bitmap_widget_reference,
			0x8,
			k_string_id_empty_string,
			-5,
			203, 140, 1220, 643,
			96, 103, 1220, 470,
			3,
			c_motd_popup_screen_widget::get_bitmap_widget_from_screen(
				GUI_WIDGET_ANIMATION_COLLECTION_DEFINITION_TAG,
				STRING_ID(gui, start_menu),
				0, 0)->override_definition.animation_collection_reference,
			c_motd_popup_screen_widget::get_bitmap_tag_reference_from_screen(
				STRING_ID(gui, start_menu),
				0, 0));

		c_motd_popup_screen_widget::add_bitmap_widget(&m_fake_group0_bitmaps[1],
			c_motd_popup_screen_widget::get_bitmap_widget_from_screen(
				BITMAP_TAG,
				STRING_ID(gui, start_menu_settings),
				2, 0)->widget_template_reference,
			0x0,
			STRING_ID(gui, flavor_image),
			2,
			247, 140, 554, 605,
			173, 102, 480, 567,
			0,
			c_motd_popup_screen_widget::get_bitmap_widget_from_screen(
				GUI_WIDGET_ANIMATION_COLLECTION_DEFINITION_TAG,
				STRING_ID(gui, start_menu),
				0, 0)->override_definition.animation_collection_reference,
			empty_bitmap_reference);

		c_motd_popup_screen_widget::add_bitmap_widget(&m_fake_group0_bitmaps[2],
			empty_bitmap_widget_reference,
			0x8,
			k_string_id_empty_string,
			-22,
			-180, -64, 1250, 1000,
			0, 0, 0, 0,
			3,
			c_motd_popup_screen_widget::get_bitmap_widget_from_screen(
				GUI_WIDGET_ANIMATION_COLLECTION_DEFINITION_TAG,
				STRING_ID(gui, start_menu),
				0, 2)->override_definition.animation_collection_reference,
			c_motd_popup_screen_widget::get_bitmap_tag_reference_from_screen(
				STRING_ID(gui, start_menu),
				0, 2));

		c_motd_popup_screen_widget::add_bitmap_widget(&m_fake_group0_bitmaps[3],
			empty_bitmap_widget_reference,
			0x18,
			k_string_id_empty_string,
			-23,
			-180, -64, 1250, 1000,
			0, 0, 0, 0,
			3,
			c_motd_popup_screen_widget::get_bitmap_widget_from_screen(
				GUI_WIDGET_ANIMATION_COLLECTION_DEFINITION_TAG,
				STRING_ID(gui, start_menu),
				0, 2)->override_definition.animation_collection_reference,
			c_motd_popup_screen_widget::get_bitmap_tag_reference_from_screen(
				STRING_ID(gui, start_menu),
				0, 2));

		c_motd_popup_screen_widget::add_bitmap_widget(&m_fake_group0_bitmaps[4],
			c_motd_popup_screen_widget::get_bitmap_widget_from_screen(
				BITMAP_TAG,
				STRING_ID(gui, start_menu_settings),
				2, 0)->widget_template_reference,
			0x8,
			k_string_id_empty_string,
			0,
			247, 140, 554, 642,
			0, 0, 0, 0,
			3,
			c_motd_popup_screen_widget::get_bitmap_widget_from_screen(
				GUI_WIDGET_ANIMATION_COLLECTION_DEFINITION_TAG,
				STRING_ID(gui, start_menu),
				0, 0)->override_definition.animation_collection_reference,
			c_motd_popup_screen_widget::get_bitmap_tag_reference_from_screen(
				STRING_ID(gui, start_menu_settings),
				1, 4));

		c_motd_popup_screen_widget::add_bitmap_widget(&m_fake_group0_bitmaps[5],
			empty_bitmap_widget_reference,
			0x18,
			k_string_id_empty_string,
			5,
			511, 160, 1036, 586,
			0, 0, 0, 0,
			3,
			c_motd_popup_screen_widget::get_bitmap_widget_from_screen(
				GUI_WIDGET_ANIMATION_COLLECTION_DEFINITION_TAG,
				STRING_ID(gui, start_menu),
				0, 0)->override_definition.animation_collection_reference,
			c_motd_popup_screen_widget::get_bitmap_tag_reference_from_screen(
				STRING_ID(gui, start_menu),
				0, 2));

		c_motd_popup_screen_widget::add_bitmap_widget(&m_fake_group0_bitmaps[6],
			c_motd_popup_screen_widget::get_bitmap_widget_from_screen(
				BITMAP_TAG,
				STRING_ID(gui, start_menu_settings),
				2, 0)->widget_template_reference,
			0x8,
			k_string_id_empty_string,
			0,
			554, 140, 1036, 605,
			0, 0, 0, 0,
			3,
			c_motd_popup_screen_widget::get_bitmap_widget_from_screen(
				GUI_WIDGET_ANIMATION_COLLECTION_DEFINITION_TAG,
				STRING_ID(gui, start_menu),
				0, 0)->override_definition.animation_collection_reference,
			c_motd_popup_screen_widget::get_bitmap_tag_reference_from_screen(
				STRING_ID(gui, start_menu_settings_appearance_colors),
				7, 1));

		c_motd_popup_screen_widget::add_bitmap_widget(&m_fake_group0_bitmaps[7],
			empty_bitmap_widget_reference,
			0x8,
			k_string_id_empty_string,
			7,
			511, 160, 1300, 586,
			0, 0, 0, 0,
			3,
			c_motd_popup_screen_widget::get_bitmap_widget_from_screen(
				GUI_WIDGET_ANIMATION_COLLECTION_DEFINITION_TAG,
				STRING_ID(gui, start_menu),
				0, 0)->override_definition.animation_collection_reference,
			c_motd_popup_screen_widget::get_bitmap_tag_reference_from_screen(
				STRING_ID(gui, game_options),
				0, 5));

		c_motd_popup_screen_widget::add_bitmap_widget(&m_fake_group0_bitmaps[8],
			empty_bitmap_widget_reference,
			0x8,
			k_string_id_empty_string,
			8,
			203, 86, 1220, 140,
			150, 70, 1220, 103,
			3,
			c_motd_popup_screen_widget::get_bitmap_widget_from_screen(
				GUI_WIDGET_ANIMATION_COLLECTION_DEFINITION_TAG,
				STRING_ID(gui, start_menu),
				0, 0)->override_definition.animation_collection_reference,
			c_motd_popup_screen_widget::get_bitmap_tag_reference_from_screen(
				STRING_ID(gui, start_menu),
				0, 1));

		c_motd_popup_screen_widget::add_bitmap_widget(&m_fake_group0_bitmaps[9],
			empty_bitmap_widget_reference,
			0x8,
			k_string_id_empty_string,
			8,
			511, 160, 1300, 161,
			0, 0, 0, 0,
			3,
			c_motd_popup_screen_widget::get_bitmap_widget_from_screen(
				GUI_WIDGET_ANIMATION_COLLECTION_DEFINITION_TAG,
				STRING_ID(gui, start_menu),
				0, 0)->override_definition.animation_collection_reference,
			c_motd_popup_screen_widget::get_bitmap_tag_reference_from_screen(
				STRING_ID(gui, pregame_switch_lobby),
				0, 0));

		c_motd_popup_screen_widget::add_bitmap_widget(&m_fake_group0_bitmaps[10],
			empty_bitmap_widget_reference,
			0x8,
			k_string_id_empty_string,
			8,
			511, 197, 1300, 198,
			0, 0, 0, 0,
			3,
			c_motd_popup_screen_widget::get_bitmap_widget_from_screen(
				GUI_WIDGET_ANIMATION_COLLECTION_DEFINITION_TAG,
				STRING_ID(gui, start_menu),
				0, 0)->override_definition.animation_collection_reference,
			c_motd_popup_screen_widget::get_bitmap_tag_reference_from_screen(
				STRING_ID(gui, pregame_switch_lobby),
				0, 0));

		c_motd_popup_screen_widget::add_bitmap_widget(&m_fake_group0_bitmaps[11],
			empty_bitmap_widget_reference,
			0x8,
			k_string_id_empty_string,
			8,
			511, 161, 1300, 197,
			0, 0, 0, 0,
			3,
			c_motd_popup_screen_widget::get_bitmap_widget_from_screen(
				GUI_WIDGET_ANIMATION_COLLECTION_DEFINITION_TAG,
				STRING_ID(gui, start_menu),
				0, 0)->override_definition.animation_collection_reference,
			c_motd_popup_screen_widget::get_bitmap_tag_reference_from_screen(
				STRING_ID(gui, start_menu_settings),
				1, 1));
	}

	m_fake_groups[1].override_definition.widget_identifier = STRING_ID(gui, in_progress);
	tag_block_set_element_count(&m_fake_groups[1].override_definition.bitmap_blocks, NUMBEROF(m_fake_group1_bitmaps));
	tag_block_set_elements(&m_fake_groups[1].override_definition.bitmap_blocks, m_fake_group1_bitmaps);

	{
		c_motd_popup_screen_widget::add_bitmap_widget(&m_fake_group1_bitmaps[0],
			empty_bitmap_widget_reference,
			0x8,
			k_string_id_empty_string,
			0,
			247, 140, 554, 605,
			0, 0, 0, 0,
			3,
			c_motd_popup_screen_widget::get_bitmap_widget_from_screen(
				GUI_WIDGET_ANIMATION_COLLECTION_DEFINITION_TAG,
				STRING_ID(gui, start_menu),
				0, 0)->override_definition.animation_collection_reference,
			c_motd_popup_screen_widget::get_bitmap_tag_reference_from_screen(
				STRING_ID(gui, start_menu_hq_service_record),
				0, 5));

		c_motd_popup_screen_widget::add_bitmap_widget(&m_fake_group1_bitmaps[1],
			empty_bitmap_widget_reference,
			0x8,
			k_string_id_empty_string,
			1,
			306, 250, 488, 432,
			0, 0, 0, 0,
			3,
			c_motd_popup_screen_widget::get_bitmap_widget_from_screen(
				GUI_WIDGET_ANIMATION_COLLECTION_DEFINITION_TAG,
				STRING_ID(gui, start_menu_hq),
				9, 1)->override_definition.animation_collection_reference,
			c_motd_popup_screen_widget::get_bitmap_tag_reference_from_screen(
				STRING_ID(gui, in_progress),
				0, 0));

		c_motd_popup_screen_widget::add_bitmap_widget(&m_fake_group1_bitmaps[2],
			empty_bitmap_widget_reference,
			0x8,
			k_string_id_empty_string,
			1,
			340, 284, 456, 400,
			0, 0, 0, 0,
			3,
			c_motd_popup_screen_widget::get_bitmap_widget_from_screen(
				GUI_WIDGET_ANIMATION_COLLECTION_DEFINITION_TAG,
				STRING_ID(gui, start_menu_hq),
				9, 2)->override_definition.animation_collection_reference,
			c_motd_popup_screen_widget::get_bitmap_tag_reference_from_screen(
				STRING_ID(gui, in_progress),
				0, 1));

		c_motd_popup_screen_widget::add_bitmap_widget(&m_fake_group1_bitmaps[3],
			empty_bitmap_widget_reference,
			0x8,
			k_string_id_empty_string,
			1,
			360, 304, 436, 380,
			0, 0, 0, 0,
			3,
			c_motd_popup_screen_widget::get_bitmap_widget_from_screen(
				GUI_WIDGET_ANIMATION_COLLECTION_DEFINITION_TAG,
				STRING_ID(gui, start_menu_hq),
				9, 3)->override_definition.animation_collection_reference,
			c_motd_popup_screen_widget::get_bitmap_tag_reference_from_screen(
				STRING_ID(gui, in_progress),
				0, 2));
	}

	c_gui_screen_widget::add_definition_fields(&m_fake_definition, false);
}

void c_motd_popup_screen_widget::update(uns32 current_milliseconds)
{
	//INVOKE_CLASS_MEMBER(0x00B13D70, c_motd_popup_screen_widget, update, current_milliseconds);

	c_gui_screen_widget::update(current_milliseconds);

	c_gui_custom_bitmap_widget* flavor_image_bitmap = (c_gui_custom_bitmap_widget*)c_gui_widget::get_child_bitmap_widget(STRING_ID(gui, flavor_image));
	if (flavor_image_bitmap)
	{
		int32 image_size = 0;
		const char* image_buffer = NULL;

		if (m_image_url_key != g_online_url_hopper_directory_key)
		{
			m_image_url_key = g_online_url_hopper_directory_key;
			c_url_string image_url{};
			if (m_message_index)
			{
				online_url_make_vidmaster_popup_image(&image_url);
			}
			else
			{
				online_url_make_message_of_the_day_popup_image(&image_url);
			}
			m_image_downloader.set_url(&image_url);
		}

		if (!m_motd_displayed)
		{
			e_download_status download_status = m_image_downloader.get_data(&image_buffer, &image_size);
			if (image_buffer &&
				download_status == _http_download_status_succeeded &&
				!m_image_downloader.is_submitting_to_cache())
			{
				flavor_image_bitmap->load_from_buffer(true, image_buffer, image_size, _custom_bitmap_desired_aspect_ratio_stretch_to_fit);
				flavor_image_bitmap->start_animation((e_animation_state)(download_status + _child_submenu_ambient_focused), true);
				m_motd_displayed = true;
			}

			c_gui_widget::set_child_visible(_gui_group, STRING_ID(gui, in_progress), download_status == _http_download_status_pending);
			c_gui_widget::set_child_visible(_gui_bitmap, STRING_ID(global, none), download_status == _http_download_status_failed || download_status == _http_download_status_failed_file_not_found);
		}
	}

	bool button_key_visible = (int32)user_interface_milliseconds() >= m_allow_dismissal_milliseconds;
	c_gui_widget::set_child_visible(_gui_text, STRING_ID(gui, button_key_default), button_key_visible);
	c_gui_widget::set_child_visible(_gui_text, STRING_ID(gui, button_key_wait), !button_key_visible);
}

