/*
		platform script decorder

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P4/GrP4DoBase.h>
#include	<GrDebug.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<GrStrTool.h>
#include	<P4/GrP4Script.h>
#include	<P4/GrP4DoWin.h>
#include	<P4/GrP4Mng.h>
#include	<P4/GrP4UiCmd.h>
#include	<P4/GrP4DoFrame.h>
#include	<P4/GrP4DoRelStc.h>

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

#define E_GrP4DoBaseParaCnt		12

St_GrP4DvlDoPrInfoData	V_GrP4DoBaseParaInfo[E_GrP4DoBaseParaCnt]	=	
{
	{	L"FocusLeft",		TRUE,		E_GrP4DvlParaTypeScptIdAny,		1,	(void*)&((Ptr_GrP4DoBasePara)0)->FcsMvTbl[E_GrP4DoFocusMvIdxLeft]	, NULL, L"left focus target"	},
	{	L"FocusRight",	TRUE,		E_GrP4DvlParaTypeScptIdAny,		1,	(void*)&((Ptr_GrP4DoBasePara)0)->FcsMvTbl[E_GrP4DoFocusMvIdxRight]	, NULL,	L"right focus target"	},
	{	L"FocusUp",			TRUE,		E_GrP4DvlParaTypeScptIdAny,		1,	(void*)&((Ptr_GrP4DoBasePara)0)->FcsMvTbl[E_GrP4DoFocusMvIdxUp]	, NULL,	L"up focus target"	},
	{	L"FocusDown",		TRUE,		E_GrP4DvlParaTypeScptIdAny,		1,	(void*)&((Ptr_GrP4DoBasePara)0)->FcsMvTbl[E_GrP4DoFocusMvIdxDown]	, NULL,	L"down focus target"	},
	{	L"UserData0",		TRUE,		E_GrP4DvlParaTypeDword,				1,	(void*)&((Ptr_GrP4DoBasePara)0)->UserDataTbl[0]	, NULL,	L"User data 0"	},
	{	L"UserData1",		TRUE,		E_GrP4DvlParaTypeDword,				1,	(void*)&((Ptr_GrP4DoBasePara)0)->UserDataTbl[1]	, NULL,	L"User data 1"	},
	{	L"UserData2",		TRUE,		E_GrP4DvlParaTypeDword,				1,	(void*)&((Ptr_GrP4DoBasePara)0)->UserDataTbl[2]	, NULL,	L"User data 2"	},
	{	L"UserData3",		TRUE,		E_GrP4DvlParaTypeDword,				1,	(void*)&((Ptr_GrP4DoBasePara)0)->UserDataTbl[3]	, NULL,	L"User data 3"	},
	{	L"Enable",			TRUE,		E_GrP4DvlParaTypeBool8,				1,	(void*)&((Ptr_GrP4DoBasePara)0)->IsEnable	, NULL,	L"Enable input mounse and key"	},
	{ L"Visible", TRUE, E_GrP4DvlParaTypeBool8, 1, (void*)&((Ptr_GrP4DoBasePara)0)->IsVisible, NULL, L"Visible object" },
	{ L"KeyBlock", TRUE, E_GrP4DvlParaTypeBool8, 1, (void*)&((Ptr_GrP4DoBasePara)0)->IsKeyBlock, NULL, L"Key input block" },
	{ L"RelStdId",		 TRUE,	E_GrP4DvlParaTypeScptIdAny,		1,	(void*)&((Ptr_GrP4DoBasePara)0)->RelStdId, NULL, L"Relate standard action object id" },

};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoBase::Cls_GrP4DoBase(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt)
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId			=	E_GrP4DoClassBase;
		m_PosRel			=	A_PosRel;
		m_PosRect			=	*A_PtrRect;
		m_ScptId			=	A_ScptId;
		m_ObjPrnt			=	A_ObjPrnt;
		m_ParaSize		=	(__u16)(sizeof(m_ParaDoBase)	+ sizeof(St_GrP4ScptParaHd));		//parameter size
		m_ObjFrm			=	NULL;

		m_FindId			=	0;

#ifdef GR_P4_EDITOR
		m_IsScptDel		=	FALSE;		// script delete mode
#endif

		//get console
		if ( NULL != m_ObjPrnt )
		{
			//sub object
			m_ObjWin	=	m_ObjPrnt->WindowGet();
			m_ObjCon	=	m_ObjPrnt->ConsoleGet();
			m_ObjFrm	=	m_ObjPrnt->FrameGet();
		}
		else
		{
			//window
			m_ObjWin	=	this;
		}

		m_IsVisible		=	TRUE;
		m_IsKeyAble		=	FALSE;
		m_IsMouseAble	=	FALSE;
		m_IsOnMouse		=	FALSE;
		//parameter initialize
		GrDumyZeroMem( &m_ParaDoBase, sizeof(m_ParaDoBase) );
		m_ParaDoBase.IsEnable	=	TRUE;
		m_ParaDoBase.FcsMvTbl[E_GrP4DoFocusMvIdxLeft]		=	E_GrP4ScptIdNone;
		m_ParaDoBase.FcsMvTbl[E_GrP4DoFocusMvIdxRight]	=	E_GrP4ScptIdNone;
		m_ParaDoBase.FcsMvTbl[E_GrP4DoFocusMvIdxUp]			=	E_GrP4ScptIdNone;
		m_ParaDoBase.FcsMvTbl[E_GrP4DoFocusMvIdxDown]		=	E_GrP4ScptIdNone;
		m_ParaDoBase.IsVisible	=	TRUE;

		// object regist
		m_ObjSlave	=	NULL;
		m_ObjNext		=	NULL;
		if ( NULL != m_ObjPrnt )
		{
			m_ObjNext	=	m_ObjPrnt->SlaveObjGet();		// get next object
			m_ObjPrnt->RtlSlaveObjSet( this );
		}

		// script id
		m_ScptId		=	A_ScptId;
#ifdef GR_P4_EDITOR
		if ( E_GrP4ScptIdNone == A_ScptId )
		{
			// check window
			if ( NULL == A_ObjPrnt )
			{
				// error
				DbgMsgPrint( "Cls_GrP4DoBase::Cls_GrP4DoBase - None script id window!\n" );
			}
			else
			{
				// auto allocate
				m_ScptId	=	((Cls_GrP4DvlMng*)V_GrP4Mng)->DvlSlaveScptIdAlloc();
			}
		}
#endif
}
//--------------------------------------------------------------------
Cls_GrP4DoBase::~Cls_GrP4DoBase( void )
{
	// local -------------------
		Cls_GrP4DoBase*	Tv_ObjPrv;
		Cls_GrP4DoBase*	Tv_ObjFocus;
	// code --------------------
		// redraw now area
		ReqDraw();

		//release slave - chain
		ObjSlaveFreeAll();

		//chain patch
		if ( NULL != m_ObjPrnt )
		{
			Tv_ObjPrv	=	PrevObjGet();
			if ( NULL != Tv_ObjPrv )
			{
				Tv_ObjPrv->RtlNextObjSet( m_ObjNext );
			}
			else
			{
				m_ObjPrnt->RtlSlaveObjSet( m_ObjNext );
			}
		}

		//release mouse object
		if ( this == ((Cls_GrP4ConBase*)m_ObjCon)->MouseOnObjGet() )
		{
			//on mouse object reset
			((Cls_GrP4ConBase*)m_ObjCon)->MouseOnObjReset();
		}

		//release local focus
		if ( !IsWindow() )
		{
			//check match
			Tv_ObjFocus	=	((Cls_GrP4DoWin*)m_ObjWin)->LastFocusObjGet();
			if ( Tv_ObjFocus == this )
			{
				((Cls_GrP4DoWin*)m_ObjWin)->RtlLastFocusObjSet( NULL );			//last focus clear
			}
		}

		//release global focus
		Tv_ObjFocus	=	((Cls_GrP4ConBase*)m_ObjCon)->FocusKeyObjGet();
		if ( this == Tv_ObjFocus )
		{
			//focus release
			((Cls_GrP4ConBase*)m_ObjCon)->FocusReset();
			//request focus rebuild
			((Cls_GrP4ConBase*)m_ObjCon)->FocusCorrect();
		}

		// mouse capture release

		// release host object
		V_GrP4Mng->RtlProcObjRelease( this );

		//free obj id
#ifdef GR_P4_EDITOR
		if ( m_IsScptDel )
		{
			//delete script id
			((Cls_GrP4DvlMng*)V_GrP4Mng)->DvlSlaveScptIdFree( m_ScptId );
		}
#endif
}
//--------------------------------------------------------------------
__u16	Cls_GrP4DoBase::ScptIdGet( void )
{
		return	m_ScptId;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBase::RtlScptIdSet( __u16 A_Id )
{
		m_ScptId	=	A_Id;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBase::ParaCopyFromScpt( void* A_PtrPara, __u32 A_ParaSize, void* A_PtrScpt, __u8 A_ClassId )
{
	// local -------------------
		Ptr_GrP4ScptObjHd	Tv_PtrScpt;
		__u32	Tv_ParaPrgs;
		Ptr_GrP4ScptParaHd	Tv_PtrPrHd;
		void*	Tv_PtrPrCt;
		__u32	Tv_CtSize;
	// code --------------------

#ifdef GR_P4_EDITOR
		// check parametter no copy mode
		if ( m_IsDvlParaNoCopy )
		{
			return;
		}
#endif

		//find parametter start
		Tv_PtrScpt	=	(Ptr_GrP4ScptObjHd)A_PtrScpt;
#ifdef GR_P4_EDITOR
		if ( E_GrP4ScptObjStx != Tv_PtrScpt->Stx )
		{
			DbgMsgPrint( "Cls_GrP4DoBase::ParaCopyFromScpt - bad object script.\n" );
		}
#endif
		//find current parametter
		Tv_ParaPrgs	=	0;
		while ( Tv_ParaPrgs < Tv_PtrScpt->ParaSize )
		{
			Tv_PtrPrHd	=	(Ptr_GrP4ScptParaHd)( (__u32)Tv_PtrScpt + sizeof(St_GrP4ScptObjHd) + Tv_ParaPrgs );
#ifdef GR_P4_EDITOR
			// check parameter header
			if ( E_GrP4ScptParaStx != Tv_PtrPrHd->Stx )
			{
				DbgMsgPrint( "Cls_GrP4DoBase::ParaCopyFromScpt - script %d : bad parameter.\n" , Tv_PtrScpt->ScptId );
			}
#endif
			//check class id
			if ( A_ClassId == Tv_PtrPrHd->ClassId )
			{
				Tv_CtSize	=	A_ParaSize;
				if ( Tv_CtSize >= (__u32)Tv_PtrPrHd->ParaSize )
				{
					Tv_CtSize	=	(__u32)Tv_PtrPrHd->ParaSize;
				}
#ifdef GR_P4_EDITOR
				// check parameter header
				if ( A_ParaSize != (__u32)Tv_PtrPrHd->ParaSize )
				{
					DbgMsgPrint( "Cls_GrP4DoBase::ParaCopyFromScpt - script %d , class %d : para size missmatch.\n" , Tv_PtrScpt->ScptId, A_ClassId );
				}
#endif
				Tv_PtrPrCt	=	(void*)( (__u32)Tv_PtrPrHd + sizeof(St_GrP4ScptParaHd) );
				GrDumyCopyMem( A_PtrPara, Tv_PtrPrCt, Tv_CtSize );
				break;
			}
			//next
			Tv_ParaPrgs	=	Tv_ParaPrgs + sizeof(St_GrP4ScptParaHd) + Tv_PtrPrHd->ParaSize;
		}
#ifdef GR_P4_EDITOR
		if ( Tv_ParaPrgs >= Tv_PtrScpt->ParaSize )
		{
			if ( (E_GrP4ScptIdPageBase > Tv_PtrScpt->ScptId) || ((E_GrP4ScptIdPageBase + E_GrP4DvlPageMaxCnt) <= Tv_PtrScpt->ScptId) )
			{
				DbgMsgPrint( "Cls_GrP4DoBase::ParaCopyFromScpt - script %d : parameter not found.\n", Tv_PtrScpt->ScptId );
			}
		}
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBase::RtlScptParaSubUpdt( void )
{
		ParaCopyFromScpt( &m_ParaDoBase, sizeof(m_ParaDoBase) , m_PtrScpt , E_GrP4DoClassBase );
		m_IsVisible	=	m_ParaDoBase.IsVisible;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBase::RtlScptParaUpdt( void* A_PtrScpt, BOOL8 A_IsNoCopy )
{
	// local -------------------
	// code --------------------
		m_PtrScpt			=	A_PtrScpt;
#ifdef GR_P4_EDITOR
		m_IsDvlParaNoCopy	=	A_IsNoCopy;
#endif
		RtlScptParaSubUpdt();
		// ReqDraw();
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
__u16	Cls_GrP4DoBase::RtlParaSizeGet( void )
{
		return	m_ParaSize;
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoBase::RtlScptParaSubDump(void)
{
	// local -------------------
		Ptr_GrP4ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		//build head
		Tv_PtrHd			=	(Ptr_GrP4ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP4ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP4DoClassBase;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaDoBase);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaDoBase, sizeof(m_ParaDoBase) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaDoBase)	+ sizeof(St_GrP4ScptParaHd);
}
#endif
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBase::IsVisible( void )
{
		return	m_IsVisible;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBase::IsCanVisible( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrP4DoBase*	Tv_Obj;
	// code --------------------
		Tv_Result	=	TRUE;
		Tv_Obj		=	this;
		while ( NULL != Tv_Obj )
		{
			if ( !Tv_Obj->IsVisible() )
			{
				Tv_Result		=	FALSE;
				break;
			}
			//next
			Tv_Obj	=	Tv_Obj->ParentObjGet();
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBase::IsEnable( void )
{
		return	m_ParaDoBase.IsEnable;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBase::IsCanEnable( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrP4DoBase*	Tv_Obj;
	// code --------------------
		//init
		Tv_Result	=	m_ParaDoBase.IsEnable;
		Tv_Obj		=	m_ObjPrnt;
		while ( NULL != Tv_Obj )
		{
			// get parent
			if ( !Tv_Obj->IsEnable() )
			{
				Tv_Result		=	FALSE;
				break;
			}
			//next
			Tv_Obj	=	Tv_Obj->ParentObjGet();
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBase::IsKeyAble( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( m_IsKeyAble && (!m_ParaDoBase.IsKeyBlock) )
		{
			Tv_Result	=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBase::IsKeyBlock( void )
{
		return	m_ParaDoBase.IsKeyBlock;
}
//--------------------------------------------------------------------
Cls_GrP4DoBase*	Cls_GrP4DoBase::WindowGet( void )
{
		return	m_ObjWin;
}
//--------------------------------------------------------------------
Cls_GrP4DoBase*	Cls_GrP4DoBase::FrameGet( void )
{
		return	m_ObjFrm;
}
//--------------------------------------------------------------------
void*	Cls_GrP4DoBase::ConsoleGet( void )
{
		return	m_ObjCon;
}
//--------------------------------------------------------------------
Cls_GrP4DoBase*	Cls_GrP4DoBase::ParentObjGet( void )
{
		return	m_ObjPrnt;
}
//--------------------------------------------------------------------
Cls_GrP4DoBase*	Cls_GrP4DoBase::NextObjGet( void )
{
		return	m_ObjNext;
}
//--------------------------------------------------------------------
Cls_GrP4DoBase*	Cls_GrP4DoBase::SlaveObjGet( void )
{
		return	m_ObjSlave;
}
//--------------------------------------------------------------------
Cls_GrP4DoBase*	Cls_GrP4DoBase::PrevObjGet( void )
{
	// local -------------------
		Cls_GrP4DoBase*	Tv_Result;
		Cls_GrP4DoBase*	Tv_ObjNext;
	// code --------------------
		//init
		Tv_Result		=	NULL;

		if ( NULL != m_ObjPrnt )
		{
			Tv_Result	=	m_ObjPrnt->SlaveObjGet();
			if ( Tv_Result == (Cls_GrP4DoBase*)this )
			{
				Tv_Result	=	NULL;
			}
			else
			{
				while ( NULL != Tv_Result )
				{
					//get next
					Tv_ObjNext	=	Tv_Result->NextObjGet();
					if ( Tv_ObjNext == (Cls_GrP4DoBase*)this )
					{
						//found
						break;
					}
					//next
					Tv_Result	=	Tv_ObjNext;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBase::PosSet(Ptr_GrRect A_PtrRect, __u8 A_PosRel)
{
	// local -------------------
		BOOL8	Tv_IsVisible;
	// code --------------------
		Tv_IsVisible	=	IsCanVisible();
		if ( Tv_IsVisible )
		{
			ReqDraw();		// predraw
		}
		m_PosRect	=	*A_PtrRect;
		m_PosRel	=	A_PosRel;
		if ( Tv_IsVisible )
		{
			ReqDraw();		// postdraw
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBase::PosMoveAbs(__s32 A_X, __s32 A_Y, BOOL8 A_IsMovX, BOOL8 A_IsMovY)
{
	// local -------------------
		BOOL8	Tv_IsAble;
		__s32	Tv_DeltaX;
		__s32	Tv_DeltaY;
		St_GrRect	Tv_RcPrnt;
		St_GrRect	Tv_RcThis;
	// code --------------------

		Tv_IsAble	=	TRUE;
		// get parent position
		if (NULL == m_ObjPrnt)
		{
			Tv_RcPrnt.left		=	0;
			Tv_RcPrnt.top			=	0;
			Tv_RcPrnt.right		=	(__s32)((Cls_GrP4ConBase*)m_ObjCon)->GetResX();
			Tv_RcPrnt.bottom	=	(__s32)((Cls_GrP4ConBase*)m_ObjCon)->GetResY();
		}
		else
		{
			Tv_IsAble	=	m_ObjPrnt->ScrRectGet(&Tv_RcPrnt, TRUE);
		}

		if (Tv_IsAble)
		{
			// get this rect
			if (ScrRectGet(&Tv_RcThis,TRUE))
			{
				// calculate delta
				Tv_DeltaX	=	A_X - (Tv_RcThis.left - Tv_RcPrnt.left);
				Tv_DeltaY	=	A_Y - (Tv_RcThis.top - Tv_RcPrnt.top);

				PosMoveRel(Tv_DeltaX, Tv_DeltaY, A_IsMovX, A_IsMovY);
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP4DoBase::PosMoveRel(__s32 A_X, __s32 A_Y, BOOL8 A_IsMovX, BOOL8 A_IsMovY)
{
	// local -------------------
		BOOL8	Tv_IsVisible;
		__u8	Tv_Basis;
	// code --------------------
		Tv_IsVisible	=	IsCanVisible();
		if (Tv_IsVisible)
		{
			ReqDraw();		// predraw
		}

		if (A_IsMovX)
		{
			// left
			Tv_Basis	=	m_PosRel & 3;
			if ((E_GrP4PosBasisSt == Tv_Basis) || (E_GrP4PosBasisCtSt == Tv_Basis))
			{
				m_PosRect.left	=	m_PosRect.left + A_X;
			}
			else
			{
				m_PosRect.left	=	m_PosRect.left - A_X;
			}
			// right
			Tv_Basis	=	(m_PosRel >> 4) & 3;
			if ((E_GrP4PosBasisSt == Tv_Basis) || (E_GrP4PosBasisCtSt == Tv_Basis))
			{
				m_PosRect.right	=	m_PosRect.right + A_X;
			}
			else
			{
				m_PosRect.right	=	m_PosRect.right - A_X;
			}
		}

		if (A_IsMovY)
		{
			// top
			Tv_Basis	=	(m_PosRel >> 2) & 3;
			if ((E_GrP4PosBasisSt == Tv_Basis) || (E_GrP4PosBasisCtSt == Tv_Basis))
			{
				m_PosRect.top		=	m_PosRect.top + A_Y;
			}
			else
			{
				m_PosRect.top		=	m_PosRect.top - A_Y;
			}

			// bottom
			Tv_Basis	=	(m_PosRel >> 6) & 3;
			if ((E_GrP4PosBasisSt == Tv_Basis) || (E_GrP4PosBasisCtSt == Tv_Basis))
			{
				m_PosRect.bottom	=	m_PosRect.bottom + A_Y;
			}
			else
			{
				m_PosRect.bottom	=	m_PosRect.bottom - A_Y;
			}
		}

		// redraw
		if (Tv_IsVisible)
		{
			ReqDraw();		// postdraw
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBase::SizeSetAbs(__s32 A_X, __s32 A_Y, BOOL8 A_IsChgX, BOOL8 A_IsChgY)
{
	// local -------------------
		__s32	Tv_DeltaX;
		__s32	Tv_DeltaY;
		St_GrRect	Tv_RcThis;
	// code --------------------
		// get this rect
		if (ScrRectGet(&Tv_RcThis, TRUE))
		{
			// calculate delta
			Tv_DeltaX	=	A_X - (Tv_RcThis.right - Tv_RcThis.left);
			Tv_DeltaY	=	A_Y - (Tv_RcThis.bottom - Tv_RcThis.top);

			SizeSetRel(Tv_DeltaX, Tv_DeltaY, A_IsChgX, A_IsChgY);
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBase::SizeSetRel(__s32 A_X, __s32 A_Y, BOOL8 A_IsChgX, BOOL8 A_IsChgY)
{
	// local -------------------
		BOOL8	Tv_IsVisible;
		__u8	Tv_Basis;
	// code --------------------
		Tv_IsVisible	=	IsCanVisible();
		if (Tv_IsVisible)
		{
			ReqDraw();		// predraw
		}

		if (A_IsChgX)
		{
			// right
			Tv_Basis	=	(m_PosRel >> 4) & 3;
			if ((E_GrP4PosBasisSt == Tv_Basis) || (E_GrP4PosBasisCtSt == Tv_Basis))
			{
				m_PosRect.right	=	m_PosRect.right + A_X;
			}
			else
			{
				m_PosRect.right	=	m_PosRect.right - A_X;
			}
		}

		if (A_IsChgY)
		{
			// bottom
			Tv_Basis	=	(m_PosRel >> 6) & 3;
			if ((E_GrP4PosBasisSt == Tv_Basis) || (E_GrP4PosBasisCtSt == Tv_Basis))
			{
				m_PosRect.bottom	=	m_PosRect.bottom + A_Y;
			}
			else
			{
				m_PosRect.bottom	=	m_PosRect.bottom - A_Y;
			}
		}

		// redraw
		if (Tv_IsVisible)
		{
			ReqDraw();		// postdraw
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBase::IsWindow( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( NULL == m_ObjPrnt )
		{
			Tv_Result	=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBase::IsFocused( void )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		//check focused
		if ( this == ((Cls_GrP4ConBase*)m_ObjCon)->FocusObjGet() )
		{
			Tv_Result		=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBase::LcFreeFocus( void )
{
	// local -------------------
	// code --------------------
		if ( !IsWindow() )
		{
			//check window focused
			if ( this == ((Cls_GrP4DoWin*)m_ObjWin)->LastFocusObjGet() )
			{
				//release window last focus
				((Cls_GrP4DoWin*)m_ObjWin)->RtlLastFocusObjSet( NULL );
			}
			if ( IsFocused() )
			{
				((Cls_GrP4ConBase*)m_ObjCon)->FocusObjSet( NULL );
				((Cls_GrP4ConBase*)m_ObjCon)->FocusCorrect();
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBase::VisibleSet( BOOL8 A_IsVisible )
{
	// local -------------------
		BOOL8	Tv_IsOld;
	// code --------------------
		Tv_IsOld		=	IsCanVisible();
		m_IsVisible	=	A_IsVisible;
		if ( Tv_IsOld != IsCanVisible() )
		{
			//check focus
			if ( !m_IsVisible )
			{
				LcFreeFocus();
			}
			ReqDraw( TRUE );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBase::EnableSet( BOOL8 A_IsEnable )
{
	// local -------------------
		BOOL8	Tv_IsOld;
	// code --------------------
		Tv_IsOld		=	IsCanEnable();
		m_ParaDoBase.IsEnable	=	A_IsEnable;
		if ( Tv_IsOld != IsCanEnable() )
		{
			//check focus
			if ( !m_ParaDoBase.IsEnable )
			{
				LcFreeFocus();
			}
			ReqDraw();
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBase::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
		//__u8	Tv_RelPos;
		St_GrRect	Tv_Rc;

	// code --------------------
		Tv_Result	=	TRUE;

		switch ( A_Cmd )
		{
			case	E_GrP4UpiClassIdGet:
				A_PtrInt[0]	=	(__s32)((__u32)m_ClassId);
				break;
			case E_GrP4UpiScptIdGet:
				A_PtrInt[0]	=	(__s32)((__u32)m_ScptId);
				break;
			case E_GrP4UpiPosGet:
				A_PtrInt[1]	=	0;
				A_PtrInt[2]	=	0;
				if (CliRectGet(&Tv_Rc, TRUE))
				{
					A_PtrInt[1]	=	Tv_Rc.left;
					A_PtrInt[2]	=	Tv_Rc.top;
				}
				break;
			case E_GrP4UpiPosMovAbs:
				PosMoveAbs(A_PtrInt[1], A_PtrInt[2]);
				break;
			case E_GrP4UpiPosMovRel:
				PosMoveRel(A_PtrInt[1], A_PtrInt[2]);
				break;
			case E_GrP4UpiSizeGet:
				// calculate size
				SizeGet(&A_PtrInt[1], &A_PtrInt[2]);
				break;
			case E_GrP4UpiSizeSetAbs:
				SizeSetAbs(A_PtrInt[1], A_PtrInt[2]);
				break;
			case E_GrP4UpiSizeSetRel:
				SizeSetRel(A_PtrInt[1], A_PtrInt[2]);
				break;
			case E_GrP4UpiVisibleGet:
				A_PtrInt[0]	=	(__s32)m_IsVisible;
				break;
			case E_GrP4UpiVisibleSet:
				VisibleSet( (BOOL8)A_PtrInt[1] );
				break;
			case E_GrP4UpiEnableGet:
				A_PtrInt[0]	=	(__s32)IsEnable();
				break;
			case E_GrP4UpiEnableSet:
				EnableSet( (BOOL8)A_PtrInt[1] );
				break;
			case E_GrP4UpiFcsMvTblGet:
				A_PtrInt[1]	=	(__s32)m_ParaDoBase.FcsMvTbl[E_GrP4DoFocusMvIdxLeft];
				A_PtrInt[2]	=	(__s32)m_ParaDoBase.FcsMvTbl[E_GrP4DoFocusMvIdxRight];
				A_PtrInt[3]	=	(__s32)m_ParaDoBase.FcsMvTbl[E_GrP4DoFocusMvIdxUp];
				A_PtrInt[4]	=	(__s32)m_ParaDoBase.FcsMvTbl[E_GrP4DoFocusMvIdxDown];
				break;
			case E_GrP4UpiFcsMvTblSet:
				m_ParaDoBase.FcsMvTbl[E_GrP4DoFocusMvIdxLeft]		=	(__u16)A_PtrInt[1];
				m_ParaDoBase.FcsMvTbl[E_GrP4DoFocusMvIdxRight]	=	(__u16)A_PtrInt[2];
				m_ParaDoBase.FcsMvTbl[E_GrP4DoFocusMvIdxUp]			=	(__u16)A_PtrInt[3];
				m_ParaDoBase.FcsMvTbl[E_GrP4DoFocusMvIdxDown]		=	(__u16)A_PtrInt[4];
				break;
			case E_GrP4UpiUserDataGet:
				A_PtrInt[0]	=	0;
				if ( E_GrP4DoUserDataCnt > (__u32)A_PtrInt[1] )
				{
					A_PtrInt[0]	=	(__s32)m_ParaDoBase.UserDataTbl[ A_PtrInt[1] ];
				}
				break;
			case E_GrP4UpiUserDataSet:
				if ( E_GrP4DoUserDataCnt > (__u32)A_PtrInt[1] )
				{
					m_ParaDoBase.UserDataTbl[A_PtrInt[1]]	=	A_PtrInt[2];
				}
				break;
			case E_GrP4UpiReqDraw:
				ReqDraw( (BOOL8)A_PtrInt[1] );
				break;
			case E_GrP4UpiFocusIdSet:
				((Cls_GrP4ConBase*)m_ObjCon)->FocusObjSet( this );
				break;
			default:
				Tv_Result	=	FALSE;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBase::RtlNextObjSet( Cls_GrP4DoBase* A_Obj )
{
		m_ObjNext	=	A_Obj;		
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBase::RtlSlaveObjSet( Cls_GrP4DoBase* A_Obj )
{
		m_ObjSlave	=	A_Obj;
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoBase::RtlScptDelMode( void )
{

	// local -------------------
		Cls_GrP4DoBase*	Tv_Slave;
	// code --------------------
		m_IsScptDel	=	TRUE;
		// check frame
		if ( E_GrP4DoClassFrame != m_ClassId )
		{
			// set slave
			Tv_Slave	=	m_ObjSlave;
			while ( NULL != Tv_Slave )
			{
				Tv_Slave->RtlScptDelMode();
				// next
				Tv_Slave	=	Tv_Slave->NextObjGet();
			}
		}
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoBase::ObjSlaveFreeAll( void )
{
		while ( NULL != m_ObjSlave )
		{
#ifdef GR_P4_EDITOR
			if ( (NULL == m_ObjFrm) && m_IsScptDel )
			{
				m_ObjSlave->RtlScptDelMode();		// update script delete mode
			}
#endif
			delete	m_ObjSlave;
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBase::ScrRectGet(Ptr_GrRect A_PtrRtRect, BOOL8 A_IsForceVisible)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrRect	Tv_RcBase;
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		//check visible
		if (m_IsVisible || A_IsForceVisible)
		{
			if (NULL == m_ObjPrnt)
			{
				// get console rect
				Tv_RcBase.left		=	0;
				Tv_RcBase.top			=	0;
				Tv_RcBase.right		=	(__s32)((Cls_GrP4ConBase*)m_ObjCon)->GetResX();
				Tv_RcBase.bottom	=	(__s32)((Cls_GrP4ConBase*)m_ObjCon)->GetResY();

				Tv_Result	=	TRUE;
			}
			else
			{
				Tv_Result	=	m_ObjPrnt->ScrRectGet(&Tv_RcBase, A_IsForceVisible);
			}

			if (Tv_Result)
			{
				Tv_Result	=	BasisRectCalc(&Tv_RcBase, m_PosRel, (__s16)m_PosRect.left, (__s16)m_PosRect.top, (__s16)m_PosRect.right, (__s16)m_PosRect.bottom, A_PtrRtRect);
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBase::CliRectGet(Ptr_GrRect A_PtrRtRect, BOOL8 A_IsForceVisible)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrRect	Tv_RcBase;
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		//check visible
		if (m_IsVisible || A_IsForceVisible)
		{
			if (NULL == m_ObjPrnt)
			{
				// get console rect
				Tv_RcBase.left		=	0;
				Tv_RcBase.top			=	0;
				Tv_RcBase.right		=	(__s32)((Cls_GrP4ConBase*)m_ObjCon)->GetResX();
				Tv_RcBase.bottom	=	(__s32)((Cls_GrP4ConBase*)m_ObjCon)->GetResY();

				Tv_Result	=	TRUE;
			}
			else
			{
				Tv_Result	=	m_ObjPrnt->ScrRectGet(&Tv_RcBase, A_IsForceVisible);
			}

			if (Tv_Result)
			{
				Tv_Result	=	BasisRectCalc(&Tv_RcBase, m_PosRel, (__s16)m_PosRect.left, (__s16)m_PosRect.top, (__s16)m_PosRect.right, (__s16)m_PosRect.bottom, A_PtrRtRect);
				if (Tv_Result)
				{
					A_PtrRtRect->left		=	A_PtrRtRect->left - Tv_RcBase.left;
					A_PtrRtRect->top			=	A_PtrRtRect->top - Tv_RcBase.top;
					A_PtrRtRect->right		=	A_PtrRtRect->right	 - Tv_RcBase.left;
					A_PtrRtRect->bottom	=	A_PtrRtRect->bottom - Tv_RcBase.top;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBase::ReqDraw( BOOL8 A_IsNoChk )
{
	// local -------------------
		St_GrRect	Tv_RcScr;
	// code --------------------
		if ( ScrRectGet( &Tv_RcScr, A_IsNoChk ) )
		{
			((Cls_GrP4ConBase*)m_ObjCon)->PaintReq( &Tv_RcScr );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBase::ReqPartDraw( Ptr_GrRect A_PtrRc, BOOL8 A_IsNoChk )
{
	// local -------------------
		St_GrRect	Tv_RcScr;
		St_GrRect	Tv_RcTg;
	// code --------------------
		if ( ScrRectGet( &Tv_RcScr, A_IsNoChk ) )
		{
			Tv_RcTg.left		=	Tv_RcScr.left;
			Tv_RcTg.top			=	Tv_RcScr.top;
			Tv_RcTg.right		=	Tv_RcScr.right;
			Tv_RcTg.bottom	=	Tv_RcScr.bottom;
			if ( (0 < A_PtrRc->left) && ((Tv_RcScr.right - Tv_RcScr.left) > A_PtrRc->left) )
			{
				Tv_RcTg.left	=	Tv_RcScr.left + A_PtrRc->left;
			}
			if ( (0 < A_PtrRc->top) && ((Tv_RcScr.bottom - Tv_RcScr.top) > A_PtrRc->top) )
			{
				Tv_RcTg.top	=	Tv_RcScr.top + A_PtrRc->top;
			}
			if ( (0 < A_PtrRc->right) && ((Tv_RcScr.right - Tv_RcScr.left) > A_PtrRc->right) )
			{
				Tv_RcTg.right	=	Tv_RcScr.left + A_PtrRc->right;
			}
			if ( (0 < A_PtrRc->bottom) && ((Tv_RcScr.bottom - Tv_RcScr.top) > A_PtrRc->bottom) )
			{
				Tv_RcTg.bottom	=	Tv_RcScr.top + A_PtrRc->bottom;
			}
						
			if ( (Tv_RcTg.left < Tv_RcTg.right) && (Tv_RcTg.top < Tv_RcTg.bottom) )
			{
				((Cls_GrP4ConBase*)m_ObjCon)->PaintReq( &Tv_RcTg );
			}
			
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBase::RtlPaint( Ptr_GrRect A_PtrRect )
{
	// local -------------------
		BOOL8	Tv_IsCan;
		Cls_GrP4DoBase*	Tv_Obj;
	// code --------------------
		// check can paint
		if ( ((Cls_GrP4ConBase*)m_ObjCon)->IsPstCanPush() )
		{
			// check can visible
			if ( IsCanVisible() )
			{
				// get my screen rect
				if ( ScrRectGet( &m_PntRcScr ) )
				{
					// get update rect
					m_PntRcUpdt	=	m_PntRcScr;
					if ( m_PntRcUpdt.left < A_PtrRect->left )
					{
						m_PntRcUpdt.left	=	A_PtrRect->left;
					}
					if ( m_PntRcUpdt.right > A_PtrRect->right )
					{
						m_PntRcUpdt.right	=	A_PtrRect->right;
					}
					if ( m_PntRcUpdt.top < A_PtrRect->top )
					{
						m_PntRcUpdt.top	=	A_PtrRect->top;
					}
					if ( m_PntRcUpdt.bottom > A_PtrRect->bottom )
					{
						m_PntRcUpdt.bottom	=	A_PtrRect->bottom;
					}
					// check able display
					if ( (m_PntRcUpdt.left < m_PntRcUpdt.right) && (m_PntRcUpdt.top < m_PntRcUpdt.bottom) )
					{
						// scan slave
						Tv_IsCan	=	TRUE;
						Tv_Obj		=	m_ObjSlave;
						while ( (NULL != Tv_Obj) && (Tv_IsCan) )
						{
							Tv_Obj->RtlPaint( &m_PntRcUpdt );
							Tv_IsCan	=	((Cls_GrP4ConBase*)m_ObjCon)->IsPstCanPush();
							Tv_Obj	=	Tv_Obj->NextObjGet();
						}
						// check able paint
						if ( Tv_IsCan )
						{
							// update script
							ProcPaint();
						}
					}
				}
			}

			/*
			// paint next control
			if ( NULL != m_ObjNext )
			{
				m_ObjNext->RtlPaint( A_PtrRect );
			}
			*/
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBase::ProcPaint( void )
{
	// paint script update
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBase::ProcUiMsg( Ptr_GrP4MsgHd A_PtrMsg )
{
	// local -------------------
		BOOL8	Tv_IsRun;
	// code --------------------
		switch ( A_PtrMsg->Cmd )
		{
			case E_GrP4MsgCmdKeyDown:
				Tv_IsRun	=	RtlKeyDownProc(((Ptr_GrP4MsgKey)A_PtrMsg)->Dev, ((Ptr_GrP4MsgKey)A_PtrMsg)->Key);
				break;
			case E_GrP4MsgCmdKeyUp:
				Tv_IsRun	=	RtlKeyUpProc(((Ptr_GrP4MsgKey)A_PtrMsg)->Dev, ((Ptr_GrP4MsgKey)A_PtrMsg)->Key);
				break;
			case E_GrP4MsgCmdMouseLbtnDown:
				Tv_IsRun	=	RtlMseLbtnDownProc(((Ptr_GrP4MsgMouse)A_PtrMsg)->X, ((Ptr_GrP4MsgMouse)A_PtrMsg)->Y, ((Ptr_GrP4MsgMouse)A_PtrMsg)->BtnMap);
				break;
			case E_GrP4MsgCmdMouseLbtnUp:
				Tv_IsRun	=	RtlMseLbtnUpProc(((Ptr_GrP4MsgMouse)A_PtrMsg)->X, ((Ptr_GrP4MsgMouse)A_PtrMsg)->Y, ((Ptr_GrP4MsgMouse)A_PtrMsg)->BtnMap);
				break;
			case E_GrP4MsgCmdMouseRbtnDown:
				Tv_IsRun	=	RtlMseRbtnDownProc(((Ptr_GrP4MsgMouse)A_PtrMsg)->X, ((Ptr_GrP4MsgMouse)A_PtrMsg)->Y, ((Ptr_GrP4MsgMouse)A_PtrMsg)->BtnMap);
				break;
			case E_GrP4MsgCmdMouseRbtnUp:
				Tv_IsRun	=	RtlMseRbtnUpProc(((Ptr_GrP4MsgMouse)A_PtrMsg)->X, ((Ptr_GrP4MsgMouse)A_PtrMsg)->Y, ((Ptr_GrP4MsgMouse)A_PtrMsg)->BtnMap);
				break;
			case E_GrP4MsgCmdMouseMbtnDown:
				Tv_IsRun	=	RtlMseMbtnDownProc(((Ptr_GrP4MsgMouse)A_PtrMsg)->X, ((Ptr_GrP4MsgMouse)A_PtrMsg)->Y, ((Ptr_GrP4MsgMouse)A_PtrMsg)->BtnMap);
				break;
			case E_GrP4MsgCmdMouseMbtnUp:
				Tv_IsRun	=	RtlMseMbtnUpProc(((Ptr_GrP4MsgMouse)A_PtrMsg)->X, ((Ptr_GrP4MsgMouse)A_PtrMsg)->Y, ((Ptr_GrP4MsgMouse)A_PtrMsg)->BtnMap);
				break;
			case E_GrP4MsgCmdWheel:
				if ( ((Ptr_GrP4MsgWheel)A_PtrMsg)->IsDown )
				{
					Tv_IsRun	=	ProcEvtWheelDown();
				}
				else
				{
					Tv_IsRun	=	ProcEvtWheelUp();
				}
				break;
			case E_GrP4MsgCmdFocusGet:
				ProcEvtFocusGet();
				break;
			case E_GrP4MsgCmdFocusLoss:
				ProcEvtFocusLoss();
				break;
			case E_GrP4MsgCmdMouseEnter:
				ProcEvtMseEnter();
				break;
			case	E_GrP4MsgCmdMouseLeave:
				ProcEvtMseLeave();
				break;
			case E_GrP4MsgCmdTickPeriod:
				ProcEvtTimeTick( ((Ptr_GrP4MsgTimeTick)A_PtrMsg)->Tick );
				break;
			case E_GrP4MsgCmdSysStat:
				ProcEvtSysStat( ((Ptr_GrP4MsgSysStat)A_PtrMsg)->Type );
				break;
			case E_GrP4MsgCmdFramePageChged:
				ProcEvtFramePageChged( ((Ptr_GrP4MsgFrmPageChged)A_PtrMsg)->FramdId, ((Ptr_GrP4MsgFrmPageChged)A_PtrMsg)->PageId );
				break;
			case E_GrP4MsgCmdVkbdEnd:
				ProcEvtVkbdEditEnd();
				break;
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBase::RtlUiMsgProc( Ptr_GrP4MsgHd A_PtrMsg )
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrP4DoBase*	Tv_Obj;
	// code --------------------
		Tv_Result	=	FALSE;
		// check this window
		if ( (m_ScptId == A_PtrMsg->ObjId) || (E_GrP4ScptIdAll == A_PtrMsg->ObjId) )
		{
			// run message
			ProcUiMsg( A_PtrMsg );
		}
		// check this object
		if ( m_ScptId == A_PtrMsg->ObjId )
		{
			Tv_Result	=	TRUE;
		}
		else
		{
			Tv_Obj	=	m_ObjSlave;
			while ( NULL != Tv_Obj )
			{
				Tv_Result	=	Tv_Obj->RtlUiMsgProc( A_PtrMsg );
				if ( Tv_Result )
				{
					break;
				}
				// next
				Tv_Obj	=	Tv_Obj->NextObjGet();
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBase::ProcEvtKeyDown( __u8 A_Dev, __u8 A_Key )
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBase::ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key )
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBase::ProcEvtMseMove( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBase::ProcEvtMseLbtnDown( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( m_IsKeyAble )
		{
			((Cls_GrP4ConBase*)m_ObjCon)->FocusObjSet( this );
			Tv_Result	=	TRUE;
		}
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBase::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBase::ProcEvtMseRbtnDown( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBase::ProcEvtMseRbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBase::ProcEvtMseMbtnDown( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBase::ProcEvtMseMbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBase::ProcEvtWheelUp( void )
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBase::ProcEvtWheelDown( void )
{
		return	FALSE;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBase::ProcEvtFocusGet( void )
{
		((Cls_GrP4DoWin*)m_ObjWin)->RtlLastFocusObjSet( this );
		if ( NULL != m_ObjFrm )
		{
			((Cls_GrP4DoFrame*)m_ObjFrm)->RtlLastFocusObjIdSet( m_ScptId );
		}
		RelStdCtrlActive( TRUE );
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBase::ProcEvtFocusLoss( void )
{
		RelStdCtrlActive( FALSE );
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBase::ProcEvtMseEnter( void )
{
	if ( m_IsMouseAble )
	{
			m_IsOnMouse	=	TRUE;
			RelStdCtrlActive( TRUE );
			ReqDraw();
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBase::ProcEvtMseLeave( void )
{
		if ( m_IsMouseAble )
		{
			m_IsOnMouse	=	FALSE;
			RelStdCtrlActive( FALSE );
			ReqDraw();
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBase::ProcEvtTimeTick( __u32 A_Tick )
{

}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP4DoBase::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
{
		*A_PtrRtCus	=	-1;
		return	NULL;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBase::BasisRectCalc( Ptr_GrRect A_PtrRcBase, __u8 A_Basis, __s16 A_Left, __s16 A_Top, __s16 A_Right, __s16 A_Bottom, Ptr_GrRect A_PtrRtRc )
{
	// local -------------------
		BOOL8	Tv_Result;
		__s32		Tv_CtHz;
		__s32		Tv_CtVt;
		__u8	Tv_Basis;
	// code --------------------
		// init
		Tv_Result	=	FALSE;
		Tv_CtHz		=	((A_PtrRcBase->right - A_PtrRcBase->left) >> 1) + A_PtrRcBase->left;
		Tv_CtVt		=	((A_PtrRcBase->bottom - A_PtrRcBase->top) >> 1) + A_PtrRcBase->top;
		// left
		Tv_Basis	=	A_Basis & 0x03;
		switch ( Tv_Basis )
		{
			case E_GrP4PosBasisSt:
				A_PtrRtRc->left	=	A_PtrRcBase->left + (__s32)A_Left;
				break;
			case E_GrP4PosBasisEd:
				A_PtrRtRc->left	=	A_PtrRcBase->right - (__s32)A_Left;
				break;
			case E_GrP4PosBasisCtSt:
				A_PtrRtRc->left	=	Tv_CtHz - (__s32)A_Left;
				break;
			default:
				A_PtrRtRc->left	=	Tv_CtHz + (__s32)A_Left;
				break;
		}
		// top
		Tv_Basis	=	(A_Basis >> 2) & 0x03;
		switch ( Tv_Basis )
		{
			case E_GrP4PosBasisSt:
				A_PtrRtRc->top		=	A_PtrRcBase->top + (__s32)A_Top;
				break;
			case E_GrP4PosBasisEd:
				A_PtrRtRc->top		=	A_PtrRcBase->bottom - (__s32)A_Top;
				break;
			case E_GrP4PosBasisCtSt:
				A_PtrRtRc->top		=	Tv_CtVt - (__s32)A_Top;
				break;
			default:
				A_PtrRtRc->top		=	Tv_CtVt + (__s32)A_Top;
				break;
		}
		// right
		Tv_Basis	=	(A_Basis >> 4) & 0x03;
		switch ( Tv_Basis )
		{
			case E_GrP4PosBasisSt:
				A_PtrRtRc->right	=	A_PtrRcBase->left + (__s32)A_Right;
				break;
			case E_GrP4PosBasisEd:
				A_PtrRtRc->right	=	A_PtrRcBase->right - (__s32)A_Right;
				break;
			case E_GrP4PosBasisCtSt:
				A_PtrRtRc->right	=	Tv_CtHz - (__s32)A_Right;
				break;
			default:
				A_PtrRtRc->right	=	Tv_CtHz + (__s32)A_Right;
				break;
		}
		// bottom
		Tv_Basis	=	(A_Basis >> 6) & 0x03;
		switch ( Tv_Basis )
		{
			case E_GrP4PosBasisSt:
				A_PtrRtRc->bottom	=	A_PtrRcBase->top + (__s32)A_Bottom;
				break;
			case E_GrP4PosBasisEd:
				A_PtrRtRc->bottom	=	A_PtrRcBase->bottom - (__s32)A_Bottom;
				break;
			case E_GrP4PosBasisCtSt:
				A_PtrRtRc->bottom	=	Tv_CtVt - (__s32)A_Bottom;
				break;
			default:
				A_PtrRtRc->bottom	=	Tv_CtVt + (__s32)A_Bottom;
				break;
		}
		// check able
		if ( (A_PtrRtRc->left < A_PtrRtRc->right) && (A_PtrRtRc->top < A_PtrRtRc->bottom) )
		{
			Tv_Result	=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBase::PaintScriptPush( __u16 A_PstId, Ptr_GrRect A_PtrRc )
{
	// local -------------------
		Ptr_GrP4ScptPstData	Tv_PtrPst;
		__u8	Tv_PstCnt;
		__u8	Tv_WkIdx;
		__u8	Tv_Basis;
		Ptr_GrP4ConPntStackItm	Tv_PtrStk; 
		__s32		Tv_CtHz;
		__s32		Tv_CtVt;
		St_GrRect	Tv_RcUpdt;
		__u16	Tv_TxtId;
	// code --------------------
		// init
		Tv_CtHz		=	((A_PtrRc->right - A_PtrRc->left) >> 1) + A_PtrRc->left;
		Tv_CtVt		=	((A_PtrRc->bottom - A_PtrRc->top) >> 1) + A_PtrRc->top;
		// get script
		Tv_PtrPst	=	V_GrP4Mng->PstDataGet( A_PstId, &Tv_PstCnt );
		if ( 0 != Tv_PstCnt )
		{
			// decode script
			for (Tv_WkIdx=0;Tv_WkIdx < Tv_PstCnt;Tv_WkIdx++)
			{
				// get stack pointer
				Tv_PtrStk	=	((Cls_GrP4ConBase*)m_ObjCon)->PaintStackGetPushPtr();
				if ( NULL == Tv_PtrStk )
				{
					// quit loop
					break;
				}
				// check type
				if ( (E_GrP4ScptPstTypeFont == Tv_PtrPst->Type) || (E_GrP4ScptPstTypeStyle == Tv_PtrPst->Type) )
				{
					Tv_RcUpdt	=	*A_PtrRc;
				}
				else
				{
					// left
					Tv_Basis	=	Tv_PtrPst->Basis & 0x03;
					switch ( Tv_Basis )
					{
						case E_GrP4PosBasisSt:
							Tv_PtrStk->Rect.left	=	A_PtrRc->left + (__s32)Tv_PtrPst->Left;
							break;
						case E_GrP4PosBasisEd:
							Tv_PtrStk->Rect.left	=	A_PtrRc->right - (__s32)Tv_PtrPst->Left;
							break;
						case E_GrP4PosBasisCtSt:
							Tv_PtrStk->Rect.left	=	Tv_CtHz - (__s32)Tv_PtrPst->Left;
							break;
						default:
							Tv_PtrStk->Rect.left	=	Tv_CtHz + (__s32)Tv_PtrPst->Left;
							break;
					}
					// top
					Tv_Basis	=	(Tv_PtrPst->Basis >> 2) & 0x03;
					switch ( Tv_Basis )
					{
						case E_GrP4PosBasisSt:
							Tv_PtrStk->Rect.top		=	A_PtrRc->top + (__s32)Tv_PtrPst->Top;
							break;
						case E_GrP4PosBasisEd:
							Tv_PtrStk->Rect.top		=	A_PtrRc->bottom - (__s32)Tv_PtrPst->Top;
							break;
						case E_GrP4PosBasisCtSt:
							Tv_PtrStk->Rect.top		=	Tv_CtVt - (__s32)Tv_PtrPst->Top;
							break;
						default:
							Tv_PtrStk->Rect.top		=	Tv_CtVt + (__s32)Tv_PtrPst->Top;
							break;
					}
					// right
					Tv_Basis	=	(Tv_PtrPst->Basis >> 4) & 0x03;
					switch ( Tv_Basis )
					{
						case E_GrP4PosBasisSt:
							Tv_PtrStk->Rect.right	=	A_PtrRc->left + (__s32)Tv_PtrPst->Right;
							break;
						case E_GrP4PosBasisEd:
							Tv_PtrStk->Rect.right	=	A_PtrRc->right - (__s32)Tv_PtrPst->Right;
							break;
						case E_GrP4PosBasisCtSt:
							Tv_PtrStk->Rect.right	=	Tv_CtHz - (__s32)Tv_PtrPst->Right;
							break;
						default:
							Tv_PtrStk->Rect.right	=	Tv_CtHz + (__s32)Tv_PtrPst->Right;
							break;
					}
					// bottom
					Tv_Basis	=	(Tv_PtrPst->Basis >> 6) & 0x03;
					switch ( Tv_Basis )
					{
						case E_GrP4PosBasisSt:
							Tv_PtrStk->Rect.bottom	=	A_PtrRc->top + (__s32)Tv_PtrPst->Bottom;
							break;
						case E_GrP4PosBasisEd:
							Tv_PtrStk->Rect.bottom	=	A_PtrRc->bottom - (__s32)Tv_PtrPst->Bottom;
							break;
						case E_GrP4PosBasisCtSt:
							Tv_PtrStk->Rect.bottom	=	Tv_CtVt - (__s32)Tv_PtrPst->Bottom;
							break;
						default:
							Tv_PtrStk->Rect.bottom	=	Tv_CtVt + (__s32)Tv_PtrPst->Bottom;
							break;
					}
					// common update area
					Tv_RcUpdt	=	Tv_PtrStk->Rect;
					if ( Tv_RcUpdt.left < A_PtrRc->left )
					{
						Tv_RcUpdt.left	=	A_PtrRc->left;
					}
					if ( Tv_RcUpdt.top < A_PtrRc->top )
					{
						Tv_RcUpdt.top		=	A_PtrRc->top;
					}
					if ( Tv_RcUpdt.right > A_PtrRc->right )
					{
						Tv_RcUpdt.right	=	A_PtrRc->right;
					}
					if ( Tv_RcUpdt.bottom > A_PtrRc->bottom )
					{
						Tv_RcUpdt.bottom	=	A_PtrRc->bottom;
					}
				}

				// check current rect
				if ( (Tv_RcUpdt.left < Tv_RcUpdt.right) && (Tv_RcUpdt.top < Tv_RcUpdt.bottom) )
				{
					// update stack item
					Tv_PtrStk->Type				=	Tv_PtrPst->Type;
					Tv_PtrStk->Para0			=	Tv_PtrPst->Para0;
					Tv_PtrStk->Para1			=	Tv_PtrPst->Para1;
					Tv_PtrStk->Value			=	Tv_PtrPst->Value;
					switch ( Tv_PtrStk->Type )
					{
						case E_GrP4ScptPstTypeText:
							// get text id
							Tv_TxtId	=	(__u16)Tv_PtrPst->Para0 | (((__u16)Tv_PtrPst->Para1) << 8);
							// check local text
							if ( E_GrP4ScptTxtIdCtrl <= Tv_TxtId )
							{
								Tv_PtrStk->PtrOpt	=	(void*)RtlInTextGet( (__u8)(Tv_TxtId - E_GrP4ScptTxtIdCtrl), &Tv_PtrStk->IntOpt );
							}
							else
							{
								Tv_PtrStk->PtrOpt	=	(void*)V_GrP4Mng->TextPtrGet( Tv_TxtId );
								Tv_PtrStk->IntOpt	=	-1;		// no cursor
							}
							if ( NULL != Tv_PtrStk->PtrOpt )
							{
								// stack push
								((Cls_GrP4ConBase*)m_ObjCon)->PaintStackPush();
							}
							break;
						default:
							// stack push
							((Cls_GrP4ConBase*)m_ObjCon)->PaintStackPush();
							break;
					}

				}
				// next
				Tv_PtrPst ++;
			}
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBase::PaintFillClrPush( __u32 A_Clr, Ptr_GrRect A_PtrRc )
{
	// local -------------------
		Ptr_GrP4ConPntStackItm	Tv_PtrStk; 
	// code --------------------
		// get stack pointer
		Tv_PtrStk	=	((Cls_GrP4ConBase*)m_ObjCon)->PaintStackGetPushPtr();
		if ( NULL != Tv_PtrStk )
		{
			Tv_PtrStk->Rect		=	*A_PtrRc;
			Tv_PtrStk->Type		=	E_GrP4ScptPstTypeClrFill;
			Tv_PtrStk->Value	=	A_Clr;

			// stack push
			((Cls_GrP4ConBase*)m_ObjCon)->PaintStackPush();
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBase::IsCanMouse( void )
{
	// local -------------------
		BOOL8		Tv_Result;
		Cls_GrP4DoWin*	Tv_Win;
		Cls_GrP4DoBase*	Tv_Obj;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Win		=	(Cls_GrP4DoWin*)m_ObjWin;;
		if ( Tv_Win->IsWinMouseAble() )
		{
			Tv_Result		=	TRUE;
			Tv_Obj			=	m_ObjPrnt;
			while ( NULL != Tv_Obj )
			{
				// get parent
				if ( (!Tv_Obj->IsEnable()) || (!Tv_Obj->IsVisible()) )
				{
					Tv_Result		=	FALSE;
					break;
				}
				//next
				Tv_Obj	=	Tv_Obj->ParentObjGet();
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBase::IsCanKey( void )
{
	// local -------------------
		BOOL8		Tv_Result;
		Cls_GrP4DoWin*	Tv_Win;
		Cls_GrP4DoBase*	Tv_Obj;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( m_IsKeyAble && (!m_ParaDoBase.IsKeyBlock) && m_ParaDoBase.IsEnable && m_IsVisible )
		{
			Tv_Win		=	(Cls_GrP4DoWin*)m_ObjWin;;
			if ( Tv_Win->IsWinKeyAble() )
			{
				Tv_Result		=	TRUE;
				Tv_Obj			=	m_ObjPrnt;
				while ( NULL != Tv_Obj )
				{
					// get parent
					if ( (!Tv_Obj->IsEnable()) || (!Tv_Obj->IsVisible()) )
					{
						Tv_Result		=	FALSE;
						break;
					}
					//next
					Tv_Obj	=	Tv_Obj->ParentObjGet();
				}
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
Cls_GrP4DoBase*	Cls_GrP4DoBase::RtlFindMseObjByPos(__s32 A_X, __s32 A_Y, Ptr_GrRect A_PtrRcBase)
{
	// local -------------------
		Cls_GrP4DoBase*	Tv_Result;
		St_GrRect	Tv_RcScr;
		Cls_GrP4DoBase*	Tv_ObjSlave;
		Cls_GrP4DoBase*	TV_ObjFind;
	// code --------------------
		Tv_Result	=	NULL;
		// check attribute 
		if ( m_IsVisible && m_ParaDoBase.IsEnable  )
		{
			if (BasisRectCalc(A_PtrRcBase, m_PosRel, (__s16)m_PosRect.left, (__s16)m_PosRect.top, (__s16)m_PosRect.right, (__s16)m_PosRect.bottom, &Tv_RcScr))
			{
				// check window rect
				if ((A_X >= Tv_RcScr.left) && (A_X < Tv_RcScr.right) && (A_Y >= Tv_RcScr.top) && (A_Y < Tv_RcScr.bottom))
				{
					// update result
					if (m_IsMouseAble)
					{
						// update object
						Tv_Result	=	this;
					}
					// check slave
					Tv_ObjSlave	=	m_ObjSlave;
					while (NULL != Tv_ObjSlave)
					{
						// find
						TV_ObjFind	=	Tv_ObjSlave->RtlFindMseObjByPos(A_X, A_Y, &Tv_RcScr);
						if (NULL != TV_ObjFind)
						{
							Tv_Result	=	TV_ObjFind;
							break;
						}
						// next
						Tv_ObjSlave	=	Tv_ObjSlave->NextObjGet();
					}
				}
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
Cls_GrP4DoBase*	Cls_GrP4DoBase::RtlFindObjByPos(__s32 A_X, __s32 A_Y, Ptr_GrRect A_PtrRcBase)
{
	// local -------------------
		Cls_GrP4DoBase*	Tv_Result;
		St_GrRect	Tv_RcScr;
		Cls_GrP4DoBase*	Tv_ObjSlave;
		Cls_GrP4DoBase*	TV_ObjFind;
	// code --------------------
		Tv_Result	=	NULL;

		// calculate screen rect
		if (BasisRectCalc(A_PtrRcBase, m_PosRel, (__s16)m_PosRect.left, (__s16)m_PosRect.top, (__s16)m_PosRect.right, (__s16)m_PosRect.bottom, &Tv_RcScr))
		{
			// check my rect
			if ((A_X >= Tv_RcScr.left) && (A_X < Tv_RcScr.right) && (A_Y >= Tv_RcScr.top) && (A_Y < Tv_RcScr.bottom))
			{
				// update result
				Tv_Result	=	this;

				// check slave
				Tv_ObjSlave	=	m_ObjSlave;
				while (NULL != Tv_ObjSlave)
				{
					// find
					TV_ObjFind	=	Tv_ObjSlave->RtlFindObjByPos(A_X, A_Y, &Tv_RcScr);
					if (NULL != TV_ObjFind)
					{
						Tv_Result	=	TV_ObjFind;
						break;
					}
					// next
					Tv_ObjSlave	=	Tv_ObjSlave->NextObjGet();
				}
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBase::RtlListObjByPos(__s32 A_X, __s32 A_Y, Ptr_GrRect A_PtrRcBase, __u32* A_PtrCnt, __u32* A_PtrBuf)
{
	// local -------------------
		St_GrRect	Tv_RcScr;
		Cls_GrP4DoBase*	Tv_ObjSlave;
		__u32	Tv_Cnt;
	// code --------------------
		// init
		Tv_Cnt	=	*A_PtrCnt;
		// calculate screen rect
		if (BasisRectCalc(A_PtrRcBase, m_PosRel, (__s16)m_PosRect.left, (__s16)m_PosRect.top, (__s16)m_PosRect.right, (__s16)m_PosRect.bottom, &Tv_RcScr))
		{
			// check my rect
			if ((A_X >= Tv_RcScr.left) && (A_X < Tv_RcScr.right) && (A_Y >= Tv_RcScr.top) && (A_Y < Tv_RcScr.bottom))
			{
				// add this object
				A_PtrBuf[Tv_Cnt]	=	(__u32)this;		// add
				Tv_Cnt ++;
				*A_PtrCnt	=	Tv_Cnt;

				// check slave
				Tv_ObjSlave	=	m_ObjSlave;
				while (NULL != Tv_ObjSlave)
				{
					// find
					Tv_ObjSlave->RtlListObjByPos(A_X, A_Y, &Tv_RcScr, A_PtrCnt, A_PtrBuf);
					// next
					Tv_ObjSlave	=	Tv_ObjSlave->NextObjGet();
				}
			}
		}

}
//--------------------------------------------------------------------
__u8	Cls_GrP4DoBase::ClassIdGet( void )
{
		return	m_ClassId;
}
//--------------------------------------------------------------------
Cls_GrP4DoBase*	Cls_GrP4DoBase::FindSlaveObjByScptId( __u16 A_ScptId )
{
	// local -------------------
		Cls_GrP4DoBase*	Tv_Result;
		Cls_GrP4DoBase*	Tv_Slave;
	// code --------------------
		Tv_Result	=	NULL;
		Tv_Slave	=	m_ObjSlave;
		while ( NULL != Tv_Slave )
		{
			// check match id
			if ( A_ScptId == Tv_Slave->ScptIdGet() )
			{
				// find
				Tv_Result	=	Tv_Slave;
			}
			else
			{
				// find slave
				Tv_Result	=	Tv_Slave->FindSlaveObjByScptId( A_ScptId );
			}
			// check found
			if ( NULL != Tv_Result )
			{
				break;
			}
			// go next
			Tv_Slave	=	Tv_Slave->NextObjGet();
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
Cls_GrP4DoBase*	Cls_GrP4DoBase::FindPrntObjByScptId( __u16 A_ScptId )
{
	// local -------------------
		Cls_GrP4DoBase*	Tv_Result;
		Cls_GrP4DoBase*	Tv_Prnt;
	// code --------------------
		Tv_Result	=	NULL;
		Tv_Prnt		=	m_ObjPrnt;
		if ( NULL != Tv_Prnt )
		{
			// check match id
			if ( A_ScptId == Tv_Prnt->ScptIdGet() )
			{
				// find
				Tv_Result	=	Tv_Prnt;
			}
			else
			{
				// find parent
				Tv_Result	=	Tv_Prnt->FindPrntObjByScptId( A_ScptId );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBase::PosGet(Ptr_GrRect A_PtrRtRect, __u8* A_PtrRtPosRel)
{
		*A_PtrRtRect		=	m_PosRect;
		*A_PtrRtPosRel	=	m_PosRel;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBase::SizeGet(__s32* A_PtrRtSizeX, __s32* A_PtrRtSizeY)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrRect	Tv_RcScr;
	// code --------------------
		Tv_Result			=	FALSE;
		*A_PtrRtSizeX	=	0;
		*A_PtrRtSizeY	=	0;

		if (ScrRectGet(&Tv_RcScr))
		{
			*A_PtrRtSizeX	=	Tv_RcScr.right - Tv_RcScr.left;
			*A_PtrRtSizeY	=	Tv_RcScr.bottom - Tv_RcScr.top;
			Tv_Result			=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
__u32	Cls_GrP4DoBase::RtlParaInfoDump(void* A_PtrBuf)
{
	// local -------------------
	// code --------------------
		m_PtrDvlParaBuf	=	A_PtrBuf;
		m_DvlParaPrgs		=	0;
		RtlDvlParaInfoSubDump();
		return	m_DvlParaPrgs;
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoBase::LcDvlParaInfoSaveAdd( void* A_PtrPara, void* A_PtrInfo, __u32 A_Cnt )
{
	// local -------------------
		Ptr_GrP4DvlDoPrInfoData	Tv_PtrBuf;
		Ptr_GrP4DvlDoPrInfoData	Tv_PtrInfo;
		__u32	Tv_WkIdx;
	// code --------------------
		Tv_PtrBuf		=	(Ptr_GrP4DvlDoPrInfoData)( (__u32)m_PtrDvlParaBuf + m_DvlParaPrgs );
		Tv_PtrInfo	=	(Ptr_GrP4DvlDoPrInfoData)A_PtrInfo;
		for ( Tv_WkIdx=0;Tv_WkIdx < A_Cnt;Tv_WkIdx++ )
		{
			Tv_PtrBuf->StrName	=	Tv_PtrInfo->StrName;
			Tv_PtrBuf->IsNewLn	=	Tv_PtrInfo->IsNewLn;
			Tv_PtrBuf->Type			=	Tv_PtrInfo->Type;
			Tv_PtrBuf->Ptr			=	(void*)( (__u32)A_PtrPara + (__u32)Tv_PtrInfo->Ptr );
			Tv_PtrBuf->Ref			=	NULL;
			if ( NULL != Tv_PtrInfo->Ref )
			{
				Tv_PtrBuf->Ref	=	(void*)( (__u32)A_PtrPara + (__u32)Tv_PtrInfo->Ref );
			}
			Tv_PtrBuf->StrDesc	=	Tv_PtrInfo->StrDesc;
			// next
			Tv_PtrBuf ++;
			Tv_PtrInfo ++;
			m_DvlParaPrgs	=	m_DvlParaPrgs + sizeof(St_GrP4DvlDoPrInfoData);
		}
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoBase::RtlDvlParaInfoSubDump(void)
{
	// local -------------------
	// code --------------------
		LcDvlParaInfoSaveAdd( &m_ParaDoBase, V_GrP4DoBaseParaInfo, E_GrP4DoBaseParaCnt );
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
__u32	Cls_GrP4DoBase::RtlScptDump(void* A_PtrScpt, BOOL8 A_IsCopyMode)
{
	// local -------------------
		Ptr_GrP4ScptObjHd		Tv_PtrHd;
		Ptr_GrP4ScptObjEnd	Tv_PtrEd;
		Cls_GrP4DoBase*	Tv_Slave;
		__u32	Tv_ScptSize;
		void*	Tv_PtrScpt;
	// code --------------------
		//init
		m_ScptPrgs	=	0;
		m_PtrScpt		=	A_PtrScpt;

		// make header
		Tv_PtrHd	=	(Ptr_GrP4ScptObjHd)A_PtrScpt;
		Tv_PtrHd->Stx			=	E_GrP4ScptObjStx;
		Tv_PtrHd->ClassId	=	m_ClassId;
		Tv_PtrHd->ScptId	=	m_ScptId;
		if ( A_IsCopyMode )
		{
			// copy mode
			Tv_PtrHd->ScptId	=	E_GrP4ScptIdNone;
		}
		Tv_PtrHd->ParaSize			=	m_ParaSize;
		Tv_PtrHd->PosRel				=	m_PosRel;
		Tv_PtrHd->Rect.left		=	(__s16)m_PosRect.left;
		Tv_PtrHd->Rect.top			=	(__s16)m_PosRect.top;
		Tv_PtrHd->Rect.right		=	(__s16)m_PosRect.right;
		Tv_PtrHd->Rect.bottom	=	(__s16)m_PosRect.bottom;

		// update progass
		m_ScptPrgs	=	sizeof(St_GrP4ScptObjHd);

		// check page window
		if ( (E_GrP4ScptIdPageBase <= m_ScptId) && ((E_GrP4ScptIdPageBase + E_GrP4DvlPageMaxCnt) > m_ScptId) )
		{
			Tv_PtrHd->ParaSize	=	0;
		}
		else
		{
			// get parametters
			RtlScptParaSubDump();
		}

		// check this class type
		if ( E_GrP4DoClassFrame != m_ClassId )
		{
			// get slave object
			Tv_Slave	=	m_ObjSlave;
			while ( NULL != Tv_Slave )
			{
				// encode
				Tv_PtrScpt	=	(void*)( (__u32)A_PtrScpt + m_ScptPrgs );
				Tv_ScptSize	=	Tv_Slave->RtlScptDump( Tv_PtrScpt, A_IsCopyMode );
				m_ScptPrgs	=	m_ScptPrgs + Tv_ScptSize;	// update size
				// next
				Tv_Slave	=	Tv_Slave->NextObjGet();
			}
		}
		
		// mark tail
		Tv_PtrEd	=	(Ptr_GrP4ScptObjEnd)( (__u32)A_PtrScpt + m_ScptPrgs );
		Tv_PtrEd->Edx			=	E_GrP4ScptObjEdx;
		Tv_PtrEd->ScptId	=	m_ScptId;

		// update size
		m_ScptPrgs	=	m_ScptPrgs + sizeof(St_GrP4ScptObjEnd);
		
		return	m_ScptPrgs;
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoBase::ProcEvtSysStat( __u8 A_Type )
{
		if ( E_GrP4SysStatLangChg == A_Type )
		{
			ReqDraw();
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBase::RtlInit( void )
{
	// local -------------------
		Cls_GrP4DoBase*	Tv_Obj;
	// code --------------------
		// update last focus
		if ( IsCanKey() )
		{
			((Cls_GrP4DoWin*)m_ObjWin)->RtlLastFocusObjSet( this );
		}
		// do slave
		Tv_Obj	=	m_ObjSlave;
		while ( NULL != Tv_Obj )
		{
			// init
			Tv_Obj->RtlInit();
			// next
			Tv_Obj	=	Tv_Obj->NextObjGet();
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBase::RtlFindFocusableSlave( Ptr_GrP4DoFindFocusAble A_PtrEnv )
{
	// local -------------------
		Cls_GrP4DoBase*	Tv_ObjSlave;
		St_GrRect		Tv_Rc;
	// code --------------------
		Tv_ObjSlave	=	m_ObjSlave;
		while ( NULL != Tv_ObjSlave )
		{
			//check view able
			if ( Tv_ObjSlave->ScrRectGet( &Tv_Rc ) )
			{
				//check able
				if ( (A_PtrEnv->y > Tv_Rc.top) || ((A_PtrEnv->y == Tv_Rc.top) && (A_PtrEnv->x > Tv_Rc.left) ) )
				{
					if ( Tv_ObjSlave->IsCanKey() )
					{
						//update object
						A_PtrEnv->Obj	=	(void*)Tv_ObjSlave;
						A_PtrEnv->x		=	Tv_Rc.left;
						A_PtrEnv->y		=	Tv_Rc.top;
					}
					else
					{
						//slave check
						Tv_ObjSlave->RtlFindFocusableSlave( A_PtrEnv );
					}
				}
			}

			//next
			Tv_ObjSlave	=	Tv_ObjSlave->NextObjGet();
		}
}
//--------------------------------------------------------------------
Cls_GrP4DoBase*	Cls_GrP4DoBase::FindFocusableSlave( void )
{
	// local -------------------
		St_GrP4DoFindFocusAble	Tv_Env;
	// code --------------------
		Tv_Env.x		=	0x7FFFFFFF;
		Tv_Env.y		=	0x7FFFFFFF;
		Tv_Env.Obj	=	NULL;
		RtlFindFocusableSlave( &Tv_Env );
		return	(Cls_GrP4DoBase*)Tv_Env.Obj;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBase::ProcEvtFramePageChged( __u16 A_FrmadId, __u16 A_PageId )
{
		// not implement
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBase::FindSlaveWithCallback( void )
{
		RtlFindSlaveWithCallback( this );
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBase::RtlFindSlaveWithCallback( Cls_GrP4DoBase* A_Host )
{
	// local -------------------
		Cls_GrP4DoBase*	Tv_Obj;
	// code --------------------
		Tv_Obj	=	m_ObjSlave;
		while (NULL != Tv_Obj)
		{
			A_Host->RtlCallbackSlave( Tv_Obj );
			Tv_Obj->RtlFindSlaveWithCallback( A_Host );
			Tv_Obj	=	Tv_Obj->NextObjGet();
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBase::RtlCallbackSlave( Cls_GrP4DoBase* A_Obj )
{
		// not implement
}
//--------------------------------------------------------------------
__u32	Cls_GrP4DoBase::UserDataGet( __u8 A_Idx )
{
	// local -------------------
		__u32	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		if ( E_GrP4DoUserDataCnt > A_Idx )
		{
			Tv_Result	=	m_ParaDoBase.UserDataTbl[A_Idx];
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBase::ProcEvtFocusDirection( __u8 A_Direction, __u32 A_Atb )
{
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBase::ProcEvtVkbdEditEnd( void )
{

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBase::MoveFocusByDir( __u8 A_Direction, __u32 A_Atb )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_ScptId;
		Cls_GrP4DoBase*	Tv_Obj;
	// code --------------------
		Tv_Result	=	FALSE;
		Tv_ScptId	=	m_ParaDoBase.FcsMvTbl[A_Direction];
		if ( E_GrP4ScptIdNone != Tv_ScptId )
		{
			Tv_Obj	=	m_ObjWin->FindSlaveObjByScptId( Tv_ScptId );
			if ( NULL != Tv_Obj )
			{
				m_FindId	=	V_GrP4Mng->UiFindIdAlloc();
				Tv_Result	=	Tv_Obj->RtlMoveFcsByDir( A_Direction, A_Atb, m_FindId );
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBase::RtlMoveFcsByDir( __u8 A_Direction, __u32 A_Atb, __u32 A_FindId )
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrP4DoBase*	Tv_Obj;
		__u16	Tv_ScptId;
	// code --------------------
		Tv_Result	=	FALSE;

		// check overrappted host
		if ( m_FindId != A_FindId )
		{
			// update find id
			m_FindId	=	A_FindId;
			// do process
			if ( IsCanKey() )
			{
				Tv_Result	=	ProcEvtFocusDirection( A_Direction, A_Atb );
			}

			if ( Tv_Result )
			{
				((Cls_GrP4ConBase*)m_ObjCon)->FocusObjSet( this );
			}
			else
			{
				Tv_ScptId	=	m_ParaDoBase.FcsMvTbl[A_Direction];
				Tv_Obj	=	m_ObjWin->FindSlaveObjByScptId( Tv_ScptId );
				if ( NULL != Tv_Obj )
				{
					Tv_Result	=	Tv_Obj->RtlMoveFcsByDir( A_Direction, A_Atb, m_FindId );
				}
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBase::RelStdCtrlActive(BOOL8 A_IsActive)
{
	// local -------------------
		Cls_GrP4DoBase*	Tv_Obj;
	// code --------------------
		// get relate control
		if (E_GrP4ScptIdNone != m_ParaDoBase.RelStdId)
		{
			Tv_Obj	=	m_ObjWin->FindSlaveObjByScptId(m_ParaDoBase.RelStdId);
			if ( NULL != Tv_Obj )
			{
				switch ( Tv_Obj->ClassIdGet() )
				{
					case E_GrP4DoClassRelStc:
						((Cls_GrP4DoRelStc*)Tv_Obj)->ActiveSet( A_IsActive );
						break;
				}
				
			}
		}
}
//--------------------------------------------------------------------
__u16	Cls_GrP4DoBase::FocusTargetGet( __u8 A_Direction )
{
		return	m_ParaDoBase.FcsMvTbl[A_Direction];
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBase::FocusTargetSet( __u8 A_Direction, __u16 A_TargetId )
{
	// local -------------------
	// code --------------------
		m_ParaDoBase.FcsMvTbl[A_Direction]	=	A_TargetId;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBase::IsOnMouse( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		
		if ( this == ((Cls_GrP4ConBase*)m_ObjCon)->MouseOnObjGet() )
		{
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBase::RtlKeyDownProc(__u8 A_Dev, __u8 A_Key)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if (IsCanKey())
		{
			Tv_Result	=	ProcEvtKeyDown(A_Dev, A_Key);
		}

		if (!Tv_Result)
		{
			if (NULL != m_ObjPrnt)
			{
				Tv_Result	=	m_ObjPrnt->RtlKeyDownProc(A_Dev, A_Key);
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBase::RtlKeyUpProc(__u8 A_Dev, __u8 A_Key)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if (IsCanKey())
		{
			Tv_Result	=	ProcEvtKeyUp(A_Dev, A_Key);
		}
		
		if (!Tv_Result)
		{
			if (NULL != m_ObjPrnt)
			{
				Tv_Result	=	m_ObjPrnt->RtlKeyUpProc(A_Dev, A_Key);
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBase::RtlMseMoveProc(__u16 A_X, __u16 A_Y, __u8 A_BtnMap)
{
		return	ProcEvtMseMove(A_X, A_Y, A_BtnMap);
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBase::RtlMseLbtnDownProc(__u16 A_X, __u16 A_Y, __u8 A_BtnMap)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	ProcEvtMseLbtnDown(A_X,A_Y,A_BtnMap);
		if (!Tv_Result)
		{
			if (NULL != m_ObjPrnt)
			{
				Tv_Result	=	m_ObjPrnt->RtlMseLbtnDownProc(A_X, A_Y, A_BtnMap);
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBase::RtlMseLbtnUpProc(__u16 A_X, __u16 A_Y, __u8 A_BtnMap)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	ProcEvtMseLbtnUp(A_X, A_Y, A_BtnMap);
		if (!Tv_Result)
		{
			if (NULL != m_ObjPrnt)
			{
				Tv_Result	=	m_ObjPrnt->RtlMseLbtnUpProc(A_X, A_Y, A_BtnMap);
			}
		}
		return	Tv_Result;

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBase::RtlMseRbtnDownProc(__u16 A_X, __u16 A_Y, __u8 A_BtnMap)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	ProcEvtMseRbtnDown(A_X, A_Y, A_BtnMap);
		if (!Tv_Result)
		{
			if (NULL != m_ObjPrnt)
			{
				Tv_Result	=	m_ObjPrnt->RtlMseRbtnDownProc(A_X, A_Y, A_BtnMap);
			}
		}
		return	Tv_Result;

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBase::RtlMseRbtnUpProc(__u16 A_X, __u16 A_Y, __u8 A_BtnMap)
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		Tv_Result	=	ProcEvtMseRbtnUp(A_X, A_Y, A_BtnMap);
		if (!Tv_Result)
		{
			if (NULL != m_ObjPrnt)
			{
				Tv_Result	=	m_ObjPrnt->RtlMseRbtnUpProc(A_X, A_Y, A_BtnMap);
			}
		}
		return	Tv_Result;

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBase::RtlMseMbtnDownProc(__u16 A_X, __u16 A_Y, __u8 A_BtnMap)
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		Tv_Result	=	ProcEvtMseMbtnDown(A_X, A_Y, A_BtnMap);
		if (!Tv_Result)
		{
			if (NULL != m_ObjPrnt)
			{
				Tv_Result	=	m_ObjPrnt->RtlMseMbtnDownProc(A_X, A_Y, A_BtnMap);
			}
		}
		return	Tv_Result;

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBase::RtlMseMbtnUpProc(__u16 A_X, __u16 A_Y, __u8 A_BtnMap)
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		Tv_Result	=	ProcEvtMseMbtnUp(A_X, A_Y, A_BtnMap);
		if (!Tv_Result)
		{
			if (NULL != m_ObjPrnt)
			{
				Tv_Result	=	m_ObjPrnt->RtlMseMbtnUpProc(A_X, A_Y, A_BtnMap);
			}
		}
		return	Tv_Result;

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBase::RtlWheelUpProc(void)
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		Tv_Result	=	ProcEvtWheelUp();
		if (!Tv_Result)
		{
			if (NULL != m_ObjPrnt)
			{
				Tv_Result	=	m_ObjPrnt->RtlWheelUpProc();
			}
		}
		return	Tv_Result;

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBase::RtlWheelDownProc(void)
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		Tv_Result	=	ProcEvtWheelDown();
		if (!Tv_Result)
		{
			if (NULL != m_ObjPrnt)
			{
				Tv_Result	=	m_ObjPrnt->RtlWheelDownProc();
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
