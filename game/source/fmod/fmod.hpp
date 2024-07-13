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
	void __thiscall LoadCinePreload();
	void __thiscall LoadTagParams();

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
	struct System
	{
	};

	struct EventSystemI
	{
		struct EventSystemI_vtbl
		{
			void* init;
			void* release;
			void* update;
			void* setMediaPath;
			void* setPluginPath;
			void* getVersion;
			void* getInfo;
			long(__stdcall* getSystemObject)(EventSystemI*, void*);
			void* getMusicSystem;
			void* load;
			void* unload;
			void* getProject;
			void* getProjectByIndex;
			void* getNumProjects;
			void* getCategory;
			void* getCategoryByIndex;
			void* getMusicCategory;
			void* getNumCategories;
			void* getGroup;
			void* getEvent;
			void* getEventBySystemID;
			void* getEventByGUID;
			void* getEventByGUIDString;
			void* getNumEvents;
			void* setReverbProperties;
			void* getReverbProperties;
			void* getReverbPreset;
			void* getReverbPresetByIndex;
			void* getNumReverbPresets;
			void* createReverb;
			void* setReverbAmbientProperties;
			void* getReverbAmbientProperties;
			void* set3DNumListeners;
			void* get3DNumListeners;
			void* set3DListenerAttributes;
			void* get3DListenerAttributes;
			void* setUserData;
			void* getUserData;
			void* preloadFSB;
			void* unloadFSB;
			void* getMemoryInfo;
			void* EventSystemDtor;
			void* getMemoryUsedImpl;
			void* getMemoryUsed;
			void* getMemoryUsed2;
			void* loadPlugins;
			void* dereferenceSoundnames;
			void* createDSPByName;
			void* getSoundbank;
			void* getSoundDef;
			void* releaseSoundDefs;
			void* releaseSoundBanks;
			void* getMute;
			void* rebuildGUIDTable;
			void* rebuildEventTable;
			void* createEventTable;
			void* setEventTableEntry;
			void* getEventID;
			void* setFlags;
			void* countEvents;
			void* readSoundDefDef;
			void* buildEventI;
			void* dereferenceSoundname;
			void* openFile;
			void* closeFile;
			void* writeFileData;
			void* callEventCallback;
			void* targetObjectCommand;
		};

		EventSystemI_vtbl* __vftable /*VFT*/;
		byte __data[0xE0];
	};
	static_assert(sizeof(EventSystemI) == 0xE4);

	extern long __stdcall System_init(int system, int maxchannels, unsigned int flags, void* extradriverdata);
	extern long __cdecl sub_4035E0();
};

namespace snd
{
	struct SYSTEM
	{
		struct SYSTEM_vtbl
		{
			void* Destructor;
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
			void* CommitRegisteredWavebanks;
			void(__thiscall* UnregisterWavebank)(SYSTEM*, long*);
			void* GetRegisteredWavebanks;
			void* PreloadSoundbank;
			void* UnloadSoundbank;
			void* PrepareWavebanks;
			void* EnableEAX;
			void(__thiscall* SetPreset)(SYSTEM*, dword*);
			void* Func20;
			void(__thiscall* Func21)(SYSTEM*, dword, dword);
			void(__thiscall* Func22)(SYSTEM*, long);
			void* SetListener;
			void(__thiscall* GetListener)(SYSTEM*, long, qword*, dword, dword, dword);
			void* GetInfo;
			void* GetPlayingIter;
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

		FMOD::EventSystemI* system;
		byte __data34[0xC];
		dword m_SpeakerMode;
		byte __data44[0x324];
	};
	static_assert(sizeof(SYSTEM_FMOD) == 0x368);

	//extern int& THREAD_ID;
	//extern FMOD::System*& fmodOriginalSystem;
	//extern SYSTEM_FMOD*& SystemFMod;
	//extern HALO_SOUND_SYSTEM*& SystemCustom;
	//extern SYSTEM_FMOD*& System;
}

extern void __cdecl fmod_initialize_for_new_map();
extern void __cdecl fmod_initialize();
extern void __cdecl fmod_dispose_from_old_map();
extern void __cdecl fmod_dispose();

