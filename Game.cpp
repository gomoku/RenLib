// Game.cpp: implementation of the Game class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RenLib.h"
#include "Game.h"
#include "Utils.h"
#include "TextBoard.h"

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
	const CString strRenLibApplet1("code=\"RenLibApplet.class\"");
	const CString strRenLibApplet2("code=RenLibApplet.class");

	const CString strDiagramShow("diagram_show.cgi?data=");

	const CString strShowApplet("show_applet.php?data=");

	const CString strScript("<script");

	const CenterPos = 8 + 7*16; // Center Intersection
	const char SEPS[] = " ,+\"|";

	void extractValue(const CString& strLine, CString& strValue)
	{
		int index1 = strLine.Find('"');
		int index2 = strLine.ReverseFind('"');

		if (index1 < index2)
		{
			strValue = strLine.Mid(index1 + 1, index2-index1-1);
		}
	}

	void swap(CString& strLeft, CString& strRight)
	{
		const CString strSwap(strLeft);
		strLeft  = strRight;
		strRight = strSwap;
	}

	void swapResult(CString& strResult)
	{
		const CString strBlackWin("1-0");
		const CString strWhiteWin("0-1");

		if (strResult == strBlackWin)
		{
			strResult = strWhiteWin;
		}
		else if (strResult == strWhiteWin)
		{
			strResult = strBlackWin;
		}
	}

	void addComment(CString& strComment, const CString& strInfo)
	{
		if (!strComment.IsEmpty())
		{
			strComment += TCHAR(13);
			strComment += TCHAR(10);
		}

		strComment += strInfo;
	}

	CString fileInfo(const CString& strFilename)
	{
		return ("File: " + strFilename);
	}

	CString makeResult(const CString& strBlackPlayer, const CString& strWhitePlayer, const CString& strResultLine)
	{
		CString strResult("0.5-0.5");

		if (strResultLine.Find(strBlackPlayer) != -1)
		{
			strResult = "1-0";
		}
		else if (strResultLine.Find(strWhitePlayer) != -1)
		{
			strResult = "0-1";
		}

		return strResult;
	}

	CString resultInfo(const CString& strBlackPlayer, const CString& strWhitePlayer, const CString& strResult)
	{
		return strBlackPlayer + " - " + strWhitePlayer + ' ' + strResult;
	}

	CString extractTag(CStringEx& strText, CString strLeft, CString strRight)
	{
		long tagLeft  = -1;
		long tagRight = -1;
		CString strTag;

		tagLeft  = strText.FindNoCase(strLeft);

		if (tagLeft != -1)
		{
            tagRight = strText.FindNoCase(strRight, tagLeft + strLeft.GetLength());
		}

		if (tagLeft != -1 && tagRight != -1)
		{
            tagRight += strRight.GetLength();

            strTag = strText.Mid(
                tagLeft + strLeft.GetLength(),
                tagRight - tagLeft - strLeft.GetLength() - strRight.GetLength());

            strText.Delete(tagLeft, tagRight - tagLeft);
		}

		return strTag;
	}
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Game::Game()
: mKind(NONE),
  mIndex(0),
  mMultipleGames(false)
{
	clear();
}

//------------------------------------------------------------------------

void Game::clear()
{
    mKind = NONE;
	mMoves.clear();
	mIndex = 0;
}

//------------------------------------------------------------------------

void Game::setKind(Kind kind)
{
	mKind = kind;
}

//------------------------------------------------------------------------

bool Game::GetLine (CString& strLine)
{
	if (mGameFile.ReadString(strLine))
	{
		Utils::trim(strLine);
        return true;
	}

	return false;
}

//------------------------------------------------------------------------

void Game::getToken(CString& strLine, CString& strToken)
{
	strToken = strtok( NULL, SEPS );

	if (strToken.IsEmpty())
	{
		GetLine(strLine);
        strLine.MakeUpper();
		strToken = strtok(strLine.GetBuffer(1), SEPS);
	}
}

//------------------------------------------------------------------------

CPoint Game::MoveToPoint(CString strMove)
{
	CPoint Move;

	if (strMove.GetLength() >= 2 && strMove.GetLength() <= 3)
	{
		if (strMove[0] >= 'A' && strMove[0] <= 'S')
		{
			Move.x = strMove[0] - 'A' + 1;

			if (mKind == PBEM && strMove[0] >= 'J')
			{
				Move.x--;
			}
		}

		strMove = strMove.Right(strMove.GetLength()-1);

		if ((strMove.GetLength() == 1 && strMove >= "1" && strMove <= "9") ||
		    (strMove.GetLength() == 2 && strMove >= "10" && strMove <= "19"))
		{
			Move.y = atoi(strMove);
		}
	}

	Move.y = 16 - Move.y;

	return Move;
}

