#include "sound/sound_xaudio2.hpp"

#include "cache/cache_files.hpp"
#include "cseries/cseries.hpp"
#include "memory/module.hpp"

#include <XAudio2.h>
#pragma comment(lib, "X3DAudio.lib")

HOOK_DECLARE(0x00656170, platform_sound_source_queue_sound);
HOOK_DECLARE(0x00667640, platform_sound_dispose);
HOOK_DECLARE(0x00667770, platform_sound_initialize);

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
	if (FAILED(hr = XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR)))
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

void __cdecl platform_sound_source_queue_sound(long a1, void const* a2)
{
	long sound_definition_index = *(long*)a2;
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

void __cdecl platform_sound_dispose()
{
	XAudioDispose(g_pXAudio2, g_pMasterVoice, g_pSourceVoice);
}

bool __cdecl platform_sound_initialize(void* preferences)
{
	HRESULT hr = S_OK;
	if (FAILED(hr = XAudioInitialize(&g_pXAudio2)))
		return false;

	if (FAILED(hr = XAudioCreateMasteringVoice(g_pXAudio2, &g_pMasterVoice)))
		return false;

	s_file_reference file{};
	if (file_exists(file_reference_create_from_path_wide(&file, SOUND_QUEUE_FILE, false)))
	{
		WAVEFORMATEXTENSIBLE wfx = { 0 };
		BYTE* pDataBuffer = NULL;
		DWORD dwChunkSize = 0;
		if (FAILED(hr = XAudioReadBufferFromFile(SOUND_QUEUE_FILE, &wfx, &pDataBuffer, &dwChunkSize)))
			return false;

		if (FAILED(hr = XAudioCreateSourceVoiceFromMemory(g_pXAudio2, dwChunkSize, pDataBuffer, wfx, &g_pSourceVoice, &g_buffer)))
			return false;
	}

	return true;
}

