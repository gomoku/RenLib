// Buf.cpp: implementation of the Buf class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RenLib.h"
#include "Buf.h"
#include "Utils.h"

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
	const RecordSize = 256;      // Record Size
	const StartIndex = 30;       // First move in record
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Buf::Buf()
: m_dwRead(0),
  m_nRecord(0),
  m_nNoMoves(0)
{
}

//------------------------------------------------------------------------

Game& Buf::getGame()
{
	return m_Game;
}

//------------------------------------------------------------------------

CString Buf::getFilePath()
{
	return m_strFilePath;
}

//------------------------------------------------------------------------

bool Buf::OpenFile(const CString& strFile)
{
	CFileException e;

	if( !m_bufFile.Open( strFile, CFile::modeRead, &e ) )
	{
		return false;
	}

	m_strFilePath = m_bufFile.GetFilePath();

	Next();

	return true;
}

//------------------------------------------------------------------------

bool Buf::Done()
{
	return (m_dwRead < RecordSize);
}

//------------------------------------------------------------------------

void Buf::Next()
{
	BYTE buffer[RecordSize];

	m_Game.clear();

	if ((m_dwRead = m_bufFile.Read(buffer, RecordSize)) && (buffer[0] != 0))
	{
		bool isTime     = false;
		bool isReverse  = false;
		bool isDraw     = false;
		bool isBlackWin = false;
		bool isWhiteWin = false;

		CString strBlackPlayer;
		CString strWhitePlayer;

		for (int i = 0; i < StartIndex; i++)
		{
			if (buffer[i] > 127)
			{
				buffer[i] -= 100;

				if (i == 24)
				{
					isTime = true;
				}
				else if (i == 25)
				{
					isDraw = true;
				}
				else if (i == 26)
				{
					isReverse = false;
				}
				else if (i == 27)
				{
					isWhiteWin = true;
				}
				else if (i == 28)
				{
					isBlackWin = true;
				}
				else if (i == 29)
				{
					isReverse = true;
				}
			}
		}

        for (i = 0; i < 15; i++)
        {
            strBlackPlayer += buffer[i];
        }

        for (i = 15; i < 30; i++)
        {
			strWhitePlayer += buffer[i];
        }

		Utils::trim(strBlackPlayer);
		Utils::trim(strWhitePlayer);
        
        CString strResult;
        
        if (isDraw)
        {
            strResult = "0.5 - 0.5";
        }
        else if (isBlackWin)
        {
            strResult = "1 - 0";
        }
        else //if (isWhiteWin)
        {
            strResult = "0 - 1";
        }

		CString str5A("?");

		for (i=StartIndex; i < RecordSize; i++)
		{
			if (buffer[i] == 0)
			{
				break;
			}

			int y = 16 - (buffer[i] + 14) / 15;
			int x = (buffer[i] + 14) % 15 + 1;

			if (i == StartIndex && (x != 8 || y != 8))
			{
				str5A.Format(
					"%s%s",
					Utils::XCoordinateImage(x, true, false),
					Utils::YCoordinateImage(y, false));

				x = 8;
				y = 8;
			}

			m_Game.addPos(BYTE((15-y) * 16 + x));
		}

		const int lastMove = m_Game.numberOfMoves();

		const CString strResultComment(strBlackPlayer + " - " + strWhitePlayer + " " + strResult);

		m_Game.addOneLineComment(lastMove, strResultComment);

		CString strFileAndNumber;
		strFileAndNumber.Format("File: %s, No: %d", m_bufFile.GetFileTitle(), m_nRecord);
		m_Game.addMultiLineComment(lastMove, strFileAndNumber);

		CString strReverse("Reverse: ");
		strReverse += (isReverse ? "R" : "-");
		m_Game.addMultiLineComment(lastMove, strReverse);

		m_Game.addMultiLineComment(lastMove, "5A: " + str5A);

        if (isTime)
        {
			m_Game.addMultiLineComment(lastMove, "Time loss");
        }

		m_nRecord++;
	}

	if (Done())
	{
		m_bufFile.Close();
	}
}

//------------------------------------------------------------------------

bool Buf::Create(const CString& fileName)
{
    return false;
}

void Buf::Save(const MoveList& aMoveList)
{
//    if (!m_pdbFile.Open(fileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
//    {
//        CString strMessage(Utils::GetString(IDS_MSG_OPEN_FILE, fileName));
//        Utils::ShowMessage(strMessage, Utils::GetString(IDS_CAP_CREATE_PDB), MB_ICONERROR);
//        return false;
//    }
//
//    m_pdbFile.Close();
//
}

void Buf::Close()
{
}
