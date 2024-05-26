#include "fmod/fmod.hpp"

#include "cseries/cseries.hpp"
#include "memory/module.hpp"

#define MAX_CHANNELS 1024

struct HALO_SOUND_SYSTEM;
struct HALO_SOUND_SYSTEM_vtbl
{
	long(__thiscall* Init)(HALO_SOUND_SYSTEM*, long, void**);
	void(__thiscall* Term)(HALO_SOUND_SYSTEM*);
	void(__thiscall* Update)(HALO_SOUND_SYSTEM*);
	void(__thiscall* InitForNewMap)(HALO_SOUND_SYSTEM*);
	long(__thiscall* DisposeFromOldMap)(HALO_SOUND_SYSTEM*);
	void* __func14;
	void* __func18;
	long(__thiscall* InitUIScreenSounds)(HALO_SOUND_SYSTEM*);
	long(__thiscall* TermUIScreenSounds)(HALO_SOUND_SYSTEM*);
	void* __func24;
	void* __func28;
	void* __func2C;
	void* __func30;
	void* __func34;
	void* __func38;
};

struct HALO_SOUND_SYSTEM
{
	HALO_SOUND_SYSTEM_vtbl* __vftable /*VFT*/;

	void __thiscall sub_64EF50()
	{
		// skip loading `data\sound\pc\lst\cine_preload.ps`
	}

	void __thiscall sub_64F6B0()
	{
		// skip loading `data\sound\pc\lst\tags_params.ps`
	}

	real Volume;
	byte __data[0x1FC];
};
static_assert(sizeof(HALO_SOUND_SYSTEM) == 0x204);

REFERENCE_DECLARE(0x0698D054, dword, g_SoundThreadId);
REFERENCE_DECLARE(0x069AD05C, void*, dword_69AD05C);
REFERENCE_DECLARE(0x069AD068, HALO_SOUND_SYSTEM*, g_HaloSoundSystem);

byte const popping_sound_on_startup_patch_bytes[] = { 0x2 };
DATA_PATCH_DECLARE(0x0140DA75, popping_sound_on_startup, popping_sound_on_startup_patch_bytes);

union
{
	byte const patch_bytes[sizeof(dword)];
	dword value = MAX_CHANNELS;

} max_channels_data;
DATA_PATCH_DECLARE(0x00404DF8 + 1, max_channels, max_channels_data.patch_bytes);

HOOK_DECLARE_CLASS_MEMBER(0x0064EF50, HALO_SOUND_SYSTEM, sub_64EF50);
HOOK_DECLARE_CLASS_MEMBER(0x0064F6B0, HALO_SOUND_SYSTEM, sub_64F6B0);

namespace FMOD
{
	struct EventSystemI;
	struct EventSystemI_vtbl
	{
		void* __func0;
		void* __func4;
		void* __func8;
		void* __funcC;
		void* __func10;
		void* __func14;
		void* __func18;
		long(__stdcall* __func1C)(EventSystemI*, void*);
		void* __func20;
		void* __func24;
		void* __func28;
		void* __func2C;
		void* __func30;
		void* __func34;
		void* __func38;
		void* __func3C;
		void* __func40;
		void* __func44;
		void* __func48;
		void* __func4C;
		void* __func50;
		void* __func54;
		void* __func58;
		void* __func5C;
		void* __func60;
		void* __func64;
		void* __func68;
		void* __func6C;
		void* __func70;
		void* __func74;
		void* __func78;
		void* __func7C;
		void* __func80;
		void* __func84;
		void* __func88;
		void* __func8C;
		void* __func90;
		void* __func94;
		void* __func98;
		void* __func9C;
		void* __funcA0;
		void* __funcA4;
		void* __funcA8;
		void* __funcAC;
		void* __funcB0;
		void* __funcB4;
		void* __funcB8;
		void* __funcBC;
		void* __funcC0;
		void* __funcC4;
		void* __funcC8;
		void* __funcCC;
		void* __funcD0;
		void* __funcD4;
		void* __funcD8;
		void* __funcDC;
		void* __funcE0;
		void* __funcE4;
		void* __funcE8;
		void* __funcEC;
		void* __funcF0;
		void* __funcF4;
		void* __funcF8;
		void* __funcFC;
		void* __func100;
		void* __func104;
		void* __func108;
		void* __func10C;
	};

	struct EventSystemI
	{
		EventSystemI_vtbl* __vftable /*VFT*/;
		byte __data[0xE0];
	};
	static_assert(sizeof(EventSystemI) == 0xE4);

	long __stdcall sub_13883C1(long a1, long max_channels, long flags, long extra_driver_data)
	{
		return INVOKE(0x013883C1, sub_13883C1, a1, MAX_CHANNELS, flags, extra_driver_data);
	}
	HOOK_DECLARE_CALL(0x01369B0D, sub_13883C1);
};

