// Board.cpp: implementation of the Board class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RenLib.h"
#include "Board.h"
#include "Utils.h"
#include "Registry.h"
#include "MoveNode.h"
#include "External/bestmove.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//----------------------------------------------------------

namespace
{
	const CPoint NullPoint(0, 0);

	const char charMarker[] =
	{
		EMPTYSTONE,
		BLACKSTONE,
		WHITESTONE
	};

	bool isValid(CPoint Pos)
	{
		return
			Pos == NullPoint || (Pos.x >= 1 && Pos.x <= 15 && Pos.y >= 1 && Pos.y <= 15);
	}
}

//----------------------------------------------------------

Board::Board() :
	m_LastMove(0,0),
	m_PreviousVariant(0,0),
    mVc3(0),
    mFindStatus(FIND_IDLE)
{
	Clear();
	ClearWin();

	Registry reg;

    m_EvaluatorSelection =
        (EvaluatorSelection) reg.getInt(Registry::Evaluator, RENJU_CLASS_RENJU_SOLVER);
}

//----------------------------------------------------------

Board::~Board()
{
}

//----------------------------------------------------------

void Board::Clear()
{
	for (int x = 1; x <= 15; x++)
	{
		for (int y = 1; y <= 15; y++)
		{
			Position& Pos     = m_Board[x][y];
			Pos.Marker        = NO_MOVE;
			Pos.Forbidden     = false;
			Pos.pMove         = 0;
			Pos.pChangedOrder = 0;
			Pos.Text.Empty();
		}
	}

	switch (m_EvaluatorSelection)
	{
		case ORIGINAL:
		case RENJU_CLASS_RENJU_SOLVER:
		{
			mEvaluator.Clear();
			break;
		}

		case NONE:
		{
			break;
		}
	}
}

//----------------------------------------------------------

void Board::ClearWin()
{
	for (int x = 1; x <= 15; x++)
	{
		for (int y = 1; y <= 15; y++)
		{
			Position& Pos = m_Board[x][y];

			if (Pos.Win > 0 && Pos.Win <= 225)
			{
				m_strWinMarker[Pos.Win].Empty();
			}

			Pos.Win = 0;
		}
	}
}

//----------------------------------------------------------

bool Board::isEqual(Board& aBoard)
{
	for (int x = 1; x <= 15; x++)
	{
		for (int y = 1; y <= 15; y++)
		{
			if (m_Board[x][y].Marker != aBoard.m_Board[x][y].Marker)
			{
				return false;
			}
		}
	}

	return true;
}

//----------------------------------------------------------

Board::BoardMarker Board::Get(CPoint Pos)
{
	VERIFY(isValid(Pos));
	return m_Board[Pos.x][Pos.y].Marker;
}

//----------------------------------------------------------

BYTE Board::GetNumber(CPoint Pos)
{
	VERIFY(isValid(Pos));
	return m_Board[Pos.x][Pos.y].Number;
}

//----------------------------------------------------------

MoveNode* Board::GetMove(CPoint Pos)
{
	VERIFY(isValid(Pos));

	MoveNode* pMove = m_Board[Pos.x][Pos.y].pMove;
	VERIFY(pMove != 0);
	return pMove;
}

//----------------------------------------------------------

MoveNode* Board::GetChangedOrder(CPoint Pos)
{
	VERIFY(isValid(Pos));

	MoveNode* pChangedOrder = m_Board[Pos.x][Pos.y].pChangedOrder;
	VERIFY(pChangedOrder != 0);
	return pChangedOrder;
}

//----------------------------------------------------------

CString Board::GetText(CPoint Pos)
{
	VERIFY(isValid(Pos));

	return m_Board[Pos.x][Pos.y].Text;
}

//----------------------------------------------------------

bool Board::isForbidden(CPoint Pos)
{
	VERIFY(isValid(Pos));
	return m_Board[Pos.x][Pos.y].Forbidden;
}

//----------------------------------------------------------

