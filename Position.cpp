// Position.cpp: implementation of the Position class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RenLib.h"
#include "Position.h"
#include "Utils.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace
{
    const CPoint NullPoint(0, 0);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Position::Position()
{
}

Position::~Position()
{
}

bool Position::IsEqual(Pos& positionA, Pos& positionB)
{
	for (int x=0; x < SIZE; x++)
	{
		for (int y=0; y < SIZE; y++)
		{
			if (positionA[x][y] != positionB[x][y])
			{
				return false;
			}
		}
	}

	return true;
}

void Position::Clear(Pos& thePosition)
{
	for (int x=0; x < SIZE; x++)
	{
		for (int y=0; y < SIZE; y++)
		{
			thePosition[x][y] = 0;
		}
	}
}

void Position::Put(const MoveList& theMoves, Pos& thePosition, bool findSimilar)
{
	const int cBlack = 1;
	const int cWhite = 2;

	MoveList myMoves = theMoves;

	if (findSimilar)
	{
		CPoint smallest(15, 15);

		for (int i=0; i < myMoves.nMoves; i++)
		{
			if (smallest.x > myMoves.Moves[i].x)
			{
				smallest.x = myMoves.Moves[i].x;
			}

			if (smallest.y > myMoves.Moves[i].y)
			{
				smallest.y = myMoves.Moves[i].y;
			}
		}

		smallest -= CPoint(1, 1);
        VERIFY(Utils::isValidPoint(smallest));

		for (i=0; i < myMoves.nMoves; i++)
		{
			myMoves.Moves[i] -= smallest;
		}
	}

	for (int i=0; i < myMoves.nMoves; i++)
	{
		const CPoint myPoint(myMoves.Moves[i]);

		VERIFY(myPoint == NullPoint || Utils::isValidPoint(myPoint));

		thePosition[myPoint.x][myPoint.y] = ((i & 1) == 0 ? cBlack : cWhite);
	}
}

void Position::Rotate(MoveList& theMoves)
{
	for (int i=0; i < theMoves.nMoves; i++)
	{
		const CPoint myPoint(theMoves.Moves[i]);
		
        if (myPoint != NullPoint)
        {
		    // Rotate +90 degrees
		    CPoint coord(CPoint(8 - myPoint.y, myPoint.x - 8));

		    coord += CPoint(8,8);
            VERIFY(Utils::isValidPoint(coord));
		    theMoves.Moves[i] = coord;
        }
	}
}

void Position::Reflect(MoveList& theMoves)
{
	for (int i=0; i < theMoves.nMoves; i++)
	{
        if (theMoves.Moves[i] != NullPoint)
        {
		    // Reflect 8th row
		    const CPoint myPoint(theMoves.Moves[i].x, 16 - theMoves.Moves[i].y);

		    VERIFY(Utils::isValidPoint(myPoint));

            theMoves.Moves[i] = myPoint;
	    }
	}
}

void Position::Init(const MoveList& theMoves, bool findSimilar)
{
	// Clear position tables
	for (int i=0; i < NOOFTABLES; i++)
	{
		Clear(m_Position[i]);
	}

	// Setup the tables
	MoveList myMoves = theMoves;

	// 0 degrees
	Put(myMoves, m_Position[0], findSimilar);

	// +90 degrees
	Rotate(myMoves);
	Put(myMoves, m_Position[1], findSimilar);

	// +180 degrees
	Rotate(myMoves);
	Put(myMoves, m_Position[2], findSimilar);

	// +270 degrees
	Rotate(myMoves);
	Put(myMoves, m_Position[3], findSimilar);

	myMoves = theMoves;
	Reflect(myMoves);

	// 0 degrees, Reflect 
	Put(myMoves, m_Position[4], findSimilar);

	// +90 degrees, Reflect 
	Rotate(myMoves);
	Put(myMoves, m_Position[5], findSimilar);

	// +180 degrees, Reflect 
	Rotate(myMoves);
	Put(myMoves, m_Position[6], findSimilar);

	// +270 degrees, Reflect 
	Rotate(myMoves);
	Put(myMoves, m_Position[7], findSimilar);
}

BYTE Position::Match(const MoveList& theMoves, bool findSimilar)
{
	Pos myPosition;
	Clear(myPosition);

	Put(theMoves, myPosition, findSimilar);

	for (int i=0; i < NOOFTABLES; i++)
	{
		if (IsEqual(myPosition, m_Position[i]))
		{
			return i+1;
		}
	}

	return 0;
}

void Position::NormalizeCoord(CPoint& Coord, BYTE nMatch)
{
	if (nMatch >= 1 && nMatch <= 8 && Coord != NullPoint)
	{
		switch (nMatch)
		{
			case 1:
			case 5:
			{
				Coord = CPoint(Coord.x - 8, Coord.y - 8);
				break;
			}

			case 2:
			case 6:
			{
				Coord = CPoint(Coord.y - 8, 8 - Coord.x);
				break;
			}

			case 3:
			case 7:
			{
				Coord = CPoint(8 - Coord.x, 8 - Coord.y);
				break;
			}
		
			case 4:
			case 8:
			{
				Coord = CPoint(8 - Coord.y, Coord.x - 8);
				break;
			}
		}

		Coord += CPoint(8,8);

		if (nMatch > 4)
		{
			// Reflect 8th row
			Coord.y = 16 - Coord.y;
		}

        VERIFY(Utils::isValidPoint(Coord));
	}
}

void Position::TransposeCoord(CPoint& Coord, BYTE nMatch)
{
	if (nMatch >= 1 && nMatch <= 8 && Coord != NullPoint)
	{
		if (nMatch > 4)
		{
			// Reflect 8th row
			Coord.y = 16 - Coord.y;
		}

		switch (nMatch)
		{
			case 1:
			case 5:
			{
				Coord = CPoint(Coord.x - 8, Coord.y - 8);
				break;
			}

			case 2:
			case 6:
			{
				Coord = CPoint(8 - Coord.y, Coord.x - 8);
				break;
			}

			case 3:
			case 7:
			{
				Coord = CPoint(8 - Coord.x, 8 - Coord.y);
				break;
			}
			
			case 4:
			case 8:
			{
				Coord = CPoint(Coord.y - 8, 8 - Coord.x);
				break;
			}
		}

		Coord += CPoint(8,8);

        VERIFY(Utils::isValidPoint(Coord));
	}
}