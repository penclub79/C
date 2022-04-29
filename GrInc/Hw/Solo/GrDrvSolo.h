/*
	solo driver
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrDrvSolo
#define	_EPC_GrDrvSolo

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

#ifdef	LINUX_APP
#ifdef	LINUX_DRV
#include	<linux/ioctl.h>
#else
#include	<sys/ioctl.h>
#endif
#endif

#include	<GrBufTool.h>
#include	<Dvr/GrDvrBase.h>

//====================================================================
//constance

//local memory map
#define E_GrDrvSoloLmemDispPos				0x0000000
#define E_GrDrvSoloLmemDispSize				0x0480000
#define E_GrDrvSoloLmemDec2LivePos		0x0480000
#define E_GrDrvSoloLmemDec2LiveSize		0x0240000
#define E_GrDrvSoloLmemOsgPos					0x06C0000
#define E_GrDrvSoloLmemOsgSize				0x0130000
#define E_GrDrvSoloLmemMotionPos			0x07F0000
#define E_GrDrvSoloLmemMotionSize			0x0080000
#define E_GrDrvSoloLmemAdoPos					0x0870000
#define E_GrDrvSoloLmemAdoSize				0x0010000
#define E_GrDrvSoloLmemEncCapPos			0x0880000
#define E_GrDrvSoloLmemEncCapSize			0x1320000
#define E_GrDrvSoloLmemEncOsdPos			0x2800000
#define E_GrDrvSoloLmemEncOsdSize			0x0400000
#define E_GrDrvSoloLmemDecRefPos			0x2C00000
#define E_GrDrvSoloLmemDecRefSize			0x1400000
#define E_GrDrvSoloLmemEncRefPos			0x4000000
#define E_GrDrvSoloLmemEncRefSize			0x2800000
#define E_GrDrvSoloLmemJpegPos				0x6800000
#define E_GrDrvSoloLmemJpegSize				0x0800000
#define E_GrDrvSoloLmemEncStrmPos			0x7000000
#define E_GrDrvSoloLmemEncStrmSize		0x0800000
#define E_GrDrvSoloLmemDecStrmPos			0x7800000
#define E_GrDrvSoloLmemDecStrmSize		0x0800000

//ioctl struct

typedef	struct St_GrDrvSoloSysInfo
{
	__u32	ChipCnt;
	__u32	PhysMem;
	__u32	MemSize;
	__u32	PllMhz;			//pll Mhz value
}	*Ptr_GrDrvSoloSysInfo;

typedef	struct St_GrDrvSoloDevInfo
{
	__u32	DevIdx;
	__u32	VdoChCnt;		//hardware video channel count
	__u32	CodecPwr;			//codec encode speed
}	*Ptr_GrDrvSoloDevInfo;

typedef	struct St_GrDrvSoloMemPosItm 
{
	__u32	Pos;
	__u32	Size;
}	*Ptr_GrDrvSoloMemPosItm;

typedef	struct  St_GrDrvSoloRegCtrl
{
	__u32		DevIdx;		//chip index
	__u32		Adr;
	__u32		Val;
}	*Ptr_GrDrvSoloRegCtrl;

typedef	struct St_GrDrvSoloGpioOut
{
	__u32	DevIdx;
	__u32	Map;
}	*Ptr_GrDrvSoloGpioOut;

typedef	struct St_GrDrvSoloGpioWrite 
{
	__u32	DevIdx;
	__u32	Set;
	__u32	Clear;
}	*Ptr_GrDrvSoloGpioWrite;

typedef	struct St_GrDrvSoloTime
{
	__u32	DevIdx;
	__u32	Sec;
}	*Ptr_GrDrvSoloTime;

typedef	struct  St_GrDrvSoloMseCusImg
{
	__u32		DevIdx;
	__u32		CusIdx;			//cursor index
}	*Ptr_GrDrvSoloMseCusImg;

typedef	struct  St_GrDrvSoloMseCusPos
{
	__u32		DevIdx;
	__u16		X;
	__u16		Y;
}	*Ptr_GrDrvSoloMseCusPos;

typedef	struct  St_GrDrvSoloIsWmrk
{
	__u32		DevIdx;
	__u32		IsWmrk;			//water mark enable
}	*Ptr_GrDrvSoloIsWmrk;

typedef	struct  St_GrDrvSoloZoomPos
{
	__u32		DevIdx;
	__u16		ZoomX;		//zoom position
	__u16		ZoomY;
}	*Ptr_GrDrvSoloZoomPos;

//ioctl command
#define	E_GrDrvSoloIoctlMagic					'j'
#define	E_GrDrvSoloDnodeMinorStart		0

#ifdef LINUX_APP

#define	E_GrDrvSoloIoctlGetSysInfo		_IOR( E_GrDrvSoloIoctlMagic, 0 , struct St_GrDrvSoloSysInfo )
#define	E_GrDrvSoloIoctlWriteReg			_IOW( E_GrDrvSoloIoctlMagic, 1 , struct St_GrDrvSoloRegCtrl )
#define	E_GrDrvSoloIoctlReadReg				_IOWR( E_GrDrvSoloIoctlMagic, 2 , struct St_GrDrvSoloRegCtrl )
#define	E_GrDrvSoloIoctlGpioOutSet		_IOW( E_GrDrvSoloIoctlMagic, 3 , struct St_GrDrvSoloGpioOut )
#define	E_GrDrvSoloIoctlGpioOutClear	_IOW( E_GrDrvSoloIoctlMagic, 4 , struct St_GrDrvSoloGpioOut )
#define	E_GrDrvSoloIoctlGpioWrite			_IOW( E_GrDrvSoloIoctlMagic, 5 , struct St_GrDrvSoloGpioWrite )
#define	E_GrDrvSoloIoctlGpioRead			_IOWR( E_GrDrvSoloIoctlMagic, 6 , __u32 )
#define	E_GrDrvSoloIoctlDevInfo				_IOWR( E_GrDrvSoloIoctlMagic, 7 , struct St_GrDrvSoloDevInfo )
#define	E_GrDrvSoloIoctlTimeSet				_IOW( E_GrDrvSoloIoctlMagic, 8 , struct St_GrDrvSoloTime )
#define	E_GrDrvSoloIoctlDevCfg				_IOW( E_GrDrvSoloIoctlMagic, 9 , __u32 )
#define	E_GrDrvSoloIoctlAvCfg					_IOW( E_GrDrvSoloIoctlMagic, 10 , __u32 )
#define	E_GrDrvSoloIoctlRecCfg				_IOW( E_GrDrvSoloIoctlMagic, 11 , __u32 )
#define	E_GrDrvSoloIoctlRecPara				_IOW( E_GrDrvSoloIoctlMagic, 12 , __u32 )
#define	E_GrDrvSoloIoctlNetCfg				_IOW( E_GrDrvSoloIoctlMagic, 13 , __u32 )
#define	E_GrDrvSoloIoctlPumpIrq				_IOW( E_GrDrvSoloIoctlMagic, 14 , __u32 )
#define	E_GrDrvSoloIoctlMseCusImg			_IOW( E_GrDrvSoloIoctlMagic, 15 , struct St_GrDrvSoloMseCusImg )
#define	E_GrDrvSoloIoctlMseCusPos		 	_IOW( E_GrDrvSoloIoctlMagic, 16 , struct St_GrDrvSoloMseCusPos )
#define	E_GrDrvSoloIoctlQuadSet		 		_IOW( E_GrDrvSoloIoctlMagic, 17 , __u32 )
#define	E_GrDrvSoloIoctlMtnStatGet		_IOWR( E_GrDrvSoloIoctlMagic, 18 , __u32 )
#define	E_GrDrvSoloIoctlCapBtwRst			_IOW( E_GrDrvSoloIoctlMagic, 19 , __u32 )
#define	E_GrDrvSoloIoctlCapTimeUpdt		_IOW( E_GrDrvSoloIoctlMagic, 20 , __u32 )
#define	E_GrDrvSoloIoctlCapTimeSync		_IOW( E_GrDrvSoloIoctlMagic, 21 , __u32 )
#define	E_GrDrvSoloIoctlWmrkSet				_IOW( E_GrDrvSoloIoctlMagic, 22 , struct St_GrDrvSoloIsWmrk )
#define	E_GrDrvSoloIoctlZoomMove			_IOW( E_GrDrvSoloIoctlMagic, 23 , struct St_GrDrvSoloZoomPos )
#define	E_GrDrvSoloIoctlZoomOff				_IOW( E_GrDrvSoloIoctlMagic, 24 , __u32 )
#else

#define	E_GrDrvSoloIoctlGetSysInfo		_LEMU_IOR( E_GrDrvSoloIoctlMagic, 0 , struct St_GrDrvSoloSysInfo )
#define	E_GrDrvSoloIoctlWriteReg			_LEMU_IOW( E_GrDrvSoloIoctlMagic, 1 , struct St_GrDrvSoloRegCtrl )
#define	E_GrDrvSoloIoctlReadReg				_LEMU_IOWR( E_GrDrvSoloIoctlMagic, 2 , struct St_GrDrvSoloRegCtrl )
#define	E_GrDrvSoloIoctlGpioOutSet		_LEMU_IOW( E_GrDrvSoloIoctlMagic, 3 , struct St_GrDrvSoloGpioOut )
#define	E_GrDrvSoloIoctlGpioOutClear	_LEMU_IOW( E_GrDrvSoloIoctlMagic, 4 , struct St_GrDrvSoloGpioOut )
#define	E_GrDrvSoloIoctlGpioWrite			_LEMU_IOW( E_GrDrvSoloIoctlMagic, 5 , struct St_GrDrvSoloGpioWrite )
#define	E_GrDrvSoloIoctlGpioRead			_LEMU_IOWR( E_GrDrvSoloIoctlMagic, 6 , __u32 )
#define	E_GrDrvSoloIoctlDevInfo				_LEMU_IOWR( E_GrDrvSoloIoctlMagic, 7 , struct St_GrDrvSoloDevInfo )
#define	E_GrDrvSoloIoctlTimeSet				_LEMU_IOW( E_GrDrvSoloIoctlMagic, 8 , struct St_GrDrvSoloTime )
#define	E_GrDrvSoloIoctlDevCfg				_LEMU_IOW( E_GrDrvSoloIoctlMagic, 9 , __u32 )
#define	E_GrDrvSoloIoctlAvCfg					_LEMU_IOW( E_GrDrvSoloIoctlMagic, 10 , __u32 )
#define	E_GrDrvSoloIoctlRecCfg				_LEMU_IOW( E_GrDrvSoloIoctlMagic, 11 , __u32 )
#define	E_GrDrvSoloIoctlRecPara				_LEMU_IOW( E_GrDrvSoloIoctlMagic, 12 , __u32 )
#define	E_GrDrvSoloIoctlNetCfg				_LEMU_IOW( E_GrDrvSoloIoctlMagic, 13 , __u32 )
#define	E_GrDrvSoloIoctlPumpIrq				_LEMU_IOW( E_GrDrvSoloIoctlMagic, 14 , __u32 )
#define	E_GrDrvSoloIoctlMseCusImg			_LEMU_IOW( E_GrDrvSoloIoctlMagic, 15 , struct St_GrDrvSoloMseCusImg )
#define	E_GrDrvSoloIoctlMseCusPos		 	_LEMU_IOW( E_GrDrvSoloIoctlMagic, 16 , struct St_GrDrvSoloMseCusPos )
#define	E_GrDrvSoloIoctlQuadSet		 		_LEMU_IOW( E_GrDrvSoloIoctlMagic, 17 , __u32 )
#define	E_GrDrvSoloIoctlMtnStatGet		_LEMU_IOWR( E_GrDrvSoloIoctlMagic, 18 , __u32 )
#define	E_GrDrvSoloIoctlCapBtwRst			_LEMU_IOW( E_GrDrvSoloIoctlMagic, 19 , __u32 )
#define	E_GrDrvSoloIoctlCapTimeUpdt		_LEMU_IOW( E_GrDrvSoloIoctlMagic, 20 , __u32 )
#define	E_GrDrvSoloIoctlCapTimeSync		_LEMU_IOW( E_GrDrvSoloIoctlMagic, 21 , __u32 )
#define	E_GrDrvSoloIoctlWmrkSet				_LEMU_IOW( E_GrDrvSoloIoctlMagic, 22 , struct St_GrDrvSoloIsWmrk )
#define	E_GrDrvSoloIoctlZoomMove			_LEMU_IOW( E_GrDrvSoloIoctlMagic, 23 , struct St_GrDrvSoloZoomPos )
#define	E_GrDrvSoloIoctlZoomOff				_LEMU_IOW( E_GrDrvSoloIoctlMagic, 24 , __u32 )
#endif	//LINUX_APP

//#define	E_GrDrvSoloIoctlMaxNr					23
#define	E_GrDrvSoloIoctlMaxNr					25

//system rule
#define E_GrDrvSoloMaxDevCnt					1		/* max device count */

#define E_GrDrvSoloP2mQueSize					0x2000

#define E_GrDrvSoloP2mEventTblSize		32

#define E_GrDrvSolo6010EncBugSize			(E_GrDrvSoloLmemEncStrmSize + 0x10000)
#define E_GrDrvSolo6010JpgBugSize			(E_GrDrvSoloLmemJpegSize + 0x10000)

//p2m channel
#define E_GrDrvSoloP2mChCommon				0
#define E_GrDrvSoloP2mChOsg						0		/* osg p2m channel */
#define E_GrDrvSoloP2mChMtnCfg				0		/* motion configuration p2m channel */
#define E_GrDrvSoloP2mChMtnStat				0
#define E_GrDrvSoloP2mChRecVdo				1
#define E_GrDrvSoloP2mChRecAdo				1
#define E_GrDrvSoloP2mChPlayVdo				2
#define E_GrDrvSoloP2mChPlayAdo				3		/* app que in , driver p2m in , driver out */
#define E_GrDrvSoloP2mChCapture				2
#define E_GrDrvSoloP2mChNetVdo				3
#define E_GrDrvSoloP2mChNetAdo				3
#define E_GrDrvSoloP2mChJpeg					2


#define E_GrDrvSoloP2mStatNone				0
#define E_GrDrvSoloP2mStatRun					1

