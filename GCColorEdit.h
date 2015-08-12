#if !defined(AFX_COLOREDITCLASS_H__61C65D1F_8D63_11D3_A76F_00105AA7390A__INCLUDED_)
#define AFX_COLOREDITCLASS_H__61C65D1F_8D63_11D3_A76F_00105AA7390A__INCLUDED_

// ************************************************************************
// ************************************************************************
//                             CGCColorEdit
//
//                           Color Edit Control
//
//                     Version 1.2      Date: 04/19/00
//
//                Developed By:  George Chastain
//                               Software Consultant
//                               e-mail: gcoutpost@hotmail.com
//
//                             
//
// DESCRIPTION:
// Use this class to create a member variable for a CEdit control to
// allow you to dynamically change the background and text colors for the
// CEdit box.  This class also provides the ability to set the background
// of the edit box to a specified pattern.
//
// Refer to the comments associated with the class methods below.
//
//
// Revision History:
//
// VERSION		DEVELOPER		CHANGE
// 1.0			Chastain		Created
// 1.1			Chastain		Removed some old obsolete code from 
//								SetBackgroundColor() and SetTextColor() 
//								that should have been deleted from
//								Added more comments.
//
// 1.2			Chastain		Added Flat style. (SetFlat() method).
//
// 1.3          Frank Arkbo     Changes for RenLib
//
// ************************************************************************
// ************************************************************************



#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ColorEditClass.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGCColorEdit window

class ModeInformationI;


class CGCColorEdit : public CEdit
{
public:
	CGCColorEdit();

	// Set mode information interface
	void SetModeInformation(ModeInformationI* modeInformation);


	// SetTextColor() -- Sets new text color.  Returns previous text color.
	//
	COLORREF SetTextColor(const COLORREF textColor);		


	// SetBackgroundColor() -- Sets new background color. Returns previous background color.
	// If the previous background was a pattern, then the returned color will be black (0).
	//
	COLORREF SetBackgroundColor(const COLORREF bkColor);	 

	// SetBackgroundPattern() -- Sets the background of the edit box to the specified
	// pattern.  The pattern must be a bitmap of 8 pixels high by 8 pixels wide.
	// If the previous background was a pattern, then the returned color will be black (0).
	//
	COLORREF SetBackgroundPattern(CBitmap* pattern);


	// GetTextColor() -- Returns current text color.
	//
	COLORREF GetTextColor() const;						

	// GetTextColorRGB() -- Returns the current text color as separate RGB values.
	//
	void GetTextColorRGB(BYTE& Red, BYTE& Green, BYTE& Blue) const;

	// GetBackgroundColor() -- Returns current background color.
	// If the current background is a pattern, then the returned color will be black (0).
	//
	COLORREF GetBackgroundColor() const;					

	// GetBackgroundColorRGB() -- Returns current background color as separate RGB values.
	// If the current background is a pattern, then the returned color will be black (0,0,0).
	//
	void GetBackgroundColorRGB(BYTE& Red, BYTE& Green, BYTE& Blue) const;

	// SetStaticFrame() -- Changes the edit box to have a static frame style.
	// Extended Style WS_EX_STATICEDGE.
	//
	BOOL SetStaticFrame(const BOOL bOn);

	// SetModalFrame() -- Changes the edit box to have a modal frame style.
	// Extended type WS_EX_DLGMODALFRAME.  Modal style overrides static edge style.
	//
	BOOL SetModalFrame(const BOOL bOn);


	// SetPointFont() sets the edit control to use the specified font at the specified
	// point size. Examples of how to specify font name: "Comic Sans MS" or "Haettenschweiler".
	// It is up to the developer to create the edit control large-enough to display
	// the largest font required.
	//
	BOOL SetPointFont(const int PointSize, const LPCTSTR fontname);

	// SetFontIndirect() sets the edit control to use the specified font.
	// For a description of how to use the input parameter, refer to the MFC documentation
	// for CFont::CreateFontIndirect();
	//
	BOOL SetFontIndirect(const LOGFONT* lpLogFont);

	// SetPointFontIndirect() sets the edit control to use the specified font.
	// For a description of how to use the input parameter, refer to the MFC documentation
	// for CFont::CreatePointFontIndirect();
	//
	BOOL SetPointFontIndirect(const LOGFONT* lpLogFont);

	// SetBold() -- Allows you to change the font weight between 400 (normal) and 800 (bold).
	// Returns the previous state of the bold attribute.
	//
	BOOL SetBold(const BOOL bOn = TRUE);

	// SetFontWeight() -- Provides more control over the font weight that SetBold().
	// The effect dependes on the font in use.
	//
	void SetFontWeight(const int Weight);

	// SetItalic() -- Sets the text in the edit control to italics or turns it off.
	// Returns the previous state of the italic attribute.
	//
	BOOL SetItalic(const BOOL bOn = TRUE);

	// SetUnderline() -- Sets/removes underlining from the text.
	// Returns the previous state of the underline attribute.
	//
	BOOL SetUnderline(const BOOL bOn = TRUE);

	// GetLogFont() -- returns the logical font in use in a LOGFONT structure.
	//
	LOGFONT GetLogFont() const;

	BOOL SetFlat(const BOOL bFlat);

	// GetFontName() retrieves the current font name.
	void GetFontName(CString& fontname) const;

	virtual ~CGCColorEdit();


	// By defining constants such as these in this manner instead of using
	// #defines outside the class definition, the names will not conflict
	// with other #defines in the clients code.  A client can still reference
	// these if needed as in this example: CGCColorEdit::DEFAULT_BACKGROUND_COLOR.

	enum {	DEFAULT_BACKGROUND_COLOR = RGB(255,255,255),
			DEFAULT_TEXT_COLOR       = RGB(0,0,0)};

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGCColorEdit)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CGCColorEdit)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnNcPaint();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	bool isReadOnly();

private:
	BOOL m_bFlat;
	BOOL m_bModal;					// Indicates if modal frame is set.
	BOOL m_bStatic;					// Indicates if static frame is set. Modal frame overrides.
	BOOL m_bBold;					// Indicates bold face.
	BOOL m_bItalic;					// Indicates Italic.
	BOOL m_bUnderline;				// Indicates underline.
	CFont m_Font;					// Current client-created font object.

	COLORREF m_CurrentBackColor;	// Current background color.
	COLORREF m_CurrentTextColor;	// Current text color.
	CBrush m_BackBrush;				// Current background brush.

	ModeInformationI* mModeInformation; // pointer to ModeInformation
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLOREDITCLASS_H__61C65D1F_8D63_11D3_A76F_00105AA7390A__INCLUDED_)
