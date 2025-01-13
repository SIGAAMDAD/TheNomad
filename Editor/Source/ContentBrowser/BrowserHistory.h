#ifndef __VALDEN_BROWSER_HISTORY_H__
#define __VALDEN_BROWSER_HISTORY_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include "FileView.h"
#include "../ValdenApplication.h"

namespace Valden {
	class CBrowserHistory
	{
	public:
		CBrowserHistory( void )
		{ }
		~CBrowserHistory()
		{ }

		bool CanGoBack( void ) const;
		bool CanGoForward( void ) const;

		bool GoForward( void );
		bool GoBack( void );

		void AddHistoryData( FileView_t *dir );

		FileView_t *GetBack( void ) const;
		FileView_t *GetForward( void ) const;
	private:
		eastl::vector<FileView_t *> m_HistoryData;
		uint64_t m_nCurrentHistoryIndex;
	};
};

#endif