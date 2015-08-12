#if !defined(FORBIDDENPOINTFINDER_H_INCLUDED_)
#define FORBIDDENPOINTFINDER_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ForbiddenPointFinder.h : header file
//

#define BOARDSIZE 15
#define BLACKSTONE 'X'
#define WHITESTONE 'O'
#define EMPTYSTONE '.'
#define BLACKFIVE 0
#define WHITEFIVE 1
#define BLACKFORBIDDEN 2

class CForbiddenPointFinder
{
public:
	int	nForbiddenPoints;
	CPoint ptForbidden[BOARDSIZE * BOARDSIZE];
	
private:
	char cBoard[BOARDSIZE+2][BOARDSIZE+2];

public:
	CForbiddenPointFinder();
	virtual ~CForbiddenPointFinder();

	void Clear();
	int  AddStone(int x, int y, char cStone);

private:
	void SetStone(int x, int y, char cStone);
	BOOL IsFive(int x, int y, int nColor);
	BOOL IsOverline(int x, int y);
	BOOL IsFive(int x, int y, int nColor, int nDir);
	BOOL IsFour(int x, int y, int nColor, int nDir);
	int  IsOpenFour(int x, int y, int nColor, int nDir);
	BOOL IsOpenThree(int x, int y, int nColor, int nDir	);
	BOOL IsDoubleFour(int x, int y);
	BOOL IsDoubleThree(int x, int y);

	void FindForbiddenPoints();
};

#endif // !defined(FORBIDDENPOINTFINDER_H_INCLUDED_)