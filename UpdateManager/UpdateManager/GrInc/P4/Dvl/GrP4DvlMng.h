/*
	Gause Platform develop manager

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4DvlMng
#define	_EPC_GrP4DvlMng

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P4/GrP4Mng.h>
#include	<P4/Dvl/GrP4DvlBase.h>
#include	<P4/GrP4AsmBase.h>
#include	<P4/GrP4Script.h>
#include	<P4/GrP4DoBase.h>
#include	<P4/GrP4Font.h>
#include	<GrFileTool.h>
#include	<GrFileCtrl.h>
#include	<GrProcess.h>

#include <Dvr/PtzV2/GrPtzV2Mng.h>

//====================================================================
//constance

#define E_GrP4DvlWinMaxCnt					128			/* window max count */
#define E_GrP4DvlPageMaxCnt					256			/* page max count */
#define	E_GrP4DvlPstMaxCnt					1024		/* paint script count */
#define E_GrP4DvlAsmMaxCnt					512			/* assembly function max count */

#define	E_GrP4DvlBrkpntMaxCnt				32			/* max data break point count */

#define E_GrP4DvlLumpSlvMax					256			

#define E_GrP4DvlScptBufMax					0x8000	/* UI script buffer max size */
#define E_GrP4DvlPstCodeMax					64
#define E_GrP4DvlPstBufMax					(sizeof(St_GrP4ScptPstData) * E_GrP4DvlPstCodeMax)

#define E_GrP4DvlTextSysCnt					128			/* system text count */
#define E_GrP4DvlTextMaxLen					511			/* 511 character */
#define E_GrP4DvlTextBufSize				((E_GrP4DvlTextMaxLen +1) *2)

#define E_GrP4DvlSetupUsrMaxCnt			1024

#define E_GrP4DvlSetupHeaderMax			0x20000	/* import export file max size */

#define E_GrP4DvlAsmBufMax					(E_GrP4AsmOpUnitSize * E_GrP4DvlAsmCodeMax)
//#define E_GrP4DvlAsmRmkBufSize			4096

#define E_GrP4DvlDfltLayer					1				/* window default layer */
#define E_GrP4DvlDfltWinLeft				10
#define E_GrP4DvlDfltWinTop					10
#define E_GrP4DvlDfltWinRight				300
#define E_GrP4DvlDfltWinBottom			300

#define	E_GrP4DvlDfltThumbFcc				E_GrFccRGB24		/* default image RGB15 */

#define	E_GrP4DvlImgThumbCchCnt			64	/* image thumbnail cache count */

#define E_GrP4DvlStpDefNameMin			12

#define E_GrP4DvlStpPsTypeReady			0		/* wait sub text pars */
#define E_GrP4DvlStpPsTypeText			1		/* sub text pars text */
#define E_GrP4DvlStpPsTypeNumZero		2		/* sub text pars number zero */
#define E_GrP4DvlStpPsTypeNumHpx		3		/* sub text pars number hex prefix */
#define E_GrP4DvlStpPsTypeNumDec		4		/* sub text pars number decimal */
#define E_GrP4DvlStpPsTypeNumHex		5		/* sub text pars number hexadecimal */
#define E_GrP4DvlStpPsTypeRemPref		6		/* sub text pars remark prefix */
#define E_GrP4DvlStpPsTypeRemLine		7		/* sub text pars remark line */
#define E_GrP4DvlStpPsTypeRemBlk		8		/* sub text pars remark block */
#define E_GrP4DvlStpPsTypeDirect		9		/* sub text pars number directive */
#define E_GrP4DvlStpPsTypeUnknown		10		/* sub text pars unknown */
#define E_GrP4DvlStpPsTypeLineEnd		11	/* line end */

#define E_GrP4DvlStpPsModeReady			0		/* wait parsing mode */
#define E_GrP4DvlStpPsModeDefine		1		/* directive define scan */
#define E_GrP4DvlStpPsModeName			2		/* scan name */
#define E_GrP4DvlStpPsModeAddr			3		/* scan address */
#define E_GrP4DvlStpPsModeDescChk		4		/* descriptor check */
#define E_GrP4DvlStpPsModeDescLine	5		/* description line */
#define E_GrP4DvlStpPsModeDescBlk		6		/* description block */
#define E_GrP4DvlStpPsModeRemLine		7		/* remark line */
#define E_GrP4DvlStpPsModeRemBlk		8		/* remark block */
#define E_GrP4DvlStpPsModeLineEnd		9		/* line end */

#define E_GrP4DvlMdlDbgClstCnt			7

#define E_GrP4DvlDfltObjSizeX				50
#define E_GrP4DvlDfltObjSizeY				20

#define E_GrP4DvlModelThis					0xFF	/* this model */

#define E_GrP4DvlDbgModeStop						0		/* debug stop */
#define E_GrP4DvlDbgModeRun							1		/* debug run */
#define E_GrP4DvlDbgModePause						2		/* debug pause */
#define E_GrP4DvlDbgModeStep						3		/* debug step */

#define E_GrP4DvlDbgRunLimit						3000	/* command running limit */

