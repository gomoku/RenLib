// Pdb.cpp: implementation of the Pdb class.
//
//////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include "stdafx.h"
#include "RenLib.h"
#include "Pdb.h"
#ifdef SUPPORT_WRITE_PDB
#include "MoveList.h"
#include "MoveNode.h"
#endif
#include "Utils.h"
#include "Stack.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//--------------------------------------------------------------------
// const
//--------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Pdb::Pdb()
: m_nRecord(0),
  m_nNoMoves(0)
{
    maxCnt = 0;
    rlist = NULL;
    tp = NULL;
    numRec = 0;
    currentRec = 0;
    offsetTable = NULL;
}

//------------------------------------------------------------------------

Pdb::~Pdb()
{
    if (rlist)
    {
        free(rlist);
    }

    if (offsetTable)
    {
        free(offsetTable);
    }

    if (tp)
    {        
        free(tp);
    }
}

//------------------------------------------------------------------------

Game& Pdb::getGame()
{
    return m_Game;
}

//------------------------------------------------------------------------

CString Pdb::getComment()
{
    return m_strComment;
}

//------------------------------------------------------------------------

CString Pdb::getFilePath()
{
    return m_strFilePath;
}

//------------------------------------------------------------------------

bool Pdb::OpenFile(const CString& strFile)
{
    CFileException e;
    ULONG *tmpOffsetTable;
    int i;

    if (!m_pdbFile.Open( strFile, CFile::modeRead, &e))
    {
        return false;
    }

    if (m_pdbFile.GetLength() <= PDB_HEADER_SIZE)
    {
        /* no record */
        return false;
    }

    m_strFilePath = m_pdbFile.GetFilePath();

    m_pdbFile.Read(header, PDB_HEADER_SIZE);

    numRec = header[OFST_NUM_ITEM] * 0x100 + header[OFST_NUM_ITEM + 1];

    if (!numRec || strncmp((const char *)&header[OFST_CREATORID],
                           (const char *)GORENJER_CREATORID, 4))
    {
        return false;
    }

    if ((rlist = (recordList_t *)malloc(sizeof(recordList_t))) == NULL)
    {
        perror("couldnt alloc memory");
        return false;
    }


    if ((offsetTable = (ULONG *)malloc(sizeof(ULONG) * numRec)) == NULL)
    {
        perror("couldnt alloc memory");
        return false;
    }

    for (i = 0, tmpOffsetTable = offsetTable; i < numRec; i++, tmpOffsetTable++)
    {
        m_pdbFile.Read(rlist, sizeof(*rlist));
        rlist->offset = endianExchange(rlist->offset, sizeof(rlist->offset));
        *tmpOffsetTable = rlist->offset;
    }

    return true;
}

//------------------------------------------------------------------------

bool Pdb::Done()
{
    return (currentRec >= numRec);
}

//------------------------------------------------------------------------

bool Pdb::Next()
{
    ULONG offset;
    trace_t *tmptp;
    int i;

    m_Game.clear();

    m_strComment.Empty();

    offset = *(offsetTable + currentRec);
    Pdb::m_pdbFile.Seek(offset, CFile::begin);
    m_pdbFile.Read(&db, sizeof(db));
    db.board_type = (unsigned short)endianExchange(db.board_type, 2);
    currentRec++;

    if (db.board_type & BOARD_TYPE_PROBLEM_MODE)
    {
            return false;
    }

    if (maxCnt < db.maxCnt)
    {
        maxCnt = db.maxCnt;
        if ((tp = (trace_t *)realloc(tp, sizeof(trace_t) * maxCnt)) == NULL)
        {
            return false;
        }
    }

    m_pdbFile.Read(tp, sizeof(trace_t) * db.maxCnt);
    if (!isValidData(tp, db.maxCnt))
    {
            return false;
    }

    readExtraInfo(&eInfo, db.board_type);

    CString strRecord;

    strRecord.Format("Title: %s\r\nBlack: %s\r\nWhite: %s\r\n",
                     eInfo.title, eInfo.bPlayer, eInfo.wPlayer, eInfo.comment);

    m_strComment += strRecord;

    for (i = 0, tmptp = tp; i < db.maxCnt; i++, tmptp++)
    {
        m_Game.addPos(byte(tmptp->x * 16 + tmptp->y + 1));
    }

    if (eInfo.pos5_valid)
    {
        bool found;

        for (i = 0, tmptp = tp, found = false; (i < db.maxCnt) && !found; i++, tmptp++)
        {
            if ((tmptp->x == eInfo.pos5_x) && (tmptp->y == eInfo.pos5_y))
            {
                found = true;
            }
        }

        if (found)
        {
                    strRecord.Format("5->%d\r\n", i);
        }
        else
        {
                    strRecord.Format("5->%c%d(a-o, 1-15 coordinate)\r\n",
                                     'a' + eInfo.pos5_x, eInfo.pos5_y + 1);
        }

        m_strComment += strRecord;
    }

    if (Done())
    {
        m_pdbFile.Close();
    }

    return true;
}

/* exchange endian from Big Endian to Little Endian */
ULONG Pdb::endianExchange(ULONG data, int size)
{
    ULONG ret = data;

    switch(size)
    {
      case 2:
      {
          ret = (((data & 0xff00) >> 8) & 0xff) +
              ((data & 0xff) << 8);
          break;
      }
      case 4:
      {
          ret = (endianExchange(data & 0xffff, 2) << 16) +
              endianExchange(((data & 0xffff0000) >> 16) & 0xffff, 2);
          break;
      }
    }
    return(ret);
}

void Pdb::readExtraInfo(extraInfo_t *p, unsigned short type)
{
    static UCHAR buf[sizeof(extraInfo_t)];
    int len;
    UCHAR *ptr;

    memset(p, 0, sizeof(*p));

    ptr = buf;
    m_pdbFile.Read(buf, sizeof(buf));
    len = strlen((const char *)ptr);
    strncpy((char *)p->title, (const char *)ptr, len);
    ptr += len + 1;

    len = strlen((const char *)ptr);
    strncpy((char *)p->bPlayer, (const char *)ptr, len);
    ptr += len + 1;
    len = strlen((const char *)ptr);
    strncpy((char *)p->wPlayer, (const char *)ptr, len);
    ptr += len + 1;
    len = strlen((const char *)ptr);
    strncpy((char *)p->comment, (const char *)ptr, len);

    if ((type & (BOARD_TYPE_PROF_MODE | BOARD_TYPE_MOVE5_AVAILABLE)) ==
        (BOARD_TYPE_PROF_MODE | BOARD_TYPE_MOVE5_AVAILABLE))
    {
        /* date */
        ptr += len + 1;
        ptr += strlen((const char *)ptr) + 1;
        /* swap x/y because trace_t is reversed */
        p->pos5_y = *ptr++;
        p->pos5_x = *ptr;
        p->pos5_valid = 1;
    }
    else
    {
        p->pos5_valid = 0;
    }
}

bool Pdb::isValidData(trace_t *p, int cnt)
{
    int i;
    UCHAR oldType = PDB_WHITE;

    for (i = 0; i < cnt; i++, p++)
    {
        if (p->type == PDB_EMPTY)
        {
            return false;
        }

        if (oldType + p->type != SUM_WITH_PREV)
        {
            return false;
        }

        oldType = p->type;
    }
    return true;
}

//------------------------------------------------------------------------
#ifdef SUPPORT_WRITE_PDB
#define PDBFILE_MAX_SIZE (64 * 1024)
#define TIME_OFFSET (66 * 365 * 24 * 60 * 60 + 17 * 24 * 60 * 60 + 9 * 60 * 60)  // 1970/1/1 - 1904/1/1

