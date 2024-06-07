#pragma once

#include "cseries/cseries.hpp"

struct HALO_CHANNEL;

struct HALO_SOUND_SYSTEM
{
	struct HALO_SOUND_SYSTEM_vtbl
	{
		long(__thiscall* Init)(HALO_SOUND_SYSTEM*, long, void**);
		void(__thiscall* Term)(HALO_SOUND_SYSTEM*);
		void(__thiscall* Update)(HALO_SOUND_SYSTEM*);
		void(__thiscall* InitForNewMap)(HALO_SOUND_SYSTEM*, void*);
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
	HALO_SOUND_SYSTEM_vtbl* __vftable;

	long Init(long a1, void** a2) { return __vftable->Init(this, a1, a2); }
	void Term() { __vftable->Term(this); }
	void Update() { __vftable->Update(this); }
	void InitForNewMap() { __vftable->InitForNewMap(this, 0); }
	long DisposeFromOldMap() { return __vftable->DisposeFromOldMap(this); }
	long InitUIScreenSounds() { return __vftable->InitUIScreenSounds(this); }
	long TermUIScreenSounds() { return __vftable->TermUIScreenSounds(this); }

	static HALO_SOUND_SYSTEM* __cdecl GetInstance();
	void __thiscall sub_64EF50();
	void __thiscall sub_64F6B0();

	struct VolumeStruct
	{
		long __unknown0;
		real __unknown4;
	};
	
	// starts as volume but morths into some data in `FMOD::EventSystemI`
	real Volume;

	long __unknown8;
	long SampleRate; // 48000
	VolumeStruct SfxVolume;
	VolumeStruct VoiceVolume;
	VolumeStruct MusicVolume;
	VolumeStruct MasterVolume;
	dword_flags __flags30;
	long __unknown34;
	long __unknown38;
	long __unknown3C;
	long __unknown40;
	real __unknown44;
	long __unknown48;
	real __unknown4C;
	long __unknown50;
	real __unknown54;
	long __unknown58; // 5, SoundEncoding?
	byte __data5C[0x18];
	HALO_CHANNEL* Channels;
	long ChannelCount;
	long MaximumChannelCount;
	byte __data80[0x12C];
	byte __data1AC[0x58];
};
static_assert(sizeof(HALO_SOUND_SYSTEM) == 0x204);

namespace FMOD
{
	struct EventSystemI
	{
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

		EventSystemI_vtbl* __vftable /*VFT*/;
		byte __data[0xE0];
	};
	static_assert(sizeof(EventSystemI) == 0xE4);

	extern long __stdcall sub_13883C1(long a1, long max_channels, long flags, long extra_driver_data);
	extern long __cdecl sub_4035E0();
};

namespace snd
{
	struct SYSTEM
	{
		struct SYSTEM_vtbl
		{
			void* Free;
			void(__thiscall* Init)(SYSTEM*, long, long);
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

		SYSTEM_vtbl* __vftable /*VFT*/;
		dword_flags flags;
		byte __data8[0x28];
	};
	static_assert(sizeof(SYSTEM) == 0x30);

	struct SYSTEM_FMOD :
		SYSTEM
	{
		static long __cdecl sub_4035E0();
		bool __thiscall Init(long a1, void** a2);
		static bool __cdecl PrepareInit();
		bool InitEventSystem(void* a1);
		//void __thiscall Update(real a1);

		FMOD::EventSystemI* m_pEventSystemI;
		byte __data34[0xC];
		dword m_SpeakerMode;
		byte __data44[0x324];
	};
	static_assert(sizeof(SYSTEM_FMOD) == 0x368);

	//extern dword& g_SoundThreadId;
	//extern void*& dword_69AD05C;
	//extern SYSTEM_FMOD*& g_SYSTEM_FMOD;
	//extern HALO_SOUND_SYSTEM*& g_HaloSoundSystem;
	//extern SYSTEM_FMOD*& g_SYSTEM_FMOD_for_threads;
}

extern void __cdecl fmod_initialize_for_new_map();
extern void __cdecl fmod_initialize();
extern void __cdecl fmod_dispose_from_old_map();
extern void __cdecl fmod_dispose();

