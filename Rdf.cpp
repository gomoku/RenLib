// Rdf.cpp: implementation of the Rdf class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "renlib.h"
#include "Rdf.h"
#include "Utils.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace
{
    CString cFile("File: ");
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Rdf::Rdf()
{
}

//------------------------------------------------------------------------

Game& Rdf::getGame()
{
	return mGame;
}

//------------------------------------------------------------------------

CString Rdf::getFilePath()
{
	return mstrFilePath;
}

//------------------------------------------------------------------------

bool Rdf::OpenFile(const CString& strFile)
{
	CFileException e;

	if (!mGameFile.Open(strFile, CFile::modeRead, &e))
	{
		return false;
	}

	mstrFilePath = mGameFile.GetFilePath();

	Parse();

	mGameFile.Close();

	return true;
}

//------------------------------------------------------------------------

void Rdf::Parse()
{
	mGame.clear();

	CString strLine;
	mGameFile.ReadString(strLine);

    //----------------------

	int index1 = strLine.Find(":RDFA");

    if (index1 == -1) return;

	int index2 = strLine.Find(":", index1 + 1);

    if (index2 == -1) return;

	int index3 = strLine.Find("!", index2 + 1);

    if (index3 == -1) return;

    index2++;

    CString str5A;
    CString strMoves(strLine.Mid(index2, index3 - index2));

    CPoint Coord;

    while (strMoves.GetLength() >= 4)
    {
        CString strNumber(strMoves.Mid(0, 2));
        CString strCoord(strMoves.Mid(2, 2));

        if (str5A.IsEmpty() && strNumber == "A5" &&
            strCoord[0] >= 'a' && strCoord[0] <= 'o' &&
            strCoord[1] >= 'a' && strCoord[1] <= 'o')
        {
            Coord.x = strCoord[0] - 'a' + 1;
            Coord.y = strCoord[1] - 'a' + 1;

            str5A =
                Utils::XCoordinateImage(Coord.x, true, false) + 
                Utils::YCoordinateImage(Coord.y, true);
        }
        else
        {
            Coord.x = strCoord[0] - 'A' + 1;
            Coord.y = strCoord[1] - 'A' + 1;
            
            mGame.addPos(Utils::PointToPos(Coord));
        }

        strMoves.Delete(0, 4);
    }

	const int lastMove = mGame.numberOfMoves();

	mGame.addMultiLineComment(lastMove, "File: " + mGameFile.GetFileTitle());
    mGame.addMultiLineComment(lastMove, "5A: " + str5A);
}
