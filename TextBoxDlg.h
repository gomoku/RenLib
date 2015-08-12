#if !defined(AFX_TEXTBOXDLG_H__8EE17462_EE76_11D6_92A3_0000E89F396C__INCLUDED_)
#define AFX_TEXTBOXDLG_H__8EE17462_EE76_11D6_92A3_0000E89F396C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextBoxDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// TextBoxDlg dialog

class TextBoxDlg : public CDialog
{
// Construction
public:
	TextBoxDlg(const CString& title,
			   const CString& message,
			   const CString& messageList);

// Dialog Data
	//{{AFX_DATA(TextBoxDlg)
	enum { IDD = IDD_TEXT_BOX_DIALOG };
	CStatic	mMsg;
	CStatic	mIcon;
	CEdit	mListMsg;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TextBoxDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(TextBoxDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CString mTitle;
	CString mMessage;
	CString mMessageList;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTBOXDLG_H__8EE17462_EE76_11D6_92A3_0000E89F396C__INCLUDED_)
