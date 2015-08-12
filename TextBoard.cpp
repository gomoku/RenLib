// TextBoard.cpp: implementation of the TextBoard class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "renlib.h"
#include "TextBoard.h"
#include "MoveList.h"
#include "MoveNode.h"
#include "Utils.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const CString TextBoard::cHeader("----- Text Board -----");

namespace
{
	const CString cPrefix("    ");
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TextBoard::TextBoard()
{
	m_Board[ 0] = "          X=Black   O=White          ";
	m_Board[ 1] = "                                     ";
	m_Board[ 2] = "    A B C D E F G H I J K L M N O    ";
	m_Board[ 3] = " 15 . . . . . . . . . . . . . . . 15 ";
	m_Board[ 4] = " 14 . . . . . . . . . . . . . . . 14 ";
	m_Board[ 5] = " 13 . . . . . . . . . . . . . . . 13 ";
	m_Board[ 6] = " 12 . . . . . . . . . . . . . . . 12 ";
	m_Board[ 7] = " 11 . . . . . . . . . . . . . . . 11 ";
	m_Board[ 8] = " 10 . . . . . . . . . . . . . . . 10 ";
	m_Board[ 9] = "  9 . . . . . . . . . . . . . . . 9  ";
	m_Board[10] = "  8 . . . . . . . . . . . . . . . 8  ";
	m_Board[11] = "  7 . . . . . . . . . . . . . . . 7  ";
	m_Board[12] = "  6 . . . . . . . . . . . . . . . 6  ";
	m_Board[13] = "  5 . . . . . . . . . . . . . . . 5  ";
	m_Board[14] = "  4 . . . . . . . . . . . . . . . 4  ";
	m_Board[15] = "  3 . . . . . . . . . . . . . . . 3  ";
	m_Board[16] = "  2 . . . . . . . . . . . . . . . 2  ";
	m_Board[17] = "  1 . . . . . . . . . . . . . . . 1  ";
	m_Board[18] = "    A B C D E F G H I J K L M N O    ";

	m_strBoard.Empty();
}

TextBoard::~TextBoard()
{

}

void TextBoard::Create (const MoveList& aMoveList)
{
	for (int i = 1; i <= aMoveList.Index(); i++)
	{
		CPoint Coord(aMoveList.Get(i)->getPos());
		const int row = 2 + Coord.y;
		const int col = 2 + 2 * Coord.x;
		const TCHAR marker(i&1 ? 'X' : 'O');
		m_Board[row].SetAt(col, marker);
	}

	for (i = 0; i < NOOFROWS; i++)
	{
		m_strBoard += m_Board[i] + TCHAR(10);
	}

	m_strBoard += cPrefix + cHeader;
	m_strBoard += TCHAR(10);
	m_strBoard += TCHAR(10);

	CString strFirst(aMoveList.Current()->getOneLineComment());

	if (!strFirst.IsEmpty())
	{
		m_strBoard += cPrefix + strFirst;
		m_strBoard += TCHAR(10);
	}

	for (i = 1; i <= aMoveList.Index(); i += 2)
	{
		CString strLine;

		CString strNum;
		strNum.Format("%3d ", i);
		strLine += strNum;

		CPoint Coord(aMoveList.Get(i)->getPos());
		CString strMove(Utils::XCoordinateImage(Coord.x, true, false) + Utils::YCoordinateImage(Coord.y, true));
		strMove.MakeUpper();
		strLine += strMove + (strMove.GetLength() < 3 ? " " : "");

		if (i+1 <= aMoveList.Index())
		{
			CString strNum;
			strNum.Format("%5d ", i+1);
			strLine += strNum;

			CPoint Coord(aMoveList.Get(i+1)->getPos());
			CString strMove(Utils::XCoordinateImage(Coord.x, true, false) + Utils::YCoordinateImage(Coord.y, true));
			strMove.MakeUpper();
			strLine += strMove;
		}

		m_strBoard += cPrefix + strLine + TCHAR(10);
	}

	m_strBoard += TCHAR(10);
}

bool TextBoard::Save(const CString& fileName)
{
	if (!m_file.Open(fileName, CFile::modeCreate | CFile::modeWrite | CFile::typeText))
	{
		CString strMessage(Utils::GetString(IDS_MSG_OPEN_FILE, fileName));
		Utils::ShowMessage(strMessage, Utils::GetString(IDS_CAP_CREATE_TEXT_BOARD), MB_ICONERROR);
		return false;
	}

	m_file.WriteString(m_strBoard);
	m_file.Close();

	return true;
}
