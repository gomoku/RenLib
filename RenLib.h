// RenLib.h : main header file for the RENLIB application
//

#if !defined(AFX_RENLIB_H__72EC9D45_F85E_11D3_92A3_947F7AC2F525__INCLUDED_)
#define AFX_RENLIB_H__72EC9D45_F85E_11D3_92A3_947F7AC2F525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CRenLibApp:
// See RenLib.cpp for the implementation of this class
//

class CRenLibApp : public CWinApp
{
public:
	CRenLibApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRenLibApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CRenLibApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RENLIB_H__72EC9D45_F85E_11D3_92A3_947F7AC2F525__INCLUDED_)
