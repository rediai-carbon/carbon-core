#include <cstdio>
#include <string>

// Exercise the Win32 fallback even when the compiler ships ATL headers.
#define CARBON_CORE_FORCE_WIN32_STRING_CONVERSION 1
#include "StringConversions.h"

namespace
{
bool Expect( bool cond, const char* step )
{
	std::printf( "%s %s\n", step, cond ? "OK" : "FAIL" );
	return cond;
}
}

int main()
{
	std::printf( "CORE_CONVERSION_SMOKE_START\n" );
	std::wstring wide = L"ascii-roundtrip";
	CW2A narrowUtf8( wide.c_str(), CP_UTF8 );
	if( !Expect( narrowUtf8 && narrowUtf8[0] != '\0', "CW2A_utf8" ) )
		return 1;

	CW2A narrowUnicode( L"\x0442\x0435", CP_UTF8 );
	if( !Expect( narrowUnicode && narrowUnicode[0] != '\0', "CW2A_unicode" ) )
		return 1;

	CW2A empty( L"" );
	if( !Expect( empty && empty[0] == '\0', "CW2A_empty" ) )
		return 1;

	{
		CA2W back( narrowUtf8 );
		if( !Expect( back && std::wstring( back ) == wide, "CA2W_roundtrip" ) )
			return 1;
	}

	CW2A invalidCp( wide.c_str(), static_cast<UINT>( 99999 ) );
	if( !Expect( invalidCp[0] == '\0', "CW2A_invalid_cp" ) )
		return 1;

	std::printf( "CORE_CONVERSION_SMOKE_OK\n" );
	return 0;
}