// RenLibDoc.h : interface of the CRenLibDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RENLIBDOC_H__72EC9D4B_F85E_11D3_92A3_947F7AC2F525__INCLUDED_)
#define AFX_RENLIBDOC_H__72EC9D4B_F85E_11D3_92A3_947F7AC2F525__INCLUDED_

#include "Board.h"
#include "Game.h"
#include "RenJS.h"
#include "Buf.h"
#include "Wzq.h"
#include "Pdb.h"
#include "Pos.h"
#include "Sgf.h"
#include "Bdt.h"
#include "Rdf.h"
#include "MoveNode.h"
#include "SearchList.h"
#include "Position.h"
#include "Stack.h"
#include "MoveList.h"
#include "LibraryFile.h"
#include "RenLibApplet.h"
#include <vector>
#include "MergeComment.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRenLibDoc : public CDocument
{
protected: // create from serialization only
	CRenLibDoc();
	DECLARE_DYNCREATE(CRenLibDoc)

public:
	enum Mode
	{
		DISPLAY_MODE,
		UPDATE_MODE,
        PROBLEM_MODE
	};

	enum WinKind
	{
		VCF,
		VCT
    };

public:
	static CString getProgramDate();
	static CString getProgramVersion();
	static BYTE    getMajorProgramVersion();
	static BYTE    getMinorProgramVersion();
	static BYTE    getBugFixProgramVersion();
	static int     getVersion();

	bool OpenLibraryCommand(const CString& strFile, const CString& strTitle, const CString& strBackup);
	bool MergeLibraryCommand(const CString& strFile);
	void NewLibraryCommand();
	void AddGame(Game& aGame);
	void AddRenJS(RenJS& aRenjs);
	void AddRenJS(RenJS::Node* node, int& number, MoveNode*& pCurrentMove, int& nNewMoves);
	void AddBuf(Buf& aBuf);
	void AddWzq(Wzq& aWzq);
	void AddPdb(Pdb& aPdb);
    void AddPos(Pos& aPos);
	void AddSgf(Sgf& aSgf);
	void AddBdt(Bdt& aBdt);
	void AddRdf(Rdf& aRdf);
	void UpdateBoard(bool ShowAutoLinkVariants, bool ShowChangedOrderOfMovesVariants);
	Board* GetBoard();
	CString getOneLineComment();
	void    getOneLineComments(std::vector<CString>& comments);
	CString getMultiLineComment();
	CString GetBoardText(CPoint Pos);
	CString GetHighestAutoBoardText();
	void SetOneLineComment(const CString& strOneLine);
	void SetMultiLineComment(const CString& strMultiLine);
	void ButtonCommand(CPoint Pos, bool MakeSingleVariant, bool& hasChangedBranch);
	void RButtonCommand(CPoint Pos);
	void BoardTextCommand(CPoint Pos, CString strBoardText);
	void BackCommand();
	bool IsForwardCommand();
	void ForwardCommand();
	bool IsDownCommand();
	void DownCommand();
	void InsertPassMove();
	void DisplayPassMove();
	void UpCommand();
	void FirstCommand();
	void SetTitle();
	BOOL SaveCommand(bool Confirm, bool AskFileName = false, bool cancelButton = true);
	void SaveBranchCommand(const CString& strFile);
	int GetCurrentMove();
	void SetMode(Mode mode);
	void DeleteCommand();
	void DeleteVariantsCommand();
	void ClearAllMarksCommand();
	void MarkCommand();
	bool IsMark();
	bool IsStartPosition();
	bool FindMarkCommand();
	bool FindCommentCommand(const CString& strCommentToFind);
	bool FindBoardTextCommand(const CString& strBoardTextToFind);
	bool FindMoveCommand(int MoveNo);
	void FindVariantPositionCommand();
	bool FindSamePositionCommand();
	bool autoFindSamePositionCommand();
	bool FindSimilarPositionCommand();
	bool FindNextCommand();
	bool FindPreviousCommand();
	void EmptyPaneText();
	void SetPaneText(const CString& text);
	bool IsFindInProgress();
	int  NoOfMovesInBranch();
	int  NoOfVariantMoves();
	bool hasVariants();
    bool hasPassMove();
	bool hasPassVariant();
	void StartPositionCommand();
	bool FindStartPositionCommand(bool Notify);
	bool MakeRenartistAppletCommand(const CString& strFile);

	bool MakeRenLibAppletCommand(
		const CString& strFile, CString& strComment,
		const RenLibApplet::Information& info);
	
	bool RenjuclassDiagramCommand(const CString& strFile);
	void TextBoardCommand(const CString& strFile);
	void ExportTextBoardCommand(const CString& strDirectory);

	bool ExportRenLibAppletCommand(
		const CString& strDirectory,
		CString& strFile, COLORREF* custColors);

	bool ExportGameCollectionCommand(CString& strFile, COLORREF* custColors);

	void ClearAutoFindSamePositionList();
	void SetIsWinPosition(WinKind kind, bool isWin);
	bool IsWinPosition(WinKind kind);
    bool SavePosCommand(const CString& strFile);
    bool SavePdbCommand(const CString& strFile);
    void SaveBdtCommand(const CString& strFile);
    void SaveBufCommand(const CString& strFile);
	bool addComment(MoveNode* pMove, CString& strNewOneLine, CString& strNewExtra);
	bool addBoardText(MoveNode* pMove, CString& boardText);
	void addAttributes(MoveNode* pMove, MoveNode* pFrom);
	void addAttributes(MoveNode* pMove, MoveNode* pFrom,
                          bool& bMark, bool& bMove, bool& bStart);
	void initMergeComment(bool keepPrevious = false);
	bool hasMergeComment();
	bool mergeComment();
	bool isMergeComment();
	bool checkConvertToStandard();
	void ConvertToStandardCommand();
	void removeEmptyBoardTexts();
	MoveNode* GetCurrentMoveNode();
	bool isUpdateMode();
	bool isDisplayMode();
    bool isProblemMode();
	CString getLastFile();
	virtual BOOL IsModified();
    bool findWin(WinKind kind);
    bool checkFindWin(Board::WinMarker marker, const CString& prefix);
    bool isFindActive();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRenLibDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void OnCloseDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	protected:
	virtual BOOL SaveModified();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRenLibDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRenLibDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	Board		m_Board;

	MoveList	m_MoveList;

	Mode		m_eMode;

	SearchList	m_SearchList;
	SearchList	m_AutoSearchList;
	SearchList	m_VariantList;

	CString  m_strCommentToFind;
	CString  m_strBoardTextToFind;
	int      m_MoveNoToFind;

	Position m_Position;

	bool mIsFindInProgress;
	bool mIsNext;

    MergeComment::Selection mSelection;

	CString m_strFile;
	CString m_strBackup;
	CString m_strTitle;

	MoveNode* m_WinMoveNode;
    bool      m_IsWinPosition[2];

	CString m_strLastFile;

	BOOL    m_IsModified;

private:
	void		SetModifiedFlag(BOOL isModified = TRUE, BOOL isUpdate = FALSE);
	void		Init();
	bool		AddLibrary(LibraryFile& libFile, bool FullTree);
	int			GetNextFree();
	void		ReadOldComment(LibraryFile& libFile, CString& strOneLine, CString& strMultiLine);
	void		ReadNewComment(LibraryFile& libFile, CString& strOneLine, CString& strMultiLine);
	void		ReadBoardText(LibraryFile& libFile, CString& strBoardText);
	void		AddMove(MoveNode* pMove, MoveNode* pNewMove);
	void		DisplayMove(CPoint Pos);
	void		InsertMove(CPoint Pos);
    void        InsertProblemMove(CPoint Pos, bool blackMove);
    int         FindProblemMove(CPoint Pos);
	void 		DeleteMove(MoveNode* pCurrentMove, MoveNode* pPreviousMove, bool keepText=true);
	void		DeleteVariants(MoveNode* pCurrentMove, MoveNode* pPreviousMove);
	bool		GetSingleVariant(CPoint& Pos);
	int			GetTotal(MoveNode* pMove);
	void		SetPosition(CPoint Pos);
	int			GetNumberOfMarks(MoveNode* pMove);
	void		ClearMarks(MoveNode* pMove);
	void		ClearStartPosition(MoveNode* pMove);
	MoveNode*	FindStartPosition(MoveNode* pMove);

	MoveNode*	GetVariant(MoveNode* pMove, CPoint Pos);
	bool		FindMoveNode(MoveNode* pMoveToFind);
	MoveNode*	GetVariant(CPoint Pos);
	bool        IsExistingMove(CPoint Pos);
	void		DeleteTree(MoveNode* pMove);

	void		WriteLibrary(const CString& strFile, int MoveNo);

	void		WriteMove(LibraryFile& libFile, int MoveNo,
                          int& nMoves, int& nComments, int& nBoardTexts, int& nMarks, bool isOldFormat);

	void		WriteNode(LibraryFile& libFile, MoveNode* pMove, bool FullTree,
                          int& nMoves, int& nComments, int& nBoardTexts, int& nMarks);
	
	void		WriteComment(LibraryFile& libFile, CString& strOneLine, CString& strMultiLine);
	void		WriteBoardText(LibraryFile& libFile, CString& strBoardText);
	void		WriteText(LibraryFile& libFile, CString& strText);

	bool		FindNextPrevious(bool isNext);
	bool		IsMarkFound(MoveNode* pMove);
	bool		IsCommentFound(MoveNode* pMove);
	bool		IsBoardTextFound(MoveNode* pMove);
	bool		IsMoveFound(MoveNode* pMove, MoveList& theMoveList);
	void		MakeMoveList(Position::MoveList& theMoves, MoveList& theMoveList);
	bool		FindPosition(bool findSimilar);
	void		SetPreviousVariant();
	void		SetFindInfo(bool isFound);
	void		SetTextCommand(MoveNode* pMove, const CString& strBoardText);
	void		ConvertXY(MoveNode* pMove, int Fx, int Fy, bool convertSibling = false);
	void		ConvertYX(MoveNode* pMove, int Fx, int Fy, bool convertSibling = false);
	void		ConvertToStandard(MoveList& moveList);
	bool		checkConvertToStandard(MoveList& moveList);
	void		mergeLibrary(MoveNode* pMove);
	void		mergeSibling(MoveNode* pMove);
	int			NoOfVariantMoves(MoveNode* pMove);
	MoveNode*   getPassVariant(MoveNode* pMove);
	int			NoOfBoardTexts(MoveNode* pMove);
	void		removeEmptyBoardTexts(MoveNode* pMove);
	void		splitBoardText(MoveNode* pMove);
	bool		FindNextPrevious(SearchList& searchList, bool isNext);
	void		SetFindInfo(SearchList& searchList, bool isFound);
	bool		isFirstMoveH8Only(int moveNo);
	void		moveBack();
	MoveNode*	FindNullPoint(MoveNode* pMove);
	bool		hasPassMove(int moveNo);
	void		mergeCommentAll();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RENLIBDOC_H__72EC9D4B_F85E_11D3_92A3_947F7AC2F525__INCLUDED_)
