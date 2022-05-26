/*
	GAUSE DVR file system v2 group

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrDvrFs2Grp
#define	_EPC_GrDvrFs2Grp

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrBufTool.h>

#include <GrTaskBase.h>
#include <GrTaskCli.h>

#include <Dvr/Fs/V2/GrDvrFs2Base.h>
#include <Dvr/Fs/V2/GrDvrFs2Disk.h>
	
//====================================================================
//constance

#define E_GrDvrFs2GrpGthBufSize				0x200000
#define E_GrDvrFs2GrpGthBufDwCnt			(E_GrDvrFs2GrpGthBufSize >> 2)

#define E_GrDvrFs2GrpDbgMsgView

//====================================================================
//global macro

//====================================================================
//global type

typedef	struct St_GrDvrFs2GrpAdoSync
{
	void*	PtrBuf;		// gather buffer position
	__u32	_rsvd0;
	__u64	Hndl;
}	*Ptr_GrDvrFs2GrpAdoSync;

typedef	struct St_GrDvrFs2GrpChInfo 
{
	__u8	AdoPrgs;		// audio reserved progress
	__u8	_rsvd1;
	__u8	_rsvd2;
	__u8	_rsvd3;
	St_GrDvrFs2GrpAdoSync	AdoSync[E_GrDvrFs2AdoSyncMax];	// reserved handle table
}	*Ptr_GrDvrFs2GrpChInfo;

typedef	struct St_GrDvrFs2GrpBdtLst 
{
	Ptr_GrDvrFs2Bdt	PtrBdt;
	__u32	BlkCnt;
}	*Ptr_GrDvrFs2GrpBdtLst;

typedef struct St_GrDvrFs2GrpV1UpgdInfo
{
	BOOL8	IsExist;
	BOOL8	IsV1;
	__u32	BlkPast;
	__u32	BlkPresent;
	__u32	RecIdPast;
	__u32	RecIdPresent;
}	*Ptr_GrDvrFs2GrpV1UpgdInfo;


//====================================================================
//class

class	Cls_GrDvrFs2Grp
{
	private:

		Cls_GrTaskCli*	m_ObjMng;		// manager

		// disk
		__u8	m_GrpType;			// group type
		__u8	m_DiskCnt;			// group disk count
		BOOL8	m_IsRecAble;		// record able
		BOOL8	m_IsDefective;	// defective
		__u8	m_DiskMap;			// disk map
		Cls_GrDvrFs2Disk*	m_DiskTbl[E_GrDvrFs2MaxDriveCnt];		// disk table

		__u32	m_AdoUnitSize;	// audio data size

		BOOL8	m_IsOvwtMode;	// overwrite mode
		__u8	m_AdoSyncCnt;	// audio sync frame count

		__u8	m_RecIntIdx;	// record disk internal index
		__u32	m_RecBlk;			// record block index
		__u32	m_RecId;			// record id

		St_GrDvrFs2GrpChInfo	m_ChTbl[E_GrDvrMaxChCnt];

		__u32	m_GthBuf[E_GrDvrFs2GrpGthBufDwCnt];	// write buffer
		__u32	m_GthAlloc;	// gather buffer allocated size
		__u32	m_GthIn;
		St_GrDvrFs2BlkAddInfo	m_GthTmb;
		Def_GrTime	m_GthBaseTime;
	protected:
		//local
		void	LcDiskTblBuild( void );
		BOOL8	LcChkRecIdOvlp( void );
		void	LcBlockRecUpdt( void );
		void	LcRebuildRecId( void );
		__u8	LcV1DiskCount( void );
		void	LcV1Upgrade( void );

		BOOL8	LcBlockEmptyFind( __u8* A_PtrRtIntIdx, __u32* A_PtrRtBlkIdx );
		__u32	LcBlockMinRecIdFind( __u8* A_PtrRtIntIdx, __u32* A_PtrRtBlkIdx );
		__u32	LcBlockMaxRecIdFind( __u8* A_PtrRtIntIdx, __u32* A_PtrRtBlkIdx );

		BOOL8	LcPacketReplace( __u64 A_Hndl, Ptr_GrDvrAvHd A_PtrPkt );

		BOOL8	LcHndlInfoGet( __u64 A_Hndl, __u8* A_PtrRtIntIdx, 
			__u64* A_PtrRtDpos, __u32* A_PtrRtBlkId, __u32* A_PtrRtRecId, 
			__u16* A_PtrRtOvwtId, __u32* A_PtrRtBlkOfs, __u32* A_PtrRtBlkRecSize );

		__u64	LcTimeFindFw( Def_GrTime* A_PtrTime );
		__u64	LcTimeFindBw( Def_GrTime* A_PtrTime );

		// gather
		BOOL8	LcGthPktAdd( Ptr_GrDvrAvHd A_PtrPkt );
		void*	LcGthAdoRsvd( __u8 A_Ch, __u8 A_Evt, Def_GrTime A_Time, __u32 A_Mili, BOOL8 A_IsSm );
		void	LcGthBufFlush( BOOL8 A_IsBdtUpdt );
		BOOL8	LcGthBufAlloc( __u32 A_ReqMin, __u32 A_ReqMax );

		__u32	LcBlockFwRecIdFind( __u32 A_RecId, __u8* A_PtrRtIntIdx, __u32* A_PtrRtBlkId, __u16* A_PtrRtOvwtId, __u32* A_PtrRtRecSize );
		__u32	LcBlockBwRecIdFind( __u32 A_RecId, __u8* A_PtrRtIntIdx, __u32* A_PtrRtBlkId, __u16* A_PtrRtOvwtId, __u32* A_PtrRtRecSize );

		void	LcEvtLogRefNext( Ptr_GrDvrLogCtrl A_PtrCtrl );
		void	LcEvtLogRefPrev( Ptr_GrDvrLogCtrl A_PtrCtrl );
		void	LcEvtLogSort( Ptr_GrDvrLogCtrl A_PtrCtrl );

	public:
    Cls_GrDvrFs2Grp( Cls_GrTaskCli* A_ObjMng, __u8 A_DskMap, BOOL8 A_IsOvrtMode );
    virtual		~Cls_GrDvrFs2Grp();

		__u8	GroupStatusGet( void );
		void	RecSizeGet( __u64* A_PtrRtTotal, __u64* A_PtrRtRec );
		BOOL8	RecTImeRngGet( Def_GrTime* A_PtrRtTimeBegin, Def_GrTime* A_PtrRtTimeEnd );
		__u64	TimeRecSizeCalc( __u16 A_ChMap, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd );

		BOOL8	VideoAdd( Ptr_GrDvrAvHd A_PtrPkt );		// video packet add
		BOOL8	AudioAdd( Ptr_GrDvrAvHd A_PtrPkt );		// audio packet add

		void	ReadForward( Ptr_GrDvrFs2SeekCtrl A_PtrCtrl, BOOL8 A_IsBufChg = TRUE );
		void	ReadBackward( Ptr_GrDvrFs2SeekCtrl A_PtrCtrl, BOOL8 A_IsBufChg = TRUE );
		void	ReadBegin( Ptr_GrDvrFs2SeekCtrl A_PtrCtrl );
		void	ReadEnd( Ptr_GrDvrFs2SeekCtrl A_PtrCtrl );
		void	ReadByTimeFw( Ptr_GrDvrFs2SeekCtrl A_PtrCtrl, Def_GrTime A_Time );
		void	ReadByTimeBw( Ptr_GrDvrFs2SeekCtrl A_PtrCtrl, Def_GrTime A_Time );
		BOOL8	ReadByHndlFw( Ptr_GrDvrFs2SeekCtrl A_PtrCtrl, __u64 A_Hndl );
		BOOL8	ReadByHndlBw( Ptr_GrDvrFs2SeekCtrl A_PtrCtrl, __u64 A_Hndl );

		__u32	DayMapGet( Def_GrTime A_TimeSt, Def_GrTime A_TimeEd );
		BOOL8	RecMinMapGet( Def_GrTime A_TimeSt, void* A_PtrBuf, __u32 A_Cnt );
		Def_GrTime	RecDayFindFw( Def_GrTime A_TimeBase );
		Def_GrTime	RecDayFindBw( Def_GrTime A_TimeBase );

		BOOL8	EvtLogAdd( Ptr_GrDvrLogItm A_PtrItem );
		void	EvtLogFindStart( Ptr_GrDvrLogCtrl A_PtrCtrl );
		void	EvtLogFindNext( Ptr_GrDvrLogCtrl A_PtrCtrl );
		void	EvtLogFindPrev( Ptr_GrDvrLogCtrl A_PtrCtrl );

};
	
//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif


