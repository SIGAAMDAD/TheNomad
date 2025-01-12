#include "IniSerializer.h"
#include <Engine/Core/Logging/Logger.h>
#include <Engine/Core/FileSystem/FileWriter.h>

namespace SIREngine::Serialization {

bool CIniSerializer::Save( const CFilePath& filePath )
{
	m_FilePath = filePath;
	const char *str;
	CFileWriter file;

	if ( !file.Open( filePath ) ) {
		SIRENGINE_WARNING( "Error opening write-only .ini file \"%s\"", filePath.c_str() );
		return false;
	}

	SIRENGINE_NOTIFICATION( "Writing %lu sections...", m_Values.size() );
	for ( const auto& section : m_Values ) {
		file.Printf( "[%s]\n", section.first.c_str() );
		for ( const auto& value : section.second ) {
			file.Printf( "%s=%s\n", value.first.c_str(), value.second.c_str() );
		}
		file.Printf( "\n" );
	}

	return true;
}

};