// Copyright © 2013 CCP ehf.
//
// This file provides the CW2A and CA2W macros. On Windows we simply pull in
// the ATL macros - on other platforms we provide our own implemented with
// wcsrtombs and mbsrtowcs.
//

#pragma once

#include <string>
#include "carbon_core_export.h"

CARBON_CORE_API std::wstring UTF8ToWide( const char* utf8String );
CARBON_CORE_API std::wstring UTF8ToWide( const std::string& utf8String );
CARBON_CORE_API std::string WideToUTF8( const wchar_t* wideString );
CARBON_CORE_API std::string WideToUTF8( const std::wstring& wideString );

#if _WIN32
#if __has_include(<atlbase.h>)
#include <atlbase.h>
#else
// Build Tools installs can omit ATL. These match the two-argument ATL
// converters this library calls, using the Win32 code-page APIs.
#include <windows.h>

class CW2A
{
public:
	explicit CW2A( const wchar_t* src, unsigned codePage = CP_ACP )
		: heap_( nullptr )
		, converted_( stack_ )
	{
		Convert( src, codePage );
	}

	~CW2A()
	{
		delete[] heap_;
	}

	operator const char*() const
	{
		return converted_;
	}

private:
	void Convert( const wchar_t* src, unsigned codePage )
	{
		stack_[0] = 0;
		if( src == nullptr )
		{
			return;
		}
		const int length = WideCharToMultiByte( codePage, 0, src, -1, nullptr, 0, nullptr, nullptr );
		if( length <= 0 )
		{
			return;
		}
		if( length > BUFFER_SIZE )
		{
			heap_ = new char[length];
			converted_ = heap_;
		}
		WideCharToMultiByte( codePage, 0, src, -1, converted_, length, nullptr, nullptr );
	}

	CW2A( const CW2A& ) = delete;
	CW2A& operator=( const CW2A& ) = delete;

	static const int BUFFER_SIZE = 128;
	char stack_[BUFFER_SIZE];
	char* heap_;
	char* converted_;
};

class CA2W
{
public:
	explicit CA2W( const char* src, unsigned codePage = CP_ACP )
		: heap_( nullptr )
		, converted_( stack_ )
	{
		Convert( src, codePage );
	}

	~CA2W()
	{
		delete[] heap_;
	}

	operator const wchar_t*() const
	{
		return converted_;
	}

private:
	void Convert( const char* src, unsigned codePage )
	{
		stack_[0] = 0;
		if( src == nullptr )
		{
			return;
		}
		const int length = MultiByteToWideChar( codePage, 0, src, -1, nullptr, 0 );
		if( length <= 0 )
		{
			return;
		}
		if( length > BUFFER_SIZE )
		{
			heap_ = new wchar_t[length];
			converted_ = heap_;
		}
		MultiByteToWideChar( codePage, 0, src, -1, converted_, length );
	}

	CA2W( const CA2W& ) = delete;
	CA2W& operator=( const CA2W& ) = delete;

	static const int BUFFER_SIZE = 128;
	wchar_t stack_[BUFFER_SIZE];
	wchar_t* heap_;
	wchar_t* converted_;
};
#endif
#else

#include <stdint.h>

class CARBON_CORE_API BlueConvertWideToAscii
{
public:
	BlueConvertWideToAscii( const wchar_t* src );
	~BlueConvertWideToAscii();

	operator const char*() const
	{
		return m_converted;
	}

private:
	void Init( const wchar_t* src );

private:
	static const uint32_t BUFFER_SIZE = 128;
	char* m_converted;
	char m_buffer[BUFFER_SIZE];
};


class CARBON_CORE_API BlueConvertAsciiToWide
{
public:
	BlueConvertAsciiToWide( const char* src );
	~BlueConvertAsciiToWide();

	operator const wchar_t*() const
	{
		return m_converted;
	}

private:
	void Init( const char* src );

private:
	static const uint32_t BUFFER_SIZE = 128;
	wchar_t* m_converted;
	wchar_t m_buffer[BUFFER_SIZE];
};

typedef BlueConvertWideToAscii CW2A;
typedef BlueConvertAsciiToWide CA2W;

#endif
