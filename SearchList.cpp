// SearchList.cpp: implementation of the SearchList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RenLib.h"
#include "SearchList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

namespace
{
	const SearchItem emptyItem(0, "", SearchItem::NONE);
}

SearchList::SearchList()
: mCurrent(0),
  mSearchKind(NONE)
{
}

SearchList::~SearchList()
{
	mTable.clear();
}

void SearchList::Empty()
{
	mTable.clear();
	mCurrent = 0;
	mstrInfo.Empty();
}

void SearchList::purge()
{
	std::vector<SearchItem>::iterator iter = mTable.begin();

	for (int i=mTable.size()-1; i>=1; i--)
	{
		if (mTable[i].getMoveNode() == 0)
		{
			mTable.erase(iter + i);
		}
	}

	mCurrent = 0;
	mstrInfo.Empty();
}

void SearchList::SetStart(SearchItem item)
{
	if (mTable.empty())
	{
		mTable.push_back(item);
	}
	else
	{
		mTable[0] = item;
	}
}

bool SearchList::HasStart()
{
	return !mTable.empty();
}

SearchItem* SearchList::GetStart()
{
	VERIFY(!mTable.empty());

	return &mTable[0];
}

void SearchList::Add(SearchItem item)
{
	bool isFound = false;

	if (item.getMoveNode() != GetStart()->getMoveNode())
	{
		for (int i=1; i < mTable.size(); i++)
		{
			if (mTable[i] == item)
			{
				isFound = true;
				break;
			}
		}

		if (!isFound)
		{
			mTable.push_back(item);
		}
	}
}

void SearchList::Add(SearchItem item1, SearchItem item2)
{
	mTable.push_back(item1);
	mTable.push_back(item2);
}

void SearchList::Clear()
{
	VERIFY(mCurrent < mTable.size());

	mTable[mCurrent] = emptyItem;
}

void SearchList::Clear(MoveNode* ptr)
{
	for (int i=1; i < mTable.size(); i++)
	{
		if (mTable[i].getMoveNode() == ptr)
		{
			mTable[i] = emptyItem;
			break;
		}
	}
}

void SearchList::replace(MoveNode* fromPtr, MoveNode* toPtr)
{
	for (int i=1; i < mTable.size(); i++)
	{
		if (mTable[i].getMoveNode() == fromPtr)
		{
			mTable[i].setMoveNode(toPtr);
			break;
		}
	}
}

SearchItem* SearchList::Get(int nIndex)
{
	int size = mTable.size();
	VERIFY(nIndex >= 0 && nIndex < mTable.size());

	return &mTable[nIndex];
}

SearchItem* SearchList::Current()
{
	VERIFY(!mTable.empty());
	return &mTable[mCurrent];
}

SearchItem* SearchList::Next()
{
	VERIFY(!mTable.empty());

	mCurrent++;

	if (mCurrent >= mTable.size())
	{
		mCurrent = 0;
	}

	return &mTable[mCurrent];
}

SearchItem* SearchList::Previous()
{
	VERIFY(!mTable.empty());

	mCurrent--;

	if (mCurrent < 0)
	{
		mCurrent = mTable.size()-1;
	}

	return &mTable[mCurrent];
}

bool SearchList::IsEmpty() const
{
	for (int i=1; i < mTable.size(); i++)
	{
		if (mTable[i].getMoveNode() != 0)
		{
			return false;
		}
	}

	return true;
}

int SearchList::LastItem() const
{
	VERIFY(!mTable.empty());

	return mTable.size()-1;
}

int SearchList::CurrentIndex() const
{
	VERIFY(!mTable.empty());
	
	return mCurrent;
}

void SearchList::SetInfo(const CString& info, Kind SearchKind)
{
	mstrInfo    = info;
	mSearchKind = SearchKind;
}

CString SearchList::GetInfo()
{
	return mstrInfo;
}

SearchList::Kind SearchList::GetKind()
{
	return mSearchKind;
}

