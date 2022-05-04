/*
	GAUSE DVR file system v3 disk

	version history
		2012.4.27		-	1 made

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrDvrFs3Disk
#define	_EPC_GrDvrFs3Disk

//====================================================================
// uses
#include	<GrTypeBase.h>
#include	<GrBufTool.h>
#include	<Dvr/GrDvrBase.h>
#include	<Dvr/Fs/V3/GrDvrFs3Base.h>
	
//====================================================================
//constance

#define E_GrDvrFs3Fcc									Mac_GrMakeFcc('F','S','D','3')

#define E_GrDvrFs3Ver									5		/* version */

// default size
#define E_GrDvrFs3ElaDfltSize					0x0400000		/* default ELA size 4 Mega byte */
//#define E_GrDvrFs3VsaDfltSize					0x0100000		/* default VSA size 1 Mega byte */

#define E_GrDvrFs3DfltPosRda					0x0000000100000000LL	/* record data area position */

// fixed size
#define E_GrDvrFs3UcaMaxSize					0xF0000
#define E_GrDvrFs3ElaMinSize					0x100000
#define E_GrDvrFs3ElaMaxSize					0xF00000

// fixed address
#define E_GrDvrFs3PosMis							0x00000000		/* media info sector */
#define E_GrDvrFs3PosRca							0x00001000		/* run time control area */
#define E_GrDvrFs3PosDmd							0x00002000		/* date map data */
#define E_GrDvrFs3PosUca							0x00010000		/* user configuration area */
#define E_GrDvrFs3PosEla							0x00100000		/* event log area */
#define E_GrDvrFs3PosMmd							0x01000000		/* minute map data */
#define E_GrDvrFs3PosBdt							0x02000000		/* block directory table */
#define E_GrDvrFs3PosBcs							0x03000000		/* block channel size table */
#define E_GrDvrFs3PosVsa							0x23000000		/* video surveillance service area */
//#define E_GrDvrFs3PosRda							0x0000000008000000LL	/* record data area position */

#define E_GrDvrFs3DiskRdaPosAlgMask		0x000000000FFFFFFFLL
#define E_GrDvrFs3DiskRdaPosAlgUnit		0x0000000010000000LL

#define E_GrDvrFs3DiskMinRdaSize			0x0000000100000000LL
#define E_GrDvrFs3DiskMaxRdaSize			0x0000080000000000LL

// BDT
#define E_GrDvrFs3BdtMaxCnt						8192

// BCS
#define E_GrDvrFs3BcsMaxCnt						963
#define E_GrDvrFs3BcsAreaSize					0x20000000
#define E_GrDvrFs3BcsUnitSize					0x10000

// block
#define E_GrDvrFs3BlockSize						0x40000000
#define E_GrDvrFs3BlockOfsMask				0x3FFFFFFF
#define E_GrDvrFs3BlockShift					30
	
// time map
#define E_GrDvrFs3DmdItemCnt					512

#define E_GrDvrFs3MmdDayItemCnt				(24 * 60)
#define E_GrDvrFs3MmdDaySize					(E_GrDvrFs3MmdDayItemCnt * sizeof(St_GrDvrRecMinThumb))

#define E_GrDvrFs3MmdTotalItemCnt			(E_GrDvrFs3MmdDayItemCnt * E_GrDvrFs3DmdItemCnt)
#define E_GrDvrFs3MmdTotalSize				(E_GrDvrFs3MmdTotalItemCnt * sizeof(St_GrDvrRecMinThumb))

#define E_GrDvrFs3RecSizeWorkUnit			4

// smart
#define E_GrDvrFs3DiskSmartValIdxId			0
#define E_GrDvrFs3DiskSmartValIdxStat0	1
#define E_GrDvrFs3DiskSmartValIdxStat1	2
#define E_GrDvrFs3DiskSmartValIdxValue	3
#define E_GrDvrFs3DiskSmartValIdxVendor	4
#define E_GrDvrFs3DiskSmartValIdxRawVal	5		/* 6byte */

#define E_GrDvrFs3DiskSmartValItemSize	12

#define E_GrDvrFs3DiskSmartValCnt				30

#define E_GrDvrFs3DiskSmartIdTemp				194

#define E_GrDvrFs3DiskMmdCmbBufPos			(E_GrDvrFs3TmpBufSize - E_GrDvrFs3MmdDaySize)

#define E_GrDvrFs3DiskDbgMsgView

//====================================================================
//global macro

//====================================================================
//global type

typedef	struct St_GrDvrFs3Mis
{
	__u32	FsFcc;			// file system FCC code
	__u32	FsVer;			// file system version
	__u32	StrmFcc;		// stream FCC
	__u32	ElaSize;		// ELD size
	__u32	VsaSize;		// VSS size
	__u32	BlkCnt;			// record block count
	__u64	RdaPos;			// record data area position
}	*Ptr_GrDvrFs3Mis;

typedef	struct	St_GrDvrFs3Rca
{
	BOOL8	IsEvtOvwt;		// ELA overwrote
	__u8	_rsvd0;				// reserved
	__u16	_rsvd1;
	__u32	EvtInIdx;			// ELA input position
	__u64	DvrId;				// DVR id
	//__u32	VsaPosIn;			// VSC input position
	//__u32	VsaPosOut;		// VSC output position

}	*Ptr_GrDvrFs3Rca;

typedef struct St_GrDvrFs3Bdt
{
	BOOL8	IsBad;
	BOOL8	IsFull;
	__u16	OvwtId;
	__u32	RecId;

	__u32	TimeSt;
	__u32	TimeEd;

	__u32	RecSize;
	__u32	_rsvd0;

}	*Ptr_GrDvrFs3Bdt;

typedef struct St_GrDvrFs3Bcs
{
	__u32	Cnt;		// item count
	__u32	_rsvd0[12];
	__u32	TimeTbl[E_GrDvrFs3BcsMaxCnt];
	__u32	SizeTbl[E_GrDvrFs3BcsMaxCnt][E_GrDvrMaxChCnt];
}	*Ptr_GrDvrFs3Bcs;

typedef struct St_GrDvrFs3SmartData 
{
	__u16		Revision;
	__u8		Values[E_GrDvrFs3DiskSmartValCnt * E_GrDvrFs3DiskSmartValItemSize];
	__u8		Offline_status;
	__u8		Vendor1;
	__u16		Offline_timeout;
	__u8		Vendor2;
	__u8		Offline_capability;
	__u16		Smart_capability;
	__u8		_rsv[16];
	__u8		VendorSpc[125];
	__u8		Checksum;
} *Ptr_GrDvrFs3SmartData;

typedef	struct St_GrDvrFs3DiskIocSmart
{
	__u8	Cmds[4];
	St_GrDvrFs3SmartData	Data;
}	*Ptr_GrDvrFs3DiskIocSmart;

//====================================================================
//class

class	Cls_GrDvrFs3Disk
{
	private:
		__u8	m_DevIdx;					// device index
		__u8	m_GrpIntIdx;			// group internal index
		BOOL8	m_IsOvwtAble;			// overwrite able

		__u32	m_ReqVsaSize;			// request VSA size
		__u64	m_ReqRdaPos;			// request RDA position

		void*	m_PtrTmpBuf;
		__u64	m_DvrId;

		__u8	m_DskStat;				// disk status
		__u64	m_RecCap;					// record capable - MByte unit
		__u64	m_RecSize;				// recorded size - MByte unit
		
		__u64	m_DskSize;				// disk size

		St_GrDvrFs3Mis	m_Mis;
		St_GrDvrFs3Rca	m_Rca;
		St_GrDvrFs3Bdt	m_Bdt[E_GrDvrFs3BdtMaxCnt];
		Def_GrTime	m_Dmd[E_GrDvrFs3DmdItemCnt];

		// runtime
		Def_GrTime	m_RecTimeBegin;
		Def_GrTime	m_RecTimeEnd;
		__u64				m_HndlLastRec;

		// thumb write
		Def_GrTime	m_ThumbTime;			// thumb minute time
		__s32		m_ThumbDmdIdx;				// thumb DMD index
		__u32	m_ThumbMmdIdx;
		St_GrDvrRecMinThumb	m_ThumbData;	// thumb read / write data

		// BCS write
		St_GrDvrFs3Bcs	m_BcsData;
		__s32		m_BcsBlk;							// BCS block index

		// SMART
		St_GrDvrFs3DiskIocSmart	m_SmartIoc;		//smart IOC

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
		void	LcFlush( void );
		BOOL8	LcFillZero( __u64 A_Pos, __u32 A_Size );
		BOOL8	LcPread( __u64 A_Pos, void* A_PtrBuf, __u32 A_Size );
		BOOL8	LcPwrite( __u64 A_Pos, void* A_PtrBuf, __u32 A_Size );

		void	LcInit( void );										// disk initialize
		BOOL8	LcRcaUpdt( void );								// RCA update
		void	LcTimeRngUpdt( void );						// time range update

		void	LcDmdWrite( void );
		void	LcDmdCheck( void );
		__s32		LcDmdFind( Def_GrTime A_Day );
		__s32		LcDmdAlloc( void );
		__s32		LcDmdNearFw( Def_GrTime A_Time, Def_GrTime* A_PtrRtTime );
		__s32		LcDmdNearBw( Def_GrTime A_Time, Def_GrTime* A_PtrRtTime );

		__u64	LcLocalHndlByRdaPos( __u64 A_RdaPos );

		BOOL8	LcBdtChkIntra( void );
		BOOL8	LcBdtWriteByBlkId( __u32 A_BlkId );

		void	LcNormalStatUpdt( void );

		BOOL8	LcBlockWrite( __u32 A_BlkId, __u32 A_Ofs, void* A_PtrBuf, __u32 A_Size, BOOL8 A_IsLhndlUpdt );

		BOOL8	LcMmdLoadByDmdIdx( __u32 A_DmdIdx, void* A_PtrBuf );
		__s32		LcMmdFindOldestThumb( __u32 A_DmdIdx, Ptr_GrDvrRecMinThumb A_PtrRtThumb );	// result = -1, not found, other = found index
		__s32		LcMmdFindNewestThumb( __u32 A_DmdIdx, Ptr_GrDvrRecMinThumb A_PtrRtThumb );	// result = -1, not found, other = found index

		BOOL8	LcElaDisable( void );

		BOOL8	LcThumbUpdt( Ptr_GrDvrFs3BlkAddInfo A_PtrInfo );	// when time changed return TRUE.
		BOOL8	LcThumbWrite( void );

		BOOL8	LcBcsUpdt( __u32 A_BlkIdx, Ptr_GrDvrFs3BlkAddInfo A_PtrInfo );
		BOOL8	LcBcsWrite( void );
		void	LcBcsRecSizeScan( Ptr_GrDvrFs3RecSizeReq A_PtrReq );

		BOOL8	LcSmartRead( void );
		BOOL8	LcSmartFindRawByte( __u8 A_Id, __u8* A_PtrRtVal );

	public:
    Cls_GrDvrFs3Disk( __u8 A_DevIdx, void* A_PtrTmpBuf, __u64 A_DvrId, __u32 A_ReqVsaSize );
    virtual		~Cls_GrDvrFs3Disk();

		BOOL8	IsValidHndl( __u64 A_Hndl );
		__u8	DiskStatGet( void );
		__u8	DevIdxGet( void );
		__u8	GrpIntIdxGet( void );
		__u64	RecLastHndlGet( void );			// last recorded handle get
		Def_GrTime	BeginTimeGet( void );
		Def_GrTime	EndTimeGet( void );
		BOOL8	RecSizeGet( __u64* A_PtrRtTotal, __u64* A_PtrRtRec );
		__u64	HndlFromRdaPos( __u64 A_RdaPos );

		Ptr_GrDvrFs3Bdt	BdtTableGet( void );
		BOOL8	IsBdtOverlap( __u32 A_RecId );
		BOOL8	BdtSave( void );
		BOOL8	IsBdtCrossCheck( Cls_GrDvrFs3Disk* A_Disk );

		__u32	BlockCountGet( void );
		BOOL8	BlockEmptyFind( __u32* A_PtrRtIdx );
		__u32	BlockFreeSizeGet( __u32 A_BlkIdx );
		void	BlockFullSet( __u32 A_BlkIdx );
		__u32	BlockMaxRecIdFind( __u32* A_PtrRtIdx );
		__u32	BlockMinRecIdFind( __u32* A_PtrRtIdx );
		void	BlockDelete( __u32 A_BlkIdx );
		BOOL8	BlockDataAdd( __u32 A_BlkId, __u32 A_RecId, void* A_PtrBuf, __u32 A_Size, Ptr_GrDvrFs3BlkAddInfo A_PtrInfo, BOOL8 A_IsBdtUpdt );
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

		BOOL8	MinMapCombine( Def_GrTime A_TimeSt, void* A_PtrBuf, __u32 A_Cnt );	// combine record thumb table
		Def_GrTime	RecDayFindFw( Def_GrTime A_TimeBase );
		Def_GrTime	RecDayFindBw( Def_GrTime A_TimeBase );

		__u64	TimeHndlFindFw( Def_GrTime A_TimeSt, Def_GrTime* A_PtrRtTime );
		__u64	TimeHndlFindBw( Def_GrTime A_TimeSt, Def_GrTime* A_PtrRtTime );

		BOOL8	LogAdd( Ptr_GrDvrLogV2Item A_PtrLog );
		void	LogGetNext( Ptr_GrDvrLogV2Ctrl A_PtrCtrl );
		void	LogGetPrev( Ptr_GrDvrLogV2Ctrl A_PtrCtrl );

		__u8	TempratureRead( void );

		BOOL8	ScanRecSize( Ptr_GrDvrFs3RecSizeReq A_PtrReq );	// result - go next

};
	
//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif


