// Renjuclass.cpp: implementation of the Renjuclass class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "renlib.h"
#include "Renjuclass.h"
#include "MoveList.h"
#include "MoveNode.h"
#include "Utils.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace
{
	const CString cPrefix("<script src=http://ns02.compute.com.tw/~renjuclass/cgi-bin/forum/diagram_show.cgi?data=");
	const CString cSuffix("></script>");
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Renjuclass::Renjuclass()
{
}

Renjuclass::~Renjuclass()
{
}

void Renjuclass::Create(const MoveList& aMoveList)
{
	for (int i = 1; i <= aMoveList.Index(); i++)
	{
		CPoint Coord(aMoveList.Get(i)->getPos());
		CString strMove(Utils::XCoordinateImage(Coord.x, true, false) + Utils::YCoordinateImage(Coord.y, true));
		strMove.MakeLower();
		m_strMoves += strMove + "+";
	}
	
	m_strMoves.Delete(m_strMoves.GetLength()-1);

	m_strOneLineComment = aMoveList.Get(aMoveList.Index())->getOneLineComment();

	if (m_strOneLineComment.IsEmpty())
	{
		m_strOneLineComment = "Renjuclass Diagram";
	}
}

bool Renjuclass::Save(const CString& fileName)
{
	if (!m_file.Open(fileName, CFile::modeCreate | CFile::modeWrite | CFile::typeText))
	{
		CString strMessage(Utils::GetString(IDS_MSG_OPEN_FILE, fileName));
		Utils::ShowMessage(strMessage, Utils::GetString(IDS_CAP_CREATE_RENJUCLASS_DIAGRAM), MB_ICONERROR);
		return false;
	}

	m_file.WriteString("<html>\n");
	m_file.WriteString("<head>");
	m_file.WriteString("<title>Renjuclass Diagram</title>\n");
	m_file.WriteString("</head>\n");
	m_file.WriteString("<body>\n");
	m_file.WriteString(m_strOneLineComment + "<br>");
	m_file.WriteString(cPrefix + m_strMoves + cSuffix + "\n");
	m_file.WriteString("</body>\n");
	m_file.WriteString("</html>\n");

	m_file.Close();

	return true;
}
