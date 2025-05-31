#pragma once

#include <windows.h>

// ==============================================================================================

// from `https://github.com/pnill/cartographer/blob/f280be10d04975fd48dda115c2eab3a239ffca4d/xlive/xliveless.cpp#L69-L111`

// Xbox-specific Overlapped

typedef struct _XOVERLAPPED             XOVERLAPPED, * PXOVERLAPPED;

typedef
VOID
(WINAPI* PXOVERLAPPED_COMPLETION_ROUTINE)(
	DWORD dwErrorCode,
	DWORD dwNumberOfBytesTransfered,
	DWORD pOverlapped
	);


typedef struct _XOVERLAPPED {
	ULONG_PTR                           InternalLow;
	ULONG_PTR                           InternalHigh;
	ULONG_PTR                           InternalContext;
	HANDLE                              hEvent;
	PXOVERLAPPED_COMPLETION_ROUTINE     pCompletionRoutine;
	DWORD_PTR                           dwCompletionContext;
	DWORD                               dwExtendedError;
} XOVERLAPPED, * PXOVERLAPPED;

extern DWORD WINAPI XGetOverlappedExtendedError(PXOVERLAPPED pOverlapped);
extern DWORD WINAPI XGetOverlappedResult(PXOVERLAPPED pOverlapped, LPDWORD pResult, BOOL bWait);

// ==============================================================================================

extern bool __cdecl get_clipboard_as_text(char* buf, int32 len);
extern bool __cdecl set_clipboard_as_text(char* buf, int32 len);

extern int32 XShowKeyboardUI(
	int32 controller_index,
	uns32 character_flags,
	const wchar_t* default_text,
	const wchar_t* title_text,
	const wchar_t* description_text,
	wchar_t* result_text,
	uns32 maximum_character_count,
	void* overlapped
);

extern void display_notification(const wchar_t* tip = L"Notification Tip", const wchar_t* info = L"Notification Message", const wchar_t* info_title = L"Notification Title");

