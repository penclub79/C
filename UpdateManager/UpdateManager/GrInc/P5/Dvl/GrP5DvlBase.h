/*
	Platform 5 Developer base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5DvlBase
#define	_EPC_GrP5DvlBase

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>

#include <GrError.h>

#include <GrTaskSvr.h>
#include <GrTaskCli.h>

#include <P5/GrP5Base.h>

#include <P5/GrP5Script.h>
#include <P5/GrP5Text.h>

//====================================================================
//constance

// script id
#define	E_GrP5DvlScptIdReqAlloc					0xFFFE			/* request allocate */

#define	E_GrP5DvlScptSlaveMaxCnt				10000				/* max slave count */

#define	E_GrP5DvlRintMaxCnt							256
#define	E_GrP5DvlRtxtMaxCnt							64

#define	E_GrP5DvlSetupAdrMaxCnt					256

#define	E_GrP5DvlWorkEnvPathCnt					8
#define	E_GrP5DvlWorkEnvPathMaxLen			255

#define	E_GrP5DvlTextMaxLen							1023

#define	E_GrP5DvlScptSlaveMapSize				(E_GrP5DvlScptSlaveMaxCnt >> 3)

#define	E_GrP5DvlTestEnvDfltResX				1920
#define	E_GrP5DvlTestEnvDfltResY				1080

// parametter info action
#define	E_GrP5DvlParaInfoActNone						0
#define	E_GrP5DvlParaInfoActPosSizeChg			1
#define	E_GrP5DvlParaInfoActFontParaChg			2

#define	E_GrP5DvlParaInfoActDefault					0xFF

// custom value type
#define	E_GrP5DvlValAtbStart				(0 + E_GrVarAtbUserStart)

#define	E_GrP5DvlValTypeSeparator		(((0 + E_GrP5DvlValAtbStart) << E_GrVarAtbBitPos) | 0)
#define	E_GrP5DvlValTypeTextId			(((1 + E_GrP5DvlValAtbStart) << E_GrVarAtbBitPos) | E_GrVarLen2Byte)
#define	E_GrP5DvlValTypeSkinId			(((2 + E_GrP5DvlValAtbStart) << E_GrVarAtbBitPos) | E_GrVarLen1Byte)
#define	E_GrP5DvlValTypeClassId			(((3 + E_GrP5DvlValAtbStart) << E_GrVarAtbBitPos) | E_GrVarLen2Byte)
#define	E_GrP5DvlValTypeCtrlId			(((4 + E_GrP5DvlValAtbStart) << E_GrVarAtbBitPos) | E_GrVarLen2Byte)
#define	E_GrP5DvlValTypeRintId			(((5 + E_GrP5DvlValAtbStart) << E_GrVarAtbBitPos) | E_GrVarLen2Byte)
#define	E_GrP5DvlValTypeRtxtId			(((6 + E_GrP5DvlValAtbStart) << E_GrVarAtbBitPos) | E_GrVarLen2Byte)
#define	E_GrP5DvlValTypePath				(((7 + E_GrP5DvlValAtbStart) << E_GrVarAtbBitPos) | E_GrVarLen2Byte)
#define	E_GrP5DvlValTypeSetupAdr		(((8 + E_GrP5DvlValAtbStart) << E_GrVarAtbBitPos) | E_GrVarLen2Byte)
#define	E_GrP5DvlValTypeVkbdType		(((9 + E_GrP5DvlValAtbStart) << E_GrVarAtbBitPos) | E_GrVarLen1Byte)

// data id
#define	E_GrP5DvlDataIdBase					100
#define	E_GrP5DvlDataIdWinInfo			(0 + E_GrP5DvlDataIdBase)
#define	E_GrP5DvlDataIdSlaveIdMap		(1 + E_GrP5DvlDataIdBase)
#define	E_GrP5DvlDataIdUiTest				(2 + E_GrP5DvlDataIdBase)
#define	E_GrP5DvlDataIdWorkEnv			(3 + E_GrP5DvlDataIdBase)


//====================================================================
//global macro


//====================================================================
//global type

typedef	void(*Fnc_GrP5DvlRegChgEvt)(BOOL8 A_IsVarText, __u16 A_VarId);
typedef	void(*Fnc_GrP5DvlRefChgEvt)(void);

typedef struct St_GrP5DvlInfoIdName
{
	__u32	Id;
	Def_WstrName	StrName;
}	*Ptr_GrP5DvlInfoIdName;

typedef struct St_GrP5DvlScptParaInfo
{
	WCHAR*	StrName;
	__u16		ValType;
	__u16		ValCnt;
	void*		PtrVal;
	BOOL8		IsEdit;
	__u32		Act;
	WCHAR*	StrDesc;
}*Ptr_GrP5DvlScptParaInfo;

/*
typedef	struct St_GrP5DvlScptPrjInfo
{
	__u32	ScrFcc;			// screen gdib fcc

}	*Ptr_GrP5DvlScptPrjInfo;
*/

typedef struct St_GrP5DvlCtrlIdInfo
{
	Def_WstrName	StrName;
}	*Ptr_GrP5DvlCtrlIdInfo;

typedef struct St_GrP5DvlVarIdInfo
{
	Def_WstrName	StrName;
}	*Ptr_GrP5DvlVarIdInfo;

typedef struct St_GrP5DvlWinInfo
{
	Def_WstrName	StrName;
}	*Ptr_GrP5DvlWinInfo;

typedef struct St_GrP5DvlWorkEnv
{
	Def_WstrGrFnPath	StrPathTbl[E_GrP5DvlWorkEnvPathCnt];		// excute export path
	Def_WstrGrFnPath	StrPathRefHd;		// reference header file name
	Def_StrTag				StrRefHdPrefix;	// prefix reference header
}	*Ptr_GrP5DvlWorkEnv;

typedef struct St_GrP5DvlUiTestCtrl
{
	__u32 ResX;
	__u32	ResY;

}*Ptr_GrP5DvlUiTestCtrl;

typedef struct St_GrP5DvlSetupAdr
{
	Def_WstrName	StrName;
	__u32	Ofs;		// offset
}	*Ptr_GrP5DvlSetupAdr;

typedef struct St_GrP5DvlPrjCtrl
{
	St_GrP5ScptUiInfoBasic	UiInfoBasic;
	
	St_GrP5DvlCtrlIdInfo	CtrlIdInfoTbl[E_GrP5CtrlIdMaxCnt];
	St_GrP5DvlVarIdInfo		RintInfoTbl[E_GrP5DvlRintMaxCnt];
	St_GrP5DvlVarIdInfo		RtxtInfoTbl[E_GrP5DvlRtxtMaxCnt];
	St_GrP5DvlWinInfo		WinInfoTbl[E_GrP5ScptWinMaxCnt];
	__u8	ScptSlvIdMap[E_GrP5DvlScptSlaveMapSize];
	
	WCHAR	StrTxtFixTbl[E_GrP5ScptLanguageMaxCnt][E_GrP5TxtDedicatedCnt][E_GrP5DvlTextMaxLen + 1];
	WCHAR	StrTxtUserTbl[E_GrP5ScptLanguageMaxCnt][E_GrP5ScptTxtUserMaxCnt][E_GrP5DvlTextMaxLen + 1];

	__u32	StpAdrCnt;
	St_GrP5DvlSetupAdr	StpAdrTbl[E_GrP5DvlSetupAdrMaxCnt];

	St_GrP5DvlUiTestCtrl	UiTest;

	St_GrP5DvlWorkEnv	WorkEnv;

	St_GrP5VkbdCfg	VkbdCfg;

}	*Ptr_GrP5DvlPrjCtrl;

typedef struct St_GrP5DvlDebug
{
	Fnc_GrP5DvlRegChgEvt	FncEvtRegChg;

	__s32	RintTbl[E_GrP5DvlRintMaxCnt];
	Def_WstrTag	RtxtTbl[E_GrP5DvlRtxtMaxCnt];

}	*Ptr_GrP5DvlDebug;

typedef struct St_GrP5DvlEnv
{
	St_GrP5DvlPrjCtrl	PrjCtrl;

	// header parser
	Cls_GrTaskSvr*	TaskRefPrs;
	Cls_GrTaskCli*	ObjRefPrs;
	Fnc_GrP5DvlRefChgEvt	FncEvtRefChg;

	// object mark
	__u32	ClrUoMark1;
	__u32	ClrUoMark2;

	// debug
	St_GrP5DvlDebug	Debug;

}	*Ptr_GrP5DvlEnv;


//====================================================================
//global class


//====================================================================
//global function

Def_GrErrCode	GrP5DvlInit(Cls_GrTaskSvr* A_TaskSvr);
Def_GrErrCode	GrP5DvlFinish(void);

WCHAR*	GrP5DvlClassNameGet(__u16 A_ClassId);
WCHAR*	GrP5DvlScptGetCommentPtr(__u16 A_ScptId);

__u32	GrP5DvlScptWindowBuild(void* A_PtrBuf,__u16 A_ScptId, __u8 A_Layer, Ptr_GrP5RelRect A_PtrRelRect,__u16 A_ClassId = E_GrP5ClassIdWinStd);
//__u32	GrP5DvlScptBuildPage(void* A_PtrBuf, __u16 A_ScptId);

BOOL8	GrP5DvlScptWindowCreate(__u16 A_ScptId, __u16 A_ClassId = E_GrP5ClassIdWinStd);
BOOL8	GrP5DvlScptWindowDelete(__u16 A_ScptId);
BOOL8	GrP5DvlScptWindowSet(__u16 A_ScptId, void* A_PtrScpt, __u32 A_ScptSize);

void	GrP5DvlScptSlaveIdFree(void* A_PtrScpt, __u32 A_ScptSize);

__u16	GrP5DvlWindowScptDecode(__u16 A_ScptId, __u8 A_ConIdx);	// retrun = object id

void*	GrP5DvlUiObjEditableFindByPos(__u8 A_ConIdx, __s32 A_PosX, __s32 A_PosY);

Ptr_GrP5DvlWinInfo	GrP5DvlWindowInfoPtrGet(__u16 A_ScptId);
Ptr_GrP5DvlCtrlIdInfo	GrP5DvlCtrlIdInfoPtrGet(__u16 A_CtrlId);
Ptr_GrP5DvlVarIdInfo	GrP5DvlRintInfoPtrGet(__u16 A_VarId);
Ptr_GrP5DvlVarIdInfo	GrP5DvlRtxtInfoPtrGet(__u16 A_VarId);

void	GrP5DvlSetupInfoGet(__u16 A_Adr, WCHAR* A_StrRt);

void*	GrP5DvlScriptDecSlave(void* A_PtrScpt, void* A_ObjPrnt, __u32* A_PtrRtDecoded, Ptr_GrP5RelRect A_PtrRelRect = NULL, BOOL8 A_IsScptIdNone = FALSE);

__u16	GrP5DvlSlaveScptIdAlloc(void);
void	GrP5DvlSlaveScptIdFree(__u16 A_ScptId);

WCHAR*	GrP5DvlWorkEnvPathGet(__u8 A_Idx);

void	GrP5DvlPrjNew(__u32 A_PxFmt);

BOOL8	GrP5DvlPrjSave(WCHAR* A_StrFn, BOOL8 A_IsPrjInc);
BOOL8	GrP5DvlPrjLoad(WCHAR* A_StrFn);

void	GrP5DvlDbgRegChgEvtSet(Fnc_GrP5DvlRegChgEvt A_Fnc);
void	GrP5DvlDbgRegChgNotify(BOOL8 A_IsText, __u16 A_VarId);

void	GrP5DvlRefHeaderChgEvtSet(Fnc_GrP5DvlRefChgEvt A_Fnc);
void	GrP5DvlRefHeaderChgNotify(void);


//====================================================================
//out Environment conditional
#endif

