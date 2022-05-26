/*
		image process adaptor split YUV

 */

//====================================================================
// uses
#include <Ipa/GrIpaObjBindYuv.h>
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

#define	E_GrIpaObjBindYuvParaCnt						6

St_GrIpaAdptParaInfo	V_GrIpaObjBindYuvParaInfo[E_GrIpaObjBindYuvParaCnt]	=	
{
	{ L"IsFixY",			E_GrVarTypeBool8,		1,	(void*)&((Ptr_GrIpaObjBindYuvPara)0)->IsFix[E_GrIpaObjBindYuvConsentY], L"Fix Y value mode on" },
	{ L"FixValY",			E_GrVarTypeByte,		1,	(void*)&((Ptr_GrIpaObjBindYuvPara)0)->FixVal[E_GrIpaObjBindYuvConsentY], L"Fix value Y" },
	{ L"IsFixU",			E_GrVarTypeBool8,		1,	(void*)&((Ptr_GrIpaObjBindYuvPara)0)->IsFix[E_GrIpaObjBindYuvConsentU], L"Fix U value mode on" },
	{ L"FixValU",			E_GrVarTypeByte,		1,	(void*)&((Ptr_GrIpaObjBindYuvPara)0)->FixVal[E_GrIpaObjBindYuvConsentU], L"Fix value U" },
	{ L"IsFixV",			E_GrVarTypeBool8,		1,	(void*)&((Ptr_GrIpaObjBindYuvPara)0)->IsFix[E_GrIpaObjBindYuvConsentV], L"Fix V value mode on" },
	{ L"FixValV",			E_GrVarTypeByte,		1,	(void*)&((Ptr_GrIpaObjBindYuvPara)0)->FixVal[E_GrIpaObjBindYuvConsentV], L"Fix value V" },
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrIpaObjBindYuv::Cls_GrIpaObjBindYuv( void ):
Cls_GrIpaObjBase()
{
	// local -------------------
	// code --------------------
		//init
		m_AdaptorType		=	E_GrIpaAdptTypeBindYuv;

		m_ConsentCnt		=	E_GrIpaObjBindYuvConsentCnt;
		m_PlugCnt				=	E_GrIpaObjBindYuvPlugCnt;

		m_IsBindAble		=	FALSE;

		m_GdibPlug			=	NULL;

		GrDumyZeroMem( &m_ParaBindYuv, sizeof(m_ParaBindYuv) );

		GrDumyZeroMem( m_ConsentCtrlTbl, sizeof(m_ConsentCtrlTbl) );
}
//--------------------------------------------------------------------
Cls_GrIpaObjBindYuv::~Cls_GrIpaObjBindYuv( void )
{
	// local -------------------
	// code --------------------
		LcBindBufFree();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrIpaObjBindYuv::LcParaSubDec( __u16 A_Type, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_Size;
	// code --------------------
		// inherited
		Tv_Result	=	Cls_GrIpaObjBase::LcParaSubDec( A_Type, A_PtrBuf, A_Size );
		// check type
		if ( E_GrIpaAdptTypeBindYuv == A_Type )
		{
			Tv_Size		=	A_Size;
			if ( sizeof(m_ParaBindYuv) < Tv_Size )
			{
				Tv_Size	=	sizeof(m_ParaBindYuv);
			}
			GrDumyCopyMem( &m_ParaBindYuv, A_PtrBuf, Tv_Size );
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
#ifdef GR_IPA_EDIT
void	Cls_GrIpaObjBindYuv::LcParaSubEnc( void )
{
	// local -------------------
		Ptr_GrIpaScptParaHead	Tv_PtrHd;	// header
		void*	Tv_PtrPl;				// payload
	// code --------------------
		// inherited
		Cls_GrIpaObjBase::LcParaSubEnc();

		// build header
		Tv_PtrHd	=	(Ptr_GrIpaScptParaHead)( (__u32)m_PtrEnc + m_EncPrgs );
		Tv_PtrHd->Type		=	E_GrIpaAdptTypeBindYuv;
		Tv_PtrHd->Size		=	sizeof(m_ParaBindYuv);
		m_EncPrgs					=	m_EncPrgs + sizeof(St_GrIpaScptParaHead);

		// copy payload
		Tv_PtrPl	=	(void*)( (__u32)m_PtrEnc + m_EncPrgs );
		GrDumyCopyMem( Tv_PtrPl, &m_ParaBindYuv, (__u32)Tv_PtrHd->Size );
		m_EncPrgs					=	m_EncPrgs + Tv_PtrHd->Size;
}
#endif
//--------------------------------------------------------------------
#ifdef GR_IPA_EDIT
void	Cls_GrIpaObjBindYuv::LcParaInfoSubDump( void )
{
	// local -------------------
	// code --------------------
		Cls_GrIpaObjBase::LcParaInfoSubDump();

		LcParaInfoAdd( &m_ParaBindYuv, V_GrIpaObjBindYuvParaInfo, E_GrIpaObjBindYuvParaCnt );
}
#endif
//--------------------------------------------------------------------
void	Cls_GrIpaObjBindYuv::LcBindBufFree( void )
{
	// local -------------------
	// code --------------------

		if ( NULL != m_GdibPlug )
		{
			delete	m_GdibPlug;
			m_GdibPlug	=	NULL;
		}

}
//--------------------------------------------------------------------
BOOL8	Cls_GrIpaObjBindYuv::RtlSetupConsent( __u32 A_ConsentIdx, __u32 A_StrmType, void* A_PtrStrmInfo, __u32 A_StrmInfoSize )
{
	// local -------------------
		BOOL8	Tv_Result;
		BOOL8	Tv_IsCanBind;
		Ptr_GrIpaStrmInfo2d	Tv_PtrInfo;
		__u32	Tv_WkIdx;
		St_GrIpaMngCmdSetup	Tv_Cmd;
	// code --------------------
		Tv_Result	=	FALSE;

		// reset consent stream pointer
		for (Tv_WkIdx=0;Tv_WkIdx < E_GrIpaObjBindYuvConsentCnt;Tv_WkIdx++)
		{
			m_ConsentCtrlTbl[Tv_WkIdx].PtrStrm	=	NULL;
		}

		// check setup able
		if ( Cls_GrIpaObjBase::RtlSetupConsent(A_ConsentIdx, A_StrmType, A_PtrStrmInfo, A_StrmInfoSize ) )
		{
			// check stream type
			if ( (E_GrIpaStrmType2dByte == A_StrmType) && ( sizeof(St_GrIpaStrmInfo2d) == A_StrmInfoSize) )
			{
				// make
				Tv_PtrInfo	=	(Ptr_GrIpaStrmInfo2d)A_PtrStrmInfo;
				// check different info
				Tv_IsCanBind	=	TRUE;
				for (Tv_WkIdx=0;Tv_WkIdx < E_GrIpaObjBindYuvConsentCnt;Tv_WkIdx++)
				{
					if ( Tv_WkIdx != A_ConsentIdx )
					{
						if ( (0 != m_ConsentCtrlTbl[Tv_WkIdx].StrmInfo.ResX) || (0 != m_ConsentCtrlTbl[Tv_WkIdx].StrmInfo.ResY) )
						{
							if ( (Tv_PtrInfo->ResX != m_ConsentCtrlTbl[Tv_WkIdx].StrmInfo.ResX) || (Tv_PtrInfo->ResY != m_ConsentCtrlTbl[Tv_WkIdx].StrmInfo.ResY) )
							{
								Tv_IsCanBind	=	FALSE;
								DbgMsgPrint( "Cls_GrIpaObjBindYuv::RtlSetupConsent - Adaptor ( %d ) different stream format other consent ( %d )\n", m_ScptId, A_ConsentIdx );
								break;
							}
						}
					}
				}

				m_IsBindAble	=	Tv_IsCanBind;
				m_ConsentCtrlTbl[A_ConsentIdx].StrmInfo.ResX	=	Tv_PtrInfo->ResX;
				m_ConsentCtrlTbl[A_ConsentIdx].StrmInfo.ResY	=	Tv_PtrInfo->ResY;

				// release old GDIB
				if ( NULL != m_GdibPlug )
				{
					if ( (m_GdibPlug->GetResX() != Tv_PtrInfo->ResX) || (m_GdibPlug->GetResY() != Tv_PtrInfo->ResY) || (!m_IsBindAble) )
					{
						delete	m_GdibPlug;
						m_GdibPlug	=	NULL;
					}
				}

				// check need new GDIB
				if ( (NULL == m_GdibPlug) && m_IsBindAble )
				{
					m_GdibPlug	=	GrGdibCreate( E_GrIpaObjBindYuvPlugImgFcc, Tv_PtrInfo->ResX, Tv_PtrInfo->ResY );
					if ( NULL != m_GdibPlug )
					{
						// update stream info
						m_PlugStrmInfo.FccImg		=	E_GrIpaObjBindYuvPlugImgFcc;
						m_PlugStrmInfo.ResX			=	Tv_PtrInfo->ResX;
						m_PlugStrmInfo.ResY			=	Tv_PtrInfo->ResY;
						m_PlugStrmInfo.Pitch		=	m_GdibPlug->GetDibPitch();
						
						m_PlugStrmSize					=	m_GdibPlug->GetDibSize();
					}
					else
					{
						m_IsBindAble	=	FALSE;
						DbgMsgPrint( "Cls_GrIpaObjBindYuv::RtlSetupConsent - adaptor ( %d ) plug GDIB create fail.\n" , m_ScptId );
					}
				}

				// request plug setup
				if ( m_IsBindAble )
				{
					if ( NULL != m_PlugBindTbl[0].Adaptor )
					{
						Tv_Cmd.CmdHd.Cmd		=	E_GrIpaMngCmdSetup;
						Tv_Cmd.StrmType			=	E_GrIpaStrmTypeImage;
						Tv_Cmd.PtrStrmInfo	=	&m_PlugStrmInfo;
						Tv_Cmd.StrmInfoSize	=	sizeof(m_PlugStrmInfo);
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
				DbgMsgPrint( "Cls_GrIpaObjBindYuv::RtlSetupConsent - adaptor ( %d ) bad stream type.\n" , m_ScptId );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrIpaObjBindYuv::RtlConsentRecv( __u32 A_ConsentIdx, void* A_PtrStrm, __u32 A_StrmSize )
{
	// local -------------------
		BOOL8	Tv_Result;
		BOOL8	Tv_IsCvtAble;
		__u32	Tv_WkIdx;
		__u8*	Tv_Ptr2dY;
		__u8*	Tv_Ptr2dU;
		__u8*	Tv_Ptr2dV;
		St_GrImgFmtUYVY	Tv_ClrOrg;
		__u32	Tv_X;
		__u32	Tv_Y;
		__u32	Tv_ClrCvt;
		St_GrIpaMngCmdProcess	Tv_Cmd;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( m_IsBindAble )
		{
			// check consent index
			if ( m_ConsentCnt > A_ConsentIdx )
			{
				// update stream pointer
				m_ConsentCtrlTbl[A_ConsentIdx].PtrStrm	=	A_PtrStrm;
				// success
				Tv_Result	=	TRUE;

				// check convert able
				Tv_IsCvtAble	=	TRUE;
				for (Tv_WkIdx=0;Tv_WkIdx < E_GrIpaObjBindYuvConsentCnt;Tv_WkIdx++)
				{
					if ( (!m_ParaBindYuv.IsFix[Tv_WkIdx]) && (NULL == m_ConsentCtrlTbl[Tv_WkIdx].PtrStrm) )
					{
						Tv_IsCvtAble	=	FALSE;
					}
				}
				// do bind
				if ( Tv_IsCvtAble )
				{
					Tv_Ptr2dY		=	(__u8*)m_ConsentCtrlTbl[E_GrIpaObjBindYuvConsentY].PtrStrm;
					Tv_Ptr2dU		=	(__u8*)m_ConsentCtrlTbl[E_GrIpaObjBindYuvConsentU].PtrStrm;
					Tv_Ptr2dV		=	(__u8*)m_ConsentCtrlTbl[E_GrIpaObjBindYuvConsentV].PtrStrm;

					for (Tv_Y=0;Tv_Y < m_PlugStrmInfo.ResY;Tv_Y++)
					{
						for (Tv_X=0;Tv_X < m_PlugStrmInfo.ResX;Tv_X++)
						{
							Tv_ClrOrg.Y1		=	m_ParaBindYuv.FixVal[E_GrIpaObjBindYuvConsentY];
							Tv_ClrOrg.Y2		=	m_ParaBindYuv.FixVal[E_GrIpaObjBindYuvConsentY];
							Tv_ClrOrg.U			=	m_ParaBindYuv.FixVal[E_GrIpaObjBindYuvConsentU];
							Tv_ClrOrg.V			=	m_ParaBindYuv.FixVal[E_GrIpaObjBindYuvConsentV];

							if ( !m_ParaBindYuv.IsFix[E_GrIpaObjBindYuvConsentY] )
							{
								Tv_ClrOrg.Y1		=	*Tv_Ptr2dY;
								Tv_ClrOrg.Y2		=	*Tv_Ptr2dY;
							}
							if ( !m_ParaBindYuv.IsFix[E_GrIpaObjBindYuvConsentU] )
							{
								Tv_ClrOrg.U			=	*Tv_Ptr2dU;
							}
							if ( !m_ParaBindYuv.IsFix[E_GrIpaObjBindYuvConsentV] )
							{
								Tv_ClrOrg.V			=	*Tv_Ptr2dV;
							}

							// convert
							Tv_ClrCvt	=	GrDrawCvtColorByFcc( E_GrFccUYVY, *((__u32*)&Tv_ClrOrg), E_GrIpaObjBindYuvPlugImgFcc );
							m_GdibPlug->SetPixelVal( (__s32)Tv_X, (__s32)Tv_Y, Tv_ClrCvt );

							// next
							Tv_Ptr2dY ++;
							Tv_Ptr2dU ++;
							Tv_Ptr2dV ++;
						}
					}

					// do command
					if ( NULL != m_PlugBindTbl[0].Adaptor )
					{
						Tv_Cmd.CmdHd.Cmd	=	E_GrIpaMngCmdProcess;
						Tv_Cmd.ScptId			=	m_ScptId;
						Tv_Cmd.StrmSize		=	m_PlugStrmSize;
						Tv_Cmd.PlugIdx		=	0;
						Tv_Cmd.PtrStrm		=	m_GdibPlug->GetDibPtr();
						V_GrIpaMng->CommandInput( (Ptr_GrIpaMngCmdBase)&Tv_Cmd, sizeof(Tv_Cmd) );
					}

				}

			}
			else
			{
				DbgMsgPrint( "Cls_GrIpaObjBindYuv::RtlConsentRecv - adaptor ( %d ) bad conset index ( %d ).\n" , m_ScptId, A_ConsentIdx );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrIpaObjBindYuv::RtlConsentRecv - adaptor ( %d ) did not setup.\n" , m_ScptId );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------

