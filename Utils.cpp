// Utils.cpp: implementation of the Utils class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Utils.h"
#include "RenLib.h"
#include "cgfiltyp.h"	//	CGCFileTypeAccess
#include "FolderDlg.h"
#include "AmHttpSocket.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace
{
	const BYTE ch_aa = 0xE5;
	const BYTE ch_ae = 0xE4;
	const BYTE ch_oe = 0xF6;
	const BYTE ch_AA = 0xC5;
	const BYTE ch_AE = 0xC4;
	const BYTE ch_OE = 0xD6;

	const CString cBak(_T("bak"));
	const CString cBmp(_T("bmp"));
	const CString cBuf(_T("buf"));
	const CString cHtm(_T("htm"));
	const CString cHtml(_T("html"));
	const CString cLib(_T("lib"));
	const CString cPdb(_T("pdb"));
	const CString cPos(_T("pos"));
	const CString cRenjs(_T("renjs"));
	const CString cRen(_T("ren"));
	const CString cTxt(_T("txt"));
	const CString cWzq(_T("wzq"));
	const CString cSgf(_T("sgf"));
	const CString cBdt(_T("bdt"));
	const CString cRdf(_T("rdf"));

	const CString cOpenFilterAll(
        _T("*.buf;*.lib;*.pdb;*.pos;*.renjs;*.ren;*.txt;*.wzq;*.sgf;*.bdt;*.rdf"));

	const COLORREF cWhite = RGB(255, 255, 255);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Utils::Utils()
{
}

//------------------------------------------------------------------------

Utils::~Utils()
{
}

//------------------------------------------------------------------------

int Utils::msb (BYTE ch)
{
	return (ch & 0x80);
}

//------------------------------------------------------------------------

bool Utils::bit_is_one (int bit_value, UINT value)
{
	return ((value & bit_value) != 0);
}

//------------------------------------------------------------------------

void Utils::clear_bit (int bit_value, UINT& value)
{
	value &= ~bit_value;
}

//------------------------------------------------------------------------

void Utils::set_bit (int bit_value, UINT& value)
{
	value |= bit_value;
}

//------------------------------------------------------------------------

bool Utils::exists (const CString& strFile)
{
	CFileStatus status;
	return CFile::GetStatus(strFile, status) == TRUE;
}

//------------------------------------------------------------------------

void Utils::remove (const CString& strFile)
{
	try
	{
		CFile::Remove( strFile );
	}
	catch(...)
	{
	}
}

//------------------------------------------------------------------------

bool Utils::rename (const CString& strOldFileName, CString& strNewFileName)
{
	try
	{
		CFile::Rename( strOldFileName, strNewFileName );
	}
	catch(...)
	{
		return false;
	}

	return true;
}

//------------------------------------------------------------------------

void Utils::AsciiToWinChar (CString& strComment)
{
	for (int i = 0; i < strComment.GetLength(); i++)
	{
		BYTE ch = BYTE(strComment.GetAt(i) & 0x7F);

		switch (ch)
		{
			case  '}' : ch = ch_aa; break;
			case  '{' : ch = ch_ae; break;
			case  '|' : ch = ch_oe; break;
			case  ']' : ch = ch_AA; break;
			case  '[' : ch = ch_AE; break;
			case '\\' : ch = ch_OE; break;
		}

		strComment.SetAt(i, ch);
	}
}

//------------------------------------------------------------------------

void Utils::WinCharToAscii (CString& strComment)
{
	for (int i = 0; i < strComment.GetLength(); i++)
	{
		BYTE ch = BYTE(strComment.GetAt(i));

		switch (ch)
		{
			case  ch_aa : ch = '}';  break;
			case  ch_ae : ch = '{';  break;
			case  ch_oe : ch = '|';  break;
			case  ch_AA : ch = ']';  break;
			case  ch_AE : ch = '[';  break;
			case  ch_OE : ch = '\\'; break;

			case   8 :
			case  13 :
			case  10 : break;

			default:
				if (ch < ' ' || ch > '~')
				{
					ch = ' ';
				}
				break;
		}

		strComment.SetAt(i, ch);
	}
}

//------------------------------------------------------------------------

CString Utils::Center(CString strComment, int nChar)
{
	Utils::trim(strComment);

	if (nChar > strComment.GetLength())
	{
		int n = nChar - strComment.GetLength();

		for (int i = 1; i < (n+1)/2; i++)
		{
			strComment = ' ' + strComment;
		}
	}

	return strComment;
}

//------------------------------------------------------------------------

bool Utils::IsSubstring(CString strString, CString strCheck)
{
	Utils::trim(strString);
	strString.MakeUpper();

	Utils::trim(strCheck);
	strCheck.MakeUpper();

	return (strstr(strString, strCheck) != NULL);
}

//------------------------------------------------------------------------

bool Utils::OpenFileDialog(CString& strFile)
{
	CFileDialog dlg(TRUE);

	std::vector<CString> filter = getOpenFilter();
	CString strFilter(makeFilter(filter));
	dlg.m_ofn.lpstrFilter = strFilter;

	dlg.m_ofn.lpstrDefExt = NULL;

	dlg.m_ofn.Flags |= OFN_FILEMUSTEXIST;

	if (dlg.DoModal() == IDOK)
	{
		strFile = dlg.GetPathName();
		return true;
	}

	return false;
}

//------------------------------------------------------------------------

bool Utils::SavePositionDialog(CString& strFile, FileType& fileType)
{
	fileType = NONE;

	std::vector<CString>  filter;
	std::vector<FileType> fileTypes;

	getSavePositionFilter(filter, fileTypes);

	return SaveDialog(strFile, fileType, filter, fileTypes);
}

//------------------------------------------------------------------------

bool Utils::SaveGameCollectionDialog(CString& strFile, FileType& fileType)
{
	fileType = NONE;

	std::vector<CString>  filter;
	std::vector<FileType> fileTypes;

	getSaveGameCollectionFilter(filter, fileTypes);

	return SaveDialog(strFile, fileType, filter, fileTypes);
}

//------------------------------------------------------------------------

bool Utils::SaveLibraryDialog(CString& strFile, FileType& fileType)
{
	fileType = NONE;

	std::vector<CString>  filter;
	std::vector<FileType> fileTypes;

	getSaveLibraryFilter(filter, fileTypes);

	return SaveDialog(strFile, fileType, filter, fileTypes);
}

//------------------------------------------------------------------------

bool Utils::SaveDialog(CString& strFile, FileType& fileType, std::vector<CString>& filter, std::vector<FileType>& fileTypes)
{
	CFileDialog dlg(FALSE);

	CString strFilter(makeFilter(filter));
	dlg.m_ofn.lpstrFilter = strFilter;

	dlg.m_ofn.lpstrDefExt = NULL;

	CString initalDirectory(getCurrentDirectory());
	dlg.m_ofn.lpstrInitialDir = initalDirectory;

	dlg.m_ofn.Flags |= OFN_CREATEPROMPT;

	if (dlg.DoModal() == IDOK)
	{
		int typeIndex = dlg.m_ofn.nFilterIndex;
		strFile = getFileName(filter, typeIndex, dlg.GetPathName());
		fileType = fileTypes[typeIndex - 1];
		return true;
	}

	return false;
}

//------------------------------------------------------------------------

CString Utils::GetExtension(const CString& strFile)
{
	int index = strFile.ReverseFind('.');

	if (index == -1)
	{
		return "";
	}
	else
	{
		return strFile.Mid(index + 1);
	}
}

//------------------------------------------------------------------------

CString Utils::GetTitle(const CString& strFile)
{
	int index1 = strFile.ReverseFind('\\');
	int index2 = strFile.ReverseFind('.');

	if (index1 == -1 || index2 == -1)
	{
		return "";
	}
	else
	{
		return strFile.Mid(index1 + 1, index2 - index1 - 1);
	}
}

//------------------------------------------------------------------------

bool Utils::IsExtension(const CString& strFile, const CString& strExt)
{
	return strExt.CompareNoCase(GetExtension(strFile)) == 0;
}

//------------------------------------------------------------------------

bool Utils::IsExtensionLib(const CString& strFile)
{
	return IsExtension(strFile, cLib);
}

//------------------------------------------------------------------------

bool Utils::IsExtensionTxt(const CString& strFile)
{
	return IsExtension(strFile, cTxt);
}

//------------------------------------------------------------------------

bool Utils::IsExtensionWzq(const CString& strFile)
{
	return IsExtension(strFile, cWzq);
}

//------------------------------------------------------------------------

bool Utils::IsExtensionBuf(const CString& strFile)
{
	return IsExtension(strFile, cBuf);
}

//------------------------------------------------------------------------

bool Utils::IsExtensionPdb(const CString& strFile)
{
	return IsExtension(strFile, cPdb);
}

//------------------------------------------------------------------------

bool Utils::IsExtensionPos(const CString& strFile)
{
    return IsExtension(strFile, cPos);
}

//------------------------------------------------------------------------

bool Utils::IsExtensionRenjs(const CString& strFile)
{
	return IsExtension(strFile, cRenjs) || IsExtension(strFile, cRen);
}

//------------------------------------------------------------------------

bool Utils::IsExtensionHtml(const CString& strFile)
{
	return IsExtension(strFile, cHtml) || IsExtension(strFile, cHtm);
}

//------------------------------------------------------------------------

bool Utils::IsExtensionSgf(const CString& strFile)
{
	return IsExtension(strFile, cSgf);
}

//------------------------------------------------------------------------

bool Utils::IsExtensionBdt(const CString& strFile)
{
	return IsExtension(strFile, cBdt);
}

//------------------------------------------------------------------------

bool Utils::IsExtensionRdf(const CString& strFile)
{
	return IsExtension(strFile, cRdf);
}

//------------------------------------------------------------------------

bool Utils::IsExtensionAccepted(const CString& strFile)
{
	const CString strExt(GetExtension(strFile));

	return
		(strExt.CompareNoCase(cBuf)   == 0) ||
		(strExt.CompareNoCase(cLib)   == 0) ||
		(strExt.CompareNoCase(cPdb)   == 0) ||
		(strExt.CompareNoCase(cPos)   == 0) ||
		(strExt.CompareNoCase(cRenjs) == 0) ||
		(strExt.CompareNoCase(cRen)   == 0) ||
		(strExt.CompareNoCase(cTxt)   == 0) ||
		(strExt.CompareNoCase(cWzq)   == 0) ||
		(strExt.CompareNoCase(cSgf)   == 0) ||
		(strExt.CompareNoCase(cBdt)   == 0);
}

//------------------------------------------------------------------------

void Utils::MakeExtension(CString& strFile, const CString& strExt)
{
	int index = strFile.ReverseFind('.');

	if (index != -1)
	{
		strFile.Delete(index, strFile.GetLength() - index);
	}

	strFile += "." + strExt;
}

//------------------------------------------------------------------------

void Utils::MakeExtensionLib(CString& strFile)
{
	MakeExtension(strFile, cLib);
}

//------------------------------------------------------------------------

void Utils::MakeExtensionBak(CString& strFile)
{
	MakeExtension(strFile, cBak);
}

//------------------------------------------------------------------------

bool Utils::OpenSaveDialog(CString& strFile, const std::vector<CString>& filter, const CString& strInitialDirectory)
{
	CFileDialog dlg(true);

	CString strFilter(makeFilter(filter));
	dlg.m_ofn.lpstrFilter = strFilter;

	dlg.m_ofn.lpstrDefExt = NULL;

	dlg.m_ofn.lpstrInitialDir = strInitialDirectory;

	dlg.m_ofn.Flags |= OFN_FILEMUSTEXIST;

	if (dlg.DoModal() == IDOK)
	{
		strFile = dlg.GetPathName();
		return true;
	}

	return false;
}

//------------------------------------------------------------------------

bool Utils::OpenMultiFileDialog(CStringList& fileNameList)
{
	CFileDialog dlg(TRUE);
	std::vector<CString> filter = getOpenFilter();

	CString strFilter(makeFilter(filter));
	dlg.m_ofn.lpstrFilter = strFilter;

	dlg.m_ofn.lpstrDefExt = NULL;

	dlg.m_ofn.Flags |= OFN_FILEMUSTEXIST|OFN_ALLOWMULTISELECT;

	const int maxChar = 5000;
	CString strBuffer;
	dlg.m_ofn.lpstrFile = strBuffer.GetBuffer(maxChar);
	dlg.m_ofn.nMaxFile = maxChar;

	int choice = dlg.DoModal();
	strBuffer.ReleaseBuffer();

	if (choice == IDOK)
	{
		POSITION pos = dlg.GetStartPosition();

		while (pos != NULL)
		{
			fileNameList.AddTail(dlg.GetNextPathName(pos));
		}

		return true;
	}

	return false;
}

//------------------------------------------------------------------------

bool Utils::OpenBmpDialog(CString& strFile, const CString& strDirectory)
{
	std::vector<CString> filter;

	filter.push_back(Utils::GetString(IDS_FILTER_BMP_FILE));
	filter.push_back("*.bmp");

	if (exists(strDirectory))
	{
		return OpenSaveDialog(strFile, filter, strDirectory);
	}
	else
	{
		return OpenSaveDialog(strFile, filter, getCurrentDirectory());
	}
}

//------------------------------------------------------------------------

bool Utils::StringToInteger(const CString& strInt, int& intValue)
{
	intValue = 0;

	if (strInt.IsEmpty())
	{
		return false;
	}

	for (int i = 0; i < strInt.GetLength(); i++)
	{
		if (::isdigit(strInt[i]))
		{
			intValue = 10*intValue + strInt[i] - '0';
		}
		else
		{
			intValue = 0;
			return false;
		}
	}

	return true;
}

//------------------------------------------------------------------------

bool Utils::HexToInt(const TCHAR& hex, int& intValue)
{
	intValue = 0;
    bool retValue = true;

    if (hex >= '0' && hex <= '9')
    {
        intValue = hex - '0';
    }
    else if (hex >= 'A' && hex <= 'F')
    {
        intValue = 10 + hex - 'A';
    }
    else
	{
		retValue = false;
	}

    return retValue;
}

//------------------------------------------------------------------------

bool Utils::IntToHex(const int intValue, TCHAR& hex)
{
	hex = '0';
    bool retValue = true;

    if (intValue >= 0 && intValue <= 9)
    {
        hex = '0' + intValue;
    }
    else if (intValue >= 10 && intValue <= 15)
    {
        hex = 'A' - 10 + intValue;
    }
    else
	{
		retValue = false;
	}

    return retValue;
}

//------------------------------------------------------------------------

void Utils::Beep()
{
	MessageBeep(MB_ICONEXCLAMATION);
}

//------------------------------------------------------------------------

CPoint Utils::PosToPoint(int pos)
{
	if (pos == 0)
	{
		return CPoint(0,0);
	}
	else
	{
		return CPoint(pos % 16, pos / 16 + 1);
	}
}

//------------------------------------------------------------------------

BYTE Utils::PointToPos(CPoint Point)
{
	if (isValidPoint(Point))
	{
		return 16 * (BYTE(Point.y) - 1) + BYTE(Point.x);
	}
	else
	{
		return 0;
	}
}

//------------------------------------------------------------------------

bool Utils::isValidPoint(CPoint Point)
{
    return Point.x >= 1 && Point.x <= 15 && Point.y >= 1 && Point.y <= 15;
}

//------------------------------------------------------------------------

bool Utils::isLessThan(CPoint Point1, CPoint Point2)
{
    return
        (Point1.y < Point2.y) ||
        (Point1.y == Point2.y && Point1.x < Point2.x);
}

//------------------------------------------------------------------------

CString Utils::GetString(UINT nIDPrompt)
{
	CString strText;

	if (strText.LoadString(nIDPrompt))
	{
		return strText;
	}
	else
	{
		strText.Format("Error loading string: %d", nIDPrompt);
		AfxMessageBox(strText);
		return "<?>";
	}
}

//------------------------------------------------------------------------

CString Utils::GetString( UINT nIDPrompt, int nParam )
{
	CString strText;
	strText.Format(GetString(nIDPrompt), nParam);
	return strText;
}

//------------------------------------------------------------------------

CString Utils::GetString( UINT nIDPrompt, int nParam1, int nParam2 )
{
	CString strText;
	strText.Format(GetString(nIDPrompt), nParam1, nParam2);
	return strText;
}

//------------------------------------------------------------------------

CString Utils::GetString( UINT nIDPrompt, const CString& strParam )
{
	CString strText;
	strText.Format(GetString(nIDPrompt), strParam);
	return strText;
}

//------------------------------------------------------------------------

CString Utils::GetString( UINT nIDPrompt, int nParam, const CString& strParam )
{
	CString strText;
	strText.Format(GetString(nIDPrompt), nParam, strParam);
	return strText;
}

//------------------------------------------------------------------------

CString Utils::GetString( UINT nIDPrompt, CPoint pntParam)
{
	CString strText;
	strText.Format(GetString(nIDPrompt), pntParam.x, pntParam.y);
	return strText;
}

//------------------------------------------------------------------------

CString Utils::GetString( UINT nIDPrompt, const CString& strParam, int nParam1, int nParam2 )
{
	CString strText;
	strText.Format(GetString(nIDPrompt), strParam, nParam1, nParam2);
	return strText;
}

//------------------------------------------------------------------------

int Utils::ShowMessage(const CString& strMessage, const CString& strCaption, UINT nType )
{
	return
		AfxGetApp()->m_pMainWnd->MessageBox(strMessage, strCaption, nType);
}

//------------------------------------------------------------------------

CString Utils::XCoordinateImage( int x, bool AO, bool Falling )
{
	CString strX;

	if (x >= 1 && x <= 15)
	{
		if (Falling)
		{
			strX.Format("%c", 'A' + 15 - x + ((!AO && x > 8) ? 1 : 0));
		}
		else
		{
			strX.Format("%c", 'A' + x - 1 + ((!AO && x > 8) ? 1 : 0));
		}
	}

	return strX;
}

//------------------------------------------------------------------------

CString Utils::YCoordinateImage( int y, bool Falling )
{
	CString strY;

	if (y >= 1 && y <= 15)
	{
		if (Falling)
		{
			strY.Format("%d", 16 - y);
		}
		else
		{
			strY.Format("%d", y);
		}
	}

	return strY;
}

//------------------------------------------------------------------------

bool Utils::LessThan(CPoint Left, CPoint Right)
{
	return PointToPos(Left) < PointToPos(Right);
}

//------------------------------------------------------------------------

int Utils::isNumber(const CString& strNumber)
{
	int number = 0;

	for (int i=0; i < strNumber.GetLength(); i++)
	{
		if (isdigit(strNumber[i]))
		{
			number = 10 * number + int(strNumber[i]) - '0';
		}
		else
		{
			return -1;
		}
	}

	return number;
}

//------------------------------------------------------------------------

void Utils::Increment(TCHAR& ch0, TCHAR& ch1)
{
	if (ch1 == 'Z')
	{
		ch1 = 'A';

		if (ch0 == 0)
		{
			ch0 = 'A';
		}
		else
		{
			ch0++;
		}
	}
	else if (ch1 == 'z')
	{
		ch1 = 'a';

		if (ch0 == 0)
		{
			ch0 = 'a';
		}
		else
		{
			ch0++;
		}
	}
	else
	{
		ch1++;
	}
}

//------------------------------------------------------------------------

void Utils::Increment(CString& strText)
{
	int number = isNumber(strText);

	if (number != -1)
	{
		strText.Format("%d", number+1);
	}
	else if (strText.GetLength() == 1 || strText.GetLength() == 2)
	{
		TCHAR ch0;
		TCHAR ch1;

		if (strText.GetLength() == 1)
		{
			ch0 = 0;
			ch1 = strText[0];
		}
		else if (strText.GetLength() == 2)
		{
			ch0 = strText[0];
			ch1 = strText[1];
		}

		Increment(ch0, ch1);
		strText.Empty();

		if (ch0 != 0)
		{
			strText += ch0;
		}

		strText += ch1;
	}
}

//------------------------------------------------------------------------

void Utils::CopyToClipboard(const CString& strText)
{
	HGLOBAL hGlobal;        // Global memory handle
	LPSTR lpszData;         // Pointer to clipboard data
	unsigned long nSize;    // Size of clipboard data

	// First, open the clipboard. OpenClipboard() takes one
	// parameter, the handle of the window that will temporarily
	// be it's owner. If NULL is passed, the current process
	// is assumed. After opening, empty the clipboard so we
	// can put our text on it.
	OpenClipboard(NULL);
	EmptyClipboard();

	// Get the size of the string in the buffer that was
	// passed into the function, so we know how much global
	// memory to allocate for the string.
	nSize = strText.GetLength();

	// Allocate the memory for the string.
	hGlobal = GlobalAlloc(GMEM_ZEROINIT, nSize+1);

	// If we got any error during the memory allocation,
	// we have been returned a NULL handle.
	if (hGlobal != NULL)
	{
		// Now we have a global memory handle to the text
		// stored on the clipboard. We have to lock this global
		// handle so that we have access to it.
		lpszData = (LPSTR)GlobalLock(hGlobal);

		// Now, copy the text from the buffer into the allocated
		// global memory pointer.
		for (UINT i = 0; i < nSize; ++i)
		{
			*(lpszData + i) = (char)strText.GetAt(i);
		}

		*(lpszData + nSize) = 0;

		// Now, simply unlock the global memory pointer,
		// set the clipboard data type and pointer,
		// and close the clipboard.
		GlobalUnlock(hGlobal);
		SetClipboardData(CF_TEXT, hGlobal);
	}

	CloseClipboard();
}

//------------------------------------------------------------------------

void Utils::PasteFromClipboard(CString& strText)
{
	strText.Empty();

    HGLOBAL hGlobal;        // Global memory handle
    LPSTR lpszData;         // Pointer to clipboard data
    unsigned long nSize;    // Size of clipboard data

    // First, open the clipboard. OpenClipboard() takes one
    // parameter, the handle of the window that will temporarily
    // be it's owner. If NULL is passed, the current process
    // is assumed.
    OpenClipboard(NULL);

    // Request a pointer to the text on the clipboard.
    hGlobal = GetClipboardData(CF_TEXT);

    // If there was no text on the clipboard, we have
    // been returned a NULL handle. 
    if (hGlobal != NULL)
    {
		// Now we have a global memory handle to the text
		// stored on the clipboard. We have to lock this global
		// handle so that we have access to it.
		lpszData = (LPSTR)GlobalLock(hGlobal);

		// Now get the size of the text on the clipboard.
		nSize = GlobalSize(hGlobal);

		CString buffer;
		LPSTR lpszBuffer = (LPSTR) buffer.GetBufferSetLength(nSize);

		// Now, copy the text into the return buffer. At the
		// end, we need to add a NULL string terminator.
		for (UINT i = 0; i < nSize; ++i)
		{
			*(lpszBuffer + i) = *(lpszData + i);
		}

		*(lpszBuffer + nSize) = 0;

		// Now, simply unlock the global memory pointer
		// and close the clipboard.
		GlobalUnlock(hGlobal);

		strText = CString(lpszBuffer);
    }

    CloseClipboard();
}


//------------------------------------------------------------------------

void Utils::trim(CString& strText)
{
	strText.TrimLeft();
	strText.TrimRight();
}

//------------------------------------------------------------------------

void Utils::associateFile(const CString& strExt)
{
	CGCFileTypeAccess TheFTA;

	CString szProgPath(getRenLibPath());

	TheFTA.SetExtension(strExt);

	// just pass file path in quotes on command line
	CString csTempText = szProgPath + " \"%1\"";

	TheFTA.SetShellOpenCommand(csTempText);

	TheFTA.SetDocumentShellOpenCommand(csTempText);

	TheFTA.SetDocumentClassName("RenLib.Document");

	// use first icon in program
	csTempText = szProgPath + ",0";

	TheFTA.SetDocumentDefaultIcon(csTempText);
	
	TheFTA.RegSetAllInfo();
}

//------------------------------------------------------------------------

void Utils::associateLib()
{
	associateFile(cLib);
}

//------------------------------------------------------------------------

void Utils::associateWzq()
{
	associateFile(cWzq);
}

//------------------------------------------------------------------------

void Utils::associateBuf()
{
	associateFile(cBuf);
}

//------------------------------------------------------------------------

void Utils::associatePdb()
{
	associateFile(cPdb);
}

//------------------------------------------------------------------------

void Utils::associatePos()
{
	associateFile(cPos);
}

//------------------------------------------------------------------------

void Utils::associateRenjs()
{
	associateFile(cRenjs);
	associateFile(cRen);
}

//------------------------------------------------------------------------

void Utils::associateSgf()
{
	associateFile(cSgf);
}

//------------------------------------------------------------------------

void Utils::associateBdt()
{
	associateFile(cBdt);
}

//------------------------------------------------------------------------

void Utils::associateRdf()
{
	associateFile(cRdf);
}

//------------------------------------------------------------------------

CString Utils::getCurrentDirectory()
{
	TCHAR szPath[_MAX_PATH * 2];
	::GetCurrentDirectory( _MAX_PATH, szPath );

	return szPath;
}

//------------------------------------------------------------------------

CString Utils::getRenLibPath()
{
	TCHAR szProgPath[_MAX_PATH * 2];
	::GetModuleFileName(NULL, szProgPath, sizeof(szProgPath)/sizeof(TCHAR));
	return szProgPath;
}

//------------------------------------------------------------------------

CString Utils::getRenLibDirectory()
{
	CString path(getRenLibPath());

	path.Replace("\\Release\\", "\\");
	path.Replace("\\Debug\\", "\\");

	int index = path.ReverseFind('\\') + 1;

	return path.Left(index);
}

//------------------------------------------------------------------------

CString Utils::getBitmapDirectory()
{
	CString path(getRenLibDirectory() + "BoardBitmap\\");

	return path;
}

//------------------------------------------------------------------------

CString Utils::getAppletDirectory()
{
	CString path(getRenLibDirectory() + "applet\\");

	return path;
}

//------------------------------------------------------------------------

std::vector<CString> Utils::getOpenFilter()
{
	std::vector<CString> filter;

	filter.push_back(Utils::GetString(IDS_FILTER_ALLFILES));
	filter.push_back(cOpenFilterAll);

	filter.push_back(Utils::GetString(IDS_FILTER_RENLIB_LIBRARY));
	filter.push_back("*.lib");

	filter.push_back(Utils::GetString(IDS_FILTER_TEXT_GAME));
	filter.push_back("*.txt");

	filter.push_back(Utils::GetString(IDS_FILTER_DATAFREE_BUF));
	filter.push_back("*.buf");

	filter.push_back(Utils::GetString(IDS_FILTER_GORENJER_PDB));
	filter.push_back("*.pdb");

	filter.push_back(Utils::GetString(IDS_FILTER_POS_FILE));
	filter.push_back("*.pos");

	filter.push_back(Utils::GetString(IDS_FILTER_BASECITY_GAME));
	filter.push_back("*.wzq");

	filter.push_back(Utils::GetString(IDS_FILTER_RENJS));
	filter.push_back("*.renjs;*.ren");

	filter.push_back(Utils::GetString(IDS_FILTER_SGF_GAME));
	filter.push_back("*.sgf");

	filter.push_back(Utils::GetString(IDS_FILTER_BDT_FILE));
	filter.push_back("*.bdt");

	filter.push_back(Utils::GetString(IDS_FILTER_RDF_FILE));
	filter.push_back("*.rdf");

    return filter;
}

//------------------------------------------------------------------------

void Utils::getSavePositionFilter(std::vector<CString>& filter, std::vector<FileType>& fileTypes)
{
	filter.push_back(Utils::GetString(IDS_FILTER_RENLIB_LIBRARY));
	filter.push_back("*.lib");
	fileTypes.push_back(RENLIB_LIBRARY);
	
	filter.push_back(Utils::GetString(IDS_FILTER_TEXT_GAME));
	filter.push_back("*.txt");
	fileTypes.push_back(TEXT_GAME);

	filter.push_back(Utils::GetString(IDS_FILTER_POS_FILE));
	filter.push_back("*.pos");
	fileTypes.push_back(POS);

	filter.push_back(Utils::GetString(IDS_FILTER_RENLIB_APPLET_FILE));
	filter.push_back("*.htm");
	fileTypes.push_back(RENLIB_APPLET);

	filter.push_back(Utils::GetString(IDS_FILTER_RENARTIST_APPLET_FILE));
	filter.push_back("*.htm");
	fileTypes.push_back(RENARTIST_APPLET);

	filter.push_back(Utils::GetString(IDS_FILTER_RENJUCLASS_DIAGRAM));
	filter.push_back("*.htm");
	fileTypes.push_back(RENJU_CLASS_DIAGRAM);

	filter.push_back(Utils::GetString(IDS_FILTER_BMP_FILE));
	filter.push_back("*.bmp");
	fileTypes.push_back(BMP);
}

//------------------------------------------------------------------------

void Utils::getSaveGameCollectionFilter(std::vector<CString>& filter, std::vector<FileType>& fileTypes)
{
	filter.push_back(Utils::GetString(IDS_FILTER_GAME_COLLECTION));
	filter.push_back("*.htm");
	fileTypes.push_back(GAME_COLLECTION);
}

//------------------------------------------------------------------------

void Utils::getSaveLibraryFilter(std::vector<CString>& filter, std::vector<FileType>& fileTypes)
{
	filter.push_back(Utils::GetString(IDS_FILTER_RENLIB_LIBRARY));
	filter.push_back("*.lib");
	fileTypes.push_back(RENLIB_LIBRARY);

	filter.push_back(Utils::GetString(IDS_FILTER_PDB_FILE));
	filter.push_back("*.pdb");
	fileTypes.push_back(PDB);

	filter.push_back(Utils::GetString(IDS_FILTER_BDT_FILE));
	filter.push_back("*.bdt");
	fileTypes.push_back(BDT);

//	filter.push_back(Utils::GetString(IDS_FILTER_DATAFREE_BUF));
//	filter.push_back("*.buf");
//	fileTypes.push_back(BUF);
}

//------------------------------------------------------------------------

CString Utils::makeFilter(const std::vector<CString>& filter)
{
	CString strFilter;

	for (int i=0; i+1 < filter.size(); i+=2)
	{
		strFilter += filter[i];
		strFilter += (TCHAR)NULL;
		strFilter += filter[i+1];
		strFilter += (TCHAR)NULL;
	}

	return strFilter;
}

//------------------------------------------------------------------------

CString Utils::SelectDirectory(const CString& strCaption)
{
    CFolderDialog dlg(strCaption, getCurrentDirectory());

	CString strFolderPath;

    if( dlg.DoModal() == IDOK  )
    {	
        strFolderPath = dlg.GetFolderPath();
		strFolderPath += "\\";
    }	

	return strFolderPath;
}

//------------------------------------------------------------------------

CString Utils::getFileName(const std::vector<CString>& filter, int& typeIndex, CString fileName)
{
	const int index = (typeIndex - 1) * 2 + 1;

	const CString strExtension(GetExtension(fileName));
	const int extensionIndex = findExtension(filter, strExtension);

	if (index < filter.size())
	{
		if (extensionIndex == -1)
		{
			setExtension(fileName, GetExtension(filter[index]));
		}
		else if (!IsExtension(filter[index], strExtension))
		{
			typeIndex = extensionIndex / 2 + 1;
		}
	}

	return fileName;
}

//------------------------------------------------------------------------

void Utils::setExtension(CString& strFile, const CString& strExt)
{
	if (strFile.Right(1) != ".")
	{
		strFile += ".";
	}

	strFile += strExt;
}

//------------------------------------------------------------------------

int Utils::findExtension(const std::vector<CString>& filter, const CString& strExt)
{
	int index = -1;

	for (int i=1; i < filter.size(); i+=2)
	{
		if (IsExtension(filter[i], strExt))
		{
			index = i;
			break;
		}
	}

	return index;
}

//------------------------------------------------------------------------

void Utils::SelectColor(COLORREF& color, COLORREF defaultColor, COLORREF* custColors)
{
	int i;

	CColorDialog dlg(color);

	dlg.m_cc.Flags |= CC_FULLOPEN;

	for (i=0; i<16; i++)
	{
		dlg.m_cc.lpCustColors[i] = custColors[i];
	}

	for (i=0; i<16; i++)
	{
		if (dlg.m_cc.lpCustColors[i] == cWhite ||
			dlg.m_cc.lpCustColors[i] == defaultColor)
		{
			dlg.m_cc.lpCustColors[i] = defaultColor;
			break;
		}
	}

	if (dlg.DoModal() == IDOK)
	{
		color = dlg.GetColor();

		for (i=0; i<16; i++)
		{
			custColors[i] = dlg.m_cc.lpCustColors[i];
		}
	}
}

//------------------------------------------------------------------------

CString Utils::ReadWebPage(const std::vector<CString>& webPages)
{
	CString        strAllPages;
	CAmHttpSocket  httpSocket;
	CStdioFile     file;
	CFileException e;

	for (int i=0; i<webPages.size(); i++)
	{
		CString strUrl(webPages[i]);

		CString strProtocol(strUrl.Left(4));
		strProtocol.MakeLower();

		if (strProtocol == "http")
		{
			CString strPage(httpSocket.GetPage(strUrl));
			strAllPages += strPage;
		}
		else if (file.Open(strUrl, CFile::modeRead, &e))
		{
			CString strLine;
			while (file.ReadString(strLine))
			{
				strAllPages += strLine;
			}
		}
	}

	return strAllPages;
}

//------------------------------------------------------------------------

CString Utils::ReadWebPage(const CString& webPage)
{
	std::vector<CString> webPages;
	webPages.push_back(webPage);
	return ReadWebPage(webPages);
}

//------------------------------------------------------------------------

bool Utils::StringToInt(const CString& strInt, long& intValue)
{
	intValue = -1;

	if (strInt.IsEmpty())
	{
		return false;
	}

	for (int i=0; i <strInt.GetLength(); i++)
	{
		if (!isdigit(strInt[i]))
		{
			return false;
		}
	}

	intValue = atol(strInt);
	return true;
}

//------------------------------------------------------------------------