bool Board::isBoardText(CPoint Pos)
{
	VERIFY(isValid(Pos));
	return !m_Board[Pos.x][Pos.y].Text.IsEmpty();
}

//----------------------------------------------------------

CString Board::GetWin(CPoint Pos)
{
	VERIFY(isValid(Pos));

	BYTE pos = m_Board[Pos.x][Pos.y].Win;

	if (pos >= 1 && pos <= 225)
	{
		return m_strWinMarker[pos];
	}
	else
	{
		return "";
	}
}

//----------------------------------------------------------

void Board::Set(CPoint Pos, Board::BoardMarker marker, MoveNode* pMove, bool findForbidden)
{
	VERIFY(isValid(Pos));
		
	m_Board[Pos.x][Pos.y].Marker = marker;
	m_Board[Pos.x][Pos.y].pMove  = pMove;
	
	switch (marker)
	{
		case NO_MOVE:
		case BLACK_MOVE:
		case WHITE_MOVE:
		{
			switch (m_EvaluatorSelection)
			{
				case ORIGINAL:
				case RENJU_CLASS_RENJU_SOLVER:
				{
					mEvaluator.AddStone(Pos.x-1, Pos.y-1, charMarker[marker]);
					break;
				}

				case NONE:
				{
					break;
				}
			}

			break;
		}
			
		default:
		{
			break;
		}
	}
}

//----------------------------------------------------------

void Board::SetChangedOrder(CPoint Pos, MoveNode* pChangedOrder)
{
	VERIFY(isValid(Pos));
	m_Board[Pos.x][Pos.y].pChangedOrder = pChangedOrder;
}

//----------------------------------------------------------

void Board::SetNumber(CPoint Pos, BYTE number)
{
	VERIFY(isValid(Pos));
	m_Board[Pos.x][Pos.y].Number = number;
}

//----------------------------------------------------------

void Board::SetText(CPoint Pos, const CString& Text)
{
	VERIFY(isValid(Pos));
	m_Board[Pos.x][Pos.y].Text = Text;
}

//----------------------------------------------------------

void Board::SetLastMove(const CPoint& Point)
{
	m_LastMove = Point;
}

//----------------------------------------------------------

CPoint Board::GetLastMove()
{
	return m_LastMove;
}

//----------------------------------------------------------

void Board::SetPreviousVariant(const CPoint& Point)
{
	m_PreviousVariant = Point;
}

//----------------------------------------------------------

CPoint Board::GetPreviousVariant()
{
	return m_PreviousVariant;
}

//----------------------------------------------------------

void Board::setForbiddenInfo(const MoveList& moveList)
{
	switch (m_EvaluatorSelection)
	{
		case ORIGINAL:
		case RENJU_CLASS_RENJU_SOLVER:
		{
			if (mEvaluator.nForbiddenPoints > 0)
			{
				for (int i=0; i < mEvaluator.nForbiddenPoints; i++)
				{
					const BYTE x = mEvaluator.ptForbidden[i].x + 1;
					const BYTE y = mEvaluator.ptForbidden[i].y + 1;

                    VERIFY(Utils::isValidPoint(CPoint(x, y)));

					m_Board[x][y].Forbidden = true;
				}
			}
			break;
		}

        case NONE:
		{
			break;
		}
	}
}

//----------------------------------------------------------

void Board::doFindVcf()
{
    mFindStatus = FIND_VCF_IN_PROGRESS;

    //call the function in the dll.
    mVc3 = Vc3Solver(mData);

    if (mVc3 == -20001)
    {
        mFindStatus = FIND_IDLE;
		TRACE0("!!! not enough memory\n");
    }
	else if ((mVc3 >= 9900) && (GetLevel() == 0))
	{
        mFindStatus = FOUND_VCF;
	}
	else
	{
        mFindStatus = FOUND_NO_VCF;
	}
}

//----------------------------------------------------------

UINT FindVcfThread(LPVOID pParam)
{
    ((Board*)pParam)->doFindVcf();

    return 0;   // thread completed successfully
}

//----------------------------------------------------------

Board::FindStatus
Board::findVcf(const MoveList& moveList)
{
    mFindStatus = FIND_IDLE;

    switch (m_EvaluatorSelection)
	{
		case ORIGINAL:
		case RENJU_CLASS_RENJU_SOLVER:
		{
            mData[0] = 4;
            mData[1] = 0; // the maximum time in seconds for the computation: char[0]*256 + char[1]
            mData[2] = 0; // BestMove level:1-9, actually this is the search depth; if it is 0 ,try to find VCF.
            mData[3] = 0;
            mData[4] = 0;
            mData[5] = 0;
            mData[6] = 2; // HashTableSize; 0:8M, 1:16M, 2:32M
            mData[7] = 0;

            mData[8] = moveList.Index();

  	        MoveNode* pMove;

            for (int i = 1; i <= moveList.Index(); i++)
	        {
		        pMove = moveList.Get(i);
		        CPoint Point(pMove->getPos());

                int pos = (Point.y - 1) * 15 + (Point.x - 1);

                mData[8 + i] = pos;
	        }

            mFindStatus = FIND_VCF_IN_PROGRESS;
            AfxBeginThread(FindVcfThread, this);

            break;
		}

		case NONE:
		{
			break;
		}
	}

    return mFindStatus;
}

//----------------------------------------------------------

void Board::doFindVct()
{
    mFindStatus = FIND_VCT_IN_PROGRESS;

    //call the function in the dll.
    mVc3 = Vc3Solver(mData);

    if (mVc3 == -20001)
    {
        mFindStatus = FIND_IDLE;
		TRACE0("!!! not enough memory\n");
    }
	else if (mVc3 >= 9900)
	{
        mFindStatus = FOUND_VCT;
	}
	else
	{
        mFindStatus = FOUND_NO_VCT;
	}
}

//----------------------------------------------------------

UINT FindVctThread(LPVOID pParam)
{
    ((Board*)pParam)->doFindVct();

    return 0;   // thread completed successfully
}

//----------------------------------------------------------

void Board::stopFind()
{
    StopThinking();
}

//----------------------------------------------------------

Board::FindStatus
Board::findVct(const MoveList& moveList)
{
    mFindStatus = FIND_IDLE;

    switch (m_EvaluatorSelection)
	{
		case ORIGINAL:
		case RENJU_CLASS_RENJU_SOLVER:
		{
            mData[0] = 4;
            mData[1] = 0; // the maximum time in seconds for the computation: char[0]*256 + char[1]
            mData[2] = 5; // BestMove level:1-9, actually this is the search depth; if it is 0 ,try to find VCF.
            mData[3] = 0;
            mData[4] = 0;
            mData[5] = 0;
            mData[6] = 2; // HashTableSize; 0:8M, 1:16M, 2:32M
            mData[7] = 5; // CollectMakeVCFMoves TYPE 5 is good

            mData[8] = moveList.Index();

  	        MoveNode* pMove;

            for (int i = 1; i <= moveList.Index(); i++)
	        {
		        pMove = moveList.Get(i);
		        CPoint Point(pMove->getPos());

                int pos = (Point.y - 1) * 15 + (Point.x - 1);

                mData[8 + i] = pos;
	        }

            mFindStatus = FIND_VCT_IN_PROGRESS;
            AfxBeginThread(FindVctThread, this);

            break;
		}

		case NONE:
		{
			break;
		}
	}

    return mFindStatus;
}

//----------------------------------------------------------