// message command
#define E_GrP4DvlMcmdPrjNew							1		/* project new : St_GrP4DvlMsgPrjNew */
#define E_GrP4DvlMcmdPrjClose						2		/* project close : St_GrP4DvlMsgBase */
#define E_GrP4DvlMcmdPrjSave						3		/* project save : St_GrP4DvlMsgPrjSave */
#define E_GrP4DvlMcmdPrjLoad						4		/* project load : St_GrP4DvlMsgPrjLoad */
//#define E_GrP4DvlMcmdScptExport					5		/* script export : St_GrP4DvlMsgScptExport */
#define E_GrP4DvlMcmdWindowNew					6		/* window new : St_GrP4DvlMsgWindowNew */
#define E_GrP4DvlMcmdPageNew						7		/* page new : St_GrP4DvlMsgPageNew */
#define E_GrP4DvlMcmdWindowDel					8		/* delete window : St_GrP4DvlMsgWindowDel */
#define E_GrP4DvlMcmdPageDel						9		/* delete page : St_GrP4DvlMsgPageDel */
#define E_GrP4DvlMcmdScriptDec					10	/* Script decode : St_GrP4DvlMsgScriptDec */
#define E_GrP4DvlMcmdEditSkinSet				11	/* edit skin set : St_GrP4DvlMsgEditSkinSet */
#define E_GrP4DvlMcmdEditModelIdxSet		12	/* edit model index set : St_GrP4DvlMsgEditModelIdxSet */
#define E_GrP4DvlMcmdEditObjCreate			13	/* page new : St_GrP4DvlMsgEditObjCreate */
#define E_GrP4DvlMcmdEditLumpSet				16	/* edit lump set : St_GrP4DvlMsgEditLumpSet */
#define E_GrP4DvlMcmdEditObjClassIdGet	21	/* edit object class id get : St_GrP4DvlMsgEditObjClassIdGet */
#define E_GrP4DvlMcmdEditObjScptDump		22	/* edit object script dump : St_GrP4DvlMsgEditObjScptDump */
#define E_GrP4DvlMcmdEditObjFrameGet		23	/* edit object frame get : St_GrP4DvlMsgEditObjFrameGet */
#define E_GrP4DvlMcmdEditObjRedraw			24	/* edit object redraw : St_GrP4DvlMsgEditObjRedraw */
#define E_GrP4DvlMcmdEditObjListByPos		25	/* edit object list by position : St_GrP4DvlMsgEditObjListByPos */
#define E_GrP4DvlMcmdEditObjSlaveObjGet	26	/* edit object slave obj get : St_GrP4DvlMsgEditObjSlaveObjGet */
#define E_GrP4DvlMcmdEditObjPrntObjGet	28	/* edit object parent obj get : St_GrP4DvlMsgEditObjPrntObjGet */
#define E_GrP4DvlMcmdEditObjIsWindow		29	/* edit object check type window : St_GrP4DvlMsgEditObjIsWindow */
#define E_GrP4DvlMcmdEditObjDelete			30	/* edit object delete : St_GrP4DvlMsgEditObjDelete */
#define E_GrP4DvlMcmdEditObjParaInfoDump	31	/* edit object parameter info dump : St_GrP4DvlMsgEditObjParaInfoDump */
#define E_GrP4DvlMcmdEditObjParaUpdt		32	/* edit object parameter info dump : St_GrP4DvlMsgEditObjParaUpdt */

#define E_GrP4DvlMcmdEditObjInfoT1			33	/* edit object info type 1 : St_GrP4DvlMsgEditObjInfoT1 */

#define E_GrP4DvlMcmdDbgStart						40	/* debug start : St_GrP4DvlMsgBase */
#define E_GrP4DvlMcmdDbgFinish					41	/* debug finish : St_GrP4DvlMsgBase */
#define E_GrP4DvlMcmdIsDbgPause					42	/* check debug pause : St_GrP4DvlMsgIsDbgPause */
#define E_GrP4DvlMcmdDbgVmCsGet					43	/* get VM cs : St_GrP4DvlMsgDbgVmCsGet */
#define E_GrP4DvlMcmdDbgVmIpGet					44	/* get VM ip : St_GrP4DvlMsgDbgVmIpGet */


//====================================================================
//global macro

//====================================================================
//global type

typedef	struct St_GrP4DvlObjInfo
{
	__u16	Id;
	St_GrRect	Rc;
}	*Ptr_GrP4DvlObjInfo;

typedef struct St_GrP4DvlImgTmbCch
{
	__u32		Tick;	// last access tick
	__u16		ImgId;
	Cls_GrGdib*	Gdib;
	BITMAPV4HEADER	Bmhd;	// bitmap header
}	*Ptr_GrP4DvlImgTmbCch;

typedef	struct St_GrP4DvlMsgBase 
{
	St_GrTaskMsgHd	MsgHd;
	__u32	Cmd;
}	*Ptr_GrP4DvlMsgBase;

typedef struct St_GrP4DvlMsgPrjNew
{
	St_GrP4DvlMsgBase	Hd;
	BOOL8*	PtrRslt;
	__u32 FccPx;
	__u32 Res1X;
	__u32 Res1Y;
	__u32 Res2X;
	__u32 Res2Y;
	BOOL8 IsDual;
	BOOL8 IsInputDual;

}	*Ptr_GrP4DvlMsgPrjNew;

typedef struct St_GrP4DvlMsgPrjSave
{
	St_GrP4DvlMsgBase	Hd;
	BOOL8*	PtrRslt;
	WCHAR*	StrFn;
}	*Ptr_GrP4DvlMsgPrjSave;

typedef struct St_GrP4DvlMsgPrjLoad
{
	St_GrP4DvlMsgBase	Hd;
	BOOL8*	PtrRslt;
	WCHAR*	StrFn;
}	*Ptr_GrP4DvlMsgPrjLoad;

/*
typedef	struct St_GrP4DvlMsgScptExport
{
	St_GrP4DvlMsgBase	Hd;
	BOOL8*	PtrRslt;
	WCHAR*	StrFn;
	__u8		Model;
}	*Ptr_GrP4DvlMsgScptExport;
*/

typedef	struct St_GrP4DvlMsgEditLumpSet
{
	St_GrP4DvlMsgBase	Hd;
	BOOL8*	PtrRslt;
	__u16	ScptId;
}	*Ptr_GrP4DvlMsgEditLumpSet;

typedef	struct St_GrP4DvlMsgEditLumpGet
{
	St_GrP4DvlMsgBase	Hd;
	__u16*	PtrRslt;
}	*Ptr_GrP4DvlMsgEditLumpGet;

typedef struct St_GrP4DvlMsgWindowNew
{
	St_GrP4DvlMsgBase	Hd;
	__u16*	PtrRslt;
}	*Ptr_GrP4DvlMsgWindowNew;

typedef struct St_GrP4DvlMsgPageNew
{
	St_GrP4DvlMsgBase	Hd;
	__u16*	PtrRslt;
}	*Ptr_GrP4DvlMsgPageNew;

typedef struct St_GrP4DvlMsgEditObjCreate
{
	St_GrP4DvlMsgBase	Hd;
	__u16*	PtrRslt;
	__s32	ScrX;
	__s32	ScrY;
	__u8	ClassId;
}	*Ptr_GrP4DvlMsgEditObjCreate;

