/*
	GAUSE DVR file system v4 disk

	version history
		2013.5.8		-	1 made

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrDvrFs4Disk
#define	_EPC_GrDvrFs4Disk

//====================================================================
// uses
#include	<GrTypeBase.h>
#include	<GrBufTool.h>
#include	<Dvr/GrDvrBase.h>
#include	<Dvr/Fs/V4/GrDvrFs4Base.h>
	
//====================================================================
//constance

#define E_GrDvrFs4Fcc											Mac_GrMakeFcc('F','S','V','4')

#define E_GrDvrFs4Ver											1		/* sub version */

#define E_GrDvrFs4ElaSize									0x0400000		/* ELA size 4 Mega byte */
#define E_GrDvrFs4UcaMaxSize							0xFE000

// fixed address
#define E_GrDvrFs4AdrMia									0x00000000		/* media info area */
#define E_GrDvrFs4AdrRca									0x00001000		/* run time control area */
#define E_GrDvrFs4AdrUca									0x00002000		/* user configuration area */
#define E_GrDvrFs4AdrEla									0x00100000		/* event log area */
#define E_GrDvrFs4AdrBdt									0x01000000		/* block directory table */
#define E_GrDvrFs4AdrPos									0x02000000		/* Pos data area */

#define	E_GrDvrFs4AdrRda									0x0000000020000000LL		/* RDA */


#define	E_GrDvrFs4DiskMinDskSize					0x0000000200000000LL
#define E_GrDvrFs4DiskMaxDskSize					0x0000100000000000LL

// sequence id

// BDT
#define E_GrDvrFs4BdtMaxCnt								0xFFFE

#define E_GrDvrFs4BdtStatNormal						0
#define E_GrDvrFs4BdtStatFull							1

#define E_GrDvrFs4BdtStatBad							0xFF

// block
#define E_GrDvrFs4BlockSize								0x0000000010000000LL
#define E_GrDvrFs4BlockOfsMask						0x0FFFFFE0
#define E_GrDvrFs4BlockShift							28

#define	E_GrDvrFs4BlockDataAdr						0x100000
#define	E_GrDvrFs4BlockRecAreaSize				(0x10000000 - E_GrDvrFs4BlockDataAdr)

// ELA
#define E_GrDvrFs4ElaMaxCnt								(E_GrDvrFs4ElaSize >> E_GrDvrLogShiftSize)

// POS
#define E_GrDvrFs4PosAreaSize							0x8000000

// TMP buffer size
#define E_GrDvrFs4DiskFillTmpSize					0x100000
#define	E_GrDvrFs4DiskElaTmpSize					0x100000

// smart
#define E_GrDvrFs4DiskSmartValIdxId				0
#define E_GrDvrFs4DiskSmartValIdxStat0		1
#define E_GrDvrFs4DiskSmartValIdxStat1		2
#define E_GrDvrFs4DiskSmartValIdxValue		3
#define E_GrDvrFs4DiskSmartValIdxVendor		4
#define E_GrDvrFs4DiskSmartValIdxRawVal		5		/* 6byte */

#define E_GrDvrFs4DiskSmartValItemSize		12

#define E_GrDvrFs4DiskSmartValCnt					30

#define E_GrDvrFs4DiskSmartIdTemp					194

#define E_GrDvrFs4DiskMmdCmbBufPos				(E_GrDvrFs4TmpBufSize - E_GrDvrFs4MmdDaySize)

#define E_GrDvrFs4DiskDbgMsgView

//====================================================================
//global macro

//====================================================================
//global type

typedef	struct St_GrDvrFs4Mia
{
	__u32	FsFcc;			// file system FCC code
	__u32	FsVer;			// file system version
	__u32	BlkCnt;			// record block count
}	*Ptr_GrDvrFs4Mia;

typedef	struct	St_GrDvrFs4Rca
{
	BOOL8	IsEvtOvwt;		// ELA overwrited
	__u8	_rsvd0;				// reserved
	__u16	_rsvd1;
	__u32	EvtPosIn;			// ELA input position
	__u64	DvrId;				// DVR id
}	*Ptr_GrDvrFs4Rca;

typedef struct St_GrDvrFs4Bdt		/* 24 byte */
{
	__u8	Stat;
	BOOL8	IsNrib;		// need record id rebuild - runtime only
	__u16	BitCnt;		// BIT count

	__u16	OvwtId;		// overwrite ID
	__u16	_rsvd0;

	__u32	SeqId;		// 0 = not use
	__u32	InSize;
	__u32	TimeSt;
	__u32	TimeEd;
}	*Ptr_GrDvrFs4Bdt;

typedef struct St_GrDvrFs4SmartData 
{
	__u16		Revision;
	__u8		Values[E_GrDvrFs4DiskSmartValCnt * E_GrDvrFs4DiskSmartValItemSize];
	__u8		Offline_status;
	__u8		Vendor1;
	__u16		Offline_timeout;
	__u8		Vendor2;
	__u8		Offline_capability;
	__u16		Smart_capability;
	__u8		_rsv[16];
	__u8		VendorSpc[125];
	__u8		Checksum;
} *Ptr_GrDvrFs4SmartData;

typedef	struct St_GrDvrFs4DiskIocSmart
{
	__u8	Cmds[4];
	St_GrDvrFs4SmartData	Data;
}	*Ptr_GrDvrFs4DiskIocSmart;

//====================================================================
//class

class	Cls_GrDvrFs4Disk
{
	private:
		void*	m_FsMng;					// file system manager

		__u8	m_DevIdx;					// device index
		__u8	m_GrpIntIdx;			// group internal index
		__u8	m_DskStat;				// disk status

		__u64	m_RecCap;					// record capable - MByte unit
		__u64	m_RecSize;				// recorded size - MByte unit
		
		__u64	m_DskSize;				// disk size

		St_GrDvrFs4Mia	m_Mia;
		St_GrDvrFs4Rca	m_Rca;
		St_GrDvrFs4Bdt	m_Bdt[E_GrDvrFs4BdtMaxCnt];

		// runtime
		Def_GrTime	m_RecTimeBegin;
		Def_GrTime	m_RecTimeEnd;
		__u64				m_HndlLastRec;


		// SMART
		St_GrDvrFs4DiskIocSmart	m_SmartIoc;		//smart IOC

#ifdef LINUX_APP
		__s32		m_HndlDsk;
#else
		HANDLE	m_HndlDsk;
#endif

	protected:
		// local
		BOOL8	LcSeek(__s64 A_Adr, BOOL8 A_IsBegin = TRUE);
		BOOL8	LcRead( void* A_PtrBuf, __u32 A_Size );
		BOOL8	LcWrite( void* A_PtrBuf, __u32 A_Size );
		BOOL8	LcPread( __u64 A_Adr, void* A_PtrBuf, __u32 A_Size );
		BOOL8	LcPwrite(__u64 A_Adr, void* A_PtrBuf, __u32 A_Size);
		BOOL8	LcIovPread(__u64 A_Adr, iovec* A_PtrIovBuf, __u32 A_IovCnt);
		BOOL8	LcIovPwrite(__u64 A_Adr, iovec* A_PtrIovBuf, __u32 A_IovCnt);
		void	LcFlush(void);
		BOOL8	LcFillZero(__u64 A_Adr, __u32 A_Size);

		void	LcInit( void );										// disk initialize
		BOOL8	LcRcaUpdt( void );								// RCA update
		void	LcTimeRngUpdt( void );						// time range update

		__u64	LcHndlEncByRdaPos( __u64 A_RdaPos );
		__u64	LcHndlEncByBlkId(__u32 A_BlkId);

		BOOL8	LcBdtWriteByBlkId( __u32 A_BlkId );

		BOOL8	LcBitWriteByChg(__u32 A_BlkIdx, Ptr_GrDvrFs4Bit A_PtrBitBuf);
		void	LcBitCchUpdtLife(Ptr_GrDvrFs4GrpWorkEnv A_PtrWork);
		void	LcBitCchDelByBlkId(__u32 A_BlkId, Ptr_GrDvrFs4GrpWorkEnv A_PtrWork);
		void	LcBitCchDelThisDisk(Ptr_GrDvrFs4GrpWorkEnv A_PtrWork);
		Ptr_GrDvrFs4Bit	LcBitCchGet(__u32 A_BlkId, Ptr_GrDvrFs4GrpWorkEnv A_PtrWork);
		
		void	LcStatUpdtNormal(void);

		__u8	LcBlockChkRecAble(__u32 A_BlkId, __u32 A_SeqId);
		BOOL8	LcBlockLoadBit(__u32 A_BlkId, void* A_PtrBuf);
		__u8	LcBlockMakeRecIov(__u32 A_BlkId, Ptr_GrDvrFs4RecReqInfo A_PtrReq, 
			__u32 A_ReqPrgs, __u32* A_PtrRtPrgs, Ptr_GrDvrFs4Bit A_PtrBitBuf,Ptr_GrDvrFs4GrpWorkEnv A_PtrWork);
		void	LcBlockAddRecInfo(__u8 A_Ch, __u8 A_Type, __u8 A_Atb, Def_GrTime A_Time, BOOL8 A_IsSm, 
			__u32 A_Size, __u32 A_BlkId, __u32 A_BlkOfs, Ptr_GrDvrFs4Bit A_PtrBitBuf);




		BOOL8	LcElaDisable( void );


		BOOL8	LcSmartRead( void );
		BOOL8	LcSmartFindRawByte( __u8 A_Id, __u8* A_PtrRtVal );

	public:
    Cls_GrDvrFs4Disk( __u8 A_DevIdx, void* A_FsMng );
    virtual		~Cls_GrDvrFs4Disk();

		// info
		BOOL8	IsValidHndl( __u64 A_Hndl );
		__u8	DiskStatGet( void );
		__u8	DevIdxGet( void );
		__u8	GrpIntIdxGet( void );
		__u64	RecLastHndlGet( void );			// last recorded handle get
		Def_GrTime	BeginTimeGet( void );
		Def_GrTime	EndTimeGet( void );
		BOOL8	RecSizeGet( __u64* A_PtrRtTotal, __u64* A_PtrRtRec );
		__u64	HndlFromRdaPos( __u64 A_RdaPos );

		// BDT
		Ptr_GrDvrFs4Bdt	BdtGetTable( void );
		BOOL8	BdtSave( void );

		// block
		__u8	BlockAddData(__u32 A_BlkId, __u32 A_SeqId, __u32 A_Prgs, Ptr_GrDvrFs4RecReqInfo A_PtrReq, __u32* A_PtrRtPrgs, Ptr_GrDvrFs4GrpWorkEnv A_PtrWork);
		BOOL8	BlockSetStatFull(__u32 A_BlkId);
		__u32	BlockGetCount(void);

		BOOL8	BlockFindEmpty(__u32* A_PtrRtBlkId);
		__u32	BlockFindFirst(__u32* A_PtrRtIdx);
		__u32	BlockFindLast(__u32* A_PtrRtIdx);
		BOOL8	BlockDelete(__u32 A_BlkId, Ptr_GrDvrFs4GrpWorkEnv A_PtrWork);

		BOOL8	Format(BOOL8 A_IsWithLog);

		void	GrpIntIdxSet(__u8 A_Idx);			// group internal index set

		BOOL8	HndlFindSeqIdFw(__u32 A_SeqId, __u64* A_PtrRtHndl, __u32* A_PtrRtSeqId);
		BOOL8	HndlFindSeqIdBw(__u32 A_SeqId, __u64* A_PtrRtHndl, __u32* A_PtrRtSeqId);
		BOOL8	HndlFindFirst(__u64* A_PtrRtHndl, __u32* A_PtrRtSeqId);
		BOOL8	HndlFindTimeFw(Def_GrTime A_Time, __u64* A_PtrRtHndl, __u32* A_PtrRtSeqId, __u32* A_PtrRtTimeGap, Ptr_GrDvrFs4GrpWorkEnv A_PtrWork);
		BOOL8	HndlFindTimeBw(Def_GrTime A_Time, __u64* A_PtrRtHndl, __u32* A_PtrRtSeqId, __u32* A_PtrRtTimeGap, Ptr_GrDvrFs4GrpWorkEnv A_PtrWork);

		__u8	ReadByHndlFw(__u64 A_Hndl, __u32 A_Ofs, void* A_PtrBuf, __u32 A_BufSize, __u32* A_PtrRtSize, __u32* A_PtrRtSeqId, __u64* A_PtrRtHndl);
		__u8	ReadByHndlBw(__u64 A_Hndl, __u32 A_Ofs, void* A_PtrBuf, __u32 A_BufSize, __u32* A_PtrRtSize, __u32* A_PtrRtSeqId, __u64* A_PtrRtHndl);

		__u32	DayMapGet(Def_GrTime A_TimeSt, Def_GrTime A_TimeEd, Ptr_GrDvrFs4GrpWorkEnv A_PtrWork);
		BOOL8	MinMapCombine(Def_GrTime A_TimeSt, Def_GrTime A_TimeEd, Ptr_GrDvrRecMinMap A_PtrRtBuf, Ptr_GrDvrFs4GrpWorkEnv A_PtrWork);	// combine record thumb table

		BOOL8	LogAdd( Ptr_GrDvrLogItm A_PtrLog );
		void	LogGetNext( Ptr_GrDvrLogCtrl A_PtrCtrl );
		void	LogGetPrev( Ptr_GrDvrLogCtrl A_PtrCtrl );

		__u8	TempratureRead( void );

		void	BkupSizeEstmProc(Ptr_GrDvrFs4BkupSizeEstm A_PtrCtrl, Ptr_GrDvrFs4GrpWorkEnv A_PtrWork);

};
	
//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif


