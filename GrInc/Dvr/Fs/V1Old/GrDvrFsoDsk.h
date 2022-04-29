/*
	dvr file system version 1 disk

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrDvrFsoDsk
#define	_EPC_GrDvrFsoDsk

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <Dvr/GrDvrBase.h>
#include <GrTimeTool.h>
	
//====================================================================
//constance

#define E_GrDvrFsoFcc					Mac_GrMakeFcc('F','S','D','1')
#define E_GrDvrFsoVerBigTma		5			//version
#define E_GrDvrFsoVerNew			6

#define E_GrDvrFsoMaxDriveCnt	8

#define E_GrDvrFsoStrmAlignSize		32
#define E_GrDvrFsoStrmAlignMask		0xFFFFFFE0

#define E_GrDvrFsoBadBlkCnt		8
#define E_GrDvrFsoPassCoff		0x2685			/* password CODEC mask */

#define E_GrDvrFsoHndlSeqPos			48					/* record handle sequence id bit position */
#define E_GrDvrFsoHndlGrpIdxMask	7LL					/* group idx mask */
#define E_GrDvrFsoHndlNone				0LL					/* handle value none */
#define E_GrDvrFsoHndlPosMask			0x0000FFFFFFFFFFF8LL					/* position mask */
#define E_GrDvrFsoHndlSeqLimite		0xFFFF			/* record handle sequence limite value */

//fixed size
#define E_GrDvrFsoMisSize			0x00000200	
#define E_GrDvrFsoUcaMaxSize	0x00000400	
#define E_GrDvrFsoMcaSize			0x00000400	
#define E_GrDvrFsoRcaItmSize	0x00000200	
#define E_GrDvrFsoRcaItmCnt		32	
#define E_GrDvrFsoRcaCntMask	0x1F	
#define E_GrDvrFsoRcaAreaSize	(E_GrDvrFsoRcaItmSize * E_GrDvrFsoRcaItmCnt)	
#define E_GrDvrFsoElaSize			0x00200000		//modified 0x10000 -> 0x200000
#define E_GrDvrFsoWbaSize			0x00010000

#define E_GrDvrFsoTmaItemCnt	0xE00000
#define E_GrDvrFsoTmaSize			(E_GrDvrFsoTmaItemCnt << E_GrDvrRecMinThumbSizeShft)

#define E_GrDvrFsoTmaScanCnt	0x600000

#define E_GrDvrFsoSizeLimite	0x40000000LL

#define	E_GrDvrFsoDmdCnt			512
#define	E_GrDvrFsoDmdAreaSize	0x100000

#define	E_GrDvrFsoMmdDaySize	(E_GrTimeMinOfDay << E_GrDvrRecMinThumbSizeShft)
#define	E_GrDvrFsoMmdAreaSize	0xF00000

//fixed address
#define E_GrDvrFsoDposMis			0
#define E_GrDvrFsoDposUca			(E_GrDvrFsoDposMis + E_GrDvrFsoMisSize)	
#define E_GrDvrFsoDposMca			(E_GrDvrFsoDposUca + E_GrDvrFsoUcaMaxSize)	
#define E_GrDvrFsoDposRca			(E_GrDvrFsoDposMca + E_GrDvrFsoMcaSize)	
#define E_GrDvrFsoDposEla			(E_GrDvrFsoDposRca + E_GrDvrFsoRcaAreaSize)
#define E_GrDvrFsoDposWba			(E_GrDvrFsoDposEla + E_GrDvrFsoElaSize)
#define E_GrDvrFsoDposTma			(E_GrDvrFsoDposWba + E_GrDvrFsoWbaSize)	/* old compatible */

#define	E_GrDvrFsoDposDmd			(E_GrDvrFsoDposTma + E_GrDvrFsoTmaSize)
#define	E_GrDvrFsoDposMmd			(E_GrDvrFsoDposDmd + E_GrDvrFsoDmdAreaSize)

#define E_GrDvrFsoDposUwa			(E_GrDvrFsoDposMmd + E_GrDvrFsoMmdAreaSize)

//frame mark
#define E_GrDvrFsoPktIdMask		0xFF000000
#define E_GrDvrFsoPktSizeMask	0x00FFFFFF

// HDD status
#define E_GrDvrFsoDiskStatNormal	0
#define E_GrDvrFsoDiskStatFull		1
#define E_GrDvrFsoDiskStatJam			2
#define E_GrDvrFsoDiskStatBad			3

// RCA
#define E_GrDvrFsoRcaPeriod			0x3000000	/* rca change period */

// RDA
#define E_GrDvrFsoRdaAlignUnit	0x0000000010000000LL	
#define E_GrDvrFsoRdaAlignMask	0xFFFFFFFFF0000000LL

// ELA
#define E_GrDvrFsoElaItmSize		16
#define E_GrDvrFsoElaItmSzSft		4			//item size shift
#define E_GrDvrFsoElaMaxItmCnt	(E_GrDvrFsoElaSize >> E_GrDvrFsoElaItmSzSft)

// TMA
#define E_GrDvrFsoTmaTimeUnit		60

#define E_GrDvrFsoTmaTmpCnt			(60*24)
#define E_GrDvrFsoTmaTmpSize		(E_GrDvrFsoTmaTmpCnt << E_GrDvrRecMinThumbSizeShft)

//smart
#define E_GrDvrFsoSmartValIdxId			0
#define E_GrDvrFsoSmartValIdxStat0	1
#define E_GrDvrFsoSmartValIdxStat1	2
#define E_GrDvrFsoSmartValIdxValue	3
#define E_GrDvrFsoSmartValIdxVendor	4
#define E_GrDvrFsoSmartValIdxRawVal	5		/* 6byte */

#define E_GrDvrFsoSmartValItemSize	12

#define E_GrDvrFsoSmartValCnt		30

//runtime
#define E_GrDvrFsoTmaBadIdx		0xFFFFFFFF
//#define E_GrDvrFsoBadAreaIdxNone	0xFF

#define E_GrDvrFsoTmpBufSize	0x20000

#define	E_GrDvrFsoUpgdBufSize	0x10000

#define	E_GrDvrFsoBufOfsDmdCr	0x10000

//====================================================================
//global macro

//====================================================================
//global type

typedef	struct St_GrDvrFsoDskCfg 
{
	__u32		SysId;
	__u32		FccStrm;				//stream fcc
	//Def_GrTime	TimeBase;		//request time base
	//__u32		TmaCnt;					//request tma count
	__u32		IdaSize;				//request instance replay area size
	__u64		UwaSize;				//request user working area size
	//BOOL8		IsLogNoFmt;			//log no format - notuse, with option
	Def_StrGrFnPath	DevFnTbl[E_GrDvrFsoMaxDriveCnt];			//device file name table

	__u8	TmpBuf[E_GrDvrFsoTmpBufSize];

}	*Ptr_GrDvrFsoDskCfg;

typedef	struct St_GrDvrFsoMis 
{
	__u32	Fcc;
	__u32	Ver;
	__u32	SysId;

	Def_GrTime	TimeBase;
	__u32	TmaCnt;

	__u32	_rsv0;		//reserved

	__u64	UwaPos;
	__u64	UwaSize;

	__u64	IdaPos;		
	__u64	IdaSize;

	__u64	RdaPos;
	__u64	RdaLast;

}	*Ptr_GrDvrFsoMis;

typedef	struct St_GrDvrFsoMca 
{
	__u8	RcaIdx;
	__u8	_rsv0[7];		//reserved
	__u64	BadBlkTbl[E_GrDvrFsoBadBlkCnt];
	Def_WstrNum	StrPass;
}	*Ptr_GrDvrFsoMca;

typedef	struct St_GrDvrFsoRca 
{
	__u64	RecPosIn;
	__u64	RecPosOut;
	__u32	EvtPosIn;			//event input index
	__u32	EvtPosOut;
	__u32	_rsvd0;
	__u16	RecSeqId;			//record sequence id
	__u8	DiskStat;			//disk status
	BOOL8	IsVss;
}	*Ptr_GrDvrFsoRca;

typedef struct St_GrDvrFsoSmartData 
{
	__u16		Revision;
	__u8		Values[E_GrDvrFsoSmartValCnt * E_GrDvrFsoSmartValItemSize];
	__u8		Offline_status;
	__u8		Vendor1;
	__u16		Offline_timeout;
	__u8		Vendor2;
	__u8		Offline_capability;
	__u16		Smart_capability;
	__u8		_rsv[16];
	__u8		VendorSpc[125];
	__u8		Checksum;
} *Ptr_GrDvrFsoSmartData;

typedef	struct St_GrDvrFsoSmartIoc
{
	__u8	Cmds[4];
	St_GrDvrFsoSmartData	Data;
}	*Ptr_GrDvrFsoSmartIoc;

