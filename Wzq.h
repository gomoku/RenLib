// Wzq.h: interface for the Wzq class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WZQ_H__8C09631B_E972_4EDA_8FDB_C8B62B8A58F7__INCLUDED_)
#define AFX_WZQ_H__8C09631B_E972_4EDA_8FDB_C8B62B8A58F7__INCLUDED_

#include "Game.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Wzq  
{
public:
	Wzq();

	bool	OpenFile(const CString& strFile);

	Game&	getGame();
	CString	getComment();
	CString	getFilePath();

private:
	void	Parse();

private:
	CFile	m_wzqFile;
	DWORD	m_dwRead;

	Game	m_Game;
	CString	m_strComment;
	CString	m_strFilePath;
};

#endif // !defined(AFX_WZQ_H__8C09631B_E972_4EDA_8FDB_C8B62B8A58F7__INCLUDED_)
