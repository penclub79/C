/*
	DVR file system version 1 disk

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrDvrFs1Dsk
#define	_EPC_GrDvrFs1Dsk

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <Dvr/GrDvrBase.h>
#include <GrTimeTool.h>
#include <Dvr/Fs/V1/GrDvrFs1Base.h>
	
//====================================================================
//constance

#define E_GrDvrFs1Fcc							Mac_GrMakeFcc('F','S','D','1')
#define E_GrDvrFs1Ver							5			//version

#define E_GrDvrFs1BadBlkCnt				8
#define E_GrDvrFs1PassCoff				0x2685			/* password CODEC mask */

//fixed size
#define E_GrDvrFs1MisSize			0x00000200	
#define E_GrDvrFs1UcaMaxSize	0x00000400	
#define E_GrDvrFs1McaSize			0x00000400	
#define E_GrDvrFs1RcaItmSize	0x00000200	
#define E_GrDvrFs1RcaItmCnt		32	
#define E_GrDvrFs1RcaCntMask	0x1F	
#define E_GrDvrFs1RcaAreaSize	(E_GrDvrFs1RcaItmSize * E_GrDvrFs1RcaItmCnt)	
#define E_GrDvrFs1ElaSize			0x00200000		//modified 0x10000 -> 0x200000
#define E_GrDvrFs1WbaSize			0x00010000

#define E_GrDvrFs1TmaItemCnt	0xF00000
#define E_GrDvrFs1TmaSize			(E_GrDvrFs1TmaItemCnt << E_GrDvrRecMinThumbSizeShft)

#define E_GrDvrFs1SizeMin			0x0000000040000000LL
#define E_GrDvrFs1SizeMax			0x0000040000000000LL

//fixed address
#define E_GrDvrFs1DposMis			0
#define E_GrDvrFs1DposUca			(E_GrDvrFs1DposMis + E_GrDvrFs1MisSize)	
#define E_GrDvrFs1DposMca			(E_GrDvrFs1DposUca + E_GrDvrFs1UcaMaxSize)	
#define E_GrDvrFs1DposRca			(E_GrDvrFs1DposMca + E_GrDvrFs1McaSize)	
#define E_GrDvrFs1DposEla			(E_GrDvrFs1DposRca + E_GrDvrFs1RcaAreaSize)
#define E_GrDvrFs1DposWba			(E_GrDvrFs1DposEla + E_GrDvrFs1ElaSize)
#define E_GrDvrFs1DposTma			(E_GrDvrFs1DposWba + E_GrDvrFs1WbaSize)

#define E_GrDvrFs1DposUwa			(E_GrDvrFs1DposTma + E_GrDvrFs1TmaSize)

//frame mark
#define E_GrDvrFs1PktIdMask		0xFF000000
#define E_GrDvrFs1PktSizeMask	0x00FFFFFF

// HDD status
#define E_GrDvrFs1DiskStatNormal	0
#define E_GrDvrFs1DiskStatFull		1
#define E_GrDvrFs1DiskStatJam			2
#define E_GrDvrFs1DiskStatBad			3

// RCA
#define E_GrDvrFs1RcaPeriod			0x3000000	/* rca change period */

// RDA
#define E_GrDvrFs1RdaAlignUnit	0x0000000010000000LL	
#define E_GrDvrFs1RdaAlignMask	0xFFFFFFFFF0000000LL

// ELA
#define E_GrDvrFs1ElaItmSize		16
#define E_GrDvrFs1ElaItmSzSft		4			//item size shift
#define E_GrDvrFs1ElaMaxItmCnt	(E_GrDvrFs1ElaSize >> E_GrDvrFs1ElaItmSzSft)

// TMA
#define E_GrDvrFs1TmaTimeUnit		60

#define E_GrDvrFs1TmaTmpCnt			(60*24)
#define E_GrDvrFs1TmaTmpSize		(E_GrDvrFs1TmaTmpCnt << E_GrDvrRecMinThumbSizeShft)

//smart
#define E_GrDvrFs1SmartValIdxId			0
#define E_GrDvrFs1SmartValIdxStat0	1
#define E_GrDvrFs1SmartValIdxStat1	2
#define E_GrDvrFs1SmartValIdxValue	3
#define E_GrDvrFs1SmartValIdxVendor	4
#define E_GrDvrFs1SmartValIdxRawVal	5		/* 6byte */

#define E_GrDvrFs1SmartValItemSize	12

#define E_GrDvrFs1SmartValCnt		30

//runtime
#define E_GrDvrFs1TmaBadIdx		0xFFFFFFFF
//#define E_GrDvrFs1BadAreaIdxNone	0xFF

//====================================================================
//global macro

//====================================================================
//global type

typedef	struct St_GrDvrFs1DskCfg 
{
	__u32		SysId;
	__u32		FccStrm;				//stream fcc
	Def_GrTime	TimeBase;		//request time base
	//__u32		TmaCnt;					//request tma count
	__u32		IdaSize;				//request instance replay area size
	__u64		UwaSize;				//request user working area size
	//BOOL8		IsLogNoFmt;			//log no format - notuse, with option
	Def_StrGrFnPath	DevFnTbl[E_GrDvrFs1MaxDriveCnt];			//device file name table
}	*Ptr_GrDvrFs1DskCfg;

typedef	struct St_GrDvrFs1Mis 
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

}	*Ptr_GrDvrFs1Mis;

typedef	struct St_GrDvrFs1Mca 
{
	__u8	RcaIdx;
	__u8	_rsv0[7];		//reserved
	__u64	BadBlkTbl[E_GrDvrFs1BadBlkCnt];
	Def_WstrNum	StrPass;
}	*Ptr_GrDvrFs1Mca;

