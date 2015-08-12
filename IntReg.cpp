// IntReg.cpp: implementation of the CIntReg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "renlib.h"
#include "IntReg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIntReg::CIntReg(const CString& strSection, const CString& strEntry, int nDefault)
: mSection(strSection),
  mEntry(strEntry)
{
	mValue = AfxGetApp()->GetProfileInt(mSection, mEntry, nDefault);
}

CIntReg::~CIntReg()
{
	AfxGetApp()->WriteProfileInt(mSection, mEntry, mValue);
}
