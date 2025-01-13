#ifndef __SIRENGINE_VULKAN_SHADER_H__
#define __SIRENGINE_VULKAN_SHADER_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include <Engine/Core/Types.h>
#include <Engine/Core/FileSystem/FilePath.h>
#include "VulkanCommon.h"

namespace SIREngine::RenderLib::Vulkan {

class VKShader
{
public:
	VKShader( void );
	~VKShader();

	bool Compile( const CFilePath& name );

	VkShaderModule GetVertexModule( void );
	VkShaderModule GetFragmentModule( void );
private:
	VkShaderModule m_hVertexShader;
	VkShaderModule m_hFragmentShader;
};

SIRENGINE_FORCEINLINE VkShaderModule VKShader::GetVertexModule( void )
{
	return m_hVertexShader;
}

SIRENGINE_FORCEINLINE VkShaderModule VKShader::GetFragmentModule( void )
{
	return m_hFragmentShader;
}

};

#endif