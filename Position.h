// Position.h: interface for the Position class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POSITION_H__B52D9462_EF12_11D4_92A3_E1F468F15D25__INCLUDED_)
#define AFX_POSITION_H__B52D9462_EF12_11D4_92A3_E1F468F15D25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Position
{
public:
	class MoveList
	{
	public:
		int    nMoves;
		CPoint Moves[15*15];
	};

	Position();
	virtual ~Position();

	void Init(const MoveList& theMoves, bool findSimilar);
	BYTE Match(const MoveList& theMoves, bool findSimilar);

	static void NormalizeCoord(CPoint& Coord, BYTE nMatch);
	static void TransposeCoord(CPoint& Coord, BYTE nMatch);

private:
	enum
	{
		NOOFTABLES =  8,
		SIZE       = 16
	};

	typedef BYTE Pos[SIZE][SIZE]; // 1 based

	void Rotate(MoveList& theMoves);
	void Reflect(MoveList& theMoves);
	
	bool IsEqual(Pos& positionA, Pos& positionB);
	void Clear(Pos& thePosition);
	void Put(const MoveList& theMoves, Pos& thePosition, bool findSimilar);

	Pos m_Position[NOOFTABLES];
};

#endif // !defined(AFX_POSITION_H__B52D9462_EF12_11D4_92A3_E1F468F15D25__INCLUDED_)
