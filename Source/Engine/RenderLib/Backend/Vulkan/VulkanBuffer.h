#ifndef __SIRENGINE_VULKAN_BUFFER_H__
#define __SIRENGINE_VULKAN_BUFFER_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include <Engine/Core/Compiler.h>
#include "VulkanContext.h"

namespace SIREngine::RenderLib::Vulkan {

class VKBuffer
{
public:
	VKBuffer( void );
	~VKBuffer();

	void Allocate( const void *pBuffer, size_t nSize, EBufferType nType );
	void Release( void );
	void Bind( void );
private:
	VmaAllocation m_Allocation;
	VkBuffer m_hBuffer;
};

};

#endif