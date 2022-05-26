/*
		image process adaptor binary

 */

//====================================================================
// uses
#include <Ipa/GrIpaObjWdr.h>
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

#define	E_GrIpaObjWdrParaCnt						2

St_GrIpaAdptParaInfo	V_GrIpaObjWdrParaInfo[E_GrIpaObjWdrParaCnt]	=	
{
	{ L"Basis", E_GrVarTypeByte, 1, (void*)&((Ptr_GrIpaObjWdrPara)0)->Basis, L"Luminance basis" },
	{ L"Velocity", E_GrVarTypeByte, 1, (void*)&((Ptr_GrIpaObjWdrPara)0)->Velocity, L"Velocity" },
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrIpaObjWdr::Cls_GrIpaObjWdr( void ):
Cls_GrIpaObjBase()
{
	// local -------------------
	// code --------------------
		//init
		m_AdaptorType		=	E_GrIpaAdptTypeWdr;

		m_ConsentCnt		=	E_GrIpaObjWdrConsentCnt;
		m_PlugCnt				=	E_GrIpaObjWdrPlugCnt;

		m_StrmSize			=	0;
		m_PtrStrmPlug		=	NULL;

		GrDumyZeroMem( &m_ParaWdr, sizeof(m_ParaWdr) );

}
//--------------------------------------------------------------------
Cls_GrIpaObjWdr::~Cls_GrIpaObjWdr( void )
{
	// local -------------------
	// code --------------------
		LcBufFree();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrIpaObjWdr::LcParaSubDec( __u16 A_Type, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_Size;
	// code --------------------
		// inherited
		Tv_Result	=	Cls_GrIpaObjBase::LcParaSubDec( A_Type, A_PtrBuf, A_Size );
		// check type
		if ( E_GrIpaAdptTypeBinary == A_Type )
		{
			Tv_Size		=	A_Size;
			if ( sizeof(m_ParaWdr) < Tv_Size )
			{
				Tv_Size	=	sizeof(m_ParaWdr);
			}
			GrDumyCopyMem( &m_ParaWdr, A_PtrBuf, Tv_Size );
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
#ifdef GR_IPA_EDIT
void	Cls_GrIpaObjWdr::LcParaSubEnc( void )
{
	// local -------------------
		Ptr_GrIpaScptParaHead	Tv_PtrHd;	// header
		void*	Tv_PtrPl;				// payload
	// code --------------------
		// inherited
		Cls_GrIpaObjBase::LcParaSubEnc();

		// build header
		Tv_PtrHd	=	(Ptr_GrIpaScptParaHead)( (__u32)m_PtrEnc + m_EncPrgs );
		Tv_PtrHd->Type		=	E_GrIpaAdptTypeBinary;
		Tv_PtrHd->Size		=	sizeof(m_ParaWdr);
		m_EncPrgs					=	m_EncPrgs + sizeof(St_GrIpaScptParaHead);

		// copy payload
		Tv_PtrPl	=	(void*)( (__u32)m_PtrEnc + m_EncPrgs );
		GrDumyCopyMem( Tv_PtrPl, &m_ParaWdr, (__u32)Tv_PtrHd->Size );
		m_EncPrgs					=	m_EncPrgs + Tv_PtrHd->Size;
}
#endif
//--------------------------------------------------------------------
#ifdef GR_IPA_EDIT
void	Cls_GrIpaObjWdr::LcParaInfoSubDump( void )
{
	// local -------------------
	// code --------------------
		Cls_GrIpaObjBase::LcParaInfoSubDump();

		LcParaInfoAdd( &m_ParaWdr, V_GrIpaObjWdrParaInfo, E_GrIpaObjWdrParaCnt );
}
#endif
//--------------------------------------------------------------------
BOOL8	Cls_GrIpaObjWdr::RtlSetupConsent( __u32 A_ConsentIdx, __u32 A_StrmType, void* A_PtrStrmInfo, __u32 A_StrmInfoSize )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrIpaMngCmdSetup	Tv_Cmd;
	// code --------------------
		Tv_Result	=	FALSE;

		// check setup able
		if ( Cls_GrIpaObjBase::RtlSetupConsent(A_ConsentIdx, A_StrmType, A_PtrStrmInfo, A_StrmInfoSize ) )
		{
			// check stream type
			if ( (E_GrIpaStrmType2dByte == A_StrmType) && ( sizeof(St_GrIpaStrmInfo2d) == A_StrmInfoSize) )
			{
				// update stream info
				if ( (m_StrmInfo.ResX != ((Ptr_GrIpaStrmInfo2d)A_PtrStrmInfo)->ResX) || (m_StrmInfo.ResY != ((Ptr_GrIpaStrmInfo2d)A_PtrStrmInfo)->ResY) )
				{
					LcBufFree();
					GrDumyCopyMem( &m_StrmInfo, A_PtrStrmInfo, sizeof(m_StrmInfo) );
					m_StrmSize	=	m_StrmInfo.ResX * m_StrmInfo.ResY;

					if ( 0 != m_StrmSize )
					{
						m_PtrStrmPlug	=	malloc( m_StrmSize );
						if ( NULL == m_PtrStrmPlug )
						{
							DbgMsgPrint( "Cls_GrIpaObjWdr::RtlSetupConsent - adaptor ( %d ) stream buffer allocate fail.\n" , m_ScptId );
						}
					}
				}

				if ( NULL != m_PtrStrmPlug )
				{
					if ( NULL != m_PlugBindTbl[0].Adaptor )
					{
						Tv_Cmd.CmdHd.Cmd		=	E_GrIpaMngCmdSetup;
						Tv_Cmd.StrmType			=	E_GrIpaStrmType2dByte;
						Tv_Cmd.PtrStrmInfo	=	&m_StrmInfo;
						Tv_Cmd.StrmInfoSize	=	sizeof(m_StrmInfo);
						Tv_Cmd.ScptId				=	m_ScptId;
						Tv_Cmd.PlugIdx			=	0;
						V_GrIpaMng->CommandInput( (Ptr_GrIpaMngCmdBase)&Tv_Cmd, sizeof(Tv_Cmd) );
					}
					// success
					Tv_Result	=	TRUE;
				}

			}
			else
			{
				DbgMsgPrint( "Cls_GrIpaObjWdr::RtlSetupConsent - adaptor ( %d ) bad stream type.\n" , m_ScptId );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrIpaObjWdr::RtlConsentRecv( __u32 A_ConsentIdx, void* A_PtrStrm, __u32 A_StrmSize )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrIpaMngCmdProcess	Tv_Cmd;
	// code --------------------
		Tv_Result	=	FALSE;

		// check consent index
		if ( m_ConsentCnt > A_ConsentIdx )
		{
			if ( NULL != m_PtrStrmPlug )
			{
				if ( m_StrmSize == A_StrmSize )
				{
					// do WDR
					LcDoWdr(A_PtrStrm);

					// do command
					if ( NULL != m_PlugBindTbl[0].Adaptor )
					{
						Tv_Cmd.CmdHd.Cmd	=	E_GrIpaMngCmdProcess;
						Tv_Cmd.ScptId			=	m_ScptId;
						Tv_Cmd.StrmSize		=	m_StrmSize;
						Tv_Cmd.PlugIdx		=	0;
						Tv_Cmd.PtrStrm		=	m_PtrStrmPlug;
						V_GrIpaMng->CommandInput( (Ptr_GrIpaMngCmdBase)&Tv_Cmd, sizeof(Tv_Cmd) );
					}

					Tv_Result	=	TRUE;
				}
				else
				{
					DbgMsgPrint( "Cls_GrIpaObjWdr::RtlConsentRecv - adaptor ( %d ) bad input stream size.\n" , m_ScptId );
				}
			}
			else
			{
				DbgMsgPrint( "Cls_GrIpaObjWdr::RtlConsentRecv - adaptor ( %d ) did not setup.\n" , m_ScptId );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrIpaObjWdr::RtlConsentRecv - adaptor ( %d ) bad conset index ( %d ).\n" , m_ScptId, A_ConsentIdx );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrIpaObjWdr::LcBufFree( void )
{
	// local -------------------
	// code --------------------
		if (NULL != m_PtrStrmPlug)
		{
			free( m_PtrStrmPlug );
			m_PtrStrmPlug		=	NULL;
		}
}
//--------------------------------------------------------------------
void	Cls_GrIpaObjWdr::LcDoWdr(void* A_PtrStrm)
{
	// local -------------------
	// code --------------------
		// scan histogram
		LcHstgBuild(A_PtrStrm);
		LcQuantize();
		LcNormalize();
		LcRemapClr(A_PtrStrm);

}
//--------------------------------------------------------------------
void	Cls_GrIpaObjWdr::LcHstgBuild(void* A_PtrStrm)
{
	// local -------------------
		__u32	Tv_WkX;
		__u32	Tv_WkY;
		__u8*	Tv_PtrSr;
	// code --------------------
		// reset table
		GrDumyZeroMem(m_HstgTbl, sizeof(m_HstgTbl));

		Tv_PtrSr		=	(__u8*)A_PtrStrm;

		for (Tv_WkY=0; Tv_WkY < m_StrmInfo.ResY; Tv_WkY++)
		{
			for (Tv_WkX=0; Tv_WkX < m_StrmInfo.ResX; Tv_WkX++)
			{
				m_HstgTbl[(*Tv_PtrSr)].PxCnt ++;

				// next
				Tv_PtrSr ++;
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrIpaObjWdr::LcClrTblBld(__u32 A_Tolerance)
{
	// local -------------------
		__u32	Tv_WkIdx;
	// code --------------------

		GrDumyZeroMem(m_ClrTbl, sizeof(m_ClrTbl));
		m_ClrCnt	=	0;

		for (Tv_WkIdx=0; Tv_WkIdx < 256;Tv_WkIdx ++)
		{
			m_HstgTbl[Tv_WkIdx].PtrClrItm	=	NULL;
			if (0 != m_HstgTbl[Tv_WkIdx].PxCnt)
			{
				m_ClrTbl[m_ClrCnt].PxCnt		=	m_ClrTbl[m_ClrCnt].PxCnt + m_HstgTbl[Tv_WkIdx].PxCnt;
				m_HstgTbl[Tv_WkIdx].PtrClrItm			=	&m_ClrTbl[m_ClrCnt];
				if (m_ClrTbl[m_ClrCnt].PxCnt >= A_Tolerance)
				{
					m_ClrCnt ++;
				}
			}
		}

		if (m_ClrCnt < 256)
		{
			if (0 != m_ClrTbl[m_ClrCnt].PxCnt)
			{
				m_ClrCnt ++;
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrIpaObjWdr::LcQuantize(void)
{
	// local -------------------
		__u32	Tv_Basis;
		__u32	Tv_Vel;
		__u32	Tv_CalLim;
		__u32	Tv_Tolr;
	// code --------------------
		Tv_Basis	=	(__u32)m_ParaWdr.Basis;
		Tv_Vel		=	(__u32)m_ParaWdr.Velocity;
		if (Tv_Vel > Tv_Basis)
		{
			Tv_Vel	=	Tv_Basis;
		}

		Tv_Tolr		=	0;
		Tv_CalLim	=	256 - Tv_Vel;
		do
		{
			Tv_Tolr ++;
			LcClrTblBld(Tv_Tolr);
		} while (m_ClrCnt > Tv_CalLim);
}
//--------------------------------------------------------------------
void	Cls_GrIpaObjWdr::LcNormalize(void)
{
	// local -------------------
		__u32	Tv_NowClr;
		__u32	Tv_WkCnt;
		__u32	Tv_WkIdx;
		__u32	Tv_Rng;
	// code --------------------
		// check color count
		if (1 < m_ClrCnt)
		{
			// remap first pixel to zero
			m_ClrTbl[0].ClrNew	=	0;

			// start base
			Tv_Rng		=	256 - (__u32)m_ParaWdr.Basis;
			Tv_WkCnt	=	m_ClrCnt - 1;
			Tv_WkIdx	=	0;
			while (Tv_WkIdx < Tv_WkCnt)
			{
				Tv_NowClr	=	((Tv_Rng * Tv_WkIdx) / Tv_WkCnt) + (__u32)m_ParaWdr.Basis;
				m_ClrTbl[Tv_WkIdx + 1].ClrNew	=	(__u8)Tv_NowClr;
				// next
				Tv_WkIdx ++;
			}
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrIpaObjWdr::LcRemapClr(void* A_PtrStrm)
{
	// local -------------------
		__u32	Tv_WkX;
		__u32	Tv_WkY;
		__u8*	Tv_PtrSr;
		__u8*	Tv_PtrTg;
		Ptr_GrIpaObjWdrClrItem	Tv_PtrClrItm;
	// code --------------------
		Tv_PtrSr		=	(__u8*)A_PtrStrm;
		Tv_PtrTg		=	(__u8*)m_PtrStrmPlug;

		for (Tv_WkY=0; Tv_WkY < m_StrmInfo.ResY; Tv_WkY++)
		{
			for (Tv_WkX=0; Tv_WkX < m_StrmInfo.ResX; Tv_WkX++)
			{
				Tv_PtrClrItm	=	m_HstgTbl[(*Tv_PtrSr)].PtrClrItm;
				*Tv_PtrTg			=	Tv_PtrClrItm->ClrNew;
				// next
				Tv_PtrSr ++;
				Tv_PtrTg ++;
			}
		}

}
//--------------------------------------------------------------------

