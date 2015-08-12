// MoveList.h: interface for the MoveList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOVELIST_H__3C2C7489_F3BA_11D4_92A3_FE4B13876025__INCLUDED_)
#define AFX_MOVELIST_H__3C2C7489_F3BA_11D4_92A3_FE4B13876025__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class MoveNode;

class MoveList  
{
public:
	MoveList();
	virtual ~MoveList();

	bool IsEmpty() const;
	bool IsFull() const;

	void SetRoot(MoveNode* pMove);
	void Add(MoveNode* pMove);
	void Swap(int nIndex1, int nIndex2);

	MoveNode* Get(int nIndex) const;
	MoveNode* GetRoot() const;
	MoveNode* Current() const;

	MoveNode* Next();
	MoveNode* Previous();

	void SetIndex(int nIndex);
	void SetRootIndex();
	void Decrement();
	int  Index() const;

	void ClearAll();
	void ClearEnd();

private:
	enum { MAXINDEX = 225 };

	MoveNode*	m_List [MAXINDEX + 1]; // 1 based
	int			m_nIndex;
};

#endif // !defined(AFX_MOVELIST_H__3C2C7489_F3BA_11D4_92A3_FE4B13876025__INCLUDED_)
