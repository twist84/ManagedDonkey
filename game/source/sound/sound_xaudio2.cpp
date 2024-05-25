#include "sound/sound_xaudio2.hpp"

#include "cache/cache_files.hpp"
#include "cseries/cseries.hpp"
#include "memory/module.hpp"

#include <XAudio2.h>
#include <X3DAudio.h>
#pragma comment(lib, "X3DAudio.lib")

HOOK_DECLARE(0x00656170, platform_sound_source_queue_sound);
HOOK_DECLARE(0x00667640, platform_sound_dispose);
HOOK_DECLARE(0x00667770, platform_sound_initialize);

//class c_sound_xaudio2_engine : IXAudio2EngineCallback;
//class c_sound_xaudio2_source : IXAudio2VoiceCallback;

//c_pc_audio_output_manager
//c_pc_audio_reverb_manager
//c_pc_audio_source_manager
//c_pc_audio_voice_manager
//c_pc_audio_dsp_manager
//s_sound_permutation_chunk
//c_platform_source_datum
//  m_state << 16 >> 28
//  m_queued_sound_count << 23 >> 29
//  m_playing_sound_count << 26 >> 29
//c_platform_source_packet_controller
//t_event_queue<s_pending_audio_callback, 392> pending_audio_queue; // 392, total_source_count * 2?

// ONLY WAV IS SUPPORTED FOR NOW
#define SOUND_QUEUE_FILE LR"(D:\Games\ProjectCartographer\Game\sounds\grunt_birthday_party.wav)"

IXAudio2* g_pXAudio2 = NULL;
IXAudio2MasteringVoice* g_pMasterVoice = NULL;
IXAudio2SourceVoice* g_pSourceVoice = NULL;
XAUDIO2_BUFFER g_buffer = { 0 };

#ifdef _XBOX //Big-Endian
#define fourccRIFF 'RIFF'
#define fourccDATA 'data'
#define fourccFMT 'fmt '
#define fourccWAVE 'WAVE'
#define fourccXWMA 'XWMA'
#define fourccDPDS 'dpds'
#endif

#ifndef _XBOX //Little-Endian
#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'
#endif

HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());

	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD bytesRead = 0;
	DWORD dwOffset = 0;

	while (hr == S_OK)
	{
		DWORD dwRead;
		if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());

		if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());

		switch (dwChunkType)
		{
		case fourccRIFF:
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL))
				hr = HRESULT_FROM_WIN32(GetLastError());
			break;

		default:
			if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT))
				return HRESULT_FROM_WIN32(GetLastError());
		}

		dwOffset += sizeof(DWORD) * 2;

		if (dwChunkType == fourcc)
		{
			dwChunkSize = dwChunkDataSize;
			dwChunkDataPosition = dwOffset;
			return S_OK;
		}

		dwOffset += dwChunkDataSize;

		if (bytesRead >= dwRIFFDataSize) return S_FALSE;

	}

	return S_OK;
}

HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());
	DWORD dwRead;
	if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL))
		hr = HRESULT_FROM_WIN32(GetLastError());
	return hr;
}

HRESULT XAudioInitialize(IXAudio2** lpXAudio2)
{
	ASSERT(lpXAudio2);

	HRESULT hr;
	hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hr))
		return hr;

	IXAudio2* pXAudio2 = NULL;
	if (FAILED(hr = XAudio2Create(&pXAudio2, 0, Processor1)))
		return hr;

	*lpXAudio2 = pXAudio2;

	return S_OK;
}

HRESULT XAudioCreateMasteringVoice(IXAudio2* pXAudio2, IXAudio2MasteringVoice** lpMasterVoice)
{
	ASSERT(pXAudio2);
	ASSERT(lpMasterVoice);

	HRESULT hr;
	IXAudio2MasteringVoice* pMasterVoice = NULL;
	if (FAILED(hr = pXAudio2->CreateMasteringVoice(&pMasterVoice)))
		return hr;

	*lpMasterVoice = pMasterVoice;

	return S_OK;
}

void XAudioDispose(IXAudio2* pXAudio2, IXAudio2MasteringVoice* pMasterVoice, IXAudio2SourceVoice* pSourceVoice)
{
	if (pSourceVoice != NULL)
	{
		pSourceVoice->DestroyVoice();
		pSourceVoice = NULL;
	}

	if (pMasterVoice != NULL)
	{
		pMasterVoice->DestroyVoice();
		pMasterVoice = NULL;
	}

	if (pSourceVoice != NULL)
	{
		pXAudio2->Release();
		pXAudio2 = NULL;
	}

	CoUninitialize();
}

HRESULT XAudioReadBufferFromFile(wchar_t const* strFileName, WAVEFORMATEXTENSIBLE* pWfx, BYTE** lpDataBuffer, DWORD* pdwChunkSize)
{
	ASSERT(strFileName);
	ASSERT(pWfx);
	ASSERT(lpDataBuffer);
	ASSERT(pdwChunkSize);

	HRESULT hr = S_OK;

	// Open the file
	HANDLE hFile = CreateFile(
		strFileName,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return HRESULT_FROM_WIN32(GetLastError());

	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());

	DWORD dwChunkSize;
	DWORD dwChunkPosition;
	//check the file type, should be fourccWAVE or 'XWMA'
	FindChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
	DWORD filetype;
	ReadChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);
	if (filetype != fourccWAVE)
		return S_FALSE;

	WAVEFORMATEXTENSIBLE wfx = { 0 };

	FindChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
	ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);

	//fill out the audio data buffer with the contents of the fourccDATA chunk
	FindChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
	BYTE* pDataBuffer = new BYTE[dwChunkSize];
	ReadChunkData(hFile, pDataBuffer, dwChunkSize, dwChunkPosition);

	*pWfx = wfx;
	*lpDataBuffer = pDataBuffer;
	*pdwChunkSize = dwChunkSize;

	return hr;
}

HRESULT XAudioCreateSourceVoiceFromMemory(IXAudio2* pXAudio2, DWORD dwChunkSize, BYTE* pDataBuffer, WAVEFORMATEXTENSIBLE wfx, IXAudio2SourceVoice** lpSourceVoice, XAUDIO2_BUFFER* pBuffer)
{
	ASSERT(pXAudio2);
	ASSERT(lpSourceVoice);

	HRESULT hr;

	XAUDIO2_BUFFER buffer = { 0 };

	buffer.AudioBytes = dwChunkSize;  //size of the audio buffer in bytes
	buffer.pAudioData = pDataBuffer;  //buffer containing audio data
	buffer.Flags = XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer

	IXAudio2SourceVoice* pSourceVoice = NULL;
	if (FAILED(hr = pXAudio2->CreateSourceVoice(&pSourceVoice, (WAVEFORMATEX*)&wfx)))
		return hr;

	*pBuffer = buffer;
	*lpSourceVoice = pSourceVoice;

	return hr;
}

//.text:00667560 ; unreferenced
//.text:00667570 ; void __cdecl platform_sound_begin_frame(real)
//.text:00667580 ; void __cdecl platform_sound_build_global_playback_submit_parameters(long, s_platform_sound_effect_input_parameters const*, s_platform_sound_global_effect_submit_parameters*)
//.text:00667590 ; void __cdecl platform_sound_build_low_frequency_output(long, long, s_platform_sound_low_frequency_output_definition*)
//.text:006675A0 ; void __cdecl platform_sound_build_playback_submit_parameters(long, long, s_platform_sound_effect_input_parameters const*, s_platform_sound_playback_submit_parameters*)
//.text:006675B0 ; unreferenced
//.text:006675C0 ; unreferenced
//.text:006675E0 ; void __cdecl platform_sound_codec_set_memory_size(bool)
//.text:006675F0 ; unreferenced
//.text:00667600 ; referenced, called from `sound_definition_hardware_formats_match`

