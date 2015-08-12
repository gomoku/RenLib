#if !defined(AFX_RENLIB_APPLETDIALOG_H__FE6B22CA_64EF_4EEC_81C7_3C8FDF8B656D__INCLUDED_)
#define AFX_RENLIB_APPLETDIALOG_H__FE6B22CA_64EF_4EEC_81C7_3C8FDF8B656D__INCLUDED_

#include "RenLibApplet.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RenLibAppletDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// RenLibAppletDialog dialog

class RenLibAppletDialog : public CDialog
{
// Construction
public:
	RenLibAppletDialog(
		bool editTitleAppletWebPage, bool editTitleGameWebPage, COLORREF* custColors);

	RenLibAppletDialog(
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
        COLORREF* custColors);

    void setInfo(const RenLibApplet::Information& info);
    void getInfo(RenLibApplet::Information& info);

	void setOneLineComments(std::vector<CString>& comments);

private:
    void initData();

// Dialog Data
	//{{AFX_DATA(RenLibAppletDialog)
	enum { IDD = IDD_RENLIB_APPLET };
	CString	mTitleAppletWebPage;
	CString	mTitleGameWebPage;
	int		mSize;
	BOOL	mShowOneLineComment;
	BOOL	mShowMultiLineComment;
	BOOL	mShowBoardText;
	BOOL	mShowCoordinates;
	BOOL	mShowNumbers;
	BOOL	mShowCommands;
	BOOL	mShowBoard;
	int		mUrl;
	CString mRepeatOneLineComment;
	//}}AFX_DATA
    COLORREF mBackgroundColor;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(RenLibAppletDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(RenLibAppletDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnChooseBackgroundColor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	bool      mEditTitleAppletWebPage;
	bool      mEditTitleGameWebPage;
	bool      mEditSize;
	bool      mEditShowOneLineComment;
	bool      mEditShowMultiLineComment;
	bool      mEditShowBoardText;
	bool      mEditShowCoordinates;
	bool      mEditShowNumbers;
	bool      mEditShowCommands;
	bool      mEditUrl;
	bool      mEditShowBoard;
	bool      mEditBackgroundColor;
	bool      mEditRepeatOneLineComment;

    std::vector<CString> mOneLineComments;

    COLORREF* mCustColors;
	CRect     mColorRect;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RENLIB_APPLETDIALOG_H__FE6B22CA_64EF_4EEC_81C7_3C8FDF8B656D__INCLUDED_)
