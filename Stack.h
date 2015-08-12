// Stack.h: interface for the Stack class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STACK_H__3C2C7482_F3BA_11D4_92A3_FE4B13876025__INCLUDED_)
#define AFX_STACK_H__3C2C7482_F3BA_11D4_92A3_FE4B13876025__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MoveNode.h"

class Stack  
{
public:
	Stack();
	virtual ~Stack();

	bool IsEmpty();

	void Push(int nMove, MoveNode*  pMove);
	void Pop(int& nMove, MoveNode*& pMove);

	void Push(MoveNode*  pMove);
	void Pop(MoveNode*& pMove);

	void Push(int nMove);
	void Pop(int& nMove);

private:
	struct Item
	{
		int       nMove;
		MoveNode* pMove;
	};

	enum { SIZE = 225 };
	Item	m_Stack [SIZE];
	int		m_nIndex;
};

#endif // !defined(AFX_STACK_H__3C2C7482_F3BA_11D4_92A3_FE4B13876025__INCLUDED_)
