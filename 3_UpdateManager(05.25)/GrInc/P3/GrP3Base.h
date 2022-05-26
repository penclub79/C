/*
	Gause Platform Base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3Base
#define	_EPC_GrP3Base

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

//====================================================================
//constance

#define E_GrP3UserIdMax							8

#define E_GrP3CtlgParaMax						6

#define E_GrP3CtlgIdNone						0		/* catalog id none */
#define E_GrP3CtlgIdFwUpgd					1		/* catalog id firmware upgrade */
//#define E_GrP3CtlgIdHoliday					2		/* catalog id holiday */
#define E_GrP3CtlgIdCfg							3		/* catalog id config file */
#define E_GrP3CtlgIdLog							4		/* catalog id event log */

#define E_GrP3DoUserDataCnt					4

#define E_GrP3MngVmRparaMax					4

// layer
#define E_GrP3UiLayerDflt						1	/* default UI layer */

// console index
#define E_GrP3ConIdxAll							0xFE	/* all console */
#define E_GrP3ConIdxFocus						0xFF	/* focused index */

// window id
#define E_GrP3WinIdAll							0xFFFE				/* all layer */
#define E_GrP3WinIdNone							0xFFFF				/* none - command effective console only */

#define E_GrP3WinIdSlotBitPos				8
#define E_GrP3WinIdSlotBitMask			0x07
#define E_GrP3WinIdLayerBitPos			12
#define E_GrP3WinIdLayerBitMask			0x07

// class id
#define E_GrP3DoClassNone						0		/* none */
#define E_GrP3DoClassBase						1		/* Cls_GrP3DoBase */
#define E_GrP3DoClassWin						2		/* Cls_GrP3DoWin */
#define E_GrP3DoClassFrame					3		/* Cls_GrP3DoFrame */
#define E_GrP3DoClassLabel					4		/* Cls_GrP3DoLbl */
#define E_GrP3DoClassPrgsBarLoc			5		/* Cls_GrP3DoPrgsBarLoc */
#define E_GrP3DoClassQuad						6		/* Cls_GrP3DoQuad */
#define E_GrP3DoClassBtnPage				7		/* Cls_GrP3DoBtnPage */
#define E_GrP3DoClassStatic					8		/* Cls_GrP3DoStatic */
#define E_GrP3DoClassEdtStpStr			9		/* Cls_GrP3DoEdtStpStr */
#define E_GrP3DoClassBtnLbl					10	/* Cls_GrP3DoBtnLbl */
#define E_GrP3DoClassSpinNumBase		11	/* Cls_GrP3DoSpinNumBase */
#define E_GrP3DoClassSpinNumStp			12	/* Cls_GrP3DoSpinNumStp */
#define E_GrP3DoClassSpinTxtBase		13	/* Cls_GrP3DoSpinTxtBase */
#define E_GrP3DoClassSpinTxtStp			14	/* Cls_GrP3DoSpinTxtStp */
#define E_GrP3DoClassRelStc					15	/* Cls_GrP3DoSRelStc */
#define E_GrP3DoClassSpinPageTxt		16	/* Cls_GrP3DoSpinPageTxt */
#define E_GrP3DoClassSpinDdgtBase		17	/* Cls_GrP3DoSpinDdgtBase */
#define E_GrP3DoClassSpinDdgtStp		18	/* Cls_GrP3DoSpinDdgtStp */
#define E_GrP3DoClassBtnStc					19	/* Cls_GrP3DoBtnStc */
#define E_GrP3DoClassChkbBase				20	/* Cls_GrP3DoChkbBase */
#define E_GrP3DoClassChkbSsb				21	/* Cls_GrP3DoChkbSsb */
#define E_GrP3DoClassChkbSsm				22	/* Cls_GrP3DoChkbSsm */
#define E_GrP3DoClassSbarVar				23	/* Cls_GrP3DoSbarVar */
#define E_GrP3DoClassSbarStp				24	/* Cls_GrP3DoSbarStp */
#define E_GrP3DoClassRbtnBase				25	/* Cls_GrP3DoRbtnBase */
#define E_GrP3DoClassRbtnStp				26	/* Cls_GrP3DoRbtnStp */
#define E_GrP3DoClassChkbSgb				27	/* Cls_GrP3DoChkbSgb */
#define E_GrP3DoClassChkbSgm				28	/* Cls_GrP3DoChkbSgm */
#define E_GrP3DoClassSpinPnt				29	/* Cls_GrP3DoSpinPnt */
#define E_GrP3DoClassSpinNumLoc			30	/* Cls_GrP3DoSpinNumLoc */
#define E_GrP3DoClassSpinDdgtLoc		31	/* Cls_GrP3DoSpinDdgtLoc */
#define E_GrP3DoClassSpinTxtLoc			32	/* Cls_GrP3DoSpinTxtLoc */
#define E_GrP3DoClassTimeCtrl				33	/* Cls_GrP3DoTimeCtrl */
#define E_GrP3DoClassCalBase				34	/* Cls_GrP3DoCalBase */
#define E_GrP3DoClassCalRec					35	/* Cls_GrP3DoCalRec */
#define E_GrP3DoClassSbarBase				36	/* Cls_GrP3DoSbarBase */
#define E_GrP3DoClassPrgsBarBase		37	/* Cls_GrP3DoPrgsBarBase */
#define E_GrP3DoClassPrgsBarGvs			38	/* Cls_GrP3DoPrgsBarGvs */
#define E_GrP3DoClassGridBase				39	/* Cls_GrP3DoGridBase */
#define E_GrP3DoClassGridRscStp			40	/* Cls_GrP3DoGridRscStp */
#define E_GrP3DoClassBtnRelStc			41	/* Cls_GrP3DoBtnRelStc */
#define E_GrP3DoClassGridRstat			42	/* Cls_GrP3DoGridRstat */
#define E_GrP3DoClassTimeLbl				43	/* Cls_GrP3DoTimeLbl */
#define E_GrP3DoClassLbxBase				44	/* Cls_GrP3DoLbxBase */
#define E_GrP3DoClassLbxCtlg				45	/* Cls_GrP3DoLbxCtlg */
#define E_GrP3DoClassSpinQuadStp		46	/* Cls_GrP3DoSpinQuadStp */
#define E_GrP3DoClassRbtnGvu				47	/* Cls_GrP3DoRbtnGvu */
#define E_GrP3DoClassRbtnSsm				48	/* Cls_GrP3DoRbtnSsm */
#define E_GrP3DoClassChkbLoc				49	/* Cls_GrP3DoChkbLoc */
#define E_GrP3DoClassEdtBase				50	/* Cls_GrP3DoEdtBase */
#define E_GrP3DoClassEdtLoc					51	/* Cls_GrP3DoEdtLoc */
#define E_GrP3DoClassSpinId					52	/* Cls_GrP3DoSpinId */
#define E_GrP3DoClassGridMtnStp			53	/* Cls_GrP3DoGridMtnStp */
#define E_GrP3DoClassCalHday				54	/* Cls_GrP3DoCalHday */
#define E_GrP3DoClassPnlPtz					55	/* Cls_GrP3DoPnlPtz */
#define E_GrP3DoClassSpinBtxtBase		56	/* Cls_GrP3DoSpinBtxtBase */
#define E_GrP3DoClassSpinBtxtStp		57	/* Cls_GrP3DoSpinTxtStp */
#define E_GrP3DoClassGridRrng				58	/* Cls_GrP3DoGridRrng */
#define E_GrP3DoClassSpinValBase		59	/* Cls_GrP3DoSpinValBase */
#define E_GrP3DoClassSpinValStp			60	/* Cls_GrP3DoSpinValStp */
#define E_GrP3DoClassGridPushStp		61	/* Cls_GrP3DoGridPushStp */
#define E_GrP3DoClassGridPlayBar		62	/* Cls_GrP3DoGridPlayBar */

#define E_GrP3DoClassCnt						63	/* class count */

// image
#define E_GrP3ImgIdNone							0
#define E_GrP3ImgIdBase							1
#define E_GrP3ImgUserIdStart				0x8000

// input device type
#define E_GrP3InDevFront						0		/* input device front */
#define E_GrP3InDevRemocon					1		/* input device remote controller */
#define E_GrP3InDevPtzKbd						2		/* input device PTZ keyboard */