#define E_GrDrvSoloP2mParaCnt					5		/* p2m parameter count */
#define E_GrDrvSoloP2mParaModeNone		0
#define E_GrDrvSoloP2mParaModeEvent		1		/* update completion status */
#define E_GrDrvSoloP2mParaModeVencDiv	2		/* video encode divide */
#define E_GrDrvSoloP2mParaModeVencCpl	3		/* video encode complete */
#define E_GrDrvSoloP2mParaModeAencCpl	4		/* audio encode complete */
#define E_GrDrvSoloP2mParaModeJpegCpl	5		/* jpeg encode complete */
#define E_GrDrvSoloP2mParaModeVdec		6		/* video decode */
#define E_GrDrvSoloP2mParaModeNmpgCpl	7		/* network mpeg complete */
#define E_GrDrvSoloP2mParaModeNadoCpl	8		/* network audio complete */
#define E_GrDrvSoloP2mParaModeNjpgCpl	9		/* network jpeg complete */
#define E_GrDrvSoloP2mParaModeAdec		10	/* audio decode */

#define E_GrDrvSoloP2mVdoFcc	Mac_GrMakeFcc('P','2','M','V')		/* video p2m fcc */
#define E_GrDrvSoloP2mAdoFcc	Mac_GrMakeFcc('P','2','M','A')		/* audio p2m fcc */
#define E_GrDrvSoloP2mBadFcc	Mac_GrMakeFcc('P','2','M','B')		/* bad p2m fcc */

#define E_GrDrvSoloP2mAvHeadSize			8			/* p2m av header size */

//jpeg request mode
#define E_GrDrvSoloJpgReqNone					0		//jpeg capture completed or not work
#define E_GrDrvSoloJpgReqWait					1		//wait jpeg request accept
#define E_GrDrvSoloJpgReqRun					2		//jpeg transmitting

//audio
#define E_GrDrvSoloAdoClustElemCnt		(E_SoloAdoHwQueCnt >> 1)		/* audio cluster element count */		
#define	E_GrDrvSoloAdoClustSize				(E_SoloAdoChUnitSize * E_GrDrvSoloAdoClustElemCnt)
#define E_GrDrvSoloAdoDecCh						17	//decoding channel
#define E_GrDrvSoloAdoSmpRate					8000
#define E_GrDrvSoloAdoBitRate					25

#define E_GrDrvSoloAdoPlayQueSize			0x80

//i2c
#define E_GrDrvSoloI2cCmdMaxCnt				16		/* max command count */
#define E_GrDrvSoloI2cReadMaxSize			8			/* read max size */
#define E_GrDrvSoloI2cReqQueSize			0x2000		/* request queue size */
#define E_GrDrvSoloI2cTimeOutSec			1					/* time out sec */
#define E_GrDrvSoloI2cRtSlotCnt				32		/* return value slot count */
#define E_GrDrvSoloI2cRtSlotIdxNone		0xFF	/* bad slot index */

//uart
#define E_GrDrvSoloUartBufSize				128

//mpeg encode
#define E_GrDrvSoloVencStrmAlign			64		/* encode stream align */
#define E_GrDrvSoloVencAlignRval			3			/* stream align register value */
#define E_GrDrvSoloVencP2mAlignUnit		32						/* p2m transfer align unit */
#define E_GrDrvSoloVencP2mAlignMask		0xFFFFFFE0		/* p2m transfer align mask */
#define E_GrDrvSoloVencStrmTailSize		4			/* video stream tail size */
#define E_GrDrvSoloVencEvtBkIdx				15		/* event backup index */
#define E_GrDrvSoloVencStat0BkIdx			14		/* status 0 backup index */
#define E_GrDrvSoloVencStat1BkIdx			13		/* status 1 backup index */
#define E_GrDrvSoloVencTickIdx				1			/* video frame tick index */
//#define E_GrDrvSoloVencRefCntIdx			12		/* reference count index */
//#define E_GrDrvSoloVencSendCntIdx			11		/* sended count */
#define E_GrDrvSoloVencSeqIdIdx				8			/* sequence id */

//disp
#define E_GrDrvSoloMseCusCnt					5			/* mouse cosor count */

//network
#ifdef DVR_STRM
#define E_GrDrvSoloNetClientCnt				32					/* network client count - stream server type */
#else
#define E_GrDrvSoloNetClientCnt				8					/* network client count */
#endif
#define E_GrDrvSoloNetLmqSize					0x1000		/* live mpeg queue size */
#define E_GrDrvSoloNetLjqSize					1024			/* live jpeg queue size */
#define E_GrDrvSoloNetLaqSize					0x10000		/* live audio buffer size */
#define E_GrDrvSoloNetLaHeadSize			8					/* network header size */
#define E_GrDrvSoloNetLaItmSize				(E_GrDrvSoloNetLaHeadSize + E_GrDrvSoloAdoClustSize)	/* [0]=p2mfcc, [1]=channel */
#define E_GrDrvSoloNetLaP2mFccIdx			0
#define E_GrDrvSoloNetLaChIdx					1
#define E_GrDrvSoloNetP2mAlignUnit		16						/* p2m transfer align unit */
#define E_GrDrvSoloNetP2mAlignMask		0xFFFFFFF0		/* p2m transfer align mask */

/* index 8 = nosignal tail code located */
#define E_GrDrvSoloNetJpgRefCntIdx		9			/* reference count index */
#define E_GrDrvSoloNetJpgSendCntIdx		10		/* sended count */
#define E_GrDrvSoloNetJpgStrmHeadSize	608		/* net jpeg stream head size - 593 + 15 */
#define E_GrDrvSoloNetJpgHeadGap			640
#define E_GrDrvSoloNetJpgStrmSoiOfs		47
#define E_GrDrvSoloNetJpgStrmDqtOfs		49
#define E_GrDrvSoloNetJpgStrmDhtOfs		187
#define E_GrDrvSoloNetJpgStrmSofOfs		607
#define E_GrDrvSoloNetJpgStrmSosOfs		626

