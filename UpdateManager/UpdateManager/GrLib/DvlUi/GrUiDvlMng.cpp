/*
 osd develop manager

 */

//====================================================================
// uses
#include	<DvlUi/GrUiDvlMng.h>
#include	<Ui/GrUiScript.h>
#include	<Ui/GrUiBase.h>
#include	<GrStrTool.h>
#include	<Ui/GrUiDoSkin.h>
#include	<GrDumyTool.h>
#include	<Ui/GrUiDoWin.h>
#include	<Ui/GrUiDoFrm.h>
#include	<Ui/GrUiDoListbox.h>
#include	<Ui/GrUiDoBtn.h>
#include	<Ui/GrUiDoLbl.h>
#include	<Ui/GrUiScptDec.h>
#include	<GrFileCtrl.h>
#include	<Ui/GrUiScptDec.h>
#include	<GrGdibTool.h>
#include	<GrFileCtrl.h>
#include	<GrFileTool.h>
#include	<time.h>
#include	<DvlUi/GrUiDvlPara.h>
#include	<stdlib.h>
#include	<GrTimeTool.h>

//====================================================================
//local const

#define E_GrUiDvlMngNewWinName			L"WinNew"
#define E_GrUiDvlMngNewWinDfltResX	320
#define E_GrUiDvlMngNewWinDfltResY	240

#define E_GrUiDvlMngNewPageName			L"PageNew"

#define E_GrUiDvlMngTextDlim				L";"

//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var
extern	Cls_GrUiMng*		V_GrUiMngObj;

__s8	V_GrUiDvlMngStrHeaderStart[]	=	
{	
"//====================================================================\n"
"//      Gause resource ver 2 header\n"
"//====================================================================\n\n"
};

__u16	V_GrUiDvlQuadSizeX;
__u16	V_GrUiDvlQuadSizeY;
__u16	V_GrUiDvlQuadPalExt;

