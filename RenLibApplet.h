// RenLibApplet.h: interface for the RenLibApplet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RENLIB_APPLET_H__BB1690C8_CF99_48C1_B318_9A98599582C2__INCLUDED_)
#define AFX_RENLIB_APPLET_H__BB1690C8_CF99_48C1_B318_9A98599582C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>

class MoveList;
class MoveNode;

class RenLibApplet
{
public:
	enum Size
	{
		BIG,
		SMALL
	};

	enum Url
	{
		WEB,
		LOCAL
	};

    static const CString cFirstComment;
    static const CString cLastComment;

	class Information
	{
	public:
        Information();

		Information(
			Size     size,
			CString  titleAppletWebPage,
			CString  titleGameWebPage,
			BOOL     showOneLineComment,
			BOOL     showMultiLineComment,
			BOOL     showBoardText,
			BOOL     showCoordinates,
			BOOL     showNumbers,
			BOOL     showCommands,
            BOOL     showBoard,
            Url      url,
			CString  repeatOneLineComment,
            COLORREF backgroundColor);

	public:
		Size     mSize;
		CString  mTitleAppletWebPage;
		CString  mTitleGameWebPage;
		BOOL     mShowOneLineComment;
		BOOL     mShowMultiLineComment;
		BOOL     mShowBoardText;
		BOOL     mShowCoordinates;
		BOOL     mShowNumbers;
		BOOL     mShowCommands;
		BOOL     mShowBoard;
        Url      mUrl;
        CString  mRepeatOneLineComment;
		COLORREF mBackgroundColor;
	};

public:
	RenLibApplet(Information info);

	virtual ~RenLibApplet();

	bool getAppletInformation(
		const CString& strDirectory, Information& info, COLORREF* custColors);

	bool Create(
		const CString& fileName, const MoveList& aMoveList,
		CString& strComment, const Information& info);

	bool CreateExportGameFile(
		const CString& strDirectory, CStringList& strList, const CString& titleGameWebPage,
		const CString& strCaption, CString& strFile);

	bool CreateTournamentGameFile(
		const CString& strDirectory, CStringList& strList, const CString& titleGameWebPage,
		const CString& strCaption, CString& strFile);

	bool CreateGameCollectionFile(
		const CString& titleGameWebPage, const CString& strCaption, CString& strFile);

	bool AddGameCollectionFile(
		const MoveList& aMoveList, CString& strOneLineComment, CString& strMultiLineComment,
		const Information& info);

	bool CloseGameCollectionFile();

private:
    RenLibApplet();

	void generateAppletCode(
		CString& strAppletCode, const MoveList& aMoveList, CString& strComment,
		const Information& info);

private:
    typedef struct GameData
    {
        CString   applet;
        CString   oneLineComment;
        CString   multiLineComment;
    };

    CStdioFile            mFile;
	std::vector<GameData> mSortedList;

    CString mAppletUrl;
    CString mBlackWinImage;
    CString mWhiteWinImage;
    CString mDrawImage;

	void WriteLine(const CString& line);
};

#endif // !defined(AFX_RENLIB_APPLET_H__BB1690C8_CF99_48C1_B318_9A98599582C2__INCLUDED_)
