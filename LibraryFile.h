// LibraryFile.h: interface for the LibraryFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIBRARYFILE_H__D04656A2_2118_11D5_92A3_A76A2BB73024__INCLUDED_)
#define AFX_LIBRARYFILE_H__D04656A2_2118_11D5_92A3_A76A2BB73024__INCLUDED_

#include "MoveNode.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class LibraryFile  
{
public:
	LibraryFile();
	virtual ~LibraryFile();

	bool OpenRead(const CString& fileName);
	bool OpenWrite(const CString& fileName);

	bool Get(BYTE& data1, BYTE& data2);
	bool Get(MoveNode& node);

	void Put(BYTE data1, BYTE data2);
	void Put(const MoveNode& node);
	void Put(const CString& str);

	CString GetFilePath() const;

	bool CheckVersion();
	void WriteHeader(bool isOldFormat);

	void Close();

	CString GetVersion() const;

private:
	bool Open(const CString& fileName, UINT mode);
	void Write();

private:
	enum { BUFFERSIZE = 1024 };

	CFile   m_file;
	BYTE    m_buffer[BUFFERSIZE];
	int     m_indexStart;
	int     m_indexEnd;
	UINT    m_mode;
	CString m_Version;
	BYTE    m_MajorFileVersion;
	BYTE    m_MinorFileVersion;
};

#endif // !defined(AFX_LIBRARYFILE_H__D04656A2_2118_11D5_92A3_A76A2BB73024__INCLUDED_)