typedef	struct St_GrDvrFs1Rca 
{
	__u64	RecPosIn;
	__u64	RecPosOut;
	__u32	EvtPosIn;			//event input index
	__u32	EvtPosOut;
	__u32	_rsvd0;
	__u16	RecSeqId;			//record sequence id
	__u8	DiskStat;			//disk status
	BOOL8	IsVss;
}	*Ptr_GrDvrFs1Rca;

typedef struct St_GrDvrFs1SmartData 
{
	__u16		Revision;
	__u8		Values[E_GrDvrFs1SmartValCnt * E_GrDvrFs1SmartValItemSize];
	__u8		Offline_status;
	__u8		Vendor1;
	__u16		Offline_timeout;
	__u8		Vendor2;
	__u8		Offline_capability;
	__u16		Smart_capability;
	__u8		_rsv[16];
	__u8		VendorSpc[125];
	__u8		Checksum;
} *Ptr_GrDvrFs1SmartData;

typedef	struct St_GrDvrFs1SmartIoc
{
	__u8	Cmds[4];
	St_GrDvrFs1SmartData	Data;
}	*Ptr_GrDvrFs1SmartIoc;

typedef	struct St_GrDvrFs1RecStat 
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
}	*Ptr_GrDvrFs1RecStat;

//====================================================================
//class

class	Cls_GrDvrFs1Dsk
{
	private:
		__u8		m_DskIdx;
		__u8		m_GrpSubIdx;			//disk index in group
		__u64		m_DskSize;
		BOOL8		m_IsFormatted;
		Ptr_GrDvrFs1DskCfg	m_PtrCfg;
		BOOL8		m_IsOverWrite;
		void*		m_PtrTmpBuf;			// 2MByte temporary buffer

#ifdef LINUX_APP
		__s32		m_HndlDsk;
#else
		HANDLE	m_HndlDsk;
#endif

		St_GrDvrFs1Mis	m_Mis;
		St_GrDvrFs1Mca	m_Mca;	
		St_GrDvrFs1Rca	m_Rca;

		//runtime
		Def_GrTime	m_TimeLimite;					//limit time

		St_GrDvrRecMinThumb	m_CchWtTma;		// TMA write cache
		Def_GrTime	m_TmaTmBase;					// TMA base time

		BOOL8				m_IsModified;					//data modified
		St_GrDvrFs1SmartIoc	m_SmartIoc;		//smart IOC
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
		BOOL8	LcTmaWrite( Ptr_GrDvrRecMinThumb A_PtrTma, __u64 A_Pos );
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

	public:
    Cls_GrDvrFs1Dsk( __u8 A_DskIdx, Ptr_GrDvrFs1DskCfg A_PtrCfg, BOOL8 A_IsOverwrite, void* A_PtrTmpBuf );
    virtual		~Cls_GrDvrFs1Dsk();

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
		void	RecStatusGet( Ptr_GrDvrFs1RecStat A_PtrStat );
		__u64	UwaDiskPosGet( void );
		__u64	UwaSizeGet( void );
		BOOL8	IsOverwriting( void );
		BOOL8	IsEmpty( void );
		BOOL8	IsReadAble( void );
		__u64	RecOutPosGet( void );
		__u64	RecInPosGet( void );
		__u64	RdaStartPosGet( void );
		__u64	RdaLastPosGet( void );
		Def_GrTime	RecTimeBeginGet( void );
		Def_GrTime	RecTimeEndGet( void );
		__u64	FindHndlByMinFw( Def_GrTime A_TimeSt, Def_GrTime A_TimeEd, void* A_PtrTmaBuf );
		__u64	FindHndlByMinBw( Def_GrTime A_TimeSt, Def_GrTime A_TimeEd, void* A_PtrTmaBuf );
		__u64	DataSizeByMinuteRange( Def_GrTime A_TimeSt, Def_GrTime A_TimeEd, void* A_PtrTmaBuf );
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
		BOOL8	TmaReadCmb( Def_GrTime A_Time, void* A_PtrBuf, __u32 A_Cnt );
		void	BufSync( void );
		void	EvtLogAdd( Ptr_GrDvrLogItm A_PtrItem );
		__u32	EvtLogGet( Def_GrTime A_TimeSt, Def_GrTime A_TimeEd, __u32 A_EvtMask, __u32 A_ChMask, Ptr_GrDvrLogItm A_PtrRt, __u32 A_MaxCnt );
		BOOL8	UcaWrite( __u32 A_Pos, void* A_PtrBuf, __u32 A_Size );
		BOOL8	UcaRead( __u32 A_Pos, void* A_PtrBuf, __u32 A_Size );
		BOOL8	UwaWrite( __u64 A_Pos, void* A_PtrBuf, __u32 A_Size );
		BOOL8	UwaRead( __u64 A_Pos, void* A_PtrBuf, __u32 A_Size );
		BOOL8	WbaWrite( void* A_PtrBuf, __u32 A_Size );
		BOOL8	WbaRead( void* A_PtrBuf, __u32 A_Size );

		// new function
		BOOL8	IsOldPartExist( void );
		__u64	RdaBeginHndlGet( BOOL8 A_IsOldPart );
		__u64	RdaEndHndlGet( BOOL8 A_IsOldPart );

		__u8	ChkHndlFw( __u64 A_Hndl, __u32* A_PtrRtAbleSize );
		__u8	ChkHndlBw( __u64 A_Hndl, __u32* A_PtrRtAbleSize );
		BOOL8	ReadByRpos( __u64 A_RdaPos, void* A_PtrBuf, __u32 A_Size );

};
	
//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif

