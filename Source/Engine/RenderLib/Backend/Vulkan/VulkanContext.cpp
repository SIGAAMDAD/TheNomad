#include "../RenderContext.h"
#include "VulkanContext.h"
#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>
#include <Engine/Core/Util.h>

PFN_vkCreateSwapchainKHR fn_vkCreateSwapchainKHR = NULL;
PFN_vkCmdPushDescriptorSetKHR fn_vkCmdPushDescriptorSetKHR = NULL;
PFN_vkCreateDescriptorUpdateTemplateKHR fn_vkCreateDescriptorUpdateTemplateKHR = NULL;

static PFN_vkDestroyDebugUtilsMessengerEXT fn_vkDestroyDebugUtilsMessengerEXT = NULL;

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void *pUserData
)
{
	using namespace SIREngine::RenderLib::Vulkan;
	if ( messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT || messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT ) {
		SIRENGINE_LOG_LEVEL( VulkanBackend, ELogLevel::Warning, "[VULKAN DEBUG] %s", pCallbackData->pMessage );
	} else if ( messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT ) {
		SIRENGINE_LOG_LEVEL( VulkanBackend, ELogLevel::Developer, "[VULKAN DEBUG] %s", pCallbackData->pMessage );
	}
	switch ( messageType ) {
	case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
		break;
	case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
		break;
	case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
		break;
	};

	return VK_FALSE;
}

void VkError( const char *pCall, VkResult nResult )
{
	typedef struct VkErrorString {
		const char *pString;
		VkResult nError;
	} VkErrorString;

	#define VK_ERROR( x ) { SIRENGINE_XSTRING( x ), x }
	const VkErrorString szErrorStrings[] = {
		VK_ERROR( VK_SUCCESS ),
		VK_ERROR( VK_NOT_READY ),
		VK_ERROR( VK_TIMEOUT ),
		VK_ERROR( VK_EVENT_SET ),
		VK_ERROR( VK_EVENT_RESET ),
		VK_ERROR( VK_INCOMPLETE ),
		VK_ERROR( VK_ERROR_OUT_OF_HOST_MEMORY ),
		VK_ERROR( VK_ERROR_OUT_OF_DEVICE_MEMORY ),
		VK_ERROR( VK_ERROR_INITIALIZATION_FAILED ),
		VK_ERROR( VK_ERROR_DEVICE_LOST ),
		VK_ERROR( VK_ERROR_MEMORY_MAP_FAILED ),
		VK_ERROR( VK_ERROR_LAYER_NOT_PRESENT ),
		VK_ERROR( VK_ERROR_EXTENSION_NOT_PRESENT ),
		VK_ERROR( VK_ERROR_FEATURE_NOT_PRESENT ),
		VK_ERROR( VK_ERROR_INCOMPATIBLE_DRIVER ),
		VK_ERROR( VK_ERROR_TOO_MANY_OBJECTS ),
		VK_ERROR( VK_ERROR_FORMAT_NOT_SUPPORTED ),
		VK_ERROR( VK_ERROR_FRAGMENTED_POOL ),
		VK_ERROR( VK_ERROR_UNKNOWN ),
		VK_ERROR( VK_ERROR_OUT_OF_POOL_MEMORY ),
		VK_ERROR( VK_ERROR_INVALID_EXTERNAL_HANDLE ),
		VK_ERROR( VK_ERROR_FRAGMENTATION ),
		VK_ERROR( VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS ),
		VK_ERROR( VK_PIPELINE_COMPILE_REQUIRED ),
		VK_ERROR( VK_ERROR_SURFACE_LOST_KHR ),
		VK_ERROR( VK_ERROR_NATIVE_WINDOW_IN_USE_KHR ),
		VK_ERROR( VK_SUBOPTIMAL_KHR),
		VK_ERROR( VK_ERROR_OUT_OF_DATE_KHR ),
		VK_ERROR( VK_ERROR_INCOMPATIBLE_DISPLAY_KHR ),
		VK_ERROR( VK_ERROR_VALIDATION_FAILED_EXT ),
		VK_ERROR( VK_ERROR_INVALID_SHADER_NV ),
		VK_ERROR( VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT ),
		VK_ERROR( VK_ERROR_NOT_PERMITTED_KHR ),
		VK_ERROR( VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT ),
		VK_ERROR( VK_THREAD_IDLE_KHR ),
		VK_ERROR( VK_THREAD_DONE_KHR ),
		VK_ERROR( VK_OPERATION_DEFERRED_KHR ),
		VK_ERROR( VK_OPERATION_NOT_DEFERRED_KHR ),
		VK_ERROR( VK_ERROR_OUT_OF_POOL_MEMORY_KHR ),
		VK_ERROR( VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR ),
		VK_ERROR( VK_ERROR_FRAGMENTATION_EXT ),
		VK_ERROR( VK_ERROR_NOT_PERMITTED_EXT ),
		VK_ERROR( VK_ERROR_INVALID_DEVICE_ADDRESS_EXT ),
		VK_ERROR( VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS_KHR ),
		VK_ERROR( VK_PIPELINE_COMPILE_REQUIRED_EXT ),
		VK_ERROR( VK_ERROR_PIPELINE_COMPILE_REQUIRED_EXT ),
	};

	const VkErrorString *pError;
	for ( const auto& it : szErrorStrings ) {
		if ( it.nError == nResult ) {
			pError = eastl::addressof( it );
		}
	}
	
	using namespace SIREngine::RenderLib::Vulkan;
	
	if ( nResult == VK_ERROR_UNKNOWN ) {
		SIRENGINE_LOG_LEVEL( VulkanBackend, ELogLevel::Warning, "%s: VK_ERROR_UNKNOWN", pCall );
	} else {
		SIRENGINE_LOG_LEVEL( VulkanBackend, ELogLevel::Error, "%s: %s (%u)", pCall, pError->pString, pError->nError );
	}
}

