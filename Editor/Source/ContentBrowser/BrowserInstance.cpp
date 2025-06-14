#include "BrowserInstance.h"
#include "../IconsFontAwesome5.h"

namespace Valden {

FileInfo_t CBrowserInstance::GetFileInfo( const CFilePath& path )
{
	FileInfo_t info;

	if ( !System::GetFileStats( &info.Stats, path ) ) {
		return {};
	}

	if ( *path.end() == SIRENGINE_PATH_SEPERATOR ) {
		// its a directory
		info.Type = EContentType::Folder;
		return info;
	}

	const CString ext = GetExtension( path );
	if ( ext == "anim2d" ) {
		info.Type = EContentType::Animation;
	} else if ( ext == "ogg" || ext == "wav" || ext == "mp3" ) {
		info.Type = EContentType::SoundEffect;
	} else if ( ext == "bmf" ) {
		info.Type = EContentType::Map;
	} else if ( ext == "as" ) {
		info.Type = EContentType::ScriptClass;
	} else if ( ext == "tga" || ext == "jpg" || ext == "jpeg" || ext == "png" || ext == "dds" ) {
		info.Type = EContentType::Material;
	} else {
		info.Type = EContentType::File;
	}
	return info;
}

CBrowserInstance::CBrowserInstance( const CFilePath& directory )
	: m_pSelectedDirectory( NULL )
{
	SIRENGINE_LOG_LEVEL( Valden, ELogLevel::Info, "Creating ContentBrowser instance..." );

	CFileList dirCache;
	dirCache.LoadList( directory, true );

	m_ID.sprintf( "Content Browser##%s", directory.c_str() );

	m_FileList.reserve( dirCache.FileCount() );
	for ( uint32_t i = 0; i < dirCache.FileCount(); i++ ) {
		System::FileStats_t stats;

		const CFilePath& path = dirCache.GetList()[i];
		m_FileList.try_emplace( path, GetFileInfo( path ) );
	}
}

CBrowserInstance::~CBrowserInstance()
{
}

void CBrowserInstance::DrawFile( FileView_t *pFile )
{

}

void CBrowserInstance::DrawDirectory( DirectoryView_t *pFile )
{
	
}

void CBrowserInstance::Draw( void )
{
	const float cellSize = 32.0f;
	const float panelWidth = ImGui::GetContentRegionAvail().x;
	int columnCount = 0, columnIndex;

	if ( ImGui::Begin( m_ID.c_str(), NULL ) ) {
		if ( ImGui::Button( ICON_FA_PLUS "Add" ) ) {

		}
		ImGui::SameLine();
		if ( ImGui::Button( ICON_FA_FILE_IMPORT "Import" ) ) {

		}
		ImGui::SameLine();

		bool bPushedColor = false;
		DirectoryView_t *pHistory = m_HistoryBuffer.GetBack();

		if ( !m_HistoryBuffer.CanGoBack() ) {
			bPushedColor = true;
			ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 0.90f, 0.90f, 0.90f, 1.0f ) );
			ImGui::PushStyleColor( ImGuiCol_ButtonActive, ImVec4( 0.90f, 0.90f, 0.90f, 1.0f ) );
			ImGui::PushStyleColor( ImGuiCol_ButtonHovered, ImVec4( 0.90f, 0.90f, 0.90f, 1.0f ) );
		}
		if ( ImGui::Button( ICON_FA_ARROW_CIRCLE_LEFT ) && pHistory ) {
			m_HistoryBuffer.GoBack();
			m_pSelectedDirectory = pHistory;
		}
		if ( bPushedColor ) {
			ImGui::PopStyleColor( 3 );
		}
		else if ( m_HistoryBuffer.CanGoBack() ) {
			if ( ImGui::IsItemHovered( ImGuiHoveredFlags_DelayNone ) ) {
				ImGui::SetTooltip( "Go back to %s", m_HistoryBuffer.GetBack()->Path.c_str() );
			}
		}
		ImGui::SameLine();

