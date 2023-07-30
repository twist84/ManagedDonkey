#include "game/game_engine_event_definitions.hpp"

void s_sound_response_extra_sounds_definition::update_reference_names()
{
	japanese_sound.get_name();
	german_sound.get_name();
	french_sound.get_name();
	spanish_sound.get_name();
	mexican_sound.get_name();
	italian_sound.get_name();
	korean_sound.get_name();
	chinese_sound_traditional.get_name();
	chinese_sound_simplified.get_name();
	portuguese_sound.get_name();
	russian_sound.get_name();
}

void s_multiplayer_event_sound_response_definition::update_reference_names()
{
	english_sound.get_name();
	extra_sounds.update_reference_names();
}

void s_multiplayer_event_response_definition::update_reference_names()
{
	sound.get_name();
	extra_sounds.update_reference_names();
	UPDATE_REFERENCE_NAMES(sound_permutations);
}

