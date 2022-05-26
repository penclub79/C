/*
	Gause Platform develop manager

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DvlMng
#define	_EPC_GrP3DvlMng

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P3/GrP3Mng.h>
#include	<P3/Dvl/GrP3DvlBase.h>
#include	<P3/GrP3AsmBase.h>
#include	<P3/GrP3Script.h>
#include	<P3/GrP3DoBase.h>
#include	<P3/GrP3Font.h>
#include	<GrFileTool.h>
#include	<GrFileCtrl.h>
#include	<GrProcess.h>

#include <Dvr/PtzV2/GrPtzV2Mng.h>

//====================================================================
//constance

#define E_GrP3DvlWinMaxCnt					128			/* window max count */
#define E_GrP3DvlPageMaxCnt					256			/* page max count */
#define	E_GrP3DvlPstMaxCnt					1024		/* paint script count */
#define E_GrP3DvlAsmMaxCnt					512			/* assembly function max count */

#define E_GrP3DvlLumpSlvMax					256			

#define E_GrP3DvlScptBufMax					0x8000	/* UI script buffer max size */
#define E_GrP3DvlPstCodeMax					64
#define E_GrP3DvlPstBufMax					(sizeof(St_GrP3ScptPstData) * E_GrP3DvlPstCodeMax)

#define E_GrP3DvlTextSysCnt					128			/* system text count */
#define E_GrP3DvlTextMaxLen					511			/* 511 character */
#define E_GrP3DvlTextBufSize				((E_GrP3DvlTextMaxLen +1) *2)

#define E_GrP3DvlSetupUsrMaxCnt			1024

#define E_GrP3DvlSruMaxCnt					1024

#define E_GrP3DvlSetupHeaderMax			0x20000	/* import export file max size */

#define E_GrP3DvlAsmCodeMax					256			/* assembly code in function */
#define E_GrP3DvlAsmBufMax					(E_GrP3AsmOpUnitSize * E_GrP3DvlAsmCodeMax)
#define E_GrP3DvlAsmRmkBufSize			4096

#define E_GrP3DvlDfltLayer					1				/* window default layer */
#define E_GrP3DvlDfltWinLeft				10
#define E_GrP3DvlDfltWinTop					10
#define E_GrP3DvlDfltWinRight				300
#define E_GrP3DvlDfltWinBottom			300

#define E_GrP3DvlStpDefNameMin			12

#define E_GrP3DvlStpPsTypeReady			0		/* wait sub text pars */
#define E_GrP3DvlStpPsTypeText			1		/* sub text pars text */
#define E_GrP3DvlStpPsTypeNumZero		2		/* sub text pars number zero */
#define E_GrP3DvlStpPsTypeNumHpx		3		/* sub text pars number hex prefix */
#define E_GrP3DvlStpPsTypeNumDec		4		/* sub text pars number decimal */
#define E_GrP3DvlStpPsTypeNumHex		5		/* sub text pars number hexadecimal */
#define E_GrP3DvlStpPsTypeRemPref		6		/* sub text pars remark prefix */
#define E_GrP3DvlStpPsTypeRemLine		7		/* sub text pars remark line */
#define E_GrP3DvlStpPsTypeRemBlk		8		/* sub text pars remark block */
#define E_GrP3DvlStpPsTypeDirect		9		/* sub text pars number directive */
#define E_GrP3DvlStpPsTypeUnknown		10		/* sub text pars unknown */
#define E_GrP3DvlStpPsTypeLineEnd		11	/* line end */

#define E_GrP3DvlStpPsModeReady			0		/* wait parsing mode */
#define E_GrP3DvlStpPsModeDefine		1		/* directive define scan */
#define E_GrP3DvlStpPsModeName			2		/* scan name */
#define E_GrP3DvlStpPsModeAddr			3		/* scan address */
#define E_GrP3DvlStpPsModeDescChk		4		/* descriptor check */
#define E_GrP3DvlStpPsModeDescLine	5		/* description line */
#define E_GrP3DvlStpPsModeDescBlk		6		/* description block */
#define E_GrP3DvlStpPsModeRemLine		7		/* remark line */
#define E_GrP3DvlStpPsModeRemBlk		8		/* remark block */
#define E_GrP3DvlStpPsModeLineEnd		9		/* line end */

#define E_GrP3DvlMdlDbgClstCnt			7

#define E_GrP3DvlDfltObjSizeX				50
#define E_GrP3DvlDfltObjSizeY				20

#define E_GrP3DvlModelThis					0xFF	/* this model */

