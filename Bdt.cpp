// Bdt.cpp: implementation of the Bdt class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RenLib.h"
#include "Bdt.h"
#include "Utils.h"
#include "MoveList.h"
#include "MoveNode.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//--------------------------------------------------------------------
// const
//--------------------------------------------------------------------

namespace
{
	const char SEPS[] = ",";
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Bdt::Bdt()
: mDone(false)
{
}

//------------------------------------------------------------------------

Game& Bdt::getGame()
{
	return mGame;
}

//------------------------------------------------------------------------

CString Bdt::getFilePath()
{
	return mFilePath;
}

//------------------------------------------------------------------------

bool Bdt::OpenFile(const CString& strFile)
{
	CFileException e;

	if (!mGameFile.Open( strFile, CFile::modeRead, &e))
	{
		return false;
	}

	mFilePath = mGameFile.GetFilePath();

	Next();

	return true;
}

//------------------------------------------------------------------------

bool Bdt::Done()
{
	return mDone;
}

//------------------------------------------------------------------------

void Bdt::Next()
{
	CString strLine;

	if (!GetLine(strLine))
    {
        mDone = true;
        mGameFile.Close();

        return;
    }

    //----------------------

	int index = strLine.Find("=[");

    if (index == -1) return;

    CString strTournament(strLine.Left(index));

    strLine.Delete(0, index + 2);

    index = strLine.Find("]");

    if (index == -1) return;

    strLine.Delete(index);

    CString strBlackPlayer;
    CString strWhitePlayer;
    CString strResult;
    CString strMoves;
    CString str5A;
    CString strReverse;
    CString strComment;

    index = strLine.Find(",");

    if (index != -1)
    {
        strBlackPlayer = strLine.Left(index);
        strLine.Delete(0, index + 1);
    }

    index = strLine.Find(",");

    if (index != -1)
    {
        strWhitePlayer = strLine.Left(index);
        strLine.Delete(0, index + 1);
    }

    index = strLine.Find(",");

    if (index != -1)
    {
        strResult = strLine.Left(index);
        strLine.Delete(0, index + 1);
    }

    index = strLine.Find(",");

    if (index != -1)
    {
        strMoves = strLine.Left(index);
        strLine.Delete(0, index + 1);
    }

    index = strLine.Find(",");

    if (index != -1)
    {
        str5A = strLine.Left(index);
        strLine.Delete(0, index + 1);
    }

    index = strLine.Find(",");

    if (index != -1)
    {
        strReverse = strLine.Left(index);
        strLine.Delete(0, index + 1);
    }

    strComment = strLine;

	Utils::trim(strTournament);
    Utils::trim(strBlackPlayer);
    Utils::trim(strWhitePlayer);
    Utils::trim(strResult);
    Utils::trim(strMoves);
    Utils::trim(str5A);
    Utils::trim(strReverse);
    Utils::trim(strComment);

	mGame.clear();

    if (strTournament = ',')
    {
        strTournament.Empty();
    }

    if (strResult == "=")
    {
        strResult = "0.5 - 0.5";
    }
    else if (strResult == "+")
    {
        strResult = "1 - 0";
    }
    else if (strResult == "-")
    {
        strResult = "0 - 1";
    }

    if (strMoves.GetLength() % 2 == 0)
    {
        for (int i=0; i < strMoves.GetLength(); i += 2)
        {
		    int x;
            int y;
        
            if (!Utils::HexToInt(strMoves[i],   x) ||
                !Utils::HexToInt(strMoves[i+1], y))
            {
                mGame.clear();
                return;
            }

            if (x == 0 && y == 0)
            {
    		    mGame.addPos(0);

                int moveNo = mGame.numberOfMoves();

				CString strPass;
				strPass.Format("%d. Pass", moveNo);
				mGame.addMultiLineComment(moveNo, strPass);
            }
            else if (x == 0 || y == 0)
            {
    		    mGame.addPos(0);

                int moveNo = mGame.numberOfMoves();

				CString strPass;
				strPass.Format("%d. Strange move: x: %d, y: %d", moveNo, x, y);
				mGame.addMultiLineComment(moveNo, strPass);
            }
            else
            {
    		    mGame.addPos(BYTE((15-y) * 16 + x));
            }
        }
	}

	const int lastMove = mGame.numberOfMoves();

	mGame.addOneLineComment(
        lastMove, strBlackPlayer + " - " + strWhitePlayer + " " + strResult);

	mGame.addMultiLineComment(lastMove, "File: " + mGameFile.GetFileTitle());
    mGame.addMultiLineComment(lastMove, "Tournament: " + strTournament);
	mGame.addMultiLineComment(lastMove, "Reverse: " + strReverse);

    if (!str5A.IsEmpty() && str5A[0] >= 'J')
    {
        str5A.SetAt(0, str5A[0]-1);
    }

    mGame.addMultiLineComment(lastMove, "5A: " + str5A);
}

//------------------------------------------------------------------------

bool Bdt::GetLine (CString& strLine)
{
	if (mGameFile.ReadString(strLine))
	{
		Utils::trim(strLine);
        return true;
	}

	return false;
}

//------------------------------------------------------------------------

bool Bdt::Create(const CString& fileName)
{
	if (!mGameFile.Open(fileName, CFile::modeCreate | CFile::modeWrite | CFile::typeText))
	{
		CString strMessage(Utils::GetString(IDS_MSG_OPEN_FILE, fileName));
		Utils::ShowMessage(strMessage, Utils::GetString(IDS_CAP_CREATE_BDT), MB_ICONERROR);
		return false;
	}

    return true;
}

//------------------------------------------------------------------------

bool Bdt::GetPlayers(CString& strLine, CString& strBlackPlayer, CString& strWhitePlayer)
{
    bool result = false;

    int index = strLine.Find("-");

    if (index != -1)
    {
        result = true;

        int index2 = strLine.Find("-", index + 1);

        if (index2 != -1 && strLine[index2 - 1] == ' ' && strLine[index + 1] != ' ')
        {
            index = index2;
        }

        strBlackPlayer = strLine.Left(index);
        strWhitePlayer = strLine.Mid(index + 1);

        int indexLeft = strBlackPlayer.ReverseFind(',');

        if (indexLeft != -1)
        {
            strBlackPlayer = strBlackPlayer.Mid(indexLeft + 1);
        }

        int indexRight = strWhitePlayer.Find(',');

        if (indexRight != -1)
        {
            strWhitePlayer = strWhitePlayer.Left(indexRight);
        }

        strLine.Delete(index + 1, strWhitePlayer.GetLength());
        strLine.Delete(index - strBlackPlayer.GetLength(), strBlackPlayer.GetLength() + 1);

        Utils::trim(strBlackPlayer);
        Utils::trim(strWhitePlayer);
    }

    return result;
}

//------------------------------------------------------------------------

bool Bdt::GetReverse(CString& strLine, CString& strReverse)
{
    bool result = false;
    int  firstPos = 0;

    int index = strLine.Find("R:");

    if (index == -1)
    {
        index = strLine.Find("r:");
    }

    if (index == -1)
    {
        index = strLine.Find("Reverse:");
    }

    if (index != -1)
    {
        firstPos = index;

        while (strLine[index++] != ':');

        for (; index < strLine.GetLength(); index++)
        {
            if (strLine[index] != ' ') break;
        }

        if (index < strLine.GetLength())
        {
            strReverse = strLine[index];
            strReverse.MakeUpper();

            if (strReverse == "-" || strReverse == '?')
            {
                result  = true;
            }
            else if (strReverse == '+' || strReverse == 'R')
            {
                strReverse = 'R';
                result  = true;
            }

            if (result)
            {
                strLine.Delete(firstPos, index + 1 - firstPos);
            }
        }
    }

    return result;
}

//------------------------------------------------------------------------

bool Bdt::Get5a(CString& strLine, CString& str5a)
{
    bool result = false;
    int  firstPos = 0;

    str5a.Empty();

    int index = strLine.Find("5a");

    if (index == -1)
    {
        index = strLine.Find("5A");
    }

    if (index != -1)
    {
        firstPos = index;

        index += 2;

        while (index < strLine.GetLength() &&
               strLine[index] == ' ') index++;

        if (index < strLine.GetLength() &&
            strLine[index] == ':') index++;

        if (index < strLine.GetLength() &&
            strLine[index] == '=') index++;

        while (index < strLine.GetLength() &&
               strLine[index] == ' ') index++;

        if (index < strLine.GetLength() &&
            ((strLine[index] >= 'a' && strLine[index] <= 'p') ||
             (strLine[index] >= 'A' && strLine[index] <= 'P')))
        {
            str5a += strLine[index++];
        }

        if (index < strLine.GetLength() &&
            strLine[index] >= '1' && strLine[index] <= '9')
        {
            str5a += strLine[index++];
        }

        if (index < strLine.GetLength() &&
            strLine[index] >= '0' && strLine[index] <= '9')
        {
            str5a += strLine[index++];
        }

        if (!str5a.IsEmpty())
        {
            strLine.Delete(firstPos, index - firstPos);
            result = true;
        }
    }

    return result;
}

//------------------------------------------------------------------------

bool Bdt::GetTournament(CString& strLine, CString& strTournament)
{
    bool result = false;

    strTournament = ",";

    int index = strLine.Find(",");

    if (index != -1)
    {
        result = true;

        int index2 = strLine.Find(",", index + 1);

        if (index2 != -1)
        {
            strTournament = strLine.Left(index2);
            strLine.Delete(0, index2+1);
        }
        else
        {
            strTournament = strLine;
            strLine.Empty();
        }

        Utils::trim(strTournament);
    }

    return result;
}

//------------------------------------------------------------------------

bool Bdt::GetResult(
    CString& strLine, CString strLeft, CString strRight)
{
    bool result = false;

    int index1 = -1;
    int index2 = -1;
    int firstPos = 0;

    while (true)
    {
        index1 = strLine.Find(strLeft, firstPos);
        
        if (index1 == -1) break;

        firstPos = index1 + strLeft.GetLength();
        index2 = strLine.Find(strRight, firstPos);
        
        if (index2 == -1) continue;

        CString strDelimiter(
            strLine.Mid(firstPos, index2 - firstPos));

        if (strDelimiter.IsEmpty()) continue;

        strDelimiter.Replace('-', ' ');
        strDelimiter.Replace(':', ' ');
        Utils::trim(strDelimiter);

        if (strDelimiter.IsEmpty())
        {
            strLine.Delete(index1, index2 - index1 + strRight.GetLength());
            result = true;
            break;
        }
    }

    return result;
}

//------------------------------------------------------------------------

void Bdt::GetInformation(
    const MoveList& aMoveList,
    CString& strBlackPlayer,
    CString& strWhitePlayer,
    CString& strResult,
    CString& str5A,
    CString& strReverse,
    CString& strTournament,
    CString& strComment)
{
    strBlackPlayer.Empty();
    strWhitePlayer.Empty();
    strResult.Empty();
    str5A = "?";
    strReverse.Empty();
    strComment.Empty();

    CString strOneLineComment(aMoveList.Current()->getOneLineComment());
    strOneLineComment.Replace(TCHAR(13), ' ');
    strOneLineComment.Replace(TCHAR(10), ' ');
    Utils::trim(strOneLineComment);

	CString strMultiLineComment(aMoveList.Current()->getMultiLineComment());
    strMultiLineComment.Replace(TCHAR(13), ' ');
    strMultiLineComment.Replace(TCHAR(10), ' ');
    Utils::trim(strMultiLineComment);

    CString strLine;
    CString strSeparator;

    if (!strOneLineComment.IsEmpty() && !strMultiLineComment.IsEmpty())
    {
        strSeparator = ",";
    }

    strLine = strOneLineComment + strSeparator + strMultiLineComment;

    strLine.Replace(";", ",");

    //
    // Result
    //
    if (GetResult(strLine, "0.5", "0.5") || GetResult(strLine, "0,5", "0,5"))
    {
        strResult = "=";
    }
    else if (GetResult(strLine, "1", "0"))
    {
        strResult = "+";
    }
    else if (GetResult(strLine, "0", "1"))
    {
        strResult = "-";
    }

    //
    // Reverse
    //
    GetReverse(strLine, strReverse);

    //
    // Players
    //
    GetPlayers(strLine, strBlackPlayer, strWhitePlayer);

    //
    // 5A
    //
    Get5a(strLine, str5A);

    //
    // Tournament
    //
    while (strLine.Replace("  ", " ") +
           strLine.Replace(", ", ",") +
           strLine.Replace(" ,", ",") +
           strLine.Replace(",,", ","));

    while (strLine.Left(1) == ',')
    {
        strLine.Delete(0);
    }

    while (strLine.Right(1) == ',')
    {
        strLine.Delete(strLine.GetLength()-1);
    }

    GetTournament(strLine, strTournament);

    //
    // Comment
    //
    if (!strLine.IsEmpty())
    {
        strComment = strLine;
        Utils::trim(strComment);
    }
}

void Bdt::Save(const MoveList& aMoveList)
{
    CString strBlackPlayer;
    CString strWhitePlayer;
    CString strResult;
    CString str5A;
    CString strReverse;
    CString strTournament;
    CString strComment;

    GetInformation(
        aMoveList, strBlackPlayer, strWhitePlayer, strResult,
        str5A, strReverse, strTournament, strComment);

	CString strMoves;
    TCHAR hex;

    for (int i = 1; i < aMoveList.Index(); i++)
	{
		CPoint Coord(aMoveList.Get(i)->getPos());

        Utils::IntToHex(Coord.x, hex);
		strMoves += hex;

        Utils::IntToHex(Coord.y, hex);
		strMoves += hex;
    }

    mGameFile.WriteString(
        strTournament + "=[" + strBlackPlayer + "," + strWhitePlayer + "," + strResult +
        "," + strMoves + "," + str5A + "," + strReverse + "," + strComment + "]" + 
        TCHAR(10));
}

void Bdt::Close()
{
	mGameFile.Close();
}
