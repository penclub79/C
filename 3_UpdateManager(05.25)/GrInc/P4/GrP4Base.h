/*
	Gause Platform Base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4Base
#define	_EPC_GrP4Base

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

//====================================================================
//constance

#define E_GrP4UserIdMax							32

#define	E_GrP4VdoMaxChCnt						16		/* maximum video channel count */
#define	E_GrP4AdoMaxChCnt						16		/* maximum audio channel count */

#define	E_GrP4SysErrMaxCnt					32		/* system error max type count */

#define E_GrP4CtlgParaMax						6

#define E_GrP4CtlgIdNone						0		/* catalog id none */
#define E_GrP4CtlgIdFwUpgd					1		/* catalog id firmware upgrade */
//#define E_GrP4CtlgIdHoliday					2		/* catalog id holiday */
#define E_GrP4CtlgIdCfg							3		/* catalog id config file */
#define E_GrP4CtlgIdLog							4		/* catalog id event log */

#define E_GrP4GvsVarCnt							128

#define	E_GrP4RgstSysCnt						3

#define E_GrP4DoUserDataCnt					4

#define E_GrP4MngVmRparaMax					4

// basis
#define E_GrP4PosBasisSt						0				/* basis start */
#define E_GrP4PosBasisEd						1				/* basis end */
#define E_GrP4PosBasisCtSt					2				/* basis center start */
#define E_GrP4PosBasisCtEd					3				/* basis center end */

// direction 
#define E_GrP4Dir2dLeft							0
#define E_GrP4Dir2dUp								1
#define E_GrP4Dir2dRight						2
#define E_GrP4Dir2dDown							3

// layer
#define E_GrP4UiLayerDflt						1	/* default UI layer */

// console index
#define E_GrP4ConIdxAll							0xFE	/* all console */
#define E_GrP4ConIdxFocus						0xFF	/* focused index */

// window id
#define E_GrP4WinIdAll							0xFFFE				/* all layer */
#define E_GrP4WinIdNone							0xFFFF				/* none - command effective console only */

#define E_GrP4WinIdSlotBitPos				8
#define E_GrP4WinIdSlotBitMask			0x07
#define E_GrP4WinIdLayerBitPos			12
#define E_GrP4WinIdLayerBitMask			0x07

// popup style
#define	E_GrP4PopupStyleImmediate		0
#define	E_GrP4PopupStyleMove				1
#define	E_GrP4PopupStyleGrow				2

#define	E_GrP4PopupStyleCnt					3

