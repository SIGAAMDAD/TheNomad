#ifndef __SIRENGINE_FILE_PATH_H__
#define __SIRENGINE_FILE_PATH_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include <Engine/Core/Pch.h>
#include <Engine/Core/Compiler.h>
#include <Engine/Core/Types.h>

namespace SIREngine {

typedef eastl::fixed_string<char, SIRENGINE_MAX_OSPATH> CFilePath;

SIRENGINE_FORCEINLINE CString GetExtension( const CFilePath& path ) {
	const char *pString = strrchr( path.data(), '.' );
	if ( !pString ) {
		return "";
	}
	return pString + 1;
}

};

namespace eastl {
	template<> struct hash<SIREngine::CFilePath> {
		size_t operator()( const SIREngine::CFilePath& str ) const {
			const unsigned char *p = (const unsigned char *)str.c_str(); // To consider: limit p to at most 256 chars.
			unsigned int c, result = 2166136261U; // We implement an FNV-like string hash.
			while((c = *p++) != 0) // Using '!=' disables compiler warnings.
				result = (result * 16777619) ^ c;
			return (size_t)result;
		}
	};
};

#endif