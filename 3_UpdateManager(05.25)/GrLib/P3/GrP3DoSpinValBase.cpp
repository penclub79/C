/*
		platform spin value base

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P3/GrP3DoSpinValBase.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<GrStrTool.h>
#include	<P3/GrP3Script.h>
#include	<P3/GrP3ConBase.h>
#include	<P3/GrP3AsmBase.h>
#include	<P3/GrP3Mng.h>
#include	<P3/GrP3UiCmd.h>

#ifdef GR_P3_EDITOR
#include	<P3/Dvl/GrP3DvlBase.h>
#include	<P3/Dvl/GrP3DvlMng.h>
#endif

//====================================================================
//local const

//====================================================================
//local type

//====================================================================
//local macro

//====================================================================
//local function

//====================================================================
//var
extern	Cls_GrP3Mng*	V_GrP3Mng;

#ifdef GR_P3_EDITOR

#define E_GrP3DoSpinValBaseParaCnt		73

St_GrP3DvlDoPrInfoData	V_GrP3DoSpinValBaseParaInfo[E_GrP3DoSpinValBaseParaCnt]	=	
{
	{	L"Paint Disable",			TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->PntStat[E_GrP3DoSpinValStatDisable],NULL,	L"Paint status disable"	},
	{	L"Paint Normal",			TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->PntStat[E_GrP3DoSpinValStatNormal],NULL,	L"Paint status normal"	},
	{	L"Paint Focus",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->PntStat[E_GrP3DoSpinValStatFocus],NULL,	L"Paint status focus"	},
	{	L"Paint Control",			TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->PntStat[E_GrP3DoSpinValStatControl],NULL,	L"Paint status control value"	},
	{	L"IsOnKeyCtrl",				TRUE,		E_GrP3DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->IsOnKeyCtrl,NULL,	L"anytime value control by key"	},
	{	L"IsVertical",				TRUE,		E_GrP3DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->IsVertical,NULL,	L"vertical type"	},
	{	L"IsValRotate",				TRUE,		E_GrP3DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->IsValRotate,NULL,	L"value rotate"	},
	{	L"AsmEvtValChg",			TRUE,		E_GrP3DvlParaTypeAsmCode,				1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->AsmEvtValChg,NULL,	L"assembly code id for the value change event"	},
	{	L"ItemCnt",						TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->ItemCnt,NULL,	L"item count"	},
	{	L"TxtId0",						TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->TxtTbl[0],NULL,	L"text id 0"	},
	{	L"TxtId1",						TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->TxtTbl[1],NULL,	L"text id 1"	},
	{	L"TxtId2",						TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->TxtTbl[2],NULL,	L"text id 2"	},
	{	L"TxtId3",						TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->TxtTbl[3],NULL,	L"text id 3"	},
	{	L"TxtId4",						TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->TxtTbl[4],NULL,	L"text id 4"	},
	{	L"TxtId5",						TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->TxtTbl[5],NULL,	L"text id 5"	},
	{	L"TxtId6",						TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->TxtTbl[6],NULL,	L"text id 6"	},
	{	L"TxtId7",						TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->TxtTbl[7],NULL,	L"text id 7"	},
	{	L"TxtId8",						TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->TxtTbl[8],NULL,	L"text id 8"	},
	{	L"TxtId9",						TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->TxtTbl[9],NULL,	L"text id 9"	},
	{	L"TxtId10",						TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->TxtTbl[10],NULL,	L"text id 10"	},
	{	L"TxtId11",						TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->TxtTbl[11],NULL,	L"text id 11"	},
	{	L"TxtId12",						TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->TxtTbl[12],NULL,	L"text id 12"	},
	{	L"TxtId13",						TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->TxtTbl[13],NULL,	L"text id 13"	},
	{	L"TxtId14",						TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->TxtTbl[14],NULL,	L"text id 14"	},
	{	L"TxtId15",						TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->TxtTbl[15],NULL,	L"text id 15"	},
	{	L"TxtId16",						TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->TxtTbl[16],NULL,	L"text id 16"	},
	{	L"TxtId17",						TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->TxtTbl[17],NULL,	L"text id 17"	},
	{	L"TxtId18",						TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->TxtTbl[18],NULL,	L"text id 18"	},
	{	L"TxtId19",						TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->TxtTbl[19],NULL,	L"text id 19"	},
	{	L"TxtId20",						TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->TxtTbl[20],NULL,	L"text id 20"	},
	{	L"TxtId21",						TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->TxtTbl[21],NULL,	L"text id 21"	},
	{	L"TxtId22",						TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->TxtTbl[22],NULL,	L"text id 22"	},
	{	L"TxtId23",						TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->TxtTbl[23],NULL,	L"text id 23"	},
	{	L"TxtId24",						TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->TxtTbl[24],NULL,	L"text id 24"	},
	{	L"TxtId25",						TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->TxtTbl[25],NULL,	L"text id 25"	},
	{	L"TxtId26",						TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->TxtTbl[26],NULL,	L"text id 26"	},
	{	L"TxtId27",						TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->TxtTbl[27],NULL,	L"text id 27"	},
	{	L"TxtId28",						TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->TxtTbl[28],NULL,	L"text id 28"	},
	{	L"TxtId29",						TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->TxtTbl[29],NULL,	L"text id 29"	},
	{	L"TxtId30",						TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->TxtTbl[30],NULL,	L"text id 30"	},
	{	L"TxtId31",						TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->TxtTbl[31],NULL,	L"text id 31"	},
	{	L"Val0",							TRUE,		E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->ValTbl[0],NULL,		L"value 0"	},
	{	L"Val1",							TRUE,		E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->ValTbl[1],NULL,		L"value 1"	},
	{	L"Val2",							TRUE,		E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->ValTbl[2],NULL,		L"value 2"	},
	{	L"Val3",							TRUE,		E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->ValTbl[3],NULL,		L"value 3"	},
	{	L"Val4",							TRUE,		E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->ValTbl[4],NULL,		L"value 4"	},
	{	L"Val5",							TRUE,		E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->ValTbl[5],NULL,		L"value 5"	},
	{	L"Val6",							TRUE,		E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->ValTbl[6],NULL,		L"value 6"	},
	{	L"Val7",							TRUE,		E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->ValTbl[7],NULL,		L"value 7"	},
	{	L"Val8",							TRUE,		E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->ValTbl[8],NULL,		L"value 8"	},
	{	L"Val9",							TRUE,		E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->ValTbl[9],NULL,		L"value 9"	},
	{	L"Val10",							TRUE,		E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->ValTbl[10],NULL,	L"value 10"	},
	{	L"Val11",							TRUE,		E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->ValTbl[11],NULL,	L"value 11"	},
	{	L"Val12",							TRUE,		E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->ValTbl[12],NULL,	L"value 12"	},
	{	L"Val13",							TRUE,		E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->ValTbl[13],NULL,	L"value 13"	},
	{	L"Val14",							TRUE,		E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->ValTbl[14],NULL,	L"value 14"	},
	{	L"Val15",							TRUE,		E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->ValTbl[15],NULL,	L"value 15"	},
	{	L"Val16",							TRUE,		E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->ValTbl[16],NULL,	L"value 16"	},
	{	L"Val17",							TRUE,		E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->ValTbl[17],NULL,	L"value 17"	},
	{	L"Val18",							TRUE,		E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->ValTbl[18],NULL,	L"value 18"	},
	{	L"Val19",							TRUE,		E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->ValTbl[19],NULL,	L"value 19"	},
	{	L"Val20",							TRUE,		E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->ValTbl[20],NULL,	L"value 20"	},
	{	L"Val21",							TRUE,		E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->ValTbl[21],NULL,	L"value 21"	},
	{	L"Val22",							TRUE,		E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->ValTbl[22],NULL,	L"value 22"	},
	{	L"Val23",							TRUE,		E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->ValTbl[23],NULL,	L"value 23"	},
	{	L"Val24",							TRUE,		E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->ValTbl[24],NULL,	L"value 24"	},
	{	L"Val25",							TRUE,		E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->ValTbl[25],NULL,	L"value 25"	},
	{	L"Val26",							TRUE,		E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->ValTbl[26],NULL,	L"value 26"	},
	{	L"Val27",							TRUE,		E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->ValTbl[27],NULL,	L"value 27"	},
	{	L"Val28",							TRUE,		E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->ValTbl[28],NULL,	L"value 28"	},
	{	L"Val29",							TRUE,		E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->ValTbl[29],NULL,	L"value 29"	},
	{	L"Val30",							TRUE,		E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->ValTbl[30],NULL,	L"value 30"	},
	{	L"Val31",							TRUE,		E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3DoSpinValBasePara)0)->ValTbl[31],NULL,	L"value 31"	},
};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP3DoSpinValBase::Cls_GrP3DoSpinValBase( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt ):
Cls_GrP3DoBase( A_ScptId, A_PtrRect, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP3DoClassSpinValBase;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaSpinValBase) + sizeof(St_GrP3ScptParaHd);
		
		m_IsKeyAble	=	TRUE;
		m_IsMouseAble	=	TRUE;

		GrDumyZeroMem( &m_ParaSpinValBase, sizeof(m_ParaSpinValBase) );

		m_IsCtrlMode	=	FALSE;
		m_Value				=	0;
		m_ItemIdx			=	0;

}
//--------------------------------------------------------------------
Cls_GrP3DoSpinValBase::~Cls_GrP3DoSpinValBase( void )
{
		
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinValBase::RtlScptParaSubDump( void )
{
#ifdef GR_P3_EDITOR
	// local -------------------
		Ptr_GrP3ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherited
		Cls_GrP3DoBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP3ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP3ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP3DoClassSpinValBase;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaSpinValBase);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP3ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaSpinValBase, sizeof(m_ParaSpinValBase) );
		//update progress
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaSpinValBase)	+ sizeof(St_GrP3ScptParaHd);
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinValBase::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaSpinValBase, sizeof(m_ParaSpinValBase) , m_PtrScpt , E_GrP3DoClassSpinValBase );

		// check item count over
		if ( E_GrP3DoSpinValItemMax < m_ParaSpinValBase.ItemCnt )
		{
			m_ParaSpinValBase.ItemCnt	=	E_GrP3DoSpinValItemMax;
		}
		// update
		LcValToItemIdx();
		ReqDraw();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoSpinValBase::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_Index;
	// code --------------------
		Tv_Result	=	TRUE;
		switch ( A_Cmd )
		{
			case E_GrP3UpiValueGet:
				LcValLoad();
				A_PtrInt[0]	=	m_Value;
				break;
			case E_GrP3UpiValueSet:
				m_Value	=	A_PtrInt[1];
				LcValSave();
				LcValToItemIdx();
				ReqDraw();
				break;
			case E_GrP3UpiItemTxtIdSet:
				Tv_Index	=	(__u32)A_PtrInt[1];
				if ( E_GrP3DoSpinValItemMax > Tv_Index )
				{
					m_ParaSpinValBase.TxtTbl[Tv_Index]	=	(__u16)A_PtrInt[2];
					ReqDraw();
				}
				break;
			default:
				Tv_Result	=	Cls_GrP3DoBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinValBase::ProcPaint( void )
{
	// local -------------------
		__u8	Tv_Stat;
	// code --------------------
		// check value change
		LcValLoad();
		LcValToItemIdx();
		// update status
		Tv_Stat	=	E_GrP3DoSpinValStatDisable;
		if ( IsCanEnable() )
		{
			Tv_Stat	=	E_GrP3DoSpinValStatNormal;
			if ( m_IsCtrlMode ||  m_IsOnMouse )
			{
				Tv_Stat	=	E_GrP3DoSpinValStatControl;
			}
			else
			{
				if ( IsFocused() )
				{
					Tv_Stat	=	E_GrP3DoSpinValStatFocus;
					if ( m_ParaSpinValBase.IsOnKeyCtrl )
					{
						Tv_Stat	=	E_GrP3DoSpinValStatControl;
					}
				}
			}
		}
		PaintScriptPush( m_ParaSpinValBase.PntStat[Tv_Stat], &m_PntRcScr );
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinValBase::RtlDvlParaInfoSubDump( void )
{
#ifdef GR_P3_EDITOR
		// inherited
		Cls_GrP3DoBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaSpinValBase, V_GrP3DoSpinValBaseParaInfo, E_GrP3DoSpinValBaseParaCnt );
#endif
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP3DoSpinValBase::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
{
	// local -------------------
		WCHAR*	Tv_Result;
	// code --------------------
		Tv_Result		=	NULL;
		*A_PtrRtCus	=	-1;
		if ( 0 == A_Idx )
		{
			Tv_Result	=	V_GrP3Mng->TextPtrGet( m_ParaSpinValBase.TxtTbl[m_ItemIdx] );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinValBase::LcNotifyValChg( void )
{
	// local -------------------
		//St_GrP3MsgObj	Tv_Msg;
		St_GrP3MngVmRpara	Tv_Rpara;
	// code --------------------
		// relate event

		// user event
		if ( V_GrP3Mng->IsUiEventAble() )
		{
			// run event assembly
			Tv_Rpara.Cnt	=	1;
			Tv_Rpara.Ridx[0]	=	1;
			Tv_Rpara.Rval[0]	=	m_Value;
			V_GrP3Mng->VmRunCode( m_ParaSpinValBase.AsmEvtValChg, this, &Tv_Rpara );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinValBase::ProcEvtFocusGet( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoBase::ProcEvtFocusGet();
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinValBase::ProcEvtFocusLoss( void )
{
	// local -------------------
	// code --------------------
		m_IsCtrlMode	=	FALSE;
		Cls_GrP3DoBase::ProcEvtFocusLoss();
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinValBase::LcValIncrease( void )
{
	// local -------------------
	// code --------------------
		LcValLoad();
		LcValToItemIdx();
		m_ItemIdx ++;
		if ( m_ParaSpinValBase.ItemCnt <= m_ItemIdx )
		{
			if ( m_ParaSpinValBase.IsValRotate )
			{
				m_ItemIdx	=	0;
			}
			else
			{
				m_ItemIdx	=	m_ParaSpinValBase.ItemCnt;
				if ( 0 != m_ItemIdx )
				{
					m_ItemIdx --;
				}
			}
		}
		LcItemIdxToVal();
		LcValSave();
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinValBase::LcValDecrease( void )
{
	// local -------------------
	// code --------------------
		LcValLoad();
		LcValToItemIdx();
		if ( 0 == m_ItemIdx )
		{
			if ( m_ParaSpinValBase.IsValRotate )
			{
				m_ItemIdx	=	m_ParaSpinValBase.ItemCnt;
				if ( 0 != m_ItemIdx )
				{
					m_ItemIdx --;
				}
			}
		}
		else
		{
			m_ItemIdx --;
		}
		LcItemIdxToVal();
		LcValSave();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoSpinValBase::ProcEvtKeyDown( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( m_ParaSpinValBase.IsOnKeyCtrl || m_IsCtrlMode )
		{
			switch ( A_Key )
			{
				case E_GrP3KeyLeft:
					if ( !m_ParaSpinValBase.IsVertical )
					{
						LcValDecrease();
						Tv_Result	=	TRUE;
					}
					break;
				case E_GrP3KeyRight:
					if ( !m_ParaSpinValBase.IsVertical )
					{
						LcValIncrease();
						Tv_Result	=	TRUE;
					}
					break;
				case E_GrP3KeyUp:
					if ( m_ParaSpinValBase.IsVertical )
					{
						LcValIncrease();
						Tv_Result	=	TRUE;
					}
					break;
				case E_GrP3KeyDown:
					if ( m_ParaSpinValBase.IsVertical )
					{
						LcValDecrease();
						Tv_Result	=	TRUE;
					}
					break;

			}
		}

		// change event
		if ( Tv_Result )
		{
			ReqDraw();
			LcNotifyValChg();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoSpinValBase::ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( !m_ParaSpinValBase.IsOnKeyCtrl )
		{
			if ( E_GrP3KeyEnter == A_Key )
			{
				m_IsCtrlMode	=	!m_IsCtrlMode;
				ReqDraw();
				Tv_Result	=	TRUE;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoSpinValBase::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
		St_GrRect	Tv_Rc;
		__s32		Tv_Ct;
	// code --------------------
		// check position
		if ( ScrRectGet( &Tv_Rc ) )
		{
			if ( m_ParaSpinValBase.IsVertical )
			{
				// vertical area calculate
				Tv_Ct	=	((Tv_Rc.bottom - Tv_Rc.top) >> 1) + Tv_Rc.top;
				if ( Tv_Ct > (__s32)A_Y )
				{
					LcValIncrease();
				}
				else
				{
					LcValDecrease();
				}
			}
			else
			{
				// horizontal area calculate
				Tv_Ct	=	((Tv_Rc.right - Tv_Rc.left) >> 1) + Tv_Rc.left;
				if ( Tv_Ct > (__s32)A_X )
				{
					LcValDecrease();
				}
				else
				{
					LcValIncrease();
				}
			}
			ReqDraw();
			LcNotifyValChg();
		}
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoSpinValBase::ProcEvtWheelUp( void )
{
	// local -------------------
	// code --------------------
		LcValIncrease();
		ReqDraw();
		LcNotifyValChg();
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoSpinValBase::ProcEvtWheelDown( void )
{
	// local -------------------
	// code --------------------
		LcValDecrease();
		ReqDraw();
		LcNotifyValChg();
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinValBase::RtlInit( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoBase::RtlInit();
		LcValLoad();
		LcValToItemIdx();
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinValBase::LcValLoad( void )
{

}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinValBase::LcValSave( void )
{

}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinValBase::LcValToItemIdx( void )
{
	// local -------------------
		__u8	Tv_ItmIdx;
		__u8	Tv_WkIdx;
	// code --------------------
		Tv_ItmIdx	=	0;
		if ( 0 != m_ParaSpinValBase.ItemCnt )
		{
			for ( Tv_WkIdx=0;Tv_WkIdx < m_ParaSpinValBase.ItemCnt;Tv_WkIdx++ )
			{
				if ( m_Value == m_ParaSpinValBase.ValTbl[Tv_WkIdx] )
				{
					Tv_ItmIdx	=	Tv_WkIdx;
					break;
				}
			}
		}
		m_ItemIdx	=	Tv_ItmIdx;
		
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinValBase::LcItemIdxToVal( void )
{
	// local -------------------
	// code --------------------
		if ( 0 != m_ParaSpinValBase.ItemCnt )
		{
			m_Value	=	m_ParaSpinValBase.ValTbl[m_ItemIdx];
		}
}
//--------------------------------------------------------------------
