// IntReg.h: interface for the CIntReg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INTREG_H__CC8271A5_92F5_4D5F_A015_89351D894845__INCLUDED_)
#define AFX_INTREG_H__CC8271A5_92F5_4D5F_A015_89351D894845__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CIntReg  
{
public:
	CIntReg(const CString& strSection, const CString& strEntry, int nDefault);
	~CIntReg();

private:
	CIntReg();

private:
	CString mSection;
	CString mEntry;
	int     mValue;
};

#endif // !defined(AFX_INTREG_H__CC8271A5_92F5_4D5F_A015_89351D894845__INCLUDED_)
