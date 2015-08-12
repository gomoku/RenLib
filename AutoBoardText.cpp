// AutoBoardText.cpp : implementation file
//

#include "stdafx.h"
#include "renlib.h"
#include "AutoBoardText.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// AutoBoardText dialog


AutoBoardText::AutoBoardText(CWnd* pParent /*=NULL*/)
	: CDialog(AutoBoardText::IDD, pParent)
{
	//{{AFX_DATA_INIT(AutoBoardText)
	m_NextAutoBoardText = _T("");
	//}}AFX_DATA_INIT
}


void AutoBoardText::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(AutoBoardText)
	DDX_Text(pDX, IDC_BOARD_TEXT, m_NextAutoBoardText);
	DDV_MaxChars(pDX, m_NextAutoBoardText, 2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(AutoBoardText, CDialog)
	//{{AFX_MSG_MAP(AutoBoardText)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// AutoBoardText message handlers

int AutoBoardText::DoModal() 
{
   CDialogTemplate dlt;
   int             nResult;

   // load dialog template
   if (!dlt.Load(MAKEINTRESOURCE(AutoBoardText::IDD))) return -1;

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
