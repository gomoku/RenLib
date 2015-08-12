// Board.h: interface for the Board class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BOARD_H__5B4D9628_FD87_11D3_92A3_F35A49B42825__INCLUDED_)
#define AFX_BOARD_H__5B4D9628_FD87_11D3_92A3_F35A49B42825__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "External/ForbiddenPointFinder.h"
#include "MoveList.h"

class MoveNode;

class Board  
{

public:
	enum EvaluatorSelection
	{
		ORIGINAL,
		RENJU_CLASS_RENJU_SOLVER,
		NONE
	};

	enum BoardMarker
	{
		NO_MOVE,		//
		BLACK_MOVE,		// charMarker[0,1,2]
		WHITE_MOVE,		// 
		BLACK_VARIANT,
		WHITE_VARIANT,
		BLACK_AUTOLINKVARIANT,
		WHITE_AUTOLINKVARIANT,
		BLACK_MOVEORDERVARIANT,
		WHITE_MOVEORDERVARIANT,
		PASS
	};

	enum WinMarker
	{
		NUMBER,
		UPPER,
		LOWER
	};

	enum FindStatus
	{
		FIND_IDLE,
		FIND_VCF_IN_PROGRESS,
        FOUND_VCF,
        FOUND_NO_VCF,
		FIND_VCT_IN_PROGRESS,
        FOUND_VCT,
        FOUND_NO_VCT
	};

public:
	Board();
	virtual ~Board();

	void Clear();
	void ClearWin();

	bool isEqual(Board& aBoard);

	BoardMarker Get(CPoint Pos);
	BYTE		GetNumber (CPoint Pos);
	CString		GetText (CPoint Pos);
	bool		isForbidden(CPoint Pos);
	bool		isBoardText(CPoint Pos);
	CString		GetWin(CPoint Pos);

	MoveNode* GetMove(CPoint Pos);
	MoveNode* GetChangedOrder(CPoint Pos);

	void Set (CPoint Pos, BoardMarker marker, MoveNode* pMove = 0, bool findForbidden = false);

	void SetChangedOrder(CPoint Pos, MoveNode* pChangedOrder);

	void SetNumber (CPoint Pos, BYTE number);
	void SetText (CPoint Pos, const CString& Text);

	void SetLastMove (const CPoint& Point);
	CPoint GetLastMove();

	void SetPreviousVariant (const CPoint& Point);
	CPoint GetPreviousVariant();

	void setEvaluatorSelection(EvaluatorSelection evaluatorSelection);
	EvaluatorSelection getEvaluatorSelection();

	void setForbiddenInfo(const MoveList& moveList);
	
	FindStatus findVcf(const MoveList& moveList);
	FindStatus findVct(const MoveList& moveList);

	int  setWinTrace(WinMarker marker);
	void updateWinTrace(WinMarker marker);

	bool canShowForbiddenPoints();
	bool canFindVcf();
	bool canFindVct();

    void doFindVcf();
    void doFindVct();
    void stopFind();

    FindStatus checkFindStatus(CString& strInfo);
    void resetFindStatus();
    bool isFindActive();
    bool isFindVctActive();
    bool isFindVcfActive();

private:

	class Position
	{
	public:
		BoardMarker Marker;
		BYTE		Number;
		BYTE		Win;
		bool		Forbidden;
		CString		Text;
		MoveNode*	pMove;
		MoveNode*	pChangedOrder;
	};

	Position	m_Board[16][16];
	CPoint		m_LastMove;
	CPoint		m_PreviousVariant;

    unsigned char mData[260];
    int           mVc3;
    FindStatus    mFindStatus;

	EvaluatorSelection m_EvaluatorSelection;

	CForbiddenPointFinder mEvaluator;

	CString		m_strWinMarker[226];

};

#endif // !defined(AFX_BOARD_H__5B4D9628_FD87_11D3_92A3_F35A49B42825__INCLUDED_)
