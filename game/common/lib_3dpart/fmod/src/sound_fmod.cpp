#include "fmod/src/sound_fmod.hpp"

#include "memory/module.hpp"

#define MAX_CHANNELS 256

const t_value_type<uns8> fmod_wasapi_fix = { .value = 2 };
const t_value_type<uns32> max_channels_as_dword = { .value = uns32(MAX_CHANNELS) };
const t_value_type<uns8> max_channels_as_byte = { .value = uns8(MAX_CHANNELS) };
//const t_value_type<uns32> sound_preferences_channel_counts_loop_count = { .value = 0 };
//const t_value_type<uns32> sound_channels_datum_size = { .value = uns32(MAX_CHANNELS * 0x38) };
//const t_value_type<int16[5]> sound_preferences_channel_counts = { .value = { 112, 112, 18, 12, 0 } };

HOOK_DECLARE_CLASS_MEMBER(0x0064EF50, HALO_SOUND_SYSTEM, LoadCinePreload);
HOOK_DECLARE_CLASS_MEMBER(0x0064F6B0, HALO_SOUND_SYSTEM, LoadTagParams);

// Adds the FMOD WASAPI output fix from FMODEx 4.44.56, which stops weird popping sound at startup
DATA_PATCH_DECLARE(0x0140DA75, fmod_wasapi_fix, fmod_wasapi_fix.bytes);

// increase fmod software channel count, 192
// http://www.fmod.org/docs/content/generated/FMOD_System_SetSoftwareChannels.html
DATA_PATCH_DECLARE(0x00404DF8 + 1, max_channels, max_channels_as_dword.bytes);

// increase fmod max virtual audio channel count
// http://www.fmod.org/docs/content/generated/FMOD_System_Init.html
DATA_PATCH_DECLARE(0x00404EA0, max_channels, max_channels_as_byte.bytes);
DATA_PATCH_DECLARE(0x00404EC4, max_channels, max_channels_as_byte.bytes);

//// channels counts in `s_sound_preferences` loop count?, 10
//DATA_PATCH_DECLARE(0x00517BE5 + 1, sound_preferences_channel_counts_loop_count, sound_preferences_channel_counts_loop_count.bytes);
//
//// total `sound_channel_datum` alloction size
//// 0x2AE0, 196 * sizeof(sound_channel_datum)
//// 0x3800, 256 * sizeof(sound_channel_datum)
//DATA_PATCH_DECLARE(0x00667725 + 3, sound_channels_datum_size, sound_channels_datum_size.bytes);
//DATA_PATCH_DECLARE(0x00667725 + 3, sound_channels_datum_size, sound_channels_datum_size.bytes);
//
//// channel counts in `s_sound_preferences`, { 42, 42, 21, 14, 0 }
//DATA_PATCH_DECLARE(0x018BE324, sound_preferences_channel_counts, sound_preferences_channel_counts.bytes);
//
//// channel counts in status for `platform_sound_get_status`, 196
//DATA_PATCH_DECLARE(0x00667725 + 3, max_channels, max_channels_as_dword.bytes);
//DATA_PATCH_DECLARE(0x00667733 + 3, max_channels, max_channels_as_dword.bytes);
//DATA_PATCH_DECLARE(0x00667741 + 3, max_channels, max_channels_as_dword.bytes);
//DATA_PATCH_DECLARE(0x0066774F + 3, max_channels, max_channels_as_dword.bytes);
//DATA_PATCH_DECLARE(0x0066775D + 3, max_channels, max_channels_as_dword.bytes);
//
//// voice_map for `build_channel_transmission_sources`, 196
//DATA_PATCH_DECLARE(0x00518F85 + 1, max_channels, max_channels_as_dword.bytes);
//
//// source_map for `build_channel_transmission_sources` 196
//DATA_PATCH_DECLARE(0x00518F93 + 1, max_channels, max_channels_as_dword.bytes);

HALO_SOUND_SYSTEM* __cdecl HALO_SOUND_SYSTEM::GetInstance()
{
	return INVOKE(0x0064CE90, GetInstance);
}

void __thiscall HALO_SOUND_SYSTEM::LoadCinePreload()
{
	// skip loading `data\sound\pc\lst\cine_preload.ps`
}

void __thiscall HALO_SOUND_SYSTEM::LoadTagParams()
{
	// skip loading `data\sound\pc\lst\tags_params.ps`
}

namespace FMOD
{
	// call to `FMOD::System::init` from `FMOD::EventSystemI::init`
	HOOK_DECLARE_CALL(0x01369B0D, System_init);

