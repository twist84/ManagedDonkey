#include "interface/gui_selected_items.hpp"

#include "memory/module.hpp"
#include "interface/gui_selected_items_map.hpp"
#include "interface/gui_selected_items_game_variant.hpp"
#include "interface/gui_selected_items_saved_film.hpp"

// replace call `c_load_pregame_players_selection_screen_message` with `c_load_pregame_selection_screen_message`
uns32 load_pregame_selection_screen_message_ctor = 0x00AC34D0;
HOOK_DECLARE_CALL_WITH_ADDRESS(0x00B21F33, load_pregame_selection_screen_message_ctor, load_pregame_players_selection_screen_message_ctor);

bool gui_selected_item_get_file_path(c_gui_selected_item const* selected_item, wchar_t* path, int32 maximum_characters)
{
	switch (selected_item->m_selection_type)
	{
	case _gui_selection_type_map:
	{
		//return INVOKE_CLASS_MEMBER(0x00AD97B0, c_gui_selected_item, get_file_path, path, maximum_characters); // c_gui_map_selected_item
		c_gui_map_selected_item const* map_selected_item = (c_gui_map_selected_item const*)selected_item;
		return map_selected_item->c_gui_map_selected_item::get_file_path(path, maximum_characters);
	}
	break;
	case _gui_selection_type_game_variant:
	{
		//return INVOKE_CLASS_MEMBER(0x00ADB4B0, c_gui_selected_item, get_file_path, path, maximum_characters); // c_gui_game_variant_selected_item
		c_gui_game_variant_selected_item const* game_variant_selected_item = (c_gui_game_variant_selected_item const*)selected_item;
		return game_variant_selected_item->c_gui_game_variant_selected_item::get_file_path(path, maximum_characters);
	}
	break;
	case _gui_selection_type_film:
	{
		//return INVOKE_CLASS_MEMBER(0x00ADCAE0, c_gui_selected_item, get_file_path, path, maximum_characters); // c_gui_saved_film_selected_item
		c_gui_saved_film_selected_item const* saved_film_selected_item = (c_gui_saved_film_selected_item const*)selected_item;
		return saved_film_selected_item->c_gui_saved_film_selected_item::get_file_path(path, maximum_characters);
	}
	break;
	}

	return false;
}

bool c_gui_selected_item::get_corrupt() const
{
	//return INVOKE_CLASS_MEMBER(0x00680AC0, c_gui_selected_item, get_corrupt);

	return m_corrupt;
}

const utf8* c_gui_selected_item::get_description(utf8* buffer, int32 buffer_length) const
{
	//return INVOKE_CLASS_MEMBER(0x00680B00, c_gui_selected_item, get_description, buffer, buffer_length);
	return DECLFUNC(0x00680B00, const utf8*, __thiscall, c_gui_selected_item const*, utf8*, int32)(this, buffer, buffer_length);
}

const wchar_t* c_gui_selected_item::get_description() const
{
	//return INVOKE_CLASS_MEMBER(0x00680B20, c_gui_selected_item, get_description);
	//return DECLFUNC(0x00680B20, const wchar_t*, __thiscall, c_gui_selected_item const*)(this);

	return m_metadata.description;
}

bool c_gui_selected_item::get_enabled() const
{
	//return INVOKE_CLASS_MEMBER(0x00680B30, c_gui_selected_item, get_enabled);

	return m_enabled;
}

bool c_gui_selected_item::get_file_path(wchar_t* path, int32 maximum_characters) const
{
	//return INVOKE_CLASS_MEMBER(0x00680BA0, c_gui_selected_item, get_file_path, path, maximum_characters);

	return false;
}

bool c_gui_selected_item::get_is_new() const
{
	//return INVOKE_CLASS_MEMBER(0x00680BC0, c_gui_selected_item, get_is_new);

	return m_is_new;
}

e_gui_selected_item_location c_gui_selected_item::get_location() const
{
	//return INVOKE_CLASS_MEMBER(0x00680BD0, c_gui_selected_item, get_location);

	return m_location;
}

const wchar_t* c_gui_selected_item::get_title() const
{
	//return INVOKE_CLASS_MEMBER(0x00680C40, c_gui_selected_item, get_title);

	return m_metadata.display_name;
}

bool c_gui_selected_item::is_valid() const
{
	//return INVOKE_CLASS_MEMBER(0x00680C50, c_gui_selected_item, is_valid);

	return m_selection_type != _gui_selection_type_none;
}

const s_ui_saved_game_item_metadata* c_gui_selected_item::get_metadata(s_ui_saved_game_item_metadata* metadata) const
{
	//return INVOKE_CLASS_MEMBER(0x00AC35C0, c_gui_selected_item, get_metadata, metadata);

	*metadata = m_metadata;
	return metadata;
}

