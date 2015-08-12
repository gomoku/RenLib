// ColorEditClass.cpp : implementation file
//

#include "stdafx.h"
#include "GCColorEdit.h"
#include "ModeInformationI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGCColorEdit

CGCColorEdit::CGCColorEdit()
{
	m_CurrentBackColor = DEFAULT_BACKGROUND_COLOR;	
	m_CurrentTextColor = DEFAULT_TEXT_COLOR;
	m_BackBrush.CreateSolidBrush(m_CurrentBackColor);  
	m_bBold = FALSE;
	m_bItalic = FALSE;
	m_bUnderline = FALSE;
	m_bStatic = FALSE;
	m_bModal = FALSE;
	m_bFlat = FALSE;
	mModeInformation = 0;
}

CGCColorEdit::~CGCColorEdit()
{
	m_BackBrush.DeleteObject();
}

BEGIN_MESSAGE_MAP(CGCColorEdit, CEdit)
	//{{AFX_MSG_MAP(CGCColorEdit)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_NCPAINT()
	ON_WM_RBUTTONDOWN()
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_SETFOCUS()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGCColorEdit message handlers

void CGCColorEdit::SetModeInformation(ModeInformationI* modeInformation)
{
	mModeInformation = modeInformation;
}

COLORREF CGCColorEdit::SetBackgroundColor(const COLORREF bkColor)
{
	COLORREF prevColor = m_CurrentBackColor;	// Prepare to return previous color.

	m_CurrentBackColor = bkColor;				// Store new background color.

	m_BackBrush.DeleteObject();					// Delete previous background brush.
	m_BackBrush.CreateSolidBrush(bkColor);		// Create new background brush.

	this->Invalidate(TRUE);						// Force edit window to repaint.

	return (prevColor);							// Return previous background color.
}

COLORREF CGCColorEdit::SetTextColor(const COLORREF textColor)
{
	COLORREF prevColor = m_CurrentTextColor;	// Prepare to return previous text color.
	m_CurrentTextColor = textColor;				// Store new text color.

	this->Invalidate(TRUE);						// Force edit window to repaint.

	return prevColor;							// Return previous text color.
}

COLORREF CGCColorEdit::GetBackgroundColor() const
{
	return (m_CurrentBackColor);
}


COLORREF CGCColorEdit::GetTextColor() const
{
	return (m_CurrentTextColor);
}

void CGCColorEdit::GetBackgroundColorRGB(BYTE& Red, BYTE& Green, BYTE& Blue) const
{
	Red = GetRValue(m_CurrentBackColor);
	Green = GetGValue(m_CurrentBackColor);
	Blue = GetBValue(m_CurrentBackColor);
}

void CGCColorEdit::GetTextColorRGB(BYTE& Red, BYTE& Green, BYTE& Blue) const
{
	Red = GetRValue(m_CurrentTextColor);
	Green = GetGValue(m_CurrentTextColor);
	Blue = GetBValue(m_CurrentTextColor);
}

// ****************************************************************************
// ****************************************************************************
// CGCColorEdit::CtlColor()
// Handles the appropriate colors when the control is repainted on screen
// 
//									REMEMBER	
//
// If you handle WM_CTLCOLOR messages in your parent window (the window that
// contains this control) you must be sure to call the parent window's base 
// class OnCtlColor() handler if the parent's OnCtlColor() handler is being
// called for this control.  Otherwise, the OnCtlColor() handler in the 
// parent window will OVERRIDE the OnCtlColor() handler for this control.
//
// Windows is designed that way to give the parent a shot at handling the 
// message before reflecting it back to this control.
// ****************************************************************************

HBRUSH CGCColorEdit::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	pDC->SetTextColor(m_CurrentTextColor);
	pDC->SetBkMode(TRANSPARENT);
	return static_cast<HBRUSH>(m_BackBrush.GetSafeHandle());
}

BOOL CGCColorEdit::SetPointFont(const int PointSize, const LPCTSTR fontname)
{
	BOOL bResult = FALSE;

	// Get device context for this edit box.

	CDC* pDC = this->GetDC();

	// Delete previous CGdiObject and free its memory.

	m_Font.DeleteObject();		

	// Create a new Font object.

	bResult = m_Font.CreatePointFont(PointSize * 10,fontname,pDC);

	if (! bResult) return bResult;

	// Set the new font for this edit box.

	CEdit::SetFont(&m_Font,TRUE);

	// Release the device context.

	ReleaseDC(pDC);

	return bResult;
}

void CGCColorEdit::GetFontName(CString& fontname) const
{
	LOGFONT fontstruct;
	CFont* pFont = NULL;

	pFont = CEdit::GetFont();			// Get a pointer to the current font.

	pFont->GetLogFont(&fontstruct);		// Get the current logical font.

	fontname = fontstruct.lfFaceName;	// Return the face name of the current font.
}

BOOL CGCColorEdit::SetFontIndirect(const LOGFONT * lpLogFont)
{
	BOOL bResult = FALSE;

	// Delete previous CGdiObject and free its memory.
	m_Font.DeleteObject();		

	// Create a new Font object.
	bResult = m_Font.CreateFontIndirect(lpLogFont);

	if (! bResult) return bResult;

	// Set the new font for this edit box.
	CEdit::SetFont(&m_Font,TRUE);
	return bResult;
}

BOOL CGCColorEdit::SetPointFontIndirect(const LOGFONT * lpLogFont)
{
	BOOL bResult;

	// Delete previous CGdiObject and free its memory.

	m_Font.DeleteObject();	

	// Get device context for this edit box.
	CDC* pDC = this->GetDC();

	// Create a new Font object.
	bResult = m_Font.CreatePointFontIndirect(lpLogFont, pDC);

	if (! bResult)
	{
		// Release the device context.
		ReleaseDC(pDC);
		return bResult;
	}

	// Set the new font for this edit box.

	CEdit::SetFont(&m_Font,TRUE);

	// Release the device context.

	ReleaseDC(pDC);

	return bResult;
}