//------------------------------------------------------------------------

void Game::AddMove(CString strMove)
{
	const CString strOriginalMove(strMove);
	CPoint Move(MoveToPoint(strMove));

	if (Move.x >=1 && Move.x <= 15 && Move.y >=1 && Move.y <= 15)
	{
		addPos(Utils::PointToPos(Move));
	}
}

//------------------------------------------------------------------------

void Game::addPos(BYTE pos)
{
	if (mMoves.size() < 225)
	{
		Node node;
		node.setPos(pos);
		mMoves.push_back(node);
	}
}

//------------------------------------------------------------------------

CString Game::getFilePath()
{
	return mFilePath;
}

//------------------------------------------------------------------------

Game::Result
Game::CheckMoves()
{
	if (mMoves.size() < 5)
	{
		return LESS_THAN_5_MOVES;
	}

	const int cMax = 256;
	BYTE board[cMax];

	for (int i=0; i < cMax; i++)
	{
		board[i] = 0;
	}

	for (i=0; i < mMoves.size(); i++)
	{
		BYTE pos = mMoves[i].getPos();

		if (board[pos] != 0)
		{
			return SAME_COORDINATE;
		}

		board[pos] = pos;
	}

	return VALID;
}

//------------------------------------------------------------------------

void Game::ParsePBeM()
{
	CString strLine;

	GetLine(strLine);

	CString strBlackPlayer(strtok(strLine.GetBuffer(1), SEPS));
	CString strWhitePlayer(strtok(NULL, SEPS));

	if (strBlackPlayer.IsEmpty() || strWhitePlayer.IsEmpty())
	{
		return; // Syntax Error
	}

	//
	//	--- Old Notation ---
	//
	//   Eks          Ohs
	//   norihiko     areims
	//   1 H8         2 J9
	//   3 G7         4 H9
	//   5 G9,K9      6 G9,J7
	//   7 J10        8 G10
	//   9 F8        10 J8
	//
	//	--- New Notation ---
	//
	//   Eks          Ohs       
	//   dubolom      kryp      
	//   1 G8,H9      2 H7      
	//   3 F9,J6      4 F9,H10  
	//   5 G10        6 J8      
	//   7 G9         8 J9      
	
	
	CString strSkip;
	CString strDouble;

	while (GetLine(strLine))
	{
		if (strLine.IsEmpty())
		{
			break;
		}

		CString strToken(strtok(strLine.GetBuffer(1), SEPS));

		while (!strToken.IsEmpty())
		{
			if (strToken == "1")
			{
				AddMove("H8");

				strToken = strtok( NULL, SEPS );

				if (strToken == "H8")
				{
					strSkip   = "5";
					strDouble = "6";
				}
				else
				{
					AddMove(strToken);

					strToken = strtok( NULL, SEPS );
					AddMove(strToken);

					strSkip   = "3";
					strDouble = "4";
				}

			}
			else if (strToken == strDouble)
			{
				strToken = strtok( NULL, SEPS );
				AddMove(strToken);

				strToken = strtok( NULL, SEPS );
				AddMove(strToken);

			}
			else if (strToken == strSkip)
			{
				strToken = strtok( NULL, SEPS );
				strToken = strtok( NULL, SEPS );

			}
			else
			{
				strToken = strtok( NULL, SEPS );

                if (strToken == "--")
			    {
				    addPos(0);

				    int moveNo = numberOfMoves();

				    CString strPass;
				    strPass.Format("%d. Pass", moveNo);
				    addMultiLineComment(moveNo, strPass);
			    }
			    else
			    {
                    AddMove(strToken);
			    }
			}

            strToken = strtok( NULL, SEPS );
		}
	}

	CString strResult(makeResult(strBlackPlayer, strWhitePlayer, mResult));
	addOneLineComment(mMoves.size(), resultInfo(strBlackPlayer, strWhitePlayer, strResult));

	addMultiLineComment(mMoves.size(), fileInfo(mGameFile.GetFileTitle()));
}

//------------------------------------------------------------------------

