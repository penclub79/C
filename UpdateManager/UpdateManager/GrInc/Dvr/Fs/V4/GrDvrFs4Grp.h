/*
	GAUSS DVR file system v4 group

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrDvrFs4Grp
#define	_EPC_GrDvrFs4Grp

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrBufTool.h>

#include <GrTaskBase.h>
#include <GrTaskCli.h>

#ifdef LINUX_APP
#include <sys/uio.h>
#else
#include <EmuLnx\GrElaBase.h>
#endif

#include <Dvr/Fs/V4/GrDvrFs4Base.h>
#include <Dvr/Fs/V4/GrDvrFs4Disk.h>
	
//====================================================================
//constance


//#define E_GrDvrFs4GrpDbgMsgView
//#define	E_GrDvrFs4GrpDbgTailCort

//====================================================================
//global macro

//====================================================================
//global type

typedef	struct St_GrDvrFs4GrpChInfo 
{
	__u8	AdoPrgs;		// audio reserved progress
	__u8	_rsvd1;
	__u8	_rsvd2;
	__u8	_rsvd3;
}	*Ptr_GrDvrFs4GrpChInfo;

//====================================================================
//class

class	Cls_GrDvrFs4Grp
{
	private:

		void*	m_FsMng;				// manager

		// disk
		__u8	m_GrpType;			// group type
		__u8	m_DiskCnt;			// group disk count
		BOOL8	m_IsRecAble;		// record able
		__u8	m_DiskMap;			// disk map
		Cls_GrDvrFs4Disk*	m_DiskTbl[E_GrDvrFs4MaxDriveCnt];		// disk table

		BOOL8	m_IsOvwtMode;		// overwrite mode
		BOOL8	m_IsOvwting;		// now status is overwriting

		__u8	m_RecDskIdx;		// record disk internal index
		__u32	m_RecBdtIdx;		// record BDT index
		__u32	m_RecSeqId;			// record sequence id

		//iovec	m_IovBuf[E_GrDvrFs4IovTblCnt];
		St_GrDvrFs4GrpWorkEnv	m_GrpWorkEnv;

		St_GrDvrFs4GrpChInfo	m_ChTbl[E_GrDvrMaxChCnt];

	protected:
		//local
		void	LcDiskTblBuild( void );
		BOOL8	LcRecIdChkSelfOvlp( void );
		BOOL8	LcRecIdChkRefOvlp(void);
		BOOL8	LcRecIdRebuild(void);

		void	LcRecBlockUpdt(void);

		BOOL8	LcBlockFindEmpty(__u8* A_PtrRtDskIdx, __u32* A_PtrRtBlkIdx);
		__u32	LcBlockFindFirst(__u8* A_PtrRtIntIdx, __u32* A_PtrRtBlkIdx);
		__u32	LcBlockFindLast(__u8* A_PtrRtIntIdx, __u32* A_PtrRtBlkIdx);
		BOOL8	LcBlockDeleteOldest(__u8* A_PtrRtIntIdx, __u32* A_PtrRtBlkIdx);
		
		BOOL8	LcHndlFindSeqIdFw(__u32 A_SeqId, __u64* A_PtrRtHndl, __u32* A_PtrRtSeqId);
		BOOL8	LcHndlFindSeqIdBw(__u32 A_SeqId, __u64* A_PtrRtHndl, __u32* A_PtrRtSeqId);
		BOOL8	LcHndlFindTimeFw(Def_GrTime A_Time, __u64* A_PtrRtHndl, __u32* A_PtrRtSeqId);
		BOOL8	LcHndlFindTimeBw(Def_GrTime A_Time, __u64* A_PtrRtHndl, __u32* A_PtrRtSeqId);
		BOOL8	LcHndlFindFirst(__u64* A_PtrRtHndl, __u32* A_PtrRtSeqId);



		void	LcLogRefNext( Ptr_GrDvrLogCtrl A_PtrCtrl );
		void	LcLogRefPrev( Ptr_GrDvrLogCtrl A_PtrCtrl );
		void	LcLogSort( Ptr_GrDvrLogCtrl A_PtrCtrl );


	public:
		Cls_GrDvrFs4Grp(void* A_FsMng, __u8 A_DskMap, BOOL8 A_IsOvrtMode);
    virtual		~Cls_GrDvrFs4Grp();

		BOOL8	IsRecAble( void );

		__u8	GroupGetStatus( void );
		void	RecGetSize( __u64* A_PtrRtTotal, __u64* A_PtrRtRec );
		void	RecGetTimeRng( Ptr_GrTime A_PtrRtTimeSt, Ptr_GrTime A_PtrRtTimeEd );
		void	GroupFormat(BOOL8 A_IsWithLog);

		BOOL8	RecAddData(Ptr_GrDvrFs4RecReqInfo A_PtrReq);

		__u8	ReadFw(Ptr_GrDvrFs4SeekCtrl A_PtrCtrl, __u32 A_Ofs);	// read forward
		__u8	ReadBw(Ptr_GrDvrFs4SeekCtrl A_PtrCtrl, __u32 A_Ofs);	// read backward
		__u8	ReadTimeFw(Ptr_GrDvrFs4SeekCtrl A_PtrCtrl, Def_GrTime A_Time);	// read sub time unit forward
		__u8	ReadTimeBw(Ptr_GrDvrFs4SeekCtrl A_PtrCtrl, Def_GrTime A_Time);	// read sub time unit forward
		__u8	ReadBegin(Ptr_GrDvrFs4SeekCtrl A_PtrCtrl);

		__u32	DayMapGet(Def_GrTime A_TimeSt, Def_GrTime A_TimeEd);
		BOOL8	MinMapGet(Def_GrTime A_TimeSt, Def_GrTime A_TimeEd, Ptr_GrDvrRecMinMap A_PtrRtBuf);

		BOOL8	LogAdd( Ptr_GrDvrLogItm A_PtrItem );
		void	LogFindStart( Ptr_GrDvrLogCtrl A_PtrCtrl );
		void	LogFindNext( Ptr_GrDvrLogCtrl A_PtrCtrl );
		void	LogFindPrev( Ptr_GrDvrLogCtrl A_PtrCtrl );

		void	BkupSizeEstmProc(Ptr_GrDvrFs4BkupSizeEstm A_PtrCtrl);

};
	
//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif


