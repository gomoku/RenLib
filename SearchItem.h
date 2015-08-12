// SearchItem.h: interface for the SearchItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEARCHITEM_H__BFA4E822_E43A_11D6_92A3_0000E89F396C__INCLUDED_)
#define AFX_SEARCHITEM_H__BFA4E822_E43A_11D6_92A3_0000E89F396C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class MoveNode;

class SearchItem  
{
public:
	enum MergeKind
	{
		ONE_LINE_COMMENT,
		MULTI_LINE_COMMENT,
		BOARD_TEXT,
		NONE
	};

	explicit SearchItem(MoveNode* moveNode);
	SearchItem(MoveNode* moveNode, int nMatch);
	SearchItem(MoveNode* moveNode, const CString& mergeText, MergeKind kind);

	MoveNode* getMoveNode() const;
	void setMoveNode(MoveNode* ptr);

	CString getMergeText() const;

	void emptyMergeText();

	MergeKind getKind() const;

	int getMatch() const;

	bool operator==(SearchItem& item);

private:
	MoveNode* mMoveNode;
	CString   mMergeText;
	MergeKind mKind;
	int       mMatch;
};

#endif // !defined(AFX_SEARCHITEM_H__BFA4E822_E43A_11D6_92A3_0000E89F396C__INCLUDED_)
