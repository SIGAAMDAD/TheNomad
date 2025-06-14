#include "../ImageLoader.h"
#include <Engine/Core/Types.h>
#include <Engine/Core/FileSystem/CachedFile.h>
#include "../RenderContext.h"
#include <vulkan/vulkan.h>
#include <GL/gl.h>

namespace SIREngine::RenderLib {

typedef struct DDSHeader_s {
	uint32_t dwHeader;
	uint32_t dwSize;
	uint32_t dwFlags;
	uint32_t dwHeight;
	uint32_t dwWidth;
	uint32_t dwPitchOrLinearSize;
	uint32_t dwDepth;
	uint32_t dwMipMapCount;
	uint32_t dwReserved[11];
	uint32_t dwPixelFormatSize;
	uint32_t dwPixelFormatFlags;
	uint32_t dwFourCC;
	uint32_t dwRGBBitCount;
	uint32_t dwRBitMask;
	uint32_t dwGBitMask;
	uint32_t dwBBitMask;
	uint32_t dwABitMask;
	uint32_t dwCaps;
	uint32_t dwCaps2;
	uint32_t dwCaps3;
	uint32_t dwCaps4;
	uint32_t dwReserved2;
} DDSHeader_t;

bool CImageLoader::LoadDDS( const CFilePath& path )
{
	CCachedFile file( path );

	if ( !file.GetSize() || !file.GetBuffer() ) {
		return false;
	}

	if ( file.GetSize() < sizeof( DDSHeader_t ) ) {
		return false;
	}

	const DDSHeader_t *pHeader = (const DDSHeader_t *)file.GetBuffer();
	byte *pData;

	m_nHeight = pHeader->dwHeight;
	m_nWidth = pHeader->dwWidth;
	m_nMipMapCount = pHeader->dwMipMapCount;

	// determine FourCC format
	if ( ( (const byte *)pHeader )[84] == 'D' ) {
		switch ( ( (const byte *)pHeader )[87] ) {
		case '1': // DXT1
			switch ( ERenderAPI( vid_RenderAPI.GetValue() ) ) {
			case ERenderAPI::OpenGL:
				m_nFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
				break;
			case ERenderAPI::Vulkan:
				m_nFormat = VK_FORMAT_BC1_RGBA_UNORM_BLOCK;
				break;
			};
			break;
		case '3': // DXT3
			switch ( ERenderAPI( vid_RenderAPI.GetValue() ) ) {
			case ERenderAPI::OpenGL:
				m_nFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
				break;
			case ERenderAPI::Vulkan:
				m_nFormat = VK_FORMAT_BC2_UNORM_BLOCK;
				break;
			};
			break;
		case '5': // DXT5
			switch ( ERenderAPI( vid_RenderAPI.GetValue() ) ) {
			case ERenderAPI::OpenGL:
				m_nFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
				break;
			case ERenderAPI::Vulkan:
				m_nFormat = VK_FORMAT_BC3_UNORM_BLOCK;
				break;
			};
			break;
		};
	}

	return true;
}

};