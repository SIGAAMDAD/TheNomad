#ifndef __SIRENGINE_VULKAN_CONTEXT_H__
#define __SIRENGINE_VULKAN_CONTEXT_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include "../RenderContext.h"
#include "VulkanCommon.h"

namespace SIREngine::RenderLib::Vulkan {

class VKContext : public IRenderContext
{
public:
	VKContext( const ContextInfo_t& contextInfo );

	VkDevice GetDevice( void );
	VkPhysicalDevice GetPhysicalDevice( void );
	VkSurfaceKHR GetSurface( void );

	static VkAllocationCallbacks AllocationCallbacks;
private:
	virtual bool CreateWindow( void ) override;
	virtual void ShutdownBackend( void ) override;
	
	void InitSwapChain( void );
	void InitPhysicalDevice( void );
	void InitLogicalDevice( void );
	void InitRenderPass( void );
	void InitCommandBuffer( void );
	void CreateFixedFunctionPipeline( void );

	void RecordCommandBuffer( VkCommandBuffer hCommandBuffer, uint32_t nImageIndex );

	VkDebugUtilsMessengerEXT m_hDebugHandler;

	VmaAllocator m_hAllocator;

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

	eastl::vector<VkImage> m_SwapChainImages;
	eastl::vector<VkImageView> m_SwapChainImageViews;
	eastl::vector<VkFramebuffer> m_SwapChainFramebuffers;
	eastl::vector<VmaAllocation> m_SwapChainImageAllocations;
	VkSwapchainKHR m_hSwapChain;
	VkFormat m_nSwapChainFormat;
	VkExtent2D m_nSwapChainExtent;

	VkQueue m_hGraphicsQueue;
	VkQueue m_hPresentQueue;
};

extern VKContext *g_pVKContext;

SIRENGINE_FORCEINLINE VkDevice VKContext::GetDevice( void )
{
	return m_hDevice;
}

SIRENGINE_FORCEINLINE VkPhysicalDevice VKContext::GetPhysicalDevice( void )
{
	return m_hPhysicalDevice;
}

SIRENGINE_FORCEINLINE VkSurfaceKHR VKContext::GetSurface( void )
{
	return m_hSurface;
}

SIRENGINE_DECLARE_LOG_CATEGORY( VulkanBackend, ELogLevel::Info );

};

#endif