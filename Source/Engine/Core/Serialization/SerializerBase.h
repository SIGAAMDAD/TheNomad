#ifndef __SIRENGINE_SERIALIZER_BASE_H__
#define __SIRENGINE_SERIALIZER_BASE_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
    #pragma once
#endif

namespace SIREngine::Serialization {
    class ISerializerBase
    {
    public:
        ISerializerBase( void )
        { }
        virtual ~ISerializerBase()
        { }

        virtual const CFilePath& GetFilePath( void ) const
        { return m_FilePath; }
        virtual const CFilePath& GetFileName( void ) const
        { return m_FilePath; }

        virtual bool Load( const CFilePath& fileName ) = 0;
        virtual bool Save( const CFilePath& filePath ) = 0;
    protected:
        CFilePath m_FilePath;
    };
};

#endif