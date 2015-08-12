// Wzq.cpp: implementation of the Wzq class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "renlib.h"
#include "Wzq.h"
#include "Utils.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------
// const
//--------------------------------------------------------------------

namespace
{
	const Size = 1024;     // Record Size
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Wzq::Wzq()
: m_dwRead(0)
{
}

//------------------------------------------------------------------------

Game& Wzq::getGame()
{
	return m_Game;
}

//------------------------------------------------------------------------

CString Wzq::getComment()
{
	return m_strComment;
}

//------------------------------------------------------------------------

CString Wzq::getFilePath()
{
	return m_strFilePath;
}

//------------------------------------------------------------------------

bool Wzq::OpenFile(const CString& strFile)
{
	CFileException e;

	if (!m_wzqFile.Open(strFile, CFile::modeRead, &e))
	{
		return false;
	}

	m_strFilePath = m_wzqFile.GetFilePath();

	Parse();

	return true;
}

//------------------------------------------------------------------------

void Wzq::Parse()
{
	const int nameIndex = 10;

	BYTE buffer[Size];

	m_Game.clear();

	m_strComment.Empty();

	int  index = 0;

	BYTE a5 = 0;
	BYTE b5 = 0;

	if (m_dwRead = m_wzqFile.Read(buffer, Size))
	{
		//
		// Read player names
		//
		int nChar = buffer[nameIndex];

		index = nameIndex + 1;

		for (int i=1; i <= nChar; i++)
		{
			m_strComment += buffer[index++];
		}

		m_strComment += " - ";

		nChar = buffer[index++];

		for (i=0; i < nChar; i++)
		{
			m_strComment += buffer[index++];
		}

		for (i=index; i < (int)m_dwRead; i++)
		{
			if (buffer[i] == 0x70)
			{
				break;
			}
		}

		//
		// Read moves
		//
		for (; i < (int)m_dwRead; i++)
		{
			if (buffer[i] != 0)
			{
				BYTE data = buffer[i];

				if (m_Game.numberOfMoves() == 4)
				{
					if (a5 == 0)
					{
						a5 = data;
						continue;
					}
					else if (b5 == 0)
					{
						b5 = data;
						continue;
					}
					else if (data == a5)
					{
						data = b5;
					}
					else
					{
						data = a5;
					}
				}

				int y = data % 15 + 1;
				int x = data / 15 + 1;

				m_Game.addPos((y-1) * 16 + x);
			}
		}
	}

	m_wzqFile.Close();
}

//------------------------------------------------------------------------
