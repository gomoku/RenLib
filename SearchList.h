// SearchList.h: interface for the SearchList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEARCHLIST_H__42608CC2_ED74_11D4_92A3_940247F04F25__INCLUDED_)
#define AFX_SEARCHLIST_H__42608CC2_ED74_11D4_92A3_940247F04F25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SearchItem.h"
#include <vector>

class MoveNode;

class SearchList  
{
public:
	void		Empty();
	void		purge();

	void		SetStart(SearchItem item);
	bool		HasStart();
	SearchItem*	GetStart();

	void		Add(SearchItem item);
	void		Add(SearchItem item1, SearchItem item2);
	void		Clear();
	void		Clear(MoveNode* ptr);
	void		replace(MoveNode* fromPtr, MoveNode* toPtr);

	SearchItem*	Get(int nIndex);
	SearchItem*	Current();
	SearchItem*	Next();
	SearchItem*	Previous();

	bool		IsEmpty() const;
	int			LastItem() const;
	int			CurrentIndex() const;

	enum Kind
	{
		NONE,
		MARK,
		MOVE,
		SIMILAR_POSITION,
		SAME_POSITION,
		COMMENT,
		BOARD_TEXT,
		MERGE_COMMENT
	};

	void		SetInfo(const CString& info, Kind SearchKind);
	CString		GetInfo();
	Kind		GetKind();

public:
	SearchList();
	virtual ~SearchList();

private:
	std::vector<SearchItem> mTable;

	int     mCurrent;
	CString mstrInfo;
	Kind    mSearchKind;
};

#endif // !defined(AFX_SEARCHLIST_H__42608CC2_ED74_11D4_92A3_940247F04F25__INCLUDED_)
