/*
	DVR basic 
*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrDvrBase
#define	_EPC_GrDvrBase

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<GrTimeTool.h>

//====================================================================
// constance

#define E_GrDvrMaxChCnt						32					/* this solution support able max channel count */

#define	E_GrDvrVdoFmtResInterlace	0x80000000		/* interlace bit */
#define	E_GrDvrVdoFmtResEvenBit		0x40000000		/* even field bit */
#define	E_GrDvrVdoFmtResxPos			0
#define	E_GrDvrVdoFmtResxMask			0xFFFF
#define	E_GrDvrVdoFmtResyPos			16
#define	E_GrDvrVdoFmtResyMask			0x7FFF
#define	E_GrDvrVdoFmtResValMask		0xBFFFFFFF

#define	E_GrDvrVdoFmtResNone			0

#define	E_GrDvrVdoFmtRes352x240		(352 | (240 << E_GrDvrVdoFmtResyPos))
#define	E_GrDvrVdoFmtRes352x288		(352 | (288 << E_GrDvrVdoFmtResyPos))
#define	E_GrDvrVdoFmtRes704x240		(704 | (240 << E_GrDvrVdoFmtResyPos))
#define	E_GrDvrVdoFmtRes704x288		(704 | (288 << E_GrDvrVdoFmtResyPos))
#define	E_GrDvrVdoFmtRes704x480		(704 | (480 << E_GrDvrVdoFmtResyPos))
#define	E_GrDvrVdoFmtRes704x576		(704 | (576 << E_GrDvrVdoFmtResyPos))
#define	E_GrDvrVdoFmtRes480x240		(480 | (240 << E_GrDvrVdoFmtResyPos))
#define	E_GrDvrVdoFmtRes480x288		(480 | (288 << E_GrDvrVdoFmtResyPos))
#define	E_GrDvrVdoFmtRes944x240		(944 | (240 << E_GrDvrVdoFmtResyPos))
#define	E_GrDvrVdoFmtRes944x288		(944 | (288 << E_GrDvrVdoFmtResyPos))
#define	E_GrDvrVdoFmtRes720x480		(720 | (480 << E_GrDvrVdoFmtResyPos))
#define	E_GrDvrVdoFmtRes720x576		(720 | (576 << E_GrDvrVdoFmtResyPos))
#define	E_GrDvrVdoFmtRes944x480		(944 | (480 << E_GrDvrVdoFmtResyPos))
#define	E_GrDvrVdoFmtRes944x576		(944 | (576 << E_GrDvrVdoFmtResyPos))
#define	E_GrDvrVdoFmtRes960x480		(960 | (480 << E_GrDvrVdoFmtResyPos))
#define	E_GrDvrVdoFmtRes960x576		(960 | (576 << E_GrDvrVdoFmtResyPos))
#define	E_GrDvrVdoFmtRes800x600		(800 | (600 << E_GrDvrVdoFmtResyPos))
#define	E_GrDvrVdoFmtRes1024x768	(1024 | (768 << E_GrDvrVdoFmtResyPos))
#define	E_GrDvrVdoFmtRes640x360		(640 | (360 << E_GrDvrVdoFmtResyPos))
#define	E_GrDvrVdoFmtRes1280x360	(1280 | (360 << E_GrDvrVdoFmtResyPos))
#define	E_GrDvrVdoFmtRes1280x720	(1280 | (720 << E_GrDvrVdoFmtResyPos))
#define	E_GrDvrVdoFmtRes1280x1024	(1280 | (1024 << E_GrDvrVdoFmtResyPos))
#define	E_GrDvrVdoFmtRes1366x768	(1366 | (768 << E_GrDvrVdoFmtResyPos))
#define	E_GrDvrVdoFmtRes960x540		(960 | (540 << E_GrDvrVdoFmtResyPos))
#define	E_GrDvrVdoFmtRes1920x540	(1920 | (540 << E_GrDvrVdoFmtResyPos))
#define	E_GrDvrVdoFmtRes1920x1080	(1920 | (1080 << E_GrDvrVdoFmtResyPos))
#define	E_GrDvrVdoFmtRes2560x720	(2560 | (720 << E_GrDvrVdoFmtResyPos))
#define	E_GrDvrVdoFmtRes2560x1440	(2560 | (1440 << E_GrDvrVdoFmtResyPos))
#define	E_GrDvrVdoFmtRes2560x1600	(2560 | (1600 << E_GrDvrVdoFmtResyPos))
#define	E_GrDvrVdoFmtRes3840x2160	(3840 | (2160 << E_GrDvrVdoFmtResyPos))

#define	E_GrDvrFpsNtsc						30					/*	NTSC frame rate	*/
#define	E_GrDvrFpsPal							25					/*	PAL frame rate	*/

//record reference type
#define E_GrDvrRefTypeNone				0
#define E_GrDvrRefTypeX2					1
#define E_GrDvrRefTypeX4					2

//record attribute index
#define	E_GrDvrRecAidxRecord			0		/*	record mode	*/
#define	E_GrDvrRecAidxMotion			1		/*	motion detect */
#define	E_GrDvrRecAidxSensor			2		/*	sensor detect */
#define	E_GrDvrRecAidxSpc					3		/*	special sensor detect */

#define E_GrDvrRecAidxLogMtn			5		/*	log motion - internal only	*/
#define E_GrDvrRecAidxLogSen			6		/*	log motion - internal only	*/
#define E_GrDvrRecAidxLogSpc			7		/*	log special - internal only	*/

// record base type
#define	E_GrDvrRecAtbNonRec				0x00		/*	no record mode	*/
#define	E_GrDvrRecAtbRecord				(1 << E_GrDvrRecAidxRecord)			/*	record mode	*/
#define	E_GrDvrRecAtbMotion				(1 << E_GrDvrRecAidxMotion)			/*	motion detect */
#define	E_GrDvrRecAtbSensor				(1 << E_GrDvrRecAidxSensor)			/*	sensor detect */
#define	E_GrDvrRecAtbSpc					(1 << E_GrDvrRecAidxSpc)				/*	special sensor detect */
#define	E_GrDvrRecAtbLogMtn				(1 << E_GrDvrRecAidxLogMtn)			/*	log motion */
#define E_GrDvrRecAtbLogSen				(1 << E_GrDvrRecAidxLogSen)			/*	log sensor	*/
#define E_GrDvrRecAtbLogSpc				(1 << E_GrDvrRecAidxLogSpc)			/*	log special sensor	*/

#define E_GrDvrRecAtbSpcMask			0x38
#define E_GrDvrRecAtbNoramlMask		(E_GrDvrRecAtbRecord | E_GrDvrRecAtbMotion | E_GrDvrRecAtbSensor | E_GrDvrRecAtbSpc)
#define E_GrDvrRecAtbEvtMask			(E_GrDvrRecAtbMotion | E_GrDvrRecAtbSensor | E_GrDvrRecAtbSpc )
#define	E_GrDvrRecAtbLogMask			(E_GrDvrRecAtbLogMtn | E_GrDvrRecAtbLogSen | E_GrDvrRecAtbLogSpc)

// record disk type
#define	E_GrDvrRecDevNoraml				0				/* record device normal */
#define	E_GrDvrRecDevMirror				1				/* record device mirror */

#define E_GrDvrRecHndlNone				0LL			/* none handle */

#define	E_GrDvrRecMaxHddCnt				8				/* max hdd count */

// frame 
#define E_GrDvrFrmHeadFcc					Mac_GrMakeFcc('F', 'R', 'M', 'S')
#define E_GrDvrFrmTailFcc					Mac_GrMakeFcc('F', 'R', 'M', 'E')

// frame - type
#define E_GrDvrFrmTypeNone				0				/* none frame type */

#define E_GrDvrFrmTypeVdoI				1
#define E_GrDvrFrmTypeVdoP				2
#define E_GrDvrFrmTypeVdoB				3				/* MPEG b frame */
#define E_GrDvrFrmTypeVdoP2x			4				/* reference 2x */
#define E_GrDvrFrmTypeVdoP4x			5				/* reference 4x */

#define	E_GrDvrFrmTypeJpeg				8				/* JPEG	*/

#define E_GrDvrFrmTypeAdo					10

#define E_GrDvrFrmTypeLogPool			0xFE		/* log pool */

#define E_GrDvrFrmTypeNotUse			0xFF		/* not use area */

#define E_GrDvrFrmAlignSize				32
#define E_GrDvrFrmAlignMask				0xFFFFFFE0

// event log
#define E_GrDvrLogSize						32
#define E_GrDvrLogShiftSize				5
#define E_GrDvrLogAlignMask				0xFFFFFFE0

#define E_GrDvrLogChAny						0xFF

// event log type
#define E_GrDvrLogTypeNone				0

#define E_GrDvrLogTypeMotion			1			/* motion record event */
#define E_GrDvrLogTypeSensor			2			/* sensor record event */
#define E_GrDvrLogTypeVdoLoss			3			/* video loss */
#define E_GrDvrLogTypeVdoRcvy			4			/* video recovery */
#define E_GrDvrLogTypePowerOn			5			/* power on */
#define E_GrDvrLogTypePowerOff		6			/* power off */
#define E_GrDvrLogTypeLogin				7			/* log-in */
#define E_GrDvrLogTypeLogout			8			/* log-out */
#define E_GrDvrLogTypeSetupIn			9			/* setup menu in */
#define E_GrDvrLogTypeSetupOut		10		/* setup menu exit */
#define E_GrDvrLogTypeBkupSt			11		/* backup start */
#define E_GrDvrLogTypeBkupEd			12		/* backup end */
#define E_GrDvrLogTypeHddErr			13		/* hdd record error */
#define E_GrDvrLogTypeHddFull			14		/* hdd full */
#define E_GrDvrLogTypeHddFormat		15		/* hdd format */
#define E_GrDvrLogTypeFsFix				16
#define E_GrDvrLogTypeFwUpgd			17		/* firmware upgrade */
#define E_GrDvrLogTypeRtcErr			18		/* rtc error */
#define E_GrDvrLogTypeNetLogin		19		/* network client connect */
#define E_GrDvrLogTypeNetDiscon		20		/* network disconnect */
#define E_GrDvrLogTypeNetSetup		21		/* network setup change */
#define E_GrDvrLogTypeEmgcSt			22		/* emergency start */
#define E_GrDvrLogTypeEmgcEd			23		/* emergency start */
#define E_GrDvrLogTypeCfgRecPara	24		/* setup change record */
#define E_GrDvrLogTypeCfgMotion		25		/* setup change Motion */
#define E_GrDvrLogTypeCfgIdPass		26		/* setup change account */
#define E_GrDvrLogTypeCfgRecSchd	27		/* setup change record schedule */
#define E_GrDvrLogTypeCfgHdd			28		/* setup change HDD  */
#define E_GrDvrLogTypeSetupChg		29		/* setup change */
#define E_GrDvrLogTypeReBoot			30		/* rebooting */
#define E_GrDvrLogTypeTimeSet			31		/* Time Set */
#define E_GrDvrLogTypeCfgInit			32		/* setup initialize */

#define E_GrDvrLogTypeCfgPtz			33		/* setup change ptz */
#define E_GrDvrLogTypeCfgNet			34		/* setup change network */
#define E_GrDvrLogTypeCfgCamSeq		35		/* setup change sequence camera */
#define E_GrDvrLogTypeCfgSpot			36		/* setup change spot */
#define E_GrDvrLogTypeCfgLoad			37		/* setup load from device */
#define E_GrDvrLogTypeCfgSave			38		/* setup save to device */
#define E_GrDvrLogTypeCfgNetFrm		39		/* setup change network frame */
#define E_GrDvrLogTypeCfgDispRes	40		/* setup change display resolution */

#define E_GrDvrLogTypeMailErr			41		/* mail error */
#define E_GrDvrLogTypeWdogVdo			42		/* watch dog video */
#define E_GrDvrLogTypeWdogAdo			43		/* watch dog audio */

#define E_GrDvrLogTypeSpcSen			44		/* special sensor (ex: linc cross) */
#define E_GrDvrLogTypeSoftErr			45		/* software error detection */
#define	E_GrDvrLogTypeFsUpgrade		46		/* file system upgrade */

#define	E_GrDvrLogTypeHddTemp			47		/* hdd temperature */

#define	E_GrDvrLogTypeCnt					E_GrDvrLogTypeHddTemp	/* log type count */

// record status
#define E_GrDvrRecStatNotexist		0			// disk not exist
#define E_GrDvrRecStatBad					1			// bad disk
#define E_GrDvrRecStatUnformated	2			// unformatted
#define E_GrDvrRecStatEmpty				3			// empty status
#define E_GrDvrRecStatNormal			4			// normal status
#define E_GrDvrRecStatFull				5			// full status
#define E_GrDvrRecStatOverwrite		6			// overwrite status

// play status
#define E_GrDvrPlayStatStop				0			// play status stop
#define E_GrDvrPlayStatPlay				1			// play status play
#define E_GrDvrPlayStatSeek				2			// play status seek
#define E_GrDvrPlayStatBegin			3			// play status arrival begin
#define E_GrDvrPlayStatEnd				4			// play status arrival end

//====================================================================
//global macro

#define Mac_GrDvrMakeVdoFmt(ch0, ch1)	\
	((__u32)(ch0) | ((__u32)(ch1) << E_GrDvrVdoFmtResyPos))

//====================================================================
//global type

//frame header

typedef struct St_GrDvrFrmHdBase	/* 20 byte */
{
	__u32	Fcc;				// FCC code

	__u8	Type;				// frame type
	__u8	Ch;
	__u8	IsSm;				// summer time
	__u8	Ratb;				// record event or attribute
	
	__u32	TimeSec;		// time sec - linux base
	__u32	TimeMsec;		// time milliseconds - micro sec

	__s32	DataSize;

}	*Ptr_GrDvrFrmHdBase;

typedef struct St_GrDvrFrmTail
{
	__u32	Fcc;
	__u32	PktSize;	// frame total size
}	*Ptr_GrDvrFrmTail;

typedef struct St_GrDvrFrmHdUnknown
{
	St_GrDvrFrmHdBase	Base;
	__u32	_rsvd0;			// 
	__u32	_rsvd1;			// 
	__u32	_rsvd2;			// 

}	*Ptr_GrDvrFrmHdUnknown;

typedef	struct St_GrDvrFrmHdVdo
{
	St_GrDvrFrmHdBase	Base;

	__u8	SeqId;			// frame sequence id
	__u8	Fps;				// signal frame rate

	__u8	Codec;
	__u8	_rsvd;

	__u32	VdoFmt;			// interlace[31], even field[30], width[16~29], height[0~15] 

	__u32	Wtmk;				// water mark
	
}	*Ptr_GrDvrFrmHdVdo;

typedef	struct St_GrDvrFrmHdAdo
{
	St_GrDvrFrmHdBase	Base;
	BOOL8	IsAble;			// FALSE = reserve area, TRUE = able data
	__u8	SeqId;
	__u8	Codec;
	__u8	_rsvd0;
	__u32	BufOfs;			// dec buffer offset
	__u32	_rsvd1[1];			// 
}	*Ptr_GrDvrFrmHdAdo;

typedef	struct St_GrDvrLogItm 
{
	Def_GrTime	Time;
	__u16	LogType;	//event type
	__u8	Ch;				// 0xFF any channel
	__u8	ParaBt0;
	__u16	ParaWd0;
	__u16	ParaWd1;
	__u32	ParaDw0;
	__u32	ParaDw1;
	__u32	ParaDw2;
	__u32	ParaDw3;
	__u32	ParaDw4;
}	*Ptr_GrDvrLogItm;

typedef	struct St_GrDvrLogLvl
{
	Def_GrTime	Time;
	__u8	Type;
	__u8	Ch;
	__u16	_rsvd0;
}	*Ptr_GrDvrLogLvl;

typedef	struct St_GrDvrRecMinMap
{
	__u8	ChAtb[E_GrDvrMaxChCnt];
}	*Ptr_GrDvrRecMinMap;


//====================================================================
//global function

__u8	GrDvrRecAtbToThumb( __u8 A_RecAtb );

BOOL8	GrDvrVdoDecAbleChk(__u8 A_PrvType, __u8 A_PrvSeq, __u8 A_NowType, __u8 A_NowSeq);

#ifndef LINUX_DRV

__u32	GrDvrVdoFmtMake(__u32 A_ResX, __u32 A_ResY, BOOL8 A_IsInterlace = FALSE);
void	GrDvrVdoFmtInfoGet( __u32 A_VdoFmt, __u32* A_PtrRtResX, __u32* A_PtrRtResY, BOOL8* A_PtrRtIsInterlace );

#endif

//====================================================================
//out Environment conditional
#endif

