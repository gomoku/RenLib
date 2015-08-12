// Renjuclass.h: interface for the Renjuclass class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RENJUCLASS_H__DEFEADC1_1E5E_11D6_92A3_0000E89F396C__INCLUDED_)
#define AFX_RENJUCLASS_H__DEFEADC1_1E5E_11D6_92A3_0000E89F396C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class MoveList;
class Game;

class Renjuclass
{
public:
	Renjuclass();
	virtual ~Renjuclass();

	void Create(const MoveList& aMoveList);
	bool Save(const CString& fileName);

private:
	CStdioFile m_file;
	CString    m_strMoves;
	CString    m_strOneLineComment;
};

#endif // !defined(AFX_RENJUCLASS_H__DEFEADC1_1E5E_11D6_92A3_0000E89F396C__INCLUDED_)
