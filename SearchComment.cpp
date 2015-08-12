// SearchComment.cpp : implementation file
//

#include "stdafx.h"
#include "RenLib.h"
#include "SearchComment.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SearchComment dialog


SearchComment::SearchComment(CWnd* pParent /*=NULL*/)
	: CDialog(SearchComment::IDD, pParent)
{
	//{{AFX_DATA_INIT(SearchComment)
	m_strComment = _T("");
	mFindOption = 0;
	//}}AFX_DATA_INIT
}


void SearchComment::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SearchComment)
	DDX_Text    (pDX, IDC_ONE_LINE_COMMENT, m_strComment);
	DDV_MaxChars(pDX, m_strComment, 55);
	DDX_Radio   (pDX, IDC_RADIO_COMMENT,    mFindOption);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(SearchComment, CDialog)
	//{{AFX_MSG_MAP(SearchComment)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SearchComment message handlers

int SearchComment::DoModal() 
{
   CDialogTemplate dlt;
   int             nResult;

   // load dialog template
   if (!dlt.Load(MAKEINTRESOURCE(SearchComment::IDD))) return -1;

   // set your own font, for example "Arial", 10 pts. 
   dlt.SetFont(m_strFaceName, 10);
	
   // get pointer to the modified dialog template
   LPSTR pdata = (LPSTR)GlobalLock(dlt.m_hTemplate);
	
   // let MFC know that you are using your own template
   m_lpszTemplateName = NULL;
   m_hDialogTemplate  = NULL;

   InitModalIndirect(pdata);

   // display dialog box
   nResult = CDialog::DoModal();

   // unlock memory object
   GlobalUnlock(dlt.m_hTemplate);

   return nResult;	
}
