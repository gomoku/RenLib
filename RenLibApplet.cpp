// RenLibApplet.cpp: implementation of the RenLibApplet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "renlib.h"
#include "RenLibApplet.h"
#include "MoveList.h"
#include "MoveNode.h"
#include "Utils.h"
#include "RenLibAppletDialog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const CString RenLibApplet::cFirstComment("--- First Comment ---");
const CString RenLibApplet::cLastComment("--- Last Comment ---");

namespace
{
	const CString cRenLibTxt("RenLib.txt");
	const CString cGamesHtm("games.htm");

	const CString cBig("Big");
	const CString cSmall("Small");
	const CString cOneLineComment("OneLineComment");
	const CString cMultiLineComment("MultiLineComment");
	const CString cBoardText("BoardText");
	const CString cBackgroundColor("BackgroundColor(");
	const CString cCoordinates("Coordinates");
	const CString cNumbers("Numbers");
	const CString cCommands("Commands");
	const CString cUrl("Url");
	const CString cShowBoard("ShowBoard");

	const COLORREF cGray = RGB(225, 225, 225);

    const CPoint NullPoint(0, 0);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RenLibApplet::RenLibApplet(Information info)
{
    mAppletUrl = (info.mUrl == WEB) ? "http://www.renju.se/renlib/applet/" : Utils::getAppletDirectory();

    mBlackWinImage = "<img border=\"0\" src=\"" + mAppletUrl + "BlackWin.gif\">";
    mWhiteWinImage = "<img border=\"0\" src=\"" + mAppletUrl + "WhiteWin.gif\">";
    mDrawImage     = "<img border=\"0\" src=\"" + mAppletUrl + "Draw.gif\">";
}

RenLibApplet::~RenLibApplet()
{
}

void RenLibApplet::WriteLine(const CString& line)
{
	mFile.WriteString(line + '\n');
}

bool RenLibApplet::getAppletInformation(
	const CString& strDirectory, Information& info, COLORREF* custColors)
{
	RenLibAppletDialog renLibAppletDialog(true, true, custColors);

	CString strFile(strDirectory + cRenLibTxt);

	CStdioFile stdioFile;

	// Read information from RenLib.txt
	if (stdioFile.Open(strFile, CFile::modeRead))
	{
		stdioFile.ReadString(info.mTitleAppletWebPage);
		stdioFile.ReadString(info.mTitleGameWebPage);

		CString line;
		stdioFile.ReadString(line);

        info.mSize                 = (line.Find(cBig)              != -1) ? BIG  : SMALL;
		info.mShowOneLineComment   = (line.Find(cOneLineComment)   != -1) ? TRUE : FALSE;
		info.mShowMultiLineComment = (line.Find(cMultiLineComment) != -1) ? TRUE : FALSE;
		info.mShowBoardText        = (line.Find(cBoardText)        != -1) ? TRUE : FALSE;
		info.mShowCoordinates      = (line.Find(cCoordinates)      != -1) ? TRUE : FALSE;
		info.mShowNumbers          = (line.Find(cNumbers)          != -1) ? TRUE : FALSE;
		info.mShowCommands         = (line.Find(cCommands)         != -1) ? TRUE : FALSE;
		info.mUrl                  = (line.Find(cUrl)              != -1) ? WEB  : LOCAL;
		info.mShowBoard            = (line.Find(cShowBoard)        != -1) ? TRUE : FALSE;

		int index = line.Find(cBackgroundColor);

		if (index != -1)
		{
			int left  = line.Find('(', index);
			int right = line.Find(')', index);

			if (left != -1 && right != -1 )
			{
				left++;
				CString strRGB(line.Mid(left, right-left));
				info.mBackgroundColor = atoi(strRGB);
			}
		}

		stdioFile.Close();
	}

    renLibAppletDialog.setInfo(info);

    if (renLibAppletDialog.DoModal() == IDCANCEL)
	{
		return false;
	}

    renLibAppletDialog.getInfo(info);

    // Write information to RenLib.txt
	if (stdioFile.Open(strFile, CFile::modeCreate | CFile::modeWrite | CFile::typeText))
	{
		stdioFile.WriteString(info.mTitleAppletWebPage + '\n');
		stdioFile.WriteString(info.mTitleGameWebPage + '\n');

		CString line((info.mSize == BIG) ? cBig : cSmall);

		if (info.mShowOneLineComment)
		{
			line += ' ' + cOneLineComment;
		}

		if (info.mShowMultiLineComment)
		{
			line += ' ' + cMultiLineComment;
		}

		if (info.mShowBoardText)
		{
			line += ' ' + cBoardText;
		}

		if (info.mShowCoordinates)
		{
			line += ' ' + cCoordinates;
		}

		if (info.mShowNumbers)
		{
			line += ' ' + cNumbers;
		}

		if (info.mShowCommands)
		{
			line += ' ' + cCommands;
		}

		if (info.mUrl == WEB)
		{
			line += ' ' + cUrl;
		}

		if (info.mShowBoard)
		{
			line += ' ' + cShowBoard;
		}

		CString strRGB;
		strRGB.Format(cBackgroundColor + "%d)", info.mBackgroundColor);
		line += ' ' + strRGB;

		stdioFile.WriteString(line + '\n');
		stdioFile.Close();
	}

	return true;
}

void RenLibApplet::generateAppletCode(
	CString& strAppletCode, const MoveList& aMoveList, CString& strComment,
	const Information& info)
{
	CString strParam;

	int width;
	int height;

	if (info.mSize == BIG)
	{
		width  = 525;
		height = 525;

		if (info.mShowOneLineComment)
		{
			height += 35;
		}

		if (info.mShowCommands)
		{
			height += 35;
		}

		if (info.mShowMultiLineComment)
		{
			width += 250;
		}

		if (info.mShowCoordinates)
		{
			width  += 25;
            height += 25;
		}
	}
	else
	{
		width  = 315;
		height = 315;

		if (info.mShowOneLineComment)
		{
			height += 30;
		}

		if (info.mShowCommands)
		{
			height += 30;
		}

		if (info.mShowMultiLineComment)
		{
			width += 195;
		}

		if (info.mShowCoordinates)
		{
			width  += 25;
            height += 20;
		}
	}

	strParam.Format(
        "<applet codebase=\"" + mAppletUrl + "\" code=\"RenLibApplet.class\" width=%d height=%d>",
		width, height);

	strAppletCode += strParam + "\n";

	strAppletCode += "alt=\"Your browser understands the &lt;APPLET&gt; tag but isn't running the applet, " \
		             "for some reason. Your browser is completely ignoring the &lt;APPLET&gt; tag!\"\n";

	//
	// Moves
	//
	CString strMoves;

	for (int i = 1; i <= aMoveList.Index(); i++)
	{
		CPoint Coord(aMoveList.Get(i)->getPos());

        if (Coord == NullPoint)
        {
		    strMoves += "pass ";
        }
        else
        {
		    CString strMove(Utils::XCoordinateImage(Coord.x, true, false) + Utils::YCoordinateImage(Coord.y, true));
		    strMove.MakeLower();
		    strMoves += strMove + ' ';
        }
	}

	strMoves.Delete(strMoves.GetLength()-1);
	strAppletCode += "<PARAM NAME=\"moves\" VALUE=\"" + strMoves + "\">\n";

	//
	// Comment
	//
	strComment = aMoveList.Get(aMoveList.Index())->getOneLineComment();

	if (strComment.IsEmpty())
	{
		strComment = aMoveList.Get(aMoveList.Index())->getMultiLineComment();

		int index = strComment.Find(TCHAR(13));

		if (index != -1)
		{
			strComment.Left(index);
		}
	}

	if (info.mShowOneLineComment)
	{
        CString strRepeatOneLineComment(info.mRepeatOneLineComment);

        if (!strRepeatOneLineComment.IsEmpty())
        {
            if (strRepeatOneLineComment == cFirstComment)
            {
                strRepeatOneLineComment = aMoveList.Get(1)->getOneLineComment();
            }
            else if (strRepeatOneLineComment == cLastComment)
            {
                strRepeatOneLineComment = strComment;
            }

            Utils::trim(strRepeatOneLineComment);

            if (!strRepeatOneLineComment.IsEmpty())
            {
                strParam.Format("<PARAM NAME=\"oneLineComment\" VALUE=\"%s\">", strRepeatOneLineComment);
                strAppletCode += strParam + "\n";
            }
        }

		for (i = 1; i <= aMoveList.Index(); i++)
		{
			CString strOneLineComment = aMoveList.Get(i)->getOneLineComment();

			if (!strOneLineComment.IsEmpty() && strOneLineComment != strRepeatOneLineComment)
			{
				strParam.Format("<PARAM NAME=\"oneLineComment%d\" VALUE=\"%s\">", i, strOneLineComment);
				strAppletCode += strParam + "\n";
			}
		}
	}

	//
	// Multi Line Comment
	//
	if (info.mShowMultiLineComment)
	{
		for (i = 1; i <= aMoveList.Index(); i++)
		{
			CString strMultiLineComment(aMoveList.Get(i)->getMultiLineComment());
			
			if (!strMultiLineComment.IsEmpty())
			{
				CString strComment;

				int nLeft = 0;
				int nRight;

				while (nLeft < strMultiLineComment.GetLength())
				{
					nRight = strMultiLineComment.Find(TCHAR(13), nLeft);

					if (nRight == -1)
					{
						nRight = strMultiLineComment.GetLength();
					}				

					CString strSubComment(strMultiLineComment.Mid(nLeft, nRight - nLeft) + "\\n");

					strComment += strSubComment;

					nLeft = nRight + 2;
				}

				strComment.Delete(strComment.GetLength()-2, 2);

				strParam.Format("<PARAM NAME=\"multiLineComment%d\" VALUE=\"%s\">", i, strComment);
				strAppletCode += strParam + "\n";
			}
		}
	}
	
	//
	// Text
	//
	if (info.mShowBoardText)
	{
		for (i = 1; i <= aMoveList.Index(); i++)
		{
			CString strText;

			MoveNode* pMove = aMoveList.Get(i)->getDown();

			while (pMove)
			{
				if (pMove->isBoardText())
				{
					CPoint Coord(pMove->getPos());
					
					CString strCoord(
						Utils::XCoordinateImage(Coord.x, true, false) +
						Utils::YCoordinateImage(Coord.y, true));

					strCoord.MakeLower();

					strText += strCoord + '|';
					strText += pMove->getBoardText() + '|';
				}

				pMove = pMove->getRight();
			}

			if (!strText.IsEmpty())
			{
				strText.Delete(strText.GetLength()-1);
				
				strParam.Format("<PARAM NAME=\"text%d\" VALUE=\"%s\">", i, strText);
				strAppletCode += strParam + "\n";
			}
		}
	}

	//
	// Applet Size
	//
	strParam.Format("<PARAM NAME=\"size\" VALUE=\"%d\">", (info.mSize == BIG ? 32 : 19));
	strAppletCode += strParam + "\n";

	//
	// One Line Comment Box
	//
    strParam.Format("<PARAM NAME=\"showOneLineComment\" VALUE=\"%d\">", info.mShowOneLineComment);
	strAppletCode += strParam + "\n";

	//
	// Multiple Line Comment Box
	//
	strParam.Format("<PARAM NAME=\"showMultiLineComment\" VALUE=\"%d\">", info.mShowMultiLineComment);
	strAppletCode += strParam + "\n";

	//
	// Coordinates
	//
	strParam.Format("<PARAM NAME=\"showCoordinates\" VALUE=\"%d\">", info.mShowCoordinates);
	strAppletCode += strParam + "\n";

	//
	// Numbers
	//
	strParam.Format("<PARAM NAME=\"showNumbers\" VALUE=\"%d\">", info.mShowNumbers);
	strAppletCode += strParam + "\n";

	//
	// Commands Box
	//
	strParam.Format("<PARAM NAME=\"showCommands\" VALUE=\"%d\">", info.mShowCommands);
	strAppletCode += strParam + "\n";

	//
	// Board
	//
	strParam.Format("<PARAM NAME=\"showBoard\" VALUE=\"%d\">", info.mShowBoard);
	strAppletCode += strParam + "\n";

	//
	// Back Ground Color
	//
	const int R = info.mBackgroundColor & 0xFF;
	const int G = (info.mBackgroundColor >>  8) & 0xFF;
	const int B = (info.mBackgroundColor >> 16) & 0xFF;

	strParam.Format("<PARAM NAME=\"backgroundColor\" VALUE=\"%d|%d|%d\">", R, G, B);
	strAppletCode += strParam + "\n";

	strAppletCode += "</applet>";
}

bool RenLibApplet::Create(
    const CString& fileName, const MoveList& aMoveList,
	CString& strComment, const Information& info)
{
	if (!mFile.Open(fileName, CFile::modeCreate | CFile::modeWrite | CFile::typeText))
	{
		CString strMessage(Utils::GetString(IDS_MSG_OPEN_FILE, fileName));
		Utils::ShowMessage(strMessage, Utils::GetString(IDS_CAP_CREATE_RENLIB_APPLET), MB_ICONERROR);
		return false;
	}

	WriteLine("<html>");
	WriteLine("<head>");
	WriteLine("<meta name=\"GENERATOR\" content=\"RenLib\">");
	WriteLine("<title>" + info.mTitleAppletWebPage + "</title>");
	WriteLine("</head>");
	WriteLine("<body>");
	WriteLine("<h3>" + info.mTitleAppletWebPage + "</h3>");
	
	CString strAppletCode;
	generateAppletCode(strAppletCode, aMoveList, strComment, info);
	WriteLine(strAppletCode);

	WriteLine("</body>");
	WriteLine("</html>");

	mFile.Close();

	return true;
}

bool RenLibApplet::CreateExportGameFile(
    const CString& strDirectory, CStringList& strList, const CString& titleGameWebPage,
	const CString& strCaption, CString& strFile)

{
	strFile = strDirectory + cGamesHtm;

	if (!mFile.Open( strFile, CFile::modeCreate | CFile::modeWrite | CFile::typeText))
	{
		CString strMessage(Utils::GetString(IDS_MSG_OPEN_FILE, strFile));
		Utils::ShowMessage(strMessage, strCaption, MB_ICONERROR);
		return false;
	}

	WriteLine("<html>");
	WriteLine("<body>");
	WriteLine("<h2>" + titleGameWebPage + "</h2>");
	WriteLine("<ol>");

	POSITION pos = strList.GetHeadPosition();

	while (pos != NULL)
	{
		CString strFile = strList.GetNext(pos);
		CString strGame = strList.GetNext(pos);
		WriteLine(
			"<li><a target=\"_blank\" href=\"" + strFile + "\">" + strGame + "</a></li><br>");
	}

	WriteLine("</ol>");
	WriteLine("</body>");
	WriteLine("</html>");

	mFile.Close();

	return true;
}

bool RenLibApplet::CreateTournamentGameFile(
    const CString& strDirectory, CStringList& strList, const CString& titleGameWebPage,
	const CString& strCaption, CString& strFile)
{
	strFile = strDirectory + cGamesHtm;

	if (!mFile.Open(strFile, CFile::modeCreate | CFile::modeWrite | CFile::typeText))
	{
		CString strMessage(Utils::GetString(IDS_MSG_OPEN_FILE, strFile));
		Utils::ShowMessage(strMessage, strCaption, MB_ICONERROR);
		return false;
	}

	WriteLine("<html>");
	WriteLine("<body>");
	WriteLine("<h2>" + titleGameWebPage + "</h2>");

	POSITION pos = strList.GetHeadPosition();
	int lastRound = 0;

	while (pos != NULL)
	{
		CString strFile = strList.GetNext(pos);
		CString strGame = strList.GetNext(pos);

		CString strRight(strFile.Right(10));

		if ((strRight.GetLength() != 10) |
			(strRight[0] != 'r') | 
			(strRight[1] <  '0') |
			(strRight[1] >  '9') |
			(strRight[2] <  '0') |
			(strRight[2] >  '9') |
			(strRight[3] != 'g'))
		{
			CString strError;
			strError.Format("<b style=color:#ff0000>Invalid file name: %s</b><br>", strFile);
			WriteLine(strError);
			continue;
		}

		CString strRound(strRight.Mid(1,2));
		int round = atoi(strRound);

		if (lastRound != round)
		{
			if (lastRound > 0)
			{
				WriteLine("<br>");
			}

			CString strLine;
			strLine.Format("<b>Round %d</b><br>", round);
			WriteLine(strLine);

			lastRound = round;
		}

		WriteLine("<a target=\"_blank\" href=\"" + strFile + "\">" + strGame + "</a><br>");
	}

	WriteLine("</body>");
	WriteLine("</html>");

	mFile.Close();

	return true;
}

RenLibApplet::Information::Information()
: mSize(BIG),
  mTitleAppletWebPage(),
  mTitleGameWebPage(),
  mShowOneLineComment(TRUE),
  mShowMultiLineComment(TRUE),
  mShowBoardText(TRUE),
  mShowCoordinates(TRUE),
  mShowNumbers(TRUE),
  mShowCommands(TRUE),
  mShowBoard(TRUE),
  mUrl(WEB),
  mRepeatOneLineComment(),
  mBackgroundColor(cGray)
{
}

RenLibApplet::Information::Information(
	Size     size,
	CString  titleAppletWebPage,
	CString  titleGameWebPage,
	BOOL     showOneLineComment,
	BOOL     showMultiLineComment,
	BOOL     showBoardText,
	BOOL     showCoordinates,
	BOOL     showNumbers,
	BOOL     showCommands,
	BOOL     showBoard,
    Url      url,
    CString  repeatOneLineComment,
	COLORREF backgroundColor)
: mSize(size),
  mTitleAppletWebPage(titleAppletWebPage),
  mTitleGameWebPage(titleGameWebPage),
  mShowOneLineComment(showOneLineComment),
  mShowMultiLineComment(showMultiLineComment),
  mShowBoardText(showBoardText),
  mShowCoordinates(showCoordinates),
  mShowNumbers(showNumbers),
  mShowCommands(showCommands),
  mShowBoard(showBoard),
  mUrl(url),
  mRepeatOneLineComment(repeatOneLineComment),
  mBackgroundColor(backgroundColor)
{
}

bool RenLibApplet::CreateGameCollectionFile(
	const CString& titleGameWebPage, const CString& strCaption, CString& strFile)
{
    if (!mFile.Open(strFile, CFile::modeCreate | CFile::modeWrite | CFile::typeText))
	{
		CString strMessage(Utils::GetString(IDS_MSG_OPEN_FILE, strFile));
		Utils::ShowMessage(strMessage, strCaption, MB_ICONERROR);
		return false;
	}

	WriteLine("<html>");
	WriteLine("<head>");
	WriteLine("<body>");

	return true;
}

bool RenLibApplet::AddGameCollectionFile(
	const MoveList& aMoveList, CString& strOneLineComment, CString& strMultiLineComment,
	const Information& info)
{
	CString strAppletCode;
	CString strComment;
	generateAppletCode(strAppletCode, aMoveList, strComment, info);

    GameData gameData;
    gameData.applet           = strAppletCode;
    gameData.oneLineComment   = strOneLineComment;
    gameData.multiLineComment = "\n" + strMultiLineComment;

    gameData.multiLineComment.Replace("\r\n", "<br>\n");

    gameData.multiLineComment.Replace("\n1 - 0",     "\n" + mBlackWinImage);
    gameData.multiLineComment.Replace("\n1-0",       "\n" + mBlackWinImage);
    gameData.multiLineComment.Replace("\nBLACK",     "\n" + mBlackWinImage);
    gameData.multiLineComment.Replace("\nSVART",     "\n" + mBlackWinImage);

    gameData.multiLineComment.Replace("\n0,5 - 0,5", "\n" + mDrawImage);
    gameData.multiLineComment.Replace("\n0,5-0,5",   "\n" + mDrawImage);
    gameData.multiLineComment.Replace("\n0.5 - 0.5", "\n" + mDrawImage);
    gameData.multiLineComment.Replace("\n0.5-0.5",   "\n" + mDrawImage);
    gameData.multiLineComment.Replace("\nDRAW",      "\n" + mDrawImage);
    gameData.multiLineComment.Replace("\nREMI",      "\n" + mDrawImage);

    gameData.multiLineComment.Replace("\n0 - 1",     "\n" + mWhiteWinImage);
    gameData.multiLineComment.Replace("\n0-1",       "\n" + mWhiteWinImage);
    gameData.multiLineComment.Replace("\nWHITE",     "\n" + mWhiteWinImage);
    gameData.multiLineComment.Replace("\nVIT",       "\n" + mWhiteWinImage);

    gameData.multiLineComment.Delete(0);

    aMoveList.Current();

    if (mSortedList.size() == 0)
    {
        mSortedList.push_back(gameData);
    }
    else
    {
        bool added = false;

        for (std::vector<GameData>::iterator iter = mSortedList.begin();
             iter != mSortedList.end();
             ++iter)
        {
            if (strOneLineComment < (*iter).oneLineComment)
            {
                mSortedList.insert(iter, gameData);
                added = true;
                break;
            }
        }

        if (!added)
        {
            mSortedList.push_back(gameData);
        }
    }

	return true;
}

bool RenLibApplet::CloseGameCollectionFile()
{
    int gameNumber = 0;

    for (std::vector<GameData>::iterator iter = mSortedList.begin();
         iter != mSortedList.end();
         ++iter)
    {
        if (gameNumber++ % 3 == 0)
        {
            WriteLine("<br>");
	        WriteLine("<table>");
        }

//        WriteLine("<div>");
        WriteLine("<tr>");
	    WriteLine("<td>");

        CString strApplet((*iter).applet);

        if (gameNumber % 3 == 0)
        {
            strApplet.Replace("height=335", "height=315");
        }

        WriteLine(strApplet);
	    
        WriteLine("</td>");
	    
	    WriteLine("<td valign=\"top\">");
	    WriteLine("<br>");

        CString strMultiLineComment((*iter).multiLineComment);
        WriteLine(strMultiLineComment);

        WriteLine("</td>");
	    WriteLine("</tr>");
//        WriteLine("</div>");

        if (gameNumber % 3 == 0)
        {
            WriteLine("</table>");
//            WriteLine("<br>");
        }
    }

    if (gameNumber % 3 != 0)
    {
        WriteLine("</table>");
    }

    WriteLine("</body>");
	WriteLine("</html>");

	mFile.Close();

	return true;
}