// virtual keyboard
#define	E_GrP3VkeyTxtCnt						47	/* virtual keyboard text key count */
#define E_GrP3VkeyShftCnt						4		/* virtual keyboard shift page count */

#define E_GrP3VkeyLang0							100	/* language 0 key */

#define E_GrP3VkeyEnter							150	/* enter key */
#define E_GrP3VkeySpace							151	/* space key */
#define E_GrP3VkeyPagePlus					152	/* page plus key */
#define E_GrP3VkeyPageMinus					153	/* page minus key */
#define E_GrP3VkeyDelete						154	/* delete key */
#define E_GrP3VkeyBackSpace					155	/* back space key */
#define E_GrP3VkeyClear							156	/* clear key */
#define E_GrP3VkeyHome							157	/* home key */
#define E_GrP3VkeyEnd								158	/* end key */
#define E_GrP3VkeyCusLeft						159	/* cursor left key */
#define E_GrP3VkeyCusRight					160	/* cursor right key */

// HDD
#define E_GrP3HddMaxCnt							8		/* max HDD count */

// record
#define E_GrP3RecHolyMax						4		/* max holiday count */
#define E_GrP3RecWeekCnt						(7 + E_GrP3RecHolyMax)	/* record schedule week count */

#define E_GrP3VencQtyCnt						5		/* video encode quality count */

// -------------------------------------------
// key
#define E_GrP3KeyNone								0x00

#define E_GrP3KeyCh0								0x01
#define E_GrP3KeyCh1								0x02
#define E_GrP3KeyCh2								0x03
#define E_GrP3KeyCh3								0x04
#define E_GrP3KeyCh4								0x05
#define E_GrP3KeyCh5								0x06
#define E_GrP3KeyCh6								0x07
#define E_GrP3KeyCh7								0x08
#define E_GrP3KeyCh8								0x09
#define E_GrP3KeyCh9								0x0A
#define E_GrP3KeyCh10								0x0B
#define E_GrP3KeyCh11								0x0C
#define E_GrP3KeyCh12								0x0D
#define E_GrP3KeyCh13								0x0E
#define E_GrP3KeyCh14								0x0F
#define E_GrP3KeyCh15								0x10
#define E_GrP3KeyCh16								0x11

#define E_GrP3KeyEnter							0x12
#define E_GrP3KeyEsc								0x13
#define E_GrP3KeyDelete							0x14
#define E_GrP3KeyLock								0x15

#define E_GrP3KeyLeft								0x16
#define E_GrP3KeyUp									0x17
#define E_GrP3KeyRight							0x18
#define E_GrP3KeyDown								0x19

#define E_GrP3KeyJogLeft						0x1A			/* same + key */	
#define E_GrP3KeyValMinus						0x1A			/* same -key */	
#define E_GrP3KeyJogRight						0x1B			/* same -key */	
#define E_GrP3KeyValPlus						0x1B			/* same + key */	

#define E_GrP3KeyEject							0x1C

/* reserve 1C ~ 1F */

#define E_GrP3KeyShtlLeft						0x20			//low 3 bit is position
#define E_GrP3KeyShtlL0							0x20
#define E_GrP3KeyShtlL1							0x21
#define E_GrP3KeyShtlL2							0x22
#define E_GrP3KeyShtlL3							0x23
#define E_GrP3KeyShtlL4							0x24
#define E_GrP3KeyShtlL5							0x25
#define E_GrP3KeyShtlL6							0x26
#define E_GrP3KeyShtlL7							0x27
#define E_GrP3KeyShtlRight					0x28			//low 3 bit is position
#define E_GrP3KeyShtlR0							0x28
#define E_GrP3KeyShtlR1							0x29
#define E_GrP3KeyShtlR2							0x2A
#define E_GrP3KeyShtlR3							0x2B
#define E_GrP3KeyShtlR4							0x2C
#define E_GrP3KeyShtlR5							0x2D
#define E_GrP3KeyShtlR6							0x2E
#define E_GrP3KeyShtlR7							0x2F

#define E_GrP3KeyId									0x30		//low 2 bit is id number -macro