int Board::setWinTrace(WinMarker marker)
{
	VERIFY(marker >= NUMBER && marker <= LOWER);

	int cnt = 0;

	CString strMarker("1Aa");
	CString strWin(strMarker[marker]);

    switch (m_EvaluatorSelection)
	{
		case ORIGINAL:
		case RENJU_CLASS_RENJU_SOLVER:
		{
            switch (mFindStatus)
            {
                case FOUND_VCF:
                case FOUND_VCT:
                {
			        int numberOfMoves = 0;
            
			        if (mVc3 >= 20000) // found VCF
                    {
				        numberOfMoves = mVc3 - 20000 + 1;
                    }
			        else if (mVc3 >= 9900) // found VCT
			        {
				        numberOfMoves = 1;
			        }

                    for (int i=0; i < numberOfMoves; i++)
                    {
		                int pos = (mData[i] / 15) * 16 + mData[i] % 15 + 1;
                        CPoint point(Utils::PosToPoint(pos));

                        VERIFY(Utils::isValidPoint(point));

			            m_Board[point.x][point.y].Win = ++cnt;

			            m_strWinMarker[cnt] = strWin;
			            Utils::Increment(strWin);
                    }
                    break;
                }

                default:
                {
                    break;
                }
            }

            break;
		}

		case NONE:
		{
			break;
		}
	}

	return cnt;
}

//----------------------------------------------------------

void Board::updateWinTrace(WinMarker marker)
{
	VERIFY(marker >= NUMBER && marker <= LOWER);

	CString strMarker("1Aa");
	CString strWin(strMarker[marker]);

	for (int i=1; i <= 225; i++)
	{
		if (m_strWinMarker[i].IsEmpty())
		{
			break;
		}
		else
		{
			m_strWinMarker[i] = strWin;
			Utils::Increment(strWin);
		}
	}
}

//----------------------------------------------------------

void Board::setEvaluatorSelection(EvaluatorSelection evaluatorSelection)
{
	m_EvaluatorSelection = evaluatorSelection;

	Registry reg;
	reg.setInt(Registry::Evaluator, m_EvaluatorSelection);
}

//----------------------------------------------------------

Board::EvaluatorSelection Board::getEvaluatorSelection()
{
	return m_EvaluatorSelection;
}

//----------------------------------------------------------

bool Board::canShowForbiddenPoints()
{
	return
		m_EvaluatorSelection == ORIGINAL ||
		m_EvaluatorSelection == RENJU_CLASS_RENJU_SOLVER;
}

//----------------------------------------------------------

bool Board::canFindVcf()
{
	return
		(m_EvaluatorSelection == ORIGINAL ||
		 m_EvaluatorSelection == RENJU_CLASS_RENJU_SOLVER) &&
         mFindStatus == FIND_IDLE;
}

//----------------------------------------------------------

bool Board::canFindVct()
{
	return
		(m_EvaluatorSelection == ORIGINAL ||
		 m_EvaluatorSelection == RENJU_CLASS_RENJU_SOLVER) &&
         mFindStatus == FIND_IDLE;
}

//----------------------------------------------------------

Board::FindStatus
Board::checkFindStatus(CString& strInfo)
{
    strInfo.Empty();

    switch (mFindStatus)
    {
        case FIND_VCF_IN_PROGRESS:
	    {
            if (GetLevel() > 0)
            {
                StopThinking();
            }
            else
            {
                strInfo.Format(
                    Utils::GetString(IDS_RENJU_SOLVER_VCF),
                    GetNodeNum());
            }

            break;
	    }

        case FIND_VCT_IN_PROGRESS:
	    {
            strInfo.Format(
                Utils::GetString(IDS_RENJU_SOLVER_VCT), 
                GetNodeNum(),
                GetLevel());

            break;
	    }
    }

    return mFindStatus;
}

//----------------------------------------------------------

void Board::resetFindStatus()
{
    mFindStatus = FIND_IDLE;
}

//----------------------------------------------------------

bool Board::isFindActive()
{
    return
        mFindStatus == FIND_VCF_IN_PROGRESS ||
        mFindStatus == FIND_VCT_IN_PROGRESS;
}

//----------------------------------------------------------

bool Board::isFindVctActive()
{
    return mFindStatus == FIND_VCT_IN_PROGRESS;
}

//----------------------------------------------------------

bool Board::isFindVcfActive()
{
    return mFindStatus == FIND_VCF_IN_PROGRESS;
}