#define E_GrP3DvlDbgModeRun							0		/* debug run */
#define E_GrP3DvlDbgModePause						1		/* debug pause */
#define E_GrP3DvlDbgModeResume					2		/* debug resume */
#define E_GrP3DvlDbgModeStep						3		/* debug step */

// message command
#define E_GrP3DvlMcmdPrjNew							1		/* project new : St_GrP3DvlMsgPrjNew */
#define E_GrP3DvlMcmdPrjClose						2		/* project close : St_GrP3DvlMsgBase */
#define E_GrP3DvlMcmdPrjSave						3		/* project save : St_GrP3DvlMsgPrjSave */
#define E_GrP3DvlMcmdPrjLoad						4		/* project load : St_GrP3DvlMsgPrjLoad */
#define E_GrP3DvlMcmdScptExport					5		/* script export : St_GrP3DvlMsgScptExport */
#define E_GrP3DvlMcmdWindowNew					6		/* window new : St_GrP3DvlMsgWindowNew */
#define E_GrP3DvlMcmdPageNew						7		/* page new : St_GrP3DvlMsgPageNew */
#define E_GrP3DvlMcmdWindowDel					8		/* delete window : St_GrP3DvlMsgWindowDel */
#define E_GrP3DvlMcmdPageDel						9		/* delete page : St_GrP3DvlMsgPageDel */
#define E_GrP3DvlMcmdScriptDec					10	/* Script decode : St_GrP3DvlMsgScriptDec */
#define E_GrP3DvlMcmdEditSkinSet				11	/* edit skin set : St_GrP3DvlMsgEditSkinSet */
#define E_GrP3DvlMcmdEditModelIdxSet		12	/* edit model index set : St_GrP3DvlMsgEditModelIdxSet */
#define E_GrP3DvlMcmdEditObjCreate			13	/* page new : St_GrP3DvlMsgEditObjCreate */
#define E_GrP3DvlMcmdEdtLumpScptUpdt		14	/* edit lump script update : St_GrP3DvlMsgBase */
#define E_GrP3DvlMcmdEditLumpGet				15	/* edit lump get : St_GrP3DvlMsgEditLumpGet */
#define E_GrP3DvlMcmdEditLumpSet				16	/* edit lump set : St_GrP3DvlMsgEditLumpSet */
#define E_GrP3DvlMcmdEditObjScrRectGet	17	/* edit object screen rect get : St_GrP3DvlMsgEditObjScrRectGet */
#define E_GrP3DvlMcmdEditObjRectSet			18	/* edit object rect set : St_GrP3DvlMsgEditObjRectSet */
#define E_GrP3DvlMcmdEditObjRectGet			19	/* edit object rect get : St_GrP3DvlMsgEditObjRectGet */
#define E_GrP3DvlMcmdEditObjFindByPos		20	/* edit object find by position : St_GrP3DvlMsgEditObjFindByPos */
#define E_GrP3DvlMcmdEditObjClassIdGet	21	/* edit object class id get : St_GrP3DvlMsgEditObjClassIdGet */
#define E_GrP3DvlMcmdEditObjScptDump		22	/* edit object script dump : St_GrP3DvlMsgEditObjScptDump */
#define E_GrP3DvlMcmdEditObjFrameGet		23	/* edit object frame get : St_GrP3DvlMsgEditObjFrameGet */
#define E_GrP3DvlMcmdEditObjRedraw			24	/* edit object redraw : St_GrP3DvlMsgEditObjRedraw */
#define E_GrP3DvlMcmdEditObjListByPos		25	/* edit object list by position : St_GrP3DvlMsgEditObjListByPos */
#define E_GrP3DvlMcmdEditObjSlaveObjGet	26	/* edit object slave obj get : St_GrP3DvlMsgEditObjSlaveObjGet */
#define E_GrP3DvlMcmdEditObjNextObjGet	27	/* edit object next obj get : St_GrP3DvlMsgEditObjNextObjGet */
#define E_GrP3DvlMcmdEditObjPrntObjGet	28	/* edit object parent obj get : St_GrP3DvlMsgEditObjPrntObjGet */
#define E_GrP3DvlMcmdEditObjIsWindow		29	/* edit object check type window : St_GrP3DvlMsgEditObjIsWindow */
#define E_GrP3DvlMcmdEditObjDelete			30	/* edit object delete : St_GrP3DvlMsgEditObjDelete */
#define E_GrP3DvlMcmdEditObjParaInfoDump	31	/* edit object parameter info dump : St_GrP3DvlMsgEditObjParaInfoDump */
#define E_GrP3DvlMcmdEditObjParaUpdt		32	/* edit object parameter info dump : St_GrP3DvlMsgEditObjParaUpdt */

