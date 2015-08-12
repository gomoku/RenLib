// SearchItem.cpp: implementation of the SearchItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "renlib.h"
#include "SearchItem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SearchItem::SearchItem(MoveNode* moveNode)
: mMoveNode(moveNode),
  mMergeText(NONE),
  mKind(NONE),
  mMatch(0)
{
}

SearchItem::SearchItem(MoveNode* moveNode, int nMatch)
: mMoveNode(moveNode),
  mMergeText(NONE),
  mKind(NONE),
  mMatch(nMatch)
{
}

SearchItem::SearchItem(MoveNode* moveNode, const CString& mergeText, SearchItem::MergeKind kind)
: mMoveNode(moveNode),
  mMergeText(mergeText),
  mKind(kind),
  mMatch(0)
{
}

MoveNode* SearchItem::getMoveNode() const
{
	return mMoveNode;
}

void SearchItem::setMoveNode(MoveNode* ptr)
{
	mMoveNode = ptr;
}

CString SearchItem::getMergeText() const
{
	return mMergeText;
}

void SearchItem::emptyMergeText()
{
	mMergeText.Empty();
}

SearchItem::MergeKind SearchItem::getKind() const
{
	return mKind;
}

int SearchItem::getMatch() const
{
	return mMatch;
}

bool SearchItem::operator==(SearchItem& item)
{
	return
		mMoveNode  == item.mMoveNode  &&
		mMergeText == item.mMergeText &&
		mKind      == item.mKind      &&
		mMatch     == item.mMatch;
}
