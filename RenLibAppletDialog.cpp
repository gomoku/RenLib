// RenLibAppletDialog.cpp : implementation file
//

#include "stdafx.h"
#include "renlib.h"
#include "RenLibAppletDialog.h"
#include "Utils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace
{
	const COLORREF cGray = RGB(225, 225, 225);
}

/////////////////////////////////////////////////////////////////////////////
// RenLibAppletDialog dialog

RenLibAppletDialog::RenLibAppletDialog(
    bool editTitleAppletWebPage, bool editTitleGameWebPage, COLORREF* custColors)
: CDialog(RenLibAppletDialog::IDD, NULL),
  mEditTitleAppletWebPage(editTitleAppletWebPage),
  mEditTitleGameWebPage(editTitleGameWebPage),
  mEditSize(true),
  mEditShowOneLineComment(true),
  mEditShowMultiLineComment(true),
  mEditShowBoardText(true),
  mEditShowCoordinates(true),
  mEditShowNumbers(true),
  mEditShowCommands(true),
  mEditUrl(true),
  mEditShowBoard(true),
  mEditBackgroundColor(true),
  mEditRepeatOneLineComment(true),
  mCustColors(custColors)
{
    initData();
}

RenLibAppletDialog::RenLibAppletDialog(
    bool editTitleAppletWebPage,
    bool editTitleGameWebPage,
    bool editSize,
    bool editShowOneLineComment,
    bool editShowMultiLineComment,
    bool editShowBoardText,
    bool editShowCoordinates,
    bool editShowNumbers,
    bool editShowCommands,
    bool editUrl,
    bool editShowBoard,
    bool editBackgroundColor,
    bool editRepeatOneLineComment,
    COLORREF* custColors)
: CDialog(RenLibAppletDialog::IDD, NULL),
  mEditTitleAppletWebPage(editTitleAppletWebPage),
  mEditTitleGameWebPage(editTitleGameWebPage),
  mEditSize(editSize),
  mEditShowOneLineComment(editShowOneLineComment),
  mEditShowMultiLineComment(editShowMultiLineComment),
  mEditShowBoardText(editShowBoardText),
  mEditShowCoordinates(editShowCoordinates),
  mEditShowNumbers(editShowNumbers),
  mEditShowCommands(editShowCommands),
  mEditUrl(editUrl),
  mEditShowBoard(editShowBoard),
  mEditBackgroundColor(editBackgroundColor),
  mEditRepeatOneLineComment(editRepeatOneLineComment),
  mCustColors(custColors)
{
    initData();
}

void RenLibAppletDialog::initData()
{
	//{{AFX_DATA_INIT(RenLibAppletDialog)
	mTitleAppletWebPage   = "The XXX tournament in Renju.&nbsp;&nbsp;&nbsp;&nbsp;Date, City, Country";
	mTitleGameWebPage     = "Group/Tournament";
	mSize                 = RenLibApplet::BIG;
	mShowOneLineComment   = TRUE;
	mShowMultiLineComment = TRUE;
	mShowBoardText        = TRUE;
	mShowCoordinates      = TRUE;
	mShowNumbers          = TRUE;
	mShowCommands         = TRUE;
    mUrl                  = RenLibApplet::WEB;
    mShowBoard            = TRUE;
	//}}AFX_DATA_INIT
	mBackgroundColor      = RGB(225, 225, 225);
}

void RenLibAppletDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RenLibAppletDialog)
	DDX_Text (pDX, IDC_TITLE_APPLET_WEB_PAGE,   mTitleAppletWebPage);
	DDX_Text (pDX, IDC_TITLE_GAME_WEB_PAGE,     mTitleGameWebPage);
	DDX_Radio(pDX, IDC_RADIO_SIZE_BIG,          mSize);
	DDX_Check(pDX, IDC_ONE_LINE_COMMENT,        mShowOneLineComment);
	DDX_Check(pDX, IDC_MULTI_LINE_COMMENT,      mShowMultiLineComment);
	DDX_Check(pDX, IDC_BOARD_TEXT,              mShowBoardText);
	DDX_Check(pDX, IDC_SHOW_COORDINATES,        mShowCoordinates);
	DDX_Check(pDX, IDC_SHOW_NUMBERS,            mShowNumbers);
	DDX_Check(pDX, IDC_SHOW_COMMANDS,           mShowCommands);
	DDX_Check(pDX, IDC_SHOW_BOARD,              mShowBoard);
	DDX_Radio(pDX, IDC_RADIO_URL_WEB,           mUrl);
	DDX_Text (pDX, IDC_REPEAT_ONE_LINE_COMMENT, mRepeatOneLineComment);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(RenLibAppletDialog, CDialog)
	//{{AFX_MSG_MAP(RenLibAppletDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHOOSE_BACKGROUND_COLOR, OnChooseBackgroundColor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// RenLibAppletDialog message handlers

