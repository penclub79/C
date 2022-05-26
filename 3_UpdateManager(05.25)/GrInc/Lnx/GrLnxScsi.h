/*
linux scsi

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrLnxScsi
#define	_EPC_GrLnxScsi

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

#ifdef LINUX_APP
	#include	<sys/ioctl.h>
	#include	<linux/cdrom.h>
	#include	<mntent.h>
	#include	<sys/wait.h>
	#include	<sys/utsname.h>
	#include	<scsi/sg.h>
	#include	<sys/stat.h>
	#include	<unistd.h>
#else
	#include	<LnxW32Inc/mntent.h>
	#include	<LnxW32Inc/errno.h>
	#include	<LnxW32Inc/linux/cdrom.h>
	#include	<LnxW32Inc/sys/ioctl.h>
	#include	<LnxW32Inc/scsi/sg.h>
	#include	<LnxW32Inc/sys/stat.h>
	#include	<LnxW32Inc/unistd.h>
#endif


//====================================================================
//constance

#define E_GrLnxScsiStatMax		18
#define E_GrLnxScsiCmdMax			16

#define E_GrLnxScsiDirNone		SG_DXFER_NONE
#define E_GrLnxScsiDirRead		SG_DXFER_FROM_DEV
#define E_GrLnxScsiDirWrite		SG_DXFER_TO_DEV

//profile
#define E_GrLnxScsiPfNone									0x00			/* no media */
#define E_GrLnxScsiPfNonRemoveableDisk		0x01
#define E_GrLnxScsiPfRemoveableDisk				0x02
#define E_GrLnxScsiPfMoErasable						0x03
#define E_GrLnxScsiPfOpticalWriteOnce			0x04
#define E_GrLnxScsiPfAsMo									0x05	
#define E_GrLnxScsiPfCdRom								0x08
#define E_GrLnxScsiPfCdR									0x09
#define E_GrLnxScsiPfCdRw									0x0A
#define E_GrLnxScsiPfDvdRom								0x10
#define E_GrLnxScsiPfDvdMnRSeq						0x11			/* dvd - r sequence */
#define E_GrLnxScsiPfDvdMnRam							0x12
#define E_GrLnxScsiPfDvdMnRwRstOverwt			0x13			/* dvd - rw restrict overwrite - formatted */
#define E_GrLnxScsiPfDvdMnRwSeq						0x14			/* dvd - rw sequence recording or not formatted */
#define E_GrLnxScsiPfDvdMnRDlSeq					0x15			/* dvd - r dual layer sequence recording */
#define E_GrLnxScsiPfDvdMnRDlJmp					0x16			/* dvd - r dual layer jump recording */
#define E_GrLnxScsiPfDvdPlRw							0x1A			/* dvd + rw */
#define E_GrLnxScsiPfDvdPlR								0x1B			/* dvd + r */
#define E_GrLnxScsiPfDvdPlRwDl						0x2A			/* dvd + rw dual layer */
#define E_GrLnxScsiPfDvdPlRDl							0x2B			/* dvd + r dual layer */
#define E_GrLnxScsiPfBdRom								0x40			/* blu-lay */
#define E_GrLnxScsiPfBdRSrm								0x41			/* blu-lay rom sequence record */
#define E_GrLnxScsiPfBdRRrm								0x42			/* blu-lay rom random record */
#define E_GrLnxScsiPfBdRe									0x43			/* blu-lay rewriteable */
#define E_GrLnxScsiPfHdRom								0x50			/* HD DVD rom */
#define E_GrLnxScsiPfHdR									0x51			/* HD DVD write once */
#define E_GrLnxScsiPfHdRam								0x52			/* HD DVD rewriteable */

//peripheral device type
#define E_GrLnxScsiPpDevDirBlkAcc					0
#define E_GrLnxScsiPpDevSeqBlkAcc					1
#define E_GrLnxScsiPpDevPrinter						2
#define E_GrLnxScsiPpDevProcessor					3
#define E_GrLnxScsiPpDevWriteOnce					4
#define E_GrLnxScsiPpDevCdOrDvd						5
#define E_GrLnxScsiPpDevScanner						6
#define E_GrLnxScsiPpDevOpticalMemory			7
#define E_GrLnxScsiPpDevMediaChanger			8
#define E_GrLnxScsiPpDevCommunication			9
#define E_GrLnxScsiPpDevStorageArray			0x0C
#define E_GrLnxScsiPpDevEnclosureSvc			0x0D
#define E_GrLnxScsiPpDevSimpleDirAcc			0x0E
#define E_GrLnxScsiPpDevOpticalCard				0x0F
#define E_GrLnxScsiPpDevBridge						0x10
#define E_GrLnxScsiPpDevObjBaseStorage		0x11


//get event status
#define E_GrLnxScsiEvtMaskOpChg						(1 << 1)
#define E_GrLnxScsiEvtMaskPwrMng					(1 << 2)
#define E_GrLnxScsiEvtMaskExtReq					(1 << 3)
#define E_GrLnxScsiEvtMaskMedia						(1 << 4)
#define E_GrLnxScsiEvtMaskMultHost				(1 << 5)
#define E_GrLnxScsiEvtMaskDevBusy					(1 << 6)

#define E_GrLnxScsiEvtMaskAll							0xFF

#define E_GrLnxScsiEvtClsNotSupport				0
#define E_GrLnxScsiEvtClsOpChgReq					1
#define E_GrLnxScsiEvtClsPwrMng						2
#define E_GrLnxScsiEvtClsExtReq						3
#define E_GrLnxScsiEvtClsMedia						4
#define E_GrLnxScsiEvtClsMultHost					5
#define E_GrLnxScsiEvtClsDevBusy					6

//get configuration
#define E_GrLnxScsiGetConfNormal					0
#define E_GrLnxScsiGetConfSpecific				1
#define E_GrLnxScsiGetConfByIdx						2

//start stop unit
#define E_GrLnxScsiSsuCmdStop							0
#define E_GrLnxScsiSsuCmdStart						1
#define E_GrLnxScsiSsuCmdEject						2
#define E_GrLnxScsiSsuCmdLoad							3

#define E_GrLnxScsiSsuPwNoChg							0
#define E_GrLnxScsiSsuPwIdle							2
#define E_GrLnxScsiSsuPwStandby						3
#define E_GrLnxScsiSsuPwSleep							5

//read track information
#define E_GrLnxScsiRtiAdrLba							0
#define E_GrLnxScsiRtiAdrTrack						1
#define E_GrLnxScsiRtiAdrSession					2

//mode sense
#define E_GrLnxScsiMsPcCurrentVal					0
#define E_GrLnxScsiMsPcChangeAble					1
#define E_GrLnxScsiMsPcDefaultVal					2
#define E_GrLnxScsiMsPcSavedVal						3

//read disck information
#define E_GrLnxScsiRdiReadStdInfo					0
#define E_GrLnxScsiRdiReadTrackInfo				1
#define E_GrLnxScsiRdiReadPowInfo					2

#define E_GrLnxScsiRdiDiscStatEmpty				0
#define E_GrLnxScsiRdiDiscOpenSession			1
#define E_GrLnxScsiRdiDiscCloseSession		2

//api return code
#define E_GrLnxScsiApiOk									0
#define E_GrLnxScsiApiNotSupport					1
#define E_GrLnxScsiApiNoMedia							2
#define E_GrLnxScsiApiOpInPrograss				3

#define E_GrLnxScsiApiError								-1

//define
#ifndef FATAL_START
#define	FATAL_START(er)	(0x80|(er))
#endif

#define	SK(errcode)	(((errcode)>>16)&0xF)
#define	ASC(errcode)	(((errcode)>>8)&0xFF)
#define ASCQ(errcode)	((errcode)&0xFF)

//====================================================================
//global macro

//====================================================================
//global type

#pragma pack(push)
#pragma pack(1)

typedef	struct	St_GrLnxScsiEvtHd		
{
	__u8		EvtDescLenMsb;
	__u8		EvtDescLenLsb;
	__u8		Atb;
	__u8		EvtMask;				// evented mask
}	*Ptr_GrLnxScsiEvtHd;

typedef	struct St_GrLnxScsiConfHd 
{
	__u8		Len3;
	__u8		Len2;
	__u8		Len1;
	__u8		Len0;
	__u8		_rsv0;
	__u8		_rsv1;
	__u8		ProfileMsb;
	__u8		ProfileLsb;
}	*Ptr_GrLnxScsiConfHd;

typedef	struct St_GrLnxScsiTrackInfo 
{
	__u8		LenMsb;
	__u8		LenLsb;
	__u8		TrackNumB0;
	__u8		SessionNumB0;

	__u8		_rsv0;
	__u8		AtbTrackMode;
	__u8		AtbDataMode;
	__u8		AtbNextAdr;

	__u8		TrackStAdrB3;
	__u8		TrackStAdrB2;
	__u8		TrackStAdrB1;
	__u8		TrackStAdrB0;

	__u8		NextWriteAdrB3;
	__u8		NextWriteAdrB2;
	__u8		NextWriteAdrB1;
	__u8		NextWriteAdrB0;

	__u8		FreeBlocksB3;
	__u8		FreeBlocksB2;
	__u8		FreeBlocksB1;
	__u8		FreeBlocksB0;

	__u8		FixPkSizeBlkFcB3;	
	__u8		FixPkSizeBlkFcB2;	
	__u8		FixPkSizeBlkFcB1;	
	__u8		FixPkSizeBlkFcB0;	

	__u8		TrackSizeB3;
	__u8		TrackSizeB2;
	__u8		TrackSizeB1;
	__u8		TrackSizeB0;

	__u8		LastRecAdrB3;
	__u8		LastRecAdrB2;
	__u8		LastRecAdrB1;
	__u8		LastRecAdrB0;

	__u8		TrackNumB1;
	__u8		SessionNumB1;
	__u8		_rsv1;
	__u8		_rsv2;

	__u8		ReadCptbLbaB3;
	__u8		ReadCptbLbaB2;
	__u8		ReadCptbLbaB1;
	__u8		ReadCptbLbaB0;

	__u8		NextLayerJmpAdrB3;
	__u8		NextLayerJmpAdrB2;
	__u8		NextLayerJmpAdrB1;
	__u8		NextLayerJmpAdrB0;

	__u8		LastLayerJmpAdrB3;
	__u8		LastLayerJmpAdrB2;
	__u8		LastLayerJmpAdrB1;
	__u8		LastLayerJmpAdrB0;

}	*Ptr_GrLnxScsiTrackInfo;

typedef	struct St_GrLnxScsiInquiry
{
	__u8		DevType;
	__u8		Atb1;			//bit 7 = removeable check
	__u8		Ver;			//version
	__u8		Atb2;

	__u8		AddLen;		//total size - 4
	__u8		Atb3;
	__u8		Atb4;
	__u8		Atb5;			

	__s8		VendorId[8];	//[8]
	__s8		PrdId[16];
	__s8		PrdRev[4];
	__s8		VendorSpc[20];		//

	__u8		Atb6;					//[56]
	__u8		_rsv1;

	__u8		Vd1Msb;				//[58]
	__u8		Vd1Lsb;
	__u8		Vd2Msb;
	__u8		Vd2Lsb;
	__u8		Vd3Msb;
	__u8		Vd3Lsb;
	__u8		Vd4Msb;
	__u8		Vd4Lsb;
	__u8		Vd5Msb;
	__u8		Vd5Lsb;
	__u8		Vd6Msb;
	__u8		Vd6Lsb;
	__u8		Vd7Msb;
	__u8		Vd7Lsb;
	__u8		Vd8Msb;
	__u8		Vd8Lsb;

	__u8		_rsv2[22];		//[74]

	__u8		VendorSpcAdd[32];	//[96]

}	*Ptr_GrLnxScsiInquiry;

typedef struct St_GrLnxScsiReadDiscInfoStd 
{
	__u8		LenMsb;
	__u8		LenLsb;
	__u8		Atb1;
	__u8		NumOfFirstTrack;
	__u8		NumOfSessionLsb;
	__u8		FirstTrackInLastSessionLsb;
	__u8		LastTrackInLastSessionLsb;
	__u8		Atb2;
	__u8		DiscType;
	__u8		NumOfSessionMsb;
	__u8		FirstTrackInLastSessionMsb;
	__u8		LastTrackInLastSessionMsb;
	__u8		DiskIdB3;
	__u8		DiskIdB2;
	__u8		DiskIdB1;
	__u8		DiskIdB0;
	__u8		LastSessionLeadInAdrB3;
	__u8		LastSessionLeadInAdrB2;
	__u8		LastSessionLeadInAdrB1;
	__u8		LastSessionLeadInAdrB0;
	__u8		LastSessionLeadOutAdrB3;
	__u8		LastSessionLeadOutAdrB2;
	__u8		LastSessionLeadOutAdrB1;
	__u8		LastSessionLeadOutAdrB0;
	__u8		DiscBarCode[8];
	__u8		DiscAppCode;
	__u8		NumOpeTabl;
}	*Ptr_GrLnxScsiReadDiscInfoStd;

#pragma pack(pop)

//====================================================================
//class
class Cls_GrLnxScsi
{
	private:
		Def_StrTag		m_StrDev;
		__s32			m_HndlFl;		//file handle
		BOOL8		m_IsUsable;

		__u8		m_SensBuf[E_GrLnxScsiStatMax];
		__u8		m_CmdBuf[E_GrLnxScsiCmdMax];
		struct sg_io_hdr	m_SgIo;

	protected:
		//control
		void	LcCmdInit( void );
		__s32		LcSendCmd( __u32 A_Dir = E_GrLnxScsiDirNone,void* A_PtrBuf=NULL,__u32 A_BufSize = 0 );
		__s32		LcMakeRetErrCode( void );
		void	LcBuildErrno( void );
	public:
		Cls_GrLnxScsi( __s8* A_StrDev );
		virtual	~Cls_GrLnxScsi();

		//info
		BOOL8	IsUsable( void );
		BOOL8	IsOddMediaChanged( void );
		BOOL8	IsOddMediaExist( void );
		BOOL8	IsOddTrayOpen( void );

		//api
		BOOL8	GetEventStatus( __u8 A_EvtMask, void* A_PtrRtBuf, __u16 A_BufSize, BOOL8 A_IsPoll = TRUE );
		BOOL8	GetConfiguration( void* A_PtrRtBuf, __u16 A_BufSize, __u8 A_RtType = E_GrLnxScsiGetConfNormal, __u16 A_StIdx = 0 );
		__s32		StartStopUnit( __u8 A_SsuCmd, BOOL8 A_IsImmed = FALSE, __u8 A_Power = E_GrLnxScsiSsuPwNoChg );
		__s32		TestUnitReady( void );
		BOOL8	ReadTrackInformation( void* A_PtrRtBuf, __u16 A_BufSize, 
					__u8 A_AdrType, __u32 A_Adr, BOOL8 A_IsAdrOpen = FALSE );
		BOOL8	ModeSense(	void* A_PtrRtBuf, __u16 A_BufSize, __u8 A_PageCode, __u8 A_SubPage = 0, BOOL8 A_IsDbd = TRUE, 
											__u8 A_PageCtrl = E_GrLnxScsiMsPcCurrentVal, BOOL8 A_IsLba48 = FALSE );
		BOOL8	ModeSelect( void* A_PtrBuf, __u16 A_BufSize, BOOL8 A_IsCompatible = TRUE, BOOL8 A_IsSavePage = FALSE );
		BOOL8	Inquiry( void* A_PtrRtBuf, __u16 A_BufSize, BOOL8 A_IsVpd = FALSE,__u8 A_PageCode = 0 );
		BOOL8	ReadDiscInformation( void* A_PtrRtBuf, __u16 A_BufSize, __u8 A_ReadType = E_GrLnxScsiRdiReadStdInfo );

};


//====================================================================
//global function

//====================================================================
//out Enviroment conditional
#endif