#define E_GrDrvSoloNetJpgStrmDqtSize	138
#define E_GrDrvSoloNetJpgStrmDhtSize	420
#define E_GrDrvSoloNetJpgStrmSofSize	19
#define E_GrDrvSoloNetJpgStrmSosSize	14

//====================================================================
//global macro


//====================================================================
//global type

typedef	struct St_GrDrvSoloAdoPlayItm 
{
	__u32		BufPos;		//play data buffer position
}	*Ptr_GrDrvSoloAdoPlayItm;

typedef struct St_GrDrvSoloP2mReq
{
	//p2m register
	__u32		RegTarAdr;
	__u32		RegExtAdr;
	__u32		RegExtCfg;
	__u32		RegCtrl;
	//maintanence
	__u32		Tick;								//time out tick
	__u32		PciErrId;						//error occure check
	__u8		Stat;								//p2m status
	__u8		ParaType;						//parameter type
	__u8		_rsvd[2];						//reserved
	__u32		Para[E_GrDrvSoloP2mParaCnt];						//parameter
}	*Ptr_GrDrvSoloP2mReq;

typedef struct St_GrDrvSoloI2cReq 
{
	__u8		Ch;					//i2c channel
	__u8		DevAdr;			//device address - error check
	__u16		SubAdr;			//sub address - error check
	BOOL8		IsRun;			//started
	__u8		Prgs;				//progress
	__u8		CmdCnt;			//command count
	__u8		ReadCnt;		//read count
	__u8		Slot;				//read slot
	__u8		_rsvd[3];		//reserved
	__u32		CmdTbl[E_GrDrvSoloI2cCmdMaxCnt];		//command table
	__u8		ValTbl[E_GrDrvSoloI2cCmdMaxCnt];		//val table
}	*Ptr_GrDrvSoloI2cReq;

typedef struct St_GrDrvSoloI2cRtSlot
{
	__u8		Ch;
	__u8		DevAdr;
	__u16		SubAdr;
	BOOL8		IsCpl;				//completed
	BOOL8		IsErr;
	__u8		ReadSize;
	__u8		_rsvd[1];		//reserved
	__u8		ReadBuf[E_GrDrvSoloI2cReadMaxSize];
}	*Ptr_GrDrvSoloI2cRtSlot;

typedef	struct St_GrDrvSoloBufCfg
{
	struct St_GrDrvSoloMemPosItm	MtnCfg;		//motion configuration
	struct St_GrDrvSoloMemPosItm	AdoDec;		//audio decode que
	struct St_GrDrvSoloMemPosItm	AdoSlc;		//audio silent data
	struct St_GrDrvSoloMemPosItm	EncOsd;		
	struct St_GrDrvSoloMemPosItm	NetAdo;		
	struct St_GrDrvSoloMemPosItm	JpegCap;	//jpeg capture
	struct St_GrDrvSoloMemPosItm	ImgCap;		//image capture buffer
	struct St_GrDrvSoloMemPosItm	Osg;			//osg buffer
	struct St_GrDrvSoloMemPosItm	NetJpg;		//network jpeg buffer
	struct St_GrDrvSoloMemPosItm	Play;			//mpeg play buffer
	struct St_GrDrvSoloMemPosItm	NetMpg;		//network mpeg buffer
	struct St_GrDrvSoloMemPosItm	Rec;			//record vque
}	*Ptr_GrDrvSoloBufCfg;

typedef struct St_GrDrvSoloGpioCfg 
{
	__u32	MapDir;					//direction 0 = input , 1 = output
	__u32	MapI2c;					//function i2c 
	__u32	MapUart;				//uart channel use
	__u32	MapIrqEnable;		//interrupt enable 0 = disable , 1 = enable
	__u32	MapIrqRise;			//rising edge interrupt  1 = rising edge
	__u32	MapIrqFall;			//falling edge interrupt 1 = falling edge
}	*Ptr_GrDrvSoloGpioCfg;

typedef struct St_GrDrvSoloVdoInCfg 
{
	__u32	OutClkSel;
	__u32	InClkDelay;
	__u32	Hstart;
	__u32	Vstart;
}	*Ptr_GrDrvSoloVdoInCfg;

typedef struct St_GrDrvSoloVdoOutCfg 
{
	__u32	Hstart;
	__u32	Vstart;
}	*Ptr_GrDrvSoloVdoOutCfg;

typedef struct St_GrDrvSoloAdoCfg 
{
	__u32	ChCnt;			//encode channel count
	BOOL8	IsRec;
	BOOL8	IsNet;
}	*Ptr_GrDrvSoloAdoCfg;

typedef	struct  St_GrDrvSoloAveCfg
{
	BOOL8		IsVdoPal;				// video signal pal
	//__u32		VdoChCnt;				// video channel count
	__u32		EncPwr;					// encode power

	struct St_GrDrvSoloVdoInCfg	VdoIn;		//video in
	struct St_GrDrvSoloVdoOutCfg	VdoOut;	//video out
	struct St_GrDrvSoloAdoCfg Ado;				//audio

	BOOL8		IsMtnMap;				// motion map
	BOOL8		IsDualStrm;			// dual stream record
	BOOL8		IsEncRec;				// record
	BOOL8		IsEncNet;				// network record

}	*Ptr_GrDrvSoloAveCfg;

typedef	struct St_GrDrvSoloVencChCfg
{
	__u32		Scl;			//resolution scale
	__u32		Qp;				//quantize - max
	__u32		Skip;			//skip count - frame rate
	__u32		Gop;
}	*Ptr_GrDrvSoloVencChCfg;

typedef struct St_GrDrvSoloVrecCfg
{
	struct St_GrDrvSoloVencChCfg	ChTbl[E_SoloMaxChCnt];
}	*Ptr_GrDrvSoloVrecCfg;

typedef	struct St_GrDrvSoloRecCfg 
{
	struct St_GrDrvSoloVrecCfg	Vdo;
	BOOL8	IsRec;
}	*Ptr_GrDrvSoloRecCfg;

typedef	struct St_GrDrvSoloQuadItm 
{
	__u32	WinCtrl0;
	__u32	WinCtrl1;
	__u32	LiveOn;
}	*Ptr_GrDrvSoloQuadItm;

typedef struct St_GrDrvSoloNetCfg
{
	BOOL8	IsExtCodec;	//use extra codec use
	__u8	JpgQp;			//jpeg qp
	__u8	_rsvd[2];		//reserved
	struct St_GrDrvSoloVencChCfg	ChTbl[E_SoloMaxChCnt];
}	*Ptr_GrDrvSoloNetCfg;

typedef	struct  St_GrDrvSoloNetLvqItm
{
	__u32	PosSys;			//driver position
}	*Ptr_GrDrvSoloNetLvqItm;

typedef	struct St_GrDrvSoloNetChItm 
{
	BOOL8	IsLvOn;			//live video channel on
	BOOL8	IsIrqPable;	//irq pframe able
	BOOL8	IsP2mPable;	//p2m pframe able
	BOOL8	IsIfrmOnly;	//i frame only
	BOOL8	IsAudio;		//audio on
	__u8	LvPfrmCnt;	//live pframe count
	__u8	LvSkipGop;	//love skip gop frame
	__u8	_rsvd[1];		//reserved
}	*Ptr_GrDrvSoloNetChItm;

typedef	struct St_GrDrvSoloNetCliMng 
{
	BOOL8		IsLiveOn;				//live on
	BOOL8		IsLvJpg;				//live jpeg
	BOOL8		IsLmFlush;			//live mpeg flush
	__u8		BwPfrmCap;			//bandwidth pframe counter
	__u8		BwGopSkip;			//bandwidth skip gop
	BOOL8		IsLvFull;				//live buffer full 
	__u8		_rsvd[2];
	__u32		LvmIn;						//live mpeg in size
	__u32		LvmOut;						//live mpeg out size
	__u32		LvmLim;						//live mpeg limite size
	__u32		LvjIn;						//live mpeg in size
	__u32		LvjOut;						//live mpeg out size
	__u32		LvjLim;						//live mpeg limite size
	struct St_GrDrvSoloNetChItm	ChTbl[E_SoloMaxChCnt];
	__u8	LvmqBuf[E_GrDrvSoloNetLmqSize];		//live mpeg queue
	__u8	LvjqBuf[E_GrDrvSoloNetLjqSize];		//live jpeg queue
}	*Ptr_GrDrvSoloNetCliMng;

typedef	struct St_GrDrvSoloDevCtrl		//device control
{

	//status
	__u32	IrqOcuCnt;		//interrupt occured count
	__u32	PciErrCnt;		//pci error count

	//interrupt
	__u32	JifIrq;				//interrupt jiffies
	__u32	IrqRunMap;		//interrupt run map - not use

	//p2m
	__u8	P2mQue[E_SoloP2mChCnt][E_GrDrvSoloP2mQueSize];		//p2m queue
	BOOL8	P2mEventTbl[E_GrDrvSoloP2mEventTblSize];					//p2m completion table

	//encode
	//__u32	EncFrmCnt;				//encode video audio frame count
	__u32	EncMpgWtc;				//encode mpeg watchdog

	//encode audio

	//record
	__u32	RecSchd[E_SoloMaxChCnt];		//schedule update
	__u32	MotionMap;				//motion map
	__u16	NosigMap;					//nosignal map
	__u16	_rsvd0;						//reserved

	//jpeg
	__u8	JpgReq;						//jpeg request mode
	__u8	JpgCh;						//jpeg channel
	__u8	JpgResX;					//jpeg resx
	__u8	JpgResY;					//jpeg resy
	__u32	JpgSize;					//jpeg size

	//decode
	__u32	DecVprsId;				// video decode processed id
	__u32	DecAprsId;				// audio decode processed id

	//decode video
	__u32	VdecLbPosIn;			//mpeg decode hw buffer in position (no lmptr)
	__u32	VdecLbPosOut;			//mpeg decode hw buffer out position (no lmptr)

	//decode audio
	__u8	AdecPlayQue[E_GrDrvSoloAdoPlayQueSize];

	//i2c
	__u8	I2cQueReq[E_GrDrvSoloI2cReqQueSize];
	struct St_GrDrvSoloI2cRtSlot	I2cRtSlot[E_GrDrvSoloI2cRtSlotCnt];

	//uart
	__u8	UartQueRecv[E_SoloUartChCnt][E_GrDrvSoloUartBufSize];
	__u8	UartQueSend[E_SoloUartChCnt][E_GrDrvSoloUartBufSize];
	__u32	UartReadOvrn[E_SoloUartChCnt];		//read overrun count

	//cfg
	struct St_GrDrvSoloBufCfg		CfgBuf;
	struct St_GrDrvSoloGpioCfg	CfgGpio;
	struct St_GrDrvSoloAveCfg		CfgAv;
	struct St_GrDrvSoloRecCfg		CfgRec;
	struct St_GrDrvSoloNetCfg		CfgNet;	

	//quad
	struct	St_GrDrvSoloQuadItm	QuadRegTbl[E_GrDvrMaxChCnt];
	__u32	QuadLineMap;
	__u32	QuadBarMap;
	__u16	QuadBarPosTbl[2][E_SoloDispBarPosCnt];	//bar pos x, y table
	BOOL8	IsQuadPlayOne;			//one channel quad mode
	__u8	QuadFullCh;					//quad full channel number
	__u8	QuadMode;						//quad mode
	BOOL8	IsQuadExp;

	//network
	struct	St_GrDrvSoloNetCliMng	NetMng[E_GrDrvSoloNetClientCnt];		//network client table

}	*Ptr_GrDrvSoloDevCtrl;

typedef	struct St_GrDrvSoloMng			//driver manager
{
	__u32	JifSec;					//jiffies 1 sec base
	struct  St_GrDrvSoloDevCtrl	DevTbl[E_GrDrvSoloMaxDevCnt];
	//schedule
	__u8		NetVdoChUse[E_SoloMaxChCnt];	//network client video channel usage - bitmap				
}	*Ptr_GrDrvSoloMng;

//====================================================================
//class


//====================================================================
//global function
	
//====================================================================
//out Enviroment conditional
#endif
