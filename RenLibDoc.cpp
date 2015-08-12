//
// RenLibDoc.cpp : implementation of the CRenLibDoc class
//

#include "stdafx.h"
#include "RenLib.h"
#include "RenLibDoc.h"
#include "MoveNode.h"
#include "Utils.h"
#include "Game.h"
#include "Renartist.h"
#include "Renjuclass.h"
#include "TextBoard.h"
#include <vector>
#include "SearchItem.h"
#include "MainFrm.h"
#include "ProgressWnd.h"
#include "RenLibAppletDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//--------------------------------------------------------------------
// const
//--------------------------------------------------------------------

namespace
{
	const CPoint NullPoint(0, 0);
	const CPoint Center(8, 8);

	const MajorProgramVersion = 3;
	const MinorProgramVersion = 6;

	const BugFixProgramVersion = 0;

	const Year  = 2006;
	const Month =    7;
	const Day   =   16;

	const Version = (Year * 100 + Month) * 100 + Day;

	CString strEmpty;

	const XY_6_6   =  6 + 5*16;
	const XY_7_6   =  7 + 5*16;
	const XY_8_6   =  8 + 5*16;
	const XY_9_6   =  9 + 5*16;
	const XY_10_6  = 10 + 5*16;

	const XY_6_7   =  6 + 6*16;
	const XY_7_7   =  7 + 6*16;
	const XY_8_7   =  8 + 6*16; // Direct Opening
	const XY_9_7   =  9 + 6*16; // Indirect Opening
	const XY_10_7  = 10 + 6*16;

	const XY_6_8   =  6 + 7*16;
	const XY_7_8   =  7 + 7*16;
	const XY_9_8   =  9 + 7*16;
	const XY_10_8  = 10 + 7*16;

	const XY_6_9   =  6 + 8*16;
	const XY_7_9   =  7 + 8*16;
	const XY_8_9   =  8 + 8*16;
	const XY_9_9   =  9 + 8*16;
	const XY_10_9  = 10 + 8*16;

	const XY_6_10  =  6 + 9*16;
	const XY_7_10  =  7 + 9*16;
	const XY_8_10  =  8 + 9*16;
	const XY_9_10  =  9 + 9*16;
	const XY_10_10 = 10 + 9*16;
}

CString CRenLibDoc::getProgramDate()
{
	CString strDate;
	strDate.Format("%4d.%02d.%02d", Year, Month, Day);
	return strDate;
}

CString CRenLibDoc::getProgramVersion()
{
	CString strVersion;

	if (getBugFixProgramVersion() != 0)
	{
		strVersion.Format(
			"%d.%d.%d", getMajorProgramVersion(), getMinorProgramVersion(),
			getBugFixProgramVersion());
	}
	else
	{
		strVersion.Format(
			"%d.%d", getMajorProgramVersion(), getMinorProgramVersion());
	}

	return strVersion + " - " + getProgramDate();
}

BYTE CRenLibDoc::getMajorProgramVersion()
{
	return MajorProgramVersion;
}

BYTE CRenLibDoc::getMinorProgramVersion()
{
	return MinorProgramVersion;
}

BYTE CRenLibDoc::getBugFixProgramVersion()
{
	return BugFixProgramVersion;
}

int CRenLibDoc::getVersion()
{
	return Version;
}

/////////////////////////////////////////////////////////////////////////////
// CRenLibDoc

IMPLEMENT_DYNCREATE(CRenLibDoc, CDocument)

BEGIN_MESSAGE_MAP(CRenLibDoc, CDocument)
	//{{AFX_MSG_MAP(CRenLibDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRenLibDoc construction/destruction

CRenLibDoc::CRenLibDoc()
{
	Init();
}

CRenLibDoc::~CRenLibDoc()
{
}

BOOL CRenLibDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
	{
		return FALSE;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CRenLibDoc serialization

void CRenLibDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// add storing code here
	}
	else
	{
		// add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CRenLibDoc diagnostics

#ifdef _DEBUG
void CRenLibDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRenLibDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRenLibDoc commands

BOOL CRenLibDoc::IsModified()
{
	return m_IsModified;
}

//---------------------------------------------------------------------------

void CRenLibDoc::SetModifiedFlag(BOOL isModified, BOOL isUpdate)
{
	m_IsModified = isModified;

	if (isModified && isUpdate)
	{
		SetTitle();
	}
}

//---------------------------------------------------------------------------

void CRenLibDoc::Init()
{
	m_Board.Clear();
	m_Board.SetLastMove(NullPoint);
	m_Board.SetPreviousVariant(NullPoint);

	if (!m_MoveList.IsEmpty())
	{
		DeleteTree(m_MoveList.GetRoot());
	}

	m_MoveList.ClearAll();

	m_eMode        = DISPLAY_MODE;

	m_strCommentToFind.Empty();
	m_strBoardTextToFind.Empty();
	m_MoveNoToFind     = 0;
	mIsFindInProgress  = false;

	m_WinMoveNode = 0;

    m_IsWinPosition[VCF] = false;
    m_IsWinPosition[VCT] = false;

	SetModifiedFlag(FALSE);
}

//---------------------------------------------------------------------------

Board* CRenLibDoc::GetBoard()
{
 	return &m_Board;
}

//---------------------------------------------------------------------------

void CRenLibDoc::UpdateBoard(bool ShowAutoLinkVariants, bool ShowChangedOrderOfMovesVariants)
{
	if (m_MoveList.IsEmpty()) return;

	MoveNode* pMove;

	Board::BoardMarker variant;
	Board::BoardMarker autoLinkVariant;
	Board::BoardMarker moveOrderVariant;

	if (m_MoveList.Index() & 1)
	{
		variant          = Board::WHITE_VARIANT;
		autoLinkVariant  = Board::WHITE_AUTOLINKVARIANT;
		moveOrderVariant = Board::WHITE_MOVEORDERVARIANT;
	}
	else
	{
		variant          = Board::BLACK_VARIANT;
		autoLinkVariant  = Board::BLACK_AUTOLINKVARIANT;
		moveOrderVariant = Board::BLACK_MOVEORDERVARIANT;
	}

	// clear board
	m_Board.Clear();

	// set made move markers
	for (int i = 1; i <= m_MoveList.Index(); i++)
	{
		pMove = m_MoveList.Get(i);
		CPoint Pos(pMove->getPos());

		Board::BoardMarker marker;

		if (Pos == NullPoint)
		{
			marker = Board::PASS;
		}
		else if (i & 1)
		{
			marker = Board::BLACK_MOVE;
		}
		else
		{
			marker = Board::WHITE_MOVE;
		}

		m_Board.Set(Pos, marker, pMove, true);
		m_Board.SetNumber(Pos, i);
	}

	// set variant markers
	pMove = m_MoveList.Current()->getDown();

	while (pMove)
	{
		if (pMove->isMove())
		{
			m_Board.Set(pMove->getPos(), variant, pMove);
		}

		if (pMove->isBoardText())
		{
			// set Board Text
			m_Board.SetText(pMove->getPos(), pMove->getBoardText());
		}

		pMove = pMove->getRight();
	}

	if (m_AutoSearchList.GetKind() == SearchList::SAME_POSITION &&
		m_MoveList.Current() == m_AutoSearchList.Current()->getMoveNode())
	{
		//
		// set Auto Link Variant markers
		//
		if (ShowAutoLinkVariants)
		{
			int i;

			for (i=0; i <= m_AutoSearchList.LastItem(); i++)
			{
				pMove = m_AutoSearchList.Get(i)->getMoveNode();

				if (pMove)
				{
					const int nMatch = pMove->getMatch();

					pMove = pMove->getDown();

					while (pMove)
					{
						if (pMove->isMove())
						{
							CPoint Point(pMove->getPos());
							Position::NormalizeCoord(Point, nMatch);
							Position::TransposeCoord(Point, m_MoveList.Current()->getMatch());

							if (m_Board.Get(Point) == Board::NO_MOVE)
							{
								m_Board.Set(Point, autoLinkVariant, pMove);
							}
						}

						pMove = pMove->getRight();
					}
				}
			}
		}

		//
		// set Changed Order Of Moves Variant markers
		//
		if (ShowChangedOrderOfMovesVariants && !m_VariantList.IsEmpty())
		{
			for (i=1; i < m_VariantList.LastItem(); i+=2)
			{
				pMove = m_VariantList.Get(i)->getMoveNode();

				if (pMove)
				{
					const int nMatch = m_VariantList.Get(i)->getMatch();

					CPoint Point(pMove->getPos());
					Position::NormalizeCoord(Point, nMatch);
					Position::TransposeCoord(Point, m_MoveList.Current()->getMatch());
					
					if (m_Board.Get(Point) == Board::NO_MOVE)
					{
						m_Board.Set(Point, moveOrderVariant, pMove);
						m_Board.SetChangedOrder(Point, m_VariantList.Get(i+1)->getMoveNode());
					}
				}
			}
		}
	}

	// set forbidden point markers
	m_Board.setForbiddenInfo(m_MoveList);
}

//---------------------------------------------------------------------------

CString CRenLibDoc::GetHighestAutoBoardText()
{
	CString highestBoardText;

	if (!m_MoveList.IsEmpty())
	{
		MoveNode* pMove = m_MoveList.Current()->getDown();

		while (pMove)
		{
			if (pMove->isBoardText())
			{
				CString boardText(pMove->getBoardText());

				if ((boardText.GetLength() == 1 || boardText.GetLength() == 2) &&
					 highestBoardText < boardText)
				{
					highestBoardText = boardText;
				}
			}

			pMove = pMove->getRight();
		}
	}

	return highestBoardText;
}

//---------------------------------------------------------------------------

BOOL CRenLibDoc::SaveCommand(bool Confirm, bool AskFileName, bool cancelButton)
{
	bool DoSave = true;
	CString newFile;
	Utils::FileType fileType = Utils::RENLIB_LIBRARY;

	if (!AskFileName && (!IsModified() || m_strFile.IsEmpty()))
	{
		return TRUE;
	}

	if (Confirm)
	{
		CString strQuestion(Utils::GetString(IDS_MSG_SAVE, m_strFile));

		int choice = Utils::ShowMessage(strQuestion,
			                            Utils::GetString(IDS_CAP_CONFIRM_SAVE),
										(cancelButton ? MB_YESNOCANCEL : MB_YESNO) + MB_ICONQUESTION);

		if (choice == IDCANCEL)
		{
			return FALSE;
		}

		DoSave = (choice == IDYES);
	}

	if (DoSave && (AskFileName || m_strFile == Utils::GetString(IDS_NEW_LIBRARY)))
	{
		DoSave = Utils::SaveLibraryDialog(newFile, fileType);

		if (fileType == Utils::RENLIB_LIBRARY)
		{
			m_strFile = newFile;

			m_strTitle = Utils::GetTitle(m_strFile);

			m_strBackup = m_strFile;
			Utils::MakeExtensionBak(m_strBackup);
		}

		if (!DoSave)
		{
			return FALSE; // Cancel
		}
	}

	if (DoSave)
	{
		switch (fileType)
		{
			case Utils::RENLIB_LIBRARY:
			{
				if (Utils::exists(m_strBackup))
				{
					Utils::remove(m_strBackup);
				}

				Utils::rename(m_strFile, m_strBackup);

				if (!Utils::IsExtensionLib(m_strFile))
				{
					Utils::MakeExtensionLib(m_strFile);
				}

				CDocument::SetPathName(m_strFile);

				WriteLibrary(m_strFile, 0);

				SetModifiedFlag(FALSE);

				break;
			}
			
			case Utils::PDB:
			{
				SavePdbCommand(newFile);
				break;
			}
			
			case Utils::BDT:
			{
				SaveBdtCommand(newFile);
				break;
			}
			
//			case Utils::BUF:
//			{
//				SaveBufCommand(newFile);
//				break;
//			}
			
			default:
			{
				break;
			}
		}
	}

	return TRUE;
}

//---------------------------------------------------------------------------

void CRenLibDoc::SaveBranchCommand(const CString& strFile)
{
	if (m_MoveList.IsEmpty()) return;

	CString strBackup(strFile);
	Utils::MakeExtensionBak(strBackup);

	if (Utils::exists(strBackup))
	{
		Utils::remove(strBackup);
	}

	Utils::rename(strFile, strBackup);

	AfxGetApp()->AddToRecentFileList(strFile);

	WriteLibrary(strFile, m_MoveList.Index());
}

//---------------------------------------------------------------------------

bool CRenLibDoc::OpenLibraryCommand(const CString& strFile, const CString& strTitle, const CString& strBackup)
{
	LibraryFile libFile;
	bool IsValid;

	if (!libFile.OpenRead(strFile))
	{
		return false;
	}

	Init();

	IsValid = AddLibrary(libFile, true);

	if (IsValid)
	{
		m_strFile   = strFile;
		m_strTitle  = strTitle;
		m_strBackup = strBackup;
	}
	else
	{
		m_strFile   = Utils::GetString(IDS_INVALID_LIBRARY);
		m_strTitle  = Utils::GetString(IDS_INVALID_LIBRARY);
		m_strBackup = Utils::GetString(IDS_INVALID_LIBRARY);
	}

	libFile.Close();

	CDocument::SetPathName(m_strFile, FALSE);

	return IsValid;
}

//---------------------------------------------------------------------------

bool CRenLibDoc::MergeLibraryCommand(const CString& strFile)
{
	if (m_MoveList.IsEmpty()) return false;

	LibraryFile libFile;
	bool IsValid;

	if (!libFile.OpenRead(strFile))
	{
		return false;
	}

	IsValid = AddLibrary(libFile, false);

	libFile.Close();

	return IsValid;
}

//---------------------------------------------------------------------------

void CRenLibDoc::NewLibraryCommand()
{
	Init();

	MoveNode* pCurrentMove = new MoveNode(NullPoint);

	m_MoveList.SetRoot(pCurrentMove);

	m_Board.SetLastMove(NullPoint);
	m_Board.SetPreviousVariant(NullPoint);

	m_eMode = UPDATE_MODE;
    SetModifiedFlag();

	m_strFile   = Utils::GetString(IDS_NEW_LIBRARY);
	m_strTitle  = Utils::GetString(IDS_NEW_LIBRARY);
	m_strBackup = Utils::GetString(IDS_NEW_LIBRARY);

	CDocument::SetPathName(m_strFile, FALSE);
}

//------------------------------------------------------------------------

MoveNode* CRenLibDoc::GetVariant (MoveNode* pMove, CPoint Pos)
{
	if (pMove->getDown())
	{
		pMove = pMove->getDown();

		if (pMove->getPos() == Pos) return pMove;

		while (pMove->getRight())
		{
			pMove = pMove->getRight();
			if (pMove->getPos() == Pos) return pMove;
		}
	}

	return 0;
}

//------------------------------------------------------------------------

void CRenLibDoc::AddMove (MoveNode* pMove, MoveNode* pNewMove)
{
	if (pMove->getDown() == 0)
	{
		pMove->setDown(pNewMove);
	}
	else
	{
		if (Utils::LessThan(pNewMove->getPos(), pMove->getDown()->getPos()))
		{
			pNewMove->setRight(pMove->getDown());
			pMove->setDown(pNewMove);
		}
		else
		{
			pMove = pMove->getDown();

			while (true)
			{
				if (pMove->getRight() == 0)
				{
					pMove->setRight(pNewMove);
					break;
				}
				else if (Utils::LessThan(pNewMove->getPos(), pMove->getRight()->getPos()))
				{
					pNewMove->setRight(pMove->getRight());
					pMove->setRight(pNewMove);
					break;
				}
				pMove = pMove->getRight();
			}
		}
	}
}

//------------------------------------------------------------------------

bool CRenLibDoc::addComment(MoveNode* pMove, CString& strOneLine, CString& strMultiLine)
{
	bool isComment = false;

	if (!strOneLine.IsEmpty())
	{
		isComment = true;

		if (pMove->getOneLineComment().IsEmpty())
		{
			pMove->setOneLineComment(strOneLine);
		}
		else if (pMove->getOneLineComment() != strOneLine)
		{
			m_SearchList.Add(SearchItem(pMove, strOneLine, SearchItem::ONE_LINE_COMMENT));
		}
	}

	if (!strMultiLine.IsEmpty())
	{
		isComment = true;

		if (pMove->getMultiLineComment().IsEmpty() && !strMultiLine.IsEmpty())
		{
			pMove->setMultiLineComment(strMultiLine);
		}
		else if (pMove->getMultiLineComment() != strMultiLine &&
            !Sgf::equalComment(pMove->getMultiLineComment(), strMultiLine))
		{
			m_SearchList.Add(SearchItem(pMove, strMultiLine, SearchItem::MULTI_LINE_COMMENT));
		}
	}

	return isComment;
}

//------------------------------------------------------------------------

bool CRenLibDoc::addBoardText(MoveNode* pMove, CString& boardText)
{
	bool isBoardText = false;

	if (!boardText.IsEmpty())
	{
		isBoardText = true;

		if (pMove->getBoardText().IsEmpty())
		{
			pMove->setBoardText(boardText);
		}
		else if (pMove->getBoardText() != boardText)
		{
			m_SearchList.Add(SearchItem(pMove, boardText, SearchItem::BOARD_TEXT));
		}
	}

	return isBoardText;
}

//------------------------------------------------------------------------

void CRenLibDoc::addAttributes(MoveNode* pMove, MoveNode* pFrom)
{
	bool bMark;
	bool bMove;
	bool bStart;

	addAttributes(pMove, pFrom, bMark, bMove, bStart);
}

//------------------------------------------------------------------------

void CRenLibDoc::addAttributes(MoveNode* pMove, MoveNode* pFrom,
                               bool& bMark, bool& bMove, bool& bStart)
{
	bMark  = false;
	bMove  = false;
	bStart = false;

	if (pFrom->isMark() && !pMove->isMark())
	{
		bMark = true;
		pMove->setIsMark(bMark);
	}

	if (pFrom->isMove() && !pMove->isMove())
	{
		bMove  = true;
		pMove->setIsMove(bMove);
	}

	if (pFrom->isStart() && !pMove->isStart())
	{
		bStart = true;
		pMove->setIsStart(bStart);
	}
}

//------------------------------------------------------------------------

void CRenLibDoc::ReadOldComment (LibraryFile& libFile, CString& strOneLine, CString& strMultiLine)
{
	BYTE buffer[2];
	CString strNew;

	strOneLine.Empty();
	strMultiLine.Empty();

	while (true)
	{
		libFile.Get(buffer[0], buffer[1]);

		strNew += TCHAR(buffer[0]);

		if (Utils::msb(buffer[0]))
		{
			break;
		}

		strNew += TCHAR(buffer[1]);

		if (Utils::msb(buffer[1]))
		{
			break;
		}
	}

	Utils::AsciiToWinChar(strNew);

	int n = strNew.Find(TCHAR(8));

	if (n == -1)
	{
		strOneLine = strNew;
	}
	else
	{
		strOneLine   = strNew.Left(n);
		strMultiLine = strNew.Right(strNew.GetLength() - n - 1);
	}
}

//------------------------------------------------------------------------

void CRenLibDoc::ReadNewComment (LibraryFile& libFile, CString& strOneLine, CString& strMultiLine)
{
	BYTE	buffer[2];
	CString strNew;

	strOneLine.Empty();
	strMultiLine.Empty();

	while (true)
	{
		libFile.Get(buffer[0], buffer[1]);

		if (buffer[0] == 0)
		{
			break;
		}

		strNew += TCHAR(buffer[0]);

		if (buffer[1] == 0)
		{
			break;
		}

		strNew += TCHAR(buffer[1]);
	}

	int n = strNew.Find(TCHAR(8));

	if (n == -1)
	{
		strOneLine = strNew;
	}
	else
	{
		strOneLine   = strNew.Left(n);
		strMultiLine = strNew.Right(strNew.GetLength() - n - 1);
	}
}

//------------------------------------------------------------------------

void CRenLibDoc::ReadBoardText (LibraryFile& libFile, CString& strBoardText)
{
	BYTE	buffer[2];

	strBoardText.Empty();

	while (true)
	{
		libFile.Get(buffer[0], buffer[1]);

		if (buffer[0] == 0)
		{
			break;
		}

		strBoardText += TCHAR(buffer[0]);

		if (buffer[1] == 0)
		{
			break;
		}

		strBoardText += TCHAR(buffer[1]);
	}
}

//------------------------------------------------------------------------

bool CRenLibDoc::FindMoveNode(MoveNode* pMoveToFind)
{
	Stack m_Stack;

	bool isFound = false;

	MoveNode* pMove = m_MoveList.GetRoot();
	m_MoveList.ClearAll();

	bool done = false;

	while (!done)
	{
		if (pMove)
		{
			m_MoveList.Add(pMove);

			if (pMove == pMoveToFind)
			{
				isFound = true;
				break;
			}

			if (pMove->getRight())
			{
				m_Stack.Push(m_MoveList.Index(), pMove->getRight());
			}

			pMove = pMove->getDown();
		}
		else if (!m_Stack.IsEmpty())
		{
			int nMove;
			m_Stack.Pop(nMove, pMove);
			m_MoveList.SetIndex(nMove - 1);
		}
		else
		{
			m_MoveList.SetRootIndex();
			pMove = m_MoveList.Current();
			done  = true;
		}
	}

	m_Board.SetLastMove(pMove->getPos());
	SetPreviousVariant();

	// Clear end of move list
	m_MoveList.ClearEnd();

	return isFound;
}

//------------------------------------------------------------------------

MoveNode* CRenLibDoc::GetVariant (CPoint Pos)
{
	MoveNode* pMove = 0;

	if (!m_MoveList.IsEmpty())
	{
		pMove = m_MoveList.Current()->getDown();
	}

	while (pMove)
	{
		if (Pos == pMove->getPos())
		{
			return pMove;
		}
		pMove = pMove->getRight();
	}

	return 0;
}

//------------------------------------------------------------------------

void CRenLibDoc::DisplayMove (CPoint Pos)
{
	MoveNode* pMove = GetVariant(Pos);
	MoveNode* pNextMove;

	if (pMove && pMove->isMove())
	{
		pNextMove = m_MoveList.Next();
		m_MoveList.Add(pMove);

		m_Board.SetLastMove(Pos);
		SetPreviousVariant();

		if (pNextMove != pMove)
		{
			m_MoveList.ClearEnd();
		}
		else
		{
			pNextMove = m_MoveList.Next();

			if (pNextMove != 0)
			{
				m_Board.SetPreviousVariant(pNextMove->getPos());
			}
		}
	}
}

//------------------------------------------------------------------------

bool CRenLibDoc::IsExistingMove (CPoint Pos)
{
	for (int i = 1; i <= m_MoveList.Index(); i++)
	{
		if (Pos == m_MoveList.Get(i)->getPos())
		{
			return true;
		}
	}

	return false;
}

//------------------------------------------------------------------------

void CRenLibDoc::InsertMove(CPoint Pos)
{
	MoveNode* pMove = GetVariant(Pos);

	if (pMove && pMove->isMove())
	{
		DisplayMove (Pos);
	}
	else if (pMove && !pMove->isMove())
	{
		pMove->setIsMove(true);
		DisplayMove (Pos);
		SetModifiedFlag();
	}
    else
    {
	    if (m_eMode == UPDATE_MODE)
        {
	        if (!IsExistingMove(Pos) || Pos == NullPoint)
	        {
		        pMove = new MoveNode(Pos);

                if (Pos == NullPoint)
                {
                    pMove->setMultiLineComment(Utils::GetString(IDS_PASS));
                }

		        AddMove (m_MoveList.Current(), pMove);

		        m_MoveList.Add(pMove);

		        m_Board.SetLastMove(Pos);
		        m_Board.SetPreviousVariant(NullPoint);

		        SetModifiedFlag();
	        }
        }
        else // PROBLEM_MODE
        {
            InsertProblemMove(Pos, true);
        }
    }
}

//------------------------------------------------------------------------

int CRenLibDoc::FindProblemMove(CPoint Pos)
{
	for (int i = 1; i <= m_MoveList.Index(); i++)
	{
		if (Pos == m_MoveList.Get(i)->getPos())
		{
			return i;
		}
	}

	return -1;
}

//------------------------------------------------------------------------

void CRenLibDoc::InsertProblemMove(CPoint Pos, bool blackMove)
{
    MoveNode* pExistingBlackMove = 0;
    MoveNode* pExistingWhiteMove = 0;

    MoveNode* pBlackEmptyMove = 0;
    MoveNode* pWhiteEmptyMove = 0;
    
    MoveNode* pMove;

    bool addPassMove    = false;
    bool addProblemMove = false;

    //
    // Find move and also empty black/white moves
    //
    for (int i = 1; i <= m_MoveList.Index(); i++)
	{
        pMove = m_MoveList.Get(i);

        if (pMove->getPos() == Pos)
		{
            if (i % 2 == 1)
            {
                pExistingBlackMove = pMove;
            }
            else
            {
                pExistingWhiteMove = pMove;
            }
		}
        else if (pMove->getPos() == NullPoint)
		{
            if (i % 2 == 1)
            {
                if (pBlackEmptyMove == 0)
                {
                    pBlackEmptyMove = pMove;
                }
            }
            else
            {
                if (pWhiteEmptyMove == 0)
                {
                    pWhiteEmptyMove = pMove;
                }
            }
		}
	}

    if (pExistingBlackMove)
    {
        pExistingBlackMove->setPos(NullPoint);
    }
    else if (pExistingWhiteMove)
    {
        pExistingWhiteMove->setPos(NullPoint);
    }

    if (blackMove && !pExistingBlackMove)
    {
        if (pBlackEmptyMove)
        {
            pBlackEmptyMove->setPos(Pos);
        }
        else
        {
            if (m_MoveList.Index() % 2 == 1)
            {
                addPassMove = true;
            }
            
            addProblemMove = true;
        }
    }
    else if (!blackMove && !pExistingWhiteMove)
    {
        if (pWhiteEmptyMove)
        {
            pWhiteEmptyMove->setPos(Pos);
        }
        else
        {
            if (m_MoveList.Index() % 2 == 0)
            {
                addPassMove = true;
            }
            
            addProblemMove = true;
        }
    }

    if (addPassMove)
    {
        pMove = new MoveNode(NullPoint);
        AddMove (m_MoveList.Current(), pMove);
        m_MoveList.Add(pMove);
    }

    if (addProblemMove)
    {
        pMove = new MoveNode(Pos);
        AddMove (m_MoveList.Current(), pMove);
        m_MoveList.Add(pMove);
    }

    //
    // Put empty black/white moves at end
    // Sort moves left -> right and top -> down
    //
    for (i = 1; i <= m_MoveList.Index(); i++)
	{
        pMove = m_MoveList.Get(i);
        CPoint pos(pMove->getPos());

        if (pos == NullPoint)
        {
            // Swap empty pos with first non empty pos
            //
            for (int j = i+2; j <= m_MoveList.Index(); j += 2)
	        {
                MoveNode* pMove2 = m_MoveList.Get(j);
                CPoint pos2(pMove2->getPos());

                if (pos2 != NullPoint)
		        {
                    pMove->setPos(pos2);
                    pMove2->setPos(pos);
                    pos = pos2;
                    break;
		        }
    	    }
        }

        if (pos != NullPoint)
        {
            // Swap first pos with higher pos
            //
            for (int j = i+2; j <= m_MoveList.Index(); j += 2)
	        {
                MoveNode* pMove2 = m_MoveList.Get(j);
                CPoint pos2(pMove2->getPos());

                if (pos2 != NullPoint && Utils::isLessThan(pos2, pos))
		        {
                    pMove->setPos(pos2);
                    pMove2->setPos(pos);
                    pos = pos2;
		        }
    	    }
        }
	}

    for (i = m_MoveList.Index(); i > 0 ; i--)
	{
        pMove = m_MoveList.Get(i);

        if (pMove->getPos() == NullPoint)
        {
            // setDown // remove empty pos at end
            DeleteMove(pMove, m_MoveList.Get(i-1));
		    m_MoveList.Decrement();
		    m_MoveList.ClearEnd();
	    }
        else
        {
            break;
	    }
	}

    m_Board.SetLastMove(NullPoint);
	m_Board.SetPreviousVariant(NullPoint);

	SetModifiedFlag();
}

//------------------------------------------------------------------------

void CRenLibDoc::DeleteMove(MoveNode* pCurrentMove, MoveNode* pPreviousMove, bool keepText)
{
	if (pCurrentMove)
	{
		bool doDelete = false;

		if (pPreviousMove->getDown() == pCurrentMove)
		{
			if (keepText && pCurrentMove->isBoardText())
			{
				pCurrentMove->setIsMove(false);
				DeleteMove(pCurrentMove->getDown(), pCurrentMove, false);
			}
			else
			{
				pPreviousMove->setDown(pCurrentMove->getRight());
				doDelete = true;
			}
		}
		else if (pPreviousMove->getRight() == pCurrentMove)
		{
			if (keepText && pCurrentMove->isBoardText())
			{
				pCurrentMove->setIsMove(false);
				DeleteMove(pCurrentMove->getDown(), pCurrentMove, false);
			}
			else
			{
				pPreviousMove->setRight(pCurrentMove->getRight());
				doDelete = true;
			}
		}
		else
		{
			pPreviousMove = pPreviousMove->getDown();

			while (pPreviousMove)
			{
				if (pPreviousMove->getRight() == pCurrentMove)
				{
					if (keepText && pCurrentMove->isBoardText())
					{
						pCurrentMove->setIsMove(false);
						DeleteMove(pCurrentMove->getDown(), pCurrentMove, false);
					}
					else
					{
						pPreviousMove->setRight(pCurrentMove->getRight());
						doDelete = true;
					}

					break;
				}

				pPreviousMove = pPreviousMove->getRight();
			}
		}

		if (doDelete)
		{
			m_SearchList.Clear(pCurrentMove);

			pCurrentMove->setRight(0);
			DeleteTree (pCurrentMove);
		}

		SetModifiedFlag();
	}
}

//------------------------------------------------------------------------

void CRenLibDoc::DeleteCommand ()
{
	if (m_MoveList.Index() < 1) return;

	int     total    = NoOfMovesInBranch();
	bool	DoDelete = false;
	CString strQuestion;

	if (total > 0)
	{
		strQuestion = Utils::GetString(IDS_MSG_DELETE_MOVES, total + 1);
	}
	else
	{
		strQuestion = Utils::GetString(IDS_MSG_DELETE_CUR_MOVE);
	}

	DoDelete = Utils::ShowMessage(strQuestion,
		                          Utils::GetString(IDS_CAP_DELETE_MOVE),
								  MB_YESNO + MB_ICONQUESTION) == IDYES;

	if (DoDelete)
	{
		MoveNode* pCurrentMove  = m_MoveList.Current();
		MoveNode* pPreviousMove = m_MoveList.Previous();

		DeleteMove(pCurrentMove, pPreviousMove);

		m_MoveList.Decrement();
		m_MoveList.ClearEnd();

		pPreviousMove = m_MoveList.Current();

		m_Board.SetLastMove(pPreviousMove->getPos());
		SetPreviousVariant();
	}
}

//------------------------------------------------------------------------

void CRenLibDoc::DeleteVariants(MoveNode* pCurrentMove, MoveNode* pPreviousMove)
{
	if (pCurrentMove)
	{
		DeleteVariants(pCurrentMove->getRight(), pCurrentMove);
		DeleteMove(pCurrentMove, pPreviousMove);
	}
}

//----------------------------------------------------------------------

void CRenLibDoc::DeleteVariantsCommand ()
{
	int     total    = NoOfMovesInBranch();
	bool	DoDelete = false;
	CString strQuestion;

	if (total == 0)
	{
		return;
	}
	else if (total == 1)
	{
		strQuestion = Utils::GetString(IDS_MSG_DELETE_ONE_MOVE);
	}
	else
	{
		strQuestion = Utils::GetString(IDS_MSG_DELETE_MOVES, total);
	}

	DoDelete = Utils::ShowMessage(strQuestion,
		                          Utils::GetString(IDS_CAP_DELETE_VARIANTS),
								  MB_YESNO + MB_ICONQUESTION) == IDYES;

	if (DoDelete)
	{
		MoveNode* pPreviousMove = m_MoveList.Current();
		MoveNode* pCurrentMove  = pPreviousMove->getDown();

		DeleteVariants(pCurrentMove, pPreviousMove);
	}
}

//------------------------------------------------------------------------

void CRenLibDoc::ClearAllMarksCommand()
{
	if (!m_MoveList.IsEmpty())
	{
		int NrOfMarks = GetNumberOfMarks(m_MoveList.GetRoot());

		if (NrOfMarks > 0)
		{
			CString strQuestion(Utils::GetString(IDS_MSG_CLEAR_ALL_MARKS, NrOfMarks));

			if (Utils::ShowMessage(strQuestion,
				                   Utils::GetString(IDS_CAP_CLEAR_ALL_MARKS),
								   MB_YESNO + MB_ICONQUESTION) == IDYES)
			{
				ClearMarks(m_MoveList.GetRoot());
				SetModifiedFlag();
			}
		}
	}
}

//------------------------------------------------------------------------

void CRenLibDoc::ButtonCommand (CPoint Pos, bool MakeSingleVariant, bool& hasChangedBranch)
{
	switch (m_Board.Get(Pos))
	{
		case Board::BLACK_AUTOLINKVARIANT:
		case Board::WHITE_AUTOLINKVARIANT:
		{
			hasChangedBranch = true;

			FindMoveNode(m_Board.GetMove(Pos));
			break;
		}

		case Board::BLACK_MOVEORDERVARIANT:
		case Board::WHITE_MOVEORDERVARIANT:
		{
			hasChangedBranch = true;

			FindMoveNode(m_Board.GetChangedOrder(Pos));
			break;
		}

		default:
		{
			hasChangedBranch = false;

			CPoint singlePos;

			if (m_eMode == DISPLAY_MODE &&
				MakeSingleVariant &&
				GetSingleVariant(singlePos))
			{
				SetPosition(singlePos);
			}
			else
			{
				SetPosition (Pos);
			}

			if (!m_MoveList.IsEmpty())
			{
				m_MoveList.Current()->setMatch(0);
			}

			break;
		}
	}
}

//------------------------------------------------------------------------

void CRenLibDoc::RButtonCommand (CPoint Pos)
{
    InsertProblemMove(Pos, false);
}

//------------------------------------------------------------------------

void CRenLibDoc::SetPosition (CPoint Pos)
{
	switch (m_eMode)
	{
		case UPDATE_MODE:
		case PROBLEM_MODE:
		{
			InsertMove(Pos);
			break;
		}

		case DISPLAY_MODE:
		{
			DisplayMove(Pos);
			break;
		}
	}
}

//----------------------------------------------------------------------

void CRenLibDoc::BoardTextCommand(CPoint Pos, CString strBoardText)
{
	Utils::trim(strBoardText);

	MoveNode* pMove = 0;

	if (GetVariant(Pos))
	{
		pMove = GetVariant(Pos);
	}
	else if (!IsExistingMove(Pos))
	{
		pMove = new MoveNode(Pos);
		pMove->setIsMove(false);

		AddMove (m_MoveList.Current(), pMove);
	}

	SetTextCommand(pMove, strBoardText);
}

//----------------------------------------------------------------------

void CRenLibDoc::SetTextCommand(MoveNode* pMove, const CString& strBoardText)
{
	if (pMove)
	{
		pMove->setBoardText(strBoardText);
		SetModifiedFlag();
	}
}

//----------------------------------------------------------------------

bool CRenLibDoc::GetSingleVariant(CPoint& Pos)
{
	bool isSingleVariant = false;

	Pos = NullPoint;

	if (NoOfVariantMoves() == 1)
	{
		MoveNode* pMove = m_MoveList.Current()->getDown();

		while (pMove)
		{
			if (pMove->isMove())
			{
				Pos = pMove->getPos();
				isSingleVariant = true;
				break;
			}

			pMove = pMove->getRight();
		}
	}

	return isSingleVariant;
}

//----------------------------------------------------------------------

int CRenLibDoc::GetTotal (MoveNode* pMove)
{
	if (pMove)
	{
		return (pMove->isMove() ? 1 : 0 ) +
			   GetTotal(pMove->getDown()) +
			   GetTotal(pMove->getRight());
	}

	return 0;
}

//----------------------------------------------------------------------

void CRenLibDoc::DeleteTree (MoveNode* pMove)
{
	if (pMove)
	{
		DeleteTree (pMove->getDown());
		DeleteTree (pMove->getRight());

		if (pMove == m_WinMoveNode)
		{
			m_WinMoveNode = 0;
		}

		delete pMove;
	}
}

//----------------------------------------------------------------------

int CRenLibDoc::NoOfMovesInBranch()
{
	int Total = 0;

	if (!m_MoveList.IsEmpty())
	{
		MoveNode* pMove = m_MoveList.Current()->getDown();

		Total = GetTotal(pMove);
	}

	return Total;
}

//----------------------------------------------------------------------

int CRenLibDoc::NoOfVariantMoves()
{
	int Local = 0;

	if (!m_MoveList.IsEmpty())
	{
		Local = NoOfVariantMoves(m_MoveList.Current());
	}

	return Local;
}

//----------------------------------------------------------------------

int CRenLibDoc::NoOfVariantMoves(MoveNode* pMove)
{
	int Local = 0;

	if (pMove)
	{
		pMove = pMove->getDown();
	}

	while (pMove)
	{
		if (pMove->isMove())
		{
			Local++;
		}

		pMove = pMove->getRight();
	}

	return Local;
}

//----------------------------------------------------------------------

bool CRenLibDoc::hasPassMove()
{
	for (int i = 1; i <= m_MoveList.Index(); i++)
	{
		if (m_MoveList.Get(i)->getPos() == NullPoint)
		{
            AfxTrace("hasPassMove() : true");
            return true;
		}
	}

    AfxTrace("hasPassMove() : false");
    return false;
}

//----------------------------------------------------------------------

bool CRenLibDoc::hasPassVariant()
{
    MoveNode* pPassMove = 0;

    if (!m_MoveList.IsEmpty())
	{
		pPassMove = getPassVariant(m_MoveList.Current());
	}

	return pPassMove != 0;
}

//----------------------------------------------------------------------

MoveNode* CRenLibDoc::getPassVariant(MoveNode* pMove)
{
    MoveNode* pPassMove = 0;

	if (pMove)
	{
		pMove = pMove->getDown();
	}

	while (pMove)
	{
		if (pMove->isPassMove())
		{
			pPassMove = pMove;
            break;
		}

		pMove = pMove->getRight();
	}

	return pPassMove;
}

//----------------------------------------------------------------------

bool CRenLibDoc::hasVariants()
{
    MoveNode* pMove = m_MoveList.GetRoot();

	while (pMove)
	{
		if (pMove->getRight())
		{
			return true;
		}

		pMove = pMove->getDown();
	}

	return false;
}

//----------------------------------------------------------------------

int CRenLibDoc::NoOfBoardTexts(MoveNode* pMove)
{
	int Local = 0;

	if (pMove)
	{
		pMove = pMove->getDown();
	}

	while (pMove)
	{
		if (pMove->isBoardText())
		{
			Local++;
		}

		pMove = pMove->getRight();
	}

	return Local;
}

//----------------------------------------------------------------------

int CRenLibDoc::GetNumberOfMarks (MoveNode* pMove)
{
	if (pMove)
	{
		return (pMove->isMark() ? 1 : 0) +
			    GetNumberOfMarks(pMove->getDown()) + GetNumberOfMarks(pMove->getRight());
	}

	return 0;
}

//------------------------------------------------------------------------

void CRenLibDoc::ClearMarks (MoveNode* pMove)
{
	if (pMove)
	{
		pMove->setIsMark(false);
		ClearMarks(pMove->getDown());
		ClearMarks(pMove->getRight());
	}
}

//------------------------------------------------------------------------

void CRenLibDoc::ClearStartPosition(MoveNode* pMove)
{
	if (pMove)
	{
		pMove->setIsStart(false);
		ClearStartPosition(pMove->getDown());
		ClearStartPosition(pMove->getRight());
	}
}

//------------------------------------------------------------------------

MoveNode* CRenLibDoc::FindStartPosition(MoveNode* pMove)
{
	MoveNode* pFirst = 0;

	if (pMove)
	{
		if (pMove->isStart())
		{
			return pMove;
		}

		pFirst = FindStartPosition(pMove->getDown());

		if (pFirst == 0)
		{
			pFirst = FindStartPosition(pMove->getRight());
		}
	}

	return pFirst;
}

//------------------------------------------------------------------------

void CRenLibDoc::UpCommand ()
{
	int backIndex = (m_MoveList.Index() > 1) ? 1 : 0;

	do
	{
		BackCommand();
	}
	while (m_MoveList.Index() > backIndex && NoOfVariantMoves() <= 1);
}

//----------------------------------------------------------------------

void CRenLibDoc::FirstCommand()
{
	int backIndex = (m_MoveList.Index() > 1) ? 1 : 0;

	while (m_MoveList.Index() > backIndex)
	{
		BackCommand();
	}
}

//----------------------------------------------------------------------

bool CRenLibDoc::IsDownCommand()
{
	return NoOfVariantMoves() == 1;
}

//------------------------------------------------------------------------

void CRenLibDoc::DownCommand()
{
	CPoint singlePos;

	while (GetSingleVariant(singlePos))
	{
		SetPosition (singlePos);
	}
}

//----------------------------------------------------------------------

void CRenLibDoc::InsertPassMove()
{
    InsertMove(NullPoint);
}

//----------------------------------------------------------------------

void CRenLibDoc::DisplayPassMove()
{
    DisplayMove(NullPoint);
}

//----------------------------------------------------------------------

void CRenLibDoc::BackCommand()
{
	moveBack();
}

//------------------------------------------------------------------------

void CRenLibDoc::moveBack()
{
	if (m_MoveList.Index() > 0)
	{
		m_Board.SetPreviousVariant(m_MoveList.Current()->getPos());
		m_MoveList.Decrement();
		m_Board.SetLastMove(m_MoveList.Current()->getPos());
	}
}

//------------------------------------------------------------------------

bool CRenLibDoc::IsForwardCommand()
{
	return (!m_MoveList.IsFull() &&
		    (m_MoveList.Next() != 0 || NoOfVariantMoves() == 1));
}

//------------------------------------------------------------------------

void CRenLibDoc::ForwardCommand()
{
	if (IsForwardCommand())
	{
		if (m_MoveList.Next() != 0)
		{
			SetPosition(m_MoveList.Next()->getPos());
		}
		else
		{
			CPoint singlePos;

			GetSingleVariant(singlePos);
			SetPosition(singlePos);
		}
	}
}

//------------------------------------------------------------------------

CString CRenLibDoc::getOneLineComment()
{
	if (!m_MoveList.IsEmpty())
	{
		return m_MoveList.Current()->getOneLineComment();
	}
	else
	{
		return "";
	}
}

//------------------------------------------------------------------------

void CRenLibDoc::getOneLineComments(std::vector<CString>& comments)
{
    CString oneLineComment;

    for (int i = 1; i <= m_MoveList.Index(); i++)
	{
		oneLineComment = m_MoveList.Get(i)->getOneLineComment();

        if (!oneLineComment.IsEmpty())
        {
            bool doAdd = true;

            for (int i=0; i < comments.size(); i++)
            {
                if (oneLineComment == comments[i])
                {
                    doAdd = false;
                    break;
                }
            }

            if (doAdd)
            {
                comments.push_back(oneLineComment);
            }
        }
	}
}

//------------------------------------------------------------------------

CString CRenLibDoc::GetBoardText(CPoint Pos)
{
	MoveNode* pMove = GetVariant(Pos);

	if (pMove)
	{
		return pMove->getBoardText();
	}
	else
	{
		return "";
	}
}

//------------------------------------------------------------------------

CString CRenLibDoc::getMultiLineComment ()
{
	if (!m_MoveList.IsEmpty())
	{
		return m_MoveList.Current()->getMultiLineComment();
	}
	else
	{
		return "";
	}
}

//------------------------------------------------------------------------

bool CRenLibDoc::IsMark ()
{
	if (!m_MoveList.IsEmpty())
	{
		return m_MoveList.Current()->isMark();
	}
	else
	{
		return false;
	}
}

//------------------------------------------------------------------------

bool CRenLibDoc::IsStartPosition()

{
	if (!m_MoveList.IsEmpty())
	{
		return m_MoveList.Current()->isStart();
	}
	else
	{
		return false;
	}
}

//------------------------------------------------------------------------

void CRenLibDoc::SetOneLineComment(const CString& strOneLine)
{
	if (!m_MoveList.IsEmpty())
	{
		const CString OriginalComment(m_MoveList.Current()->getOneLineComment());

		if (strOneLine != OriginalComment)
		{
			m_MoveList.Current()->setOneLineComment(strOneLine);
			SetModifiedFlag(TRUE, TRUE);
		}
	}
}

//------------------------------------------------------------------------

void CRenLibDoc::SetMultiLineComment(const CString& strMultiLine)
{
	if (!m_MoveList.IsEmpty())
	{
		const CString OriginalExtra(m_MoveList.Current()->getMultiLineComment());

		if (strMultiLine != OriginalExtra)
		{
			m_MoveList.Current()->setMultiLineComment(strMultiLine);
			SetModifiedFlag(TRUE, TRUE);
		}
	}
}

//------------------------------------------------------------------------

bool CRenLibDoc::AddLibrary(LibraryFile& libFile, bool FullTree)
{
	if (!libFile.CheckVersion())
	{
		return false;
	}

	CWaitCursor cursor;

	Stack m_Stack;

	int number = 0;

	MoveNode* pCurrentMove = 0;

	if (m_MoveList.IsEmpty())
	{
		pCurrentMove = new MoveNode(NullPoint);
		m_MoveList.SetRoot(pCurrentMove);
	}
	else
	{
		pCurrentMove = m_MoveList.GetRoot();
		m_MoveList.SetRootIndex();
	}

	MoveNode* pFirstMove = 0;
	MoveNode* pNextMove;

	int nComments   = 0;
	int nBoardTexts = 0;
	int nMarks      = 0;
	int nNewMoves   = 0;

	bool bMark;
	bool bMove;
	bool bStart;

	bool checkRoot = true;

	MoveNode next;

	while (libFile.Get(next))
	{
		const CPoint Point(next.getPos());

		if (checkRoot && next.getPos() == NullPoint)
		{
			// Skip root node
			checkRoot = false;
		}
		else if ((Point.x != 0 || Point.y != 0) && (Point.x < 1 || Point.x > 15 || Point.y < 1 || Point.y > 15))
		{
			// ERROR checking code
			CString strMessage;
			strMessage += Utils::GetString(IDS_MSG_LIBRARY_DATA, number);
			strMessage += "\n";
			strMessage += Utils::GetString(IDS_MSG_POSITION, next.getPos());
			strMessage += "\n\n";
			strMessage += Utils::GetString(IDS_MSG_REPORT);
			strMessage += "\n\n";
			strMessage += Utils::GetString(IDS_MSG_NOTE);
			Utils::ShowMessage(strMessage, Utils::GetString(IDS_CAP_ADD_TABLE), MB_ICONERROR);

			next.setPos(CPoint(1,1));
			next.setIsMark(true);
			SetModifiedFlag();

			return false;
		}
		else
		{
			number++;

			pNextMove = GetVariant(pCurrentMove, next.getPos());

			if (pNextMove)
			{
				pCurrentMove = pNextMove;
			}
			else
			{
				pNextMove = new MoveNode(next);

				AddMove(pCurrentMove, pNextMove);
				pCurrentMove = pNextMove;

				if (pCurrentMove->isMove())
				{
					nNewMoves++;

					if (pFirstMove == 0)
					{
						pFirstMove = pCurrentMove;
					}
				}
			}

			m_MoveList.Add(pCurrentMove);
		}

		if (next.isOldComment() || next.isNewComment())
		{
			CString strOneLine;
			CString strMultiLine;

			if (next.isOldComment())
			{
				ReadOldComment(libFile, strOneLine, strMultiLine);
			}
			else if (next.isNewComment())
			{
				ReadNewComment(libFile, strOneLine, strMultiLine);
			}

			if (addComment(pCurrentMove, strOneLine, strMultiLine))
			{
				nComments++;
			}
		}

		CString strBoardText;

		if (next.isBoardText())
		{
			ReadBoardText(libFile, strBoardText);

			if (addBoardText(pCurrentMove, strBoardText))
			{
				nBoardTexts++;
			}
		}

		// Add attributes
		addAttributes(pCurrentMove, &next, bMark, bMove, bStart);

		if (bMark || next.isMark())
		{
			nMarks++;
		}

		if (next.isDown())
		{
			m_Stack.Push(m_MoveList.Index());
		}

		if (next.isRight())
		{
			if (!m_Stack.IsEmpty())
			{
				int nMove;
				m_Stack.Pop(nMove);
				m_MoveList.SetIndex(nMove - 1);
				pCurrentMove = m_MoveList.Current();
			}
		}
	}

	if (number > 0)
	{
		m_MoveList.SetRootIndex();
		m_MoveList.ClearEnd();
	}

	m_Board.SetLastMove(Center);
	SetPreviousVariant();

	CString strInfo;

	if (FullTree)
	{
		strInfo = Utils::GetString(IDS_READ_MOVES, nNewMoves);
	}
	else
	{
		strInfo = Utils::GetString(IDS_ADDED_MOVES, nNewMoves, number);

		if (nNewMoves || nComments || nMarks)
		{
			m_eMode = UPDATE_MODE;
			SetModifiedFlag();
		}

		if (pFirstMove)
		{
			FindMoveNode(pFirstMove);
		}
	}

	strInfo += Utils::GetString(IDS_INFO_COMMENTS, nComments);
	strInfo += Utils::GetString(IDS_INFO_BOARD_TEXTS, nBoardTexts);
	strInfo += Utils::GetString(IDS_INFO_MARKS, nMarks);
	strInfo += Utils::GetString(IDS_INFO_FROM, libFile.GetFilePath());

	SetPaneText(strInfo);

	return true;
}

//------------------------------------------------------------------------

void CRenLibDoc::AddGame (Game& aGame)
{
	if (aGame.CheckMoves() != Game::VALID)
	{
		return;
	}

	int number = 0;

	MoveNode* pCurrentMove = m_MoveList.GetRoot();
	MoveNode* pNextMove;

	int nNewMoves = 0;

	MoveNode next;

	while (aGame.hasMoreMoves())
	{
		Game::Node* gameNode = aGame.nextMove();

		number++;
		next = MoveNode(Utils::PosToPoint(gameNode->getPos()));

		pNextMove = GetVariant(pCurrentMove, next.getPos());

		if (pNextMove)
		{
			pCurrentMove = pNextMove;
			pCurrentMove->setIsMove(true);
		}
		else
		{
			pNextMove = new MoveNode(next);

			AddMove (pCurrentMove, pNextMove);
			pCurrentMove = pNextMove;

			nNewMoves++;
		}

		addComment(pCurrentMove, gameNode->getOneLineComment(), gameNode->getMultiLineComment());

		while (gameNode->hasMoreBoardTexts())
		{
			BYTE    coord;
			CString strBoardText;

			gameNode->nextBoardText(coord, strBoardText);

			const CPoint Pos(Utils::PosToPoint(coord));

			pNextMove = GetVariant(pCurrentMove, Pos);

			if (!pNextMove)
			{
				pNextMove = new MoveNode(Pos);
				pNextMove->setIsMove(false);
				AddMove (pCurrentMove, pNextMove);
			}

			addBoardText(pNextMove, strBoardText);
		}
	}

	m_Board.SetLastMove(Center);
	SetPreviousVariant();

	CString strInfo;

	if (nNewMoves == number)
	{
		strInfo = Utils::GetString(IDS_READ_MOVES, number);
	}
	else
	{
		strInfo = Utils::GetString(IDS_ADDED_MOVES, nNewMoves, number);

		if (nNewMoves)
		{
			SetModifiedFlag();
		}
	}

	FindMoveNode(pCurrentMove);

	strInfo += Utils::GetString(IDS_INFO_FROM, aGame.getFilePath());

	SetPaneText(strInfo);
}

//------------------------------------------------------------------------

void CRenLibDoc::AddRenJS(RenJS& aRenjs)
{
	CWaitCursor cursor;

	int nNewMoves = 0;
	int number = 0;

	MoveNode* pCurrentMove;

	while (aRenjs.hasMoreGames())
	{
		AddRenJS(aRenjs.nextGame(), number, pCurrentMove, nNewMoves);
	}

	m_Board.SetLastMove(Center);
	SetPreviousVariant();

	CString strInfo;

	if (nNewMoves == number)
	{
		strInfo = Utils::GetString(IDS_READ_MOVES, number);
	}
	else
	{
		strInfo = Utils::GetString(IDS_ADDED_MOVES, nNewMoves, number);

		if (nNewMoves)
		{
			SetModifiedFlag();
		}
	}

	FindMoveNode(pCurrentMove);

	strInfo += Utils::GetString(IDS_INFO_FROM, aRenjs.getFilePath());

	SetPaneText(strInfo);
}

//------------------------------------------------------------------------

void CRenLibDoc::AddRenJS(RenJS::Node* node, int& number, MoveNode*& pCurrentMove, int& nNewMoves)
{
	MoveNode* pNextMove;

	MoveNode next;

	number++;
	next = MoveNode(Utils::PosToPoint(node->getPos()));

	m_MoveList.SetIndex(node->getNumber() - 1);
	pCurrentMove = m_MoveList.Current();

	pNextMove = GetVariant(pCurrentMove, next.getPos());

	if (pNextMove)
	{
		pCurrentMove = pNextMove;
		pCurrentMove->setIsMove(true);
	}
	else
	{
		pNextMove = new MoveNode(next);

		AddMove (pCurrentMove, pNextMove);
		pCurrentMove = pNextMove;

		nNewMoves++;
	}

	m_MoveList.Add(pCurrentMove);

	addComment(pCurrentMove, node->getOneLineComment(), node->getMultiLineComment());
	addBoardText(pCurrentMove, node->getBoardText());

	while (node->hasMoreChildren())
	{
		AddRenJS(node->nextChild(), number, pCurrentMove, nNewMoves);
	}
}

//------------------------------------------------------------------------

void CRenLibDoc::AddBuf(Buf& aBuf)
{
	int nTotalNewMoves = 0;
	int nTotal         = 0;

	MoveNode* pCurrentMove;
	MoveNode* pNextMove;

	while (!aBuf.Done())
	{
		if (aBuf.getGame().CheckMoves() != Game::VALID)
		{
			aBuf.Next();
			continue;
		}

		pCurrentMove = m_MoveList.GetRoot();

		int number = 0;
		int nNewMoves = 0;

		MoveNode next;

		while (aBuf.getGame().hasMoreMoves())
		{
			Game::Node* gameNode = aBuf.getGame().nextMove();

			number++;
			next = MoveNode(Utils::PosToPoint(gameNode->getPos()));

			pNextMove = GetVariant(pCurrentMove, next.getPos());

			if (pNextMove)
			{
				pCurrentMove = pNextMove;
				pCurrentMove->setIsMove(true);
			}
			else
			{
				pNextMove = new MoveNode(next);

				AddMove (pCurrentMove, pNextMove);
				pCurrentMove = pNextMove;

				nNewMoves++;
			}

			addComment(pCurrentMove, gameNode->getOneLineComment(), gameNode->getMultiLineComment());
		}

		nTotalNewMoves += nNewMoves;
		nTotal         += number;

		aBuf.Next();
	}

	m_Board.SetLastMove(Center);
	SetPreviousVariant();

	CString strInfo(Utils::GetString(IDS_ADDED_MOVES, nTotalNewMoves, nTotal));

	if (nTotalNewMoves)
	{
		SetModifiedFlag();
	}

	FindMoveNode(pCurrentMove);

	strInfo += Utils::GetString(IDS_INFO_FROM, aBuf.getFilePath());

	SetPaneText(strInfo);
}

//------------------------------------------------------------------------

void CRenLibDoc::AddWzq(Wzq& aWzq)
{
	if (aWzq.getGame().CheckMoves() != Game::VALID)
	{
		return;
	}

	int nTotalNewMoves = 0;
	int nTotal         = 0;

	MoveNode* pCurrentMove = m_MoveList.GetRoot();
	MoveNode* pNextMove;

	int number = 0;
	int nNewMoves = 0;

	MoveNode next;

	while (aWzq.getGame().hasMoreMoves())
	{
		Game::Node* gameNode = aWzq.getGame().nextMove();

		number++;
		next = MoveNode(Utils::PosToPoint(gameNode->getPos()));

		pNextMove = GetVariant(pCurrentMove, next.getPos());

		if (pNextMove)
		{
			pCurrentMove = pNextMove;
			pCurrentMove->setIsMove(true);

		}
		else
		{
			pNextMove = new MoveNode(next);

			AddMove (pCurrentMove, pNextMove);
			pCurrentMove = pNextMove;

			nNewMoves++;
		}
	}

	nTotalNewMoves += nNewMoves;
	nTotal         += number;

	addComment(pCurrentMove, aWzq.getComment(), strEmpty);

	m_Board.SetLastMove(Center);
	SetPreviousVariant();

	CString strInfo(Utils::GetString(IDS_ADDED_MOVES, nTotalNewMoves, nTotal));

	if (nTotalNewMoves)
	{
		SetModifiedFlag();
	}

	FindMoveNode(pCurrentMove);

	strInfo += Utils::GetString(IDS_INFO_FROM, aWzq.getFilePath());

	SetPaneText(strInfo);
}

//------------------------------------------------------------------------

void CRenLibDoc::AddPdb (Pdb& aPdb)
{
	int nTotalNewMoves = 0;
	int nTotal         = 0;

	MoveNode* pCurrentMove;
	MoveNode* pNextMove;

	while (!aPdb.Done())
	{
		if (!aPdb.Next())
		{
			continue;
		}
          
		if (aPdb.getGame().CheckMoves() != Game::VALID)
		{
			continue;
		}

		int number = 0;
		int nNewMoves = 0;

		MoveNode next;

		pCurrentMove = m_MoveList.GetRoot();

		while (aPdb.getGame().hasMoreMoves())
		{
			Game::Node* gameNode = aPdb.getGame().nextMove();

			number++;
			next = MoveNode(Utils::PosToPoint(gameNode->getPos()));

			pNextMove = GetVariant(pCurrentMove, next.getPos());

			if (pNextMove)
			{
				pCurrentMove = pNextMove;
				pCurrentMove->setIsMove(true);
			}
			else
			{
				pNextMove = new MoveNode(next);

				AddMove (pCurrentMove, pNextMove);
				pCurrentMove = pNextMove;

				nNewMoves++;
			}
		}

		nTotalNewMoves += nNewMoves;
		nTotal         += number;

		addComment(pCurrentMove, strEmpty, aPdb.getComment());
	}

	m_Board.SetLastMove(Center);
	SetPreviousVariant();

 	CString strInfo(Utils::GetString(IDS_ADDED_MOVES, nTotalNewMoves, nTotal));

	if (nTotalNewMoves)
	{
		SetModifiedFlag();
	}

	FindMoveNode(pCurrentMove);

	strInfo += Utils::GetString(IDS_INFO_FROM, aPdb.getFilePath());

	SetPaneText(strInfo);
}

//------------------------------------------------------------------------

void CRenLibDoc::AddPos(Pos& aPos)
{
	int nTotalNewMoves = 0;
	int nTotal         = 0;

	MoveNode* pCurrentMove = m_MoveList.GetRoot();
	MoveNode* pNextMove;

	int      number    = 0;
	int      nNewMoves = 0;
	MoveNode next;

	if (aPos.Read())
	{
		if (aPos.getGame().CheckMoves() != Game::VALID)
		{
			return;
		}

		while (aPos.getGame().hasMoreMoves())
		{
			Game::Node* gameNode = aPos.getGame().nextMove();

			number++;
			next = MoveNode(Utils::PosToPoint(gameNode->getPos()));

			pNextMove = GetVariant(pCurrentMove, next.getPos());

			if (pNextMove)
			{
				pCurrentMove = pNextMove;
				pCurrentMove->setIsMove(true);
			}
			else
			{
				pNextMove = new MoveNode(next);

				AddMove (pCurrentMove, pNextMove);
				pCurrentMove = pNextMove;

				nNewMoves++;
			}

			addComment(pCurrentMove, gameNode->getOneLineComment(), gameNode->getMultiLineComment());
		}

		nTotalNewMoves += nNewMoves;
		nTotal         += number;
	}

	m_Board.SetLastMove(Center);
	SetPreviousVariant();

	CString strInfo(Utils::GetString(IDS_ADDED_MOVES, nTotalNewMoves, nTotal));

	if (nTotalNewMoves)
	{
		SetModifiedFlag();
	}

	FindMoveNode(pCurrentMove);

	strInfo += Utils::GetString(IDS_INFO_FROM, aPos.m_strFilePath);

	SetPaneText(strInfo);
}

//------------------------------------------------------------------------

void CRenLibDoc::AddSgf(Sgf& aSgf)
{
	if (aSgf.getGame().CheckMoves() != Game::VALID)
	{
		return;
	}

	int nTotalNewMoves = 0;
	int nTotal         = 0;

	MoveNode* pCurrentMove = m_MoveList.GetRoot();
	MoveNode* pNextMove;

	int number = 0;
	int nNewMoves = 0;

	MoveNode next;

	while (aSgf.getGame().hasMoreMoves())
	{
		Game::Node* gameNode = aSgf.getGame().nextMove();

		number++;
		next = MoveNode(Utils::PosToPoint(gameNode->getPos()));

		pNextMove = GetVariant(pCurrentMove, next.getPos());

		if (pNextMove)
		{
			pCurrentMove = pNextMove;
			pCurrentMove->setIsMove(true);
		}
		else
		{
			pNextMove = new MoveNode(next);

			AddMove (pCurrentMove, pNextMove);
			pCurrentMove = pNextMove;

			nNewMoves++;
		}

		addComment(pCurrentMove, gameNode->getOneLineComment(), gameNode->getMultiLineComment());
	}

	nTotalNewMoves += nNewMoves;
	nTotal         += number;

	m_Board.SetLastMove(Center);
	SetPreviousVariant();

	CString strInfo(Utils::GetString(IDS_ADDED_MOVES, nTotalNewMoves, nTotal));

	if (nTotalNewMoves)
	{
		SetModifiedFlag();
	}

	FindMoveNode(pCurrentMove);

	strInfo += Utils::GetString(IDS_INFO_FROM, aSgf.getFilePath());

	SetPaneText(strInfo);
}

//------------------------------------------------------------------------

void CRenLibDoc::AddBdt(Bdt& aBdt)
{
	int nTotalNewMoves = 0;
	int nTotal         = 0;

	MoveNode* pCurrentMove;
	MoveNode* pNextMove;

	while (!aBdt.Done())
	{
		if (aBdt.getGame().CheckMoves() != Game::VALID)
		{
			aBdt.Next();
			continue;
		}

		pCurrentMove = m_MoveList.GetRoot();

		int number = 0;
		int nNewMoves = 0;

		MoveNode next;

		while (aBdt.getGame().hasMoreMoves())
		{
			Game::Node* gameNode = aBdt.getGame().nextMove();

			number++;
			next = MoveNode(Utils::PosToPoint(gameNode->getPos()));

			pNextMove = GetVariant(pCurrentMove, next.getPos());

			if (pNextMove)
			{
				pCurrentMove = pNextMove;
				pCurrentMove->setIsMove(true);
			}
			else
			{
				pNextMove = new MoveNode(next);

				AddMove (pCurrentMove, pNextMove);
				pCurrentMove = pNextMove;

				nNewMoves++;
			}

			addComment(pCurrentMove, gameNode->getOneLineComment(), gameNode->getMultiLineComment());
		}

		nTotalNewMoves += nNewMoves;
		nTotal         += number;

		aBdt.Next();
	}

	m_Board.SetLastMove(Center);
	SetPreviousVariant();

	CString strInfo(Utils::GetString(IDS_ADDED_MOVES, nTotalNewMoves, nTotal));

	if (nTotalNewMoves)
	{
		SetModifiedFlag();
	}

	FindMoveNode(pCurrentMove);

	strInfo += Utils::GetString(IDS_INFO_FROM, aBdt.getFilePath());

	SetPaneText(strInfo);
}

//------------------------------------------------------------------------

void CRenLibDoc::AddRdf(Rdf& aRdf)
{
	if (aRdf.getGame().CheckMoves() != Game::VALID)
	{
		return;
	}

	int nTotalNewMoves = 0;
	int nTotal         = 0;

	MoveNode* pCurrentMove = m_MoveList.GetRoot();
	MoveNode* pNextMove;

	int number = 0;
	int nNewMoves = 0;

	MoveNode next;

	while (aRdf.getGame().hasMoreMoves())
	{
		Game::Node* gameNode = aRdf.getGame().nextMove();

		number++;
		next = MoveNode(Utils::PosToPoint(gameNode->getPos()));

		pNextMove = GetVariant(pCurrentMove, next.getPos());

		if (pNextMove)
		{
			pCurrentMove = pNextMove;
			pCurrentMove->setIsMove(true);
		}
		else
		{
			pNextMove = new MoveNode(next);

			AddMove (pCurrentMove, pNextMove);
			pCurrentMove = pNextMove;

			nNewMoves++;
		}

		addComment(pCurrentMove, gameNode->getOneLineComment(), gameNode->getMultiLineComment());
	}

	nTotalNewMoves += nNewMoves;
	nTotal         += number;

	m_Board.SetLastMove(Center);
	SetPreviousVariant();

	CString strInfo(Utils::GetString(IDS_ADDED_MOVES, nTotalNewMoves, nTotal));

	if (nTotalNewMoves)
	{
		SetModifiedFlag();
	}

	FindMoveNode(pCurrentMove);

	strInfo += Utils::GetString(IDS_INFO_FROM, aRdf.getFilePath());

	SetPaneText(strInfo);
}

//------------------------------------------------------------------------

void CRenLibDoc::SetTitle()
{
	CString Title;

	if (m_strTitle.IsEmpty())
	{
		Title += Utils::GetString(IDS_NO_LIBRARY);
	}
	else
	{
		Title += m_strTitle;

		if (IsModified())
		{
			Title += "*";
		}
	}

	Title += " - RenLib " + getProgramVersion();

	AfxGetApp()->m_pMainWnd->SetWindowText(Title);
}

//------------------------------------------------------------------------

void CRenLibDoc::OnCloseDocument() 
{
	SaveCommand(true, false, false); // Yes, No buttons only

	if (!m_MoveList.IsEmpty())
	{
		DeleteTree(m_MoveList.GetRoot());
	}

	CDocument::OnCloseDocument();
}

//------------------------------------------------------------------------

void CRenLibDoc::WriteComment (LibraryFile& libFile, CString& strOneLine, CString& strMultiLine)
{
	CString strNew(strOneLine);

	if (strMultiLine.GetLength() > 0)
	{
		strNew += TCHAR(8) + strMultiLine;
	}

	WriteText (libFile, strNew);
}

//------------------------------------------------------------------------

void CRenLibDoc::WriteBoardText (LibraryFile& libFile, CString& strBoardText)
{
	WriteText (libFile, strBoardText);
}

//------------------------------------------------------------------------

void CRenLibDoc::WriteText (LibraryFile& libFile, CString& strText)
{
	CString strNew(strText + TCHAR(0));

	if (strNew.GetLength() % 2)
	{
		strNew += TCHAR(0);
	}

	libFile.Put(strNew);
}

//------------------------------------------------------------------------

void CRenLibDoc::WriteNode(LibraryFile& libFile, MoveNode* pMove, bool FullTree,
                           int& nMoves, int& nComments, int& nBoardTexts, int& nMarks)
{
	if (pMove)
	{
		// ERROR checking code
		const CPoint Point(pMove->getPos());

		if ((Point.x != 0 || Point.y != 0) && (Point.x < 1 || Point.x > 15 || Point.y < 1 || Point.y > 15))
		{
			CString strMessage;
			strMessage += Utils::GetString(IDS_MSG_POSITION, pMove->getPos());
			strMessage += "\n\n";
			strMessage += Utils::GetString(IDS_MSG_REPORT);
			strMessage += "\n\n";
			strMessage += Utils::GetString(IDS_MSG_NOTE);
			Utils::ShowMessage(strMessage, Utils::GetString(IDS_CAP_WRITE), MB_ICONERROR);

			pMove->setPos(CPoint(1,1));
			pMove->setIsMark(true);
			SetModifiedFlag();
		}

		bool rightBit = FullTree;

		// set down bit
		pMove->setIsDown(pMove->getRight() != 0);

		// set right bit
		pMove->setIsRight(rightBit && !pMove->getDown());

		// write data
		libFile.Put(*pMove);

		if (pMove->isMove())
		{
			nMoves++;
		}

		// write comment
		if (pMove->isNewComment())
		{
			WriteComment (libFile, pMove->getOneLineComment(), pMove->getMultiLineComment());
			nComments++;
		}

		// write comment
		if (pMove->isBoardText())
		{
			WriteBoardText (libFile, pMove->getBoardText());
			nBoardTexts++;
		}

		if (pMove->isMark())
		{
			nMarks++;
		}
	}
}

//------------------------------------------------------------------------

void CRenLibDoc::WriteMove(LibraryFile& libFile, int MoveNo,
                           int& nMoves, int& nComments, int& nBoardTexts, int& nMarks, bool isOldFormat)
{
	Stack m_Stack;

	int       start = 0;
	MoveNode* pMove = 0;

	if (isOldFormat)
	{
		start  = 1;

		if (MoveNo == 0)
		{
			MoveNo = 1;
		}
	}

	if (MoveNo >= 0 && MoveNo <= m_MoveList.Index())
	{
		pMove = m_MoveList.Get(MoveNo)->getDown();
	}

	bool done = (pMove == 0);

	for (int i=start; i <= MoveNo; i++)
	{
		MoveNode* pMove = m_MoveList.Get(i);

		WriteNode(libFile, pMove, false, nMoves, nComments, nBoardTexts, nMarks);
	}

	while (!done)
	{
		if (pMove)
		{
			WriteNode(libFile, pMove, true, nMoves, nComments, nBoardTexts, nMarks);

			if (pMove->getRight())
			{
				m_Stack.Push(pMove->getRight());
			}

			pMove = pMove->getDown();
		}
		else if (!m_Stack.IsEmpty())
		{
			m_Stack.Pop(pMove);
		}
		else
		{
			done = true;
		}
	}
}

//------------------------------------------------------------------------

void CRenLibDoc::WriteLibrary(const CString& strFile, int MoveNo)
{
	CWaitCursor cursor;

	int nMoves      = 0;
	int nComments   = 0;
	int nBoardTexts = 0;
	int nMarks      = 0;

	LibraryFile libFile;

	if ( !libFile.OpenWrite(strFile))
	{
		CString strMessage(Utils::GetString(IDS_MSG_WRITE, strFile));
		Utils::ShowMessage(strMessage, Utils::GetString(IDS_CAP_WRITE), MB_ICONERROR);
		return;
	}

	bool isOldFormat = isFirstMoveH8Only(MoveNo) && !hasPassMove(MoveNo);

	libFile.WriteHeader(isOldFormat);

	removeEmptyBoardTexts();

	WriteMove(libFile, MoveNo, nMoves, nComments, nBoardTexts, nMarks, isOldFormat);

	CString strInfo;
	strInfo =  Utils::GetString(IDS_SAVED_MOVES, nMoves);
	strInfo += Utils::GetString(IDS_INFO_COMMENTS, nComments);
	strInfo += Utils::GetString(IDS_INFO_BOARD_TEXTS, nBoardTexts);
	strInfo += Utils::GetString(IDS_INFO_MARKS, nMarks);
	strInfo += Utils::GetString(IDS_INFO_IN, libFile.GetFilePath());

	SetPaneText(strInfo);

	libFile.Close();
}

//------------------------------------------------------------------------

void CRenLibDoc::MarkCommand ()
{
	if (!m_MoveList.IsEmpty())
	{
		MoveNode* pMove = m_MoveList.Current();

		pMove->setIsMark(!pMove->isMark());

		SetModifiedFlag();
	}
}

//------------------------------------------------------------------------

int CRenLibDoc::GetCurrentMove()
{
	return m_MoveList.Index();
}

//------------------------------------------------------------------------

void CRenLibDoc::SetMode(Mode mode)
{
    m_eMode = mode;

    if (m_eMode == PROBLEM_MODE)
    {
        DownCommand();
    }
}

//------------------------------------------------------------------------

bool CRenLibDoc::IsMarkFound(MoveNode* pMove)
{
	return pMove->isMove() && pMove->isMark();
}

//------------------------------------------------------------------------

bool CRenLibDoc::FindMarkCommand()
{
	Stack m_Stack;

	MoveList LocalMoveList = m_MoveList;

	MoveNode* pMove = LocalMoveList.Current();

	bool isFound       = false;
	bool done          = false;

	m_SearchList.Empty();
	m_SearchList.SetStart(SearchItem(pMove));
	m_SearchList.SetInfo(Utils::GetString(IDS_MARK), SearchList::MARK);

	if (pMove)
	{
		pMove = pMove->getDown();
	}

	while (!done)
	{
		if (pMove)
		{
			LocalMoveList.Add(pMove);

			if (IsMarkFound(pMove))
			{
				m_SearchList.Add(SearchItem(pMove));
			}

			if (pMove->getRight())
			{
				m_Stack.Push(LocalMoveList.Index(), pMove->getRight());
			}

			pMove = pMove->getDown();

		}
		else if (!m_Stack.IsEmpty())
		{
			int nMove;
			m_Stack.Pop(nMove, pMove);
			LocalMoveList.SetIndex(nMove - 1);
		}
		else
		{
			done = true;
		}
	}

	isFound = !m_SearchList.IsEmpty();
	mIsFindInProgress = isFound;

	SetFindInfo(isFound);

	return isFound;
}

//------------------------------------------------------------------------

void CRenLibDoc::SetFindInfo(SearchList& searchList, bool isFound)
{
	CString strInfo;

	if (isFound)
	{
		strInfo = Utils::GetString(IDS_MSG_FIND, 
			                       searchList.GetInfo(),
			                       searchList.CurrentIndex(),
			                       searchList.LastItem());
	}
	else
	{
		strInfo = Utils::GetString(IDS_MSG_FIND_NOT_FOUND, 
			                       searchList.GetInfo());
	}

	SetPaneText(strInfo);
}

//------------------------------------------------------------------------

void CRenLibDoc::SetFindInfo(bool isFound)
{
	if (m_SearchList.HasStart())
	{
		SetFindInfo(m_SearchList, isFound);
	}
	else
	{
		SetFindInfo(m_AutoSearchList, isFound);
	}
}

//------------------------------------------------------------------------

bool CRenLibDoc::FindNextPrevious(SearchList& searchList, bool isNext)
{
	MoveNode* pMove;

	bool isFound = false;

	mIsFindInProgress = !searchList.IsEmpty();

	while (mIsFindInProgress && !isFound)
	{
		if (searchList.IsEmpty())
		{
			searchList.Empty();

			pMove             = 0;
			mIsFindInProgress = false;
			isFound           = false;
		}
		else if (isNext)
		{
			pMove = searchList.Next()->getMoveNode();
		}
		else
		{
			pMove = searchList.Previous()->getMoveNode();
		}

		if (pMove != 0)
		{
			if (FindMoveNode(pMove))
			{
				if (searchList.CurrentIndex() == 0)
				{
					if (searchList.GetKind() == SearchList::MERGE_COMMENT)
					{
						searchList.Clear();
					}
					else
					{
						isFound = true;
						m_Board.SetPreviousVariant(m_MoveList.Current()->getPos());
					}
				}
				else
				{
					switch (searchList.GetKind())
					{
						case SearchList::MARK:
						{
							isFound = IsMarkFound(pMove);
							break;
						}

						case SearchList::COMMENT:
						{
							isFound = IsCommentFound(pMove);
							break;
						}

						case SearchList::BOARD_TEXT:
						{
							isFound = IsBoardTextFound(pMove);

							if (isFound)
							{
								moveBack();
								m_Board.SetPreviousVariant(pMove->getPos());
							}
							break;
						}

						case SearchList::MOVE:
						case SearchList::SIMILAR_POSITION:
						case SearchList::SAME_POSITION:
						case SearchList::MERGE_COMMENT:
						{
							isFound = true; // Always true
							break;
						}

						default:
						{
							isFound = false;
							break;
						}
					}
				}
			}
			else
			{
				searchList.Clear();
			}
		}
	}

	isFound = isFound && !searchList.IsEmpty();

	SetFindInfo(isFound);

	if (!isFound && searchList.HasStart())
	{
		FindMoveNode(searchList.GetStart()->getMoveNode());
	}

	return isFound;
}

//------------------------------------------------------------------------

bool CRenLibDoc::FindNextPrevious(bool isNext)
{
	bool isFound = false;

	mIsNext = isNext;

	EmptyPaneText();

	isFound = FindNextPrevious(m_SearchList, isNext);

	if (!isFound && !m_AutoSearchList.IsEmpty())
	{
		isFound = FindNextPrevious(m_AutoSearchList, isNext);
	}

	return isFound;
}

//------------------------------------------------------------------------

bool CRenLibDoc::FindNextCommand()
{
	return FindNextPrevious(true);
}

//------------------------------------------------------------------------

bool CRenLibDoc::FindPreviousCommand()
{
	return FindNextPrevious(false);
}

//------------------------------------------------------------------------

bool CRenLibDoc::IsMoveFound(MoveNode* pMove, MoveList& theMoveList)
{
	return pMove->isMove() &&
		   (m_MoveNoToFind == theMoveList.Index() ||
		   (m_MoveNoToFind > 225 && NoOfVariantMoves(pMove) == 0));
}

//------------------------------------------------------------------------

bool CRenLibDoc::FindMoveCommand (int MoveNo)
{
	MoveList LocalMoveList = m_MoveList;

	Stack m_Stack;

	MoveNode* pMove = LocalMoveList.Current();

	bool isFound       = false;
	bool done          = false;

	m_MoveNoToFind     = MoveNo;

	m_SearchList.Empty();
	m_SearchList.SetStart(SearchItem(pMove));
	m_SearchList.SetInfo(Utils::GetString(IDS_MOVE), SearchList::MOVE);

	if (pMove)
	{
		pMove = pMove->getDown();
	}

	while (!done)
	{
		if (pMove)
		{
			LocalMoveList.Add(pMove);

			if (IsMoveFound(pMove, LocalMoveList))
			{
				m_SearchList.Add(SearchItem(pMove));
			}

			if (pMove->getRight())
			{
				m_Stack.Push(LocalMoveList.Index(), pMove->getRight());
			}

			if (LocalMoveList.Index() < MoveNo)
			{
				pMove = pMove->getDown();
			}
			else
			{
				pMove = 0;
			}
		}
		else if (!m_Stack.IsEmpty())
		{
			int nMove;
			m_Stack.Pop(nMove, pMove);
			LocalMoveList.SetIndex(nMove - 1);
		}
		else
		{
			done = true;
		}
	}

	isFound = !m_SearchList.IsEmpty();
	mIsFindInProgress = isFound;

	SetFindInfo(isFound);

	return isFound;
}

//------------------------------------------------------------------------

void CRenLibDoc::MakeMoveList(Position::MoveList& theMoves, MoveList& theMoveList)
{
	theMoves.nMoves = theMoveList.Index();

	for (int i=1; i <= theMoveList.Index(); i++)
	{
		theMoves.Moves[i-1] = theMoveList.Get(i)->getPos();
	}
}

//------------------------------------------------------------------------

bool CRenLibDoc::FindSamePositionCommand()
{
	FindVariantPositionCommand();
	return FindPosition(false);
}

//------------------------------------------------------------------------

bool CRenLibDoc::autoFindSamePositionCommand()
{
	FindVariantPositionCommand();

	if (m_MoveList.IsEmpty())
	{
		return false;
	}

	MoveList LocalMoveList = m_MoveList;

	Stack m_Stack;

	const int MoveNo = LocalMoveList.Index();

	Position::MoveList theMoves;

	// Current Position
	MakeMoveList(theMoves, LocalMoveList);
	m_Position.Init(theMoves, false);

	MoveNode* pMove = LocalMoveList.Current();

	bool isFound       = false;
	bool done          = false;

	m_MoveNoToFind     = MoveNo;

	m_AutoSearchList.Empty();
	m_AutoSearchList.SetStart(SearchItem(pMove));
	m_AutoSearchList.SetInfo(Utils::GetString(IDS_SAME_POSITION), SearchList::SAME_POSITION);

	LocalMoveList.SetRootIndex();
	pMove = LocalMoveList.Current();

	if (pMove)
	{
		pMove = pMove->getDown();
	}

	while (!done)
	{
		if (pMove)
		{
			LocalMoveList.Add(pMove);

			if (pMove->isMove() && LocalMoveList.Index() == MoveNo)
			{
				// Search Position
				MakeMoveList(theMoves, LocalMoveList);

				const int nMatch = m_Position.Match(theMoves, false);

				if (nMatch)
				{
					pMove->setMatch(nMatch);
					m_AutoSearchList.Add(SearchItem(pMove));
				}
			}

			if (pMove->getRight())
			{
				m_Stack.Push(LocalMoveList.Index(), pMove->getRight());
			}

			if (LocalMoveList.Index() < MoveNo)
			{
				pMove = pMove->getDown();
			}
			else
			{
				pMove = 0;
			}
		}
		else if (!m_Stack.IsEmpty())
		{
			int nMove;
			m_Stack.Pop(nMove, pMove);
			LocalMoveList.SetIndex(nMove - 1);
		}
		else
		{
			done = true;
		}
	}

	isFound = !m_AutoSearchList.IsEmpty();

	mIsFindInProgress = (mIsFindInProgress || isFound);

	SetFindInfo(isFound);

	return isFound;
}

//------------------------------------------------------------------------

void CRenLibDoc::ClearAutoFindSamePositionList()
{
	if (m_AutoSearchList.GetKind() == SearchList::SAME_POSITION)
	{
		m_AutoSearchList.Clear();
	}
}

//------------------------------------------------------------------------

bool CRenLibDoc::FindSimilarPositionCommand()
{
	return FindPosition(true);
}

//------------------------------------------------------------------------

bool CRenLibDoc::FindPosition(bool findSimilar)
{
	if (m_MoveList.IsEmpty())
	{
		return false;
	}

	MoveList LocalMoveList = m_MoveList;

	Stack m_Stack;

	const int MoveNo = LocalMoveList.Index();

	Position::MoveList theMoves;

	// Current Position
	MakeMoveList(theMoves, LocalMoveList);
	m_Position.Init(theMoves, findSimilar);

	MoveNode* pMove = LocalMoveList.Current();

	bool isFound       = false;
	bool done          = false;

	m_MoveNoToFind     = MoveNo;

	m_SearchList.Empty();
	m_SearchList.SetStart(SearchItem(pMove));

	if (findSimilar)
	{
		m_SearchList.SetInfo(Utils::GetString(IDS_SIMILAR_POSITION), SearchList::SIMILAR_POSITION);
	}
	else
	{
		m_SearchList.SetInfo(Utils::GetString(IDS_SAME_POSITION), SearchList::SAME_POSITION);
	}

	LocalMoveList.SetRootIndex();
	pMove = LocalMoveList.Current();

	if (pMove)
	{
		pMove = pMove->getDown();
	}

	while (!done)
	{
		if (pMove)
		{
			LocalMoveList.Add(pMove);

			if (pMove->isMove() && LocalMoveList.Index() == MoveNo)
			{
				// Search Position
				MakeMoveList(theMoves, LocalMoveList);

				const int nMatch = m_Position.Match(theMoves, findSimilar);

				if (nMatch)
				{
					pMove->setMatch(nMatch);
					m_SearchList.Add(SearchItem(pMove));
				}
			}

			if (pMove->getRight())
			{
				m_Stack.Push(LocalMoveList.Index(), pMove->getRight());
			}

			if (LocalMoveList.Index() < MoveNo)
			{
				pMove = pMove->getDown();
			}
			else
			{
				pMove = 0;
			}
		}
		else if (!m_Stack.IsEmpty())
		{
			int nMove;
			m_Stack.Pop(nMove, pMove);
			LocalMoveList.SetIndex(nMove - 1);
		}
		else
		{
			done = true;
		}
	}

	isFound = !m_SearchList.IsEmpty();
	mIsFindInProgress = isFound;

	SetFindInfo(isFound);

	return isFound;
}

//------------------------------------------------------------------------

void CRenLibDoc::FindVariantPositionCommand()
{
	m_VariantList.Empty();

	if (m_MoveList.Index() < 3)
	{
		return;
	}

	MoveList LocalMoveList = m_MoveList;

	Stack m_Stack;

	const int MoveNo = LocalMoveList.Index() + 1;

	Position::MoveList theMoves;

	// Current Position
	MakeMoveList(theMoves, LocalMoveList);
	m_Position.Init(theMoves, false);

	MoveNode* pMove = LocalMoveList.Current();

	bool done          = false;

	m_MoveNoToFind     = MoveNo;

	m_VariantList.SetStart(SearchItem(pMove));

	m_VariantList.SetInfo(Utils::GetString(IDS_VARIANT_POSITION), SearchList::SIMILAR_POSITION);

	LocalMoveList.SetRootIndex();
	pMove = LocalMoveList.Current();

	if (pMove)
	{
		pMove = pMove->getDown();
	}

	while (!done)
	{
		if (pMove)
		{
			LocalMoveList.Add(pMove);

			if (LocalMoveList.Index() == MoveNo)
			{
				MoveList tempMoveList = LocalMoveList;
				tempMoveList.Swap(MoveNo, MoveNo-2);
				MoveNode* pVariant = tempMoveList.Current();
				tempMoveList.Decrement();

				// Search Position
				MakeMoveList(theMoves, tempMoveList);

				const int nMatch = m_Position.Match(theMoves, false);

				if (nMatch)
				{
					m_VariantList.Add(SearchItem(pVariant, nMatch), SearchItem(pMove));
				}
			}

			if (pMove->getRight())
			{
				m_Stack.Push(LocalMoveList.Index(), pMove->getRight());
			}

			if (LocalMoveList.Index() < MoveNo)
			{
				pMove = pMove->getDown();
			}
			else
			{
				pMove = 0;
			}
		}
		else if (!m_Stack.IsEmpty())
		{
			int nMove;
			m_Stack.Pop(nMove, pMove);
			LocalMoveList.SetIndex(nMove - 1);
		}
		else
		{
			done = true;
		}
	}
}

//------------------------------------------------------------------------

bool CRenLibDoc::IsCommentFound(MoveNode* pMove)
{
	bool isFound = false;

	if (pMove->isMove())
	{
		if (m_strCommentToFind.IsEmpty())
		{
			isFound = pMove->isOneLineComment() || pMove->isMultiLineComment();
		}
		else
		{
			CString strOneLine(pMove->getOneLineComment());
			CString strMultiLine(pMove->getMultiLineComment());
			strOneLine.MakeUpper();
			strMultiLine.MakeUpper();

			isFound = Utils::IsSubstring(strOneLine,   m_strCommentToFind) ||
				      Utils::IsSubstring(strMultiLine, m_strCommentToFind);
		}
	}

	return isFound;
}

//------------------------------------------------------------------------

bool CRenLibDoc::FindCommentCommand(const CString& strCommentToFind)
{
	MoveList LocalMoveList = m_MoveList;

	Stack m_Stack;

	MoveNode* pMove = LocalMoveList.Current();

	bool isFound       = false;
	bool done          = false;

	m_strCommentToFind = strCommentToFind;

	m_SearchList.Empty();
	m_SearchList.SetStart(SearchItem(pMove));
	m_SearchList.SetInfo(Utils::GetString(IDS_COMMENT), SearchList::COMMENT);

	if (pMove)
	{
		pMove = pMove->getDown();
	}

	while (!done)
	{
		if (pMove)
		{
			LocalMoveList.Add(pMove);

			if (IsCommentFound(pMove))
			{
				m_SearchList.Add(SearchItem(pMove));
			}

			if (pMove->getRight())
			{
				m_Stack.Push(LocalMoveList.Index(), pMove->getRight());
			}

			pMove = pMove->getDown();

		}
		else if (!m_Stack.IsEmpty())
		{
			int nMove;
			m_Stack.Pop(nMove, pMove);
			LocalMoveList.SetIndex(nMove - 1);
		}
		else
		{
			done = true;
		}
	}

	isFound = !m_SearchList.IsEmpty();
	mIsFindInProgress = isFound;

	SetFindInfo(isFound);

	return isFound;
}

//------------------------------------------------------------------------

bool CRenLibDoc::IsBoardTextFound(MoveNode* pMove)
{
	if (m_strBoardTextToFind.IsEmpty())
	{
		return pMove->isBoardText();
	}
	else
	{
		CString strBoardText(pMove->getBoardText());
		strBoardText.MakeUpper();

		return Utils::IsSubstring(strBoardText, m_strBoardTextToFind);
	}
}

//------------------------------------------------------------------------

bool CRenLibDoc::FindBoardTextCommand(const CString& strBoardTextToFind)
{
	MoveList LocalMoveList = m_MoveList;

	Stack m_Stack;

	MoveNode* pMove = LocalMoveList.Current();

	bool isFound       = false;
	bool done          = false;

	m_strBoardTextToFind = strBoardTextToFind;

	m_SearchList.Empty();
	m_SearchList.SetStart(SearchItem(pMove));
	m_SearchList.SetInfo(Utils::GetString(IDS_BOARD_TEXT), SearchList::BOARD_TEXT);

	if (pMove)
	{
		pMove = pMove->getDown();
	}

	while (!done)
	{
		if (pMove)
		{
			LocalMoveList.Add(pMove);

			if (IsBoardTextFound(pMove))
			{
				m_SearchList.Add(SearchItem(pMove));
			}

			if (pMove->getRight())
			{
				m_Stack.Push(LocalMoveList.Index(), pMove->getRight());
			}

			pMove = pMove->getDown();

		}
		else if (!m_Stack.IsEmpty())
		{
			int nMove;
			m_Stack.Pop(nMove, pMove);
			LocalMoveList.SetIndex(nMove - 1);
		}
		else
		{
			done = true;
		}
	}

	isFound = !m_SearchList.IsEmpty();
	mIsFindInProgress = isFound;

	SetFindInfo(isFound);

	return isFound;
}

//------------------------------------------------------------------------

bool CRenLibDoc::IsFindInProgress()
{
	return mIsFindInProgress;
}

//------------------------------------------------------------------------

BOOL CRenLibDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (isFindActive())
	{
		m_Board.stopFind();
	}

	BOOL success = FALSE; // Remove from recent list

	EmptyPaneText();

	if (!SaveCommand(true))
	{
		return TRUE; // Cancel
	}

	initMergeComment();

	CString strFile(lpszPathName);
	CString strTitle(Utils::GetTitle(lpszPathName));

	CString strBackup(lpszPathName);
	Utils::MakeExtensionBak(strBackup);

	CString strMessage;

	if (Utils::IsExtensionLib(lpszPathName))
	{
		if (OpenLibraryCommand(strFile, strTitle, strBackup))
		{
			success = TRUE;

			if (!FindStartPositionCommand(false))
			{
				DownCommand();
			}

			AfxGetApp()->AddToRecentFileList(strFile);
		}
		else
		{
			strMessage = Utils::GetString(IDS_MSG_OPEN_FILE, strFile);
		}
	}
	else if (Utils::IsExtensionTxt(lpszPathName))
	{
		NewLibraryCommand();

		Game aGame;
		const bool multipleGames = false;

        if (aGame.open(strFile, multipleGames))
        {
            Game::Result result = aGame.addGame();

		    if (result == Game::VALID)
		    {
			    success = TRUE;
			    AddGame(aGame);
			    AfxGetApp()->AddToRecentFileList(strFile);
		    }

            aGame.close();
        }
		else
		{
			strMessage = Utils::GetString(IDS_MSG_OPEN_FILE, strFile);
		}
	}
	else if (Utils::IsExtensionWzq(lpszPathName))
	{
		NewLibraryCommand();

		Wzq aWzq;

		if (aWzq.OpenFile(strFile))
		{
			success = TRUE;
			AddWzq(aWzq);
			AfxGetApp()->AddToRecentFileList(strFile);
		}
		else
		{
			strMessage = Utils::GetString(IDS_MSG_OPEN_FILE, strFile);
		}
	}
	else if (Utils::IsExtensionBuf(lpszPathName))
	{
		NewLibraryCommand();

		Buf	aBuf;

		if (aBuf.OpenFile(strFile))
		{
			success = TRUE;
			AddBuf(aBuf);
			AfxGetApp()->AddToRecentFileList(strFile);
		}
		else
		{
			strMessage = Utils::GetString(IDS_MSG_OPEN_FILE, strFile);
		}
	}
    else if (Utils::IsExtensionPdb(lpszPathName))
    {
		NewLibraryCommand();

		Pdb	aPdb;

		if (aPdb.OpenFile(strFile))
		{
			success = TRUE;
			AddPdb(aPdb);              
			AfxGetApp()->AddToRecentFileList(strFile);
		}
		else
		{
			strMessage = Utils::GetString(IDS_MSG_OPEN_FILE, strFile);
		}
	}
	else if (Utils::IsExtensionPos(lpszPathName))
	{
		NewLibraryCommand();

		Pos aPos;

		if (aPos.Open(strFile))
		{
			success = TRUE;
			AddPos(aPos);
			AfxGetApp()->AddToRecentFileList(strFile);
		}
		else
		{
			strMessage = Utils::GetString(IDS_MSG_OPEN_FILE, strFile);
		}
	}
	else if (Utils::IsExtensionRenjs(lpszPathName))
	{
		NewLibraryCommand();

		RenJS renjs;

		if (renjs.addGame(strFile) == RenJS::VALID)
		{
			success = TRUE;
			AddRenJS(renjs);              
			AfxGetApp()->AddToRecentFileList(strFile);
		}
		else
		{
			strMessage = Utils::GetString(IDS_MSG_OPEN_FILE, strFile);
		}
	}
	else if (Utils::IsExtensionSgf(lpszPathName))
	{
		NewLibraryCommand();

		Sgf aSgf;
		const bool multipleGames = false;

		if (aSgf.OpenFile(strFile, multipleGames))
		{
			success = TRUE;
			AddSgf(aSgf);
			AfxGetApp()->AddToRecentFileList(strFile);
		}
		else
		{
			strMessage = Utils::GetString(IDS_MSG_OPEN_FILE, strFile);
		}
	}
	else if (Utils::IsExtensionHtml(lpszPathName))
	{
		m_strLastFile = lpszPathName;
		AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_OPEN_WEB_PAGE);
	}
	else if (Utils::IsExtensionBdt(lpszPathName))
	{
		NewLibraryCommand();

		Bdt	aBdt;

		if (aBdt.OpenFile(strFile))
		{
			success = TRUE;
			AddBdt(aBdt);
			AfxGetApp()->AddToRecentFileList(strFile);
		}
		else
		{
			strMessage = Utils::GetString(IDS_MSG_OPEN_FILE, strFile);
		}
	}
	else if (Utils::IsExtensionRdf(lpszPathName))
	{
		NewLibraryCommand();

		Rdf	aRdf;

		if (aRdf.OpenFile(strFile))
		{
			success = TRUE;
			AddRdf(aRdf);
			AfxGetApp()->AddToRecentFileList(strFile);
		}
		else
		{
			strMessage = Utils::GetString(IDS_MSG_OPEN_FILE, strFile);
		}
	}
	else
	{
		strMessage += Utils::GetString(IDS_MSG_OPEN_FILE, strFile);
		strMessage += "\n\n";
		strMessage += Utils::GetString(IDS_FILE_EXTENSIONS);
		strMessage += "\n";
		strMessage += Utils::GetString(IDS_EXT_BDT);
		strMessage += "\n";
		strMessage += Utils::GetString(IDS_EXT_BUF);
		strMessage += "\n";
		strMessage += Utils::GetString(IDS_EXT_HTML);
		strMessage += "\n";
		strMessage += Utils::GetString(IDS_EXT_LIB);
		strMessage += "\n";
		strMessage += Utils::GetString(IDS_EXT_PDB);
        strMessage += "\n";
        strMessage += Utils::GetString(IDS_EXT_POS);
		strMessage += "\n";
		strMessage += Utils::GetString(IDS_EXT_RDF);
		strMessage += "\n";
		strMessage += Utils::GetString(IDS_EXT_RENJS);
		strMessage += "\n";
		strMessage += Utils::GetString(IDS_EXT_SGF);
		strMessage += "\n";
		strMessage += Utils::GetString(IDS_EXT_TXT);
		strMessage += "\n";
		strMessage += Utils::GetString(IDS_EXT_WZQ);
	}

	if (strMessage.IsEmpty())
	{
		const CWnd* mainWindow = AfxGetMainWnd();

		if (mainWindow != 0)
		{
			// The window is not yet created when the program is activated
			// by double clicking a file associated with RenLib
			AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_SHOW_COMMENT_TAB);
		}
	}
	else
	{
		Utils::ShowMessage(strMessage, Utils::GetString(IDS_CAP_OPEN), MB_ICONERROR);
	}

	return success;
}

//------------------------------------------------------------------------

void CRenLibDoc::StartPositionCommand()
{
	if (!m_MoveList.IsEmpty())
	{
		MoveNode* pMove = m_MoveList.Current();

		const bool isStart = pMove->isStart();

		ClearStartPosition(m_MoveList.GetRoot());

		//
		// Toggle start position
		//
		pMove->setIsStart(!isStart);

		SetModifiedFlag();
	}
}

//------------------------------------------------------------------------

bool CRenLibDoc::FindStartPositionCommand(bool Notify)
{
	bool isFound = false;

	if (!m_MoveList.IsEmpty())
	{
		MoveNode* pFirstMove = FindStartPosition(m_MoveList.GetRoot());

		if (pFirstMove)
		{
			FindMoveNode(pFirstMove);
			isFound = true;
		}
		else if (Notify)
		{
			SetPaneText(Utils::GetString(IDS_NO_START_POSITION));
		}
	}

	return isFound;
}

//------------------------------------------------------------------------

void CRenLibDoc::SetPreviousVariant()
{
	CPoint singlePos;

	GetSingleVariant(singlePos);
	m_Board.SetPreviousVariant(singlePos);
}

//------------------------------------------------------------------------

bool CRenLibDoc::MakeRenartistAppletCommand(const CString& strFile)
{
	Renartist renartist;
	return renartist.Create(strFile, m_MoveList);
}

//------------------------------------------------------------------------

bool CRenLibDoc::MakeRenLibAppletCommand(
    const CString& strFile, CString& strComment,
	const RenLibApplet::Information& info)
{
	RenLibApplet renlibApplet(info);

	return renlibApplet.Create(
		strFile, m_MoveList, strComment, info);
}

//------------------------------------------------------------------------

bool CRenLibDoc::RenjuclassDiagramCommand(const CString& strFile)
{
	Renjuclass command;

	command.Create(m_MoveList);
	return command.Save(strFile);
}

//------------------------------------------------------------------------

void CRenLibDoc::TextBoardCommand(const CString& strFile)
{
	TextBoard command;

	command.Create(m_MoveList);
	command.Save(strFile);
}

//------------------------------------------------------------------------

void CRenLibDoc::ExportTextBoardCommand(const CString& strDirectory)
{
	bool doSaveAll = true;

	if (mIsFindInProgress && m_SearchList.GetKind() == SearchList::COMMENT)
	{
		int choice =
			Utils::ShowMessage(Utils::GetString(IDS_MSG_EXPORT_SELECTED_POSITIONS),
			Utils::GetString(IDS_CAP_EXPORT_TEXT_BOARD),
			MB_YESNO + MB_ICONQUESTION);

		doSaveAll = (choice == IDNO);		
	}

	if (doSaveAll)
	{
		FirstCommand();
		FindMoveCommand(999);
	}

	if (!m_SearchList.IsEmpty())
	{
		int nTotalFiles = m_SearchList.LastItem();
		
		CProgressWnd wndProgress(AfxGetApp()->m_pMainWnd, Utils::GetString(IDS_CAP_EXPORT_TEXT_BOARD, nTotalFiles));
		
		wndProgress.GoModal();
		wndProgress.SetRange(0,nTotalFiles);
		
		for (int i=1; i<=nTotalFiles; i++)
		{
			MoveNode* pMove = m_SearchList.Get(i)->getMoveNode();
			
			if (pMove != 0 && FindMoveNode(pMove))
			{
				CString strFile;
				strFile.Format("RenLib%05d.txt", i);

				CString strText(Utils::GetString(IDS_MSG_SAVING_FILE));
				strText += "\n" + strFile;
				wndProgress.SetText(strText);
				
				TextBoardCommand(strDirectory + strFile);

				wndProgress.StepIt();
				wndProgress.PeekAndPump();
				
				if (wndProgress.Cancelled())
				{
					CString strMessage(Utils::GetString(IDS_MSG_CANCELLED));
					Utils::ShowMessage(strMessage, Utils::GetString(IDS_CAP_EXPORT_TEXT_BOARD), MB_ICONWARNING);
					
					break;
				}
			}
		}
	}

	if (doSaveAll)
	{
		FirstCommand();
		m_SearchList.Empty();
		mIsFindInProgress = false;
	}
}

//------------------------------------------------------------------------

bool CRenLibDoc::ExportRenLibAppletCommand(const CString& strDirectory, CString& strFile, COLORREF* custColors)
{
	bool result    = false;
	bool doSaveAll = true;

	CString strCaption(Utils::GetString(IDS_CAP_EXPORT_RENLIB_APPLET));

	if (mIsFindInProgress && m_SearchList.GetKind() == SearchList::COMMENT)
	{
		int choice =
			Utils::ShowMessage(
			    Utils::GetString(IDS_MSG_EXPORT_SELECTED_POSITIONS),
				strCaption,
				MB_YESNO + MB_ICONQUESTION);

		doSaveAll = (choice == IDNO);		
	}

	RenLibApplet::Information info;

	RenLibApplet renlibApplet(info);

	if (!renlibApplet.getAppletInformation(strDirectory, info, custColors))
	{
		return false; // User clicked Cancel in RenLibApplet dialogue
	}

	CStringList strList;

	if (doSaveAll)
	{
		FirstCommand();
		FindMoveCommand(999);
	}

	if (!m_SearchList.IsEmpty())
	{
		int nTotalFiles = m_SearchList.LastItem();
		
		CProgressWnd wndProgress(AfxGetApp()->m_pMainWnd, strCaption);
		
		wndProgress.GoModal();
		wndProgress.SetRange(0,nTotalFiles);
		
		for (int i=1; i<=nTotalFiles; i++)
		{
			MoveNode* pMove = m_SearchList.Get(i)->getMoveNode();
			
			if (pMove != 0 && FindMoveNode(pMove))
			{
				CString strFile;
				strFile.Format("RenLib%05d.htm", i);

				CString strText(Utils::GetString(IDS_MSG_SAVING_FILE));
				strText += "\n" + strFile;
				wndProgress.SetText(strText);
				
				CString strComment;

				MakeRenLibAppletCommand(
					strDirectory + strFile, strComment, info);

				Utils::trim(strComment);

				if (strComment.IsEmpty())
				{
					strComment = "<b style=color:#ff0000>Missing comment</b>";
				}

				strList.AddTail(strFile);
				strList.AddTail(strComment);

				wndProgress.StepIt();
				wndProgress.PeekAndPump();
				
				if (wndProgress.Cancelled())
				{
					CString strMessage(Utils::GetString(IDS_MSG_CANCELLED));
					Utils::ShowMessage(strMessage, strCaption, MB_ICONWARNING);
					
					break;
				}
			}
		}
		
		result =
			renlibApplet.CreateExportGameFile(
			    strDirectory, strList, info.mTitleGameWebPage, strCaption, strFile);
	}

	if (doSaveAll)
	{
		FirstCommand();
		m_SearchList.Empty();
		mIsFindInProgress = false;
	}

	return result;
}

//------------------------------------------------------------------------

bool CRenLibDoc::ExportGameCollectionCommand(CString& strFile, COLORREF* custColors)
{
	bool result    = false;
	bool doSaveAll = true;

	CString strCaption(Utils::GetString(IDS_CAP_EXPORT_GAME_COLLECTION));

	const COLORREF cWhite = RGB(255, 255, 255);

    RenLibApplet::Information info(RenLibApplet::SMALL, "", "",
								   FALSE, FALSE, TRUE, TRUE, TRUE, FALSE,
                                   FALSE, RenLibApplet::WEB, "", cWhite);

	RenLibAppletDialog renLibAppletDialog(false, false, false, false, false,
                                          false, false, false, false, true,
                                          true,  false, false, custColors);
	
    renLibAppletDialog.setInfo(info);

    if (renLibAppletDialog.DoModal() != IDOK)
	{
        return result;
	}

    renLibAppletDialog.getInfo(info);

    RenLibApplet renlibApplet(info);

	if (doSaveAll)
	{
		FirstCommand();

        CString strCommentToFind;
        FindCommentCommand(strCommentToFind);
	}

	if (!m_SearchList.IsEmpty())
	{
		if (renlibApplet.CreateGameCollectionFile(info.mTitleGameWebPage, strCaption, strFile))
		{
			result = true;

			int nTotal = m_SearchList.LastItem();

			CProgressWnd wndProgress(AfxGetApp()->m_pMainWnd, strCaption);

			wndProgress.GoModal();
			wndProgress.SetRange(0, nTotal);

			for (int i=1; i <= nTotal; i++)
			{
				MoveNode* pMove = m_SearchList.Get(i)->getMoveNode();

				if (pMove != 0 && FindMoveNode(pMove))
				{
					CString strText;
					strText.Format("%d of %d", i, nTotal);
					wndProgress.SetText(strText);
                    
                    CString strOneLineComment(getOneLineComment());
                    CString strMultiLineComment(getMultiLineComment());

					if (!renlibApplet.AddGameCollectionFile(m_MoveList, strOneLineComment, strMultiLineComment, info))
					{
						result = false;
						break;
					}

					wndProgress.StepIt();
					wndProgress.PeekAndPump();

					if (wndProgress.Cancelled())
					{
						CString strMessage(Utils::GetString(IDS_MSG_CANCELLED));
						Utils::ShowMessage(strMessage, strCaption, MB_ICONWARNING);

						break;
					}
				}
			}

			result = renlibApplet.CloseGameCollectionFile() || result;
		}
	}

	if (doSaveAll)
	{
		FirstCommand();
		m_SearchList.Empty();
		mIsFindInProgress = false;
	}

	return result;
}

//------------------------------------------------------------------------

BOOL CRenLibDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	return SaveCommand(true);
}

//------------------------------------------------------------------------

BOOL CRenLibDoc::SaveModified()
{
	return TRUE; // Indicate that the document can be closed 
}

//------------------------------------------------------------------------

void CRenLibDoc::SetIsWinPosition(WinKind kind, bool isWin)
{
    m_IsWinPosition[kind] = isWin;
    m_WinMoveNode = isWin ? m_MoveList.Current() : 0;
}

//------------------------------------------------------------------------

bool CRenLibDoc::IsWinPosition(WinKind kind)
{
	return
        m_IsWinPosition[kind] &&
        m_WinMoveNode != 0 && m_WinMoveNode == m_MoveList.Current();
}

//------------------------------------------------------------------------

bool CRenLibDoc::SavePosCommand(const CString& strFile)
{
  Pos pos;
  return pos.Save(strFile, m_MoveList);
}

//------------------------------------------------------------------------

bool CRenLibDoc::SavePdbCommand(const CString& strFile)
{
  Pdb pdb;
  return pdb.Save(strFile, m_MoveList);
}

//------------------------------------------------------------------------

void CRenLibDoc::SaveBdtCommand(const CString& strFile)
{
    Bdt bdt;

	FirstCommand();
	FindMoveCommand(999);

	if (!m_SearchList.IsEmpty())
	{
        bdt.Create(strFile);

        int nTotalPositions = m_SearchList.LastItem();
		
		CProgressWnd wndProgress(AfxGetApp()->m_pMainWnd, Utils::GetString(IDS_CAP_CREATE_BDT, nTotalPositions));
		
		wndProgress.GoModal();
		wndProgress.SetRange(0, nTotalPositions);
		
		for (int i=1; i <= nTotalPositions; i++)
		{
			MoveNode* pMove = m_SearchList.Get(i)->getMoveNode();
			
			if (pMove != 0 && FindMoveNode(pMove))
			{
				CString strNum;
				strNum.Format("%d", i);

				CString strText(Utils::GetString(IDS_MSG_SAVING_FILE));
				strText += "\n" + strNum;
				wndProgress.SetText(strText);
				
                bdt.Save(m_MoveList);

				wndProgress.StepIt();
				wndProgress.PeekAndPump();
				
				if (wndProgress.Cancelled())
				{
					CString strMessage(Utils::GetString(IDS_MSG_CANCELLED));
					Utils::ShowMessage(strMessage, Utils::GetString(IDS_CAP_CREATE_BDT), MB_ICONWARNING);
					
					break;
				}
			}
		}

        bdt.Close();
	}

    FirstCommand();
    m_SearchList.Empty();
    mIsFindInProgress = false;
}

void CRenLibDoc::SaveBufCommand(const CString& strFile)
{
    Buf buf;

	FirstCommand();
	FindMoveCommand(999);

	if (!m_SearchList.IsEmpty())
	{
        buf.Create(strFile);

        buf.Save(m_MoveList);

        int nTotalPositions = m_SearchList.LastItem();
		
		CProgressWnd wndProgress(AfxGetApp()->m_pMainWnd, Utils::GetString(IDS_CAP_CREATE_BUF, nTotalPositions));
		
		wndProgress.GoModal();
		wndProgress.SetRange(0, nTotalPositions);
		
		for (int i=1; i <= nTotalPositions; i++)
		{
			MoveNode* pMove = m_SearchList.Get(i)->getMoveNode();
			
			if (pMove != 0 && FindMoveNode(pMove))
			{
				CString strFile;
				strFile.Format("RenLib%05d.txt", i);

				CString strText(Utils::GetString(IDS_MSG_SAVING_FILE));
				strText += "\n" + strFile;
				wndProgress.SetText(strText);
				
//				TextBoardCommand(strDirectory + strFile);

				wndProgress.StepIt();
				wndProgress.PeekAndPump();
				
				if (wndProgress.Cancelled())
				{
					CString strMessage(Utils::GetString(IDS_MSG_CANCELLED));
					Utils::ShowMessage(strMessage, Utils::GetString(IDS_CAP_CREATE_BUF), MB_ICONWARNING);
					
					break;
				}
			}
		}

        buf.Close();
	}

    FirstCommand();
    m_SearchList.Empty();
    mIsFindInProgress = false;
}

//------------------------------------------------------------------------

void CRenLibDoc::initMergeComment(bool keepPrevious)
{
	if (m_SearchList.GetKind() == SearchList::MERGE_COMMENT && keepPrevious)
	{
		m_SearchList.purge();
	}
	else
	{
		m_SearchList.Empty();
	}

	m_SearchList.SetStart(SearchItem(0));
	m_SearchList.SetInfo(Utils::GetString(IDS_MERGE_COMMENT), SearchList::MERGE_COMMENT);

    mIsFindInProgress    = false;
}

//------------------------------------------------------------------------

bool CRenLibDoc::hasMergeComment()
{
	if (!m_SearchList.IsEmpty())
	{
		m_SearchList.SetStart(SearchItem(m_MoveList.Current()));
		mIsFindInProgress = true;
		SetFindInfo(true);
	}

	return mIsFindInProgress;
}

//------------------------------------------------------------------------

bool CRenLibDoc::mergeComment()
{
	MoveNode*   pMove = m_MoveList.Current();
	SearchItem*	item  = m_SearchList.Current();

	bool isMerged = false;
    bool applySelectionToAll = false;
	
	m_eMode = UPDATE_MODE;

	if (pMove == item->getMoveNode())
	{
		switch (item->getKind())
		{
			case SearchItem::ONE_LINE_COMMENT:
			{
				MergeComment mergeDlg(pMove->getOneLineComment(),
									  item->getMergeText(),
									  MergeComment::ONE_LINE_COMMENT);
				
				if (mergeDlg.DoModal() == IDOK)
				{
					isMerged = true;
                    mSelection = mergeDlg.getSelection();
                    applySelectionToAll = mergeDlg.getApplySelectionToAll();
					
                    pMove->setOneLineComment(mergeDlg.getMergedComment());
					
                    m_SearchList.Clear();
					SetModifiedFlag();
				}
				break;
			}

			case SearchItem::MULTI_LINE_COMMENT:
			{
				MergeComment mergeDlg(pMove->getMultiLineComment(),
									  item->getMergeText(),
									  MergeComment::MULTI_LINE_COMMENT);
				
				if (mergeDlg.DoModal() == IDOK)
				{
					isMerged = true;
                    mSelection = mergeDlg.getSelection();
                    applySelectionToAll = mergeDlg.getApplySelectionToAll();

                    pMove->setMultiLineComment(mergeDlg.getMergedComment());
					
                    m_SearchList.Clear();
					SetModifiedFlag();
				}
				break;
			}

			case SearchItem::BOARD_TEXT:
			{
				moveBack();
				m_Board.SetPreviousVariant(NullPoint);

				AfxGetApp()->GetMainWnd()->Invalidate(FALSE);
				AfxGetApp()->GetMainWnd()->UpdateWindow();

				MergeComment mergeDlg(pMove->getBoardText(),
									  item->getMergeText(),
									  MergeComment::BOARD_TEXT);
				
				if (mergeDlg.DoModal() == IDOK)
				{
					isMerged = true;
                    mSelection = mergeDlg.getSelection();
                    applySelectionToAll = mergeDlg.getApplySelectionToAll();

                    pMove->setBoardText(mergeDlg.getMergedComment());
					
                    m_SearchList.Clear();
					SetModifiedFlag();
				}
				break;
			}

			case SearchItem::NONE:
			{
				break;
			}
		}
	}

	if (applySelectionToAll)
	{
		mergeCommentAll();
		isMerged = false;
	}

	AfxGetApp()->GetMainWnd()->Invalidate(FALSE);
	AfxGetApp()->GetMainWnd()->UpdateWindow();

	return isMerged;
}

//------------------------------------------------------------------------

void CRenLibDoc::mergeCommentAll()
{
	CWaitCursor cursor;

    CString strCaption(Utils::GetString(IDS_CAP_MERGE_ALL, m_SearchList.LastItem()));

	CProgressWnd wndProgress(AfxGetApp()->GetMainWnd(), strCaption);
	
	wndProgress.GoModal();
	wndProgress.SetRange(0, m_SearchList.LastItem());

	while (FindNextPrevious(mIsNext))
	{
		MoveNode*   pMove = m_MoveList.Current();
		SearchItem*	item  = m_SearchList.Current();

		m_eMode = UPDATE_MODE;

		CString strText;
		strText.Format("%d", m_SearchList.CurrentIndex());
		wndProgress.SetText(strText);

        if (pMove == item->getMoveNode())
		{
			//
            // Do not show dialog
            //
            CString strMergedComment;

		    switch (item->getKind())
		    {
			    case SearchItem::ONE_LINE_COMMENT:
			    {
		            switch (mSelection)
		            {
			            case MergeComment::CURRENT:
				        {
                            strMergedComment = pMove->getOneLineComment();
					        break;
				        }
			            case MergeComment::NEW:
				        {
                            strMergedComment = item->getMergeText();
					        break;
				        }
			            case MergeComment::COMBINED:
				        {
                            strMergedComment = pMove->getOneLineComment() + ' ' + item->getMergeText();
					        break;
				        }
		            }
				    
					pMove->setOneLineComment(strMergedComment);
					m_SearchList.Clear();
					SetModifiedFlag();
                    break;
			    }

			    case SearchItem::MULTI_LINE_COMMENT:
			    {
		            switch (mSelection)
		            {
			            case MergeComment::CURRENT:
				        {
                            strMergedComment = pMove->getMultiLineComment();
					        break;
				        }
			            case MergeComment::NEW:
				        {
                            strMergedComment = item->getMergeText();
					        break;
				        }
			            case MergeComment::COMBINED:
				        {
                            strMergedComment = pMove->getMultiLineComment() + "\r\n" + item->getMergeText();
					        break;
				        }
		            }
				    
					pMove->setMultiLineComment(strMergedComment);
					m_SearchList.Clear();
					SetModifiedFlag();
				    break;
			    }

			    case SearchItem::BOARD_TEXT:
			    {
				    moveBack();
				    m_Board.SetPreviousVariant(NullPoint);

		            switch (mSelection)
		            {
			            case MergeComment::CURRENT:
				        {
                            strMergedComment = pMove->getBoardText();
					        break;
				        }
			            case MergeComment::NEW:
				        {
                            strMergedComment = item->getMergeText();
					        break;
				        }
			            case MergeComment::COMBINED:
				        {
                            strMergedComment = pMove->getBoardText() + ' ' + item->getMergeText();
					        break;
				        }
		            }
				    
					pMove->setBoardText(strMergedComment);
					m_SearchList.Clear();
					SetModifiedFlag();
				    break;
			    }

			    case SearchItem::NONE:
			    {
				    break;
			    }
		    }
        }

        wndProgress.StepIt();
		wndProgress.PeekAndPump();

		if (wndProgress.Cancelled())
		{
			CString strMessage(Utils::GetString(IDS_MSG_CANCELLED));
			Utils::ShowMessage(strMessage, strCaption, MB_ICONWARNING);
			break;
		}
	}

	SetPaneText(Utils::GetString(IDS_MERGE_ALL_READY));
}

//------------------------------------------------------------------------

bool CRenLibDoc::isMergeComment()
{
	return (IsFindInProgress() &&
		    m_SearchList.GetKind() == SearchList::MERGE_COMMENT);
}

//------------------------------------------------------------------------

MoveNode* CRenLibDoc::GetCurrentMoveNode()
{
	if (m_MoveList.IsEmpty())
	{
		return 0;
	}
	else
	{
		return m_MoveList.Current();
	}
}

//------------------------------------------------------------------------

void CRenLibDoc::splitBoardText(MoveNode* pMove)
{
	if (pMove->isMove() && pMove->isBoardText())
	{
		// Create new board text node
		MoveNode* pBoardText = new MoveNode(pMove->getPos());
		pBoardText->setBoardText(pMove->getBoardText());
		pBoardText->setIsMove(false);

		// Empty current board text
		pMove->setBoardText("");

		// Link in new board text node
		pBoardText->setRight(pMove->getRight());
		pMove->setRight(pBoardText);
	}
}

//------------------------------------------------------------------------

void CRenLibDoc::ConvertXY(MoveNode* pMove, int Fx, int Fy, bool convertSibling)
{
	if (pMove)
	{
		ConvertXY(pMove->getDown(), Fx, Fy, true);

		if (convertSibling)
		{
			ConvertXY(pMove->getRight(), Fx, Fy, true);
		}
		else
		{
			splitBoardText(pMove);
		}

		CPoint point = pMove->getPos();
		int x = (point.x - 8) * Fx + 8;
		int y = (point.y - 8) * Fy + 8;
		point = CPoint(x,y);

		const bool cIsMove = pMove->isMove();
		pMove->setPos(point);
		pMove->setIsMove(cIsMove);

		SetModifiedFlag();
	}
}

//------------------------------------------------------------------------

void CRenLibDoc::ConvertYX(MoveNode* pMove, int Fx, int Fy, bool convertSibling)
{
	if (pMove)
	{
		ConvertYX(pMove->getDown(), Fx, Fy, true);

		if (convertSibling)
		{
			ConvertYX(pMove->getRight(), Fx, Fy, true);
		}
		else
		{
			splitBoardText(pMove);
		}

		CPoint point = pMove->getPos();
		int y = (point.x - 8) * Fx + 8;
		int x = (point.y - 8) * Fy + 8;
		point = CPoint(x,y);

		const bool cIsMove = pMove->isMove();
		pMove->setPos(point);
		pMove->setIsMove(cIsMove);

		SetModifiedFlag();
	}
}

//------------------------------------------------------------------------

void CRenLibDoc::ConvertToStandard(MoveList& moveList)
{
	if (moveList.Index() >= 2 && moveList.Get(1)->getPos() == Center)
	{
		MoveNode* pMove2 = moveList.Get(2);

		switch (Utils::PointToPos(pMove2->getPos()))
		{
			case XY_8_7: // Direct Opening
			case XY_9_7: // Indirect Opening
			{
				break; // continue checking
			}

			case XY_7_7:
			{
				ConvertXY(pMove2, -1, 1);
				break;
			}
			
			case XY_7_8:
			{
				ConvertYX(pMove2, 1, -1);
				break;
			}
			
			case XY_9_8:
			{
				ConvertYX(pMove2, -1, 1);
				break;
			}
			
			case XY_7_9:
			{
				ConvertXY(pMove2, -1, -1);
				break;
			}
			
			case XY_8_9:
			{
				ConvertXY(pMove2, 1, -1);
				break;
			}
			
			case XY_9_9:
			{
				ConvertXY(pMove2, 1, -1);
				break;
			}
			
			default:
			{
				return; // not renju opening
			}
		}
	}

	if (moveList.Index() >= 3)
	{
		MoveNode* pMove3 = moveList.Get(3);

		if (Utils::PointToPos(moveList.Get(2)->getPos()) == XY_8_7)
		{
			// Direct Opening
			switch (Utils::PointToPos(pMove3->getPos()))
			{
				case XY_8_6:
				case XY_9_6:
				case XY_10_6:
				case XY_9_7:
				case XY_10_7:
				case XY_9_8:
				case XY_10_8:
				case XY_8_9:
				case XY_9_9:
				case XY_10_9:
				case XY_8_10:
				case XY_9_10:
				case XY_10_10:
				{
					break; // continue checking
				}

				case XY_6_6:
				case XY_6_7:
				case XY_6_8:
				case XY_6_9:
				case XY_6_10:
				case XY_7_6:
				case XY_7_7:
				case XY_7_8:
				case XY_7_9:
				case XY_7_10:
				{
					ConvertXY(pMove3, -1, 1);
					break;
				}

				default:
				{
					return; // not renju opening
				}
			}

			if (moveList.Index() >= 4 &&
				moveList.Get(3)->getPos().x == 8)
			{
				if (moveList.Get(4)->getPos().x < 8)
				{
					ConvertXY(moveList.Get(4), -1, 1);

				}
				else if (moveList.Index() >= 5 &&
                         moveList.Get(4)->getPos().x == 8 &&
						 moveList.Get(5)->getPos().x < 8)
				{
					ConvertXY(moveList.Get(5), -1, 1);
				}
			}
		}
		else
		{
			// Indirect Opening
			switch (Utils::PointToPos(pMove3->getPos()))
			{
				case XY_10_6:
				case XY_10_7:
				case XY_10_8:
				case XY_10_9:
				case XY_10_10:
				case XY_9_8:
				case XY_9_9:
				case XY_9_10:
				case XY_8_9:
				case XY_8_10:
				case XY_7_9:
				case XY_7_10:
				case XY_6_10:
				{
					break; // continue checking
				}

				case XY_6_6:
				case XY_6_7:
				case XY_6_8:
				case XY_6_9:
				case XY_7_6:
				case XY_7_7:
				case XY_7_8:
				case XY_8_6:
				case XY_8_7:
				case XY_9_6:
				{
					ConvertYX(pMove3, -1, -1);
					break;
				}

				default:
				{
					return; // not renju opening
				}
			}

			if (moveList.Index() >= 4)
			{
				CPoint pos3(moveList.Get(3)->getPos());
				CPoint pos4(moveList.Get(4)->getPos());

				if (pos3.x == 16 - pos3.y &&
					pos4.x  > 16 - pos4.y)
				{
					ConvertYX(moveList.Get(4), -1, -1);
				}
			}
		}
	}
}

//------------------------------------------------------------------------

void CRenLibDoc::ConvertToStandardCommand()
{
	const int cMoveNo = 5;

	m_MoveList.SetRootIndex();

	MoveList LocalMoveList = m_MoveList;

	Stack m_Stack;

	MoveNode* pMove = LocalMoveList.GetRoot();

	bool isFound = false;
	bool done    = false;

	if (pMove)
	{
		pMove = pMove->getDown();
	}

	while (!done)
	{
		if (pMove)
		{
			LocalMoveList.Add(pMove);

			if (pMove->isMove() && (LocalMoveList.Index() == cMoveNo || NoOfVariantMoves(pMove) == 0))
			{
				ConvertToStandard(LocalMoveList);
			}

			if (pMove->getRight())
			{
				m_Stack.Push(LocalMoveList.Index(), pMove->getRight());
			}

			if (LocalMoveList.Index() < cMoveNo)
			{
				pMove = pMove->getDown();
			}
			else
			{
				pMove = 0;
			}
		}
		else if (!m_Stack.IsEmpty())
		{
			int nMove;
			m_Stack.Pop(nMove, pMove);
			LocalMoveList.SetIndex(nMove - 1);
		}
		else
		{
			done = true;
		}
	}

	mergeLibrary(LocalMoveList.GetRoot());

	m_Board.SetLastMove(NullPoint);
	m_Board.SetPreviousVariant(NullPoint);

	// Set first move
	m_MoveList.SetRootIndex();
	m_MoveList.ClearEnd();

	pMove = m_MoveList.GetRoot()->getDown();

	if (NoOfVariantMoves() == 1 && pMove != 0)
	{
		m_MoveList.Add(pMove);
	}
}

//------------------------------------------------------------------------

void CRenLibDoc::mergeLibrary(MoveNode* pMove)
{
	if (pMove)
	{
		mergeSibling(pMove);
		mergeLibrary(pMove->getRight());
		mergeLibrary(pMove->getDown());
	}
}

//------------------------------------------------------------------------

void CRenLibDoc::mergeSibling(MoveNode* pMove)
{
	if (pMove)
	{
		MoveNode* pPrevious = pMove;
		MoveNode* pSibling  = pMove->getRight();

		while (pSibling)
		{
			if (pMove->getPos() == pSibling->getPos())
			{
				MoveNode* pFirst = pSibling->getDown();
				MoveNode* pLast  = pFirst;

				if (pFirst)
				{
					// Find rightmost child nodes
					while (pLast && pLast->getRight())
					{
						pLast = pLast->getRight();
					}

					// Unlink child nodes from this sibling
					ASSERT(pLast->getRight() == 0);
					pLast->setRight(pMove->getDown());
					pMove->setDown(pFirst);
				}

				// Unlink this sibling
				pPrevious->setRight(pSibling->getRight());

				// Merge comments and board text from this sibling
				addComment(pMove, pSibling->getOneLineComment(), pSibling->getMultiLineComment());
				addBoardText(pMove, pSibling->getBoardText());

				// Merge attributes from this sibling
				addAttributes(pMove, pSibling);

				// Replace merged away move node in search list
				m_SearchList.replace(pSibling, pMove);

				// Delete sibling
				pSibling->setDown(0);
				pSibling->setRight(0);

				delete pSibling;
				pSibling = 0;

				SetModifiedFlag();
			}
			else
			{
				pPrevious = pSibling;
			}

			// Next sibling to check
			pSibling = pPrevious->getRight();
		}
	}
}

//------------------------------------------------------------------------

void CRenLibDoc::removeEmptyBoardTexts()
{
	if (!m_MoveList.IsEmpty())
	{
		removeEmptyBoardTexts(m_MoveList.GetRoot());
	}
}

//------------------------------------------------------------------------

void CRenLibDoc::removeEmptyBoardTexts(MoveNode* pMove)
{
	if (pMove)
	{
		while (pMove->getRight())
		{
			MoveNode* pRight = pMove->getRight();
			
			if (!pRight->isMove() && pRight->getBoardText().IsEmpty())
			{
				// Unlink empty board text
				pMove->setRight(pRight->getRight());

				// Delete sibling
				pRight->setDown(0);
				pRight->setRight(0);

				delete pRight;
				pRight = 0;

				SetModifiedFlag();
			}
			else
			{
				break;
			}
		}

		removeEmptyBoardTexts(pMove->getRight());

		while (pMove->getDown())
		{
			MoveNode* pDown = pMove->getDown();
			
			if (!pDown->isMove() && pDown->getBoardText().IsEmpty())
			{
				// Unlink empty board text
				pMove->setDown(pDown->getRight());

				// Delete sibling
				pDown->setDown(0);
				pDown->setRight(0);

				delete pDown;
				pDown = 0;

				SetModifiedFlag();
			}
			else
			{
				break;
			}
		}

		removeEmptyBoardTexts(pMove->getDown());
	}
}

//------------------------------------------------------------------------

void CRenLibDoc::EmptyPaneText()
{
	SetPaneText("");
}

//------------------------------------------------------------------------

void CRenLibDoc::SetPaneText(const CString& text)
{
	CMainFrame* pFrame = (CMainFrame*) AfxGetApp()->GetMainWnd();
	pFrame->SetPaneText(text);
}

//------------------------------------------------------------------------

bool CRenLibDoc::checkConvertToStandard(MoveList& moveList)
{
	if (moveList.Index() >= 2 && moveList.Get(1)->getPos() == Center)
	{
		MoveNode* pMove2 = moveList.Get(2);

		switch (Utils::PointToPos(pMove2->getPos()))
		{
			case XY_8_7: // Direct Opening
			case XY_9_7: // Indirect Opening
			{
				break; // continue checking
			}

			case XY_7_7:
			case XY_7_8:
			case XY_9_8:
			case XY_7_9:
			case XY_8_9:
			case XY_9_9:
			{
				return true; // conversion needed
			}
			
			default:
			{
				return false; // not renju opening
			}
		}
	}

	if (moveList.Index() >= 3)
	{
		MoveNode* pMove3 = moveList.Get(3);

		if (Utils::PointToPos(moveList.Get(2)->getPos()) == XY_8_7)
		{
			// Direct Opening
			switch (Utils::PointToPos(pMove3->getPos()))
			{
				case XY_8_6:
				case XY_9_6:
				case XY_10_6:
				case XY_9_7:
				case XY_10_7:
				case XY_9_8:
				case XY_10_8:
				case XY_8_9:
				case XY_9_9:
				case XY_10_9:
				case XY_8_10:
				case XY_9_10:
				case XY_10_10:
				{
					break; // continue checking
				}

				case XY_7_6:
				case XY_6_6:
				case XY_7_7:
				case XY_6_7:
				case XY_7_8:
				case XY_6_8:
				case XY_7_9:
				case XY_6_9:
				case XY_7_10:
				case XY_6_10:
				{
					return true; // conversion needed
				}
				
				default:
				{
					return false; // not renju opening
				}
			}

			if (moveList.Index() >= 4 &&
				moveList.Get(3)->getPos().x == 8)
			{
				if (moveList.Get(4)->getPos().x < 8)
				{
					return true;
				}
				else if (moveList.Index() >= 5 &&
                         moveList.Get(4)->getPos().x == 8 &&
						 moveList.Get(5)->getPos().x < 8)
				{
					moveList.Get(5)->setIsStart(true);
					return true;
				}
			}
		}
		else
		{
			// Indirect Opening
			switch (Utils::PointToPos(pMove3->getPos()))
			{
				case XY_10_6:
				case XY_10_7:
				case XY_10_8:
				case XY_10_9:
				case XY_10_10:
				case XY_9_8:
				case XY_9_9:
				case XY_9_10:
				case XY_8_9:
				case XY_8_10:
				case XY_7_9:
				case XY_7_10:
				case XY_6_10:
				{
					break; // continue checking
				}

				case XY_6_6:
				case XY_6_7:
				case XY_6_8:
				case XY_6_9:
				case XY_7_6:
				case XY_7_7:
				case XY_7_8:
				case XY_8_6:
				case XY_8_7:
				case XY_9_6:
				{
					return true; // conversion needed
				}
				
				default:
				{
					return false; // not renju opening
				}
			}

			if (moveList.Index() >= 4)
			{
				CPoint pos3(moveList.Get(3)->getPos());
				CPoint pos4(moveList.Get(4)->getPos());

				if (pos3.x == 16 - pos3.y &&
					pos4.x  > 16 - pos4.y)
				{
					return true;
				}
			}
		}
	}

	return false;
}

//------------------------------------------------------------------------

bool CRenLibDoc::checkConvertToStandard()
{
	const int cMoveNo = 5;

	MoveList LocalMoveList = m_MoveList;
	LocalMoveList.SetRootIndex();

	Stack m_Stack;

	MoveNode* pMove = LocalMoveList.GetRoot();

	bool isFound = false;
	bool done    = false;

	if (pMove)
	{
		pMove = pMove->getDown();
	}

	while (!done)
	{
		if (pMove)
		{
			LocalMoveList.Add(pMove);

			if (pMove->isMove() && (LocalMoveList.Index() == cMoveNo || NoOfVariantMoves(pMove) == 0))
			{
				if (checkConvertToStandard(LocalMoveList))
				{
					return true;
				}
			}

			if (pMove->getRight())
			{
				m_Stack.Push(LocalMoveList.Index(), pMove->getRight());
			}

			if (LocalMoveList.Index() < cMoveNo)
			{
				pMove = pMove->getDown();
			}
			else
			{
				pMove = 0;
			}
		}
		else if (!m_Stack.IsEmpty())
		{
			int nMove;
			m_Stack.Pop(nMove, pMove);
			LocalMoveList.SetIndex(nMove - 1);
		}
		else
		{
			done = true;
		}
	}

	return false;
}

//------------------------------------------------------------------------

bool CRenLibDoc::isUpdateMode()
{
	return m_eMode == UPDATE_MODE;
}

//------------------------------------------------------------------------

bool CRenLibDoc::isDisplayMode()
{
	return m_eMode == DISPLAY_MODE;
}

//------------------------------------------------------------------------

bool CRenLibDoc::isProblemMode()
{
	return m_eMode == PROBLEM_MODE;
}

//------------------------------------------------------------------------

bool CRenLibDoc::isFirstMoveH8Only(int moveNo)
{
	MoveNode* pMove = m_MoveList.GetRoot();

	if (pMove && pMove->isInformation())
	{
		return false;
	}

	if (moveNo == 0)
	{
		pMove = pMove->getDown();

		while (pMove)
		{
			if (pMove->getPos() != Center)
			{
				return false;
			}

			pMove = pMove->getRight();
		}

		return true;
	}

	return (m_MoveList.Get(1)->getPos() == Center) ? true : false;
}

//------------------------------------------------------------------------

MoveNode* CRenLibDoc::FindNullPoint(MoveNode* pMove)
{
	MoveNode* pNullPoint = 0;

	if (pMove)
	{
		if (pMove->getPos() == NullPoint)
		{
			return pMove;
		}

		pNullPoint = FindNullPoint(pMove->getDown());

		if (pNullPoint == 0)
		{
			pNullPoint = FindNullPoint(pMove->getRight());
		}
	}

	return pNullPoint;
}

//------------------------------------------------------------------------

bool CRenLibDoc::hasPassMove(int moveNo)
{
	if (moveNo == 0)
	{
		moveNo = 1;
	}

	for (int i=1; i <= moveNo; i++)
	{
		if (m_MoveList.Get(i)->getPos() == NullPoint)
		{
			return true;
		}
	}

	return FindNullPoint(m_MoveList.Get(moveNo)->getDown()) != 0;
}

//------------------------------------------------------------------------

CString CRenLibDoc::getLastFile()
{
	return m_strLastFile;
}

//------------------------------------------------------------------------

bool CRenLibDoc::findWin(WinKind kind)
{
    bool findInProgress = false;

    bool doFind = !IsWinPosition(kind);

    m_Board.ClearWin();

    SetIsWinPosition(VCF, false);
    SetIsWinPosition(VCT, false);

	if (doFind)
	{
        switch (kind)
        {
            case VCF:
            {
                findInProgress =
                    (m_Board.findVcf(m_MoveList) == Board::FIND_VCF_IN_PROGRESS);

                break;
            }

            case VCT:
            {
                findInProgress =
                    (m_Board.findVct(m_MoveList) == Board::FIND_VCT_IN_PROGRESS);

                break;
            }
        }
	}

    return findInProgress;
}

//------------------------------------------------------------------------

bool CRenLibDoc::checkFindWin(Board::WinMarker marker, const CString& prefix)
{
    bool findInProgress = false;

    CString strInfo;

    switch (m_Board.checkFindStatus(strInfo))
    {
        case Board::FIND_IDLE:
	    {
            break;
	    }

        case Board::FIND_VCF_IN_PROGRESS:
        case Board::FIND_VCT_IN_PROGRESS:
	    {
            strInfo = prefix + strInfo;
            SetPaneText(strInfo);
		    findInProgress = true;
            break;
	    }

        case Board::FOUND_VCF:
	    {
		    int noOfMoves = m_Board.setWinTrace(marker);
		    SetIsWinPosition(VCF, true);
            strInfo = prefix + Utils::GetString(IDS_VCF_FOUND, noOfMoves);
            SetPaneText(strInfo);
            break;
	    }

        case Board::FOUND_NO_VCF:
	    {
            strInfo = prefix + Utils::GetString(IDS_VCF_NOT_FOUND);
            SetPaneText(strInfo);
		    Utils::Beep();
            break;
	    }

        case Board::FOUND_VCT:
	    {
            m_Board.setWinTrace(marker);
		    SetIsWinPosition(VCT, true);
            strInfo = prefix + Utils::GetString(IDS_VCT_FOUND);
            SetPaneText(strInfo);
            break;
	    }
	    
        case Board::FOUND_NO_VCT:
	    {
            strInfo = prefix + Utils::GetString(IDS_VCT_NOT_FOUND);
            SetPaneText(strInfo);
		    Utils::Beep();
            break;
	    }
    }

    if (!findInProgress)
    {
        m_Board.resetFindStatus();
    }

    return findInProgress;
}

bool CRenLibDoc::isFindActive()
{
    return m_Board.isFindActive();
}

//------------------------------------------------------------------------
// End of File
//------------------------------------------------------------------------
