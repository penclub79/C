/*
	GAUSE Platform script

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4Script
#define	_EPC_GrP4Script

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P4/GrP4Base.h>
#include	<GrGdibBase.h>
#include <Dvr/GrDvrBase.h>

//====================================================================
//constance

// script version
#define E_GrP4ScptFcc									Mac_GrMakeFcc('P','3','S','0')
#define E_GrP4ScptVer									1							/* script version */

#define E_GrP4ScptConMaxCnt						2							/* console max count */

//#define E_GrP4ScptRuleMaxCnt					32						/* rule count */

#define	E_GrP4VencResTypeCnt					4							/* resolution type count */
#define	E_GrP4VencResSubCnt						4							/* resolution sub type count */

#if 0
#define E_GrP4ScptRuleWinInfo					0							/* window info */
#define E_GrP4ScptRulePageInfo				1							/* page info */
#define E_GrP4ScptRuleAsmInfo					2							/* assembly info */
#define E_GrP4ScptRulePstInfo					3							/* paint script info */
#define E_GrP4ScptRuleFontInfo				4							/* font info */
#define E_GrP4ScptRuleImgInfo					5							/* image info */
#define E_GrP4ScptRuleTextInfo				6							/* text info */
#endif

// data script type
/* 1 ~ 2 : use by DVL */
#define	E_GrP4ScptDataWinScptTbl			3		/* window script info table */
/* 4 : use by DVL */
#define	E_GrP4ScptDataWinScptData			5		/* window script data */
/* 6 ~ 7 : use by DVL */
#define	E_GrP4ScptDataPstScptData			8		/* PST script data */
#define	E_GrP4ScptDataImageData				9		/* image data */
#define	E_GrP4ScptDataFontData				10	/* font data */
#define	E_GrP4ScptDataTextData				11	/* text data */
/* 12 ~ 16 : use by DVL */
#define	E_GrP4ScptDataAsmTbl					17	/* assembly script table */
#define	E_GrP4ScptDataAsmData					18	/* assembly code data */
/* 19 : use by DVL */
#define	E_GrP4ScptDataPageScptTbl			20	/* page script info table */
/* 21 : use by DVL */
#define	E_GrP4ScptDataPageScptData		22	/* page script data */
/* 23 ~ 26 : use by DVL */
#define	E_GrP4ScptDataCfgPrj					27	/* configuration project */
#define	E_GrP4ScptDataCfgRtl					28	/* configuration runtime */
#define	E_GrP4ScptDataMdlAtb					29	/* model attribute */
#define	E_GrP4ScptDataPstTbl					30	/* PST single skin script  */

// script model attribute
#define E_GrP4ScptMdlAtbClstCnt				204						/* script model attribute control list item count */

// script attribute
//#define E_GrP4ScptAtbAdrReloc					0x00000001		/* address relocated */


// text
#define E_GrP4ScptLangMaxCnt					32						/* max language count */
#define E_GrP4ScptTxtIdNone						0
#define E_GrP4ScptTxtIdBase						1
#define E_GrP4ScptTxtIdCtrl						30000					/* control text id */
#define E_GrP4ScptTxtCtrlCnt					16						/* max control text count */

