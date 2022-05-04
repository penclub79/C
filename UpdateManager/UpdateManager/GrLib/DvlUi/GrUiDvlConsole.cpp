/*
 virtual keyboard base class

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<DvlUi/GrUiDvlConsole.h>
#include	<GrStrTool.h>
#include	<GrTimeTool.h>
#include	<Ui/GrUiMng.h>
#include	<Ui/GrUiConsole.h>
#include	<Ui/GrUiScptDec.h>
#include	<DvlUi/GrUiDvlDoWin.h>
#include	<DvlUi/GrUiDvlMng.h>

//====================================================================
//local const
#define		m_UiDvlMng		((Cls_GrUiDvlMng*)m_ObjUiDvlMng)

//====================================================================
//local type


//====================================================================
//local macro
#define m_ObjConsole		((Cls_GrUiConsole*)m_ObjCon)

//====================================================================
//local function

//====================================================================
//var
extern	Cls_GrUiMng*				V_GrUiMngObj;


//====================================================================
//--------------------------------------------------------------------
Cls_GrUiDvlConsole::Cls_GrUiDvlConsole(	Cls_GrGdib* A_GdibScr, void* A_ObjUiDvlMng, Fnc_GrUiDvlPaintTrs A_FncPntTrs ):
Cls_GrUiConsole( A_GdibScr )
{
	// local -------------------
	// code --------------------
		//init
		m_ObjUiDvlMng	=	A_ObjUiDvlMng;
		m_IsIdDelAble	=	FALSE;
		m_FncPntTrs		=	A_FncPntTrs;
}
//--------------------------------------------------------------------
Cls_GrUiDvlConsole::~Cls_GrUiDvlConsole()
{
}
//--------------------------------------------------------------------
__u16	Cls_GrUiDvlConsole::ObjIdAlloc( void )
{
	// local -------------------
		__u16		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	m_UiDvlMng->FindNotUseObjId();
		if ( E_GrUiObjIdNone != Tv_Result )
		{
			//alloc
			m_UiDvlMng->ObjIdAlocSet( Tv_Result, TRUE );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDvlConsole::ObjIdFree( __u16 A_Id )
{
	// local -------------------
	// code --------------------
		//check range
		if ( m_IsIdDelAble && (E_GrUiObjIdNone != A_Id) )
		{
			//free
			m_UiDvlMng->ObjNameSet( A_Id, L"" );			//name clear
			m_UiDvlMng->ObjIdAlocSet( A_Id, FALSE );
			if ( E_GrUiObjIdEncStart <= A_Id )
			{
				m_UiDvlMng->OBjTextDel( A_Id );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDvlConsole::ObjIdDelAble( BOOL8 A_IsAble )
{
		m_IsIdDelAble	=	A_IsAble;
}
//--------------------------------------------------------------------
Cls_GrUiDoBase*	Cls_GrUiDvlConsole::FocusObjGet( void )
{
	// local -------------------
	// code --------------------
		//inherited
		return	Cls_GrUiConsole::FocusObjGet();
}
//--------------------------------------------------------------------
void	Cls_GrUiDvlConsole::FocusObjSet( Cls_GrUiDoBase* A_ObjUiDo )
{
	// local -------------------
		Cls_GrUiDvlDoWin*	Tv_Win;
		BOOL8		Tv_IsAble;
	// code --------------------
		if ( NULL != A_ObjUiDo )
		{
			Tv_IsAble	=	TRUE;
			Tv_Win	=	(Cls_GrUiDvlDoWin*)A_ObjUiDo->WindowGet();
			if ( E_GrUiDoWinTypeNormal != Tv_Win->WinTypeGet() )
			{
				if ( Tv_Win->IsOsdEditMode() )
				{
					Tv_IsAble		=	FALSE;
				}
			}
			//update
			if ( Tv_IsAble )
			{
				//inherited
				Cls_GrUiConsole::FocusObjSet( A_ObjUiDo );
			}
			else			
			{
				m_ObjFocus	=	NULL;
			}
		}
		else
		{
			//inherited
			Cls_GrUiConsole::FocusObjSet( A_ObjUiDo );
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDvlConsole::ProcDoWinFree( __u8 A_Layer, __u8 A_WinIdx )
{
	// local -------------------
		Cls_GrUiDvlDoWin*	Tv_Win;
		BOOL8	Tv_IsDel;
	// code --------------------
		//check delete
		Tv_IsDel	=	FALSE;
		Tv_Win	=	(Cls_GrUiDvlDoWin*)m_LayerTbl[A_Layer][A_WinIdx];
		if ( E_GrUiDoWinTypeNormal != Tv_Win->WinTypeGet() )
		{
			Tv_IsDel	=	Tv_Win->IsOsdDelMode();
		}
		//run
		if ( Tv_IsDel )
		{
			m_IsIdDelAble		=	TRUE;
			Cls_GrUiConsole::ProcDoWinFree( A_Layer, A_WinIdx );
			m_IsIdDelAble		=	FALSE;
		}
		else
		{
			Cls_GrUiConsole::ProcDoWinFree( A_Layer, A_WinIdx );
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDvlConsole::HalPntReqTransfer( Ptr_GrRect A_PtrRect )
{
		if ( NULL != m_FncPntTrs )
		{
			m_FncPntTrs( A_PtrRect );
		}
		return	TRUE;
}
//--------------------------------------------------------------------

