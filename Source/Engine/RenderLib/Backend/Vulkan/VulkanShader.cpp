#include "VulkanShader.h"
#include "VulkanContext.h"
#include <Engine/Core/FileSystem/CachedFile.h>

namespace SIREngine::RenderLib::Vulkan {

VKShader::VKShader( void )
{
}

VKShader::~VKShader()
{
	if ( m_hVertexShader ) {
		vkDestroyShaderModule( g_pVKContext->GetDevice(), m_hVertexShader, NULL );
	}
	if ( m_hFragmentShader ) {
		vkDestroyShaderModule( g_pVKContext->GetDevice(), m_hFragmentShader, NULL );
	}
}

bool VKShader::Compile( const CFilePath& name )
{
	{
		const CFilePath path = CFilePath( "Shaders/Vulkan/" + name + "_vp.spv" );
		CCachedFile file( path );

		if ( !file.GetSize() || !file.GetBuffer() ) {
			SIRENGINE_LOG_LEVEL( VulkanBackend, ELogLevel::Warning, "Error loading shader file \"%s\"", path.c_str() );
			return false;
		}

		VkShaderModuleCreateInfo createInfo;
		memset( &createInfo, 0, sizeof( createInfo ) );
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = file.GetSize();
		createInfo.pCode = (const uint32_t *)file.GetBuffer();

		VK_CALL( vkCreateShaderModule( g_pVKContext->GetDevice(), &createInfo, NULL, &m_hVertexShader ) );
	}
	{
		const CFilePath path = CFilePath( "Shaders/Vulkan/" + name + "_fp.spv" );
		CCachedFile file( path );

		if ( !file.GetSize() || !file.GetBuffer() ) {
			SIRENGINE_LOG_LEVEL( VulkanBackend, ELogLevel::Warning, "Error loading shader file \"%s\"", path.c_str() );
			return false;
		}

		VkShaderModuleCreateInfo createInfo;
		memset( &createInfo, 0, sizeof( createInfo ) );
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = file.GetSize();
		createInfo.pCode = (const uint32_t *)file.GetBuffer();

		VK_CALL( vkCreateShaderModule( g_pVKContext->GetDevice(), &createInfo, NULL, &m_hFragmentShader ) );
	}

	return true;
}

};