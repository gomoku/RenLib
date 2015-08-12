// MoveList.cpp: implementation of the MoveList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RenLib.h"
#include "MoveList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MoveList::MoveList()
{
	ClearAll();
}

MoveList::~MoveList()
{
}

void MoveList::ClearAll()
{
	for (int i=0; i <= MAXINDEX; i++)
	{
		m_List[i] = 0;
	}
	
	m_nIndex = -1;
}

void MoveList::ClearEnd()
{
	for (int i = m_nIndex + 1; i <= MAXINDEX; i++)
	{
		m_List[i] = 0;
	}
}

bool MoveList::IsEmpty() const
{
	return m_nIndex == -1;
}

bool MoveList::IsFull() const
{
	return m_nIndex == MAXINDEX;
}

void MoveList::SetRoot(MoveNode* pMove)
{
	VERIFY(IsEmpty());
	Add(pMove);
}

void MoveList::Add(MoveNode* pMove)
{
	VERIFY(m_nIndex < MAXINDEX);
	m_List[++m_nIndex] = pMove;
}

void MoveList::Swap(int nIndex1, int nIndex2)
{
	VERIFY(nIndex1 >= 1 && nIndex1 <= m_nIndex);
	VERIFY(nIndex2 >= 1 && nIndex2 <= m_nIndex);

	MoveNode* temp  = m_List[nIndex1];
	m_List[nIndex1] = m_List[nIndex2];
	m_List[nIndex2] = temp;
}

MoveNode* MoveList::GetRoot() const
{
	VERIFY(!IsEmpty());
	return m_List[0];
}

MoveNode* MoveList::Get(int nIndex) const
{
	VERIFY(!IsEmpty());
	VERIFY(nIndex >= 0 && nIndex <= MAXINDEX);
	return m_List[nIndex];
}

MoveNode* MoveList::Current() const
{
	VERIFY(!IsEmpty());
	return m_List[m_nIndex];
}

MoveNode* MoveList::Next()
{
	if (m_nIndex < MAXINDEX)
	{
		return m_List[m_nIndex + 1];
	}
	else
	{
		return 0;
	}
}

MoveNode* MoveList::Previous()
{
	if (m_nIndex > 0)
	{
		return m_List[m_nIndex - 1];
	}
	else
	{
		return 0;
	}
}

void MoveList::SetIndex(int nIndex)
{
	VERIFY(nIndex >= 0 && nIndex <= m_nIndex);
	m_nIndex = nIndex;
}

void MoveList::SetRootIndex()
{
	SetIndex(0);
}

void MoveList::Decrement()
{
	VERIFY(m_nIndex > 0);
	m_nIndex--;
}

int MoveList::Index() const
{
	return m_nIndex;
}
