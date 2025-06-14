#ifndef __ASSETLIB_ASSET_FILE_H__
#define __ASSETLIB_ASSET_FILE_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include <Engine/Core/FileSystem/FilePath.h>
#include <Engine/Core/Logging/Logger.h>

namespace SIREngine::AssetLib {

enum class EAssetType : uint32_t
{
	Map,
	SpriteSheet,
	Animation,
	SoundEffect,

	Count
};

class IAssetFile
{
public:
	IAssetFile( EAssetType nType );
	IAssetFile( const CFilePath& path, EAssetType nType );
	virtual ~IAssetFile();

	const CFilePath& GetPath( void ) const;
	EAssetType GetType( void ) const;

	virtual bool IsLoaded( void ) const = 0;

	virtual bool Load( const CFilePath& path ) = 0;
protected:
	CFilePath m_Path;
	EAssetType m_nType;
};

SIRENGINE_FORCEINLINE IAssetFile::IAssetFile( EAssetType nType )
	: m_nType( nType )
{
}

SIRENGINE_FORCEINLINE IAssetFile::IAssetFile( const CFilePath& path, EAssetType nType )
	: m_Path( path ), m_nType( nType )
{
}

SIRENGINE_FORCEINLINE IAssetFile::~IAssetFile()
{
}

SIRENGINE_FORCEINLINE const CFilePath& IAssetFile::GetPath( void ) const
{
	return m_Path;
}

SIRENGINE_FORCEINLINE EAssetType IAssetFile::GetType( void ) const
{
	return m_nType;
}

SIRENGINE_DECLARE_LOG_CATEGORY( AssetLib, ELogLevel::Info );

};

#endif