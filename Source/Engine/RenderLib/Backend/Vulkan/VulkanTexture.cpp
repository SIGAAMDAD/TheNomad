#include "VulkanTexture.h"
#include "VulkanContext.h"
#include "../ImageLoader.h"

namespace SIREngine::RenderLib::Vulkan {

VKTexture::VKTexture( void )
{
}

VKTexture::~VKTexture()
{
	if ( m_hAllocation ) {
		vmaDestroyImage( g_pVKContext->GetAllocator(), m_hImage, m_hAllocation );
	}
}

bool VKTexture::Load( const CFilePath& path )
{
	CImageLoader loader( path );

	VkImageCreateInfo createInfo;
	memset( &createInfo, 0, sizeof( createInfo ) );
	createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	createInfo.imageType = VK_IMAGE_TYPE_2D;
	createInfo.extent.width = loader.GetWidth();
	createInfo.extent.height = loader.GetHeight();
	createInfo.extent.depth = 1;
	createInfo.mipLevels = 1;
	createInfo.arrayLayers = 1;
	createInfo.format = VkFormat( loader.GetFormat() );
	createInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	createInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	createInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
	createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	createInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	createInfo.flags = 0;

	VmaAllocationCreateInfo allocCreateInfo;
	memset( &allocCreateInfo, 0, sizeof( allocCreateInfo ) );
	allocCreateInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
	allocCreateInfo.requiredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

	VmaAllocationInfo allocInfo;
	VK_CALL( vmaCreateImage( g_pVKContext->GetAllocator(), &createInfo, &allocCreateInfo, &m_hImage, &m_hAllocation, &allocInfo ) );

	return true;
}

};