// class id
#define E_GrP4DoClassNone						0		/* none */
#define E_GrP4DoClassBase						1		/* Cls_GrP4DoBase */
#define E_GrP4DoClassWin						2		/* Cls_GrP4DoWin */
#define E_GrP4DoClassFrame					3		/* Cls_GrP4DoFrame */
#define E_GrP4DoClassLabel					4		/* Cls_GrP4DoLbl */
#define E_GrP4DoClassPrgsBarLoc			5		/* Cls_GrP4DoPrgsBarLoc */
#define E_GrP4DoClassQuad						6		/* Cls_GrP4DoQuad */
#define E_GrP4DoClassBtnPage				7		/* Cls_GrP4DoBtnPage */
#define E_GrP4DoClassStatic					8		/* Cls_GrP4DoStatic */
#define E_GrP4DoClassEdtStpStr			9		/* Cls_GrP4DoEdtStpStr */
#define E_GrP4DoClassBtnLbl					10	/* Cls_GrP4DoBtnLbl */
#define E_GrP4DoClassSpinNumBase		11	/* Cls_GrP4DoSpinNumBase */
#define E_GrP4DoClassSpinNumStp			12	/* Cls_GrP4DoSpinNumStp */
#define E_GrP4DoClassSpinTxtBase		13	/* Cls_GrP4DoSpinTxtBase */
#define E_GrP4DoClassSpinTxtStp			14	/* Cls_GrP4DoSpinTxtStp */
#define E_GrP4DoClassRelStc					15	/* Cls_GrP4DoSRelStc */
#define E_GrP4DoClassSpinPageTxt		16	/* Cls_GrP4DoSpinPageTxt */
#define E_GrP4DoClassSpinDdgtBase		17	/* Cls_GrP4DoSpinDdgtBase */
#define E_GrP4DoClassSpinDdgtStp		18	/* Cls_GrP4DoSpinDdgtStp */
#define E_GrP4DoClassBtnStc					19	/* Cls_GrP4DoBtnStc */
#define E_GrP4DoClassChkbBase				20	/* Cls_GrP4DoChkbBase */
#define E_GrP4DoClassChkbSsb				21	/* Cls_GrP4DoChkbSsb */
#define E_GrP4DoClassChkbSsm				22	/* Cls_GrP4DoChkbSsm */
#define E_GrP4DoClassSbarVar				23	/* Cls_GrP4DoSbarVar */
#define E_GrP4DoClassSbarStp				24	/* Cls_GrP4DoSbarStp */
#define E_GrP4DoClassRbtnBase				25	/* Cls_GrP4DoRbtnBase */
#define E_GrP4DoClassRbtnStp				26	/* Cls_GrP4DoRbtnStp */
#define E_GrP4DoClassChkbSgb				27	/* Cls_GrP4DoChkbSgb */
#define E_GrP4DoClassChkbSgm				28	/* Cls_GrP4DoChkbSgm */
#define E_GrP4DoClassSpinPnt				29	/* Cls_GrP4DoSpinPnt */
#define E_GrP4DoClassSpinNumLoc			30	/* Cls_GrP4DoSpinNumLoc */
#define E_GrP4DoClassSpinDdgtLoc		31	/* Cls_GrP4DoSpinDdgtLoc */
#define E_GrP4DoClassSpinTxtLoc			32	/* Cls_GrP4DoSpinTxtLoc */
#define E_GrP4DoClassTimeCtrl				33	/* Cls_GrP4DoTimeCtrl */
#define E_GrP4DoClassCalBase				34	/* Cls_GrP4DoCalBase */
#define E_GrP4DoClassCalRec					35	/* Cls_GrP4DoCalRec */
#define E_GrP4DoClassSbarBase				36	/* Cls_GrP4DoSbarBase */
#define E_GrP4DoClassPrgsBarBase		37	/* Cls_GrP4DoPrgsBarBase */
#define E_GrP4DoClassPrgsBarGvs			38	/* Cls_GrP4DoPrgsBarGvs */
#define E_GrP4DoClassGridBase				39	/* Cls_GrP4DoGridBase */
#define E_GrP4DoClassGridRscStp			40	/* Cls_GrP4DoGridRscStp */
#define E_GrP4DoClassBtnRelStc			41	/* Cls_GrP4DoBtnRelStc */
#define E_GrP4DoClassGridRstat			42	/* Cls_GrP4DoGridRstat */
#define E_GrP4DoClassTimeLbl				43	/* Cls_GrP4DoTimeLbl */
#define E_GrP4DoClassLbxBase				44	/* Cls_GrP4DoLbxBase */
#define E_GrP4DoClassLbxCtlg				45	/* Cls_GrP4DoLbxCtlg */
#define E_GrP4DoClassSpinQuadStp		46	/* Cls_GrP4DoSpinQuadStp */
#define E_GrP4DoClassRbtnGvu				47	/* Cls_GrP4DoRbtnGvu */
#define E_GrP4DoClassRbtnSsm				48	/* Cls_GrP4DoRbtnSsm */
#define E_GrP4DoClassChkbLoc				49	/* Cls_GrP4DoChkbLoc */
#define E_GrP4DoClassEdtBase				50	/* Cls_GrP4DoEdtBase */
#define E_GrP4DoClassEdtLoc					51	/* Cls_GrP4DoEdtLoc */
#define E_GrP4DoClassSpinId					52	/* Cls_GrP4DoSpinId */
#define E_GrP4DoClassGridMtnStp			53	/* Cls_GrP4DoGridMtnStp */
#define E_GrP4DoClassCalHday				54	/* Cls_GrP4DoCalHday */
#define E_GrP4DoClassPnlPtz					55	/* Cls_GrP4DoPnlPtz */
#define E_GrP4DoClassSpinBtxtBase		56	/* Cls_GrP4DoSpinBtxtBase */
#define E_GrP4DoClassSpinBtxtStp		57	/* Cls_GrP4DoSpinTxtStp */
#define E_GrP4DoClassGridRrng				58	/* Cls_GrP4DoGridRrng */
#define E_GrP4DoClassSpinValBase		59	/* Cls_GrP4DoSpinValBase */
#define E_GrP4DoClassSpinValStp			60	/* Cls_GrP4DoSpinValStp */
#define E_GrP4DoClassGridPlayBar		61	/* Cls_GrP4DoGridPlayBar */
#define E_GrP4DoClassTreeBase				62	/* Cls_GrP4DoTreeBase */
#define E_GrP4DoClassTreeLtxt				63	/* Cls_GrP4DoTreeLtxt */
#define E_GrP4DoClassPlayBar				64	/* Cls_GrP4DoGridPlayBar */

