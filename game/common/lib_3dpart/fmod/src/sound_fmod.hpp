#pragma once

#include "cseries/cseries.hpp"
#include "ds/ds_flags.hpp"
#include "ds/ds_string.hpp"
#include "ds/ds_vector.hpp"
#include "fileio/fileio.hpp"

struct FMOD_REVERB_PROPERTIES
{
	int Instance;
	int Environment;
	float EnvSize;
	float EnvDiffusion;
	int Room;
	int RoomHF;
	int RoomLF;
	float DecayTime;
	float DecayHFRatio;
	float DecayLFRatio;
	int Reflections;
	float ReflectionsDelay;
	float ReflectionsPan[3];
	int Reverb;
	float ReverbDelay;
	float ReverbPan[3];
	float EchoTime;
	float EchoDepth;
	float ModulationTime;
	float ModulationDepth;
	float AirAbsorptionHF;
	float HFReference;
	float LFReference;
	float RoomRolloffFactor;
	float Diffusion;
	float Density;
	unsigned int Flags;
};
COMPILE_ASSERT(sizeof(FMOD_REVERB_PROPERTIES) == 0x7C);

struct HALO_CHANNEL;

struct HALO_SOUND_SYSTEM
{
	struct HALO_SOUND_SYSTEM_vtbl
	{
		int32(__thiscall* Init)(HALO_SOUND_SYSTEM*, int32, void**);
		void(__thiscall* Term)(HALO_SOUND_SYSTEM*);
		void(__thiscall* Update)(HALO_SOUND_SYSTEM*);
		void(__thiscall* InitForNewMap)(HALO_SOUND_SYSTEM*, void*);
		int32(__thiscall* DisposeFromOldMap)(HALO_SOUND_SYSTEM*);
		void* __func14;
		void* __func18;
		int32(__thiscall* InitUIScreenSounds)(HALO_SOUND_SYSTEM*);
		int32(__thiscall* TermUIScreenSounds)(HALO_SOUND_SYSTEM*);
		void* __func24;
		void* __func28;
		void* __func2C;
		void* __func30;
		void* __func34;
		void* __func38;
	};
	HALO_SOUND_SYSTEM_vtbl* __vftable;

	int32 Init(int32 a1, void** a2) { return __vftable->Init(this, a1, a2); }
	void Term() { __vftable->Term(this); }
	void Update() { __vftable->Update(this); }
	void InitForNewMap() { __vftable->InitForNewMap(this, 0); }
	int32 DisposeFromOldMap() { return __vftable->DisposeFromOldMap(this); }
	int32 InitUIScreenSounds() { return __vftable->InitUIScreenSounds(this); }
	int32 TermUIScreenSounds() { return __vftable->TermUIScreenSounds(this); }

	static HALO_SOUND_SYSTEM* __cdecl GetInstance();
	void __thiscall LoadCinePreload();
	void __thiscall LoadTagParams();

	struct VolumeStruct
	{
		int32 __unknown0;
		real32 __unknown4;
	};
	
	// starts as volume but morths into some data in `FMOD::EventSystemI`
	real32 Volume;

	int32 __unknown8;
	int32 SampleRate; // 48000
	VolumeStruct SfxVolume;
	VolumeStruct VoiceVolume;
	VolumeStruct MusicVolume;
	VolumeStruct MasterVolume;
	uns32 __flags30;
	int32 __unknown34;
	int32 __unknown38;
	int32 __unknown3C;
	int32 __unknown40;
	real32 __unknown44;
	int32 __unknown48;
	real32 __unknown4C;
	int32 __unknown50;
	real32 __unknown54;
	int32 __unknown58; // 5, SoundEncoding?
	byte __data5C[0x18];
	HALO_CHANNEL* Channels;
	int32 ChannelCount;
	int32 MaximumChannelCount;
	byte __data80[0x12C];
	byte __data1AC[0x58];
};
COMPILE_ASSERT(sizeof(HALO_SOUND_SYSTEM) == 0x204);

namespace FMOD
{
	struct ChannelGroup
	{

	};

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
			int32(__stdcall* getSystemObject)(EventSystemI*, void*);
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
	COMPILE_ASSERT(sizeof(EventSystemI) == 0xE4);

	extern int32 __stdcall System_init(int system, int maxchannels, unsigned int flags, void* extradriverdata);
};

namespace snd
{
	enum SYSTEM_STATE
	{
		SST_INITED = FLAG(0),
		SST_EAX = FLAG(1),
		SST_EAX_ENABLED = FLAG(2),
		SST_MUTED = FLAG(3),
		SST_UPDATE_CATEGORIES = FLAG(4),
		SST_USER = FLAG(5),
	};

	struct SYSTEM
	{
		struct SYSTEM_vtbl
		{
			void* Destructor;
			void(__thiscall* Init)(SYSTEM*, int32, int32);
			int32(__thiscall* Term)(SYSTEM*);
			void* Mute;
			void* StopAll;
			int32(__thiscall* Update)(SYSTEM*, uns32);
			void* GetBuffer;
			void* Func07;
			void* Func08;
			void* RegisterData;
			void* RegisterLevelSounds;
			void* RegisterWavebank;
			void* CommitRegisteredWavebanks;
			void(__thiscall* UnregisterWavebank)(SYSTEM*, int32*);
			void* GetRegisteredWavebanks;
			void* PreloadSoundbank;
			void* UnloadSoundbank;
			void* PrepareWavebanks;
			void* EnableEAX;
			void(__thiscall* SetPreset)(SYSTEM*, uns32*);
			void* Func20;
			void(__thiscall* Func21)(SYSTEM*, uns32, uns32);
			void(__thiscall* Func22)(SYSTEM*, int32);
			void* SetListener;
			void(__thiscall* GetListener)(SYSTEM*, int32, uns64*, uns32, uns32, uns32);
			void* GetInfo;
			void* GetPlayingIter;
			void* GetInfoIter;
			void* FindInfoIter;
			int32(__thiscall* Func29)(SYSTEM*);
			void* DropBuffer;
			void* UpdateListenersCount;
		};

		SYSTEM_vtbl* __vftable /*VFT*/;
		dsFLAGS<SYSTEM_STATE, int> state;
		byte __data8[0x24];
		int listenersCount;
	};
	COMPILE_ASSERT(sizeof(SYSTEM) == 0x30);

	struct SYSTEM_FMOD :
		SYSTEM
	{
		static int32 __cdecl sub_4035E0();
		bool __thiscall Init(int32 a1, void** a2);
		static bool __cdecl PrepareInit();
		bool InitEventSystem(void* a1);
		//void __thiscall Update(real32 a1);

		FMOD::EventSystemI* system;
		FMOD::ChannelGroup* master;
		FMOD::ChannelGroup* reverb;
		byte __data3C[04];
		uns32 m_SpeakerMode;
		byte __data44[0x3C];
		float mostAudible;
		int numPlaying;
		int numStreamPlaying;
		int getBufferCounter;
		byte __data90[0x70];
		osTIMER lastExpiration;
		byte __data108[0x58];
		fioFILE_MEM psbin;
		byte __data1FC[0x5C];
		FMOD_REVERB_PROPERTIES targetPreset;
		FMOD_REVERB_PROPERTIES currentPreset;
		dsVECTOR<FMOD_REVERB_PROPERTIES, 8> auxPresets;
		dsVECTOR<dsTSTRING<char>, 8> auxPresetNames;
	};
	COMPILE_ASSERT(sizeof(SYSTEM_FMOD) == 0x368);

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

