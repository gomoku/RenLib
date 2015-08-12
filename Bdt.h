// Bdt.h: interface for the Bdt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BDT_H__01A513A2_A1E7_11D4_92A3_444553540000__INCLUDED_)
#define AFX_BDT_H__01A513A2_A1E7_11D4_92A3_444553540000__INCLUDED_

#include "Game.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class MoveList;

class Bdt  
{
public:
	Bdt();

	bool	OpenFile(const CString& strFile);
	bool	Done();
	void	Next();

    bool Create(const CString& fileName);
    void Save(const MoveList& aMoveList);
    void Close();

    Game&	getGame();
	CString	getFilePath();

private:
	bool GetLine (CString& strLine);

    bool GetPlayers(CString& strLine, CString& strBlackPlayer, CString& strWhitePlayer);

    bool GetReverse(CString& strLine, CString& strReverse);

    bool Get5a(CString& strLine, CString& str5a);

    bool GetResult(
        CString& strLine, CString strLeft, CString strRight);

    bool GetTournament(CString& strLine, CString& strTournament);

    void GetInformation(
        const MoveList& aMoveList,
        CString& strBlackPlayer,
        CString& strWhitePlayer,
        CString& strResult,
        CString& str5A,
        CString& strReverse,
        CString& strTournament,
        CString& strComment);

private:
	CStdioFile mGameFile;
	Game	   mGame;
	CString	   mFilePath;
    bool       mDone;
};

#endif // !defined(AFX_BDT_H__01A513A2_A1E7_11D4_92A3_444553540000__INCLUDED_)
