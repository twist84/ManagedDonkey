#pragma once

#include "tag_files/tag_groups.hpp"

struct s_sound_globals_definition
{
	c_typed_tag_reference<SOUND_CLASSES_TAG> sound_classes;
	c_typed_tag_reference<SOUND_EFFECT_COLLECTION_TAG> sound_effects;
	c_typed_tag_reference<SOUND_MIX_TAG> sound_mix;
	c_typed_tag_reference<SOUND_DIALOGUE_CONSTANTS_TAG> sound_combat_dialogue_constants;
	c_typed_tag_reference<SOUND_GLOBAL_PROPAGATION_TAG> sound_propagation;
	c_typed_tag_reference<SOUND_UI_SOUNDS_TAG> gfx_ui_sounds;

	void update_reference_names();
};
static_assert(sizeof(s_sound_globals_definition) == 0x60);