void Game::ParsePlaySite()
{
	CString strLine;

	GetLine(strLine);

	CString strBlackPlayer(strtok(strLine.GetBuffer(1), SEPS));
	CString strWhitePlayer(strtok(NULL, SEPS));

	if (strBlackPlayer.IsEmpty() || strWhitePlayer.IsEmpty())
	{
		return; // Syntax Error
	}

	//     SleepyTurtle ThAngBoM
	//1.   H8           H9
	//2.   J9           J8
	//3.   I7           H7
	//4.   G10          I10
	//5.   F9           F10
	//6.   I6           I5
	//7.   E8           D7
	//8.   E11          G11
	//9.   J6           K5
	//10.  E9           E7
	//11.  F7           resign
	//
	//     ThAngBoM wins.
	
	while (GetLine(strLine))
	{
		if (strLine.IsEmpty() || strLine.Left(1) < '1'  || strLine.Left(1) > '9' )
		{
			break;
		}

		CString strToken(strtok(strLine.GetBuffer(1), SEPS));

		while (!strToken.IsEmpty())
		{
			if (strToken == "1.")
			{
				strToken = strtok( NULL, SEPS );

				if (strToken != "H8")
				{
					AddMove("H8");
				}

				AddMove(strToken);
			}
			else
			{
				strToken = strtok( NULL, SEPS );
				AddMove(strToken);
			}
		}
	}

	mResult = strLine;

	while (mResult.IsEmpty() && GetLine(strLine))
	{
		mResult = strLine;
	}

	CString strResult(makeResult(strBlackPlayer, strWhitePlayer, mResult));
	addOneLineComment(mMoves.size(), resultInfo(strBlackPlayer, strWhitePlayer, strResult));

	addMultiLineComment(mMoves.size(), fileInfo(mGameFile.GetFileTitle()));
}

//------------------------------------------------------------------------

void Game::ParseTextBoard()
{
	CString strLine;
	CString strPlayers;

	bool isPlayerLineChecked = false;

	GetLine(strLine);

	//
	//    ----- Text Board -----
	//
	//      Black    White
	//      1 H8     2 H9
	//      3 J10    4 I9
	//      5 I8     6 G9
	//      7 J9     8 J8
	//      9 F9    10 K10

	while (GetLine(strLine))
	{
		if (!isPlayerLineChecked)
		{
			isPlayerLineChecked = true;

			if (strLine.Left(2) != "1 ")
			{
				strPlayers = strLine;
				continue;
			}
		}

		if (strLine.IsEmpty() || strLine.Left(1) < '1'  || strLine.Left(1) > '9' )
		{
			break;
		}

		CString strToken(strtok(strLine.GetBuffer(1), SEPS));

		while (!strToken.IsEmpty())
		{
			strToken = strtok( NULL, SEPS );
			AddMove(strToken);

			strToken = strtok( NULL, SEPS );
		}
	}

	addOneLineComment(mMoves.size(), strPlayers);
	addMultiLineComment(mMoves.size(), fileInfo(mGameFile.GetFileTitle()));
}

//------------------------------------------------------------------------

Game::Result
Game::parseGame()
{
	mFilePath = mGameFile.GetFilePath();

	CString strLine;

	bool FoundHeader = false;

	//
	// Header
	//
	while (mKind == NONE && !FoundHeader && GetLine(strLine))
	{
		if (strLine.Left(4) == "Game")
		{
			mResult = strLine;
		}

		if (strLine == TextBoard::cHeader)
		{
			setKind(TEXTBOARD);
			FoundHeader = true;
		}
		else if (strLine.Left(2) == "--" && strLine.Find("HISTORY") != -1)
		{
			setKind(PLAYSITE);
			FoundHeader = true;
		}
		else if (strLine.Left(6) == "1. Vit" || strLine.Left(8) == "1. Svart")
		{
			setKind(BLIP);
			FoundHeader = true;
		}
		else if ((strLine == "[Site \"www.kurnik.pl\"]") ||
                 (strLine == "[Site \"kurnik\"]"))
		{
			setKind(KURNIK);
			FoundHeader = true;
		}
		else if ((strLine.Find("Eks") != -1) && (strLine.Find("Ohs") != -1))
		{
			setKind(PBEM);
		}
		else if (strLine.Find("Game Board Window") != -1)
		{
			setKind(RENJUCLASS);
		}
	}

	switch(mKind)
	{
		case PBEM:
		{
			ParsePBeM();
			break;
		}
		case PLAYSITE:
		{
			ParsePlaySite();
			break;
		}
		case TEXTBOARD:
		{
			ParseTextBoard();
			break;
		}
		case BLIP:
		{
			ParseBlip(strLine);
			break;
		}
		case KURNIK:
		{
			ParseKurnik(strLine);
			break;
		}
		case RENJUCLASS:
		{
			ParseRenjuclass(strLine);
			break;
		}
		default:
		{
			return INVALID_KIND;
		}
	}

	return CheckMoves();
}

//------------------------------------------------------------------------

Game::Result
Game::parseWebGame(CStringEx& strPage)
{
	mFilePath = "web page";

	if (strPage.Find(strRenLibApplet1) != -1 || strPage.Find(strRenLibApplet2) != -1)
	{
		return parseApplet(strPage);
	}
	else
	{
		return parseRenjuclassDiagram(strPage);
	}
}

//------------------------------------------------------------------------

Game::Result
Game::parseApplet(CStringEx& strPage)
{
	long appletLeft  = 0;
	long appletRight = -1;

	CString strMoves;

	enum
	{
		ONE_LINE_COMMENT   = 0,
		MULTI_LINE_COMMENT = 1,
		TEXT               = 2,
		NUMBER_OF_KEYS     = 3
	};

	const CString cKey[NUMBER_OF_KEYS] =
	{
		"onelinecomment",
		"multilinecomment",
		"text"
	};

	std::vector<int>     key[NUMBER_OF_KEYS];
	std::vector<CString> value[NUMBER_OF_KEYS];

	CStringEx strApplet(extractTag(strPage, "<applet", "</applet>"));

	while (true)
	{
		CStringEx strParam(extractTag(strApplet, "<param", ">"));

		if (strParam.IsEmpty())
		{
			break;
		}

		CString strKey(extractTag(strParam, "name=\"", "\""));
		CString strValue(extractTag(strParam, "value=\"", "\""));

		if (!strKey.IsEmpty() && !strValue.IsEmpty())
		{
			strKey.MakeLower();

			if (strKey == "moves")
			{
				strMoves = strValue;
			}
			else
			{
				for (int i=0; i < NUMBER_OF_KEYS; i++)
				{
					if (strKey.Find(cKey[i]) == 0)
					{
						int keyValue = atoi(strKey.Mid(cKey[i].GetLength()));

						if (keyValue > 0)
						{
							key[i].push_back(keyValue);
							value[i].push_back(strValue);
						}
						break;
					}
				}
			}
		}
	}

	strMoves.MakeUpper();
	CString strToken(strtok(strMoves.GetBuffer(1), SEPS));

	while (!strToken.IsEmpty())
	{
		AddMove(strToken);
		strToken = strtok( NULL, SEPS );
	}

	for (int i=0; i<key[ONE_LINE_COMMENT].size(); i++)
	{
		addOneLineComment(key[ONE_LINE_COMMENT][i], value[ONE_LINE_COMMENT][i]);
	}

	CString strNewline;
	strNewline += TCHAR(13);
	strNewline += TCHAR(10);

	for (i=0; i<key[MULTI_LINE_COMMENT].size(); i++)
	{
		CString strComment(value[MULTI_LINE_COMMENT][i]);
		strComment = replaceEvery (strComment, "\\n", strNewline);
		addMultiLineComment(key[MULTI_LINE_COMMENT][i], strComment);
	}

	for (i=0; i<key[TEXT].size(); i++)
	{
		int moveNo = key[TEXT][i];

		CString strAllText(value[TEXT][i]);

		strToken = strtok(strAllText.GetBuffer(1), SEPS);

		while (!strToken.IsEmpty())
		{
			CString strCoord(strToken);
			strCoord.MakeUpper();

			CString strText(strtok( NULL, SEPS ));
			addBoardText(moveNo, Utils::PointToPos(MoveToPoint(strCoord)), strText);

			strToken = strtok( NULL, SEPS );
		}
	}

	return CheckMoves();
}

//------------------------------------------------------------------------

Game::Result
Game::parseRenjuclassDiagram(CString& strPage)
{
	CString strDiagram(strDiagramShow);

	long left = strPage.Find(strDiagram);

	if (left == -1)
	{
		strDiagram = strShowApplet;
		left = strPage.Find(strDiagram);
	}

	if (left == -1)
	{
		strPage.Empty();
	}
	else
	{
        long movesLeft = left + strDiagram.GetLength();
		long right     = strPage.Find(">", left);

		CString strMoves = strPage.Mid(movesLeft, right - movesLeft);
		strMoves.MakeUpper();

		CString strToken(strtok(strMoves.GetBuffer(1), SEPS));

		while (!strToken.IsEmpty())
		{
			AddMove(strToken);
			strToken = strtok( NULL, SEPS );
		}

		strPage.Delete(left, right - left + 1);

        long commentLeft  =  0;
		long commentRight = -1;

		CString strLeft("<td");
		CString strRight("</td");

		if (strPage.Find(strRight) == -1)
		{
			strLeft  = "<body";
			strRight = "</body";
		}

		long index = left - 1;

        //
        // Find start of comment
        //
		while (index != -1)
		{
			if (strPage.GetAt(index) == '<')
			{
				CString strFind(strPage.Mid(index, strLeft.GetLength()));
				strFind.MakeLower();

				if (strFind == strLeft)
				{
					commentLeft = index;
					break;
				}
			}

			index--;
		}

		index = strPage.Find("<", index + 1);

        //
        // Find end of comment
        //
		while (index != -1 && index < strPage.GetLength() - strRight.GetLength())
		{
			CString strFind(strPage.Mid(index, strScript.GetLength()));
			strFind.MakeLower();

			if (strFind == strScript)
			{
                index = left;

                //
                // Find start of script tag
                //
		        while (index != -1)
		        {
			        if (strPage.GetAt(index) == '<')
			        {
				        CString strFind(strPage.Mid(index, strScript.GetLength()));
				        strFind.MakeLower();

				        if (strFind == strScript)
				        {
               				commentRight = index;
					        break;
				        }
			        }

			        index--;
		        }

                break;
			}

			strFind = strPage.Mid(index, strRight.GetLength());
			strFind.MakeLower();

			if (strFind == strRight)
			{
				commentRight = index;
				break;
			}

			index = strPage.Find("<", index + 1);
		}

		if (commentRight != -1)
		{
			CString strComment(strPage.Mid(commentLeft, commentRight - commentLeft));
			removeHtml(strComment);
			addMultiLineComment(mMoves.size(), strComment);

			strPage.Delete(0, commentRight);
		}
		else
		{
			strPage.Delete(0, right);
		}
	}

	return CheckMoves();
}

//------------------------------------------------------------------------

void Game::ParseBlip(CString& strLine)
{
	//1. Vit G9, Svart G8
	//2. Vit H8, Svart I7
	//3. Vit I9, Svart H9
	//4. Vit G7, Svart J10
	//5. Vit F6, Svart E5
	//6. Vit F8, Svart F7
	//7. Vit E6, Svart I10
	//8. Vit J11, Svart H10
	//9. Vit K10, Svart D6
	//10. Vit H6, Svart E9
	//11. Vit I5, Svart J4
	//12. Vit L9, Svart I12
	//13. Vit M8, Svart N7
	//14. Vit G6, Svart I6
	//15. Vit H7, Svart C7
	//16. Vit F4, Svart F10

	do
	{
		if (strLine.IsEmpty() || strLine.Left(1) < '1'  || strLine.Left(1) > '9' )
		{
			break;
		}

		CString strToken(strtok(strLine.GetBuffer(1), SEPS));

		strToken = strtok( NULL, SEPS );
		strToken = strtok( NULL, SEPS );

		if (strToken.IsEmpty())
		{
			break;
		}
		else
		{
			AddMove(strToken);
		}

		strToken = strtok( NULL, SEPS );
		strToken = strtok( NULL, SEPS );

		if (strToken.IsEmpty())
		{
			break;
		}
		else
		{
			AddMove(strToken);
		}
	}
	while (GetLine(strLine));

	mResult.Empty();

	addOneLineComment(mMoves.size(), "Blip game");
	addMultiLineComment(mMoves.size(), fileInfo(mGameFile.GetFileTitle()));
}

//------------------------------------------------------------------------

void Game::ParseKurnik(CString& strLine)
{
	//[Event "?"]
	//[Site "www.kurnik.pl"]
	//[Date "2003.08.23"]
	//[Round "-"]
	//[Black "kungen"]
	//[White "frassegawa"]
	//[Result "0-1"]
	//[Time "18:43:17"]
	//[TimeControl "1800"]
	//[BlackRank "1437"]
	//[WhiteRank "1252"]
	//
	//1. h8 -- 2. h9 -- 3. h6 white 4. -- i8 5. j7 -- 6. i7 i7 7. -- j7 8. k6 j8 9.
	//j9 g8 10. f7 j5 11. j4 g7 12. g9 j6 13. h5 h4 14. g6 e8 15. f6 g10 16. f11 e6
	//17. h11 e11 18. f5 f4 19. i11 e7 20. e5 i5 21. f9 f8 22. k7 d10 23. e9 f10 24.
	//e10 d8 25. c8 d11 26. j11 g11 0-1

	//1. h8 --		1 RENJU
	//2. h9 --		2
	//3. h6 white	3
	//4. -- i8		4
	//5. j7 --		A
	//6. i7 i7		B and 5
	//7. -- j7		6
	//8. k6 j8		7 8

	//1. h8 --		1
	//2. h9 --		2
	//3. h7 black	3
	//4. h6 i8		4 A
	//5. -- j8		B
	//6. j8 --		5
	//7. g8 f7		6 7
	//8. g7 g6

    //[Event "?"]
    //[Site "kurnik"]
    //[Date "2005.03.16"]
    //[Round "-"]
    //[Black "roszler"]
    //[White "autickocz"]
    //[Result "0-1"]
    //[Time "16:38:57"]
    //[TimeControl "300"]
    //[GameType "60,15"]
    //[BlackElo "1933"]
    //[WhiteElo "2083"]
    //
    //1. g5 m8      GO-MOKU
    //2. h10 white
    //3. -- k9
    //4. j8 k10
    //5. k11 l9
    //6. j11 j9
    //7. m9 l11
    //8. i8 l10
    //
    //1. d4 d5
    //2. e5 black
    //3. f6 f7
    //4. g8 e3
    //5. e4 c5
    //6. b6 f4
    //7. c6 b7
    //8. d6 e6

    CString strDate;
	CString strTime;
	CString strBlack;
	CString strWhite;
	CString strResult;
    CString strOriginalResult;
	CString strWhiteChoice;
	CString str5A;
	CString str5B;
	CString strTimeControl;

	while (GetLine(strLine))
	{
		if (strLine.Find("Date") != -1)
		{
			extractValue(strLine, strDate);
		}
		else if (strLine.Find("Black") != -1)
		{
			extractValue(strLine, strBlack);
		}
		else if (strLine.Find("White") != -1)
		{
			extractValue(strLine, strWhite);
		}
		else if (strLine.Find("Result") != -1)
		{
			extractValue(strLine, strResult);
            strOriginalResult = strResult;
		}
		else if (strLine.Find("Time ") != -1)
		{
			extractValue(strLine, strTime);
		}
		else if (strLine.Find("TimeControl") != -1)
		{
			extractValue(strLine, strTimeControl);
			break;
		}
	}

    while (GetLine(strLine))
	{
		if (strLine.Left(2) == "1.")
		{
			break;
		}
	}

	strLine.MakeUpper();

	CString strToken;
	strToken = strtok(strLine.GetBuffer(1), SEPS);

	long n       = 0;
    bool done    = false;
    bool isRenju = false;

	while (!done && !strToken.IsEmpty())
	{
		for (long i=0; i<2; i++)
		{
			getToken(strLine, strToken);

			if (strToken == "--")
			{
			    if (n == 1)
			    {
				    isRenju = true;
			    }

                continue;
			}
			else if (strToken.IsEmpty() || strToken == strOriginalResult)
			{
                done = true;
				break;
			}

			n++;

			if (n == 4)
			{
				strWhiteChoice = strToken;

				if (strWhiteChoice == "BLACK")
				{
					swap(strBlack, strWhite);
					swapResult(strResult);
				}
			}
			else if (isRenju && n == 6)
			{
				str5A = strToken;
			}
			else if (isRenju && n == 7)
			{
				str5B = strToken;
			}
			else
			{
				AddMove(strToken);

				if (isRenju && n == 8 && strToken == str5A)
				{
					swap(str5A, str5B);
				}
			}
		}

        if (!done)
        {
		    getToken(strLine, strToken);
        }

        if (strToken.IsEmpty() || strToken == strOriginalResult)
		{
            done = true;
		}
	}

	const int lastMove = mMoves.size();

	const CString strResultComment(resultInfo(strBlack, strWhite, strResult));
	const CString strDateTimeComment(strDate + " " + strTime);
	const CString strWhiteChoiceComment("White's Choice: " + strWhiteChoice);
	const CString str5AComment("5A: " + str5A);
	const CString strTimeControlComment("Time Control: " + strTimeControl);

	addOneLineComment(lastMove, strResultComment);

	addMultiLineComment(lastMove, fileInfo(mGameFile.GetFileTitle()));
	addMultiLineComment(lastMove, strDateTimeComment);
	addMultiLineComment(lastMove, strWhiteChoiceComment);	

    if (isRenju)
    {
        addMultiLineComment(lastMove, str5AComment);
    }
    
    addMultiLineComment(lastMove, strTimeControlComment);

	if (!mMultipleGames)
	{
		addOneLineComment(1, strResultComment);

		addMultiLineComment(1, strDateTimeComment);
		addMultiLineComment(4, strWhiteChoiceComment);

        if (isRenju)
        {
            addMultiLineComment(5, str5AComment);
        }
        
        addMultiLineComment(1, strTimeControlComment);
	}
}

//------------------------------------------------------------------------