void __cdecl platform_sound_dispose()
{
	//INVOKE(0x00667640, platform_sound_dispose);

	//g_xaudio_voice_manager.dispose();
	//g_xaudio_source_manager.dispose();
	//g_xaudio_reverb_manager.dispose();
	//XAudioShutDown();
	//shell_sound_memory_dispose();

	XAudioDispose(g_pXAudio2, g_pMasterVoice, g_pSourceVoice);
}

//.text:00667650 ; bool __cdecl platform_sound_enabled()
//.text:00667660 ; void __cdecl platform_sound_end_frame()
//.text:00667670 ; called from `sound_render_dispatch` used in `sound_render` and `audio_thread_loop`
//.text:00667680 ; void __cdecl platform_sound_flush()
//.text:00667690 ; void __cdecl platform_sound_flush_events()
//.text:006676A0 ; void __cdecl platform_sound_get_status(s_platform_sound_status*)

//bool __cdecl platform_sound_initialize(s_sound_preferences const*)
bool __cdecl platform_sound_initialize(void const* preferences)
{
	//INVOKE(0x00667640, platform_sound_dispose, preferences);

	//shell_sound_memory_initialize();

	// #TODO: use `X3DAudioInitialize`

	HRESULT hr = XAudioInitialize(&g_pXAudio2);
	ASSERT(SUCCEEDED(hr));
	if (SUCCEEDED(hr))
	{
		//g_xaudio_dsp_manager.initialize(); // CreateSubmixVoice
		//g_xaudio_reverb_manager.initialize(); // CreateSubmixVoice
		//g_xaudio_output_manager.initialize(&g_xaudio_reverb_manager, &g_xaudio_dsp_manager);
		//g_xaudio_source_manager.initialize(&g_xaudio_dsp_manager, preferences, &g_xaudio_output_manager, platform_sound_xaudio_pc_source_callback); // CreateSourceVoice
		//g_xaudio_voice_manager.initialize(&g_xaudio_source_manager. preferences->something); // CreateMasteringVoice?

		hr = XAudioCreateMasteringVoice(g_pXAudio2, &g_pMasterVoice);
		ASSERT(SUCCEEDED(hr));

		s_file_reference file{};
		if (file_exists(file_reference_create_from_path_wide(&file, SOUND_QUEUE_FILE, false)))
		{
			WAVEFORMATEXTENSIBLE wfx = { 0 };
			BYTE* pDataBuffer = NULL;
			DWORD dwChunkSize = 0;
			hr = XAudioReadBufferFromFile(SOUND_QUEUE_FILE, &wfx, &pDataBuffer, &dwChunkSize);
			ASSERT(SUCCEEDED(hr));

			hr = XAudioCreateSourceVoiceFromMemory(g_pXAudio2, dwChunkSize, pDataBuffer, wfx, &g_pSourceVoice, &g_buffer);
			ASSERT(SUCCEEDED(hr));
		}
	}
	//pending_audio_queue.initialize(392);

	return true;
}

