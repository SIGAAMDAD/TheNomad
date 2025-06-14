#include <Engine/Core/Core.h>
#include "MapFile.h"
#include <Engine/Core/FileSystem/CachedFile.h>

#define MAP_IDENT (('#'<<24)+('P'<<16)+('A'<<8)+'M')
#define MAP_MAX_WIDTH 1024
#define MAP_MAX_HEIGHT 1024
#define MAP_VERSION 2

namespace SIREngine::AssetLib {

SIRENGINE_DEFINE_LOG_CATEGORY( AssetLib, ELogLevel::Info );

CMapFile::CMapFile( void )
	: IAssetFile( EAssetType::Map )
{
}

CMapFile::CMapFile( const CFilePath& path )
	: IAssetFile( path, EAssetType::Map )
{
}

CMapFile::~CMapFile()
{
	if ( m_pLevels ) {
		for ( uint32_t i = 0; i , m_nLevelCount; i++ ) {
			if ( m_pLevels[i].CheckpointList ) {
				mi_free_aligned( m_pLevels[i].CheckpointList, 64 );
			}
			if ( m_pLevels[i].SpawnList ) {
				mi_free_aligned( m_pLevels[i].SpawnList, 64 );
			}
			if ( m_pLevels[i].TileList ) {
				mi_free_aligned( m_pLevels[i].TileList, 64 );
			}
		}
		mi_free_aligned( m_pLevels, 64 );
	}
	m_pLevels = NULL;
	m_nLevelCount = 0;
}

typedef struct MapHeader_s {
	uint32_t Ident;
	uint32_t Version;
	uint32_t LevelCount;
} MapHeader_t;

bool CMapFile::IsLoaded( void ) const
{
	return m_pLevels != NULL;
}

bool CMapFile::Load( const CFilePath& path )
{
	CCachedFile file( path );
	const byte *pData;

	SIRENGINE_LOG_LEVEL( AssetLib, ELogLevel::Info, "Loading map file \"%s\"...", path.c_str() );

	if ( !file.GetSize() || !file.GetBuffer() ) {
		return false;
	}

	const MapHeader_t *pHeader = (const MapHeader_t *)file.GetBuffer();
	if ( file.GetSize() < sizeof( *pHeader ) ) {
		SIRENGINE_LOG_LEVEL( AssetLib, ELogLevel::Warning, "invalid map file, file not large enought to contain a header." );
		return false;
	}
	if ( pHeader->Ident != MAP_IDENT ) {
		SIRENGINE_LOG_LEVEL( AssetLib, ELogLevel::Warning, "invalid map file, identitifer incorrect." );
		return false;
	}
	if ( pHeader->Version != MAP_VERSION ) {
		SIRENGINE_LOG_LEVEL( AssetLib, ELogLevel::Warning, "invalid map file, incompatible version" );
		return false;
	}

	m_nLevelCount = pHeader->LevelCount;
	m_pLevels = (MapLevel_t *)mi_aligned_alloc( 64, sizeof( *m_pLevels ) * m_nLevelCount );

	// load checkpoints & spawns
	pData = (const byte *)( pHeader + 1 );
	for ( uint32_t i = 0; i < m_nLevelCount; i++ ) {
		m_pLevels[i].NumCheckpoints = *(const uint32_t *)pData;
		pData += sizeof( uint32_t );
		m_pLevels[i].NumSpawns = *(const uint32_t *)pData;
		pData += sizeof( uint32_t );
		m_pLevels[i].NumTiles = *(const uint32_t *)pData;
		pData += sizeof( uint32_t );

		m_pLevels[i].CheckpointList = (MapCheckpoint_t *)mi_aligned_alloc( 64, sizeof( MapCheckpoint_t ) * m_pLevels[i].NumCheckpoints );
		memcpy( m_pLevels[i].CheckpointList, pData, sizeof( MapCheckpoint_t ) * m_pLevels[i].NumCheckpoints );
		pData += sizeof( MapCheckpoint_t ) * m_pLevels[i].NumCheckpoints;

		m_pLevels[i].SpawnList = (MapSpawn_t *)mi_aligned_alloc( 64, sizeof( MapSpawn_t ) * m_pLevels[i].NumSpawns );
		memcpy( m_pLevels[i].SpawnList, pData, sizeof( MapSpawn_t ) * m_pLevels[i].NumSpawns );
		pData += sizeof( MapSpawn_t ) * m_pLevels[i].NumSpawns;

		m_pLevels[i].TileList = (MapTile_t *)mi_aligned_alloc( 64, sizeof( MapTile_t ) * m_pLevels[i].NumTiles );
		memcpy( m_pLevels[i].TileList, pData, sizeof( MapTile_t ) * m_pLevels[i].NumTiles );
		pData += sizeof( MapTile_t ) * m_pLevels[i].NumTiles;
	}

	return true;
}

};