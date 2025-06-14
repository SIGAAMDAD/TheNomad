#ifndef __ASSETLIB_MAP_FILE_H__
#define __ASSETLIB_MAP_FILE_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include "AssetFile.h"

namespace SIREngine::AssetLib {

typedef struct MapCheckpoint_s {
	uint16_t Position[2];
} MapCheckpoint_t;

typedef struct MapSpawn_s {
	uint16_t Position[2];
	uint16_t EntityType;
	uint32_t EntityID;
} MapSpawn_t;

typedef struct MapTile_s {
	float Position[2];
	float Scale;
	float Rotation;
	uint32_t SpriteOffsetX;
	uint32_t SpriteOffsetY;
} MapTile_t;

typedef struct MapLevel_s {
	MapCheckpoint_t *CheckpointList;
	MapSpawn_t *SpawnList;
	MapTile_t *TileList;

	uint32_t NumTiles;
	uint16_t NumCheckpoints;
	uint16_t NumSpawns;
} MapLevel_t;

class CMapFile : public IAssetFile
{
public:
	CMapFile( void );
	CMapFile( const CFilePath& path );
	virtual ~CMapFile() override;

	virtual bool IsLoaded( void ) const override;

	virtual bool Load( const CFilePath& path ) override;

	const MapLevel_t *GetLevels( void ) const;
	uint32_t NumLevels( void ) const;
private:
	MapLevel_t *m_pLevels;
	uint32_t m_nLevelCount;
};

SIRENGINE_FORCEINLINE const MapLevel_t *CMapFile::GetLevels( void ) const
{
	return m_pLevels;
}

SIRENGINE_FORCEINLINE uint32_t CMapFile::NumLevels( void ) const
{
	return m_nLevelCount;
}

};

#endif