/*
	DVR file system version 1 group

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrDvrFs1Grp
#define	_EPC_GrDvrFs1Grp

//====================================================================
// uses
#include <GrTypeBase.h>
#include <Dvr/Fs/V1/GrDvrFs1Dsk.h>
	
//====================================================================
//constance

#define E_GrDvrFs1GrpMaxDiskCnt		E_GrDvrFs1MaxDriveCnt
#define E_GrDvrFs1GrpLocalIdxNon	0xFF

//====================================================================
//global macro

//====================================================================
//global type


//====================================================================
//class

class	Cls_GrDvrFs1Grp
{
	private:
		Cls_GrDvrFs1Dsk*	m_DskTbl[E_GrDvrFs1GrpMaxDiskCnt];
		__u32	m_DskCnt;
		BOOL8	m_IsOverWrite;
		void*	m_PtrLogRdBuf;
		__u8	m_GrpType;			//group type
		BOOL8	m_IsDiskFull;

		void*	m_PtrBuf;

		//runtime
		__u32	m_RecDskIdx;		//record index
	protected:
		__u8	LcFindDisk( __u8 A_DskIdx );

		__u64	LcTimeFindFw( Ptr_GrTime A_PtrTime );
		__u64	LcTimeFindBw( Ptr_GrTime A_PtrTime, Def_GrTime A_TimeLowLim );

		// new function


	public:
    Cls_GrDvrFs1Grp( __u8 A_GrpType, BOOL8 A_IsOverWrite, void* A_PtrBuf );
    virtual		~Cls_GrDvrFs1Grp();

		//stauts
		__u64	RecBeginHndlGet( void );
		__u64	RecEndHndlGet( void );
		void	RecStatusGet( Ptr_GrDvrFs1RecStat A_PtrStatTbl );
		__u64	UwaDiskPosGet( void );
		__u8	UwaDiskIndexGet( void );
		__u64	UwaSizeGet( void );
		Def_GrTime	RecTimeBeginGet( void );
		Def_GrTime	RecTimeEndGet( void );
		BOOL8	IsDiskFull( void );
		__u64	CalcDataSize( __u16 A_ChMap, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd, void* A_PtrTmaBuf );
		BOOL8	IsVssSaved( void );
		BOOL8	IsAllError( void );

		//record disk index


		//maintenance
		BOOL8	DiskAdd( Cls_GrDvrFs1Dsk* A_ObjDisk );
		void	DiskInit( void );		//disk system init
		BOOL8	Format( __u8 A_DskIdx, BOOL8 A_IsWithoutLog );

		//io
		BOOL8	PacketAdd( Ptr_GrDvrAvHd A_PtrPkt, __u64* A_PtrRtHndl );		//input bi direct packet
		__u64	PacketReserve( __u32 A_Size );		//packet area reserve - do not use by video packet
		BOOL8	TmaReadCmb( Def_GrTime A_Time, void* A_PtrBuf, __u32 A_Cnt );
		void	BufSync( void );
		BOOL8	DirectWrite( __u64 A_Hndl, void* A_PtrBuf, __u32 A_Size );
		void	EvtLogAdd( Ptr_GrDvrLogItm A_PtrItem );
		__u32	EvtLogGet( Def_GrTime A_TimeSt, Def_GrTime A_TimeEd, __u32 A_EvtMask, __u32 A_ChMask, Ptr_GrDvrLogItm A_PtrRt, __u32 A_MaxCnt );
		BOOL8	UcaWrite( __u32 A_Pos, void* A_PtrBuf, __u32 A_Size );
		BOOL8	UcaRead( __u32 A_Pos, void* A_PtrBuf, __u32 A_Size );
		BOOL8	UwaWrite( __u64 A_Pos, void* A_PtrBuf, __u32 A_Size );
		BOOL8	UwaRead( __u64 A_Pos, void* A_PtrBuf, __u32 A_Size );
		__u8	TemperatureGet( __u8 A_DskIdx );
		void	TemperatureUpdt( void );
		BOOL8	WbaWrite( void* A_PtrBuf, __u32 A_Size );
		BOOL8	WbaRead( void* A_PtrBuf, __u32 A_Size );

		// function
	
		void	ReadForward( Ptr_GrDvrFs1SeekCtrl A_PtrCtrl, BOOL8 A_IsBufChg = TRUE );
		void	ReadBackward( Ptr_GrDvrFs1SeekCtrl A_PtrCtrl, BOOL8 A_IsBufChg = TRUE );
		void	ReadBegin( Ptr_GrDvrFs1SeekCtrl A_PtrCtrl );
		void	ReadEnd( Ptr_GrDvrFs1SeekCtrl A_PtrCtrl );
		void	ReadByTimeFw( Ptr_GrDvrFs1SeekCtrl A_PtrCtrl, Def_GrTime A_Time );
		void	ReadByTimeBw( Ptr_GrDvrFs1SeekCtrl A_PtrCtrl, Def_GrTime A_Time, Def_GrTime A_TimeLimLow );
		void	ReadByHndlFw( Ptr_GrDvrFs1SeekCtrl A_PtrCtrl, __u64 A_Hndl );
		void	ReadByHndlBw( Ptr_GrDvrFs1SeekCtrl A_PtrCtrl, __u64 A_Hndl );

		
};
	
//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif

