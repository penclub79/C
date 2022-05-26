/*
 relate button

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<Ui/GrUiDoAcBtn.h>
#include	<Ui/GrUiMng.h>
#include	<Ui/GrUiScript.h>
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
Cls_GrUiDoAcBtn::Cls_GrUiDoAcBtn( Ptr_GrRect A_PtrRect, Cls_GrUiDoBase* A_ObjPrnt, BOOL8 A_IsIdAlloc ):
Cls_GrUiDoBtn( A_PtrRect, A_ObjPrnt, A_IsIdAlloc )
{
		//init
		m_TypeId			=	E_GrUiDoTypeIdAcBtn;

		m_IsActRvs		=	FALSE;	
		GrDumyZeroMem( m_ActObjTbl, sizeof(m_ActObjTbl) );

}
//--------------------------------------------------------------------
Cls_GrUiDoAcBtn::~Cls_GrUiDoAcBtn()
{
}
//--------------------------------------------------------------------
void	Cls_GrUiDoAcBtn::ActObjSet( __u8 A_Idx, __u16 A_ObjId )
{
		if ( E_GrUiDoAcBtnRelCnt > A_Idx )
		{	
			m_ActObjTbl[A_Idx]	=	A_ObjId;
		}
}
//--------------------------------------------------------------------
__u16	Cls_GrUiDoAcBtn::ActObjGet( __u8 A_Idx )
{
	// local -------------------
		__u16	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrUiObjIdNone;
		if ( E_GrUiDoAcBtnRelCnt > A_Idx )
		{
			Tv_Result	=	m_ActObjTbl[A_Idx];
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoAcBtn::ActRvsSet( BOOL8 A_IsRvs, BOOL8 A_IsAction )
{
		m_IsActRvs	=	A_IsRvs;
		if ( A_IsAction )
		{
			LcDoRtAction();
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoAcBtn::IsActRvs( void )
{
		return	m_IsActRvs;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoAcBtn::LcDoRtAction( void )
{
	// local -------------------
		__u8	Tv_WkIdx;
		Cls_GrUiDoBase*	Tv_Win;
		Cls_GrUiDoBase*	Tv_Obj;
		BOOL8	Tv_IsChk;
		Cls_GrUiDoBtn*	Tv_Btn;
	// code --------------------
		Tv_IsChk	=	m_IsBtnChked;
		if ( m_IsActRvs )
		{
			Tv_IsChk	=	!Tv_IsChk;
		}
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiDoAcBtnRelCnt;Tv_WkIdx++ )
		{
			if ( E_GrUiObjIdNone != m_ActObjTbl[Tv_WkIdx] )
			{
				Tv_Win	=	m_ObjWin;
				Tv_Obj	=	Tv_Win->FindSlaveObjByObjId( m_ActObjTbl[Tv_WkIdx] );
				if ( NULL != Tv_Obj )
				{
					//check type
					if ( E_GrUiDoTypeIdButton == Tv_Obj->TypeIdGet() )
					{
						//check focus able
						if ( Tv_Obj->IsCanFocus() )
						{
							Tv_Btn	=	(Cls_GrUiDoBtn*)Tv_Obj;
							//check now status
							if ( Tv_IsChk != Tv_Btn->IsBtnChecked() )
							{
								//try change
								Tv_Btn->BtnCheckSet( Tv_IsChk );
							}
						}
					}
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoAcBtn::ProcEvtScptDecoded( void )
{
		//LcDoRtAction();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoAcBtn::LcBtnEventMakeUp( void )
{
		Cls_GrUiDoBtn::LcBtnEventMakeUp();
		LcDoRtAction();
}
//--------------------------------------------------------------------