#define E_GrP4DoClassCnt						65	/* class count */

// image
#define E_GrP4ImgIdNone							0
#define E_GrP4ImgIdBase							1
#define E_GrP4ImgUserIdStart				0x8000

// input device type
#define E_GrP4InDevFront						0		/* input device front */
#define E_GrP4InDevRemocon					1		/* input device remote controller */
#define E_GrP4InDevPtzKbd						2		/* input device PTZ keyboard */

// virtual keyboard
#define	E_GrP4VkeyTxtCnt						47	/* virtual keyboard text key count */
#define E_GrP4VkeyShftCnt						4		/* virtual keyboard shift page count */

#define E_GrP4VkeyLang0							100	/* language 0 key */

#define E_GrP4VkeyEnter							150	/* enter key */
#define E_GrP4VkeySpace							151	/* space key */
#define E_GrP4VkeyPagePlus					152	/* page plus key */
#define E_GrP4VkeyPageMinus					153	/* page minus key */
#define E_GrP4VkeyDelete						154	/* delete key */
#define E_GrP4VkeyBackSpace					155	/* back space key */
#define E_GrP4VkeyClear							156	/* clear key */
#define E_GrP4VkeyHome							157	/* home key */
#define E_GrP4VkeyEnd								158	/* end key */
#define E_GrP4VkeyCusLeft						159	/* cursor left key */
#define E_GrP4VkeyCusRight					160	/* cursor right key */

// HDD
#define E_GrP4HddMaxCnt							8		/* max HDD count */

// record
#define E_GrP4RecHolyMax						4		/* max holiday count */
#define E_GrP4RecWeekCnt						(7 + E_GrP4RecHolyMax)	/* record schedule week count */

#define E_GrP4VencQtyCnt						5		/* video encode quality count */

// -------------------------------------------
// key
#define E_GrP4KeyNone								0x00

#define E_GrP4KeyCh0								0x01
#define E_GrP4KeyCh1								0x02
#define E_GrP4KeyCh2								0x03
#define E_GrP4KeyCh3								0x04
#define E_GrP4KeyCh4								0x05
#define E_GrP4KeyCh5								0x06
#define E_GrP4KeyCh6								0x07
#define E_GrP4KeyCh7								0x08
#define E_GrP4KeyCh8								0x09
#define E_GrP4KeyCh9								0x0A
#define E_GrP4KeyCh10								0x0B
#define E_GrP4KeyCh11								0x0C
#define E_GrP4KeyCh12								0x0D
#define E_GrP4KeyCh13								0x0E
#define E_GrP4KeyCh14								0x0F
#define E_GrP4KeyCh15								0x10
#define E_GrP4KeyCh16								0x11

