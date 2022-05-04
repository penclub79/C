/*
		display object font use

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<Ui/GrUiScptDec.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<Ui/GrUiConsole.h>
#include	<GrStrTool.h>
#include	<Ui/GrUiRtl.h>

//====================================================================
//local const

//====================================================================
//local type

//====================================================================
//local macro
#define m_ObjConsole		((Cls_GrUiConsole*)m_ObjCon)

//====================================================================
//local function

//====================================================================
//var

//====================================================================
//--------------------------------------------------------------------
Cls_GrUiScptDec::Cls_GrUiScptDec(  )
{
	// local -------------------
	// code --------------------
		//init
		m_PtrScpt		=	NULL;
		m_IsScptBrk	=	FALSE;

		//font table reset
		GrDumyZeroMem( (void*)m_FontTbl, sizeof(m_FontTbl) );
}
//--------------------------------------------------------------------
Cls_GrUiScptDec::~Cls_GrUiScptDec()
{
		LcFontTblReset();
}
//--------------------------------------------------------------------
void	Cls_GrUiScptDec::LcFontTblReset( void )
{
	// local -------------------
		__u32		Tv_WkIdx;
	// code --------------------
		//release all font
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiScptDecFontMax;Tv_WkIdx++ )
		{
			if ( NULL !=  m_FontTbl[Tv_WkIdx] )
			{
				delete	m_FontTbl[Tv_WkIdx];
				m_FontTbl[Tv_WkIdx]	=	NULL;
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiScptDec::LcScptRelocate( Ptr_GrUiScptHd A_PtrScript )
{
	// local -------------------
		__u32	Tv_WkIdx;
		Ptr_GrUiScptLumpHd	Tv_PtrObj;
		Ptr_GrUiScptFontTblHd	Tv_PtrFont;
		Ptr_GrUiScptImgTblHd	Tv_PtrImg;
		Ptr_GrUiScptLangTxtHd	Tv_PtrTxt;
		__u32	Tv_TxtIdx;
	// code --------------------
		//patch section header
		A_PtrScript->SctLump.PtrTbl		=	(Ptr_GrUiScptLumpHd)( (__u32)A_PtrScript + (__u32)A_PtrScript->SctLump.PtrTbl );
		A_PtrScript->SctFont.PtrTbl		=	(Ptr_GrUiScptFontTblHd)( (__u32)A_PtrScript + (__u32)A_PtrScript->SctFont.PtrTbl );
		A_PtrScript->SctImg.PtrTbl		=	(Ptr_GrUiScptImgTblHd)( (__u32)A_PtrScript + (__u32)A_PtrScript->SctImg.PtrTbl );
		A_PtrScript->SctText.PtrTbl		=	(Ptr_GrUiScptLangTxtHd)( (__u32)A_PtrScript + (__u32)A_PtrScript->SctText.PtrTbl );

		//patch object table
		if ( 0 != A_PtrScript->SctLump.Cnt )
		{
			Tv_PtrObj	=	A_PtrScript->SctLump.PtrTbl;
			for (	Tv_WkIdx = 0;Tv_WkIdx < A_PtrScript->SctLump.Cnt;Tv_WkIdx++ )
			{
				Tv_PtrObj[Tv_WkIdx].Ptr	=	(Ptr_GrUiScptObjHd)( (__u32)A_PtrScript + (__u32)Tv_PtrObj[Tv_WkIdx].Ptr );
			}
		}

		//patch font table
		if ( 0 != A_PtrScript->SctFont.Cnt )
		{
			Tv_PtrFont	=	A_PtrScript->SctFont.PtrTbl;
			for (	Tv_WkIdx = 0;Tv_WkIdx < A_PtrScript->SctFont.Cnt;Tv_WkIdx++ )
			{
				Tv_PtrFont[Tv_WkIdx].Ptr	=	(Ptr_GrUiFontV2Head)( (__u32)A_PtrScript + (__u32)Tv_PtrFont[Tv_WkIdx].Ptr );
			}
		}

		//patch image table
		if ( 0 != A_PtrScript->SctImg.Cnt )
		{
			Tv_PtrImg	=	A_PtrScript->SctImg.PtrTbl;
			for (	Tv_WkIdx = 0;Tv_WkIdx < A_PtrScript->SctImg.Cnt;Tv_WkIdx++ )
			{
				Tv_PtrImg[Tv_WkIdx].Ptr	=	(Ptr_GrGdib)( (__u32)A_PtrScript + (__u32)Tv_PtrImg[Tv_WkIdx].Ptr );
			}
		}

		//patch language
		if ( 0 != A_PtrScript->SctText.Cnt )
		{
			Tv_PtrTxt	=	A_PtrScript->SctText.PtrTbl;
			for (	Tv_WkIdx = 0;Tv_WkIdx < A_PtrScript->SctText.Cnt;Tv_WkIdx++ )
			{
				for ( Tv_TxtIdx=0;Tv_TxtIdx < E_GrUiScptLangCnt;Tv_TxtIdx++ )
				{
					//check exist
					if ( NULL != Tv_PtrTxt[Tv_WkIdx].LngPosTbl[Tv_TxtIdx] )
					{
						Tv_PtrTxt[Tv_WkIdx].LngPosTbl[Tv_TxtIdx]	=	(WCHAR*)( (__u32)A_PtrScript + (__u32)Tv_PtrTxt[Tv_WkIdx].LngPosTbl[Tv_TxtIdx] );
					}
				}
			}
		}
		
		//finished
		A_PtrScript->IsAdrReloc	=	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrUiScptDec::LcFontTblBuild( void )
{
	// local -------------------
		__u32		Tv_WkIdx;
		Ptr_GrUiScptFontTblHd	Tv_PtrFont;
	// code --------------------
		//check able
		if ( 0 != m_PtrScpt->SctFont.Cnt )
		{
			for ( Tv_WkIdx = 0;Tv_WkIdx < m_PtrScpt->SctFont.Cnt;Tv_WkIdx++ )
			{
				if ( NULL != m_PtrScpt->SctFont.PtrTbl[Tv_WkIdx].Ptr )
				{
					//init
					Tv_PtrFont	=	&m_PtrScpt->SctFont.PtrTbl[Tv_WkIdx];
					//create font object
					m_FontTbl[Tv_PtrFont->Id]	=	(Cls_GrUiFontObj*)new Cls_GrUiFontObj( Tv_PtrFont->Ptr );
				}
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiScptDec::ScriptSet( Ptr_GrUiScptHd A_PtrScript )
{
	// local -------------------
		BOOL8		Tv_Result;		
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		m_IsScptBrk	=	FALSE;
		m_PtrScpt		=	A_PtrScript;
		//reset
		LcFontTblReset();
		//check clear
		if ( NULL != m_PtrScpt )
		{
			//check decoded
			if ( !m_PtrScpt->IsAdrReloc	)
			{
				//address relocate
				LcScptRelocate( m_PtrScpt );
			}
			//update font
			LcFontTblBuild();
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
Cls_GrUiFontObj*	Cls_GrUiScptDec::FontGet( __u8 A_Id )
{
		return	m_FontTbl[A_Id];		
}
//--------------------------------------------------------------------
Ptr_GrGdib	Cls_GrUiScptDec::ImgGet( __u16 A_Id )
{
	// local -------------------
		Ptr_GrGdib	Tv_Result;
		__u32		Tv_WkIdx;
		Ptr_GrUiScptImgTblHd	Tv_PtrImg;
	// code --------------------
		//init
		Tv_Result	=	NULL;
		//check able
		if ( (0 != m_PtrScpt->SctImg.Cnt) && (E_GrUiImgIdNone != A_Id) )
		{
			Tv_PtrImg		=	m_PtrScpt->SctImg.PtrTbl;
			for ( Tv_WkIdx=0;Tv_WkIdx < m_PtrScpt->SctImg.Cnt;Tv_WkIdx++ )
			{
				//check match id
				if ( (A_Id == Tv_PtrImg->Id) && ( NULL != Tv_PtrImg->Ptr ) )
				{
					//finded
					Tv_Result		=	Tv_PtrImg->Ptr;
					break;
				}
				//next
				Tv_PtrImg ++;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiScptDec::TextGet( __u32 A_Lang, __u32 A_Id, WCHAR* A_StrRt, __u32 A_LenMax )
{
	// local -------------------
		BOOL8		Tv_Result;
		WCHAR*	Tv_Str;
	// code --------------------
		//init
		Tv_Result	=	FALSE;
		GrStrClear( A_StrRt );
		Tv_Str	=	TextPtrGet( A_Lang, A_Id );
		if ( NULL != Tv_Str )
		{
			GrStrWcopy( A_StrRt, Tv_Str, A_LenMax );
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrUiScptDec::TextPtrGet( __u32 A_Lang, __u32 A_Id )
{
	// local -------------------
		WCHAR*	Tv_Result;
		__u32		Tv_WkIdx;
		Ptr_GrUiScptLangTxtHd	Tv_PtrTxt;
	// code --------------------
		//init
		Tv_Result	=	NULL;
		//check script exist
		if ( NULL != m_PtrScpt )
		{
			//find text
			if ( 0 != m_PtrScpt->SctText.Cnt )
			{
				Tv_PtrTxt		=	m_PtrScpt->SctText.PtrTbl;
				for ( Tv_WkIdx=0;Tv_WkIdx < m_PtrScpt->SctText.Cnt;Tv_WkIdx++ )
				{
					//check match id
					if ( A_Id == Tv_PtrTxt->Id )
					{
						//check exist language
						if ( NULL != Tv_PtrTxt->LngPosTbl[A_Lang] )
						{
							Tv_Result	=	Tv_PtrTxt->LngPosTbl[A_Lang];
						}
						break;
					}
					//next
					Tv_PtrTxt ++;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrUiScptDec::LcDecParaSet( Cls_GrUiDoBase* A_Obj, Ptr_GrUiScptObjHd A_PtrScpt, BOOL8 A_IsAlocId )
{
	// local -------------------
		__u32				Tv_Result;
		Ptr_GrUiScptValHd	Tv_PtrValHd;
		void*				Tv_PtrValData;
		__u32				Tv_WkIdx;
		__u8				Tv_ValBuf[E_GrUiScptValMaxSize];
		__u32				Tv_CpIdx;
		__u8*				Tv_PtrCp;
	// code --------------------
		//init
		Tv_Result	=	0;

		//set script id
		if ( !A_IsAlocId )
		{
			A_Obj->RtlObjIdSet( A_PtrScpt->ScptId );
		}
		//set para
		Tv_Result			=	sizeof(St_GrUiScptObjHd);
		if ( 0 != A_PtrScpt->ValCnt )
		{
			Tv_PtrValHd		=	(Ptr_GrUiScptValHd)( (__u32)A_PtrScpt + sizeof(St_GrUiScptObjHd) );
			Tv_PtrValData	=	(void*)( (__u32)Tv_PtrValHd + sizeof(St_GrUiScptValHd) );
			for ( Tv_WkIdx=0;Tv_WkIdx < A_PtrScpt->ValCnt;Tv_WkIdx++ )
			{
				//update value
				Tv_PtrCp	=	(__u8*)Tv_PtrValData;
				for ( Tv_CpIdx=0;Tv_CpIdx < Tv_PtrValHd->Size;Tv_CpIdx++ )
				{
					Tv_ValBuf[Tv_CpIdx]	=	*Tv_PtrCp;
					Tv_PtrCp++;
				}
				GrUiRtlParaSetVal( A_Obj, Tv_PtrValHd->ParaId, (void*)Tv_ValBuf );
				//next
				Tv_Result			=	Tv_Result + sizeof(St_GrUiScptValHd) + Tv_PtrValHd->Size;
				Tv_PtrValHd		=	(Ptr_GrUiScptValHd)( (__u32)Tv_PtrValHd + sizeof(St_GrUiScptValHd) + Tv_PtrValHd->Size );
				Tv_PtrValData	=	(void*)( (__u32)Tv_PtrValHd + sizeof(St_GrUiScptValHd) );
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrUiScptDec::LcDecScpt( void* A_PtrScpt, Cls_GrUiDoBase* A_ObjPrnt, BOOL8 A_IsAlocId )
{
	// local -------------------
		__u32	Tv_Result;
		Ptr_GrUiScptObjHd	Tv_PtrOh;		//object head
		Cls_GrUiDoBase*		Tv_Obj;			//created object
		__u32		Tv_Deced;
		__u8*		Tv_PtrDec;						//sub decode pointer
		St_GrRect	Tv_Rc;
	// code --------------------
		//init
		Tv_Deced	=	0;
		Tv_PtrOh	=	(Ptr_GrUiScptObjHd)A_PtrScpt;
		//create object
		Tv_Rc.left		=	Tv_PtrOh->Rect.left;
		Tv_Rc.top			=	Tv_PtrOh->Rect.top;
		Tv_Rc.right		=	Tv_PtrOh->Rect.right;
		Tv_Rc.bottom	=	Tv_PtrOh->Rect.bottom;
		Tv_Obj		=	GrUiRtlCreateObj( Tv_PtrOh->TypeId, &Tv_Rc, A_ObjPrnt, A_IsAlocId );
		if ( NULL != Tv_Obj )
		{
			Tv_Deced	=	LcDecParaSet( Tv_Obj, Tv_PtrOh, A_IsAlocId );
			Tv_Result	=	Tv_Deced;
			Tv_PtrDec	=	(__u8*)( (__u32)A_PtrScpt + Tv_Deced );
			//make slave
			while ( E_GrUiScptObjEndId != *Tv_PtrDec )
			{
				//check begin
				if ( E_GrUiScptObjBeginId == *Tv_PtrDec )
				{
					//slave object decode
					Tv_Deced	=	LcDecScpt( (void*)Tv_PtrDec, Tv_Obj, A_IsAlocId );
					//next
					Tv_Result	=	Tv_Result + Tv_Deced;
					Tv_PtrDec	=	(__u8*)( (__u32)Tv_PtrDec + Tv_Deced );
				}
				else
				{
					m_IsScptBrk	=	TRUE;
					break;
				}
				//check break script
				if ( m_IsScptBrk )
				{
					break;
				}
			}
			//add finish
			if ( !m_IsScptBrk )
			{
				Tv_Result	=	Tv_Result + 1;	//finish mark 
			}
		}
		else
		{
			m_IsScptBrk	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void*	Cls_GrUiScptDec::ObjScriptGet( __u16 A_ScptId )
{
	// local -------------------
		void*		Tv_Result;
		__u32		Tv_WkIdx;
		Ptr_GrUiScptLumpHd	Tv_PtrTbl;
	// code --------------------
		//init
		Tv_Result	=	NULL;

		//check exist script
		if ( NULL != m_PtrScpt )
		{
			//check exist item
			if ( 0 != m_PtrScpt->SctLump.Cnt )
			{
				Tv_PtrTbl	=	m_PtrScpt->SctLump.PtrTbl;
				for ( Tv_WkIdx = 0;Tv_WkIdx < m_PtrScpt->SctLump.Cnt;Tv_WkIdx++ )
				{
					//check exist script
					if ( NULL != Tv_PtrTbl[Tv_WkIdx].Ptr )
					{
						if ( A_ScptId == Tv_PtrTbl[Tv_WkIdx].Ptr->ScptId )
						{
							//finde
							Tv_Result	=	Tv_PtrTbl[Tv_WkIdx].Ptr;
							break;
						}
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiScptDec::ObjAtbGet( __u16 A_ScptId, __u8* A_PtrRtLayer, Ptr_GrRect A_PtrRtRect, BOOL8* A_PtrRtIsSlave )
{
	// local -------------------
		BOOL8		Tv_Result;
		Ptr_GrUiScptObjHd		Tv_PtrScpt;
	// code --------------------
		//init
		Tv_Result	=	FALSE;

		Tv_PtrScpt	=	(Ptr_GrUiScptObjHd)ObjScriptGet( A_ScptId );
		if ( NULL != Tv_PtrScpt )
		{
			*A_PtrRtLayer			=	Tv_PtrScpt->Layer;
			A_PtrRtRect->left		=	Tv_PtrScpt->Rect.left;
			A_PtrRtRect->top		=	Tv_PtrScpt->Rect.top;
			A_PtrRtRect->right	=	Tv_PtrScpt->Rect.right;
			A_PtrRtRect->bottom	=	Tv_PtrScpt->Rect.bottom;
			*A_PtrRtIsSlave		=	FALSE;
			//check slave
			if ( E_GrUiDoTypeIdNone == Tv_PtrScpt->TypeId )
			{
				*A_PtrRtIsSlave		=	TRUE;
			}
			Tv_Result	=	TRUE;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiScptDec::DecScript( Cls_GrUiDoBase* A_ObjBase, void* A_PtrScpt, BOOL8 A_IsAlocId )
{
	// local -------------------
		BOOL8		Tv_Result;
		Ptr_GrUiScptObjHd	Tv_PtrScpt;
		__u32		Tv_Deced;
	// code --------------------
		//init
		Tv_Result		=	TRUE;
		m_IsScptBrk	=	FALSE;
		Tv_PtrScpt	=	(Ptr_GrUiScptObjHd)A_PtrScpt;
		//check current head
		if ( E_GrUiScptObjBeginId == Tv_PtrScpt->BeginId )
		{
			//check slave
			if ( E_GrUiDoTypeIdNone == Tv_PtrScpt->TypeId )
			{
				Tv_Deced	=	sizeof(St_GrUiScptObjHd);
			}
			else
			{
				//check match type
				if ( A_ObjBase->TypeIdGet() == Tv_PtrScpt->TypeId )
				{
					Tv_Deced	=	LcDecParaSet( A_ObjBase, Tv_PtrScpt, A_IsAlocId );
				}
				else
				{
					//bad object type
					m_IsScptBrk	=	TRUE;
				}
			}
			//next
			Tv_PtrScpt	=	(Ptr_GrUiScptObjHd)( (__u32)Tv_PtrScpt + Tv_Deced );
			while ( (E_GrUiScptObjEndId != Tv_PtrScpt->BeginId) && (!m_IsScptBrk) )
			{
				//check script begin
				if ( E_GrUiScptObjBeginId == Tv_PtrScpt->BeginId )
				{
					Tv_Deced	=	LcDecScpt( (void*)Tv_PtrScpt, A_ObjBase, A_IsAlocId );
					//next
					Tv_PtrScpt	=	(Ptr_GrUiScptObjHd)( (__u32)Tv_PtrScpt + Tv_Deced );
				}
				else
				{
					//bad header fcc
					m_IsScptBrk	=	TRUE;
				}
			}
		}
		else
		{
			m_IsScptBrk	=	TRUE;
		}
		//update result
		if ( m_IsScptBrk )
		{
			Tv_Result		=	FALSE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiScptDec::IsObjInScpt( __u16 A_ScptId, void* A_PtrScpt )
{
	// local -------------------
		BOOL8		Tv_Result;
		Ptr_GrUiScptObjHd	Tv_PtrScpt;
		__u32		Tv_Deced;
		__u32		Tv_Level;
		Ptr_GrUiScptValHd	Tv_PtrValHd;
		__u32		Tv_WkIdx;
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		m_IsScptBrk	=	FALSE;
		Tv_PtrScpt	=	(Ptr_GrUiScptObjHd)A_PtrScpt;
		//
		Tv_Level		=	0;
		while ( (E_GrUiScptObjBeginId == Tv_PtrScpt->BeginId) || (0 != Tv_Level) )
		{
			//check begin id
			if ( E_GrUiScptObjBeginId == Tv_PtrScpt->BeginId )
			{
				//increase level
				Tv_Level ++;
				//check match id
				if ( A_ScptId == Tv_PtrScpt->ScptId )
				{
					//finded
					Tv_Result	=	TRUE;
					break;
				}
				//skip parameter area
				Tv_Deced			=	sizeof(St_GrUiScptObjHd);
				if ( 0 != Tv_PtrScpt->ValCnt )
				{
					Tv_PtrValHd		=	(Ptr_GrUiScptValHd)( (__u32)Tv_PtrScpt + sizeof(St_GrUiScptObjHd) );
					for ( Tv_WkIdx=0;Tv_WkIdx < Tv_PtrScpt->ValCnt;Tv_WkIdx++ )
					{
						//next
						Tv_Deced			=	Tv_Deced + sizeof(St_GrUiScptValHd) + Tv_PtrValHd->Size;
						Tv_PtrValHd		=	(Ptr_GrUiScptValHd)( (__u32)Tv_PtrValHd + sizeof(St_GrUiScptValHd) + Tv_PtrValHd->Size );
					}
				}
				//next script
				Tv_PtrScpt	=	(Ptr_GrUiScptObjHd)( (__u32)Tv_PtrScpt + Tv_Deced );
			}
			//check finished
			if ( E_GrUiScptObjEndId == Tv_PtrScpt->BeginId )
			{
				//decrease level
				Tv_Level --;
				Tv_PtrScpt	=	(Ptr_GrUiScptObjHd)( (__u32)Tv_PtrScpt + 1 );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiScptDec::DecById( Cls_GrUiDoBase* A_ObjBase, __u16 A_ScptId )
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrUiScptObjHd	Tv_PtrScpt;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		Tv_PtrScpt	=	(Ptr_GrUiScptObjHd)ObjScriptGet( A_ScptId );
		if ( NULL != Tv_PtrScpt )
		{
			//decode
			Tv_Result	=	DecScript( A_ObjBase, (void*)Tv_PtrScpt );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiScptDec::DvlFontUpdt( void )
{
		LcFontTblReset();
		LcFontTblBuild();
}
//--------------------------------------------------------------------
__u32	Cls_GrUiScptDec::DvlImgIdFindByPtr( Ptr_GrGdib A_PtrGdib )
{
	// local -------------------
		__u32		Tv_Result;
		__u32		Tv_WkIdx;
		__u32		Tv_WkCnt;
		Ptr_GrUiScptImgTblHd	Tv_PtrTbl;
	// code --------------------
		Tv_Result		=	E_GrUiImgIdNone;

		//check exist image
		Tv_WkCnt	=	m_PtrScpt->SctImg.Cnt;
		if ( (0 != Tv_WkCnt) && (NULL != A_PtrGdib) )
		{
			//find
			Tv_PtrTbl	=	m_PtrScpt->SctImg.PtrTbl;
			for ( Tv_WkIdx=0;Tv_WkIdx < Tv_WkCnt;Tv_WkIdx++ )
			{
				//check
				if ( A_PtrGdib == Tv_PtrTbl->Ptr )
				{
					//finded
					Tv_Result		=	Tv_PtrTbl->Id;
					break;
				}
									
				//next
				Tv_PtrTbl ++;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32*	Cls_GrUiScptDec::PalleteTblGet( void )
{
	// local -------------------
		__u32*	Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	NULL;

		if ( NULL != m_PtrScpt )
		{
			Tv_Result	=	m_PtrScpt->Config.PltTbl;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiScptDec::IsDispPltMode( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( NULL != m_PtrScpt )
		{
			Tv_Result	=	m_PtrScpt->Config.IsPltDisp;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrUiScptDec::UserVersionGet( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	0;

		if ( NULL != m_PtrScpt )
		{
			Tv_Result	=	m_PtrScpt->VerUser;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrUiScptDec::CursorColorGet( void )
{
	// local -------------------
		__u32		Tv_Result;
	// code --------------------
		Tv_Result	=	0;

		if ( NULL != m_PtrScpt )
		{
			Tv_Result	=	m_PtrScpt->Config.ClrCus;
		}
	
		return	Tv_Result;
}
//--------------------------------------------------------------------

