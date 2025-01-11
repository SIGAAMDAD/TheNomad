#ifndef __SIRENGINE_FILE_PATH_H__
#define __SIRENGINE_FILE_PATH_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include "Pch.h"

namespace SIREngine {

typedef eastl::fixed_string<char, MAX_OSPATH> CFilePath;

};

#endif