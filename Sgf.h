#if !defined(AFX_SGF_H__INCLUDED_)
#define AFX_SGF_H__INCLUDED_

#include "Game.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Sgf  
{
public:
	Sgf();

	bool	OpenFile(const CString& strFile, bool multipleGames);

	Game&	getGame();
	CString	getFilePath();

    static bool equalComment(const CString& strComment1, const CString& strComment2);

private:
	void	Parse();
	CString PreProcess(const CString& strContent);
	CString FindString(const CString& strContent, const CString& strBegin, const CString& strEnd);
	CString ParseCommand(const CString& strContent, const CString& strCmd);

    static void swapComment(CString& strComment);

private:
	CStdioFile m_sgfFile;

	Game	   m_Game;
	CString	   m_strFilePath;
	bool       mMultipleGames;
};

#endif // !defined(AFX_SGF_H__INCLUDED_)
