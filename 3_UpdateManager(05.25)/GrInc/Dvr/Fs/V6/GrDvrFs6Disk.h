/*
	GAUSE DVR file system v5 disk

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrDvrFs6Disk
#define	_EPC_GrDvrFs6Disk

//====================================================================
// uses
#include	<GrTypeBase.h>
#include	<GrBufTool.h>
#include	<Dvr/GrDvrBase.h>
#include	<Dvr/Fs/V6/GrDvrFs6Base.h>
	
//====================================================================
//constance

// TMP buffer size
#define E_GrDvrFs6DiskFillTmpSize					0x100000

#define	E_GrDvrFs6DiskModelNameMaxLen			128

// smart
#define E_GrDvrFs6DiskSmartValIdxId				0
#define E_GrDvrFs6DiskSmartValIdxStat0		1
#define E_GrDvrFs6DiskSmartValIdxStat1		2
#define E_GrDvrFs6DiskSmartValIdxValue		3
#define E_GrDvrFs6DiskSmartValIdxVendor		4
#define E_GrDvrFs6DiskSmartValIdxRawVal		5		/* 6byte */

#define E_GrDvrFs6DiskSmartValItemSize		12

#define E_GrDvrFs6DiskSmartValCnt					30

#define E_GrDvrFs6DiskSmartIdTemp					194


#define E_GrDvrFs6DiskDbgMsgView

//====================================================================
//global macro

//====================================================================
//global type


typedef struct St_GrDvrFs6SmartData 
{
	__u16		Revision;
	__u8		Values[E_GrDvrFs6DiskSmartValCnt * E_GrDvrFs6DiskSmartValItemSize];
	__u8		Offline_status;
	__u8		Vendor1;
	__u16		Offline_timeout;
	__u8		Vendor2;
	__u8		Offline_capability;
	__u16		Smart_capability;
	__u8		_rsv[16];
	__u8		VendorSpc[125];
	__u8		Checksum;
} *Ptr_GrDvrFs6SmartData;

typedef	struct St_GrDvrFs6DiskIocSmart
{
	__u8	Cmds[4];
	St_GrDvrFs6SmartData	Data;
}	*Ptr_GrDvrFs6DiskIocSmart;

//====================================================================
//class

class	Cls_GrDvrFs6Disk
{
	private:

		__u8	m_DevIdx;					// device index
		__u8	m_DskStat;				// disk status - not exist , unformatted , bad , normal only

		__u64	m_DskSize;				// disk size

		__u32	m_BlkCnt;					// block count
		
		char	m_StrModel[E_GrDvrFs6DiskModelNameMaxLen];

		// runtime
		Ptr_GrDvrFs6BlkBaseHead	m_BlkHdBuf;		// block head buffer

		// SMART
		St_GrDvrFs6DiskIocSmart	m_SmartIoc;		//smart IOC

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

		BOOL8	LcCchPread(__u64 A_Adr, void* A_PtrBuf, __u32 A_Size);
		BOOL8	LcCchPwrite(__u64 A_Adr, void* A_PtrBuf, __u32 A_Size);

		BOOL8	LcCchBlkRead(__u64 A_BlkAdr, __u32 A_BlkOfs, void* A_PtrBuf, __u32 A_Size);
		BOOL8	LcCchBlkWrite(__u64 A_BlkAdr, __u32 A_BlkOfs, void* A_PtrBuf, __u32 A_Size);

		__s32	LcCchFindByAdr(__u64 A_DskAdr);
		__s32	LcCchFindEmpty(void);
		__s32	LcCchFindOldest(void);
		BOOL8	LcCchItemSave(__s32 A_ItmIdx);
		BOOL8	LcCchItemLoad(__s32 A_ItmIdx, __u64 A_DskAdr);

		void	LcInit( void );										// disk initialize
		BOOL8	LcBlkHdLoad(void);								// block heads load
		__u32	LcBlkGetDbCnt(void);							// DB block count

		BOOL8	LcSmartRead( void );
		BOOL8	LcSmartFindRawByte( __u8 A_Id, __u8* A_PtrRtVal );

	public:
    Cls_GrDvrFs6Disk( __u8 A_DevIdx );
    virtual		~Cls_GrDvrFs6Disk();

		// info
		__u8	DiskStatGet(void);
		Ptr_GrDvrFs6BlkBaseHead	BlkHeadTableGet(__u32* A_PtrRtCnt);
		__u8	DevIdxGet(void);


		// control
		BOOL8	BlkWriteHeadByIdx(__u32 A_BlkIdx, BOOL8 A_IsForce = FALSE);
		BOOL8	BlkWriteHeadAll(BOOL8 A_IsForce);

		__u8	BlkWriteDataByOfs(__u32 A_BlkIdx, __u32 A_Ofs, void* A_PtrData, __u32 A_Size);
		__u8	BlkReadDataByOfs(__u32 A_BlkIdx, __u32 A_Ofs, void* A_PtrData, __u32 A_Size);

		__u8	BlkDbAddData(__u32 A_BlkIdx, void* A_PtrBuf, __u32 A_Size,__u32* A_PtrRtOfs);

		void	StatusUpdate(void);
		__u8	DiskInfoGet(__u64* A_PtrRtTotal, __u64* A_PtrRtRec);

		BOOL8	Format(__u8 A_GroupType,__u32 A_SeqLast, __u32 A_SeqOvwt, BOOL8 A_IsPrgsBrdcst);

		__u8	TempratureRead( void );

		void	ModelNameGet(WCHAR* A_StrRt);
};
	
//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif


