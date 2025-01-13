#ifndef __SIRENGINE_VULKAN_COMMON_H__
#define __SIRENGINE_VULKAN_COMMON_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include <Engine/Core/Compiler.h>
#include "vk_mem_alloc.h"
#include <SDL2/SDL_vulkan.h>
#include <vulkan/vulkan.hpp>

#define VK_MAX_FRAMES_IN_FLIGHT 2

extern void VkError( const char *pCall, VkResult nResult );

SIRENGINE_FORCEINLINE void VkCall( const char *pCall, VkResult nResult )
{
	if ( nResult != VK_SUCCESS ) {
		VkError( pCall, nResult );
	}
}

#define VK_CALL( call ) VkCall( #call, call )

#endif