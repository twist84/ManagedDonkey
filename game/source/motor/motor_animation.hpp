#pragma once

enum e_motor_resource
{
	_motor_resource_none = NONE,
	_motor_resource_state,
	_motor_resource_overlay,
	_motor_resource_primary_impulse,
	_motor_resource_secondary_impulse,

	k_motor_resource_count,

	_motor_resource_mask_all = FLAG(_motor_resource_state) | FLAG(_motor_resource_overlay) | FLAG(_motor_resource_primary_impulse) | FLAG(_motor_resource_secondary_impulse),
	_motor_resource_mask_impulse = FLAG(_motor_resource_primary_impulse) | FLAG(_motor_resource_secondary_impulse),
	_motor_resource_mask_state = FLAG(_motor_resource_state),
};
COMPILE_ASSERT(0xF == _motor_resource_mask_all);
COMPILE_ASSERT(0xC == _motor_resource_mask_impulse);
COMPILE_ASSERT(0x1 == _motor_resource_mask_state);

extern bool __cdecl motor_animation_resource_busy(int32 mover_index, e_motor_resource resource);

