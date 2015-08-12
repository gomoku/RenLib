// Pos.h: interface for the Pos class.
//
//////////////////////////////////////////////////////////////////////

#include "Game.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class MoveList;

class Pos
{
public:
    Pos();
    virtual ~Pos();

    bool Open(const CString& strFile);
    bool Read();
    bool Save(const CString& fileName, const MoveList& aMoveList);

	Game&	getGame();
	CString	getFilePath();

private:
	void addZjrenjuComments();

private:
	enum { BUFFERSIZE = 226 };

	CFile   m_file;
	BYTE    m_buffer[BUFFERSIZE];

public:
    Game    m_Game;
    CString m_strFilePath;
};
