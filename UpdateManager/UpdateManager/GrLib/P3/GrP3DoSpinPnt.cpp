/*
		platform spin paint

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P3/GrP3DoSpinPnt.h>
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

#define E_GrP3DoSpinPntParaCnt		37

St_GrP3DvlDoPrInfoData	V_GrP3DoSpinPntParaInfo[E_GrP3DoSpinPntParaCnt]	=	
{
	{	L"Paint 0",						TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoSpinPntPara)0)->PntList[0],NULL,	L"Paint list"	},
	{	L"Paint 1",						TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoSpinPntPara)0)->PntList[1],NULL,	L"Paint list"	},
	{	L"Paint 2",						TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoSpinPntPara)0)->PntList[2],NULL,	L"Paint list"	},
	{	L"Paint 3",						TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoSpinPntPara)0)->PntList[3],NULL,	L"Paint list"	},
	{	L"Paint 4",						TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoSpinPntPara)0)->PntList[4],NULL,	L"Paint list"	},
	{	L"Paint 5",						TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoSpinPntPara)0)->PntList[5],NULL,	L"Paint list"	},
	{	L"Paint 6",						TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoSpinPntPara)0)->PntList[6],NULL,	L"Paint list"	},
	{	L"Paint 7",						TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoSpinPntPara)0)->PntList[7],NULL,	L"Paint list"	},
	{	L"Paint 8",						TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoSpinPntPara)0)->PntList[8],NULL,	L"Paint list"	},
	{	L"Paint 9",						TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoSpinPntPara)0)->PntList[9],NULL,	L"Paint list"	},
	{	L"Paint 10",					TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoSpinPntPara)0)->PntList[10],NULL,	L"Paint list"	},
	{	L"Paint 11",					TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoSpinPntPara)0)->PntList[11],NULL,	L"Paint list"	},
	{	L"Paint 12",					TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoSpinPntPara)0)->PntList[12],NULL,	L"Paint list"	},
	{	L"Paint 13",					TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoSpinPntPara)0)->PntList[13],NULL,	L"Paint list"	},
	{	L"Paint 14",					TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoSpinPntPara)0)->PntList[14],NULL,	L"Paint list"	},
	{	L"Paint 15",					TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoSpinPntPara)0)->PntList[15],NULL,	L"Paint list"	},

	{	L"Text 0",						TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoSpinPntPara)0)->TxtList[0],NULL,	L"text list"	},
	{	L"Text 1",						TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoSpinPntPara)0)->TxtList[1],NULL,	L"text list"	},
	{	L"Text 2",						TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoSpinPntPara)0)->TxtList[2],NULL,	L"text list"	},
	{	L"Text 3",						TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoSpinPntPara)0)->TxtList[3],NULL,	L"text list"	},
	{	L"Text 4",						TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoSpinPntPara)0)->TxtList[4],NULL,	L"text list"	},
	{	L"Text 5",						TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoSpinPntPara)0)->TxtList[5],NULL,	L"text list"	},
	{	L"Text 6",						TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoSpinPntPara)0)->TxtList[6],NULL,	L"text list"	},
	{	L"Text 7",						TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoSpinPntPara)0)->TxtList[7],NULL,	L"text list"	},
	{	L"Text 8",						TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoSpinPntPara)0)->TxtList[8],NULL,	L"text list"	},
	{	L"Text 9",						TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoSpinPntPara)0)->TxtList[9],NULL,	L"text list"	},
	{	L"Text 10",						TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoSpinPntPara)0)->TxtList[10],NULL,	L"text list"	},
	{	L"Text 11",						TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoSpinPntPara)0)->TxtList[11],NULL,	L"text list"	},
	{	L"Text 12",						TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoSpinPntPara)0)->TxtList[12],NULL,	L"text list"	},
	{	L"Text 13",						TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoSpinPntPara)0)->TxtList[13],NULL,	L"text list"	},
	{	L"Text 14",						TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoSpinPntPara)0)->TxtList[14],NULL,	L"text list"	},
	{	L"Text 15",						TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoSpinPntPara)0)->TxtList[15],NULL,	L"text list"	},

	{	L"IsOnKeyCtrl",				TRUE,		E_GrP3DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP3DoSpinPntPara)0)->IsOnKeyCtrl,NULL,	L"anytime value control by key"	},
	{	L"IsVertical",				TRUE,		E_GrP3DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP3DoSpinPntPara)0)->IsVertical,NULL,	L"vertical type"	},
	{	L"IsValRotate",				TRUE,		E_GrP3DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP3DoSpinPntPara)0)->IsValRotate,NULL,	L"value rotate"	},
	{	L"Item Count",				TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3DoSpinPntPara)0)->ItemCnt,NULL,	L"display cipher"	},

	{	L"AsmEvtValChg",			TRUE,		E_GrP3DvlParaTypeAsmCode,				1,	(void*)&((Ptr_GrP3DoSpinPntPara)0)->AsmEvtValChg,NULL,	L"asm value change event"	},
};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP3DoSpinPnt::Cls_GrP3DoSpinPnt( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt ):
Cls_GrP3DoBase( A_ScptId, A_PtrRect, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP3DoClassSpinPnt;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaDoSpinPnt) + sizeof(St_GrP3ScptParaHd);
		
		m_IsKeyAble	=	TRUE;
		m_IsMouseAble	=	TRUE;

		GrDumyZeroMem( &m_ParaDoSpinPnt, sizeof(m_ParaDoSpinPnt) );

		m_IsCtrlMode	=	FALSE;
		m_Value				=	0;

		LcValTxtUpdt();
}
//--------------------------------------------------------------------
Cls_GrP3DoSpinPnt::~Cls_GrP3DoSpinPnt( void )
{
		
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinPnt::RtlScptParaSubDump( void )
{
#ifdef GR_P3_EDITOR
	// local -------------------
		Ptr_GrP3ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherite
		Cls_GrP3DoBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP3ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP3ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP3DoClassSpinPnt;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaDoSpinPnt);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP3ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaDoSpinPnt, sizeof(m_ParaDoSpinPnt) );
		//update progress
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaDoSpinPnt)	+ sizeof(St_GrP3ScptParaHd);
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinPnt::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaDoSpinPnt, sizeof(m_ParaDoSpinPnt) , m_PtrScpt , E_GrP3DoClassSpinPnt );

		// patch item max count
		if ( E_GrP3DoSpinPntItemCnt < m_ParaDoSpinPnt.ItemCnt )
		{
			m_ParaDoSpinPnt.ItemCnt	=	E_GrP3DoSpinPntItemCnt;		// patch item count
		}
		LcValTxtUpdt();		// text update
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoSpinPnt::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		if ( !Cls_GrP3DoBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr ) )
		{
			switch ( A_Cmd )
			{
				case E_GrP3UpiValueGet:
					A_PtrInt[0]	=	(__s32)m_Value;
					break;
				case E_GrP3UpiValueSet:
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
void	Cls_GrP3DoSpinPnt::ProcPaint( void )
{
	// local -------------------
		__u16	Tv_PstId;
	// code --------------------

		Tv_PstId	=	E_GrP3ScptPstIdNone;
		if ( m_Value < m_ParaDoSpinPnt.ItemCnt )
		{
			Tv_PstId	=	m_ParaDoSpinPnt.PntList[m_Value];
		}
		if ( E_GrP3ScptPstIdNone != Tv_PstId )
		{
			PaintScriptPush( Tv_PstId, &m_PntRcScr );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinPnt::RtlDvlParaInfoSubDump( void )
{
#ifdef GR_P3_EDITOR
		// inherited
		Cls_GrP3DoBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaDoSpinPnt, V_GrP3DoSpinPntParaInfo, E_GrP3DoSpinPntParaCnt );
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinPnt::LcValueUpdt( BOOL8 A_IsNotify )
{
		LcValTxtUpdt();
		if ( A_IsNotify )
		{
			LcNotifyValChg();
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinPnt::LcValTxtUpdt( void )
{
	// local -------------------
		WCHAR*	Tv_StrAdd;
	// code --------------------
		GrStrClear( m_StrTxt );		// text clear
		// check exist
		if ( m_Value < m_ParaDoSpinPnt.ItemCnt )
		{
			if ( E_GrP3ScptTxtIdNone != m_ParaDoSpinPnt.TxtList[m_Value] )
			{
				Tv_StrAdd	=	V_GrP3Mng->TextPtrGet( m_ParaDoSpinPnt.TxtList[m_Value] );
				GrStrWcopy( m_StrTxt, Tv_StrAdd, GrStrMaxLen(m_StrTxt) );
			}
		}

}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP3DoSpinPnt::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
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
void	Cls_GrP3DoSpinPnt::LcNotifyValChg( void )
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
			V_GrP3Mng->VmRunCode( m_ParaDoSpinPnt.AsmEvtValChg, this, &Tv_Rpara );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinPnt::ProcEvtFocusGet( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoBase::ProcEvtFocusGet();
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinPnt::ProcEvtFocusLoss( void )
{
	// local -------------------
	// code --------------------
		m_IsCtrlMode	=	FALSE;
		Cls_GrP3DoBase::ProcEvtFocusLoss();
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinPnt::LcValIncrease( void )
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
void	Cls_GrP3DoSpinPnt::LcValDecrease( void )
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
BOOL8	Cls_GrP3DoSpinPnt::ProcEvtKeyDown( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( m_ParaDoSpinPnt.IsOnKeyCtrl || m_IsCtrlMode )
		{
			switch ( A_Key )
			{
				case E_GrP3KeyLeft:
					if ( !m_ParaDoSpinPnt.IsVertical )
					{
						LcValDecrease();
						Tv_Result	=	TRUE;
					}
					break;
				case E_GrP3KeyRight:
					if ( !m_ParaDoSpinPnt.IsVertical )
					{
						LcValIncrease();
						Tv_Result	=	TRUE;
					}
					break;
				case E_GrP3KeyUp:
					if ( m_ParaDoSpinPnt.IsVertical )
					{
						LcValIncrease();
						Tv_Result	=	TRUE;
					}
					break;
				case E_GrP3KeyDown:
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
BOOL8	Cls_GrP3DoSpinPnt::ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( !m_ParaDoSpinPnt.IsOnKeyCtrl )
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
BOOL8	Cls_GrP3DoSpinPnt::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
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
BOOL8	Cls_GrP3DoSpinPnt::ProcEvtWheelUp( void )
{
	// local -------------------
	// code --------------------
		LcValIncrease();
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoSpinPnt::ProcEvtWheelDown( void )
{
	// local -------------------
	// code --------------------
		LcValDecrease();
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinPnt::RtlInit( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoBase::RtlInit();
		ReqDraw();
}
//--------------------------------------------------------------------

