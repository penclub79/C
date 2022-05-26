/*
 label

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrDebug.h>
#include	<Ui/GrUiDoCdvArea.h>
#include	<Ui/GrUiMng.h>
#include	<Ui/GrUiScript.h>
#include	<GrDumyTool.h>
#ifdef GR_OSD_EDIT
#include	<DvlUi/GrUiDvlDoWin.h>
#endif

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
Cls_GrUiDoCdvArea::Cls_GrUiDoCdvArea( Ptr_GrRect A_PtrRect, Cls_GrUiDoBase* A_ObjPrnt, BOOL8 A_IsIdAlloc ):
Cls_GrUiDoBase( A_PtrRect, A_ObjPrnt, E_GrUiConIdxFocused, A_IsIdAlloc )
{
		//init
		m_TypeId			=	E_GrUiDoTypeIdCdvArea;

		GrDumyZeroMem( m_CondTbl, sizeof(m_CondTbl) );

}
//--------------------------------------------------------------------
Cls_GrUiDoCdvArea::~Cls_GrUiDoCdvArea()
{
}
//--------------------------------------------------------------------
/*
void	Cls_GrUiDoCdvArea::ProcEvtScptDecoded( void )
{
	// local -------------------
#ifdef GR_OSD_EDIT
		Cls_GrUiDvlDoWin*	Tv_WinDvl;
		BOOL8	Tv_IsAble;
#endif
		Cls_GrUiDoBase*	Tv_Obj;
		__u32	Tv_CondIdx;
		BOOL8	Tv_IsVisible;
		__u32	Tv_SrcVal;
		__u32	Tv_RefVal;
	// code --------------------
		//inherited
		Cls_GrUiDoBase::ProcEvtScptDecoded();

#ifdef GR_OSD_EDIT
		Tv_IsAble		=	FALSE;
		if ( E_GrUiDoWinTypeNormal == ((Cls_GrUiDoWin*)m_ObjWin)->WinTypeGet() )
		{
			Tv_IsAble		=	TRUE;
		}
		else
		{
			Tv_WinDvl	=	(Cls_GrUiDvlDoWin*)m_ObjWin;
			Tv_IsAble	=	!Tv_WinDvl->IsOsdEditMode();
		}
		if ( Tv_IsAble )
		{
#endif
			//find all slave
			Tv_Obj	=	SlaveObjGet();
			while ( NULL != Tv_Obj )
			{
				//check user data 2
				Tv_IsVisible	=	TRUE;
				Tv_CondIdx	=	Tv_Obj->UserDataGet( E_GrUiDoCdvAreaChkUserData );
				if ( (1 <= Tv_CondIdx) && (E_GrUiDoCdvAreaMaxCondCnt >= Tv_CondIdx) )
				{
					Tv_CondIdx --;
					Tv_SrcVal	=	V_GrUiMngObj->GlbVarGet( m_CondTbl[Tv_CondIdx].SrcIdx );
					Tv_RefVal	=	m_CondTbl[Tv_CondIdx].Ref;
					//check condition
					switch( m_CondTbl[Tv_CondIdx].Cond )
					{
						case	E_GrUiDoCdvAreaCondEqual:
							if ( Tv_SrcVal == Tv_RefVal )
							{
								Tv_IsVisible	=	FALSE;
							}
							break;
						case	E_GrUiDoCdvAreaCondDiff:
							if ( Tv_SrcVal != Tv_RefVal )
							{
								Tv_IsVisible	=	FALSE;
							}
							break;
						case	E_GrUiDoCdvAreaCondBig:
							if ( Tv_SrcVal > Tv_RefVal )
							{
								Tv_IsVisible	=	FALSE;
							}
							break;
						case	E_GrUiDoCdvAreaCondLittle:
							if ( Tv_SrcVal < Tv_RefVal )
							{
								Tv_IsVisible	=	FALSE;
							}
							break;
						case	E_GrUiDoCdvAreaCondEqBig:
							if ( Tv_SrcVal >= Tv_RefVal )
							{
								Tv_IsVisible	=	FALSE;
							}
							break;
						case	E_GrUiDoCdvAreaCondEqLittle:
							if ( Tv_SrcVal <= Tv_RefVal )
							{
								Tv_IsVisible	=	FALSE;
							}
							break;
					}
				}
				//set visible
				Tv_Obj->VisibleSet( Tv_IsVisible );
				//go next
				Tv_Obj	=	Tv_Obj->NextObjGet();
			}
#ifdef GR_OSD_EDIT
		}
#endif
}
*/
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoCdvArea::IsConditionOk( __u8 A_Idx )
{
	// local -------------------
		BOOL8	Tv_Result;
#ifdef GR_OSD_EDIT
		Cls_GrUiDvlDoWin*	Tv_WinDvl;
		BOOL8	Tv_IsAble;
#endif
		__u32	Tv_SrcVal;
		__u32	Tv_RefVal;
	// code --------------------
		//init
		Tv_Result	=	FALSE;

	#ifdef GR_OSD_EDIT
		Tv_IsAble		=	FALSE;
		if ( E_GrUiDoWinTypeNormal == ((Cls_GrUiDoWin*)m_ObjWin)->WinTypeGet() )
		{
			Tv_IsAble		=	TRUE;
		}
		else
		{
			Tv_WinDvl	=	(Cls_GrUiDvlDoWin*)m_ObjWin;
			Tv_Result	=	Tv_WinDvl->IsOsdEditMode();
			Tv_IsAble	=	!Tv_Result;	//edit mode
		}
		if ( Tv_IsAble )
		{
	#endif
			Tv_SrcVal	=	V_GrUiMngObj->GlbVarGet( m_CondTbl[A_Idx].SrcIdx );
			Tv_RefVal	=	m_CondTbl[A_Idx].Ref;
			//check condition
			switch( m_CondTbl[A_Idx].Cond )
			{
				case	E_GrUiDoCdvAreaCondEqual:
					if ( Tv_SrcVal == Tv_RefVal )
					{
						Tv_Result	=	TRUE;
					}
					break;
				case	E_GrUiDoCdvAreaCondDiff:
					if ( Tv_SrcVal != Tv_RefVal )
					{
						Tv_Result	=	TRUE;
					}
					break;
				case	E_GrUiDoCdvAreaCondBig:
					if ( Tv_SrcVal > Tv_RefVal )
					{
						Tv_Result	=	TRUE;
					}
					break;
				case	E_GrUiDoCdvAreaCondLittle:
					if ( Tv_SrcVal < Tv_RefVal )
					{
						Tv_Result	=	TRUE;
					}
					break;
				case	E_GrUiDoCdvAreaCondEqBig:
					if ( Tv_SrcVal >= Tv_RefVal )
					{
						Tv_Result	=	TRUE;
					}
					break;
				case	E_GrUiDoCdvAreaCondEqLittle:
					if ( Tv_SrcVal <= Tv_RefVal )
					{
						Tv_Result	=	TRUE;
					}
					break;
			}
	#ifdef GR_OSD_EDIT
		}
	#endif

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoCdvArea::CondSrcIdxSet( __u8 A_Idx, __u8 A_SrcIdx )
{
		if ( E_GrUiMngGlobalValCnt > A_Idx )
		{
			m_CondTbl[A_Idx].SrcIdx	=	A_SrcIdx;
		}
#ifndef LINUX_APP
		else
		{
			DbgMsgPrint( "Cls_GrUiDoCdvArea::CondSrcIdxSet - source index over - %d \n", A_SrcIdx );
		}
#endif
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoCdvArea::CondSrcIdxGet( __u8 A_Idx )
{
		return	m_CondTbl[A_Idx].SrcIdx;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoCdvArea::CondTypeSet( __u8 A_Idx, __u8 A_Type )
{
		m_CondTbl[A_Idx].Cond	=	A_Type;
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoCdvArea::CondTypeGet( __u8 A_Idx )
{
		return	m_CondTbl[A_Idx].Cond;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoCdvArea::CondRefSet( __u8 A_Idx, __u32 A_Val )
{
		m_CondTbl[A_Idx].Ref	=	A_Val;
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoCdvArea::CondRefGet( __u8 A_Idx )
{
		return	m_CondTbl[A_Idx].Ref;
}
//--------------------------------------------------------------------

