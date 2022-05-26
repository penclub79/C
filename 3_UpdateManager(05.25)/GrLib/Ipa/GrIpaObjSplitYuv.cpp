/*
		image process adaptor split YUV

 */

//====================================================================
// uses
#include <Ipa/GrIpaObjSplitYuv.h>
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

/*
#define	E_GrIpaObjSplitYuvParaCnt						1

St_GrIpaAdptParaInfo	V_GrIpaObjSplitYuvParaInfo[E_GrIpaObjSplitYuvParaCnt]	=	
{
	{ L"Source ID",		E_GrVarTypeDword,		1,	(void*)&((Ptr_GrIpaObjSplitYuvPara)0)->SrcId, L"Source ID" },
};
*/

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrIpaObjSplitYuv::Cls_GrIpaObjSplitYuv( void ):
Cls_GrIpaObjBase()
{
	// local -------------------
	// code --------------------
		//init
		m_AdaptorType		=	E_GrIpaAdptTypeSplitYuv;

		m_ConsentCnt		=	E_GrIpaObjSplitYuvConsentCnt;
		m_PlugCnt				=	E_GrIpaObjSplitYuvPlugCnt;


		GrDumyZeroMem( &m_ParaSplitYuv, sizeof(m_ParaSplitYuv) );

		GrDumyZeroMem( m_PlugCtrlTbl, sizeof(m_PlugCtrlTbl) );
}
//--------------------------------------------------------------------
Cls_GrIpaObjSplitYuv::~Cls_GrIpaObjSplitYuv( void )
{
	// local -------------------
	// code --------------------
		LcSplitBufFree();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrIpaObjSplitYuv::LcParaSubDec( __u16 A_Type, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_Size;
	// code --------------------
		// inherited
		Tv_Result	=	Cls_GrIpaObjBase::LcParaSubDec( A_Type, A_PtrBuf, A_Size );
		// check type
		if ( E_GrIpaAdptTypeSplitYuv == A_Type )
		{
			Tv_Size		=	A_Size;
			if ( sizeof(m_ParaSplitYuv) < Tv_Size )
			{
				Tv_Size	=	sizeof(m_ParaSplitYuv);
			}
			GrDumyCopyMem( &m_ParaSplitYuv, A_PtrBuf, Tv_Size );
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
#ifdef GR_IPA_EDIT
void	Cls_GrIpaObjSplitYuv::LcParaSubEnc( void )
{
	// local -------------------
		//Ptr_GrIpaScptParaHead	Tv_PtrHd;	// header
		//void*	Tv_PtrPl;				// payload
	// code --------------------
		// inherited
		Cls_GrIpaObjBase::LcParaSubEnc();

		/*
		// build header
		Tv_PtrHd	=	(Ptr_GrIpaScptParaHead)( (__u32)m_PtrEnc + m_EncPrgs );
		Tv_PtrHd->Type		=	E_GrIpaAdptTypeSplitYuv;
		Tv_PtrHd->Size		=	sizeof(m_ParaSplitYuv);
		m_EncPrgs					=	m_EncPrgs + sizeof(St_GrIpaScptParaHead);

		// copy payload
		Tv_PtrPl	=	(void*)( (__u32)m_PtrEnc + m_EncPrgs );
		GrDumyCopyMem( Tv_PtrPl, &m_ParaSplitYuv, (__u32)Tv_PtrHd->Size );
		m_EncPrgs					=	m_EncPrgs + Tv_PtrHd->Size;
		*/
}
#endif
//--------------------------------------------------------------------
#ifdef GR_IPA_EDIT
void	Cls_GrIpaObjSplitYuv::LcParaInfoSubDump( void )
{
	// local -------------------
	// code --------------------
		Cls_GrIpaObjBase::LcParaInfoSubDump();

		//LcParaInfoAdd( &m_ParaSplitYuv, V_GrIpaObjSplitYuvParaInfo, E_GrIpaObjSplitYuvParaCnt );
}
#endif
//--------------------------------------------------------------------
void	Cls_GrIpaObjSplitYuv::LcSplitBufFree( void )
{
	// local -------------------
		__s32		Tv_WkIdx;
	// code --------------------
		for (Tv_WkIdx=0;Tv_WkIdx < E_GrIpaObjSplitYuvPlugCnt;Tv_WkIdx++)
		{
			if ( NULL != m_PlugCtrlTbl[Tv_WkIdx].PtrBuf )
			{
				free( m_PlugCtrlTbl[Tv_WkIdx].PtrBuf );
				m_PlugCtrlTbl[Tv_WkIdx].PtrBuf	=	NULL;
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrIpaObjSplitYuv::RtlSetupConsent( __u32 A_ConsentIdx, __u32 A_StrmType, void* A_PtrStrmInfo, __u32 A_StrmInfoSize )
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrIpaStrmInfoImg	Tv_PtrInfo;
		__u32	Tv_WkIdx;
		St_GrIpaMngCmdSetup	Tv_Cmd;
	// code --------------------
		Tv_Result	=	FALSE;

		// release plug buffer
		LcSplitBufFree();

		if ( Cls_GrIpaObjBase::RtlSetupConsent(A_ConsentIdx, A_StrmType, A_PtrStrmInfo, A_StrmInfoSize ) )
		{
			// check stream type
			if ( (E_GrIpaStrmTypeImage == A_StrmType) && ( sizeof(St_GrIpaStrmInfoImg) == A_StrmInfoSize) )
			{
				// make
				Tv_PtrInfo	=	(Ptr_GrIpaStrmInfoImg)A_PtrStrmInfo;
				GrGdibInitHead( &m_GdibConsent, Tv_PtrInfo->FccImg, Tv_PtrInfo->ResX, Tv_PtrInfo->ResY, Tv_PtrInfo->Pitch );
				// init image info
				m_PlugStrmInfo.ResX		=	Tv_PtrInfo->ResX;
				m_PlugStrmInfo.ResY		=	Tv_PtrInfo->ResY;

				// buffer size
				m_PlugStrmSize	=	Tv_PtrInfo->ResX * Tv_PtrInfo->ResY;

				if ( 0 != m_PlugStrmSize )
				{
					Tv_Result	=	TRUE;
					for (Tv_WkIdx=0;Tv_WkIdx < E_GrIpaObjSplitYuvPlugCnt;Tv_WkIdx++)
					{
						m_PlugCtrlTbl[Tv_WkIdx].PtrBuf	=	malloc( m_PlugStrmSize );
						if ( NULL == m_PlugCtrlTbl[Tv_WkIdx].PtrBuf )
						{
							LcSplitBufFree();
							DbgMsgPrint( "Cls_GrIpaObjSplitYuv::RtlSetupConsent - adaptor ( %d ) memory allocate fail.\n" , m_ScptId );
							break;
						}
					}

					// check success
					if ( Tv_Result )
					{
						// request plug setup
						Tv_Cmd.CmdHd.Cmd		=	E_GrIpaMngCmdSetup;
						Tv_Cmd.StrmType			=	E_GrIpaStrmType2dByte;
						Tv_Cmd.PtrStrmInfo	=	&m_PlugStrmInfo;
						Tv_Cmd.StrmInfoSize	=	sizeof(m_PlugStrmInfo);
						Tv_Cmd.ScptId				=	m_ScptId;
						for (Tv_WkIdx=0;Tv_WkIdx < E_GrIpaObjSplitYuvPlugCnt;Tv_WkIdx++)
						{
							if ( NULL != m_PlugBindTbl[Tv_WkIdx].Adaptor )
							{
								Tv_Cmd.PlugIdx	=	Tv_WkIdx;
								V_GrIpaMng->CommandInput( (Ptr_GrIpaMngCmdBase)&Tv_Cmd, sizeof(Tv_Cmd) );
							}
						}
					}
				}
				else
				{
					DbgMsgPrint( "Cls_GrIpaObjSplitYuv::RtlSetupConsent - adaptor ( %d ) bad resolution ( x: %d , y: %d ).\n" , m_ScptId, Tv_PtrInfo->ResX, Tv_PtrInfo->ResY );
				}
			}
			else
			{
				DbgMsgPrint( "Cls_GrIpaObjSplitYuv::RtlSetupConsent - adaptor ( %d ) bad stream type.\n" , m_ScptId );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrIpaObjSplitYuv::RtlConsentRecv( __u32 A_ConsentIdx, void* A_PtrStrm, __u32 A_StrmSize )
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrGdib*	Tv_GdibObjStrm;
		__u32	Tv_X;
		__u32	Tv_Y;
		__u8*	Tv_Ptr2dY;
		__u8*	Tv_Ptr2dU;
		__u8*	Tv_Ptr2dV;
		__u32	Tv_ClrOrg;
		__u32	Tv_ClrCvt;
		__u32	Tv_WkIdx;
		St_GrIpaMngCmdProcess	Tv_Cmd;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( (NULL != m_PlugCtrlTbl[0].PtrBuf) && (0 == A_ConsentIdx) )
		{
			// set DIB pointer
			m_GdibConsent.PtrExtDib	=	A_PtrStrm;
			// create GDIB object
			Tv_GdibObjStrm	=	GrGdibOvlp( &m_GdibConsent );
			if ( NULL != Tv_GdibObjStrm )
			{
				Tv_Ptr2dY		=	(__u8*)m_PlugCtrlTbl[E_GrIpaObjSplitYuvPlugY].PtrBuf;
				Tv_Ptr2dU		=	(__u8*)m_PlugCtrlTbl[E_GrIpaObjSplitYuvPlugU].PtrBuf;
				Tv_Ptr2dV		=	(__u8*)m_PlugCtrlTbl[E_GrIpaObjSplitYuvPlugV].PtrBuf;

				for (Tv_Y=0;Tv_Y < m_PlugStrmInfo.ResY;Tv_Y++)
				{
					for (Tv_X=0;Tv_X < m_PlugStrmInfo.ResX;Tv_X++)
					{
						Tv_GdibObjStrm->GetPixelColor( Tv_X, Tv_Y, &Tv_ClrOrg );
						// convert
						Tv_ClrCvt	=	GrDrawCvtColorByFcc( m_GdibConsent.FccImg, Tv_ClrOrg, E_GrFccUYVY );
						*Tv_Ptr2dY	=	((Ptr_GrImgFmtUYVY)&Tv_ClrCvt)->Y1;
						*Tv_Ptr2dU	=	((Ptr_GrImgFmtUYVY)&Tv_ClrCvt)->U;
						*Tv_Ptr2dV	=	((Ptr_GrImgFmtUYVY)&Tv_ClrCvt)->V;
						// next
						Tv_Ptr2dY ++;
						Tv_Ptr2dU ++;
						Tv_Ptr2dV ++;
					}
				}

				// process plug
				Tv_Cmd.CmdHd.Cmd	=	E_GrIpaMngCmdProcess;
				Tv_Cmd.ScptId			=	m_ScptId;
				Tv_Cmd.StrmSize		=	m_PlugStrmSize;

				for (Tv_WkIdx=0;Tv_WkIdx < E_GrIpaObjSplitYuvPlugCnt;Tv_WkIdx++)
				{
					if ( NULL != m_PlugBindTbl[Tv_WkIdx].Adaptor )
					{
						Tv_Cmd.PlugIdx		=	Tv_WkIdx;
						Tv_Cmd.PtrStrm		=	m_PlugCtrlTbl[Tv_WkIdx].PtrBuf;
						V_GrIpaMng->CommandInput( (Ptr_GrIpaMngCmdBase)&Tv_Cmd, sizeof(Tv_Cmd) );
					}
				}

				// success
				Tv_Result	=	TRUE;

				delete	Tv_GdibObjStrm;		// release GDIB
			}
			else
			{
				DbgMsgPrint( "Cls_GrIpaObjSplitYuv::RtlConsentRecv - adaptor ( %d ) GDIB create fail.\n" , m_ScptId );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrIpaObjSplitYuv::RtlConsentRecv - adaptor ( %d ) did not setup.\n" , m_ScptId );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------

