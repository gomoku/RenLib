// Pdb.h: interface for the Pdb class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(SUPPORT_WRITE_PDB)
#define SUPPORT_WRITE_PDB
#endif

#include "Game.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PDB_HEADER_SIZE 78
#define OFST_NUM_ITEM 0x4c
#define OFST_CREATORID 0x40
#define SIZE_CREATORID 4
#define GORENJER_CREATORID "5Ren"
  
#define BOARD_TYPE_POS_DISP        0x0001/* ボードの枠に1,2 and A,B を表示 */
#define BOARD_TYPE_CHECK_WIN       0x0002/* 勝敗判定あり／なし */
#define BOARD_TYPE_AUTOSTOP4       0x0004/* 4自動止め */
#define BOARD_TYPE_MOVABLE         0x0008/* 石を置けるかどうか */
#define BOARD_TYPE_PROF_MODE       0x0010/* ５珠２題打ちモード */
#define BOARD_TYPE_MOVE5_SHOWING   0x0100/* ５珠表示中 */
#define BOARD_TYPE_MOVE5_AVAILABLE 0x0200/* ５珠有効 */
#define BOARD_TYPE_WRONG_MOVE      0x0800/* 無効な手 */
#define BOARD_TYPE_PROBLEM_TYPE    (0x3 << SHIFT_PROB_TYPE) /* 詰め連珠のタイプ */
#define BOARD_TYPE_PROBLEM_MODE    0x4000/* 詰め連珠モード */
#define BOARD_TYPE_DEBUG_MODE      0x8000/* デバッグモード */

#define WHITE 2
#define BLACK 1

typedef unsigned short ushort;

#define SUM_WITH_PREV (BLACK + WHITE)

#define PDB_EMPTY 0
#define PDB_WHITE 1
#define PDB_BLACK 2

typedef struct {
    UCHAR title[64];
    UCHAR bPlayer[32];
    UCHAR wPlayer[32];
    UCHAR comment[1024];
    UCHAR pos5_x;
    UCHAR pos5_y;
    UCHAR pos5_valid;
} extraInfo_t;

#ifdef SUPPORT_WRITE_PDB
class MoveList;
#endif

class Pdb  
{
public:
    Pdb();
	~Pdb();

    bool	OpenFile(const CString& strFile);
    bool	Done();
    bool	Next();
#ifdef SUPPORT_WRITE_PDB
    bool Save(const CString& fileName, const MoveList& aMoveList);
#endif

	Game&	getGame();
	CString	getComment();
	CString	getFilePath();

private:
    CFile	m_pdbFile;
    int	m_nRecord;
    int numRec;
    int currentRec;
    CString	m_strMessage;
    int	m_nNoMoves;
    typedef struct {
        UCHAR x:4;
        UCHAR y:4;
        UCHAR type;
    } trace_t;

    typedef struct {
        ULONG offset;
        UCHAR attr;
        UCHAR uid[3];
    } recordList_t;

    typedef struct {
        USHORT board_type;
        UCHAR moveCnt;
        UCHAR maxCnt;
        UCHAR startStone;
#ifdef INCLUDE_DB		       /* 定義はしないが DBには入る */
        trace_t trace[1];
        UCHAR title[1];		       /* save するときの見出し */
#endif
    } goRenDB_t;

#ifdef SUPPORT_WRITE_PDB
#pragma pack(1)
    typedef struct {
        char name[32];
        char dummy1[4];
        unsigned int createTime;           /* sec from 1904/1/1 */
        unsigned int updateTime;
        char dummy2[16];
        char type[4];               /* 'DATA' */
        char creatorID[4];          /* creatorID */
        char dummy3[8];
        USHORT numOfRecords;        /* number of Records */
        recordList_t recordList;
    } pdb_t;

#pragma pack()
#endif
    int maxCnt;
    trace_t *tp;
    goRenDB_t db;
    recordList_t *rlist;
    ULONG *offsetTable;
    extraInfo_t eInfo;
#ifdef SUPPORT_WRITE_PDB
    trace_t trace[225];
#endif
  
    BYTE header[PDB_HEADER_SIZE];

    ULONG endianExchange(ULONG, int);
    void readExtraInfo(extraInfo_t *, unsigned short);
    bool isValidData(trace_t *, int);
  
    Game	m_Game;
    CString	m_strComment;
    CString	m_strFilePath;
};