#define E_GrP4KeyEnter							0x12
#define E_GrP4KeyEsc								0x13
#define E_GrP4KeyDelete							0x14
#define E_GrP4KeyLock								0x15

#define E_GrP4KeyLeft								0x16
#define E_GrP4KeyUp									0x17
#define E_GrP4KeyRight							0x18
#define E_GrP4KeyDown								0x19

#define E_GrP4KeyJogLeft						0x1A			/* same + key */	
#define E_GrP4KeyValMinus						0x1A			/* same -key */	
#define E_GrP4KeyJogRight						0x1B			/* same -key */	
#define E_GrP4KeyValPlus						0x1B			/* same + key */	

#define E_GrP4KeyEject							0x1C

/* reserve 1C ~ 1F */

#define E_GrP4KeyShtlLeft						0x20			//low 3 bit is position
#define E_GrP4KeyShtlL0							0x20
#define E_GrP4KeyShtlL1							0x21
#define E_GrP4KeyShtlL2							0x22
#define E_GrP4KeyShtlL3							0x23
#define E_GrP4KeyShtlL4							0x24
#define E_GrP4KeyShtlL5							0x25
#define E_GrP4KeyShtlL6							0x26
#define E_GrP4KeyShtlL7							0x27
#define E_GrP4KeyShtlRight					0x28			//low 3 bit is position
#define E_GrP4KeyShtlR0							0x28
#define E_GrP4KeyShtlR1							0x29
#define E_GrP4KeyShtlR2							0x2A
#define E_GrP4KeyShtlR3							0x2B
#define E_GrP4KeyShtlR4							0x2C
#define E_GrP4KeyShtlR5							0x2D
#define E_GrP4KeyShtlR6							0x2E
#define E_GrP4KeyShtlR7							0x2F

#define E_GrP4KeyId									0x30		//low 2 bit is id number -macro

#define E_GrP4KeySelId							0x34		//remote control id selection button
#define E_GrP4KeyMenu								0x35
#define E_GrP4KeyBackup							0x36
#define E_GrP4KeyCapture						0x37		/* snap shot */
#define E_GrP4KeySearch							0x38
#define E_GrP4KeyDisplay						0x39		/* change console focus */
#define E_GrP4KeyPtzMode						0x3A
#define E_GrP4KeyEmergency					0x3B		/* toggle type emergency - record */
#define E_GrP4KeyQuad								0x3C		/* fixed split mode  */
#define E_GrP4KeySequence						0x3D		/* auto sequence */
#define E_GrP4KeyAudio							0x3E
#define E_GrP4KeySplit							0x3F		/* multi split mode */
#define E_GrP4KeyEmgcyOn						0x40		/* emergency on */
#define E_GrP4KeyEmgcyOff						0x41		/* emergency off */
#define E_GrP4KeyNextQuad						0x42		/* quad group next */
#define E_GrP4KeyPrevQuad						0x43		/* quad group previous */
#define E_GrP4KeyZoomMode						0x44		/* zoom mode */
#define	E_GrP4KeyPlayRw							0x45		/* play rewind */
#define	E_GrP4KeyPlayFw							0x46		/* play forward */
#define	E_GrP4KeyPlayPause					0x47		/* play pause */

/* reserved 49 ~ 4F */
#define E_GrP4KeyPtzTele						0x50
#define E_GrP4KeyPtzWide						0x51
#define E_GrP4KeyPtzFar							0x52
#define E_GrP4KeyPtzNear						0x53

/* reserved 54 ~ 54 */

#define E_GrP4KeyReboot							0x55
#define E_GrP4KeyZmIn								0x56
#define E_GrP4KeyZmOut							0x57
#define E_GrP4KeyFcIn								0x58
#define E_GrP4KeyFcOut							0x59
#define E_GrP4KeyPtzSpd							0x5A
/* reserved 5B */
#define E_GrP4KeyPtzTour						0x5C

/* reserved 5D ~ 5F */

