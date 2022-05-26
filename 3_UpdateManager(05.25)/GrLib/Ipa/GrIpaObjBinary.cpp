/*
		image process adaptor binary

 */

//====================================================================
// uses
#include <Ipa/GrIpaObjBinary.h>
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

#define	E_GrIpaObjBinaryParaCnt						1

St_GrIpaAdptParaInfo	V_GrIpaObjBinaryParaInfo[E_GrIpaObjBinaryParaCnt]	=	
{
	{ L"Radius",			E_GrVarTypeDword,		1,	(void*)&((Ptr_GrIpaObjBinaryPara)0)->Radius, L"Calculate area radius" },
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrIpaObjBinary::Cls_GrIpaObjBinary( void ):
Cls_GrIpaObjBase()
{
	// local -------------------
	// code --------------------
		//init
		m_AdaptorType		=	E_GrIpaAdptTypeBinary;

		m_ConsentCnt		=	E_GrIpaObjBinaryConsentCnt;
		m_PlugCnt				=	E_GrIpaObjBinaryPlugCnt;

		m_StrmSize			=	0;
		m_PtrStrmPlug		=	NULL;

		GrDumyZeroMem( &m_ParaBinary, sizeof(m_ParaBinary) );

}
//--------------------------------------------------------------------
Cls_GrIpaObjBinary::~Cls_GrIpaObjBinary( void )
{
	// local -------------------
	// code --------------------
		LcBufFree();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrIpaObjBinary::LcParaSubDec( __u16 A_Type, void* A_PtrBuf, __u32 A_Size )
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
			if ( sizeof(m_ParaBinary) < Tv_Size )
			{
				Tv_Size	=	sizeof(m_ParaBinary);
			}
			GrDumyCopyMem( &m_ParaBinary, A_PtrBuf, Tv_Size );
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
#ifdef GR_IPA_EDIT
void	Cls_GrIpaObjBinary::LcParaSubEnc( void )
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
		Tv_PtrHd->Size		=	sizeof(m_ParaBinary);
		m_EncPrgs					=	m_EncPrgs + sizeof(St_GrIpaScptParaHead);

		// copy payload
		Tv_PtrPl	=	(void*)( (__u32)m_PtrEnc + m_EncPrgs );
		GrDumyCopyMem( Tv_PtrPl, &m_ParaBinary, (__u32)Tv_PtrHd->Size );
		m_EncPrgs					=	m_EncPrgs + Tv_PtrHd->Size;
}
#endif
//--------------------------------------------------------------------
#ifdef GR_IPA_EDIT
void	Cls_GrIpaObjBinary::LcParaInfoSubDump( void )
{
	// local -------------------
	// code --------------------
		Cls_GrIpaObjBase::LcParaInfoSubDump();

		LcParaInfoAdd( &m_ParaBinary, V_GrIpaObjBinaryParaInfo, E_GrIpaObjBinaryParaCnt );
}
#endif
//--------------------------------------------------------------------
BOOL8	Cls_GrIpaObjBinary::RtlSetupConsent( __u32 A_ConsentIdx, __u32 A_StrmType, void* A_PtrStrmInfo, __u32 A_StrmInfoSize )
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
							DbgMsgPrint( "Cls_GrIpaObjBinary::RtlSetupConsent - adaptor ( %d ) stream buffer allocate fail.\n" , m_ScptId );
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
				DbgMsgPrint( "Cls_GrIpaObjBinary::RtlSetupConsent - adaptor ( %d ) bad stream type.\n" , m_ScptId );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrIpaObjBinary::RtlConsentRecv( __u32 A_ConsentIdx, void* A_PtrStrm, __u32 A_StrmSize )
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
					// do LBP
					LcDoLbp( A_PtrStrm );

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
					DbgMsgPrint( "Cls_GrIpaObjBinary::RtlConsentRecv - adaptor ( %d ) bad input stream size.\n" , m_ScptId );
				}
			}
			else
			{
				DbgMsgPrint( "Cls_GrIpaObjBinary::RtlConsentRecv - adaptor ( %d ) did not setup.\n" , m_ScptId );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrIpaObjBinary::RtlConsentRecv - adaptor ( %d ) bad conset index ( %d ).\n" , m_ScptId, A_ConsentIdx );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrIpaObjBinary::LcBufFree( void )
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
void	Cls_GrIpaObjBinary::LcDoLbp( void* A_PtrStrm )
{
	// local -------------------
		__u32	Tv_WkX;
		__u32	Tv_WkY;
		__u8*	Tv_PtrSr;
		__u8*	Tv_PtrTg;
		__u8	Tv_Tolerlance;
	// code --------------------
		Tv_PtrSr		=	(__u8*)A_PtrStrm;
		Tv_PtrTg		=	(__u8*)m_PtrStrmPlug;
		for (Tv_WkY=0;Tv_WkY < m_StrmInfo.ResY;Tv_WkY++)
		{
			for (Tv_WkX=0;Tv_WkX < m_StrmInfo.ResX;Tv_WkX++)
			{
				Tv_Tolerlance	=	LcAvrLocalExcptMe( A_PtrStrm, Tv_WkX, Tv_WkY, m_ParaBinary.Radius );
				if ( Tv_Tolerlance < (*Tv_PtrSr) )
				{
					*Tv_PtrTg	=	0xFF;
				}
				else
				{
					*Tv_PtrTg	=	0x00;
				}
				// next
				Tv_PtrSr ++;
				Tv_PtrTg ++;
			}
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrIpaObjBinary::LcAvrLocalExcptMe( void* A_PtrStrm, __u32 A_X, __u32 A_Y, __u32 A_Radius )
{
	// local -------------------
		__u8	Tv_Result;

		__u32	Tv_StX;
		__u32	Tv_EdX;
		__u32	Tv_StY;
		__u32	Tv_EdY;

		__u32	Tv_WkX;
		__u32	Tv_WkY;

		__u32	Tv_SubVal;

		__u32	Tv_CntX;
		__u32	Tv_CntY;
		__u32	Tv_AddX;

		__u8*	Tv_PtrStrm;
		__u32	Tv_Total;

		__u32	Tv_Divisor;
	// code --------------------
		Tv_Result	=	0;

		if ( 0 != A_Radius )
		{
			// get range
			Tv_StX	=	0;
			if ( A_X >= A_Radius )
			{
				Tv_StX	=	A_X - A_Radius;
			}
			Tv_EdX	=	A_X + A_Radius;
			if ( Tv_EdX >= m_StrmInfo.ResX )
			{
				Tv_EdX	=	m_StrmInfo.ResX - 1;
			}
			Tv_StY	=	0;
			if ( A_Y >= A_Radius )
			{
				Tv_StY	=	A_Y - A_Radius;
			}
			Tv_EdY	=	A_Y + A_Radius;
			if ( Tv_EdY >= m_StrmInfo.ResY )
			{
				Tv_EdY	=	m_StrmInfo.ResY - 1;
			}

			Tv_PtrStrm	=	(__u8*)A_PtrStrm;
			Tv_WkX			=	(A_Y * m_StrmInfo.ResX) + A_X;
			Tv_SubVal		=	(__u32)Tv_PtrStrm[Tv_WkX];

			// calculate count
			Tv_CntX			=	Tv_EdX - Tv_StX + 1;
			Tv_CntY			=	Tv_EdY - Tv_StY + 1;
			Tv_AddX			=	m_StrmInfo.ResX - Tv_CntX;

			// calculate start position
			Tv_PtrStrm	=	(__u8*)( (__u32)A_PtrStrm + (Tv_StY * m_StrmInfo.ResX) + Tv_StX );
			
			// calculate
			Tv_Total		=	0;
			for (Tv_WkY=0;Tv_WkY < Tv_CntY;Tv_WkY++)
			{
				for (Tv_WkX=0;Tv_WkX < Tv_CntX;Tv_WkX++)
				{
					Tv_Total	=	Tv_Total + (__u32)(*Tv_PtrStrm);
					// next
					Tv_PtrStrm ++;
				}
				// next
				Tv_PtrStrm	=	Tv_PtrStrm + Tv_AddX;
			}

			// update result
			Tv_Total		=	Tv_Total - Tv_SubVal;
			Tv_Divisor	=	Tv_CntX * Tv_CntY - 1;
			if ( 0 != Tv_Divisor )
			{
				Tv_Result	=	(__u8)( Tv_Total / Tv_Divisor );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------

