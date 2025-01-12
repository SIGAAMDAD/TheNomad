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

	// 16-bit shader input required for colors, TODO: make it so that this is
	// not a required device feature
	if ( !deviceFeatures.shaderInt16 ) {
		score = 0;
	}
	if ( !deviceFeatures.fullDrawIndexUint32 ) {
		score = 0;
	}
	if ( !deviceFeatures.textureCompressionBC ) {
		score += 500;
	}
	if ( !deviceFeatures.textureCompressionETC2 ) {
		score += 500;
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

VKContext::VKContext( const ContextInfo_t& contextInfo )
	: IRenderContext()
{
	m_ContextData = contextInfo;
	SIRENGINE_LOG_LEVEL( RenderBackend, ELogLevel::Info, "Initializing RenderBackend...\n" );
	if ( !CreateWindow() ) {
		SIRENGINE_LOG_LEVEL( RenderBackend, ELogLevel::Fatal, "Error creating SDL2 window: %s", SDL_GetError() );
	}
}

bool VKContext::CreateWindow( void )
{
	Uint32 flags;
	uint32_t nBorderMode;

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
	return true;
}

void VKContext::InitPhysicalDevice( void )
{
	uint32_t nDeviceCount, i;
	int nDeviceScore;
	VkPhysicalDevice *pDeviceList;

	vkEnumeratePhysicalDevices( m_hInstance, &nDeviceCount, NULL );
	if ( !nDeviceCount ) {
		SIRENGINE_LOG_LEVEL( VulkanBackend, ELogLevel::Fatal, "No vulkan support found on this device, please install the latest vulkan driver" );
	}

	pDeviceList = (VkPhysicalDevice *)alloca( sizeof( *pDeviceList ) * nDeviceCount );
	vkEnumeratePhysicalDevices( m_hInstance, &nDeviceCount, pDeviceList );

	for ( i = 0; i < nDeviceCount; i++ ) {
		nDeviceScore = RateVKDevice( pDeviceList[i] );
	}
}

void VKContext::ShutdownBackend( void )
{
	if ( m_hInstance ) {
		vkDestroyInstance( m_hInstance, NULL );
	}
}

};