#define E_GrP3KeySelId							0x34		//remote control id selection button
#define E_GrP3KeyMenu								0x35
#define E_GrP3KeyBackup							0x36
#define E_GrP3KeyCapture						0x37
#define E_GrP3KeySearch							0x38
#define E_GrP3KeyDisplay						0x39		/* change console focus */
#define E_GrP3KeyPtzMode						0x3A
#define E_GrP3KeyEmergency					0x3B		/* toggle type emergency */
#define E_GrP3KeyQuad								0x3C		/* fixed split mode  */
#define E_GrP3KeySequence						0x3D		/* auto sequence */
#define E_GrP3KeyAudio							0x3E
#define E_GrP3KeySplit							0x3F		/* multi split mode */
#define E_GrP3KeyEmgcyOn						0x40		/* emergency on */
#define E_GrP3KeyEmgcyOff						0x41		/* emergency off */
#define E_GrP3KeyNextQuad						0x42		/* quad group next */
#define E_GrP3KeyPrevQuad						0x43		/* quad group previous */

/* reserved 44 ~ 4F */
#define E_GrP3KeyPtzTele						0x50
#define E_GrP3KeyPtzWide						0x51
#define E_GrP3KeyPtzFar							0x52
#define E_GrP3KeyPtzNear						0x53

/* reserved 54 ~ 54 */

#define E_GrP3KeyReboot							0x55
#define E_GrP3KeyZmIn								0x56
#define E_GrP3KeyZmOut							0x57
#define E_GrP3KeyFcIn								0x58
#define E_GrP3KeyFcOut							0x59
#define E_GrP3KeyPtzSpd							0x5A
/* reserved 5B */
#define E_GrP3KeyPtzTour						0x5C

/* reserved 5D ~ 5F */

#define E_GrP3KeyQdMode1						0x60
#define E_GrP3KeyQdMode4						0x61
#define E_GrP3KeyQdMode6						0x62
#define E_GrP3KeyQdMode8						0x63
#define E_GrP3KeyQdMode9						0x64
#define E_GrP3KeyQdMode10						0x65
#define E_GrP3KeyQdMode13						0x66
#define E_GrP3KeyQdMode16						0x67

/* reserved 68 ~ 6F */
#define E_GrP3KeyMnLive							0x70


#define E_GrP3KeyExit								0x7F		//power off
// key max count 0x80 = 128
// -------------------------------------------
#define	E_GrP3EvtKeyTypeDown				0
#define	E_GrP3EvtKeyTypeUp					1
#define	E_GrP3EvtKeyTypeLong				2

// -------------------------------------------
// mouse
// mouse button index
#define E_GrP3MouseBtnLeft					0x0000
#define E_GrP3MouseBtnRight					0x0001
#define E_GrP3MouseBtnMiddle				0x0002

// mouse button map
#define E_GrP3MouseBmkkLeft					(1 << E_GrP3MouseBtnLeft)
#define E_GrP3MouseBmkRight					(1 << E_GrP3MouseBtnRight)
#define E_GrP3MouseBmkMiddle				(1 << E_GrP3MouseBtnMiddle)

// mouse message attribute
#define E_GrP3MouseAtbLbtnChg				(1 << E_GrP3MouseBtnLeft)
#define E_GrP3MouseAtbRbtnChg				(1 << E_GrP3MouseBtnRight)
#define E_GrP3MouseAtbMbtnChg				(1 << E_GrP3MouseBtnMiddle)
#define E_GrP3MouseAtbPosChg				(1 << 3)

// -------------------------------------------
// system status

#define E_GrP3SysStatQuadMode				0x01		/* quad mode change */
#define E_GrP3SysStatLangChg				0x02		/* language change */

#define E_GrP3SysStatSetupChg				0x10		/* setup data change */
#define E_GrP3SysStatLivePlay				0x11		/* live play mode change */
#define E_GrP3SysStatAccountChg			0x12		/* login account change */
#define E_GrP3SysStatVlossChg				0x13		/* video loss change */
#define E_GrP3SysStatTimeFmtChg			0x14		/* time format change */
#define E_GrP3SysStatRecEmgcChg			0x15		/* emergency status change */

// -------------------------------------------
// message
// manager message
#define E_GrP3MsgCmdMngMsgStart			0x0000

// console message
#define E_GrP3MsgCmdConMsgStart			0x0100

#define E_GrP3MsgCmdConWinClose			(0x00 + E_GrP3MsgCmdConMsgStart)		/* console window close : St_GrP3MsgWinClose */
#define E_GrP3MsgCmdConFocusCrt			(0x01 + E_GrP3MsgCmdConMsgStart)		/* console focus correct */
#define E_GrP3MsgCmdConChgConFcs		(0x02 + E_GrP3MsgCmdConMsgStart)		/* console focus change - dual monitor only - reach window */
#define E_GrP3MsgCmdConPaint				(0x03 + E_GrP3MsgCmdConMsgStart)		/* console paint : no parameter */

#define E_GrP3MsgCmdConInpKey				(0x04 + E_GrP3MsgCmdConMsgStart)		/* console input key : St_GrP3MsgKey */
#define E_GrP3MsgCmdConInpMouse			(0x05 + E_GrP3MsgCmdConMsgStart)		/* mouse status change : St_GrP3MsgMouse */
#define E_GrP3MsgCmdConInpWheel			(0x06 + E_GrP3MsgCmdConMsgStart)		/* mouse wheel change : St_GrP3MsgWheel */

#define E_GrP3MsgCmdConLayUpClose		(0x07 + E_GrP3MsgCmdConMsgStart)		/* console layer up close : St_GrP3MsgLayer */

// object common message
#define E_GrP3MsgCmdObjMsgStart			0x0200
#define E_GrP3MsgCmdKey							(0x00 + E_GrP3MsgCmdObjMsgStart)		/* key : St_GrP3MsgKey */
#define E_GrP3MsgCmdMouse						(0x01 + E_GrP3MsgCmdObjMsgStart)		/* mouse status change : St_GrP3MsgMouse */
#define E_GrP3MsgCmdWheel						(0x02 + E_GrP3MsgCmdObjMsgStart)		/* mouse wheel change */

#define E_GrP3MsgCmdSysStat					(0x08 + E_GrP3MsgCmdObjMsgStart)		/* system status change : St_GrP3MsgSysStat */
//#define E_GrP3MsgCmdScptDecCpl			(0x09 + E_GrP3MsgCmdObjMsgStart)		/* ui script decode completed */

#define E_GrP3MsgCmdFocusGet				(0x10 + E_GrP3MsgCmdObjMsgStart)		/* object focus get */
#define E_GrP3MsgCmdFocusLoss				(0x11 + E_GrP3MsgCmdObjMsgStart)		/* object focus loss */
#define E_GrP3MsgCmdMouseEnter			(0x12 + E_GrP3MsgCmdObjMsgStart)		/* St_GrP3MsgHd	*/
#define E_GrP3MsgCmdMouseLeave			(0x13 + E_GrP3MsgCmdObjMsgStart)		/* St_GrP3MsgHd	*/
#define E_GrP3MsgCmdTickPeriod			(0x14 + E_GrP3MsgCmdObjMsgStart)		/* St_GrP3EvtTick */	

#define E_GrP3MsgCmdVkbdEnd					(0x20 + E_GrP3MsgCmdObjMsgStart)		/* virtual keyboard edit end St_GrP3MsgHd */	

// window message
#define E_GrP3MsgCmdWinMsgStart			0x0300

// frame message
#define E_GrP3MsgCmdFrmMsgStart			0x0400
#define E_GrP3MsgCmdFrameReqPageChg	(0x00 + E_GrP3MsgCmdFrmMsgStart)		/* request page change : St_GrP3MsgFrmReqPageChg */
#define E_GrP3MsgCmdFramePageChged	(0x01 + E_GrP3MsgCmdFrmMsgStart)		/* page changed : St_GrP3MsgFrmPageChged */

// button message
#define E_GrP3MsgCmdBtnMsgStart			0x0500
#define E_GrP3MsgCmdBtnDown					(0x00 + E_GrP3MsgCmdBtnMsgStart)		/* button down : St_GrP3MsgObj */
#define E_GrP3MsgCmdBtnUp						(0x01 + E_GrP3MsgCmdBtnMsgStart)		/* button up : St_GrP3MsgObj */
#define E_GrP3MsgCmdBtnChkChg				(0x02 + E_GrP3MsgCmdBtnMsgStart)		/* check value change : St_GrP3MsgObj */

// spin control message
#define E_GrP3MsgCmdSpinMsgStart		0x0600
#define E_GrP3MsgCmdSpinValChg			(0x00 + E_GrP3MsgCmdSpinMsgStart)		/* spin control value change : St_GrP3MsgObj */

// slider bar message		
#define E_GrP3MsgCmdSbarMsgStart		0x0700
#define E_GrP3MsgCmdSbarValChg			(0x00 + E_GrP3MsgCmdSbarMsgStart)		/* slider bar value change : St_GrP3MsgObj */

// edit message
#define E_GrP3MsgCmdEditMsgStart		0x0800
#define E_GrP3MsgCmdEditValChg			(0x00 + E_GrP3MsgCmdEditMsgStart)		/* edit value change : St_GrP3MsgObj */

//====================================================================
//global macro

//====================================================================
//global type

// para patch
typedef	struct St_GrP3MngVmRpara
{
	__u32	Cnt;		// count
	__u8	Ridx[E_GrP3MngVmRparaMax];
	__s32		Rval[E_GrP3MngVmRparaMax];
}	*Ptr_GrP3MngVmRpara;

// UI message header
typedef	struct St_GrP3EvtCallMsg
{
	__u16	CodeId;
	__u8	ConIdx;
	__u16	WinId;
	__u16	ScptId;
	St_GrP3MngVmRpara	Para;
}	*Ptr_GrP3EvtCallMsg;

typedef	struct St_GrP3MsgHd
{
	__u16		Cmd;			// message command
	__u8		ConIdx;		// console index
	__u8		_rsvd0;		// reserved
	__u16		WinId;		// window id
	__u16		ObjId;		// object script id
}	*Ptr_GrP3MsgHd;

typedef	struct	St_GrP3MsgKey
{
	St_GrP3MsgHd	Hd;
	__u8	Dev;
	__u8	Key;
	BOOL8	IsPush;
}	*Ptr_GrP3MsgKey;

typedef	struct	St_GrP3MsgMouse
{
	St_GrP3MsgHd	Hd;
	__u16	X;
	__u16	Y;
	__u8	BtnMap;
	__u8	Atb;
}	*Ptr_GrP3MsgMouse;

typedef	struct	St_GrP3MsgWheel
{
	St_GrP3MsgHd	Hd;
	BOOL8	IsDown;
}	*Ptr_GrP3MsgWheel;

typedef	struct	St_GrP3MsgTimeTick
{
	St_GrP3MsgHd	Hd;
	__u32	Tick;
}	*Ptr_GrP3MsgTimeTick;

typedef	struct	St_GrP3MsgWinClose
{
	St_GrP3MsgHd	Hd;
	__u16	WinId;		// window id
}	*Ptr_GrP3MsgWinClose;

typedef	struct	St_GrP3MsgLayer
{
	St_GrP3MsgHd	Hd;
	__u8	Layer;
}	*Ptr_GrP3MsgLayer;

typedef	struct	St_GrP3MsgSysStat
{
	St_GrP3MsgHd	Hd;
	__u8	Type;			// change system type

}	*Ptr_GrP3MsgSysStat;

typedef	struct	St_GrP3MsgFrmReqPageChg
{
	St_GrP3MsgHd	Hd;
	__u16	PageId;		// page script id
	__u8	Layer;
	BOOL8	IsBack;		// go to back layer
	BOOL8	IsMovFcs;	// move focus
}	*Ptr_GrP3MsgFrmReqPageChg;

typedef	struct	St_GrP3MsgFrmPageChged
{
	St_GrP3MsgHd	Hd;
	__u16	FramdId;	// frame id
	__u16	PageId;		// display page script id
}	*Ptr_GrP3MsgFrmPageChged;

typedef	struct	St_GrP3MsgObj
{
	St_GrP3MsgHd	Hd;
	__u16	ObjId;	// object 
	__u16	_rsvd0;
}	*Ptr_GrP3MsgObj;


//====================================================================
//global function

//====================================================================
//out Environment conditional
#endif

