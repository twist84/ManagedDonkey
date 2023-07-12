#include "units/bipeds.hpp"

#include "ai/ai.hpp"
#include "cutscene/recorded_animations.hpp"
#include "game/cheats.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"

HOOK_DECLARE(0x00B6B8F0, biped_bumped_object);

void __cdecl biped_bumped_object(long object_index, long bump_object_index, vector3d const* linear_velocity)
{
	byte* biped = (byte*)(object_header_get(object_index))->datum;
	REFERENCE_DECLARE(biped + 0x198, long, biped_player_index);
	REFERENCE_DECLARE(biped + 0x590, word_flags, biped_biped_flags);
	REFERENCE_DECLARE(biped + 0x5D4, long, biped_bump_object_index);
	REFERENCE_DECLARE(biped + 0x5D8, char, biped_bump_ticks);

	if (biped_bump_ticks < 0)
	{
		if (bump_object_index == NONE)
		{
			biped_bump_ticks++;
			return;
		}

		biped_bump_ticks = static_cast<char>(-game_seconds_to_ticks_round(0.5f));
	}

	if (bump_object_index == NONE)
		return;

	byte* bump_object = (byte*)object_try_and_get_and_verify_type(bump_object_index, 0xFFFFFFFF);
	if (!bump_object)
		return;

	REFERENCE_DECLARE(bump_object + 0x94, c_object_identifier, bump_object_object_identifier);
	REFERENCE_DECLARE(bump_object + 0x198, long, bump_object_player_index);
	REFERENCE_DECLARE(bump_object + 0x5D8, char, bump_object_bump_ticks);

	if (TEST_BIT(BIPED_OBJECT_MASK, object_get_type(bump_object_index)))
	{
		byte* bumped_object = (byte*)object_get_and_verify_type(bump_object_index, BIPED_OBJECT_MASK);

		REFERENCE_DECLARE(bumped_object + 0x590, word_flags, bumped_object_biped_flags);
		//REFERENCE_DECLARE(bumped_object + 0x624, c_character_physics_component, bumped_object_physics);
		byte* bumped_object_physics = bumped_object + 0x624;

		REFERENCE_DECLARE(bumped_object_physics + 0x2, long, physics_mode);
		if (physics_mode == 6)
		{
			//biped->biped_flags.set(15, true);
			biped_biped_flags |= (1 << 15);
		}
	}

	if (biped_bump_ticks >= 0)
	{
		ai_handle_bump(object_index, bump_object_index, linear_velocity);

		if (biped_player_index != -1 || recorded_animation_controlling_unit(object_index))
		{
			if (biped_bump_object_index == bump_object_index)
			{
				if (game_ticks_to_seconds(++biped_bump_ticks) > 0.1f)
				{
					if (TEST_BIT(UNIT_OBJECTS_MASK, bump_object_object_identifier.m_type.get()) &&
						cheat.bump_possession &&
						bump_object_player_index == -1)
					{
						player_set_unit_index(biped_player_index, bump_object_index);

						if (bump_object_object_identifier.m_type == 0)
							bump_object_bump_ticks = static_cast<char>(-game_seconds_to_ticks_round(0.5f));
					}

					biped_bump_ticks = static_cast<char>(-game_seconds_to_ticks_round(0.5f));
				}
			}
			else
			{
				biped_bump_object_index = bump_object_index;
				biped_bump_ticks = 0;
			}
		}
	}
}

