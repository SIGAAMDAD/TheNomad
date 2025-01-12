#ifndef __SIRENGINE_VULKAN_CONTEXT_H__
#define __SIRENGINE_VULKAN_CONTEXT_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include "../RenderContext.h"
#include <SDL2/SDL_vulkan.h>
#include <vulkan/vulkan.hpp>
#include "VulkanCommon.h"

namespace SIREngine::RenderLib::Vulkan {

class VKContext : public IRenderContext
{
public:
	VKContext( const ContextInfo_t& contextInfo );
private:
	virtual bool CreateWindow( void ) override;
	virtual void ShutdownBackend( void ) override;

	void InitPhysicalDevice( void );

	VkInstance m_hInstance;
	VkSurfaceKHR m_hSurface;
	VkDevice m_hDevice;
	VkPhysicalDevice m_hPhysicalDevice;
	VkPipeline m_hPipeline;
	VkPipelineLayout m_hPipelineLayout;
	VkRenderPass m_hRenderPass;
	VkCommandPool m_hCommandPool;
	VkCommandBuffer m_hCommandBuffers[ VK_MAX_FRAMES_IN_FLIGHT ];
	VkFence m_hInFlightFences[ VK_MAX_FRAMES_IN_FLIGHT ];
};

SIRENGINE_DECLARE_LOG_CATEGORY( VulkanBackend, ELogLevel::Info );

};

#endif