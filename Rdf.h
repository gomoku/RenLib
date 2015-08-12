// Rdf.h: interface for the Rdf class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RDF_H__D9D1757E_2A78_4AC6_AF64_D3AB73E17A05__INCLUDED_)
#define AFX_RDF_H__D9D1757E_2A78_4AC6_AF64_D3AB73E17A05__INCLUDED_

#include "Game.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Rdf  
{
public:
	Rdf();

	bool	OpenFile(const CString& strFile);

	Game&	getGame();
	CString	getFilePath();

private:
	void	Parse();

private:
	CStdioFile mGameFile;

	Game	   mGame;
	CString	   mstrFilePath;
};

#endif // !defined(AFX_RDF_H__D9D1757E_2A78_4AC6_AF64_D3AB73E17A05__INCLUDED_)
