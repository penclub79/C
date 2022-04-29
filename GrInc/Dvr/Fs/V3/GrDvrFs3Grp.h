/*
	GAUSE DVR file system v2 group

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrDvrFs3Grp
#define	_EPC_GrDvrFs3Grp

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrBufTool.h>

#include <GrTaskBase.h>
#include <GrTaskCli.h>

#include <Dvr/Fs/V3/GrDvrFs3Base.h>
#include <Dvr/Fs/V3/GrDvrFs3Disk.h>

#ifdef LINUX_APP

#include <sys/uio.h>

#else

#include <EmuLnx\GrElaBase.h>

#endif
	
//====================================================================
//constance

#define E_GrDvrFs3GrpGthBufSize				0x400000
#define	E_GrDvrFs3GrpGthFrmCnt				512

//#define E_GrDvrFs3GrpDbgMsgView
//#define	E_GrDvrFs3GrpDbgTailCort

//====================================================================
//global macro

//====================================================================
//global type

typedef	struct St_GrDvrFs3GrpAdoSync
{
	void*	PtrBuf;		// gather buffer position
	__u32	_rsvd0;
	__u64	Hndl;
}	*Ptr_GrDvrFs3GrpAdoSync;

typedef	struct St_GrDvrFs3GrpChInfo 
{
	__u8	AdoPrgs;		// audio reserved progress
	__u8	_rsvd1;
	__u8	_rsvd2;
	__u8	_rsvd3;
	St_GrDvrFs3GrpAdoSync	AdoSync[E_GrDvrFs3AdoSyncMax];	// reserved handle table
}	*Ptr_GrDvrFs3GrpChInfo;

typedef	struct St_GrDvrFs3GrpBdtLst 
{
	Ptr_GrDvrFs3Bdt	PtrBdt;
	__u32	BlkCnt;
}	*Ptr_GrDvrFs3GrpBdtLst;

typedef struct St_GrDvrFs3GrpGthItem
{
	St_GrDvrAvHd	AvHd;
}	*Ptr_GrDvrFs3GrpGthItem;

//====================================================================
//class

class	Cls_GrDvrFs3Grp
{
	private:

		Cls_GrTaskCli*	m_ObjMng;		// manager

		Ptr_GrBufTsMng	m_RecBufMng;
		__u16						m_RecOwnerId;
		__u16						m_RecThrdId;

		// disk
		__u8	m_GrpType;			// group type
		__u8	m_DiskCnt;			// group disk count
		BOOL8	m_IsRecAble;		// record able
		BOOL8	m_IsDefective;	// defective
		__u8	m_DiskMap;			// disk map
		Cls_GrDvrFs3Disk*	m_DiskTbl[E_GrDvrFs3MaxDriveCnt];		// disk table

		__u32	m_AdoUnitSize;	// audio data size

		BOOL8	m_IsOvwtMode;	// overwrite mode
		BOOL8	m_IsOvwting;	// now status is overwriting

		__u8	m_RecIntIdx;	// record disk internal index
		__u32	m_RecBlk;			// record block index
		__u32	m_RecId;			// record id

		// gathering
		St_GrDvrFs3GrpGthItem	m_GthTbl[E_GrDvrFs3GrpGthFrmCnt];
		__u32	m_GthAlloc;		// gather buffer allocated size
		__u32	m_GthInSize;	// gather in size
		__u32	m_GthInFrm;		// gather in frame count
		St_GrDvrFs3BlkAddInfo	m_GthTmb;
		Def_GrTime	m_GthBaseTime;
		struct iovec	m_GthIov[E_GrDvrFs3GrpGthFrmCnt*2];

	protected:
		//local
		void	LcDiskTblBuild( void );
		BOOL8	LcChkRecIdOvlp( void );
		void	LcBlockRecUpdt( void );
		void	LcRebuildRecId( void );

		BOOL8	LcBlockEmptyFind( __u8* A_PtrRtIntIdx, __u32* A_PtrRtBlkIdx );
		__u32	LcBlockMinRecIdFind( __u8* A_PtrRtIntIdx, __u32* A_PtrRtBlkIdx );
		__u32	LcBlockMaxRecIdFind( __u8* A_PtrRtIntIdx, __u32* A_PtrRtBlkIdx );

		BOOL8	LcPacketReplace( __u64 A_Hndl, Ptr_GrDvrAvHd A_PtrPkt );

		BOOL8	LcHndlInfoGet( __u64 A_Hndl, __u8* A_PtrRtIntIdx, 
			__u64* A_PtrRtRdaPos, __u32* A_PtrRtBlkId, __u32* A_PtrRtRecId, 
			__u16* A_PtrRtOvwtId, __u32* A_PtrRtBlkOfs, __u32* A_PtrRtBlkRecSize );

		__u64	LcTimeFindFw( Def_GrTime* A_PtrTime );
		__u64	LcTimeFindBw( Def_GrTime* A_PtrTime );

		// gather
		BOOL8	LcGthPktAdd( __u8 A_Ch, __u8 A_Type, __u8 A_Evt, __u8 A_Res, __u8 A_SkipCnt, __u8 A_SeqId, 
			Def_GrTime A_TimeSec, __u32 A_TimeMili, BOOL8 A_IsSm, void* A_PtrBuf, __u32	A_StrmSize );
		void	LcGthBufFlush( BOOL8 A_IsBdtUpdt );
		BOOL8	LcGthBufAlloc( __u32 A_ReqMin, __u32 A_ReqMax );
		void	LcGthStrmBufFree( void );

		__u32	LcBlockFwRecIdFind( __u32 A_RecId, __u8* A_PtrRtIntIdx, __u32* A_PtrRtBlkId, __u16* A_PtrRtOvwtId, __u32* A_PtrRtRecSize );
		__u32	LcBlockBwRecIdFind( __u32 A_RecId, __u8* A_PtrRtIntIdx, __u32* A_PtrRtBlkId, __u16* A_PtrRtOvwtId, __u32* A_PtrRtRecSize );

		void	LcLogRefNext( Ptr_GrDvrLogV2Ctrl A_PtrCtrl );
		void	LcLogRefPrev( Ptr_GrDvrLogV2Ctrl A_PtrCtrl );
		void	LcLogSort( Ptr_GrDvrLogV2Ctrl A_PtrCtrl );

	public:
    Cls_GrDvrFs3Grp( Cls_GrTaskCli* A_ObjMng, __u8 A_DskMap, BOOL8 A_IsOvrtMode, BOOL8 A_IsAutoFix,
			Ptr_GrBufTsMng A_RecBufMng, __u16 A_RecOwnerId, __u16 A_RecThrdId );
    virtual		~Cls_GrDvrFs3Grp();

		BOOL8	IsRecAble( void );

		__u8	GroupStatusGet( void );
		void	RecSizeGet( __u64* A_PtrRtTotal, __u64* A_PtrRtRec );
		void	RecTimeRngGet( Ptr_GrTime A_PtrRtTimeSt, Ptr_GrTime A_PtrRtTimeEd );

		BOOL8	StreamAdd( __u8 A_Ch, __u8 A_Type, __u8 A_Evt, __u8 A_Res, __u8 A_SkipCnt, __u8 A_SeqId, 
			Def_GrTime A_TimeSec, __u32 A_TimeMili, BOOL8 A_IsSm, void* A_PtrBuf, __u32	A_StrmSize );		// video packet add

		void	ReadForward( Ptr_GrDvrFs3SeekCtrl A_PtrCtrl, BOOL8 A_IsBufChg = TRUE );
		void	ReadBackward( Ptr_GrDvrFs3SeekCtrl A_PtrCtrl, BOOL8 A_IsBufChg = TRUE );
		void	ReadBegin( Ptr_GrDvrFs3SeekCtrl A_PtrCtrl );
		void	ReadEnd( Ptr_GrDvrFs3SeekCtrl A_PtrCtrl );
		void	ReadByTimeFw( Ptr_GrDvrFs3SeekCtrl A_PtrCtrl, Def_GrTime A_Time );
		void	ReadByTimeBw( Ptr_GrDvrFs3SeekCtrl A_PtrCtrl, Def_GrTime A_Time, Def_GrTime A_TimeLimLow );
		void	ReadByHndlFw( Ptr_GrDvrFs3SeekCtrl A_PtrCtrl, __u64 A_Hndl );
		void	ReadByHndlBw( Ptr_GrDvrFs3SeekCtrl A_PtrCtrl, __u64 A_Hndl );

		__u32	DayMapGet( Def_GrTime A_TimeSt, Def_GrTime A_TimeEd );
		BOOL8	MinMapGet( Def_GrTime A_TimeSt, void* A_PtrBuf, __u32 A_Cnt );
		Def_GrTime	RecDayFindFw( Def_GrTime A_TimeBase );
		Def_GrTime	RecDayFindBw( Def_GrTime A_TimeBase );

		BOOL8	LogAdd( Ptr_GrDvrLogV2Item A_PtrItem );
		void	LogFindStart( Ptr_GrDvrLogV2Ctrl A_PtrCtrl );
		void	LogFindNext( Ptr_GrDvrLogV2Ctrl A_PtrCtrl );
		void	LogFindPrev( Ptr_GrDvrLogV2Ctrl A_PtrCtrl );

		BOOL8	ScanRecSize( Ptr_GrDvrFs3RecSizeReq A_PtrReq );	// result = finish
};
	
//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif


