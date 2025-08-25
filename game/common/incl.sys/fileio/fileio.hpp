#pragma once

#include "ds/ds_flags.hpp"

struct osTIMER
{
	int start;
	int time;
};
static_assert(sizeof(osTIMER) == 0x8);

enum FIO_OPEN
{
	// $TODO confirm these values

	FIO_OPEN_R = 1 << 0,
	FIO_OPEN_W = 1 << 1,
	FIO_OPEN_A = 1 << 2,
	FIO_OPEN_CHUNK = 1 << 3,
	FIO_OPEN_NO_OFFSET_END = 1 << 4,
	FIO_OPEN_BYTE_REORDER = 1 << 5,
	FIO_ERROR = 1 << 6,
	FIO_ERROR_EOF = 1 << 7,
	FIO_OPEN_ALIGN_SECTOR = 1 << 8,
	FIO_OPEN_USER = 1 << 9,
};

struct osHANDLE_DUMMY
{
};

struct osFILE_HANDLE_DUMMY
{
};

struct osASYNC_FILE_HANDLE_DUMMY
{
};

struct osASYNC_INFO
{
	int isEOF;
	osHANDLE_DUMMY* signal;
	int isSignaled;
	int mustRetry;
	int holdSignal;
	byte data[0x4A];
};
static_assert(sizeof(osASYNC_INFO) == 0x60);

struct fioCHUNK_DESCR;
struct fioFILE_BUFFER;
struct fioFILE
{
	struct
	{
		void(__thiscall* Dtor)(fioFILE*);
		int(__thiscall* IsReadyToWrite)(fioFILE*, int);
		int(__thiscall* Term)(fioFILE*);
		int(__thiscall* IsEOF)(fioFILE*);
		void(__thiscall* SetBuffer)(fioFILE*);
		void* Unknown1;
		const char* (__thiscall* GetStreamName)(fioFILE*);
		void* Unknown1C;
		void* Unknown20;
		void* Unknown24;
		void* Unknown28;
		void* Unknown2C;
		void(__thiscall* DoSeek)(fioFILE*, unsigned int);
		void(__thiscall* DoReadData)(fioFILE*, void*, int);
		void(__thiscall* DoWriteData)(fioFILE*, const void*, int);
		void(__thiscall* DoTruncate)(fioFILE*);
	}*__vftable /*VFT*/;

	unsigned int offset;
	unsigned int streamSize;
	int nDescr;
	int nDescrAlloc;
	fioCHUNK_DESCR* descrList;
	fioFILE_BUFFER* pBuffer;
	int random_seed;
	dsFLAGS<FIO_OPEN, int> state;
};
static_assert(sizeof(fioFILE) == 0x24);

struct fioFILE_DISK :
	public fioFILE
{
	wchar_t fileName[260];
	char StreamName[260];
	osFILE_HANDLE_DUMMY* fp;
	int attrWasChanged;
	unsigned int dwFileAttributes;
	char* buffer;
	int bufSize;
	int bufSizeMax;
};
static_assert(sizeof(fioFILE_DISK) == 0x348);

struct fioFILE_MEM :
	public fioFILE
{
	unsigned char* pBuf;
	unsigned char* curPtr;
	unsigned int allocSize;
	int isSelfManaged;
	int predictAllocSize;
	osASYNC_FILE_HANDLE_DUMMY* fpAsync;
	osASYNC_INFO fpAsyncInfo;
};
static_assert(sizeof(fioFILE_MEM) == 0x9C);