void Game::ParseRenjuclass(CString& strLine)
{
	int i = strLine.Find(CString("Black: <b>"));
	int j = strLine.Find(CString("</b>"),i+1);

	CString strBlackName = strLine.Mid(i+10, j-i-10);	// Find Black Name

	i = strLine.Find(CString("White: <b>"));
	j = strLine.Find(CString("</b>"), i+1);
	
	CString strWhiteName = strLine.Mid(i+10, j-i-10);	// Find White Name

	CString strGif;
	CString strMove;
	int n;

	for (int moveNo=1; moveNo <= 225; moveNo++)
	{
		if (moveNo & 0x1)
		{
			strGif.Format("b%d.gif", (moveNo+1)/2);	// Find Black Move
		}
		else
		{
			strGif.Format("w%d.gif", moveNo/2);     // Find White Move
		}

		if ((n = strLine.Find(strGif)) == -1)
		{
			break;
		}

		i = strLine.Find(CString("<img name="), n-55);
		j = strLine.Find(CString("border="),    n-55);

		strMove = strLine.Mid(i+10, j-1-i-10);
		strMove.MakeUpper();
		AddMove(strMove);
	}

	addOneLineComment(moveNo, strBlackName + " - " + strWhiteName);
}

//------------------------------------------------------------------------

bool Game::open(const CString& strFile, bool multipleGames)
{
	mMultipleGames = multipleGames;

	CFileException e;

    bool success = true;

	if (!mGameFile.Open( strFile, CFile::modeRead, &e))
	{
		success = false;
	}

    return success;
}

//------------------------------------------------------------------------

void Game::close()
{
	mGameFile.Close();
}

//------------------------------------------------------------------------

Game::Result
Game::addGame()
{
	clear();
	return parseGame();
}

//------------------------------------------------------------------------

Game::Result
Game::addWebGame(CStringEx& strPage)
{
	mMultipleGames = true;

	return parseWebGame(strPage);
}

//------------------------------------------------------------------------

void Game::addOneLineComment(int moveNo, const CString& comment)
{
	const index = moveNo-1;

	if (index >= 0 && index < mMoves.size())
	{
		CString strOneLineComment(mMoves[index].getOneLineComment());
		addComment(strOneLineComment, comment);
		mMoves[index].setOneLineComment(strOneLineComment);
	}
}

//------------------------------------------------------------------------

void Game::addMultiLineComment(int moveNo, const CString& comment)
{
	const index = moveNo-1;

	if (index >= 0 && index < mMoves.size())
	{
		CString strMultiLineComment(mMoves[index].getMultiLineComment());
		addComment(strMultiLineComment, comment);
		mMoves[index].setMultiLineComment(strMultiLineComment);
	}
}

//------------------------------------------------------------------------

void Game::addBoardText(int moveNo, const BYTE& coord, const CString& boardText)
{
	const index = moveNo-1;

	if (index >= 0 && index < mMoves.size())
	{
		mMoves[index].addBoardText(coord, boardText);
	}
}

//------------------------------------------------------------------------

Game::Node::Node()
: mIndex(0)
{
}

//------------------------------------------------------------------------

BYTE Game::Node::getPos()
{
	return mPos;
}

//------------------------------------------------------------------------

void Game::Node::setPos(BYTE pos)
{
	mPos = pos;
}

//------------------------------------------------------------------------

CString Game::Node::getOneLineComment()
{
	return mOneLineComment;
}

//------------------------------------------------------------------------

void Game::Node::setOneLineComment(const CString& comment)
{
	mOneLineComment = comment;
}

//------------------------------------------------------------------------

CString Game::Node::getMultiLineComment()
{
	return mMultiLineComment;
}

//------------------------------------------------------------------------

void Game::Node::setMultiLineComment(const CString& comment)
{
	mMultiLineComment = comment;
}

//------------------------------------------------------------------------

bool Game::Node::hasMoreBoardTexts()
{
	return mIndex < mCoord.size();
}

//------------------------------------------------------------------------

void Game::Node::nextBoardText(BYTE& coord, CString& boardText)
{
	VERIFY(hasMoreBoardTexts());

	coord     = mCoord[mIndex];
	boardText = mBoardText[mIndex];
	mIndex++;
}

//------------------------------------------------------------------------

void Game::Node::addBoardText(const BYTE& coord, const CString& boardText)
{
	mCoord.push_back(coord);
	mBoardText.push_back(boardText);
}

//------------------------------------------------------------------------

int Game::numberOfMoves()
{
	return mMoves.size();
}

//------------------------------------------------------------------------

bool Game::hasMoreMoves()
{
	return mIndex < mMoves.size();
}

//------------------------------------------------------------------------

Game::Node*
Game::nextMove()
{
	VERIFY(hasMoreMoves());

	return &mMoves[mIndex++];
}

//------------------------------------------------------------------------

void Game::removeHtml(CString& strText)
{
	// Remove all tags except "<br>"
	CString strClean;
	long nIndex = 0;
	long nStartTag = 0;
	
	while ((nStartTag = strText.Find ('<', nIndex)) != -1)
	{
		// Extract to start of tag
		CString strSubstring(strText.Mid (nIndex, (nStartTag - nIndex)));

		strClean += strSubstring;
		
		nIndex = nStartTag + 1;

		// Skip over tag
		long nEndTag = strText.Find ('>', nIndex);
		
		if (nEndTag == (-1))
		{
			break;
		}
		
		nIndex = nEndTag + 1;
		
		CString strTag(strText.Mid(nStartTag, nIndex-nStartTag));
		strTag.MakeLower();

		if (strTag == "<br>")
		{
			strClean += strTag;
		}
	}
	
	// Gather remaining text
	if (nIndex < strText.GetLength())
	{
		strClean += strText.Right (strText.GetLength() - nIndex);
	}

	strText = strClean;
	strClean.Empty();

	CString strLinefeed;
	strLinefeed += TCHAR(10);

	strText = replaceEvery (strText, strLinefeed, "");

	// Do some common replacements
	if (strText.Find ("&nbsp;") != (-1))
	{
		strText = replaceEvery (strText, "&nbsp;", " ");
	}

	if (strText.Find ("&amp;") != (-1))
	{
		strText = replaceEvery (strText, "&amp;", "&");
	}
	
	if (strText.Find ("&aring;") != (-1))
	{
		strText = replaceEvery (strText, "&aring;", "å");
	}
	
	if (strText.Find ("&auml;") != (-1))
	{
		strText = replaceEvery (strText, "&auml;", "ä");
	}
	
	if (strText.Find ("&eacute;") != (-1))
	{
		strText = replaceEvery (strText, "&eacute;", "é");
	}
	
	if (strText.Find ("&iacute;") != (-1))
	{
		strText = replaceEvery (strText, "&iacute;", "i");
	}
	
	if (strText.Find ("&igrave;") != (-1))
	{
		strText = replaceEvery (strText, "&igrave;", "i");
	}
	
	if (strText.Find ("&ograve;") != (-1))
	{
		strText = replaceEvery (strText, "&ograve;", "o");
	}
	
	if (strText.Find ("&ouml;") != (-1))
	{
		strText = replaceEvery (strText, "&ouml;", "ö");
	}
	
	if (strText.Find ("&szlig;") != (-1))
	{
		strText = replaceEvery (strText, "&szlig;", "");
	}

	strText = replaceEvery (strText, " <br>", "<br>");
	strText = replaceEvery (strText, "<br> ", "<br>");
	strText = replaceEvery (strText, "<br><br><br>", "<br><br>");
	strText = trim(strText, "<br>");

	CString strNewline;
	strNewline += TCHAR(13);
	strNewline += TCHAR(10);

	strText = replaceEvery (strText, "<br>", strNewline);
}

CString Game::replaceEvery(CString& strTarget, const CString& strOccurence, const CString& strWith)
{
	CString output(strTarget);
	
	// lowercase-versions to search in.
	CString input_lower(strTarget);
	CString oldone_lower(strOccurence);
	input_lower.MakeLower();
	oldone_lower.MakeLower();
	
	const int length = strOccurence.GetLength();

	// search in the lowercase versions,
	// replace in the original-case version.
	int pos=0;
	while ((pos = input_lower.Find(oldone_lower)) !=-1)
	{
		// need for empty "newstr" cases.
		input_lower.Delete( pos, length );	
		input_lower.Insert( pos, strWith );
		
		// actually replace.
		output.Delete( pos, length );
		output.Insert( pos, strWith );
	}
	
	return output;
}

CString Game::trim(CString& strTarget, const CString& strOccurence)
{
	CString output(strTarget);
	
	// lowercase-versions to search in.
	CString input_lower(strTarget);
	CString oldone_lower(strOccurence);
	input_lower.MakeLower();
	oldone_lower.MakeLower();
	
	const int length = strOccurence.GetLength();

	// search in the lowercase versions,
	// delete in the original-case version.

	while ((input_lower.Left(length)) == strOccurence)
	{
		// need for empty "newstr" cases.
		input_lower.Delete(0, length);
		
		// actually delete.
		output.Delete(0, length);
	}
	
	while ((input_lower.Right(length)) == strOccurence)
	{
		const int pos = input_lower.GetLength();

		// need for empty "newstr" cases.
		input_lower.Delete(pos - length, length);
		
		// actually delete.
		output.Delete(pos - length, length);
	}
	
	return output;
}
