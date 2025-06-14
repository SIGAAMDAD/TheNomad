#ifndef __SIRENGINE_IMAGE_LOADER_H__
#define __SIRENGINE_IMAGE_LOADER_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include <Engine/Core/FileSystem/FilePath.h>

namespace SIREngine::RenderLib {

class CImageLoader
{
public:
	CImageLoader( const CFilePath& path );
	~CImageLoader();

	uint32_t GetWidth( void ) const;
	uint32_t GetHeight( void ) const;
	uint32_t GetFormat( void ) const;
private:
	bool LoadDDS( const CFilePath& path );

	uint32_t m_nWidth;
	uint32_t m_nHeight;
	uint32_t m_nMipMapCount;
	uint32_t m_nFormat;
};

SIRENGINE_FORCEINLINE uint32_t CImageLoader::GetWidth( void ) const
{
	return m_nWidth;
}

SIRENGINE_FORCEINLINE uint32_t CImageLoader::GetHeight( void ) const
{
	return m_nHeight;
}

SIRENGINE_FORCEINLINE uint32_t CImageLoader::GetFormat( void ) const
{
	return m_nFormat;
}

};

#endif