/*
 label

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<Ui/GrUiDoArea.h>
#include	<Ui/GrUiMng.h>
#include	<Ui/GrUiScript.h>

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
Cls_GrUiDoArea::Cls_GrUiDoArea( Ptr_GrRect A_PtrRect, Cls_GrUiDoBase* A_ObjPrnt, BOOL8 A_IsIdAlloc ):
Cls_GrUiDoBase( A_PtrRect, A_ObjPrnt, E_GrUiConIdxFocused, A_IsIdAlloc )
{
		//init
		m_TypeId			=	E_GrUiDoTypeIdArea;

}
//--------------------------------------------------------------------
Cls_GrUiDoArea::~Cls_GrUiDoArea()
{
}
//--------------------------------------------------------------------


