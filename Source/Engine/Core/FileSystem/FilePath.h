#ifndef __SIRENGINE_FILE_PATH_H__
#define __SIRENGINE_FILE_PATH_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include <Engine/Core/Pch.h>
#include <Engine/Core/Compiler.h>

namespace SIREngine {

typedef eastl::fixed_string<char, SIRENGINE_MAX_OSPATH> CFilePath;

};

#endif