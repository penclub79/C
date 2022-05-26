/*
		platform spin paint

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P4/GrP4DoSpinPnt.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<GrStrTool.h>
#include	<P4/GrP4Script.h>
#include	<P4/GrP4ConBase.h>
#include	<P4/GrP4AsmBase.h>
#include	<P4/GrP4Mng.h>
#include	<P4/GrP4UiCmd.h>

#ifdef GR_P4_EDITOR
#include	<P4/Dvl/GrP4DvlBase.h>
#include	<P4/Dvl/GrP4DvlMng.h>
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
extern	Cls_GrP4Mng*	V_GrP4Mng;

#ifdef GR_P4_EDITOR

#define E_GrP4DoSpinPntParaCnt		37

St_GrP4DvlDoPrInfoData	V_GrP4DoSpinPntParaInfo[E_GrP4DoSpinPntParaCnt]	=	
{
	{	L"Paint 0",						TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoSpinPntPara)0)->PntList[0],NULL,	L"Paint list"	},
	{	L"Paint 1",						TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoSpinPntPara)0)->PntList[1],NULL,	L"Paint list"	},
	{	L"Paint 2",						TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoSpinPntPara)0)->PntList[2],NULL,	L"Paint list"	},
	{	L"Paint 3",						TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoSpinPntPara)0)->PntList[3],NULL,	L"Paint list"	},
	{	L"Paint 4",						TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoSpinPntPara)0)->PntList[4],NULL,	L"Paint list"	},
	{	L"Paint 5",						TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoSpinPntPara)0)->PntList[5],NULL,	L"Paint list"	},
	{	L"Paint 6",						TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoSpinPntPara)0)->PntList[6],NULL,	L"Paint list"	},
	{	L"Paint 7",						TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoSpinPntPara)0)->PntList[7],NULL,	L"Paint list"	},
	{	L"Paint 8",						TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoSpinPntPara)0)->PntList[8],NULL,	L"Paint list"	},
	{	L"Paint 9",						TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoSpinPntPara)0)->PntList[9],NULL,	L"Paint list"	},
	{	L"Paint 10",					TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoSpinPntPara)0)->PntList[10],NULL,	L"Paint list"	},
	{	L"Paint 11",					TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoSpinPntPara)0)->PntList[11],NULL,	L"Paint list"	},
	{	L"Paint 12",					TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoSpinPntPara)0)->PntList[12],NULL,	L"Paint list"	},
	{	L"Paint 13",					TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoSpinPntPara)0)->PntList[13],NULL,	L"Paint list"	},
	{	L"Paint 14",					TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoSpinPntPara)0)->PntList[14],NULL,	L"Paint list"	},
	{	L"Paint 15",					TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoSpinPntPara)0)->PntList[15],NULL,	L"Paint list"	},

	{	L"Text 0",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinPntPara)0)->TxtList[0],NULL,	L"text list"	},
	{	L"Text 1",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinPntPara)0)->TxtList[1],NULL,	L"text list"	},
	{	L"Text 2",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinPntPara)0)->TxtList[2],NULL,	L"text list"	},
	{	L"Text 3",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinPntPara)0)->TxtList[3],NULL,	L"text list"	},
	{	L"Text 4",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinPntPara)0)->TxtList[4],NULL,	L"text list"	},
	{	L"Text 5",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinPntPara)0)->TxtList[5],NULL,	L"text list"	},
	{	L"Text 6",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinPntPara)0)->TxtList[6],NULL,	L"text list"	},
	{	L"Text 7",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinPntPara)0)->TxtList[7],NULL,	L"text list"	},
	{	L"Text 8",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinPntPara)0)->TxtList[8],NULL,	L"text list"	},
	{	L"Text 9",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinPntPara)0)->TxtList[9],NULL,	L"text list"	},
	{	L"Text 10",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinPntPara)0)->TxtList[10],NULL,	L"text list"	},
	{	L"Text 11",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinPntPara)0)->TxtList[11],NULL,	L"text list"	},
	{	L"Text 12",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinPntPara)0)->TxtList[12],NULL,	L"text list"	},
	{	L"Text 13",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinPntPara)0)->TxtList[13],NULL,	L"text list"	},
	{	L"Text 14",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinPntPara)0)->TxtList[14],NULL,	L"text list"	},
	{	L"Text 15",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinPntPara)0)->TxtList[15],NULL,	L"text list"	},

	{	L"IsOnKeyCtrl",				TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4DoSpinPntPara)0)->IsOnKeyCtrl,NULL,	L"anytime value control by key"	},
	{	L"IsVertical",				TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4DoSpinPntPara)0)->IsVertical,NULL,	L"vertical type"	},
	{	L"IsValRotate",				TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4DoSpinPntPara)0)->IsValRotate,NULL,	L"value rotate"	},
	{	L"Item Count",				TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4DoSpinPntPara)0)->ItemCnt,NULL,	L"display cipher"	},

	{	L"AsmEvtValChg",			TRUE,		E_GrP4DvlParaTypeAsmCode,				1,	(void*)&((Ptr_GrP4DoSpinPntPara)0)->AsmEvtValChg,NULL,	L"asm value change event"	},
};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoSpinPnt::Cls_GrP4DoSpinPnt(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt) :
Cls_GrP4DoBase( A_ScptId, A_PtrRect,A_PosRel, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP4DoClassSpinPnt;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaDoSpinPnt) + sizeof(St_GrP4ScptParaHd);
		
		m_IsKeyAble	=	TRUE;
		m_IsMouseAble	=	TRUE;

		GrDumyZeroMem( &m_ParaDoSpinPnt, sizeof(m_ParaDoSpinPnt) );

		m_IsCtrlMode	=	FALSE;
		m_Value				=	0;

		LcValTxtUpdt();
}
//--------------------------------------------------------------------
Cls_GrP4DoSpinPnt::~Cls_GrP4DoSpinPnt( void )
{
		
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoSpinPnt::RtlScptParaSubDump(void)
{
	// local -------------------
		Ptr_GrP4ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherite
		Cls_GrP4DoBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP4ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP4ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP4DoClassSpinPnt;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaDoSpinPnt);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaDoSpinPnt, sizeof(m_ParaDoSpinPnt) );
		//update progress
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaDoSpinPnt)	+ sizeof(St_GrP4ScptParaHd);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinPnt::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaDoSpinPnt, sizeof(m_ParaDoSpinPnt) , m_PtrScpt , E_GrP4DoClassSpinPnt );

		// patch item max count
		if ( E_GrP4DoSpinPntItemCnt < m_ParaDoSpinPnt.ItemCnt )
		{
			m_ParaDoSpinPnt.ItemCnt	=	E_GrP4DoSpinPntItemCnt;		// patch item count
		}
		LcValTxtUpdt();		// text update
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoSpinPnt::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		if ( !Cls_GrP4DoBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr ) )
		{
			switch ( A_Cmd )
			{
				case E_GrP4UpiValueGet:
					A_PtrInt[0]	=	(__s32)m_Value;
					break;
				case E_GrP4UpiValueSet:
					m_Value	=	(__u8)A_PtrInt[1];
					LcValueUpdt( FALSE );
					break;
				default:
					Tv_Result	=	FALSE;
					break;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinPnt::ProcPaint( void )
{
	// local -------------------
		__u16	Tv_PstId;
	// code --------------------

		Tv_PstId	=	E_GrP4ScptPstIdNone;
		if ( m_Value < m_ParaDoSpinPnt.ItemCnt )
		{
			Tv_PstId	=	m_ParaDoSpinPnt.PntList[m_Value];
		}
		if ( E_GrP4ScptPstIdNone != Tv_PstId )
		{
			PaintScriptPush( Tv_PstId, &m_PntRcScr );
		}
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoSpinPnt::RtlDvlParaInfoSubDump(void)
{
		// inherited
		Cls_GrP4DoBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaDoSpinPnt, V_GrP4DoSpinPntParaInfo, E_GrP4DoSpinPntParaCnt );
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinPnt::LcValueUpdt( BOOL8 A_IsNotify )
{
		LcValTxtUpdt();
		if ( A_IsNotify )
		{
			LcNotifyValChg();
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinPnt::LcValTxtUpdt( void )
{
	// local -------------------
		WCHAR*	Tv_StrAdd;
	// code --------------------
		GrStrClear( m_StrTxt );		// text clear
		// check exist
		if ( m_Value < m_ParaDoSpinPnt.ItemCnt )
		{
			if ( E_GrP4ScptTxtIdNone != m_ParaDoSpinPnt.TxtList[m_Value] )
			{
				Tv_StrAdd	=	V_GrP4Mng->TextPtrGet( m_ParaDoSpinPnt.TxtList[m_Value] );
				GrStrWcopy( m_StrTxt, Tv_StrAdd, GrStrMaxLen(m_StrTxt) );
			}
		}

}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP4DoSpinPnt::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
{
	// local -------------------
		WCHAR*	Tv_Result;
	// code --------------------
		Tv_Result		=	NULL;
		*A_PtrRtCus	=	-1;
		if ( 0 == A_Idx )
		{
			Tv_Result	=	m_StrTxt;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinPnt::LcNotifyValChg( void )
{
	// local -------------------
		//St_GrP4MsgObj	Tv_Msg;
		St_GrP4MngVmRpara	Tv_Rpara;
	// code --------------------
		// relate event

		// user event
		if ( V_GrP4Mng->IsUiEventAble() )
		{
			// run event assembly
			Tv_Rpara.In.Cnt		=	0;

			Tv_Rpara.In.Ridx[Tv_Rpara.In.Cnt]	=	1;
			Tv_Rpara.In.Rval[Tv_Rpara.In.Cnt]	=	(__s32)((__u32)m_Value);
			Tv_Rpara.In.Cnt ++;

			Tv_Rpara.Out.Cnt	=	0;

			V_GrP4Mng->VmRunCode( m_ParaDoSpinPnt.AsmEvtValChg, this, &Tv_Rpara );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinPnt::ProcEvtFocusGet( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoBase::ProcEvtFocusGet();
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinPnt::ProcEvtFocusLoss( void )
{
	// local -------------------
	// code --------------------
		m_IsCtrlMode	=	FALSE;
		Cls_GrP4DoBase::ProcEvtFocusLoss();
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinPnt::LcValIncrease( void )
{
	// local -------------------
	// code --------------------
		m_Value ++;
		if ( m_ParaDoSpinPnt.ItemCnt <= m_Value )
		{
			if ( m_ParaDoSpinPnt.IsValRotate )
			{
				m_Value	=	0;
			}
			else
			{
				m_Value	=	m_ParaDoSpinPnt.ItemCnt;
				if ( 0 != m_Value )
				{
					m_Value --;
				}
			}
		}
		LcValueUpdt( FALSE );
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinPnt::LcValDecrease( void )
{
	// local -------------------
	// code --------------------
		if ( 0 != m_Value )
		{
			m_Value --;
		}
		else
		{
			if ( m_ParaDoSpinPnt.IsValRotate )
			{
				m_Value	=	m_ParaDoSpinPnt.ItemCnt;
				if ( 0 != m_Value )
				{
					m_Value --;
				}
			}
		}

		LcValueUpdt( FALSE );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoSpinPnt::ProcEvtKeyDown( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( m_ParaDoSpinPnt.IsOnKeyCtrl || m_IsCtrlMode )
		{
			switch ( A_Key )
			{
				case E_GrP4KeyLeft:
					if ( !m_ParaDoSpinPnt.IsVertical )
					{
						LcValDecrease();
						Tv_Result	=	TRUE;
					}
					break;
				case E_GrP4KeyRight:
					if ( !m_ParaDoSpinPnt.IsVertical )
					{
						LcValIncrease();
						Tv_Result	=	TRUE;
					}
					break;
				case E_GrP4KeyUp:
					if ( m_ParaDoSpinPnt.IsVertical )
					{
						LcValIncrease();
						Tv_Result	=	TRUE;
					}
					break;
				case E_GrP4KeyDown:
					if ( m_ParaDoSpinPnt.IsVertical )
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
BOOL8	Cls_GrP4DoSpinPnt::ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( !m_ParaDoSpinPnt.IsOnKeyCtrl )
		{
			if ( E_GrP4KeyEnter == A_Key )
			{
				m_IsCtrlMode	=	!m_IsCtrlMode;
				ReqDraw();
				Tv_Result	=	TRUE;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoSpinPnt::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
		St_GrRect	Tv_Rc;
		__s32		Tv_Ct;
	// code --------------------
		// check position
		if ( ScrRectGet( &Tv_Rc ) )
		{
			if ( m_ParaDoSpinPnt.IsVertical )
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
BOOL8	Cls_GrP4DoSpinPnt::ProcEvtWheelUp( void )
{
	// local -------------------
	// code --------------------
		LcValIncrease();
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoSpinPnt::ProcEvtWheelDown( void )
{
	// local -------------------
	// code --------------------
		LcValDecrease();
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinPnt::RtlInit( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoBase::RtlInit();
		ReqDraw();
}
//--------------------------------------------------------------------