#define E_GrP4KeyQdMode1						0x60
#define E_GrP4KeyQdMode4						0x61
#define E_GrP4KeyQdMode6						0x62
#define E_GrP4KeyQdMode8						0x63
#define E_GrP4KeyQdMode9						0x64
#define E_GrP4KeyQdMode10						0x65
#define E_GrP4KeyQdMode13						0x66
#define E_GrP4KeyQdMode16						0x67

/* reserved 68 ~ 6F */
#define E_GrP4KeyMnLive							0x70


#define E_GrP4KeyPower							0x7F		//power on / off
// key max count 0x80 = 128
// -------------------------------------------
#define	E_GrP4EvtKeyTypeDown				0
#define	E_GrP4EvtKeyTypeUp					1
#define	E_GrP4EvtKeyTypeLong				2

// -------------------------------------------
// mouse
// mouse button index
#define E_GrP4MouseBtnLeft					0x0000
#define E_GrP4MouseBtnRight					0x0001
#define E_GrP4MouseBtnMiddle				0x0002

// mouse button map
#define E_GrP4MouseBmkLeft					(1 << E_GrP4MouseBtnLeft)
#define E_GrP4MouseBmkRight					(1 << E_GrP4MouseBtnRight)
#define E_GrP4MouseBmkMiddle				(1 << E_GrP4MouseBtnMiddle)

// mouse message attribute
#define E_GrP4MouseAtbLbtnChg				(1 << E_GrP4MouseBtnLeft)
#define E_GrP4MouseAtbRbtnChg				(1 << E_GrP4MouseBtnRight)
#define E_GrP4MouseAtbMbtnChg				(1 << E_GrP4MouseBtnMiddle)
#define E_GrP4MouseAtbPosChg				(1 << 3)

// -------------------------------------------
// system status

#define E_GrP4SysStatQuadMode				0x01		/* quad mode change */
#define E_GrP4SysStatLangChg				0x02		/* language change */

#define E_GrP4SysStatSetupChg				0x10		/* setup data change */
#define E_GrP4SysStatLivePlay				0x11		/* live play mode change */
#define E_GrP4SysStatAccountChg			0x12		/* login account change */
#define E_GrP4SysStatVlossChg				0x13		/* video loss change */
#define E_GrP4SysStatTimeFmtChg			0x14		/* time format change */
#define E_GrP4SysStatRecEmgcChg			0x15		/* emergency status change */

// -------------------------------------------
// message
// manager message
#define E_GrP4MsgCmdMngMsgStart			0x0000

// console message
#define E_GrP4MsgCmdConMsgStart			0x0100

#define E_GrP4MsgCmdConWinClose			(0x00 + E_GrP4MsgCmdConMsgStart)		/* console window close : St_GrP4MsgWinClose */
#define E_GrP4MsgCmdConFocusCrt			(0x01 + E_GrP4MsgCmdConMsgStart)		/* console focus correct */
#define E_GrP4MsgCmdConChgConFcs		(0x02 + E_GrP4MsgCmdConMsgStart)		/* console focus change - dual monitor only - reach window */
#define E_GrP4MsgCmdConPaint				(0x03 + E_GrP4MsgCmdConMsgStart)		/* console paint : no parameter */

#define E_GrP4MsgCmdConInpKey				(0x04 + E_GrP4MsgCmdConMsgStart)		/* console input key : St_GrP4MsgKey */
#define E_GrP4MsgCmdConInpMouse			(0x05 + E_GrP4MsgCmdConMsgStart)		/* mouse status change : St_GrP4MsgMouse */
#define E_GrP4MsgCmdConInpWheel			(0x06 + E_GrP4MsgCmdConMsgStart)		/* mouse wheel change : St_GrP4MsgWheel */

#define E_GrP4MsgCmdConLayUpClose		(0x07 + E_GrP4MsgCmdConMsgStart)		/* console layer up close : St_GrP4MsgLayer */