#define E_GrP3DvlMcmdEditObjInfoT1			33	/* edit object info type 1 : St_GrP3DvlMsgEditObjInfoT1 */

#define E_GrP3DvlMcmdDbgStart						40	/* debug start : St_GrP3DvlMsgBase */
#define E_GrP3DvlMcmdDbgFinish					41	/* debug finish : St_GrP3DvlMsgBase */
#define E_GrP3DvlMcmdIsDbgPause					42	/* check debug pause : St_GrP3DvlMsgIsDbgPause */
#define E_GrP3DvlMcmdDbgVmCsGet					43	/* get VM cs : St_GrP3DvlMsgDbgVmCsGet */
#define E_GrP3DvlMcmdDbgVmIpGet					44	/* get VM ip : St_GrP3DvlMsgDbgVmIpGet */


//====================================================================
//global macro

//====================================================================
//global type

typedef	struct St_GrP3DvlObjInfo
{
	__u16	Id;
	St_GrRect	Rc;
}	*Ptr_GrP3DvlObjInfo;

typedef	struct St_GrP3DvlMsgBase 
{
	St_GrTaskMsgHd	MsgHd;
	__u32	Cmd;
}	*Ptr_GrP3DvlMsgBase;

typedef struct St_GrP3DvlMsgPrjNew
{
	St_GrP3DvlMsgBase	Hd;
	BOOL8*	PtrRslt;
	__u32 FccPx;
	__u32 Res1X;
	__u32 Res1Y;
	__u32 Res2X;
	__u32 Res2Y;
	BOOL8 IsDual;
	BOOL8 IsInputDual;

}	*Ptr_GrP3DvlMsgPrjNew;

typedef struct St_GrP3DvlMsgPrjSave
{
	St_GrP3DvlMsgBase	Hd;
	BOOL8*	PtrRslt;
	WCHAR*	StrFn;
}	*Ptr_GrP3DvlMsgPrjSave;

typedef struct St_GrP3DvlMsgPrjLoad
{
	St_GrP3DvlMsgBase	Hd;
	BOOL8*	PtrRslt;
	WCHAR*	StrFn;
}	*Ptr_GrP3DvlMsgPrjLoad;

typedef	struct St_GrP3DvlMsgScptExport
{
	St_GrP3DvlMsgBase	Hd;
	BOOL8*	PtrRslt;
	WCHAR*	StrFn;
	__u8		Model;
}	*Ptr_GrP3DvlMsgScptExport;

typedef	struct St_GrP3DvlMsgEditLumpSet
{
	St_GrP3DvlMsgBase	Hd;
	BOOL8*	PtrRslt;
	__u16	ScptId;
}	*Ptr_GrP3DvlMsgEditLumpSet;

typedef	struct St_GrP3DvlMsgEditLumpGet
{
	St_GrP3DvlMsgBase	Hd;
	__u16*	PtrRslt;
}	*Ptr_GrP3DvlMsgEditLumpGet;

typedef struct St_GrP3DvlMsgWindowNew
{
	St_GrP3DvlMsgBase	Hd;
	__u16*	PtrRslt;
}	*Ptr_GrP3DvlMsgWindowNew;

typedef struct St_GrP3DvlMsgPageNew
{
	St_GrP3DvlMsgBase	Hd;
	__u16*	PtrRslt;
}	*Ptr_GrP3DvlMsgPageNew;

typedef struct St_GrP3DvlMsgEditObjCreate
{
	St_GrP3DvlMsgBase	Hd;
	__u16*	PtrRslt;
	__s32	ScrX;
	__s32	ScrY;
	__u8	ClassId;
}	*Ptr_GrP3DvlMsgEditObjCreate;

typedef struct St_GrP3DvlMsgWindowDel
{
	St_GrP3DvlMsgBase	Hd;
	BOOL8*	PtrRslt;
	__u16	ScptId;
}	*Ptr_GrP3DvlMsgWindowDel;

typedef struct St_GrP3DvlMsgPageDel
{
	St_GrP3DvlMsgBase	Hd;
	BOOL8*	PtrRslt;
	__u16	ScptId;
}	*Ptr_GrP3DvlMsgPageDel;

typedef struct St_GrP3DvlMsgScriptDec
{
	St_GrP3DvlMsgBase	Hd;
	__u32*	PtrRslt;
	__u16	PrntId;
	void*	PtrScpt;
}	*Ptr_GrP3DvlMsgScriptDec;

typedef struct St_GrP3DvlMsgEditSkinSet
{
	St_GrP3DvlMsgBase	Hd;
	__u8	SkinIdx;
}	*Ptr_GrP3DvlMsgEditSkinSet;