//.text:00667780 ; void __cdecl platform_sound_listener_set_properties(platform_sound_listener_properties const*)
//.text:00667790 ; void __cdecl platform_sound_pause()
//.text:006677A0 ; void __cdecl platform_sound_postprocess_sound_impulse_definition(long, s_sound_impulse_definition*)
//.text:006677B0 ; void __cdecl platform_sound_reset()
//.text:006677C0 ; void __cdecl platform_sound_set_global_doppler_factor(real)
//.text:006677D0 ; void __cdecl platform_sound_set_global_environment_parameters(s_sound_global_environment_parameters const*)
//.text:006677E0 ; void __cdecl platform_sound_set_global_mixbin_headroom(long, long)
//.text:006677F0 ; void __cdecl platform_sound_set_global_mixes(s_sound_surround_mix_definition const*, s_sound_surround_mix_definition const*, s_sound_global_mix_configuration const*)
//.text:00667800 ; void __cdecl platform_sound_set_global_playback_parameters(s_platform_sound_global_effect_submit_parameters const*)
//.text:00667810 ; void __cdecl platform_sound_set_low_frequency_effect_input(s_platform_sound_low_frequency_output_definition const*)
//.text:00667820 ; unreferenced
//.text:00667830 ; bool __cdecl platform_sound_valid_sound_definition_pc(s_cache_file_sound_definition const*)
//.text:00667840 ; real __cdecl platform_sound_source_get_bend(long, e_sound_sample_rate)
//.text:00667850 ; short __cdecl platform_sound_source_get_state(long)
//.text:00667860 ; void __cdecl platform_sound_source_initialize_properties(long, s_platform_sound_source_properties const*, s_platform_sound_effect_properties const*)
//.text:00667890 ; unreferenced, calls `.text:00656110`
//.text:006678A0 ; void __cdecl platform_sound_source_pause(long)
//.text:006678B0 ; void __cdecl platform_sound_source_prepare_for_unpause(long)

//void __cdecl platform_sound_source_queue_sound(long, s_sound_permutation_chunk const*)
void __cdecl platform_sound_source_queue_sound(long source_index, void const* sound)
{
	//INVOKE(0x006678C0, platform_sound_source_queue_sound, source_index, sound);

	//g_xaudio_source_manager.source_queue_sound(source_index, sound);

	if (g_pSourceVoice == NULL)
		return;

	long sound_definition_index = *(long*)sound;
	if (sound_definition_index == NONE)
		return;

	char const* sound_name = tag_get_name(sound_definition_index);
	if (sound_name == NULL)
		return;

	char const* sound_path = R"(sound\dialog\combat\)";
	if (csmemcmp(sound_path, sound_name, csstrnlen(sound_path, 256)) == 0)
	{
		HRESULT hr = S_OK;

		if (FAILED(hr = g_pSourceVoice->FlushSourceBuffers()))
			printf("");

		if (FAILED(hr = g_pSourceVoice->SubmitSourceBuffer(&g_buffer)))
			printf("");

		if (FAILED(hr = g_pSourceVoice->SetVolume(0.05f)))
			printf("");

		if (FAILED(hr = g_pSourceVoice->Start(0)))
			printf("");
	}
}

//.text:006678D0 ; void __cdecl platform_sound_source_set_initial_sample_offset(long, long)
//.text:006678E0 ; void __cdecl platform_sound_source_signal_complete(long)
//.text:006678F0 ; void __cdecl platform_sound_source_stop(long)
//.text:00667900 ; void __cdecl platform_sound_source_unpause(long)
//.text:00667910 ; void __cdecl platform_sound_source_update_properties(long, s_platform_sound_source_properties const*, s_platform_sound_playback_submit_parameters const*, bool)
//.text:00667930 ; unreferenced
//.text:00667940 ; unreferenced
//.text:00667950 ; void __cdecl platform_sound_synch_playback(long)
//.text:00667960 ; void __cdecl platform_sound_unpause()
//.text:00667970 ; bool __cdecl platform_sound_valid_sound_definition(s_cache_file_sound_definition const*)
//.text:00667980 ; bool __cdecl platform_sound_should_reimport_sound_definition_pc(s_cache_file_sound_definition const*)
//.text:00667990 ; void __cdecl platform_sound_voice_initialize_properties(long)
//.text:006679A0 ; bool __cdecl platform_sound_voice_is_idle_async(long)
//.text:006679B0 ; unreferenced
//.text:006679C0 ; void __cdecl platform_sound_voice_update_properties(long, s_platform_sound_voice_update_definition const*)
//.text:006679E0 ; bool __cdecl platform_sound_valid_sound_definition_xenon(s_cache_file_sound_definition const*)
//.text:00667A30 ; bool __cdecl platform_sound_should_reimport_sound_definition_xenon(s_cache_file_sound_definition const*)