// object common message
#define E_GrP4MsgCmdObjMsgStart			0x0200
#define E_GrP4MsgCmdKeyDown					(0x00 + E_GrP4MsgCmdObjMsgStart)		/* key : St_GrP4MsgKey */
#define E_GrP4MsgCmdKeyUp						(0x01 + E_GrP4MsgCmdObjMsgStart)		/* key : St_GrP4MsgKey */
#define E_GrP4MsgCmdMouseMove				(0x02 + E_GrP4MsgCmdObjMsgStart)		/* mouse move : St_GrP4MsgMouse */
#define E_GrP4MsgCmdMouseLbtnDown		(0x03 + E_GrP4MsgCmdObjMsgStart)		/* mouse lbtn down : St_GrP4MsgMouse */
#define E_GrP4MsgCmdMouseLbtnUp			(0x04 + E_GrP4MsgCmdObjMsgStart)		/* mouse lbtn up : St_GrP4MsgMouse */
#define E_GrP4MsgCmdMouseRbtnDown		(0x05 + E_GrP4MsgCmdObjMsgStart)		/* mouse rbtn down : St_GrP4MsgMouse */
#define E_GrP4MsgCmdMouseRbtnUp			(0x06 + E_GrP4MsgCmdObjMsgStart)		/* mouse rbtn up : St_GrP4MsgMouse */
#define E_GrP4MsgCmdMouseMbtnDown		(0x07 + E_GrP4MsgCmdObjMsgStart)		/* mouse mbtn down : St_GrP4MsgMouse */
#define E_GrP4MsgCmdMouseMbtnUp			(0x08 + E_GrP4MsgCmdObjMsgStart)		/* mouse mbtn up : St_GrP4MsgMouse */
#define E_GrP4MsgCmdWheel						(0x09 + E_GrP4MsgCmdObjMsgStart)		/* mouse wheel change */

#define E_GrP4MsgCmdSysStat					(0x0A + E_GrP4MsgCmdObjMsgStart)		/* system status change : St_GrP4MsgSysStat */
//#define E_GrP4MsgCmdScptDecCpl			(0x0B + E_GrP4MsgCmdObjMsgStart)		/* ui script decode completed */

#define E_GrP4MsgCmdFocusGet				(0x10 + E_GrP4MsgCmdObjMsgStart)		/* object focus get */
#define E_GrP4MsgCmdFocusLoss				(0x11 + E_GrP4MsgCmdObjMsgStart)		/* object focus loss */
#define E_GrP4MsgCmdMouseEnter			(0x12 + E_GrP4MsgCmdObjMsgStart)		/* St_GrP4MsgHd	*/
#define E_GrP4MsgCmdMouseLeave			(0x13 + E_GrP4MsgCmdObjMsgStart)		/* St_GrP4MsgHd	*/
#define E_GrP4MsgCmdTickPeriod			(0x14 + E_GrP4MsgCmdObjMsgStart)		/* St_GrP4EvtTick */	

#define E_GrP4MsgCmdVkbdEnd					(0x20 + E_GrP4MsgCmdObjMsgStart)		/* virtual keyboard edit end St_GrP4MsgHd */	

// window message
#define E_GrP4MsgCmdWinMsgStart			0x0300

// frame message
#define E_GrP4MsgCmdFrmMsgStart			0x0400
#define E_GrP4MsgCmdFrameReqPageChg	(0x00 + E_GrP4MsgCmdFrmMsgStart)		/* request page change : St_GrP4MsgFrmReqPageChg */
#define E_GrP4MsgCmdFramePageChged	(0x01 + E_GrP4MsgCmdFrmMsgStart)		/* page changed : St_GrP4MsgFrmPageChged */

// button message
#define E_GrP4MsgCmdBtnMsgStart			0x0500
#define E_GrP4MsgCmdBtnDown					(0x00 + E_GrP4MsgCmdBtnMsgStart)		/* button down : St_GrP4MsgObj */
#define E_GrP4MsgCmdBtnUp						(0x01 + E_GrP4MsgCmdBtnMsgStart)		/* button up : St_GrP4MsgObj */
#define E_GrP4MsgCmdBtnChkChg				(0x02 + E_GrP4MsgCmdBtnMsgStart)		/* check value change : St_GrP4MsgObj */

