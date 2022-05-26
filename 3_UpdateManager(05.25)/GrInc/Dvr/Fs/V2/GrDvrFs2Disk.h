/*
	GAUSE DVR file system v2 disk

	version history
		2012.5.15		-	1 made

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrDvrFs2Disk
#define	_EPC_GrDvrFs2Disk

//====================================================================
// uses
#include	<GrTypeBase.h>
#include	<GrBufTool.h>
#include	<Dvr/GrDvrBase.h>
#include	<Dvr/Fs/V2/GrDvrFs2Base.h>
	
//====================================================================
//constance

#define E_GrDvrFs2Fcc									Mac_GrMakeFcc('F','S','D','2')

#define E_GrDvrFs2Ver									1		/* version */

// default size
#define E_GrDvrFs2ElaDfltSize					0x0400000		/* default ELA size 4 Mega byte */
#define E_GrDvrFs2VsaDfltSize					0x0100000		/* default VSA size 1 Mega byte */

// fixed size
#define E_GrDvrFs2UcaMaxSize					0x100000
#define E_GrDvrFs2ElaMinSize					0x100000
#define E_GrDvrFs2ElaMaxSize					0xF00000
#define E_GrDvrFs2VsaMinSize					0x100000
#define E_GrDvrFs2VsaMaxSize					0x6000000

#define E_GrDvrFs2V1RcaItmSize				0x00000200	
#define E_GrDvrFs2V1UcaSize						0x00000400

// V1 address
#define E_GrDvrFs2PosV1Uca						0x00000200		/* V1 UCA */
#define E_GrDvrFs2PosV1Mca						0x00000600		/* V1 MCA */
#define E_GrDvrFs2PosV1Rca						0x00000A00		/* V1 RCA */
#define E_GrDvrFs2PosV1Ela						0x00004A00		/* V1 ELA */
#define E_GrDvrFs2PosV1Tma						0x00214A00		/* V1 TMA */

// new address
#define E_GrDvrFs2PosMis							0x00000000		/* media info sector */
#define E_GrDvrFs2PosRca							0x00001000		/* run time control area */
#define E_GrDvrFs2PosDmd							0x00002000		/* date map data */
#define E_GrDvrFs2PosEla							0x00004A00		/* event log area */
#define E_GrDvrFs2PosUca							0x00A00000		/* user configuration area */
#define E_GrDvrFs2PosMmd							0x05000000		/* minute map data */
#define E_GrDvrFs2PosBdt							0x07000000		/* block directory table */
#define E_GrDvrFs2PosVsa							0x08000000		/* video surveillance service area */
#define E_GrDvrFs2PosRda							0x0000000010000000LL	/* record data area position */

#define E_GrDvrFs2DiskMinSize					0x0000000040000000LL
#define E_GrDvrFs2DiskMaxSize					0x0000080010000000LL

// BDT
#define E_GrDvrFs2BdtMaxCnt						0x4000

// block
#define E_GrDvrFs2BlockSize						0x10000000
#define E_GrDvrFs2BlockOfsMask				0x0FFFFFFF
#define E_GrDvrFs2BlockShift					28

#define E_GrDvrFs2DiskBlkAlignMask		0x000000000FFFFFFFLL
	
// time map
#define E_GrDvrFs2DmdItemCnt					1024

#define E_GrDvrFs2MmdDayItemCnt				(24 * 60)
#define E_GrDvrFs2MmdDaySize					(E_GrDvrFs2MmdDayItemCnt * sizeof(St_GrDvrRecMinThumb))

#define E_GrDvrFs2MmdTotalItemCnt			(E_GrDvrFs2MmdDayItemCnt * E_GrDvrFs2DmdItemCnt)
#define E_GrDvrFs2MmdTotalSize				(E_GrDvrFs2MmdTotalItemCnt * sizeof(St_GrDvrRecMinThumb))

#define E_GrDvrFs2StrmAlignSize				32
#define E_GrDvrFs2StrmAlignMask				0xFFFFFFE0

#define E_GrDvrFs2CvtTmaWkCnt					(E_GrDvrFs2TmpBufSize >> E_GrDvrRecMinThumbSizeShft)

// smart
#define E_GrDvrFs2SmartValIdxId				0
#define E_GrDvrFs2SmartValIdxStat0		1
#define E_GrDvrFs2SmartValIdxStat1		2
#define E_GrDvrFs2SmartValIdxValue		3
#define E_GrDvrFs2SmartValIdxVendor		4
#define E_GrDvrFs2SmartValIdxRawVal		5		/* 6byte */

#define E_GrDvrFs2SmartValItemSize		12

#define E_GrDvrFs2SmartValCnt					30

#define E_GrDvrFs2SmartIdTemp					194

#define E_GrDvrFs2DiskDbgMsgView

//====================================================================
//global macro

//====================================================================
//global type

// old version 

typedef	struct St_GrDvrFs2Mis
{
	__u32	FsFcc;			// file system FCC code
	__u32	FsVer;			// file system version
	__u32	ElaSize;		// ELD size
	__u32	VsaSize;		// VSS size
	__u32	V1TmaCnt;		// V1 compatible
	__u32	BlkCnt;			// record block count
	__u64	V1UwaPos;
	__u64	V1UwaSize;
	__u64	_rsvd0;
	__u64	_rsvd1;	
	__u64	V1RdaPos;
	__u64	V1RdaLast;

}	*Ptr_GrDvrFs2Mis;

typedef	struct St_GrDvrFs2V1Mca 
{
	__u8	RcaIdx;
	__u8	_rsvd0[3];		//reserved
}	*Ptr_GrDvrFs2V1Mca;

typedef	struct St_GrDvrFs2V1Rca
{
	__u64	RecPosIn;
	__u64	RecPosOut;
	__u32	EvtPosIn;			//event input index
	__u32	EvtPosOut;
	__u32	UseCnt;
	__u16	RecSeqId;			//record sequence id
	__u8	DiskStat;			//disk status
	BOOL8	IsVss;
}	*Ptr_GrDvrFs2V1Rca;

typedef	struct	St_GrDvrFs2Rca
{
	BOOL8	IsEvtOvwt;		// ELA overwrote
	__u8	_rsvd0;				// reserved
	__u16	_rsvd1;
	__u32	EvtPosIn;			// ELA input position
	__u64	DvrId;				// DVR id

}	*Ptr_GrDvrFs2Rca;

typedef struct St_GrDvrFs2Bdt
{
	BOOL8	IsBad;
	BOOL8	IsFull;
	__u16	OvwtId;
	__u32	RecId;

	__u32	TimeSt;
	__u32	TimeEd;

	__u32	RecSize;
	BOOL8	IsCanSort;	// for sort
	__u8	_rsvd0;
	__u16	_rsvd1;

}	*Ptr_GrDvrFs2Bdt;

typedef struct St_GrDvrFs2SmartData 
{
	__u16		Revision;
	__u8		Values[E_GrDvrFs2SmartValCnt * E_GrDvrFs2SmartValItemSize];
	__u8		Offline_status;
	__u8		Vendor1;
	__u16		Offline_timeout;
	__u8		Vendor2;
	__u8		Offline_capability;
	__u16		Smart_capability;
	__u8		_rsv[16];
	__u8		VendorSpc[125];
	__u8		Checksum;
} *Ptr_GrDvrFs2SmartData;

typedef	struct St_GrDvrFs2SmartIoc
{
	__u8	Cmds[4];
	St_GrDvrFs2SmartData	Data;
}	*Ptr_GrDvrFs2SmartIoc;

//====================================================================
//class

class	Cls_GrDvrFs2Disk
{
	private:
		__u8	m_DevIdx;					// device index
		__u8	m_GrpIntIdx;			// group internal index
		BOOL8	m_IsOvwtAble;			// overwrite able
		BOOL8	m_IsNeedCvt;			// need convert

		void*	m_PtrTmpBuf;
		__u64	m_DvrId;

		__u8	m_DskStat;				// disk status
		__u64	m_RecCap;					// record capable - MByte unit
		__u64	m_RecSize;				// recorded size - MByte unit
		
		__u64	m_DskSize;				// disk size

		St_GrDvrFs2V1Rca	m_V1Rca;

		St_GrDvrFs2Mis	m_Mis;
		St_GrDvrFs2Rca	m_Rca;
		St_GrDvrFs2Bdt	m_Bdt[E_GrDvrFs2BdtMaxCnt];
		Def_GrTime	m_Dmd[E_GrDvrFs2DmdItemCnt];

		// runtime
		Def_GrTime	m_RecTimeBegin;
		Def_GrTime	m_RecTimeEnd;
		__u64				m_HndlLastRec;

		// thumb write
		Def_GrTime	m_ThumbTime;		// thumb write time
		__s32		m_ThumbDmdIdx;				// thumb DMD index
		__u32	m_ThumbMmdIdx;
		St_GrDvrRecMinThumb	m_ThumbData;	// thumb read / write data

		St_GrDvrFs2SmartIoc	m_SmartIoc;		//smart IOC

#ifdef LINUX_APP
		__s32		m_HndlDsk;
#else
		HANDLE	m_HndlDsk;
#endif

	protected:
		// local
		BOOL8	LcSeek( __s64 A_Pos, BOOL8 A_IsBegin = TRUE );
		BOOL8	LcRead( void* A_PtrBuf, __u32 A_Size );
		BOOL8	LcWrite( void* A_PtrBuf, __u32 A_Size );
		BOOL8	LcPread( __u64 A_Pos, void* A_PtrBuf, __u32 A_Size );
		BOOL8	LcPwrite( __u64 A_Pos, void* A_PtrBuf, __u32 A_Size );
		void	LcFlush( void );

		BOOL8	LcCopy( __u64 A_PosTg, __u64 A_PosSr, __u32 A_Size );
		BOOL8	LcFillByte( __u64 A_PosTg, __u32 A_Size, __u8 A_Fill );

		void	LcInit( void );										// disk initialize
		void	LcRcaWrite( void );								// RCA write
		BOOL8	LcTimeBeginByBdt( void );
		BOOL8	LcTimeEndByBdt( void );
		BOOL8	LcTimeBeginByMmd( void );
		BOOL8	LcTimeEndByMMd( void );
		BOOL8	LcTimeBeginUpdt( void );
		BOOL8	LcTimeEndUpdt( void );

		void	LcDmdWrite( void );
		void	LcDmdCheck( void );
		__s32		LcDmdFind( Def_GrTime A_Day );
		__s32		LcDmdAlloc( void );
		__s32		LcDmdNearFw( Def_GrTime A_Time );
		__s32		LcDmdNearBw( Def_GrTime A_Time );
		__s32		LcDmdLowest( void );
		__s32		LcDmdHighest( void );

		BOOL8	LcCvtTma( void );
		BOOL8	LcIsV1ValidHndl( __u64 A_V1Hndl );

		BOOL8	LcBdtChkIntra( void );
		BOOL8	LcBdtWriteByBlkId( __u32 A_BlkId );

		void	LcNormalStatUpdt( void );

		BOOL8	LcEvtLogDisable( void );

		BOOL8	LcBlockWrite( __u32 A_BlkId, __u32 A_Ofs, void* A_PtrBuf, __u32 A_Size, BOOL8 A_IsLhndlUpdt );

		BOOL8	LcMmdLoadByDmdIdx( __u32 A_DmdIdx, void* A_PtrBuf );
		__s32		LcMmdFindOldestThumb( __u32 A_DmdIdx, Ptr_GrDvrRecMinThumb A_PtrRtThumb );	// result = -1, not found, other = found index
		__s32		LcMmdFindNewestThumb( __u32 A_DmdIdx, Ptr_GrDvrRecMinThumb A_PtrRtThumb );	// result = -1, not found, other = found index

		BOOL8	LcElaDisable( void );

		BOOL8	LcThumbUpdt( Ptr_GrDvrFs2BlkAddInfo A_PtrInfo );	// when thumb update return TRUE.
		BOOL8	LcThumbWrite( void );
		BOOL8	LcThumbDirWrite( Def_GrTime A_Time, Ptr_GrDvrRecMinThumb A_PtrThumb );

		BOOL8	LcSmartRead( void );
		BOOL8	LcSmartFindRawByte( __u8 A_Id, __u8* A_PtrRtVal );

	public:
    Cls_GrDvrFs2Disk( __u8 A_DevIdx, void* A_PtrTmpBuf, __u64 A_DvrId );
    virtual		~Cls_GrDvrFs2Disk();

		BOOL8	IsValidHndl( __u64 A_Hndl );
		__u8	DiskStatGet( void );
		__u8	DevIdxGet( void );
		__u8	GrpIntIdxGet( void );
		__u64	RecLastHndlGet( void );			// last recorded handle get
		Def_GrTime	BeginTimeGet( void );
		Def_GrTime	EndTimeGet( void );
		BOOL8	RecSizeGet( __u64* A_PtrRtTotal, __u64* A_PtrRtRec );
		__u64	HndlFromDpos( __u64 A_DiskPos );
		__u64	TimeRecSizeCalc( __u16 A_ChMap, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd );

		Ptr_GrDvrFs2Bdt	BdtTableGet( void );
		BOOL8	IsBdtOverlap( __u32 A_RecId );
		BOOL8	BdtSave( void );
		BOOL8	IsBdtCrossCheck( Cls_GrDvrFs2Disk* A_Disk );
		BOOL8	BdtRecIdGather( __u32 A_BaseId );

		BOOL8	IsV1System( void );
		BOOL8	IsV1Full( void );
		__u32	V1PastBlockCnt( void );
		__u32	V1PresentBlockCnt( void );
		BOOL8	V1FsUpgrade( __u32 A_PastRecId, __u32 A_NearRecId );

		__u32	BlockCountGet( void );
		BOOL8	BlockEmptyFind( __u32* A_PtrRtIdx );
		__u32	BlockFreeSizeGet( __u32 A_BlkIdx );
		void	BlockFullSet( __u32 A_BlkIdx );
		__u32	BlockMaxRecIdFind( __u32* A_PtrRtIdx );
		__u32	BlockMinRecIdFind( __u32* A_PtrRtIdx );
		void	BlockDelete( __u32 A_BlkIdx );
		BOOL8	BlockDataAdd( __u32 A_BlkId, __u32 A_RecId, void* A_PtrBuf, __u32 A_Size, Ptr_GrDvrFs2BlkAddInfo A_PtrInfo, BOOL8 A_IsBdtUpdt );
		BOOL8	BlockInfoGet( __u32 A_BlkId, __u32* A_PtrRtRecId, __u32* A_PtrRtDataSize, __u16* A_PtrRtOvwtId );
		__u32	BlockFwRecIdFind( __u32 A_RecId, __u32* A_PtrRtBlkId, __u16* A_PtrRtOvwtId, __u32* A_PtrRtRecSize );
		__u32	BlockBwRecIdFind( __u32 A_RecId, __u32* A_PtrRtBlkId, __u16* A_PtrRtOvwtId, __u32* A_PtrRtRecSize );
		BOOL8	BlockInfoFlush( __u32 A_BlkId );

		void	OverwriteSet( BOOL8 A_IsOvwt );	// set over writable
		void	GrpIntIdxSet( __u8 A_Idx );			// group internal index set

		BOOL8	Format( BOOL8 A_IsWithLog );
		BOOL8	PacketReplace( __u64 A_Hndl, Ptr_GrDvrAvHd A_PtrPkt );

		BOOL8	ReadByHndl( __u64 A_Hndl, void* A_PtrBuf, __u32 A_Size );

		__u32	DayMapGet( Def_GrTime A_TimeSt, Def_GrTime A_TimeEd );

		BOOL8	MmdMapCombine( Def_GrTime A_TimeSt, void* A_PtrBuf, __u32 A_Cnt );	// combine record thumb table
		Def_GrTime	RecDayFindFw( Def_GrTime A_TimeBase );
		Def_GrTime	RecDayFindBw( Def_GrTime A_TimeBase );

		__u64	TimeHndlFindFw( Def_GrTime A_TimeSt, Def_GrTime* A_PtrRtTime );
		__u64	TimeHndlFindBw( Def_GrTime A_TimeSt, Def_GrTime* A_PtrRtTime );

		BOOL8	EvtLogAdd( Ptr_GrDvrLogItm A_PtrLog );
		void	EvtLogGetNext( Ptr_GrDvrLogCtrl A_PtrCtrl );
		void	EvtLogGetPrev( Ptr_GrDvrLogCtrl A_PtrCtrl );

		BOOL8	UcaWrite( __u32 A_Pos, void* A_PtrBuf, __u32 A_Size );
		BOOL8	UcaRead( __u32 A_Pos, void* A_PtrBuf, __u32 A_Size );

		BOOL8	VssReset( void );
		BOOL8	VssWrite( __u32 A_Pos, void* A_PtrBuf, __u32 A_Size );
		BOOL8	VssRead( __u32 A_Pos, void* A_PtrBuf, __u32 A_Size );

		__u8	TemperatureGet( void );
};
	
//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif


