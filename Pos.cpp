// Pos.cpp: implementation of the Pdb class.
//
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "stdafx.h"
#include "RenLib.h"
#include "Pos.h"
#include "MoveList.h"
#include "MoveNode.h"
#include "Utils.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//--------------------------------------------------------------------
// const
//--------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Pos::Pos()
{
}

//------------------------------------------------------------------------

Pos::~Pos()
{
}

//------------------------------------------------------------------------

Game& Pos::getGame()
{
	return m_Game;
}

//------------------------------------------------------------------------

CString Pos::getFilePath()
{
	return m_strFilePath;
}

//------------------------------------------------------------------------

bool Pos::Open(const CString& strFile)
{
	CFileException e;

	if (m_file.Open( strFile, CFile::modeRead, &e) == 0)
	{
		return false;
	}

	m_strFilePath = m_file.GetFilePath();

	return true;
}

//------------------------------------------------------------------------

bool Pos::Read()
{
	m_Game.clear();

	UINT nBytesRead = m_file.Read(m_buffer, BUFFERSIZE);

	if (nBytesRead == 0 || m_buffer[0] == 0 || m_buffer[0] > 225)
	{
		return false;
	}

	for (int i = 1; i <= m_buffer[0]; i++)
	{
		int index = (m_buffer[i] / 15) * 16 + m_buffer[i] % 15 + 1;
		m_Game.addPos(BYTE(index));
	}

	addZjrenjuComments();

	return true;
}

//------------------------------------------------------------------------

void Pos::addZjrenjuComments()
{
	CString fileName(m_file.GetFileTitle());

	int index[6];
	int last = 0;

	int NoOfDelimiters = 0;

	for (int i=0; i<5; i++)
	{
		index[i] = fileName.Find('#', last);
		last = index[i] + 1;

		if (index[i] != -1)
		{
			NoOfDelimiters++;
		}
		else
		{
			break;
		}
	}

	int first = 0;

	if (NoOfDelimiters == 5)
	{
		index[5] = fileName.ReverseFind('.');

		CString info[6];

		for (i=0; i<6; i++)
		{
			last = index[i];
			info[i] = fileName.Mid(first, last-first);
			first = last + 1;
		}

		CString strTableNumber("Table: " + info[0]);

		CString strRule("Rule: ");

		if (info[1] == "0")
		{
			strRule += "Sakata";
		}
		else if (info[1] == "1")
		{
			strRule += "RIF";
		}
		else if (info[1] == "2")
		{
			strRule += "Two swap";
		}

		CString strBlack(info[2]);

		CString strWhite(info[3]);

		CString strSwap("Swap: ");

		if (info[1] == "0" || info[1] == "1")
		{
			if (info[4] == "0")
			{
				strSwap += "No swap";
			}
			else if (info[4] == "1")
			{
				strSwap += "Swap";
			}
		}
		else if (info[1] == "2")
		{
			if (info[4] == "0")
			{
				strSwap += "No swap | No swap";
			}
			else if (info[4] == "1")
			{
				strSwap += "Swap | No swap";
			}
			else if (info[4] == "2")
			{
				strSwap += "No swap | Swap";
			}
			else if (info[4] == "3")
			{
				strSwap += "Swap | Swap";
			}
		}

		CString strResult(strBlack + " - " + strWhite + " ");

		CString strState("State: ");

		if (info[5] == "0")
		{
			strState += "Active";
		}
		else if (info[5] == "1")
		{
			strState  += "Forbidden point";
			strResult += "0-1";
		}
		else if (info[5] == "2")
		{
			strState  += "Draw";
			strResult += "0.5-0.5";
		}
		else if (info[5] == "3")
		{
			strState  += "Black won";
			strResult += "1-0";
		}
		else if (info[5] == "4")
		{
			strState  += "White won";
			strResult += "0-1";
		}
		else if (info[5] == "5")
		{
			strState  += "White resign";
			strResult += "1-0";
		}
		else if (info[5] == "6")
		{
			strState  += "Black resign";
			strResult += "0-1";
		}
		else if (info[5] == "7")
		{
			strState += "White untime";
		}
		else if (info[5] == "8")
		{
			strState += "Black untime";
		}

		m_Game.addOneLineComment(m_Game.numberOfMoves(), strResult);

		m_Game.addMultiLineComment(m_Game.numberOfMoves(), strTableNumber);
		m_Game.addMultiLineComment(m_Game.numberOfMoves(), strRule);
		m_Game.addMultiLineComment(m_Game.numberOfMoves(), strSwap);
		m_Game.addMultiLineComment(m_Game.numberOfMoves(), strState);
	}
}

//------------------------------------------------------------------------

bool Pos::Save(const CString& fileName, const MoveList& aMoveList)
{
	if (!m_file.Open(fileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
	{
		CString strMessage(Utils::GetString(IDS_MSG_OPEN_FILE, fileName));
		Utils::ShowMessage(strMessage, Utils::GetString(IDS_CAP_CREATE_POS), MB_ICONERROR);
		return false;
	}

	m_buffer[0] = 0;

	for(int i= 1; i <= aMoveList.Index(); i++)
	{
		CPoint Coord(aMoveList.Get(i)->getPos());
		m_buffer[++m_buffer[0]] = BYTE((Coord.y - 1) * 15 + Coord.x - 1);
	}

	m_file.Write(m_buffer, m_buffer[0] + 1);

	m_file.Close();

	return(true);
}

//------------------------------------------------------------------------
