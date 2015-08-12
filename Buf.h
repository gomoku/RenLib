// Buf.h: interface for the Buf class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BUF_H__01A513A2_A1E7_11D4_92A3_444553540000__INCLUDED_)
#define AFX_BUF_H__01A513A2_A1E7_11D4_92A3_444553540000__INCLUDED_

#include "Game.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class MoveList;

class Buf  
{
public:
	Buf();

	bool	OpenFile(const CString& strFile);
	bool	Done();
	void	Next();

    bool Create(const CString& fileName);
    void Save(const MoveList& aMoveList);
    void Close();

public:
	Game&	getGame();
	CString	getFilePath();

private:
	CFile	m_bufFile;
	DWORD	m_dwRead;
	int		m_nRecord;
	int		m_nNoMoves;

	Game	m_Game;
	CString	m_strFilePath;
};

#endif // !defined(AFX_BUF_H__01A513A2_A1E7_11D4_92A3_444553540000__INCLUDED_)