		bPushedColor = false;
		pHistory = m_HistoryBuffer.GetForward();
		if ( !m_HistoryBuffer.CanGoForward() ) {
			bPushedColor = true;
			ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 0.90f, 0.90f, 0.90f, 1.0f ) );
			ImGui::PushStyleColor( ImGuiCol_ButtonActive, ImVec4( 0.90f, 0.90f, 0.90f, 1.0f ) );
			ImGui::PushStyleColor( ImGuiCol_ButtonHovered, ImVec4( 0.90f, 0.90f, 0.90f, 1.0f ) );
		}
		if ( ImGui::Button( ICON_FA_ARROW_CIRCLE_RIGHT ) && pHistory ) {
			m_HistoryBuffer.GoForward();
			m_pSelectedDirectory = pHistory;
		}
		if ( bPushedColor ) {
			ImGui::PopStyleColor( 3 );
		}
		else if ( m_HistoryBuffer.CanGoForward() ) {
			if ( ImGui::IsItemHovered( ImGuiHoveredFlags_DelayNone ) ) {
				ImGui::SetTooltip( "Go to %s", m_HistoryBuffer.GetForward()->Path.c_str() );
			}
		}
		ImGui::SameLine();
		if ( ImGui::Button( ICON_FA_FOLDER ) ) {

		}

		for ( const auto& id : m_FileList ) {

		}

		ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 0.0f, 0.0f, 0.0f, 0.0f ) );

		if ( ImGui::BeginTable( "##ContentBrowserTable", 2, ImGuiTableFlags_Resizable ) ) {
			ImGui::TableNextColumn();

			if ( ImGui::BeginTable( "##ContentBrowserSidePanel", 1, ImGuiTableFlags_ScrollY ) ) {
				ImGui::TableNextColumn();
				if ( ImGui::CollapsingHeader( "Favorites##ContentBrowserSidePanelFavorites" ) ) {
					for ( auto& it : m_Favorites ) {
						const char *pName = strrchr( it.View->GetData().Path.c_str(), '/' ) + 1;
						if ( ImGui::MenuItem( pName ) ) {
						}
					}
				}
				if ( ImGui::CollapsingHeader( "Project##ContentBrowserSidePanelProject" ) ) {
					DrawDirectory( m_pSelectedDirectory );
				}
				ImGui::EndTable();
			}

			ImGui::TableNextColumn();

			columnCount = (int)( panelWidth / cellSize );
			if ( columnCount < 1 ) {
				columnCount = 1;
			}

			ImGui::PushStyleVar( ImGuiStyleVar_CellPadding, ImVec2( 1.0f, 1.0f ) );
			ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2( 0, 0 ) );
			if ( ImGui::BeginTable( "##ContentBrowserMainPanelTable", 3, ImGuiTableFlags_ScrollY ) ) {
				columnIndex = 0;

				DirectoryView_t *pDirectory = m_pSelectedDirectory;

				if ( pDirectory ) {
					for ( auto& file : pDirectory->FileList ) {
						/*
						ImGui::PushID( file.Path.c_str() );

						CMaterial *pIcon, *Preview;
						const char *pFilePath = strstr( file.c_str(), CProjectManager::Get()->GetProject()->GetName().c_str() ) - 1;
						const auto& fileInfo = m_FileDatas.find( file );

						EContentType nType = EContentType::File;
						uint64_t nDiskSize = 0;

						if ( fileInfo != m_FileDatas.cend() ) {
							nType = fileInfo->second.nType;
							nDiskSize = fileInfo->second.nDiskSize;
						}

						switch ( nType ) {
						case EContentType::File:
						default:
							pIcon = Cast<CMaterial>( CContentBrowser::Get().m_pFileIcon );
							break;
						case EContentType::IniFile:
							pIcon = Cast<CMaterial>( CContentBrowser::Get().m_pIniFileIcon );
							break;
						case EContentType::CsvFile:
							pIcon = Cast<CMaterial>( CContentBrowser::Get().m_pCsvFileIcon );
							break;
						case EContentType::JsonFile:
							pIcon = Cast<CMaterial>( CContentBrowser::Get().m_pJsonFileIcon );
							break;
						case EContentType::XmlFile:
							pIcon = Cast<CMaterial>( CContentBrowser::Get().m_pXmlFileIcon );
							break;
						case EContentType::Audio:
							pIcon = Cast<CMaterial>( CContentBrowser::Get().m_pAudioIcon );
							break;
						};

						pIcon->GetTexture()->StreamBuffer();

						if ( ImGui::ImageButtonEx( ImGui::GetID( pIcon ), IMGUI_TEXTURE_ID( pIcon ),
							{ ThumbnailSize.GetValue(), ThumbnailSize.GetValue() }, { 0, 0 }, { 1, 1 }, { 0, 0, 0, 0 }, { 1, 1, 1, 1 },
							ImGuiButtonFlags_PressedOnDoubleClick ) )
						{
						}
						if ( ImGui::IsItemClicked( ImGuiMouseButton_Right ) ) {
							ImGui::OpenPopup( "File Utilities##ContentBrowserFileUtilities" );
							m_MousePopupPosition = ImGui::GetMousePos();
							m_pPopupFile = &file;
						}
						if ( ImGui::BeginPopup( "File Utilities##ContentBrowserFileUtilities" ) ) {
							ImGui::SetWindowPos( m_MousePopupPosition );
							ImGui::SetWindowFocus();

							ImGui::SeparatorText( "Common" );
							ImGui::Indent( 0.5f );
							if ( ImGui::MenuItem( ICON_FA_EDIT "Edit" ) ) {
							}
							ITEM_TOOLTIP_STRING( "Open selected item(s) for editing" );
							ImGui::Unindent( 0.5f );

							ImGui::EndPopup();
						}
						if ( ImGui::IsItemHovered( ImGuiHoveredFlags_AllowWhenDisabled | ImGuiHoveredFlags_DelayNone ) ) {
							if ( ImGui::BeginTooltip() ) {
								ImGui::SetWindowFontScale( ImGui::GetFont()->Scale * 1.90f );
								ImGui::TextUnformatted( pFilePath );
								ImGui::SetWindowFontScale( ImGui::GetFont()->Scale * 1.25f );

								ImGui::Separator();

								ImGui::Text( "Path: %s", pFilePath );
								ImGui::Text( "Resource Filepath Length: %lu/%lu", strlen( pFilePath ), MAX_RESOURCE_PATH );
								ImGui::TextUnformatted( "Content Type: " );
								ImGui::SameLine();
								switch ( nType ) {
								case EContentType::JsonFile:
									ImGui::TextUnformatted( "JSon Data" );
									break;
								case EContentType::XmlFile:
									ImGui::TextUnformatted( "XML Data" );
									break;
								case EContentType::IniFile:
									ImGui::TextUnformatted( "Ini Data" );
									break;
								case EContentType::CsvFile:
									ImGui::TextUnformatted( "CSV Data" );
									break;
								case EContentType::SoundEffect:
									ImGui::TextUnformatted( "Audio" );
									break;
								case EContentType::ScriptClass:
									ImGui::TextUnformatted( "Script Class" );
									break;
								case EContentType::File:
									ImGui::TextUnformatted( "Misc. File" );
									break;
								case EContentType::Animation:
									ImGui::TextUnformatted( "Animation" );
									break;
								};
								ImGui::Text( "Disk Size: %lu", nDiskSize );
								ImGui::EndTooltip();
							}
						}
						ImGui::TextWrapped( "%s", pFilePath );
						ImGui::PopID();
						ImGui::TableNextColumn();
						*/
					}
					for ( auto& directory : pDirectory->FileList ) {
						ImGui::PushID( directory->Path.c_str() );
						/*
						if ( ImGui::ImageButton( ImGui::GetID( CContentBrowser::Get().m_pDirectoryIcon ),
							IMGUI_TEXTURE_ID( CContentBrowser::Get().m_pDirectoryIcon ),
							{ ThumbnailSize.GetValue(), ThumbnailSize.GetValue() }, { 0, 0 }, { 1, 1 }, { 0, 0, 0, 0 }, { 1, 1, 1, 1 },
							ImGuiButtonFlags_PressedOnDoubleClick ) )
						{
							SIRENGINE_LOG( "Setting directory to \"%s\"", directory.Path.c_str() );
							m_AssetTree.SetSelectedDir( &directory );
							m_HistoryBuffer.AddHistoryData( &directory );
						}
						*/
						if ( ImGui::IsItemClicked( ImGuiMouseButton_Right ) ) {
							ImGui::OpenPopup( "Folder Utilities##ContentBrowserDirectoryUtilities" );
							m_MousePopupPosition = ImGui::GetMousePos();
//							m_pPopupFile = &directory.Path;
						}
						if ( ImGui::BeginDragDropSource() ) {
							ImGui::EndDragDropSource();
						}
						if ( ImGui::IsItemHovered( ImGuiHoveredFlags_AllowWhenDisabled | ImGuiHoveredFlags_DelayNone ) ) {
							if ( ImGui::BeginTooltip() ) {
								ImGui::SetWindowFontScale( ImGui::GetFont()->Scale * 1.90f );
								ImGui::TextUnformatted( directory->DirectoryName.c_str() );
								ImGui::SameLine();
								ImGui::TextUnformatted( " (Folder)" );
								ImGui::SetWindowFontScale( ImGui::GetFont()->Scale * 1.25f );
							
								ImGui::Separator();

//								ImGui::Text( "Path: %s", strstr( directory.Path.c_str(),
//									CProjectManager::Get()->GetProject()->GetName().c_str() ) - 1 );

								ImGui::EndTooltip();
							}
						}
						if ( ImGui::BeginPopup( "Folder Utilities##ContentBrowserDirectoryUtilities" ) ) {
							ImGui::SetWindowPos( m_MousePopupPosition );
							ImGui::SetWindowFocus();

							ImGui::Indent( 0.5f );
							if ( ImGui::MenuItem( ICON_FA_FOLDER_PLUS "New Folder##NewFolder_ContentBrowserDirectoryUtilities" ) ) {
								/*
								directory->Children.emplace_back();
								directory->Children.back().DirectoryName = "New Folder";
//								directory->Children.back().Path = SIRENGINE_TEMP_VSTRING( "%s/New Folder", directory.Path.c_str() );
								*/
							}
							/*
							ITEM_TOOLTIP_STRING( "Creates a new folder in selected folder" );
							if ( ImGui::MenuItem( ICON_FA_FOLDER_OPEN
								"Show Folder in New Content Browser##ShowFolder_ContentBrowserDirectoryUtilities" ) )
							{

							}
							ITEM_TOOLTIP_STRING( "Opens a new Content Browser window set at the selected folder" );
							*/
							ImGui::Unindent( 0.5f );

							ImGui::SeparatorText( "Folder Actions" );
							ImGui::Indent( 0.5f );
							/*
							if ( ImGui::MenuItem( ICON_FA_STAR "Add To Favorites##AddToFav_ContentBrowserFileUtilitiesFavorite" ) ) {
								m_Favorites.push_back( { m_pPopupFile,
									m_AssetTree.GetDirectory( *m_pPopupFile ) != NULL } );
								ImGui::CloseCurrentPopup();
							}
							ITEM_TOOLTIP_STRING( "Flags selected folder as favorite" );
							if ( ImGui::MenuItem( ICON_FA_EDIT "Rename##Rename_ContentBrowserDirectoryUtilities" ) ) {
							}
							ITEM_TOOLTIP_STRING( "Rename selected folder" );
							*/
							ImGui::Unindent( 0.5f );

							ImGui::EndPopup();
						}
						ImGui::PopID();
						ImGui::TableNextColumn();
					}
				}
				ImGui::EndTable();
			}

			ImGui::PopStyleVar( 2 );
			ImGui::PopStyleColor();

			ImGui::EndTable();
		}

		ImGui::End();
	}
}

};