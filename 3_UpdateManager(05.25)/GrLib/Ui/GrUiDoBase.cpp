/*
 osd object

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<Ui/GrUiDoBase.h>
#include	<GrStrTool.h>
#include	<GrDumyTool.h>
#include	<stdlib.h>
#include	<Ui/GrUiDoWin.h>
#include	<Ui/GrUiMng.h>
#include	<Ui/GrUiConsole.h>
#include	<Ui/GrUiDoMsgbox.h>
#include	<Ui/GrUiScptDec.h>
#include	<Ui/GrUiDoFrm.h>
#include	<Ui/GrUiDoLbl.h>
#include	<Ui/GrUiDoCdvArea.h>
#include	<GrDebug.h>

//====================================================================
//local const

#define E_GrUiDoBaseTxtSpcShr		2


//====================================================================
//local macro
#define m_ObjConsole		((Cls_GrUiConsole*)m_ObjCon)

//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var
extern	Cls_GrUiMng*	V_GrUiMngObj;
extern	BOOL8					V_IsGrUiMngPalMode;

//====================================================================
//--------------------------------------------------------------------
Cls_GrUiDoBase::Cls_GrUiDoBase(	Ptr_GrRect A_PtrRect, Cls_GrUiDoBase* A_ObjPrnt, __u8 A_ConIdx, BOOL8 A_IsIdAlloc )
{
	// local -------------------
		__u32		Tv_WkIdx;
	// code --------------------
		//init
		m_ObjId		=	E_GrUiObjIdNone;
		
		m_RelFcsLblId	=	E_GrUiObjIdNone;
		m_RelFcsTxtId	=	E_GrUiTxtIdNone;

		//get console
		if ( NULL != A_ObjPrnt )
		{
			//sub object
			m_ObjWin	=	A_ObjPrnt->WindowGet();
			m_ObjCon	=	((Cls_GrUiDoWin*)m_ObjWin)->ConsolGet();
		}
		else
		{
			//window
			m_ObjWin	=	this;
			m_ObjCon	=	(void*)V_GrUiMngObj->GetConsoleObj( A_ConIdx );
		}

		if ( A_IsIdAlloc )
		{
			m_ObjId	=	m_ObjConsole->ObjIdAlloc();
		}
		
		if ( NULL != A_PtrRect )
		{
			m_Rect					=	*A_PtrRect;
		}
		else
		{
			//default value - not useable
			m_Rect.left			=	0;
			m_Rect.top			=	0;
			m_Rect.right		=	50;
			m_Rect.bottom		=	50;
		}

		m_ObjPrnt				=	A_ObjPrnt;
		m_TypeId				=	E_GrUiDoTypeIdNone;
		m_IsFocusAble		=	FALSE;
		m_IsFocusBlock	=	FALSE;
		m_IsVisible			=	TRUE;
		m_IsEnable			=	TRUE;
		m_FocusMoveMap	=	E_GrUiDoFocusMapAll;
		m_FocusBaseId		=	E_GrUiObjIdNone;
		m_CondVisMap[0]	=	0;
		m_CondVisMap[1]	=	0;
		m_PalExtPosY		=	0;
		m_PalExtSizeY		=	0;

		m_IsCondDeced		=	FALSE;		//condition decoded

		for ( Tv_WkIdx = 0;Tv_WkIdx < E_GrUiDoUserDataCnt;Tv_WkIdx++ )
		{
			m_UserDataTbl[Tv_WkIdx]			=	0;
		}

		m_CliMgn.left		=	0;
		m_CliMgn.top 		=	0;
		m_CliMgn.right	=	0;
		m_CliMgn.bottom	=	0;

		m_ObjSlave			=	NULL;
		m_GdibScr				=	m_ObjConsole->GetGdibScr();

		//get next
		m_ObjNext				=	NULL;
		if ( NULL != A_ObjPrnt )
		{
			m_ObjNext			=	m_ObjPrnt->SlaveObjGet();	//patch next object
		}

		//register parent
		if ( NULL != m_ObjPrnt )
		{
			m_ObjNext	=	m_ObjPrnt->SlaveObjGet();
			m_ObjPrnt->RtlSlaveObjSet( this );
		}
}
//--------------------------------------------------------------------
Cls_GrUiDoBase::~Cls_GrUiDoBase()
{
	// local -------------------
		Cls_GrUiDoBase*	Tv_ObjPrv;
		Cls_GrUiDoBase*	Tv_ObjFocus;
	// code --------------------
		//draw
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
		if ( this == m_ObjConsole->MouseOnObjGet() )
		{
			//on mouse object reset
			m_ObjConsole->MouseOnObjReset();
		}

		//release local focus
		if ( !IsWindow() )
		{
			//check match
			Tv_ObjFocus	=	((Cls_GrUiDoWin*)m_ObjWin)->LastFocusObjGet();
			if ( Tv_ObjFocus == this )
			{
				((Cls_GrUiDoWin*)m_ObjWin)->RtlLastFocusObjSet( NULL );			//last focus clear
			}
		}

		//release global focus
		Tv_ObjFocus	=	m_ObjConsole->FocusKeyObjGet();
		if ( this == Tv_ObjFocus )
		{
			//focus release
			m_ObjConsole->FocusReset();
			//request focus rebuild
			m_ObjConsole->FocusCorrect();
		}

		//free obj id
		m_ObjConsole->ObjIdFree( m_ObjId );

}
//--------------------------------------------------------------------
void	Cls_GrUiDoBase::ObjSlaveFreeAll( void )
{
		while ( NULL != m_ObjSlave )
		{
			delete	m_ObjSlave;
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoBase::TypeIdGet( void )
{
		return	m_TypeId;
}
//--------------------------------------------------------------------
void*	Cls_GrUiDoBase::ConsoleGet( void )
{
		return	m_ObjCon;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoBase::IsWindow( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//check type
		if ( E_GrUiDoTypeIdWin == m_TypeId )
		{
			Tv_Result		=	TRUE;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
Cls_GrUiDoBase*	Cls_GrUiDoBase::ParentObjGet( void )
{
		return	m_ObjPrnt;
}
//--------------------------------------------------------------------
Cls_GrUiDoBase*	Cls_GrUiDoBase::WindowGet( void )
{
		return	m_ObjWin;
}
//--------------------------------------------------------------------
Ptr_GrRect	Cls_GrUiDoBase::RectGet( void )
{
		return	&m_Rect;
}
//--------------------------------------------------------------------
__s32		Cls_GrUiDoBase::WidthGet( void )
{
		return	m_Rect.right - m_Rect.left;
}
//--------------------------------------------------------------------
__s32		Cls_GrUiDoBase::HeightGet( void )
{
		return	m_Rect.bottom - m_Rect.top;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoBase::ScrRectGet( Ptr_GrRect A_PtrRtRect )
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrUiDoBase*	Tv_Obj;
	// code --------------------
		//init
		Tv_Result		=	TRUE;
		Tv_Obj			=	this;

		//check visible
		//if ( m_IsVisible )
		{
			//init rect
			*A_PtrRtRect	=	m_Rect;
			if ( V_IsGrUiMngPalMode )
			{
				A_PtrRtRect->top		=	A_PtrRtRect->top + (__s32)m_PalExtPosY;
				A_PtrRtRect->bottom	=	A_PtrRtRect->bottom + (__s32)m_PalExtPosY + (__s32)m_PalExtSizeY;
			}

			//parent get
			Tv_Obj		=	m_ObjPrnt;
			while ( (NULL != Tv_Obj) && Tv_Result )
			{
				//patch
				Tv_Result	=	Tv_Obj->RtlScrRectCal( A_PtrRtRect );

				//next
				Tv_Obj	=	Tv_Obj->ParentObjGet();
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoBase::ClientRectGet( Ptr_GrRect A_PtrRtRect )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		if ( ScrRectGet( A_PtrRtRect ) )
		{
			A_PtrRtRect->left		=	A_PtrRtRect->left + (__s32)m_CliMgn.left;
			A_PtrRtRect->top		=	A_PtrRtRect->top + (__s32)m_CliMgn.top;
			A_PtrRtRect->right	=	A_PtrRtRect->right - (__s32)m_CliMgn.right;
			A_PtrRtRect->bottom	=	A_PtrRtRect->bottom - (__s32)m_CliMgn.bottom;
			//check able
			if ( (A_PtrRtRect->left < A_PtrRtRect->right) && (A_PtrRtRect->top < A_PtrRtRect->bottom) )
			{
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Cls_GrUiDoBase*	Cls_GrUiDoBase::NextObjGet( void )
{
		return	m_ObjNext;
}
//--------------------------------------------------------------------
Cls_GrUiDoBase*	Cls_GrUiDoBase::SlaveObjGet( void )
{
		return	m_ObjSlave;
}
//--------------------------------------------------------------------
Cls_GrUiDoBase*	Cls_GrUiDoBase::PrevObjGet( void )
{
	// local -------------------
		Cls_GrUiDoBase*	Tv_Result;
		Cls_GrUiDoBase*	Tv_ObjNext;
	// code --------------------
		//init
		Tv_Result		=	NULL;

		if ( NULL != m_ObjPrnt )
		{
			Tv_Result	=	m_ObjPrnt->SlaveObjGet();
			if ( Tv_Result == (Cls_GrUiDoBase*)this )
			{
				Tv_Result	=	NULL;
			}
			else
			{
				while ( NULL != Tv_Result )
				{
					//get next
					Tv_ObjNext	=	Tv_Result->NextObjGet();
					if ( Tv_ObjNext == (Cls_GrUiDoBase*)this )
					{
						//finded
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
void	Cls_GrUiDoBase::RtlNextObjSet( Cls_GrUiDoBase* A_Obj )
{
		m_ObjNext		=	A_Obj;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBase::RtlSlaveObjSet( Cls_GrUiDoBase* A_Obj )
{
		m_ObjSlave	=	A_Obj;
}
//--------------------------------------------------------------------
__u16	Cls_GrUiDoBase::ObjIdGet( void )
{
		return	m_ObjId;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBase::RectSet( Ptr_GrRect A_PtrRect )
{
		if ( IsVisible() )
		{
			//redraw
			ReqDraw();
			//change position
			m_Rect	=	*A_PtrRect;
			//redraw
			ReqDraw();
		}
		else
		{
			//change position
			m_Rect	=	*A_PtrRect;
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBase::ReqDraw( void )
{
	// local -------------------
		St_GrRect	Tv_RcScr;
	// code --------------------
		if ( ScrRectGet( &Tv_RcScr ) )
		{
			m_ObjConsole->PaintReq( &Tv_RcScr );
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBase::EnableSet( BOOL8 A_IsEnable )
{
	// local -------------------
		BOOL8	Tv_IsOld;
	// code --------------------
		Tv_IsOld		=	IsCanEnable();
		m_IsEnable	=	A_IsEnable;
		if ( Tv_IsOld != IsCanEnable() )
		{
			if ( !m_IsEnable )
			{
				LcFreeFocus();
			}
			ReqDraw();
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoBase::IsEnable( void )
{
		return	m_IsEnable;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBase::LcFreeFocus( void )
{
		if ( !IsWindow() )
		{
			//check window focused
			if ( this == ((Cls_GrUiDoWin*)m_ObjWin)->LastFocusObjGet() )
			{
				//release window last focus
				((Cls_GrUiDoWin*)m_ObjWin)->RtlLastFocusObjSet( NULL );
			}
			if ( IsFocused() )
			{
				m_ObjConsole->FocusObjSet( NULL );
				m_ObjConsole->FocusCorrect();
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBase::VisibleSet( BOOL8 A_IsVisible )
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
			ReqDraw();
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoBase::IsVisible( void )
{
		return	m_IsVisible;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoBase::IsCanVisible( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrUiDoBase*	Tv_Obj;
	// code --------------------
		//init
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
BOOL8	Cls_GrUiDoBase::IsCanEnable( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrUiDoBase*	Tv_Obj;
	// code --------------------
		//init
		Tv_Result	=	TRUE;

		Tv_Obj		=	this;
		while ( NULL != Tv_Obj )
		{
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
BOOL8	Cls_GrUiDoBase::IsFocusAble( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( m_IsFocusAble && (!m_IsFocusBlock) )
		{
			Tv_Result	=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoBase::IsInputAble( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrUiDoWin*	Tv_Win;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( IsCanEnable() && IsCanVisible() && m_IsFocusAble )
		{
			Tv_Result		=	TRUE;
			//check window focusable
			if ( !IsWindow() )
			{
				Tv_Win	=	(Cls_GrUiDoWin*)m_ObjWin;;
				Tv_Result	=	Tv_Win->IsFocusAble();
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoBase::UserDataGet( __u32 A_Idx )
{
		return	m_UserDataTbl[A_Idx];
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBase::UserDataSet( __u32 A_Data, __u32 A_Idx )
{
		m_UserDataTbl[A_Idx]	=	A_Data;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBase::ProcPaint( void )
{
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBase::RtlPaint( Ptr_GrRect A_PtrUpdtRect )
{
	// local -------------------
	// code --------------------
		if ( IsCanVisible() )
		{
			if ( ScrRectGet( &m_PntRcScr ) )
			{
				if (GrDrawPatchPaintRect(	m_ObjConsole->GetResX(), m_ObjConsole->GetResY(), 
																	&m_PntRcScr, A_PtrUpdtRect, &m_PntRcClip ) )
				{
					//calculate client paint rect
					m_PntRcClient.left		=	m_PntRcScr.left + (__s32)m_CliMgn.left;
					m_PntRcClient.top			=	m_PntRcScr.top + (__s32)m_CliMgn.top;
					m_PntRcClient.right		=	m_PntRcScr.right - (__s32)m_CliMgn.right;
					m_PntRcClient.bottom	=	m_PntRcScr.bottom - (__s32)m_CliMgn.bottom;
					m_IsPntCliAble	=	GrDrawPatchPaintRect(	(__s32)m_ObjConsole->GetResX(), (__s32)m_ObjConsole->GetResY(), 
														&m_PntRcClient, A_PtrUpdtRect, &m_PntRcCliClip );
					//paint this
					ProcPaint();

					//paint sub control
					if ( NULL != m_ObjSlave )
					{
						m_ObjSlave->RtlPaint( A_PtrUpdtRect );
					}
				}
			}
		}
		//paint next control
		if ( NULL != m_ObjNext )
		{
			m_ObjNext->RtlPaint( A_PtrUpdtRect );
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBase::LcTextRtlReset( Ptr_GrUiDoTextRtl A_PtrRtl )
{
		A_PtrRtl->CharPos		=	0;
		A_PtrRtl->MaxX			=	0;
		A_PtrRtl->MaxY			=	0;
		A_PtrRtl->DrawX			=	0;
		A_PtrRtl->DrawY			=	0;
		A_PtrRtl->PcSize		=	0;
		A_PtrRtl->IsRunAble	=	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBase::LcTextRtlInit( Ptr_GrUiDoTextRtl A_PtrRtl, WCHAR* A_Str, Ptr_GrUiDoTextPara A_PtrPara, __u32 A_ClrTxt, Ptr_GrRect A_PtrRcDraw, BOOL8 A_IsCursor, __s32 A_CursorPos )
{
		A_PtrRtl->ObjFont		=	V_GrUiMngObj->FontGet( A_PtrPara->FontId );
		A_PtrRtl->ClrTxt		=	A_ClrTxt;
		A_PtrRtl->WarpSizeX	=	A_PtrRcDraw->right - A_PtrRcDraw->left;
		//A_PtrRtl->SizeX	=	A_PtrRcDraw->right - A_PtrRcDraw->left;
		//A_PtrRtl->SizeY	=	A_PtrRcDraw->bottom - A_PtrRcDraw->top;
		A_PtrRtl->IsCursOn	=	A_IsCursor;
		A_PtrRtl->CusPos		=	A_CursorPos;
		A_PtrRtl->PtrChar		=	(__u16*)A_Str;
		LcTextRtlReset( A_PtrRtl );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoBase::LcTextRtlPars( Ptr_GrUiDoTextPara A_PtrPara, Ptr_GrUiDoTextRtl A_PtrRtl, BOOL8* A_PtrIsFontExist, BOOL8* A_PtrIsCurs )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u16		Tv_Code;
		__u32		Tv_MapIdx;
		__s32			Tv_FontPitch;
		__s32			Tv_DrawSizeX;
	// code --------------------
		//init
		Tv_Result	=	A_PtrRtl->IsRunAble;
		*A_PtrIsFontExist	=	FALSE;
		*A_PtrIsCurs			=	FALSE;

		//check first
		if ( 0 == A_PtrRtl->MaxY )
		{
			A_PtrRtl->MaxY	=	A_PtrRtl->ObjFont->GetHeight() + A_PtrPara->LineGap;
		}

		//check run able
		if ( A_PtrRtl->IsRunAble )
		{
			//calculate now position
			A_PtrRtl->DrawX		=	A_PtrRtl->DrawX + A_PtrRtl->PcSize;
			A_PtrRtl->PcSize	=	0;
			//get now code
			Tv_Code		=	A_PtrRtl->PtrChar[A_PtrRtl->CharPos];
			if ( 0 != Tv_Code	)
			{
				//check now position is cursor
				if ( A_PtrRtl->IsCursOn && ( A_PtrRtl->CharPos == A_PtrRtl->CusPos ) )
				{
					*A_PtrIsCurs	=	TRUE;
				}
				//check password mode
				if ( 0 != (E_GrUiDoTextAtbPasswd & A_PtrPara->Atb) )
				{
					Tv_Code		=	L'*';
				}
				//next __s8
				A_PtrRtl->CharPos ++;
				//get font info
				if ( A_PtrRtl->ObjFont->GetCharInfo( Tv_Code, &Tv_MapIdx, &A_PtrRtl->RcFont, &Tv_FontPitch, &A_PtrRtl->StGap ) )
				{
					//calc __s8 space
					Tv_DrawSizeX	=	Tv_FontPitch + (__s32)A_PtrPara->PitchGap;
					if ( 0 != (E_GrUiDoTextAtbWidthDraw & A_PtrPara->Atb) )
					{
						Tv_DrawSizeX	=	(__s32)A_PtrRtl->ObjFont->GetWidth() + (__s32)A_PtrPara->PitchGap;
					}
					//font exist
					*A_PtrIsFontExist	=	TRUE;
				}
				else
				{
					//check special code
					switch( Tv_Code )
					{
						case	0x0A:
							//enter code
							Tv_DrawSizeX	=	0;
							if ( 0 != (E_GrUiDoTextAtbMultiLine & A_PtrPara->Atb) )
							{
								//go next pos
								A_PtrRtl->DrawX		=	0;
								A_PtrRtl->DrawY		=	A_PtrRtl->MaxY;
								A_PtrRtl->MaxY		=	A_PtrRtl->DrawY + A_PtrRtl->ObjFont->GetHeight() + A_PtrPara->LineGap;
							}
							else
							{
								//finish
								A_PtrRtl->IsRunAble	=	FALSE;
							}
							break;
						case	0x0D:
							//not pars
							Tv_DrawSizeX	=	0;
							break;
						default:
							//no code = space
							Tv_DrawSizeX	=	( (__s32)A_PtrRtl->ObjFont->GetWidth() >> E_GrUiDoBaseTxtSpcShr ) + (__s32)A_PtrPara->PitchGap;
							break;
					}
				}
				//check __u16 warp
				if ( A_PtrRtl->WarpSizeX < (A_PtrRtl->DrawX + Tv_DrawSizeX) )
				{
					if ( (0 != (E_GrUiDoTextAtbMultiLine & A_PtrPara->Atb)) && (0 != (E_GrUiDoTextAtbWordwarp & A_PtrPara->Atb)) )
					{
						//change position
						A_PtrRtl->DrawX		=	0;
						A_PtrRtl->DrawY		=	A_PtrRtl->MaxY;
						A_PtrRtl->MaxY		=	A_PtrRtl->DrawY + A_PtrRtl->ObjFont->GetHeight() + A_PtrPara->LineGap;
					}
				}
				//check run able
				if ( A_PtrRtl->IsRunAble )
				{
					//update max x size
					if ( A_PtrRtl->MaxX < (A_PtrRtl->DrawX + Tv_DrawSizeX) )
					{
						A_PtrRtl->MaxX	=	(A_PtrRtl->DrawX + Tv_DrawSizeX);
					}
					//update prevoius x size
					A_PtrRtl->PcSize	=	Tv_DrawSizeX;
				}
				//update result
				Tv_Result		=	A_PtrRtl->IsRunAble;
			}
			else
			{
				//set finish
				A_PtrRtl->IsRunAble		=	FALSE;
				Tv_Result		=	FALSE;
				//cursor last position
				if ( A_PtrRtl->IsCursOn && ( A_PtrRtl->CharPos == A_PtrRtl->CusPos ) )
				{
					//check 
					Tv_DrawSizeX	=	( (__s32)A_PtrRtl->ObjFont->GetWidth() >> E_GrUiDoBaseTxtSpcShr ) + (__s32)A_PtrPara->PitchGap;
					
					//check word warp
					if ( A_PtrRtl->WarpSizeX < (A_PtrRtl->DrawX + Tv_DrawSizeX) )
					{
						//check need word warp
						if ( (0 != (E_GrUiDoTextAtbMultiLine & A_PtrPara->Atb)) && (0 != (E_GrUiDoTextAtbWordwarp & A_PtrPara->Atb)) )
						{
							//change position
							A_PtrRtl->DrawX		=	0;
							A_PtrRtl->DrawY		=	A_PtrRtl->MaxY;
							A_PtrRtl->MaxY		=	A_PtrRtl->DrawY + A_PtrRtl->ObjFont->GetHeight() + A_PtrPara->LineGap;
							//cursor draw
							*A_PtrIsCurs	=	TRUE;
							Tv_Result			=	TRUE;
						}
					}
					else
					{
						//cursor draw
						*A_PtrIsCurs	=	TRUE;
						Tv_Result			=	TRUE;
					}
				}
			}
		}

		return	A_PtrRtl->IsRunAble;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBase::LcTextAreaGet( __s32* A_PtrRtSizeX, __s32* A_PtrRtSizeY, Ptr_GrUiDoTextPara A_PtrPara, Ptr_GrUiDoTextRtl A_PtrRtl )
{
	// local -------------------
		BOOL8	Tv_IsFont;
		BOOL8	Tv_IsCurs;
	// code --------------------
		//reset
		LcTextRtlReset( A_PtrRtl );
		//calculate
		while ( LcTextRtlPars( A_PtrPara, A_PtrRtl, &Tv_IsFont, &Tv_IsCurs ) )
		{
			//not work
		}
		//update
		*A_PtrRtSizeX	=	A_PtrRtl->MaxX;
		*A_PtrRtSizeY	=	A_PtrRtl->MaxY;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBase::LcTextDrawing( __s32 A_X, __s32 A_Y, Ptr_GrRect A_PtrRcUpdt, Ptr_GrUiDoTextPara A_PtrPara, Ptr_GrUiDoTextRtl A_PtrRtl )
{
	// local -------------------
		BOOL8	Tv_IsFont;
		BOOL8	Tv_IsCurs;
		St_GrRect	Tv_RcTmp;
		St_GrRect	Tv_RcUpdt;
		__s32		Tv_DrawX;
		__s32		Tv_DrawY;
		__u32	Tv_ClrTxt;
		BOOL8	Tv_IsShadow;
	// code --------------------
		//reset
		LcTextRtlReset( A_PtrRtl );
		//set blt mode
		Tv_IsShadow		=	FALSE;
		if ( (0 != ( E_GrUiDoTextAtbDrawShadow & A_PtrPara->Atb)) )
		{
			Tv_IsShadow		=	TRUE;
		}

		//calculate
		while ( LcTextRtlPars( A_PtrPara, A_PtrRtl, &Tv_IsFont, &Tv_IsCurs ) )
		{
			//check font drawing
			if ( Tv_IsFont )
			{
				//draw font
				Tv_DrawX	=	A_X + A_PtrRtl->DrawX;
				Tv_DrawY	=	A_Y + A_PtrRtl->DrawY;
				Tv_ClrTxt	=	A_PtrRtl->ClrTxt;
				if ( Tv_IsCurs )
				{
					Tv_ClrTxt	=	m_ObjConsole->CursColorGet();
				}
				//draw
				m_GdibScr->FontBlt( Tv_DrawX, Tv_DrawY, A_PtrRtl->ObjFont->GetGdibPtr(), Tv_ClrTxt, 
														A_PtrPara->ClrShadow, Tv_IsShadow, &A_PtrRtl->RcFont );
			}
			else
			{
				//check cursor
				if ( Tv_IsCurs )
				{
					//draw cursor
					Tv_RcTmp.left			=	A_PtrRtl->DrawX + A_X;
					Tv_RcTmp.top			=	A_PtrRtl->DrawY + A_Y;
					Tv_RcTmp.right		=	Tv_RcTmp.left + A_PtrRtl->PcSize;
					Tv_RcTmp.bottom		=	Tv_RcTmp.top + A_PtrRtl->ObjFont->GetHeight();
					if ( GrDrawCnGetComRect( &Tv_RcUpdt, &Tv_RcTmp, A_PtrRcUpdt ) )
					{
						m_GdibScr->FillRect( &Tv_RcUpdt, m_ObjConsole->CursColorGet() );
					}
				}
			}
		}
		//draw last cursor
		if ( Tv_IsCurs )
		{
			//draw cursor
			Tv_RcTmp.left			=	A_PtrRtl->DrawX + A_X;
			Tv_RcTmp.top			=	A_PtrRtl->DrawY + A_Y;
			Tv_RcTmp.right		=	Tv_RcTmp.left + ( ( (__s32)A_PtrRtl->ObjFont->GetWidth() >> E_GrUiDoBaseTxtSpcShr ) + (__s32)A_PtrPara->PitchGap );
			Tv_RcTmp.bottom		=	Tv_RcTmp.top + A_PtrRtl->ObjFont->GetHeight();
			if ( GrDrawCnGetComRect( &Tv_RcUpdt, &Tv_RcTmp, A_PtrRcUpdt ) )
			{
				m_GdibScr->FillRect( &Tv_RcUpdt, m_ObjConsole->CursColorGet() );
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoBase::TextDraw( WCHAR* A_Str, Ptr_GrUiDoTextPara A_PtrPara, __u32 A_ClrTxt, 
																Ptr_GrRect A_PtrRcScr, BOOL8 A_IsCursor, __s32 A_CursorPos )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrRect	Tv_RcClipBk;
		St_GrRect	Tv_RcClipDw;
		__s32		Tv_TxtAreaX;
		__s32		Tv_TxtAreaY;
		St_GrUiDoTextRtl	Tv_Rtl;
		__s32		Tv_TxtX;			//text draw position
		__s32		Tv_TxtY;
	// code --------------------
		//init
		Tv_Result	=	FALSE;

		//check exist string
		if ( NULL != A_Str )
		{
			//initialize runtime
			LcTextRtlInit( &Tv_Rtl, A_Str, A_PtrPara, A_ClrTxt, A_PtrRcScr, A_IsCursor, A_CursorPos );
			if ( NULL != Tv_Rtl.ObjFont )
			{
				//get text size
				LcTextAreaGet( &Tv_TxtAreaX, &Tv_TxtAreaY, A_PtrPara, &Tv_Rtl );
				//check drawable
				if ( 0 != Tv_TxtAreaX )
				{
					//calc horizontal position
					switch( (E_GrUiDoTextAtbAlignHzMask & A_PtrPara->Atb) )
					{
					case	E_GrUiDoTextAtbAlignHzLeft:
						Tv_TxtX		=	A_PtrRcScr->left;
						break;
					case	E_GrUiDoTextAtbAlignHzRight:
						Tv_TxtX		=	A_PtrRcScr->right - Tv_TxtAreaX;
						break;
					default:
						//center
						Tv_TxtX		=	A_PtrRcScr->left + (( (A_PtrRcScr->right - A_PtrRcScr->left) - Tv_TxtAreaX ) / 2 );
						break;
					}
					//calc vertical position
					switch( (E_GrUiDoTextAtbAlignVtMask & A_PtrPara->Atb) )
					{
					case	E_GrUiDoTextAtbAlignVtTop:
						Tv_TxtY		=	A_PtrRcScr->top;
						break;
					case	E_GrUiDoTextAtbAlignVtBottom:
						Tv_TxtY		=	A_PtrRcScr->bottom - Tv_TxtAreaY;
						break;
					default:
						//center
						Tv_TxtY		=	A_PtrRcScr->top + (( (A_PtrRcScr->bottom - A_PtrRcScr->top) - Tv_TxtAreaY ) / 2 );
						break;
					}
					//clip set
					m_GdibScr->ClipperGet( &Tv_RcClipBk );
					if ( GrDrawCnGetComRect( &Tv_RcClipDw, &Tv_RcClipBk, A_PtrRcScr ) )
					{
						m_GdibScr->ClipperSet( &Tv_RcClipDw );
						//draw
						LcTextDrawing( Tv_TxtX, Tv_TxtY, A_PtrRcScr, A_PtrPara, &Tv_Rtl );
						//cliper restore
						m_GdibScr->ClipperSet( &Tv_RcClipBk );
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoBase::IsCanFocus( void )
{
	// local -------------------
		BOOL8		Tv_Result;
		Cls_GrUiDoWin*	Tv_Win;
	// code --------------------
		//init
		Tv_Result	=	FALSE;

		if ( IsCanEnable() && IsCanVisible() && m_IsFocusAble && (!m_IsFocusBlock) )
		{
			Tv_Result		=	TRUE;
			//check window focusable
			if ( !IsWindow() )
			{
				Tv_Win	=	(Cls_GrUiDoWin*)m_ObjWin;;
				Tv_Result	=	Tv_Win->IsFocusAble();
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoBase::IsOvlpRect( Ptr_GrRect A_PtrRect )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrRect	Tv_RcScr;
		St_GrRect	Tv_RcCom;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//check visible
		if ( IsVisible() )
		{
			Tv_Result		=	GrDrawCnGetComRect( &Tv_RcCom, &Tv_RcScr, A_PtrRect );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBase::RtlFindFocusSlaveObjByDir( Ptr_GrUiDoFindFmObjByDir A_PtrFind )
{
	// local -------------------
		Cls_GrUiDoBase*	Tv_ObjSlave;
		St_GrRect	Tv_RcScr;
		St_GrDrawFindDirDist	Tv_Dist;
	// code --------------------
		//check exist slave object
		Tv_ObjSlave	=	m_ObjSlave;
		while ( NULL != Tv_ObjSlave )
		{
			//check owner
			if ( Tv_ObjSlave != (Cls_GrUiDoBase*)A_PtrFind->ObjOwner )
			{
				//check able
				if ( Tv_ObjSlave->IsEnable() && Tv_ObjSlave->IsVisible() )
				{
					//now rect check
					if ( Tv_ObjSlave->IsFocusAble() )
					{
						if ( Tv_ObjSlave->ScrRectGet( &Tv_RcScr ) )
						{
							//check able can able
							if ( GrDrawDistSimRectByDir( &A_PtrFind->RcOwner, &Tv_RcScr, (__u8)A_PtrFind->Dir, &Tv_Dist, A_PtrFind->IsWide ) )
							{
								//update result
								if ( NULL == A_PtrFind->ObjFinded )
								{
									//direct update
									A_PtrFind->ObjFinded	=	(void*)Tv_ObjSlave;
									A_PtrFind->Dist				=	Tv_Dist;
								}
								else
								{
									//check distance
									if (	((__u32)A_PtrFind->Dist.Dist > (__u32)Tv_Dist.Dist) || 
												( (A_PtrFind->Dist.Dist == Tv_Dist.Dist) && (A_PtrFind->Dist.Ref > Tv_Dist.Ref) ) )
									{
										//direct update
										A_PtrFind->ObjFinded	=	(void*)Tv_ObjSlave;
										A_PtrFind->Dist				=	Tv_Dist;
									}
								}
							}
						}
					}
					else
					{
						//slave check
						Tv_ObjSlave->RtlFindFocusSlaveObjByDir( A_PtrFind );
					}
				}
			}
			//check already find
			if ( (NULL != A_PtrFind->ObjFinded) && (0 == A_PtrFind->Dist.Dist) )
			{
				break;
			}

			//next
			Tv_ObjSlave	=	Tv_ObjSlave->NextObjGet();
		}
}
//--------------------------------------------------------------------
Cls_GrUiDoBase*	Cls_GrUiDoBase::FindFocusObjByDir( __u32 A_Dir, BOOL8 A_IsWide )
{
	// local -------------------
		Cls_GrUiDoBase*	Tv_Result;
		St_GrUiDoFindFmObjByDir	Tv_Find;
		Cls_GrUiDoBase*	Tv_ObjBase;
	// code --------------------
		//init
		Tv_Result	=	NULL;

		//check window
		if ( !IsWindow() )
		{
			//check able window
			if ( m_ObjWin->IsCanFocus() )
			{
				Tv_Find.Dir			=	A_Dir;
				Tv_Find.IsWide	=	A_IsWide;
				Tv_Find.ObjOwner	=	(void*)this;
				if ( ScrRectGet( &Tv_Find.RcOwner ) )
				{
					Tv_Find.ObjFinded		=	NULL;
					GrDrawDistSimRectInit( &Tv_Find.Dist );
					Tv_ObjBase	=	NULL;
					if ( E_GrUiObjIdNone != m_FocusBaseId )
					{
						Tv_ObjBase	=	m_ObjWin->FindSlaveObjByObjId( m_FocusBaseId );
					}
					if ( NULL == Tv_ObjBase )
					{
						Tv_ObjBase	=	m_ObjWin;
					}
					if ( NULL != Tv_ObjBase )
					{
						Tv_ObjBase->RtlFindFocusSlaveObjByDir( &Tv_Find );
						Tv_Result	=	(Cls_GrUiDoBase*)Tv_Find.ObjFinded;
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Cls_GrUiDoBase*	Cls_GrUiDoBase::RtlFindObjByPos( __s32 A_X, __s32 A_Y, BOOL8 A_IsInpAble )
{
	// local -------------------
		Cls_GrUiDoBase*	Tv_Result;
		St_GrRect	Tv_RcScr;
		Cls_GrUiDoBase*	Tv_ObjSlave;
		Cls_GrUiDoBase*	Tv_ObjFind;
	// code --------------------
		//init
		Tv_Result		=	NULL;

		//check visible
		if ( m_IsVisible && (m_IsEnable || !A_IsInpAble) )
		{
			//get rect
			if ( ScrRectGet( &Tv_RcScr ) )
			{
				//check able 
				if ( (A_X >= Tv_RcScr.left) && (A_X < Tv_RcScr.right) && (A_Y >= Tv_RcScr.top) && (A_Y < Tv_RcScr.bottom) )
				{
					//set result
					if ( (A_IsInpAble && m_IsFocusAble ) || (!A_IsInpAble) )
					{
						Tv_Result		=	this;
					}
					//check slave
					Tv_ObjSlave	=	m_ObjSlave;
					while ( NULL != Tv_ObjSlave )
					{
						//check slave
						Tv_ObjFind	=	Tv_ObjSlave->RtlFindObjByPos( A_X, A_Y, A_IsInpAble );
						if ( NULL != Tv_ObjFind )
						{
							Tv_Result	=	Tv_ObjFind;
							break;
						}
						//next
						Tv_ObjSlave	=	Tv_ObjSlave->NextObjGet();
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Cls_GrUiDoBase*	Cls_GrUiDoBase::FindObjByPos( __s32 A_X, __s32 A_Y, BOOL8 A_IsInputAble )
{
	// local -------------------
		Cls_GrUiDoBase*	Tv_Result;
		St_GrRect	Tv_RcScr;
		Cls_GrUiDoBase*	Tv_ObjSlave;
		Cls_GrUiDoBase*	Tv_ObjFind;
	// code --------------------
		//init
		Tv_Result		=	NULL;

		//check visible
		if ( (!A_IsInputAble) || (IsCanVisible() && IsCanEnable()) )
		{
			//get rect
			if ( ScrRectGet( &Tv_RcScr ) )
			{
				//check able 
				if ( (A_X >= Tv_RcScr.left) && (A_X < Tv_RcScr.right) && (A_Y >= Tv_RcScr.top) && (A_Y < Tv_RcScr.bottom) )
				{
					//set result
					if ( (!A_IsInputAble) || (A_IsInputAble && m_IsFocusAble ) )
					{
						Tv_Result		=	this;
					}
					//check slave
					Tv_ObjSlave	=	m_ObjSlave;
					while ( NULL != Tv_ObjSlave )
					{
						//check slave
						Tv_ObjFind	=	Tv_ObjSlave->RtlFindObjByPos( A_X, A_Y, A_IsInputAble );
						if ( NULL != Tv_ObjFind )
						{
							Tv_Result	=	Tv_ObjFind;
							break;
						}
						//next
						Tv_ObjSlave	=	Tv_ObjSlave->NextObjGet();
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoBase::IsFocused( void )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//check focused
		if ( this == m_ObjConsole->FocusObjGet() )
		{
			Tv_Result		=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Cls_GrUiDoBase*	Cls_GrUiDoBase::FindSlaveObjByObjId( __u16 A_ObjId )
{
	// local -------------------
		Cls_GrUiDoBase*	Tv_Result;
		Cls_GrUiDoBase*	Tv_ObjSlave;
	// code --------------------
		//init
		Tv_Result	=	NULL;

		Tv_ObjSlave	=	m_ObjSlave;
		while ( NULL != Tv_ObjSlave )
		{
			//check skin id
			if ( A_ObjId == Tv_ObjSlave->ObjIdGet() )
			{
				//finded
				Tv_Result		=	Tv_ObjSlave;
				break;
			}
			else 
			{
				//find slave
				Tv_Result		=	Tv_ObjSlave->FindSlaveObjByObjId( A_ObjId );
				if ( NULL != Tv_Result )
				{
					break;
				}
			}
			//next
			Tv_ObjSlave	=	Tv_ObjSlave->NextObjGet();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoBase::FocusMapGet( void )
{
		return	m_FocusMoveMap;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBase::FocusMapSet( __u8 A_Map )
{
		m_FocusMoveMap	=	A_Map;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBase::RtlFindFocusableSlave( Ptr_GrUiDoFindFocusAble A_PtrEnv )
{
	// local -------------------
		Cls_GrUiDoBase*	Tv_ObjSlave;
		St_GrRect		Tv_Rc;
	// code --------------------
		Tv_ObjSlave	=	m_ObjSlave;
		while ( NULL != Tv_ObjSlave )
		{
			//check view able
			if ( Tv_ObjSlave->ScrRectGet( &Tv_Rc ) )
			{
				//check focusable
				if ( Tv_ObjSlave->IsCanFocus() )
				{
					//check able
					if ( (A_PtrEnv->y > Tv_Rc.top) || ((A_PtrEnv->y == Tv_Rc.top) && (A_PtrEnv->x > Tv_Rc.left) ) )
					{
						//update object
						A_PtrEnv->Obj	=	(void*)Tv_ObjSlave;
						A_PtrEnv->x		=	Tv_Rc.left;
						A_PtrEnv->y		=	Tv_Rc.top;
					}
				}
				else
				{
					//slave check
					Tv_ObjSlave->RtlFindFocusableSlave( A_PtrEnv );
				}
			}
			//next
			Tv_ObjSlave	=	Tv_ObjSlave->NextObjGet();
		}
}
//--------------------------------------------------------------------
Cls_GrUiDoBase*	Cls_GrUiDoBase::FindFocusableSlave( void )
{
	// local -------------------
		St_GrUiDoFindFocusAble	Tv_Env;
	// code --------------------
		//init
		Tv_Env.x		=	0x7FFFFFFF;
		Tv_Env.y		=	0x7FFFFFFF;
		Tv_Env.Obj	=	NULL;
		RtlFindFocusableSlave( &Tv_Env );
		return	(Cls_GrUiDoBase*)Tv_Env.Obj;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoBase::IsMouseBtnPushed( __u8 A_Button )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_BtnMap;
	// code --------------------
		Tv_Result	=	FALSE;
		Tv_BtnMap	=	V_GrUiMngObj->MouseBtnMapGet();
		if ( 0 != (Tv_BtnMap & (1 << A_Button)) )
		{
			Tv_Result		=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
/*
void	Cls_GrUiDoBase::MouseCusImgIdxSet( __u32 A_ImgIdx )
{
		m_MseCus	=	A_ImgIdx;
}
*/
//--------------------------------------------------------------------
/*
__u32	Cls_GrUiDoBase::MouseCusImgIdxGet( void )
{
		return	m_MseCus;
}
*/
//--------------------------------------------------------------------
__u8	Cls_GrUiDoBase::MouseBtnMapGet( void )
{
		return	V_GrUiMngObj->MouseBtnMapGet();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoBase::IsExistSlave( Cls_GrUiDoBase* A_Obj )
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrUiDoBase*	Tv_Obj;
	// code --------------------
		//init
		Tv_Result	=	FALSE;
		//check 
		Tv_Obj	=	m_ObjSlave;
		while ( NULL != Tv_Obj )
		{
			//check match
			if ( A_Obj == Tv_Obj )
			{
				//finded
				Tv_Result	=	TRUE;
				break;
			}
			else
			{
				//find slave
				if ( Tv_Obj->IsExistSlave( A_Obj ) )
				{
					Tv_Result	=	TRUE;
					break;
				}
			}
			//next
			Tv_Obj	=	Tv_Obj->NextObjGet();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoBase::IsOnMouse( void )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result	=	FALSE;

		if ( this == m_ObjConsole->MouseOnObjGet() )
		{
			Tv_Result		=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoBase::PaintClipperSet( BOOL8 A_IsCli, Ptr_GrRect A_PtrRect )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrRect	Tv_RcClip;
		Ptr_GrRect	Tv_PtrRcClip;
	// code --------------------
		//init
		Tv_Result	=	FALSE;

		//check client mode
		if ( m_IsPntCliAble || !A_IsCli )
		{
			//init
			Tv_Result	=	TRUE;
			//check base rect
			Tv_PtrRcClip	=	&m_PntRcClip;
			if ( A_IsCli )
			{
				Tv_PtrRcClip	=	&m_PntRcCliClip;
			}
			//partial mode
			if ( NULL != A_PtrRect )
			{
				Tv_Result	=	GrDrawCnGetComRect( &Tv_RcClip, Tv_PtrRcClip, A_PtrRect );
				Tv_PtrRcClip	=	&Tv_RcClip;
			}
			//set clipper
			if ( Tv_Result )
			{
				m_GdibScr->ClipperSet( Tv_PtrRcClip );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBase::ClientMarginSet( Ptr_GrByteRect A_PtrMgn )
{
		m_CliMgn	=	*A_PtrMgn;		
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBase::ClientMarginGet( Ptr_GrByteRect A_PtrMgn )
{
		*A_PtrMgn		=	m_CliMgn;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBase::RtlObjIdSet( __u16 A_ObjId )
{
		m_ObjConsole->ObjIdFree( m_ObjId );			//user allocated object id free
		m_ObjId	=	A_ObjId;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoBase::RosTextDraw(	__u32 A_TextId, Ptr_GrUiDoTextPara A_PtrPara, __u32 A_ClrTxt, 
																		Ptr_GrRect A_PtrRcScr, BOOL8 A_IsCursor, __s32 A_CursorPos )
{
	// local -------------------
		BOOL8		Tv_Result;
		WCHAR*	Tv_Str;
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		//change font
		if ( E_GrUiTxtIdNone != A_TextId )
		{
			Tv_Str	=	V_GrUiMngObj->TextPtrGet( A_TextId );
			if ( NULL != Tv_Str )
			{
				Tv_Result	=	TextDraw( Tv_Str, A_PtrPara, A_ClrTxt, A_PtrRcScr, A_IsCursor, A_CursorPos );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBase::ProcUiMsg( Ptr_GrUiMsgHd A_PtrMsg )
{
	// local -------------------
		BOOL8	Tv_IsRun;
		Ptr_GrUiMsgKey		Tv_PtrMsgKey;
		Ptr_GrUiMsgMouse	Tv_PtrMsgMse;
		Ptr_GrUiMsgWheel	Tv_PtrMsgWhl;
		Ptr_GrUiMsgTimeTick	Tv_PtrMsgTick;
		Ptr_GrUiMsgVkFinish	Tv_PtrMsgVkbd;
		Ptr_GrUiMsgStatChg	Tv_PtrMsgStatChg;
		//Ptr_GrUiMsgMbxRslt	Tv_PtrMsgMbx;
	// code --------------------
		switch ( A_PtrMsg->Cmd )
		{
			case	E_GrUiMsgCmdKey:
				Tv_PtrMsgKey	=	(Ptr_GrUiMsgKey)A_PtrMsg;
				if ( Tv_PtrMsgKey->IsPush )
				{
					Tv_IsRun	=	ProcEvtKeyDown( Tv_PtrMsgKey->Key );
				}
				else
				{
					Tv_IsRun	=	ProcEvtKeyUp( Tv_PtrMsgKey->Key );
				}
				//send not use message
				if ( !Tv_IsRun )
				{
					UiMsgToWin( A_PtrMsg );
				}
				break;
			case	E_GrUiMsgCmdMouse:
				Tv_IsRun		=	FALSE;
				Tv_PtrMsgMse	=	(Ptr_GrUiMsgMouse)A_PtrMsg;
				if ( 0 != (E_GrUiMouseAtbPosChg & Tv_PtrMsgMse->Atb) )
				{
					//mouse move
					Tv_IsRun	=	ProcEvtMseMove( Tv_PtrMsgMse->X, Tv_PtrMsgMse->Y, Tv_PtrMsgMse->BtnMap );
				}
				else if ( 0 != (E_GrUiMouseAtbLbtnChg & Tv_PtrMsgMse->Atb) )
				{
					if ( 0 != ( (1 << E_GrUiMouseBtnIdxLeft) & Tv_PtrMsgMse->BtnMap )  )
					{
						Tv_IsRun	=	ProcEvtMseLbtnDown( Tv_PtrMsgMse->X, Tv_PtrMsgMse->Y, Tv_PtrMsgMse->BtnMap );
					}
					else
					{
						Tv_IsRun	=	ProcEvtMseLbtnUp( Tv_PtrMsgMse->X, Tv_PtrMsgMse->Y, Tv_PtrMsgMse->BtnMap );
					}
				}
				else if ( 0 != (E_GrUiMouseAtbRbtnChg & Tv_PtrMsgMse->Atb) )
				{
					if ( 0 != ( (1 << E_GrUiMouseBtnIdxRight) & Tv_PtrMsgMse->BtnMap )  )
					{
						Tv_IsRun	=	ProcEvtMseRbtnDown( Tv_PtrMsgMse->X, Tv_PtrMsgMse->Y, Tv_PtrMsgMse->BtnMap );
					}
					else
					{
						Tv_IsRun	=	ProcEvtMseRbtnUp( Tv_PtrMsgMse->X, Tv_PtrMsgMse->Y, Tv_PtrMsgMse->BtnMap );
					}
				}
				else if ( 0 != (E_GrUiMouseAtbMbtnChg & Tv_PtrMsgMse->Atb) )
				{
					if ( 0 != ( (1 << E_GrUiMouseBtnIdxMiddle) & Tv_PtrMsgMse->BtnMap )  )
					{
						Tv_IsRun	=	ProcEvtMseMbtnDown( Tv_PtrMsgMse->X, Tv_PtrMsgMse->Y, Tv_PtrMsgMse->BtnMap );
					}
					else
					{
						Tv_IsRun	=	ProcEvtMseMbtnUp( Tv_PtrMsgMse->X, Tv_PtrMsgMse->Y, Tv_PtrMsgMse->BtnMap );
					}
				}
				//send not use message
				if ( !Tv_IsRun )
				{
					UiMsgToWin( A_PtrMsg );
				}
				break;
			case	E_GrUiMsgCmdWeel:
				Tv_IsRun			=	FALSE;
				Tv_PtrMsgWhl	=	(Ptr_GrUiMsgWheel)A_PtrMsg;
				if ( Tv_PtrMsgWhl->IsDown )
				{
					Tv_IsRun	=	ProcEvtWheelDown();
				}
				else
				{
					Tv_IsRun	=	ProcEvtWheelUp();
				}
				//send not use message
				if ( !Tv_IsRun )
				{
					UiMsgToWin( A_PtrMsg );
				}
				break;
			case	E_GrUiMsgCmdFocusGet:
				ProcEvtFocusGet();
				break;
			case	E_GrUiMsgCmdFocusLoss:
				ProcEvtFocusLoss();
				break;
			case	E_GrUiMsgCmdMouseEnter:
				ProcEvtMseEnter();
				break;
			case	E_GrUiMsgCmdMouseLeave:
				ProcEvtMseLeave();
				break;
			case	E_GrUiMsgCmdTickPeriod:
				Tv_PtrMsgTick	=	(Ptr_GrUiMsgTimeTick)A_PtrMsg;
				ProcEvtTimeTick( Tv_PtrMsgTick->Tick );
				break;
			case	E_GrUiMsgCmdStatChg:
				Tv_PtrMsgStatChg	=	(Ptr_GrUiMsgStatChg)A_PtrMsg;
				ProcEvtGlbStatChg( Tv_PtrMsgStatChg->Type );
				break;
			case	E_GrUiMsgCmdVkFinish:
				Tv_PtrMsgVkbd	=	(Ptr_GrUiMsgVkFinish)A_PtrMsg;
				ProcEvtVkbd( Tv_PtrMsgVkbd->VkId );
				break;
			case	E_GrUiMsgCmdRelate:
				ProcEvtRelate( ((Ptr_GrUiMsgObjRel)A_PtrMsg)->Para );
				break;
			case	E_GrUiMsgCmdScptDecCpl:
				ProcEvtScptDecoded();
				break;
		}

}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoBase::RtlUiMsgProc( Ptr_GrUiMsgHd A_PtrMsg )
{
	// local -------------------	
		BOOL8	Tv_Result;
		Cls_GrUiDoBase*	Tv_ObjWk;
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		//check this event
		if ( (m_ObjId == A_PtrMsg->ObjId) || (E_GrUiObjIdBroadcast == A_PtrMsg->ObjId) )
		{
			ProcUiMsg( A_PtrMsg );
		}
		//check this object event
		if ( m_ObjId == A_PtrMsg->ObjId )
		{
			Tv_Result	=	TRUE;
		}
		else
		{
			Tv_ObjWk	=	m_ObjSlave;
			while ( NULL != Tv_ObjWk )
			{
				//message process
				Tv_Result	=	Tv_ObjWk->RtlUiMsgProc( A_PtrMsg );
				if ( Tv_Result )
				{
					break;
				}
				//next
				Tv_ObjWk	=	Tv_ObjWk->NextObjGet();
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoBase::UiMsgSend( Ptr_GrUiMsgHd A_PtrMsg )
{
		//check parent
		if ( (E_GrUiObjIdParent == A_PtrMsg->ObjId) && (NULL != m_ObjPrnt) )
		{
			A_PtrMsg->ObjId		=	m_ObjPrnt->ObjIdGet();
		}
		return	m_ObjConsole->UiMsgSend( A_PtrMsg );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoBase::UiMsgPost( Ptr_GrUiMsgHd A_PtrMsg, __u32 A_Size )
{
		//check parent
		if ( (E_GrUiObjIdParent == A_PtrMsg->ObjId) && (NULL != m_ObjPrnt) )
		{
			A_PtrMsg->ObjId		=	m_ObjPrnt->ObjIdGet();
		}
		return	m_ObjConsole->UiMsgPost( A_PtrMsg, A_Size );
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBase::UiMsgToWin( Ptr_GrUiMsgHd A_PtrMsg )
{
	// local -------------------
	// code --------------------
		if ( !IsWindow() )
		{
			A_PtrMsg->WinId		=	((Cls_GrUiDoWin*)m_ObjWin)->WinIdGet();
			A_PtrMsg->ObjId		=	E_GrUiObjIdNone;
			((Cls_GrUiDoWin*)m_ObjWin)->ProcUiMsg( A_PtrMsg );
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoBase::ProcEvtKeyDown( __u8 A_Key )
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoBase::ProcEvtKeyUp( __u8 A_Key )
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoBase::ProcEvtMseMove( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoBase::ProcEvtMseLbtnDown( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( m_IsFocusAble )
		{
			m_ObjConsole->FocusObjSet( this );
			Tv_Result	=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoBase::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoBase::ProcEvtMseRbtnDown( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoBase::ProcEvtMseRbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoBase::ProcEvtMseMbtnDown( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoBase::ProcEvtMseMbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoBase::ProcEvtWheelUp( void )
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoBase::ProcEvtWheelDown( void )
{
		return	FALSE;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBase::ProcEvtFocusGet( void )
{
		if ( IsCanFocus() )
		{
			((Cls_GrUiDoWin*)m_ObjWin)->RtlLastFocusObjSet( this );
		}
		ReqDraw();
		if ( m_IsFocusAble )
		{
			LcRelFcsLblUpdt();
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBase::ProcEvtFocusLoss( void )
{
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBase::ProcEvtGlbStatChg( __u8 A_Type )
{
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBase::ProcEvtMseEnter( void )
{
		if ( m_IsFocusAble )
		{
			ReqDraw();
			LcRelFcsLblUpdt();
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBase::ProcEvtMseLeave( void )
{
		if ( m_IsFocusAble )
		{
			ReqDraw();
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBase::ProcEvtTimeTick( __u32 A_Tick )
{
}
//--------------------------------------------------------------------
Cls_GrUiDoBase*	Cls_GrUiDoBase::FindSlaveObjByType( __u32 A_TypeId )
{
	// local -------------------
		Cls_GrUiDoBase*	Tv_Result;
		Cls_GrUiDoBase*	Tv_ObjSlave;
	// code --------------------
		//init
		Tv_Result	=	NULL;

		Tv_ObjSlave	=	m_ObjSlave;
		while ( NULL != Tv_ObjSlave )
		{
			//check skin id
			if ( A_TypeId == Tv_ObjSlave->TypeIdGet() )
			{
				//finded
				Tv_Result		=	Tv_ObjSlave;
				break;
			}
			else 
			{
				//find slave
				Tv_Result		=	Tv_ObjSlave->FindSlaveObjByType( A_TypeId );
				if ( NULL != Tv_Result )
				{
					break;
				}
			}
			//next
			Tv_ObjSlave	=	Tv_ObjSlave->NextObjGet();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoBase::PalExtPosyGet( void )
{
		return	m_PalExtPosY;
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoBase::PalExtSizeyGet( void )
{
		return	m_PalExtSizeY;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBase::PalExtPosySet( __u8 A_Val )
{
		if ( IsVisible() )
		{
			//redraw
			ReqDraw();
			//change position
			m_PalExtPosY		=	A_Val;
			//redraw
			ReqDraw();
		}
		else
		{
			//change position
			m_PalExtPosY		=	A_Val;
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBase::PalExtSizeySet( __u8 A_Val )
{
	if ( IsVisible() )
	{
		//redraw
		ReqDraw();
		//change position
		m_PalExtSizeY		=	A_Val;
		//redraw
		ReqDraw();
	}
	else
	{
		//change position
		m_PalExtSizeY		=	A_Val;
	}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoBase::RtlScrRectCal( Ptr_GrRect A_PtrRect )
{
	// local -------------------
		BOOL8		Tv_Result;
		__s32			Tv_StY;
	// code --------------------
		Tv_Result		=	FALSE;

		//check visiable
		if ( m_IsVisible )
		{
			//set
			Tv_StY			=	m_Rect.top;
			//osd expantion patch
			if ( V_IsGrUiMngPalMode )
			{
				Tv_StY		=	Tv_StY + (__s32)m_PalExtPosY;
			}
			A_PtrRect->left		=	A_PtrRect->left + m_Rect.left;
			A_PtrRect->top		=	A_PtrRect->top + Tv_StY;
			A_PtrRect->right	=	A_PtrRect->right + m_Rect.left;
			A_PtrRect->bottom	=	A_PtrRect->bottom + Tv_StY;

			Tv_Result		=	TRUE;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBase::ProcEvtVkbd( __u32 A_VkId	)
{
		ReqDraw();
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoBase::MsgBox( WCHAR* A_StrTitle, WCHAR* A_StrContents, __u8 A_Type )
{
	// local -------------------
		__u32		Tv_Result;
		__u16		Tv_ScptId;
		__u8		Tv_Layer;
		St_GrRect	Tv_Rect;
		BOOL8		Tv_IsSlave;
	// code --------------------
		Tv_Result		=	0;
		Tv_ScptId		=	E_GrUiObjIdWinMsgBoxType0 + (__u16)A_Type;
		if ( E_GrUiObjIdWinMsgBoxType0 > Tv_ScptId )
		{
			Tv_ScptId		=	E_GrUiObjIdWinMsgBoxType0;
		}
		if ( E_GrUiObjIdWinMsgBoxType2 < Tv_ScptId )
		{
			Tv_ScptId		=	E_GrUiObjIdWinMsgBoxType2;
		}

		if ( V_GrUiMngObj->ScptAtbGet( Tv_ScptId, &Tv_Layer, &Tv_Rect, &Tv_IsSlave ) )
		{
			//disable this window
			m_ObjWin->EnableSet( FALSE );
			//do message box
			Tv_Result	=	m_ObjConsole->UiMessageBox( Tv_ScptId, A_StrTitle, A_StrContents, Tv_Layer );
			//enable this window
			m_ObjWin->EnableSet( TRUE );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoBase::MsgBox( __u32 A_TitleId, __u32 A_ContentsId, __u8 A_Type )
{
	// local -------------------
		__u32		Tv_Result;
		__u16		Tv_ScptId;
		__u8		Tv_Layer;
		St_GrRect	Tv_Rect;
		BOOL8		Tv_IsSlave;
		WCHAR*	Tv_StrTitle;
		WCHAR*	Tv_StrConts;
	// code --------------------
		Tv_Result		=	0;
		Tv_ScptId		=	E_GrUiObjIdWinMsgBoxType0 + (__u16)A_Type;
		if ( E_GrUiObjIdWinMsgBoxType0 > Tv_ScptId )
		{
			Tv_ScptId		=	E_GrUiObjIdWinMsgBoxType0;
		}
		if ( E_GrUiObjIdWinMsgBoxType2 < Tv_ScptId )
		{
			Tv_ScptId		=	E_GrUiObjIdWinMsgBoxType2;
		}

		if ( V_GrUiMngObj->ScptAtbGet( Tv_ScptId, &Tv_Layer, &Tv_Rect, &Tv_IsSlave ) )
		{
			//disable this window
			m_ObjWin->EnableSet( FALSE );
			//do message box
			Tv_StrTitle	=	V_GrUiMngObj->TextPtrGet( A_TitleId );
			Tv_StrConts	=	V_GrUiMngObj->TextPtrGet( A_ContentsId );
			Tv_Result	=	m_ObjConsole->UiMessageBox( Tv_ScptId, Tv_StrTitle, Tv_StrConts, Tv_Layer );
			//enable this window
			m_ObjWin->EnableSet( TRUE );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoBase::Popup( WCHAR* A_StrTitle, WCHAR* A_StrContents )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u8		Tv_Layer;
		St_GrRect	Tv_Rect;
		BOOL8		Tv_IsSlave;
		Cls_GrUiDoMsgbox*	Tv_MsgBox;
	// code --------------------
		Tv_Result		=	FALSE;
		if ( V_GrUiMngObj->ScptAtbGet( E_GrUiObjIdWinMsgBoxType0, &Tv_Layer, &Tv_Rect, &Tv_IsSlave ) )
		{
			//create message box
			Tv_MsgBox	=	(Cls_GrUiDoMsgbox*)new Cls_GrUiDoMsgbox(	E_GrUiObjIdWinMsgBoxType0, A_StrTitle, 
				A_StrContents );
			Tv_Result	=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoBase::Popup( __u32 A_TitleId, __u32 A_ContentsId )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u8		Tv_Layer;
		St_GrRect	Tv_Rect;
		BOOL8		Tv_IsSlave;
		Cls_GrUiDoMsgbox*	Tv_MsgBox;
	// code --------------------
		Tv_Result		=	FALSE;
		if ( V_GrUiMngObj->ScptAtbGet( E_GrUiObjIdWinMsgBoxType0, &Tv_Layer, &Tv_Rect, &Tv_IsSlave ) )
		{
			//create message box
			Tv_MsgBox	=	(Cls_GrUiDoMsgbox*)new Cls_GrUiDoMsgbox(	E_GrUiObjIdWinMsgBoxType0, 
				V_GrUiMngObj->TextPtrGet( A_TitleId ), V_GrUiMngObj->TextPtrGet( A_ContentsId ) );
			Tv_Result	=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoBase::VirtKbdCreate( BOOL8 A_IsNum, BOOL8 A_IsPasswd, __u32 A_VkId, WCHAR* A_Str, __u32 A_MaxLen, BOOL8 A_IsPassChk, BOOL8 A_IsLangLock )
{
	// local -------------------
		__u16		Tv_ObjId;
	// code --------------------
		Tv_ObjId		=	m_ObjId;
		if ( IsWindow() )
		{
			Tv_ObjId	=	E_GrUiObjIdNone;
		}
		return	m_ObjConsole->VirtKbdCreate( ((Cls_GrUiDoWin*)m_ObjWin)->WinIdGet(), Tv_ObjId, A_IsNum, A_IsPasswd, A_VkId, A_Str, A_MaxLen, A_IsPassChk, A_IsLangLock );
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBase::ProcEvtRelate( __u32 A_Para )
{
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBase::ProcEvtScptDecoded( void )
{
	// local -------------------
		Cls_GrUiDoBase*	Tv_ObjPrnt;
		Cls_GrUiDoCdvArea*	Tv_CdvArea;
		__u8	Tv_SlotIdx;
		__u8	Tv_IsSucs;
		__u8	Tv_CndIdx;
		__u32	Tv_Mask;
		BOOL8	Tv_IsVisible;
	// code --------------------
		//check already decode
		if ( !m_IsCondDeced )
		{
			//check need check
			if ( 0 != m_CondVisMap[0] )
			{
				Tv_CdvArea	=	NULL;
				Tv_ObjPrnt	=	m_ObjPrnt;
				while ( NULL != Tv_ObjPrnt )
				{
					if ( E_GrUiDoTypeIdCdvArea == Tv_ObjPrnt->TypeIdGet() )
					{
						//finded
						Tv_CdvArea	=	(Cls_GrUiDoCdvArea*)Tv_ObjPrnt;
						break;
					}
					//next
					Tv_ObjPrnt	=	Tv_ObjPrnt->ParentObjGet();
				}
				//check finded
				if ( NULL != Tv_CdvArea )
				{
					//check visible
					Tv_IsVisible	=	TRUE;
					for ( Tv_SlotIdx=0;E_GrUiDoCondSlotCnt > Tv_SlotIdx;Tv_SlotIdx++ )
					{
						Tv_IsSucs	=	TRUE;
						if ( 0 != m_CondVisMap[Tv_SlotIdx] )
						{
							Tv_IsSucs	=	FALSE;
							Tv_Mask		=	1;
							for ( Tv_CndIdx=0;Tv_CndIdx < E_GrUiDoCdvAreaMaxCondCnt;Tv_CndIdx++ )
							{
								if ( 0 != (m_CondVisMap[Tv_SlotIdx] & Tv_Mask) )
								{
									if ( Tv_CdvArea->IsConditionOk(Tv_CndIdx) )
									{
										Tv_IsSucs	=	TRUE;
										break;
									}
								}
								Tv_Mask	=	Tv_Mask << 1;
							}
						}
						//check success
						if ( !Tv_IsSucs )
						{
							Tv_IsVisible	=	FALSE;
							break;
						}
					}
					//update visible
					VisibleSet( Tv_IsVisible );
				}
			}

			//mark codition processed
			m_IsCondDeced	=	TRUE;
		}
}
//--------------------------------------------------------------------
__u16	Cls_GrUiDoBase::RelFocusLabelIdGet( void )
{
		return	m_RelFcsLblId;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBase::RelFocusLabelIdSet( __u16 A_ObjId )
{
		m_RelFcsLblId	=	A_ObjId;
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoBase::RelFocusTextIdGet( void )
{
		return	m_RelFcsTxtId;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBase::RelFocusTextIdSet( __u32 A_TxtId )
{
		m_RelFcsTxtId	=	A_TxtId;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBase::LcRelFcsLblUpdt( void )
{
	// local -------------------
		Cls_GrUiDoLbl*	Tv_Lbl;
	// code --------------------
		if ( E_GrUiObjIdNone != m_RelFcsLblId )
		{
			Tv_Lbl	=	(Cls_GrUiDoLbl*)m_ObjWin->FindSlaveObjByObjId( m_RelFcsLblId );
			if ( NULL != Tv_Lbl )
			{
				//check type
				if ( E_GrUiDoTypeIdLabel == Tv_Lbl->TypeIdGet() )
				{
					Tv_Lbl->TextIdSet( m_RelFcsTxtId );
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBase::FocusBlockSet( BOOL8 A_IsBlock )
{
		m_IsFocusBlock	=	A_IsBlock;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoBase::IsFocusBlock( void )
{
		return	m_IsFocusBlock;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBase::FocusBaseIdSet( __u16 A_ObjId )
{
		m_FocusBaseId	=	A_ObjId;
}
//--------------------------------------------------------------------
__u16	Cls_GrUiDoBase::FocusBaseIdGet( void )
{
		return	m_FocusBaseId;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBase::CondVisibleMapSet( __u8 A_Idx, __u32 A_Map )
{
		if ( E_GrUiDoCondSlotCnt > A_Idx )
		{
			m_CondVisMap[A_Idx]	=	A_Map;
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoBase::CondVisibleMapGet( __u8 A_Idx )
{
	// local -------------------
		__u32	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		if ( E_GrUiDoCondSlotCnt > A_Idx )
		{
			Tv_Result	=	m_CondVisMap[A_Idx];
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------