namespace snd
{
	struct SYSTEM;
	struct SYSTEM_vtbl
	{
		void* Free;
		void(__thiscall* Init)(SYSTEM*, byte, long, long);
		long(__thiscall* Term)(SYSTEM*);
		void* Mute;
		void* StopAll;
		long(__thiscall* Update)(SYSTEM*, dword);
		void* GetBuffer;
		void* Func07;
		void* Func08;
		void* RegisterData;
		void* RegisterLevelSounds;
		void* RegisterWavebank;
		void* Func12;
		void(__thiscall* UnregisterWavebank)(SYSTEM*, long*);
		void* Func14;
		void* GetWavebank;
		void* GetWavebank2;
		void* PrepareWavebanks;
		void* EnableEAX;
		void(__thiscall* SetPreset)(SYSTEM*, dword*);
		void* Func20;
		void(__thiscall* Func21)(SYSTEM*, dword, dword);
		void(__thiscall* Func22)(SYSTEM*, long);
		void* SetListener;
		void(__thiscall* GetListener)(SYSTEM*, long, qword*, dword, dword, dword);
		void* GetInfo;
		void* Func26;
		void* GetInfoIter;
		void* FindInfoIter;
		long(__thiscall* Func29)(SYSTEM*);
		void* DropBuffer;
		void* UpdateListenersCount;
	};

	struct SYSTEM
	{
		SYSTEM_vtbl* __vftable /*VFT*/;
		dword_flags flags;
		byte __data8[0x28];
	};
	static_assert(sizeof(SYSTEM) == 0x30);

	struct SYSTEM_FMOD :
		SYSTEM
	{
		static long __cdecl sub_4035E0()
		{
			return MAX_CHANNELS;
		}

		bool __thiscall Init(long a1, void** a2)
		{
			// get pointer to `FMOD::EventSystemI*`
			static bool(__stdcall * sub_1353A80)(FMOD::EventSystemI**) = reinterpret_cast<decltype(sub_1353A80)>(0x01353A80);

			g_SoundThreadId = system_get_current_thread_id();

			if (!PrepareInit() ||
				sub_1353A80(&m_pEventSystemI) ||
				m_pEventSystemI->__vftable->__func1C(m_pEventSystemI, &dword_69AD05C) ||
				!InitEventSystem(&a1))
			{
				return false;
			}

			if (!g_HaloSoundSystem || g_HaloSoundSystem->__vftable->Init(g_HaloSoundSystem, a1, a2)) // `HALO_SOUND_SYSTEM::Init` crashes
			{
				flags |= FLAG(0);
				return true;
			}

			return false;
		}

		static bool __cdecl PrepareInit()
		{
			return INVOKE(0x00404D10, PrepareInit);
		}

		bool InitEventSystem(void* a1)
		{
			return DECLFUNC(0x00404D70, bool, __thiscall, SYSTEM_FMOD*, void*)(this, a1);
		}

		//void __thiscall Update(real a1)
		//{
		//	if (g_HaloSoundSystem)
		//		g_HaloSoundSystem->__vftable->Update(g_HaloSoundSystem);
		//
		//	// #TODO: implement me
		//}

		FMOD::EventSystemI* m_pEventSystemI;
		byte __data34[0xC];
		dword m_SpeakerMode;
		byte __data44[0x324];
	};
	static_assert(sizeof(SYSTEM_FMOD) == 0x368);

	REFERENCE_DECLARE(0x069AD064, snd::SYSTEM_FMOD*, g_SYSTEM_FMOD);
	REFERENCE_DECLARE(0x069AD06C, snd::SYSTEM_FMOD*, g_SYSTEM_FMOD_for_threads);

	HOOK_DECLARE_CLASS(0x004035E0, SYSTEM_FMOD, sub_4035E0);
	HOOK_DECLARE_CLASS_MEMBER(0x004047B0, SYSTEM_FMOD, Init);
	//HOOK_DECLARE_CLASS_MEMBER(0x00409280, SYSTEM_FMOD, Update);
}

short __cdecl sound_definition_find_pitch_range_by_pitch_for_looping_sound_find_or_create_sound(struct s_cache_file_sound_definition* sound, real pitch_modifier, short pitch_range_index)
{
	short result = INVOKE(0x00661C20, sound_definition_find_pitch_range_by_pitch_for_looping_sound_find_or_create_sound, sound, pitch_modifier, pitch_range_index);
	if (result != -1)
		return result;

	return 0;
}
HOOK_DECLARE_CALL(0x00664E39, sound_definition_find_pitch_range_by_pitch_for_looping_sound_find_or_create_sound);

void __cdecl fmod_initialize()
{
	//INVOKE(0x0064E190, fmod_initialize);

	//snd::SYSTEM_FMOD::Init
	DECLFUNC(0x004047B0, bool, __thiscall, void*, long, long)(snd::g_SYSTEM_FMOD, 256, 1);
}

void __cdecl fmod_terminate()
{
	//INVOKE(0x00652EE0, fmod_terminate);

	DECLFUNC(0x004067F0, void, __thiscall, void*)(snd::g_SYSTEM_FMOD);
}

