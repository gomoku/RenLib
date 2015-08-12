// TextBoard.h: interface for the TextBoard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXTBOARD_H__DEFEADC2_1E5E_11D6_92A3_0000E89F396C__INCLUDED_)
#define AFX_TEXTBOARD_H__DEFEADC2_1E5E_11D6_92A3_0000E89F396C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class MoveList;

class TextBoard  
{
public:
	const static CString cHeader;

	TextBoard();
	virtual ~TextBoard();

	void Create(const MoveList& aMoveList);
	bool Save(const CString& fileName);

private:
	enum { NOOFROWS = 20 };

	CStdioFile m_file;
	CString    m_Board[NOOFROWS];
	CString    m_strBoard;
};

#endif // !defined(AFX_TEXTBOARD_H__DEFEADC2_1E5E_11D6_92A3_0000E89F396C__INCLUDED_)