BOOL RenLibAppletDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (!mEditTitleAppletWebPage)
	{
		GetDlgItem(IDC_TITLE_APPLET_WEB_PAGE)->EnableWindow(FALSE);
		mTitleAppletWebPage   = "";
	}
	
	if (!mEditTitleGameWebPage)
	{
		GetDlgItem(IDC_TITLE_GAME_WEB_PAGE)->EnableWindow(FALSE);
		mTitleGameWebPage   = "";
	}

	if (!mEditSize)
	{
		GetDlgItem(IDC_RADIO_SIZE_BIG)->EnableWindow(FALSE);
	}

	if (!mEditShowOneLineComment)
	{
		GetDlgItem(IDC_ONE_LINE_COMMENT)->EnableWindow(FALSE);
	}

	if (!mEditShowMultiLineComment)
	{
		GetDlgItem(IDC_MULTI_LINE_COMMENT)->EnableWindow(FALSE);
	}

	if (!mEditShowBoardText)
	{
		GetDlgItem(IDC_BOARD_TEXT)->EnableWindow(FALSE);
	}

	if (!mEditShowCoordinates)
	{
		GetDlgItem(IDC_SHOW_COORDINATES)->EnableWindow(FALSE);
	}

	if (!mEditShowNumbers)
	{
		GetDlgItem(IDC_SHOW_NUMBERS)->EnableWindow(FALSE);
	}

	if (!mEditShowCommands)
	{
		GetDlgItem(IDC_SHOW_COMMANDS)->EnableWindow(FALSE);
	}

	if (!mEditUrl)
	{
		GetDlgItem(IDC_RADIO_URL_WEB)->EnableWindow(FALSE);
	}

	if (!mEditShowBoard)
	{
		GetDlgItem(IDC_SHOW_BOARD)->EnableWindow(FALSE);
	}

	if (!mEditBackgroundColor)
	{
		GetDlgItem(IDC_CHOOSE_BACKGROUND_COLOR)->EnableWindow(FALSE);
	}

	if (!mEditRepeatOneLineComment)
	{
        GetDlgItem(IDC_REPEAT_ONE_LINE_COMMENT)->EnableWindow(FALSE);
	}

    CComboBox* comboBox = (CComboBox*)GetDlgItem(IDC_REPEAT_ONE_LINE_COMMENT);

    comboBox->AddString(RenLibApplet::cFirstComment);
    comboBox->AddString(RenLibApplet::cLastComment);

    for (int i=0; i < mOneLineComments.size(); i++)
    {
        comboBox->AddString(mOneLineComments[i]);
    }

    // Transfer data from variables to controls
	UpdateData(FALSE);

	return TRUE;
}

void RenLibAppletDialog::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	GetDlgItem(IDC_BACKGROUND_COLOR)->GetWindowRect(mColorRect);
	ScreenToClient(&mColorRect);

	CBrush backgroundBrush(mBackgroundColor);
	dc.SelectObject(&backgroundBrush);

	dc.Rectangle(&mColorRect);
	dc.SelectStockObject(BLACK_BRUSH);
}

void RenLibAppletDialog::OnChooseBackgroundColor() 
{
	Utils::SelectColor(mBackgroundColor, cGray, mCustColors);
	InvalidateRect(mColorRect);
}

void RenLibAppletDialog::setInfo(const RenLibApplet::Information& info)
{
    mTitleAppletWebPage   = info.mTitleAppletWebPage;
    mTitleGameWebPage     = info.mTitleGameWebPage;
    mSize                 = info.mSize;
    mShowOneLineComment   = info.mShowOneLineComment;
    mShowMultiLineComment = info.mShowMultiLineComment;
    mShowBoardText        = info.mShowBoardText;
    mShowCoordinates      = info.mShowCoordinates;
    mShowNumbers          = info.mShowNumbers;
    mShowCommands         = info.mShowCommands;
    mUrl                  = info.mUrl;
    mShowBoard            = info.mShowBoard;
    mBackgroundColor      = info.mBackgroundColor;
    mRepeatOneLineComment = info.mRepeatOneLineComment;
}

void RenLibAppletDialog::getInfo(RenLibApplet::Information& info)
{
    info.mTitleAppletWebPage   = mTitleAppletWebPage;
    info.mTitleGameWebPage     = mTitleGameWebPage;
    info.mSize                 = (RenLibApplet::Size)mSize;
    info.mShowOneLineComment   = mShowOneLineComment;
    info.mShowMultiLineComment = mShowMultiLineComment;
    info.mShowBoardText        = mShowBoardText;
    info.mShowCoordinates      = mShowCoordinates;
    info.mShowNumbers          = mShowNumbers;
    info.mShowCommands         = mShowCommands;    
    info.mUrl                  = (RenLibApplet::Url)mUrl;    
    info.mShowBoard            = mShowBoard;
    info.mBackgroundColor      = mBackgroundColor;
    info.mRepeatOneLineComment = mRepeatOneLineComment;

    Utils::trim(info.mTitleAppletWebPage);
    Utils::trim(info.mTitleGameWebPage);
    Utils::trim(info.mRepeatOneLineComment);
}

void RenLibAppletDialog::setOneLineComments(std::vector<CString>& comments)
{
    mOneLineComments = comments;
}