// base text id
// language
#define E_GrP4ScptTxtIdLang0					(0 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdLang1					(1 + E_GrP4ScptTxtIdBase) 
#define E_GrP4ScptTxtIdLang2					(2 + E_GrP4ScptTxtIdBase) 
#define E_GrP4ScptTxtIdLang3					(3 + E_GrP4ScptTxtIdBase) 
#define E_GrP4ScptTxtIdLang4					(4 + E_GrP4ScptTxtIdBase) 
#define E_GrP4ScptTxtIdLang5					(5 + E_GrP4ScptTxtIdBase) 
#define E_GrP4ScptTxtIdLang6					(6 + E_GrP4ScptTxtIdBase) 
#define E_GrP4ScptTxtIdLang7					(7 + E_GrP4ScptTxtIdBase) 
#define E_GrP4ScptTxtIdLang8					(8 + E_GrP4ScptTxtIdBase) 
#define E_GrP4ScptTxtIdLang9					(9 + E_GrP4ScptTxtIdBase) 
#define E_GrP4ScptTxtIdLang10					(10 + E_GrP4ScptTxtIdBase) 
#define E_GrP4ScptTxtIdLang11					(11 + E_GrP4ScptTxtIdBase) 
#define E_GrP4ScptTxtIdLang12					(12 + E_GrP4ScptTxtIdBase) 
#define E_GrP4ScptTxtIdLang13					(13 + E_GrP4ScptTxtIdBase) 
#define E_GrP4ScptTxtIdLang14					(14 + E_GrP4ScptTxtIdBase) 
#define E_GrP4ScptTxtIdLang15					(15 + E_GrP4ScptTxtIdBase) 
#define E_GrP4ScptTxtIdLang16					(16 + E_GrP4ScptTxtIdBase) 
#define E_GrP4ScptTxtIdLang17					(17 + E_GrP4ScptTxtIdBase) 
#define E_GrP4ScptTxtIdLang18					(18 + E_GrP4ScptTxtIdBase) 
#define E_GrP4ScptTxtIdLang19					(19 + E_GrP4ScptTxtIdBase) 
#define E_GrP4ScptTxtIdLang20					(20 + E_GrP4ScptTxtIdBase) 
#define E_GrP4ScptTxtIdLang21					(21 + E_GrP4ScptTxtIdBase) 
#define E_GrP4ScptTxtIdLang22					(22 + E_GrP4ScptTxtIdBase) 
#define E_GrP4ScptTxtIdLang23					(23 + E_GrP4ScptTxtIdBase) 
#define E_GrP4ScptTxtIdLang24					(24 + E_GrP4ScptTxtIdBase) 
#define E_GrP4ScptTxtIdLang25					(25 + E_GrP4ScptTxtIdBase) 
#define E_GrP4ScptTxtIdLang26					(26 + E_GrP4ScptTxtIdBase) 
#define E_GrP4ScptTxtIdLang27					(27 + E_GrP4ScptTxtIdBase) 
#define E_GrP4ScptTxtIdLang28					(28 + E_GrP4ScptTxtIdBase) 
#define E_GrP4ScptTxtIdLang29					(29 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdLang30					(30 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdLang31					(31 + E_GrP4ScptTxtIdBase) 

// day unit
#define E_GrP4ScptTxtIdUnitYear				(35 + E_GrP4ScptTxtIdBase) 
#define E_GrP4ScptTxtIdUnitMonth			(36 + E_GrP4ScptTxtIdBase) 
#define E_GrP4ScptTxtIdUnitDay				(37 + E_GrP4ScptTxtIdBase) 
#define E_GrP4ScptTxtIdUnitHour				(38 + E_GrP4ScptTxtIdBase) 
#define E_GrP4ScptTxtIdUnitMin				(39 + E_GrP4ScptTxtIdBase) 
#define E_GrP4ScptTxtIdUnitSec				(40 + E_GrP4ScptTxtIdBase) 

// am / pm
#define E_GrP4ScptTxtIdTimeAm					(41 + E_GrP4ScptTxtIdBase) 
#define E_GrP4ScptTxtIdTimePm					(42 + E_GrP4ScptTxtIdBase) 

// system part 1
#define E_GrP4ScptTxtIdChannel				(50 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdDetected				(51 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdChanged				(52 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdStart					(53 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdEnd						(54 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdResolution			(55 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdRecord					(56 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdRecQuility			(57 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdRecSpeed				(58 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdRecPost				(59 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdRecPre					(60 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdOn							(61 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdOff						(62 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdMotionMap			(63 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdRecSchd				(64 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdPassword				(65 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdCamera					(66 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdTitle					(67 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdNetwork				(68 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdDisconnect			(69 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdConnect				(70 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdSearch					(71 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdBackup					(72 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdMotion					(73 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdSensor					(74 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdEvtExt1				(75 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdEvtExt2				(76 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdEvtExt3				(77 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdEvtExt4				(78 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdEvtExt5				(79 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdVdoLoss				(80 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdVdoRcvy				(81 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdPowerOn				(82 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdPowerOff				(83 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdLogin					(84 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdLogout					(85 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdSetupChg				(86 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdHddErr					(87 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdSetupIn				(88 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdSetupOut				(89 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdHddFormat			(90 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdRamup					(91 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdEmergency			(92 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdMacaddress			(93 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdIp							(94 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdPort						(95 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdUse						(96 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdSensitivity		(97 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdWebcode				(98 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdFwVer					(99 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdNwVer					(100 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdModelNum				(101 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdFileSystem			(102 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdFirmWare				(103 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdUpgrade				(104 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdRtcErr					(105 + E_GrP4ScptTxtIdBase)

// month
#define E_GrP4ScptTxtIdMonth1					(106 + E_GrP4ScptTxtIdBase) 
#define E_GrP4ScptTxtIdMonth2					(107 + E_GrP4ScptTxtIdBase) 
#define E_GrP4ScptTxtIdMonth3					(108 + E_GrP4ScptTxtIdBase) 
#define E_GrP4ScptTxtIdMonth4					(109 + E_GrP4ScptTxtIdBase) 
#define E_GrP4ScptTxtIdMonth5					(110 + E_GrP4ScptTxtIdBase) 
#define E_GrP4ScptTxtIdMonth6					(111 + E_GrP4ScptTxtIdBase) 
#define E_GrP4ScptTxtIdMonth7					(112 + E_GrP4ScptTxtIdBase) 
#define E_GrP4ScptTxtIdMonth8					(113 + E_GrP4ScptTxtIdBase) 
#define E_GrP4ScptTxtIdMonth9					(114 + E_GrP4ScptTxtIdBase) 
#define E_GrP4ScptTxtIdMonth10				(115 + E_GrP4ScptTxtIdBase) 
#define E_GrP4ScptTxtIdMonth11				(116 + E_GrP4ScptTxtIdBase) 
#define E_GrP4ScptTxtIdMonth12				(117 + E_GrP4ScptTxtIdBase) 

// week
#define E_GrP4ScptTxtIdWeekSun				(118 + E_GrP4ScptTxtIdBase) 
#define E_GrP4ScptTxtIdWeekMon				(119 + E_GrP4ScptTxtIdBase) 
#define E_GrP4ScptTxtIdWeekTue				(120 + E_GrP4ScptTxtIdBase) 
#define E_GrP4ScptTxtIdWeekWed				(121 + E_GrP4ScptTxtIdBase) 
#define E_GrP4ScptTxtIdWeekThu				(122 + E_GrP4ScptTxtIdBase) 
#define E_GrP4ScptTxtIdWeekFri				(123 + E_GrP4ScptTxtIdBase) 
#define E_GrP4ScptTxtIdWeekSat				(124 + E_GrP4ScptTxtIdBase) 

// system part 2
#define E_GrP4ScptTxtIdHddSysChg			(125 + E_GrP4ScptTxtIdBase)
#define E_GrP4ScptTxtIdHdd						(126 + E_GrP4ScptTxtIdBase)


// STX
#define E_GrP4ScptObjStx							0x4F
#define E_GrP4ScptParaStx							0x50
#define E_GrP4ScptObjEdx							0x45

// script id
#define E_GrP4ScptIdNone							0				/* none script */
#define E_GrP4ScptIdCrnt							0xFFFE	/* current script id - event ... */
#define E_GrP4ScptIdAll								0xFFFF	/* all script */

#define E_GrP4ScptIdWinBase						1				/* window base script id */
#define E_GrP4ScptIdPageBase					1000		/* page base script id */
#define E_GrP4ScptIdSlaveBase					4000		/* slave object base script id */

// paint script id
#define E_GrP4ScptPstIdNone						0				/* paint script id */
#define E_GrP4ScptPstIdBase						1				/* paint script id base */
#define E_GrP4ScptPstIdDflt						1				/* paint script id default */

#define E_GrP4ScptPstTypeNone					0
#define E_GrP4ScptPstTypeClrFill			1				/* color fill */
#define E_GrP4ScptPstTypeLine					2				/* line draw */
#define E_GrP4ScptPstTypeImage				3				/* image */
#define E_GrP4ScptPstTypeText					4				/* text */
#define E_GrP4ScptPstTypeFont					5				/* font */
#define E_GrP4ScptPstTypeStyle				6				/* text style */

#define	E_GrP4ScptPstTypeMask					0x7F		/* type mask */

#define E_GrP4ScptPstTypeDisable			0x80		/* disable mask */

#define E_GrP4ScptPstImgAtbFill				0x01		/* image fill */
#define E_GrP4ScptPstImgAtbSprite			0x02		/* sprite image */

#define E_GrP4ScptPstTxtAtbFixPitch				(1 << 0)	/* fixed pitch */
#define E_GrP4ScptPstTxtAtbDrawShadow			(1 << 1)	/* shadow draw */
#define E_GrP4ScptPstTxtAtbMultiLine			(1 << 2)	/* multi line */
#define E_GrP4ScptPstTxtAtbWarp						(1 << 3)	/* warp */
#define E_GrP4ScptPstTxtAtbSemiEnter			(1 << 4)	/* semicolon enter */
#define E_GrP4ScptPstTxtAtbPasswd					(1 << 5)	/* password mode */

#define E_GrP4ScptPstTxtAlignHzCenter			0
#define E_GrP4ScptPstTxtAlignHzLeft				1
#define E_GrP4ScptPstTxtAlignHzRight			2
#define E_GrP4ScptPstTxtAlignVtCenter			0
#define E_GrP4ScptPstTxtAlignVtTop				1
#define E_GrP4ScptPstTxtAlignVtBottom			2

// para type id
#define E_GrP4ScptParaTypeChar				0				/* __s8 */
#define E_GrP4ScptParaTypeShort				1				/* __s16 */
#define E_GrP4ScptParaTypeInt					2				/* __s32 */
#define E_GrP4ScptParaTypeBool8				3				/* bool8 */
#define E_GrP4ScptParaTypeByteRect		4				/* St_GrByteRect */
#define E_GrP4ScptParaTypeWordRect		5				/* St_GrWordRect */
#define E_GrP4ScptParaTypeIntRect			6				/* St_GrRect */
#define E_GrP4ScptParaTypeScptId			7				/* script id */
#define E_GrP4ScptParaTypeColor				8				/* color */
#define E_GrP4ScptParaTypeSkin				9				/* skin image */
#define E_GrP4ScptParaTypeBrdMap			10			/* border map */

#define E_GrP4ScptParaTypeTextId			20			/* text id */


//====================================================================
//global macro

//====================================================================
//global type

typedef	struct	St_GrP4ScptDataHd
{
	__u8	Type;			// data type
	__u8	SubIdx;		// skin index or language index
	__u16	Id;				// script id
	__u32	Size;			// data size
}	*Ptr_GrP4ScptDataHd;

typedef	struct St_GrP4ScptObjHd 
{
	__u8	Stx;			// O
	__u8	ClassId;	// class id
	__u16	ScptId;		// script id
	__u8	_rsvd0;
	__u8	PosRel;
	__u16	ParaSize;			// parameter size
	St_GrSwordRect	Rect;
}	*Ptr_GrP4ScptObjHd;

typedef	struct	St_GrP4ScptParaHd
{
	__u8	Stx;			// P
	__u8	ClassId;	// class id
	__u16	ParaSize;	// parameter size
}	*Ptr_GrP4ScptParaHd;

typedef	struct St_GrP4ScptObjEnd 
{
	__u8	Edx;		// E
	__u8	_rsvd;
	__u16	ScptId;
}	*Ptr_GrP4ScptObjEnd;

typedef	struct	St_GrP4ScptAsmInfo
{
	void*	Pos;
	__u16	Cnt;		// code count
	__u16	_rsvd;
}	*Ptr_GrP4ScptAsmInfo;

typedef	struct	St_GrP4ScptTxtItm
{
	WCHAR*	LngTbl[E_GrP4ScptLangMaxCnt];
}	*Ptr_GrP4ScptTxtItm;


typedef struct St_GrP4ScptPstItm 
{
	void*	Pos;
	__u8	Cnt;
	__u8	_rsvd0;
	__u16	_rsvd1;
}	*Ptr_GrP4ScptPstItm;

typedef	struct	St_GrP4ScptPstData
{
	__s16	Left;
	__s16	Top;
	__s16	Right;
	__s16	Bottom;
	__u8	Basis;
	__u8	Type;
	__u8	Para0;
	__u8	Para1;
	__u32	Value;
}	*Ptr_GrP4ScptPstData;

typedef	struct	St_GrP4ScptWinInfo
{
	void*	Pos;
	__u8	Layer;
	__u8	_rsvd0;
	__u16	Privilege;
}	*Ptr_GrP4ScptWinInfo;

typedef	struct	St_GrP4ScptPageInfo
{
	void*	Pos;
	__u16	AsmOpen;			// open code id
	__u16	AsmClose;			// close code id
	__u16	EscKeyFcsId;	// exit focus id
	__u16	_rsvd1;
	__u32	_rsvd[1];			// reserved
}	*Ptr_GrP4ScptPageInfo;

typedef	struct	St_GrP4ScptCfgSkin
{
	__u32	ClrCus;		// cursor color
	__u32	_rsvd[3];	
}	*Ptr_GrP4ScptCfgSkin;

typedef	struct	St_GrP4ScptVkbdLang
{
	__u8	PageCnt;
	__u8	_rsvd0;
	__u16	_rsvd1;
	WCHAR	KeyTbl[E_GrP4VkeyShftCnt][E_GrP4VkeyTxtCnt];
}	*Ptr_GrP4ScptVkbdLang;

typedef	struct St_GrP4ScptCfgPrj		// project config
{
	__u32	FmtFcc;				// format FCC
	__u16	CusImgId;			// software cursor image id
	__u8	UartCnt;			// UART count
	__u8	_rsvd0;				
	__u32	_rsvd1;				
	__u32	FwVer;				// firmware version
	St_GrP4ScptVkbdLang	VkbdTbl[E_GrP4ScptLangMaxCnt];		// virtual keyboard table
}	*Ptr_GrP4ScptCfgPrj;

typedef	struct St_GrP4ScptCfgRtl
{
	BOOL8	IsChgConAble;	// can change console focus
	__u8	_rsvd1;				// 
	__u8	_rsvd2;				// 
	__u8	_rsvd3;				// 
	St_GrP4ScptCfgSkin	Skin;
}	*Ptr_GrP4ScptCfgRtl;

typedef	struct	St_GrP4ScptCfgMdl		// model config
{
	__u16	ModelNum;			// [0000] model number
	__u16	AsmInit;			// [0002] assembly init
	__u16	AsmStart;			// [0004] assembly start
	__u16	AsmCfgReset;	// [0006] assembly configuration reset
	__u16	AsmCfgPatch;	// [0008] assembly configuration patch
	__u16	AsmFormat;		// [000A] assembly HDD format
	__u16	AsmLivePlay;	// [000C] assembly live / play mode change
	__u16	AsmFwUpDone;	// [000E] firmware upgrade finished
	__u8	VsstType;			// [0010] 0 = none , 1 ...
	BOOL8	IsIoBox;			// [0011] extended IO box able
	BOOL8	IsEvtPreset;	// [0012] event preset
	BOOL8	IsFreeDdns;		// [0013] free DDNS
	BOOL8	IsPtzKbd;			// [0014] pan-tilt keyboard
	__u8	DfltVgaRes;		// [0015] default VGA resolution
	__u8	DfltIconPos;	// [0016] default quad icon position
	__u8	DfltLang;			// [0017] default language
	__u8	DfltSchd;			// [0018] default record schedule
	BOOL8	IsBkvwLater;	// [0019] backup viewer extract later
	BOOL8	IsRamup;			// [001A] ram-up use
	BOOL8	IsSmart;			// [001B] HDD smart use
	BOOL8	IsQuadSave;		// [001C] last quad save
	BOOL8	IsLoginSave;	// [001D] last login save
	__u8	MaxHddCnt;		// [001E] usable HDD map - bitmap
	__u8	IsPassChk;		// [001F] password check 
	BOOL8	IsEnvSenor;		// [0020] environment sensor use
	__u8	LongKeyTime;	// [0021] long key time
	__u16	TxtVkbdId;		// [0x22] text virtual keyboard window id
	__u16	NumVkbdId;		// [0x24] number virtual keyboard window id
	__u16	NotifyId;			// [0x26] Notify window default
	__u16	_rsvd11;			// [0x28] 
	__u16	_rsvd12;			// [0x2A] 

	__u16	AsmLogEnd;		// [0x2C] log backup end event
	__u16	AsmLogErr;		// [0x2E] log backup error event

	__u16	_rsvd13;			// [0x30] 
	__u16	_rsvd14;			// [0x32] 
	__u16	_rsvd15;			// [0x34] 
	__u16	_rsvd16;			// [0x36] 
	__u16	_rsvd17;			// [0x38] 
	__u16	_rsvd18;			// [0x3A] 
	__u16	_rsvd19;			// [0x3C] 
	__u16	_rsvd20;			// [0x3E] 
	__u16	_rsvd21;			// [0x40] 

	__u16	_rsvd1;				// [0x42] reserved

	__u32	_rsvd2[4];		// [0x44] reserved

	__u16	PassRuleTitle;	// [0x54] bad password rule message title text id
	__u16	PassRuleCont;		// [0x56] bad password rule message contents text id

	__u8	UserIdCnt;			// [0x58] authority id count
	BOOL8	IsPassDual;			// [0x59] dual password
	BOOL8	IsEmgcLock;			// [0x5A] emergency key lock usable
	__u8	IsMtnSchd;			// [0x5B] motion schedule

	WCHAR	MasterPass[16];	// [0x5C] master password

	__u16	AsmDenEmgc;			// [0x7C] emergency denial event
	__u16	AsmWinPriv;			// [0x7E] window has not privilege

	// version 4 expansion
	__u8	StartLogin;			// [0x80] start login type , 0=ADMIN,1=by setup
	__u8	IsLogRec;				// [0x81] event log record only
	__u16	AsmLanOnOff;		// [0x82]	LAN on off status

	BOOL8	IsCmsFireWall;	// [0x84] CMS firewall use
	BOOL8	IsCvtUsr;				// [0x85] user covert active
	__u16	AsmPlayStat;		// [0x86] play status change

	__u16	AsmUsbChg;			// [0x88] USB change event
	__u16	AsmCmsChg;			// [0x8A] CMS client count change event

	__u16	AsmQuad0Chg;		// [0x8C] console 0 QUAD change
	__u16	AsmQuad1Chg;		// [0x8C] console 1 QUAD change

	__u16	AsmBkupStart;		// [0x90] backup start
	__u16	AsmBkupEnd;			// [0x92] backup end
	__u16	AsmBkupErr;			// [0x94] backup error
	BOOL8	IsDmaxFront;		// [0x96] D-Max front use
	__u8	WebLogo;				// [0x97] web logo

	__u16	AsmErrOcu;			// [0x98] error occur
	__u16	AsmUserChg;			// [0x9A] log in log out status change

	__u16	AsmNtpTest;			// [0x9C] NTP Test finish event
	__u16	AsmLogStart;		// [0x9E] log backup start

	__u16	AsmNetUpgd;			// [0xA0] network upgrade ready
	__u16	_rsvd3;					// [0xA2] reserved

	__u16	VrecQt[E_GrP4VencResTypeCnt][E_GrP4VencResSubCnt][E_GrP4VencQtyCnt];	// [0xA4]
	__u16	VnetQt[E_GrP4VencResTypeCnt][E_GrP4VencResSubCnt][E_GrP4VencQtyCnt];	// [0x144]

	__u16	_rsvd4;					// [0x1E4] reserved
	__u16	_rsvd5;					// [0x1E6] reserved

}	*Ptr_GrP4ScptCfgMdl;

typedef	struct	St_GrP4ScptHdInfo
{
	__u32	Fcc;
	__u32	Ver;
	Def_GrTime	Time;		// make time
	__u32	_rsvd[5];
}	*Ptr_GrP4ScptHdInfo;

//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif

