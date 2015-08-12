// CISBitmap.h: interface for the CCISBitmap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CISBITMAP_H__08BA6EB3_DB4C_11D1_8A89_0040052E2D91__INCLUDED_)
#define AFX_CISBITMAP_H__08BA6EB3_DB4C_11D1_8A89_0040052E2D91__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CCISBitmap : public CBitmap  
{
public:
	CCISBitmap();
	virtual ~CCISBitmap();

	// Functions
	int Height();
	int Width();	
	virtual void DrawTransparent(CDC* pDC, int x, int y, COLORREF crColour);	
	
private:
	COLORREF	m_crBlack;
	COLORREF	m_crWhite;
};

#endif // !defined(AFX_CISBITMAP_H__08BA6EB3_DB4C_11D1_8A89_0040052E2D91__INCLUDED_)
