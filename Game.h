// Game.h: interface for the Game class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAME_H__768479C2_8105_11D4_92A3_BABD602D7A25__INCLUDED_)
#define AFX_GAME_H__768479C2_8105_11D4_92A3_BABD602D7A25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "StringEx.h"

class Game  
{
public:
	enum Kind
	{
		NONE,
		PBEM,
		PLAYSITE,
		TEXTBOARD,
		BLIP,
		KURNIK,
		RENJUCLASS
	};

	enum Result
	{
		VALID,
		OPEN_FILE_ERROR,
		INVALID_KIND,
		LESS_THAN_5_MOVES,
		SAME_COORDINATE
	};

	class Node
	{
	public:
		Node();

		BYTE getPos();
		void setPos(BYTE pos);

		CString getOneLineComment();
		void setOneLineComment(const CString& comment);

		CString getMultiLineComment();
		void setMultiLineComment(const CString& comment);

		bool hasMoreBoardTexts();
		void nextBoardText(BYTE& coord, CString& boardText);
		void addBoardText(const BYTE& coord, const CString& boardText);

	private:
		BYTE    mPos;
		CString mOneLineComment;
		CString mMultiLineComment;

		std::vector<BYTE>    mCoord;
		std::vector<CString> mBoardText;
		int                  mIndex;
	};

public:
	Game();

    bool open(const CString& strFile, bool multipleGames);
	Result addGame();
    void close();

    Result addWebGame(CStringEx& strPage);

	void clear();

	void addPos(BYTE pos);

	void AddMove(CString strMove);

	void addOneLineComment(int moveNo, const CString& comment);
	void addMultiLineComment(int moveNo, const CString& comment);
	void addBoardText(int moveNo, const BYTE& coord, const CString& boardText);

	Result CheckMoves();

	CString getFilePath();

	int numberOfMoves();

	bool  hasMoreMoves();
	Node* nextMove();

private:
	void setKind(Kind kind);

	bool GetLine (CString& strLine);
	void getToken(CString& strLine, CString& strToken);

	void ParsePBeM();
	void ParsePlaySite();
	void ParseTextBoard();
	void ParseBlip(CString& strLine);
	void ParseKurnik(CString& strLine);
	void ParseRenjuclass(CString& strLine);

	Result parseGame();
	Result parseWebGame(CStringEx& strPage);
	Result parseApplet(CStringEx& strPage);
	Result parseRenjuclassDiagram(CString& strPage);
	void removeHtml(CString& strText);
	CString replaceEvery(CString& strTarget, const CString& strOccurence, const CString& strWith);
	CString trim(CString& strTarget, const CString& strOccurence);
	CPoint MoveToPoint(CString strMove);
	
private:
	std::vector<Node> mMoves;
	int               mIndex;
	CStdioFile        mGameFile;
	Kind              mKind;
	CString           mResult;
	CString           mFilePath;
	bool              mMultipleGames;
};

#endif // !defined(AFX_GAME_H__768479C2_8105_11D4_92A3_BABD602D7A25__INCLUDED_)