//====================================================================
//--------------------------------------------------------------------
Cls_GrUiDvlMng::Cls_GrUiDvlMng( Cls_GrTaskSvr* A_TaskSvr, __u32 A_FccEdit, __u32 A_ResX, __u32 A_ResY, Fnc_GrUiDvlPaintTrs A_FncPntTrs )
{
		//init
		m_FccEdit		=	A_FccEdit;
		m_EdtResX		=	A_ResX;
		m_EdtResY		=	A_ResY;
		m_TaskSvr		=	A_TaskSvr;

		m_GdibCon		=	NULL;
		m_UiCon			=	NULL;

		m_PtrCopy		=	NULL;

		m_IsPainted	=	FALSE;
		m_FncPntTrs	=	A_FncPntTrs;
		m_SkinIdx		=	0;

		GrDumyZeroMem( m_ObjIdAlocTbl, sizeof(m_ObjIdAlocTbl) );
		GrDumyZeroMem( m_ObjNameTbl, sizeof(m_ObjNameTbl) );

		GrDumyZeroMem( (void*)m_LumpTbl, sizeof(m_LumpTbl) );
		GrDumyZeroMem( (void*)m_LumpExtTbl, sizeof(m_LumpExtTbl) );
		GrDumyZeroMem( (void*)m_LumpFoldTbl, sizeof(m_LumpFoldTbl) );
		GrDumyZeroMem( (void*)m_FontTbl, sizeof(m_FontTbl) );
		GrDumyZeroMem( (void*)m_SkinTbl, sizeof(m_SkinTbl) );
		GrDumyZeroMem( (void*)m_LangTbl, sizeof(m_LangTbl) );
		GrDumyZeroMem( (void*)m_MacTbl, sizeof(m_MacTbl) );
		GrDumyZeroMem( (void*)m_MacExtTbl, sizeof(m_MacExtTbl) );
		GrDumyZeroMem( (void*)m_PfoldTbl, sizeof(m_PfoldTbl) );

		//make manager
		GrUiMngInit( m_TaskSvr );
		m_UiMng		=	V_GrUiMngObj;

		//make script encoder
		m_ScptEnc	=	(Cls_GrUiScptEnc*) new Cls_GrUiScptEnc();

		//make screen 
		m_GdibScr	=	GrGdibCreate( m_FccEdit, m_EdtResX, m_EdtResY );
		//make screen bitmap header
#ifndef LINUX_APP
		GrDumyZeroMem( &m_BhScr, sizeof(m_BhScr) );
		m_BhScr.biSize		=	sizeof(m_BhScr);
		m_BhScr.biWidth		=	(__s32)m_EdtResX;
		m_BhScr.biHeight	=	-((__s32)m_EdtResY);
		m_BhScr.biPlanes	=	1;
		m_BhScr.biSizeImage	=	m_GdibScr->GetDibSize();
		m_BhScr.biCompression	=	BI_RGB;
		m_BhScr.biBitCount		=	GrDrawGetPixelBitByFcc( m_FccEdit );
		if ( 15 == m_BhScr.biBitCount )
		{
			m_BhScr.biBitCount	=	16;
		}
#endif

		//make develop console

}
//--------------------------------------------------------------------
Cls_GrUiDvlMng::~Cls_GrUiDvlMng( void )
{
		//release edit object
		LcPrjObjFree();					
		//object free
		LcLumpAllFree();
		//macro free
		LcMacAllFree();
		//font free
		LcFontAllFree();
		//image free
		LcImgAllFree();
		//language free
		LcTxtAllFree();

		//copy object free
		if ( NULL != m_PtrCopy )
		{
			free( m_PtrCopy );
		}
		
		//release screen
		delete	m_GdibScr;
		//scripte encoder release
		delete	m_ScptEnc;
		//release object
		GrUiMngRelease();
}
//--------------------------------------------------------------------
void	Cls_GrUiDvlMng::LcPrjObjFree( void )
{
	// local -------------------
	// code --------------------
		//check exist console
		if ( NULL != m_UiCon )
		{
			//unregieter console
			m_UiMng->ConsoleUnregist( m_UiCon );
			//release console
			delete	m_UiCon;
			m_UiCon		=	NULL;
		}
		//release console gdib
		if ( NULL != m_GdibCon )
		{
			//check screen same
			if ( m_GdibCon != m_GdibScr )
			{
				delete	m_GdibCon;
			}
			m_GdibCon		=	NULL;
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDvlMng::NewPrj( __u32 A_Fcc, BOOL8 A_IsPltMode )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u32		Tv_PltCnt;
	// code --------------------
		//init
		Tv_Result	=	FALSE;
		m_DvlScptHd.VerUser		=	0;
		V_GrUiDvlQuadSizeX		=	704;
		V_GrUiDvlQuadSizeY		=	480;
		V_GrUiDvlQuadPalExt		=	96;

		//release obj
		LcPrjObjFree();

		//reset setting
		LcScptReset();
		m_UiMng->ScptSet( (void*)&m_DvlScptHd );
		m_UiMng->GlbVarSet( 0, 0 );
		m_UiMng->GlbVarSet( 1, 16 );		//16 channel video
		m_UiMng->GlbVarSet( 2, 4 );			//4 channel audio
		//reset project
		GrDumyZeroMem( (void*)m_PfoldTbl, sizeof(m_PfoldTbl) );
		
		//update develop manager
		GrUiDvlOsdFccSet( A_Fcc, A_IsPltMode );

		//calculate pallete count
		Tv_PltCnt	=	0;
		if ( A_IsPltMode )
		{
			Tv_PltCnt	=	8;
		}

		//create gdib
		if ( (m_FccEdit == A_Fcc) && (0 == Tv_PltCnt) )
		{
			m_GdibCon		=	m_GdibScr;
		}
		else
		{
			m_GdibCon		=	GrGdibCreate( A_Fcc, m_EdtResX, m_EdtResY, E_GrGdibPitchAuto, NULL,Tv_PltCnt, (void*)E_GrGdibPltBase );
			m_GdibCon->FillRect( NULL, 0 );
			m_IsPainted	=	TRUE;
		}
		if ( NULL != m_GdibCon )
		{
			//create console
			m_UiCon	=	(Cls_GrUiDvlConsole*)new Cls_GrUiDvlConsole( m_GdibCon, (void*)this, m_FncPntTrs );
			m_UiMng->ConsoleRegist( m_UiCon, 0 );
			//success
			Tv_Result		=	TRUE;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
Cls_GrGdib*	Cls_GrUiDvlMng::ScrObjGet( void )
{
		//check painted
		if ( m_IsPainted )
		{
			//check console exist
			if ( NULL != m_GdibCon )
			{
				//do convert
				if ( m_GdibCon != m_GdibScr )
				{
					//convert
					m_GdibCon->CvtTo( m_GdibScr );
					m_IsPainted	=	FALSE;
				}
			}
		}
		
		return	m_GdibScr;
}
//--------------------------------------------------------------------
#ifndef LINUX_APP
BITMAPINFO*	Cls_GrUiDvlMng::BitmapinfoGet( void )
{
		return	(BITMAPINFO*)&m_BhScr;
}
#endif
//--------------------------------------------------------------------
__u32	Cls_GrUiDvlMng::LumpSlotCount( void )
{
		return	E_GrUiDvlLumpMaxCnt;
}
//--------------------------------------------------------------------
Ptr_GrUiScptObjHd	Cls_GrUiDvlMng::LumpGetInfoPtr( __u32 A_Idx )
{
	// local -------------------
		Ptr_GrUiScptObjHd	Tv_Result;
	// code --------------------
		//init
		Tv_Result	=	NULL;

		//check bad index
		if ( E_GrUiDvlLumpMaxCnt > A_Idx )
		{
			//check exist
			if ( NULL != m_LumpTbl[A_Idx].Ptr )
			{
				Tv_Result	=	m_LumpTbl[A_Idx].Ptr;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Ptr_GrUiDvlLumpExt	Cls_GrUiDvlMng::LumpGetExtPtr( __u32 A_Idx )
{
	// local -------------------
		Ptr_GrUiDvlLumpExt	Tv_Result;
	// code --------------------
		//init
		Tv_Result	=	NULL;

		//check bad index
		if ( E_GrUiDvlLumpMaxCnt > A_Idx )
		{
			//check exist
			if ( NULL != m_LumpTbl[A_Idx].Ptr )
			{
				Tv_Result	=	&m_LumpExtTbl[A_Idx];
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Ptr_GrUiDvlLumpFold	Cls_GrUiDvlMng::LumpGetFoldPtr( __u32 A_Idx )
{
	// local -------------------
		Ptr_GrUiDvlLumpFold	Tv_Result;
	// code --------------------
		//init
		Tv_Result	=	NULL;

		//check bad index
		if ( E_GrUiDvlLumpMaxCnt > A_Idx )
		{
			//check exist
			if ( NULL != m_LumpTbl[A_Idx].Ptr )
			{
				Tv_Result	=	&m_LumpFoldTbl[A_Idx];
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Ptr_GrUiDvlPrjFoldInfo	Cls_GrUiDvlMng::PrjFoldGetPtr( __u32 A_Idx )
{
		return	&m_PfoldTbl[A_Idx-1];
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDvlMng::PrjFolderNew( WCHAR* A_StrName )
{
	// local -------------------
		__u32	Tv_Result;
		__u32	Tv_WkIdx;
	// code --------------------
		Tv_Result	=	0;

		//check able
		if ( GrStrIsExist(A_StrName) )
		{
			for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiDvlPrjFoldMaxCnt;Tv_WkIdx++ )
			{
				if ( !GrStrIsExist(m_PfoldTbl[Tv_WkIdx].StrName) )
				{
					m_PfoldTbl[Tv_WkIdx].IdPrnt	=	0;
					GrStrWcopy( m_PfoldTbl[Tv_WkIdx].StrName, A_StrName );
					Tv_Result	=	Tv_WkIdx + 1;
					break;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDvlMng::MacSlotCount( void )
{
		return	E_GrUiDvlMacMaxCnt;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDvlMng::MacSlotInfo( __u32 A_Idx, WCHAR* A_StrRtName )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		//init
		Tv_Result	=	FALSE;

		//check bad index
		if ( E_GrUiDvlMacMaxCnt > A_Idx )
		{
			//check exist
			if ( NULL != m_MacTbl[A_Idx].Ptr )
			{
				//get info
				GrStrWcopy( A_StrRtName, m_MacExtTbl[A_Idx].StrName );
				Tv_Result			=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDvlMng::LcFindEmptyLumpIdx( __u32* A_PtrRtIdx )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_WkIdx;
	// code --------------------
		//init
		Tv_Result	=	FALSE;
		//check
		for ( Tv_WkIdx = 0;Tv_WkIdx < E_GrUiDvlLumpMaxCnt;Tv_WkIdx++ )
		{
			//check exist item
			if ( NULL == m_LumpTbl[Tv_WkIdx].Ptr )
			{
				//finded
				*A_PtrRtIdx	=	Tv_WkIdx;
				Tv_Result	=	TRUE;
				break;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDvlMng::FindLumpIdxById( __u16 A_Id, __u32* A_PtrRtIdx )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_WkIdx;
	// code --------------------
		//init
		Tv_Result	=	FALSE;
		//check
		for ( Tv_WkIdx = 0;Tv_WkIdx < E_GrUiDvlLumpMaxCnt;Tv_WkIdx++ )
		{
			//check exist item
			if ( NULL != m_LumpTbl[Tv_WkIdx].Ptr )
			{
				//check match id
				if ( A_Id == m_LumpTbl[Tv_WkIdx].Ptr->ScptId )
				{
					//exist
					*A_PtrRtIdx	=	Tv_WkIdx;
					Tv_Result	=	TRUE;
					break;
				}
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDvlMng::IsObjIdExist( __u16 A_Id )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_ByteIdx;
		__u16	Tv_BitIdx;
		__u8	Tv_BitMask;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//check id range
		if ( (E_GrUiObjIdSysStart <= A_Id) && ((E_GrUiObjIdSysStart + E_GrUiDvlObjMaxCnt) > A_Id )  )
		{
			Tv_ByteIdx	=	(A_Id - E_GrUiObjIdSysStart) >> 3;
			Tv_BitIdx		=	(A_Id - E_GrUiObjIdSysStart) & 0x7;
			Tv_BitMask	=	1 << Tv_BitIdx;
			if ( 0 != (Tv_BitMask & m_ObjIdAlocTbl[Tv_ByteIdx]) )
			{
				Tv_Result	=	TRUE;
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDvlMng::ObjIdAlocSet( __u16 A_Id, BOOL8 A_IsSet )
{
	// local -------------------
		__u16	Tv_ByteIdx;
		__u16	Tv_BitIdx;
		__u8	Tv_BitMask;
	// code --------------------
		//check id range
		if ( (E_GrUiObjIdSysStart <= A_Id) && ((E_GrUiObjIdSysStart + E_GrUiDvlObjMaxCnt) > A_Id )  )
		{
			Tv_ByteIdx	=	(A_Id - E_GrUiObjIdSysStart) >> 3;
			Tv_BitIdx		=	(A_Id - E_GrUiObjIdSysStart) & 0x7;
			Tv_BitMask	=	1 << Tv_BitIdx;
			if ( A_IsSet )
			{
				m_ObjIdAlocTbl[Tv_ByteIdx]	=	m_ObjIdAlocTbl[Tv_ByteIdx] | Tv_BitMask;
			}
			else
			{
				m_ObjIdAlocTbl[Tv_ByteIdx]	=	m_ObjIdAlocTbl[Tv_ByteIdx] & ~Tv_BitMask;
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDvlMng::LcIsExistObjByName( WCHAR* A_StrName )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u16		Tv_ByteIdx;
		__u16		Tv_BitIdx;
		__u16		Tv_ObjIdx;
	// code --------------------
		Tv_Result		=	FALSE;
		for ( Tv_ByteIdx=0;Tv_ByteIdx < E_GrUiDvlObjIdAlocTblSize;Tv_ByteIdx++ )
		{
			//check exist object
			for ( Tv_BitIdx = 0;Tv_BitIdx < 8;Tv_BitIdx++ )
			{
				//check exist
				if ( 0 != ((1 << Tv_BitIdx) & m_ObjIdAlocTbl[Tv_ByteIdx] ) )
				{
					//check name match
					Tv_ObjIdx	=	(Tv_ByteIdx << 3) + Tv_BitIdx;
					if ( 0 == GrStrWcmp( A_StrName, m_ObjNameTbl[Tv_ObjIdx], TRUE ) )
					{
						Tv_Result	=	TRUE;
						break;
					}
				}
			}
			if ( Tv_Result )
			{
				break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDvlMng::LcMakeNewName( WCHAR* A_StrRt, BOOL8 A_IsPage )
{
	// local -------------------
		Def_WstrName	Tv_StrName;
		Def_WstrNum		Tv_StrNum;
		__u32		Tv_ObjIdx;
	// code --------------------
		for ( Tv_ObjIdx=0;Tv_ObjIdx < E_GrUiDvlObjMaxCnt;Tv_ObjIdx++ )
		{
			//make name
			if ( A_IsPage )
			{
				GrStrWcopy( Tv_StrName, E_GrUiDvlMngNewPageName );
			}
			else
			{
				GrStrWcopy( Tv_StrName, E_GrUiDvlMngNewWinName );
			}
			GrStrIntToWstr( Tv_StrNum, (__s32)Tv_ObjIdx + 1 );
			GrStrWcat( Tv_StrName, Tv_StrNum );
			//check already exist
			if ( !LcIsExistObjByName(Tv_StrName) )
			{
				//finded
				GrStrWcopy( A_StrRt, Tv_StrName );
				break;
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDvlMng::NewWin( __u16 A_Id )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_ObjIdx;
		WCHAR*	Tv_StrObjName;
	// code --------------------
		//init
		Tv_Result	=	FALSE;
		//check useable id
		if ( !IsObjIdExist( A_Id ) )
		{
			//find empty obj slot
			if ( LcFindEmptyLumpIdx( &Tv_ObjIdx ) )
			{
				//set object
				ObjIdAlocSet( A_Id, TRUE );
				//make new window info
				m_LumpTbl[Tv_ObjIdx].Ptr	=	(Ptr_GrUiScptObjHd)malloc( E_GrUiDvlObjBufSize );
				//object name set
				Tv_StrObjName	=	ObjNameGet( A_Id );
				LcMakeNewName( Tv_StrObjName, FALSE );

				m_LumpExtTbl[Tv_ObjIdx].Size	=	m_ScptEnc->EncodeEmptyWin( A_Id, m_LumpTbl[Tv_ObjIdx].Ptr );

				//success
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDvlMng::NewPage( __u16 A_Id )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_ObjIdx;
		WCHAR*	Tv_StrObjName;
	// code --------------------
		//init
		Tv_Result	=	FALSE;
		//check useable id
		if ( !IsObjIdExist( A_Id ) )
		{
			//find empty obj slot
			if ( LcFindEmptyLumpIdx( &Tv_ObjIdx ) )
			{
				//set object
				ObjIdAlocSet( A_Id, TRUE );
				//make new window info
				m_LumpTbl[Tv_ObjIdx].Ptr	=	(Ptr_GrUiScptObjHd)malloc( E_GrUiDvlObjBufSize );
				//object name set
				Tv_StrObjName	=	ObjNameGet( A_Id );
				LcMakeNewName( Tv_StrObjName, TRUE );

				m_LumpExtTbl[Tv_ObjIdx].Size	=	m_ScptEnc->EncodeEmptyPage( A_Id, m_LumpTbl[Tv_ObjIdx].Ptr );

				//success
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrUiDvlMng::FindNotUseObjId( void )
{
	// local -------------------
		__u16		Tv_Result;
		__u16		Tv_ByteIdx;
		__u16		Tv_BitIdx;
	// code --------------------
		//init
		Tv_Result		=	E_GrUiObjIdNone;

		Tv_ByteIdx	=	(E_GrUiObjIdEncStart - E_GrUiObjIdSysStart) >> 3;
		Tv_BitIdx		=	(E_GrUiObjIdEncStart - E_GrUiObjIdSysStart) & 0x7;

		while ( (Tv_ByteIdx < E_GrUiDvlObjIdAlocTblSize) && ( E_GrUiObjIdNone == Tv_Result ) )
		{
			if ( 0xFF != m_ObjIdAlocTbl[Tv_ByteIdx] )
			{
				while ( Tv_BitIdx < 8 )
				{
					if ( 0 == ((1 << Tv_BitIdx) & m_ObjIdAlocTbl[Tv_ByteIdx]) )
					{
						//finded
						Tv_Result	=	E_GrUiObjIdSysStart + (Tv_ByteIdx << 3) + Tv_BitIdx;
						break;
					}
					//next
					Tv_BitIdx ++;
				}
			}
			//next
			Tv_ByteIdx ++;
			Tv_BitIdx		=	0;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDvlMng::FindNotUseImgId( void )
{
	// local -------------------
		__u32		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	E_GrUiImgIdRosStart;

		//check start
		while ( Tv_Result < (E_GrUiDvlImgMaxCnt + E_GrUiImgIdRosStart) )
		{
			//check exist
			if ( NULL == LcFindImgSlot( Tv_Result ) )
			{
				//finde
				break;
			}
			//next
			Tv_Result ++;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDvlMng::FindNotUseTxtId( void )
{
	// local -------------------
		__u32		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	E_GrUiTxtIdRosStart;

		//check start
		while ( Tv_Result < (E_GrUiDvlImgMaxCnt + E_GrUiImgIdRosStart) )
		{
			//check exist
			if ( NULL == LcFindTxtSlot( Tv_Result ) )
			{
				//finde
				break;
			}
			//next
			Tv_Result ++;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDvlMng::LcLumpFreeByIdx( __u32 A_Idx )
{
	// local -------------------
	// code --------------------
		//check exist
		if ( NULL != m_LumpTbl[A_Idx].Ptr )
		{
			//delete name
			ObjNameSet( m_LumpTbl[A_Idx].Ptr->ScptId, L"" );
			//release object
			//ObjIdAlocSet( m_LumpTbl[A_Idx].Ptr->ScptId, FALSE );
			//folder info reset
			m_LumpFoldTbl[A_Idx].FoldId	=	0;		//reset folder info
			//free
			free( (void*)m_LumpTbl[A_Idx].Ptr );
			m_LumpTbl[A_Idx].Ptr		=	NULL;	
			m_LumpExtTbl[A_Idx].Size	=	0;
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDvlMng::LcLumpAllFree( void )
{
	// local -------------------
		__u32		Tv_WkIdx;
	// code --------------------
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiDvlLumpMaxCnt;Tv_WkIdx++ )
		{
			LcLumpFreeByIdx( Tv_WkIdx );
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDvlMng::IsPrjExist( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		//init
		Tv_Result	=	FALSE;

		//check project exist
		if ( NULL != m_GdibCon )
		{
			Tv_Result		=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDvlMng::ScrChgSet( void )
{
		m_IsPainted		=	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrUiDvlMng::LcScptReset( void )
{
	// local -------------------
	// code --------------------
		//release old object
		LcLumpAllFree();
		LcImgAllFree();
		LcFontAllFree();
		LcTxtAllFree();
		LcMacAllFree();
		//reset sub table
		m_SkinIdx			=	0;
		GrDumyZeroMem( m_ObjIdAlocTbl, sizeof(m_ObjIdAlocTbl) )	;			//id allocate table reset

		//init script header
		m_DvlScptHd.Fcc						=	E_GrUiScptFhFcc;
		m_DvlScptHd.VerScpt				=	E_GrUiScptFhVer;
		m_DvlScptHd.IsAdrReloc		=	TRUE;
		m_DvlScptHd.VerUser				=	0;

		m_DvlScptHd.SctLump.Cnt			=	E_GrUiDvlLumpMaxCnt;
		m_DvlScptHd.SctLump.PtrTbl	=	m_LumpTbl;
		m_DvlScptHd.SctFont.Cnt			=	E_GrUiDvlFontMaxCnt;
		m_DvlScptHd.SctFont.PtrTbl	=	m_FontTbl[m_SkinIdx];
		m_DvlScptHd.SctImg.Cnt			=	E_GrUiDvlImgMaxCnt;
		m_DvlScptHd.SctImg.PtrTbl		=	m_SkinTbl[m_SkinIdx];
		m_DvlScptHd.SctText.Cnt			=	E_GrUiDvlTextMaxCnt;
		m_DvlScptHd.SctText.PtrTbl	=	m_LangTbl;

}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDvlMng::LumpScptEnc( Cls_GrUiDoWin* A_Win )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u16		Tv_ObjId;
		__u32		Tv_Idx;
		Def_WstrShort	Tv_StrMsg;
		Def_WstrNum		Tv_StrNum;
		__u8		Tv_Layer;
		__u32		Tv_Tmp;
	// code --------------------
		//init
		Tv_Result	=	FALSE;

		//get id
		Tv_ObjId	=	A_Win->ObjIdGet();
		if ( (E_GrUiObjIdNone != Tv_ObjId) && A_Win->IsWindow() )
		{
			//check exist
			if ( FindLumpIdxById( Tv_ObjId, &Tv_Idx ) )
			{
				//check match type
				if ( E_GrUiDoTypeIdNone	== m_LumpTbl[Tv_Idx].Ptr->TypeId )
				{
					//frame page mode
					m_LumpExtTbl[Tv_Idx].Size	=	m_ScptEnc->EncodeSlaveObj( (Cls_GrUiDoBase*)A_Win, m_LumpTbl[Tv_Idx].Ptr );
					Tv_Tmp		=	A_Win->UserDataGet( 0 );		//page index
					m_LumpTbl[Tv_Idx].Ptr->Rect.left	=	(__s16)(0xFFFF & Tv_Tmp);		
					m_LumpTbl[Tv_Idx].Ptr->Rect.top		=	(__s16)(0xFFFF & (Tv_Tmp >> 16));		
				}
				else
				{
					//window encoding
					Tv_Layer	=	m_LumpTbl[Tv_Idx].Ptr->Layer;			//layer backup
					m_LumpExtTbl[Tv_Idx].Size	=	m_ScptEnc->EncodeObj( (Cls_GrUiDoBase*)A_Win, m_LumpTbl[Tv_Idx].Ptr  );
					m_LumpTbl[Tv_Idx].Ptr->Layer	=	Tv_Layer;		//restore layer
				}
			}
			else
			{
				//bad id
				GrStrWcopy( Tv_StrMsg, L"[BUG] Script update failure - obj id :" );
				GrStrIntToWstr( Tv_StrNum, Tv_ObjId );
				GrStrWcat( Tv_StrMsg, Tv_StrNum );
#ifndef LINUX_APP
				MessageBox( NULL, Tv_StrMsg, L"error", MB_OK );
#endif
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDvlMng::IsPageLump( __u16 A_ObjId )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_LumpIdx;
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		//check exist
		if ( FindLumpIdxById( A_ObjId, &Tv_LumpIdx ) )
		{
			//check page
			if ( E_GrUiDoTypeIdNone	== m_LumpTbl[Tv_LumpIdx].Ptr->TypeId )
			{
				Tv_Result	=	TRUE;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDvlMng::ObjNameSet( __u16 A_Id, WCHAR* A_StrName )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result	=	FALSE;

		//check id range
		if ( (E_GrUiObjIdSysStart <= A_Id) && ((E_GrUiObjIdSysStart + E_GrUiDvlObjMaxCnt) > A_Id ) )
		{
			GrStrWcopy( m_ObjNameTbl[A_Id-E_GrUiObjIdSysStart], A_StrName );
			Tv_Result	=	TRUE;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrUiDvlMng::ObjNameGet( __u16 A_Id )
{
	// local -------------------
		WCHAR*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;

		//check id range
		if ( (E_GrUiObjIdSysStart <= A_Id) && ((E_GrUiObjIdSysStart + E_GrUiDvlObjMaxCnt) > A_Id ) )
		{
			Tv_Result	=	m_ObjNameTbl[A_Id-E_GrUiObjIdSysStart];
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDvlMng::WinLayerChangeById( __u16 A_Id, __u8 A_Layer )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u32		Tv_Idx;
	// code --------------------
		//init
		Tv_Result	=	FALSE;

		//check bad id
		if ( 0 != A_Id )
		{
			//find slot
			if ( FindLumpIdxById( A_Id, &Tv_Idx ) )
			{
				m_LumpTbl[Tv_Idx].Ptr->Layer	=	(__u8)A_Layer;
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDvlMng::IsScrChanged( void )
{
		return	m_IsPainted;
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDvlMng::FindImgIdByPtr( Ptr_GrGdib A_PtrGdib )
{
	// local -------------------
		Cls_GrUiScptDec*	Tv_ScptDec;
	// code --------------------
		Tv_ScptDec	=	m_UiMng->ScptDecGet();
		return	Tv_ScptDec->DvlImgIdFindByPtr( A_PtrGdib );
}
//--------------------------------------------------------------------
Ptr_GrUiScptFontTblHd	Cls_GrUiDvlMng::LcFindFontSlot( __u32 A_Id )
{
	// local -------------------
		Ptr_GrUiScptFontTblHd	Tv_Result;
		__u32	Tv_WkIdx;
	// code --------------------
		//init
		Tv_Result	=	NULL;

		//check slot
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiDvlFontMaxCnt;Tv_WkIdx++ )
		{
			//check exist
			if ( NULL != m_FontTbl[m_SkinIdx][Tv_WkIdx].Ptr )
			{
				//check match id
				if ( A_Id == m_FontTbl[m_SkinIdx][Tv_WkIdx].Id )
				{
					//finded
					Tv_Result	=	&m_FontTbl[m_SkinIdx][Tv_WkIdx];
					break;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Ptr_GrUiScptFontTblHd	Cls_GrUiDvlMng::LcFindEmptyFontSlot( void )
{
	// local -------------------
		Ptr_GrUiScptFontTblHd	Tv_Result;
		__u32	Tv_WkIdx;
	// code --------------------
		//init
		Tv_Result	=	NULL;

		//check slot
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiDvlFontMaxCnt;Tv_WkIdx++ )
		{
			//check exist
			if ( NULL == m_FontTbl[m_SkinIdx][Tv_WkIdx].Ptr )
			{
				//finded
				Tv_Result	=	&m_FontTbl[m_SkinIdx][Tv_WkIdx];
				break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDvlMng::LcFontAllFree( void )
{
	// local -------------------
		__u32	Tv_WkIdx;
		__u8	Tv_SkinIdx;
	// code --------------------
		for ( Tv_SkinIdx=0;Tv_SkinIdx < E_GrUiDvlSkinMaxCnt;Tv_SkinIdx++ )
		{
			//free
			for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiDvlFontMaxCnt;Tv_WkIdx++ )
			{
				//check exist
				if ( NULL != m_FontTbl[Tv_SkinIdx][Tv_WkIdx].Ptr )
				{
					//release
					free( m_FontTbl[Tv_SkinIdx][Tv_WkIdx].Ptr );
					m_FontTbl[Tv_SkinIdx][Tv_WkIdx].Ptr	=	NULL;
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDvlMng::LcImgAllFree( void )
{
	// local -------------------
		__u32	Tv_WkIdx;
		__u8	Tv_SkinIdx;
	// code --------------------
		//free
		for ( Tv_SkinIdx=0;Tv_SkinIdx < E_GrUiDvlSkinMaxCnt;Tv_SkinIdx++ )
		{
			for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiDvlImgMaxCnt;Tv_WkIdx++ )
			{
				//check exist
				if ( NULL != m_SkinTbl[Tv_SkinIdx][Tv_WkIdx].Ptr )
				{
					//release
					free( m_SkinTbl[Tv_SkinIdx][Tv_WkIdx].Ptr );
					m_SkinTbl[Tv_SkinIdx][Tv_WkIdx].Ptr	=	NULL;
				}
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDvlMng::FontLoad( __u8 A_Id, WCHAR* A_StrFn )
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrFileCtrl*	Tv_ObjFl;		
		Ptr_GrUiScptFontTblHd	Tv_PtrSlot;
		__u32	Tv_SzFont;
		Cls_GrUiScptDec*	Tv_ScptDec;
	// code --------------------
		//init
		Tv_Result	=	FALSE;

		//find already exist
		Tv_PtrSlot	=	LcFindFontSlot( A_Id );
		if ( NULL == Tv_PtrSlot )
		{
			Tv_PtrSlot	=	LcFindEmptyFontSlot();
		}

		//check loadable
		if ( NULL != Tv_PtrSlot )
		{
			//load file
			Tv_ObjFl	=	(Cls_GrFileCtrl*) new Cls_GrFileCtrl( A_StrFn, FALSE, FALSE );
			if ( Tv_ObjFl->IsOpened() )
			{
				//release old font
				if ( NULL != Tv_PtrSlot->Ptr	)
				{
					free( Tv_PtrSlot->Ptr );
					Tv_PtrSlot->Ptr	=	NULL;
				}
				//get size
				Tv_SzFont	=	(__u32)Tv_ObjFl->GetSize();
				//allocate font
				Tv_PtrSlot->Ptr		=	(Ptr_GrUiFontV2Head)malloc(Tv_SzFont );
				if ( NULL != Tv_PtrSlot->Ptr )
				{
					//update id
					Tv_PtrSlot->Id	=	A_Id;
					//load
					Tv_ObjFl->Seek( 0 );
					Tv_ObjFl->Read( Tv_PtrSlot->Ptr, Tv_SzFont );
					//update font
					Tv_ScptDec	=	m_UiMng->ScptDecGet();
					Tv_ScptDec->DvlFontUpdt();
					//success
					Tv_Result	=	TRUE;
				}
			}
			delete	Tv_ObjFl;
		}
		else
		{
#ifndef LINUX_APP
			MessageBox( NULL, L"[BUG]Break font slot.", L"error" , MB_OK );
#endif
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDvlMng::FontDelete( __u8 A_Id )
{
	// local -------------------
		BOOL8		Tv_Result;
		Ptr_GrUiScptFontTblHd	Tv_PtrSlot;
		Cls_GrUiScptDec*	Tv_ScptDec;
	// code --------------------
		//init
		Tv_Result	=	FALSE;

		//find slot
		Tv_PtrSlot	=	LcFindFontSlot( A_Id );
		if ( NULL != Tv_PtrSlot )
		{
			//release
			free( Tv_PtrSlot->Ptr	);
			Tv_PtrSlot->Ptr		=	NULL;
			Tv_PtrSlot->Id		=	0;
			//update font
			Tv_ScptDec	=	m_UiMng->ScptDecGet();
			Tv_ScptDec->DvlFontUpdt();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Ptr_GrUiScptImgTblHd	Cls_GrUiDvlMng::LcFindImgSlot( __u32 A_Id )
{
	// local -------------------
		Ptr_GrUiScptImgTblHd	Tv_Result;
		__u32		Tv_WkIdx;
	// code --------------------
		//init
		Tv_Result		=	NULL;

		//find
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiDvlImgMaxCnt;Tv_WkIdx++ )
		{
			//check exist
			if ( NULL != m_SkinTbl[m_SkinIdx][Tv_WkIdx].Ptr )
			{
				//check id
				if ( A_Id == m_SkinTbl[m_SkinIdx][Tv_WkIdx].Id )
				{
					Tv_Result	=	&m_SkinTbl[m_SkinIdx][Tv_WkIdx];
					break;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Ptr_GrUiScptImgTblHd	Cls_GrUiDvlMng::LcFindEmptyImgSlot( void )
{
	// local -------------------
		Ptr_GrUiScptImgTblHd	Tv_Result;
		__u32		Tv_WkIdx;
	// code --------------------
		//init
		Tv_Result		=	NULL;

		//find
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiDvlImgMaxCnt;Tv_WkIdx++ )
		{
			//check exist
			if ( NULL == m_SkinTbl[m_SkinIdx][Tv_WkIdx].Ptr )
			{
				Tv_Result	=	&m_SkinTbl[m_SkinIdx][Tv_WkIdx];
				break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDvlMng::ImgLoad( __u16 A_Id, WCHAR* A_StrFn )
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrGdib*		Tv_GdibBmp;
		Cls_GrGdib*		Tv_GdibCvt;
		Ptr_GrUiScptImgTblHd	Tv_PtrSlot;
		__u32	Tv_SizeGdib;
		__u32	Tv_FccPrj;
		BOOL8	Tv_IsAble;
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		//check id
		if ( E_GrUiImgIdNone != A_Id )
		{
			//init
			Tv_FccPrj		=	PrjFccGet();
			//load bmp
			Tv_GdibBmp	=	GrGdibLoadBmp( A_StrFn );
			if ( NULL != Tv_GdibBmp )
			{
				//convert gdib
				Tv_GdibCvt	=	NULL;
				Tv_IsAble		=	TRUE;
				if ( IsDispPlt() )
				{
					if ( Tv_GdibBmp->IsPalleteUse() )
					{
						//chanage pallete default
						Tv_GdibBmp->PalleteDefault();
						Tv_GdibBmp->GetGdibPtr()->FccImg	=	PrjFccGet();
						Tv_GdibCvt	=	Tv_GdibBmp;
					}
					else
					{
						Tv_IsAble		=	FALSE;
					}
				}
				else
				{
					if ( Tv_FccPrj != Tv_GdibBmp->GetFcc() )
					{
						if ( Tv_GdibBmp->IsPalleteUse() )
						{
							Tv_GdibBmp->ChgPalletForamt( Tv_FccPrj );
							Tv_GdibCvt	=	Tv_GdibBmp;
						}
						else
						{
							Tv_GdibCvt	=	GrGdibCreate( Tv_FccPrj, 
														Tv_GdibBmp->GetResX(), Tv_GdibBmp->GetResY() );
							Tv_GdibBmp->CvtTo( Tv_GdibCvt );
							delete	Tv_GdibBmp;
						}
					}
					else
					{
						Tv_GdibCvt	=	Tv_GdibBmp;
					}
				}

				Tv_GdibBmp	=	NULL;			//not use mark
				if ( Tv_IsAble )
				{
					//find bitmap slot
					Tv_PtrSlot	=	LcFindImgSlot( A_Id );
					if ( NULL == Tv_PtrSlot )
					{
						Tv_PtrSlot	=	LcFindEmptyImgSlot();
					}
					//check finded
					if ( NULL != Tv_PtrSlot )
					{
						//release old slot
						if ( NULL != Tv_PtrSlot->Ptr )
						{
							free( Tv_PtrSlot->Ptr );
							Tv_PtrSlot->Ptr		=	NULL;
						}
						Tv_SizeGdib	=	Tv_GdibCvt->SaveToMem( NULL );
						Tv_PtrSlot->Ptr	=	(Ptr_GrGdib)malloc( Tv_SizeGdib );
						Tv_GdibCvt->SaveToMem( (void*)Tv_PtrSlot->Ptr );
						Tv_PtrSlot->Id		=	A_Id;
						//success
						Tv_Result		=	TRUE;
					}				
				}
				//release converted gdib
				delete	Tv_GdibCvt;
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDvlMng::ImgDelete( __u32 A_Id )
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrUiScptImgTblHd	Tv_PtrSlot;
	// code --------------------
		//init
		Tv_Result	=	FALSE;
	
		//find slot
		Tv_PtrSlot	=	LcFindImgSlot( A_Id );
		if ( NULL != Tv_PtrSlot )
		{
			free( Tv_PtrSlot->Ptr );
			Tv_PtrSlot->Ptr		=	NULL;
			Tv_PtrSlot->Id		=	E_GrUiImgIdNone;
			Tv_Result		=	TRUE;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDvlMng::PrjFccGet( void )
{
	// local -------------------
		__u32		Tv_Result;
		Cls_GrUiConsole*	Tv_ObjCon;
	// code --------------------
		//init
		Tv_Result	=	0;

		if ( NULL != m_UiMng )
		{
			Tv_ObjCon	=	m_UiMng->GetConsoleObj( 0 );
			if ( NULL != Tv_ObjCon )
			{
				Tv_Result	=	Tv_ObjCon->GetFcc();
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDvlMng::IsDispPlt( void )
{
	// local -------------------
		BOOL8		Tv_Result;
		Cls_GrUiConsole*	Tv_ObjCon;
		Cls_GrGdib*	Tv_Gdib;
	// code --------------------
		//init
		Tv_Result	=	FALSE;

		if ( NULL != m_UiMng )
		{
			Tv_ObjCon	=	m_UiMng->GetConsoleObj( 0 );
			if ( NULL != Tv_ObjCon )
			{
				Tv_Gdib	=	Tv_ObjCon->GetGdibScr();
				Tv_Result	=	Tv_Gdib->IsPalleteUse();
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDvlMng::TxtSet( __u32 A_TxtId, __u32 A_Lang, WCHAR* A_Str )
{
	// local -------------------
		BOOL8		Tv_Result;
		Ptr_GrUiScptLangTxtHd	Tv_PtrSlot;
		__u32		Tv_WkLng;
		BOOL8		Tv_IsExist;
	// code --------------------
		Tv_Result		=	FALSE;
		if ( GrStrIsExist( A_Str ) )
		{
			//set
			Tv_PtrSlot	=	LcFindTxtSlot( A_TxtId );
			if ( NULL == Tv_PtrSlot )
			{
				Tv_PtrSlot	=	LcFindEmptyTxtSlot();
			}
			//check find slot
			if ( NULL != Tv_PtrSlot )
			{
				//update id
				Tv_PtrSlot->Id	=	A_TxtId;
				//check exist slot
				if ( NULL == Tv_PtrSlot->LngPosTbl[A_Lang] )
				{
					//make slot
					Tv_PtrSlot->LngPosTbl[A_Lang]	=	(WCHAR*)malloc( E_GrUiDvlTextBufSize );
				}
				//check able
				if ( NULL != Tv_PtrSlot->LngPosTbl[A_Lang] )
				{
					//update text
					GrStrWcopy( Tv_PtrSlot->LngPosTbl[A_Lang], A_Str );			//copy contents
					Tv_Result	=	TRUE;
				}
			}
		}
		else
		{
			//release
			Tv_PtrSlot	=	LcFindTxtSlot( A_TxtId );
			//check find slot
			if ( NULL != Tv_PtrSlot )
			{
				//check able
				if ( NULL != Tv_PtrSlot->LngPosTbl[A_Lang] )
				{
					//release text
					free( Tv_PtrSlot->LngPosTbl[A_Lang] );
					Tv_PtrSlot->LngPosTbl[A_Lang]	=	NULL;
					Tv_Result	=	TRUE;
				}
				//check not exist slot
				Tv_IsExist	=	FALSE;
				for ( Tv_WkLng=0;Tv_WkLng < E_GrUiLanguageCnt;Tv_WkLng++ )
				{
					if ( NULL != Tv_PtrSlot->LngPosTbl[Tv_WkLng] )
					{
						Tv_IsExist	=	TRUE;
						break;
					}
				}
				//check release
				if ( !Tv_IsExist )
				{
					Tv_PtrSlot->Id		=	E_GrUiTxtIdNone;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Ptr_GrUiScptLangTxtHd	Cls_GrUiDvlMng::LcFindTxtSlot( __u32 A_Id )
{
	// local -------------------
		Ptr_GrUiScptLangTxtHd	Tv_Result;
		__u32		Tv_WkIdx;
	// code --------------------
		//init
		Tv_Result	=	NULL;

		//check
		if ( E_GrUiTxtIdNone != A_Id )
		{
			for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiDvlTextMaxCnt;Tv_WkIdx++ )
			{
				//check exist
				if ( A_Id == m_LangTbl[Tv_WkIdx].Id )
				{
					//finded
					Tv_Result	=	&m_LangTbl[Tv_WkIdx];
					break;
				}
			}
		}
		
		return	Tv_Result;		
}
//--------------------------------------------------------------------
Ptr_GrUiScptLangTxtHd	Cls_GrUiDvlMng::LcFindEmptyTxtSlot( void )
{
	// local -------------------
		Ptr_GrUiScptLangTxtHd	Tv_Result;
		__u32		Tv_WkIdx;
	// code --------------------
		//init
		Tv_Result	=	NULL;

		//check
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiDvlTextMaxCnt;Tv_WkIdx++ )
		{
			//check exist
			if ( E_GrUiTxtIdNone == m_LangTbl[Tv_WkIdx].Id )
			{
				//finded
				Tv_Result	=	&m_LangTbl[Tv_WkIdx];
				break;
			}
		}

		return	Tv_Result;		
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDvlMng::TxtDel( __u32 A_TxtId )
{
	// local -------------------
		BOOL8		Tv_Result;
		Ptr_GrUiScptLangTxtHd	Tv_PtrSlot;
		__u32		Tv_WkIdx;
	// code --------------------
		//init
		Tv_Result	=	FALSE;

		//find
		Tv_PtrSlot	=	LcFindTxtSlot( A_TxtId );
		if ( NULL != Tv_PtrSlot )
		{
			for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiLanguageCnt;Tv_WkIdx++ )
			{
				//check exist
				if ( NULL != Tv_PtrSlot->LngPosTbl[Tv_WkIdx] )
				{
					free( Tv_PtrSlot->LngPosTbl[Tv_WkIdx] );
					Tv_PtrSlot->LngPosTbl[Tv_WkIdx]	=	NULL;
				}
			}
			Tv_PtrSlot->Id		=	E_GrUiTxtIdNone;
			Tv_Result	=	TRUE;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDvlMng::TxtIdGetByIdx( __u32 A_Idx )
{
	// local -------------------
		__u32	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrUiTxtIdNone;

		//check able range
		if ( E_GrUiDvlTextMaxCnt > A_Idx )
		{
			Tv_Result	=	m_LangTbl[A_Idx].Id;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDvlMng::IsTxtExist( __u32 A_TxtId )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		//init
		Tv_Result	=	FALSE;

		//check able
		if ( E_GrUiTxtIdNone != A_TxtId )
		{
			if ( NULL != LcFindTxtSlot( A_TxtId ) )
			{
				//finded
				Tv_Result	=	TRUE;
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDvlMng::LcTxtAllFree( void )
{
	// local -------------------
		__u32		Tv_TxtIdx;
		__u32		Tv_LngIdx;
	// code --------------------
		for ( Tv_TxtIdx=0;Tv_TxtIdx < E_GrUiDvlTextMaxCnt;Tv_TxtIdx++ )
		{
			//check exist
			if ( E_GrUiTxtIdNone != m_LangTbl[Tv_TxtIdx].Id )
			{
				for ( Tv_LngIdx=0;Tv_LngIdx < E_GrUiLanguageCnt;Tv_LngIdx++ )
				{
					if ( NULL != m_LangTbl[Tv_TxtIdx].LngPosTbl[Tv_LngIdx] )
					{
						free( m_LangTbl[Tv_TxtIdx].LngPosTbl[Tv_LngIdx] );
						m_LangTbl[Tv_TxtIdx].LngPosTbl[Tv_LngIdx]	=	NULL;
					}
				}
				m_LangTbl[Tv_TxtIdx].Id	=	E_GrUiTxtIdNone;
			}
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrUiDvlMng::LcMacAllFree( void )
{
	// local -------------------
		__u32		Tv_WkIdx;
	// code --------------------
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiDvlMacMaxCnt;Tv_WkIdx++ )
		{
			if ( NULL != m_MacTbl[Tv_WkIdx].Ptr )
			{
				free( m_MacTbl[Tv_WkIdx].Ptr );
				m_MacTbl[Tv_WkIdx].Ptr	=	NULL;
			}
			m_MacExtTbl[Tv_WkIdx].Size	=	0;
			GrStrClear( m_MacExtTbl[Tv_WkIdx].StrName );
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDvlMng::LcUpdtPrjText( void )
{
	// local -------------------
		Def_WstrShort	Tv_StrTime;
		Def_GrTime	Tv_Time;
		__u32		Tv_WkLng;
		__u32		Tv_Mili;
		BOOL8		Tv_IsSm;
	// code --------------------
		//update save time
		Tv_Time	=	GrTimeGet( &Tv_Mili, &Tv_IsSm );
		GrStrTimeToWstr( Tv_StrTime, Tv_Time, FALSE, FALSE, FALSE, 0, 0, 1 );
		for (Tv_WkLng=0;Tv_WkLng < E_GrUiLanguageCnt;Tv_WkLng++)
		{
			TxtSet( E_GrUiTxtRosMakeDate, Tv_WkLng, Tv_StrTime );
		}

		//update user version
		GrStrDwordToHexWstr( Tv_StrTime, V_GrUiMngObj->UserVersionGet() );
		for (Tv_WkLng=0;Tv_WkLng < E_GrUiLanguageCnt;Tv_WkLng++)
		{
			TxtSet( E_GrUiTxtIdUserVer, Tv_WkLng, Tv_StrTime );
		}

}
//--------------------------------------------------------------------
void*	Cls_GrUiDvlMng::LcMkGrspLumpExt( void )
{
	// local -------------------
		Ptr_GrUiDvlLumpExt	Tv_Result;			
		__u32		Tv_WkIdx;
	// code --------------------
		//init
		Tv_Result		=	m_PtrGrspObjExt;

		//find
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiDvlLumpMaxCnt;Tv_WkIdx++ )
		{
			//check exist
			if ( (NULL != m_LumpTbl[Tv_WkIdx].Ptr) && (0 != m_LumpExtTbl[Tv_WkIdx].Size) )
			{
				//add
				GrDumyCopyMem( Tv_Result, &m_LumpExtTbl[Tv_WkIdx], sizeof(St_GrUiDvlLumpExt) );
				//next
				m_PtrGrspFh->LumpCnt ++;		//update lump count
				Tv_Result ++;
			}
		}
	
		return	(void*)Tv_Result;
}
//--------------------------------------------------------------------
void*	Cls_GrUiDvlMng::LcMkGrspLumpFold( void )
{
	// local -------------------
		Ptr_GrUiDvlLumpFold	Tv_Result;			
		__u32		Tv_WkIdx;
	// code --------------------
		//init
		Tv_Result		=	m_PtrGrspObjFold;

		//find
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiDvlLumpMaxCnt;Tv_WkIdx++ )
		{
			//check exist
			if ( (NULL != m_LumpTbl[Tv_WkIdx].Ptr) && (0 != m_LumpExtTbl[Tv_WkIdx].Size) )
			{
				//add
				GrDumyCopyMem( Tv_Result, &m_LumpFoldTbl[Tv_WkIdx], sizeof(St_GrUiDvlLumpFold) );
				//next
				Tv_Result ++;
			}
		}

		return	(void*)Tv_Result;
}
//--------------------------------------------------------------------
void*	Cls_GrUiDvlMng::LcMkGrspPfold( void )
{
	// local -------------------
		void*		Tv_Result;			
	// code --------------------
		//init
		Tv_Result		=	m_PtrGrspPfold;

		//find
		GrDumyCopyMem( Tv_Result, (void*)m_PfoldTbl, sizeof(m_PfoldTbl) );

		return	(void*)( (__u32)Tv_Result + sizeof(m_PfoldTbl) );
}
//--------------------------------------------------------------------
void*	Cls_GrUiDvlMng::LcMkGrspMacExt( void )
{
	// local -------------------
		Ptr_GrUiDvlMacExt	Tv_Result;			
		__u32		Tv_WkIdx;
	// code --------------------
		//init
		Tv_Result		=	m_PtrGrspMacExt;

		//find
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiDvlMacMaxCnt;Tv_WkIdx++ )
		{
			//check exist
			if ( (NULL != m_MacTbl[Tv_WkIdx].Ptr) && (0 != m_MacExtTbl[Tv_WkIdx].Size) )
			{
				//add
				GrDumyCopyMem( Tv_Result, &m_MacExtTbl[Tv_WkIdx], sizeof(St_GrUiDvlMacExt) );
				//next
				m_PtrGrspFh->MacCnt ++;		//update macro count
				Tv_Result ++;
			}
		}

		return	(void*)Tv_Result;
}
//--------------------------------------------------------------------
void*	Cls_GrUiDvlMng::LcMkGrspMacTbl( void )
{
	// local -------------------
		Ptr_GrUiScptLumpHd	Tv_Result;			
		__u32		Tv_WkIdx;
	// code --------------------
		//init
		Tv_Result		=	m_PtrGrspMacObj;

		//find
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiDvlMacMaxCnt;Tv_WkIdx++ )
		{
			//check exist
			if ( NULL != m_MacTbl[Tv_WkIdx].Ptr )
			{
				//add
				GrDumyCopyMem( Tv_Result, &m_MacTbl[Tv_WkIdx], sizeof(St_GrUiScptLumpHd) );
				Tv_Result->Ptr	=	NULL;		//clear
				//next
				Tv_Result ++;
			}
		}

		return	(void*)Tv_Result;
}
//--------------------------------------------------------------------
void*	Cls_GrUiDvlMng::LcMkGrspMacScpt( void )
{
	// local -------------------
		__u8*		Tv_Result;			
		__u32		Tv_WkIdx;
		Ptr_GrUiScptLumpHd	Tv_PtrObj;
	// code --------------------
		//init
		Tv_Result		=	(__u8*)m_PtrGrspMacScpt;
		Tv_PtrObj		=	m_PtrGrspMacObj;

		//check count
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiDvlMacMaxCnt;Tv_WkIdx++ )
		{
			//check exist
			if ( (NULL != m_MacTbl[Tv_WkIdx].Ptr) && (0 != m_MacExtTbl[Tv_WkIdx].Size) )
			{
				//add
				GrDumyCopyMem( Tv_Result, m_MacTbl[Tv_WkIdx].Ptr, m_MacExtTbl[Tv_WkIdx].Size );
				Tv_PtrObj->Ptr	=	(Ptr_GrUiScptObjHd)( (__u32)Tv_Result - (__u32)m_PtrGrspBuf );
				//next
				Tv_PtrObj ++;
				Tv_Result =	Tv_Result + m_MacExtTbl[Tv_WkIdx].Size;
			}
		}

		return	(void*)Tv_Result;
}
//--------------------------------------------------------------------
void*	Cls_GrUiDvlMng::LcMkGrspMac( void )
{
	// local -------------------
		void*		Tv_Result;
		__u32		Tv_Size;
	// code --------------------
		Tv_Result	=	m_PtrGrspMacBuf;
		//make ext
		m_PtrGrspMacExt	=	(Ptr_GrUiDvlMacExt)Tv_Result;
		Tv_Result	=	LcMkGrspMacExt();
		//make obj
		m_PtrGrspMacObj	=	(Ptr_GrUiScptLumpHd)Tv_Result;
		Tv_Result	=	LcMkGrspMacTbl();
		//make script
		m_PtrGrspMacScpt	=	Tv_Result;
		Tv_Result	=	LcMkGrspMacScpt();
		//patch size
		Tv_Size		=	(__u32)Tv_Result - (__u32)m_PtrGrspBuf;
		Tv_Size		=	(Tv_Size + 3 ) & (~3);		//struct align
		Tv_Result	=	(void*)( (__u32)m_PtrGrspBuf + Tv_Size );
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void*	Cls_GrUiDvlMng::LcMkRosLumpTbl( void )
{
	// local -------------------
		Ptr_GrUiScptLumpHd	Tv_Result;
		__u32		Tv_WkIdx;
	// code --------------------
		//init
		Tv_Result	=	m_PtrRosLumpTbl;
		m_PtrRosFh->SctLump.PtrTbl	=	(Ptr_GrUiScptLumpHd)( (__u32)Tv_Result - (__u32)m_PtrRosBuf  );

		//find
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiDvlLumpMaxCnt;Tv_WkIdx++ )
		{
			//check exist
			if ( NULL != m_LumpTbl[Tv_WkIdx].Ptr )
			{
				//add
				GrDumyCopyMem( Tv_Result, &m_LumpTbl[Tv_WkIdx], sizeof(St_GrUiScptLumpHd) );
				Tv_Result->Ptr	=	NULL;
				//next
				m_PtrRosFh->SctLump.Cnt	++;
				Tv_Result ++;
			}
		}

		return	(void*)Tv_Result;
}
//--------------------------------------------------------------------
void*	Cls_GrUiDvlMng::LcMkRosFontTbl( void )
{
	// local -------------------
		Ptr_GrUiScptFontTblHd	Tv_Result;
		__u32		Tv_WkIdx;
	// code --------------------
		//init
		Tv_Result	=	m_PtrRosFontTbl;
		m_PtrRosFh->SctFont.PtrTbl	=	(Ptr_GrUiScptFontTblHd)( (__u32)Tv_Result - (__u32)m_PtrRosBuf  );

		//find
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiDvlFontMaxCnt;Tv_WkIdx++ )
		{
			//check exist
			if ( NULL != m_FontTbl[m_SkinIdx][Tv_WkIdx].Ptr )
			{
				//add
				GrDumyCopyMem( Tv_Result, &m_FontTbl[m_SkinIdx][Tv_WkIdx], sizeof(St_GrUiScptFontTblHd) );
				Tv_Result->Ptr	=	NULL;
				//next
				m_PtrRosFh->SctFont.Cnt	++;
				Tv_Result ++;
			}
		}

		return	(void*)Tv_Result;
}
//--------------------------------------------------------------------
void*	Cls_GrUiDvlMng::LcMkRosImgTbl( BOOL8 A_IsPrj )
{
	// local -------------------
		Ptr_GrUiScptImgTblHd	Tv_Result;
		__u32		Tv_WkIdx;
	// code --------------------
		//init
		Tv_Result	=	m_PtrRosImgTbl;
		m_PtrRosFh->SctImg.PtrTbl	=	(Ptr_GrUiScptImgTblHd)( (__u32)Tv_Result - (__u32)m_PtrRosBuf  );

		//check project
		if ( !A_IsPrj )
		{
			//find
			for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiDvlImgMaxCnt;Tv_WkIdx++ )
			{
				//check exist
				if ( NULL != m_SkinTbl[m_SkinIdx][Tv_WkIdx].Ptr )
				{
					//add
					GrDumyCopyMem( Tv_Result, &m_SkinTbl[m_SkinIdx][Tv_WkIdx], sizeof(St_GrUiScptImgTblHd) );
					Tv_Result->Ptr	=	NULL;
					//next
					m_PtrRosFh->SctImg.Cnt	++;
					Tv_Result ++;
				}
			}
		}

		return	(void*)Tv_Result;
}
//--------------------------------------------------------------------
void*	Cls_GrUiDvlMng::LcMkRosTxtTbl( void )
{
	// local -------------------
		Ptr_GrUiScptLangTxtHd	Tv_Result;
		__u32		Tv_WkIdx;
		__u32		Tv_WkLng;
	// code --------------------
		//init
		Tv_Result	=	m_PtrRosTxtTbl;
		m_PtrRosFh->SctText.PtrTbl	=	(Ptr_GrUiScptLangTxtHd)( (__u32)Tv_Result - (__u32)m_PtrRosBuf  );

		//find
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiDvlTextMaxCnt;Tv_WkIdx++ )
		{
			//check exist
			if ( E_GrUiTxtIdNone != m_LangTbl[Tv_WkIdx].Id )
			{
				//add
				GrDumyCopyMem( Tv_Result, &m_LangTbl[Tv_WkIdx], sizeof(St_GrUiScptLangTxtHd) );
				for ( Tv_WkLng=0;Tv_WkLng < E_GrUiLanguageCnt;Tv_WkLng++ )
				{
					Tv_Result->LngPosTbl[Tv_WkLng]	=	NULL;
				}
				//next
				m_PtrRosFh->SctText.Cnt	++;
				Tv_Result ++;
			}
		}

		return	(void*)Tv_Result;
}
//--------------------------------------------------------------------
void*	Cls_GrUiDvlMng::LcMkRosLumpScpt( void )
{
	// local -------------------
		__u8*		Tv_Result;
		Ptr_GrUiScptLumpHd	Tv_PtrObj;
		__u32		Tv_WkIdx;
	// code --------------------
		//init
		Tv_Result		=	(__u8*)m_PtrRosObjScpt;
		Tv_PtrObj		=	m_PtrRosLumpTbl;

		//find
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiDvlLumpMaxCnt;Tv_WkIdx++ )
		{
			//check exist
			if ( (NULL != m_LumpTbl[Tv_WkIdx].Ptr) && (0 != m_LumpExtTbl[Tv_WkIdx].Size) )
			{
				//copy object
				GrDumyCopyMem( Tv_Result, m_LumpTbl[Tv_WkIdx].Ptr, m_LumpExtTbl[Tv_WkIdx].Size );
				//patch address
				Tv_PtrObj->Ptr	=	(Ptr_GrUiScptObjHd)( (__u32)Tv_Result - (__u32)m_PtrRosBuf );
				//next
				Tv_PtrObj ++;
				Tv_Result	=	Tv_Result + ((m_LumpExtTbl[Tv_WkIdx].Size + 3) & (~3) );	// 4 byte align
			}
		}

		//patch align
		Tv_Result	=	(__u8*)( ( (__u32)Tv_Result + 3 ) & (~3) );

		return	(void*)Tv_Result;
}
//--------------------------------------------------------------------
void*	Cls_GrUiDvlMng::LcMkRosFontData( void )
{
	// local -------------------
		__u8*		Tv_Result;
		Ptr_GrUiScptFontTblHd	Tv_PtrFontTbl;
		__u32		Tv_WkIdx;
		__u32		Tv_FontSize;
	// code --------------------
		//init
		Tv_Result			=	(__u8*)m_PtrRosFontData;
		Tv_PtrFontTbl	=	m_PtrRosFontTbl;

		//find
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiDvlFontMaxCnt;Tv_WkIdx++ )
		{
			//check exist
			if ( NULL != m_FontTbl[m_SkinIdx][Tv_WkIdx].Ptr )
			{
				//add
				Tv_FontSize	=	GrUiFontCalcSize( m_FontTbl[m_SkinIdx][Tv_WkIdx].Ptr );
				GrDumyCopyMem( Tv_Result, m_FontTbl[m_SkinIdx][Tv_WkIdx].Ptr, Tv_FontSize );
				//patch address
				Tv_PtrFontTbl->Ptr	=	(Ptr_GrUiFontV2Head)( (__u32)Tv_Result - (__u32)m_PtrRosBuf );
				//next
				Tv_PtrFontTbl ++;
				Tv_Result		=	Tv_Result + Tv_FontSize;
			}
		}

		return	(void*)Tv_Result;
}
//--------------------------------------------------------------------
void*	Cls_GrUiDvlMng::LcMkRosImgData( BOOL8 A_IsPrj )
{
	// local -------------------
		__u8*		Tv_Result;
		Ptr_GrUiScptImgTblHd	Tv_PtrImgTbl;
		__u32		Tv_WkIdx;
		__u32		Tv_GdibSize;
	// code --------------------
		//init
		Tv_Result			=	(__u8*)m_PtrRosImgData;
		Tv_PtrImgTbl	=	m_PtrRosImgTbl;

		if ( !A_IsPrj )
		{
			//find
			for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiDvlImgMaxCnt;Tv_WkIdx++ )
			{
				//check exist
				if ( NULL != m_SkinTbl[m_SkinIdx][Tv_WkIdx].Ptr )
				{
					//add
					Tv_GdibSize	=	m_SkinTbl[m_SkinIdx][Tv_WkIdx].Ptr->Size;
					GrDumyCopyMem( Tv_Result, m_SkinTbl[m_SkinIdx][Tv_WkIdx].Ptr, Tv_GdibSize );
					//patch address
					Tv_PtrImgTbl->Ptr	=	(Ptr_GrGdib)( (__u32)Tv_Result - (__u32)m_PtrRosBuf );
					//next
					Tv_PtrImgTbl ++;
					Tv_Result		=	Tv_Result + Tv_GdibSize;
				}
			}
		}

		return	(void*)Tv_Result;
}
//--------------------------------------------------------------------
void*	Cls_GrUiDvlMng::LcMkRosTxtData( void )
{
	// local -------------------
		__u8*		Tv_Result;
		Ptr_GrUiScptLangTxtHd	Tv_PtrTxtTbl;
		__u32		Tv_WkIdx;
		__u32		Tv_WKLng;
		__u32		Tv_TxtSize;
		WCHAR*	Tv_Str;
	// code --------------------
		//init
		Tv_Result			=	(__u8*)m_PtrRosTxtData;
		Tv_PtrTxtTbl	=	m_PtrRosTxtTbl;

		//find
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiDvlTextMaxCnt;Tv_WkIdx++ )
		{
			//check exist
			if ( E_GrUiTxtIdNone != m_LangTbl[Tv_WkIdx].Id )
			{
				//add
				for ( Tv_WKLng =0;Tv_WKLng < E_GrUiLanguageCnt;Tv_WKLng++ )
				{
					//check exist
					if ( NULL != m_LangTbl[Tv_WkIdx].LngPosTbl[Tv_WKLng] )
					{
						Tv_Str		=	m_LangTbl[Tv_WkIdx].LngPosTbl[Tv_WKLng];
						Tv_Str ++;
						Tv_TxtSize	=	(GrStrWlen( Tv_Str ) + 2) * 2;
						GrDumyCopyMem( Tv_Result, m_LangTbl[Tv_WkIdx].LngPosTbl[Tv_WKLng], Tv_TxtSize );
						//patch address
						Tv_PtrTxtTbl->LngPosTbl[Tv_WKLng]	=	(WCHAR*)( (__u32)Tv_Result - (__u32)m_PtrRosBuf );
						//next
						Tv_Result	=	Tv_Result + Tv_TxtSize;
					}
				}
				//next
				Tv_PtrTxtTbl ++;
			}
		}

		return	(void*)Tv_Result;
}
//--------------------------------------------------------------------
void*	Cls_GrUiDvlMng::LcMkRos( void* A_PtrBuf, BOOL8 A_IsPrj )
{
	// local -------------------
		void*		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	A_PtrBuf;
		m_PtrRosBuf	=	A_PtrBuf;

		//init file header
		m_PtrRosFh	=	(Ptr_GrUiScptHd)Tv_Result;
		m_PtrRosFh->Fcc				=	E_GrUiScptFhFcc;
		m_PtrRosFh->VerScpt		=	E_GrUiScptFhVer;
		m_PtrRosFh->VerUser		=	m_DvlScptHd.VerUser;
		m_PtrRosFh->IsAdrReloc	=	FALSE;
		
		m_PtrRosFh->Config.IsPltDisp	=	IsDispPlt();
		
		//color table update
		GrDumyCopyMem( m_PtrRosFh->Config.PltTbl, GrGdibGetBasePalletePtr(), (4*E_GrGdibBasePltCnt) );
		//update cursor color
		m_PtrRosFh->Config.ClrCus	=	m_UiCon->CursColorGet();

		//build object table
		Tv_Result		=	(void*)( (__u32)Tv_Result + sizeof(St_GrUiScptHd) );
		m_PtrRosLumpTbl		=	(Ptr_GrUiScptLumpHd)Tv_Result;
		m_PtrRosFontTbl		=	(Ptr_GrUiScptFontTblHd)LcMkRosLumpTbl();

		//build font table
		m_PtrRosImgTbl		=	(Ptr_GrUiScptImgTblHd)LcMkRosFontTbl();

		//build image table
		m_PtrRosTxtTbl		=	(Ptr_GrUiScptLangTxtHd)LcMkRosImgTbl( A_IsPrj );

		//build text table
		m_PtrRosObjScpt		=	LcMkRosTxtTbl();

		//build object script
		m_PtrRosFontData	=	LcMkRosLumpScpt();

		//buiod font data
		m_PtrRosImgData		=	LcMkRosFontData();

		//build image data
		m_PtrRosTxtData		=	LcMkRosImgData( A_IsPrj );

		//build text data
		Tv_Result	=	LcMkRosTxtData();

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDvlMng::SavePrj( WCHAR* A_StrFn )
{
	// local -------------------
		BOOL8		Tv_Result;
		void*		Tv_PtrLast;
		__u32		Tv_Size;
		Cls_GrFileCtrl*	Tv_ObjFl;
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		//check save able
		if ( IsPrjExist() )
		{
			//update project date
			LcUpdtPrjText();
			//allocate buffer
			m_PtrGrspBuf		=	malloc( E_GrUiDvlPrjMaxSize );
			if ( NULL != m_PtrGrspBuf )
			{
				//clear buffer
				GrDumyZeroMem( m_PtrGrspBuf, E_GrUiDvlPrjMaxSize );
				//build header
				m_PtrGrspFh	=	(Ptr_GrUiDvlFh)m_PtrGrspBuf;
				m_PtrGrspFh->FccFh			=	E_GrUiDvlPrjFcc;
				m_PtrGrspFh->Ver				=	E_GrUiDvlPrjVer;
				m_PtrGrspFh->FccDisp		=	PrjFccGet();
				m_PtrGrspFh->IsDispPlt	=	IsDispPlt();
				m_PtrGrspFh->QuadSizeX	=	V_GrUiDvlQuadSizeX;
				m_PtrGrspFh->QuadSizeY	=	V_GrUiDvlQuadSizeY;
				m_PtrGrspFh->QuadPalExt	=	V_GrUiDvlQuadPalExt;
				m_PtrGrspFh->SkinIdx		=	m_SkinIdx;
				GrDumyCopyMem( m_PtrGrspFh->ObjIdAlocTbl, m_ObjIdAlocTbl, sizeof(m_ObjIdAlocTbl) );
				GrDumyCopyMem( m_PtrGrspFh->ObjNameTbl, m_ObjNameTbl, sizeof(m_ObjNameTbl) );
				m_PtrGrspFh->FptrExt		=	sizeof(St_GrUiDvlFh);			//ver 2

				//object extender
				m_PtrGrspObjExt		=	(Ptr_GrUiDvlLumpExt)( (__u32)m_PtrGrspFh + m_PtrGrspFh->FptrExt );
				m_PtrGrspObjFold	=	(Ptr_GrUiDvlLumpFold)LcMkGrspLumpExt( );

				//object folder
				m_PtrGrspPfold		=	(Ptr_GrUiDvlPrjFoldInfo)LcMkGrspLumpFold();

				//project folder
				m_PtrGrspMacBuf		=	LcMkGrspPfold();

				//macro buffer
				m_PtrGrspSkin		=	LcMkGrspMac();

				//skin buffer
				m_PtrGrspFh->FptrSkin	=	(__u32)m_PtrGrspSkin - (__u32)m_PtrGrspBuf;
				m_PtrGrspFont		=	LcMkGrspSkin();

				//font buffer
				m_PtrGrspFh->FptrFont	=	(__u32)m_PtrGrspFont - (__u32)m_PtrGrspBuf;
				m_PtrGrspRos		=	LcMkGrspFont();

				//encode ros
				m_PtrGrspFh->FptrRos	=	(__u32)m_PtrGrspRos - (__u32)m_PtrGrspBuf;
				Tv_PtrLast	=	LcMkRos( m_PtrGrspRos, TRUE );

				//calculate size
				Tv_Size		=	(__u32)Tv_PtrLast - (__u32)m_PtrGrspBuf;

				//delete old file
				GrFileDelete( A_StrFn );
				//write
				Tv_ObjFl	=	(Cls_GrFileCtrl*)new Cls_GrFileCtrl( A_StrFn, TRUE, TRUE );
				if ( Tv_ObjFl->IsOpened() )
				{
					if ( Tv_Size == Tv_ObjFl->Write( m_PtrGrspBuf, Tv_Size ) )
					{
						Tv_Result	=	TRUE;
					}
				}

				delete	Tv_ObjFl;

				//release buffer
				free( m_PtrGrspBuf );
				m_PtrGrspBuf	=	NULL;
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDvlMng::LcUpGrspLumpExt( void )
{
	// local -------------------
		__u32		Tv_Size;
	// code --------------------
		//check exist obj
		Tv_Size		=	sizeof(St_GrUiDvlLumpExt) * m_PtrGrspFh->LumpCnt;
		if ( 0 != Tv_Size )
		{
			GrDumyCopyMem( m_LumpExtTbl, m_PtrGrspObjExt, Tv_Size );
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDvlMng::LcUpGrspLumpFold( void )
{
	// local -------------------
		__u32		Tv_Size;
	// code --------------------
		if ( NULL != m_PtrGrspObjFold )
		{
			Tv_Size		=	sizeof(St_GrUiDvlLumpFold) * m_PtrGrspFh->LumpCnt;
			if ( 0 != Tv_Size )
			{
				GrDumyCopyMem( (void*)m_LumpFoldTbl, m_PtrGrspObjFold, Tv_Size );
			}
		}
		else
		{
			GrDumyZeroMem( (void*)m_LumpFoldTbl, sizeof(m_LumpFoldTbl) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDvlMng::LcUpGrspPfold( void )
{
	// local -------------------
	// code --------------------
		if ( NULL != m_PtrGrspPfold )
		{
			GrDumyCopyMem( (void*)m_PfoldTbl, m_PtrGrspPfold, sizeof(m_PfoldTbl) );
		}
		else
		{
			GrDumyZeroMem( (void*)m_PfoldTbl, sizeof(m_PfoldTbl) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDvlMng::LcUpGrspMac( void )
{
	// local -------------------
		__u32		Tv_WkIdx;
		__u32		Tv_Size;
		void*		Tv_PtrScpt;
	// code --------------------
		//check exist macro
		if ( 0 != m_PtrGrspFh->MacCnt )
		{
			//updaet ext
			Tv_Size		=	sizeof(St_GrUiDvlMacExt) * m_PtrGrspFh->MacCnt;
			GrDumyCopyMem( m_MacExtTbl, m_PtrGrspMacExt, Tv_Size );
			//update obj
			for ( Tv_WkIdx=0;Tv_WkIdx < m_PtrGrspFh->MacCnt;Tv_WkIdx++  )
			{
				//update script
				m_MacTbl[Tv_WkIdx].Ptr		=	(Ptr_GrUiScptObjHd)malloc( E_GrUiDvlObjBufSize );
				if ( NULL != m_MacTbl[Tv_WkIdx].Ptr )
				{
					if ( 0 != m_MacExtTbl[Tv_WkIdx].Size )
					{
						Tv_PtrScpt	=	(void*)( (__u32)m_PtrGrspBuf + (__u32)m_PtrGrspMacObj[Tv_WkIdx].Ptr );
						GrDumyCopyMem( m_MacTbl[Tv_WkIdx].Ptr, Tv_PtrScpt, m_MacExtTbl[Tv_WkIdx].Size );
					}
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDvlMng::LcUpRosLump( void )
{
	// local -------------------
		__u32		Tv_WkIdx;
		void*		Tv_PtrScpt;
	// code --------------------
		//check exist obj
		if ( 0 != m_PtrRosFh->SctLump.Cnt )
		{
			for ( Tv_WkIdx = 0;Tv_WkIdx < m_PtrRosFh->SctLump.Cnt;Tv_WkIdx++ )
			{
				//update script	
				m_LumpTbl[Tv_WkIdx].Ptr		=	(Ptr_GrUiScptObjHd)malloc( E_GrUiDvlObjBufSize );
				Tv_PtrScpt	=	(void*)( (__u32)m_PtrRosBuf + (__u32)m_PtrRosLumpTbl[Tv_WkIdx].Ptr );
#ifndef LINUX_APP
				if ( E_GrUiDvlObjBufSize < m_LumpExtTbl[Tv_WkIdx].Size )
				{
					MessageBox( NULL, L"Lump size over!", L"Error!", MB_OK );
				}
#endif
				GrDumyCopyMem( m_LumpTbl[Tv_WkIdx].Ptr, Tv_PtrScpt, m_LumpExtTbl[Tv_WkIdx].Size );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDvlMng::LcUpRosFont( void )
{
	// local -------------------
		__u32		Tv_WkIdx;
		Ptr_GrUiFontV2Head	Tv_PtrFont;
		__u32		Tv_FontSize;
		Cls_GrUiScptDec*	Tv_ScptDec;
	// code --------------------
		//check project file version
		if ( 5 > m_PtrGrspFh->Ver )
		{
			if ( 0 != m_PtrRosFh->SctFont.Cnt )
			{
				for ( Tv_WkIdx=0;Tv_WkIdx < m_PtrRosFh->SctFont.Cnt;Tv_WkIdx++ )
				{
					m_FontTbl[m_SkinIdx][Tv_WkIdx].Id	=	m_PtrRosFontTbl[Tv_WkIdx].Id;
					Tv_PtrFont	=	(Ptr_GrUiFontV2Head)( (__u32)m_PtrRosBuf + (__u32)m_PtrRosFontTbl[Tv_WkIdx].Ptr );
					Tv_FontSize	=	GrUiFontCalcSize( Tv_PtrFont );
					m_FontTbl[m_SkinIdx][Tv_WkIdx].Ptr	=	(Ptr_GrUiFontV2Head)malloc( Tv_FontSize );
					GrDumyCopyMem( m_FontTbl[m_SkinIdx][Tv_WkIdx].Ptr, Tv_PtrFont, Tv_FontSize );
				}
			}
			//update font
			Tv_ScptDec	=	m_UiMng->ScptDecGet();
			Tv_ScptDec->DvlFontUpdt();
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDvlMng::LcUpRosImg( void )
{
	// local -------------------
		__u32				Tv_WkIdx;
		Ptr_GrGdib	Tv_PtrGdib;
		__u32				Tv_GdibSize;
	// code --------------------
		//check project file version
		if ( 4 > m_PtrGrspFh->Ver )
		{
			if ( 0 != m_PtrRosFh->SctImg.Cnt )
			{
				for ( Tv_WkIdx=0;Tv_WkIdx < m_PtrRosFh->SctImg.Cnt;Tv_WkIdx++ )
				{
					m_SkinTbl[0][Tv_WkIdx].Id	=	m_PtrRosImgTbl[Tv_WkIdx].Id;
					Tv_PtrGdib	=	(Ptr_GrGdib)( (__u32)m_PtrRosBuf + (__u32)m_PtrRosImgTbl[Tv_WkIdx].Ptr );
					Tv_GdibSize	=	Tv_PtrGdib->Size;
					m_SkinTbl[0][Tv_WkIdx].Ptr	=	(Ptr_GrGdib)malloc( Tv_GdibSize );
					GrDumyCopyMem( m_SkinTbl[0][Tv_WkIdx].Ptr, Tv_PtrGdib, Tv_GdibSize );
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDvlMng::LcUpRosTxt( void )
{
	// local -------------------
		__u32				Tv_WkIdx;
		__u32				Tv_WkLng;
		WCHAR*			Tv_StrSr;
		WCHAR*			Tv_StrTg;
	// code --------------------
		if ( 0 != m_PtrRosFh->SctText.Cnt )
		{
			for ( Tv_WkIdx=0;Tv_WkIdx < m_PtrRosFh->SctText.Cnt;Tv_WkIdx++ )
			{
				m_LangTbl[Tv_WkIdx].Id	=	m_PtrRosTxtTbl[Tv_WkIdx].Id;
				for ( Tv_WkLng=0;Tv_WkLng < E_GrUiLanguageCnt;Tv_WkLng++ )
				{
					//check exist text
					if ( NULL != m_PtrRosTxtTbl[Tv_WkIdx].LngPosTbl[Tv_WkLng] )
					{
						Tv_StrSr	=	(WCHAR*)( (__u32)m_PtrRosBuf + (__u32)m_PtrRosTxtTbl[Tv_WkIdx].LngPosTbl[Tv_WkLng] );
						m_LangTbl[Tv_WkIdx].LngPosTbl[Tv_WkLng]	=	(WCHAR*)malloc( E_GrUiDvlTextBufSize );
						Tv_StrTg	=	m_LangTbl[Tv_WkIdx].LngPosTbl[Tv_WkLng];
						//update font
						Tv_StrTg[0]	=	Tv_StrSr[0];
						//update contents
						Tv_StrSr ++;
						Tv_StrTg ++;
#ifndef LINUX_APP
						if ( (E_GrUiDvlTextBufSize / 2) <= GrStrWlen( Tv_StrSr ) )
						{
							MessageBox( NULL, L"Text length over 255 charactor.", L"ERROR" , MB_OK );
						}
#endif
						GrStrWcopy( Tv_StrTg, Tv_StrSr, (E_GrUiDvlTextBufSize / 2) -1 );
					}
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDvlMng::LcUpRos( void* A_PtrBuf )
{
	// local -------------------
	// code --------------------
		//init
		m_PtrRosBuf			=	A_PtrBuf;
		m_PtrRosFh			=	(Ptr_GrUiScptHd)m_PtrRosBuf;
		m_PtrRosLumpTbl	=	(Ptr_GrUiScptLumpHd)( (__u32)m_PtrRosBuf + (__u32)m_PtrRosFh->SctLump.PtrTbl );
		m_PtrRosFontTbl	=	(Ptr_GrUiScptFontTblHd)( (__u32)m_PtrRosBuf + (__u32)m_PtrRosFh->SctFont.PtrTbl );
		m_PtrRosImgTbl	=	(Ptr_GrUiScptImgTblHd)( (__u32)m_PtrRosBuf + (__u32)m_PtrRosFh->SctImg.PtrTbl );
		m_PtrRosTxtTbl	=	(Ptr_GrUiScptLangTxtHd)( (__u32)m_PtrRosBuf + (__u32)m_PtrRosFh->SctText.PtrTbl );

		//update user version
		m_DvlScptHd.VerUser	=	m_PtrRosFh->VerUser;
		//update pallete
		GrDumyCopyMem( GrGdibGetBasePalletePtr(), m_PtrRosFh->Config.PltTbl, (4*E_GrUiScptPltCnt) );
		//update cursor color
		m_UiCon->CursColorSet( m_PtrRosFh->Config.ClrCus );

		//update obj
		LcUpRosLump();

		//update font
		LcUpRosFont();

		//update image
		LcUpRosImg();

		//update text
		LcUpRosTxt();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDvlMng::LoadPrj( WCHAR* A_StrFn )
{
	// local -------------------
		BOOL8		Tv_Result;
		Cls_GrFileCtrl*	Tv_ObjFl;
		__u32		Tv_Size;
	// code --------------------
		//init
		Tv_Result	=	FALSE;
		m_PtrGrspBuf	=	NULL;

		//open file
		Tv_ObjFl	=	(Cls_GrFileCtrl*)new Cls_GrFileCtrl( A_StrFn, FALSE, FALSE );
		if ( Tv_ObjFl->IsOpened() )
		{
			Tv_Size		=	(__u32)Tv_ObjFl->GetSize();
			m_PtrGrspBuf	=	malloc( Tv_Size );
			if ( NULL != m_PtrGrspBuf )
			{
				Tv_ObjFl->Seek( 0 );
				if ( Tv_Size != Tv_ObjFl->Read( m_PtrGrspBuf, Tv_Size ) )
				{
					//read fail
					free( m_PtrGrspBuf );
					m_PtrGrspBuf	=	NULL;
				}
			}
		}
		delete	Tv_ObjFl;

		//check read success
		if ( NULL != m_PtrGrspBuf )
		{
			//decode address
			m_PtrGrspFh				=	(Ptr_GrUiDvlFh)m_PtrGrspBuf;

			//preinit
			m_PtrGrspSkin			=	NULL;
			m_PtrGrspFont			=	NULL;

			//check version
			switch( m_PtrGrspFh->Ver )
			{
				case	1:
					m_PtrGrspObjExt		=	(Ptr_GrUiDvlLumpExt)( (__u32)m_PtrGrspFh + E_GrUiDvlPrjHdSizeVer1 );
					m_PtrGrspObjFold	=	NULL;
					m_PtrGrspPfold		=	NULL;
					m_PtrGrspMacBuf		=	(void*)( (__u32)m_PtrGrspObjExt + ( sizeof(St_GrUiDvlLumpExt) * m_PtrGrspFh->LumpCnt ) );
					break;
				case	2:
					m_PtrGrspObjExt		=	(Ptr_GrUiDvlLumpExt)( (__u32)m_PtrGrspFh + m_PtrGrspFh->FptrExt );
					m_PtrGrspObjFold	=	NULL;
					m_PtrGrspPfold		=	NULL;
					m_PtrGrspMacBuf		=	(void*)( (__u32)m_PtrGrspObjExt + ( sizeof(St_GrUiDvlLumpExt) * m_PtrGrspFh->LumpCnt ) );
					break;
				case	3:
					m_PtrGrspObjExt		=	(Ptr_GrUiDvlLumpExt)( (__u32)m_PtrGrspFh + m_PtrGrspFh->FptrExt );
					m_PtrGrspObjFold	=	(Ptr_GrUiDvlLumpFold)( (__u32)m_PtrGrspObjExt + ( sizeof(St_GrUiDvlLumpExt) * m_PtrGrspFh->LumpCnt ) );
					m_PtrGrspPfold		=	(Ptr_GrUiDvlPrjFoldInfo)( (__u32)m_PtrGrspObjFold + ( sizeof(St_GrUiDvlLumpFold) * m_PtrGrspFh->LumpCnt ) );
					m_PtrGrspMacBuf		=	(void*)( (__u32)m_PtrGrspPfold + sizeof(m_PfoldTbl) );
					break;
				case	4:
					//version over 4
					m_PtrGrspObjExt		=	(Ptr_GrUiDvlLumpExt)( (__u32)m_PtrGrspFh + m_PtrGrspFh->FptrExt );
					m_PtrGrspObjFold	=	(Ptr_GrUiDvlLumpFold)( (__u32)m_PtrGrspObjExt + ( sizeof(St_GrUiDvlLumpExt) * m_PtrGrspFh->LumpCnt ) );
					m_PtrGrspPfold		=	(Ptr_GrUiDvlPrjFoldInfo)( (__u32)m_PtrGrspObjFold + ( sizeof(St_GrUiDvlLumpFold) * m_PtrGrspFh->LumpCnt ) );
					m_PtrGrspMacBuf		=	(void*)( (__u32)m_PtrGrspPfold + sizeof(m_PfoldTbl) );
					m_PtrGrspSkin			=	(void*)( (__u32)m_PtrGrspBuf + m_PtrGrspFh->FptrSkin );
					break;
				default:
					//version over 5
					m_PtrGrspObjExt		=	(Ptr_GrUiDvlLumpExt)( (__u32)m_PtrGrspFh + m_PtrGrspFh->FptrExt );
					m_PtrGrspObjFold	=	(Ptr_GrUiDvlLumpFold)( (__u32)m_PtrGrspObjExt + ( sizeof(St_GrUiDvlLumpExt) * m_PtrGrspFh->LumpCnt ) );
					m_PtrGrspPfold		=	(Ptr_GrUiDvlPrjFoldInfo)( (__u32)m_PtrGrspObjFold + ( sizeof(St_GrUiDvlLumpFold) * m_PtrGrspFh->LumpCnt ) );
					m_PtrGrspMacBuf		=	(void*)( (__u32)m_PtrGrspPfold + sizeof(m_PfoldTbl) );
					m_PtrGrspSkin			=	(void*)( (__u32)m_PtrGrspBuf + m_PtrGrspFh->FptrSkin );
					m_PtrGrspFont			=	(void*)( (__u32)m_PtrGrspBuf + m_PtrGrspFh->FptrFont );
					break;
			}
			
			m_PtrGrspMacExt		=	(Ptr_GrUiDvlMacExt)m_PtrGrspMacBuf;
			m_PtrGrspMacObj		=	(Ptr_GrUiScptLumpHd)( (__u32)m_PtrGrspMacExt + ( sizeof(St_GrUiDvlMacExt) * m_PtrGrspFh->MacCnt ) );
			m_PtrGrspMacScpt	=	(void*)( (__u32)m_PtrGrspMacObj + ( sizeof(St_GrUiScptLumpHd) * m_PtrGrspFh->MacCnt ) );
			m_PtrGrspRos			=	(void*)( (__u32)m_PtrGrspBuf + m_PtrGrspFh->FptrRos );

			//new project
			NewPrj( m_PtrGrspFh->FccDisp, m_PtrGrspFh->IsDispPlt );		//reset object

			//update info
			if ( 4 <= m_PtrGrspFh->Ver )
			{
				SkinIdxSet( m_PtrGrspFh->SkinIdx );
			}
			V_GrUiDvlQuadSizeX	=	m_PtrGrspFh->QuadSizeX;
			V_GrUiDvlQuadSizeY	=	m_PtrGrspFh->QuadSizeY;
			V_GrUiDvlQuadPalExt	=	m_PtrGrspFh->QuadPalExt;

			//update object id allocate table
			GrDumyCopyMem( m_ObjIdAlocTbl, m_PtrGrspFh->ObjIdAlocTbl, sizeof(m_ObjIdAlocTbl) );
			GrDumyCopyMem( m_ObjNameTbl, m_PtrGrspFh->ObjNameTbl, sizeof(m_ObjNameTbl) );

			//update object extention
			LcUpGrspLumpExt();

			//update lump folder
			LcUpGrspLumpFold();

			//update project folder
			LcUpGrspPfold();

			//update macro
			LcUpGrspMac();

			//update multi skin
			if ( 4 <= m_PtrGrspFh->Ver )
			{
				LcUpGrspSkin();
			}

			//update multi font
			if ( 5 <= m_PtrGrspFh->Ver )
			{
				LcUpGrspFont();
			}

			//update ros
			LcUpRos( m_PtrGrspRos );

			//success
			Tv_Result	=	TRUE;

			//release buffer
			free( m_PtrGrspBuf );
			m_PtrGrspBuf	=	NULL;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDvlMng::IsCopyObjExist( void )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( NULL != m_PtrCopy )
		{
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDvlMng::EditCopy( Cls_GrUiDoBase* A_Obj )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//check able object
		if ( NULL != A_Obj )
		{
			//allocate copy buffer
			//check not exist buffer
			if ( NULL == m_PtrCopy )
			{
				m_PtrCopy	=	(Ptr_GrUiScptObjHd)malloc( E_GrUiDvlObjBufSize );
			}
			if ( NULL != m_PtrCopy )
			{
				m_CopyExt.Size	=	LcEncCopyScript( A_Obj, m_PtrCopy );
				if ( 0 != m_CopyExt.Size )
				{
					Tv_Result		=	TRUE;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDvlMng::EditPast( Cls_GrUiDoBase* A_ObjPrnt )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//check able object
		if ( NULL != A_ObjPrnt )
		{
			//check buffer
			if ( NULL != m_PtrCopy )
			{
				Tv_Result	=	LcDecCopyScript( A_ObjPrnt, m_PtrCopy );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDvlMng::DelLump( __u16 A_Id )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u32		Tv_Idx;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//find object
		if ( FindLumpIdxById( A_Id, &Tv_Idx ) )
		{
			//remove
			LcLumpFreeByIdx( Tv_Idx );
			Tv_Result		=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDvlMng::MacSet( Cls_GrUiDoBase*	A_Obj, __u32 A_Idx, WCHAR* A_StrName )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result	=	FALSE;

		//check able index
		if ( E_GrUiDvlMacMaxCnt > A_Idx )
		{
			//check able object
			if ( NULL != A_Obj )
			{
				//check not exist buffer
				if ( NULL == m_MacTbl[A_Idx].Ptr )
				{
					m_MacTbl[A_Idx].Ptr	=	(Ptr_GrUiScptObjHd)malloc( E_GrUiDvlObjBufSize );
				}
				//check buffer
				if ( NULL != m_MacTbl[A_Idx].Ptr )
				{
					//encode
					GrStrWcopy( m_MacExtTbl[A_Idx].StrName, A_StrName, E_GrNameStrLen - 1 );
					m_MacExtTbl[A_Idx].Size	=	LcEncCopyScript( A_Obj, m_MacTbl[A_Idx].Ptr );
					Tv_Result	=	TRUE;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDvlMng::MacPast( Cls_GrUiDoBase* A_ObjPrnt, __u32 A_Idx )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result	=	FALSE;

		//check able index
		if ( E_GrUiDvlMacMaxCnt > A_Idx )
		{
			//check able object
			if ( NULL != A_ObjPrnt )
			{
				//check buffer
				if ( NULL != m_MacTbl[A_Idx].Ptr )
				{
					//decode
					Tv_Result	=	LcDecCopyScript( A_ObjPrnt, m_MacTbl[A_Idx].Ptr );
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDvlMng::ObjIdDelModeSet( BOOL8 A_IsDelMode )
{
		if ( NULL != m_UiCon )
		{
			m_UiCon->ObjIdDelAble( A_IsDelMode );
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDvlMng::LcEncCopyScript( Cls_GrUiDoBase* A_Obj, void* A_PtrScpt )
{
	// local -------------------
		__u32		Tv_Result;
		Ptr_GrUiScptObjHd	Tv_PtrScptHd;
		__u32		Tv_LumpIdx;
	// code --------------------
		//init
		Tv_Result		=	0;

		//check able object
		if ( NULL != A_Obj )
		{
			if ( A_Obj->IsWindow() )
			{
				//check page
				if ( IsPageLump( A_Obj->ObjIdGet() ) )
				{
					//page
					Tv_Result	=	m_ScptEnc->EncodeSlaveObj( A_Obj, A_PtrScpt );
				}
				else
				{
					//window
					Tv_Result	=	m_ScptEnc->EncodeObj( A_Obj, A_PtrScpt );
					Tv_PtrScptHd	=	(Ptr_GrUiScptObjHd)A_PtrScpt;
					//update layer
					if ( FindLumpIdxById( A_Obj->ObjIdGet(), &Tv_LumpIdx ) )
					{
						Tv_PtrScptHd->Layer	=	m_LumpTbl[Tv_LumpIdx].Ptr->Layer;
					}
				}
			}
			else
			{
				Tv_Result	=	m_ScptEnc->EncodeOneSlaveObj( A_Obj, A_PtrScpt );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDvlMng::LcDecCopyScript( Cls_GrUiDoBase* A_Obj, void* A_PtrScpt )
{
	// local -------------------
		BOOL8		Tv_Result;
		Ptr_GrUiScptObjHd	Tv_PtrScptHd;
		__u16		Tv_OrgObjId;
		St_GrRect	Tv_Rc;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//check able object
		if ( NULL != A_Obj )
		{
			//check widow script
			Tv_PtrScptHd	=	(Ptr_GrUiScptObjHd)A_PtrScpt;
			if ( E_GrUiDoTypeIdNone != Tv_PtrScptHd->TypeId )
			{
				//check window
				if ( A_Obj->IsWindow() )
				{
					//window write mode
					Tv_OrgObjId	=	A_Obj->ObjIdGet();
					if ( !IsPageLump( Tv_OrgObjId ) )
					{
						Tv_Result		=	m_UiMng->ScptDecByPtr( A_PtrScpt, A_Obj, TRUE );
						if ( Tv_Result )
						{
							//restore object id
							A_Obj->RtlObjIdSet( Tv_OrgObjId );
							Tv_Rc.left		=	Tv_PtrScptHd->Rect.left;
							Tv_Rc.top			=	Tv_PtrScptHd->Rect.top;
							Tv_Rc.right		=	Tv_PtrScptHd->Rect.right;
							Tv_Rc.bottom	=	Tv_PtrScptHd->Rect.bottom;
							A_Obj->RectSet( &Tv_Rc );		//replace position
							WinLayerChangeById( Tv_OrgObjId, Tv_PtrScptHd->Layer );
						}
					}
				}
			}
			else
			{
				//normal past
				Tv_Result		=	m_UiMng->ScptDecByPtr( A_PtrScpt, A_Obj, TRUE );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDvlMng::UserVersionSet( __u32 A_Ver )
{
		//set version
		m_DvlScptHd.VerUser	=	A_Ver;
}
//--------------------------------------------------------------------
void	Cls_GrUiDvlMng::NoneEditWinClose( void )
{
		m_UiCon->VirtKbdAllClose();
		m_UiCon->MsgBoxAllClose();
}	
//--------------------------------------------------------------------
void	Cls_GrUiDvlMng::OBjTextDel( __u16 A_Id )
{
	// local -------------------
		__u32		Tv_WkIdx;
		__u32		Tv_LngIdx;
		Ptr_GrUiScptLangTxtHd	Tv_PtrSlot;
	// code --------------------
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiDvlTextMaxCnt;Tv_WkIdx++ )
		{
			//check exist
			if ( (m_LangTbl[Tv_WkIdx].Id >> 16) == A_Id )
			{
				//find 
				Tv_PtrSlot	=	&m_LangTbl[Tv_WkIdx];
				for ( Tv_LngIdx=0;Tv_LngIdx < E_GrUiLanguageCnt;Tv_LngIdx++ )
				{
					//check exist
					if ( NULL != Tv_PtrSlot->LngPosTbl[Tv_LngIdx] )
					{
						free( Tv_PtrSlot->LngPosTbl[Tv_LngIdx] );
						Tv_PtrSlot->LngPosTbl[Tv_LngIdx]	=	NULL;
					}
				}
				Tv_PtrSlot->Id		=	E_GrUiTxtIdNone;
			}
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDvlMng::CursorColorGet( void )
{
	// local -------------------
		__u32		Tv_Result;
	// code --------------------
		Tv_Result	=	0;

		if ( NULL != m_UiCon )
		{
			Tv_Result	=	m_UiCon->CursColorGet();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDvlMng::CursorColorSet( __u32 A_Color )
{
		if ( NULL != m_UiCon )
		{
			m_UiCon->CursColorSet( A_Color );
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDvlMng::ExportRos( WCHAR* A_StrFn )
{
	// local -------------------
		BOOL8		Tv_Result;
		void*		Tv_PtrLast;
		__u32		Tv_Size;
		Cls_GrFileCtrl*	Tv_ObjFl;
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		//check save able
		if ( IsPrjExist() )
		{
			//update project date
			LcUpdtPrjText();
			//allocate buffer
			m_PtrGrspRos		=	malloc( E_GrUiDvlPrjMaxSize );
			if ( NULL != m_PtrGrspRos )
			{
				//clear buffer
				GrDumyZeroMem( m_PtrGrspRos, E_GrUiDvlPrjMaxSize );

				//encode ros
				Tv_PtrLast	=	LcMkRos( m_PtrGrspRos, FALSE );
				//calculate size
				Tv_Size		=	(__u32)Tv_PtrLast - (__u32)m_PtrGrspRos;

				//delete old file
				GrFileDelete( A_StrFn );
				//write
				Tv_ObjFl	=	(Cls_GrFileCtrl*)new Cls_GrFileCtrl( A_StrFn, TRUE, TRUE );
				if ( Tv_ObjFl->IsOpened() )
				{
					if ( Tv_Size == Tv_ObjFl->Write( m_PtrGrspRos, Tv_Size ) )
					{
						Tv_Result	=	TRUE;
					}
				}

				delete	Tv_ObjFl;

				//release buffer
				free( m_PtrGrspRos );
				m_PtrGrspRos	=	NULL;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDvlMng::ExportHead( WCHAR* A_StrFn )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u32		Tv_Size;
		__s8*		Tv_StrBuf;
		Def_StrNum	Tv_StrNum;
		Cls_GrFileCtrl*	Tv_ObjFl;
		__u16		Tv_WkIdx;
		Def_StrTag	Tv_StrTmp;
		__s8*		Tv_StrWk;				//working pointer
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		//check save able
		if ( IsPrjExist() )
		{
			//update project date
			LcUpdtPrjText();
			//allocate buffer
			Tv_StrBuf		=	(__s8*)malloc( E_GrUiDvlHeaderMaxSize );
			Tv_StrWk				=	Tv_StrBuf;
			if ( NULL != Tv_StrBuf )
			{
				//make header
				GrStrClear( Tv_StrBuf );
				//start comment set
				Tv_StrWk	=	GrStrCopy( Tv_StrWk, V_GrUiDvlMngStrHeaderStart );
				//add ros version
				Tv_StrWk	=	GrStrCopy( Tv_StrWk, "#define	E_GrRosVersion	" );
				GrStrIntToStr( Tv_StrNum, (__s32)V_GrUiMngObj->UserVersionGet() );
				Tv_StrWk	=	GrStrCopy( Tv_StrWk, Tv_StrNum );
				Tv_StrWk	=	GrStrCopy( Tv_StrWk, "\n\n" );
				//add contents start
				for ( Tv_WkIdx = 0;Tv_WkIdx < E_GrUiDvlObjMaxCnt;Tv_WkIdx++ )
				{
					//check exist object
					if ( IsObjIdExist( Tv_WkIdx + E_GrUiObjIdSysStart ) )
					{
						//check exist string
						if ( GrStrIsExist( m_ObjNameTbl[Tv_WkIdx] ) )
						{
							//make header
							Tv_StrWk	=	GrStrCopy( Tv_StrWk, "#define	E_" );
							GrStrWstrToStr( Tv_StrTmp, m_ObjNameTbl[Tv_WkIdx] );
							Tv_StrWk	=	GrStrCopy( Tv_StrWk, Tv_StrTmp );
							Tv_StrWk	=	GrStrCopy( Tv_StrWk, "		" );
							GrStrIntToStr( Tv_StrNum, (__s32)(Tv_WkIdx + E_GrUiObjIdSysStart) );
							Tv_StrWk	=	GrStrCopy( Tv_StrWk, Tv_StrNum );
							Tv_StrWk	=	GrStrCopy( Tv_StrWk, "\n" );
						}
					}
				}
				
				GrStrCopy( Tv_StrWk, "\n" );

				//calculate size
				Tv_Size		=	GrStrLen( Tv_StrBuf );
				//delete old file
				GrFileDelete( A_StrFn );
				//write
				Tv_ObjFl	=	(Cls_GrFileCtrl*)new Cls_GrFileCtrl( A_StrFn, TRUE, TRUE );
				if ( Tv_ObjFl->IsOpened() )
				{
					if ( Tv_Size == Tv_ObjFl->Write( Tv_StrBuf, Tv_Size ) )
					{
						Tv_Result	=	TRUE;
					}
				}

				delete	Tv_ObjFl;

				//release buffer
				free( Tv_StrBuf );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDvlMng::ExportText( WCHAR* A_StrFn )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u32		Tv_Size;
		WCHAR*	Tv_StrBuf;
		Def_WstrNum	Tv_StrNum;
		Cls_GrFileCtrl*	Tv_ObjFl;
		__s32			Tv_WkIdx;
		WCHAR*	Tv_StrWk;				//working pointer
		__s32			Tv_LngIdx;
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		//check save able
		if ( IsPrjExist() )
		{
			//update project date
			LcUpdtPrjText();
			//allocate buffer
			Tv_StrBuf		=	(WCHAR*)malloc( E_GrUiDvlTextMaxSize );
			Tv_StrWk		=	Tv_StrBuf;
			if ( NULL != Tv_StrBuf )
			{
				//make header
				GrStrClear( Tv_StrBuf );
				*Tv_StrWk	=	0xFEFF;				//unicode text mark
				Tv_StrWk ++;
				//add contents start
				for ( Tv_WkIdx = 0;Tv_WkIdx < E_GrUiDvlTextMaxCnt;Tv_WkIdx++ )
				{
					//check exist text
					if ( E_GrUiTxtIdNone != m_LangTbl[Tv_WkIdx].Id )
					{
						//make text id number
						GrStrDwordToHexWstr( Tv_StrNum, m_LangTbl[Tv_WkIdx].Id,8,TRUE );
						Tv_StrWk	=	GrStrWcopy( Tv_StrWk, Tv_StrNum );
						for ( Tv_LngIdx=0;Tv_LngIdx < E_GrUiLanguageCnt;Tv_LngIdx++ )
						{
							Tv_StrWk	=	GrStrWcopy( Tv_StrWk, E_GrUiDvlMngTextDlim );
							if ( NULL != m_LangTbl[Tv_WkIdx].LngPosTbl[Tv_LngIdx] )
							{
								//check exist string
								Tv_StrWk	=	GrStrWcopy( Tv_StrWk, m_LangTbl[Tv_WkIdx].LngPosTbl[Tv_LngIdx] );
							}
						}
						//next
						Tv_StrWk	=	GrStrWcopy( Tv_StrWk, L"\r\n" );
					}
				}
				GrStrWcopy( Tv_StrWk, L"\r\n" );

				//calculate size
				Tv_Size		=	(GrStrWlen( Tv_StrBuf ) + 1) << 1;
				//delete old file
				GrFileDelete( A_StrFn );
				//write
				Tv_ObjFl	=	(Cls_GrFileCtrl*)new Cls_GrFileCtrl( A_StrFn, TRUE, TRUE );
				if ( Tv_ObjFl->IsOpened() )
				{
					if ( Tv_Size == Tv_ObjFl->Write( Tv_StrBuf, Tv_Size ) )
					{
						Tv_Result	=	TRUE;
					}
				}

				delete	Tv_ObjFl;

				//release buffer
				free( Tv_StrBuf );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDvlMng::ImportText( WCHAR* A_StrFn )
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrFileCtrl*	Tv_ObjFl;
		WCHAR*	Tv_StrBuf;
		__u32	Tv_Size;
		__s32		Tv_Line;
		WCHAR*	Tv_StrWk;		//parsing position
		WCHAR*	Tv_StrWd;		//word start
		__u32		Tv_WdLen;		//word len
		BOOL8		Tv_IsLineFinsh;		//line finished
		__u32		Tv_TxtId;
		__u32		Tv_LngIdx;	//language index
		WCHAR		Tv_BkChar;
	// code --------------------
		Tv_Result		=	FALSE;

		//allocate buffer
		Tv_StrBuf	=	(WCHAR*)malloc(E_GrUiDvlTextMaxSize);
		if ( NULL != Tv_StrBuf )
		{
			//file load
			Tv_ObjFl	=	(Cls_GrFileCtrl*)new Cls_GrFileCtrl( A_StrFn, FALSE, FALSE );
			if ( Tv_ObjFl->IsOpened() )
			{
				Tv_Size	=	(__u32)Tv_ObjFl->GetSize();
				Tv_ObjFl->Seek( 0 );
				if ( Tv_Size == Tv_ObjFl->Read( Tv_StrBuf, Tv_Size ) )
				{
					//success load
					Tv_Result		=	TRUE;
					//parsing
					Tv_Line		=	1;
					Tv_StrWk	=	Tv_StrBuf;
					
					//check unicode text header
					if ( 0xFEFF == *Tv_StrWk )
					{
						Tv_StrWk++;
					}
					//parsing start
					while ( 0 != *Tv_StrWk )
					{
						//line
						Tv_IsLineFinsh	=	FALSE;
						//get text id
						//delete space
						while ( (0x20 == *Tv_StrWk) || (0x09 == *Tv_StrWk) )
						{
							if ( (0 == *Tv_StrWk) || (0x0D == *Tv_StrWk) || (0x0A == *Tv_StrWk) )
							{
								Tv_IsLineFinsh	=	TRUE;
								break;
							}
							Tv_StrWk++;
						}
						//get id
						if ( !Tv_IsLineFinsh )
						{
							Tv_StrWd	=	Tv_StrWk;
							Tv_WdLen	=	0;
							while ( ((L'0' <= *Tv_StrWk) && (L'9' >= *Tv_StrWk)) || ((L'A' <= *Tv_StrWk) && (L'F' >= *Tv_StrWk)) || ((L'a' <= *Tv_StrWk) && (L'f' >= *Tv_StrWk)) )
							{
								Tv_StrWk ++;
								Tv_WdLen ++;
							}
							if ( 0 != Tv_WdLen )
							{
								GrStrHexWstrToDword( Tv_StrWd, &Tv_TxtId, Tv_WdLen );
								//go next line
								while ( L';' != *Tv_StrWk )
								{
									if ( (0 == *Tv_StrWk) || (0x0D == *Tv_StrWk) || (0x0A == *Tv_StrWk) )
									{
										Tv_IsLineFinsh	=	TRUE;
										break;
									}
									Tv_StrWk++;
								}
								if ( !Tv_IsLineFinsh )
								{
									Tv_StrWk ++;		//skip ';'
									//set text
									for ( Tv_LngIdx=0;Tv_LngIdx < E_GrUiLanguageCnt;Tv_LngIdx++ )
									{
										//get text
										Tv_StrWd	=	Tv_StrWk;
										while ( L';' != *Tv_StrWk )
										{
											if ( (0 == *Tv_StrWk) || (0x0D == *Tv_StrWk) || (0x0A == *Tv_StrWk) )
											{
												Tv_IsLineFinsh	=	TRUE;
												break;
											}
											Tv_StrWk++;
										}
										//set text
										Tv_BkChar		=	*Tv_StrWk;
										*Tv_StrWk		=	0;		//mark finish
										TxtSet( Tv_TxtId, Tv_LngIdx, Tv_StrWd );
										*Tv_StrWk		=	Tv_BkChar;
										//next
										if ( Tv_IsLineFinsh )
										{
											break;
										}
										Tv_StrWk++;		//skip ';'
									}
								}
							}
						}
						//next
						Tv_Line ++;
						//go next line
						while ( 0x0A != *Tv_StrWk )
						{
							//check finished
							if ( 0 == *Tv_StrWk )
							{
								break;
							}
							Tv_StrWk++;
						}
						if ( 0x0A == *Tv_StrWk )
						{
							Tv_StrWk++;
						}
					}
					//parsing finished
				}
			}
			//release
			delete	Tv_ObjFl;
			free( Tv_StrBuf );
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDvlMng::QuadSetup( __u16 A_SizeX, __u16 A_SizeY, __u16 A_PalExt )
{
		V_GrUiDvlQuadSizeX	=	A_SizeX;
		V_GrUiDvlQuadSizeY	=	A_SizeY;
		V_GrUiDvlQuadPalExt	=	A_PalExt;
}
//--------------------------------------------------------------------
__u16	Cls_GrUiDvlMng::QuadSizeX( void )
{
		return	V_GrUiDvlQuadSizeX;
}
//--------------------------------------------------------------------
__u16	Cls_GrUiDvlMng::QuadSizeY( void )
{
		return	V_GrUiDvlQuadSizeY;
}
//--------------------------------------------------------------------
__u16	Cls_GrUiDvlMng::QuadPalExt( void )
{
		return	V_GrUiDvlQuadPalExt;
}
//--------------------------------------------------------------------
__u16	Cls_GrUiDvlMng::FindLumpInObj( __u16 A_ObjId )
{
	// local -------------------
		__u16	Tv_Result;
		Cls_GrUiScptDec*	Tv_ScptDec;
		__u32	Tv_WkIdx;
	// code --------------------
		Tv_Result	=	E_GrUiObjIdNone;
		Tv_ScptDec	=	m_UiMng->ScptDecGet();
		//find lump
		for ( Tv_WkIdx = 0;Tv_WkIdx < E_GrUiDvlLumpMaxCnt;Tv_WkIdx++ )
		{
			//check exist item
			if ( NULL != m_LumpTbl[Tv_WkIdx].Ptr )
			{
				//exist script
				if ( Tv_ScptDec->IsObjInScpt( A_ObjId, m_LumpTbl[Tv_WkIdx].Ptr ) )
				{
					//finded
					Tv_Result	=	m_LumpTbl[Tv_WkIdx].Ptr->ScptId;
					break;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void*	Cls_GrUiDvlMng::LcMkGrspFont( void )
{
	// local -------------------
		__u8*	Tv_Result;
		__u8	Tv_SkinIdx;
		__u32	Tv_FntIdx;
		__u32	Tv_FontSize;
	// code --------------------
		//init
		Tv_Result	=	(__u8*)m_PtrGrspFont;
		for ( Tv_SkinIdx=0;Tv_SkinIdx < E_GrUiDvlSkinMaxCnt;Tv_SkinIdx++ )
		{
			for ( Tv_FntIdx=0;Tv_FntIdx < E_GrUiDvlFontMaxCnt;Tv_FntIdx++ )
			{
				//init
				//m_PtrGrspFh->FontTbl[Tv_SkinIdx][Tv_FntIdx].Ptr	=	NULL;
				//check exist image
				if ( NULL != m_FontTbl[Tv_SkinIdx][Tv_FntIdx].Ptr )
				{
					//add
					m_PtrGrspFh->FontTbl[Tv_SkinIdx][Tv_FntIdx].Id	=	m_FontTbl[Tv_SkinIdx][Tv_FntIdx].Id;
					Tv_FontSize	=	GrUiFontCalcSize( m_FontTbl[Tv_SkinIdx][Tv_FntIdx].Ptr );
					GrDumyCopyMem( Tv_Result, m_FontTbl[Tv_SkinIdx][Tv_FntIdx].Ptr, Tv_FontSize );
					//patch address
					m_PtrGrspFh->FontTbl[Tv_SkinIdx][Tv_FntIdx].Ptr	=	
						(Ptr_GrUiFontV2Head)( (__u32)Tv_Result - (__u32)m_PtrGrspBuf );
					//next
					Tv_Result		=	Tv_Result + Tv_FontSize;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void*	Cls_GrUiDvlMng::LcMkGrspSkin( void )
{
	// local -------------------
		__u8*	Tv_Result;
		__u8	Tv_SkinIdx;
		__u32	Tv_ImgIdx;
		__u32	Tv_GdibSize;
	// code --------------------
		Tv_Result	=	(__u8*)m_PtrGrspSkin;
		for ( Tv_SkinIdx=0;Tv_SkinIdx < E_GrUiDvlSkinMaxCnt;Tv_SkinIdx++ )
		{
			for ( Tv_ImgIdx=0;Tv_ImgIdx < E_GrUiDvlImgMaxCnt;Tv_ImgIdx++ )
			{
				//check exist image
				if ( NULL != m_SkinTbl[Tv_SkinIdx][Tv_ImgIdx].Ptr )
				{
					//add data
					Tv_GdibSize	=	m_SkinTbl[Tv_SkinIdx][Tv_ImgIdx].Ptr->Size;
					GrDumyCopyMem( Tv_Result, m_SkinTbl[Tv_SkinIdx][Tv_ImgIdx].Ptr, Tv_GdibSize );
					//patch address
					m_PtrGrspFh->SkinTbl[Tv_SkinIdx][Tv_ImgIdx].Id	=	m_SkinTbl[Tv_SkinIdx][Tv_ImgIdx].Id;
					m_PtrGrspFh->SkinTbl[Tv_SkinIdx][Tv_ImgIdx].Ptr	=	
						(Ptr_GrGdib)( (__u32)Tv_Result - (__u32)m_PtrGrspBuf );
					//next
					Tv_Result		=	Tv_Result + Tv_GdibSize;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDvlMng::LcUpGrspSkin( void )
{
	// local -------------------
		__u8				Tv_SkinIdx;
		__u32				Tv_ImgIdx;
		Ptr_GrGdib	Tv_PtrGdib;
		__u32				Tv_GdibSize;
	// code --------------------
		//check project file version
		for ( Tv_SkinIdx=0;Tv_SkinIdx < E_GrUiDvlSkinMaxCnt;Tv_SkinIdx++ )
		{
			for ( Tv_ImgIdx=0;Tv_ImgIdx < E_GrUiDvlImgMaxCnt;Tv_ImgIdx++ )
			{
				//check exist
				if ( NULL != m_PtrGrspFh->SkinTbl[Tv_SkinIdx][Tv_ImgIdx].Ptr )
				{
					m_SkinTbl[Tv_SkinIdx][Tv_ImgIdx].Id	=	m_PtrGrspFh->SkinTbl[Tv_SkinIdx][Tv_ImgIdx].Id;
					Tv_PtrGdib	=	(Ptr_GrGdib)( (__u32)m_PtrGrspBuf + (__u32)m_PtrGrspFh->SkinTbl[Tv_SkinIdx][Tv_ImgIdx].Ptr );
					Tv_GdibSize	=	Tv_PtrGdib->Size;
					m_SkinTbl[Tv_SkinIdx][Tv_ImgIdx].Ptr	=	(Ptr_GrGdib)malloc( Tv_GdibSize );
					GrDumyCopyMem( m_SkinTbl[Tv_SkinIdx][Tv_ImgIdx].Ptr, Tv_PtrGdib, Tv_GdibSize );
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDvlMng::LcUpGrspFont( void )
{
	// local -------------------
		__u8				Tv_SkinIdx;
		__u32				Tv_FntIdx;
		Cls_GrUiScptDec*	Tv_ScptDec;
		Ptr_GrUiFontV2Head	Tv_PtrFont;
		__u32				Tv_FontSize;
	// code --------------------
		//check project file version
		for ( Tv_SkinIdx=0;Tv_SkinIdx < E_GrUiDvlSkinMaxCnt;Tv_SkinIdx++ )
		{
			for ( Tv_FntIdx=0;Tv_FntIdx < E_GrUiDvlFontMaxCnt;Tv_FntIdx++ )
			{
				if ( NULL != m_PtrGrspFh->FontTbl[Tv_SkinIdx][Tv_FntIdx].Ptr )
				{
					//check exist
					m_FontTbl[Tv_SkinIdx][Tv_FntIdx].Id	=	m_PtrGrspFh->FontTbl[Tv_SkinIdx][Tv_FntIdx].Id;
					Tv_PtrFont	=	(Ptr_GrUiFontV2Head)( (__u32)m_PtrGrspBuf + (__u32)m_PtrGrspFh->FontTbl[Tv_SkinIdx][Tv_FntIdx].Ptr );
					Tv_FontSize	=	GrUiFontCalcSize( Tv_PtrFont );
					m_FontTbl[Tv_SkinIdx][Tv_FntIdx].Ptr	=	(Ptr_GrUiFontV2Head)malloc( Tv_FontSize );
					GrDumyCopyMem( m_FontTbl[Tv_SkinIdx][Tv_FntIdx].Ptr, Tv_PtrFont, Tv_FontSize );
				}
			}
		}
		//update font
		Tv_ScptDec	=	m_UiMng->ScptDecGet();
		Tv_ScptDec->DvlFontUpdt();
}
//--------------------------------------------------------------------
void	Cls_GrUiDvlMng::SkinIdxSet( __u8 A_Idx )
{
	// local -------------------
		Cls_GrUiScptDec*	Tv_ScptDec;
	// code --------------------
		if ( E_GrUiDvlSkinMaxCnt > A_Idx )
		{
			m_SkinIdx	=	A_Idx;
			m_DvlScptHd.SctImg.PtrTbl		=	m_SkinTbl[m_SkinIdx];
			m_DvlScptHd.SctFont.PtrTbl	=	m_FontTbl[m_SkinIdx];
			//update font
			Tv_ScptDec	=	m_UiMng->ScptDecGet();
			Tv_ScptDec->DvlFontUpdt();
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDvlMng::SkinIdxGet( void )
{
		return	m_SkinIdx;
}
//--------------------------------------------------------------------
void	Cls_GrUiDvlMng::SkinCopy( __u8 A_TgIdx, __u8 A_SrIdx )
{
	// local -------------------
		__u32	Tv_ImgIdx;
		__u32	Tv_GdibSize;
	// code --------------------
		//check equal index
		if ( A_SrIdx != A_TgIdx )
		{
			if ( (E_GrUiDvlSkinMaxCnt > A_SrIdx) && (E_GrUiDvlSkinMaxCnt > A_TgIdx) )
			{
				for ( Tv_ImgIdx=0;Tv_ImgIdx < E_GrUiDvlImgMaxCnt;Tv_ImgIdx++ )
				{
					//release target image
					if ( NULL != m_SkinTbl[A_TgIdx][Tv_ImgIdx].Ptr )
					{
						free( m_SkinTbl[A_TgIdx][Tv_ImgIdx].Ptr );
						m_SkinTbl[A_TgIdx][Tv_ImgIdx].Ptr	=	NULL;
						m_SkinTbl[A_TgIdx][Tv_ImgIdx].Id	=	E_GrUiImgIdNone;
					}
					//copy
					if ( NULL != m_SkinTbl[A_SrIdx][Tv_ImgIdx].Ptr )
					{
						Tv_GdibSize	=	m_SkinTbl[A_SrIdx][Tv_ImgIdx].Ptr->Size;
						m_SkinTbl[A_TgIdx][Tv_ImgIdx].Ptr	=	(Ptr_GrGdib)malloc( Tv_GdibSize );
						if ( NULL != m_SkinTbl[A_TgIdx][Tv_ImgIdx].Ptr )
						{
							GrDumyCopyMem( m_SkinTbl[A_TgIdx][Tv_ImgIdx].Ptr, m_SkinTbl[A_SrIdx][Tv_ImgIdx].Ptr, Tv_GdibSize );
							m_SkinTbl[A_TgIdx][Tv_ImgIdx].Id	=	m_SkinTbl[A_SrIdx][Tv_ImgIdx].Id;
						}
					}
				}
			}
		}
}
//--------------------------------------------------------------------

