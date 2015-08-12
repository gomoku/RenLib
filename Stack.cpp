// Stack.cpp: implementation of the Stack class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RenLib.h"
#include "Stack.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Stack::Stack() :
	m_nIndex(0)
{
	const Item NullItem = {0, NULL};

	for (int i=0; i < SIZE; i++)
	{
		m_Stack[i] = NullItem;
	}
}

Stack::~Stack()
{
}

bool Stack::IsEmpty()
{
	return (m_nIndex == 0);
}

void Stack::Push(int nMove, MoveNode* pMove)
{
	VERIFY(m_nIndex < SIZE);

	m_Stack[m_nIndex].nMove = nMove;
	m_Stack[m_nIndex].pMove = pMove;
	m_nIndex++;
}

void Stack::Pop(int& nMove, MoveNode*& pMove)
{
	VERIFY(m_nIndex > 0);

	m_nIndex--;
	nMove = m_Stack[m_nIndex].nMove;
	pMove = m_Stack[m_nIndex].pMove;
}

void Stack::Push(MoveNode* pMove)
{
	VERIFY(m_nIndex < SIZE);

	m_Stack[m_nIndex].nMove = 0;
	m_Stack[m_nIndex].pMove = pMove;
	m_nIndex++;
}

void Stack::Pop(MoveNode*& pMove)
{
	VERIFY(m_nIndex > 0);

	m_nIndex--;
	pMove = m_Stack[m_nIndex].pMove;
}

void Stack::Push(int nMove)
{
	VERIFY(m_nIndex < SIZE);

	m_Stack[m_nIndex].nMove = nMove;
	m_Stack[m_nIndex].pMove = NULL;

	m_nIndex++;
}

void Stack::Pop(int& nMove)
{
	VERIFY(m_nIndex > 0);

	m_nIndex--;
	nMove = m_Stack[m_nIndex].nMove;
}
