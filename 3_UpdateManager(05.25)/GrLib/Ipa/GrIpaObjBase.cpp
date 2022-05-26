/*
		image process adaptor object base

 */

//====================================================================
// uses
#include	<Ipa/GrIpaObjBase.h>
#include	<GrDebug.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<GrStrTool.h>
#include <Ipa/GrIpaMng.h>

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

extern	Cls_GrIpaMng*	V_GrIpaMng;

//====================================================================
//--------------------------------------------------------------------
Cls_GrIpaObjBase::Cls_GrIpaObjBase( void )
{
	// local -------------------
	// code --------------------
		//init
		m_AdaptorType	=	E_GrIpaAdptTypeNone;

		m_IsSource		=	FALSE;
		m_IsMonitor		=	FALSE;

		m_ConsentCnt	=	0;
		GrDumyZeroMem( m_ConsentBindTbl, sizeof(m_ConsentBindTbl) );

		m_PlugCnt			=	0;
		GrDumyZeroMem( m_PlugBindTbl, sizeof(m_PlugBindTbl) );

#ifdef GR_IPA_EDIT
		GrDumyZeroMem( m_ExtraData, sizeof(m_ExtraData) );
#endif
}
//--------------------------------------------------------------------
Cls_GrIpaObjBase::~Cls_GrIpaObjBase( void )
{
	// local -------------------
	// code --------------------


}
//--------------------------------------------------------------------
#ifdef GR_IPA_EDIT
void	Cls_GrIpaObjBase::LcParaSubEnc( void )
{

}
#endif
//--------------------------------------------------------------------
__u16	Cls_GrIpaObjBase::AdaptorType( void )
{
		return	m_AdaptorType;
}
//--------------------------------------------------------------------
__u16	Cls_GrIpaObjBase::ScptIdGet( void )
{
		return	m_ScptId;
}
//--------------------------------------------------------------------
void	Cls_GrIpaObjBase::ScptIdSet( __u16 A_Id )
{
		m_ScptId	=	A_Id;
}
//--------------------------------------------------------------------
__u32	Cls_GrIpaObjBase::ConsentCount( void )
{
		return	m_ConsentCnt;
}
//--------------------------------------------------------------------
Cls_GrIpaObjBase*	Cls_GrIpaObjBase::ConsentGetConnObj( __u32 A_ConsentIdx, __u32* A_PtrRtPlugIdx )
{
	// local -------------------
		Cls_GrIpaObjBase*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;

		if ( m_ConsentCnt > A_ConsentIdx )
		{
			*A_PtrRtPlugIdx	=	m_ConsentBindTbl[A_ConsentIdx].BindIdx;
			Tv_Result	=	(Cls_GrIpaObjBase*)m_ConsentBindTbl[A_ConsentIdx].Adaptor;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrIpaObjBase::PlugCount( void )
{
		return	m_PlugCnt;
}
//--------------------------------------------------------------------
#ifdef GR_IPA_EDIT
WCHAR*	Cls_GrIpaObjBase::AdaptorName( void )
{
		return	GrIpaAdoptNameGet( m_AdaptorType );
}
#endif
//--------------------------------------------------------------------
#ifdef GR_IPA_EDIT
void*	Cls_GrIpaObjBase::ExtraDataGet( void )
{
		return	(void*)m_ExtraData;
}
#endif
//--------------------------------------------------------------------
Cls_GrIpaObjBase*	Cls_GrIpaObjBase::PlugGetConnObj( __u32 A_PlugIdx, __u32* A_PtrRtConsentIdx )
{
	// local -------------------
		Cls_GrIpaObjBase*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;

		if ( m_PlugCnt > A_PlugIdx )
		{
			*A_PtrRtConsentIdx	=	m_PlugBindTbl[A_PlugIdx].BindIdx;
			Tv_Result	=	(Cls_GrIpaObjBase*)m_PlugBindTbl[A_PlugIdx].Adaptor;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
#ifdef GR_IPA_EDIT
__u32	Cls_GrIpaObjBase::ParaEnc( void* A_PtrBuf )
{
	// local -------------------
		Ptr_GrIpaScptObjHead	Tv_PtrObjHd;
	// code --------------------
		m_PtrEnc		=	A_PtrBuf;
		m_EncPrgs		=	0;

		// build header
		Tv_PtrObjHd	=	(Ptr_GrIpaScptObjHead)( (__u32)m_PtrEnc + m_EncPrgs );
		Tv_PtrObjHd->Identify	=	E_GrIpaScptObjFcc;
		Tv_PtrObjHd->Type			=	m_AdaptorType;
		Tv_PtrObjHd->ScptId		=	m_ScptId;
		Tv_PtrObjHd->Size			=	sizeof(St_GrIpaScptObjHead);

		m_EncPrgs	=	m_EncPrgs + sizeof(St_GrIpaScptObjHead);

		// add parametter
		LcParaSubEnc();

		// update total
		Tv_PtrObjHd->Size		=	(__u16)m_EncPrgs;

		return	m_EncPrgs;
}
#endif
//--------------------------------------------------------------------
#ifdef GR_IPA_EDIT
__u32	Cls_GrIpaObjBase::ChainEnc( void* A_PtrBuf )
{
	// local -------------------
		__u32	Tv_Result;
		__u32	Tv_WkIdx;
		Ptr_GrIpaScptChainHead	Tv_PtrChHd;
		Ptr_GrIpaScptChainItem	Tv_PtrChItm;
		Cls_GrIpaObjBase*	Tv_ObjTg;
	// code --------------------
		Tv_PtrChHd	=	(Ptr_GrIpaScptChainHead)A_PtrBuf;
		Tv_PtrChHd->Identify	=	E_GrIpaScptChainFcc;
		Tv_PtrChHd->ScptId		=	m_ScptId;
		Tv_PtrChHd->Size			=	sizeof(St_GrIpaScptChainHead);
		Tv_Result		=	sizeof(St_GrIpaScptChainHead);

		if ( 0 != m_PlugCnt )
		{
			for (Tv_WkIdx=0;Tv_WkIdx < m_PlugCnt;Tv_WkIdx++)
			{
				if ( NULL != m_PlugBindTbl[Tv_WkIdx].Adaptor )
				{
					Tv_ObjTg	=	(Cls_GrIpaObjBase*)m_PlugBindTbl[Tv_WkIdx].Adaptor;
					Tv_PtrChItm	=	(Ptr_GrIpaScptChainItem)( (__u32)A_PtrBuf + Tv_Result );
					Tv_PtrChItm->ScptId			=	Tv_ObjTg->ScptIdGet();
					Tv_PtrChItm->PlugId			=	(__u8)Tv_WkIdx;
					Tv_PtrChItm->ConsentId	=	(__u8)m_PlugBindTbl[Tv_WkIdx].BindIdx;
					// next
					Tv_PtrChHd->Size	=	Tv_PtrChHd->Size + sizeof(St_GrIpaScptChainItem);
					Tv_Result	=	Tv_Result + sizeof(St_GrIpaScptChainItem);
				}
			}
		}
		
		return	Tv_Result;
}
#endif
//--------------------------------------------------------------------
void	Cls_GrIpaObjBase::ResetProcess( void )
{

}
//--------------------------------------------------------------------
void	Cls_GrIpaObjBase::ReleaseConnection( void )
{
		GrDumyZeroMem( m_ConsentBindTbl, sizeof(m_ConsentBindTbl) );
		GrDumyZeroMem( m_PlugBindTbl, sizeof(m_PlugBindTbl) );
}
//--------------------------------------------------------------------
void*	Cls_GrIpaObjBase::PlugStrmInfoGet( __u32 A_PlugIdx, __u32* A_PtrRtStrmType, __u32* A_PtrRtSize )
{
		return	NULL;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrIpaObjBase::RtlPlugSend( __u32 A_PlugIdx, void* A_PtrStrm, __u32 A_StrmSize )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( m_PlugCnt > A_PlugIdx )
		{
			// check chained
			if ( NULL != m_PlugBindTbl[A_PlugIdx].Adaptor )
			{
				Tv_Result	=	((Cls_GrIpaObjBase*)m_PlugBindTbl[A_PlugIdx].Adaptor)->RtlConsentRecv( m_PlugBindTbl[A_PlugIdx].BindIdx, A_PtrStrm, A_StrmSize );
			}
			else
			{
				DbgMsgPrint( "Cls_GrIpaObjBase::RtlPlugSend - adaptor ( %d ) plug ( %d ) unchained. \n", m_ScptId, A_PlugIdx );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrIpaObjBase::RtlPlugSend - bad adaptor ( %d ) plug index ( %d ) \n", m_ScptId, A_PlugIdx );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrIpaObjBase::RtlConsentRecv( __u32 A_ConsentIdx, void* A_PtrStrm, __u32 A_StrmSize )
{
		return	FALSE;
}
//--------------------------------------------------------------------
void	Cls_GrIpaObjBase::RtlScptUpdated( void )
{

}
//--------------------------------------------------------------------
BOOL8	Cls_GrIpaObjBase::RtlSetupSource( void )
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrIpaObjBase::RtlProcessSource( void )
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrIpaObjBase::RtlParaDec( void* A_PtrScpt, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrIpaScptParaHead	Tv_PtrPara;
		__u32	Tv_Prgs;
		void*	Tv_PtrData;
	// code --------------------
		Tv_Result	=	TRUE;

		Tv_Prgs		=	0;
		while ( Tv_Prgs < A_Size )
		{
			Tv_PtrPara	=	(Ptr_GrIpaScptParaHead)( (__u32)A_PtrScpt + Tv_Prgs );
			// check size
			if ( (Tv_Prgs + (__u32)Tv_PtrPara->Size) <= A_Size )
			{
				Tv_PtrData	=	(void*)( (__u32)Tv_PtrPara + sizeof(St_GrIpaScptParaHead) );
				if ( !LcParaSubDec( Tv_PtrPara->Type, Tv_PtrData, (__u32)Tv_PtrPara->Size ) )
				{
					DbgMsgPrint( "Cls_GrIpaObjBase::RtlParaDec - (%08X) unknown type parameter.\n", Tv_Prgs );
				}
				// next
				Tv_Prgs		=	Tv_Prgs + sizeof(St_GrIpaScptParaHead) + (__u32)Tv_PtrPara->Size;
			}
			else
			{
				Tv_Result	=	FALSE;
				DbgMsgPrint( "Cls_GrIpaObjBase::RtlParaDec - (%08X) bad parameter size\n", Tv_Prgs );
				break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrIpaObjBase::LcParaSubDec( __u16 A_Type, void* A_PtrBuf, __u32 A_Size )
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8		Cls_GrIpaObjBase::ChainPlug( __u32 A_PlugIdx, Cls_GrIpaObjBase* A_Adaptor, __u32 A_ConsentIdx )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		// check plug index
		if ( m_PlugCnt > A_PlugIdx )
		{
			// check already pluged
			if ( NULL == m_PlugBindTbl[A_PlugIdx].Adaptor )
			{
				// bind consetn
				if ( A_Adaptor->RtlChainConsent( A_ConsentIdx, this, A_PlugIdx) )
				{
					// update
					m_PlugBindTbl[A_PlugIdx].Adaptor	=	(void*)A_Adaptor;
					m_PlugBindTbl[A_PlugIdx].BindIdx	=	A_ConsentIdx;
					Tv_Result	=	TRUE;
				}
			}
			else
			{
				DbgMsgPrint(  "Cls_GrIpaObjBase::ChainPlug - Adaptor (%d) plug ( %d ) already binded.\n", m_ScptId, A_PlugIdx );
			}
		}
		else
		{
			DbgMsgPrint(  "Cls_GrIpaObjBase::ChainPlug - Adaptor (%d) bad plug index ( %d ) \n", m_ScptId, A_PlugIdx );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8		Cls_GrIpaObjBase::ChainConsent( __u32 A_ConsentIdx, Cls_GrIpaObjBase* A_Adaptor, __u32 A_PlugIdx )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		// check plug index
		if ( m_ConsentCnt > A_ConsentIdx )
		{
			// check already pluged
			if ( NULL == m_ConsentBindTbl[A_ConsentIdx].Adaptor )
			{
				// bind consetn
				if ( A_Adaptor->RtlChainPlug( A_PlugIdx, this, A_ConsentIdx) )
				{
					// update
					m_ConsentBindTbl[A_ConsentIdx].Adaptor	=	(void*)A_Adaptor;
					m_ConsentBindTbl[A_ConsentIdx].BindIdx	=	A_PlugIdx;
					Tv_Result	=	TRUE;
				}
			}
			else
			{
				DbgMsgPrint(  "Cls_GrIpaObjBase::ChainConsent - Adaptor (%d) consent ( %d ) already binded.\n", m_ScptId, A_ConsentIdx );
			}
		}
		else
		{
			DbgMsgPrint(  "Cls_GrIpaObjBase::ChainConsent - Adaptor (%d) bad consent index ( %d ) \n", m_ScptId, A_ConsentIdx );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrIpaObjBase::RtlChainPlug( __u32 A_PlugIdx, Cls_GrIpaObjBase* A_Adaptor, __u32 A_ConsentIdx )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		// check plug index
		if ( m_PlugCnt > A_PlugIdx )
		{
			// check already pluged
			if ( NULL == m_PlugBindTbl[A_PlugIdx].Adaptor )
			{
				// update
				m_PlugBindTbl[A_PlugIdx].Adaptor	=	(void*)A_Adaptor;
				m_PlugBindTbl[A_PlugIdx].BindIdx	=	A_ConsentIdx;
				Tv_Result	=	TRUE;
			}
			else
			{
				DbgMsgPrint(  "Cls_GrIpaObjBase::RtlChainPlug - Adaptor (%d) plug ( %d ) already binded.\n", m_ScptId, A_PlugIdx );
			}
		}
		else
		{
			DbgMsgPrint(  "Cls_GrIpaObjBase::RtlChainPlug - Adaptor (%d) bad plug index ( %d ) \n", m_ScptId, A_PlugIdx );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrIpaObjBase::RtlChainConsent( __u32 A_ConsentIdx, Cls_GrIpaObjBase* A_Adaptor, __u32 A_PlugIdx )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		// check plug index
		if ( m_ConsentCnt > A_ConsentIdx )
		{
			// check already pluged
			if ( NULL == m_ConsentBindTbl[A_ConsentIdx].Adaptor )
			{
				// update
				m_ConsentBindTbl[A_ConsentIdx].Adaptor	=	(void*)A_Adaptor;
				m_ConsentBindTbl[A_ConsentIdx].BindIdx	=	A_PlugIdx;
				Tv_Result	=	TRUE;
			}
			else
			{
				DbgMsgPrint(  "Cls_GrIpaObjBase::RtlChainConsent - Adaptor (%d) consent ( %d ) already binded.\n", m_ScptId, A_ConsentIdx );
			}
		}
		else
		{
			DbgMsgPrint(  "Cls_GrIpaObjBase::RtlChainConsent - Adaptor (%d) bad consent index ( %d ) \n", m_ScptId, A_ConsentIdx );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrIpaObjBase::RtlSetupPlug( __u32 A_PlugIdx, __u32 A_StrmType, void* A_PtrStrmInfo, __u32 A_StrmInfoSize )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( m_PlugCnt > A_PlugIdx )
		{
			if ( NULL != m_PlugBindTbl[A_PlugIdx].Adaptor )
			{
				// setup consent
				Tv_Result	=	((Cls_GrIpaObjBase*)m_PlugBindTbl[A_PlugIdx].Adaptor)->RtlSetupConsent( m_PlugBindTbl[A_PlugIdx].BindIdx, A_StrmType, A_PtrStrmInfo, A_StrmInfoSize );
			}
			else
			{
				DbgMsgPrint(  "Cls_GrIpaObjBase::RtlSetupPlug - Adaptor (%d) plug ( %d ) not chained.\n", m_ScptId, A_PlugIdx );
			}
		}
		else
		{
			DbgMsgPrint(  "Cls_GrIpaObjBase::RtlSetupPlug - Adaptor (%d) bad plug index ( %d ) \n", m_ScptId, A_PlugIdx );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrIpaObjBase::RtlSetupConsent( __u32 A_ConsentIdx, __u32 A_StrmType, void* A_PtrStrmInfo, __u32 A_StrmInfoSize )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( m_ConsentCnt > A_ConsentIdx )
		{
			if ( NULL != m_ConsentBindTbl[A_ConsentIdx].Adaptor )
			{
				Tv_Result	=	TRUE;
			}
			else
			{
				DbgMsgPrint(  "Cls_GrIpaObjBase::RtlSetupConsent - Adaptor (%d) consent ( %d ) not chained.\n", m_ScptId, A_ConsentIdx );
			}
		}
		else
		{
			DbgMsgPrint(  "Cls_GrIpaObjBase::RtlSetupConsent - Adaptor (%d) bad consent index ( %d ) \n", m_ScptId, A_ConsentIdx );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrIpaObjBase::RtlUnchainPlug( __u32 A_PlugIdx )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( m_PlugCnt > A_PlugIdx )
		{
			m_PlugBindTbl[A_PlugIdx].Adaptor	=	NULL;
			m_PlugBindTbl[A_PlugIdx].BindIdx	=	0;
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrIpaObjBase::RtlUnchainConsent( __u32 A_ConsIdx )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( m_ConsentCnt > A_ConsIdx )
		{
			m_ConsentBindTbl[A_ConsIdx].Adaptor	=	NULL;
			m_ConsentBindTbl[A_ConsIdx].BindIdx	=	0;
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8		Cls_GrIpaObjBase::UnchainPlug( __u32 A_PlugIdx )
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrIpaObjBase*	Tv_ObjTg;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( m_PlugCnt > A_PlugIdx )
		{
			if ( NULL != m_PlugBindTbl[A_PlugIdx].Adaptor )
			{
				Tv_ObjTg	=	(Cls_GrIpaObjBase*)m_PlugBindTbl[A_PlugIdx].Adaptor;
				Tv_Result	=	Tv_ObjTg->RtlUnchainConsent( m_PlugBindTbl[A_PlugIdx].BindIdx );
			}
			m_PlugBindTbl[A_PlugIdx].Adaptor	=	NULL;
			m_PlugBindTbl[A_PlugIdx].BindIdx	=	0;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8		Cls_GrIpaObjBase::UnchainConsent( __u32 A_ConsIdx )
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrIpaObjBase*	Tv_ObjTg;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( m_ConsentCnt > A_ConsIdx )
		{
			if ( NULL != m_ConsentBindTbl[A_ConsIdx].Adaptor )
			{
				Tv_ObjTg	=	(Cls_GrIpaObjBase*)m_ConsentBindTbl[A_ConsIdx].Adaptor;
				Tv_Result	=	Tv_ObjTg->RtlUnchainPlug( m_ConsentBindTbl[A_ConsIdx].BindIdx );
			}
			m_ConsentBindTbl[A_ConsIdx].Adaptor	=	NULL;
			m_ConsentBindTbl[A_ConsIdx].BindIdx	=	0;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
#ifdef GR_IPA_EDIT
__u32	Cls_GrIpaObjBase::ParaInfoDump( void* A_PtrBuf )
{
	// local -------------------
	// code --------------------
		m_PtrParaInfo		=	A_PtrBuf;
		m_ParaInfoPrgs	=	0;

		LcParaInfoSubDump();

		return	m_ParaInfoPrgs / sizeof(St_GrIpaAdptParaInfo);
}
#endif
//--------------------------------------------------------------------
#ifdef GR_IPA_EDIT
void	Cls_GrIpaObjBase::LcParaInfoSubDump( void )
{
		
}
#endif
//--------------------------------------------------------------------
#ifdef GR_IPA_EDIT
void	Cls_GrIpaObjBase::LcParaInfoAdd( void* A_PtrPara, void* A_PtrInfo, __u32 A_Cnt )
{
	// local -------------------
		__u32	Tv_WkIdx;
		Ptr_GrIpaAdptParaInfo	Tv_PtrTg;
		Ptr_GrIpaAdptParaInfo	Tv_PtrSr;
	// code --------------------
		Tv_PtrTg	=	(Ptr_GrIpaAdptParaInfo)( (__u32)m_PtrParaInfo + m_ParaInfoPrgs );
		Tv_PtrSr	=	(Ptr_GrIpaAdptParaInfo)A_PtrInfo;
		for (Tv_WkIdx=0;Tv_WkIdx < A_Cnt;Tv_WkIdx++)
		{
			GrDumyCopyMem( Tv_PtrTg, Tv_PtrSr, sizeof(St_GrIpaAdptParaInfo) );
			// patch
			Tv_PtrTg->Ptr	=	(void*)( (__u32)Tv_PtrSr->Ptr + (__u32)A_PtrPara );
			// next
			Tv_PtrSr ++;
			Tv_PtrTg ++;
			m_ParaInfoPrgs	=	m_ParaInfoPrgs + sizeof(St_GrIpaAdptParaInfo);
		}
}
#endif
//--------------------------------------------------------------------
void	Cls_GrIpaObjBase::RtlChainDec( void* A_PtrScpt, __u32 A_Size )
{
	// local -------------------
		Ptr_GrIpaScptChainItem	Tv_PtrChain;
		__u32	Tv_Prgs;
		Cls_GrIpaObjBase*	Tv_ObjTg;
	// code --------------------

		Tv_Prgs		=	0;
		while ( Tv_Prgs < A_Size )
		{
			Tv_PtrChain	=	(Ptr_GrIpaScptChainItem)( (__u32)A_PtrScpt + Tv_Prgs );
			Tv_ObjTg		=	V_GrIpaMng->ObjGet(Tv_PtrChain->ScptId);
			if ( NULL != Tv_ObjTg )
			{
				if ( !ChainPlug( Tv_PtrChain->PlugId, Tv_ObjTg, Tv_PtrChain->ConsentId ) )
				{
					DbgMsgPrint( "Cls_GrIpaObjBase::RtlChainDec - (%08X) plug (%d) can not connect.\n", Tv_Prgs, Tv_PtrChain->PlugId );
				}
			}
			else
			{
				DbgMsgPrint( "Cls_GrIpaObjBase::RtlChainDec - (%08X) object (%d) not found\n", Tv_Prgs, Tv_PtrChain->ScptId );
			}
			// next
			Tv_Prgs	=	Tv_Prgs + sizeof(St_GrIpaScptChainItem);
		}

}
//--------------------------------------------------------------------
