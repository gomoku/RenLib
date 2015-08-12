// Renartist.h: interface for the Renartist class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RENARTIST_H__821389C7_F056_4DF0_90C7_B631D092435A__INCLUDED_)
#define AFX_RENARTIST_H__821389C7_F056_4DF0_90C7_B631D092435A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class MoveList;

class Renartist  
{
public:
	Renartist();
	virtual ~Renartist();

	bool Create (const CString& fileName, const MoveList& aMoveList);

private:
	CStdioFile m_file;

	void WriteLine(const CString& line);
};

#endif // !defined(AFX_RENARTIST_H__821389C7_F056_4DF0_90C7_B631D092435A__INCLUDED_)
