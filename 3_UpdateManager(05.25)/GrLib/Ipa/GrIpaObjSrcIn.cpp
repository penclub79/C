/*
		image process adaptor source image

 */

//====================================================================
// uses
#include <Ipa/GrIpaObjSrcIn.h>
#include <GrDebug.h>
#include <GrDrawTool.h>
#include <GrDumyTool.h>
#include <GrStrTool.h>

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

#ifdef GR_IPA_EDIT

#define	E_GrIpaObjSrcInParaCnt						1

St_GrIpaAdptParaInfo	V_GrIpaObjSrcInParaInfo[E_GrIpaObjSrcInParaCnt]	=	
{
	{ L"Source ID",		E_GrVarTypeDword,		1,	(void*)&((Ptr_GrIpaObjSrcInPara)0)->SrcId, L"Source ID" },
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrIpaObjSrcIn::Cls_GrIpaObjSrcIn( void ):
Cls_GrIpaObjBase()
{
	// local -------------------
	// code --------------------
		//init
		m_AdaptorType		=	E_GrIpaAdptTypeSrcIn;

		m_PlugCnt				=	E_GrIpaObjSrcInPlugCnt;

		m_StrmType			=	E_GrIpaStrmTypeNone;
		m_PtrStrmInfo		=	NULL;
		m_StrmInfoSize	=	0;

		GrDumyZeroMem( &m_ParaSrcIn, sizeof(m_ParaSrcIn) );
}
//--------------------------------------------------------------------
Cls_GrIpaObjSrcIn::~Cls_GrIpaObjSrcIn( void )
{
	// local -------------------
	// code --------------------
		if ( NULL != m_PtrStrmInfo )
		{
			free( m_PtrStrmInfo );
			m_PtrStrmInfo	=	NULL;
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrIpaObjSrcIn::LcParaSubDec( __u16 A_Type, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_Size;
	// code --------------------
		// inherited
		Tv_Result	=	Cls_GrIpaObjBase::LcParaSubDec( A_Type, A_PtrBuf, A_Size );
		// check type
		if ( E_GrIpaAdptTypeSrcIn == A_Type )
		{
			Tv_Size		=	A_Size;
			if ( sizeof(m_ParaSrcIn) < Tv_Size )
			{
				Tv_Size	=	sizeof(m_ParaSrcIn);
			}
			GrDumyCopyMem( &m_ParaSrcIn, A_PtrBuf, Tv_Size );
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
#ifdef GR_IPA_EDIT
void	Cls_GrIpaObjSrcIn::LcParaSubEnc( void )
{
	// local -------------------
		Ptr_GrIpaScptParaHead	Tv_PtrHd;	// header
		void*	Tv_PtrPl;				// payload
	// code --------------------
		// inherited
		Cls_GrIpaObjBase::LcParaSubEnc();

		// build header
		Tv_PtrHd	=	(Ptr_GrIpaScptParaHead)( (__u32)m_PtrEnc + m_EncPrgs );
		Tv_PtrHd->Type		=	E_GrIpaAdptTypeSrcIn;
		Tv_PtrHd->Size		=	sizeof(m_ParaSrcIn);
		m_EncPrgs					=	m_EncPrgs + sizeof(St_GrIpaScptParaHead);

		// copy payload
		Tv_PtrPl	=	(void*)( (__u32)m_PtrEnc + m_EncPrgs );
		GrDumyCopyMem( Tv_PtrPl, &m_ParaSrcIn, (__u32)Tv_PtrHd->Size );
		m_EncPrgs					=	m_EncPrgs + Tv_PtrHd->Size;
}
#endif
//--------------------------------------------------------------------
#ifdef GR_IPA_EDIT
void	Cls_GrIpaObjSrcIn::LcParaInfoSubDump( void )
{
	// local -------------------
	// code --------------------
		Cls_GrIpaObjBase::LcParaInfoSubDump();

		LcParaInfoAdd( &m_ParaSrcIn, V_GrIpaObjSrcInParaInfo, E_GrIpaObjSrcInParaCnt );
}
#endif
//--------------------------------------------------------------------
void*	Cls_GrIpaObjSrcIn::PlugStrmInfoGet( __u32 A_PlugIdx, __u32* A_PtrRtStrmType, __u32* A_PtrRtSize )
{
	// local -------------------
		void*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;

		if ( E_GrIpaObjSrcInPlugCnt > A_PlugIdx )
		{
			*A_PtrRtStrmType	=	m_StrmType;
			*A_PtrRtSize			=	m_StrmInfoSize;
			Tv_Result					=	m_PtrStrmInfo;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrIpaObjSrcIn::RtlSetupSource( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_WkIdx;
		__u32	Tv_PlugIdx;
		void*	Tv_PtrInfo;

		St_GrIpaMngCmdSetup	Tv_Cmd;
	// code --------------------
		Tv_Result	=	FALSE;

		// release old stream info
		if ( NULL != m_PtrStrmInfo )
		{
			free( m_PtrStrmInfo );
			m_PtrStrmInfo	=	NULL;
		}

		Tv_PtrInfo	=	(Ptr_GrIpaStrmInfoImg)V_GrIpaMng->SourceInfoGet( m_ParaSrcIn.SrcId, &m_StrmType, &m_StrmInfoSize );
		if ( NULL != Tv_PtrInfo )
		{
			if ( E_GrIpaStrmTypeNone != m_StrmType )
			{
				if ( 0 != m_StrmInfoSize )
				{
					// allocate stream info buffer
					m_PtrStrmInfo	=	malloc( m_StrmInfoSize );
					if ( NULL != m_PtrStrmInfo )
					{
						// upate info
						GrDumyCopyMem( m_PtrStrmInfo, Tv_PtrInfo, m_StrmInfoSize );

						// connect plug
						Tv_Cmd.CmdHd.Cmd		=	E_GrIpaMngCmdSetup;
						Tv_Cmd.ScptId				=	m_ScptId;
						Tv_Cmd.StrmType			=	m_StrmType;
						Tv_Cmd.PtrStrmInfo	=	m_PtrStrmInfo;
						Tv_Cmd.StrmInfoSize	=	m_StrmInfoSize;

						for (Tv_WkIdx=0;Tv_WkIdx < E_GrIpaObjSrcInPlugCnt;Tv_WkIdx++)
						{
							Tv_PlugIdx	=	(__u32)Tv_WkIdx;
							if ( NULL != m_PlugBindTbl[Tv_PlugIdx].Adaptor )
							{
								Tv_Cmd.PlugIdx		=	Tv_PlugIdx;
								// run command
								V_GrIpaMng->CommandInput( (Ptr_GrIpaMngCmdBase)&Tv_Cmd, sizeof(Tv_Cmd) );
							}
						}

						// success
						Tv_Result	=	TRUE;
					}
					else
					{
						DbgMsgPrint( "Cls_GrIpaObjSrcIn::RtlSourceSetup - Adaptor ( %d ) stream info buffer alloc fail. \n", m_ScptId );
					}
				}
				else
				{
					DbgMsgPrint( "Cls_GrIpaObjSrcIn::RtlSourceSetup - Adaptor ( %d ) source %d zero stream info size. \n", m_ScptId, m_ParaSrcIn.SrcId );
				}
			}
			else
			{
				DbgMsgPrint( "Cls_GrIpaObjSrcIn::RtlSourceSetup - Adaptor ( %d ) source %d bad stream type \n", m_ScptId, m_ParaSrcIn.SrcId );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrIpaObjSrcIn::RtlSourceSetup - Adaptor ( %d ) source %d not found\n", m_ScptId, m_ParaSrcIn.SrcId );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrIpaObjSrcIn::RtlProcessSource( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_WkIdx;
		void*	Tv_PtrStrm;
		__u32	Tv_StrmSize;
		St_GrIpaMngCmdProcess	Tv_Cmd;
	// code --------------------
		Tv_Result	=	FALSE;

		// get source
		Tv_PtrStrm	=	V_GrIpaMng->SourceStrmGet( m_ParaSrcIn.SrcId, &Tv_StrmSize );
		if ( NULL != Tv_PtrStrm )
		{
			// do command
			Tv_Cmd.CmdHd.Cmd	=	E_GrIpaMngCmdProcess;
			Tv_Cmd.ScptId			=	m_ScptId;
			Tv_Cmd.PtrStrm		=	Tv_PtrStrm;
			Tv_Cmd.StrmSize		=	Tv_StrmSize;

			for (Tv_WkIdx=0;Tv_WkIdx < E_GrIpaObjSrcInPlugCnt;Tv_WkIdx++)
			{
				// check chained
				if ( NULL != m_PlugBindTbl[Tv_WkIdx].Adaptor )
				{
					Tv_Cmd.PlugIdx	=	(__u32)Tv_WkIdx;
					if ( V_GrIpaMng->CommandInput( (Ptr_GrIpaMngCmdBase)&Tv_Cmd, sizeof(Tv_Cmd) ) )
					{
						Tv_Result	=	TRUE;
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------