bool Pdb::Save(const CString& fileName, const MoveList& aMoveList)
{
    Stack m_Stack;
    int number = 0;
    int bytes = PDB_HEADER_SIZE;
    CString pdbName;
    unsigned long offset;
    int numberOfRecords;
    unsigned char dummy = NULL;
    pdb_t *p;
    goRenDB_t appData, *ap;
    int idx, idx2;
    MoveNode* pMove = 0;
    int num;
    unsigned char type = PDB_BLACK;
    bool done;

    if (!m_pdbFile.Open(fileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
    {
        CString strMessage(Utils::GetString(IDS_MSG_OPEN_FILE, fileName));
        Utils::ShowMessage(strMessage, Utils::GetString(IDS_CAP_CREATE_PDB), MB_ICONERROR);
        return false;
    }

    if (!(p = (pdb_t *)malloc(sizeof(pdb_t))))
    {
        perror("couldnt alloc memory");
        return(false);
    }

    pdbName = fileName;
    idx = pdbName.ReverseFind('\\');
    if (idx >= 0)
    {
        idx2 = pdbName.ReverseFind('.');
        pdbName = pdbName.Mid(idx + 1, idx2 - (idx + 1));
    }

    memset(p, 0, sizeof(*p));
    strncpy(p->name, pdbName, sizeof(p->name));

    pMove = aMoveList.Get(1)->getDown();
    num = 1;

    done = (pMove == 0);

    while (!done)
    {
        if (pMove)
        {
            if (pMove->getRight())
                m_Stack.Push(num, pMove->getRight());

            pMove = pMove->getDown();
            bytes += 2;
            num++;
        }
        else
        {
            p->numOfRecords++;

            if (!m_Stack.IsEmpty())
            {
                m_Stack.Pop(num, pMove);
                bytes += num * 2;
            }
            else
            {
                done = true;
            }

            if (bytes >= PDBFILE_MAX_SIZE)
            {
                done = true;
            }
        }
    }

    p->createTime = time(NULL) + TIME_OFFSET;
    p->createTime = endianExchange(p->createTime, sizeof(p->createTime));
    p->updateTime = p->createTime;

    p->recordList.attr = 0x40;
    p->recordList.offset = sizeof(pdb_t) + sizeof(recordList_t) * (p->numOfRecords - 1);
    offset = p->recordList.offset;
    p->recordList.offset = endianExchange(p->recordList.offset, 4);

    numberOfRecords = p->numOfRecords;
    p->numOfRecords = (ushort)endianExchange(p->numOfRecords, sizeof(p->numOfRecords));

    strncpy(p->type, "DATA", 4);
    strncpy(p->creatorID, GORENJER_CREATORID, 4);

    m_pdbFile.Write(p, sizeof(*p));

    /* create offset table */
    num = 1;
    m_nRecord = 0;

    pMove = aMoveList.Get(1)->getDown();
    done = (pMove == 0);
    bytes = PDB_HEADER_SIZE;

    while (!done)
    {
        if (pMove)
        {
            if (pMove->getRight())
            {
                m_Stack.Push(num, pMove->getRight());
            }

            pMove = pMove->getDown();
            bytes += sizeof(trace_t);
            num++;
        }
        else
        {
            m_nRecord++;
            if (m_nRecord < numberOfRecords)
            {
                char buf[32];

                sprintf(buf, "No.%d", m_nRecord);
                p->recordList.offset = offset + num * sizeof(trace_t) + sizeof(goRenDB_t) +
                    strlen(buf) + 5;

                offset = p->recordList.offset;
                p->recordList.offset = endianExchange(p->recordList.offset, 4);
                m_pdbFile.Write(&p->recordList, sizeof(p->recordList));
            }

            if (!m_Stack.IsEmpty())
            {
                m_Stack.Pop(num, pMove);
                bytes += num * sizeof(trace_t);
            }
            else
            {
                done = true;
            }

            if (bytes >= PDBFILE_MAX_SIZE)
            {
                done = true;
            }
        }
    }

    /* save application data */
    num = 1;
    m_nRecord = 0;

    pMove = aMoveList.Get(1)->getDown();
    done = (pMove == 0);
    bytes = PDB_HEADER_SIZE;

    CPoint Coord = aMoveList.Get(1)->getPos();
    /* swap x/y */
    trace[0].y = Coord.x - 1;
    trace[0].x = Coord.y - 1;
    trace[0].type = type;
    type = 3 - type;

    ap = &appData;

    ap->board_type = BOARD_TYPE_MOVABLE;
    ap->board_type = (ushort)endianExchange(ap->board_type, sizeof(ap->board_type));

    while (!done)
    {
        if (pMove)
        {
            if (pMove->getRight())
            {
                m_Stack.Push(num, pMove->getRight());
            }

            Coord = pMove->getPos();
            trace[num].y = Coord.x - 1;
            trace[num].x = Coord.y - 1;
            trace[num].type = 3 - trace[num - 1].type;

            pMove = pMove->getDown();
            bytes += sizeof(trace_t);
            num++;
        }
        else
        {
            m_nRecord++;

            ap->moveCnt = num;
            ap->maxCnt = num;
            ap->startStone = (num % 2) ? PDB_WHITE : PDB_BLACK;

            m_pdbFile.Write(ap, sizeof(*ap));
            m_pdbFile.Write(trace, sizeof(trace_t) * num);

            char buf[32];

            sprintf(buf, "No.%d", m_nRecord);
            m_pdbFile.Write(buf, strlen(buf));


            for (int i = 0; i < 5; i++)
            {
                m_pdbFile.Write(&dummy, sizeof(dummy));
            }

            if (!m_Stack.IsEmpty())
            {
                m_Stack.Pop(num, pMove);
                bytes += num * sizeof(trace_t);
            }
            else
            {
                done = true;
            }

            if (bytes >= PDBFILE_MAX_SIZE)
            {                
                done = true;
            }
        }
    }

    m_pdbFile.Close();

    return(true);
}
#endif  // SUPPORT_WRITE_PDB
//------------------------------------------------------------------------
