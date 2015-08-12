// Utils.h: interface for the Utils class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UTILS_H__7B941A41_0BA5_11D4_92A3_CEA74A1A6D25__INCLUDED_)
#define AFX_UTILS_H__7B941A41_0BA5_11D4_92A3_CEA74A1A6D25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>

class Utils
{
public:
	enum FileType
	{
		NONE,
		RENLIB_LIBRARY,
		TEXT_GAME,
		POS,
		PDB,
        BDT,
        BUF,
		RENLIB_APPLET,
		RENARTIST_APPLET,
		RENJU_CLASS_DIAGRAM,
		BMP,
		GAME_COLLECTION
	};

public:
	Utils();
	virtual ~Utils();

	static int	msb (BYTE ch);
	static bool	bit_is_one (int bit_value, UINT value);
	static void	clear_bit (int bit_value, UINT& value);
	static void	set_bit (int bit_value, UINT& value);

	static bool exists (const CString& strFile);
	static void remove (const CString& strFile);
	static bool rename (const CString& strOldFileName, CString& strNewFileName);

	static void AsciiToWinChar (CString& strComment);
	static void WinCharToAscii (CString& strComment);

	static CString Center(CString strComment, int nChar);

	static bool IsSubstring(CString strString, CString strCheck);

	static bool OpenFileDialog(CString& strFile);
	static bool SavePositionDialog(CString& strFile, FileType& fileType);
	static bool SaveGameCollectionDialog(CString& strFile, FileType& fileType);
	static bool SaveLibraryDialog(CString& strFile, FileType& fileType);

	static CString GetExtension(const CString& strFile);
	static CString GetTitle(const CString& strFile);

	static bool IsExtensionLib(const CString& strFile);
	static bool IsExtensionTxt(const CString& strFile);
	static bool IsExtensionWzq(const CString& strFile);
	static bool IsExtensionBuf(const CString& strFile);
    static bool IsExtensionPdb(const CString& strFile);
    static bool IsExtensionPos(const CString& strFile);
	static bool IsExtensionRenjs(const CString& strFile);
	static bool IsExtensionHtml(const CString& strFile);
	static bool IsExtensionSgf(const CString& strFile);
	static bool IsExtensionBdt(const CString& strFile);
	static bool IsExtensionRdf(const CString& strFile);

    static bool IsExtensionAccepted(const CString& strFile);

	static void MakeExtensionLib(CString& strFile);
	static void MakeExtensionBak(CString& strFile);

	static bool OpenMultiFileDialog(CStringList& fileNameList);
	
	static bool OpenBmpDialog(CString& strFile, const CString& strDirectory);

	static bool StringToInteger(const CString& strInt, int& integer);

	static void Beep();

	static CString GetString(UINT nIDPrompt);
	static CString GetString(UINT nIDPrompt, int nParam);
	static CString GetString(UINT nIDPrompt, int nParam1, int nParam2);
	static CString GetString(UINT nIDPrompt, const CString& strParam);
	static CString GetString(UINT nIDPrompt, int nParam, const CString& strParam);
	static CString GetString(UINT nIDPrompt, CPoint pntParam);
	static CString GetString(UINT nIDPrompt, const CString& strParam, int nParam1, int nParam2);
	static int ShowMessage(const CString& strMessage, const CString& strCaption, UINT nType);

	static CString XCoordinateImage(int x, bool AO, bool Falling);
	static CString YCoordinateImage(int y, bool Falling);

	static bool LessThan(CPoint Left, CPoint Right);

	static CPoint PosToPoint(int pos);
	static BYTE   PointToPos(CPoint Point);
	static bool   isValidPoint(CPoint Point);
	static bool   isLessThan(CPoint Point1, CPoint Point2);

	static void Increment(CString& strText);

	static void CopyToClipboard(const CString& strText);
	static void PasteFromClipboard(CString& strText);

	static void trim(CString& strText);

	static void associateLib();
	static void associateWzq();
	static void associateBuf();
    static void associatePdb();
    static void associatePos();
    static void associateRenjs();
	static void associateSgf();
	static void associateBdt();
	static void associateRdf();

	static CString getCurrentDirectory();
	static CString getRenLibPath();
	static CString getRenLibDirectory();
	static CString getBitmapDirectory();
	static CString getAppletDirectory();

	static CString SelectDirectory(const CString& strCaption);

	static void SelectColor(COLORREF& color, COLORREF defaultColor, COLORREF* custColors);

	static CString ReadWebPage(const std::vector<CString>& webPages);
	static CString ReadWebPage(const CString& webPage);

	static bool StringToInt(const CString& strInt, long& intValue);
	static bool HexToInt(const TCHAR& hex, int& intValue);
	static bool IntToHex(const int intValue, TCHAR& hex);

private:
	static bool OpenSaveDialog(CString& strFile, const std::vector<CString>& filter, const CString& strInitialDirectory);
	static bool IsExtension(const CString& strFile, const CString& strExt);
	static void MakeExtension(CString& strFile, const CString& strExt);
	static int isNumber(const CString& strNumber);
	static void Increment(TCHAR& ch0, TCHAR& ch1);
	static void associateFile(const CString& strExt);
	static std::vector<CString> getOpenFilter();
	static void getSavePositionFilter(std::vector<CString>& filter, std::vector<FileType>& fileTypes);
	static void getSaveGameCollectionFilter(std::vector<CString>& filter, std::vector<FileType>& fileTypes);
	static void getSaveLibraryFilter(std::vector<CString>& filter, std::vector<FileType>& fileTypes);
	static CString makeFilter(const std::vector<CString>& filter);
	static CString getFileName(const std::vector<CString>& filter, int& typeIndex, CString fileName);
	static void setExtension(CString& strFile, const CString& strExt);
	static int findExtension(const std::vector<CString>& filter, const CString& strExt);
	static bool SaveDialog(CString& strFile, FileType& fileType, std::vector<CString>& filter, std::vector<FileType>& fileTypes);
};

#endif // !defined(AFX_UTILS_H__7B941A41_0BA5_11D4_92A3_CEA74A1A6D25__INCLUDED_)
