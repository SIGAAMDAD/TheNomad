#ifndef __SIRENGINE_VULKAN_TEXTURE_H__
#define __SIRENGINE_VULKAN_TEXTURE_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include <Engine/Core/FileSystem/FilePath.h>
#include "VulkanCommon.h"

namespace SIREngine::RenderLib::Vulkan {

class VKTexture
{
public:
	VKTexture( void );
	~VKTexture();

	bool Load( const CFilePath& path );
private:
	VkImage m_hImage;
	VmaAllocation m_hAllocation;
};

};

#endif