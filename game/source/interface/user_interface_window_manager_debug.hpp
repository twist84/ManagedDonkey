#pragma once

extern void gui_debug_bitmap_animation_state(int32 name, bool activate);
extern void gui_debug_bitmap_bounds(int32 name, bool activate);
extern void gui_debug_bitmap_display_name(int32 name, bool activate);
extern void gui_debug_bitmap_rotation(int32 name, bool activate);
extern void gui_debug_group_animation_state(int32 name, bool activate, bool include_children);
extern void gui_debug_group_bounds(int32 name, bool activate, bool include_children);
extern void gui_debug_group_display_name(int32 name, bool activate, bool include_children);
extern void gui_debug_group_rotation(int32 name, bool activate, bool include_children);
extern void gui_debug_list_animation_state(int32 name, bool activate, bool include_children);
extern void gui_debug_list_bounds(int32 name, bool activate, bool include_children);
extern void gui_debug_list_display_name(int32 name, bool activate, bool include_children);
extern void gui_debug_list_item_animation_state(int32 name, bool activate, bool include_children);
extern void gui_debug_list_item_bounds(int32 name, bool activate, bool include_children);
extern void gui_debug_list_item_display_name(int32 name, bool activate, bool include_children);
extern void gui_debug_list_item_rotation(int32 name, bool activate, bool include_children);
extern void gui_debug_list_rotation(int32 name, bool activate, bool include_children);
extern void gui_debug_screen_animation_state(int32 name, bool activate, bool include_children);
extern void gui_debug_screen_bounds(int32 name, bool activate, bool include_children);
extern void gui_debug_screen_display_name(int32 name, bool activate, bool include_children);
extern void gui_debug_screen_rotation(int32 name, bool activate, bool include_children);
extern void gui_debug_text_animation_state(int32 name, bool activate);
extern void gui_debug_text_bounds(int32 name, bool activate);
extern void gui_debug_text_display_name(int32 name, bool activate);
extern void gui_debug_text_rotation(int32 name, bool activate);
extern void gui_print_active_screen_strings_tag_name();
extern void gui_print_active_screens();

