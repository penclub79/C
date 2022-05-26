/*
	buffer tool - on WIN32

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrBufTool
#define	_EPC_GrBufTool

//====================================================================
// uses

#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
	
//====================================================================
//constance

//variable queue
#define E_GrBufVqAlignMask			0xFFFFFFFC		/* 4 byte align mask */
#define E_GrBufVqHdSize					4							/* variable size queue header size */
#define E_GrBufVqHdIdNormal			0x88000000		/* normal header */
#define E_GrBufVqHdIdEnd				0x99000000		/* end marker - not use later */
#define E_GrBufVqHdIdMask				0xFF000000
#define E_GrBufVqHdSizeMask			0x00FFFFFF		/* include header size */

//link allocator
#define	E_GrBufLaAtbNone				0
#define	E_GrBufLaAtbFree				1
#define	E_GrBufLaAtbUsed				2

#define E_GrBufLaPageUnit				0x1000
#define E_GrBufLaPageShift			12
#define E_GrBufLaPageOfsMask		(E_GrBufLaPageUnit - 1)

//map allocator
#define E_GrBufMaPageSize				0x4000
#define E_GrBufMaPageShft				14
#define E_GrBufMaPageMask				0xFFFFC000

// client slot
#define E_GrBufCsMaxClientCnt		16
#define E_GrBufCsBlkMaxCnt			(8192 - 1)
#define E_GrBufCsBlkMinShft			6
#define E_GrBufCsBlkMaxShft			15
#define E_GrBufCsBlkMinSize			(1 << E_GrBufCsBlkMinShft)
#define E_GrBufCsBlkMaxSize			(1 << E_GrBufCsBlkMaxShft)

// thread slot
#define	E_GrBufTsMaxThrdCnt			4
#define E_GrBufTsMaxOwnerCnt		8
#define E_GrBufTsBlkMaxCnt			(0x8000 - 2)
#define E_GrBufTsBlkMinShft			6
#define E_GrBufTsBlkMaxShft			16
#define E_GrBufTsBlkMinSize			(1 << E_GrBufTsBlkMinShft)
#define E_GrBufTsBlkMaxSize			(1 << E_GrBufTsBlkMaxShft)


//====================================================================
//global macro

//====================================================================
//global type

typedef	struct St_GrBufQue 
{
	__s32			InPos;				//without header size
	__s32			OutPos;				//without header size
	__s32			BufSize;			//without header size
	__s32			SizeItem;
}	*Ptr_GrBufQue;

typedef	struct St_GrBufVarQue
{
	__u32		InPos;				// include header size
	__u32		OutPos;				// include header size
	__u32		BufSize;			// include header size
	__u32		AlgSize;			// align size
	__u32		AlgMask;			// align mask
	__u32		Ofs;					// start position
}	*Ptr_GrBufVarQue;

typedef	struct St_GrBufRefQue
{
	__u32		InPos;				//include header size
	__u32		OutPos;				//include header size 
	__u32		RefPos;				//include header size
	__u32		ScnPos;				//include header size
	__u32		BufSize;			//include header size
	__u32		AlgSize;			//align size
	__u32		AlgMask;			//align mask
	__u32		Ofs;					//start offset
}	*Ptr_GrBufRefQue;

typedef	struct St_GrBufLnkAlcItm
{
	__u16	Pos;
	__u16	Next;			
	__u16	Cnt;			//block count
	__u8	Atb;
	__u8	_rsvd;
}	*Ptr_GrBufLnkAlcItm;

typedef	struct St_GrBufLnkAlcMng 
{
	__u32	PosBase;	//base position
	__u16	ItmMax;		//item max count
	__u16	MaxLnk;		//max link count
}	*Ptr_GrBufLnkAlcMng;

typedef	struct	St_GrBufMaItm
{
	__u16	Id;				// master page index + 1, 0 = not allocated
	__s8	LockCnt;	// lock and allocate count , 0 = not allocated , not need slave pages.
	__u8	_rsvd;
}	*Ptr_GrBufMaItm;

typedef	struct	St_GrBufMaMng
{
	__u32	BufPos;		//buffer position
	__u32	PageMax;	//max page
}	*Ptr_GrBufMaMng;

typedef struct St_GrBufCsItem
{
	__u16	Map;
	__u16	Cnt;
}	*Ptr_GrBufCsItem;

typedef struct St_GrBufCsMng 
{
	__u16	BlkShft;		// block shift size
	__u16	BlkCnt;
	__u32	Items[E_GrBufCsBlkMaxCnt];
}	*Ptr_GrBufCsMng;

typedef struct St_GrBufTsItem
{
	__u8	MapAloc;
	__u8	_rsvd0;
	__u16	BlkCnt;
	__u8	MapRel[E_GrBufTsMaxThrdCnt];		// release map
}	*Ptr_GrBufTsItem;

typedef struct St_GrBufTsMng
{
	__u32	BlkShft;		// block size shifter
	__u32	BlkCnt;			// total block count
	__s32		BlkAlc;			// allocated block count
	__u32	_rsvd0;			// reserved
	struct St_GrBufTsItem	Items[E_GrBufTsBlkMaxCnt];
}	*Ptr_GrBufTsMng;


//====================================================================
//class

//====================================================================
//global function

// queue
void		GrBufQueInit( struct St_GrBufQue* A_PtrBuf, __s32 A_ItemSize, __s32 A_BufSize );
void*		GrBufQueGetInPtr( struct St_GrBufQue* A_PtrBuf );
void		GrBufQueIn( struct St_GrBufQue* A_PtrBuf );
void*		GrBufQueGetOutPtr( struct St_GrBufQue* A_PtrBuf );
BOOL8		GrBufQueOut( struct St_GrBufQue* A_PtrBuf );
void		GrBufQueClear( struct St_GrBufQue* A_PtrBuf );
__u32		GrBufQueGetInCount(struct St_GrBufQue* A_PtrBuf);

// variable queue
void		GrBufVqInit(	struct St_GrBufVarQue* A_PtrBuf, __u32 A_BufSize, __u32 A_AlignSize );
void*		GrBufVqGetInPtr( struct St_GrBufVarQue* A_PtrBuf, __u32 A_Size );
void		GrBufVqIn( struct St_GrBufVarQue* A_PtrBuf );
void		GrBufVqSizeIn(struct St_GrBufVarQue* A_PtrBuf, __u32 A_Size);
void*		GrBufVqGetOutPtr( struct St_GrBufVarQue* A_PtrBuf );
BOOL8		GrBufVqOut( struct St_GrBufVarQue* A_PtrBuf );
void		GrBufVqReset( struct St_GrBufVarQue* A_PtrBuf );
__u32		GrBufVqGetUseSize( struct St_GrBufVarQue* A_PtrBuf );
void*		GrBufVqGetPtrByPos(struct St_GrBufVarQue* A_PtrBuf, __u32 A_Pos, __u32* A_PtrRtCurPos);
BOOL8		GrBufVqGetNextPosByPos(struct St_GrBufVarQue* A_PtrBuf, __u32 A_Pos,__u32* A_PtrRtNextPos);

// reference queue
void		GrBufRqInit(	struct St_GrBufRefQue* A_PtrBuf, __u32 A_BufSize, __u32 A_AlignSize );
void*		GrBufRqGetInPtr( struct St_GrBufRefQue* A_PtrBuf, __u32 A_Size );
void		GrBufRqIn( struct St_GrBufRefQue* A_PtrBuf );
void*		GrBufRqGetOutPtr( struct St_GrBufRefQue* A_PtrBuf );
BOOL8		GrBufRqOut( struct St_GrBufRefQue* A_PtrBuf );
void		GrBufRqReset( struct St_GrBufRefQue* A_PtrBuf );
__u32		GrBufRqGetUseSize( struct St_GrBufRefQue* A_PtrBuf );
void*		GrBufRqGetRefPtr( struct St_GrBufRefQue* A_PtrBuf );
BOOL8		GrBufRqRefNext( struct St_GrBufRefQue* A_PtrBuf );
void		GrBufRqRefReset( struct St_GrBufRefQue* A_PtrBuf );
void		GrBufRqScanReset( struct St_GrBufRefQue* A_PtrBuf );
void*		GrBufRqGetScanPtr( struct St_GrBufRefQue* A_PtrBuf );
BOOL8		GrBufRqScanNext( struct St_GrBufRefQue* A_PtrBuf );

// link allocator - 4kbyte page unit
void		GrBufLaInit( struct St_GrBufLnkAlcMng* A_PtrBuf, __u32 A_BufSize );
void*		GrBufLaAlloc( struct St_GrBufLnkAlcMng* A_PtrBuf, __u32 A_Size );
void		GrBufLaFree( struct St_GrBufLnkAlcMng* A_PtrBuf, void* A_PtrAloc );
void		GrBufLaInfo( struct St_GrBufLnkAlcMng* A_PtrBuf, __u32* A_PtrRtLnkCnt, __u32* A_PtrRtFree, 
										__u32* A_PtrRtUsed );
BOOL8		GrBufLaIsExist( struct St_GrBufLnkAlcMng* A_PtrBuf, void* A_PtrAloc );	//item exist
void		GrBufLaReset( struct St_GrBufLnkAlcMng* A_PtrBuf );

// map allocator - 16kbyte page unit
void		GrBufMaInit( struct St_GrBufMaMng* A_PtrBuf, __u32 A_BufSize );
void*		GrBufMaAlloc( struct St_GrBufMaMng* A_PtrBuf, __u32 A_Size );
void		GrBufMaLock( struct St_GrBufMaMng* A_PtrBuf, void* A_PtrAloc );
void		GrBufMaFree( struct St_GrBufMaMng* A_PtrBuf, void* A_PtrAloc );
void		GrBufMaRemove( struct St_GrBufMaMng* A_PtrBuf, void* A_PtrAloc );	
BOOL8		GrBufMaInfo( struct St_GrBufMaMng* A_PtrBuf, __u32* A_PtrUsed, __u32* A_PtrFree );
void		GrBufMaReset( struct St_GrBufMaMng* A_PtrBuf );
void		GrBufMaCorrect( struct St_GrBufMaMng* A_PtrBuf );

// client slot
void		GrBufCsInit( struct St_GrBufCsMng* A_PtrBuf, __u32 A_BufSize );
void*		GrBufCsAlloc( struct St_GrBufCsMng* A_PtrBuf, __u32 A_Size, __u16 A_CliMap );
void		GrBufCsFree( struct St_GrBufCsMng* A_PtrBuf, void* A_PtrAloc, __u16 A_CliIdx );
void		GrBufCsClientFree( struct St_GrBufCsMng* A_PtrBuf, __u16 A_CliIdx );

// thread slot
void		GrBufTsInit( struct St_GrBufTsMng* A_PtrBuf, __u32 A_BufSize );
void*		GrBufTsAlloc( struct St_GrBufTsMng* A_PtrBuf, __u32 A_Size, __u8 A_OwnerMap );
void		GrBufTsFree( struct St_GrBufTsMng* A_PtrBuf, void* A_PtrAlloc, __u16 A_OwnerIdx, __u16 A_ThrdIdx );
void		GrBufTsMapFree( struct St_GrBufTsMng* A_PtrBuf, void* A_PtrAlloc, __u8 A_OwnerMap, __u16 A_ThrdIdx );
void		GrBufTsOwnerFree( struct St_GrBufTsMng* A_PtrBuf, __u16 A_OwnerIdx, __u16 A_ThrdIdx );
BOOL8		GrBufTsChkFree( struct St_GrBufTsMng* A_PtrBuf, __u32 A_FreeSize );


//====================================================================
//out Environment conditional
#endif
