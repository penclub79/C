/*
		image process adaptor out image

 */

//====================================================================
// uses
#include <Ipa/GrIpaObjMonOut.h>
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

#define	E_GrIpaObjMonOutParaCnt						1

St_GrIpaAdptParaInfo	V_GrIpaObjMonOutParaInfo[E_GrIpaObjMonOutParaCnt]	=	
{
	{ L"Monitor ID",		E_GrVarTypeDword,		1,	(void*)&((Ptr_GrIpaObjMonOutPara)0)->MonIdx, L"Monitor ID" },
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrIpaObjMonOut::Cls_GrIpaObjMonOut( void ):
Cls_GrIpaObjBase()
{
	// local -------------------
	// code --------------------
		//init
		m_AdaptorType	=	E_GrIpaAdptTypeMonOut;

		m_ConsentCnt	=	1;

		GrDumyZeroMem( &m_ParaMonOut, sizeof(m_ParaMonOut) );

}
//--------------------------------------------------------------------
Cls_GrIpaObjMonOut::~Cls_GrIpaObjMonOut( void )
{
	// local -------------------
	// code --------------------
}
//--------------------------------------------------------------------
BOOL8	Cls_GrIpaObjMonOut::LcParaSubDec( __u16 A_Type, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_Size;
	// code --------------------
		// inherited
		Tv_Result	=	Cls_GrIpaObjBase::LcParaSubDec( A_Type, A_PtrBuf, A_Size );
		// check type
		if ( E_GrIpaAdptTypeMonOut == A_Type )
		{
			Tv_Size		=	A_Size;
			if ( sizeof(m_ParaMonOut) < Tv_Size )
			{
				Tv_Size	=	sizeof(m_ParaMonOut);
			}
			GrDumyCopyMem( &m_ParaMonOut, A_PtrBuf, Tv_Size );
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
#ifdef GR_IPA_EDIT
void	Cls_GrIpaObjMonOut::LcParaSubEnc( void )
{
	// local -------------------
		Ptr_GrIpaScptParaHead	Tv_PtrHd;	// header
		void*	Tv_PtrPl;				// payload
	// code --------------------
		// inherited
		Cls_GrIpaObjBase::LcParaSubEnc();

		// build header
		Tv_PtrHd	=	(Ptr_GrIpaScptParaHead)( (__u32)m_PtrEnc + m_EncPrgs );
		Tv_PtrHd->Type		=	E_GrIpaAdptTypeMonOut;
		Tv_PtrHd->Size		=	sizeof(m_ParaMonOut);
		m_EncPrgs					=	m_EncPrgs + sizeof(St_GrIpaScptParaHead);

		// copy payload
		Tv_PtrPl	=	(void*)( (__u32)m_PtrEnc + m_EncPrgs );
		GrDumyCopyMem( Tv_PtrPl, &m_ParaMonOut, (__u32)Tv_PtrHd->Size );
		m_EncPrgs					=	m_EncPrgs + Tv_PtrHd->Size;
}
#endif
//--------------------------------------------------------------------
#ifdef GR_IPA_EDIT
void	Cls_GrIpaObjMonOut::LcParaInfoSubDump( void )
{
	// local -------------------
	// code --------------------
		Cls_GrIpaObjBase::LcParaInfoSubDump();

		LcParaInfoAdd( &m_ParaMonOut, V_GrIpaObjMonOutParaInfo, E_GrIpaObjMonOutParaCnt );
}
#endif
//--------------------------------------------------------------------
BOOL8	Cls_GrIpaObjMonOut::RtlSetupConsent( __u32 A_ConsentIdx, __u32 A_StrmType, void* A_PtrStrmInfo, __u32 A_StrmInfoSize )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( Cls_GrIpaObjBase::RtlSetupConsent( A_ConsentIdx, A_StrmType, A_PtrStrmInfo, A_StrmInfoSize ) )
		{
			Tv_Result	=	V_GrIpaMng->MonitorSetup( m_ParaMonOut.MonIdx, A_StrmType, A_PtrStrmInfo, A_StrmInfoSize );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrIpaObjMonOut::RtlConsentRecv( __u32 A_ConsentIdx, void* A_PtrStrm, __u32 A_StrmSize )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		
		if ( m_ConsentCnt > A_ConsentIdx )
		{
			Tv_Result	=	V_GrIpaMng->MonitorOut( m_ParaMonOut.MonIdx, A_PtrStrm, A_StrmSize );
		}
		else
		{
			DbgMsgPrint( "Cls_GrIpaObjMonOut::RtlConsentRecv - bad consent index" );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------