typedef struct St_GrP3DvlMsgEditModelIdxSet
{
	St_GrP3DvlMsgBase	Hd;
	__u8	ModelIdx;
}	*Ptr_GrP3DvlMsgEditModelIdxSet;

typedef struct St_GrP3DvlMsgEditObjScrRectGet
{
	St_GrP3DvlMsgBase	Hd;
	BOOL8*	PtrRslt;
	__u16 ScptId;
	Ptr_GrRect PtrRtRect;
	BOOL8 IsNoChk;
}	*Ptr_GrP3DvlMsgEditObjScrRectGet;

typedef struct St_GrP3DvlMsgEditObjRectSet
{
	St_GrP3DvlMsgBase	Hd;
	BOOL8*	PtrRslt;
	__u16 ScptId;
	Ptr_GrRect PtrRect;
}	*Ptr_GrP3DvlMsgEditObjRectSet;

typedef struct St_GrP3DvlMsgEditObjRectGet
{
	St_GrP3DvlMsgBase	Hd;
	BOOL8*	PtrRslt;
	__u16 ScptId;
	Ptr_GrRect PtrRtRect;
}	*Ptr_GrP3DvlMsgEditObjRectGet;

typedef struct St_GrP3DvlMsgEditObjFindByPos
{
	St_GrP3DvlMsgBase	Hd;
	__u16*	PtrRslt;
	__u16 PosX;
	__u16 PosY;
}	*Ptr_GrP3DvlMsgEditObjFindByPos;

typedef struct St_GrP3DvlMsgEditObjClassIdGet
{
	St_GrP3DvlMsgBase	Hd;
	__u8*	PtrRslt;
	__u16 ScptId;
}	*Ptr_GrP3DvlMsgEditObjClassIdGet;

typedef struct St_GrP3DvlMsgEditObjScptDump
{
	St_GrP3DvlMsgBase	Hd;
	__u32*	PtrRslt;
	__u16 ScptId;
	void* PtrBuf;
	BOOL8 IsCopyMode;
}	*Ptr_GrP3DvlMsgEditObjScptDump;

typedef struct St_GrP3DvlMsgEditObjFrameGet
{
	St_GrP3DvlMsgBase	Hd;
	__u16*	PtrRslt;
	__u16 ScptId;
}	*Ptr_GrP3DvlMsgEditObjFrameGet;

typedef struct St_GrP3DvlMsgEditObjRedraw 
{
	St_GrP3DvlMsgBase	Hd;
	__u16 ScptId;
}	*Ptr_GrP3DvlMsgEditObjRedraw;

typedef struct St_GrP3DvlMsgEditObjListByPos
{
	St_GrP3DvlMsgBase	Hd;
	__u32*	PtrRslt;
	__s32	X;
	__s32	Y;
	__u16* PtrBuf;
} *Ptr_GrP3DvlMsgEditObjListByPos;

typedef struct St_GrP3DvlMsgEditObjSlaveObjGet
{
	St_GrP3DvlMsgBase	Hd;
	__u16*	PtrRslt;
	__u16 ScptId;
}	*Ptr_GrP3DvlMsgEditObjSlaveObjGet;

typedef struct St_GrP3DvlMsgEditObjNextObjGet
{
	St_GrP3DvlMsgBase	Hd;
	__u16*	PtrRslt;
	__u16 ScptId;
}	*Ptr_GrP3DvlMsgEditObjNextObjGet;

typedef struct St_GrP3DvlMsgEditObjPrntObjGet
{
	St_GrP3DvlMsgBase	Hd;
	__u16*	PtrRslt;
	__u16 ScptId;
}	*Ptr_GrP3DvlMsgEditObjPrntObjGet;

typedef struct St_GrP3DvlMsgEditObjIsWindow
{
	St_GrP3DvlMsgBase	Hd;
	BOOL8*	PtrRslt;
	__u16 ScptId;
}	*Ptr_GrP3DvlMsgEditObjIsWindow;

typedef struct St_GrP3DvlMsgEditObjDelete
{
	St_GrP3DvlMsgBase	Hd;
	__u16 ScptId;
}	*Ptr_GrP3DvlMsgEditObjDelete;

typedef struct St_GrP3DvlMsgEditObjParaInfoDump
{
	St_GrP3DvlMsgBase	Hd;
	__u32*	PtrRslt;
	__u16		ScptId;
	void*		PtrBuf;
}	*Ptr_GrP3DvlMsgEditObjParaInfoDump;

typedef struct St_GrP3DvlMsgEditObjParaUpdt
{
	St_GrP3DvlMsgBase	Hd;
	__u16	ScptId;
	void*	PtrScpt;
	BOOL8	IsNoCopy;
}	*Ptr_GrP3DvlMsgEditObjParaUpdt;

typedef struct St_GrP3DvlMsgIsDbgPause
{
	St_GrP3DvlMsgBase	Hd;
	BOOL8*	PtrRslt;
}	*Ptr_GrP3DvlMsgIsDbgPause;

typedef struct St_GrP3DvlMsgDbgVmCsGet
{
	St_GrP3DvlMsgBase	Hd;
	__u16*	PtrRslt;
}	*Ptr_GrP3DvlMsgDbgVmCsGet;

typedef struct St_GrP3DvlMsgDbgVmIpGet
{
	St_GrP3DvlMsgBase	Hd;
	__u16*	PtrRslt;
}	*Ptr_GrP3DvlMsgDbgVmIpGet;

typedef struct St_GrP3DvlMsgEditObjInfoT1
{
	St_GrP3DvlMsgBase	Hd;
	BOOL8*	PtrRslt;
	__u16	ScptId;
	__u8*	PtrRtClassType;
	Ptr_GrRect	PtrRtRect;
	__u16*	PtrRtSlaveId;
	__u16*	PtrRtNextId;
}	*Ptr_GrP3DvlMsgEditObjInfoT1;

//====================================================================
//global class
class	Cls_GrP3DvlMng	:	public	Cls_GrP3Mng
{
	protected:
		// DVL msg
		__u32		m_DvlMsgAloc;

		// project
		BOOL8	m_DvlIsPrjExist;

		// project header
		St_GrP3PrjCfg				m_DvlPrjConfig;
		St_GrP3PrjRtl				m_DvlPrjRtl;				// runtime data
		St_GrP3PrjModelCfg	m_DvlPrjModel[E_GrP3DvlModelMaxCnt];	
		St_GrP3PrjSkinCfg		m_DvlPrjSkin[E_GrP3DvlSkinMaxCnt];
		St_GrP3ScptCfgMdl		m_DvlPrjMdlAtb[E_GrP3DvlModelMaxCnt];		// mode attribute

		// script
		St_GrP3ScptCfgPrj		m_DvlScptCfgPrj;		// script config project
		St_GrP3ScptCfgRtl		m_DvlScptCfgRtl;		// script config runtime

		// runtime
		St_GrP3PrjFoldInfo	m_DvlFoldTbl[E_GrP3DvlFolderMaxCnt];		// tree table

		St_GrP3ScptWinInfo	m_DvlScptWinTbl[E_GrP3DvlWinMaxCnt];	// window table
		St_GrP3PrjWinInfo		m_DvlWinInfo[E_GrP3DvlWinMaxCnt];			// window info

		St_GrP3ScptPageInfo	m_DvlScptPageTbl[E_GrP3DvlPageMaxCnt];	// page table
		St_GrP3PrjPageInfo	m_DvlPageInfo[E_GrP3DvlPageMaxCnt];		// page info

		St_GrP3ScptPstItm		m_DvlScptPstTbl[E_GrP3DvlSkinMaxCnt][E_GrP3DvlPstMaxCnt];	// paint script table
		St_GrP3PrjPstInfo		m_DvlPstInfo[E_GrP3DvlPstMaxCnt];

		Ptr_GrGdib					m_DvlScptImgTbl[E_GrP3DvlSkinMaxCnt][E_GrP3MngImgScptMaxCnt];	// image table

		Ptr_GrP3FontHead		m_DvlScptFontTbl[E_GrP3DvlSkinMaxCnt][E_GrP3MngFontMaxCnt];		// font script table

		St_GrP3ScptTxtItm		m_DvlScptTxtTbl[E_GrP3MngTextScptMaxCnt];		// text

		__u32	m_DvlStpUsrCnt;		// setup user info count
		St_GrP3PrjVarInfo		m_DvlStpUsrTbl[E_GrP3DvlSetupUsrMaxCnt];

		__u32	m_DvlSruCnt;			// setup runtime user info count
		St_GrP3PrjVarInfo		m_DvlSruTbl[E_GrP3DvlSruMaxCnt];

		St_GrP3PrjGvuInfo		m_DvlGvuTbl[E_GrP3MngGvuMaxCnt];		// global variable for user

		St_GrP3ScptAsmInfo	m_DvlScptAsmTbl[E_GrP3DvlAsmMaxCnt];	// assembly
		St_GrP3PrjAsmInfo		m_DvlAsmInfo[E_GrP3DvlAsmMaxCnt];
		St_GrP3PrjAsmCmt		m_DvlAsmCmt[E_GrP3DvlAsmMaxCnt];			// comment

		St_GrP3PrjDbgInfo		m_DvlMdlDblTbl[E_GrP3DvlModelMaxCnt];	// model debug table

		Cls_GrP3DoWin*	m_DvlEdtLump;		// edit lump

		__u32	m_DvlSlvMap[E_GrP3DvlSlaveMapCnt];

		St_GrP3DvlObjInfo	m_DvlEdtIdDispTbl[E_GrP3DvlLumpSlvMax];
		__u32	m_DvlEdtIdDispCnt;

		// setup
		St_GrP3PrjSetBase	m_DvlSetBase;	// setup basic

		// build
		St_GrP3PrjBldFile	m_DvlBldInfo[E_GrP3DvlModelMaxCnt];	// build info

		// project read write
		Cls_GrFileCtrl*	m_DvlFc;
		__u32	m_DvlSaved;
		__u32	m_DvlSaveReq;
		BOOL8	m_DvlIsLoading;

		// setup header pars
		__s8*	m_DvlPtrStpFl;			// setup file buffer
		__u32	m_DvlStpFlSize;			// header size
		__u32	m_DvlStpPsPrs;			// parsing progress
		__u32	m_DvlStpWordLen;		// parsing word size
		__s8*	m_DvlPtrStpWord;		// found word pointer
		__u8	m_DvlStpPsType;			// type parsing
		__u8	m_DvlStpPsMode;			// header parse mode
		BOOL8	m_DvlIsStpPars;			// do parsing

		BOOL8	m_DvlIsStpMult;			// multiple mark found
		__s8*	m_DvlStpStrName;		// line name address
		__u32	m_DvlStpNameLen;		// name length
		__u32	m_DvlStpAdr;				// address
		__s8*	m_DvlStpStrDesc;		// line name descriptor
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

		// debug
		BOOL8	m_DvlIsDbgRun;			// debug run mode
		__u8	m_DvlDbgMode;				// debug mode

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
		void	LcGetEditObjRect( Ptr_GrRect A_PtrRcRslt, Ptr_GrRect A_PtrRcPrnt, __s32 A_CtX, __s32 A_CtY, __s32 A_SizeX, __s32 A_SizeY );
		__u32	LcDvlUiDecPage( __u16 A_ScptId );
		void	LcSlaveIdMarkByScpt( void* A_PtrScpt );
		void	LcSlaveMapRebuild( void );

		void	LcDispIdTblBuild( void );
		void	LcDispIdSlaveAdd( Cls_GrP3DoBase* A_Obj );

		void	LcDvlRatbUpdt( void );
		void	LcDvlLanCblUpdt( void );
		void	LcDvrUsbUpdt( void );
		void	LcDvrCmsUpdt( void );
		void	LcDvrBkupUpdt( void );
		void	LcDvrLogBkupUpdt( void );

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

		void	LcStpParsSysAdd( __s8* A_StrName, __u32 A_NameLen, __u32 A_Addr, __s8* A_StrDesc, __u32 A_DescLen );

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
		void	LcPrjSaveAsmComment( void );
		void	LcPrjSaveMdlDbg( void );
		void	LcPrjSaveSdaData( void );
		void	LcPrjSaveSru( void );
		void	LcPrjSaveBldFile( void );

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
		BOOL8	LcDvlScptExport( WCHAR* A_StrFn, __u8 A_Model );
		__u16	LcDvlWindowNew( void );
		__u16	LcDvlPageNew( void );
		BOOL8	LcDvlWindowDel( __u16 A_ScptId );
		BOOL8	LcDvlPageDel( __u16 A_ScptId );
		__u16	LcDvlEditLumpGet( void );
		BOOL8	LcDvlEditLumpSet( __u16 A_ScptId );
		void	LcDvlEdtLumpScptUpdt( void );
		__u16	LcDvlEditObjCreate( __s32 A_ScrX, __s32 A_ScrY, __u8 A_ClassId );
		__u32	LcDvlScriptDec( __u16 A_PrntId, void* A_PtrScpt );
		void	LcDvlEditSkinSet( __u8 A_SkinIdx );
		void	LcDvlEditModelIdxSet( __u8 A_ModelIdx );
		BOOL8	LcDvlEditObjScrRectGet( __u16 A_ScptId, Ptr_GrRect A_PtrRtRect, BOOL8 A_IsNoChk );
		BOOL8	LcDvlEditObjRectSet( __u16 A_ScptId, Ptr_GrRect A_PtrRect );
		BOOL8	LcDvlEditObjRectGet( __u16 A_ScptId, Ptr_GrRect A_PtrRtRect );
		__u16	LcDvlEditObjFindByPos( __s32 A_X, __s32 A_Y );
		__u8	LcDvlEditObjClassIdGet( __u16 A_ScptId );
		__u32	LcDvlEditObjScptDump( __u16 A_ScptId, void* A_PtrBuf, BOOL8 A_IsCopyMode );
		__u16	LcDvlEditObjFrameGet( __u16 A_ScptId );
		void	LcDvlEditObjRedraw( __u16 A_ScptId );
		__u32	LcDvlEditObjListByPos( __s32 A_X, __s32 A_Y, __u16* A_PtrBuf );
		__u16	LcDvlEditObjSlaveObjGet( __u16 A_ScptId );
		__u16	LcDvlEditObjNextObjGet( __u16 A_ScptId );
		__u16	LcDvlEditObjPrntObjGet( __u16 A_ScptId );
		BOOL8	LcDvlEditObjIsWindow( __u16 A_ScptId );
		void	LcDvlEditObjDelete( __u16 A_ScptId );
		__u32	LcDvlEditObjParaInfoDump( __u16 A_ScptId, void* A_PtrBuf );
		void	LcDvlEditObjScptParaUpdt( __u16 A_ScptId, void* A_PtrScpt, BOOL8 A_IsNoCopy );
		BOOL8	LcDvlEditObjInfoT1( __u16 A_ScptId, __u8* A_PtrRtClassType, Ptr_GrRect A_PtrRtRect, __u16* A_PtrRtSlaveId,
			__u16* A_PtrRtNextId );

		BOOL8	LcDvlIsDbgPause( void );
		void	LcDvlDbgStart( void );
		void	LcDvlDbgFinish( void );
		__u16	LcDvlDbgVmCsGet( void );
		__u16	LcDvlDbgVmIpGet( void );

		// inherit
		virtual	void	RtlVmRun( void );
		virtual	void	RtlVmFault( void );	// VM fault
		virtual	void	RtlAtLockProc( void );

		virtual	void	RtlTimer( __u32 A_Hndl );
		virtual	BOOL8	RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg );

	public:
		Cls_GrP3DvlMng( Cls_GrTaskSvr* A_TaskSvr );
		virtual	~Cls_GrP3DvlMng( void );

		// info
		WCHAR*	DvlClassNameGet( __u8 A_ClassId );
		BOOL8	DvlIsPageWindow( __u16 A_ScptId );

		// project
		BOOL8	DvlPrjNew( __u32 A_FccPx, __u32 A_Res1X, __u32 A_Res1Y, __u32 A_Res2X, __u32 A_Res2Y, BOOL8 A_IsDual, BOOL8 A_IsInputDual );
		void	DvlPrjClose( void );

		BOOL8	DvlPrjSave( WCHAR* A_StrFn );
		BOOL8	DvlPrjLoad( WCHAR* A_StrFn );

		BOOL8	DvlScptExport( WCHAR* A_StrFn, __u8 A_Model );		// script export , model = 0xFF - this model

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
		Ptr_GrP3ScptWinInfo	DvlWinScptInfoPtrGet( __u16 A_ScptId );

		BOOL8	DvlWinIsExist( __u16 A_ScptId );
		__u16	DvlWinFindByPrntFolder( __u16 A_FolderId );

		__u16	DvlPagePrntFolderGet( __u16 A_ScptId );
		void	DvlPagePrntFolderSet( __u16 A_ScptId, __u16 A_FolderId );
		WCHAR*	DvlPageNameGet( __u16 A_ScptId );
		void	DvlPageNameSet( __u16 A_ScptId, WCHAR* A_StrName );
		Ptr_GrP3ScptPageInfo	DvlPageScptInfoPtrGet( __u16 A_ScptId );

		BOOL8	DvlPageIsExist( __u16 A_ScptId );
		__u16	DvlPageFindByPrntFolder( __u16 A_FolderId );

		BOOL8	DvlIsPrjExist( void );		// check project exist
		Ptr_GrP3PrjCfg	DvlConfigPtrGet( void );

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
		BOOL8	DvlEditObjRectSet( __u16 A_ScptId, Ptr_GrRect A_PtrRect );
		BOOL8	DvlEditObjRectGet( __u16 A_ScptId, Ptr_GrRect A_PtrRtRect );
		__u16	DvlEditObjFindByPos( __s32 A_X, __s32 A_Y );
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
		BOOL8	DvlEditObjInfoT1( __u16 A_ScptId, __u8* A_PtrRtClassType, Ptr_GrRect A_PtrRtRect, __u16* A_PtrRtSlaveId,
			__u16* A_PtrRtNextId );

		WCHAR*	DvlModelNameGet( __u8 A_ModelIdx );
		void	DvlModelNameSet( __u8 A_ModelIdx, WCHAR* A_StrName );
		__u16	DvlModelNumberGet( __u8 A_ModelIdx );
		void	DvlModelNumberSet( __u8 A_ModelIdx, __u16 A_Number );
		void	DvlModelCopyTo( __u8 A_TargetIdx );

		WCHAR*	DvlSkinNameGet( __u8 A_SkinIdx );
		void	DvlSkinNameSet( __u8 A_SkinIdx, WCHAR* A_StrName );
		void	DvlSkinCopy( __u8 A_TgIdx, __u8 A_SrIdx );

		Ptr_GrP3ScptPstData	DvlPstCreate( __u16 A_Id );
		void	DvlPstCodeCountSet( __u16 A_Id, __u8 A_Cnt );
		WCHAR*	DvlPstCodeNameGet( __u16 A_Id );
		void	DvlPstCodeNameSet( __u16 A_Id, WCHAR* A_StrName );

		BOOL8	DvlImgLoadBmp( __u16 A_Id, WCHAR* A_StrFn );
		void	DvlImgDelete( __u16 A_Id );

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

		void*	DvlSruTblGet( void );
		__u32	DvlSruCntGet( void );
		void	DvlSruCntSet( __u32 A_Cnt );

		void*	DvlSetupSysTblGet( void );
		__u32	DvlSetupSysCntGet( void );

		void*	DvlGvuInfoTblGet( void );

		void*	DvlAsmCreate( __u16 A_Id );
		void	DvlAsmCodeCountSet( __u16 A_Id, __u16 A_Cnt );		/* not use function */
		WCHAR*	DvlAsmFuncNameGet( __u16 A_Id );
		void	DvlAsmFuncNameSet( __u16 A_Id, WCHAR* A_StrName );
		WCHAR*	DvlAsmFuncCmtGet( __u16 A_Id );
		void	DvlAsmFuncCmtSet( __u16 A_Id, WCHAR* A_StrCmt );
		BOOL8	DvlAsmBreakPointToggle( __u16 A_Id, __u16 A_Ip );
		BOOL8	DvlAsmActiveToggle( __u16 A_Id, __u16 A_Ip );

		WCHAR*	DvlPathImageGet( void );
		WCHAR*	DvlPathFontGet( void );
		WCHAR*	DvlPathHexpGet( void );
		WCHAR*	DvlPathTextGet( void );
		WCHAR*	DvlPathScriptGet( void );
		WCHAR*	DvlPathBldSrGet( void );
		WCHAR*	DvlPathBldTgGet( void );

		Ptr_GrP3PrjBldFile	DvlBldFileInfoGet( __u8 A_ModelIdx );

		void	DvlVkbdTxtPageCntSet( __u8 A_Lang , __u8 A_Cnt );
		void	DvlVkbdTxtKeyCodeSet( __u8 A_Page, __u8 A_Key, __u8 A_Lang, WCHAR A_KeyCode );

		void*	DvlModelAtbGet( __u8 A_ModelIdx );
		void*	DvlModelDbgGet( __u8 A_ModelIdx );

		Ptr_GrP3DvlObjInfo	DvlDispIdTblGet( void );

		void	DvlDbgStart( void );
		void	DvlDbgFinish( void );

		void	DvlDbgResume( void );
		void	DvlDbgStep( void );

		BOOL8	DvlIsDbgPause( void );
		__u16	DvlDbgVmCsGet( void );
		__u16	DvlDbgVmIpGet( void );

		BOOL8	DvlIsDbgRun( void );			// use object
		
		void	DvlFwUpgdRun( __u32 A_CtlgHndl );
		BOOL8	DvlBkupReq( __u8 A_Media, __u8 A_RecDev, __u16 A_ChMap,
			Def_GrTime A_TimeSt, Def_GrTime A_TimeEd, BOOL8 A_IsCmbViewer );
		void	DvlBkupCancel( void );

		BOOL8	DvlLogBkupReq( __u8 A_RecDev, __u16 A_ChMap, __u16 A_EvtMap,
			Def_GrTime A_TimeSt, Def_GrTime A_TimeEd, WCHAR* A_StrFn );
		void	DvlLogBkupCancel( void );

		// inherited
		virtual	__u32	DvlConResX( __u8 A_ConIdx );
		virtual	__u32	DvlConResY( __u8 A_ConIdx );

		virtual	BOOL8	VmRunCode( __u16 A_CodeId, Cls_GrP3DoBase* A_ObjHost, Ptr_GrP3MngVmRpara A_PtrRpara = NULL );

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