namespace SIREngine::RenderLib::Vulkan {

SIRENGINE_DEFINE_LOG_CATEGORY( VulkanBackend, ELogLevel::Info );

typedef struct {
	VkSurfaceCapabilitiesKHR capabilities;
	VkSurfaceFormatKHR *pFormats;
	VkPresentModeKHR *pPresentModes;
	uint32_t nFormats;
	uint32_t nPresentModes;
} SwapChainSupportInfo_t;

typedef struct {
	bool32 bHasGraphicsFamily;
	uint32_t nGraphicsFamily;
	bool32 bHasPresentFamily;
	uint32_t nPresentFamily;

	bool IsComplete( void ) const {
		return bHasGraphicsFamily && bHasPresentFamily;
	}
} QueueFamilyIndices_t;

static int RateVKDevice( VkPhysicalDevice hDevice )
{
	int score;

	score = 0;

	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties( hDevice, &deviceProperties );

	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceFeatures( hDevice, &deviceFeatures );

	switch ( deviceProperties.deviceType ) {
	case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
		// dedicated GPUs have the greatest performance
		score += 100000;
		break;
	case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
		// most likely an intel GPU
		score += 700;
		break;
	case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
	case VK_PHYSICAL_DEVICE_TYPE_CPU:
		// software rendering, the slowest
		score += 200;
		break;
	};

	// this engine is more optimized on specific vendors
	switch ( deviceProperties.vendorID ) {
	case VK_VENDOR_ID_MESA:
		score += 1000;
		break;
	};

	score += deviceProperties.limits.maxImageDimension2D;

	// add some bonus points for framebuffer features
	score += deviceProperties.limits.maxFramebufferWidth;
	score += deviceProperties.limits.maxFramebufferHeight;

	score += deviceProperties.limits.maxDescriptorSetSamplers;
	score += deviceProperties.limits.maxDescriptorSetUniformBuffers;

	score += deviceProperties.limits.maxVertexInputAttributeOffset;
	score += deviceProperties.limits.maxVertexInputAttributes;
	score += deviceProperties.limits.maxVertexInputBindings;
	score += deviceProperties.limits.maxVertexInputBindingStride;

	if ( !deviceFeatures.shaderInt16 ) {
		score -= 500;
	}
	if ( !deviceFeatures.fullDrawIndexUint32 ) {
		score -= 500;
	}
	if ( deviceFeatures.textureCompressionBC ) {
		score += 2500;
	}
	if ( deviceFeatures.textureCompressionETC2 ) {
		score += 2500;
	}

	SIRENGINE_LOG( "VkPhysicalDevice Score: %i", score );

	return score;
}

static void DumpPhysicalDeviceFeatures( VkPhysicalDevice hDevice )
{
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties( hDevice, &deviceProperties );

	SIRENGINE_LOG( "Vulkan Driver Query:" );
	SIRENGINE_LOG( "  Device Name: %s", deviceProperties.deviceName );
	SIRENGINE_LOG( "  Device ID: %u", deviceProperties.deviceID );
	switch ( deviceProperties.deviceType ) {
	case VK_PHYSICAL_DEVICE_TYPE_CPU:
		SIRENGINE_LOG( "  Device Type: CPU/Software Rendering" );
		break;
	case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
		SIRENGINE_LOG( "  Device Type: Dedicated Graphics Card" );
		break;
	case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
		SIRENGINE_LOG( "  Device Type: Integrated Graphics Card" );
		break;
	case VK_PHYSICAL_DEVICE_TYPE_OTHER:
		SIRENGINE_LOG( "  Device Type: Unknown" );
		break;
	case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
		SIRENGINE_LOG( "  Device Type: Virtualized GPU" );
		break;
	};
	SIRENGINE_LOG( "  Driver Version: %u", deviceProperties.driverVersion );
	SIRENGINE_LOG( "  Vulkan API Version: %u", deviceProperties.apiVersion );
	switch ( deviceProperties.vendorID ) {
	case VK_VENDOR_ID_CODEPLAY:
		SIRENGINE_LOG( "  Vendor: Codeplay Software" );
		break;
	case VK_VENDOR_ID_KAZAN:
		SIRENGINE_LOG( "  Vendor: Kazan Graphics" );
		break;
	case VK_VENDOR_ID_MESA:
		SIRENGINE_LOG( "  Vendor: Mesa (NVidia)" );
		break;
	case VK_VENDOR_ID_POCL:
		SIRENGINE_LOG( "  Vendor: PoCL Think Silicon" );
		break;
	case VK_VENDOR_ID_VIV:
		SIRENGINE_LOG( "  Vendor: VIV" );
		break;
	case VK_VENDOR_ID_VSI:
		SIRENGINE_LOG( "  Vendor: VSI Media" );
		break;
	};
	{
		char str[ VK_UUID_SIZE + 1 ];
		memcpy( str, deviceProperties.pipelineCacheUUID, sizeof( deviceProperties.pipelineCacheUUID ) );
		str[ VK_UUID_SIZE ] = '\0';

		SIRENGINE_LOG( "  Pipeline Cache UUID: %s", str );    
	}
	SIRENGINE_LOG( "(LIMITS)" );
	SIRENGINE_LOG( " - maxColorAttachments: %u", deviceProperties.limits.maxColorAttachments );
	SIRENGINE_LOG( " - maxDescriptorSetStorageBuffers: %u",
		deviceProperties.limits.maxDescriptorSetStorageBuffers );
	SIRENGINE_LOG( " - maxDescriptorSetInputAttachments: %u",
		deviceProperties.limits.maxDescriptorSetInputAttachments );
	SIRENGINE_LOG( " - maxDescriptorSetSampledImages: %u",
		deviceProperties.limits.maxDescriptorSetSampledImages );
	SIRENGINE_LOG( " - maxDescriptorSetSamplers: %u",
		deviceProperties.limits.maxDescriptorSetSamplers );
	SIRENGINE_LOG( " - maxDescriptorSetStorageBuffersDynamic: %u",
		deviceProperties.limits.maxDescriptorSetStorageBuffersDynamic );
	SIRENGINE_LOG( " - maxDescriptorSetStorageImages: %u",
		deviceProperties.limits.maxDescriptorSetStorageImages );
	SIRENGINE_LOG( " - maxDescriptorSetUniformBuffers: %u",
		deviceProperties.limits.maxDescriptorSetUniformBuffers );
	SIRENGINE_LOG( " - maxDescriptorSetUniformBuffersDynamic: %u",
		deviceProperties.limits.maxDescriptorSetUniformBuffersDynamic );
	SIRENGINE_LOG( " - maxFramebufferWidth: %u",
		deviceProperties.limits.maxFramebufferWidth );
	SIRENGINE_LOG( " - maxFramebufferHeight: %u",
		deviceProperties.limits.maxFramebufferHeight );
	SIRENGINE_LOG( " - maxImageDimension2D: %u",
		deviceProperties.limits.maxImageDimension2D );
	SIRENGINE_LOG( " - maxUniformBufferRange: %u",
		deviceProperties.limits.maxUniformBufferRange );
	SIRENGINE_LOG( " - maxVertexInputAttributeOffset: %u",
		deviceProperties.limits.maxVertexInputAttributeOffset );
	SIRENGINE_LOG( " - maxVertexInputAttributes: %u",
		deviceProperties.limits.maxVertexInputAttributes );
	SIRENGINE_LOG( " - maxVertexInputBindings: %u",
		deviceProperties.limits.maxVertexInputBindings );
	SIRENGINE_LOG( " - maxVertexInputBindingStride: %u",
		deviceProperties.limits.maxVertexInputBindingStride );
	SIRENGINE_LOG( " - maxVertexOutputComponents: %u",
		deviceProperties.limits.maxVertexOutputComponents );
}

VKContext *g_pVKContext;

static void *Vulkan_Allocate( void *pUserData, size_t nSize, size_t nAlignment, VkSystemAllocationScope scope )
{
	void *pBuffer;
	pBuffer = malloc( nSize );
	return pBuffer;
}

static void Vulkan_Free( void *pUserData, void *pMemory )
{
	::free( pMemory );
}

static void *Vulkan_Reallocate( void *pUserData, void *pOriginal, size_t nSize, size_t nAlignment, VkSystemAllocationScope scope )
{
	void *pBuffer;
	pBuffer = realloc( pOriginal, nSize );
	return pBuffer;
}

VKContext::VKContext( const ContextInfo_t& contextInfo )
	: IRenderContext()
{
	m_ContextData = contextInfo;
}

bool VKContext::CreateWindow( void )
{
	Uint32 flags;
	uint32_t nBorderMode;

	g_pVKContext = dynamic_cast<VKContext *>( g_pContext );

	flags = SDL_WINDOW_VULKAN;
	nBorderMode = r_WindowMode.GetValue();

	if ( !( nBorderMode % 2 ) ) {
		// its borderless
		flags |= SDL_WINDOW_BORDERLESS;
	}
	if ( r_WindowMode.GetValue() > (uint32_t)EWindowMode::Fullscreen ) {
		flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
	}
	flags |= SDL_WINDOW_SHOWN;

	m_pWindow = SDL_CreateWindow( m_ContextData.pszWindowName, m_ContextData.nWindowPositionX, m_ContextData.nWindowPositionY,
		m_ContextData.nWindowWidth, m_ContextData.nWindowHeight, flags );
	if ( !m_pWindow ) {
		SIRENGINE_LOG_LEVEL( VulkanBackend, ELogLevel::Fatal, "Error creating SDL2 Vulkan window: %s", SDL_GetError() );
	}
	
	VkApplicationInfo appInfo;
	memset( &appInfo, 0, sizeof( appInfo ) );
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = m_ContextData.pszWindowName;
	appInfo.applicationVersion = m_ContextData.nAppVersion;
	appInfo.pEngineName = "SIREngine";
	appInfo.engineVersion = SIRENGINE_VERSION;
	appInfo.apiVersion = VK_API_VERSION_1_3;

	VkInstanceCreateInfo createInfo;
	memset( &createInfo, 0, sizeof( createInfo ) );
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	// create instance && device
	{
		uint32_t nExtensionCount, i;
		const char **pExtensionList;
		const char *szLayers[] = {
			"VK_LAYER_KHRONOS_validation"
		};
		const char **pUsedExtensions;
		uint32_t nUsedExtensions;
		const char *szRequiredExtensions[] = {
			VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
			VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
		};

		if ( !SDL_Vulkan_GetInstanceExtensions( m_pWindow, &nExtensionCount, NULL ) ) {
			SIRENGINE_LOG_LEVEL( VulkanBackend, ELogLevel::Error, "SDL_Vulkan_GetInstanceExtensions failed: %s", SDL_GetError() );
		}
		pExtensionList = (const char **)alloca( sizeof( *pExtensionList ) *nExtensionCount );
		if ( !SDL_Vulkan_GetInstanceExtensions( m_pWindow, &nExtensionCount, pExtensionList ) ) {
			SIRENGINE_LOG_LEVEL( VulkanBackend, ELogLevel::Error, "SDL_Vulkan_GetInstanceExtensions failed: %s", SDL_GetError() );
		}

		nUsedExtensions = ( nExtensionCount + SIREngine_StaticArrayLength( szRequiredExtensions ) );
		pUsedExtensions = (const char **)alloca( sizeof( *pUsedExtensions ) * nUsedExtensions );
		for ( i = 0; i < nExtensionCount; i++ ) {
			pUsedExtensions[i] = pExtensionList[i];
		}
		for ( i = 0; i < SIREngine_StaticArrayLength( szRequiredExtensions ); i++ ) {
			pUsedExtensions[ i + nExtensionCount ] = szRequiredExtensions[i];
		}

		SIRENGINE_LOG_LEVEL( RenderBackend, ELogLevel::Info, "Found %u vulkan extensions.", nExtensionCount );
		SIRENGINE_LOG_LEVEL( RenderBackend, ELogLevel::Info, "Using Vulkan Instance Extensions:" );
		for ( i = 0; i < nUsedExtensions; i++ ) {
			SIRENGINE_LOG_LEVEL( RenderBackend, ELogLevel::Info, "- \"%s\"", pUsedExtensions[i] );
		}

		createInfo.enabledExtensionCount = nUsedExtensions;
		createInfo.ppEnabledExtensionNames = pUsedExtensions;

		createInfo.enabledLayerCount = SIREngine_StaticArrayLength( szLayers );
		createInfo.ppEnabledLayerNames = szLayers;

		VK_CALL( vkCreateInstance( &createInfo, NULL, &m_hInstance ) );
		SIRENGINE_LOG_LEVEL( VulkanBackend, ELogLevel::Info, "VkInstance created." );

		if ( !SDL_Vulkan_CreateSurface( m_pWindow, m_hInstance, &m_hSurface ) ) {
			SIRENGINE_LOG_LEVEL( VulkanBackend, ELogLevel::Error, "SDL_Vulkan_CreateSurface failed: %s", SDL_GetError() );
		}

		SIRENGINE_LOG_LEVEL( VulkanBackend, ELogLevel::Info, "VkSurfaceKHR allocated." );

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
		memset( &debugCreateInfo, 0, sizeof( debugCreateInfo ) );
		debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debugCreateInfo.messageSeverity =
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		debugCreateInfo.messageType =
			VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		debugCreateInfo.pfnUserCallback = debugCallback;
		debugCreateInfo.pUserData = NULL;
	}

	InitPhysicalDevice();
	InitLogicalDevice();
	InitSwapChain();

	return true;
}

QueueFamilyIndices_t FindQueueFamilies( void )
{
	QueueFamilyIndices_t indices;
	uint32_t nQueueFamilyCount;
	VkQueueFamilyProperties *pQueueProperties;

	memset( &indices, 0, sizeof( indices ) );

	nQueueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties( g_pVKContext->GetPhysicalDevice(), &nQueueFamilyCount, NULL );

	pQueueProperties = (VkQueueFamilyProperties *)alloca( sizeof( *pQueueProperties ) * nQueueFamilyCount );
	vkGetPhysicalDeviceQueueFamilyProperties( g_pVKContext->GetPhysicalDevice(), &nQueueFamilyCount, pQueueProperties );

	for ( uint32_t i = 0; i < nQueueFamilyCount; i++ ) {
		if ( pQueueProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT ) {
			indices.bHasGraphicsFamily = true;
			indices.nGraphicsFamily = i;

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR( g_pVKContext->GetPhysicalDevice(), i, g_pVKContext->GetSurface(), &presentSupport );
			if ( presentSupport ) {
				indices.bHasPresentFamily = true;
				indices.nPresentFamily = i;
			}

			if ( indices.IsComplete() ) {
				break;
			}
		}
	}

	return indices;
}

SwapChainSupportInfo_t& QuerySwapChainSupport( void )
{
	static SwapChainSupportInfo_t swapChainSupport;
	uint32_t nFormatCount, nPresentModeCount;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR( g_pVKContext->GetPhysicalDevice(), g_pVKContext->GetSurface(), &swapChainSupport.capabilities );

	vkGetPhysicalDeviceSurfaceFormatsKHR( g_pVKContext->GetPhysicalDevice(), g_pVKContext->GetSurface(), &nFormatCount, NULL );
	if ( nFormatCount != 0 ) {
		static VkSurfaceFormatKHR *formats = (VkSurfaceFormatKHR *)alloca( sizeof( *formats ) * nFormatCount );
		memset( formats, 0, sizeof( *formats ) * nFormatCount );
		swapChainSupport.pFormats = formats;
		swapChainSupport.nFormats = nFormatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR( g_pVKContext->GetPhysicalDevice(), g_pVKContext->GetSurface(), &nFormatCount, formats );
	}

	vkGetPhysicalDeviceSurfacePresentModesKHR( g_pVKContext->GetPhysicalDevice(), g_pVKContext->GetSurface(), &nPresentModeCount, NULL );
	if ( nPresentModeCount != 0 ) {
		static VkPresentModeKHR *presentModes = (VkPresentModeKHR *)alloca(
			sizeof( *presentModes ) * nPresentModeCount );
		memset( presentModes, 0, sizeof( *presentModes ) * nPresentModeCount );
		
		swapChainSupport.pPresentModes = presentModes;
		swapChainSupport.nPresentModes = nPresentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR( g_pVKContext->GetPhysicalDevice(), g_pVKContext->GetSurface(), &nPresentModeCount, presentModes );
	}

	return swapChainSupport;
}

VkSurfaceFormatKHR ChooseSwapSurfaceFormat( const VkSurfaceFormatKHR *pAvailableFormats,
	uint32_t nAvailableFormats )
{
	for ( uint32_t i = 0; i < nAvailableFormats; i++ ) {
		if ( pAvailableFormats[i].format == VK_FORMAT_B8G8R8A8_SRGB
			&& pAvailableFormats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR )
		{
			return pAvailableFormats[i];
		}
	}

	return pAvailableFormats[0];
}

VkPresentModeKHR ChooseSwapPresentMode( const VkPresentModeKHR *pAvailablePresentModes,
	uint32_t nAvailablePresentModes )
{
	VkPresentModeKHR desiredPresentMode;

	switch ( r_VSync.GetValue() ) {
	case -1:
		desiredPresentMode = VK_PRESENT_MODE_FIFO_RELAXED_KHR;
		break;
	case 0:
		desiredPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
		break;
	case 1:
		desiredPresentMode = VK_PRESENT_MODE_FIFO_KHR;
		break;
	};

	for ( uint32_t i = 0; i < nAvailablePresentModes; i++ ) {
		if ( pAvailablePresentModes[i] == desiredPresentMode ) {
			return pAvailablePresentModes[i];
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D ChooseSwapExtent( const VkSurfaceCapabilitiesKHR& capabilities, uint32_t nWidth, uint32_t nHeight )
{
	if ( capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max() ) {
		return capabilities.currentExtent;
	} else {
		VkExtent2D actualExtent = { nWidth, nHeight };

		actualExtent.width = eastl::clamp( actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width );
		actualExtent.height = eastl::clamp( actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height );
		
		return actualExtent;
	}
}

void VKContext::InitPhysicalDevice( void )
{
	uint32_t nDeviceCount, i;
	int nDeviceScore, nCurrentScore;
	VkPhysicalDevice *pDeviceList;

	vkEnumeratePhysicalDevices( m_hInstance, &nDeviceCount, NULL );
	if ( !nDeviceCount ) {
		SIRENGINE_LOG_LEVEL( VulkanBackend, ELogLevel::Fatal, "No vulkan support found on this device, please install the latest vulkan driver" );
	}

	pDeviceList = (VkPhysicalDevice *)alloca( sizeof( *pDeviceList ) * nDeviceCount );
	vkEnumeratePhysicalDevices( m_hInstance, &nDeviceCount, pDeviceList );

	nCurrentScore = 0;
	for ( i = 0; i < nDeviceCount; i++ ) {
		nDeviceScore = RateVKDevice( pDeviceList[i] );

		if ( nDeviceScore > nCurrentScore ) {
			m_hPhysicalDevice = pDeviceList[i];
			nCurrentScore = nDeviceScore;
		}
	}
}

void VKContext::InitLogicalDevice( void )
{
	VkDeviceQueueCreateInfo szQueueCreateInfos[2];
	uint32_t nQueueCreateInfoCount, i;
	float nQueuePriority;
	const std::initializer_list<const CString> szRequiredExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME,
		VK_KHR_VULKAN_MEMORY_MODEL_EXTENSION_NAME,
		VK_KHR_SHADER_FLOAT_CONTROLS_EXTENSION_NAME,
		VK_KHR_SPIRV_1_4_EXTENSION_NAME,
		VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME,
		VK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME,
		VK_KHR_MAINTENANCE1_EXTENSION_NAME,
		VK_KHR_MAINTENANCE2_EXTENSION_NAME,
		VK_KHR_MAINTENANCE3_EXTENSION_NAME,
		VK_KHR_DESCRIPTOR_UPDATE_TEMPLATE_EXTENSION_NAME,
		VK_KHR_16BIT_STORAGE_EXTENSION_NAME,
		VK_KHR_STORAGE_BUFFER_STORAGE_CLASS_EXTENSION_NAME,
		VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME,
		VK_EXT_MEMORY_BUDGET_EXTENSION_NAME,
		VK_EXT_PAGEABLE_DEVICE_LOCAL_MEMORY_EXTENSION_NAME,
		VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
		VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME,
		VK_EXT_MEMORY_PRIORITY_EXTENSION_NAME,
	};
	const char *szLayers[] = {
		"VK_LAYER_KHRONOS_validation"
	};
	const VkValidationFeatureEnableEXT szEnabledValidationFeatures[] = {
		VK_VALIDATION_FEATURE_ENABLE_BEST_PRACTICES_EXT,
		VK_VALIDATION_FEATURE_ENABLE_SYNCHRONIZATION_VALIDATION_EXT,
	};
	uint32_t nUsedExtensions;
	VkExtensionProperties *pExtensionList;
	const char **pUsedExtensions;
	uint32_t nActualExtensions;

	const QueueFamilyIndices_t indices = FindQueueFamilies();
	const uint32_t szUniqueQueueFamilies[] = { indices.nGraphicsFamily, indices.nPresentFamily };

	nQueuePriority = 1.0f;
	memset( szQueueCreateInfos, 0, sizeof( szQueueCreateInfos ) );
	for ( i = 0; i < SIREngine_StaticArrayLength( szUniqueQueueFamilies ); i++ ) {
		szQueueCreateInfos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		szQueueCreateInfos[i].queueFamilyIndex = szUniqueQueueFamilies[i];
		szQueueCreateInfos[i].queueCount = 1;
		szQueueCreateInfos[i].pQueuePriorities = &nQueuePriority;
	}

	vkEnumerateDeviceExtensionProperties( m_hPhysicalDevice, NULL, &nUsedExtensions, NULL );
	pExtensionList = (VkExtensionProperties *)alloca( sizeof( *pExtensionList ) * nUsedExtensions );
	memset( pExtensionList, 0, sizeof( *pExtensionList ) * nUsedExtensions );
	vkEnumerateDeviceExtensionProperties( m_hPhysicalDevice, NULL, &nUsedExtensions, pExtensionList );
	
	pUsedExtensions = (const char **)alloca( sizeof( *pUsedExtensions ) * szRequiredExtensions.size() );
	nActualExtensions = 0;
	for ( i = 0; i < nUsedExtensions; i++ ) {
		if ( eastl::find( szRequiredExtensions.begin(), szRequiredExtensions.end(),
			pExtensionList[i].extensionName ) != szRequiredExtensions.end() )
		{
			pUsedExtensions[ nActualExtensions ] = pExtensionList[i].extensionName;
			nActualExtensions++;
			SIRENGINE_LOG_LEVEL( VulkanBackend, ELogLevel::Info, "Using Vulkan Extension \"%s\"", pExtensionList[i].extensionName );
		}
	}

	VkDeviceQueueCreateInfo queueCreateInfo;
	memset( &queueCreateInfo, 0, sizeof( queueCreateInfo ) );
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = indices.nGraphicsFamily;
	queueCreateInfo.queueCount = 1;
	queueCreateInfo.pQueuePriorities = &nQueuePriority;

	VkValidationFeaturesEXT enabledFeatures;
	memset( &enabledFeatures, 0, sizeof( enabledFeatures ) );
	enabledFeatures.sType = VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT;
	enabledFeatures.enabledValidationFeatureCount = SIREngine_StaticArrayLength( szEnabledValidationFeatures );
	enabledFeatures.pEnabledValidationFeatures = szEnabledValidationFeatures;

	VkPhysicalDevice16BitStorageFeaturesKHR deviceFeatures16Bit;
	memset( &deviceFeatures16Bit, 0, sizeof( deviceFeatures16Bit ) );
	deviceFeatures16Bit.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_16BIT_STORAGE_FEATURES_KHR;
	deviceFeatures16Bit.storageInputOutput16 = VK_FALSE;
	
	VkPhysicalDeviceFeatures2KHR deviceFeatures;
	memset( &deviceFeatures, 0, sizeof( deviceFeatures ) );
	deviceFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2_KHR;
	deviceFeatures.features.samplerAnisotropy = VK_TRUE;
	deviceFeatures.features.fullDrawIndexUint32 = VK_TRUE;
	deviceFeatures.features.shaderInt16 = VK_TRUE;
	deviceFeatures.pNext = &deviceFeatures16Bit;

	VkDeviceCreateInfo createInfo;
	memset( &createInfo, 0, sizeof( createInfo ) );
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	createInfo.queueCreateInfoCount = 1; // segfaults in glcore if set to 2
	createInfo.pQueueCreateInfos = szQueueCreateInfos;

	createInfo.pNext = &deviceFeatures;

	createInfo.enabledExtensionCount = nActualExtensions;
	createInfo.ppEnabledExtensionNames = pUsedExtensions;

	createInfo.enabledLayerCount = SIREngine_StaticArrayLength( szLayers );
	createInfo.ppEnabledLayerNames = szLayers;

	VK_CALL( vkCreateDevice( m_hPhysicalDevice, &createInfo, NULL, &m_hDevice ) );

	vkGetDeviceQueue( m_hDevice, indices.nGraphicsFamily, 0, &m_hGraphicsQueue );

	{
		VmaAllocatorCreateInfo allocatorInfo;
		memset( &allocatorInfo, 0, sizeof( allocatorInfo ) );
		allocatorInfo.device = m_hDevice;
		allocatorInfo.instance = m_hInstance;
		allocatorInfo.physicalDevice = m_hPhysicalDevice;
		allocatorInfo.preferredLargeHeapBlockSize = 128ull * 1024 * 1024;
		{
			static VmaVulkanFunctions funcProcs;
			funcProcs.vkAllocateMemory =
				(PFN_vkAllocateMemory)vkGetDeviceProcAddr( m_hDevice, "vkAllocateMemory" );
			funcProcs.vkBindBufferMemory2KHR =
				(PFN_vkBindBufferMemory2KHR)vkGetDeviceProcAddr( m_hDevice, "vkBindBufferMemory2KHR" );
			funcProcs.vkBindImageMemory2KHR =
				(PFN_vkBindImageMemory2KHR)vkGetDeviceProcAddr( m_hDevice, "vkBindImageMemory2KHR" );
			funcProcs.vkGetBufferMemoryRequirements2KHR =
				(PFN_vkGetBufferMemoryRequirements2KHR)vkGetDeviceProcAddr( m_hDevice, "vkGetBufferMemoryRequirements2KHR" );
			funcProcs.vkGetImageMemoryRequirements2KHR =
				(PFN_vkGetImageMemoryRequirements2KHR)vkGetDeviceProcAddr( m_hDevice, "vkGetImageMemoryRequirements2KHR" );

			allocatorInfo.pVulkanFunctions = &funcProcs;
		}
		allocatorInfo.flags = VMA_ALLOCATOR_CREATE_KHR_DEDICATED_ALLOCATION_BIT;
	
		VK_CALL( vmaCreateAllocator( &allocatorInfo, &m_hAllocator ) );

		//AllocationCallbacks.pfnAllocation = Vulkan_Allocate;
		//AllocationCallbacks.pfnFree = Vulkan_Free;
		//AllocationCallbacks.pfnReallocation = Vulkan_Reallocate;
	}
}

void VKContext::InitSwapChain( void )
{
	const SwapChainSupportInfo_t& details = QuerySwapChainSupport();

	VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat( details.pFormats, details.nFormats );
	VkPresentModeKHR presentMode = ChooseSwapPresentMode( details.pPresentModes, details.nPresentModes );
	VkExtent2D extent = ChooseSwapExtent( details.capabilities, m_ContextData.nWindowWidth, m_ContextData.nWindowHeight );

	uint32_t imageCount = details.capabilities.minImageCount;
	if ( details.capabilities.maxImageCount > 0 && imageCount > details.capabilities.maxImageCount ) {
		imageCount = details.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo;
	memset( &createInfo, 0, sizeof( createInfo ) );
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = m_hSurface;
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices_t indices = FindQueueFamilies();
	uint32_t szQueueFamilyIndices[] = {
		indices.nGraphicsFamily,
		indices.nPresentFamily
	};

	if ( indices.nGraphicsFamily != indices.nPresentFamily ) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = szQueueFamilyIndices;
	} else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0; // optional
		createInfo.pQueueFamilyIndices = szQueueFamilyIndices;
	}
	createInfo.preTransform = details.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	SIRENGINE_LOG_LEVEL( VulkanBackend, ELogLevel::Info, "Allocating vulkan swapchain...\n" );
	VK_CALL( vkCreateSwapchainKHR( m_hDevice, &createInfo, NULL, &m_hSwapChain ) );
	SIRENGINE_LOG_LEVEL( VulkanBackend, ELogLevel::Info, "Done.\n" );
}

void VKContext::ShutdownBackend( void )
{
	vkDestroySwapchainKHR( m_hDevice, m_hSwapChain, NULL );
	vkDestroySurfaceKHR( m_hInstance, m_hSurface, NULL );
	vkDestroyDevice( m_hDevice, NULL );
	vkDestroyInstance( m_hInstance, NULL );
}

};