typedef	struct St_GrDvrFsoRecStat 
{
	BOOL8	IsFormatted;
	BOOL8	IsNowRec;
	BOOL8	IsBad;
	BOOL8	IsOvwt;					//overwrite
	BOOL8	IsFull;					//full
	__u8	Temp;						//temperature
	__u8	Type;						//normal mirror none
	__u64	DiskSize;
	__u64	RecSize;
}	*Ptr_GrDvrFsoRecStat;

//====================================================================
//class

class	Cls_GrDvrFsoDsk
{
	private:
		__u8		m_DskIdx;
		__u8		m_GrpSubIdx;			//disk index in group
		__u64		m_DskSize;
		BOOL8		m_IsFormatted;
		Ptr_GrDvrFsoDskCfg	m_PtrCfg;
		BOOL8		m_IsOverWrite;
		void*		m_PtrLogRdBuf;		//2mbyte log read buffer

#ifdef LINUX_APP
		__s32		m_HndlDsk;
#else
		HANDLE	m_HndlDsk;
#endif

		St_GrDvrFsoMis	m_Mis;
		St_GrDvrFsoMca	m_Mca;	
		St_GrDvrFsoRca	m_Rca;

		Def_GrTime			m_Dmd[E_GrDvrFsoDmdCnt];

		//runtime
		//Def_GrTime	m_TimeLimite;					//limit time

		St_GrDvrRecMinThumb	m_CchWtTma;		// TMA write cache
		Def_GrTime	m_TmaTmBase;					// TMA base time

		BOOL8				m_IsModified;					//data modified
		St_GrDvrFsoSmartIoc	m_SmartIoc;		//smart IOC
		Def_GrTime	m_RecTimeBegin;
		Def_GrTime	m_RecTimeEnd;
		__u8				m_ValTemp;						//temperature

	protected:

		//basic 
		BOOL8	LcSeek( __s64 A_Pos, BOOL8 A_IsBegin = TRUE );
		BOOL8	LcRead( void* A_PtrBuf, __u32 A_Size );
		BOOL8	LcWrite( void* A_PtrBuf, __u32 A_Size );
		void	LcFlush( void );

		BOOL8	LcPread( __u64 A_Pos, void* A_PtrBuf, __u32 A_Size );
		BOOL8	LcPwrite( __u64 A_Pos, void* A_PtrBuf, __u32 A_Size );

		BOOL8	LcFillByte( __u64 A_PosTg, __u32 A_Size, __u8 A_Fill );
		BOOL8	LcCopy( __u64 A_PosTg, __u64 A_PosSr, __u32 A_Size );

		BOOL8	LcInit( void );
		BOOL8	LcIsBadBlock( __u64 A_Pos );
		BOOL8	LcAlignPadAdd( void );
		BOOL8	LcBadBlockAdd( __u64 A_Pos );
		void	LcMakeSpace( void );
		void	LcTmaUpdt( Def_GrTime A_Time, __u8 A_Ch, __u8 A_Atb );		// TMA update on rec in position
		void	LcRcaWrite( void );
		void	LcMcaUpdt( void );
		BOOL8	LcIsCrntHndl( __u64 A_Hndl );
		__u64	LcMakeHndlByPos( __u64 A_Pos );
		BOOL8	LcSmartRead( void );
		BOOL8	LcSmartFindVal( __u8 A_Id, __u8* A_PtrRtVal );
		BOOL8	LcSmartFindRawByte( __u8 A_Id, __u8* A_PtrRtVal );
		void	LcEvtLogDisable( void );			//make event log data disable
		BOOL8	LcPreparePktAdd( __u32 A_Size );	//move in position to packet add able
		__u32	LcCalcBufFrmsSizeNormal( void* A_PtrBuf, __u32 A_BufSize );
		__u32	LcCalcBufFrmsSizeBack( void* A_PtrBuf, __u32 A_BufSize );
		Def_GrTime	LcGetBufFirstTime( void* A_PtrBuf, __u32 A_BufSize );
		Def_GrTime	LcGetBufLastTime( void* A_PtrBuf, __u32 A_BufSize );

		BOOL8	LcUpgdOldFs( __u32 A_OldVer );
		BOOL8	LcThumbWrite( Ptr_GrDvrRecMinThumb A_PtrThumb, Def_GrTime A_Time );
		BOOL8	LcThumdRead( Ptr_GrDvrRecMinThumb A_PtrThumb, Def_GrTime A_Time );
		__s32	LcDmdFindTime( Def_GrTime A_TimeDay );
		__s32	LcDmdFindBegin( void );
		__s32	LcDmdFindEnd( void );
		__s32	LcDmdCreate( Def_GrTime A_TimeDay );

