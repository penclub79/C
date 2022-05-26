/*
	GAUSE DVR file system v5 disk

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrFs5Hdd
#define	_EPC_GrFs5Hdd

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrBufTool.h>
#include <GrError.h>
#include <Dvr/GrDvrBase.h>
#include <Dvr/Fs/V5/GrFs5Base.h>
	
//====================================================================
//constance

#define	E_GrFs5HddIoUnitSize					0x1000
#define	E_GrFs5HddIoAdrMask						0xFFFFF000
#define	E_GrFs5HddIoOfsMask						0x0FFF

#define	E_GrFs5HddDayInfoBufSize			0x1000

// TMP buffer size
#define E_GrFs5HddFillTmpSize					0x10000

#define	E_GrFs5HddModelNameMaxLen			128

#define	E_GrFs5HddIovMaxCnt						1024

#define	E_GrFs5HddFillZeroIovCnt			16

// smart
#define E_GrFs5HddSmartValIdxId				0
#define E_GrFs5HddSmartValIdxStat0		1
#define E_GrFs5HddSmartValIdxStat1		2
#define E_GrFs5HddSmartValIdxValue		3
#define E_GrFs5HddSmartValIdxVendor		4
#define E_GrFs5HddSmartValIdxRawVal		5		/* 6byte */

#define E_GrFs5HddSmartValItemSize		12

#define E_GrFs5HddSmartValCnt					30

#define E_GrFs5HddSmartIdSeekErrRate	7		//Seek Error Rate
#define E_GrFs5HddSmartIdPoh					9			/* power on time */
#define E_GrFs5HddSmartIdTemp					194
#define E_GrFs5HddSmartIdReallocate		196 //Reallocation Event Count
#define E_GrFs5HddSmartIdBadSect			198
#define E_GrFs5HddSmartIdReadErrRate	201	//Soft Read Error Rate or TA Counter Detected

#define E_GrFs5HddDbgMsgView

//====================================================================
//global macro

//====================================================================
//global type


typedef struct St_GrFs5SmartData 
{
	__u16		Revision;
	__u8		Values[E_GrFs5HddSmartValCnt * E_GrFs5HddSmartValItemSize];
	__u8		Offline_status;
	__u8		Vendor1;
	__u16		Offline_timeout;
	__u8		Vendor2;
	__u8		Offline_capability;
	__u16		Smart_capability;
	__u8		_rsv[16];
	__u8		VendorSpc[125];
	__u8		Checksum;
} *Ptr_GrFs5SmartData;

typedef	struct St_GrFs5HddIocSmart
{
	__u8	Cmds[4];
	St_GrFs5SmartData	Data;
}	*Ptr_GrFs5HddIocSmart;

//====================================================================
//class

class	Cls_GrFs5Hdd
{
	private:

		__u8	m_DevIdx;					// device index
		__u8	m_DskStat;				// disk status - not exist , unformatted , bad , normal only

		__u64	m_DskSize;				// disk size

		__u32	m_BlkCnt;					// block count
		__u32	m_BiBufSize;			// block info buffer size
		
		char	m_StrModel[E_GrFs5HddModelNameMaxLen];

#ifdef DVR_EMU_PHYDSK
		__u8	m_SctBuf[512];
#endif

		// runtime
		Ptr_GrFs5BlkInfoBase	m_BlkInfoBuf;	// block info buffer
		Ptr_GrFs5DayInfo		m_DayInfoTbl;

		// SMART
		St_GrFs5HddIocSmart	m_SmartIoc;		//smart IOC

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
    Cls_GrFs5Hdd( __u8 A_DevIdx );
    virtual		~Cls_GrFs5Hdd();

		// info
		__u8	DiskStatGet(void);
		Ptr_GrFs5BlkInfoBase	BlkInfoTableGet(__u32* A_PtrRtCnt,BOOL8 A_IsForce = FALSE);
		__u8	DevIdxGet(void);
		Ptr_GrFs5DayInfo	DayInfoTableGet(void);

		// control
		Def_GrErrCode	BlkInfoWriteByIdx(__u32 A_BlkIdx);
		BOOL8	BlkInfoWriteAll(void);

		Def_GrErrCode	BlkReadPage(__u32 A_BlkIdx, __u32 A_Ofs, void* A_PtrData, __u32 A_Size);
		Def_GrErrCode	BlkWritePage(__u32 A_BlkIdx, __u32 A_Ofs, void* A_PtrData, __u32 A_Size);

		void	StatusUpdate(void);
		__u8	DiskInfoGet(__u64* A_PtrRtTotal, __u64* A_PtrRtRec);

		void	ModelNameGet(WCHAR* A_StrRt);

		BOOL8	SmartRead(Ptr_GrFs5Smart A_PtrRtData);

		Def_GrErrCode	DayInfoWrite(void);

};
	
//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif


