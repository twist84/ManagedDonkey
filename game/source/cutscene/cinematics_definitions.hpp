#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/tag_groups.hpp"

struct cinematics_globals_block
{
	c_typed_tag_reference<SCENERY_TAG> cinematic_anchor_reference;
	real cinematic_film_aperture;
	real subtitles_screen_position;

	void update_reference_names();
};
static_assert(sizeof(cinematics_globals_block) == 0x18);

struct s_cinematic_reference;
struct s_scenario_cutscene_title;