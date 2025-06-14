#ifndef __VALDEN_BROWSER_HISTORY_H__
#define __VALDEN_BROWSER_HISTORY_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include "DirectoryView.h"
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

	void AddHistoryData( DirectoryView_t *dir );

	DirectoryView_t *GetBack( void ) const;
	DirectoryView_t *GetForward( void ) const;
private:
	eastl::vector<DirectoryView_t *> m_HistoryData;
	uint64_t m_nCurrentHistoryIndex;
};

};

#endif