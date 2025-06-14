#ifndef __ASSETLIB_SPRITE_SHEET_H__
#define __ASSETLIB_SPRITE_SHEET_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include "AssetFile.h"
#include <Engine/Core/Pch.h>

namespace SIREngine::AssetLib {

typedef struct Sprite_s {
	glm::ivec2 Offset;
} Sprite_t;

class CSpriteSheet : public IAssetFile
{
public:
	CSpriteSheet( void );
	CSpriteSheet( const CFilePath& path );
	virtual ~CSpriteSheet() override;

	virtual bool IsLoaded( void ) const override;

	virtual bool Load( const CFilePath& path ) override;
private:
	glm::ivec2 m_SpriteSize;
	glm::ivec2 m_SheetSize;

	Sprite_t *m_pSpriteList;
	uint32_t m_nSpriteCountX;
	uint32_t m_nSpriteCountY;
};

};