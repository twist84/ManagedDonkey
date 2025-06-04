#include "interface/gui_selected_items.hpp"

#include "memory/module.hpp"

// replace call `c_load_pregame_players_selection_screen_message` with `c_load_pregame_selection_screen_message`
uns32 load_pregame_selection_screen_message_ctor = 0x00AC34D0;
HOOK_DECLARE_CALL_WITH_ADDRESS(0x00B21F33, load_pregame_selection_screen_message_ctor, load_pregame_players_selection_screen_message_ctor);

bool c_gui_selected_item::get_corrupt() const
{
	return INVOKE_CLASS_MEMBER(0x00680AC0, c_gui_selected_item, get_corrupt);
}

const utf8* c_gui_selected_item::get_description(utf8* buffer, int32 buffer_length) const
{
	//return INVOKE_CLASS_MEMBER(0x00680B00, c_gui_selected_item, get_description, buffer, buffer_length);
	return DECLFUNC(0x00680B00, const utf8*, __thiscall, c_gui_selected_item const*, utf8*, int32)(this, buffer, buffer_length);
}

const wchar_t* c_gui_selected_item::get_description() const
{
	//return INVOKE_CLASS_MEMBER(0x00680B20, c_gui_selected_item, get_description);
	return DECLFUNC(0x00680B20, const wchar_t*, __thiscall, c_gui_selected_item const*)(this);
}

bool c_gui_selected_item::get_enabled() const
{
	return INVOKE_CLASS_MEMBER(0x00680B30, c_gui_selected_item, get_enabled);
}

bool c_gui_selected_item::get_file_path(char* path, int32 maximum_characters) const
{
	return INVOKE_CLASS_MEMBER(0x00680BA0, c_gui_selected_item, get_file_path, path, maximum_characters);
}

bool c_gui_selected_item::get_is_new() const
{
	return INVOKE_CLASS_MEMBER(0x00680BC0, c_gui_selected_item, get_is_new);
}

e_gui_selected_item_location c_gui_selected_item::get_location() const
{
	return INVOKE_CLASS_MEMBER(0x00680BD0, c_gui_selected_item, get_location);
}

const wchar_t* c_gui_selected_item::get_title() const
{
	return INVOKE_CLASS_MEMBER(0x00680C40, c_gui_selected_item, get_title);
}

bool c_gui_selected_item::is_valid() const
{
	return INVOKE_CLASS_MEMBER(0x00680C50, c_gui_selected_item, is_valid);
}

const s_ui_saved_game_item_metadata* c_gui_selected_item::get_metadata(s_ui_saved_game_item_metadata* metadata) const
{
	return INVOKE_CLASS_MEMBER(0x00AC35C0, c_gui_selected_item, get_metadata, metadata);
}