typedef struct St_GrP4DvlMsgWindowDel
{
	St_GrP4DvlMsgBase	Hd;
	BOOL8*	PtrRslt;
	__u16	ScptId;
}	*Ptr_GrP4DvlMsgWindowDel;

typedef struct St_GrP4DvlMsgPageDel
{
	St_GrP4DvlMsgBase	Hd;
	BOOL8*	PtrRslt;
	__u16	ScptId;
}	*Ptr_GrP4DvlMsgPageDel;

typedef struct St_GrP4DvlMsgScriptDec
{
	St_GrP4DvlMsgBase	Hd;
	__u32*	PtrRslt;
	__u16	PrntId;
	void*	PtrScpt;
}	*Ptr_GrP4DvlMsgScriptDec;

typedef struct St_GrP4DvlMsgEditSkinSet
{
	St_GrP4DvlMsgBase	Hd;
	__u8	SkinIdx;
}	*Ptr_GrP4DvlMsgEditSkinSet;

typedef struct St_GrP4DvlMsgEditModelIdxSet
{
	St_GrP4DvlMsgBase	Hd;
	__u8	ModelIdx;
}	*Ptr_GrP4DvlMsgEditModelIdxSet;

typedef struct St_GrP4DvlMsgEditObjScrRectGet
{
	St_GrP4DvlMsgBase	Hd;
	BOOL8*	PtrRslt;
	__u16 ScptId;
	Ptr_GrRect PtrRtRect;
	BOOL8 IsNoChk;
}	*Ptr_GrP4DvlMsgEditObjScrRectGet;

typedef struct St_GrP4DvlMsgEditObjRectSet
{
	St_GrP4DvlMsgBase	Hd;
	BOOL8*	PtrRslt;
	__u16 ScptId;
	Ptr_GrRect PtrRect;
}	*Ptr_GrP4DvlMsgEditObjRectSet;

typedef struct St_GrP4DvlMsgEditObjRectGet
{
	St_GrP4DvlMsgBase	Hd;
	BOOL8*	PtrRslt;
	__u16 ScptId;
	Ptr_GrRect PtrRtRect;
}	*Ptr_GrP4DvlMsgEditObjRectGet;

typedef struct St_GrP4DvlMsgEditObjFindByPos
{
	St_GrP4DvlMsgBase	Hd;
	__u16*	PtrRslt;
	__u16 PosX;
	__u16 PosY;
}	*Ptr_GrP4DvlMsgEditObjFindByPos;

typedef struct St_GrP4DvlMsgEditObjClassIdGet
{
	St_GrP4DvlMsgBase	Hd;
	__u8*	PtrRslt;
	__u16 ScptId;
}	*Ptr_GrP4DvlMsgEditObjClassIdGet;

typedef struct St_GrP4DvlMsgEditObjScptDump
{
	St_GrP4DvlMsgBase	Hd;
	__u32*	PtrRslt;
	__u16 ScptId;
	void* PtrBuf;
	BOOL8 IsCopyMode;
}	*Ptr_GrP4DvlMsgEditObjScptDump;

typedef struct St_GrP4DvlMsgEditObjFrameGet
{
	St_GrP4DvlMsgBase	Hd;
	__u16*	PtrRslt;
	__u16 ScptId;
}	*Ptr_GrP4DvlMsgEditObjFrameGet;

typedef struct St_GrP4DvlMsgEditObjRedraw 
{
	St_GrP4DvlMsgBase	Hd;
	__u16 ScptId;
}	*Ptr_GrP4DvlMsgEditObjRedraw;

typedef struct St_GrP4DvlMsgEditObjListByPos
{
	St_GrP4DvlMsgBase	Hd;
	__u32*	PtrRslt;
	__s32	X;
	__s32	Y;
	__u16* PtrBuf;
} *Ptr_GrP4DvlMsgEditObjListByPos;

typedef struct St_GrP4DvlMsgEditObjSlaveObjGet
{
	St_GrP4DvlMsgBase	Hd;
	__u16*	PtrRslt;
	__u16 ScptId;
}	*Ptr_GrP4DvlMsgEditObjSlaveObjGet;

typedef struct St_GrP4DvlMsgEditObjNextObjGet
{
	St_GrP4DvlMsgBase	Hd;
	__u16*	PtrRslt;
	__u16 ScptId;
}	*Ptr_GrP4DvlMsgEditObjNextObjGet;

typedef struct St_GrP4DvlMsgEditObjPrntObjGet
{
	St_GrP4DvlMsgBase	Hd;
	__u16*	PtrRslt;
	__u16 ScptId;
}	*Ptr_GrP4DvlMsgEditObjPrntObjGet;

typedef struct St_GrP4DvlMsgEditObjIsWindow
{
	St_GrP4DvlMsgBase	Hd;
	BOOL8*	PtrRslt;
	__u16 ScptId;
}	*Ptr_GrP4DvlMsgEditObjIsWindow;

typedef struct St_GrP4DvlMsgEditObjDelete
{
	St_GrP4DvlMsgBase	Hd;
	__u16 ScptId;
}	*Ptr_GrP4DvlMsgEditObjDelete;

typedef struct St_GrP4DvlMsgEditObjParaInfoDump
{
	St_GrP4DvlMsgBase	Hd;
	__u32*	PtrRslt;
	__u16		ScptId;
	void*		PtrBuf;
}	*Ptr_GrP4DvlMsgEditObjParaInfoDump;

typedef struct St_GrP4DvlMsgEditObjParaUpdt
{
	St_GrP4DvlMsgBase	Hd;
	__u16	ScptId;
	void*	PtrScpt;
	BOOL8	IsNoCopy;
}	*Ptr_GrP4DvlMsgEditObjParaUpdt;

typedef struct St_GrP4DvlMsgIsDbgPause
{
	St_GrP4DvlMsgBase	Hd;
	BOOL8*	PtrRslt;
}	*Ptr_GrP4DvlMsgIsDbgPause;

typedef struct St_GrP4DvlMsgDbgVmCsGet
{
	St_GrP4DvlMsgBase	Hd;
	__u16*	PtrRslt;
}	*Ptr_GrP4DvlMsgDbgVmCsGet;

typedef struct St_GrP4DvlMsgDbgVmIpGet
{
	St_GrP4DvlMsgBase	Hd;
	__u16*	PtrRslt;
}	*Ptr_GrP4DvlMsgDbgVmIpGet;

typedef struct St_GrP4DvlMsgEditObjInfoT1
{
	St_GrP4DvlMsgBase	Hd;
	BOOL8*	PtrRslt;
	__u16	ScptId;
	__u8*	PtrRtClassType;
	Ptr_GrRect	PtrRtRect;
	__u16*	PtrRtSlaveId;
	__u16*	PtrRtNextId;
}	*Ptr_GrP4DvlMsgEditObjInfoT1;

//====================================================================
//global class
class	Cls_GrP4DvlMng	:	public	Cls_GrP4Mng
{
	protected:
		// DVL msg
		__u32		m_DvlMsgAloc;

		// project
		BOOL8	m_DvlIsPrjExist;

		// tick id
		__u32		m_DvlTickId;

		// project header
		St_GrP4PrjCfg				m_DvlPrjConfig;
		St_GrP4PrjRtl				m_DvlPrjRtl;				// runtime data
		St_GrP4PrjModelCfg	m_DvlPrjModel[E_GrP4DvlModelMaxCnt];	
		St_GrP4PrjSkinCfg		m_DvlPrjSkin[E_GrP4DvlSkinMaxCnt];
		St_GrP4ScptCfgMdl		m_DvlPrjMdlAtb[E_GrP4DvlModelMaxCnt];		// mode attribute

		// script
		St_GrP4ScptCfgPrj		m_DvlScptCfgPrj;		// script config project
		St_GrP4ScptCfgRtl		m_DvlScptCfgRtl;		// script config runtime

		// runtime
		St_GrP4PrjFoldInfo	m_DvlFoldTbl[E_GrP4DvlFolderMaxCnt];		// tree table

		St_GrP4ScptWinInfo	m_DvlScptWinTbl[E_GrP4DvlWinMaxCnt];	// window table
		St_GrP4PrjWinInfo		m_DvlWinInfo[E_GrP4DvlWinMaxCnt];			// window info

		St_GrP4ScptPageInfo	m_DvlScptPageTbl[E_GrP4DvlPageMaxCnt];	// page table
		St_GrP4PrjPageInfo	m_DvlPageInfo[E_GrP4DvlPageMaxCnt];		// page info

		St_GrP4ScptPstItm		m_DvlScptPstTbl[E_GrP4DvlSkinMaxCnt][E_GrP4DvlPstMaxCnt];	// paint script table
		St_GrP4PrjPstInfo		m_DvlPstInfo[E_GrP4DvlPstMaxCnt];

		Ptr_GrGdib					m_DvlScptImgTbl[E_GrP4DvlSkinMaxCnt][E_GrP4MngImgScptMaxCnt];	// image table

		Ptr_GrP4FontHead		m_DvlScptFontTbl[E_GrP4DvlSkinMaxCnt][E_GrP4MngFontMaxCnt];		// font script table

		St_GrP4ScptTxtItm		m_DvlScptTxtTbl[E_GrP4MngTextScptMaxCnt];		// text

		__u32	m_DvlStpUsrCnt;		// setup user info count
		St_GrP4PrjVarInfo		m_DvlStpUsrTbl[E_GrP4DvlSetupUsrMaxCnt];

		St_GrP4PrjGvuInfo		m_DvlGvuTbl[E_GrP4MngGvuMaxCnt];		// global variable for user

		St_GrP4PrjUrgstInfo	m_DvlUrgstTbl[E_GrP4MngRgstUserCnt];	// user registry

		St_GrP4ScptAsmInfo	m_DvlScptAsmTbl[E_GrP4DvlAsmMaxCnt];	// assembly
		St_GrP4PrjAsmInfo		m_DvlAsmInfo[E_GrP4DvlAsmMaxCnt];
		St_GrP4PrjAsmCodeRmk	m_DvlAsmRmkTbl[E_GrP4DvlAsmMaxCnt];
		St_GrP4PrjAsmFncCmt		m_DvlAsmFncCmt[E_GrP4DvlAsmMaxCnt];			// function comment
		

		St_GrP4PrjDbgInfo		m_DvlMdlDblTbl[E_GrP4DvlModelMaxCnt];	// model debug table

		Cls_GrP4DoWin*	m_DvlEdtLump;		// edit lump

		__u32	m_DvlSlvMap[E_GrP4DvlSlaveMapCnt];

		St_GrP4DvlObjInfo	m_DvlEdtIdDispTbl[E_GrP4DvlLumpSlvMax];
		__u32	m_DvlEdtIdDispCnt;

		// setup
		//St_GrP4PrjSetBase	m_DvlSetBase;	// setup basic environment

		// build
		St_GrP4PrjBldFile	m_DvlBldInfo[E_GrP4DvlModelMaxCnt];	// build info

		// project read write
		Cls_GrFileCtrl*	m_DvlFc;
		__u32	m_DvlSaved;
		__u32	m_DvlSaveReq;
		BOOL8	m_DvlIsLoading;

		// setup header pars
		char*	m_DvlPtrStpFl;			// setup file buffer
		__u32	m_DvlStpFlSize;			// header size
		__u32	m_DvlStpPsPrs;			// parsing progress
		__u32	m_DvlStpWordLen;		// parsing word size
		char*	m_DvlPtrStpWord;		// found word pointer
		__u8	m_DvlStpPsType;			// type parsing
		__u8	m_DvlStpPsMode;			// header parse mode
		BOOL8	m_DvlIsStpPars;			// do parsing

		BOOL8	m_DvlIsStpMult;			// multiple mark found
		char*	m_DvlStpStrName;		// line name address
		__u32	m_DvlStpNameLen;		// name length
		__u32	m_DvlStpAdr;				// address
		char*	m_DvlStpStrDesc;		// line name descriptor
		__u32	m_DvlStpDescLen;		// descriptor length

		// attribute change
		__u32	m_DvlRatbTimer;

		// LAN cable status timer
		__u32	m_DvlLanCblTimer;

		// USB timer
		__u32	m_DvlUsbTimer;

		// CMS timer
		__u32	m_DvlCmsTimer;

		// Backup timer
		BOOL8	m_IsDvlBkupErr;
		BOOL8	m_IsDvlBkupRun;

		// Log backup timer
		BOOL8	m_IsDvlLogBkupErr;
		BOOL8	m_IsDvlLogBkupRun;

		// thumbnail image
		__u32	m_DvlImgThumbFcc;		// thumbnail image format
		St_GrP4DvlImgTmbCch	m_DvlImgTmbCch[E_GrP4DvlImgThumbCchCnt];	// image thumbnail cache

		// play
		__u32		m_DvlPlayPrgs;
		BOOL8		m_IsDvlPlayDo;
		Def_GrTime	m_DvlPlayTime;

		// debug
		__u8	m_DvlDbgMode;					// debug mode
		BOOL8	m_DvlDbgIsResume;			// resume mode
		__u8	m_DvlDbgStepVm;				// step debug VM index

		// runtime

		// local function
		void	LcDispSetup( void );		// display setup
		void	LcPrjRuleBuild( void );	// project rule build
		void	LcPrjPstDfltBuild( void );	// default PST build
		void	LcPrjPstNameBuild( void );	// default PST name build
		void	LcPrjSkinNameBuild( void );	// default skin name build
		__u16	LcFindEmptyWinId( void );
		__u16	LcFindEmptyPageId( void );
		BOOL8	LcEditWindow( __u16 A_ScptId );	// window edit
		BOOL8	LcEditPage( __u16 A_ScptId );		// page edit
		void	LcEditObjFree( void );		// now edit object release
		void	LcGetEditObjRect( Ptr_GrRect A_PtrRcRslt, Ptr_GrRect A_PtrRcScrPrnt, __s32 A_CtX, __s32 A_CtY, __s32 A_SizeX, __s32 A_SizeY );
		__u32	LcDvlUiDecPage( __u16 A_ScptId );
		void	LcSlaveIdMarkByScpt( void* A_PtrScpt );
		void	LcSlaveMapRebuild( void );

		void	LcDispIdTblBuild( void );
		void	LcDispIdSlaveAdd( Cls_GrP4DoBase* A_Obj );

		void	LcDvlRatbUpdt( void );
		void	LcDvlLanCblUpdt( void );
		void	LcDvrUsbUpdt( void );
		void	LcDvrCmsUpdt( void );
		void	LcDvrBkupUpdt( void );
		void	LcDvrLogBkupUpdt( void );
		void	LcDvlPlayUpdt(void);

		void	LcStpParsModeReady( void );
		void	LcStpParsModeDefine( void );
		void	LcStpParsModeName( void );
		void	LcStpParsModeAddr( void );
		void	LcStpParsModeDescChk( void );
		void	LcStpParsModeDescLine( void );
		void	LcStpParsModeDescBlk( void );
		void	LcStpParsModeRemLine( void );
		void	LcStpParsModeRemBlk( void );
		void	LcStpParsModeEndLine( void );

		void	LcStpParsGetWordNrm( void );		// get one word normal

		void	LcStpParsSysAdd(char* A_StrName, __u32 A_NameLen, __u32 A_Addr, char* A_StrDesc, __u32 A_DescLen);

		void	LcPrjSavePrjHead( void );
		void	LcPrjSavePrjCfg( void );
		void	LcPrjSavePrjRtl( void );
		void	LcPrjSavePrjModel( void );
		void	LcPrjSavePrjSkin( void );
		void	LcPrjSavePrjMdlAtb( void );
		void	LcPrjSaveFolderInfo( void );
		void	LcPrjSaveWinDvlInfo( void );
		void	LcPrjSavePageDvlInfo( void );
		void	LcPrjSavePstTable( void );
		void	LcPrjSavePstDvlInfo( void );
		void	LcPrjSavePstCode( void );
		void	LcPrjSaveImage( void );
		void	LcPrjSaveFont( void );
		void	LcPrjSaveSetupVar( void );
		void	LcPrjSaveSetupSys( void );
		void	LcPrjSaveGvu( void );
		void	LcPrjSaveSetBase( void );
		void	LcPrjSaveAsmDvlInfo( void );
		void	LcPrjSaveMdlDbg( void );
		void	LcPrjSaveSdaData( void );
		void	LcPrjSaveBldFile( void );
		void	LcPrjSaveUrgst( void );
		void	LcPrjSaveRgstRtl( void );
		void	LcPrjSaveAsmRmkTbl(void);
		void	LcPrjSaveAsmFncCmt(void);

		BOOL8	LcPrjLoadOldPrjHead( void );
		void	LcPrjLoadOldScptHd( __u32 A_Size );
		void	LcPrjLoadFolderTbl( __u32 A_Size );
		void	LcPrjLoadWinScptTbl( __u32 A_Size );
		void	LcPrjLoadWinDvlTbl( __u32 A_Size );
		void	LcPrjLoadWinScptData( __u32 A_Size, __u16 A_Id );
		void	LcPrjLoadPstScptTblWhole( __u32 A_Size );
		void	LcPrjLoadPstScptTblOne( __u32 A_Size, __u16 A_SkinIdx );
		void	LcPrjLoadPstDvlTbl( __u32 A_Size );
		void	LcPrjLoadPstScptData( __u32 A_Size, __u16 A_Id, __u8 A_SkinIdx );
		void	LcPrjLoadImgData( __u32 A_Size, __u16 A_Id, __u8 A_SkinIdx );
		void	LcPrjLoadFontData( __u32 A_Size, __u16 A_Id, __u8 A_SkinIdx );
		void	LcPrjLoadTextData( __u32 A_Size, __u16 A_Id, __u8 A_LngIdx );
		void	LcPrjLoadSetupVar( __u32 A_Size );
		void	LcPrjLoadSetupSys( __u32 A_Size );
		void	LcPrjLoadGvu( __u32 A_Size );
		void	LcPrjLoadSetBase( __u32 A_Size );
		void	LcPrjLoadAsmInfo( __u32 A_Size );
		void	LcPrjLoadAsmScptTbl( __u32 A_Size );
		void	LcPrjLoadAsmData( __u32 A_Size, __u16 A_Id );
		void	LcPrjLoadMdlDbg( __u32 A_Size, __u16 A_Id );
		void	LcPrjLoadPageScptTbl( __u32 A_Size );
		void	LcPrjLoadPageDvlTbl( __u32 A_Size );
		void	LcPrjLoadPageScptData( __u32 A_Size, __u16 A_Id );
		void	LcPrjLoadSdaData( __u32 A_Size );
		void	LcPrjLoadSru( __u32 A_Size );
		void	LcPrjLoadBldFile( __u32 A_Size, __u16 A_Id );
		void	LcPrjLoadAsmCmt( __u32 A_Size, __u16 A_Id );
		void	LcPrjLoadScptCfgPrj( __u32 A_Size );
		void	LcPrjLoadScptCfgRtl( __u32 A_Size );
		void	LcPrjLoadMdlAtb( __u32 A_Size, __u16 A_ModelIdx );
		void	LcPrjLoadPrjConfig( __u32 A_Size );
		void	LcPrjLoadPrjRuntime( __u32 A_Size );
		void	LcPrjLoadPrjModel( __u32 A_Size, __u16 A_ModelIdx );
		void	LcPrjLoadPrjSkin( __u32 A_Size, __u16 A_SkinIdx );
		void	LcPrjLoadPrjUrgst( __u32 A_Size );
		void	LcPrjLoadPrjRgstRtl( __u32 A_Size );
		void	LcPrjLoadPrjAsmRmkTbl(__u32 A_Size, __u16 A_Id);

		void	LcScptSaveHead( void );
		void	LcScptSaveCfgPrj( void );
		void	LcScptSaveCfgRtl( void );
		void	LcScptSaveMdlAtb( __u8 A_MdlIdx );
		void	LcScptSaveWinInfo( void );
		void	LcScptSaveWinScpt( void );
		void	LcScptSavePageInfo( void );
		void	LcScptSavePageScpt( void );
		void	LcScptSaveAsmTbl( void );
		void	LcScptSaveAsmScpt( void );
		void	LcScptSavePstTbl( __u8 A_Skin );
		void	LcScptSavePstScpt( __u8 A_Skin );
		void	LcScptSaveFontData( __u8 A_Skin );
		void	LcScptSaveImgData( __u8 A_Skin );
		void	LcScptSaveTxtData( void );

		void	LcFontUpdt( void );		// font update

		void	LcDbgGvsInit( void );
		void	LcDbgConInit( void );

		void	LcTimerEmuInit( void );

		// direct function
		BOOL8	LcDvlPrjNew( __u32 A_FccPx, __u32 A_Res1X, __u32 A_Res1Y, __u32 A_Res2X, __u32 A_Res2Y, BOOL8 A_IsDual, BOOL8 A_IsInputDual );
		void	LcDvlPrjClose( void );
		BOOL8	LcDvlPrjSave( WCHAR* A_StrFn );
		BOOL8	LcDvlPrjLoad( WCHAR* A_StrFn );
		__u16	LcDvlWindowNew( void );
		__u16	LcDvlPageNew( void );
		BOOL8	LcDvlWindowDel( __u16 A_ScptId );
		BOOL8	LcDvlPageDel( __u16 A_ScptId );
		BOOL8	LcDvlEditLumpSet( __u16 A_ScptId );
		__u16	LcDvlEditObjCreate( __s32 A_ScrX, __s32 A_ScrY, __u8 A_ClassId );
		__u32	LcDvlScriptDec( __u16 A_PrntId, void* A_PtrScpt );
		void	LcDvlEditSkinSet( __u8 A_SkinIdx );
		void	LcDvlEditModelIdxSet( __u8 A_ModelIdx );
		__u8	LcDvlEditObjClassIdGet( __u16 A_ScptId );
		__u32	LcDvlEditObjScptDump( __u16 A_ScptId, void* A_PtrBuf, BOOL8 A_IsCopyMode );
		__u16	LcDvlEditObjFrameGet( __u16 A_ScptId );
		void	LcDvlEditObjRedraw( __u16 A_ScptId );
		__u32	LcDvlEditObjListByPos( __s32 A_X, __s32 A_Y, __u16* A_PtrBuf );
		__u16	LcDvlEditObjSlaveObjGet( __u16 A_ScptId );
		__u16	LcDvlEditObjPrntObjGet( __u16 A_ScptId );
		BOOL8	LcDvlEditObjIsWindow( __u16 A_ScptId );
		void	LcDvlEditObjDelete( __u16 A_ScptId );
		void	LcDvlEditObjScptParaUpdt( __u16 A_ScptId, void* A_PtrScpt, BOOL8 A_IsNoCopy );

		void	LcDvlImgTmbFreeByCch(__u32 A_CchIdx);
		void	LcDvlImgTmbFreeById(__u16 A_ImgId);
		void	LcDvlImgTmbAllFree(void);
		__s32	LcDvlImgTmbFindId(__u16 A_ImgId);
		__s32	LcDvlImgTmbFindEmpty(void);
		__s32	LcDvlImgTmbFindOldest(void);

		BOOL8	LcDvlIsDbgPause( void );
		void	LcDvlDbgStart( void );
		void	LcDvlDbgFinish( void );

		// inherit
		virtual	void	RtlVmDoOneCmd(__u8 A_VmIdx);
		virtual	void	RtlVmDoSlice(__u8 A_VmIdx);
		virtual	void	RtlVmFault(__u8 A_VmIdx);	// VM fault
		virtual	void	RtlAtLockProc( void );

		virtual	void	RtlTimer( __u32 A_Hndl );
		virtual	BOOL8	RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg );

	public:
		Cls_GrP4DvlMng( Cls_GrTaskSvr* A_TaskSvr );
		virtual	~Cls_GrP4DvlMng( void );

		// info
		WCHAR*	DvlClassNameGet( __u8 A_ClassId );
		BOOL8	DvlIsPageWindow( __u16 A_ScptId );

		// project
		BOOL8	DvlPrjNew( __u32 A_FccPx, __u32 A_Res1X, __u32 A_Res1Y, __u32 A_Res2X, __u32 A_Res2Y, BOOL8 A_IsDual, BOOL8 A_IsInputDual );
		void	DvlPrjClose( void );

		BOOL8	DvlPrjSave( WCHAR* A_StrFn );
		BOOL8	DvlPrjLoad( WCHAR* A_StrFn );

		BOOL8	DvlScptExport( WCHAR* A_StrFn, __u8 A_Model, BOOL8 A_IsAdd );		// script export , model = 0xFF - this model

		__u16	DvlWindowNew( void );		// create new window, result = script id
		__u16	DvlPageNew( void );		// create new page, result = script id

		BOOL8	DvlWindowDel( __u16 A_ScptId );	// window delete
		BOOL8	DvlPageDel( __u16 A_ScptId );	// page delete

		__u16	DvlSlaveScptIdAlloc( void );
		void	DvlSlaveScptIdFree( __u16 A_ScptId );

		__u8	WindowLayerGet( __u16 A_ScptId );
		void	WindowLayerSet( __u16 A_ScptId, __u8 A_Layer );
		WCHAR*	DvlWinNameGet( __u16 A_ScptId );
		void	DvlWinNameSet( __u16 A_ScptId, WCHAR* A_StrName );
		__u16	DvlWinPrntFolderGet( __u16 A_ScptId );
		void	DvlWinPrntFolderSet( __u16 A_ScptId, __u16 A_FolderId );
		Ptr_GrP4ScptWinInfo	DvlWinScptInfoPtrGet( __u16 A_ScptId );

		BOOL8	DvlWinIsExist( __u16 A_ScptId );
		__u16	DvlWinFindByPrntFolder( __u16 A_FolderId );

		__u16	DvlPagePrntFolderGet( __u16 A_ScptId );
		void	DvlPagePrntFolderSet( __u16 A_ScptId, __u16 A_FolderId );
		WCHAR*	DvlPageNameGet( __u16 A_ScptId );
		void	DvlPageNameSet( __u16 A_ScptId, WCHAR* A_StrName );
		Ptr_GrP4ScptPageInfo	DvlPageScptInfoPtrGet( __u16 A_ScptId );

		BOOL8	DvlPageIsExist( __u16 A_ScptId );
		__u16	DvlPageFindByPrntFolder( __u16 A_FolderId );

		BOOL8	DvlIsPrjExist( void );		// check project exist
		Ptr_GrP4PrjCfg	DvlConfigPtrGet( void );

		BOOL8	DvlFolderInfoGet( __u16 A_Id, WCHAR* A_StrRtName, __u16* A_PtrRtPrntId );
		void	DvlFolderInfoSet( __u16 A_Id, WCHAR* A_StrName, __u16 A_PrntId );
		void	DvlFolderNameSet( __u16 A_Id, WCHAR* A_StrName );
		void	DvlFolderPrntIdSet( __u16 A_Id, __u16 A_PrntId );
		void	DvlFolderExpandSet( __u16 A_Id, BOOL8 A_IsExpand );
		void	DvlFolderDelete( __u16 A_Id );
		BOOL8	DvlFolderIsExpand( __u16 A_Id );

		__u32	DvlScriptDec( __u16 A_PrntId, void* A_PtrScpt );	// script decode

		void	DvlEditSkinSet( __u8 A_SkinIdx );
		__u8	DvlEditSkinGet( void );
		__u8	DvlEditModelIdxGet( void );
		void	DvlEditModelIdxSet( __u8 A_ModelIdx );

		__u16	DvlEditObjCreate( __s32 A_ScrX, __s32 A_ScrY, __u8 A_ClassId ); 
		void	DvlEditLumpScptUpdt( void );
		__u16	DvlEditLumpGet( void );		// now edit lump get
		BOOL8	DvlEditLumpSet( __u16 A_ScptId );

		BOOL8	DvlEditObjScrRectGet( __u16 A_ScptId, Ptr_GrRect A_PtrRtRect, BOOL8 A_IsNoChk );
		BOOL8	DvlEditObjCliRectGet(__u16 A_ScptId, Ptr_GrRect A_PtrRtRect, BOOL8 A_IsNoChk);
		BOOL8	DvlEditObjPosSet(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel);
		BOOL8	DvlEditObjPosGet(__u16 A_ScptId, Ptr_GrRect A_PtrRtRect, __u8* A_PtrRtPosRel);
		BOOL8	DvlEditObjPosMoveAbs(__u16 A_ScptId, __s32 A_X, __s32 A_Y);
		BOOL8	DvlEditObjPosMoveRel(__u16 A_ScptId, __s32 A_X, __s32 A_Y);
		BOOL8	DvlEditObjSizeSetAbs(__u16 A_ScptId, __s32 A_X, __s32 A_Y);
		BOOL8	DvlEditObjSizeSetRel(__u16 A_ScptId, __s32 A_X, __s32 A_Y);
		__u16	DvlEditObjFindByPos( __s32 A_X, __s32 A_Y, BOOL8 A_IsExcFrm = FALSE );
		__u8	DvlEditObjClassIdGet( __u16 A_ScptId );
		__u32	DvlEditObjScptDump( __u16 A_ScptId, void* A_PtrBuf, BOOL8 A_IsCopyMode );
		__u16	DvlEditObjFrameGet( __u16 A_ScptId );
		void	DvlEditObjRedraw( __u16 A_ScptId );
		__u32	DvlEditObjListByPos( __s32 A_X, __s32 A_Y, __u16* A_PtrBuf );
		__u16	DvlEditObjSlaveObjGet( __u16 A_ScptId );
		__u16	DvlEditObjNextObjGet( __u16 A_ScptId );
		__u16	DvlEditObjPrntObjGet( __u16 A_ScptId );
		BOOL8	DvlEditObjIsWindow( __u16 A_ScptId );
		void	DvlEditObjDelete( __u16 A_ScptId );
		__u32	DvlEditObjParaInfoDump( __u16 A_ScptId, void* A_PtrBuf );
		void	DvlEditObjScptParaUpdt( __u16 A_ScptId, void* A_PtrScpt, BOOL8 A_IsNoCopy );
		BOOL8	DvlEditObjInfoT1(__u16 A_ScptId, __u8* A_PtrRtClassType, Ptr_GrRect A_PtrRtRect, __u8* A_PtrRtPosRel, __u16* A_PtrRtSlaveId,
			__u16* A_PtrRtNextId);

		WCHAR*	DvlModelNameGet( __u8 A_ModelIdx );
		void	DvlModelNameSet( __u8 A_ModelIdx, WCHAR* A_StrName );
		__u16	DvlModelNumberGet( __u8 A_ModelIdx );
		void	DvlModelNumberSet( __u8 A_ModelIdx, __u16 A_Number );
		void	DvlModelCopyTo( __u8 A_TargetIdx );

		WCHAR*	DvlSkinNameGet( __u8 A_SkinIdx );
		void	DvlSkinNameSet( __u8 A_SkinIdx, WCHAR* A_StrName );
		void	DvlSkinCopy( __u8 A_TgIdx, __u8 A_SrIdx );

		Ptr_GrP4ScptPstData	DvlPstCreate( __u16 A_Id );
		void	DvlPstCodeCountSet( __u16 A_Id, __u8 A_Cnt );
		WCHAR*	DvlPstCodeNameGet( __u16 A_Id );
		void	DvlPstCodeNameSet( __u16 A_Id, WCHAR* A_StrName );

		BOOL8	DvlImgLoadBmp( __u16 A_Id, WCHAR* A_StrFn );
		void	DvlImgDelete( __u16 A_Id );
		void	DvlImgThumbFccSet(__u32 A_Fcc);
		__u32	DvlImgThumbFccGet(void);
		Ptr_GrP4DvlImgTmbCch	DvlImgThumbGet(__u16 A_ImgId);

		__u32	DvlClrPltGet( __u8 A_ClrIdx );
		void	DvlClrPltSet( __u8 A_ClrIdx, __u32 A_Color );

		void	DvlTxtNameGet( __u16 A_Id, WCHAR* A_StrRt );
		void	DvlTxtSet( __u16 A_Id, __u8 A_LangIdx, WCHAR* A_Str );

		BOOL8	DvlFontLoadFile( __u8 A_Id, WCHAR* A_StrFn );
		void	DvlFontDelete( __u8 A_Id );

		void	DvlCursorColorSet( __u32 A_Color );

		void*	DvlSetupUsrTblGet( void );		// setup variable table pointer
		__u32	DvlSetupUsrCntGet( void );
		void	DvlSetupUsrCntSet( __u32 A_Cnt );

		void*	DvlSetupSysTblGet( void );
		__u32	DvlSetupSysCntGet( void );

		void*	DvlGvuInfoTblGet( void );

		void*	DvlUrgstInfoTblGet( void );

		void*	DvlAsmCreate( __u16 A_Id );
		void	DvlAsmCodeCountSet( __u16 A_Id, __u16 A_Cnt );		/* not use function */
		WCHAR*	DvlAsmFuncNameGet( __u16 A_Id );
		void	DvlAsmFuncNameSet( __u16 A_Id, WCHAR* A_StrName );
		Ptr_GrP4PrjAsmCodeRmk	DvlAsmRmkPtrGet(__u16 A_Id);
		Ptr_GrP4PrjAsmFncCmt	DvlAsmFncCmtPtrGet(__u16 A_Id);
		BOOL8	DvlAsmBreakPointToggle( __u16 A_Id, __u16 A_Ip );
		BOOL8	DvlAsmActiveToggle( __u16 A_Id, __u16 A_Ip );

		Ptr_GrP4PrjBldFile	DvlBldFwInfoGet( __u8 A_ModelIdx );

		void	DvlVkbdTxtPageCntSet( __u8 A_Lang , __u8 A_Cnt );
		void	DvlVkbdTxtKeyCodeSet( __u8 A_Page, __u8 A_Key, __u8 A_Lang, WCHAR A_KeyCode );

		void*	DvlModelAtbGet( __u8 A_ModelIdx );
		void*	DvlModelDbgGet( __u8 A_ModelIdx );

		Ptr_GrP4DvlObjInfo	DvlDispIdTblGet( void );

		void	DvlDbgStart( void );
		void	DvlDbgFinish( void );

		void	DvlDbgResume( void );
		void	DvlDbgStep(__u8 A_VmIdx);

		BOOL8	DvlIsDbgPause( void );
		BOOL8	DvlIsDbgRun( void );			// use object
		void	DvlDbgVmRposGet(__u8 A_VmIdx,__u16* A_PtrRtCs, __u16* A_PtrRtIp);
		Ptr_GrP4AsmVm	DvlVmPtrGet(__u8 A_VmIdx);
		
		void	DvlFwUpgdRun( __u32 A_CtlgHndl );
		BOOL8	DvlBkupReq( __u8 A_Media, __u8 A_RecDev, __u16 A_ChMap,
			Def_GrTime A_TimeSt, Def_GrTime A_TimeEd, BOOL8 A_IsCmbViewer );
		void	DvlBkupCancel( void );

		BOOL8	DvlLogBkupReq( __u8 A_RecDev, __u16 A_ChMap, __u16 A_EvtMap,
			Def_GrTime A_TimeSt, Def_GrTime A_TimeEd, WCHAR* A_StrFn );
		void	DvlLogBkupCancel( void );

		void	DvlPlayTimeSet(Def_GrTime A_Time);
		void	DvlPlayDo(BOOL8 A_IsDo);
		void	DvlPlayDirSet(BOOL8 A_IsBack);
		void	DvlPlaySpeedSet(__s32 A_Speed);
		void DvlPlayTimeGet(Def_GrTime* A_PtrRtSec, BOOL8* A_PtrRtIsSm);

		// inherited
		virtual	__u32	DvlConResX( __u8 A_ConIdx );
		virtual	__u32	DvlConResY( __u8 A_ConIdx );

		virtual	BOOL8	VmRunCode(__u16 A_CodeId, Cls_GrP4DoBase* A_ObjHost, Ptr_GrP4MngVmRpara A_PtrRpara = NULL, BOOL8 A_IsRunNow = FALSE);

		virtual	void	KeyInput( __u8 A_Key, BOOL8 A_IsPush, __u8 A_DevType );
		virtual	void	MousePosInput( __s32 A_X, __s32 A_Y, BOOL8 A_IsAbs );
		virtual	void	MouseBtnInput( __u8 A_BtnIdx, BOOL8 A_IsPush );
		virtual	void	MouseWheelInput( BOOL8 A_IsDown );

};

//====================================================================
//global function

//====================================================================
//out Enviroment conditional
#endif

