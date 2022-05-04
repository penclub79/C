/*
 relate button

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<Ui/GrUiDoAcSpImg.h>
#include	<Ui/GrUiMng.h>
#include	<Ui/GrUiScript.h>
#include	<Ui/GrUiDoSpinNum.h>
#include	<Ui/GrUiDoSpinTxt.h>
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
Cls_GrUiDoAcSpImg::Cls_GrUiDoAcSpImg( Ptr_GrRect A_PtrRect, Cls_GrUiDoBase* A_ObjPrnt, BOOL8 A_IsIdAlloc ):
Cls_GrUiDoSpinImg( A_PtrRect, A_ObjPrnt, A_IsIdAlloc )
{
		//init
		m_TypeId			=	E_GrUiDoTypeIdAcSpImg;

		GrDumyZeroMem( m_ActObjTbl, sizeof(m_ActObjTbl) );
}
//--------------------------------------------------------------------
Cls_GrUiDoAcSpImg::~Cls_GrUiDoAcSpImg()
{
}
//--------------------------------------------------------------------
void	Cls_GrUiDoAcSpImg::ActObjSet( __u8 A_Idx, __u16 A_ObjId )
{
		if ( E_GrUiDoAcSpImgRelCnt > A_Idx )
		{	
			m_ActObjTbl[A_Idx]	=	A_ObjId;
		}
}
//--------------------------------------------------------------------
__u16	Cls_GrUiDoAcSpImg::ActObjGet( __u8 A_Idx )
{
	// local -------------------
		__u16	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrUiObjIdNone;
		if ( E_GrUiDoAcSpImgRelCnt > A_Idx )
		{
			Tv_Result	=	m_ActObjTbl[A_Idx];
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoAcSpImg::LcDoRtAction( void )
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
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiDoAcSpImgRelCnt;Tv_WkIdx++ )
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
								Tv_SpnNum->ValueSet( m_SelIdx );
								break;
							case	E_GrUiDoTypeIdSpbtnTxt:
								Tv_SpnTxt	=	(Cls_GrUiDoSpinTxt*)Tv_Obj;
								Tv_SpnTxt->SelectIdxSet( m_SelIdx );
								break;
							case	E_GrUiDoTypeIdSpbtnDgt:
								Tv_SpnDgt	=	(Cls_GrUiDoSpinDgt*)Tv_Obj;
								Tv_SpnDgt->ValueSet( m_SelIdx );
								break;
							case	E_GrUiDoTypeIdSpbtnImg:
								Tv_SpnImg	=	(Cls_GrUiDoSpinImg*)Tv_Obj;
								Tv_SpnImg->SelectIdxSet( m_SelIdx );
								break;
						}
					}
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoAcSpImg::ProcEvtScptDecoded( void )
{
		//LcDoRtAction();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoAcSpImg::SelectIdxSet( __s32 A_Idx, BOOL8 A_IsNotify )
{
		Cls_GrUiDoSpinImg::SelectIdxSet( A_Idx, A_IsNotify );
		LcDoRtAction();
}
//--------------------------------------------------------------------

