#ifndef __VALDEN_BROWSER_INSTANCE_H__
#define __VALDEN_BROWSER_INSTANCE_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include <Engine/Core/Core.h>
#include <Engine/Core/FileSystem/FilePath.h>
#include "DirectoryView.h"
#include "BrowserHistory.h"

namespace Valden {

enum class EContentType : uint32_t
{
	Folder,

	File,
	Material,
	SoundEffect,
	Animation,
	ScriptClass,
	Map,
	JsonFile,
	XmlFile,
	CsvFile,
	IniFile,

	Count
};

typedef struct FileInfo_s {
	System::FileStats_t Stats;
	EContentType Type;
} FileInfo_t;

typedef struct ViewFavorite_s {
	const FileInfo_t *File;
	CDirectoryView *View;
	bool IsDirectory;
} ViewFavorite_t;

class CBrowserInstance
{
public:
	CBrowserInstance( const CFilePath& directory );
	~CBrowserInstance();

	void Draw( void );
private:
	FileInfo_t GetFileInfo( const CFilePath& path );
	void DrawDirectory( DirectoryView_t *pDirectory );
	void DrawFile( FileView_t *pFile );

	eastl::unordered_map<CFilePath, FileInfo_t> m_FileList;
	eastl::vector<ViewFavorite_t> m_Favorites;

	CString m_ID;
	CBrowserHistory m_HistoryBuffer;

	ImVec2 m_MousePopupPosition;

	DirectoryView_t *m_pSelectedDirectory;
};

};

#endif