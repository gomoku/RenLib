// Font.h: interface for the Font class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FONT_H__FC3BD1E2_A7E0_11D6_92A3_0000E89F396C__INCLUDED_)
#define AFX_FONT_H__FC3BD1E2_A7E0_11D6_92A3_0000E89F396C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Font  
{
public:
	Font();
	virtual ~Font();

	void    setDefault(int boardSize);

	void    getLogFont(LOGFONT& lf);

	int     getPointSize();
	void    setPointSize(int pointSize);

    bool    isBold();
    void    setBold(bool bold);
    
	bool    isItalic();
	void    setItalic(bool italic);
    
	CString getFaceName();
	void    setFaceName(const CString& faceName);

	CString toString();
	void    parse(CString fontString, int boardSize);

private:
	int     mPointSize;
    bool    mIsBold;
    bool    mIsItalic;
    CString mFaceName;

};

#endif // !defined(AFX_FONT_H__FC3BD1E2_A7E0_11D6_92A3_0000E89F396C__INCLUDED_)