// spin control message
#define E_GrP4MsgCmdSpinMsgStart		0x0600
#define E_GrP4MsgCmdSpinValChg			(0x00 + E_GrP4MsgCmdSpinMsgStart)		/* spin control value change : St_GrP4MsgObj */

// slider bar message		
#define E_GrP4MsgCmdSbarMsgStart		0x0700
#define E_GrP4MsgCmdSbarValChg			(0x00 + E_GrP4MsgCmdSbarMsgStart)		/* slider bar value change : St_GrP4MsgObj */

// edit message
#define E_GrP4MsgCmdEditMsgStart		0x0800
#define E_GrP4MsgCmdEditValChg			(0x00 + E_GrP4MsgCmdEditMsgStart)		/* edit value change : St_GrP4MsgObj */

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrP4RgstSys
{
	BOOL8		IsPanic;
	__u8		LiveQdMode;
	__u8		LiveQdPage;
}	*Ptr_GrP4RgstSys;

typedef	struct	St_GrP4Gvs
{
	__u32		HwVer;			// hardware version	
	__u32		ScptTime;		// script time
	__u8		VdoMaxCh;		// video max channel
	__u8		AdoMaxCh;		// audio max channel
	__u8		DiChCnt;		// sensor channel count
	__u8		DoChCnt;		// alarm channel count

	BOOL8		IsEdio;			// expand digital input output found
	__u8		PrgsInit;		// initialize progress
	BOOL8		IsPlayMode;	// main play mode
	__u8		_rsvd0;

	__u8		LoginIdx;		// login index - 0xFF = log off
	__u8		PrgsFwUp;		// firmware upraged progress;
	__u16		VlossMap;		// video loss map

	__u16		DiStat;			// sensor status
	__u16		DoStat;			// alarm status

	__u8		HddMaxCnt;	// HDD max count
	BOOL8		IsEmgc;			// emergency status
	__u16		LoginPriv;	// login privilege

	BOOL8		IsAtSeq;		// auto sequence on
	__u8		IsLanOn;		// LAN cable connected
	__u8		PlayStat;		// main play status
	BOOL8		IsPlayBack;	// play direction

	__u16		CovertMap;
	BOOL8		IsRmtTmpAble;	// REMOCON temporary able
	__u8		RunRmtId;		// running REMOCON ID

	__u32		NetVer;			// network version

	__u8		FwUpErr;		// firmware upgrade error code
	__u8		UsbMemCnt;	// USB memory count
	__u8		CmsCliCnt;	// CMS client count
	__u8		BkupErrCode;	// backup error code

	__u32		NetModel;		// network model
	__s32		PlaySpd;		// play speed

	__u32		ErrMap;

	__u8		PtzCtrlMode;
	BOOL8		IsPtzCtrlLock;
	__u8		PtzCtrlCh;
	__u8		IsOddExist;	// ODD exist

	__u8		MenuMode;			// menu mode
	BOOL8		IsLoginMenu;	// login menu mode
	BOOL8		IsNtpOk;			// NTP test result
	__u8		LogRecDev;		// log backup record device

	__u32		LogChMap;			// selected log channel map

	__u32		LogTimeSt;		// log backup start time
	__u32		LogTimeEd;		// log backup end time
	__u32		LogTotal;			// log total count
	__u32		LogPrgs;			// log progress

	__u32		V4Ip;
	__u32		V4NetMask;
	__u32		V4GateWay;

	__u8		AdoLiveCh;
	__u8		AdoPlayCh;
	__u8		BkupRecDev;
	__u8		BkupMedia;

	__u8		AdoLiveVol[E_GrP4VdoMaxChCnt];
	__u8		AdoPlayVol[E_GrP4AdoMaxChCnt];

	__u32		BkupTimeSt;
	__u32		BkupTimeEd;

	__u16		BkupChMap;
	BOOL8		IsBkupCmbViewer;
	__u8		CrtRmtId;
	__u32		BkupTimePrgs;

	__u32		LastErrTime[E_GrP4SysErrMaxCnt];

	__u8		LogErrCode;			// log backup error code
	__u8		MsgBoxMode;			// message box mode
	__u16		PtzTourMsk;			//ptz touring mode bitmask

	__u8		RunDispRes;			// now running display resolution
	__u8		_rsvd1;
	__u16		_rsvd2;

	__u32		DipSw;				// dip switch status
	__u32		LogEvtMap;		// selected log event group map

}	*Ptr_GrP4Gvs;

