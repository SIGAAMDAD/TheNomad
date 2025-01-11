#ifndef __SIRENGINE_TYPES_H__
#define __SIRENGINE_TYPES_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
    #pragma once
#endif

#include "Pch.h"

typedef uint32_t bool32;
typedef uint8_t byte;

// cacheline aligned string
typedef eastl::fixed_string<char, 24, true> CString;

#endif