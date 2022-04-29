/*
		platform script decorder

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P3/GrP3DoBase.h>
#include	<GrDebug.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<GrStrTool.h>
#include	<P3/GrP3Script.h>
#include	<P3/GrP3DoWin.h>
#include	<P3/GrP3Mng.h>
#include	<P3/GrP3UiCmd.h>
#include	<P3/GrP3DoFrame.h>
#include	<P3/GrP3DoRelStc.h>

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

#define E_GrP3DoBaseParaCnt		11

St_GrP3DvlDoPrInfoData	V_GrP3DoBaseParaInfo[E_GrP3DoBaseParaCnt]	=	
{
	{	L"FocusLeft",		TRUE,		E_GrP3DvlParaTypeScptIdAny,		1,	(void*)&((Ptr_GrP3DoBasePara)0)->FcsMvTbl[E_GrP3DoFocusMvIdxLeft]	, NULL, L"left focus target"	},
	{	L"FocusRight",	TRUE,		E_GrP3DvlParaTypeScptIdAny,		1,	(void*)&((Ptr_GrP3DoBasePara)0)->FcsMvTbl[E_GrP3DoFocusMvIdxRight]	, NULL,	L"right focus target"	},
	{	L"FocusUp",			TRUE,		E_GrP3DvlParaTypeScptIdAny,		1,	(void*)&((Ptr_GrP3DoBasePara)0)->FcsMvTbl[E_GrP3DoFocusMvIdxUp]	, NULL,	L"up focus target"	},
	{	L"FocusDown",		TRUE,		E_GrP3DvlParaTypeScptIdAny,		1,	(void*)&((Ptr_GrP3DoBasePara)0)->FcsMvTbl[E_GrP3DoFocusMvIdxDown]	, NULL,	L"down focus target"	},
	{	L"UserData0",		TRUE,		E_GrP3DvlParaTypeDword,				1,	(void*)&((Ptr_GrP3DoBasePara)0)->UserDataTbl[0]	, NULL,	L"User data 0"	},
	{	L"UserData1",		TRUE,		E_GrP3DvlParaTypeDword,				1,	(void*)&((Ptr_GrP3DoBasePara)0)->UserDataTbl[1]	, NULL,	L"User data 1"	},
	{	L"UserData2",		TRUE,		E_GrP3DvlParaTypeDword,				1,	(void*)&((Ptr_GrP3DoBasePara)0)->UserDataTbl[2]	, NULL,	L"User data 2"	},
	{	L"UserData3",		TRUE,		E_GrP3DvlParaTypeDword,				1,	(void*)&((Ptr_GrP3DoBasePara)0)->UserDataTbl[3]	, NULL,	L"User data 3"	},
	{	L"Enable",			TRUE,		E_GrP3DvlParaTypeBool8,				1,	(void*)&((Ptr_GrP3DoBasePara)0)->IsEnable	, NULL,	L"Enable input mounse and key"	},
	{	L"KeyBlock",		TRUE,		E_GrP3DvlParaTypeBool8,				1,	(void*)&((Ptr_GrP3DoBasePara)0)->IsKeyBlock, NULL, L"Key input block"	},
	{	L"RelateId",		TRUE,		E_GrP3DvlParaTypeScptIdAny,		1,	(void*)&((Ptr_GrP3DoBasePara)0)->RelateId,NULL,	L"Relate object id"	},

};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP3DoBase::Cls_GrP3DoBase( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId			=	E_GrP3DoClassBase;
		m_Rect				=	*A_PtrRect;
		m_ScptId			=	A_ScptId;
		m_ObjPrnt			=	A_ObjPrnt;
		m_ParaSize		=	(__u16)(sizeof(m_ParaDoBase)	+ sizeof(St_GrP3ScptParaHd));		//parameter size
		m_ObjFrm			=	NULL;

		m_FindId			=	0;

#ifdef GR_P3_EDITOR
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
		m_ParaDoBase.FcsMvTbl[E_GrP3DoFocusMvIdxLeft]		=	E_GrP3ScptIdNone;
		m_ParaDoBase.FcsMvTbl[E_GrP3DoFocusMvIdxRight]	=	E_GrP3ScptIdNone;
		m_ParaDoBase.FcsMvTbl[E_GrP3DoFocusMvIdxUp]			=	E_GrP3ScptIdNone;
		m_ParaDoBase.FcsMvTbl[E_GrP3DoFocusMvIdxDown]		=	E_GrP3ScptIdNone;

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
#ifdef GR_P3_EDITOR
		if ( E_GrP3ScptIdNone == A_ScptId )
		{
			// check window
			if ( NULL == A_ObjPrnt )
			{
				// error
				DbgMsgPrint( "Cls_GrP3DoBase::Cls_GrP3DoBase - None script id window!\n" );
			}
			else
			{
				// auto allocate
				m_ScptId	=	((Cls_GrP3DvlMng*)V_GrP3Mng)->DvlSlaveScptIdAlloc();
			}
		}
#endif
}
//--------------------------------------------------------------------
Cls_GrP3DoBase::~Cls_GrP3DoBase( void )
{
	// local -------------------
		Cls_GrP3DoBase*	Tv_ObjPrv;
		Cls_GrP3DoBase*	Tv_ObjFocus;
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
		if ( this == ((Cls_GrP3ConBase*)m_ObjCon)->MouseOnObjGet() )
		{
			//on mouse object reset
			((Cls_GrP3ConBase*)m_ObjCon)->MouseOnObjReset();
		}

		//release local focus
		if ( !IsWindow() )
		{
			//check match
			Tv_ObjFocus	=	((Cls_GrP3DoWin*)m_ObjWin)->LastFocusObjGet();
			if ( Tv_ObjFocus == this )
			{
				((Cls_GrP3DoWin*)m_ObjWin)->RtlLastFocusObjSet( NULL );			//last focus clear
			}
		}

		//release global focus
		Tv_ObjFocus	=	((Cls_GrP3ConBase*)m_ObjCon)->FocusKeyObjGet();
		if ( this == Tv_ObjFocus )
		{
			//focus release
			((Cls_GrP3ConBase*)m_ObjCon)->FocusReset();
			//request focus rebuild
			((Cls_GrP3ConBase*)m_ObjCon)->FocusCorrect();
		}

		// mouse capture release

		// release host object
		V_GrP3Mng->RtlProcObjRelease( this );

		//free obj id
#ifdef GR_P3_EDITOR
		if ( m_IsScptDel )
		{
			//delete script id
			((Cls_GrP3DvlMng*)V_GrP3Mng)->DvlSlaveScptIdFree( m_ScptId );
		}
#endif
}
//--------------------------------------------------------------------
__u16	Cls_GrP3DoBase::ScptIdGet( void )
{
		return	m_ScptId;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBase::RtlScptIdSet( __u16 A_Id )
{
		m_ScptId	=	A_Id;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBase::ParaCopyFromScpt( void* A_PtrPara, __u32 A_ParaSize, void* A_PtrScpt, __u8 A_ClassId )
{
	// local -------------------
		Ptr_GrP3ScptObjHd	Tv_PtrScpt;
		__u32	Tv_ParaPrgs;
		Ptr_GrP3ScptParaHd	Tv_PtrPrHd;
		void*	Tv_PtrPrCt;
		__u32	Tv_CtSize;
	// code --------------------

#ifdef GR_P3_EDITOR
		// check parametter no copy mode
		if ( m_IsDvlParaNoCopy )
		{
			return;
		}
#endif

		//find parametter start
		Tv_PtrScpt	=	(Ptr_GrP3ScptObjHd)A_PtrScpt;
#ifdef GR_P3_EDITOR
		if ( E_GrP3ScptObjStx != Tv_PtrScpt->Stx )
		{
			DbgMsgPrint( "Cls_GrP3DoBase::ParaCopyFromScpt - bad object script.\n" );
		}
#endif
		//find current parametter
		Tv_ParaPrgs	=	0;
		while ( Tv_ParaPrgs < Tv_PtrScpt->ParaSize )
		{
			Tv_PtrPrHd	=	(Ptr_GrP3ScptParaHd)( (__u32)Tv_PtrScpt + sizeof(St_GrP3ScptObjHd) + Tv_ParaPrgs );
#ifdef GR_P3_EDITOR
			// check parameter header
			if ( E_GrP3ScptParaStx != Tv_PtrPrHd->Stx )
			{
				DbgMsgPrint( "Cls_GrP3DoBase::ParaCopyFromScpt - script %d : bad parameter.\n" , Tv_PtrScpt->ScptId );
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
#ifdef GR_P3_EDITOR
				// check parameter header
				if ( A_ParaSize != (__u32)Tv_PtrPrHd->ParaSize )
				{
					DbgMsgPrint( "Cls_GrP3DoBase::ParaCopyFromScpt - script %d , class %d : para size missmatch.\n" , Tv_PtrScpt->ScptId, A_ClassId );
				}
#endif
				Tv_PtrPrCt	=	(void*)( (__u32)Tv_PtrPrHd + sizeof(St_GrP3ScptParaHd) );
				GrDumyCopyMem( A_PtrPara, Tv_PtrPrCt, Tv_CtSize );
				break;
			}
			//next
			Tv_ParaPrgs	=	Tv_ParaPrgs + sizeof(St_GrP3ScptParaHd) + Tv_PtrPrHd->ParaSize;
		}
#ifdef GR_P3_EDITOR
		if ( Tv_ParaPrgs >= Tv_PtrScpt->ParaSize )
		{
			if ( (E_GrP3ScptIdPageBase > Tv_PtrScpt->ScptId) || ((E_GrP3ScptIdPageBase + E_GrP3DvlPageMaxCnt) <= Tv_PtrScpt->ScptId) )
			{
				DbgMsgPrint( "Cls_GrP3DoBase::ParaCopyFromScpt - script %d : parameter not found.\n", Tv_PtrScpt->ScptId );
			}
		}
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBase::RtlScptParaSubUpdt( void )
{
		ParaCopyFromScpt( &m_ParaDoBase, sizeof(m_ParaDoBase) , m_PtrScpt , E_GrP3DoClassBase );
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBase::RtlScptParaUpdt( void* A_PtrScpt, BOOL8 A_IsNoCopy )
{
	// local -------------------
	// code --------------------
		m_PtrScpt			=	A_PtrScpt;
#ifdef GR_P3_EDITOR
		m_IsDvlParaNoCopy	=	A_IsNoCopy;
#endif
		RtlScptParaSubUpdt();
		// ReqDraw();
}
//--------------------------------------------------------------------
__u16	Cls_GrP3DoBase::RtlParaSizeGet( void )
{
		return	m_ParaSize;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBase::RtlScptParaSubDump( void )
{
#ifdef GR_P3_EDITOR
	// local -------------------
		Ptr_GrP3ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		//build head
		Tv_PtrHd			=	(Ptr_GrP3ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP3ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP3DoClassBase;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaDoBase);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP3ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaDoBase, sizeof(m_ParaDoBase) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaDoBase)	+ sizeof(St_GrP3ScptParaHd);
#endif
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoBase::IsVisible( void )
{
		return	m_IsVisible;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoBase::IsCanVisible( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrP3DoBase*	Tv_Obj;
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
BOOL8	Cls_GrP3DoBase::IsEnable( void )
{
		return	m_ParaDoBase.IsEnable;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoBase::IsCanEnable( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrP3DoBase*	Tv_Obj;
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
BOOL8	Cls_GrP3DoBase::IsKeyAble( void )
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
BOOL8	Cls_GrP3DoBase::IsKeyBlock( void )
{
		return	m_ParaDoBase.IsKeyBlock;
}
//--------------------------------------------------------------------
Cls_GrP3DoBase*	Cls_GrP3DoBase::WindowGet( void )
{
		return	m_ObjWin;
}
//--------------------------------------------------------------------
Cls_GrP3DoBase*	Cls_GrP3DoBase::FrameGet( void )
{
		return	m_ObjFrm;
}
//--------------------------------------------------------------------
void*	Cls_GrP3DoBase::ConsoleGet( void )
{
		return	m_ObjCon;
}
//--------------------------------------------------------------------
Cls_GrP3DoBase*	Cls_GrP3DoBase::ParentObjGet( void )
{
		return	m_ObjPrnt;
}
//--------------------------------------------------------------------
Cls_GrP3DoBase*	Cls_GrP3DoBase::NextObjGet( void )
{
		return	m_ObjNext;
}
//--------------------------------------------------------------------
Cls_GrP3DoBase*	Cls_GrP3DoBase::SlaveObjGet( void )
{
		return	m_ObjSlave;
}
//--------------------------------------------------------------------
Cls_GrP3DoBase*	Cls_GrP3DoBase::PrevObjGet( void )
{
	// local -------------------
		Cls_GrP3DoBase*	Tv_Result;
		Cls_GrP3DoBase*	Tv_ObjNext;
	// code --------------------
		//init
		Tv_Result		=	NULL;

		if ( NULL != m_ObjPrnt )
		{
			Tv_Result	=	m_ObjPrnt->SlaveObjGet();
			if ( Tv_Result == (Cls_GrP3DoBase*)this )
			{
				Tv_Result	=	NULL;
			}
			else
			{
				while ( NULL != Tv_Result )
				{
					//get next
					Tv_ObjNext	=	Tv_Result->NextObjGet();
					if ( Tv_ObjNext == (Cls_GrP3DoBase*)this )
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
void	Cls_GrP3DoBase::RectSet( Ptr_GrRect A_PtrRect )
{
	// local -------------------
		BOOL8	Tv_IsVisible;
	// code --------------------
		Tv_IsVisible	=	IsCanVisible();
		if ( Tv_IsVisible )
		{
			ReqDraw();		// predraw
		}
		m_Rect	=	*A_PtrRect;
		if ( Tv_IsVisible )
		{
			ReqDraw();		// postdraw
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoBase::IsWindow( void )
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
BOOL8	Cls_GrP3DoBase::IsFocused( void )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		//check focused
		if ( this == ((Cls_GrP3ConBase*)m_ObjCon)->FocusObjGet() )
		{
			Tv_Result		=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBase::LcFreeFocus( void )
{
	// local -------------------
	// code --------------------
		if ( !IsWindow() )
		{
			//check window focused
			if ( this == ((Cls_GrP3DoWin*)m_ObjWin)->LastFocusObjGet() )
			{
				//release window last focus
				((Cls_GrP3DoWin*)m_ObjWin)->RtlLastFocusObjSet( NULL );
			}
			if ( IsFocused() )
			{
				((Cls_GrP3ConBase*)m_ObjCon)->FocusObjSet( NULL );
				((Cls_GrP3ConBase*)m_ObjCon)->FocusCorrect();
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBase::VisibleSet( BOOL8 A_IsVisible )
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
void	Cls_GrP3DoBase::EnableSet( BOOL8 A_IsEnable )
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
BOOL8	Cls_GrP3DoBase::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrRect	Tv_Rc;

	// code --------------------
		Tv_Result	=	TRUE;

		switch ( A_Cmd )
		{
			case	E_GrP3UpiClassIdGet:
				A_PtrInt[0]	=	(__s32)((__u32)m_ClassId);
				break;
			case E_GrP3UpiScptIdGet:
				A_PtrInt[0]	=	(__s32)((__u32)m_ScptId);
				break;
			case E_GrP3UpiPosGet:
				A_PtrInt[1]	=	(__s32)m_Rect.left;
				A_PtrInt[2]	=	(__s32)m_Rect.top;
				//redraw
				break;
			case E_GrP3UpiPosSet:
				Tv_Rc.left		=	A_PtrInt[1];
				Tv_Rc.top			=	A_PtrInt[2];
				Tv_Rc.right		=	Tv_Rc.left + m_Rect.right - m_Rect.left;
				Tv_Rc.bottom	=	Tv_Rc.top + m_Rect.bottom - m_Rect.top;
				RectSet( &Tv_Rc );
				break;
			case E_GrP3UpiSizeGet:
				A_PtrInt[1]	=	(__s32)(m_Rect.right - m_Rect.left);
				A_PtrInt[2]	=	(__s32)(m_Rect.bottom - m_Rect.top);
				break;
			case E_GrP3UpiSizeSet:
				Tv_Rc.left		=	m_Rect.left;
				Tv_Rc.top			=	m_Rect.top;
				Tv_Rc.right		=	Tv_Rc.left + A_PtrInt[1];
				Tv_Rc.bottom	=	Tv_Rc.top + A_PtrInt[2];
				RectSet( &Tv_Rc );
				break;
			case E_GrP3UpiVisibleGet:
				A_PtrInt[0]	=	(__s32)m_IsVisible;
				break;
			case E_GrP3UpiVisibleSet:
				VisibleSet( (BOOL8)A_PtrInt[1] );
				break;
			case E_GrP3UpiEnableGet:
				A_PtrInt[0]	=	(__s32)IsEnable();
				break;
			case E_GrP3UpiEnableSet:
				EnableSet( (BOOL8)A_PtrInt[1] );
				break;
			case E_GrP3UpiFcsMvTblGet:
				A_PtrInt[1]	=	(__s32)m_ParaDoBase.FcsMvTbl[E_GrP3DoFocusMvIdxLeft];
				A_PtrInt[2]	=	(__s32)m_ParaDoBase.FcsMvTbl[E_GrP3DoFocusMvIdxRight];
				A_PtrInt[3]	=	(__s32)m_ParaDoBase.FcsMvTbl[E_GrP3DoFocusMvIdxUp];
				A_PtrInt[4]	=	(__s32)m_ParaDoBase.FcsMvTbl[E_GrP3DoFocusMvIdxDown];
				break;
			case E_GrP3UpiFcsMvTblSet:
				m_ParaDoBase.FcsMvTbl[E_GrP3DoFocusMvIdxLeft]		=	A_PtrInt[1];
				m_ParaDoBase.FcsMvTbl[E_GrP3DoFocusMvIdxRight]	=	A_PtrInt[2];
				m_ParaDoBase.FcsMvTbl[E_GrP3DoFocusMvIdxUp]			=	A_PtrInt[3];
				m_ParaDoBase.FcsMvTbl[E_GrP3DoFocusMvIdxDown]		=	A_PtrInt[4];
				break;
			case E_GrP3UpiUserDataGet:
				A_PtrInt[0]	=	0;
				if ( E_GrP3DoUserDataCnt > (__u32)A_PtrInt[1] )
				{
					A_PtrInt[0]	=	(__s32)m_ParaDoBase.UserDataTbl[ A_PtrInt[1] ];
				}
				break;
			case E_GrP3UpiUserDataSet:
				if ( E_GrP3DoUserDataCnt > (__u32)A_PtrInt[1] )
				{
					m_ParaDoBase.UserDataTbl[A_PtrInt[1]]	=	A_PtrInt[2];
				}
				break;
			case E_GrP3UpiReqDraw:
				ReqDraw( A_PtrInt[1] );
				break;
			case E_GrP3UpiFocusIdSet:
				((Cls_GrP3ConBase*)m_ObjCon)->FocusObjSet( this );
				break;
			default:
				Tv_Result	=	FALSE;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBase::RtlNextObjSet( Cls_GrP3DoBase* A_Obj )
{
		m_ObjNext	=	A_Obj;		
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBase::RtlSlaveObjSet( Cls_GrP3DoBase* A_Obj )
{
		m_ObjSlave	=	A_Obj;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBase::RtlScptDelMode( void )
{
#ifdef GR_P3_EDITOR		
	// local -------------------
		Cls_GrP3DoBase*	Tv_Slave;
	// code --------------------
		m_IsScptDel	=	TRUE;
		// check frame
		if ( E_GrP3DoClassFrame != m_ClassId )
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
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBase::ObjSlaveFreeAll( void )
{
		while ( NULL != m_ObjSlave )
		{
#ifdef GR_P3_EDITOR
			if ( (NULL == m_ObjFrm) && m_IsScptDel )
			{
				m_ObjSlave->RtlScptDelMode();		// update script delete mode
			}
#endif
			delete	m_ObjSlave;
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoBase::RtlScrRectGet( Ptr_GrRect A_PtrRect, BOOL8 A_IsNoChk )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result		=	FALSE;
		//check visiable
		if ( m_IsVisible || A_IsNoChk )
		{
			A_PtrRect->left		=	A_PtrRect->left + m_Rect.left;
			A_PtrRect->top		=	A_PtrRect->top + m_Rect.top;
			A_PtrRect->right	=	A_PtrRect->right + m_Rect.left;
			A_PtrRect->bottom	=	A_PtrRect->bottom + m_Rect.top;
			Tv_Result		=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoBase::ScrRectGet( Ptr_GrRect A_PtrRtRect, BOOL8 A_IsNoChk )
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrP3DoBase*	Tv_Obj;
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		//check visible
		if ( m_IsVisible || A_IsNoChk )
		{
			//init rect
			Tv_Result			=	TRUE;
			*A_PtrRtRect	=	m_Rect;
			//parent get
			Tv_Obj		=	m_ObjPrnt;
			while ( ( NULL != Tv_Obj ) && Tv_Result )
			{
				// get patch rect
				Tv_Result	=	Tv_Obj->RtlScrRectGet( A_PtrRtRect, A_IsNoChk );
				// next
				Tv_Obj	=	Tv_Obj->ParentObjGet();
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBase::ReqDraw( BOOL8 A_IsNoChk )
{
	// local -------------------
		St_GrRect	Tv_RcScr;
	// code --------------------
		if ( ScrRectGet( &Tv_RcScr, A_IsNoChk ) )
		{
			((Cls_GrP3ConBase*)m_ObjCon)->PaintReq( &Tv_RcScr );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBase::ReqPartDraw( Ptr_GrRect A_PtrRc, BOOL8 A_IsNoChk )
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
				((Cls_GrP3ConBase*)m_ObjCon)->PaintReq( &Tv_RcTg );
			}
			
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBase::RtlPaint( Ptr_GrRect A_PtrRect )
{
	// local -------------------
		BOOL8	Tv_IsCan;
		Cls_GrP3DoBase*	Tv_Obj;
	// code --------------------
		// check can paint
		if ( ((Cls_GrP3ConBase*)m_ObjCon)->IsPstCanPush() )
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
							Tv_IsCan	=	((Cls_GrP3ConBase*)m_ObjCon)->IsPstCanPush();
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
void	Cls_GrP3DoBase::ProcPaint( void )
{
	// paint script update
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBase::ProcUiMsg( Ptr_GrP3MsgHd A_PtrMsg )
{
	// local -------------------
		BOOL8	Tv_IsRun;
	// code --------------------
		switch ( A_PtrMsg->Cmd )
		{
			case E_GrP3MsgCmdKey:
				if ( ((Ptr_GrP3MsgKey)A_PtrMsg)->IsPush )
				{
					Tv_IsRun	=	ProcEvtKeyDown( ((Ptr_GrP3MsgKey)A_PtrMsg)->Dev, ((Ptr_GrP3MsgKey)A_PtrMsg)->Key );
					if ( !Tv_IsRun && (!IsWindow()) )
					{
						((Cls_GrP3DoWin*)m_ObjWin)->ProcEvtKeyDown( ((Ptr_GrP3MsgKey)A_PtrMsg)->Dev, ((Ptr_GrP3MsgKey)A_PtrMsg)->Key );
					}
				}
				else
				{
					Tv_IsRun	=	ProcEvtKeyUp( ((Ptr_GrP3MsgKey)A_PtrMsg)->Dev, ((Ptr_GrP3MsgKey)A_PtrMsg)->Key );
					if ( !Tv_IsRun )
					{
						if ( (E_GrP3DoClassFrame != m_ClassId) && (NULL != m_ObjFrm) )
						{
							Tv_IsRun	=	((Cls_GrP3DoFrame*)m_ObjFrm)->ProcEvtKeyUp( ((Ptr_GrP3MsgKey)A_PtrMsg)->Dev, ((Ptr_GrP3MsgKey)A_PtrMsg)->Key );
						}
						if ( !Tv_IsRun && (!IsWindow()) )
						{
							((Cls_GrP3DoWin*)m_ObjWin)->ProcEvtKeyUp( ((Ptr_GrP3MsgKey)A_PtrMsg)->Dev, ((Ptr_GrP3MsgKey)A_PtrMsg)->Key );
						}
					}
				}

				break;
			case E_GrP3MsgCmdMouse:
				Tv_IsRun		=	FALSE;
				if ( 0 != (E_GrP3MouseAtbPosChg & ((Ptr_GrP3MsgMouse)A_PtrMsg)->Atb) )
				{
					//mouse move
					Tv_IsRun	=	ProcEvtMseMove( ((Ptr_GrP3MsgMouse)A_PtrMsg)->X, ((Ptr_GrP3MsgMouse)A_PtrMsg)->Y, ((Ptr_GrP3MsgMouse)A_PtrMsg)->BtnMap );
				}
				else if ( 0 != (E_GrP3MouseAtbLbtnChg & ((Ptr_GrP3MsgMouse)A_PtrMsg)->Atb) )
				{
					if ( 0 != ( E_GrP3MouseBmkkLeft & ((Ptr_GrP3MsgMouse)A_PtrMsg)->BtnMap )  )
					{
						Tv_IsRun	=	ProcEvtMseLbtnDown( ((Ptr_GrP3MsgMouse)A_PtrMsg)->X, ((Ptr_GrP3MsgMouse)A_PtrMsg)->Y, ((Ptr_GrP3MsgMouse)A_PtrMsg)->BtnMap );
					}
					else
					{
						Tv_IsRun	=	ProcEvtMseLbtnUp( ((Ptr_GrP3MsgMouse)A_PtrMsg)->X, ((Ptr_GrP3MsgMouse)A_PtrMsg)->Y, ((Ptr_GrP3MsgMouse)A_PtrMsg)->BtnMap );
					}
				}
				else if ( 0 != (E_GrP3MouseAtbRbtnChg & ((Ptr_GrP3MsgMouse)A_PtrMsg)->Atb) )
				{
					if ( 0 != ( E_GrP3MouseBmkRight & ((Ptr_GrP3MsgMouse)A_PtrMsg)->BtnMap )  )
					{
						Tv_IsRun	=	ProcEvtMseRbtnDown( ((Ptr_GrP3MsgMouse)A_PtrMsg)->X, ((Ptr_GrP3MsgMouse)A_PtrMsg)->Y, ((Ptr_GrP3MsgMouse)A_PtrMsg)->BtnMap );
					}
					else
					{
						Tv_IsRun	=	ProcEvtMseRbtnUp( ((Ptr_GrP3MsgMouse)A_PtrMsg)->X, ((Ptr_GrP3MsgMouse)A_PtrMsg)->Y, ((Ptr_GrP3MsgMouse)A_PtrMsg)->BtnMap );
						if ( !Tv_IsRun && (!IsWindow()) )
						{
							if ( (E_GrP3DoClassFrame != m_ClassId) && (NULL != m_ObjFrm) )
							{
								Tv_IsRun	=	((Cls_GrP3DoFrame*)m_ObjFrm)->ProcEvtMseRbtnUp( ((Ptr_GrP3MsgMouse)A_PtrMsg)->X, ((Ptr_GrP3MsgMouse)A_PtrMsg)->Y, ((Ptr_GrP3MsgMouse)A_PtrMsg)->BtnMap );
							}
							if ( !Tv_IsRun && (!IsWindow()) )
							{
								((Cls_GrP3DoWin*)m_ObjWin)->ProcEvtMseRbtnUp( ((Ptr_GrP3MsgMouse)A_PtrMsg)->X, ((Ptr_GrP3MsgMouse)A_PtrMsg)->Y, ((Ptr_GrP3MsgMouse)A_PtrMsg)->BtnMap );
							}
						}
					}
				}
				else if ( 0 != (E_GrP3MouseAtbMbtnChg & ((Ptr_GrP3MsgMouse)A_PtrMsg)->Atb) )
				{
					if ( 0 != ( E_GrP3MouseBmkMiddle & ((Ptr_GrP3MsgMouse)A_PtrMsg)->BtnMap )  )
					{
						Tv_IsRun	=	ProcEvtMseMbtnDown( ((Ptr_GrP3MsgMouse)A_PtrMsg)->X, ((Ptr_GrP3MsgMouse)A_PtrMsg)->Y, ((Ptr_GrP3MsgMouse)A_PtrMsg)->BtnMap );
					}
					else
					{
						Tv_IsRun	=	ProcEvtMseMbtnUp( ((Ptr_GrP3MsgMouse)A_PtrMsg)->X, ((Ptr_GrP3MsgMouse)A_PtrMsg)->Y, ((Ptr_GrP3MsgMouse)A_PtrMsg)->BtnMap );
					}
				}
				break;
			case E_GrP3MsgCmdWheel:
				if ( ((Ptr_GrP3MsgWheel)A_PtrMsg)->IsDown )
				{
					Tv_IsRun	=	ProcEvtWheelDown();
				}
				else
				{
					Tv_IsRun	=	ProcEvtWheelUp();
				}
				break;
			case E_GrP3MsgCmdFocusGet:
				ProcEvtFocusGet();
				break;
			case E_GrP3MsgCmdFocusLoss:
				ProcEvtFocusLoss();
				break;
			case E_GrP3MsgCmdMouseEnter:
				ProcEvtMseEnter();
				break;
			case	E_GrP3MsgCmdMouseLeave:
				ProcEvtMseLeave();
				break;
			case E_GrP3MsgCmdTickPeriod:
				ProcEvtTimeTick( ((Ptr_GrP3MsgTimeTick)A_PtrMsg)->Tick );
				break;
			case E_GrP3MsgCmdSysStat:
				ProcEvtSysStat( ((Ptr_GrP3MsgSysStat)A_PtrMsg)->Type );
				break;
			case E_GrP3MsgCmdFramePageChged:
				ProcEvtFramePageChged( ((Ptr_GrP3MsgFrmPageChged)A_PtrMsg)->FramdId, ((Ptr_GrP3MsgFrmPageChged)A_PtrMsg)->PageId );
				break;
			case E_GrP3MsgCmdVkbdEnd:
				ProcEvtVkbdEditEnd();
				break;
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoBase::RtlUiMsgProc( Ptr_GrP3MsgHd A_PtrMsg )
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrP3DoBase*	Tv_Obj;
	// code --------------------
		Tv_Result	=	FALSE;
		// check this window
		if ( (m_ScptId == A_PtrMsg->ObjId) || (E_GrP3ScptIdAll == A_PtrMsg->ObjId) )
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
BOOL8	Cls_GrP3DoBase::ProcEvtKeyDown( __u8 A_Dev, __u8 A_Key )
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoBase::ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key )
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoBase::ProcEvtMseMove( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoBase::ProcEvtMseLbtnDown( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( m_IsKeyAble )
		{
			((Cls_GrP3ConBase*)m_ObjCon)->FocusObjSet( this );
			Tv_Result	=	TRUE;
		}
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoBase::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoBase::ProcEvtMseRbtnDown( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoBase::ProcEvtMseRbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoBase::ProcEvtMseMbtnDown( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoBase::ProcEvtMseMbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoBase::ProcEvtWheelUp( void )
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoBase::ProcEvtWheelDown( void )
{
		return	FALSE;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBase::ProcEvtFocusGet( void )
{
		((Cls_GrP3DoWin*)m_ObjWin)->RtlLastFocusObjSet( this );
		if ( NULL != m_ObjFrm )
		{
			((Cls_GrP3DoFrame*)m_ObjFrm)->RtlLastFocusObjIdSet( m_ScptId );
		}
		RelateCtrlActive( TRUE );
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBase::ProcEvtFocusLoss( void )
{
		RelateCtrlActive( FALSE );
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBase::ProcEvtMseEnter( void )
{
	if ( m_IsMouseAble )
	{
			m_IsOnMouse	=	TRUE;
			RelateCtrlActive( TRUE );
			ReqDraw();
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBase::ProcEvtMseLeave( void )
{
		if ( m_IsMouseAble )
		{
			m_IsOnMouse	=	FALSE;
			RelateCtrlActive( FALSE );
			ReqDraw();
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBase::ProcEvtTimeTick( __u32 A_Tick )
{

}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP3DoBase::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
{
		*A_PtrRtCus	=	-1;
		return	NULL;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoBase::BasisRectCalc( Ptr_GrRect A_PtrRcBase, __u8 A_Basis, __s16 A_Left, __s16 A_Top, __s16 A_Right, __s16 A_Bottom, Ptr_GrRect A_PtrRtRc )
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
			case E_GrP3ScptPstBasisSt:
				A_PtrRtRc->left	=	A_PtrRcBase->left + (__s32)A_Left;
				break;
			case E_GrP3ScptPstBasisEd:
				A_PtrRtRc->left	=	A_PtrRcBase->right - (__s32)A_Left;
				break;
			case E_GrP3ScptPstBasisCtSt:
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
			case E_GrP3ScptPstBasisSt:
				A_PtrRtRc->top		=	A_PtrRcBase->top + (__s32)A_Top;
				break;
			case E_GrP3ScptPstBasisEd:
				A_PtrRtRc->top		=	A_PtrRcBase->bottom - (__s32)A_Top;
				break;
			case E_GrP3ScptPstBasisCtSt:
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
			case E_GrP3ScptPstBasisSt:
				A_PtrRtRc->right	=	A_PtrRcBase->left + (__s32)A_Right;
				break;
			case E_GrP3ScptPstBasisEd:
				A_PtrRtRc->right	=	A_PtrRcBase->right - (__s32)A_Right;
				break;
			case E_GrP3ScptPstBasisCtSt:
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
			case E_GrP3ScptPstBasisSt:
				A_PtrRtRc->bottom	=	A_PtrRcBase->top + (__s32)A_Bottom;
				break;
			case E_GrP3ScptPstBasisEd:
				A_PtrRtRc->bottom	=	A_PtrRcBase->bottom - (__s32)A_Bottom;
				break;
			case E_GrP3ScptPstBasisCtSt:
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
void	Cls_GrP3DoBase::PaintScriptPush( __u16 A_PstId, Ptr_GrRect A_PtrRc )
{
	// local -------------------
		Ptr_GrP3ScptPstData	Tv_PtrPst;
		__u8	Tv_PstCnt;
		__u8	Tv_WkIdx;
		__u8	Tv_Basis;
		Ptr_GrP3ConPntStackItm	Tv_PtrStk; 
		__s32		Tv_CtHz;
		__s32		Tv_CtVt;
		St_GrRect	Tv_RcUpdt;
		__u16	Tv_TxtId;
	// code --------------------
		// init
		Tv_CtHz		=	((A_PtrRc->right - A_PtrRc->left) >> 1) + A_PtrRc->left;
		Tv_CtVt		=	((A_PtrRc->bottom - A_PtrRc->top) >> 1) + A_PtrRc->top;
		// get script
		Tv_PtrPst	=	V_GrP3Mng->PstDataGet( A_PstId, &Tv_PstCnt );
		if ( 0 != Tv_PstCnt )
		{
			// decode script
			for (Tv_WkIdx=0;Tv_WkIdx < Tv_PstCnt;Tv_WkIdx++)
			{
				// get stack pointer
				Tv_PtrStk	=	((Cls_GrP3ConBase*)m_ObjCon)->PaintStackGetPushPtr();
				if ( NULL == Tv_PtrStk )
				{
					// quit loop
					break;
				}
				// check type
				if ( (E_GrP3ScptPstTypeFont == Tv_PtrPst->Type) || (E_GrP3ScptPstTypeStyle == Tv_PtrPst->Type) )
				{
					Tv_RcUpdt	=	*A_PtrRc;
				}
				else
				{
					// left
					Tv_Basis	=	Tv_PtrPst->Basis & 0x03;
					switch ( Tv_Basis )
					{
						case E_GrP3ScptPstBasisSt:
							Tv_PtrStk->Rect.left	=	A_PtrRc->left + (__s32)Tv_PtrPst->Left;
							break;
						case E_GrP3ScptPstBasisEd:
							Tv_PtrStk->Rect.left	=	A_PtrRc->right - (__s32)Tv_PtrPst->Left;
							break;
						case E_GrP3ScptPstBasisCtSt:
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
						case E_GrP3ScptPstBasisSt:
							Tv_PtrStk->Rect.top		=	A_PtrRc->top + (__s32)Tv_PtrPst->Top;
							break;
						case E_GrP3ScptPstBasisEd:
							Tv_PtrStk->Rect.top		=	A_PtrRc->bottom - (__s32)Tv_PtrPst->Top;
							break;
						case E_GrP3ScptPstBasisCtSt:
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
						case E_GrP3ScptPstBasisSt:
							Tv_PtrStk->Rect.right	=	A_PtrRc->left + (__s32)Tv_PtrPst->Right;
							break;
						case E_GrP3ScptPstBasisEd:
							Tv_PtrStk->Rect.right	=	A_PtrRc->right - (__s32)Tv_PtrPst->Right;
							break;
						case E_GrP3ScptPstBasisCtSt:
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
						case E_GrP3ScptPstBasisSt:
							Tv_PtrStk->Rect.bottom	=	A_PtrRc->top + (__s32)Tv_PtrPst->Bottom;
							break;
						case E_GrP3ScptPstBasisEd:
							Tv_PtrStk->Rect.bottom	=	A_PtrRc->bottom - (__s32)Tv_PtrPst->Bottom;
							break;
						case E_GrP3ScptPstBasisCtSt:
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
						case E_GrP3ScptPstTypeText:
							// get text id
							Tv_TxtId	=	(__u16)Tv_PtrPst->Para0 | (((__u16)Tv_PtrPst->Para1) << 8);
							// check local text
							if ( E_GrP3ScptTxtIdCtrl <= Tv_TxtId )
							{
								Tv_PtrStk->PtrOpt	=	(void*)RtlInTextGet( (__u8)(Tv_TxtId - E_GrP3ScptTxtIdCtrl), &Tv_PtrStk->IntOpt );
							}
							else
							{
								Tv_PtrStk->PtrOpt	=	(void*)V_GrP3Mng->TextPtrGet( Tv_TxtId );
								Tv_PtrStk->IntOpt	=	-1;		// no cursor
							}
							if ( NULL != Tv_PtrStk->PtrOpt )
							{
								// stack push
								((Cls_GrP3ConBase*)m_ObjCon)->PaintStackPush();
							}
							break;
						default:
							// stack push
							((Cls_GrP3ConBase*)m_ObjCon)->PaintStackPush();
							break;
					}

				}
				// next
				Tv_PtrPst ++;
			}
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBase::PaintFillClrPush( __u32 A_Clr, Ptr_GrRect A_PtrRc )
{
	// local -------------------
		Ptr_GrP3ConPntStackItm	Tv_PtrStk; 
	// code --------------------
		// get stack pointer
		Tv_PtrStk	=	((Cls_GrP3ConBase*)m_ObjCon)->PaintStackGetPushPtr();
		if ( NULL != Tv_PtrStk )
		{
			Tv_PtrStk->Rect		=	*A_PtrRc;
			Tv_PtrStk->Type		=	E_GrP3ScptPstTypeClrFill;
			Tv_PtrStk->Value	=	A_Clr;

			// stack push
			((Cls_GrP3ConBase*)m_ObjCon)->PaintStackPush();
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoBase::IsCanMouse( void )
{
	// local -------------------
		BOOL8		Tv_Result;
		Cls_GrP3DoWin*	Tv_Win;
		Cls_GrP3DoBase*	Tv_Obj;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Win		=	(Cls_GrP3DoWin*)m_ObjWin;;
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
BOOL8	Cls_GrP3DoBase::IsCanKey( void )
{
	// local -------------------
		BOOL8		Tv_Result;
		Cls_GrP3DoWin*	Tv_Win;
		Cls_GrP3DoBase*	Tv_Obj;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( m_IsKeyAble && (!m_ParaDoBase.IsKeyBlock) && m_ParaDoBase.IsEnable && m_IsVisible )
		{
			Tv_Win		=	(Cls_GrP3DoWin*)m_ObjWin;;
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
Cls_GrP3DoBase*	Cls_GrP3DoBase::RtlFindMseObjByPos( __s32 A_X, __s32 A_Y, __s32 A_BaseX, __s32 A_BaseY )
{
	// local -------------------
		Cls_GrP3DoBase*	Tv_Result;
		St_GrRect	Tv_RcScr;
		Cls_GrP3DoBase*	Tv_ObjSlave;
		Cls_GrP3DoBase*	TV_ObjFind;
	// code --------------------
		Tv_Result	=	NULL;
		// check attribute 
		if ( m_IsVisible && m_ParaDoBase.IsEnable  )
		{
			// calculate screen rect
			Tv_RcScr.left		=	A_BaseX + m_Rect.left;
			Tv_RcScr.top		=	A_BaseY + m_Rect.top;
			Tv_RcScr.right	=	A_BaseX + m_Rect.right;
			Tv_RcScr.bottom	=	A_BaseY + m_Rect.bottom;
			// check my rect
			if ( (A_X >= Tv_RcScr.left) && (A_X < Tv_RcScr.right) && (A_Y >= Tv_RcScr.top) && (A_Y < Tv_RcScr.bottom) )
			{
				// update result
				if ( m_IsMouseAble )
				{
					// update object
					Tv_Result	=	this;
				}
				// check slave
				Tv_ObjSlave	=	m_ObjSlave;
				while ( NULL != Tv_ObjSlave )
				{
					// find
					TV_ObjFind	=	Tv_ObjSlave->RtlFindMseObjByPos( A_X, A_Y, Tv_RcScr.left, Tv_RcScr.top );
					if ( NULL != TV_ObjFind )
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
Cls_GrP3DoBase*	Cls_GrP3DoBase::RtlFindObjByPos( __s32 A_X, __s32 A_Y, __s32 A_BaseX, __s32 A_BaseY )
{
	// local -------------------
		Cls_GrP3DoBase*	Tv_Result;
		St_GrRect	Tv_RcScr;
		Cls_GrP3DoBase*	Tv_ObjSlave;
		Cls_GrP3DoBase*	TV_ObjFind;
	// code --------------------
		Tv_Result	=	NULL;
		// calculate screen rect
		Tv_RcScr.left		=	A_BaseX + m_Rect.left;
		Tv_RcScr.top		=	A_BaseY + m_Rect.top;
		Tv_RcScr.right	=	A_BaseX + m_Rect.right;
		Tv_RcScr.bottom	=	A_BaseY + m_Rect.bottom;
		// check my rect
		if ( (A_X >= Tv_RcScr.left) && (A_X < Tv_RcScr.right) && (A_Y >= Tv_RcScr.top) && (A_Y < Tv_RcScr.bottom) )
		{
			// update result
			Tv_Result	=	this;
			// check slave
			Tv_ObjSlave	=	m_ObjSlave;
			while ( NULL != Tv_ObjSlave )
			{
				// find
				TV_ObjFind	=	Tv_ObjSlave->RtlFindObjByPos( A_X, A_Y, Tv_RcScr.left, Tv_RcScr.top );
				if ( NULL != TV_ObjFind )
				{
					Tv_Result	=	TV_ObjFind;
					break;
				}
				// next
				Tv_ObjSlave	=	Tv_ObjSlave->NextObjGet();
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBase::RtlListObjByPos( __s32 A_X, __s32 A_Y, __s32 A_BaseX, __s32 A_BaseY, __u32* A_PtrCnt, __u32* A_PtrBuf )
{
	// local -------------------
		St_GrRect	Tv_RcScr;
		Cls_GrP3DoBase*	Tv_ObjSlave;
		__u32	Tv_Cnt;
	// code --------------------
		// init
		Tv_Cnt	=	*A_PtrCnt;
		// calculate screen rect
		Tv_RcScr.left		=	A_BaseX + m_Rect.left;
		Tv_RcScr.top		=	A_BaseY + m_Rect.top;
		Tv_RcScr.right	=	A_BaseX + m_Rect.right;
		Tv_RcScr.bottom	=	A_BaseY + m_Rect.bottom;
		// check my rect
		if ( (A_X >= Tv_RcScr.left) && (A_X < Tv_RcScr.right) && (A_Y >= Tv_RcScr.top) && (A_Y < Tv_RcScr.bottom) )
		{
			// add this object
			A_PtrBuf[Tv_Cnt]	=	(__u32)this;		// add
			Tv_Cnt ++;
			*A_PtrCnt	=	Tv_Cnt;

			// check slave
			Tv_ObjSlave	=	m_ObjSlave;
			while ( NULL != Tv_ObjSlave )
			{
				// find
				Tv_ObjSlave->RtlListObjByPos( A_X, A_Y, Tv_RcScr.left, Tv_RcScr.top, A_PtrCnt, A_PtrBuf );
				// next
				Tv_ObjSlave	=	Tv_ObjSlave->NextObjGet();
			}
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrP3DoBase::ClassIdGet( void )
{
		return	m_ClassId;
}
//--------------------------------------------------------------------
Cls_GrP3DoBase*	Cls_GrP3DoBase::FindSlaveObjByScptId( __u16 A_ScptId )
{
	// local -------------------
		Cls_GrP3DoBase*	Tv_Result;
		Cls_GrP3DoBase*	Tv_Slave;
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
Cls_GrP3DoBase*	Cls_GrP3DoBase::FindPrntObjByScptId( __u16 A_ScptId )
{
	// local -------------------
		Cls_GrP3DoBase*	Tv_Result;
		Cls_GrP3DoBase*	Tv_Prnt;
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
void	Cls_GrP3DoBase::RectGet( Ptr_GrRect A_PtrRtRect )
{
		*A_PtrRtRect	=	m_Rect;
}
//--------------------------------------------------------------------
__u32	Cls_GrP3DoBase::RtlParaInfoDump( void* A_PtrBuf )
{
#ifdef GR_P3_EDITOR
	// local -------------------
	// code --------------------
		m_PtrDvlParaBuf	=	A_PtrBuf;
		m_DvlParaPrgs		=	0;
		RtlDvlParaInfoSubDump();
		return	m_DvlParaPrgs;
#else
		return	0;
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBase::LcDvlParaInfoSaveAdd( void* A_PtrPara, void* A_PtrInfo, __u32 A_Cnt )
{
#ifdef GR_P3_EDITOR
	// local -------------------
		Ptr_GrP3DvlDoPrInfoData	Tv_PtrBuf;
		Ptr_GrP3DvlDoPrInfoData	Tv_PtrInfo;
		__u32	Tv_WkIdx;
	// code --------------------
		Tv_PtrBuf		=	(Ptr_GrP3DvlDoPrInfoData)( (__u32)m_PtrDvlParaBuf + m_DvlParaPrgs );
		Tv_PtrInfo	=	(Ptr_GrP3DvlDoPrInfoData)A_PtrInfo;
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
			m_DvlParaPrgs	=	m_DvlParaPrgs + sizeof(St_GrP3DvlDoPrInfoData);
		}
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBase::RtlDvlParaInfoSubDump( void )
{
#ifdef GR_P3_EDITOR
	// local -------------------
	// code --------------------
		LcDvlParaInfoSaveAdd( &m_ParaDoBase, V_GrP3DoBaseParaInfo, E_GrP3DoBaseParaCnt );
#endif
}
//--------------------------------------------------------------------
__u32	Cls_GrP3DoBase::RtlScptDump( void* A_PtrScpt, BOOL8 A_IsCopyMode )
{
#ifdef GR_P3_EDITOR
	// local -------------------
		Ptr_GrP3ScptObjHd		Tv_PtrHd;
		Ptr_GrP3ScptObjEnd	Tv_PtrEd;
		Cls_GrP3DoBase*	Tv_Slave;
		__u32	Tv_ScptSize;
		void*	Tv_PtrScpt;
	// code --------------------
		//init
		m_ScptPrgs	=	0;
		m_PtrScpt		=	A_PtrScpt;

		// make header
		Tv_PtrHd	=	(Ptr_GrP3ScptObjHd)A_PtrScpt;
		Tv_PtrHd->Stx			=	E_GrP3ScptObjStx;
		Tv_PtrHd->ClassId	=	m_ClassId;
		Tv_PtrHd->ScptId	=	m_ScptId;
		if ( A_IsCopyMode )
		{
			// copy mode
			Tv_PtrHd->ScptId	=	E_GrP3ScptIdNone;
		}
		Tv_PtrHd->ParaSize		=	m_ParaSize;
		Tv_PtrHd->Rect.left		=	(__s16)m_Rect.left;
		Tv_PtrHd->Rect.top		=	(__s16)m_Rect.top;
		Tv_PtrHd->Rect.right	=	(__s16)m_Rect.right;
		Tv_PtrHd->Rect.bottom	=	(__s16)m_Rect.bottom;

		// update progass
		m_ScptPrgs	=	sizeof(St_GrP3ScptObjHd);

		// check page window
		if ( (E_GrP3ScptIdPageBase <= m_ScptId) && ((E_GrP3ScptIdPageBase + E_GrP3DvlPageMaxCnt) > m_ScptId) )
		{
			Tv_PtrHd->ParaSize	=	0;
		}
		else
		{
			// get parametters
			RtlScptParaSubDump();
		}

		// check this class type
		if ( E_GrP3DoClassFrame != m_ClassId )
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
		Tv_PtrEd	=	(Ptr_GrP3ScptObjEnd)( (__u32)A_PtrScpt + m_ScptPrgs );
		Tv_PtrEd->Edx			=	E_GrP3ScptObjEdx;
		Tv_PtrEd->ScptId	=	m_ScptId;

		// update size
		m_ScptPrgs	=	m_ScptPrgs + sizeof(St_GrP3ScptObjEnd);
		
		return	m_ScptPrgs;
#else
	return	0;
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBase::ProcEvtSysStat( __u8 A_Type )
{
		if ( E_GrP3SysStatLangChg == A_Type )
		{
			ReqDraw();
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBase::RtlInit( void )
{
	// local -------------------
		Cls_GrP3DoBase*	Tv_Obj;
	// code --------------------
		// update last focus
		if ( IsCanKey() )
		{
			((Cls_GrP3DoWin*)m_ObjWin)->RtlLastFocusObjSet( this );
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
void	Cls_GrP3DoBase::RtlFindFocusableSlave( Ptr_GrP3DoFindFocusAble A_PtrEnv )
{
	// local -------------------
		Cls_GrP3DoBase*	Tv_ObjSlave;
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
Cls_GrP3DoBase*	Cls_GrP3DoBase::FindFocusableSlave( void )
{
	// local -------------------
		St_GrP3DoFindFocusAble	Tv_Env;
	// code --------------------
		Tv_Env.x		=	0x7FFFFFFF;
		Tv_Env.y		=	0x7FFFFFFF;
		Tv_Env.Obj	=	NULL;
		RtlFindFocusableSlave( &Tv_Env );
		return	(Cls_GrP3DoBase*)Tv_Env.Obj;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBase::ProcEvtFramePageChged( __u16 A_FrmadId, __u16 A_PageId )
{
		// not implement
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBase::FindSlaveWithCallback( void )
{
		RtlFindSlaveWithCallback( this );
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBase::RtlFindSlaveWithCallback( Cls_GrP3DoBase* A_Host )
{
	// local -------------------
		Cls_GrP3DoBase*	Tv_Obj;
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
void	Cls_GrP3DoBase::RtlCallbackSlave( Cls_GrP3DoBase* A_Obj )
{
		// not implement
}
//--------------------------------------------------------------------
__u32	Cls_GrP3DoBase::UserDataGet( __u8 A_Idx )
{
	// local -------------------
		__u32	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		if ( E_GrP3DoUserDataCnt > A_Idx )
		{
			Tv_Result	=	m_ParaDoBase.UserDataTbl[A_Idx];
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoBase::ProcEvtFocusDirection( __u8 A_Direction, __u32 A_Atb )
{
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBase::ProcEvtVkbdEditEnd( void )
{

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoBase::MoveFocusByDir( __u8 A_Direction, __u32 A_Atb )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_ScptId;
		Cls_GrP3DoBase*	Tv_Obj;
	// code --------------------
		Tv_Result	=	FALSE;
		Tv_ScptId	=	m_ParaDoBase.FcsMvTbl[A_Direction];
		if ( E_GrP3ScptIdNone != Tv_ScptId )
		{
			Tv_Obj	=	m_ObjWin->FindSlaveObjByScptId( Tv_ScptId );
			if ( NULL != Tv_Obj )
			{
				m_FindId	=	V_GrP3Mng->UiFindIdAlloc();
				Tv_Result	=	Tv_Obj->RtlMoveFcsByDir( A_Direction, A_Atb, m_FindId );
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoBase::RtlMoveFcsByDir( __u8 A_Direction, __u32 A_Atb, __u32 A_FindId )
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrP3DoBase*	Tv_Obj;
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
				((Cls_GrP3ConBase*)m_ObjCon)->FocusObjSet( this );
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
void	Cls_GrP3DoBase::RelateCtrlActive( BOOL8 A_IsActive )
{
	// local -------------------
		Cls_GrP3DoBase*	Tv_Obj;
	// code --------------------
		// get relate control
		if ( E_GrP3ScptIdNone != m_ParaDoBase.RelateId )
		{
			Tv_Obj	=	m_ObjWin->FindSlaveObjByScptId( m_ParaDoBase.RelateId );
			if ( NULL != Tv_Obj )
			{
				switch ( Tv_Obj->ClassIdGet() )
				{
					case E_GrP3DoClassRelStc:
						((Cls_GrP3DoRelStc*)Tv_Obj)->ActiveSet( A_IsActive );
						break;
				}
				
			}
		}
}
//--------------------------------------------------------------------
__u16	Cls_GrP3DoBase::FocusTargetGet( __u8 A_Direction )
{
		return	m_ParaDoBase.FcsMvTbl[A_Direction];
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBase::FocusTargetSet( __u8 A_Direction, __u16 A_TargetId )
{
	// local -------------------
	// code --------------------
		m_ParaDoBase.FcsMvTbl[A_Direction]	=	A_TargetId;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoBase::IsOnMouse( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		
		if ( this == ((Cls_GrP3ConBase*)m_ObjCon)->MouseOnObjGet() )
		{
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