// para patch
typedef struct St_GrP4MngVmRpIo
{
	__u32	Cnt;		// count
	__u8	Ridx[E_GrP4MngVmRparaMax];
	__s32		Rval[E_GrP4MngVmRparaMax];
}	*Ptr_GrP4MngVmRpIo;

typedef	struct St_GrP4MngVmRpara
{
	St_GrP4MngVmRpIo	In;
	St_GrP4MngVmRpIo	Out;
}	*Ptr_GrP4MngVmRpara;

// UI message header
typedef	struct St_GrP4EvtCallMsg
{
	__u16	CodeId;
	__u8	ConIdx;
	__u16	WinId;
	__u16	ScptId;
	St_GrP4MngVmRpara	Para;
}	*Ptr_GrP4EvtCallMsg;

typedef	struct St_GrP4MsgHd
{
	__u16		Cmd;			// message command
	__u8		ConIdx;		// console index
	__u8		_rsvd0;		// reserved
	__u16		WinId;		// window id
	__u16		ObjId;		// object script id
}	*Ptr_GrP4MsgHd;

typedef	struct	St_GrP4MsgKey
{
	St_GrP4MsgHd	Hd;
	__u8	Dev;
	__u8	Key;
	BOOL8	IsPush;
}	*Ptr_GrP4MsgKey;

typedef	struct	St_GrP4MsgMouse
{
	St_GrP4MsgHd	Hd;
	__u16	X;
	__u16	Y;
	__u8	BtnMap;
	__u8	Atb;
}	*Ptr_GrP4MsgMouse;

typedef	struct	St_GrP4MsgWheel
{
	St_GrP4MsgHd	Hd;
	BOOL8	IsDown;
}	*Ptr_GrP4MsgWheel;

typedef	struct	St_GrP4MsgTimeTick
{
	St_GrP4MsgHd	Hd;
	__u32	Tick;
}	*Ptr_GrP4MsgTimeTick;

typedef	struct	St_GrP4MsgWinClose
{
	St_GrP4MsgHd	Hd;
	__u16	WinId;		// window id
}	*Ptr_GrP4MsgWinClose;

typedef	struct	St_GrP4MsgLayer
{
	St_GrP4MsgHd	Hd;
	__u8	Layer;
}	*Ptr_GrP4MsgLayer;

typedef	struct	St_GrP4MsgSysStat
{
	St_GrP4MsgHd	Hd;
	__u8	Type;			// change system type

}	*Ptr_GrP4MsgSysStat;

typedef	struct	St_GrP4MsgFrmReqPageChg
{
	St_GrP4MsgHd	Hd;
	__u16	PageId;		// page script id
	__u8	Layer;
	BOOL8	IsBack;		// go to back layer
	BOOL8	IsMovFcs;	// move focus
}	*Ptr_GrP4MsgFrmReqPageChg;

typedef	struct	St_GrP4MsgFrmPageChged
{
	St_GrP4MsgHd	Hd;
	__u16	FramdId;	// frame id
	__u16	PageId;		// display page script id
}	*Ptr_GrP4MsgFrmPageChged;

typedef	struct	St_GrP4MsgObj
{
	St_GrP4MsgHd	Hd;
	__u16	ObjId;	// object 
	__u16	_rsvd0;
}	*Ptr_GrP4MsgObj;


//====================================================================
//global function

//====================================================================
//out Environment conditional
#endif

