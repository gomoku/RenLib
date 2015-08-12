// Sgf.cpp: implementation of the Sgf class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "renlib.h"
#include "Sgf.h"
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

Sgf::Sgf()
: mMultipleGames(false)
{
}

//------------------------------------------------------------------------

Game& Sgf::getGame()
{
	return m_Game;
}

//------------------------------------------------------------------------

CString Sgf::getFilePath()
{
	return m_strFilePath;
}

//------------------------------------------------------------------------

bool Sgf::OpenFile(const CString& strFile, bool multipleGames)
{
	mMultipleGames = multipleGames;

	CFileException e;

	if (!m_sgfFile.Open(strFile, CFile::modeRead, &e))
	{
		return false;
	}

	m_strFilePath = m_sgfFile.GetFilePath();

	Parse();

	m_sgfFile.Close();

	return true;
}

//------------------------------------------------------------------------

void Sgf::Parse()
{
	m_Game.clear();
	
	CString strContent;
	CString strLine;

	while (m_sgfFile.ReadString(strLine))
	{
		strContent += strLine;
	}
	
	strContent = PreProcess(strContent);	// Extract only one game from the file
	
	CString strGameInfo = FindString(strContent, ";", ";");
	
	CString strPlayerB = ParseCommand(strGameInfo, "PB");
	CString strPlayerW = ParseCommand(strGameInfo, "PW");
	CString str5A      = ParseCommand(strGameInfo, "5A");
	CString strGN      = ParseCommand(strGameInfo, "GN");
	CString strResult  = ParseCommand(strGameInfo, "RE");
	CString strDate    = ParseCommand(strGameInfo, "DT");

	strResult.MakeUpper();								// Translate the result

	if (strResult == _T("B+DRAW"))
	{
		strResult = _T("0.5-0.5");
	}
	else if (strResult.Find("B+") != -1)
	{
		strResult = _T("1-0");
	}
	else if (strResult.Find("W+") != -1)
	{
		strResult = _T("0-1");
	}
	
	int n = strGN.Find(":");

	if (n != -1)
	{
		strGN = strGN.Right(strGN.GetLength() - n - 1);		// Remove the player info from GN
	}

	strGN.Replace("(Rated)", "");
	strGN.Replace("(Free)", "");
	strGN.Replace("(Teach)", "");
	Utils::trim(strGN);

	CString strTiming;

	n = strGN.Find("Timing:");
	
	if (n != -1)
	{
		strTiming = strGN.Right(strGN.GetLength() - n);	// Extract timing info
		Utils::trim(strTiming);

		strGN = strGN.Left(n);                          // Extract player info
		Utils::trim(strGN);
	}

	if (!str5A.IsEmpty())			// for RIF and Yamaguchi rule
	{
		str5A.MakeUpper();

		if ((str5A[1] >= '1') && (str5A[1] <= '9'))		// Yamaguchi rule
		{
			str5A = CString("5A: ") + str5A;
		}
		else											// RIF rule
		{
			str5A.Format("5A: %c%d", str5A[0], 15-(str5A[1]-'A'));		
		}
	}

	CString strMoveInfo = strContent.Right(strContent.GetLength() - strGameInfo.GetLength());

	while (strMoveInfo.GetLength() > 4)
	{
		CString strMove   = FindString(strMoveInfo, ";", ";");
		CString strMoveBW = ParseCommand(strMove, ";B");
		
		if (strMoveBW.IsEmpty())
		{
			strMoveBW = ParseCommand(strMove, ";W");
		}
		
		if (!strMoveBW.IsEmpty())
		{
			strMoveBW.MakeUpper();

			if (strMoveBW == _T("TT"))		// TT = pass
			{
				m_Game.addPos(0);

				int moveNo = m_Game.numberOfMoves();

				CString strPass;
				strPass.Format("%d. Pass", moveNo);
				m_Game.addMultiLineComment(moveNo, strPass);
			}
			else
			{
				m_Game.addPos((strMoveBW[1]-'A') * 16 + strMoveBW[0]-'A'+1);
			}
		}
		else
		{
			break;
		}

		if (strMove.GetLength() <= strMoveInfo.GetLength())
		{
			strMoveInfo = strMoveInfo.Right(strMoveInfo.GetLength() - strMove.GetLength());
		}
	}

	const int lastMove = m_Game.numberOfMoves();

	const CString strResultComment(strPlayerB + " - " + strPlayerW + " " + strResult);
	const CString strDateTimeComment("Date: " + strDate);

	m_Game.addOneLineComment(lastMove, strResultComment);

	m_Game.addMultiLineComment(lastMove, cFile + m_sgfFile.GetFileTitle());
	m_Game.addMultiLineComment(lastMove, strDateTimeComment);

	if (!str5A.IsEmpty())
	{
		m_Game.addMultiLineComment(lastMove, str5A);
	}

	m_Game.addMultiLineComment(lastMove, strGN);

	if (!strTiming.IsEmpty())
	{
		m_Game.addMultiLineComment(lastMove, strTiming);
	}

	if (!mMultipleGames)
	{
		m_Game.addOneLineComment(1, strResultComment);

		m_Game.addMultiLineComment(1, strDateTimeComment);

		if (!str5A.IsEmpty())
		{
			m_Game.addMultiLineComment(5, str5A);
		}
	}
}

//------------------------------------------------------------------------

CString Sgf::PreProcess(const CString& strContent)
{
	CString strResult("");
	int n = strContent.Find("(");

	if (n == -1)
	{
		return strResult;
	}

	int nP = 0;
	BOOL bValue = FALSE;

	for (int i=n; i<strContent.GetLength(); i++)
	{
		if ((nP == 1) && (strContent[i] == '['))
		{
			bValue = TRUE;
		}
		else if ((nP == 1) && (strContent[i] == ']'))
		{
			bValue = FALSE;
		}
		
		if (bValue)
		{
			if (strContent[i] == '\xd')
			{
				continue;
			}
			else if (strContent[i] == '\xa')
			{
				continue;
			}
			else
			{
				strResult += CString(strContent[i]);
			}
			continue;
		}
		else if (strContent[i] == '(')
		{
			nP++;
			continue;
		}
		else if (strContent[i] == ')')
		{
			nP--;
			continue;
		}
		else if (nP > 1)
		{
			continue;
		}
		else if (nP == 0)
		{
			break;
		}
		else if (strContent[i] == '\xd')
		{
			continue;
		}
		else if (strContent[i] == '\xa')
		{
			continue;
		}
		else
		{
			strResult += CString(strContent[i]);
		}
	}

	// Add an extra ";" at the end to make later search easier
	if (!strResult.IsEmpty() && (strResult[strResult.GetLength()-1] != ';'))
	{
		strResult += CString(";");
	}

	return strResult;
}

//------------------------------------------------------------------------

CString Sgf::FindString(const CString& strContent, const CString& strBegin, const CString& strEnd)
{
	int n1 = strContent.Find(strBegin);

	if (n1 == -1)
	{
		return CString("");
	}

	int n2 = strContent.Find(strEnd, n1+1);

	if (n2 == -1)
	{
		return CString("");
	}
	else
	{
		return strContent.Mid(n1, n2-n1);
	}
}

//------------------------------------------------------------------------

CString Sgf::ParseCommand(const CString& strContent, const CString& strCmd)
{
	int n1 = strCmd.GetLength();

	if (n1 == 0)
	{
		return CString("");
	}

	int n = 0;

	while (1)
	{
		int n2 = strContent.Find("[", n);

		if (n2 == -1)
		{
			return CString("");
		}
		else if (n2 < n1)
		{
			return CString("");
		}

		int n3 = strContent.Find("]", n2+1);

		if (n3 == -1)
		{
			return CString("");
		}

		n = n3+1;

		CString strCommand = strContent.Mid(n2-n1, n1);
		Utils::trim(strCommand);

		if (strCommand.CompareNoCase(strCmd) != 0)
		{
			continue;
		}

		CString strValue = strContent.Mid(n2+1, n3-n2-1);
		Utils::trim(strValue);

		return strValue;
	}

	return CString("");
}

//------------------------------------------------------------------------

bool Sgf::equalComment(const CString& strComment1, const CString& strComment2)
{
    CString strSwap(strComment2);
    swapComment(strSwap);

    return strComment1.Compare(strSwap) == 0;
}

//------------------------------------------------------------------------

void Sgf::swapComment(CString& strComment)
{
	int left = strComment.Find(cFile);

	if (left == -1) return;

    left += cFile.GetLength();

	int right = strComment.Find(TCHAR(13), left);

	if (right == -1) return;

    CString strCommentLeft(strComment.Left(left));
    CString strFile(strComment.Mid(left, right - left));
    CString strCommentRight(strComment.Right(strComment.GetLength() - right));

    if (Utils::IsExtensionSgf(strFile))
    {
	    left  = strFile.Find('-');
	    
        if (left == -1) return;

        right = strFile.Find('-', left+1);

        if (right == -1) return;

        CString strLeftPlayer(strFile.Left(left++));
        CString strRightPlayer(strFile.Mid(left, right - left));
        CString strDate(strFile.Right(strFile.GetLength() - right));

        strComment =
            strCommentLeft +
            strRightPlayer + '-' + strLeftPlayer + strDate +
            strCommentRight;
    }
}
