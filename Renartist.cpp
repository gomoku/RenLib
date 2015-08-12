// Renartist.cpp: implementation of the Renartist class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "renlib.h"
#include "Renartist.h"
#include "MoveList.h"
#include "MoveNode.h"
#include "Utils.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

namespace
{
	void addByte(CString& strValue, BYTE value)
	{
		CString strHex;
		strHex.Format("%02x", value);
		strValue += strHex;
	}

	void addOneLineComment(CString& strValue, CString& strComment)
	{
		CString strNew(strComment);

		while (strNew.GetLength() < 79)
		{
			strNew += ' ';
		}

		strNew += TCHAR(0);
		
		if (strNew.GetLength() % 2)
		{
			strNew += TCHAR(0);
		}

		for (int i=0; i < strNew.GetLength(); i++)
		{
			addByte(strValue, strNew[i]);
		}
	}

	void addMultiLineComment(CString& strValue, CString& strMultiLineComment)
	{
		int nLeft = 0;
		int nRight;

		while (nLeft < strMultiLineComment.GetLength())
		{
			nRight = strMultiLineComment.Find(TCHAR(13), nLeft);

			if (nRight == -1)
			{
				nRight = strMultiLineComment.GetLength();
			}				

			CString strLine(strMultiLineComment.Mid(nLeft, nRight - nLeft));

			strValue += strLine + TCHAR(10);

			nLeft = nRight + 2;
		}
	}
}

Renartist::Renartist()
{
}

Renartist::~Renartist()
{
}

bool Renartist::Create (const CString& fileName, const MoveList& aMoveList)
{
	if (!m_file.Open(fileName, CFile::modeCreate | CFile::modeWrite | CFile::typeText))
	{
		CString strMessage(Utils::GetString(IDS_MSG_OPEN_FILE, fileName));
		Utils::ShowMessage(strMessage, Utils::GetString(IDS_CAP_CREATE_RENARTIST_APPLET), MB_ICONERROR);
		return false;
	}

	CString strTitle("renartist applet");

	CString strComment(aMoveList.Current()->getOneLineComment());

	if (!strComment.IsEmpty())
	{
		strTitle = strComment;
	}

	CString strValue;

	for (int i = 1; i <= aMoveList.Index(); i++)
	{
		addByte(strValue, Utils::PointToPos(aMoveList.Get(i)->getPos()));

		CString strInfo;

		if (i < aMoveList.Index())
		{
			strInfo = "0";
		}
		else
		{
			strInfo = "4"; // Last move
		}

		strValue += strInfo;

		CString strComment(aMoveList.Get(i)->getOneLineComment());

		if (i == 1 && strComment.IsEmpty())
		{
			strComment = "renartist applet";
		}

		if (strComment.IsEmpty())
		{
			strInfo = "0";
		}
		else
		{
			strInfo = "8";
		}

		strValue += strInfo;

		if (!strComment.IsEmpty())
		{
			addOneLineComment(strValue, strComment);
		}
	}
	
	WriteLine("<html>");
	WriteLine("<head>");
	WriteLine("<meta name=\"GENERATOR\" content=\"RenLib\">");
	WriteLine("<title>" + strTitle + "</title>");
	WriteLine("</head>");
	WriteLine("<body>");
	WriteLine("<div align=\"center\">");
	WriteLine("<center>");
	WriteLine("<table border=\"0\" cellpadding=\"0\" cellspacing=\"4\">");
	WriteLine("<tr>");
	WriteLine("<td valign=\"bottom\" colspan=\"3\">");
	WriteLine("<p><font size=\"6\">&nbsp;&nbsp;&nbsp;" + strTitle + "</font></p></td>");
	WriteLine("</tr>");
	WriteLine("<tr>");
	WriteLine("<td valign=\"top\" width=\"330\">");
	WriteLine("<br>");
	WriteLine("<applet code=\"Rena.class\" codebase=\"http://www.renju.se/renlib/renartist/\" width=330 height=354 align=\"left\" hspace=\"3\">");
	WriteLine("<param name=data value=\"" + strValue + "\">");

	WriteLine("</applet>");
	WriteLine("</td>");
	WriteLine("<td width=\"15\" rowspan=\"2\"></td>");
	WriteLine("<td valign=\"top\" width=\"330\" rowspan=\"2\" height=\"420\"><font size=\"3\">");
	WriteLine("<br><br>");

	WriteLine("<textarea rows=\"19\" name=\"comment\" cols=\"30\">");

	for (i = 1; i <= aMoveList.Index(); i++)
	{
		CString strMultiLineComment(aMoveList.Get(i)->getMultiLineComment());

		if (!strMultiLineComment.IsEmpty())
		{
			strValue.Empty();
			addMultiLineComment(strValue, strMultiLineComment);

			CString strLine;
			strLine.Format("%d. %s", i, strValue);
			WriteLine(strLine);
		}
	}

	WriteLine("</textarea>");
	WriteLine("</font></td>");
	WriteLine("</tr>");
	WriteLine("<tr>");
	WriteLine("<td valign=\"top\" width=\"330\">");
	WriteLine("<br><i>renartist applet commands:<br>left click (forward) - right click (back)</i>");
	WriteLine("</td>");
	WriteLine("</tr>");
	WriteLine("</table>");
	WriteLine("</center>");
	WriteLine("</div>");
	WriteLine("</body>");
	WriteLine("</html>");

	m_file.Close();

	return true;
}

void Renartist::WriteLine(const CString& line)
{
	m_file.WriteString(line + TCHAR(10));
}