	// `FMOD::System::init`
	int32 __stdcall System_init(int system, int maxchannels, unsigned int flags, void* extradriverdata)
	{
		return INVOKE(0x013883C1, System_init, system, MAX_CHANNELS, flags, extradriverdata);
	}
};

namespace snd
{
	REFERENCE_DECLARE(0x0698D054, int, THREAD_ID);
	REFERENCE_DECLARE(0x069AD05C, FMOD::System*, fmodOriginalSystem);
	REFERENCE_DECLARE(0x069AD064, SYSTEM_FMOD*, SystemFMod);
	REFERENCE_DECLARE(0x069AD068, HALO_SOUND_SYSTEM*, SystemCustom);
	REFERENCE_DECLARE(0x069AD06C, SYSTEM_FMOD*, System);

	//// result for `SYSTEM_FMOD::sub_4035E0`
	//DATA_PATCH_DECLARE(0x004035E1, max_channels, max_channels_as_dword.bytes);

	int32 __cdecl SYSTEM_FMOD::sub_4035E0()
	{
		return MAX_CHANNELS;
	}

	bool __thiscall SYSTEM_FMOD::Init(int32 a1, void** a2)
	{
		// get pointer to `FMOD::EventSystemI*`
		static bool(__stdcall * FMOD_EventSystem_Create)(FMOD::EventSystemI**) = reinterpret_cast<decltype(FMOD_EventSystem_Create)>(0x01353A80);

		THREAD_ID = system_get_current_thread_id();

		if (!PrepareInit() ||
			FMOD_EventSystem_Create(&system) ||
			system->__vftable->getSystemObject(system, &fmodOriginalSystem) ||
			!InitEventSystem(&a1))
		{
			return false;
		}

		HALO_SOUND_SYSTEM* halo_sound_system = HALO_SOUND_SYSTEM::GetInstance();
		if (!halo_sound_system || halo_sound_system->Init(a1, a2))
		{
			this->state.val |= FLAG(0);
			return true;
		}

		return false;
	}

	bool __cdecl SYSTEM_FMOD::PrepareInit()
	{
		return INVOKE(0x00404D10, PrepareInit);
	}

	bool SYSTEM_FMOD::InitEventSystem(void* a1)
	{
		return DECLFUNC(0x00404D70, bool, __thiscall, SYSTEM_FMOD*, void*)(this, a1);
	}

	//void __thiscall SYSTEM_FMOD::Update(real32 a1)
	//{
	//	HALO_SOUND_SYSTEM::GetInstance()->Update();
	//
	//	// $IMPLEMENT
	//}

}

namespace snd
{
	HOOK_DECLARE_CLASS(0x004035E0, SYSTEM_FMOD, sub_4035E0);
	HOOK_DECLARE_CLASS_MEMBER(0x004047B0, SYSTEM_FMOD, Init);
	//HOOK_DECLARE_CLASS_MEMBER(0x00409280, SYSTEM_FMOD, Update);
}

void __cdecl fmod_initialize_for_new_map()
{
	INVOKE(0x0064DF20, fmod_initialize_for_new_map);
}

void __cdecl fmod_initialize()
{
	//INVOKE(0x0064E190, fmod_initialize);

	//DECLFUNC(0x004047B0, bool, __thiscall, void*, int32, int32)(snd::SystemFMod, 256, 1);
	snd::SystemFMod->__vftable->Init(snd::SystemFMod, 256, 1);
}

void __cdecl fmod_dispose_from_old_map()
{
	INVOKE(0x00652ED0, fmod_dispose_from_old_map);
}

void __cdecl fmod_dispose()
{
	//INVOKE(0x00652EE0, fmod_dispose);

	//DECLFUNC(0x004067F0, void, __thiscall, void*)(snd::SystemFMod);
	snd::SystemFMod->__vftable->Term(snd::SystemFMod);
}

int16 __cdecl sound_definition_find_pitch_range_by_pitch_for_looping_sound_find_or_create_sound(struct s_cache_file_sound_definition* sound, real32 pitch_modifier, int16 pitch_range_index)
{
	int16 result = INVOKE(0x00661C20, sound_definition_find_pitch_range_by_pitch_for_looping_sound_find_or_create_sound, sound, pitch_modifier, pitch_range_index);
	if (result != -1)
		return result;

	return 0;
}
HOOK_DECLARE_CALL(0x00664E39, sound_definition_find_pitch_range_by_pitch_for_looping_sound_find_or_create_sound);

