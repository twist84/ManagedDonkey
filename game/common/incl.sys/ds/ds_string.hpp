#pragma once

template<typename T>
struct dsTSTRING_BUF_HEADER
{
	int refCount;
	int strLen;
	int maxStrLen;
	T str[1];
};

template<typename T>
struct dsTSTRING
{
	dsTSTRING_BUF_HEADER<T>* pBuffer;
};

