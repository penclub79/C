/*
	dvr file system version 1 group

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrDvrFsoGrp
#define	_EPC_GrDvrFsoGrp

//====================================================================
// uses
#include <GrTypeBase.h>
#include <Dvr/Fs/V1Old/GrDvrFsoDsk.h>
	
//====================================================================
//constance

#define E_GrDvrFsoGrpMaxDiskCnt		E_GrDvrFsoMaxDriveCnt
#define E_GrDvrFsoGrpLocalIdxNon	0xFF

//====================================================================
//global macro

//====================================================================
//global type


//====================================================================
//class

class	Cls_GrDvrFsoGrp
{
	private:
		Cls_GrDvrFsoDsk*	m_DskTbl[E_GrDvrFsoGrpMaxDiskCnt];
		__u32	m_DskCnt;
		BOOL8	m_IsOverWrite;
		void*	m_PtrLogRdBuf;
		__u8	m_GrpType;			//group type
		BOOL8	m_IsDiskFull;

		//runtime
		__u32	m_RecDskIdx;		//record index
	protected:
		__u8	LcFindDisk( __u8 A_DskIdx );
	public:
    Cls_GrDvrFsoGrp( __u8 A_GrpType, BOOL8 A_IsOverWrite );
    virtual		~Cls_GrDvrFsoGrp();

		//stauts
		__u64	RecBeginHndlGet( void );
		__u64	RecEndHndlGet( void );
		void	RecStatusGet( Ptr_GrDvrFsoRecStat A_PtrStatTbl );
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


		//maintanence
		BOOL8	DiskAdd( Cls_GrDvrFsoDsk* A_ObjDisk );
		void	DiskInit( void );		//disk system init
		BOOL8	Format( __u8 A_DskIdx, BOOL8 A_IsWithoutLog );

		//io
		BOOL8	PacketAdd( Ptr_GrDvrAvHd A_PtrPkt, __u64* A_PtrRtHndl );		//input bi direct packet
		__u64	PacketReserve( __u32 A_Size );		//packet area reserve - do not use by video packet
		BOOL8	TmaReadCmb( Def_GrTime A_Time, void* A_PtrBuf, __u32 A_Cnt, void* A_PtrTmpBuf );
		__u8	ReadNormal( __u64 A_Hndl, void* A_PtrBuf, __u32 A_Size, __u32* A_PtrRtSize, __u64* A_PtrRtCrntHndl );
		__u8	ReadBack( __u64 A_Hndl, void* A_PtrBuf, __u32 A_Size, __u32* A_PtrRtSize, __u64* A_PtrRtCrntHndl );
		__u8	ReadPrev( __u64 A_Hndl, void* A_PtrBuf, __u32 A_Size, __u32* A_PtrRtSize, __u64* A_PtrRtCrntHndl );
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
		__u32	DayMapGet( Def_GrTime A_TimeDay, __u32 A_DayCnt );
		
};
	
//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

