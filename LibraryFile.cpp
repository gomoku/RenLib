// LibraryFile.cpp: implementation of the LibraryFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "renlib.h"
#include "LibraryFile.h"
#include "Utils.h"
#include "RenLibDoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace
{
	const HEADER_SIZE = 20;

	const MAJOR_FILE_VERSION_INDEX = 8;
	const MINOR_FILE_VERSION_INDEX = 9;

	const MAJOR_FILE_VERSION = 3;
	const MINOR_FILE_VERSION = 4;

	const MAJOR_FILE_VERSION_H8  = 3;
	const MINOR_FILE_VERSION_OLD = 0;

	const CENTER = 0x78;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LibraryFile::LibraryFile() :
	m_indexStart(0),
	m_indexEnd(0),
	m_mode(UINT_MAX),
	m_MajorFileVersion(0),
	m_MinorFileVersion(0)
{
}

//------------------------------------------------------------------------

LibraryFile::~LibraryFile()
{
}

//------------------------------------------------------------------------

bool LibraryFile::OpenRead(const CString& fileName)
{
	return Open(fileName, CFile::modeRead);
}

//------------------------------------------------------------------------

bool LibraryFile::OpenWrite(const CString& fileName)
{
	return Open(fileName, CFile::modeCreate | CFile::modeWrite);
}

//------------------------------------------------------------------------

bool LibraryFile::Open(const CString& fileName, UINT mode)
{
	m_mode = mode;
	CFileException e;
	return (m_file.Open(fileName, m_mode, &e ) != 0);
}

//------------------------------------------------------------------------

bool LibraryFile::Get(MoveNode& node)
{
	node.setPos(CPoint(0,0));
	node.clearInformation();

	BYTE data1;
	BYTE data2;

	bool success = Get(data1, data2);

	if (success)
	{
		node.setPosInfo(data1, data2);

		if (node.isExtension())
		{
			success = Get(data1, data2);
			node.setExtendedInfo(data1, data2);
		}
	}

	return success;
}

//------------------------------------------------------------------------

bool LibraryFile::Get(BYTE& data1, BYTE& data2)
{
	data1 = 0;
	data2 = 0;

	if (m_indexStart >= m_indexEnd)
	{
		UINT nBytesRead = m_file.Read(m_buffer, BUFFERSIZE);

		if (nBytesRead == 0)
		{
			return false;
		}

		m_indexEnd   = nBytesRead - 1;
		m_indexStart = 0;
	}

	data1 = m_buffer[m_indexStart++];
	data2 = m_buffer[m_indexStart++];

	return true;
}

//------------------------------------------------------------------------

void LibraryFile::Write()
{
	if (m_mode == (CFile::modeCreate | CFile::modeWrite))
	{
		if (m_indexEnd)
		{
			m_file.Write(m_buffer, m_indexEnd);
			m_indexEnd = 0;
		}
	}
}

//------------------------------------------------------------------------

void LibraryFile::Put(BYTE data1, BYTE data2)
{
	if (m_indexEnd >= BUFFERSIZE)
	{
		Write();
	}

	m_buffer[m_indexEnd++] = data1;

	if (m_indexEnd >= BUFFERSIZE)
	{
		Write();
	}

	m_buffer[m_indexEnd++] = data2;
}

//------------------------------------------------------------------------

void LibraryFile::Put(const MoveNode& node)
{
	BYTE data1;
	BYTE data2;

	node.getPosInfo(data1, data2);
	Put(data1, data2);

	if (node.isExtension())
	{
		node.getExtendedInfo(data1, data2);
		Put(data1, data2);
	}
}

//------------------------------------------------------------------------

void LibraryFile::Put(const CString& str)
{
	for (int i=0; i < str.GetLength(); i++)
	{
		if (m_indexEnd >= BUFFERSIZE)
		{
			Write();
		}

		m_buffer[m_indexEnd++] = str[i];
	}
}

//------------------------------------------------------------------------

void LibraryFile::Close()
{
	try
	{
		Write();
		m_file.Close();
	}
	catch (...)
	{
	}
}

//------------------------------------------------------------------------

CString LibraryFile::GetFilePath() const
{
	return m_file.GetFilePath();
}

//------------------------------------------------------------------------

bool LibraryFile::CheckVersion()
{
	bool VersionOk = false;

	//                            0     1    2    3    4    5    6    7
	BYTE header[HEADER_SIZE] = { 0xFF, 'R', 'e', 'n', 'L', 'i', 'b', 0xFF };

	BYTE buf[HEADER_SIZE];
	DWORD dwRead;

	dwRead = m_file.Read(buf, HEADER_SIZE);

	if (dwRead == HEADER_SIZE)
	{
		bool HeaderMatch = true;

		for (int i=0; i<=7; i++)
		{
			if (buf[i] != header[i])
			{
				HeaderMatch = false;
				break;
			}
		}

		if (HeaderMatch)
		{
			m_MajorFileVersion = buf[MAJOR_FILE_VERSION_INDEX];
			m_MinorFileVersion = buf[MINOR_FILE_VERSION_INDEX];

			if (100 * m_MajorFileVersion + m_MinorFileVersion <=
				100 * MAJOR_FILE_VERSION + MINOR_FILE_VERSION)
			{
				VersionOk = true;
			}
			else
			{
				CString strMessage(Utils::GetString(IDS_MSG_VERSION, m_MajorFileVersion, m_MinorFileVersion));
				Utils::ShowMessage(strMessage, Utils::GetString(IDS_CAP_VERSION), MB_ICONINFORMATION);
			}
		}
		else if (buf[0] == CENTER)
		{
			m_file.SeekToBegin();
			VersionOk = true;
		}
	}

	if (!VersionOk)
	{
		CString strMessage(Utils::GetString(IDS_INVALID_LIB, m_file.GetFileName()));
		Utils::ShowMessage(strMessage, Utils::GetString(IDS_CAP_VERSION), MB_ICONERROR);
	}

	return VersionOk;
}

//------------------------------------------------------------------------

void LibraryFile::WriteHeader(bool isOldFormat)
{
	//                            0      1     2     3     4     5     6    7
	BYTE header[HEADER_SIZE] = { 0xFF,  'R',  'e',  'n',  'L',  'i',  'b', 0xFF,
	//                            8      9    10    11    12    13    14   15
		                         0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	//                           16     17    18    19
								 0xFF, 0xFF, 0xFF, 0xFF };

	header[MAJOR_FILE_VERSION_INDEX] = MAJOR_FILE_VERSION;
	
	if (isOldFormat)
	{
		header[MINOR_FILE_VERSION_INDEX] = MINOR_FILE_VERSION_OLD;
	}
	else
	{
		header[MINOR_FILE_VERSION_INDEX] = MINOR_FILE_VERSION;
	}

	m_file.Write(header, HEADER_SIZE);
}

//------------------------------------------------------------------------

CString LibraryFile::GetVersion() const
{
	return m_Version;
}

//------------------------------------------------------------------------