	public:
    Cls_GrDvrFsoDsk( __u8 A_DskIdx, Ptr_GrDvrFsoDskCfg A_PtrCfg, BOOL8 A_IsOverwrite, void* A_PtrLogReadBuf );
    virtual		~Cls_GrDvrFsoDsk();

		//status
		__u8	DiskIdxGet( void );
		__u64	DiskSizeGet( void );
		BOOL8	IsFormatted( void );
		__u8	DiskStatGet( void );
		__u64	RecOutPosHndlGet( void );
		__u64	RecInPosHndlGet( void );
		__u64	RdaPosHndlGet( void );
		__u64	RdaLastHndlGet( void );
		__u8	TemperatureGet( void );
		void	TemperatureUpdt( void );
		void	RecStatusGet( Ptr_GrDvrFsoRecStat A_PtrStat );
		__u64	UwaDiskPosGet( void );
		__u64	UwaSizeGet( void );
		BOOL8	IsOverwriting( void );
		BOOL8	IsEmpty( void );
		__u64	RecOutPosGet( void );
		__u64	RecInPosGet( void );
		__u64	RdaStartPosGet( void );
		__u64	RdaLastPosGet( void );
		Def_GrTime	RecTimeBeginGet( void );
		Def_GrTime	RecTimeEndGet( void );
		__u64	FindHndlByMinFw( Def_GrTime A_TimeSt, Def_GrTime A_TimeEd );
		__u64	FindHndlByMinBw( Def_GrTime A_TimeSt, Def_GrTime A_TimeEd );
		__u64	DataSizeByMinuteRange( Def_GrTime A_TimeSt, Def_GrTime A_TimeEd );
		BOOL8	IsVssSaved( void );

		//interface
		void	GroupSubIndexSet( __u8 A_Idx );
		/*
		void	PasswdGet( WCHAR* A_StrRt );
		void	PasswdSet( WCHAR* A_StrPass );
		*/

		//basic control
		BOOL8	Format( BOOL8 A_IsWithoutLog );
		BOOL8	PacketAdd( Ptr_GrDvrAvHd A_PtrPkt, __u64* A_PtrRtHndl );		//input bi direct packet
		__u64	PacketReserve( __u32 A_Size );		//packet area reserve - do not use by video packet
		BOOL8	DirectWrite( __u64 A_Hndl, void* A_PtrBuf, __u32 A_Size );
		void	UnlockFull( void );
		__u32	TmaDirRead( Def_GrTime A_Time, void* A_PtrBuf, __u32 A_Cnt, __u32* A_PtrRtHeadGap );
		BOOL8	TmaReadCmb( Def_GrTime A_Time, void* A_PtrBuf, __u32 A_Cnt, void* A_PtrTmpBuf );
		__u8	ReadNormal( __u64 A_Hndl, void* A_PtrBuf, __u32 A_Size, __u32* A_PtrRtSize, __u64* A_PtrRtCrntHndl );
		__u8	ReadBack( __u64 A_Hndl, void* A_PtrBuf, __u32 A_Size, __u32* A_PtrRtSize, __u64* A_PtrRtCrntHndl );
		__u8	ReadPrev( __u64 A_Hndl, void* A_PtrBuf, __u32 A_Size, __u32* A_PtrRtSize, __u64* A_PtrRtCrntHndl );
		void	BufSync( void );
		void	EvtLogAdd( Ptr_GrDvrLogItm A_PtrItem );
		__u32	EvtLogGet( Def_GrTime A_TimeSt, Def_GrTime A_TimeEd, __u32 A_EvtMask, __u32 A_ChMask, Ptr_GrDvrLogItm A_PtrRt, __u32 A_MaxCnt );
		BOOL8	UcaWrite( __u32 A_Pos, void* A_PtrBuf, __u32 A_Size );
		BOOL8	UcaRead( __u32 A_Pos, void* A_PtrBuf, __u32 A_Size );
		BOOL8	UwaWrite( __u64 A_Pos, void* A_PtrBuf, __u32 A_Size );
		BOOL8	UwaRead( __u64 A_Pos, void* A_PtrBuf, __u32 A_Size );
		BOOL8	WbaWrite( void* A_PtrBuf, __u32 A_Size );
		BOOL8	WbaRead( void* A_PtrBuf, __u32 A_Size );
		__u32	DayMapGet( Def_GrTime A_TimeDay, __u32 A_DayCnt );

};
	
//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

