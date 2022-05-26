/*
	GAUSE DVR file system v5 disk

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrFsNvtHdd
#define	_EPC_GrFsNvtHdd

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrBufTool.h>
#include <GrError.h>
#include <Dvr/GrDvrBase.h>
#include <Dvr/Fs/Nvt/GrFsNvtBase.h>
	
//====================================================================
//constance

#define	E_GrFsNvtHddIoUnitSize					0x1000
#define	E_GrFsNvtHddIoAdrMask						0xFFFFF000
#define	E_GrFsNvtHddIoOfsMask						0x0FFF

#define	E_GrFsNvtHddDayInfoBufSize			0x1000

// TMP buffer size
#define E_GrFsNvtHddFillTmpSize					0x10000

#define	E_GrFsNvtHddModelNameMaxLen			128

#define	E_GrFsNvtHddIovMaxCnt						1024

#define	E_GrFsNvtHddFillZeroIovCnt			16

// smart
#define E_GrFsNvtHddSmartValIdxId				0
#define E_GrFsNvtHddSmartValIdxStat0		1
#define E_GrFsNvtHddSmartValIdxStat1		2
#define E_GrFsNvtHddSmartValIdxValue		3
#define E_GrFsNvtHddSmartValIdxVendor		4
#define E_GrFsNvtHddSmartValIdxRawVal		5		/* 6byte */

#define E_GrFsNvtHddSmartValItemSize		12

#define E_GrFsNvtHddSmartValCnt					30

#define E_GrFsNvtHddSmartIdSeekErrRate	7		//Seek Error Rate
#define E_GrFsNvtHddSmartIdPoh					9			/* power on time */
#define E_GrFsNvtHddSmartIdTemp					194
#define E_GrFsNvtHddSmartIdReallocate		196 //Reallocation Event Count
#define E_GrFsNvtHddSmartIdBadSect			198
#define E_GrFsNvtHddSmartIdReadErrRate	201	//Soft Read Error Rate or TA Counter Detected

#define E_GrFsNvtHddDbgMsgView

//====================================================================
//global macro

//====================================================================
//global type


typedef struct St_GrFsNvtSmartData 
{
	__u16		Revision;
	__u8		Values[E_GrFsNvtHddSmartValCnt * E_GrFsNvtHddSmartValItemSize];
	__u8		Offline_status;
	__u8		Vendor1;
	__u16		Offline_timeout;
	__u8		Vendor2;
	__u8		Offline_capability;
	__u16		Smart_capability;
	__u8		_rsv[16];
	__u8		VendorSpc[125];
	__u8		Checksum;
} *Ptr_GrFsNvtSmartData;

typedef	struct St_GrFsNvtHddIocSmart
{
	__u8	Cmds[4];
	St_GrFsNvtSmartData	Data;
}	*Ptr_GrFsNvtHddIocSmart;

//====================================================================
//class

class	Cls_GrFsNvtHdd
{
	private:

		__u8	m_DevIdx;					// device index
		__u8	m_DskStat;				// disk status - not exist , unformatted , bad , normal only

		__u64	m_DskSize;				// disk size

		__u32	m_BlkCnt;					// block count
		__u32	m_BiBufSize;			// block info buffer size
		
		char	m_StrModel[E_GrFsNvtHddModelNameMaxLen];

#ifdef DVR_EMU_PHYDSK
		__u8	m_SctBuf[512];
#endif

		// runtime
		Ptr_GrFsNvtBlkInfoBase	m_BlkInfoBuf;	// block info buffer
		Ptr_GrFsNvtDayInfo		m_DayInfoTbl;

		// SMART
		St_GrFsNvtHddIocSmart	m_SmartIoc;		//smart IOC

#ifdef LINUX_APP
		__s32		m_HndlDsk;
#else
		HANDLE	m_HndlDsk;
#endif

	protected:
		// local
		void	LcFlush(void);

		void	LcInit( void );										// disk initialize

		void	LcCvtMit(void);

		BOOL8	LcSmartRead( void );
		BOOL8	LcSmartFindRawByte( __u8 A_Id, __u8* A_PtrRtVal );
		BOOL8	LcSmartFindRawDword(__u8 A_Id, __u32* A_PtrRtVal);
		BOOL8	LcSmartFindRawQword(__u8 A_Id, __u64* A_PtrRtVal);

	public:
    Cls_GrFsNvtHdd( __u8 A_DevIdx );
    virtual		~Cls_GrFsNvtHdd();

		// info
		__u8	DiskStatGet(void);
		Ptr_GrFsNvtBlkInfoBase	BlkInfoTableGet(__u32* A_PtrRtCnt,BOOL8 A_IsForce = FALSE);
		__u8	DevIdxGet(void);
		Ptr_GrFsNvtDayInfo	DayInfoTableGet(void);

		// control
		Def_GrErrCode	BlkInfoWriteByIdx(__u32 A_BlkIdx);
		BOOL8	BlkInfoWriteAll(void);

		Def_GrErrCode	BlkReadPage(__u32 A_BlkIdx, __u32 A_Ofs, void* A_PtrData, __u32 A_Size);
		Def_GrErrCode	BlkWritePage(__u32 A_BlkIdx, __u32 A_Ofs, void* A_PtrData, __u32 A_Size);

		void	StatusUpdate(void);
		__u8	DiskInfoGet(__u64* A_PtrRtTotal, __u64* A_PtrRtRec);

		void	ModelNameGet(WCHAR* A_StrRt);

		BOOL8	SmartRead(Ptr_GrFsNvtSmart A_PtrRtData);

		Def_GrErrCode	DayInfoWrite(void);

};
	
//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif


