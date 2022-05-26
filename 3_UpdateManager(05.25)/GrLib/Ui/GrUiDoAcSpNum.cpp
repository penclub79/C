/*
 relate button

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<Ui/GrUiDoAcSpNum.h>
#include	<Ui/GrUiMng.h>
#include	<Ui/GrUiScript.h>
#include	<Ui/GrUiDoSpinTxt.h>
#include	<Ui/GrUiDoSpinImg.h>
#include	<Ui/GrUiDoSpinDgt.h>
#include	<GrDumyTool.h>

//====================================================================
//local const
#define m_ObjConsole		((Cls_GrUiConsole*)m_ObjCon)

//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var
extern	Cls_GrUiMng*				V_GrUiMngObj;

//====================================================================
//--------------------------------------------------------------------
Cls_GrUiDoAcSpNum::Cls_GrUiDoAcSpNum( Ptr_GrRect A_PtrRect, Cls_GrUiDoBase* A_ObjPrnt, BOOL8 A_IsIdAlloc ):
Cls_GrUiDoSpinNum( A_PtrRect, A_ObjPrnt, A_IsIdAlloc )
{
		//init
		m_TypeId			=	E_GrUiDoTypeIdAcSpNum;
		GrDumyZeroMem( m_ActObjTbl, sizeof(m_ActObjTbl) );
}
//--------------------------------------------------------------------
Cls_GrUiDoAcSpNum::~Cls_GrUiDoAcSpNum()
{
}
//--------------------------------------------------------------------
void	Cls_GrUiDoAcSpNum::ActObjSet( __u8 A_Idx, __u16 A_ObjId )
{
		if ( E_GrUiDoAcSpNumRelCnt > A_Idx )
		{	
			m_ActObjTbl[A_Idx]	=	A_ObjId;
		}
}
//--------------------------------------------------------------------
__u16	Cls_GrUiDoAcSpNum::ActObjGet( __u8 A_Idx )
{
	// local -------------------
		__u16	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrUiObjIdNone;
		if ( E_GrUiDoAcSpNumRelCnt > A_Idx )
		{
			Tv_Result	=	m_ActObjTbl[A_Idx];
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoAcSpNum::LcDoRtAction( void )
{
	// local -------------------
		__u8	Tv_WkIdx;
		Cls_GrUiDoBase*	Tv_Win;
		Cls_GrUiDoBase*	Tv_Obj;
		Cls_GrUiDoSpinNum*	Tv_SpnNum;
		Cls_GrUiDoSpinTxt*	Tv_SpnTxt;
		Cls_GrUiDoSpinDgt*	Tv_SpnDgt;
		Cls_GrUiDoSpinImg*	Tv_SpnImg;
	// code --------------------
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiDoAcSpNumRelCnt;Tv_WkIdx++ )
		{
			if ( E_GrUiObjIdNone != m_ActObjTbl[Tv_WkIdx] )
			{
				Tv_Win	=	m_ObjWin;
				Tv_Obj	=	Tv_Win->FindSlaveObjByObjId( m_ActObjTbl[Tv_WkIdx] );
				if ( NULL != Tv_Obj )
				{
					//check can focus
					if ( Tv_Obj->IsCanFocus() )
					{
						//check type
						switch( Tv_Obj->TypeIdGet() )
						{
							case	E_GrUiDoTypeIdSpbtnNum:
								Tv_SpnNum	=	(Cls_GrUiDoSpinNum*)Tv_Obj;
								Tv_SpnNum->ValueSet( m_Value );
								break;
							case	E_GrUiDoTypeIdSpbtnTxt:
								Tv_SpnTxt	=	(Cls_GrUiDoSpinTxt*)Tv_Obj;
								Tv_SpnTxt->SelectIdxSet( m_Value );
								break;
							case	E_GrUiDoTypeIdSpbtnDgt:
								Tv_SpnDgt	=	(Cls_GrUiDoSpinDgt*)Tv_Obj;
								Tv_SpnDgt->ValueSet( m_Value );
								break;
							case	E_GrUiDoTypeIdSpbtnImg:
								Tv_SpnImg	=	(Cls_GrUiDoSpinImg*)Tv_Obj;
								Tv_SpnImg->SelectIdxSet( m_Value );
								break;
						}
					}
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoAcSpNum::ProcEvtScptDecoded( void )
{
		//LcDoRtAction();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoAcSpNum::ValueSet( __s32 A_Val, BOOL8 A_IsNotify )
{
		Cls_GrUiDoSpinNum::ValueSet( A_Val, A_IsNotify );
		LcDoRtAction();
}
//--------------------------------------------------------------------

