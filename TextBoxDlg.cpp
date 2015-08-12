// TextBoxDlg.cpp : implementation file
//

#include "stdafx.h"
#include "renlib.h"
#include "TextBoxDlg.h"
#include "Utils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TextBoxDlg dialog


TextBoxDlg::TextBoxDlg(const CString& title,
					   const CString& message,
					   const CString& messageList)
: CDialog(TextBoxDlg::IDD, NULL),
  mTitle(title),
  mMessage(message),
  mMessageList(messageList)
{
	//{{AFX_DATA_INIT(TextBoxDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void TextBoxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(TextBoxDlg)
	DDX_Control(pDX, IDC_TEXT_BOX_MSG,      mMsg);
	DDX_Control(pDX, IDC_TEXT_BOX_ICON,     mIcon);
	DDX_Control(pDX, IDC_TEXT_BOX_LIST_MSG, mListMsg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(TextBoxDlg, CDialog)
	//{{AFX_MSG_MAP(TextBoxDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TextBoxDlg message handlers

BOOL TextBoxDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetWindowText(mTitle);
	mMsg.SetWindowText(mMessage);
	mListMsg.SetWindowText(mMessageList);

    mIcon.SetIcon(AfxGetApp()->LoadStandardIcon(IDI_EXCLAMATION));

	// Transfer data from variables to controls
	UpdateData(FALSE);

	Utils::Beep();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