LOGFONT CGCColorEdit::GetLogFont() const
{
	LOGFONT fontstruct;
	CFont* pFont = NULL;

	pFont = this->GetFont();

	pFont->GetLogFont(&fontstruct);
	
	return fontstruct;		// Return the logical font structure.
}

BOOL CGCColorEdit::SetUnderline(const BOOL bOn)
{
	if (m_bUnderline == bOn) return m_bUnderline;

	BOOL prev = m_bUnderline;				// Prepare to return previous underline state.

	m_bUnderline = bOn;						// Set the new state.

	LOGFONT fontstruct;		

	fontstruct = this->GetLogFont();		// Get the current logical font so that
											// we will only change the underline state.

	fontstruct.lfUnderline = (BYTE) bOn;	// Set the new state.

	SetFontIndirect(&fontstruct);			// Set the font with the new underline state.

	return prev;							// Return previous underline state.
}

BOOL CGCColorEdit::SetItalic(const BOOL bOn)
{
	if (m_bItalic == bOn) return m_bItalic;

	BOOL prev = m_bItalic;					// Prepare to return previous italic state.

	m_bItalic = bOn;						// Store the new state.

	LOGFONT fontstruct;			

	fontstruct = this->GetLogFont();		// Get the current logical font.

	fontstruct.lfItalic = (BYTE) bOn;		// Set the new italic state.

	SetFontIndirect(&fontstruct);			// Set the font with the new italic state.

	return prev;							// Return previous italic state.
}

BOOL CGCColorEdit::SetBold(const BOOL bOn)
{
	if (m_bBold == bOn) return m_bBold;

	BOOL prev = m_bBold;					// Prepare to return previous bold state.

	m_bBold = bOn;							// Store the new state.

	LOGFONT fontstruct;

	fontstruct = this->GetLogFont();		// Get the current logical font.

	switch (bOn)
	{
		case TRUE:	
			fontstruct.lfWeight = 700;		// Standard weight for bold face.
			break;
		case FALSE:
			fontstruct.lfWeight = 400;		// Standard weight for normal text.
	}

	SetFontIndirect(&fontstruct);			// Set the font with new weight.
	
	return prev;							// Return previous bold state.
}

void CGCColorEdit::SetFontWeight(const int Weight)
{
	if ( (Weight < 1) || (Weight > 1000) ) return;

	LOGFONT fontstruct;

	fontstruct = this->GetLogFont();		// Get the current logical font.

	fontstruct.lfWeight = Weight;			// Set the font weight as specified.

	SetFontIndirect(&fontstruct);			// Set the font with the new weight.
}

BOOL CGCColorEdit::SetModalFrame(const BOOL bOn)
{
	if (m_bModal == bOn) return m_bModal;

	m_bModal = bOn;

	BOOL Result = FALSE;

	if (!bOn)
		Result = ModifyStyleEx(WS_EX_DLGMODALFRAME,0,SWP_DRAWFRAME);
	else
		Result = ModifyStyleEx(0,WS_EX_DLGMODALFRAME,SWP_DRAWFRAME);
	this->Invalidate();
	return Result;
}

BOOL CGCColorEdit::SetStaticFrame(const BOOL bOn)
{
	if (m_bStatic == bOn) return m_bStatic;

	BOOL Result = FALSE;

	m_bStatic = bOn;

	if (!bOn)
		Result = ModifyStyleEx(WS_EX_STATICEDGE,0,SWP_DRAWFRAME);
	else
		Result = ModifyStyleEx(0,WS_EX_STATICEDGE,SWP_DRAWFRAME);

	this->Invalidate();
	return Result;
}

COLORREF CGCColorEdit::SetBackgroundPattern(CBitmap* pattern)
{
	COLORREF prev = m_CurrentBackColor;
	m_CurrentBackColor = 0;
	m_BackBrush.DeleteObject();					// Delete previous brush.
	m_BackBrush.CreatePatternBrush(pattern);	// Create pattern brush.
	this->Invalidate();							// Force a redraw.

	return prev;
}

void CGCColorEdit::OnNcPaint() 
{
	if (! m_bFlat)
		CEdit::OnNcPaint();
}

BOOL CGCColorEdit::SetFlat(const BOOL mbFlat)
{
	BOOL Prev = m_bFlat;
	m_bFlat = mbFlat;

	CWnd* pParent = GetParent();
	pParent->Invalidate();
	return Prev;
}

void CGCColorEdit::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if (!isReadOnly())
	{
		CEdit::OnRButtonDown(nFlags, point);
		//	GetParent()->SendMessage(WM_RBUTTONDOWN); // Nope
	}
}

void CGCColorEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (!isReadOnly())
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
}

void CGCColorEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (!isReadOnly())
	{
		CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
		this->Invalidate();
	}
}

void CGCColorEdit::OnSetFocus(CWnd* pOldWnd) 
{
	if (!isReadOnly())
	{
		CEdit::OnSetFocus(pOldWnd);
		this->SetSel(-1, -1);
		//	GetParent()->SetFocus(); // *** Stack Overflow ***
	}
}

void CGCColorEdit::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CEdit::OnVScroll(nSBCode, nPos, pScrollBar);
	this->Invalidate();	
}

BOOL CGCColorEdit::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{	BOOL retValue = CEdit::OnMouseWheel(nFlags, zDelta, pt);
	this->Invalidate();	
	return retValue;
}

bool CGCColorEdit::isReadOnly()
{
	return (mModeInformation != 0) && mModeInformation->isReadOnly();
}
