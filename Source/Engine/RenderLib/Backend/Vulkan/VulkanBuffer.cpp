#include "VulkanBuffer.h"

namespace SIREngine::RenderLib::Vulkan {

VKBuffer::VKBuffer( void )
{
}

VKBuffer::~VKBuffer()
{
	Release();
}

void VKBuffer::Release( void )
{
	if ( m_Allocation ) {
		vmaDestroyBuffer( g_pVKContext->GetAllocator(), m_hBuffer, m_Allocation );
	}
}

void VKBuffer::Allocate( const void *pBuffer, size_t nSize, EBufferType nType )
{
	VkBufferUsageFlags nUsage;
	VmaAllocationInfo allocInfo;

	switch ( nType ) {
	case EBufferType::Vertex:	
		nUsage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		break;
	case EBufferType::Index:
		nUsage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		break;
	case EBufferType::ShaderStorage:
		nUsage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
		break;
	case EBufferType::Uniform:
		nUsage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		break;
	};

	VkBufferCreateInfo createInfo;
	memset( &createInfo, 0, sizeof( createInfo ) );
	createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	createInfo.usage = nUsage;
	createInfo.size = nSize;
	createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VmaAllocationCreateInfo allocCreateInfo;
	memset( &allocCreateInfo, 0, sizeof( allocCreateInfo ) );
	allocCreateInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
	allocCreateInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | ( pBuffer ? VMA_ALLOCATION_CREATE_MAPPED_BIT : 0 );

	VK_CALL( vmaCreateBuffer( g_pVKContext->GetAllocator(), &createInfo, &allocCreateInfo, &m_hBuffer, &m_Allocation, &allocInfo ) );

	if ( pBuffer ) {
		memcpy( allocInfo.pMappedData, pBuffer, nSize );
	}
}

void VKBuffer::Bind( void )
{
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers( g_pVKContext->GetCommandBuffer(), 0, 1, &m_hBuffer, offsets );
}

};