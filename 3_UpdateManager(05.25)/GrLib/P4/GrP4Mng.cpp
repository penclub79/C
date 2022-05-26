/*
		platform manager

 */

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrDebug.h>
#include <P4/GrP4Mng.h>
#include <GrDrawTool.h>
#include <GrDumyTool.h>
#include <GrStrTool.h>
#include <P4/GrP4Script.h>
#include <GrTimeTool.h>
#include <GrGdibBase.h>
#include <GrUart.h>

#include <P4/GrP4DoLbl.h>
#include <P4/GrP4DoPrgsBarLoc.h>
#include <P4/GrP4DoPrgsBarGvs.h>
#include <P4/GrP4Cfg.h>
#include <P4/GrP4DoQuad.h>
#include <P4/GrP4ApiBase.h>
#include <P4/GrP4DoFrame.h>
#include <P4/GrP4DoBtnPage.h>
#include <P4/GrP4DoStatic.h>
#include <P4/GrP4DoEdtStpStr.h>
#include <P4/GrP4DoBtnLbl.h>
#include <P4/GrP4Bios.h>
#include <P4/GrP4DoSpinNumStp.h>
#include <P4/GrP4DoSpinTxtStp.h>
#include <P4/GrP4DoRelStc.h>
#include <P4/GrP4DoSpinPageTxt.h>
#include <P4/GrP4DoSpinDdgtStp.h>
#include <P4/GrP4DoBtnStc.h>
#include <P4/GrP4DoChkbSsb.h>
#include <P4/GrP4DoChkbSsm.h>
#include <P4/GrP4DoSbarVar.h>
#include <P4/GrP4DoSbarStp.h>
#include <P4/GrP4DoRbtnStp.h>
#include <P4/GrP4DoChkbSgb.h>
#include <P4/GrP4DoChkbSgm.h>
#include <P4/GrP4DoSpinPnt.h>
#include <P4/GrP4DoSpinNumLoc.h>
#include <P4/GrP4DoSpinDdgtLoc.h>
#include <P4/GrP4DoSpinTxtLoc.h>
#include <P4/GrP4DoTimeCtrl.h>
#include <P4/GrP4DoCalRec.h>
#include <P4/GrP4DoGridRscStp.h>
#include <P4/GrP4DoBtnRelStc.h>
#include <P4/GrP4DoGridRstat.h>
#include <P4/GrP4DoTimeLbl.h>

#include <P4/GrP4DoLbxCtlg.h>

#include <P4/GrP4DoVkbdTxt.h>
#include <P4/GrP4DoVkbdNum.h>

#include <P4/GrP4DoNotify.h>
#include <P4/GrP4DoQuery.h>

#include <P4/GrP4DoSpinQuadStp.h>
#include <P4/GrP4DoRbtnGvu.h>
#include <P4/GrP4DoRbtnSsm.h>
#include <P4/GrP4DoChkbLoc.h>
#include <P4/GrP4DoEdtBase.h>
#include <P4/GrP4DoEdtLoc.h>
#include <P4/GrP4DoSpinId.h>
#include <P4/GrP4DoGridMtnStp.h>
#include <P4/GrP4DoCalHday.h>
#include <P4/GrP4DoPnlPtz.h>

#include <P4/GrP4DoSpinBtxtStp.h>
#include <P4/GrP4DoGridRrng.h>

#include <P4/GrP4DoSpinValStp.h>

#include <P4/GrP4DoGridPlayBar.h>

#include <P4/GrP4DoTreeLtxt.h>
#include <P4/GrP4DoPlayBar.h>

#include <P4/GrP4UiCmd.h>


#ifdef GR_P4_EDITOR
#include	<P4/Dvl/GrP4DvlBase.h>
#include	<P4/Dvl/GrP4DvlMng.h>
#endif

//====================================================================
//local const

#define E_GrP4MngTickPeriod			100			/* 1/10 sec */
#define E_GrP4MngMsgRestTime		10

//====================================================================
//local type

//====================================================================
//local macro

//====================================================================
//local function

//====================================================================
//var
Cls_GrP4Mng*	V_GrP4Mng	=	NULL;

__u32	V_GrP4UartSpeedTbl[E_GrP4CfgUartSpeedCnt]	=	
{	1200, 2400, 4800, 9600, 19200	};

#ifdef GR_P4_EDITOR

St_GrP4DvlDoPrInfoData	V_GrP4SrgstInfo[E_GrP4RgstSysCnt]	=
{
	{	L"IsPanic(BOOL8)",			TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4RgstSys)0)->IsPanic,							L"Panic(emergency) mode"	},
	{	L"LiveQdMode(__u8)",		TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4RgstSys)0)->LiveQdMode,					L"Live quad mode"	},
	{	L"LiveQdPage(__u8)",		TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4RgstSys)0)->LiveQdPage,					L"Live quad page"	},
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4Mng::Cls_GrP4Mng( Cls_GrTaskSvr* A_TaskSvr ):
Cls_GrTaskCli()
{
	// local -------------------
	// code --------------------
		//init
		m_TaskSvr				=	A_TaskSvr;
		m_MsgAloc				=	GrTaskSvrMsgCmdAlloc();

		m_PtrScptBuf		=	NULL;
		m_ScptSize			=	0;
		m_ScptDecPrgs		=	0;
		m_IsScptAble		=	FALSE;
		m_IsScptErr			=	FALSE;

		m_IsKeyBuzRun	=	FALSE;

		m_MseX			=	0;
		m_MseY			=	0;
		m_MseBtnMap	=	0;
		m_MseSenSl	=	0;
		m_MseSenSr	=	0;

		//A_VmIdx			=	0;
		m_FcsConIdx	=	0;
		m_EventLock	=	0;

		m_PtrWinTbl		=	NULL;
		m_WinCnt		=	0;

		m_PtrPageTbl		=	NULL;
		m_PageCnt		=	0;

		m_PtrImgTbl		=	m_ImgScptTbl;
		m_PtrTxtTbl		=	m_TxtScptTbl;

		m_PtrAsmTbl		=	NULL;
		m_AsmCnt			=	0;

		m_PtrPstTbl		=	NULL;
		m_PstCnt			=	0;

		m_LangIdx			=	0;

		m_UiAlphaVal	=	0xFF;

		m_HdayCtlgPageSize	=	0;
		m_HdayCtlgPageIdx		=	0;
		m_HdayCtlgPageCnt		=	0;

		m_PtzMng			=	NULL;

		m_KeyBlock		=	0;

		m_VlossStat		=	0;

		m_ObjFindId		=	0;

		m_IsAtLock		=	FALSE;
		m_AtLockTimer	=	0;
		m_AtLockPeriod	=	0;

		// init
		m_SysStat			=	E_GrP4MngSysStatNone;

		GrDumyZeroMem( m_ConTbl, sizeof(m_ConTbl) );
		GrDumyZeroMem( m_ConMng, sizeof(m_ConMng) );
		GrDumyZeroMem( m_VmTbl, sizeof(m_VmTbl) );
		GrDumyZeroMem( m_FontMng, sizeof(m_FontMng) );
		GrDumyZeroMem( m_ImgUserTbl, sizeof(m_ImgUserTbl) );

		GrDumyZeroMem( &m_GvuTbl, sizeof(m_GvuTbl) );
		GrDumyZeroMem( &m_Gvs, sizeof(m_Gvs) );
		GrDumyZeroMem( m_SdaBuf, sizeof(m_SdaBuf) );

		GrDumyZeroMem( m_LiveAtbTbl, sizeof(m_LiveAtbTbl) );

		m_PtrCfgPrj		=	NULL;
		m_PtrCfgRtl		=	NULL;
		m_PtrCfgMdl		=	NULL;
		GrDumyZeroMem( m_TxtScptTbl, sizeof(m_TxtScptTbl) );
		GrDumyZeroMem( m_ImgScptTbl, sizeof(m_ImgScptTbl) );

		// reset GVS table
		LcGvsReset();

		//register server
		m_TaskSvr->RegistMsgClient( this );
		m_TaskSvr->RegistTimerClient( this, E_GrP4MngTickPeriod, &m_HndlTimer );
}
//--------------------------------------------------------------------
Cls_GrP4Mng::~Cls_GrP4Mng( void )
{
		//unregister server
		m_TaskSvr->UnregistTimerClient( m_HndlTimer );
		m_TaskSvr->UnregistMsgClient( this );

		GrTaskSvrMsgCmdFree( m_MsgAloc );

		LcFontReset();		// release font

}
//--------------------------------------------------------------------
BOOL8	GrP4MngInit( Cls_GrTaskSvr* A_TaskSvr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( NULL == V_GrP4Mng )
		{
			V_GrP4Mng	=	(Cls_GrP4Mng*)new Cls_GrP4Mng( A_TaskSvr );
			Tv_Result	=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrP4MngFinish( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( NULL != V_GrP4Mng )
		{
			delete	V_GrP4Mng;
			V_GrP4Mng	=	NULL;
			Tv_Result	=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
Cls_GrP4ConBase*	Cls_GrP4Mng::ConsoleGet( __u8 A_ConIdx )
{
	// local -------------------
		Cls_GrP4ConBase*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;
		if ( E_GrP4ConIdxFocus == A_ConIdx )
		{
			Tv_Result	=	m_ConTbl[m_FcsConIdx];
		}
		else if ( E_GrP4MngConMaxCnt > A_ConIdx )
		{
			Tv_Result	=	m_ConTbl[m_FcsConIdx];
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4Mng::UiMsgPost( Ptr_GrP4MsgHd A_PtrMsg, __u32 A_Size )
{
		return	m_TaskSvr->MsgExPost( E_GrTaskMsgUiMsg, (void*)A_PtrMsg, A_Size );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4Mng::UiMsgPump( void )
{
		return	m_TaskSvr->DoStep();
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::UiMsgRest( void )
{
		GrPrcSleep( E_GrP4MngMsgRestTime );
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcFontReset( void )
{
	// local -------------------
		__u8	Tv_WkIdx;
	// code --------------------
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP4MngFontMaxCnt;Tv_WkIdx++ )
		{
			if ( NULL != m_FontMng[Tv_WkIdx] )
			{
				delete	m_FontMng[Tv_WkIdx];
				m_FontMng[Tv_WkIdx]	=	NULL;
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4Mng::LcScptDecHead( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( E_GrP4ScptFcc == ((Ptr_GrP4ScptHdInfo)m_PtrScptBuf)->Fcc )
		{
			if ( E_GrP4ScptVer == ((Ptr_GrP4ScptHdInfo)m_PtrScptBuf)->Ver )
			{
				m_Gvs.ScptTime	=	((Ptr_GrP4ScptHdInfo)m_PtrScptBuf)->Time;		// update time
				m_ScptDecPrgs		=	m_ScptDecPrgs	+ sizeof(St_GrP4ScptHdInfo);
				Tv_Result	=	TRUE;
			}
			else
			{
				DbgMsgPrint( "Cls_GrP4Mng::LcScptHeadChk - script version ( %d -> %d ) mismatch!\n", E_GrP4ScptVer, ((Ptr_GrP4ScptHdInfo)m_PtrScptBuf)->Ver );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP4Mng::LcScptHeadChk - bad FCC code ( %08X )!\n", ((Ptr_GrP4ScptHdInfo)m_PtrScptBuf)->Fcc );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4Mng::LcScptDecCfgPrj( Ptr_GrP4ScptDataHd A_PtrDtHd )
{
	// local -------------------
	// code --------------------

		if ( sizeof(St_GrP4ScptCfgPrj) != A_PtrDtHd->Size )
		{
			DbgMsgPrint( "Cls_GrP4Mng::LcScptDecCfgPrj - (warnning) different size ( %d -> %d ).\n", sizeof(St_GrP4ScptCfgPrj), A_PtrDtHd->Size );
		}
		m_PtrCfgPrj	=	(Ptr_GrP4ScptCfgPrj)( (__u32)A_PtrDtHd + sizeof(St_GrP4ScptDataHd) );	// update pointer

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4Mng::LcScptDecCfgRtl( Ptr_GrP4ScptDataHd A_PtrDtHd )
{
	// local -------------------
	// code --------------------

		if ( sizeof(St_GrP4ScptCfgRtl) != A_PtrDtHd->Size )
		{
			DbgMsgPrint( "Cls_GrP4Mng::LcScptDecCfgRtl - (warnning) different size ( %d -> %d ).\n", sizeof(St_GrP4ScptCfgRtl), A_PtrDtHd->Size );
		}
		m_PtrCfgRtl	=	(Ptr_GrP4ScptCfgRtl)( (__u32)A_PtrDtHd + sizeof(St_GrP4ScptDataHd) );	// update pointer

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4Mng::LcScptDecMdlAtb( Ptr_GrP4ScptDataHd A_PtrDtHd )
{
	// local -------------------
	// code --------------------

		if ( sizeof(St_GrP4ScptCfgMdl) != A_PtrDtHd->Size )
		{
			DbgMsgPrint( "Cls_GrP4Mng::LcScptDecMdlAtb - (warnning) different size ( %d -> %d ).\n", sizeof(St_GrP4ScptCfgMdl), A_PtrDtHd->Size );
		}
		m_PtrCfgMdl	=	(Ptr_GrP4ScptCfgMdl)( (__u32)A_PtrDtHd + sizeof(St_GrP4ScptDataHd) );	// update pointer

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4Mng::LcScptDecPstTbl( Ptr_GrP4ScptDataHd A_PtrDtHd )
{
	// local -------------------
		__u32	Tv_WkIdx;
	// code --------------------
		m_PstCnt		=	A_PtrDtHd->Size / sizeof(St_GrP4ScptPstItm);
		m_PtrPstTbl	=	(Ptr_GrP4ScptPstItm)( (__u32)A_PtrDtHd + sizeof(St_GrP4ScptDataHd) );	// update pointer
		DbgMsgPrint( "[P4] PST table count = %d.\n", m_PstCnt );
		// reset pointer
		for ( Tv_WkIdx=0;Tv_WkIdx < m_PstCnt;Tv_WkIdx++ )
		{
			m_PtrPstTbl[Tv_WkIdx].Pos		=	NULL;		// reset pointer
		}

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4Mng::LcScptDecPstScpt( Ptr_GrP4ScptDataHd A_PtrDtHd )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_Size;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( NULL != m_PtrPstTbl )
		{
			// check index
			if ( A_PtrDtHd->Id < (__u16)m_PstCnt )
			{
				// check count
				Tv_Size	=	sizeof(St_GrP4ScptPstData) * ((__u32)m_PtrPstTbl[A_PtrDtHd->Id].Cnt);
				if ( A_PtrDtHd->Size == Tv_Size )
				{
					// check alreay allocated
					if ( NULL == m_PtrPstTbl[A_PtrDtHd->Id].Pos )
					{
						m_PtrPstTbl[A_PtrDtHd->Id].Pos	=	(void*)( (__u32)A_PtrDtHd + sizeof(St_GrP4ScptDataHd) );	// set pointer
						Tv_Result	=	TRUE;
					}
					else
					{
						DbgMsgPrint( "Cls_GrP4Mng::LcScptDecPstScpt - bad PST ( %d ) already loaded.\n", A_PtrDtHd->Id );
					}
				}
				else
				{
					DbgMsgPrint( "Cls_GrP4Mng::LcScptDecPstScpt - bad PST ( %d ) command count ( %d -> %d ).\n", A_PtrDtHd->Id, Tv_Size , A_PtrDtHd->Size );
				}

			}
			else
			{
				DbgMsgPrint( "Cls_GrP4Mng::LcScptDecPstScpt - bad PST index ( %d ).\n", A_PtrDtHd->Id );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP4Mng::LcScptDecPstScpt - PST table not loaded.\n" );
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4Mng::LcScptDecWinTbl( Ptr_GrP4ScptDataHd A_PtrDtHd )
{
	// local -------------------
		__u32	Tv_WkIdx;
	// code --------------------
		m_WinCnt		=	A_PtrDtHd->Size / sizeof(St_GrP4ScptWinInfo);
		m_PtrWinTbl	=	(Ptr_GrP4ScptWinInfo)( (__u32)A_PtrDtHd + sizeof(St_GrP4ScptDataHd) );	// update pointer
		DbgMsgPrint( "[P4] Window table count = %d.\n", m_WinCnt );
		// reset pointer
		for ( Tv_WkIdx=0;Tv_WkIdx < m_WinCnt;Tv_WkIdx++ )
		{
			m_PtrWinTbl[Tv_WkIdx].Pos		=	NULL;		// reset pointer
		}

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4Mng::LcScptDecWinScpt( Ptr_GrP4ScptDataHd A_PtrDtHd )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( NULL != m_PtrWinTbl )
		{
			// check index
			if ( A_PtrDtHd->Id < (__u16)m_WinCnt )
			{
				// check alreay allocated
				if ( NULL == m_PtrWinTbl[A_PtrDtHd->Id].Pos )
				{
					m_PtrWinTbl[A_PtrDtHd->Id].Pos	=	(void*)( (__u32)A_PtrDtHd + sizeof(St_GrP4ScptDataHd) );	// set pointer
					Tv_Result	=	TRUE;
				}
				else
				{
					DbgMsgPrint( "Cls_GrP4Mng::LcScptDecWinScpt - bad Window ( %d ) already loaded.\n", A_PtrDtHd->Id );
				}

			}
			else
			{
				DbgMsgPrint( "Cls_GrP4Mng::LcScptDecWinScpt - bad Window index ( %d ).\n", A_PtrDtHd->Id );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP4Mng::LcScptDecWinScpt - Window table not loaded.\n" );
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4Mng::LcScptDecPageTbl( Ptr_GrP4ScptDataHd A_PtrDtHd )
{
	// local -------------------
		__u32	Tv_WkIdx;
	// code --------------------
		m_PageCnt		=	A_PtrDtHd->Size / sizeof(St_GrP4ScptPageInfo);
		m_PtrPageTbl	=	(Ptr_GrP4ScptPageInfo)( (__u32)A_PtrDtHd + sizeof(St_GrP4ScptDataHd) );	// update pointer
		DbgMsgPrint( "[P4] Page table count = %d.\n", m_PageCnt );
		// reset pointer
		for ( Tv_WkIdx=0;Tv_WkIdx < m_PageCnt;Tv_WkIdx++ )
		{
			m_PtrPageTbl[Tv_WkIdx].Pos		=	NULL;		// reset pointer
		}

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4Mng::LcScptDecPageScpt( Ptr_GrP4ScptDataHd A_PtrDtHd )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( NULL != m_PtrPageTbl )
		{
			// check index
			if ( A_PtrDtHd->Id < (__u16)m_PageCnt )
			{
				// check alreay allocated
				if ( NULL == m_PtrPageTbl[A_PtrDtHd->Id].Pos )
				{
					m_PtrPageTbl[A_PtrDtHd->Id].Pos	=	(void*)( (__u32)A_PtrDtHd + sizeof(St_GrP4ScptDataHd) );	// set pointer
					Tv_Result	=	TRUE;
				}
				else
				{
					DbgMsgPrint( "Cls_GrP4Mng::LcScptDecPageScpt - bad Page ( %d ) already loaded.\n", A_PtrDtHd->Id );
				}

			}
			else
			{
				DbgMsgPrint( "Cls_GrP4Mng::LcScptDecPageScpt - bad Page index ( %d ).\n", A_PtrDtHd->Id );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP4Mng::LcScptDecPageScpt - Page table not loaded.\n" );
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4Mng::LcScptDecAsmTbl( Ptr_GrP4ScptDataHd A_PtrDtHd )
{
	// local -------------------
		__u32	Tv_WkIdx;
	// code --------------------
		m_AsmCnt		=	A_PtrDtHd->Size / sizeof(St_GrP4ScptAsmInfo);
		m_PtrAsmTbl	=	(Ptr_GrP4ScptAsmInfo)( (__u32)A_PtrDtHd + sizeof(St_GrP4ScptDataHd) );	// update pointer
		DbgMsgPrint( "[P4] ASM table count = %d.\n", m_AsmCnt );
		// reset pointer
		for ( Tv_WkIdx=0;Tv_WkIdx < m_AsmCnt;Tv_WkIdx++ )
		{
			m_PtrAsmTbl[Tv_WkIdx].Pos		=	NULL;		// reset pointer
		}

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4Mng::LcScptDecAsmCode( Ptr_GrP4ScptDataHd A_PtrDtHd )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_Size;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( NULL != m_PtrAsmTbl )
		{
			// check index
			if ( A_PtrDtHd->Id < (__u16)m_AsmCnt )
			{
				// check count
				Tv_Size	=	E_GrP4AsmOpUnitSize * ((__u32)m_PtrAsmTbl[A_PtrDtHd->Id].Cnt);
				if ( A_PtrDtHd->Size == Tv_Size )
				{
					// check alreay allocated
					if ( NULL == m_PtrAsmTbl[A_PtrDtHd->Id].Pos )
					{
						m_PtrAsmTbl[A_PtrDtHd->Id].Pos	=	(void*)( (__u32)A_PtrDtHd + sizeof(St_GrP4ScptDataHd) );	// set pointer
						Tv_Result	=	TRUE;
					}
					else
					{
						DbgMsgPrint( "Cls_GrP4Mng::LcScptDecAsmCode - bad ASM ( %d ) already loaded.\n", A_PtrDtHd->Id );
					}
				}
				else
				{
					DbgMsgPrint( "Cls_GrP4Mng::LcScptDecAsmCode - bad ASM ( %d ) code count ( %d -> %d ).\n", A_PtrDtHd->Id, Tv_Size , A_PtrDtHd->Size );
				}

			}
			else
			{
				DbgMsgPrint( "Cls_GrP4Mng::LcScptDecAsmCode - bad ASM index ( %d ).\n", A_PtrDtHd->Id );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP4Mng::LcScptDecAsmCode - ASM table not loaded.\n" );
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4Mng::LcScptDecImgData( Ptr_GrP4ScptDataHd A_PtrDtHd )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrP4MngImgScptMaxCnt > A_PtrDtHd->Id )
		{
			if ( NULL == m_ImgScptTbl[A_PtrDtHd->Id] )
			{
				m_ImgScptTbl[A_PtrDtHd->Id]	=	(Ptr_GrGdib)( (__u32)A_PtrDtHd + sizeof(St_GrP4ScptDataHd) );	// update pointer
				Tv_Result	=	TRUE;
			}
			else
			{
				DbgMsgPrint( "Cls_GrP4Mng::LcScptDecImgData - Image ( %d ) already allocated.\n" , A_PtrDtHd->Id );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP4Mng::LcScptDecImgData - bad index ( %d ).\n" , A_PtrDtHd->Id );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4Mng::LcScptDecTextData( Ptr_GrP4ScptDataHd A_PtrDtHd )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( (E_GrP4MngTextScptMaxCnt > A_PtrDtHd->Id) && ( E_GrP4ScptLangMaxCnt > A_PtrDtHd->SubIdx ) )
		{
			if ( NULL == m_TxtScptTbl[A_PtrDtHd->Id].LngTbl[A_PtrDtHd->SubIdx] )
			{
				m_TxtScptTbl[A_PtrDtHd->Id].LngTbl[A_PtrDtHd->SubIdx]	=	(WCHAR*)( (__u32)A_PtrDtHd + sizeof(St_GrP4ScptDataHd) );	// update pointer
				Tv_Result	=	TRUE;
			}
			else
			{
				DbgMsgPrint( "Cls_GrP4Mng::LcScptDecTextData - Text ( %d : %d ) already allocated.\n" , A_PtrDtHd->Id , A_PtrDtHd->SubIdx );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP4Mng::LcScptDecTextData - bad index ( %d : %d ).\n" , A_PtrDtHd->Id, A_PtrDtHd->SubIdx );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4Mng::LcScptDecFontData( Ptr_GrP4ScptDataHd A_PtrDtHd )
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrP4FontHead	Tv_PtrFont;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrP4MngFontMaxCnt > A_PtrDtHd->Id )
		{
			if ( NULL == m_FontMng[A_PtrDtHd->Id] )
			{
				Tv_PtrFont	=	(Ptr_GrP4FontHead)( (__u32)A_PtrDtHd + sizeof(St_GrP4ScptDataHd) );	// update pointer
				m_FontMng[A_PtrDtHd->Id]	=	(Cls_GrP4Font*)new Cls_GrP4Font( Tv_PtrFont );
#ifndef GR_P4_EDITOR
				m_FontMng[A_PtrDtHd->Id]->CodeTblBuild();
#endif
				Tv_Result	=	TRUE;
			}
			else
			{
				DbgMsgPrint( "Cls_GrP4Mng::LcScptDecFontData - Font ( %d ) already allocated.\n" , A_PtrDtHd->Id );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP4Mng::LcScptDecFontData - bad index ( %d ).\n" , A_PtrDtHd->Id );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4Mng::ScriptSet( void* A_PtrScpt, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrP4ScptDataHd	Tv_PtrDtHd;
		__u8	Tv_WkIdx;
	// code --------------------
		Tv_Result			=	FALSE;

		m_PtrScptBuf	=	A_PtrScpt;
		m_ScptSize		=	A_Size;
		m_ScptDecPrgs	=	0;
		m_IsScptErr		=	FALSE;

		m_PtrCfgPrj		=	NULL;
		m_PtrCfgRtl		=	NULL;
		m_PtrCfgMdl		=	NULL;
		m_PtrPstTbl		=	NULL;
		m_PstCnt			=	0;
		m_PtrWinTbl		=	NULL;
		m_WinCnt			=	0;
		m_PtrPageTbl	=	NULL;
		m_PageCnt			=	0;
		m_PtrAsmTbl		=	NULL;
		m_AsmCnt			=	0;
		GrDumyZeroMem( m_TxtScptTbl, sizeof(m_TxtScptTbl) );
		GrDumyZeroMem( m_ImgScptTbl, sizeof(m_ImgScptTbl) );

		// reset all object
		LcFontReset();
		// check exist script
		if ( NULL != m_PtrScptBuf )
		{
			// check script header
			if ( LcScptDecHead() )
			{
				Tv_Result	=	TRUE;
				while ( (m_ScptDecPrgs < m_ScptSize) && Tv_Result )
				{
					// get data head
					Tv_PtrDtHd	=	(Ptr_GrP4ScptDataHd)( (__u32)m_PtrScptBuf + m_ScptDecPrgs );
					switch ( Tv_PtrDtHd->Type )
					{
						case E_GrP4ScptDataCfgPrj:
							Tv_Result	=	LcScptDecCfgPrj( Tv_PtrDtHd );
							break;
						case E_GrP4ScptDataCfgRtl:
							Tv_Result	=	LcScptDecCfgRtl( Tv_PtrDtHd );
							break;
						case E_GrP4ScptDataMdlAtb:
							Tv_Result	=	LcScptDecMdlAtb( Tv_PtrDtHd );
							break;
						case E_GrP4ScptDataPstTbl:
							Tv_Result	=	LcScptDecPstTbl( Tv_PtrDtHd );
							break;
						case E_GrP4ScptDataPstScptData:
							Tv_Result	=	LcScptDecPstScpt( Tv_PtrDtHd );
							break;
						case E_GrP4ScptDataWinScptTbl:
							Tv_Result	=	LcScptDecWinTbl( Tv_PtrDtHd );
							break;
						case E_GrP4ScptDataWinScptData:
							Tv_Result	=	LcScptDecWinScpt( Tv_PtrDtHd );
							break;
						case E_GrP4ScptDataPageScptTbl:
							Tv_Result	=	LcScptDecPageTbl( Tv_PtrDtHd );
							break;
						case E_GrP4ScptDataPageScptData:
							Tv_Result	=	LcScptDecPageScpt( Tv_PtrDtHd );
							break;
						case E_GrP4ScptDataAsmTbl:
							Tv_Result	=	LcScptDecAsmTbl( Tv_PtrDtHd );
							break;
						case E_GrP4ScptDataAsmData:
							Tv_Result	=	LcScptDecAsmCode( Tv_PtrDtHd );
							break;
						case E_GrP4ScptDataImageData:
							Tv_Result	=	LcScptDecImgData( Tv_PtrDtHd );
							break;
						case E_GrP4ScptDataTextData:
							Tv_Result	=	LcScptDecTextData( Tv_PtrDtHd );
							break;
						case E_GrP4ScptDataFontData:
							Tv_Result	=	LcScptDecFontData( Tv_PtrDtHd );
							break;

						default:
							DbgMsgPrint( "Cls_GrP4Mng::ScriptSet - unknown data type ( %d ) at %08X .\n", Tv_PtrDtHd->Type, m_ScptDecPrgs );
							Tv_Result	=	FALSE;
							m_IsScptErr	=	TRUE;
							break;
					}	//	switch ( Tv_PtrDtHd->Type )
					// next
					m_ScptDecPrgs	=	m_ScptDecPrgs + Tv_PtrDtHd->Size + sizeof(St_GrP4ScptDataHd);
				}	//	while ( (m_ScptDecPrgs < m_ScptSize) && Tv_Result )

			}

			// check able
			if ( Tv_Result )
			{
				if ( (NULL != m_PtrCfgPrj) && (NULL != m_PtrCfgRtl) && (NULL != m_PtrCfgMdl) )
				{
					m_IsScptAble	=	TRUE;
				}
				else
				{
					DbgMsgPrint( "Cls_GrP4Mng::ScriptSet - CFGRTL, CFGPRJ, CFGMDL not exist!\n" );
				}
			}

			// change notify to console
			for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP4MngConMaxCnt;Tv_WkIdx++ )
			{
				if ( NULL != m_ConTbl[Tv_WkIdx] )
				{
					m_ConTbl[Tv_WkIdx]->RtlScptUpdt();
				}
			}

		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Ptr_GrGdib	Cls_GrP4Mng::ImgGet( __u16 A_Id )
{
	// local -------------------
		Ptr_GrGdib	Tv_Result;
		__u16	Tv_Idx;
	// code --------------------
		Tv_Result	=	NULL;
		if ( E_GrP4ImgUserIdStart > A_Id )
		{
			// find normal image
			if ( E_GrP4ImgIdBase <= A_Id )
			{
				Tv_Idx	=	A_Id - E_GrP4ImgIdBase;
				if ( E_GrP4MngImgScptMaxCnt > Tv_Idx )
				{
					Tv_Result	=	m_PtrImgTbl[Tv_Idx];
				}
			}
		}
		else
		{
			// user image
			Tv_Idx	=	A_Id - E_GrP4ImgUserIdStart;
			if ( E_GrP4MngImgUserMaxCnt > Tv_Idx )
			{
				Tv_Result	=	m_ImgUserTbl[Tv_Idx];
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	GrP4MngGetWinIdInfo( __u16 A_WinId, __u8* A_PtrRtLayer, __u8* A_PtrRtSlot )
{
	// local -------------------
	// code --------------------
		*A_PtrRtLayer	=	(__u8)( (A_WinId >> 12) & 0x0F );
		*A_PtrRtSlot	=	(__u8)( (A_WinId >> 8) & 0x0F );
}
//--------------------------------------------------------------------
__u16	GrP4MngMakeWinId( __u8 A_Layer, __u8 A_Slot, __u8 A_Seq )
{
		return	(__u16)A_Seq | ( ((__u16)A_Layer) << 12 ) | ( ((__u16)A_Slot) << 8 );
}
//--------------------------------------------------------------------
__u32	GrP4MngMakeCtlgHndl( __u8 A_Id, __u32 A_ItemIdx )
{
		return	((__u32)A_Id << E_GrP4MngCtlgIdShft) | A_ItemIdx;
}
//--------------------------------------------------------------------
__u32	GrP4MngCtlgHndlToIdx( __u32 A_CtlgHndl, __u32* A_PtrRtId )
{
	// local -------------------
	// code --------------------
		*A_PtrRtId	=	(A_CtlgHndl >> E_GrP4MngCtlgIdShft) & E_GrP4MngCtlgIdMask;
		return	A_CtlgHndl & E_GrP4MngCtlgValMask;
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::KeyInput( __u8 A_Key, BOOL8 A_IsPush, __u8 A_DevType )
{
	// local -------------------
		St_GrP4MsgKey	Tv_Msg;
		__s32		Tv_Rint[2];
		BOOL8	Tv_IsAble;
	// code --------------------
		// check able
		if ( m_IsScptAble )
		{
			// check able key
			if ( 0 != A_Key )
			{
				// check change focus key
				if ( E_GrP4KeyDisplay == A_Key)
				{
					if ( A_IsPush && m_PtrCfgRtl->IsChgConAble )
					{
						// change focus
						// not implement
					}
				}
				else
				{
					if ( A_Key != m_KeyBlock )
					{
						// check REMOCON able
						Tv_IsAble	=	TRUE;
						if ( (E_GrP4KeyId <= A_Key) && (E_GrP4KeySelId >= A_Key) )
						{
							if ( E_GrP4KeySelId != A_Key )
							{
								m_Gvs.RunRmtId	=	A_Key - E_GrP4KeyId;
							}
						}
						else 
						{
							if ( (E_GrP4InDevRemocon == A_DevType) && (!m_Gvs.IsRmtTmpAble) )
							{
								if ( m_Gvs.RunRmtId != m_Gvs.CrtRmtId )
								{
									Tv_IsAble	=	FALSE;
								}
							}
						}

						if ( Tv_IsAble )
						{
							// post key message
							Tv_Msg.Hd.Cmd			=	E_GrP4MsgCmdConInpKey;
							Tv_Msg.Hd.ConIdx	=	m_FcsConIdx;
							Tv_Msg.Hd.WinId		=	E_GrP4WinIdNone;
							Tv_Msg.Hd.ObjId		=	E_GrP4ScptIdNone;
							Tv_Msg.Dev				=	A_DevType;
							Tv_Msg.Key				=	A_Key;
							Tv_Msg.IsPush			=	A_IsPush;
							UiMsgPost( (Ptr_GrP4MsgHd)&Tv_Msg, sizeof(Tv_Msg) );
						}
					}
					else
					{
						if ( !A_IsPush )
						{
							m_KeyBlock	=	0;
						}
					}
				}

				if ( m_IsKeyBuzRun && A_IsPush )
				{
					Tv_Rint[1]	=	1;
					GrP4ApiCall(E_GrP4ApiBuzRun, Tv_Rint, NULL, NULL);
				}

				// auto lock time reset
				m_AtLockTimer	=	m_AtLockPeriod;
			}
		}	//	if ( m_IsScptAble )
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcProcMsgBrdcst( Ptr_GrP4MsgHd A_PtrMsg )
{
	// local -------------------
		__u8	Tv_ConIdx;
	// code --------------------
		for ( Tv_ConIdx=0;Tv_ConIdx < E_GrP4MngConMaxCnt;Tv_ConIdx++ )
		{
			if ( NULL != m_ConTbl[Tv_ConIdx] )
			{
				A_PtrMsg->ConIdx	=	Tv_ConIdx;
				m_ConTbl[Tv_ConIdx]->RtlUiMsgProc( A_PtrMsg );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcProcMsgUiMsg( Ptr_GrP4MsgHd A_PtrMsg )
{
	// local -------------------
	// code --------------------
		// check broadcast message
		if ( E_GrP4ConIdxAll == A_PtrMsg->ConIdx )
		{
			LcProcMsgBrdcst( A_PtrMsg );
		}
		else
		{
			if ( E_GrP4MngConMaxCnt > A_PtrMsg->ConIdx )
			{
				if ( NULL != m_ConTbl[A_PtrMsg->ConIdx] )
				{
					m_ConTbl[A_PtrMsg->ConIdx]->RtlUiMsgProc( A_PtrMsg );
				}
			}
			else
			{
				DbgMsgPrint( "Cls_GrP4Mng::LcProcMsgUiMsg - bad console index %d\n", A_PtrMsg->ConIdx );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::RtlTimer( __u32 A_Hndl )
{
	// local -------------------
		__u32	Tv_Tick;
	// code --------------------
		//get tick
		Tv_Tick	=	GrTimeGetTick();
		//broadcast tick
		LcTickMsgBrdcst( Tv_Tick );
		// check VM resume
		LcVmProcTimer();
		// update video loss
		LcVlossUpdt();
		// auto lock
		RtlAtLockProc();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4Mng::RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg )
{
	// local -------------------
		BOOL8	Tv_Result;
		void*		Tv_PtrPara;
		Ptr_GrP4MngMsgBase	Tv_PtrHd;
		__u8	Tv_ConIdx;
	// code --------------------
		// init
		Tv_Result	=	TRUE;

		if ( m_MsgAloc == A_PtrMsg->Cmd )
		{
			Tv_PtrHd	=	(Ptr_GrP4MngMsgBase)A_PtrMsg;
			// message command
			switch ( Tv_PtrHd->Cmd )
			{
				case E_GrP4MngCmdQuadModeSet:
					Tv_ConIdx	=	((Ptr_GrP4MngMsgQuadModeSet)Tv_PtrHd)->ConIdx;
					if ( E_GrP4MngConMaxCnt > Tv_ConIdx )
					{
						if ( NULL != m_ConTbl[Tv_ConIdx] )
						{
							m_ConTbl[Tv_ConIdx]->QuadModeSet( ((Ptr_GrP4MngMsgQuadModeSet)Tv_PtrHd)->Mode, ((Ptr_GrP4MngMsgQuadModeSet)Tv_PtrHd)->Page, ((Ptr_GrP4MngMsgQuadModeSet)Tv_PtrHd)->Fcc );
						}
					}
					break;
				case E_GrP4MngCmdQuadModeNext:
					Tv_ConIdx	=	((Ptr_GrP4MngMsgQuadMovePara)Tv_PtrHd)->ConIdx;
					if ( E_GrP4MngConMaxCnt > Tv_ConIdx )
					{
						if ( NULL != m_ConTbl[Tv_ConIdx] )
						{
							m_ConTbl[Tv_ConIdx]->QuadModeNext( ((Ptr_GrP4MngMsgQuadMovePara)Tv_PtrHd)->Fcc );
						}
					}
					break;
				case E_GrP4MngCmdQuadModePrev:
					Tv_ConIdx	=	((Ptr_GrP4MngMsgQuadMovePara)Tv_PtrHd)->ConIdx;
					if ( E_GrP4MngConMaxCnt > Tv_ConIdx )
					{
						if ( NULL != m_ConTbl[Tv_ConIdx] )
						{
							m_ConTbl[Tv_ConIdx]->QuadModePrev( ((Ptr_GrP4MngMsgQuadMovePara)Tv_PtrHd)->Fcc );
						}
					}
					break;
				case E_GrP4MngCmdQuadPageNext:
					Tv_ConIdx	=	((Ptr_GrP4MngMsgQuadMovePara)Tv_PtrHd)->ConIdx;
					if ( E_GrP4MngConMaxCnt > Tv_ConIdx )
					{
						if ( NULL != m_ConTbl[Tv_ConIdx] )
						{
							m_ConTbl[Tv_ConIdx]->QuadPageNext( ((Ptr_GrP4MngMsgQuadMovePara)Tv_PtrHd)->Fcc );
						}
					}
					break;
				case E_GrP4MngCmdQuadModeGet:
					Tv_ConIdx	=	((Ptr_GrP4MngMsgQuadModeGet)Tv_PtrHd)->ConIdx;
					if ( E_GrP4MngConMaxCnt > Tv_ConIdx )
					{
						if ( NULL != m_ConTbl[Tv_ConIdx] )
						{
							m_ConTbl[Tv_ConIdx]->QuadModeGet( ((Ptr_GrP4MngMsgQuadModeGet)Tv_PtrHd)->PtrRtMode, ((Ptr_GrP4MngMsgQuadModeGet)Tv_PtrHd)->PtrRtPage );
						}
					}
					break;
				case E_GrP4MngCmdAsmDcall:
					VmRunCode( ((Ptr_GrP4MngMsgAsmDcall)A_PtrMsg)->CodeId, NULL,NULL );
					break;
				case E_GrP4MngCmdDpcUpi:
					LcProcDpcUpi( (Ptr_GrP4MngMsgDpcUpi)A_PtrMsg );
					break;
				case E_GrP4MngCmdErrSetSetOne:
					LcProcMsgErrSetOne( (Ptr_GrP4MngMsgErrSetOne)A_PtrMsg );
					break;
				case E_GrP4MngCmdErrSetClrMap:
					LcProcMsgErrClrMap( (Ptr_GrP4MngMsgErrClrMap)A_PtrMsg );
					break;
				case E_GrP4MngCmdSetupSave:
					LcProcMsgSetupSave();
					break;
			}
		}
		else
		{
			Tv_PtrPara	=	(void*)( (__u32)A_PtrMsg + sizeof(St_GrTaskMsgHd) );
			switch( A_PtrMsg->Cmd )
			{
				case	E_GrTaskMsgUiMsg:
					LcProcMsgUiMsg( (Ptr_GrP4MsgHd)Tv_PtrPara );
					break;
				case	E_GrTaskMsgQuadChgReq:
					LcProcMsgQuadChgReq( (Ptr_GrTaskMsgQuadChgReq)A_PtrMsg );
					break;
				case E_GrTaskMsgDiskFormat:
					// notify HDD format
					LcProcMsgFormat( (Ptr_GrTaskMsgDiskFormat)A_PtrMsg );
					break;
				case E_GrTaskMsgUiEvt:
					LcProcMsgEvtCall( (Ptr_GrP4EvtCallMsg)Tv_PtrPara );
					break;
				case E_GrTaskMsgFwUpgdFinish:
					LcProcMsgFwUpgdDone( (Ptr_GrTaskMsgFwUpgdEnd)A_PtrMsg );
					break;
				case E_GrTaskMsgNetOff:
					LcProcMsgNetOnOff( FALSE );
					break;
				case E_GrTaskMsgNetOn:
					LcProcMsgNetOnOff( TRUE );
					break;
				case E_GrTaskMsgPlayStatChg:
					LcProcMsgPlayStatChg();
					break;
				case E_GrTaskMsgUsbChg:
					LcProcMsgUsbChg();
					break;
				case E_GrTaskMsgCmsCliChg:
					LcProcMsgCmsCliChg();
					break;
				case E_GrTaskMsgBkupStart:
					LcProcMsgBkupStart();
					break;
				case E_GrTaskMsgBkupEnd:
					LcProcMsgBkupEnd();
					break;
				case E_GrTaskMsgBkupError:
					LcProcMsgBkupErr();
					break;
				case E_GrTaskMsgLogBkupStart:
					LcProcMsgLogBkupStart();
					break;
				case E_GrTaskMsgLogBkupEnd:
					LcProcMsgLogBkupEnd();
					break;
				case E_GrTaskMsgLogBkupError:
					LcProcMsgLogBkupError();
					break;

				default:
					Tv_Result	=	FALSE;
					break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::MousePosInput( __s32 A_X, __s32 A_Y, BOOL8 A_IsAbs )
{
	// local -------------------
		__s32		Tv_X;
		__s32		Tv_Y;
		St_GrP4MsgMouse	Tv_Msg;
	// code --------------------
		//check able
		if ( NULL != m_ConTbl[m_FcsConIdx] )
		{
			Tv_X	=	A_X;
			Tv_Y	=	A_Y;
			if ( !A_IsAbs )
			{
				if ( 0 != m_MseSenSl )
				{
					Tv_X	=	Tv_X << m_MseSenSl;
					Tv_Y	=	Tv_Y << m_MseSenSl;
				}
				else if ( 0 != m_MseSenSr )
				{
					Tv_X	=	Tv_X >> m_MseSenSr;
					Tv_Y	=	Tv_Y >> m_MseSenSr;
				}
				Tv_X	=	Tv_X + m_MseX;
				Tv_Y	=	Tv_Y + m_MseY;
			}
			//patch position x
			if ( Tv_X < 0 )
			{
				Tv_X	=	0;
			}
			else if ( Tv_X >= (__s32)m_ConTbl[m_FcsConIdx]->GetResX() )
			{
				Tv_X	=	(__s32)m_ConTbl[m_FcsConIdx]->GetResX() - 1;
			}
			//patch position y
			if ( Tv_Y < 0 )
			{
				Tv_Y		=	0;
			}
			else if ( Tv_Y >= (__s32)m_ConTbl[m_FcsConIdx]->GetResY()  )
			{
				Tv_Y	=	(__s32)m_ConTbl[m_FcsConIdx]->GetResY() - 1;
			}
			//check change position
			if ( (m_MseX != (__u16)Tv_X) || (m_MseY != (__u16)Tv_Y ) )
			{
				//update cursor position
				m_MseX		=	(__u16)Tv_X;
				m_MseY		=	(__u16)Tv_Y;
				//make message
				Tv_Msg.Hd.Cmd			=	E_GrP4MsgCmdConInpMouse;
				Tv_Msg.Hd.ConIdx	=	m_FcsConIdx;
				Tv_Msg.Hd.WinId		=	E_GrP4WinIdNone;		//console message
				Tv_Msg.Hd.ObjId		=	E_GrP4ScptIdNone;
				Tv_Msg.BtnMap			=	m_MseBtnMap;
				Tv_Msg.X					=	m_MseX;
				Tv_Msg.Y					=	m_MseY;
				Tv_Msg.Atb				=	E_GrP4MouseAtbPosChg;

				UiMsgPost( (Ptr_GrP4MsgHd)&Tv_Msg, sizeof(Tv_Msg) );
			}
		}
		// auto lock time reset
		m_AtLockTimer	=	m_AtLockPeriod;
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::MouseBtnInput( __u8 A_BtnIdx, BOOL8 A_IsPush )
{
	// local -------------------
		St_GrP4MsgMouse		Tv_Msg;
	// code --------------------
		if ( NULL != m_ConTbl[m_FcsConIdx] )
		{
			//update button status
			if ( A_IsPush )
			{
				m_MseBtnMap	=	m_MseBtnMap | (1 << A_BtnIdx);
			}
			else
			{
				m_MseBtnMap	=	m_MseBtnMap & ~(1 << A_BtnIdx);
			}

			//make message
			Tv_Msg.Hd.Cmd			=	E_GrP4MsgCmdConInpMouse;
			Tv_Msg.Hd.ConIdx	=	m_FcsConIdx;
			Tv_Msg.Hd.WinId		=	E_GrP4WinIdNone;		//console message
			Tv_Msg.Hd.ObjId		=	E_GrP4ScptIdNone;
			Tv_Msg.BtnMap			=	m_MseBtnMap;
			Tv_Msg.X					=	m_MseX;
			Tv_Msg.Y					=	m_MseY;
			Tv_Msg.Atb				=	1 << A_BtnIdx;

			UiMsgPost( (Ptr_GrP4MsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
		// auto lock time reset
		m_AtLockTimer	=	m_AtLockPeriod;
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::MouseWheelInput( BOOL8 A_IsDown )
{
	// local -------------------
		St_GrP4MsgWheel		Tv_Msg;
	// code --------------------
		if ( NULL != m_ConTbl[m_FcsConIdx] )
		{
			Tv_Msg.Hd.Cmd			=	E_GrP4MsgCmdConInpWheel;
			Tv_Msg.Hd.ConIdx	=	m_FcsConIdx;
			Tv_Msg.Hd.WinId		=	E_GrP4WinIdNone;
			Tv_Msg.Hd.ObjId		=	E_GrP4ScptIdNone;
			Tv_Msg.IsDown			=	A_IsDown;
			UiMsgPost( (Ptr_GrP4MsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
		// auto lock time reset
		m_AtLockTimer	=	m_AtLockPeriod;
}
//--------------------------------------------------------------------
Ptr_GrP4ScptPstData	Cls_GrP4Mng::PstDataGet( __u16 A_Id, __u8* A_PtrRtCnt )
{
	// local -------------------
		Ptr_GrP4ScptPstData	Tv_Result;
		__u16	Tv_Idx;
	// code --------------------
		Tv_Result		=	NULL;
		*A_PtrRtCnt	=	0;

		if ( (E_GrP4ScptPstIdBase <= A_Id) && ( (E_GrP4ScptPstIdBase + m_PstCnt) > A_Id ) )
		{
			// check exist
			Tv_Idx	=	A_Id - E_GrP4ScptPstIdBase;
			if ( 0 != m_PtrPstTbl[Tv_Idx].Cnt )
			{
				*A_PtrRtCnt	=	m_PtrPstTbl[Tv_Idx].Cnt;
				Tv_Result	=	(Ptr_GrP4ScptPstData)m_PtrPstTbl[Tv_Idx].Pos;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP4Mng::TextPtrGet( __u16 A_Id, __u8 A_Lang )
{
	// local -------------------
		WCHAR*	Tv_Result;
		__u16		Tv_TxtIdx;
		__u8		Tv_LangIdx;
	// code --------------------
		Tv_Result	=	NULL;

		if ( (E_GrP4ScptTxtIdBase <= A_Id) && ((E_GrP4ScptTxtIdBase + E_GrP4MngTextScptMaxCnt) > A_Id) )
		{
			Tv_TxtIdx		=	A_Id - E_GrP4ScptTxtIdBase;
			Tv_LangIdx	=	m_LangIdx;
			if ( E_GrP4ScptLangMaxCnt > A_Lang )
			{
				Tv_LangIdx	=	A_Lang;
			}
			Tv_Result	=	m_PtrTxtTbl[Tv_TxtIdx].LngTbl[Tv_LangIdx];
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::MousePosGet( __u16* A_PtrRtX, __u16* A_PtrRtY )
{
		*A_PtrRtX	=	m_MseX;
		*A_PtrRtY	=	m_MseY;
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
__u32	Cls_GrP4Mng::DvlConResX( __u8 A_ConIdx )
{
		return	0;
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
__u32	Cls_GrP4Mng::DvlConResY( __u8 A_ConIdx )
{
		return	0;
}
#endif
//--------------------------------------------------------------------
__u32	Cls_GrP4Mng::PixelFormatGet( void )
{
	// local -------------------
		__u32	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		if ( NULL != m_PtrCfgPrj )
		{
			Tv_Result	=	m_PtrCfgPrj->FmtFcc;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4Mng::ConsoleRegist( Cls_GrP4ConBase* A_ObjCon, __u8 A_Idx )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		// check index
		if ( E_GrP4MngConMaxCnt > A_Idx )
		{
			m_ConTbl[A_Idx]	=	A_ObjCon;
			m_ConTbl[A_Idx]->IndexSet( A_Idx );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4Mng::ConsoleUnregist( Cls_GrP4ConBase* A_ObjCon )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_WkIdx;
	// code --------------------
		Tv_Result	=	FALSE;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP4MngConMaxCnt;Tv_WkIdx++ )
		{
			if ( A_ObjCon == m_ConTbl[Tv_WkIdx] )
			{
				// remove
				m_ConTbl[Tv_WkIdx]	=	NULL;
				Tv_Result	=	TRUE;
				break;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
Cls_GrP4DoBase*	Cls_GrP4Mng::LcUiObjCreate(__u8 A_Class, __u16 A_ScptId, Ptr_GrRect A_PtrRc, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt)
{
	// local -------------------
		Cls_GrP4DoBase*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;

		switch( A_Class )
		{
			case E_GrP4DoClassFrame:
				Tv_Result	=	(Cls_GrP4DoBase*)new Cls_GrP4DoFrame( A_ScptId, A_PtrRc, A_PosRel, A_ObjPrnt );
				break;
			case E_GrP4DoClassLabel:
				Tv_Result	=	(Cls_GrP4DoBase*)new Cls_GrP4DoLbl(A_ScptId, A_PtrRc, A_PosRel, A_ObjPrnt);
				break;
			case E_GrP4DoClassPrgsBarLoc:
				Tv_Result	=	(Cls_GrP4DoBase*)new Cls_GrP4DoPrgsBarLoc(A_ScptId, A_PtrRc, A_PosRel, A_ObjPrnt);
				break;
			case E_GrP4DoClassQuad:
				Tv_Result	=	(Cls_GrP4DoBase*)new Cls_GrP4DoQuad(A_ScptId, A_PtrRc, A_PosRel, A_ObjPrnt);
				break;
			case E_GrP4DoClassBtnPage:
				Tv_Result	=	(Cls_GrP4DoBase*)new Cls_GrP4DoBtnPage(A_ScptId, A_PtrRc, A_PosRel, A_ObjPrnt);
				break;
			case E_GrP4DoClassStatic:
				Tv_Result	=	(Cls_GrP4DoBase*)new Cls_GrP4DoStatic(A_ScptId, A_PtrRc, A_PosRel, A_ObjPrnt);
				break;
			case E_GrP4DoClassEdtStpStr:
				Tv_Result	=	(Cls_GrP4DoBase*)new Cls_GrP4DoEdtStpStr(A_ScptId, A_PtrRc, A_PosRel, A_ObjPrnt);
				break;
			case E_GrP4DoClassBtnLbl:
				Tv_Result	=	(Cls_GrP4DoBase*)new Cls_GrP4DoBtnLbl(A_ScptId, A_PtrRc, A_PosRel, A_ObjPrnt);
				break;
			case E_GrP4DoClassSpinNumStp:
				Tv_Result	=	(Cls_GrP4DoBase*)new Cls_GrP4DoSpinNumStp(A_ScptId, A_PtrRc, A_PosRel, A_ObjPrnt);
				break;
			case E_GrP4DoClassSpinTxtStp:
				Tv_Result	=	(Cls_GrP4DoBase*)new Cls_GrP4DoSpinTxtStp(A_ScptId, A_PtrRc, A_PosRel, A_ObjPrnt);
				break;
			case E_GrP4DoClassRelStc:
				Tv_Result	=	(Cls_GrP4DoBase*)new Cls_GrP4DoRelStc(A_ScptId, A_PtrRc, A_PosRel, A_ObjPrnt);
				break;
			case E_GrP4DoClassSpinPageTxt:
				Tv_Result	=	(Cls_GrP4DoBase*)new Cls_GrP4DoSpinPageTxt(A_ScptId, A_PtrRc, A_PosRel, A_ObjPrnt);
				break;
			case E_GrP4DoClassSpinDdgtStp:
				Tv_Result	=	(Cls_GrP4DoBase*)new Cls_GrP4DoSpinDdgtStp(A_ScptId, A_PtrRc, A_PosRel, A_ObjPrnt);
				break;
			case E_GrP4DoClassBtnStc:
				Tv_Result	=	(Cls_GrP4DoBase*)new Cls_GrP4DoBtnStc(A_ScptId, A_PtrRc, A_PosRel, A_ObjPrnt);
				break;
			case E_GrP4DoClassChkbSsb:
				Tv_Result	=	(Cls_GrP4DoBase*)new Cls_GrP4DoChkbSsb(A_ScptId, A_PtrRc, A_PosRel, A_ObjPrnt);
				break;
			case E_GrP4DoClassChkbSsm:
				Tv_Result	=	(Cls_GrP4DoBase*)new Cls_GrP4DoChkbSsm(A_ScptId, A_PtrRc, A_PosRel, A_ObjPrnt);
				break;
			case E_GrP4DoClassSbarVar:
				Tv_Result	=	(Cls_GrP4DoBase*)new Cls_GrP4DoSbarVar(A_ScptId, A_PtrRc, A_PosRel, A_ObjPrnt);
				break;
			case E_GrP4DoClassSbarStp:
				Tv_Result	=	(Cls_GrP4DoBase*)new Cls_GrP4DoSbarStp(A_ScptId, A_PtrRc, A_PosRel, A_ObjPrnt);
				break;
			case E_GrP4DoClassRbtnStp:
				Tv_Result	=	(Cls_GrP4DoBase*)new Cls_GrP4DoRbtnStp(A_ScptId, A_PtrRc, A_PosRel, A_ObjPrnt);
				break;
			case E_GrP4DoClassChkbSgb:
				Tv_Result	=	(Cls_GrP4DoBase*)new Cls_GrP4DoChkbSgb(A_ScptId, A_PtrRc, A_PosRel, A_ObjPrnt);
				break;
			case E_GrP4DoClassChkbSgm:
				Tv_Result	=	(Cls_GrP4DoBase*)new Cls_GrP4DoChkbSgm(A_ScptId, A_PtrRc, A_PosRel, A_ObjPrnt);
				break;
			case E_GrP4DoClassSpinPnt:
				Tv_Result	=	(Cls_GrP4DoBase*)new Cls_GrP4DoSpinPnt(A_ScptId, A_PtrRc, A_PosRel, A_ObjPrnt);
				break;
			case E_GrP4DoClassSpinNumLoc:
				Tv_Result	=	(Cls_GrP4DoBase*)new Cls_GrP4DoSpinNumLoc(A_ScptId, A_PtrRc, A_PosRel, A_ObjPrnt);
				break;
			case E_GrP4DoClassSpinDdgtLoc:
				Tv_Result	=	(Cls_GrP4DoBase*)new Cls_GrP4DoSpinDdgtLoc(A_ScptId, A_PtrRc, A_PosRel, A_ObjPrnt);
				break;
			case E_GrP4DoClassSpinTxtLoc:
				Tv_Result	=	(Cls_GrP4DoBase*)new Cls_GrP4DoSpinTxtLoc(A_ScptId, A_PtrRc, A_PosRel, A_ObjPrnt);
				break;
			case E_GrP4DoClassTimeCtrl:
				Tv_Result	=	(Cls_GrP4DoBase*)new Cls_GrP4DoTimeCtrl(A_ScptId, A_PtrRc, A_PosRel, A_ObjPrnt);
				break;
			case E_GrP4DoClassCalRec:
				Tv_Result	=	(Cls_GrP4DoBase*)new Cls_GrP4DoCalRec(A_ScptId, A_PtrRc, A_PosRel, A_ObjPrnt);
				break;
			case E_GrP4DoClassPrgsBarGvs:
				Tv_Result	=	(Cls_GrP4DoBase*)new Cls_GrP4DoPrgsBarGvs(A_ScptId, A_PtrRc, A_PosRel, A_ObjPrnt);
				break;
			case E_GrP4DoClassGridRscStp:
				Tv_Result	=	(Cls_GrP4DoBase*)new Cls_GrP4DoGridRscStp(A_ScptId, A_PtrRc, A_PosRel, A_ObjPrnt);
				break;
			case E_GrP4DoClassBtnRelStc:
				Tv_Result	=	(Cls_GrP4DoBase*)new Cls_GrP4DoBtnRelStc(A_ScptId, A_PtrRc, A_PosRel, A_ObjPrnt);
				break;
			case E_GrP4DoClassGridRstat:
				Tv_Result	=	(Cls_GrP4DoBase*)new Cls_GrP4DoGridRstat(A_ScptId, A_PtrRc, A_PosRel, A_ObjPrnt);
				break;
			case E_GrP4DoClassTimeLbl:
				Tv_Result	=	(Cls_GrP4DoBase*)new Cls_GrP4DoTimeLbl(A_ScptId, A_PtrRc, A_PosRel, A_ObjPrnt);
				break;
			case E_GrP4DoClassLbxCtlg:
				Tv_Result	=	(Cls_GrP4DoBase*)new Cls_GrP4DoLbxCtlg(A_ScptId, A_PtrRc, A_PosRel, A_ObjPrnt);
				break;
			case E_GrP4DoClassSpinQuadStp:
				Tv_Result	=	(Cls_GrP4DoBase*)new Cls_GrP4DoSpinQuadStp(A_ScptId, A_PtrRc, A_PosRel, A_ObjPrnt);
				break;
			case E_GrP4DoClassRbtnGvu:
				Tv_Result	=	(Cls_GrP4DoBase*)new Cls_GrP4DoRbtnGvu(A_ScptId, A_PtrRc, A_PosRel, A_ObjPrnt);
				break;
			case E_GrP4DoClassRbtnSsm:
				Tv_Result	=	(Cls_GrP4DoBase*)new Cls_GrP4DoRbtnSsm(A_ScptId, A_PtrRc, A_PosRel, A_ObjPrnt);
				break;
			case E_GrP4DoClassChkbLoc:
				Tv_Result	=	(Cls_GrP4DoBase*)new Cls_GrP4DoChkbLoc(A_ScptId, A_PtrRc, A_PosRel, A_ObjPrnt);
				break;
			case E_GrP4DoClassEdtLoc:
				Tv_Result	=	(Cls_GrP4DoBase*)new Cls_GrP4DoEdtLoc(A_ScptId, A_PtrRc, A_PosRel, A_ObjPrnt);
				break;
			case E_GrP4DoClassSpinId:
				Tv_Result	=	(Cls_GrP4DoBase*)new Cls_GrP4DoSpinId(A_ScptId, A_PtrRc, A_PosRel, A_ObjPrnt);
				break;
			case E_GrP4DoClassGridMtnStp:
				Tv_Result	=	(Cls_GrP4DoBase*)new Cls_GrP4DoGridMtnStp(A_ScptId, A_PtrRc, A_PosRel, A_ObjPrnt);
				break;
			case E_GrP4DoClassCalHday:
				Tv_Result	=	(Cls_GrP4DoBase*)new Cls_GrP4DoCalHday(A_ScptId, A_PtrRc, A_PosRel, A_ObjPrnt);
				break;
			case E_GrP4DoClassPnlPtz:
				Tv_Result	=	(Cls_GrP4DoBase*)new Cls_GrP4DoPnlPtz(A_ScptId, A_PtrRc, A_PosRel, A_ObjPrnt);
				break;
			case E_GrP4DoClassSpinBtxtStp:
				Tv_Result	=	(Cls_GrP4DoBase*)new Cls_GrP4DoSpinBtxtStp(A_ScptId, A_PtrRc, A_PosRel, A_ObjPrnt);
				break;
			case E_GrP4DoClassGridRrng:
				Tv_Result	=	(Cls_GrP4DoBase*)new Cls_GrP4DoGridRrng(A_ScptId, A_PtrRc, A_PosRel, A_ObjPrnt);
				break;
			case E_GrP4DoClassSpinValStp:
				Tv_Result	=	(Cls_GrP4DoBase*)new Cls_GrP4DoSpinValStp(A_ScptId, A_PtrRc, A_PosRel, A_ObjPrnt);
				break;
			case E_GrP4DoClassGridPlayBar:
				Tv_Result	=	(Cls_GrP4DoBase*)new Cls_GrP4DoGridPlayBar(A_ScptId, A_PtrRc, A_PosRel, A_ObjPrnt);
				break;
			case E_GrP4DoClassTreeLtxt:
				Tv_Result	=	(Cls_GrP4DoBase*)new Cls_GrP4DoTreeLtxt(A_ScptId, A_PtrRc, A_PosRel, A_ObjPrnt);
				break;
			case E_GrP4DoClassPlayBar:
				Tv_Result	=	(Cls_GrP4DoBase*)new Cls_GrP4DoPlayBar(A_ScptId, A_PtrRc, A_PosRel, A_ObjPrnt);
				break;
			default:
				DbgMsgPrint( "Cls_GrP4Mng::LcUiObjCreate - unknown class id ( %d )\n", A_Class );
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrP4Mng::LcUiDecSlave( Cls_GrP4DoBase* A_ObjPrnt, void* A_PtrScpt, BOOL8 A_IsWithPrnt )
{
	// local -------------------
		__u32	Tv_Result;
		__u32	Tv_Deced;			// local decode
		__u8*	Tv_PtrScpt;
		Ptr_GrP4ScptObjHd		Tv_PtrStx;
		Ptr_GrP4ScptObjEnd	Tv_PtrEdx;
		Cls_GrP4DoBase*	Tv_Obj;
		St_GrRect	Tv_RcObj;
#ifdef GR_P4_EDITOR
		BOOL8	Tv_IsParaUpdt;
		__u16	Tv_ScptId;
#endif
	// code --------------------
		Tv_Result	=	0;
		Tv_PtrStx	=	(Ptr_GrP4ScptObjHd)A_PtrScpt;
#ifdef GR_P4_EDITOR
		Tv_IsParaUpdt	=	TRUE;
#endif
		// check stx
		if (E_GrP4ScptObjStx == Tv_PtrStx->Stx)
		{
			if ( A_IsWithPrnt )
			{
				// parent parametter update mode
				Tv_Obj		=	A_ObjPrnt;
#ifdef GR_P4_EDITOR
				Tv_ScptId	=	A_ObjPrnt->ScptIdGet();
				if ( (E_GrP4ScptIdPageBase <= Tv_ScptId) && ((E_GrP4ScptIdPageBase + m_PageCnt) > Tv_ScptId) )
				{
					Tv_IsParaUpdt	=	FALSE;
				}
#endif
			}
			else
			{
				// create object
				Tv_Obj		=	NULL;
				Tv_RcObj.left		=	Tv_PtrStx->Rect.left;
				Tv_RcObj.top		=	Tv_PtrStx->Rect.top;
				Tv_RcObj.right	=	Tv_PtrStx->Rect.right;
				Tv_RcObj.bottom	=	Tv_PtrStx->Rect.bottom;

				Tv_Obj	=	LcUiObjCreate(Tv_PtrStx->ClassId, Tv_PtrStx->ScptId, &Tv_RcObj, Tv_PtrStx->PosRel, A_ObjPrnt);
			}
			if ( NULL != Tv_Obj )
			{
				// update para
#ifdef GR_P4_EDITOR
				if ( Tv_IsParaUpdt )
				{
					Tv_Obj->RtlScptParaUpdt(Tv_PtrStx);
				}
#else
				Tv_Obj->RtlScptParaUpdt(Tv_PtrStx);
#endif
				// go next
				Tv_Result		=	sizeof(St_GrP4ScptObjHd)+ (__u32)Tv_PtrStx->ParaSize;
				Tv_PtrScpt	=	(__u8*)((__u32)Tv_PtrStx + sizeof(St_GrP4ScptObjHd)+ (__u32)Tv_PtrStx->ParaSize);
				while ( E_GrP4ScptObjStx == (*Tv_PtrScpt) )
				{
					// slave decode
					Tv_Deced	=	LcUiDecSlave( Tv_Obj, Tv_PtrScpt, FALSE );
					if ( 0 == Tv_Deced )
					{
						// error quit
						break;
					}
					// go next
					Tv_Result		=	Tv_Result + Tv_Deced;
					Tv_PtrScpt	=	(__u8*)( (__u32)Tv_PtrScpt + Tv_Deced );
				}
				// check edx
				Tv_PtrEdx	=	(Ptr_GrP4ScptObjEnd)Tv_PtrScpt;
				if ( E_GrP4ScptObjEdx != Tv_PtrEdx->Edx )
				{
					DbgMsgPrint( "Cls_GrP4Mng::LcUiDecSlave - script ( %d ) edx error!  \n", A_ObjPrnt->ScptIdGet() );
				}
				// edx add
				Tv_Result	=	Tv_Result + sizeof(St_GrP4ScptObjEnd);
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP4Mng::LcUiDecSlave - script ( %d ) stx error!\n" , A_ObjPrnt->ScptIdGet() );
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4Mng::LcUiDecWindow(__u16 A_ScptId, __u8 A_ConIdx, __u16 A_RefPosWin)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_SlotIdx;
		Cls_GrP4DoWin*	Tv_Win;
		Ptr_GrP4ScptObjHd	Tv_PtrScpt;
		St_GrRect	Tv_RcObj;
		__u8	Tv_ConIdx;
		BOOL8	Tv_IsAble;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrP4ScptIdWinBase <= A_ScptId )
		{
			Tv_SlotIdx	=	A_ScptId - E_GrP4ScptIdWinBase;
			if ( m_WinCnt > Tv_SlotIdx )
			{
				// get script
				if ( NULL != m_PtrWinTbl[Tv_SlotIdx].Pos )
				{
					// check privilege
					Tv_IsAble	=	TRUE;
					if ( 0 != m_PtrWinTbl[Tv_SlotIdx].Privilege )
					{
						Tv_IsAble	=	FALSE;
						// check privilege
						if ( 0 != (m_Gvs.LoginPriv & m_PtrWinTbl[Tv_SlotIdx].Privilege) )
						{
							Tv_IsAble	=	TRUE;
						}
#ifdef GR_P4_EDITOR
						if ( !((Cls_GrP4DvlMng*)this)->DvlIsDbgRun() )
						{
							Tv_IsAble	=	TRUE;
						}
#endif
					}
					if ( Tv_IsAble )
					{
						// script decode
						Tv_PtrScpt	=	(Ptr_GrP4ScptObjHd)m_PtrWinTbl[Tv_SlotIdx].Pos;
						// check script head
						if ( E_GrP4ScptObjStx == Tv_PtrScpt->Stx )
						{
							// check first window class type
							if ( E_GrP4DoClassWin == Tv_PtrScpt->ClassId )
							{
								// check script id
								if ( A_ScptId == Tv_PtrScpt->ScptId )
								{
									// get console
									Tv_ConIdx	=	A_ConIdx;
									if ( E_GrP4ConIdxFocus ==  Tv_ConIdx )
									{
										Tv_ConIdx	=	m_FcsConIdx;
									}
									// check console index
									if ( E_GrP4MngConMaxCnt > Tv_ConIdx )
									{
										// check exist console
										if ( NULL != m_ConTbl[Tv_ConIdx] )
										{
											// check already exist
											if ( NULL == m_ConTbl[Tv_ConIdx]->WinFindByScptId( A_ScptId ) )
											{
												// create window
												Tv_RcObj.left		=	Tv_PtrScpt->Rect.left;
												Tv_RcObj.top		=	Tv_PtrScpt->Rect.top;
												Tv_RcObj.right	=	Tv_PtrScpt->Rect.right;
												Tv_RcObj.bottom	=	Tv_PtrScpt->Rect.bottom;
												Tv_Win	=	(Cls_GrP4DoWin*)new Cls_GrP4DoWin(A_ScptId, &Tv_RcObj, Tv_PtrScpt->PosRel ,m_PtrWinTbl[Tv_SlotIdx].Layer,
													m_ConTbl[Tv_ConIdx], A_RefPosWin);
												// decode other option
												Tv_Win->RtlWinScptDec(Tv_PtrScpt);
												Tv_Result	=	TRUE;
												//Tv_Result	=	LcUiDecSlave( (Cls_GrP4DoBase*)Tv_Win, Tv_PtrScpt, TRUE );
												// call init
												//Tv_Win->RtlInit();
												// request focus change
												//m_ConTbl[Tv_ConIdx]->FocusReset();
												//m_ConTbl[Tv_ConIdx]->FocusCorrect();
											}
											else
											{
												DbgMsgPrint( "Cls_GrP4Mng::LcUiDecWindow - window ( %d ) already exist!\n", A_ScptId );
											}
										}
										else
										{
											DbgMsgPrint( "Cls_GrP4Mng::LcUiDecWindow - console ( %d ) not exist!\n", Tv_ConIdx );
										}
									}
									else
									{
										DbgMsgPrint( "Cls_GrP4Mng::LcUiDecWindow - bad console ( %d ) index!\n", Tv_ConIdx );
									}
								}
								else
								{
									DbgMsgPrint( "Cls_GrP4Mng::LcUiDecWindow - buffer script id  ( %d )  is mismatch orignal id ( %d ) !\n", Tv_PtrScpt->ScptId , A_ScptId  );
								}
							}
							else
							{
								DbgMsgPrint( "Cls_GrP4Mng::LcUiDecWindow - bad script ( %d )  - class type ( %d ) missmatch!\n" , A_ScptId, Tv_PtrScpt->ClassId );
							}
						}
						else
						{
							DbgMsgPrint( "Cls_GrP4Mng::LcUiDecWindow - script ( %d ) error! : bad stx.\n" , A_ScptId );
						}	//	if ( E_GrP4ScptObjStx == Tv_PtrScpt->Stx )

					}
					else
					{
						DbgMsgPrint( "Cls_GrP4Mng::LcUiDecWindow - script ( %d ) have not privilege ( 0x%04X -> 0x%04X )!.\n" , A_ScptId, m_PtrWinTbl[Tv_SlotIdx].Privilege, m_Gvs.LoginPriv );
						// event
						if ( E_GrP4AsmIdNone != m_PtrCfgMdl->AsmWinPriv )
						{
							VmReqEvtCall( m_PtrCfgMdl->AsmWinPriv, NULL, NULL );
						}
					}
				}
				else
				{
					DbgMsgPrint( "Cls_GrP4Mng::LcUiDecWindow - script ( %d ) not exist .\n" , A_ScptId );
				}
			}
			else
			{
				DbgMsgPrint( "Cls_GrP4Mng::LcUiDecWindow - Window Id ( %d ) is out of range.\n" , A_ScptId );
			}
		}
	
		return	Tv_Result;
}
//--------------------------------------------------------------------
Ptr_GrP4ScptAsmInfo	Cls_GrP4Mng::AsmPtrGet( __u16 A_Id )
{
	// local -------------------
		Ptr_GrP4ScptAsmInfo	Tv_Result;
		__u16	Tv_AsmIdx;
	// code --------------------
		Tv_Result	=	NULL;

		// check exist code
		if ( 0 != m_AsmCnt )
		{
			Tv_AsmIdx	=	A_Id - E_GrP4AsmIdBase;
			if ( m_AsmCnt > Tv_AsmIdx )
			{
				Tv_Result	=	&m_PtrAsmTbl[Tv_AsmIdx];
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__s32	Cls_GrP4Mng::LcAsmArgLoad(__u8 A_VmIdx, __u8 A_Type, __s32 A_Para)
{
	// local -------------------
		__s32	Tv_Result;
		void*	Tv_PtrArg;
		__u8	Tv_Size;
		__s32	Tv_Para;
	// code --------------------
		Tv_Result	=	0;

		Tv_PtrArg	=	NULL;
		Tv_Size		=	E_GrP4AsmArgTypeSzDword;
		// check constant
		switch ( A_Type )
		{
			case E_GrP4AsmArgTypeConst:
				Tv_Result	=	A_Para;
				break;
			case E_GrP4AsmArgTypeUrgst:
				Tv_PtrArg	=	(__s32*)( (__u32)m_RgstBuf + E_GrP4MngRgstUserOfs + (A_Para << 2) );
				if ( (0 > A_Para) || (E_GrP4MngRgstUserCnt <= A_Para) )
				{
					DbgMsgPrint( "Cls_GrP4Mng::LcAsmArgLoad - [ %d : %d ] bad user registry index!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip );
					Tv_PtrArg	=	NULL;
				}
				break;
			case E_GrP4AsmArgTypeSrgstByte:
				Tv_PtrArg	=	(__s32*)( (__u32)m_RgstBuf + (__u32)A_Para );
				Tv_Size		=	E_GrP4AsmArgTypeSzByte;
				if ( (0 > A_Para) || (E_GrP4RgstSysSize <= A_Para) )
				{
					DbgMsgPrint( "Cls_GrP4Mng::LcAsmArgLoad - [ %d : %d ] bad system registry address!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip );
					Tv_PtrArg	=	NULL;
				}
				break;
			case E_GrP4AsmArgTypeSrgstWord:
				Tv_PtrArg	=	(__s32*)( (__u32)m_RgstBuf + (__u32)A_Para );
				Tv_Size		=	E_GrP4AsmArgTypeSzWord;
				if ( (0 > A_Para) || (E_GrP4RgstSysSize <= (A_Para + 1)) )
				{
					DbgMsgPrint( "Cls_GrP4Mng::LcAsmArgLoad - [ %d : %d ] bad system registry address!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip );
					Tv_PtrArg	=	NULL;
				}
				break;
			case E_GrP4AsmArgTypeSrgstDword:
				Tv_PtrArg	=	(__s32*)( (__u32)m_RgstBuf + (__u32)A_Para );
				if ( (0 > A_Para) || (E_GrP4RgstSysSize <= (A_Para + 3)) )
				{
					DbgMsgPrint( "Cls_GrP4Mng::LcAsmArgLoad - [ %d : %d ] bad system registry address!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip );
					Tv_PtrArg	=	NULL;
				}
				break;
			case E_GrP4AsmArgTypeIreg:
				Tv_Para	=	A_Para & 0xFF;
				Tv_PtrArg	=	(void*)&m_VmTbl[A_VmIdx].RegIntTbl[Tv_Para];
				if (E_GrP4AsmMaxIntRegCnt <= Tv_Para)
				{
					DbgMsgPrint( "Cls_GrP4Mng::LcAsmArgLoad - [ %d : %d ] register index over!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip );
					Tv_PtrArg	=	NULL;
				}
				break;
			case E_GrP4AsmArgTypeGvsByte:
				Tv_Para	=	A_Para & 0xFFFF;
				Tv_PtrArg	=	(void*)((__u32)&m_Gvs + (__u32)Tv_Para);
				Tv_Size		=	E_GrP4AsmArgTypeSzByte;
				if (sizeof(m_Gvs) <= Tv_Para)
				{
					DbgMsgPrint( "Cls_GrP4Mng::LcAsmArgLoad - [ %d : %d ] bad global variable system offset!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip );
					Tv_PtrArg	=	NULL;
				}
				break;
			case E_GrP4AsmArgTypeGvsWord:
				Tv_Para	=	A_Para & 0xFFFF;
				Tv_PtrArg	=	(void*)((__u32)&m_Gvs + (__u32)Tv_Para);
				Tv_Size		=	E_GrP4AsmArgTypeSzWord;
				if (sizeof(m_Gvs) <= (Tv_Para + 1))
				{
					DbgMsgPrint( "Cls_GrP4Mng::LcAsmArgLoad - [ %d : %d ] bad global variable system offset!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip );
					Tv_PtrArg	=	NULL;
				}
				break;
			case E_GrP4AsmArgTypeGvsDword:
				Tv_Para	=	A_Para & 0xFFFF;
				Tv_PtrArg	=	(void*)((__u32)&m_Gvs + (__u32)Tv_Para);
				if (sizeof(m_Gvs) <= (Tv_Para + 3))
				{
					DbgMsgPrint( "Cls_GrP4Mng::LcAsmArgLoad - [ %d : %d ] bad global variable system offset!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip );
					Tv_PtrArg	=	NULL;
				}
				break;
			case E_GrP4AsmArgTypeGvu:
				Tv_Para	=	A_Para & 0xFFFF;
				Tv_PtrArg	=	(void*)(&m_GvuTbl[Tv_Para]);
				if (E_GrP4MngGvuMaxCnt <= Tv_Para)
				{
					DbgMsgPrint( "Cls_GrP4Mng::LcAsmArgLoad - [ %d : %d ] bad global variable user offset!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip );
					Tv_PtrArg	=	NULL;
				}
				break;
			case E_GrP4AsmArgTypeSetupByte:
				Tv_Para	=	A_Para & 0xFFFF;
				Tv_PtrArg	=	(void*)((__u32)m_SdaBuf + (__u32)Tv_Para);
				Tv_Size		=	E_GrP4AsmArgTypeSzByte;
				if (E_GrP4MngSdaSize <= Tv_Para)
				{
					DbgMsgPrint( "Cls_GrP4Mng::LcAsmArgLoad - [ %d : %d ] bad setup offset!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip );
					Tv_PtrArg	=	NULL;
				}
				break;
			case E_GrP4AsmArgTypeSetupWord:
				Tv_Para	=	A_Para & 0xFFFF;
				Tv_PtrArg	=	(void*)((__u32)m_SdaBuf + (__u32)Tv_Para);
				Tv_Size		=	E_GrP4AsmArgTypeSzWord;
				if (E_GrP4MngSdaSize <= (Tv_Para + 1))
				{
					DbgMsgPrint( "Cls_GrP4Mng::LcAsmArgLoad - [ %d : %d ] bad setup offset!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip );
					Tv_PtrArg	=	NULL;
				}
				break;
			case E_GrP4AsmArgTypeSetupDword:
				Tv_Para	=	A_Para & 0xFFFF;
				Tv_PtrArg	=	(void*)((__u32)m_SdaBuf + (__u32)Tv_Para);
				if (E_GrP4MngSdaSize <= (Tv_Para + 3))
				{
					DbgMsgPrint( "Cls_GrP4Mng::LcAsmArgLoad - [ %d : %d ] bad setup offset!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip );
					Tv_PtrArg	=	NULL;
				}
				break;

			default:
				DbgMsgPrint( "Cls_GrP4Mng::LcAsmArgLoad - [ %d : %d ] bad argument type!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip );
				break;
		}

		if ( NULL != Tv_PtrArg )
		{
			//calculate size
			switch ( Tv_Size )
			{
				case E_GrP4AsmArgTypeSzByte:
					Tv_Result	=	(__s32)( *((__u8*)Tv_PtrArg) );
					break;
				case E_GrP4AsmArgTypeSzWord:
					Tv_Result	=	(__s32)( *((__s16*)Tv_PtrArg) );
					break;
				default:
					Tv_Result	=	*((__s32*)Tv_PtrArg);
					break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__s32	Cls_GrP4Mng::SysValueGet(__u8 A_Type, __u32 A_Para)
{
	// local -------------------
		__s32	Tv_Result;
		void*	Tv_PtrArg;
		__u8	Tv_Size;
		__s32	Tv_Para;
	// code --------------------
		Tv_Result	=	0;

		Tv_PtrArg	=	NULL;
		Tv_Size		=	E_GrP4AsmArgTypeSzDword;
		// check constant
		switch (A_Type)
		{
			case E_GrP4AsmArgTypeConst:
				Tv_Result	=	A_Para;
				break;
			case E_GrP4AsmArgTypeUrgst:
				Tv_PtrArg	=	(__s32*)((__u32)m_RgstBuf + E_GrP4MngRgstUserOfs + (A_Para << 2));
				if ((0 > A_Para) || (E_GrP4MngRgstUserCnt <= A_Para))
				{
					DbgMsgPrint("Cls_GrP4Mng::SysValueGet - bad user registry index(%d)!\n", A_Para);
					Tv_PtrArg	=	NULL;
				}
				break;
			case E_GrP4AsmArgTypeSrgstByte:
				Tv_PtrArg	=	(__s32*)((__u32)m_RgstBuf + (__u32)A_Para);
				Tv_Size		=	E_GrP4AsmArgTypeSzByte;
				if ((0 > A_Para) || (E_GrP4RgstSysSize <= A_Para))
				{
					DbgMsgPrint("Cls_GrP4Mng::SysValueGet - bad system registry address(%d)!\n", A_Para);
					Tv_PtrArg	=	NULL;
				}
				break;
			case E_GrP4AsmArgTypeSrgstWord:
				Tv_PtrArg	=	(__s32*)((__u32)m_RgstBuf + (__u32)A_Para);
				Tv_Size		=	E_GrP4AsmArgTypeSzWord;
				if ((0 > A_Para) || (E_GrP4RgstSysSize <= (A_Para + 1)))
				{
					DbgMsgPrint("Cls_GrP4Mng::SysValueGet - bad system registry address(%d)!\n", A_Para);
					Tv_PtrArg	=	NULL;
				}
				break;
			case E_GrP4AsmArgTypeSrgstDword:
				Tv_PtrArg	=	(__s32*)((__u32)m_RgstBuf + (__u32)A_Para);
				if ((0 > A_Para) || (E_GrP4RgstSysSize <= (A_Para + 3)))
				{
					DbgMsgPrint("Cls_GrP4Mng::SysValueGet - bad system registry address(%d)!\n", A_Para);
					Tv_PtrArg	=	NULL;
				}
				break;
			case E_GrP4AsmArgTypeGvsByte:
				Tv_Para	=	A_Para & 0xFFFF;
				Tv_PtrArg	=	(void*)((__u32)&m_Gvs + (__u32)Tv_Para);
				Tv_Size		=	E_GrP4AsmArgTypeSzByte;
				if (sizeof(m_Gvs) <= Tv_Para)
				{
					DbgMsgPrint("Cls_GrP4Mng::SysValueGet - bad global variable system offset(%04X)!\n", Tv_Para);
					Tv_PtrArg	=	NULL;
				}
				break;
			case E_GrP4AsmArgTypeGvsWord:
				Tv_Para	=	A_Para & 0xFFFF;
				Tv_PtrArg	=	(void*)((__u32)&m_Gvs + (__u32)Tv_Para);
				Tv_Size		=	E_GrP4AsmArgTypeSzWord;
				if (sizeof(m_Gvs) <= (Tv_Para + 1))
				{
					DbgMsgPrint("Cls_GrP4Mng::SysValueGet - bad global variable system offset(%04X)!\n", Tv_Para);
					Tv_PtrArg	=	NULL;
				}
				break;
			case E_GrP4AsmArgTypeGvsDword:
				Tv_Para	=	A_Para & 0xFFFF;
				Tv_PtrArg	=	(void*)((__u32)&m_Gvs + (__u32)Tv_Para);
				if (sizeof(m_Gvs) <= (Tv_Para + 3))
				{
					DbgMsgPrint("Cls_GrP4Mng::SysValueGet - bad global variable system offset(%04X)!\n", Tv_Para);
					Tv_PtrArg	=	NULL;
				}
				break;
			case E_GrP4AsmArgTypeGvu:
				Tv_Para	=	A_Para & 0xFFFF;
				Tv_PtrArg	=	(void*)(&m_GvuTbl[Tv_Para]);
				if (E_GrP4MngGvuMaxCnt <= Tv_Para)
				{
					DbgMsgPrint("Cls_GrP4Mng::SysValueGet - bad global variable user index(%d)!\n", Tv_Para);
					Tv_PtrArg	=	NULL;
				}
				break;
			case E_GrP4AsmArgTypeSetupByte:
				Tv_Para	=	A_Para & 0xFFFF;
				Tv_PtrArg	=	(void*)((__u32)m_SdaBuf + (__u32)Tv_Para);
				Tv_Size		=	E_GrP4AsmArgTypeSzByte;
				if (E_GrP4MngSdaSize <= Tv_Para)
				{
					DbgMsgPrint("Cls_GrP4Mng::SysValueGet - bad setup offset(%04X]!\n", Tv_Para);
					Tv_PtrArg	=	NULL;
				}
				break;
			case E_GrP4AsmArgTypeSetupWord:
				Tv_Para	=	A_Para & 0xFFFF;
				Tv_PtrArg	=	(void*)((__u32)m_SdaBuf + (__u32)Tv_Para);
				Tv_Size		=	E_GrP4AsmArgTypeSzWord;
				if (E_GrP4MngSdaSize <= (Tv_Para + 1))
				{
					DbgMsgPrint("Cls_GrP4Mng::SysValueGet - bad setup offset(%04X]!\n", Tv_Para);
					Tv_PtrArg	=	NULL;
				}
				break;
			case E_GrP4AsmArgTypeSetupDword:
				Tv_Para	=	A_Para & 0xFFFF;
				Tv_PtrArg	=	(void*)((__u32)m_SdaBuf + (__u32)Tv_Para);
				if (E_GrP4MngSdaSize <= (Tv_Para + 3))
				{
					DbgMsgPrint("Cls_GrP4Mng::SysValueGet - bad setup offset(%04X]!\n", Tv_Para);
					Tv_PtrArg	=	NULL;
				}
				break;

			default:
				DbgMsgPrint("Cls_GrP4Mng::SysValueGet - bad argument type(%d)!\n", A_Type);
				break;
		}

		if (NULL != Tv_PtrArg)
		{
			//calculate size
			switch (Tv_Size)
			{
			case E_GrP4AsmArgTypeSzByte:
				Tv_Result	=	(__s32)(*((__u8*)Tv_PtrArg));
				break;
			case E_GrP4AsmArgTypeSzWord:
				Tv_Result	=	(__s32)(*((__s16*)Tv_PtrArg));
				break;
			default:
				Tv_Result	=	*((__s32*)Tv_PtrArg);
				break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcAsmArgStore(__u8 A_VmIdx, __u8 A_Type, __s32 A_Para, __s32 A_Val)
{
	// local -------------------
		void*	Tv_PtrArg;
		__u8	Tv_Size;
		__s32	Tv_Para;
	// code --------------------
		Tv_PtrArg	=	NULL;
		Tv_Size		=	E_GrP4AsmArgTypeSzDword;

		switch ( A_Type )
		{
			case E_GrP4AsmArgTypeConst:
				DbgMsgPrint( "Cls_GrP4Mng::LcAsmArgStore - [ %d : %d ] store argumen can not be constant!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip );
				break;
			case E_GrP4AsmArgTypeUrgst:
				Tv_PtrArg	=	(__s32*)( (__u32)m_RgstBuf + E_GrP4MngRgstUserOfs + (A_Para << 2) );
				if ( (0 > A_Para) || (E_GrP4MngRgstUserCnt <= A_Para) )
				{
					DbgMsgPrint( "Cls_GrP4Mng::LcAsmArgStore - [ %d : %d ] bad user registry index!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip );
					Tv_PtrArg	=	NULL;
				}
				break;
			case E_GrP4AsmArgTypeSrgstByte:
			case E_GrP4AsmArgTypeSrgstWord:
			case E_GrP4AsmArgTypeSrgstDword:
				DbgMsgPrint( "Cls_GrP4Mng::LcAsmArgStore - [ %d : %d ] Designer can not modify system registry!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip );
				break;
			case E_GrP4AsmArgTypeIreg:
				Tv_Para	=	A_Para & 0xFF;
				Tv_PtrArg	=	(void*)&m_VmTbl[A_VmIdx].RegIntTbl[Tv_Para];
				if (E_GrP4AsmMaxIntRegCnt <= Tv_Para)
				{
					DbgMsgPrint( "Cls_GrP4Mng::LcAsmArgStore - [ %d : %d ] register index over!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip );
					Tv_PtrArg	=	NULL;
				}
				break;
			case E_GrP4AsmArgTypeGvsByte:
			case E_GrP4AsmArgTypeGvsWord:
			case E_GrP4AsmArgTypeGvsDword:
				DbgMsgPrint( "Cls_GrP4Mng::LcAsmArgStore - [ %d : %d ] Designer can not modify GVS!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip );
				break;
			case E_GrP4AsmArgTypeGvu:
				Tv_Para	=	A_Para & 0xFFFF;
				Tv_PtrArg	=	(void*)(&m_GvuTbl[Tv_Para]);
				if (E_GrP4MngGvuMaxCnt <= Tv_Para)
				{
					DbgMsgPrint( "Cls_GrP4Mng::LcAsmArgStore - [ %d : %d ] bad global variable user offset!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip );
					Tv_PtrArg	=	NULL;
				}
				break;
			case E_GrP4AsmArgTypeSetupByte:
				Tv_Para	=	A_Para & 0xFFFF;
				Tv_PtrArg	=	(void*)((__u32)m_SdaBuf + (__u32)Tv_Para);
				Tv_Size		=	E_GrP4AsmArgTypeSzByte;
				if (E_GrP4MngSdaSize <= Tv_Para)
				{
					DbgMsgPrint( "Cls_GrP4Mng::LcAsmArgStore - [ %d : %d ] bad setup offset!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip );
					Tv_PtrArg	=	NULL;
				}
				break;
			case E_GrP4AsmArgTypeSetupWord:
				Tv_Para	=	A_Para & 0xFFFF;
				Tv_PtrArg	=	(void*)((__u32)m_SdaBuf + (__u32)Tv_Para);
				Tv_Size		=	E_GrP4AsmArgTypeSzWord;
				if (E_GrP4MngSdaSize <= (Tv_Para + 1))
				{
					DbgMsgPrint( "Cls_GrP4Mng::LcAsmArgStore - [ %d : %d ] bad setup offset!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip );
					Tv_PtrArg	=	NULL;
				}
				break;
			case E_GrP4AsmArgTypeSetupDword:
				Tv_Para	=	A_Para & 0xFFFF;
				Tv_PtrArg	=	(void*)((__u32)m_SdaBuf + (__u32)Tv_Para);
				if (E_GrP4MngSdaSize <= (Tv_Para + 3))
				{
					DbgMsgPrint( "Cls_GrP4Mng::LcAsmArgStore - [ %d : %d ] bad setup offset!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip );
					Tv_PtrArg	=	NULL;
				}
				break;

			default:
				DbgMsgPrint( "Cls_GrP4Mng::LcAsmArgLoad - [ %d : %d ] bad argument type!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip );
				break;
		}

		//calculate size
		if ( NULL != Tv_PtrArg )
		{
			switch ( Tv_Size )
			{
				case E_GrP4AsmArgTypeSzByte:
					*((__s8*)Tv_PtrArg)		=	(__s8)A_Val;
					break;
				case E_GrP4AsmArgTypeSzWord:
					*((__s16*)Tv_PtrArg)		=	(__s16)A_Val;
					break;
				default:
					*((__s32*)Tv_PtrArg)	=	A_Val;
					break;
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::SysValueSet(__u8 A_Type, __u32 A_Para, __s32 A_Val)
{
	// local -------------------
		void*	Tv_PtrArg;
		__u8	Tv_Size;
		__s32	Tv_Para;
	// code --------------------
		Tv_PtrArg	=	NULL;
		Tv_Size		=	E_GrP4AsmArgTypeSzDword;

		switch (A_Type)
		{
			case E_GrP4AsmArgTypeConst:
				DbgMsgPrint("Cls_GrP4Mng::SysValueSet - store argumen can not be constant!\n");
				break;
			case E_GrP4AsmArgTypeUrgst:
				Tv_PtrArg	=	(__s32*)((__u32)m_RgstBuf + E_GrP4MngRgstUserOfs + (A_Para << 2));
				if ((0 > A_Para) || (E_GrP4MngRgstUserCnt <= A_Para))
				{
					DbgMsgPrint("Cls_GrP4Mng::SysValueSet - bad user registry index(%d)!\n", A_Para);
					Tv_PtrArg	=	NULL;
				}
				break;
			case E_GrP4AsmArgTypeSrgstByte:
			case E_GrP4AsmArgTypeSrgstWord:
			case E_GrP4AsmArgTypeSrgstDword:
				DbgMsgPrint("Cls_GrP4Mng::SysValueSet - Designer can not modify system registry!\n");
				break;
			case E_GrP4AsmArgTypeGvsByte:
			case E_GrP4AsmArgTypeGvsWord:
			case E_GrP4AsmArgTypeGvsDword:
				DbgMsgPrint("Cls_GrP4Mng::SysValueSet - Designer can not modify GVS!\n");
				break;
			case E_GrP4AsmArgTypeGvu:
				Tv_Para	=	A_Para & 0xFFFF;
				Tv_PtrArg	=	(void*)(&m_GvuTbl[Tv_Para]);
				if (E_GrP4MngGvuMaxCnt <= Tv_Para)
				{
					DbgMsgPrint("Cls_GrP4Mng::SysValueSet - bad global variable user offset(%d)!\n", A_Para);
					Tv_PtrArg	=	NULL;
				}
				break;
			case E_GrP4AsmArgTypeSetupByte:
				Tv_Para	=	A_Para & 0xFFFF;
				Tv_PtrArg	=	(void*)((__u32)m_SdaBuf + (__u32)Tv_Para);
				Tv_Size		=	E_GrP4AsmArgTypeSzByte;
				if (E_GrP4MngSdaSize <= Tv_Para)
				{
					DbgMsgPrint("Cls_GrP4Mng::SysValueSet - bad setup offset(%d)!\n", A_Para);
					Tv_PtrArg	=	NULL;
				}
				break;
			case E_GrP4AsmArgTypeSetupWord:
				Tv_Para	=	A_Para & 0xFFFF;
				Tv_PtrArg	=	(void*)((__u32)m_SdaBuf + (__u32)Tv_Para);
				Tv_Size		=	E_GrP4AsmArgTypeSzWord;
				if (E_GrP4MngSdaSize <= (Tv_Para + 1))
				{
					DbgMsgPrint("Cls_GrP4Mng::SysValueSet - bad setup offset(%d)!\n", A_Para);
					Tv_PtrArg	=	NULL;
				}
				break;
			case E_GrP4AsmArgTypeSetupDword:
				Tv_Para	=	A_Para & 0xFFFF;
				Tv_PtrArg	=	(void*)((__u32)m_SdaBuf + (__u32)Tv_Para);
				if (E_GrP4MngSdaSize <= (Tv_Para + 3))
				{
					DbgMsgPrint("Cls_GrP4Mng::SysValueSet - bad setup offset(%d)!\n", A_Para);
					Tv_PtrArg	=	NULL;
				}
				break;

			default:
				DbgMsgPrint("Cls_GrP4Mng::SysValueSet - bad argument type(%d)!\n", A_Type);
				break;
		}

		//calculate size
		if (NULL != Tv_PtrArg)
		{
			switch (Tv_Size)
			{
				case E_GrP4AsmArgTypeSzByte:
					*((__s8*)Tv_PtrArg)		=	(__s8)A_Val;
					break;
				case E_GrP4AsmArgTypeSzWord:
					*((__s16*)Tv_PtrArg)		=	(__s16)A_Val;
					break;
				default:
					*((__s32*)Tv_PtrArg)	=	A_Val;
					break;
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcAsmOpCond(__u8 A_VmIdx, Ptr_GrP4AsmOpCond A_PtrCode)
{
	// local -------------------
		__s32		Tv_ArgLeft;
		__s32		Tv_ArgRight;
		__u8	Tv_Flag;
	// code --------------------
		Tv_ArgLeft	=	LcAsmArgLoad(A_VmIdx, A_PtrCode->ArgTypeLeft, A_PtrCode->ArgLeftVal);
		Tv_ArgRight	=	LcAsmArgLoad(A_VmIdx, A_PtrCode->ArgTypeRight, A_PtrCode->ArgRightVal);

		Tv_Flag	=	0;
		switch ( A_PtrCode->CondCode )
		{
			case E_GrP4AsmCondTypeEq:
				if ( Tv_ArgLeft == Tv_ArgRight )
				{
					Tv_Flag		=	1;
				}
				break;
			case E_GrP4AsmCondTypeNe:
				if ( Tv_ArgLeft != Tv_ArgRight )
				{
					Tv_Flag		=	1;
				}
				break;
			case E_GrP4AsmCondTypeGt:
				if ( Tv_ArgLeft > Tv_ArgRight )
				{
					Tv_Flag		=	1;
				}
				break;
			case E_GrP4AsmCondTypeLt:
				if ( Tv_ArgLeft < Tv_ArgRight )
				{
					Tv_Flag		=	1;
				}
				break;
			case E_GrP4AsmCondTypeGe:
				if ( Tv_ArgLeft >= Tv_ArgRight )
				{
					Tv_Flag		=	1;
				}
				break;
			case E_GrP4AsmCondTypeLe:
				if ( Tv_ArgLeft <= Tv_ArgRight )
				{
					Tv_Flag		=	1;
				}
				break;
			case E_GrP4AsmCondTypeAnd:
				if ( (0 != Tv_ArgLeft) && (0 != Tv_ArgRight) )
				{
					Tv_Flag		=	1;
				}
				break;
			case E_GrP4AsmCondTypeOr:
				if ( (0 != Tv_ArgLeft) || (0 != Tv_ArgRight) )
				{
					Tv_Flag		=	1;
				}
				break;
			default:
				DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpCond - [ %d : %d ] unknown condition code!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip );
				break;
		}
#ifdef GR_P4_EDITOR
		if ( E_GrP4AsmFlagCnt <= A_PtrCode->FlagIdxRslt )
		{
			DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpCond - [ %d : %d ] bad flag index!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip );
		}
#endif
		Tv_Flag	=	Tv_Flag << A_PtrCode->FlagIdxRslt;
		switch ( A_PtrCode->FlagUpdtType )
		{
			case E_GrP4AsmUpdtFgDirect:
				m_VmTbl[A_VmIdx].Flag	=	m_VmTbl[A_VmIdx].Flag & ( ~(1 << A_PtrCode->FlagIdxRslt) );
				m_VmTbl[A_VmIdx].Flag	=	m_VmTbl[A_VmIdx].Flag | Tv_Flag;
				break;
			case E_GrP4AsmUpdtFgOr:
				m_VmTbl[A_VmIdx].Flag	=	m_VmTbl[A_VmIdx].Flag | Tv_Flag;
				break;
			case E_GrP4AsmUpdtFgAnd:
				m_VmTbl[A_VmIdx].Flag	=	m_VmTbl[A_VmIdx].Flag & (~Tv_Flag);
				break;
			default:
				DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpCond - [ %d : %d ] bad flag update type!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip );
				break;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcAsmOpMove(__u8 A_VmIdx, Ptr_GrP4AsmOpMove A_PtrCode)
{
	// local -------------------
		__s32		Tv_ArgSr;
	// code --------------------
		Tv_ArgSr	=	LcAsmArgLoad(A_VmIdx, A_PtrCode->SrArgType, A_PtrCode->SrArgVal);
		LcAsmArgStore(A_VmIdx, A_PtrCode->TgArgType, A_PtrCode->TgArgVal, Tv_ArgSr);
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcAsmOpFlag(__u8 A_VmIdx, Ptr_GrP4AsmOpFlag A_PtrCode)
{
	// local -------------------
		__s32		Tv_FlagIdx;
	// code --------------------
		Tv_FlagIdx	=	LcAsmArgLoad(A_VmIdx, A_PtrCode->ArgType, A_PtrCode->ArgVal);
#ifdef GR_P4_EDITOR
		if ( (0 > Tv_FlagIdx) || (E_GrP4AsmFlagCnt <= Tv_FlagIdx)  )
		{
			DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpFlag - [ %d : %d ] bad flag index!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip );
		}
#endif
		switch ( A_PtrCode->SetType )
		{
			case E_GrP4AsmSetFgClear:
				m_VmTbl[A_VmIdx].Flag	=	m_VmTbl[A_VmIdx].Flag & (~(1 << (__u8)Tv_FlagIdx));
				break;
			case E_GrP4AsmSetFgSet:
				m_VmTbl[A_VmIdx].Flag	=	m_VmTbl[A_VmIdx].Flag | (1 << (__u8)Tv_FlagIdx);
				break;
			case E_GrP4AsmSetFgReverse:
				m_VmTbl[A_VmIdx].Flag	=	m_VmTbl[A_VmIdx].Flag ^ (1 << (__u8)Tv_FlagIdx);
				break;
			default:
				DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpFlag - [ %d : %d ] bad flag setting type!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip );
				break;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcAsmOpCalc(__u8 A_VmIdx, Ptr_GrP4AsmOpCalc A_PtrCode)
{
	// local -------------------
		__s32		Tv_ArgSr;
		__s32		Tv_ArgTg;
	// code --------------------
		Tv_ArgSr	=	LcAsmArgLoad(A_VmIdx, A_PtrCode->SrArgType, A_PtrCode->SrArgVal);
		Tv_ArgTg	=	LcAsmArgLoad(A_VmIdx, A_PtrCode->TgArgType, A_PtrCode->TgArgVal);

		switch ( A_PtrCode->CalcCode )
		{
			case E_GrP4AsmCalcAdd:
				Tv_ArgTg	=	Tv_ArgTg + Tv_ArgSr;
				break;
			case E_GrP4AsmCalcSub:
				Tv_ArgTg	=	Tv_ArgTg - Tv_ArgSr;
				break;
			case E_GrP4AsmCalcMul:
				Tv_ArgTg	=	Tv_ArgTg * Tv_ArgSr;
				break;
			case E_GrP4AsmCalcDiv:
				if ( 0 != Tv_ArgSr )
				{
					Tv_ArgTg	=	Tv_ArgTg / Tv_ArgSr;
				}
				else
				{
					Tv_ArgTg	=	0;
					DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpCalc - divide by zero!\n" );
				}
				break;
			case E_GrP4AsmCalcRem:
				Tv_ArgTg	=	Tv_ArgTg % Tv_ArgSr;
				break;
			case E_GrP4AsmCalcAnd:
				Tv_ArgTg	=	Tv_ArgTg & Tv_ArgSr;
				break;
			case E_GrP4AsmCalcOr:
				Tv_ArgTg	=	Tv_ArgTg | Tv_ArgSr;
				break;
			case E_GrP4AsmCalcXor:
				Tv_ArgTg	=	Tv_ArgTg ^ Tv_ArgSr;
				break;
			case E_GrP4AsmCalcShl:
				Tv_ArgTg	=	Tv_ArgTg << Tv_ArgSr;
				break;
			case E_GrP4AsmCalcShr:
				Tv_ArgTg	=	Tv_ArgTg >> Tv_ArgSr;
				break;
			default:
				DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpCalc - [ %d : %d ] bad flag setting type!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip );
				break;
		}
		LcAsmArgStore(A_VmIdx, A_PtrCode->TgArgType, A_PtrCode->TgArgVal, Tv_ArgTg);
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcAsmOpNot(__u8 A_VmIdx, Ptr_GrP4AsmOpNot A_PtrCode)
{
	// local -------------------
		__s32		Tv_ArgSr;
	// code --------------------
		Tv_ArgSr	=	LcAsmArgLoad(A_VmIdx, A_PtrCode->SrArgType, A_PtrCode->SrArgVal);
		if ( A_PtrCode->IsLogical )
		{
			if ( 0 == Tv_ArgSr )
			{
				Tv_ArgSr	=	1;
			}
			else
			{
				Tv_ArgSr	=	0;
			}
		}
		else
		{
			Tv_ArgSr	=	~Tv_ArgSr;
		}
		LcAsmArgStore(A_VmIdx, A_PtrCode->TgArgType, A_PtrCode->TgArgVal, Tv_ArgSr);
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcAsmOpDirJump(__u8 A_VmIdx, Ptr_GrP4AsmOpDirJump A_PtrCode)
{
	// local -------------------
		Ptr_GrP4ScptAsmInfo	Tv_CodeInfo;
	// code --------------------

		Tv_CodeInfo	=	AsmPtrGet( A_PtrCode->FuncId );
		if ( NULL != Tv_CodeInfo )
		{
			if ( A_PtrCode->IsCall )
			{
				// check stack full
				if ( E_GrP4AsmCallMaxCnt > m_VmTbl[A_VmIdx].Sp )
				{
					// insert stack
					m_VmTbl[A_VmIdx].CallStack[m_VmTbl[A_VmIdx].Sp].Cs	=	m_VmTbl[A_VmIdx].Cs;
					m_VmTbl[A_VmIdx].CallStack[m_VmTbl[A_VmIdx].Sp].Ip	=	m_VmTbl[A_VmIdx].Ip + 1;
					m_VmTbl[A_VmIdx].Sp ++;

					// change pointer
					m_PtrVmCs				=	(Ptr_GrP4AsmOpBase)Tv_CodeInfo->Pos;
					m_VmCsLim				=	Tv_CodeInfo->Cnt;
					m_VmTbl[A_VmIdx].Cs			=	A_PtrCode->FuncId;
					m_VmTbl[A_VmIdx].Ip			=	0;
				}
				else
				{
					DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpDirJump - [ %d : %d ] stack overflow.\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip );
					m_VmTbl[A_VmIdx].Rstat	=	E_GrP4AsmRunStatStop;
				}
			}
			else
			{
				m_PtrVmCs		=	(Ptr_GrP4AsmOpBase)Tv_CodeInfo->Pos;
				m_VmCsLim		=	Tv_CodeInfo->Cnt;
				m_VmTbl[A_VmIdx].Cs			=	A_PtrCode->FuncId;
				m_VmTbl[A_VmIdx].Ip			=	0;
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpDirJump - [ %d : %d ] function %d not exist.\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip, A_PtrCode->FuncId );
			m_VmTbl[A_VmIdx].Rstat	=	E_GrP4AsmRunStatStop;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcAsmOpRefJump(__u8 A_VmIdx, Ptr_GrP4AsmOpRefJump A_PtrCode)
{
	// local -------------------
		Ptr_GrP4ScptAsmInfo	Tv_CodeInfo;
		__u16	Tv_FuncId;
	// code --------------------
		Tv_FuncId	=	(__u16)LcAsmArgLoad(A_VmIdx, A_PtrCode->ArgType, A_PtrCode->ArgVal);

		if ( 0x10000 > Tv_FuncId )
		{
			Tv_CodeInfo	=	AsmPtrGet( (__u16)Tv_FuncId );
			if ( NULL != Tv_CodeInfo )
			{
				if ( A_PtrCode->IsCall )
				{
					if ( E_GrP4AsmCallMaxCnt > m_VmTbl[A_VmIdx].Sp )
					{
						// insert stack
						m_VmTbl[A_VmIdx].CallStack[m_VmTbl[A_VmIdx].Sp].Cs	=	m_VmTbl[A_VmIdx].Cs;
						m_VmTbl[A_VmIdx].CallStack[m_VmTbl[A_VmIdx].Sp].Ip	=	m_VmTbl[A_VmIdx].Ip + 1;
						m_VmTbl[A_VmIdx].Sp ++;

						// change pointer
						m_PtrVmCs	=	(Ptr_GrP4AsmOpBase)Tv_CodeInfo->Pos;
						m_VmCsLim	=	Tv_CodeInfo->Cnt;
						m_VmTbl[A_VmIdx].Cs			=	Tv_FuncId;
						m_VmTbl[A_VmIdx].Ip			=	0;
					}
					else
					{
						DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpRefJump - [ %d : %d ] stack overflow.\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip );
						m_VmTbl[A_VmIdx].Rstat	=	E_GrP4AsmRunStatStop;
					}
				}
				else
				{
					m_PtrVmCs	=	(Ptr_GrP4AsmOpBase)Tv_CodeInfo->Pos;
					m_VmCsLim	=	Tv_CodeInfo->Cnt;
					m_VmTbl[A_VmIdx].Cs			=	Tv_FuncId;
					m_VmTbl[A_VmIdx].Ip			=	0;
				}
			}
			else
			{
				DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpRefJump - [ %d : %d ] function %d not exist.\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip, Tv_FuncId );
				m_VmTbl[A_VmIdx].Rstat	=	E_GrP4AsmRunStatStop;
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpRefJump - [ %d : %d ] function id %d is bad.\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip, Tv_FuncId );
			m_VmTbl[A_VmIdx].Rstat	=	E_GrP4AsmRunStatStop;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcAsmOpRelJump(__u8 A_VmIdx, Ptr_GrP4AsmOpRelJump A_PtrCode)
{
	// local -------------------
	// code --------------------
		if ( 0 == A_PtrCode->Count )
		{
			DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpRelJump - [ %d : %d ] jump count zero!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip );
			m_VmTbl[A_VmIdx].Rstat	=	E_GrP4AsmRunStatStop;
		}
		else
		{
			// check over range
			if ( A_PtrCode->IsRvs )
			{
				if ( A_PtrCode->Count > m_VmTbl[A_VmIdx].Ip )
				{
					DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpRelJump - [ %d : %d ] jump count is over (reverse - %d)!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip, A_PtrCode->Count );
					m_VmTbl[A_VmIdx].Rstat	=	E_GrP4AsmRunStatStop;
				}
				else
				{
					m_VmTbl[A_VmIdx].Ip	=	m_VmTbl[A_VmIdx].Ip - A_PtrCode->Count;
				}
			}
			else
			{
				if ( (m_VmTbl[A_VmIdx].Ip + A_PtrCode->Count) >= m_VmCsLim )
				{
					DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpRelJump - [ %d : %d ] jump count is over(normal - %d)!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip, A_PtrCode->Count );
					m_VmTbl[A_VmIdx].Rstat	=	E_GrP4AsmRunStatStop;
				}
				else
				{
					m_VmTbl[A_VmIdx].Ip	=	m_VmTbl[A_VmIdx].Ip + A_PtrCode->Count;
				}
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcAsmOpOfsJump(__u8 A_VmIdx, Ptr_GrP4AsmOpOfsJump A_PtrCode)
{
	// local -------------------
	// code --------------------
		// check equal my position
		if ( A_PtrCode->Pos == m_VmTbl[A_VmIdx].Ip )
		{
			DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpOfsJump - [ %d : %d ] jump to self!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip );
			m_VmTbl[A_VmIdx].Rstat	=	E_GrP4AsmRunStatStop;
		}
		else
		{
			if ( A_PtrCode->Pos >= m_VmCsLim )
			{
				DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpOfsJump - [ %d : %d ] jump position is over(%d)!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip, A_PtrCode->Pos );
				m_VmTbl[A_VmIdx].Rstat	=	E_GrP4AsmRunStatStop;
			}
			else
			{
				m_VmTbl[A_VmIdx].Ip	=	A_PtrCode->Pos;
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcAsmOpTxtClr(__u8 A_VmIdx, Ptr_GrP4AsmOpTxtClear A_PtrCode)
{
	// local -------------------
	// code --------------------
		if ( E_GrP4AsmMaxTxtRegCnt > A_PtrCode->RegIdx )
		{
			GrStrClear( m_VmTbl[A_VmIdx].RegTxtTbl[A_PtrCode->RegIdx] ) ;
		}
		else
		{
			DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpTxtClr - [ %d : %d ] invalid text register(%d)!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip, A_PtrCode->RegIdx );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcAsmOpTxtId(__u8 A_VmIdx, Ptr_GrP4AsmOpTxtIdGet A_PtrCode)
{
	// local -------------------
		__u16	Tv_TxtId;
	// code --------------------
		if ( E_GrP4AsmMaxTxtRegCnt > A_PtrCode->RegIdx )
		{
			Tv_TxtId	=	(__u16)LcAsmArgLoad(A_VmIdx, A_PtrCode->ArgType, A_PtrCode->ArgVal);
			GrStrWcat( m_VmTbl[A_VmIdx].RegTxtTbl[A_PtrCode->RegIdx], TextPtrGet( Tv_TxtId ), 255 );
		}
		else
		{
			DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpTxtId - [ %d : %d ] invalid text register(%d)!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip, A_PtrCode->RegIdx );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcAsmOpTxtDec(__u8 A_VmIdx, Ptr_GrP4AsmOpTxtDec A_PtrCode)
{
	// local -------------------
		__s32		Tv_Val;
		Def_WstrNum	Tv_StrNum;
	// code --------------------
		if ( E_GrP4AsmMaxTxtRegCnt > A_PtrCode->RegIdx )
		{
			Tv_Val	=	LcAsmArgLoad(A_VmIdx, A_PtrCode->ArgType, A_PtrCode->ArgVal);
			GrStrIntToWstr( Tv_StrNum, Tv_Val, (__u32)A_PtrCode->Digit, (BOOL8)A_PtrCode->IsFix, (BOOL8)A_PtrCode->IsSign );
			GrStrWcat( m_VmTbl[A_VmIdx].RegTxtTbl[A_PtrCode->RegIdx], Tv_StrNum, E_GrP4AsmTxtCharCnt - 1 );
		}
		else
		{
			DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpTxtDec - [ %d : %d ] invalid text register(%d)!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip, A_PtrCode->RegIdx );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcAsmOpTxtTime(__u8 A_VmIdx, Ptr_GrP4AsmOpTxtTime A_PtrCode)
{
	// local -------------------
		__s32		Tv_Val;
		Def_WstrTag	Tv_StrTime;
	// code --------------------
		if ( E_GrP4AsmMaxTxtRegCnt > A_PtrCode->RegIdx )
		{
			Tv_Val	=	LcAsmArgLoad(A_VmIdx, A_PtrCode->ArgType, A_PtrCode->ArgVal);
			GrStrTimeToWstr( Tv_StrTime, (Def_GrTime)Tv_Val,
				(BOOL8)A_PtrCode->IsNoTime, (BOOL8)A_PtrCode->IsNoDate, (BOOL8)A_PtrCode->IsNoSec, 
				A_PtrCode->DayGap, A_PtrCode->TimeGap, A_PtrCode->DivGap, !A_PtrCode->IsNoDiv );
			GrStrWcat( m_VmTbl[A_VmIdx].RegTxtTbl[A_PtrCode->RegIdx], Tv_StrTime, E_GrP4AsmTxtCharCnt - 1 );
		}
		else
		{
			DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpTxtTime - [ %d : %d ] invalid text register(%d)!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip, A_PtrCode->RegIdx );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcAsmOpTxtCmp(__u8 A_VmIdx, Ptr_GrP4AsmOpTxtCmp A_PtrCode)
{
	// local -------------------
		__u8	Tv_Treg1;
		__u8	Tv_Treg2;
		__u8	Tv_Flag;
	// code --------------------
		if ( E_GrP4AsmMaxTxtRegCnt > A_PtrCode->Treg1Idx )
		{
			if ( E_GrP4AsmMaxTxtRegCnt > A_PtrCode->Treg2Idx )
			{
				Tv_Treg1	=	A_PtrCode->Treg1Idx;
				Tv_Treg2	=	A_PtrCode->Treg2Idx;
				Tv_Flag		=	0;

				if ( 0 == GrStrWcmp( m_VmTbl[A_VmIdx].RegTxtTbl[Tv_Treg1], m_VmTbl[A_VmIdx].RegTxtTbl[Tv_Treg2], (BOOL8)A_PtrCode->IsCaseSense ) )
				{
					Tv_Flag		=	1;
				}
				
#ifdef GR_P4_EDITOR
				if ( E_GrP4AsmFlagCnt <= A_PtrCode->FlagIdxRslt )
				{
					DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpTxtCmp - [ %d : %d ] bad flag index!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip );
				}
#endif
				Tv_Flag	=	Tv_Flag << A_PtrCode->FlagIdxRslt;
				switch ( A_PtrCode->FlagUpdtType )
				{
					case E_GrP4AsmUpdtFgDirect:
						m_VmTbl[A_VmIdx].Flag	=	m_VmTbl[A_VmIdx].Flag & ( ~(1 << A_PtrCode->FlagIdxRslt) );
						m_VmTbl[A_VmIdx].Flag	=	m_VmTbl[A_VmIdx].Flag | Tv_Flag;
						break;
					case E_GrP4AsmUpdtFgOr:
						m_VmTbl[A_VmIdx].Flag	=	m_VmTbl[A_VmIdx].Flag | Tv_Flag;
						break;
					case E_GrP4AsmUpdtFgAnd:
						m_VmTbl[A_VmIdx].Flag	=	m_VmTbl[A_VmIdx].Flag & (~Tv_Flag);
						break;
					default:
						DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpTxtCmp - [ %d : %d ] bad flag update type!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip );
						break;
				}
				
			}
			else
			{
				DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpTxtCmp - [ %d : %d ] invalid text register(%d)!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip, A_PtrCode->Treg2Idx );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpTxtCmp - [ %d : %d ] invalid text register(%d)!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip, A_PtrCode->Treg1Idx );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcAsmOpTxtLen(__u8 A_VmIdx, Ptr_GrP4AsmOpTxtLen A_PtrCode)
{
	// local -------------------
	// code --------------------
		if ( E_GrP4AsmMaxTxtRegCnt > A_PtrCode->TregIdx )
		{
			if ( E_GrP4AsmMaxIntRegCnt > A_PtrCode->RegIdx )
			{
				m_VmTbl[A_VmIdx].RegIntTbl[A_PtrCode->RegIdx]	=	GrStrWlen( m_VmTbl[A_VmIdx].RegTxtTbl[A_PtrCode->TregIdx] );
			}
			else
			{
				DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpTxtLen - [ %d : %d ] invalid integer register(%d)!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip, A_PtrCode->RegIdx );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpTxtLen - [ %d : %d ] invalid text register(%d)!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip, A_PtrCode->TregIdx );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcAsmOpTxtHex(__u8 A_VmIdx, Ptr_GrP4AsmOpTxtHex A_PtrCode)
{
	// local -------------------
		__u32	Tv_Val;
		Def_WstrNum	Tv_StrNum;
	// code --------------------
		if ( E_GrP4AsmMaxTxtRegCnt > A_PtrCode->RegIdx )
		{
			Tv_Val	=	(__u32)LcAsmArgLoad(A_VmIdx, A_PtrCode->ArgType, A_PtrCode->ArgVal);
			GrStrDwordToHexWstr( Tv_StrNum, Tv_Val, (__u32)A_PtrCode->Digit, (BOOL8)A_PtrCode->IsFix );
			GrStrWcat( m_VmTbl[A_VmIdx].RegTxtTbl[A_PtrCode->RegIdx], Tv_StrNum, E_GrP4AsmTxtCharCnt - 1 );
		}
		else
		{
			DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpTxtHex - [ %d : %d ] invalid text register(%d)!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip, A_PtrCode->RegIdx );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcAsmOpTxtCharAdd(__u8 A_VmIdx, Ptr_GrP4AsmOpTxtChar A_PtrCode)
{
	// local -------------------
		WCHAR	Tv_StrTxt[2];
	// code --------------------
		if ( E_GrP4AsmMaxTxtRegCnt > A_PtrCode->RegIdx )
		{
			Tv_StrTxt[0]	=	A_PtrCode->Char;
			Tv_StrTxt[1]	=	0;
			GrStrWcat( m_VmTbl[A_VmIdx].RegTxtTbl[A_PtrCode->RegIdx], Tv_StrTxt, E_GrP4AsmTxtCharCnt - 1 );
		}
		else
		{
			DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpTxtCharAdd - [ %d : %d ] invalid text register(%d)!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip, A_PtrCode->RegIdx );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcAsmOpTxtCopy(__u8 A_VmIdx, Ptr_GrP4AsmOpTxtCopy A_PtrCode)
{
	// local -------------------
	// code --------------------
		if ( E_GrP4AsmMaxTxtRegCnt > A_PtrCode->SrRegIdx )
		{
			if ( E_GrP4AsmMaxTxtRegCnt > A_PtrCode->TgRegIdx )
			{
				GrStrWcopy( m_VmTbl[A_VmIdx].RegTxtTbl[A_PtrCode->TgRegIdx], m_VmTbl[A_VmIdx].RegTxtTbl[A_PtrCode->SrRegIdx], E_GrP4AsmTxtCharCnt - 1 );
			}
			else
			{
				DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpTxtCopy - [ %d : %d ] invalid target text register(%d)!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip, A_PtrCode->TgRegIdx );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpTxtCopy - [ %d : %d ] invalid source text register(%d)!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip, A_PtrCode->SrRegIdx );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcAsmOpTxtSetSetup(__u8 A_VmIdx, Ptr_GrP4AsmOpTxtSetStp A_PtrCode)
{
	// local -------------------
		__u16	Tv_StpAdr;
		WCHAR*	Tv_StrStp;
	// code --------------------
		if ( E_GrP4AsmMaxTxtRegCnt > A_PtrCode->TregIdx )
		{
			Tv_StpAdr	=	A_PtrCode->AdrSetup;
			if ( A_PtrCode->IsAdd )
			{
				if ( E_GrP4AsmMaxIntRegCnt > A_PtrCode->AddRegIdx )
				{
					Tv_StpAdr	=	(__u16)((__s32)Tv_StpAdr + m_VmTbl[A_VmIdx].RegIntTbl[A_PtrCode->AddRegIdx]);
				}
				else
				{
					DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpTxtSetSetup - [ %d : %d ] invalid address add register(%d)!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip, A_PtrCode->TregIdx );
				}
			}
			Tv_StrStp	=	(WCHAR*)SetupDataPtrGet( Tv_StpAdr );
			GrStrWcopy(	Tv_StrStp,m_VmTbl[A_VmIdx].RegTxtTbl[A_PtrCode->TregIdx] );
		}
		else
		{
			DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpTxtSetSetup - [ %d : %d ] invalid text register(%d)!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip, A_PtrCode->TregIdx );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcAsmOpTxtGetSetup(__u8 A_VmIdx, Ptr_GrP4AsmOpTxtGetStp A_PtrCode)
{
	// local -------------------
		__u16	Tv_StpAdr;
		WCHAR*	Tv_StrStp;
	// code --------------------
		if ( E_GrP4AsmMaxTxtRegCnt > A_PtrCode->TregIdx )
		{
			Tv_StpAdr	=	A_PtrCode->AdrSetup;
			if ( A_PtrCode->IsAdd )
			{
				if ( E_GrP4AsmMaxIntRegCnt > A_PtrCode->AddRegIdx )
				{
					Tv_StpAdr	=	(__u16)((__s32)Tv_StpAdr + m_VmTbl[A_VmIdx].RegIntTbl[A_PtrCode->AddRegIdx]);
				}
				else
				{
					DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpTxtGetSetup - [ %d : %d ] invalid address add register(%d)!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip, A_PtrCode->TregIdx );
				}
			}
			Tv_StrStp	=	(WCHAR*)SetupDataPtrGet( Tv_StpAdr );
			GrStrWcat(	m_VmTbl[A_VmIdx].RegTxtTbl[A_PtrCode->TregIdx], Tv_StrStp, E_GrP4AsmTxtCharCnt - 1 );
		}
		else
		{
			DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpTxtSetSetup - [ %d : %d ] invalid text register(%d)!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip, A_PtrCode->TregIdx );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcAsmOpSetupCopy(__u8 A_VmIdx, Ptr_GrP4AsmOpSetupCopy A_PtrCode)
{
	// local -------------------
		__u16	Tv_AdrSr;
		__u16	Tv_AdrTg;
		void*	Tv_PtrSr;
		void*	Tv_PtrTg;
	// code --------------------
		Tv_AdrTg	=	A_PtrCode->AdrTg;
		Tv_AdrSr	=	A_PtrCode->AdrSr;

		if ( A_PtrCode->IsTgAdd )
		{
			if ( E_GrP4AsmMaxIntRegCnt > A_PtrCode->TgRegIdx )
			{
				Tv_AdrTg	=	(__u16)((__s32)Tv_AdrTg + m_VmTbl[A_VmIdx].RegIntTbl[A_PtrCode->TgRegIdx]);
			}
			else
			{
				DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpSetupCopy - [ %d : %d ] invalid target register(%d)!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip, A_PtrCode->TgRegIdx );
			}
		}

		if ( A_PtrCode->IsSrAdd )
		{
			if ( E_GrP4AsmMaxIntRegCnt > A_PtrCode->SrRegIdx )
			{
				Tv_AdrSr	=	(__u16)((__s32)Tv_AdrSr + m_VmTbl[A_VmIdx].RegIntTbl[A_PtrCode->SrRegIdx]);
			}
			else
			{
				DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpSetupCopy - [ %d : %d ] invalid source register(%d)!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip, A_PtrCode->SrRegIdx );
			}
		}
		
		if ( (0 <= A_PtrCode->Size) && ( (Tv_AdrSr + A_PtrCode->Size) <= E_GrP4MngSdaSize) && ( (Tv_AdrTg + A_PtrCode->Size) <= E_GrP4MngSdaSize) )
		{
			Tv_PtrTg	=	SetupDataPtrGet( Tv_AdrTg );
			Tv_PtrSr	=	SetupDataPtrGet( Tv_AdrSr );
			GrDumyCopyMem( Tv_PtrTg, Tv_PtrSr, (__u32)A_PtrCode->Size );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcAsmOpSetupFill(__u8 A_VmIdx, Ptr_GrP4AsmOpSetupFill A_PtrCode)
{
	// local -------------------
		__u16	Tv_AdrTg;
		void*	Tv_PtrTg;
	// code --------------------
		Tv_AdrTg	=	A_PtrCode->AdrSetup;
		if ( A_PtrCode->IsAdd )
		{
			if ( E_GrP4AsmMaxIntRegCnt > A_PtrCode->AddRegIdx )
			{
				Tv_AdrTg	=	(__u16)((__s32)Tv_AdrTg + m_VmTbl[A_VmIdx].RegIntTbl[A_PtrCode->AddRegIdx]);
			}
			else
			{
				DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpSetupFill - [ %d : %d ] invalid adder register(%d)!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip, A_PtrCode->AddRegIdx );
			}
		}
		if ( E_GrP4AsmMaxIntRegCnt > A_PtrCode->FillRegIdx )
		{
			if ( (0 != A_PtrCode->Size) && ((Tv_AdrTg + A_PtrCode->Size) <= E_GrP4MngSdaSize ) )
			{
				Tv_PtrTg	=	SetupDataPtrGet( Tv_AdrTg );
				GrDumyFillB( Tv_PtrTg, (__u32)A_PtrCode->Size, (__u8)m_VmTbl[A_VmIdx].RegIntTbl[A_PtrCode->FillRegIdx] );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpSetupFill - [ %d : %d ] invalid fill value register(%d)!\n", m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip, A_PtrCode->FillRegIdx );
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcAsmOpWindow(__u8 A_VmIdx, Ptr_GrP4AsmOpWindow A_PtrCode)
{
	// local -------------------
		__u16	Tv_ScptId;
		__u8	Tv_ConIdx;
	// code --------------------
		Tv_ScptId	=	A_PtrCode->ScptId;
		// get console index
		Tv_ConIdx	=	A_PtrCode->ConIdx;
		if ( E_GrP4ConIdxFocus == Tv_ConIdx )
		{
			Tv_ConIdx	=	m_FcsConIdx;
		}
		// check console index over
		if ( E_GrP4MngConMaxCnt > Tv_ConIdx )
		{
			// check exist console
			if ( NULL != m_ConTbl[Tv_ConIdx] )
			{
				LcUiDecWindow( Tv_ScptId, Tv_ConIdx, A_PtrCode->RefWinId );
			}
			else
			{
				DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpWindow - console (%d) not exist.\n", Tv_ConIdx );
				RtlVmFault(A_VmIdx);
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpWindow - bad console index(%d).\n", Tv_ConIdx );
			RtlVmFault(A_VmIdx);
		}

}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcAsmOpUpi(__u8 A_VmIdx, Ptr_GrP4AsmOpUpi A_PtrCode)
{
	// local -------------------
		__u16	Tv_ScptId;
		__u8	Tv_ConIdx;
		BOOL8	Tv_IsNxIp;
		BOOL8	Tv_IsAble;
		Cls_GrP4DoWin*	Tv_Win;
		Cls_GrP4DoBase*	Tv_Obj;
		WCHAR*	Tv_TregTbl[E_GrP4AsmMaxTxtRegCnt];
	// code --------------------
		Tv_IsNxIp	=	TRUE;
		// get console index
		Tv_ConIdx	=	A_PtrCode->ConIdx;
		// check console index over
		if ( E_GrP4MngConMaxCnt > Tv_ConIdx )
		{
			// check exist console
			if ( NULL != m_ConTbl[Tv_ConIdx] )
			{
				// get window
				Tv_ScptId	=	A_PtrCode->WinScptId;
				if ( E_GrP4ScptIdCrnt == Tv_ScptId )
				{
					Tv_Win	=	m_VmHostWin;
				}
				else
				{
					Tv_Win	=	(Cls_GrP4DoWin*)m_ConTbl[Tv_ConIdx]->WinFindByScptId( Tv_ScptId );
				}
				if ( NULL != Tv_Win )
				{
					// get object
					Tv_ScptId	=	A_PtrCode->ObjScptId;
					if ( E_GrP4ScptIdNone == Tv_ScptId )
					{
						Tv_Obj	=	(Cls_GrP4DoBase*)Tv_Win;
					}
					else if ( E_GrP4ScptIdCrnt == Tv_ScptId )
					{
						Tv_Obj	=	m_VmHostObj;
					}
					else
					{
						Tv_Obj	=	Tv_Win->FindSlaveObjByScptId( Tv_ScptId );
					}
					if ( NULL != Tv_Obj )
					{
						// move next ip
						m_VmTbl[A_VmIdx].Ip ++;
						Tv_IsNxIp	=	FALSE;

						// check check UPI able
						Tv_IsAble	=	TRUE;
						if ( (E_GrP4UpiFrmPageSet ==  A_PtrCode->UpiCmd) || (E_GrP4UpiFrmPageBack ==  A_PtrCode->UpiCmd) )
						{
							if ( NULL != m_VmHostObj )
							{
								if ( NULL != m_VmHostObj->FindPrntObjByScptId( Tv_Obj->ScptIdGet() ) )
								{
									Tv_IsAble	=	FALSE;
									DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpUpi - ( %d : %d ) try self destruction!\n", m_VmTbl[A_VmIdx].Cs , m_VmTbl[A_VmIdx].Ip - 1 );
								}
							}
						}
						
						// run command
						if ( Tv_IsAble )
						{
							Tv_TregTbl[0]	=	m_VmTbl[A_VmIdx].RegTxtTbl[0];
							Tv_TregTbl[1]	=	m_VmTbl[A_VmIdx].RegTxtTbl[1];
							Tv_TregTbl[2]	=	m_VmTbl[A_VmIdx].RegTxtTbl[2];
							Tv_TregTbl[3]	=	m_VmTbl[A_VmIdx].RegTxtTbl[3];
							Tv_Obj->RtlCommand( A_PtrCode->UpiCmd, m_VmTbl[A_VmIdx].RegIntTbl, Tv_TregTbl );
						}
						
					}
					else
					{
						DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpUpi - host object (%d) not found.\n", Tv_ScptId );
					}
				}
				else
				{
					DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpUpi - window (%d) not found.\n", Tv_ScptId );
				}
			}
			else
			{
				DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpUpi - console (%d) not exist.\n", Tv_ConIdx );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpUpi - bad console index(%d).\n", Tv_ConIdx );
		}

		if ( Tv_IsNxIp )
		{
			m_VmTbl[A_VmIdx].Ip ++;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcAsmOpNotify(__u8 A_VmIdx, Ptr_GrP4AsmOpNotify A_PtrCode)
{
	// local -------------------
		WCHAR*	Tv_StrCont;
		WCHAR*	Tv_StrTitle;
	// code --------------------
		// init
		m_VmTbl[A_VmIdx].RegIntTbl[0]	=	0;

		if ( E_GrP4AsmMaxTxtRegCnt > A_PtrCode->ConTxtReg )
		{
			Tv_StrCont	=	m_VmTbl[A_VmIdx].RegTxtTbl[A_PtrCode->ConTxtReg];
			Tv_StrTitle	=	TextPtrGet( A_PtrCode->TitleTxtId );
			if ( NotifyBox( A_PtrCode->ConIdx, A_PtrCode->WinScptId, Tv_StrTitle, Tv_StrCont, A_PtrCode->Time, A_PtrCode->RefWinId ) )
			{
				// success
				m_VmTbl[A_VmIdx].RegIntTbl[0]	=	1;
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpNotify - invailid contents text register ( %d )!\n", A_PtrCode->ConTxtReg );
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcAsmOpApi(__u8 A_VmIdx, Ptr_GrP4AsmOpApi A_PtrCode)
{
	// local -------------------
		WCHAR*	Tv_RstrTbl[E_GrP4AsmMaxTxtRegCnt];
	// code --------------------
		
		Tv_RstrTbl[0]	=	m_VmTbl[A_VmIdx].RegTxtTbl[0];
		Tv_RstrTbl[1]	=	m_VmTbl[A_VmIdx].RegTxtTbl[1];
		Tv_RstrTbl[2]	=	m_VmTbl[A_VmIdx].RegTxtTbl[2];
		Tv_RstrTbl[3]	=	m_VmTbl[A_VmIdx].RegTxtTbl[3];
		if ( !GrP4ApiCall( A_PtrCode->ApiCmd, m_VmTbl[A_VmIdx].RegIntTbl, Tv_RstrTbl,m_VmHostObj ) )
		{
			DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpApi - api ( %d ) not defiened.\n" , A_PtrCode->ApiCmd );
		}

}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::RtlVmDoOneCmd(__u8 A_VmIdx)
{
	// local -------------------
		Ptr_GrP4AsmOpBase	Tv_PtrOpBase;
		BOOL8	Tv_IsAble;
		__u8	Tv_Code;
		__u8	Tv_Sp;
		Ptr_GrP4ScptAsmInfo	Tv_CodeInfo;

	// code --------------------
		// check over instruction pointer
		if (m_VmCsLim > m_VmTbl[A_VmIdx].Ip)
		{
			// prepare parse
			Tv_PtrOpBase	=	&m_PtrVmCs[m_VmTbl[A_VmIdx].Ip];
			// check flag
			Tv_IsAble	=	TRUE;
			if ( 0 != Tv_PtrOpBase->FlagAble )
			{
				if (0 == (m_VmTbl[A_VmIdx].Flag & Tv_PtrOpBase->FlagAble))
				{
					Tv_IsAble	=	FALSE;
				}
			}
			if (0 != (m_VmTbl[A_VmIdx].Flag & Tv_PtrOpBase->FlagBan))
			{
				Tv_IsAble	=	FALSE;
			}
			// check dead command
			if ( 0 != (E_GrP4AsmOpDeactMask & Tv_PtrOpBase->Code) )
			{
				Tv_IsAble	=	FALSE;
			}
			if ( Tv_IsAble )
			{
				// parse command
				Tv_Code	=	Tv_PtrOpBase->Code & E_GrP4AsmOpCodeMask;
				switch ( Tv_Code )
				{
					case E_GrP4AsmOpNop:
						m_VmTbl[A_VmIdx].Ip	++;
						break;
					case	E_GrP4AsmOpEnd:
						m_VmTbl[A_VmIdx].Rstat	=	E_GrP4AsmRunStatStop;
						// finish
						break;
					case E_GrP4AsmOpCond:
						LcAsmOpCond(A_VmIdx,(Ptr_GrP4AsmOpCond)Tv_PtrOpBase);
						m_VmTbl[A_VmIdx].Ip	++;
						break;
					case E_GrP4AsmOpMove:
						LcAsmOpMove(A_VmIdx, (Ptr_GrP4AsmOpMove)Tv_PtrOpBase);
						m_VmTbl[A_VmIdx].Ip	++;
						break;
					case E_GrP4AsmOpFlag:
						LcAsmOpFlag(A_VmIdx, (Ptr_GrP4AsmOpFlag)Tv_PtrOpBase);
						m_VmTbl[A_VmIdx].Ip	++;
						break;
					case E_GrP4AsmOpCalc:
						LcAsmOpCalc(A_VmIdx, (Ptr_GrP4AsmOpCalc)Tv_PtrOpBase);
						m_VmTbl[A_VmIdx].Ip	++;
						break;
					case E_GrP4AsmOpNot:
						LcAsmOpNot(A_VmIdx, (Ptr_GrP4AsmOpNot)Tv_PtrOpBase);
						m_VmTbl[A_VmIdx].Ip	++;
						break;
					case E_GrP4AsmOpDirJump:
						LcAsmOpDirJump(A_VmIdx, (Ptr_GrP4AsmOpDirJump)Tv_PtrOpBase);
						break;
					case E_GrP4AsmOpRefJump:
						LcAsmOpRefJump(A_VmIdx, (Ptr_GrP4AsmOpRefJump)Tv_PtrOpBase);
						break;
					case E_GrP4AsmOpRelJump:
						LcAsmOpRelJump(A_VmIdx, (Ptr_GrP4AsmOpRelJump)Tv_PtrOpBase);
						break;
					case E_GrP4AsmOpOfsJump:
						LcAsmOpOfsJump(A_VmIdx, (Ptr_GrP4AsmOpOfsJump)Tv_PtrOpBase);
						break;
					case E_GrP4AsmOpTxtClear:
						LcAsmOpTxtClr(A_VmIdx, (Ptr_GrP4AsmOpTxtClear)Tv_PtrOpBase);
						m_VmTbl[A_VmIdx].Ip	++;
						break;
					case E_GrP4AsmOpTxtIdGet:
						LcAsmOpTxtId(A_VmIdx, (Ptr_GrP4AsmOpTxtIdGet)Tv_PtrOpBase);
						m_VmTbl[A_VmIdx].Ip	++;
						break;
					case E_GrP4AsmOpTxtDec:
						LcAsmOpTxtDec(A_VmIdx, (Ptr_GrP4AsmOpTxtDec)Tv_PtrOpBase);
						m_VmTbl[A_VmIdx].Ip	++;
						break;
					case E_GrP4AsmOpTxtHex:
						LcAsmOpTxtHex(A_VmIdx, (Ptr_GrP4AsmOpTxtHex)Tv_PtrOpBase);
						m_VmTbl[A_VmIdx].Ip	++;
						break;
					case E_GrP4AsmOpTxtChar:
						LcAsmOpTxtCharAdd(A_VmIdx, (Ptr_GrP4AsmOpTxtChar)Tv_PtrOpBase);
						m_VmTbl[A_VmIdx].Ip	++;
						break;
					case E_GrP4AsmOpTxtCopy:
						LcAsmOpTxtCopy(A_VmIdx, (Ptr_GrP4AsmOpTxtCopy)Tv_PtrOpBase);
						m_VmTbl[A_VmIdx].Ip	++;
						break;
					case E_GrP4AsmOpTxtSetStp:
						LcAsmOpTxtSetSetup(A_VmIdx, (Ptr_GrP4AsmOpTxtSetStp)Tv_PtrOpBase);
						m_VmTbl[A_VmIdx].Ip	++;
						break;
					case E_GrP4AsmOpTxtGetStp:
						LcAsmOpTxtGetSetup(A_VmIdx, (Ptr_GrP4AsmOpTxtGetStp)Tv_PtrOpBase);
						m_VmTbl[A_VmIdx].Ip	++;
						break;
					case E_GrP4AsmOpSetupCopy:
						LcAsmOpSetupCopy(A_VmIdx, (Ptr_GrP4AsmOpSetupCopy)Tv_PtrOpBase);
						m_VmTbl[A_VmIdx].Ip	++;
						break;
					case E_GrP4AsmOpSetupFill:
						LcAsmOpSetupFill(A_VmIdx, (Ptr_GrP4AsmOpSetupFill)Tv_PtrOpBase);
						m_VmTbl[A_VmIdx].Ip	++;
						break;
					case E_GrP4AsmOpWindow:
						LcAsmOpWindow(A_VmIdx, (Ptr_GrP4AsmOpWindow)Tv_PtrOpBase);
						m_VmTbl[A_VmIdx].Ip	++;
						break;
					case E_GrP4AsmOpUpi:
						LcAsmOpUpi(A_VmIdx, (Ptr_GrP4AsmOpUpi)Tv_PtrOpBase);
						break;
					case E_GrP4AsmOpApi:
						m_VmTbl[A_VmIdx].Ip	++;
						LcAsmOpApi(A_VmIdx, (Ptr_GrP4AsmOpApi)Tv_PtrOpBase);
						break;
					case E_GrP4AsmOpNotify:
						LcAsmOpNotify(A_VmIdx, (Ptr_GrP4AsmOpNotify)Tv_PtrOpBase);
						m_VmTbl[A_VmIdx].Ip	++;
						break;
					case E_GrP4AsmOpQuery:
						LcAsmOpQuery(A_VmIdx, (Ptr_GrP4AsmOpQuery)Tv_PtrOpBase);
						//m_VmTbl[A_VmIdx].Ip	++;
						break;
					case E_GrP4AsmOpTxtTime:
						LcAsmOpTxtTime(A_VmIdx, (Ptr_GrP4AsmOpTxtTime)Tv_PtrOpBase);
						m_VmTbl[A_VmIdx].Ip	++;
						break;
					case E_GrP4AsmOpTxtCmp:
						LcAsmOpTxtCmp(A_VmIdx, (Ptr_GrP4AsmOpTxtCmp)Tv_PtrOpBase);
						m_VmTbl[A_VmIdx].Ip	++;
						break;
					case E_GrP4AsmOpTxtLen:
						LcAsmOpTxtLen(A_VmIdx, (Ptr_GrP4AsmOpTxtLen)Tv_PtrOpBase);
						m_VmTbl[A_VmIdx].Ip	++;
						break;
					case E_GrP4AsmOpWait:
						LcAsmOpWait(A_VmIdx, (Ptr_GrP4AsmOpWait)Tv_PtrOpBase);
						break;
					case E_GrP4AsmOpLabel:
						LcAsmOpLabel(A_VmIdx, (Ptr_GrP4AsmOpLabel)Tv_PtrOpBase);
						break;
					case E_GrP4AsmOpLblJump:
						LcAsmOpLblJump(A_VmIdx, (Ptr_GrP4AsmOpLblJump)Tv_PtrOpBase);
						break;
					case E_GrP4AsmOpReturn:
						LcAsmOpReturn(A_VmIdx, Tv_PtrOpBase);
						break;
					case E_GrP4AsmOpDcall:
						LcAsmOpDcall(A_VmIdx, (Ptr_GrP4AsmOpDcall)Tv_PtrOpBase);
						break;
					case E_GrP4AsmOpStpGet:
						LcAsmOpStpGet(A_VmIdx, (Ptr_GrP4AsmOpStpGet)Tv_PtrOpBase);
						break;
					case E_GrP4AsmOpStpPut:
						LcAsmOpStpPut(A_VmIdx, (Ptr_GrP4AsmOpStpPut)Tv_PtrOpBase);
						break;

					default:
						// bad opcode
						DbgMsgPrint( "Cls_GrP4Mng::RtlVmDoOneCmd - bad opcode (%d) at cs: %d , ip: %d \n" , Tv_Code, m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip );
						m_VmTbl[A_VmIdx].Rstat	=	E_GrP4AsmRunStatStop;
						break;
				}
			}
			else
			{
				// no execute
				m_VmTbl[A_VmIdx].Ip	++;
			}
		}

		// check status
		if (E_GrP4AsmRunStatRun == m_VmTbl[A_VmIdx].Rstat)
		{
			// check finish
			if (m_VmTbl[A_VmIdx].Ip >= m_VmCsLim)
			{
				// check stack
				if (0 != m_VmTbl[A_VmIdx].Sp)
				{
					// stack pop
					m_VmTbl[A_VmIdx].Sp --;
					// change pointer
					Tv_Sp	=	m_VmTbl[A_VmIdx].Sp;
					Tv_CodeInfo			=	AsmPtrGet(m_VmTbl[A_VmIdx].CallStack[Tv_Sp].Cs);
					if (NULL != Tv_CodeInfo)
					{
						m_PtrVmCs				=	(Ptr_GrP4AsmOpBase)Tv_CodeInfo->Pos;
						m_VmCsLim				=	Tv_CodeInfo->Cnt;
						m_VmTbl[A_VmIdx].Cs			=	m_VmTbl[A_VmIdx].CallStack[Tv_Sp].Cs;
						m_VmTbl[A_VmIdx].Ip			=	m_VmTbl[A_VmIdx].CallStack[Tv_Sp].Ip;
					}
					else
					{
						m_VmTbl[A_VmIdx].Rstat	=	E_GrP4AsmRunStatStop;
						DbgMsgPrint("Cls_GrP4Mng::RtlVmDoOneCmd - VM(%d) stack broken!\n", A_VmIdx);
					}
				}
				else
				{
					// finish
					m_VmTbl[A_VmIdx].Rstat	=	E_GrP4AsmRunStatStop;
				}
			}

		}

}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::RtlVmPumpAll(void)
{
	// local -------------------
		__u8	Tv_RerunCnt;
		__u8	Tv_VmIdx;
	// code --------------------
		Tv_RerunCnt	=	0;
		Tv_VmIdx		=	0;

		while (E_GrP4MngVmRerunMax > Tv_RerunCnt)
		{
			// reset rerun
			m_IsVmReqRerun	=	FALSE;

			// do slice
			RtlVmDoSlice(Tv_VmIdx);

			// next
			Tv_VmIdx ++;
			if (m_IsVmReqRerun)
			{
				Tv_VmIdx	=	0;
			}
			if (E_GrP4MngVmCnt <= Tv_VmIdx)
			{
				break;
			}

			// next
			Tv_RerunCnt ++;
		}

		if (E_GrP4MngVmRerunMax <= Tv_RerunCnt)
		{
			// debug message
			DbgMsgPrint("Cls_GrP4Mng::RtlVmPumpAll - rerun count over.\n");
			// display VM status

		}
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::RtlVmDoSlice(__u8 A_VmIdx)
{
	// local -------------------
		Ptr_GrP4ScptAsmInfo	Tv_CodeInfo;
		__u32	Tv_RunCnt;
	// code --------------------
		// init

		// check run able
		if (E_GrP4AsmRunStatRun == m_VmTbl[A_VmIdx].Rstat)
		{
			// prepare
			Tv_CodeInfo			=	AsmPtrGet(m_VmTbl[A_VmIdx].Cs);
			if (NULL != Tv_CodeInfo)
			{
				// prepare
				m_PtrVmCs				=	(Ptr_GrP4AsmOpBase)Tv_CodeInfo->Pos;
				m_VmCsLim				=	Tv_CodeInfo->Cnt;
				m_VmHostObj			=	NULL;
				m_VmHostWin			=	NULL;

				if (NULL != m_ConTbl[m_VmTbl[A_VmIdx].ConIdx])
				{
					m_VmHostWin	=	m_ConTbl[m_VmTbl[A_VmIdx].ConIdx]->WinFindByWinId(m_VmTbl[A_VmIdx].HostWinId);
					if (NULL != m_VmHostWin)
					{
						if (E_GrP4ScptIdNone == m_VmTbl[A_VmIdx].HostScptId)
						{
							m_VmHostObj	=	(Cls_GrP4DoBase*)m_VmHostWin;
						}
						else
						{
							m_VmHostObj	=	m_VmHostWin->FindSlaveObjByScptId(m_VmTbl[A_VmIdx].HostScptId);
						}
					}
				}

				// run
				for (Tv_RunCnt=0; Tv_RunCnt <E_GrP4MngVmSliceMax; Tv_RunCnt++)
				{
					RtlVmDoOneCmd(A_VmIdx);
					if (E_GrP4AsmRunStatRun != m_VmTbl[A_VmIdx].Rstat)
					{
						break;
					}
				}
			}
			else
			{
				m_VmTbl[A_VmIdx].Rstat	=	E_GrP4AsmRunStatStop;
			}
		}
		
}
//--------------------------------------------------------------------
__s32	Cls_GrP4Mng::LcVmFindEmptySlot(void)
{
	// local -------------------
		__s32	Tv_Result;
		__u8	Tv_WkIdx;
	// code --------------------
		// init
		Tv_Result	=	-1;

		for (Tv_WkIdx = 0; Tv_WkIdx < E_GrP4MngVmCnt; Tv_WkIdx++)
		{
			if (E_GrP4AsmRunStatStop ==  m_VmTbl[Tv_WkIdx].Rstat)
			{
				// found
				Tv_Result	=	(__s32)Tv_WkIdx;
				break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4Mng::VmRunCode(__u16 A_CodeId, Cls_GrP4DoBase* A_ObjHost, Ptr_GrP4MngVmRpara A_PtrRpara, BOOL8 A_IsRunNow)
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrP4DoWin*	Tv_Win;
		__s32	Tv_VmIdx;
	// code --------------------
		Tv_Result	=	FALSE;

		// check able page
		if ( E_GrP4AsmIdNone != A_CodeId )
		{
			// find empty slot
			Tv_VmIdx	=	LcVmFindEmptySlot();
			if (0 <= Tv_VmIdx)
			{
				// init VM
				Tv_Win		=	NULL;
				m_VmTbl[Tv_VmIdx].ConIdx			=	0;
				m_VmTbl[Tv_VmIdx].HostWinId		=	E_GrP4WinIdNone;
				m_VmTbl[Tv_VmIdx].HostScptId	=	E_GrP4ScptIdNone;
				if (NULL != A_ObjHost)
				{
					// get window id
					Tv_Win	=	(Cls_GrP4DoWin*)A_ObjHost->WindowGet();
					m_VmTbl[Tv_VmIdx].HostWinId	=	Tv_Win->WinIdGet();
					m_VmTbl[Tv_VmIdx].ConIdx		=	((Cls_GrP4ConBase*)Tv_Win->ConsoleGet())->IndexGet();
					if (!A_ObjHost->IsWindow())
					{
						m_VmTbl[Tv_VmIdx].HostScptId	=	A_ObjHost->ScptIdGet();
					}
				}
				m_VmTbl[Tv_VmIdx].Cs		=	A_CodeId;
				m_VmTbl[Tv_VmIdx].Ip		=	0;
				m_VmTbl[Tv_VmIdx].Sp		=	0;
				m_VmTbl[Tv_VmIdx].Flag	=	0;

				if (LcVmRparaIn((__u8)Tv_VmIdx, A_PtrRpara))
				{
					// run code
					m_VmTbl[Tv_VmIdx].Rstat	=	E_GrP4AsmRunStatRun;
					m_IsVmReqRerun	=	TRUE;
					if (A_IsRunNow)
					{
						RtlVmDoSlice((__u8)Tv_VmIdx);
						// check value update
						if (NULL != A_PtrRpara)
						{
							if (0 != A_PtrRpara->Out.Cnt)
							{
								// check get able
								if (E_GrP4AsmRunStatStop == m_VmTbl[Tv_VmIdx].Rstat)
								{
									LcVmRparaOut((__u8)Tv_VmIdx, A_PtrRpara);
								}
								else
								{
									DbgMsgPrint("Cls_GrP4Mng::VmRunCode - The execute was not completed ,so the value can not be reached.\n");
								}
							}

						}
					}
				}
			}
			else
			{
				// not enough VM
				DbgMsgPrint("Cls_GrP4Mng::VmRunCode - VM is full!\n");
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcTickMsgBrdcst( __u32 A_Tick )
{
	// local -------------------
		__u8	Tv_ConIdx;
		St_GrP4MsgTimeTick	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.Cmd		=	E_GrP4MsgCmdTickPeriod;
		Tv_Msg.Hd.WinId	=	E_GrP4WinIdAll;
		Tv_Msg.Hd.ObjId	=	E_GrP4ScptIdAll;
		Tv_Msg.Tick			=	A_Tick;
		for ( Tv_ConIdx=0;Tv_ConIdx < E_GrP4MngConMaxCnt;Tv_ConIdx++ )
		{
			Tv_Msg.Hd.ConIdx	=	Tv_ConIdx;
			if ( NULL != m_ConTbl[Tv_ConIdx] )
			{
				m_ConTbl[Tv_ConIdx]->RtlUiMsgProc( (Ptr_GrP4MsgHd)&Tv_Msg );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcConRedrawAll( void )
{
	// local -------------------
		__u8	Tv_ConIdx;
	// code --------------------
		for ( Tv_ConIdx=0;Tv_ConIdx < E_GrP4MngConMaxCnt;Tv_ConIdx++ )
		{
			if ( NULL != m_ConTbl[Tv_ConIdx] )
			{
				m_ConTbl[Tv_ConIdx]->PaintAllReq();
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcWinAllFree( void )
{
	// local -------------------
		__u8	Tv_ConIdx;
	// code --------------------
		for ( Tv_ConIdx=0;Tv_ConIdx < E_GrP4MngConMaxCnt;Tv_ConIdx++ )
		{
			if ( NULL != m_ConTbl[Tv_ConIdx] )
			{
				m_ConTbl[Tv_ConIdx]->WindowAllFree();
			}
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrP4Mng::LangSelGet( void )
{
		return	m_LangIdx;
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LangSelSet( __u8 A_Idx )
{
		if ( E_GrP4ScptLangMaxCnt > A_Idx )
		{
			m_LangIdx	=	A_Idx;
			// redraw
			LcConRedrawAll();
		}
		else
		{
			DbgMsgPrint("Cls_GrP4Mng::LangSelSet - language ( %d ) range over!\n", (__s32)A_Idx );
		}
}
//--------------------------------------------------------------------
Cls_GrP4Font*	Cls_GrP4Mng::FontGet(__u8 A_Id)
{
	// local -------------------
		Cls_GrP4Font*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;

		if ( E_GrP4MngFontMaxCnt > A_Id )
		{
			Tv_Result	=	m_FontMng[A_Id];
		}
		else
		{
			DbgMsgPrint("Cls_GrP4Mng::FontGet - font id ( %d ) range over!\n", (__s32)A_Id );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrP4Mng::CursorColorGet( void )
{
	// local -------------------
		__u32	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		if ( NULL != m_PtrCfgRtl )
		{
			Tv_Result	=	m_PtrCfgRtl->Skin.ClrCus;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcVmReset( void )
{
	// local -------------------
	// code --------------------
		GrDumyZeroMem( m_VmTbl, sizeof(m_VmTbl) );

		m_PtrVmCs		=	NULL;
		m_VmCsLim		=	0;
		m_VmHostObj	=	NULL;
		m_VmHostWin	=	NULL;

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4Mng::LcVmRparaIn(__u8 A_VmIdx, Ptr_GrP4MngVmRpara A_PtrRpara)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_WkIdx;
		__u8	Tv_IregIdx;
	// code --------------------
		Tv_Result	=	FALSE;

		if (E_GrP4MngVmCnt > A_VmIdx)
		{
			Tv_Result	=	TRUE;
			if (NULL != A_PtrRpara)
			{
				if ((0 != A_PtrRpara->In.Cnt)&&(E_GrP4MngVmRparaMax >= A_PtrRpara->In.Cnt))
				{
					for (Tv_WkIdx=0; Tv_WkIdx < (__u8)A_PtrRpara->In.Cnt; Tv_WkIdx++)
					{
						Tv_IregIdx	=	A_PtrRpara->In.Ridx[Tv_WkIdx];
						if (E_GrP4AsmMaxIntRegCnt > Tv_IregIdx)
						{
							m_VmTbl[A_VmIdx].RegIntTbl[Tv_IregIdx]	=	A_PtrRpara->In.Rval[Tv_WkIdx];	// update value
						}
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4Mng::LcVmRparaOut(__u8 A_VmIdx, Ptr_GrP4MngVmRpara A_PtrRpara)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_WkIdx;
		__u8	Tv_IregIdx;
	// code --------------------
		Tv_Result	=	FALSE;

		if (E_GrP4MngVmCnt > A_VmIdx)
		{
			if (NULL != A_PtrRpara)
			{
				if ((0 != A_PtrRpara->Out.Cnt)&&(E_GrP4MngVmRparaMax >= A_PtrRpara->Out.Cnt))
				{
					for (Tv_WkIdx=0; Tv_WkIdx < (__u8)A_PtrRpara->Out.Cnt; Tv_WkIdx++)
					{
						Tv_IregIdx	=	A_PtrRpara->Out.Ridx[Tv_WkIdx];
						if (E_GrP4AsmMaxIntRegCnt > Tv_IregIdx)
						{
							A_PtrRpara->Out.Rval[Tv_WkIdx]		=	m_VmTbl[A_VmIdx].RegIntTbl[Tv_IregIdx];
						}
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4Mng::SysInit( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__s32		Tv_Rint[2];
		__u8	Tv_WkIdx;
		__u8	Tv_Val;
		__u32	Tv_Speed;
		__u8	Tv_DataBit;
		__u8	Tv_StopBit;
		__u8	Tv_Parity;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( m_IsScptAble )
		{
			// GVS init
			m_Gvs.HddMaxCnt		=	m_PtrCfgMdl->MaxHddCnt;

			m_Gvs.RunDispRes	=	m_SdaBuf[E_GrP4CfgOfsDispRes];

			GrP4ApiCall(E_GrP4ApiLanCableStat, Tv_Rint, NULL, NULL);
			m_Gvs.IsLanOn		=	(BOOL8)Tv_Rint[0];	// LAN cable status initialize

			// audio
			m_Gvs.AdoLiveCh	=	0xFF;
			m_Gvs.AdoPlayCh	=	0xFF;
			for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrMaxChCnt;Tv_WkIdx++ )
			{
				m_Gvs.AdoLiveVol[Tv_WkIdx]	=	50;
				m_Gvs.AdoPlayVol[Tv_WkIdx]	=	50;
			}

			// PTZ
			LcPtzSetup();

			// UART init
			if ( 0 != m_PtrCfgPrj->UartCnt )
			{
				for ( Tv_WkIdx=0;Tv_WkIdx < m_PtrCfgPrj->UartCnt;Tv_WkIdx++ )
				{
					if ( E_GrP4CfgUartMax <= Tv_WkIdx )
					{
						break;
					}
					Tv_Speed	=	9600;
					Tv_Val		=	m_SdaBuf[E_GrP4CfgOfsUartSpeed + Tv_WkIdx];
					if ( E_GrP4CfgUartSpeedCnt > Tv_Val )
					{
						Tv_Speed	=	V_GrP4UartSpeedTbl[Tv_Val];
					}
					Tv_DataBit	=	8;
					if ( 0 != m_SdaBuf[E_GrP4CfgOfsUartDataBit + Tv_WkIdx] )
					{
						Tv_DataBit	=	7;
					}
					Tv_StopBit	=	m_SdaBuf[E_GrP4CfgOfsUartStopBit + Tv_WkIdx];
					Tv_Parity		=	m_SdaBuf[E_GrP4CfgOfsUartParity + Tv_WkIdx];
#ifndef	GR_P4_EDITOR
					GrUartSetup( Tv_WkIdx, Tv_Speed, Tv_DataBit, Tv_Parity, Tv_StopBit );
#endif
				}
			}

			// setup assign
			GrP4ApiCall(E_GrP4ApiCfgAssign, Tv_Rint, NULL, NULL);

			// check exist script
			if ( m_IsScptAble )
			{
				// close all window
				LcWinAllFree();
				// check exist init function
				m_Gvs.PrgsInit	=	0;		// init progress
				// VM reset
				LcVmReset();
				VmRunCode(m_PtrCfgMdl->AsmInit, NULL, NULL, TRUE);
				m_SysStat		=	E_GrP4MngSysStatInit;
#ifdef GR_P4_EDITOR
				DbgMsgPrint("Cls_GrP4Mng::ScriptRun - Init function running...\n");
#endif
				Tv_Result		=	TRUE;
			}
			else
			{
				DbgMsgPrint( "Cls_GrP4Mng::ScriptRun - no script exist!\n" );
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4Mng::SysStart( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_QuadMode;
		__u8	Tv_QuadPage;
	// code --------------------
		Tv_Result	=	FALSE;

		// check exist script
		if ( m_IsScptAble )
		{
			// check exist init function
			// do start function
			VmRunCode(m_PtrCfgMdl->AsmStart, NULL, NULL, TRUE);
			Tv_Result		=	TRUE;
		}
		else
		{
			DbgMsgPrint( "Cls_GrP4Mng::SysStart - no script exist!\n" );
		}

		// last emergency
		if (((Ptr_GrP4RgstSys)m_RgstBuf)->IsPanic)
		{
			// run emergency
			RtlTryRecEmgc( TRUE );
		}

		// last quad restore
		Tv_QuadMode	=	((Ptr_GrP4RgstSys)m_RgstBuf)->LiveQdMode;
		Tv_QuadPage	=	((Ptr_GrP4RgstSys)m_RgstBuf)->LiveQdPage;
		SafeQuadModeSet( 0, Tv_QuadMode, Tv_QuadPage, 0 );

		// change mode
		m_SysStat	=	E_GrP4MngSysStatStart;
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LiveAtbSet( __u8 A_Ch, __u8 A_Atb )
{
		m_LiveAtbTbl[A_Ch]	=	A_Atb;
}
//--------------------------------------------------------------------
__u8*	Cls_GrP4Mng::LiveAtbTblPtrGet( void )
{
		return	m_LiveAtbTbl;
}
//--------------------------------------------------------------------
__u8*	Cls_GrP4Mng::PlayAtbTblPtrGet( void )
{
		return	m_PlayAtbTbl;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4Mng::IsPlayMode( void )
{
		return	m_Gvs.IsPlayMode;
}
//--------------------------------------------------------------------
__u8	Cls_GrP4Mng::VdoMaxChCntGet( void )
{
		return	m_Gvs.VdoMaxCh;
}
//--------------------------------------------------------------------
__u8	Cls_GrP4Mng::AdoMaxChCntGet( void )
{
		return	m_Gvs.AdoMaxCh;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4Mng::PageDec( __u16 A_PageId, Cls_GrP4DoBase* A_Prnt )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_PgIdx;
	// code --------------------
		Tv_Result	=	FALSE;
		// check page id range
		if ( (E_GrP4ScptIdPageBase <= A_PageId) && ((E_GrP4ScptIdPageBase + m_PageCnt) > A_PageId) )
		{
			Tv_PgIdx	=	A_PageId - E_GrP4ScptIdPageBase;
			// check exist script
			if ( NULL != m_PtrPageTbl[Tv_PgIdx].Pos )
			{
				if ( 0 != LcUiDecSlave( A_Prnt, m_PtrPageTbl[Tv_PgIdx].Pos, TRUE ) )
				{
					Tv_Result	=	TRUE;
				}
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrP4Mng::PageAsmOpenGet( __u16 A_PageId )
{
	// local -------------------
		__u16	Tv_Result;
		__u16	Tv_PgIdx;
	// code --------------------
		Tv_Result	=	E_GrP4ScptIdNone;
		// check page id range
		if ( (E_GrP4ScptIdPageBase <= A_PageId) && ((E_GrP4ScptIdPageBase + m_PageCnt) > A_PageId) )
		{
			Tv_PgIdx	=	A_PageId - E_GrP4ScptIdPageBase;
			// check exist script
			if ( NULL != m_PtrPageTbl[Tv_PgIdx].Pos )
			{
				Tv_Result	=	m_PtrPageTbl[Tv_PgIdx].AsmOpen;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrP4Mng::PageAsmCloseGet( __u16 A_PageId )
{
	// local -------------------
		__u16	Tv_Result;
		__u16	Tv_PgIdx;
	// code --------------------
		Tv_Result	=	E_GrP4ScptIdNone;
		// check page id range
		if ( (E_GrP4ScptIdPageBase <= A_PageId) && ((E_GrP4ScptIdPageBase + m_PageCnt) > A_PageId) )
		{
			Tv_PgIdx	=	A_PageId - E_GrP4ScptIdPageBase;
			// check exist script
			if ( NULL != m_PtrPageTbl[Tv_PgIdx].Pos )
			{
				Tv_Result	=	m_PtrPageTbl[Tv_PgIdx].AsmClose;
			}
		}
		return	Tv_Result;

}
//--------------------------------------------------------------------
__u16	Cls_GrP4Mng::PageEscKeyFcsIdGet( __u16 A_PageId )
{
	// local -------------------
		__u16	Tv_Result;
		__u16	Tv_PgIdx;
	// code --------------------
		Tv_Result	=	E_GrP4ScptIdNone;
		// check page id range
		if ( (E_GrP4ScptIdPageBase <= A_PageId) && ((E_GrP4ScptIdPageBase + m_PageCnt) > A_PageId) )
		{
			Tv_PgIdx	=	A_PageId - E_GrP4ScptIdPageBase;
			// check exist script
			if ( NULL != m_PtrPageTbl[Tv_PgIdx].Pos )
			{
				Tv_Result	=	m_PtrPageTbl[Tv_PgIdx].EscKeyFcsId;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrP4Mng::VkbdTxtPageCntGet( __u8 A_Lang )
{
	// local -------------------
		__u8	Tv_LngIdx;
	// code --------------------
		Tv_LngIdx	=	m_LangIdx;
		if ( E_GrP4ScptLangMaxCnt > A_Lang )
		{
			Tv_LngIdx	=	A_Lang;
		}
		return	m_PtrCfgPrj->VkbdTbl[Tv_LngIdx].PageCnt;
}
//--------------------------------------------------------------------
WCHAR	Cls_GrP4Mng::VkbdTxtKeyCodeGet( __u8 A_Page, __u8 A_Key, __u8 A_Lang )
{
	// local -------------------
		__u8	Tv_LngIdx;
	// code --------------------
		Tv_LngIdx	=	m_LangIdx;
		if ( E_GrP4ScptLangMaxCnt > A_Lang )
		{
			Tv_LngIdx	=	A_Lang;
		}
		return	m_PtrCfgPrj->VkbdTbl[Tv_LngIdx].KeyTbl[A_Page][A_Key];
}
//--------------------------------------------------------------------
void*	Cls_GrP4Mng::SetupDataPtrGet( __u16 A_Ofs )
{
	// local -------------------
	// code --------------------
		return	(void*)( (__u32)m_SdaBuf + (__u32)A_Ofs );
}
//--------------------------------------------------------------------
void*	Cls_GrP4Mng::GvsPtrGet( void )
{
	// local -------------------
	// code --------------------
		return	(void*)&m_Gvs;
}
//--------------------------------------------------------------------
void*	Cls_GrP4Mng::RegistryPtrGet( void )
{
		return	(void*)m_RgstBuf;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4Mng::VkbdTxtRun( Cls_GrP4DoBase* A_Host, WCHAR* A_PtrVal, __u32 A_MaxLen, BOOL8 A_IsPasswd, BOOL8 A_IsAdvPasswd )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_SlotIdx;
		Cls_GrP4DoVkbdTxt*	Tv_Win;
		Ptr_GrP4ScptObjHd	Tv_PtrScpt;
		St_GrRect	Tv_RcObj;
		Cls_GrP4ConBase*	Tv_ObjCon;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrP4ScptIdWinBase <= m_PtrCfgMdl->TxtVkbdId )
		{
			Tv_SlotIdx	=	m_PtrCfgMdl->TxtVkbdId - E_GrP4ScptIdWinBase;
			if ( m_WinCnt > Tv_SlotIdx )
			{
				// get script
				if ( NULL != m_PtrWinTbl[Tv_SlotIdx].Pos )
				{
					// script decode
					Tv_PtrScpt	=	(Ptr_GrP4ScptObjHd)m_PtrWinTbl[Tv_SlotIdx].Pos;
					// check script head
					if ( E_GrP4ScptObjStx == Tv_PtrScpt->Stx )
					{
						// check first window class type
						if ( E_GrP4DoClassWin == Tv_PtrScpt->ClassId )
						{
							// check script id
							if ( m_PtrCfgMdl->TxtVkbdId == Tv_PtrScpt->ScptId )
							{
								// get console
								Tv_ObjCon	=	(Cls_GrP4ConBase*)A_Host->ConsoleGet();
								// check already exist
								if ( NULL == Tv_ObjCon->WinFindByScptId( m_PtrCfgMdl->TxtVkbdId ) )
								{
									// create window
									Tv_RcObj.left		=	Tv_PtrScpt->Rect.left;
									Tv_RcObj.top		=	Tv_PtrScpt->Rect.top;
									Tv_RcObj.right	=	Tv_PtrScpt->Rect.right;
									Tv_RcObj.bottom	=	Tv_PtrScpt->Rect.bottom;
									Tv_Win	=	(Cls_GrP4DoVkbdTxt*)new Cls_GrP4DoVkbdTxt( m_PtrCfgMdl->TxtVkbdId, 
										&Tv_RcObj, Tv_PtrScpt->PosRel, m_PtrWinTbl[Tv_SlotIdx].Layer,
										A_Host, A_PtrVal, A_MaxLen, A_IsPasswd, A_IsAdvPasswd );
									// decode other option
									LcUiDecSlave( (Cls_GrP4DoBase*)Tv_Win, Tv_PtrScpt, TRUE );
									// call init
									Tv_Win->RtlInit();
									// request focus change
									Tv_ObjCon->FocusReset();
									Tv_ObjCon->FocusCorrect();
									// success
									Tv_Result	=	TRUE;
								}
								else
								{
									DbgMsgPrint( "Cls_GrP4Mng::VkbdTxtRun - window ( %d ) already exist!\n", m_PtrCfgMdl->TxtVkbdId );
								}
							}
							else
							{
								DbgMsgPrint( "Cls_GrP4Mng::VkbdTxtRun - buffer script id  ( %d )  is mismatch orignal id ( %d ) !\n", Tv_PtrScpt->ScptId , m_PtrCfgMdl->TxtVkbdId  );
							}
						}
						else
						{
							DbgMsgPrint( "Cls_GrP4Mng::VkbdTxtRun - bad script ( %d )  - class type ( %d ) missmatch!\n" , m_PtrCfgMdl->TxtVkbdId, Tv_PtrScpt->ClassId );
						}
					}
					else
					{
						DbgMsgPrint( "Cls_GrP4Mng::VkbdTxtRun - script ( %d ) error! : bad stx.\n" , m_PtrCfgMdl->TxtVkbdId );
					}
				}
				else
				{
					DbgMsgPrint( "Cls_GrP4Mng::VkbdTxtRun - script ( %d ) not exist .\n" , m_PtrCfgMdl->TxtVkbdId );
				}
			}
			else
			{
				DbgMsgPrint( "Cls_GrP4Mng::VkbdTxtRun - Window Id ( %d ) is out of range.\n" , m_PtrCfgMdl->TxtVkbdId );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4Mng::VkbdNumRun( Cls_GrP4DoBase* A_Host,  WCHAR* A_PtrVal, __u32 A_MaxLen, BOOL8 A_IsPasswd )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_SlotIdx;
		Cls_GrP4DoVkbdNum*	Tv_Win;
		Ptr_GrP4ScptObjHd	Tv_PtrScpt;
		St_GrRect	Tv_RcObj;
		Cls_GrP4ConBase*	Tv_ObjCon;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrP4ScptIdWinBase <= m_PtrCfgMdl->NumVkbdId )
		{
			Tv_SlotIdx	=	m_PtrCfgMdl->NumVkbdId - E_GrP4ScptIdWinBase;
			if ( m_WinCnt > Tv_SlotIdx )
			{
				// get script
				if ( NULL != m_PtrWinTbl[Tv_SlotIdx].Pos )
				{
					// script decode
					Tv_PtrScpt	=	(Ptr_GrP4ScptObjHd)m_PtrWinTbl[Tv_SlotIdx].Pos;
					// check script head
					if ( E_GrP4ScptObjStx == Tv_PtrScpt->Stx )
					{
						// check first window class type
						if ( E_GrP4DoClassWin == Tv_PtrScpt->ClassId )
						{
							// check script id
							if ( m_PtrCfgMdl->NumVkbdId == Tv_PtrScpt->ScptId )
							{
								// get console
								Tv_ObjCon	=	(Cls_GrP4ConBase*)A_Host->ConsoleGet();

								// check already exist
								if ( NULL == Tv_ObjCon->WinFindByScptId( m_PtrCfgMdl->NumVkbdId ) )
								{
									// create window
									Tv_RcObj.left		=	Tv_PtrScpt->Rect.left;
									Tv_RcObj.top		=	Tv_PtrScpt->Rect.top;
									Tv_RcObj.right	=	Tv_PtrScpt->Rect.right;
									Tv_RcObj.bottom	=	Tv_PtrScpt->Rect.bottom;
									Tv_Win	=	(Cls_GrP4DoVkbdNum*)new Cls_GrP4DoVkbdNum( m_PtrCfgMdl->NumVkbdId, 
										&Tv_RcObj, Tv_PtrScpt->PosRel, m_PtrWinTbl[Tv_SlotIdx].Layer,
										A_Host, A_PtrVal, A_MaxLen, A_IsPasswd );
									// decode other option
									LcUiDecSlave( (Cls_GrP4DoBase*)Tv_Win, Tv_PtrScpt, TRUE );
									// call init
									Tv_Win->RtlInit();
									// requist focus change
									Tv_ObjCon->FocusReset();
									Tv_ObjCon->FocusCorrect();
									// success
									Tv_Result	=	TRUE;
								}
							}
							else
							{
								DbgMsgPrint( "Cls_GrP4Mng::VkbdNumRun - buffer script id  ( %d )  is mismatch orignal id ( %d ) !\n", Tv_PtrScpt->ScptId , m_PtrCfgMdl->NumVkbdId  );
							}
						}
						else
						{
							DbgMsgPrint( "Cls_GrP4Mng::VkbdNumRun - bad script ( %d )  - class type ( %d ) missmatch!\n" , m_PtrCfgMdl->NumVkbdId, Tv_PtrScpt->ClassId );
						}
					}
					else
					{
						DbgMsgPrint( "Cls_GrP4Mng::VkbdNumRun - script ( %d ) error! : bad stx.\n" , m_PtrCfgMdl->NumVkbdId );
					}
				}
				else
				{
					DbgMsgPrint( "Cls_GrP4Mng::VkbdNumRun - script ( %d ) not exist .\n" , m_PtrCfgMdl->NumVkbdId );
				}
			}
			else
			{
				DbgMsgPrint( "Cls_GrP4Mng::VkbdNumRun - Window Id ( %d ) is out of range.\n" , m_PtrCfgMdl->NumVkbdId );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4Mng::VkbdRun( Cls_GrP4DoBase* A_Host, WCHAR* A_PtrVal, __u32 A_MaxLen, BOOL8 A_IsPasswd, 
	BOOL8 A_IsNumType, BOOL8 A_IsAdvPasswd )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		if ( A_IsNumType )
		{
			Tv_Result	=	VkbdNumRun( A_Host, A_PtrVal, A_MaxLen, A_IsPasswd );
		}
		else
		{
			Tv_Result	=	VkbdTxtRun( A_Host, A_PtrVal, A_MaxLen, A_IsPasswd, A_IsAdvPasswd );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::SysPrgsInitSet( __u8 A_Prgs )
{
	// local -------------------
	// code --------------------
		m_Gvs.PrgsInit	=	A_Prgs;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4Mng::IsUiEventAble( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( 0 == m_EventLock )
		{
			Tv_Result	=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::UiEventLock( void )
{
		m_EventLock ++;
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::UiEventUnlock( void )
{
		m_EventLock --;
}
//--------------------------------------------------------------------
Ptr_GrP4ScptObjHd	Cls_GrP4Mng::LcGetWinScpt(__u8* A_PtrConIdx, __u16 A_ScptId, Ptr_GrRect A_PtrRtRc, __u8* A_PtrRtRelPos, __u8* A_PtrRtLayer)
{
	// local -------------------
		Ptr_GrP4ScptObjHd	Tv_Result;
		__u16	Tv_SlotIdx;
		Cls_GrP4ConBase*	Tv_ObjCon;
		Ptr_GrP4ScptObjHd	Tv_PtrScpt;
	// code --------------------
		Tv_Result	=	NULL;

		// get console index
		if ( E_GrP4ConIdxFocus == *A_PtrConIdx )
		{
			*A_PtrConIdx	=	m_FcsConIdx;
		}

		// check console index over
		if ( E_GrP4MngConMaxCnt > *A_PtrConIdx )
		{
			// check exist console
			if ( NULL != m_ConTbl[*A_PtrConIdx] )
			{
				// init
				Tv_ObjCon	=	m_ConTbl[*A_PtrConIdx];

				if ( E_GrP4ScptIdWinBase <= A_ScptId )
				{
					Tv_SlotIdx	=	A_ScptId - E_GrP4ScptIdWinBase;
					if ( m_WinCnt > Tv_SlotIdx )
					{
						// get script
						if ( NULL != m_PtrWinTbl[Tv_SlotIdx].Pos )
						{
							// script decode
							Tv_PtrScpt	=	(Ptr_GrP4ScptObjHd)m_PtrWinTbl[Tv_SlotIdx].Pos;
							// check script head
							if ( E_GrP4ScptObjStx == Tv_PtrScpt->Stx )
							{
								// check first window class type
								if ( E_GrP4DoClassWin == Tv_PtrScpt->ClassId )
								{
									// check script id
									if ( A_ScptId == Tv_PtrScpt->ScptId )
									{
										// check already exist
										if ( NULL == Tv_ObjCon->WinFindByScptId( A_ScptId ) )
										{
											A_PtrRtRc->left		=	Tv_PtrScpt->Rect.left;
											A_PtrRtRc->top			=	Tv_PtrScpt->Rect.top;
											A_PtrRtRc->right		=	Tv_PtrScpt->Rect.right;
											A_PtrRtRc->bottom	=	Tv_PtrScpt->Rect.bottom;

											*A_PtrRtRelPos		=	Tv_PtrScpt->PosRel;
											*A_PtrRtLayer			=	m_PtrWinTbl[Tv_SlotIdx].Layer;

											Tv_Result	=	Tv_PtrScpt;
										}
									}
								}
							}
						}
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcAsmOpQuery(__u8 A_VmIdx, Ptr_GrP4AsmOpQuery A_PtrCode)
{
	// local -------------------
		__u8	Tv_ConIdx;
		__u8	Tv_Layer;
		__u8	Tv_PosRel;
		__u16	Tv_ScptId;
		Cls_GrP4DoQuery*	Tv_Win;
		Ptr_GrP4ScptObjHd	Tv_PtrScpt;
		St_GrRect	Tv_RcObj;
		WCHAR*	Tv_StrCont;
		WCHAR*	Tv_StrTitle;
	// code --------------------
		// init
		m_VmTbl[A_VmIdx].RegIntTbl[0]	=	0;
		// check VM slot full
		if ( 0 == A_VmIdx )
		{
			// get console index
			Tv_ConIdx	=	A_PtrCode->ConIdx;

			Tv_ScptId	=	A_PtrCode->WinScptId;

			Tv_PtrScpt	=	LcGetWinScpt( &Tv_ConIdx, Tv_ScptId, &Tv_RcObj, &Tv_PosRel, &Tv_Layer );
			if ( NULL != Tv_PtrScpt )
			{
				if ( E_GrP4AsmMaxTxtRegCnt > A_PtrCode->ConTxtReg )
				{
					// create window
					Tv_StrCont	=	m_VmTbl[A_VmIdx].RegTxtTbl[A_PtrCode->ConTxtReg];
					Tv_StrTitle	=	TextPtrGet( A_PtrCode->TitleTxtId );

					Tv_Win	=	(Cls_GrP4DoQuery*)new Cls_GrP4DoQuery( Tv_ScptId, 
						&Tv_RcObj, Tv_PosRel, Tv_Layer, (void*)m_ConTbl[Tv_ConIdx],
						Tv_StrCont, Tv_StrTitle, A_PtrCode->RefWinId );
					// decode other option
					LcUiDecSlave( (Cls_GrP4DoBase*)Tv_Win, Tv_PtrScpt, TRUE );
					// call init
					Tv_Win->RtlModalDoingSet( TRUE, A_VmIdx );
					Tv_Win->RtlInit();
					// request focus change
					m_ConTbl[Tv_ConIdx]->FocusReset();
					m_ConTbl[Tv_ConIdx]->FocusCorrect();
					// success
					m_VmTbl[A_VmIdx].Rstat	=	E_GrP4AsmRunStatSuspend;	// suspend mode
				}
				else
				{
					DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpQuery - invailid contents text register ( %d )!\n", A_PtrCode->ConTxtReg );
				}
			}
			else
			{
				DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpQuery - script ( %d ) not exist!\n", Tv_ScptId );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpQuery - Already modal mode in!\n" );
		}
		// increase ip
		m_VmTbl[A_VmIdx].Ip ++;

}
//--------------------------------------------------------------------
__u16	Cls_GrP4Mng::SoftCusImgIdGet( void )
{
	// local -------------------
		__u16	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrP4ImgIdNone;
		if ( NULL != m_PtrCfgPrj )
		{
			Tv_Result	=	m_PtrCfgPrj->CusImgId;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void*	Cls_GrP4Mng::GvuTblGet( void )
{
		return	(void*)m_GvuTbl;
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::SysQuit( void )
{
	// local -------------------
	// code --------------------

		
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcProcMsgFormat( Ptr_GrTaskMsgDiskFormat A_PtrMsg )
{
	// local -------------------
		St_GrP4MngVmRpara	Tv_Rpara;
	// code --------------------
		if ( E_GrP4AsmIdNone != m_PtrCfgMdl->AsmFormat )
		{
			Tv_Rpara.In.Cnt		=	0;

			Tv_Rpara.In.Ridx[Tv_Rpara.In.Cnt]	=	0;
			Tv_Rpara.In.Rval[Tv_Rpara.In.Cnt]	=	(__s32)A_PtrMsg->IsSuccess;
			Tv_Rpara.In.Cnt ++;

			Tv_Rpara.In.Ridx[Tv_Rpara.In.Cnt]	=	1;
			Tv_Rpara.In.Rval[Tv_Rpara.In.Cnt]	=	(__s32)A_PtrMsg->DiskIdx;
			Tv_Rpara.In.Cnt ++;

			Tv_Rpara.Out.Cnt	=	0;

			VmRunCode( m_PtrCfgMdl->AsmFormat, NULL, &Tv_Rpara );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::RtlVmFault(__u8 A_VmIdx)
{
	// local -------------------
	// code --------------------
		// not implemented
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::RtlCfgUpdate( void )
{
	// local -------------------
		St_GrTaskMsgHd	Tv_Msg;
		__s32		Tv_Rint[2];
		__u16*	Tv_PtrPriv;
		__u32	Tv_Ofs;
		__u8	Tv_WkCh;
	// code --------------------
		if (E_GrP4MngSysStatNone != m_SysStat )
		{
			// patch configuration
			VmRunCode( m_PtrCfgMdl->AsmCfgPatch, NULL );

			// authority update
			if ( m_PtrCfgMdl->UserIdCnt > m_Gvs.LoginIdx )
			{
				Tv_Ofs					=	E_GrP4CfgOfsUserAuth + ( (__u32)m_Gvs.LoginIdx << 1 );
				Tv_PtrPriv			=	(__u16*)( (__u32)m_SdaBuf + Tv_Ofs );
				m_Gvs.LoginPriv	=	*Tv_PtrPriv;
			}
			else
			{
				m_Gvs.LoginPriv	=	0;
			}

			// check change language
			if ( m_LangIdx != m_SdaBuf[E_GrP4CfgOfsLang] )
			{
				// update language
				LangSelSet( m_SdaBuf[E_GrP4CfgOfsLang] );
			}

			// update motion
			for ( Tv_WkCh=0;Tv_WkCh < m_Gvs.VdoMaxCh;Tv_WkCh++ )
			{
				Tv_Rint[1]	=	(__s32)Tv_WkCh;
				GrP4ApiCall(E_GrP4ApiCfgMtnMapUpdt, Tv_Rint, NULL, NULL);
			}

			// update key buzzer
			m_IsKeyBuzRun	=	(BOOL8)m_SdaBuf[E_GrP4CfgOfsBtnBeep];

			// update alpha
			Tv_Rint[1]	=	(__s32)m_SdaBuf[E_GrP4CfgOfsOsdAlpha];
			GrP4ApiCall(E_GrP4ApiCfgAlphaUpdt, Tv_Rint, NULL, NULL);

			// update mouse sensitivity
			LcMseSenUpdt();

			// update date format
			RtlDateDispFmtUpdt();

			// summer time update
			GrTimeSetDstTime( (BOOL8)m_SdaBuf[E_GrP4CfgOfsIsDst], 
				m_SdaBuf[E_GrP4CfgOfsDstStMon],m_SdaBuf[E_GrP4CfgOfsDstStWkTh],m_SdaBuf[E_GrP4CfgOfsDstStWkDay],m_SdaBuf[E_GrP4CfgOfsDstStHour],
				m_SdaBuf[E_GrP4CfgOfsDstEdMon],m_SdaBuf[E_GrP4CfgOfsDstEdWkTh],m_SdaBuf[E_GrP4CfgOfsDstEdWkDay],m_SdaBuf[E_GrP4CfgOfsDstEdHour] );

			// update covert
			LcCovertUpdt();

			// update PTZ
			LcPtzSetup();

			// controller setup
			LcSioSetup();

			// auto key lock
			LcAtLockSetup();

			// update REMOCON id
			m_Gvs.CrtRmtId	=	m_SdaBuf[E_GrP4CfgOfsRmtId];

			// notify setup change
			Tv_Msg.Cmd	=	E_GrTaskMsgSetupChg;
			GrTaskSvrMsgBroadCast( &Tv_Msg, sizeof(Tv_Msg) );
		}

}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::RtlPlayModeSet( BOOL8 A_IsPlayMode )
{
	// local -------------------
		St_GrP4MsgSysStat	Tv_Msg;
		__u8	Tv_QuadMode;
	// code --------------------
		// check change
		if ( m_Gvs.IsPlayMode	!= A_IsPlayMode )
		{
			// update play mode
			m_Gvs.IsPlayMode	=	A_IsPlayMode;
			if ( NULL != m_ConTbl[0] )
			{
				if ( A_IsPlayMode )
				{
					// update last live quad 
					m_ConTbl[0]->QuadModeGet( &m_ConMng[0].LastLiveQuadMode, &m_ConMng[0].LastLiveQuadPage );
					// update play quad
					Tv_QuadMode	=	E_GrDvrQuadMode4;
					if ( 16 <= m_Gvs.VdoMaxCh )
					{
						Tv_QuadMode	=	E_GrDvrQuadMode16;
					}
					else if ( 8 <= m_Gvs.VdoMaxCh )
					{
						Tv_QuadMode	=	E_GrDvrQuadMode9;
					}
					m_ConTbl[0]->QuadModeSet( Tv_QuadMode, 0 );
				}
				else
				{
					// restore live quad
					m_ConTbl[0]->QuadModeSet( m_ConMng[0].LastLiveQuadMode, m_ConMng[0].LastLiveQuadPage );
				}

				// update play mode
				// notify to console
				Tv_Msg.Hd.Cmd			=	E_GrP4MsgCmdSysStat;
				Tv_Msg.Hd.ConIdx	=	0;
				Tv_Msg.Hd.WinId		=	E_GrP4WinIdNone;
				Tv_Msg.Hd.ObjId		=	E_GrP4ScptIdAll;
				Tv_Msg.Type				=	E_GrP4SysStatLivePlay;
				m_ConTbl[0]->RtlUiMsgProc( (Ptr_GrP4MsgHd)&Tv_Msg );

				// notify to window
				Tv_Msg.Hd.WinId		=	E_GrP4WinIdAll;
				//m_ConTbl[0]->UiMsgPost( (Ptr_GrP4MsgHd)&Tv_Msg, sizeof(Tv_Msg) );
				m_ConTbl[0]->RtlUiMsgProc( (Ptr_GrP4MsgHd)&Tv_Msg );
			}

			// model event
			if ( E_GrP4AsmIdNone != m_PtrCfgMdl->AsmLivePlay )
			{
				VmRunCode( m_PtrCfgMdl->AsmLivePlay, NULL );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcProcMsgQuadChgReq( Ptr_GrTaskMsgQuadChgReq A_PtrMsg )
{
	// local -------------------
		St_GrP4MsgSysStat	Tv_Msg;
	// code --------------------

		Tv_Msg.Hd.Cmd			=	E_GrP4MsgCmdSysStat;
		Tv_Msg.Hd.ConIdx	=	A_PtrMsg->ConIdx;
		Tv_Msg.Hd.WinId		=	E_GrP4WinIdAll;
		Tv_Msg.Hd.ObjId		=	E_GrP4ScptIdAll;
		Tv_Msg.Type				=	E_GrP4SysStatQuadMode;

		if ( E_GrP4MngConMaxCnt > A_PtrMsg->ConIdx )
		{
			if ( NULL != m_ConTbl[A_PtrMsg->ConIdx] )
			{
				// notify slave windows
				m_ConTbl[A_PtrMsg->ConIdx]->RtlUiMsgProc( (Ptr_GrP4MsgHd)&Tv_Msg );
				// notify quad message to console
				Tv_Msg.Hd.WinId		=	E_GrP4WinIdNone;
				Tv_Msg.Hd.ObjId		=	E_GrP4ScptIdNone;
				m_ConTbl[A_PtrMsg->ConIdx]->RtlUiMsgProc( (Ptr_GrP4MsgHd)&Tv_Msg );
			}
		}

		
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcGvsReset( void )
{
	// local -------------------
	// code --------------------
		m_Gvs.VdoMaxCh	=	E_GrDvrMaxChCnt;
		m_Gvs.AdoMaxCh	=	E_GrDvrMaxChCnt;
		m_Gvs.DiChCnt		=	E_GrDvrMaxChCnt;
		m_Gvs.DoChCnt		=	E_GrDvrMaxChCnt;
		m_Gvs.LoginIdx	=	0;

}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::SysBuildEnv( void )
{
	// local -------------------
		__s32	Tv_Rint[2];
	// code --------------------
		GrP4ApiCall(E_GrP4ApiCfgLoadFromRom, Tv_Rint, NULL, NULL);
}
//--------------------------------------------------------------------
__u16	Cls_GrP4Mng::VdoRecQtyGet( __u8 A_ResType, __u8 A_ResLvl, __u8 A_QtyLvl )
{
	// local -------------------
		__u16	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		if ( NULL != m_PtrCfgMdl )
		{
			if ( (E_GrP4VencResTypeCnt > A_ResType) && (E_GrP4VencResSubCnt > A_ResLvl) && (E_GrP4VencQtyCnt > A_QtyLvl) )
			{
				Tv_Result	=	m_PtrCfgMdl->VrecQt[A_ResType][A_ResLvl][A_QtyLvl];
			}
			else
			{
				DbgMsgPrint( "Cls_GrP4Mng::VdoRecQtyGet - bad input value ( %d , %d , %d ).\n", A_ResType, A_ResLvl, A_QtyLvl );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrP4Mng::VdoNetQtyGet( __u8 A_ResType, __u8 A_ResLvl, __u8 A_QtyLvl )
{
	// local -------------------
		__u16	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		if ( NULL != m_PtrCfgMdl )
		{
			if ( (E_GrP4VencResTypeCnt > A_ResType) && (E_GrP4VencResSubCnt > A_ResLvl) && (E_GrP4VencQtyCnt > A_QtyLvl) )
			{
				Tv_Result	=	m_PtrCfgMdl->VnetQt[A_ResType][A_ResLvl][A_QtyLvl];
			}
			else
			{
				DbgMsgPrint( "Cls_GrP4Mng::VdoNetQtyGet - bad input value ( %d , %d , %d ).\n", A_ResType, A_ResLvl, A_QtyLvl );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4Mng::CtlgLoad( __u8 A_Id, __s32* A_PtrParaTbl, __u32* A_PtrRtNowCnt, __u32* A_PtrRtTotalCnt, __u32* A_PtrRtPageCnt )
{
	// local -------------------
		__s32	Tv_Rint[E_GrP4CtlgParaMax + 1];
		__u8	Tv_WkIdx;
	// code --------------------

		Tv_Rint[0]	=	NULL;
		// copy parameter value
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP4CtlgParaMax;Tv_WkIdx++ )
		{
			Tv_Rint[Tv_WkIdx + 1]	=	A_PtrParaTbl[Tv_WkIdx];
		}

		switch ( A_Id )
		{
			case E_GrP4CtlgIdFwUpgd:
				GrP4ApiCall(E_GrP4ApiCtlgFwUpgdLoad, Tv_Rint, NULL, NULL);
				break;
			case E_GrP4CtlgIdCfg:
				GrP4ApiCall(E_GrP4ApiCtlgCfgLoad, Tv_Rint, NULL, NULL);
				break;
			case E_GrP4CtlgIdLog:
				GrP4ApiCall(E_GrP4ApiCtlgLogLoad, Tv_Rint, NULL, NULL);
				break;
		}

		if ( FALSE == Tv_Rint[0] )
		{
			*A_PtrRtNowCnt		=	0;
			*A_PtrRtTotalCnt	=	0;
			*A_PtrRtPageCnt		=	0;
		}
		else
		{
			*A_PtrRtNowCnt		=	Tv_Rint[1];
			*A_PtrRtTotalCnt	=	Tv_Rint[2];
			*A_PtrRtPageCnt		=	Tv_Rint[3];
		}
		
		return	(BOOL8)Tv_Rint[0];		
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4Mng::CtlgPageNext( __u8 A_Id, __u32* A_PtrRtNowCnt, __u32* A_PtrRtTotalCnt, __u32* A_PtrRtPageCnt, __u32* A_PtrRtPagePos )
{
	// local -------------------
		__s32	Tv_Rint[E_GrP4CtlgParaMax + 1];
	// code --------------------
		Tv_Rint[0]	=	FALSE;

		switch ( A_Id )
		{
			case E_GrP4CtlgIdFwUpgd:
				GrP4ApiCall(E_GrP4ApiCtlgFwUpgdNext, Tv_Rint, NULL, NULL);
				break;
			case E_GrP4CtlgIdCfg:
				GrP4ApiCall(E_GrP4ApiCtlgCfgNext, Tv_Rint, NULL, NULL);
				break;
			case E_GrP4CtlgIdLog:
				GrP4ApiCall(E_GrP4ApiCtlgLogNext, Tv_Rint, NULL, NULL);
				break;
		}
		if ( FALSE != Tv_Rint[0] )
		{
			*A_PtrRtNowCnt		=	Tv_Rint[1];
			*A_PtrRtTotalCnt	=	Tv_Rint[2];
			*A_PtrRtPageCnt		=	Tv_Rint[3];
			*A_PtrRtPagePos		=	Tv_Rint[4];
		}

		return	(BOOL8)Tv_Rint[0];
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4Mng::CtlgPagePrev( __u8 A_Id, __u32* A_PtrRtNowCnt, __u32* A_PtrRtTotalCnt, __u32* A_PtrRtPageCnt, __u32* A_PtrRtPagePos )
{
	// local -------------------
		__s32	Tv_Rint[E_GrP4CtlgParaMax + 1];
	// code --------------------
		Tv_Rint[0]	=	FALSE;

		switch ( A_Id )
		{
			case E_GrP4CtlgIdFwUpgd:
				GrP4ApiCall(E_GrP4ApiCtlgFwUpgdPrev, Tv_Rint, NULL, NULL);
				break;
			case E_GrP4CtlgIdCfg:
				GrP4ApiCall(E_GrP4ApiCtlgCfgPrev, Tv_Rint, NULL, NULL);
				break;
			case E_GrP4CtlgIdLog:
				GrP4ApiCall(E_GrP4ApiCtlgLogPrev, Tv_Rint, NULL, NULL);
				break;
		}
		if ( FALSE != Tv_Rint[0] )
		{
			*A_PtrRtNowCnt		=	Tv_Rint[1];
			*A_PtrRtTotalCnt	=	Tv_Rint[2];
			*A_PtrRtPageCnt		=	Tv_Rint[3];
			*A_PtrRtPagePos		=	Tv_Rint[4];
		}

		return	(BOOL8)Tv_Rint[0];
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4Mng::CtlgItemValueGet( __u32 A_Hndl, __u8 A_ElemCode, __s32 A_AddVal, __s32* A_PtrRtVal )
{
	// local -------------------
		__s32	Tv_Rint[4];
		__u32	Tv_Id;
		__u32	Tv_ItmIdx;
	// code --------------------
		Tv_Rint[0]	=	FALSE;

		Tv_Id			=	(A_Hndl >> E_GrP4MngCtlgIdShft) & E_GrP4MngCtlgIdMask;
		Tv_ItmIdx	=	A_Hndl & E_GrP4MngCtlgValMask;

		Tv_Rint[1]	=	(__s32)Tv_ItmIdx;
		Tv_Rint[2]	=	(__s32)A_ElemCode;
		Tv_Rint[3]	=	A_AddVal;

		switch ( Tv_Id )
		{
			case E_GrP4CtlgIdFwUpgd:
				GrP4ApiCall(E_GrP4ApiCtlgFwUpgdItemValGet, Tv_Rint, NULL, NULL);
				break;
			case E_GrP4CtlgIdCfg:
				GrP4ApiCall(E_GrP4ApiCtlgCfgItemValGet, Tv_Rint, NULL, NULL);
				break;
			case E_GrP4CtlgIdLog:
				GrP4ApiCall(E_GrP4ApiCtlgLogItemValGet, Tv_Rint, NULL, NULL);
				break;
		}

		if ( FALSE != Tv_Rint[0] )
		{
			*A_PtrRtVal	=	Tv_Rint[1];
		}

		return	(BOOL8)Tv_Rint[0];
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4Mng::CtlgItemStrGet( __u32 A_Hndl, __u8 A_ElemCode, __s32 A_AddVal, WCHAR* A_StrRt )
{
	// local -------------------
		__s32	Tv_Rint[4];
		__u32	Tv_Id;
		__u32	Tv_ItmIdx;
		WCHAR*	Tv_Rtxt[2];
	// code --------------------
		Tv_Rint[0]	=	FALSE;

		Tv_Id			=	(A_Hndl >> 24) & 0xFF;
		Tv_ItmIdx	=	A_Hndl & 0xFFFFFF;

		Tv_Rint[1]	=	(__s32)Tv_ItmIdx;
		Tv_Rint[2]	=	(__s32)A_ElemCode;
		Tv_Rint[3]	=	(__s32)A_AddVal;

		Tv_Rtxt[0]	=	A_StrRt;

		switch ( Tv_Id )
		{
			case E_GrP4CtlgIdFwUpgd:
				GrP4ApiCall(E_GrP4ApiCtlgFwUpgdItemStrGet, Tv_Rint, Tv_Rtxt, NULL);
				break;
			case E_GrP4CtlgIdCfg:
				GrP4ApiCall(E_GrP4ApiCtlgCfgItemStrGet, Tv_Rint, Tv_Rtxt, NULL);
				break;
			case E_GrP4CtlgIdLog:
				GrP4ApiCall(E_GrP4ApiCtlgLogItemStrGet, Tv_Rint, Tv_Rtxt, NULL);
				break;
		}

		return	(BOOL8)Tv_Rint[0];
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::UiAlpahSet( __u8 A_Value )
{
	// local -------------------
		__u8	Tv_AlpVal;
		__u32	Tv_Tmp;
	// code --------------------
		if ( A_Value != m_UiAlphaVal )
		{
			// check exist script
			if ( NULL != m_PtrCfgPrj )
			{
				// check able color format
				if (E_GrFccRGB32 == m_PtrCfgPrj->FmtFcc)
				{
					// make alpha value
					Tv_Tmp	=	((__u32)A_Value * 254) / 100;
					if ( 0 == Tv_Tmp )
					{
						Tv_Tmp	=	1;
					}
					else if ( 254 <= Tv_Tmp )
					{
						Tv_Tmp	=	254;
					}
					Tv_AlpVal	=	(__u8)Tv_Tmp;

					// update image
					LcImgAlphaChgRgb32( Tv_AlpVal );

					// update paint code
					LcPstAlphaChgRgb32( Tv_AlpVal );

				}
			}

			// update
			m_UiAlphaVal	=	A_Value;
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcImgAlphaChgRgb32( __u8 A_AlphaVal )
{
	// local -------------------
		__u32	Tv_WkIdx;
		Ptr_GrGdib	Tv_PtrGdib;
	// code --------------------
		// normal image
		for(Tv_WkIdx=0;Tv_WkIdx < E_GrP4MngImgScptMaxCnt;Tv_WkIdx++ )
		{
			Tv_PtrGdib	=	m_PtrImgTbl[Tv_WkIdx];
			if ( NULL != Tv_PtrGdib )
			{
				GrGdibAlphaChgRbg32( Tv_PtrGdib, A_AlphaVal );
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcPstAlphaChgRgb32( __u8 A_AlphaVal )
{
	// local -------------------
		__u16	Tv_PstIdx;
		Ptr_GrP4ScptPstData	Tv_PtrPst;
		__u8	Tv_CodeCnt;
		__u8	Tv_CodeIdx;
		__u8	Tv_ConIdx;
	// code --------------------
		if ( 0 != m_PstCnt )
		{
			for ( Tv_PstIdx=0;Tv_PstIdx < m_PstCnt;Tv_PstIdx++ )
			{
				Tv_PtrPst	=	PstDataGet( E_GrP4ScptPstIdBase + Tv_PstIdx, &Tv_CodeCnt );
				if ( (NULL != Tv_PtrPst) && (0 != Tv_CodeCnt) )
				{
					for ( Tv_CodeIdx=0;Tv_CodeIdx < Tv_CodeCnt;Tv_CodeIdx++ )
					{
						if (	(E_GrP4ScptPstTypeClrFill == Tv_PtrPst->Type) || (E_GrP4ScptPstTypeLine == Tv_PtrPst->Type) ||
									(E_GrP4ScptPstTypeText == Tv_PtrPst->Type) || (E_GrP4ScptPstTypeFont == Tv_PtrPst->Type))
						{
							if ( 0xFF != ((Ptr_GrImgFmtRGB32)(&Tv_PtrPst->Value))->A )
							{
								((Ptr_GrImgFmtRGB32)(&Tv_PtrPst->Value))->A	=	A_AlphaVal;
							}
						}
						
						// next
						Tv_PtrPst ++;
					}
				}
			}
		}

		// console redraw
		for ( Tv_ConIdx=0;Tv_ConIdx < E_GrP4MngConMaxCnt;Tv_ConIdx++ )
		{
			if ( NULL != m_ConTbl[Tv_ConIdx] )
			{
				m_ConTbl[Tv_ConIdx]->PaintAllReq();
			}
		}
		
		
}
//--------------------------------------------------------------------
__u8	Cls_GrP4Mng::LongKeyTimeGet( void )
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		if ( NULL != m_PtrCfgMdl )
		{
			Tv_Result	=	m_PtrCfgMdl->LongKeyTime;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4Mng::IsAdvPassUse( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( NULL != m_PtrCfgMdl )
		{
			Tv_Result	=	m_PtrCfgMdl->IsPassChk;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrP4Mng::HddMaxCntGet( void )
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		if ( NULL != m_PtrCfgMdl )
		{
			Tv_Result	=	m_PtrCfgMdl->MaxHddCnt;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcMseSenUpdt( void )
{
	// local -------------------
	// code --------------------
		switch ( m_SdaBuf[E_GrP4CfgOfsMseRes] )
		{
			case 0:
				m_MseSenSl	=	0;
				m_MseSenSr	=	2;
				break;
			case 1:
				m_MseSenSl	=	0;
				m_MseSenSr	=	1;
				break;
			case 2:
				m_MseSenSl	=	0;
				m_MseSenSr	=	0;
				break;
			case 3:
				m_MseSenSl	=	1;
				m_MseSenSr	=	0;
				break;
			case 4:
				m_MseSenSl	=	2;
				m_MseSenSr	=	0;
				break;
			default:
				m_MseSenSl	=	0;
				m_MseSenSr	=	0;
				break;
		}
		
}
//--------------------------------------------------------------------
__u32	Cls_GrP4Mng::FirmwareVerGet( void )
{
	// local -------------------
		__u32	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		if ( NULL != m_PtrCfgPrj )
		{
			Tv_Result	=	m_PtrCfgPrj->FwVer;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrP4Mng::ModelNumGet( void )
{
	// local -------------------
		__u32	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		if ( NULL != m_PtrCfgMdl )
		{
			Tv_Result	=	m_PtrCfgMdl->ModelNum;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::RtlDateDispFmtUpdt( void )
{
	// local -------------------
		__u8	Tv_Type;
		St_GrP4MsgSysStat	Tv_Msg;
	// code --------------------
		Tv_Type	=	m_SdaBuf[E_GrP4CfgOfsDateFmt];

		if ( GrStrDateDispTypeGet() != Tv_Type )
		{
			// change date display type
			GrStrDateDispTypeSet( Tv_Type );
			// notify UI message
			Tv_Msg.Hd.Cmd			=	E_GrP4MsgCmdSysStat;
			Tv_Msg.Hd.ConIdx	=	E_GrP4ConIdxAll;
			Tv_Msg.Hd.WinId		=	E_GrP4WinIdAll;
			Tv_Msg.Hd.ObjId		=	E_GrP4ScptIdAll;
			Tv_Msg.Type				=	E_GrP4SysStatTimeFmtChg;
			UiMsgPost( (Ptr_GrP4MsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4Mng::NotifyBox(__u8 A_ConIdx, __u16 A_WinScptId, WCHAR* A_StrTitle, WCHAR* A_StrCont, __u8 A_Time, __u16 A_RefPosWin)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_ConIdx;
		__u16	Tv_SlotIdx;
		__u16	Tv_ScptId;
		Cls_GrP4DoNotify*	Tv_Win;
		Ptr_GrP4ScptObjHd	Tv_PtrScpt;
		St_GrRect	Tv_RcObj;
		Cls_GrP4ConBase*	Tv_ObjCon;
	// code --------------------
		Tv_Result	=	FALSE;
		Tv_ConIdx	=	A_ConIdx;
		if ( E_GrP4ConIdxFocus == Tv_ConIdx )
		{
			Tv_ConIdx	=	m_FcsConIdx;
		}
		// check console index over
		if ( E_GrP4MngConMaxCnt > Tv_ConIdx )
		{
			// check exist console
			if ( NULL != m_ConTbl[Tv_ConIdx] )
			{
				// init
				Tv_ObjCon	=	m_ConTbl[Tv_ConIdx];
				Tv_ScptId	=	A_WinScptId;
				
				if ( E_GrP4ScptIdWinBase <= Tv_ScptId )
				{
					Tv_SlotIdx	=	Tv_ScptId - E_GrP4ScptIdWinBase;
					if ( m_WinCnt > Tv_SlotIdx )
					{
						// get script
						if ( NULL != m_PtrWinTbl[Tv_SlotIdx].Pos )
						{
							// script decode
							Tv_PtrScpt	=	(Ptr_GrP4ScptObjHd)m_PtrWinTbl[Tv_SlotIdx].Pos;
							// check script head
							if ( E_GrP4ScptObjStx == Tv_PtrScpt->Stx )
							{
								// check first window class type
								if ( E_GrP4DoClassWin == Tv_PtrScpt->ClassId )
								{
									// check script id
									if ( Tv_ScptId == Tv_PtrScpt->ScptId )
									{
										// check already exist
										if ( NULL == Tv_ObjCon->WinFindByScptId( Tv_ScptId ) )
										{
											// create window
											Tv_RcObj.left		=	Tv_PtrScpt->Rect.left;
											Tv_RcObj.top		=	Tv_PtrScpt->Rect.top;
											Tv_RcObj.right	=	Tv_PtrScpt->Rect.right;
											Tv_RcObj.bottom	=	Tv_PtrScpt->Rect.bottom;

											Tv_Win	=	(Cls_GrP4DoNotify*)new Cls_GrP4DoNotify( Tv_ScptId, 
												&Tv_RcObj, Tv_PtrScpt->PosRel, m_PtrWinTbl[Tv_SlotIdx].Layer, (void*)Tv_ObjCon,
												A_StrCont, A_StrTitle, A_Time, A_RefPosWin );
											// decode other option
											LcUiDecSlave( (Cls_GrP4DoBase*)Tv_Win, Tv_PtrScpt, TRUE );
											// call init
											Tv_Win->RtlInit();
											// request focus change
											Tv_ObjCon->FocusReset();
											Tv_ObjCon->FocusCorrect();

											Tv_Result	=	TRUE;

										}
										else
										{
											DbgMsgPrint( "Cls_GrP4Mng::NotifyBox - window ( %d ) already exist!\n", Tv_ScptId );
										}
									}
									else
									{
										DbgMsgPrint( "Cls_GrP4Mng::NotifyBox - buffer script id  ( %d )  is mismatch orignal id ( %d ) !\n", Tv_PtrScpt->ScptId , Tv_ScptId );
									}
								}
								else
								{
									DbgMsgPrint( "Cls_GrP4Mng::NotifyBox - bad script ( %d )  - class type ( %d ) missmatch!\n" , Tv_ScptId, Tv_PtrScpt->ClassId );
								}
							}
							else
							{
								DbgMsgPrint( "Cls_GrP4Mng::NotifyBox - script ( %d ) error! : bad stx.\n" , Tv_ScptId );
							}
						}
						else
						{
							DbgMsgPrint( "Cls_GrP4Mng::NotifyBox - script ( %d ) not exist .\n" , Tv_ScptId );
						}
					}
					else
					{
						DbgMsgPrint( "Cls_GrP4Mng::NotifyBox - Window Id ( %d ) is out of range.\n" , Tv_ScptId );
					}
				}

			}
			else
			{
				DbgMsgPrint( "Cls_GrP4Mng::NotifyBox - console (%d) not exist.\n", Tv_ConIdx );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP4Mng::NotifyBox - bad console index(%d).\n", Tv_ConIdx );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4Mng::NotifyBox(__u8 A_ConIdx, __u16 A_WinScptId, __u16 A_TxtIdTitle, __u16 A_TxtIdCont, __u8 A_Time, __u16 A_RefPosWin)
{
	// local -------------------
		BOOL8	Tv_Result;
		WCHAR*	Tv_StrCont;
		WCHAR*	Tv_StrTitle;
	// code --------------------
		Tv_Result	=	FALSE;
		
		Tv_StrTitle	=	TextPtrGet( A_TxtIdTitle );
		Tv_StrCont	=	TextPtrGet( A_TxtIdCont );

		if ( NULL != Tv_StrCont )
		{
			Tv_Result	=	NotifyBox( A_ConIdx, A_WinScptId, Tv_StrTitle, Tv_StrCont, A_Time , A_RefPosWin);
		}
		else
		{
			DbgMsgPrint( "Cls_GrP4Mng::NotifyBox - bad contents text.\n" );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4Mng::RtlLogIn( __u8 A_Id, WCHAR* A_StrPass, WCHAR* A_StrDual )
{
	// local -------------------
		BOOL8	Tv_Result;
		WCHAR*	Tv_StrPass;
		__u32	Tv_Ofs;
		__u16*	Tv_PtrPriv;
	// code --------------------
		// init
		Tv_Result		=	FALSE;
		if ( m_IsScptAble )
		{
			// check password count
			if ( m_PtrCfgMdl->UserIdCnt > A_Id )
			{
				// check master password
				if ( 0 == GrStrWcmp( A_StrPass, m_PtrCfgMdl->MasterPass, TRUE ) )
				{
					Tv_Result		=	TRUE;
				}
				else
				{
					Tv_Ofs			=	E_GrP4CfgOfsUserPass + ( (__u32)A_Id << 5 );
					Tv_StrPass	=	(WCHAR*)( (__u32)m_SdaBuf + Tv_Ofs );
					if ( 0 == GrStrWcmp( A_StrPass, Tv_StrPass, TRUE ) )
					{
						Tv_Result	=	TRUE;
						// check dual password mode
						if ( m_PtrCfgMdl->IsPassDual )
						{
							Tv_Result	=	FALSE;
							// check second password
							Tv_StrPass	=	(WCHAR*)( (__u32)Tv_StrPass + 256 );
							if ( 0 == GrStrWcmp( A_StrPass, Tv_StrPass, TRUE ) )
							{
								Tv_Result	=	TRUE;
							}
						}
					}
				}
			}
		}

		if ( Tv_Result )
		{
			// update login info
			m_Gvs.LoginIdx	=	A_Id;
			Tv_Ofs					=	E_GrP4CfgOfsUserAuth + ( (__u32)A_Id << 1 );
			Tv_PtrPriv			=	(__u16*)( (__u32)m_SdaBuf + Tv_Ofs );
			m_Gvs.LoginPriv	=	*Tv_PtrPriv;

			// call event
			LcDpcCodeRun( m_PtrCfgMdl->AsmUserChg );
		}

		// update covert
		LcCovertUpdt();
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::RtlLogOut( void )
{
	// local -------------------
		//__s32		Tv_Val;
	// code --------------------
		// update login info
		m_Gvs.LoginIdx	=	0xFF;
		m_Gvs.LoginPriv	=	0;

		// debug
		//Tv_Val	=	LcAsmArgLoad( E_GrP4AsmArgTypeAtbGvs,  (__s32)(&((Ptr_GrP4Gvs)0)->LoginIdx) );
		//DbgMsgPrint( "DAMGI - login index value = %d \n", Tv_Val );

		// call event
		LcDpcCodeRun( m_PtrCfgMdl->AsmUserChg );

		// update covert
		LcCovertUpdt();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4Mng::RtlPasswdRuleChk( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		WCHAR*	Tv_StrPass;
		__u32	Tv_Ofs;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( m_IsScptAble )
		{
			if ( m_PtrCfgMdl->UserIdCnt > m_Gvs.LoginIdx )
			{
				Tv_Ofs			=	E_GrP4CfgOfsUserPass + ( (__u32)m_Gvs.LoginIdx << 5 );
				Tv_StrPass	=	(WCHAR*)( (__u32)m_SdaBuf + Tv_Ofs );
				if ( E_GrStrPasswdGood == GrStrRulePasswdWstr( Tv_StrPass ) )
				{
					Tv_Result	=	TRUE;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4Mng::IsEmgcLockAble( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( NULL != m_PtrCfgMdl )
		{
			Tv_Result	=	m_PtrCfgMdl->IsEmgcLock;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4Mng::IsRecEmgc( void )
{
		return	m_Gvs.IsEmgc;
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::RtlTryRecEmgc( BOOL8 A_IsOn )
{
	// local -------------------
		BOOL8	Tv_IsAble;
		St_GrP4MsgSysStat	Tv_Msg;
		__s32		Tv_Rint[5];
	// code --------------------
		Tv_IsAble	=	TRUE;
		if ( m_PtrCfgMdl->IsEmgcLock )
		{
			if ( (0 != m_SdaBuf[E_GrP4CfgOfsBtnEmgcLock]) && (A_IsOn) )
			{
				Tv_IsAble	=	FALSE;
			}
		}
		// check able
		if ( Tv_IsAble )
		{
			if ( A_IsOn != m_Gvs.IsEmgc )
			{
				m_Gvs.IsEmgc	=	A_IsOn;
				// notify change emergency
				Tv_Msg.Hd.Cmd			=	E_GrP4MsgCmdSysStat;
				Tv_Msg.Hd.ConIdx	=	E_GrP4ConIdxAll;
				Tv_Msg.Hd.WinId		=	E_GrP4WinIdAll;
				Tv_Msg.Hd.ObjId		=	E_GrP4ScptIdNone;
				Tv_Msg.Type				=	E_GrP4SysStatRecEmgcChg;
				UiMsgPost( (Ptr_GrP4MsgHd)&Tv_Msg, sizeof(Tv_Msg) );
				// save registry
				((Ptr_GrP4RgstSys)m_RgstBuf)->IsPanic	=	A_IsOn;
				GrP4ApiCall(E_GrP4ApiRgstSave, Tv_Rint, NULL, NULL);
				// log
				Tv_Rint[1]	=	E_GrDvrLogTypeEmgcEd;
				if ( A_IsOn )
				{
					Tv_Rint[1]	=	E_GrDvrLogTypeEmgcSt;
				}
				Tv_Rint[2]	=	0xFFFF;
				Tv_Rint[3]	=	0;
				Tv_Rint[4]	=	0;
				GrP4ApiCall(E_GrP4ApiEvtLogAdd, Tv_Rint, NULL, NULL);
			}
		}
		else
		{
			// event emergency denial
			if ( E_GrP4AsmIdNone != m_PtrCfgMdl->AsmDenEmgc )
			{
				VmRunCode( m_PtrCfgMdl->AsmDenEmgc, NULL, NULL );
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4Mng::VmReqEvtCall( __u16 A_CodeId, Cls_GrP4DoBase* A_ObjHost, Ptr_GrP4MngVmRpara A_PtrRpara )
{
	// local -------------------
		St_GrP4EvtCallMsg	Tv_Msg;
		Cls_GrP4DoWin*	Tv_Win;
		Cls_GrP4ConBase*	Tv_Con;
	// code --------------------
		Tv_Msg.CodeId		=	A_CodeId;

		// object
		Tv_Msg.ConIdx		=	0;
		Tv_Msg.WinId		=	E_GrP4WinIdNone;
		Tv_Msg.ScptId		=	E_GrP4ScptIdNone;
		if ( NULL != A_ObjHost )
		{
			Tv_Con	=	(Cls_GrP4ConBase*)A_ObjHost->ConsoleGet();
			if ( NULL != Tv_Con )
			{
				Tv_Msg.ConIdx	=	Tv_Con->IndexGet();
				// get window id
				Tv_Win	=	(Cls_GrP4DoWin*)A_ObjHost->WindowGet();
				Tv_Msg.WinId	=	Tv_Win->WinIdGet();
				if ( !A_ObjHost->IsWindow() )
				{
					Tv_Msg.ScptId	=	A_ObjHost->ScptIdGet();
				}
			}
		}

		// parameter
		if ( NULL == A_PtrRpara )
		{
			GrDumyZeroMem( &Tv_Msg.Para, sizeof(St_GrP4MngVmRpara) );
		}
		else
		{
			GrDumyCopyMem( &Tv_Msg.Para, A_PtrRpara, sizeof(St_GrP4MngVmRpara) );
		}

		return	(BOOL8)m_TaskSvr->MsgExPost( E_GrTaskMsgUiEvt, &Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcProcMsgEvtCall( Ptr_GrP4EvtCallMsg A_PtrMsg )
{
	// local -------------------
		Cls_GrP4DoWin*	Tv_Win;
		Cls_GrP4DoBase*	Tv_Obj;
	// code --------------------
		// find window
		Tv_Win		=	NULL;
		Tv_Obj		=	NULL;
		if ( E_GrP4WinIdNone != A_PtrMsg->WinId )
		{
			// find window
			Tv_Win	=	m_ConTbl[A_PtrMsg->ConIdx]->WinFindByWinId( A_PtrMsg->WinId );
			if ( E_GrP4ScptIdNone != A_PtrMsg->ScptId )
			{
				Tv_Obj	=	Tv_Win->FindObjByScptId( A_PtrMsg->ScptId );
			}
			else
			{
				Tv_Obj	=	(Cls_GrP4DoBase*)Tv_Win;
			}
		}
		VmRunCode( A_PtrMsg->CodeId, Tv_Obj, &A_PtrMsg->Para );
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcProcMsgFwUpgdDone( Ptr_GrTaskMsgFwUpgdEnd A_PtrMsg )
{
	// local -------------------
		
	// code --------------------
		// update error code
		m_Gvs.FwUpErr	=	A_PtrMsg->ErrCode;
		// do finish code
		if ( E_GrP4AsmIdNone != m_PtrCfgMdl->AsmFwUpDone )
		{
			VmRunCode( m_PtrCfgMdl->AsmFwUpDone, NULL, NULL );
		}

}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcAsmOpWait(__u8 A_VmIdx, Ptr_GrP4AsmOpWait A_PtrCode)
{
	// local -------------------
	// code --------------------
		m_VmTbl[A_VmIdx].WaitRamain	=	A_PtrCode->Time * 10;
		m_VmTbl[A_VmIdx].Rstat			=	E_GrP4AsmRunStatWait;
		m_VmTbl[A_VmIdx].Ip ++;
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::SafeQuadModeSet( __u8 A_ConIdx, __u8 A_Mode, __u8 A_Page, __u32 A_Fcc )
{
	// local -------------------	
		St_GrP4MngMsgQuadModeSet	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP4MngCmdQuadModeSet;
		Tv_Msg.ConIdx				=	A_ConIdx;
		Tv_Msg.Mode					=	A_Mode;
		Tv_Msg.Page					=	A_Page;
		Tv_Msg.Fcc					=	A_Fcc;

		m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::SafeQuadModeNext(  __u8 A_ConIdx, __u32 A_Fcc )
{
	// local -------------------
		St_GrP4MngMsgQuadMovePara	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP4MngCmdQuadModeNext;
		Tv_Msg.ConIdx				=	A_ConIdx;
		Tv_Msg.Fcc					=	A_Fcc;
		m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::SafeQuadModePrev(  __u8 A_ConIdx, __u32 A_Fcc )
{
	// local -------------------
		St_GrP4MngMsgQuadMovePara	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP4MngCmdQuadModePrev;
		Tv_Msg.ConIdx				=	A_ConIdx;
		Tv_Msg.Fcc					=	A_Fcc;
		m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::SafeQuadPageNext( __u8 A_ConIdx, __u32 A_Fcc )
{
	// local -------------------
		St_GrP4MngMsgQuadMovePara	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP4MngCmdQuadPageNext;
		Tv_Msg.ConIdx				=	A_ConIdx;
		Tv_Msg.Fcc					=	A_Fcc;
		m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4Mng::IsDispAtSeq( void )
{
		return	m_Gvs.IsAtSeq;
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::RtlDispAutoSeqSet( BOOL8 A_IsAtSeq )
{
		m_Gvs.IsAtSeq	=	A_IsAtSeq;
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::SafeQuadModeGet( __u8 A_ConIdx, __u8* A_PtrRtMode, __u8* A_PtrRtPage )
{
	// local -------------------
		St_GrP4MngMsgQuadModeGet	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP4MngCmdQuadModeGet;
		Tv_Msg.ConIdx				=	A_ConIdx;
		Tv_Msg.PtrRtMode		=	A_PtrRtMode;
		Tv_Msg.PtrRtPage		=	A_PtrRtPage;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcProcMsgNetOnOff( BOOL8 A_IsOn )
{
	// local -------------------
	// code --------------------
		m_Gvs.IsLanOn	=	A_IsOn;

		if ( E_GrP4AsmIdNone != m_PtrCfgMdl->AsmLanOnOff )
		{
			VmRunCode( m_PtrCfgMdl->AsmLanOnOff, NULL, NULL );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcAsmOpLabel(__u8 A_VmIdx, Ptr_GrP4AsmOpLabel A_PtrCode)
{
		// go next
		m_VmTbl[A_VmIdx].Ip ++;
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcAsmOpLblJump(__u8 A_VmIdx, Ptr_GrP4AsmOpLblJump A_PtrCode)
{
	// local -------------------
		__u16	Tv_CdIdx;
		BOOL8	Tv_IsFound;
		Ptr_GrP4AsmOpLabel	Tv_PtrOpLbl;
	// code --------------------
		// find label
		if ( 0 != m_VmCsLim )
		{
			Tv_IsFound	=	FALSE;
			for ( Tv_CdIdx=0;Tv_CdIdx < m_VmCsLim;Tv_CdIdx++ )
			{
				// check label
				if ( E_GrP4AsmOpLabel == m_PtrVmCs[Tv_CdIdx].Code )
				{
					Tv_PtrOpLbl	=	(Ptr_GrP4AsmOpLabel)&m_PtrVmCs[Tv_CdIdx];
					if ( ( A_PtrCode->Name0 == Tv_PtrOpLbl->Name0 ) && ( A_PtrCode->Name1 == Tv_PtrOpLbl->Name1 ) )
					{
						// found
						m_VmTbl[A_VmIdx].Ip	=	Tv_CdIdx;
						Tv_IsFound	=	TRUE;
						break;
					}
				}
			}

			// check not found
			if ( !Tv_IsFound )
			{
				m_VmTbl[A_VmIdx].Ip ++;
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcAsmOpReturn(__u8 A_VmIdx, Ptr_GrP4AsmOpBase A_PtrCode)
{
	// local -------------------
	// code --------------------
		// go last line
		m_VmTbl[A_VmIdx].Ip	=	m_VmCsLim;
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::KeyBlockSet( __u8 A_BlocKey )
{
		m_KeyBlock	=	A_BlocKey;
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcProcMsgPlayStatChg( void )
{
	// local -------------------
	// code --------------------
		if ( E_GrP4AsmIdNone != m_PtrCfgMdl->AsmPlayStat )
		{
			VmRunCode( m_PtrCfgMdl->AsmPlayStat, NULL, NULL );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcProcMsgUsbChg( void )
{
		if ( E_GrP4AsmIdNone != m_PtrCfgMdl->AsmUsbChg )
		{
			VmRunCode( m_PtrCfgMdl->AsmUsbChg, NULL, NULL );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcProcMsgCmsCliChg( void )
{
	// local -------------------
	// code --------------------
		if ( E_GrP4AsmIdNone != m_PtrCfgMdl->AsmCmsChg )
		{
			VmRunCode( m_PtrCfgMdl->AsmCmsChg, NULL, NULL );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcProcMsgBkupStart( void )
{
		if ( E_GrP4AsmIdNone != m_PtrCfgMdl->AsmBkupStart )
		{
			VmRunCode( m_PtrCfgMdl->AsmBkupStart, NULL, NULL );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcProcMsgBkupEnd( void )
{
		if ( E_GrP4AsmIdNone != m_PtrCfgMdl->AsmBkupEnd )
		{
			VmRunCode( m_PtrCfgMdl->AsmBkupEnd, NULL, NULL );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcProcMsgBkupErr( void )
{
		if ( E_GrP4AsmIdNone != m_PtrCfgMdl->AsmBkupErr )
		{
			VmRunCode( m_PtrCfgMdl->AsmBkupErr, NULL, NULL );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcProcMsgLogBkupStart( void )
{
		if ( E_GrP4AsmIdNone != m_PtrCfgMdl->AsmLogStart )
		{
			VmRunCode( m_PtrCfgMdl->AsmLogStart, NULL, NULL );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcProcMsgLogBkupEnd( void )
{
		if ( E_GrP4AsmIdNone != m_PtrCfgMdl->AsmLogEnd )
		{
			VmRunCode( m_PtrCfgMdl->AsmLogEnd, NULL, NULL );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcProcMsgLogBkupError( void )
{
		if ( E_GrP4AsmIdNone != m_PtrCfgMdl->AsmLogErr )
		{
			VmRunCode( m_PtrCfgMdl->AsmLogErr, NULL, NULL );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcVlossUpdt( void )
{
	// local -------------------
		St_GrP4MsgSysStat	Tv_Msg;
	// code --------------------
		if ( m_VlossStat != m_Gvs.VlossMap )
		{
			// notify video loss
			Tv_Msg.Hd.Cmd			=	E_GrP4MsgCmdSysStat;
			Tv_Msg.Hd.ConIdx	=	E_GrP4ConIdxAll;
			Tv_Msg.Hd.WinId		=	E_GrP4WinIdAll;
			Tv_Msg.Hd.ObjId		=	E_GrP4ScptIdAll;
			Tv_Msg.Type				=	E_GrP4SysStatVlossChg;
			UiMsgPost( (Ptr_GrP4MsgHd)&Tv_Msg, sizeof(Tv_Msg) );
			// update
			m_VlossStat	=	m_Gvs.VlossMap;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcCovertUpdt( void )
{
	// local -------------------
		__u16	Tv_CvtMap;
		__u16*	Tv_PtrWd;
	// code --------------------
		Tv_PtrWd	=	(__u16*)( (__u32)m_SdaBuf + E_GrP4CfgOfsCamCvtMap );
		Tv_CvtMap	=	*Tv_PtrWd;
		if ( m_PtrCfgMdl->IsCvtUsr )
		{
			if ( m_PtrCfgMdl->UserIdCnt > m_Gvs.LoginIdx )
			{
				Tv_PtrWd	=	(__u16*)( (__u32)m_SdaBuf + E_GrP4CfgOfsUserCovert );
				Tv_CvtMap	=	Tv_CvtMap | Tv_PtrWd[m_Gvs.LoginIdx];
			}
			else
			{
				Tv_CvtMap	=	0xFFFF;				 
			}
		}
		// update
		m_Gvs.CovertMap	=	Tv_CvtMap;
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::PtzMngSet( Cls_GrPtzV2Mng* A_ObjMng )
{
		m_PtzMng	=	A_ObjMng;
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcPtzSetup( void )
{
	// local -------------------
		__u8	Tv_WkIdx;
		__u32	Tv_Baud;
		__u8	Tv_DataBit;
		BOOL8	Tv_IsPanRvs;
		BOOL8	Tv_IsTiltRvs;
		__u32	Tv_TurIdx;
	// code --------------------
		if ( (NULL != m_PtzMng) && (0 != m_Gvs.VdoMaxCh) )
		{
			for ( Tv_WkIdx=0;Tv_WkIdx < m_Gvs.VdoMaxCh;Tv_WkIdx++ )
			{
				// get configuration
				Tv_Baud	=	9600;
				if ( E_GrP4CfgUartSpeedCnt > m_SdaBuf[E_GrP4CfgOfsPtzBaud + Tv_WkIdx] )
				{
					Tv_Baud	=	V_GrP4UartSpeedTbl[m_SdaBuf[E_GrP4CfgOfsPtzBaud + Tv_WkIdx]];
				}
				Tv_DataBit	=	8;
				if ( 0 != m_SdaBuf[E_GrP4CfgOfsPtzDataBit + Tv_WkIdx] )
				{
					Tv_DataBit	=	7;
				}
				Tv_IsPanRvs	=	FALSE;
				if ( 0 != (1 & m_SdaBuf[E_GrP4CfgOfsPtzAtb + Tv_WkIdx]) )
				{
					Tv_IsPanRvs	=	TRUE;
				}
				Tv_IsTiltRvs	=	FALSE;
				if ( 0 != (2 & m_SdaBuf[E_GrP4CfgOfsPtzAtb + Tv_WkIdx]) )
				{
					Tv_IsTiltRvs	=	TRUE;
				}
				m_PtzMng->SafeSetup( Tv_WkIdx, m_SdaBuf[E_GrP4CfgOfsPtzType + Tv_WkIdx],
					m_SdaBuf[E_GrP4CfgOfsPtzId + Tv_WkIdx], m_SdaBuf[E_GrP4CfgOfsPtzPort + Tv_WkIdx],
					Tv_Baud, Tv_DataBit, m_SdaBuf[E_GrP4CfgOfsPtzParity + Tv_WkIdx], 
					m_SdaBuf[E_GrP4CfgOfsPtzStopBit + Tv_WkIdx], 
					m_SdaBuf[E_GrP4CfgOfsPtzSpdPan + Tv_WkIdx], m_SdaBuf[E_GrP4CfgOfsPtzSpdTilt + Tv_WkIdx],
					m_SdaBuf[E_GrP4CfgOfsPtzSpdZoom + Tv_WkIdx], m_SdaBuf[E_GrP4CfgOfsPtzSpdFocus + Tv_WkIdx],
					Tv_IsPanRvs, Tv_IsTiltRvs, m_SdaBuf[E_GrP4CfgOfsPtzTourType + Tv_WkIdx]
					);
				// check DVR mode
				if ( 0 == m_SdaBuf[E_GrP4CfgOfsPtzTourType + Tv_WkIdx] )
				{
					// DVR tour mode
					Tv_TurIdx	=	E_GrP4CfgOfsPtzTourCfg + ((__u32)Tv_WkIdx * (12 * 4) );
					m_PtzMng->SafeTourSave( Tv_WkIdx, (Ptr_GrPtzV2TourItm)&m_SdaBuf[Tv_TurIdx], 12 );
				}
			}
		}
}
//--------------------------------------------------------------------
Cls_GrPtzV2Mng*	Cls_GrP4Mng::PtzMngGet( void )
{
		return	m_PtzMng;
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP4Mng::LcChToWstr(WCHAR* A_StrTg, __u8 A_Ch)
{
	// local -------------------
		WCHAR*	Tv_Result;
	// code --------------------
		Tv_Result		=	A_StrTg;

		GrStrIntToWstr(A_StrTg, (__s32)A_Ch + 1);
		Tv_Result		=	GrStrAddWchar(A_StrTg, 0x20);

		return	Tv_Result;
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP4Mng::LcUserToWstr( WCHAR* A_StrTg, __u8 A_UserIdx )
{
	// local -------------------
		WCHAR*	Tv_Result;
		WCHAR*	Tv_StrSr;
	// code --------------------
		Tv_Result		=	A_StrTg;
		if ( m_PtrCfgMdl->UserIdCnt > A_UserIdx )
		{
			Tv_StrSr	=	(WCHAR*)( (__u32)m_SdaBuf + E_GrP4CfgOfsUserId + ( ((__u32)A_UserIdx) << 5 ) );
			Tv_Result	=	GrStrWcopy( Tv_Result, Tv_StrSr, 15 );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP4Mng::LcIp4ToWstr( WCHAR* A_StrTg, __u32 A_Ip4 )
{
	// local -------------------
		WCHAR*	Tv_Result;
	// code --------------------
		Tv_Result		=	A_StrTg;

		GrStrIntToWstr( Tv_Result, (__s32)( A_Ip4 & 0xFF ) );
		Tv_Result		=	GrStrAddWchar( Tv_Result, 0x2E );
		GrStrIntToWstr( Tv_Result, (__s32)( (A_Ip4 >> 8) & 0xFF ) );
		Tv_Result		=	GrStrAddWchar( Tv_Result, 0x2E );
		GrStrIntToWstr( Tv_Result, (__s32)( (A_Ip4 >> 16) & 0xFF ) );
		Tv_Result		=	GrStrAddWchar( Tv_Result, 0x2E );
		GrStrIntToWstr( Tv_Result, (__s32)( (A_Ip4 >> 24) & 0xFF ) );
		Tv_Result		=	GrStrAddWchar( Tv_Result, 0x20 );

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4Mng::EvtLogToWstr( Ptr_GrDvrLogItm A_PtrLog, WCHAR* A_PtrRtStr )
{
	// local -------------------
		BOOL8	Tv_Result;
		WCHAR*	Tv_StrWk;
	// code --------------------
		Tv_Result	=	FALSE;

		GrStrClear( A_PtrRtStr );
		Tv_StrWk	=	A_PtrRtStr;

		// check exist script
		if ( m_IsScptAble )
		{
			Tv_Result	=	TRUE;
			switch ( A_PtrLog->Type )
			{
				case E_GrDvrLogTypeMotion:
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP4ScptTxtIdMotion ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					Tv_StrWk	=	LcChToWstr( Tv_StrWk, A_PtrLog->Ch );
					break;
				case E_GrDvrLogTypeSensor:
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP4ScptTxtIdSensor ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					Tv_StrWk	=	LcChToWstr( Tv_StrWk, A_PtrLog->Ch );
					break;
				case E_GrDvrLogTypeVdoLoss:
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP4ScptTxtIdVdoLoss ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					Tv_StrWk	=	LcChToWstr(Tv_StrWk, A_PtrLog->Ch);
					break;
				case E_GrDvrLogTypeVdoRcvy:
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP4ScptTxtIdVdoRcvy ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					Tv_StrWk	=	LcChToWstr(Tv_StrWk, A_PtrLog->Ch);
					break;
				case E_GrDvrLogTypePowerOn:
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP4ScptTxtIdPowerOn ) );
					break;
				case E_GrDvrLogTypePowerOff:
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP4ScptTxtIdPowerOff ) );
					break;
				case E_GrDvrLogTypeLogin:
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP4ScptTxtIdLogin ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					Tv_StrWk	=	LcUserToWstr( Tv_StrWk, (__u8)A_PtrLog->Para0 );
					break;
				case E_GrDvrLogTypeLogout:
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP4ScptTxtIdLogout ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					Tv_StrWk	=	LcUserToWstr( Tv_StrWk, (__u8)A_PtrLog->Para0 );
					break;
				case E_GrDvrLogTypeHddSysChg:
					Tv_StrWk	=	GrStrWcopy(Tv_StrWk, TextPtrGet(E_GrP4ScptTxtIdHddSysChg));
					Tv_StrWk	=	GrStrAddWchar(Tv_StrWk, 0x20);
					GrStrDwordToHexWstr(Tv_StrWk, A_PtrLog->Para2);
					Tv_StrWk	=	(WCHAR*)((__u32)Tv_StrWk + GrStrWlen(Tv_StrWk));
					GrStrDwordToHexWstr(Tv_StrWk, A_PtrLog->Para1,8,TRUE);
					break;
				case E_GrDvrLogTypeCfgRes:
					Tv_StrWk	=	GrStrWcopy(Tv_StrWk, TextPtrGet(E_GrP4ScptTxtIdSetupChg));
					Tv_StrWk	=	GrStrAddWchar(Tv_StrWk, 0x20);
					Tv_StrWk	=	GrStrWcopy(Tv_StrWk, TextPtrGet(E_GrP4ScptTxtIdResolution));
					Tv_StrWk	=	GrStrAddWchar(Tv_StrWk, 0x20);
					Tv_StrWk	=	LcChMapToWstr(Tv_StrWk, A_PtrLog->Para1);
					break;
				case E_GrDvrLogTypeCfgRecCh:
					Tv_StrWk	=	GrStrWcopy(Tv_StrWk, TextPtrGet(E_GrP4ScptTxtIdSetupChg));
					Tv_StrWk	=	GrStrAddWchar(Tv_StrWk, 0x20);
					Tv_StrWk	=	GrStrWcopy(Tv_StrWk, TextPtrGet(E_GrP4ScptTxtIdRecord));
					Tv_StrWk	=	GrStrAddWchar(Tv_StrWk, 0x20);
					Tv_StrWk	=	LcChMapToWstr(Tv_StrWk, A_PtrLog->Para1);
					break;
				case E_GrDvrLogTypeCfgRecQty:
					Tv_StrWk	=	GrStrWcopy(Tv_StrWk, TextPtrGet(E_GrP4ScptTxtIdSetupChg));
					Tv_StrWk	=	GrStrAddWchar(Tv_StrWk, 0x20);
					Tv_StrWk	=	GrStrWcopy(Tv_StrWk, TextPtrGet(E_GrP4ScptTxtIdRecQuility));
					Tv_StrWk	=	GrStrAddWchar(Tv_StrWk, 0x20);
					Tv_StrWk	=	LcChMapToWstr(Tv_StrWk, A_PtrLog->Para1);
					break;
				case E_GrDvrLogTypeCfgRecSpd:
					Tv_StrWk	=	GrStrWcopy(Tv_StrWk, TextPtrGet(E_GrP4ScptTxtIdSetupChg));
					Tv_StrWk	=	GrStrAddWchar(Tv_StrWk, 0x20);
					Tv_StrWk	=	GrStrWcopy(Tv_StrWk, TextPtrGet(E_GrP4ScptTxtIdRecSpeed));
					Tv_StrWk	=	GrStrAddWchar(Tv_StrWk, 0x20);
					Tv_StrWk	=	LcChMapToWstr(Tv_StrWk, A_PtrLog->Para1);
					break;
				case E_GrDvrLogTypeCfgRecPost:
					Tv_StrWk	=	GrStrWcopy(Tv_StrWk, TextPtrGet(E_GrP4ScptTxtIdSetupChg));
					Tv_StrWk	=	GrStrAddWchar(Tv_StrWk, 0x20);
					Tv_StrWk	=	GrStrWcopy(Tv_StrWk, TextPtrGet(E_GrP4ScptTxtIdRecPost));
					Tv_StrWk	=	GrStrAddWchar(Tv_StrWk, 0x20);
					Tv_StrWk	=	LcChMapToWstr(Tv_StrWk, A_PtrLog->Para1);
					break;
				case E_GrDvrLogTypeCfgMtnVal:
					Tv_StrWk	=	GrStrWcopy(Tv_StrWk, TextPtrGet(E_GrP4ScptTxtIdSetupChg));
					Tv_StrWk	=	GrStrAddWchar(Tv_StrWk, 0x20);
					Tv_StrWk	=	GrStrWcopy(Tv_StrWk, TextPtrGet(E_GrP4ScptTxtIdMotion));
					Tv_StrWk	=	GrStrAddWchar(Tv_StrWk, 0x20);
					Tv_StrWk	=	LcChMapToWstr(Tv_StrWk, (__u8)A_PtrLog->Para1);
					break;
				case E_GrDvrLogTypeCfgMtnMap:
					Tv_StrWk	=	GrStrWcopy(Tv_StrWk, TextPtrGet(E_GrP4ScptTxtIdSetupChg));
					Tv_StrWk	=	GrStrAddWchar(Tv_StrWk, 0x20);
					Tv_StrWk	=	GrStrWcopy(Tv_StrWk, TextPtrGet(E_GrP4ScptTxtIdMotionMap));
					Tv_StrWk	=	GrStrAddWchar(Tv_StrWk, 0x20);
					Tv_StrWk	=	LcChMapToWstr(Tv_StrWk, A_PtrLog->Para1);
					break;
				case E_GrDvrLogTypeCfgRecSchd:
					Tv_StrWk	=	GrStrWcopy(Tv_StrWk, TextPtrGet(E_GrP4ScptTxtIdSetupChg));
					Tv_StrWk	=	GrStrAddWchar(Tv_StrWk, 0x20);
					Tv_StrWk	=	GrStrWcopy(Tv_StrWk, TextPtrGet(E_GrP4ScptTxtIdRecSchd));
					Tv_StrWk	=	GrStrAddWchar(Tv_StrWk, 0x20);
					Tv_StrWk	=	LcChMapToWstr(Tv_StrWk, A_PtrLog->Para1);
					break;
				case E_GrDvrLogTypeCfgIdPass:
					Tv_StrWk	=	GrStrWcopy(Tv_StrWk, TextPtrGet(E_GrP4ScptTxtIdSetupChg));
					Tv_StrWk	=	GrStrAddWchar(Tv_StrWk, 0x20);
					Tv_StrWk	=	GrStrWcopy(Tv_StrWk, TextPtrGet(E_GrP4ScptTxtIdPassword));
					Tv_StrWk	=	GrStrAddWchar(Tv_StrWk, 0x20);
					Tv_StrWk	=	LcUserToWstr(Tv_StrWk, (__u8)A_PtrLog->Para1);
					break;
				case E_GrDvrLogTypeCfgTitle:
					Tv_StrWk	=	GrStrWcopy(Tv_StrWk, TextPtrGet(E_GrP4ScptTxtIdSetupChg));
					Tv_StrWk	=	GrStrAddWchar(Tv_StrWk, 0x20);
					Tv_StrWk	=	GrStrWcopy(Tv_StrWk, TextPtrGet(E_GrP4ScptTxtIdTitle));
					Tv_StrWk	=	GrStrAddWchar(Tv_StrWk, 0x20);
					Tv_StrWk	=	LcChMapToWstr(Tv_StrWk, (__u8)A_PtrLog->Para1);
					break;

				case E_GrDvrLogTypeBkupSt:
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP4ScptTxtIdBackup ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP4ScptTxtIdStart ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					Tv_StrWk	=	LcUserToWstr( Tv_StrWk, A_PtrLog->Para0 );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					GrStrTimeToWstr( Tv_StrWk, (Def_GrTime)A_PtrLog->Para1 );
					Tv_StrWk	=	GrStrAddWchar(Tv_StrWk, 0x20);
					Tv_StrWk	=	LcChMapToWstr(Tv_StrWk, A_PtrLog->Para2);
					break;
				case E_GrDvrLogTypeBkupEd:
					Tv_StrWk	=	GrStrWcopy(Tv_StrWk, TextPtrGet(E_GrP4ScptTxtIdBackup));
					Tv_StrWk	=	GrStrAddWchar(Tv_StrWk, 0x20);
					Tv_StrWk	=	GrStrWcopy(Tv_StrWk, TextPtrGet(E_GrP4ScptTxtIdEnd));
					Tv_StrWk	=	GrStrAddWchar(Tv_StrWk, 0x20);
					Tv_StrWk	=	LcUserToWstr(Tv_StrWk, A_PtrLog->Para0);
					Tv_StrWk	=	GrStrAddWchar(Tv_StrWk, 0x20);
					GrStrTimeToWstr(Tv_StrWk, (Def_GrTime)A_PtrLog->Para1);
					//Tv_StrWk	=	GrStrAddWchar(Tv_StrWk, 0x20);
					break;

				case E_GrDvrLogTypeHddErr:
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP4ScptTxtIdHddErr ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x42 );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					GrStrIntToWstr( Tv_StrWk, (__s32)A_PtrLog->Para0 );
					break;
				case E_GrDvrLogTypeSetupIn:
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP4ScptTxtIdSetupIn ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					Tv_StrWk	=	LcUserToWstr( Tv_StrWk, (__u8)A_PtrLog->Para0 );
					break;
				case E_GrDvrLogTypeSetupOut:
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP4ScptTxtIdSetupOut ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					Tv_StrWk	=	LcUserToWstr( Tv_StrWk, (__u8)A_PtrLog->Para0 );
					break;
				case E_GrDvrLogTypeHddFormat:
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP4ScptTxtIdHddFormat ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					GrStrIntToWstr( Tv_StrWk, (__s32)A_PtrLog->Para0 );
					break;
				case E_GrDvrLogTypeRamup:
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP4ScptTxtIdRamup ) );
					break;
				case E_GrDvrLogTypeFsUpgrade:
					Tv_StrWk	=	GrStrWcopy(Tv_StrWk, TextPtrGet(E_GrP4ScptTxtIdFileSystem));
					Tv_StrWk	=	GrStrAddWchar(Tv_StrWk, 0x20);
					Tv_StrWk	=	GrStrWcopy(Tv_StrWk, TextPtrGet(E_GrP4ScptTxtIdUpgrade));
					Tv_StrWk	=	GrStrAddWchar(Tv_StrWk, 0x20);
					Tv_StrWk	=	GrStrAddWchar(Tv_StrWk, 0x46);
					Tv_StrWk	=	GrStrAddWchar(Tv_StrWk, 0x20);
					GrStrIntToWstr(Tv_StrWk, (__s32)A_PtrLog->Dev);
					break;
				case E_GrDvrLogTypeFsFix:
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP4ScptTxtIdFileSystem ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP4ScptTxtIdChanged ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x46 );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					GrStrIntToWstr( Tv_StrWk, (__s32)A_PtrLog->Dev );
					break;
				case E_GrDvrLogTypeFwUpgd:
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP4ScptTxtIdFirmWare ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP4ScptTxtIdUpgrade ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					Tv_StrWk	=	LcUserToWstr(Tv_StrWk, A_PtrLog->Para0);
					Tv_StrWk	=	GrStrAddWchar(Tv_StrWk, 0x20);
					GrStrIntToWstr(Tv_StrWk, (__s32)A_PtrLog->Para1);
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					GrStrIntToWstr( Tv_StrWk, (__s32)A_PtrLog->Para2 );
					break;
				case E_GrDvrLogTypeNetLogin:
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP4ScptTxtIdNetwork ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP4ScptTxtIdConnect ) );
					Tv_StrWk	=	GrStrAddWchar(Tv_StrWk, 0x20);
					Tv_StrWk	=	LcUserToWstr(Tv_StrWk, A_PtrLog->Para0);
					Tv_StrWk	=	GrStrAddWchar(Tv_StrWk, 0x20);
					Tv_StrWk	=	LcIp4ToWstr( Tv_StrWk, A_PtrLog->Para1 );
					break;
				case E_GrDvrLogTypeNetDiscon:
					Tv_StrWk	=	GrStrWcopy(Tv_StrWk, TextPtrGet(E_GrP4ScptTxtIdNetwork));
					Tv_StrWk	=	GrStrAddWchar(Tv_StrWk, 0x20);
					Tv_StrWk	=	GrStrWcopy(Tv_StrWk, TextPtrGet(E_GrP4ScptTxtIdDisconnect));
					Tv_StrWk	=	GrStrAddWchar(Tv_StrWk, 0x20);
					Tv_StrWk	=	LcUserToWstr(Tv_StrWk, A_PtrLog->Para0);
					Tv_StrWk	=	GrStrAddWchar(Tv_StrWk, 0x20);
					Tv_StrWk	=	LcIp4ToWstr(Tv_StrWk, A_PtrLog->Para1);
					break;
				case E_GrDvrLogTypeNetSearch:
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP4ScptTxtIdNetwork ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP4ScptTxtIdSearch ) );
					Tv_StrWk	=	GrStrAddWchar(Tv_StrWk, 0x20);
					Tv_StrWk	=	LcUserToWstr(Tv_StrWk, A_PtrLog->Para0);
					Tv_StrWk	=	GrStrAddWchar(Tv_StrWk, 0x20);
					Tv_StrWk	=	LcIp4ToWstr(Tv_StrWk, A_PtrLog->Para1);
					Tv_StrWk	=	GrStrAddWchar(Tv_StrWk, 0x20);
					GrStrTimeToWstr(Tv_StrWk, (Def_GrTime)A_PtrLog->Para2);
					break;
				case E_GrDvrLogTypeNetSetup:
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP4ScptTxtIdNetwork ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP4ScptTxtIdSetupChg ) );
					Tv_StrWk	=	GrStrAddWchar(Tv_StrWk, 0x20);
					Tv_StrWk	=	LcUserToWstr(Tv_StrWk, A_PtrLog->Para0);
					Tv_StrWk	=	GrStrAddWchar(Tv_StrWk, 0x20);
					Tv_StrWk	=	LcIp4ToWstr(Tv_StrWk, A_PtrLog->Para1);
					break;
				case E_GrDvrLogTypeEmgcSt:
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP4ScptTxtIdEmergency ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP4ScptTxtIdStart ) );
					break;
				case E_GrDvrLogTypeEmgcEd:
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP4ScptTxtIdEmergency ) );
					Tv_StrWk	=	GrStrAddWchar( Tv_StrWk, 0x20 );
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, TextPtrGet( E_GrP4ScptTxtIdEnd ) );
					break;
				case E_GrDvrLogTypeRtcErr:
					Tv_StrWk	=	GrStrWcopy(Tv_StrWk, TextPtrGet(E_GrP4ScptTxtIdRtcErr));
					break;
				case E_GrDvrLogTypeCfgHdd:
					Tv_StrWk	=	GrStrWcopy(Tv_StrWk, TextPtrGet(E_GrP4ScptTxtIdSetupChg));
					Tv_StrWk	=	GrStrAddWchar(Tv_StrWk, 0x20);
					Tv_StrWk	=	GrStrWcopy(Tv_StrWk, TextPtrGet(E_GrP4ScptTxtIdHdd));
					break;
				default:
					Tv_Result	=	FALSE;
					break;
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::RtlProcObjRelease( Cls_GrP4DoBase* A_Obj )
{
	// local -------------------
	// code --------------------
		if ( A_Obj == m_VmHostWin )
		{
			m_VmHostWin	=	NULL;
			m_VmHostObj	=	NULL;
		}
		
		if ( A_Obj == m_VmHostObj )
		{
			m_VmHostObj	=	NULL;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcAsmOpDcall(__u8 A_VmIdx, Ptr_GrP4AsmOpDcall A_PtrCode)
{
	// local -------------------
	// code --------------------
		LcDpcCodeRun( A_PtrCode->CodeId );
		m_VmTbl[A_VmIdx].Ip ++;
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcAsmOpStpGet(__u8 A_VmIdx, Ptr_GrP4AsmOpStpGet A_PtrCode)
{
	// local -------------------
		__u32	Tv_Size;
		__u32	Tv_StpAdr;
		__u8	Tv_ValBuf[4];
		__u32	Tv_WkIdx;
	// code --------------------
		// check register index
		if ( (E_GrP4AsmMaxIntRegCnt > A_PtrCode->AdrReg) && (E_GrP4AsmMaxIntRegCnt > A_PtrCode->ValReg) )
		{
			// setup address value
			if ( E_GrP4AsmArgTypeSzDword >= A_PtrCode->Type )
			{
				Tv_Size			=	1 << A_PtrCode->Type;
				Tv_StpAdr		=	(__u32)m_VmTbl[A_VmIdx].RegIntTbl[A_PtrCode->AdrReg];
				if ( E_GrP4MngSdaSize >= (Tv_StpAdr + Tv_Size) )
				{
					*((__u32*)Tv_ValBuf)	=	0;
					for ( Tv_WkIdx=0;Tv_WkIdx < Tv_Size;Tv_WkIdx++ )
					{
						Tv_ValBuf[Tv_WkIdx]	=	m_SdaBuf[Tv_StpAdr + Tv_WkIdx];
					}
					m_VmTbl[A_VmIdx].RegIntTbl[A_PtrCode->ValReg]	=	*((__s32*)Tv_ValBuf);
				}
				else
				{
					DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpStpGet - ( %d :%d ) bad setup address.\n", m_VmTbl[A_VmIdx].Cs , m_VmTbl[A_VmIdx].Ip );
				}
			}
			else
			{
				DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpStpGet - ( %d :%d ) bad type value.\n", m_VmTbl[A_VmIdx].Cs , m_VmTbl[A_VmIdx].Ip );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpStpGet - ( %d :%d ) bad register index.\n", m_VmTbl[A_VmIdx].Cs , m_VmTbl[A_VmIdx].Ip );
		}
		m_VmTbl[A_VmIdx].Ip ++;
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcAsmOpStpPut(__u8 A_VmIdx, Ptr_GrP4AsmOpStpPut A_PtrCode)
{
	// local -------------------
		__u32	Tv_Size;
		__u32	Tv_StpAdr;
		__u8	Tv_ValBuf[4];
		__u32	Tv_WkIdx;
	// code --------------------
		// check register index
		if ( (E_GrP4AsmMaxIntRegCnt > A_PtrCode->AdrReg) && (E_GrP4AsmMaxIntRegCnt > A_PtrCode->ValReg) )
		{
			// setup address value
			if ( E_GrP4AsmArgTypeSzDword >= A_PtrCode->Type )
			{
				Tv_Size			=	1 << A_PtrCode->Type;
				Tv_StpAdr		=	(__u32)m_VmTbl[A_VmIdx].RegIntTbl[A_PtrCode->AdrReg];
				if ( E_GrP4MngSdaSize >= (Tv_StpAdr + Tv_Size) )
				{
					*((__s32*)Tv_ValBuf)	=	m_VmTbl[A_VmIdx].RegIntTbl[A_PtrCode->ValReg];
					for ( Tv_WkIdx=0;Tv_WkIdx < Tv_Size;Tv_WkIdx++ )
					{
						m_SdaBuf[Tv_StpAdr + Tv_WkIdx]	=	Tv_ValBuf[Tv_WkIdx];
					}
				}
				else
				{
					DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpStpPut - ( %d :%d ) bad setup address.\n", m_VmTbl[A_VmIdx].Cs , m_VmTbl[A_VmIdx].Ip );
				}
			}
			else
			{
				DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpStpPut - ( %d :%d ) bad type value.\n", m_VmTbl[A_VmIdx].Cs , m_VmTbl[A_VmIdx].Ip );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP4Mng::LcAsmOpStpPut - ( %d :%d ) bad register index.\n", m_VmTbl[A_VmIdx].Cs , m_VmTbl[A_VmIdx].Ip );
		}
		m_VmTbl[A_VmIdx].Ip ++;
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcDpcCodeRun( __u16 A_CodeId )
{
	// local -------------------
		St_GrP4MngMsgAsmDcall	Tv_Msg;
	// code --------------------
		if ( E_GrP4AsmIdNone != A_CodeId )
		{
			Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
			Tv_Msg.Hd.Cmd				=	E_GrP4MngCmdAsmDcall;
			Tv_Msg.CodeId				=	A_CodeId;
			m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4Mng::DpcUpiCall( __u8 A_ConIdx, __u16 A_WinScptId, __u16 A_ObjScptId, __u16 A_UpiCmd, 
	__s32 A_Para1, __s32 A_Para2, __s32 A_Para3, __s32 A_Para4, __s32 A_Para5, __s32 A_Para6 )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrP4MngMsgDpcUpi	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrP4MngConMaxCnt > A_ConIdx )
		{
			Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
			Tv_Msg.Hd.Cmd				=	E_GrP4MngCmdDpcUpi;
			Tv_Msg.ConIdx				=	A_ConIdx;
			Tv_Msg.WinScptId		=	A_WinScptId;
			Tv_Msg.ObjScptId		=	A_ObjScptId;
			Tv_Msg.UpiCmd				=	A_UpiCmd;
			Tv_Msg.Para[1]			=	A_Para1;
			Tv_Msg.Para[2]			=	A_Para2;
			Tv_Msg.Para[3]			=	A_Para3;
			Tv_Msg.Para[4]			=	A_Para4;
			Tv_Msg.Para[5]			=	A_Para5;
			Tv_Msg.Para[6]			=	A_Para6;
			Tv_Result	=	(BOOL8)m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcProcDpcUpi( Ptr_GrP4MngMsgDpcUpi A_PtrMsg )
{
	// local -------------------
		Cls_GrP4DoWin*	Tv_Win;
		Cls_GrP4DoBase*	Tv_Obj;
	// code --------------------
		// check console
		if ( NULL != m_ConTbl[A_PtrMsg->ConIdx] )
		{
			// find window
			Tv_Win	=	m_ConTbl[A_PtrMsg->ConIdx]->WinFindByScptId( A_PtrMsg->WinScptId );
			if ( NULL != Tv_Win )
			{
				// find object
				Tv_Obj	=	Tv_Win->FindObjByScptId( A_PtrMsg->ObjScptId );
				if ( NULL != Tv_Obj )
				{
					Tv_Obj->RtlCommand( A_PtrMsg->UpiCmd, A_PtrMsg->Para, NULL );
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcAtLockSetup( void )
{
	// local -------------------
	// code --------------------
		switch ( m_SdaBuf[E_GrP4CfgOfsBtnAtLock] )
		{
			case 1:
				m_IsAtLock			=	TRUE;
				m_AtLockPeriod	=	100;
				m_AtLockTimer		=	100;
				break;
			case 2:
				m_IsAtLock			=	TRUE;
				m_AtLockPeriod	=	300;
				m_AtLockTimer		=	300;
				break;
			case 3:
				m_IsAtLock			=	TRUE;
				m_AtLockPeriod	=	600;
				m_AtLockTimer		=	600;
				break;
			default:
				m_IsAtLock			=	FALSE;
				m_AtLockPeriod	=	0;
				m_AtLockTimer		=	0;
				break;
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::RtlAtLockProc( void )
{
	// local -------------------
	// code --------------------
		if ( m_IsAtLock )
		{
			if ( E_GrP4UserIdMax > m_Gvs.LoginIdx )
			{
				if ( 0 != m_AtLockTimer )
				{
					m_AtLockTimer --;
					if ( 0 == m_AtLockTimer )
					{
						// logout
						RtlLogOut();
						// reset timer
						m_AtLockTimer	=	m_AtLockPeriod;
					}
				}
			}
			else
			{
				m_AtLockTimer	=	m_AtLockPeriod;
			}
		}
				
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::ErrorSetOne( __u8 A_ErrCode, BOOL8 A_IsOn )
{
	// local -------------------
		St_GrP4MngMsgErrSetOne	Tv_Msg;
	// code --------------------
		if ( 32 > A_ErrCode )
		{
			Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
			Tv_Msg.Hd.Cmd				=	E_GrP4MngCmdErrSetSetOne;
			Tv_Msg.ErrCode			=	A_ErrCode;
			Tv_Msg.IsOn					=	A_IsOn;

			m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
		else
		{
			DbgMsgPrint( "Cls_GrP4Mng::ErrorSetOne - error code %d out of range \n", A_ErrCode );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::ErrorClearMap( __u32 A_Map )
{
	// local -------------------
		St_GrP4MngMsgErrClrMap	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP4MngCmdErrSetClrMap;
		Tv_Msg.Map					=	A_Map;

		m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcProcMsgErrSetOne( Ptr_GrP4MngMsgErrSetOne A_PtrMsg )
{
	// local -------------------
		BOOL8	Tv_IsEvt;
		__u32	Tv_Mask;
		__u32	Tv_Mili;
		BOOL8	Tv_IsSm;
	// code --------------------
		Tv_IsEvt	=	FALSE;
		Tv_Mask	=	1 << A_PtrMsg->ErrCode;
		if ( A_PtrMsg->IsOn )
		{
			if ( 0 == (Tv_Mask & m_Gvs.ErrMap) )
			{
				Tv_IsEvt	=	TRUE;
				m_Gvs.ErrMap	=	m_Gvs.ErrMap | Tv_Mask;
				// update last error time
				m_Gvs.LastErrTime[A_PtrMsg->ErrCode]	=	GrTimeGet( &Tv_Mili, &Tv_IsSm );
			}
		}
		else
		{
			if ( 0 != (Tv_Mask & m_Gvs.ErrMap) )
			{
				Tv_IsEvt	=	TRUE;
				m_Gvs.ErrMap	=	m_Gvs.ErrMap & ~Tv_Mask;
			}
		}
		// notify error occur
		if ( Tv_IsEvt )
		{
			if ( E_GrP4AsmIdNone != m_PtrCfgMdl->AsmErrOcu )
			{
				VmRunCode( m_PtrCfgMdl->AsmErrOcu, NULL );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcProcMsgErrClrMap( Ptr_GrP4MngMsgErrClrMap A_PtrMsg )
{
	// local -------------------
	// code --------------------
		m_Gvs.ErrMap	=	m_Gvs.ErrMap & ~A_PtrMsg->Map;
		// notify error occur
		if ( E_GrP4AsmIdNone != m_PtrCfgMdl->AsmErrOcu )
		{
			VmRunCode( m_PtrCfgMdl->AsmErrOcu, NULL );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::SioMngSet( Cls_GrP4SioMng* A_ObjMng )
{
		m_SioMng	=	A_ObjMng;
}
//--------------------------------------------------------------------
Cls_GrP4SioMng*	Cls_GrP4Mng::SioMngGet( void )
{
		return	m_SioMng;
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcSioSetup( void )
{
	// local -------------------
	// code --------------------
		if ( NULL != m_SioMng )
		{
			m_SioMng->SafeSetup( m_SdaBuf[E_GrP4CfgOfsCtrlModel], m_SdaBuf[E_GrP4CfgOfsCtrlUart], m_SdaBuf[E_GrP4CfgOfsCtrlId] );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::SetupSaveReq( void )
{
	// local -------------------
		St_GrP4MngMsgBase	Tv_Msg;
	// code --------------------
		Tv_Msg.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Cmd				=	E_GrP4MngCmdSetupSave;

		m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcProcMsgSetupSave( void )
{
	// local -------------------
		__s32		Tv_Rint[2];
	// code --------------------
		GrP4ApiCall(E_GrP4ApiCfgSaveToRom, Tv_Rint, NULL, NULL);
		RtlCfgUpdate();
}
//--------------------------------------------------------------------
__u32	Cls_GrP4Mng::UiFindIdAlloc( void )
{
		m_ObjFindId ++;
		if ( 0 == m_ObjFindId )
		{
			m_ObjFindId	=	1;
		}
		return	m_ObjFindId;
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::SysFwUpStart(__u16 A_CodeId)
{
	// local -------------------
		St_GrP4MngMsgAsmDcall	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP4MngCmdAsmDcall;
		Tv_Msg.CodeId				=	A_CodeId;
		m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
Ptr_GrP4ScptCfgPrj	Cls_GrP4Mng::ConfigPrjPtrGet( void )
{
		return	m_PtrCfgPrj;
}
//--------------------------------------------------------------------
Ptr_GrP4ScptCfgRtl	Cls_GrP4Mng::ConfigRtlPtrGet( void )
{
		return	m_PtrCfgRtl;
}
//--------------------------------------------------------------------
Ptr_GrP4ScptCfgMdl	Cls_GrP4Mng::ConfigMdlAtbGet( void )
{
		return	m_PtrCfgMdl;
}
//--------------------------------------------------------------------
void	Cls_GrP4Mng::LcVmProcTimer(void)
{
	// local -------------------
		__u8	Tv_VmIdx;
	// code --------------------
		// timer process
		for ( Tv_VmIdx = 0; Tv_VmIdx < E_GrP4MngVmCnt; Tv_VmIdx++)
		{
			if (E_GrP4AsmRunStatWait == m_VmTbl[Tv_VmIdx].Rstat)
			{
				m_VmTbl[Tv_VmIdx].WaitRamain --;
				if (0 == m_VmTbl[Tv_VmIdx].WaitRamain)
				{
					m_VmTbl[Tv_VmIdx].Rstat	=	E_GrP4AsmRunStatRun;
				}
			}
		}

		RtlVmPumpAll();

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4Mng::RtlVmWakeup(__u8 A_VmIdx, Ptr_GrP4MngVmRpara A_PtrRpara)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if (E_GrP4MngVmCnt > A_VmIdx)
		{
			if (E_GrP4AsmRunStatSuspend == m_VmTbl[A_VmIdx].Rstat)
			{
				// register patch
				LcVmRparaIn(A_VmIdx, A_PtrRpara);
				// do wakeup
				m_VmTbl[A_VmIdx].Rstat	=	E_GrP4AsmRunStatRun;
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;

}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP4Mng::LcChMapToWstr(WCHAR* A_StrTg, __u32 A_ChMap)
{
	// local -------------------
		WCHAR*	Tv_Result;
		BOOL8	Tv_IsAddCom;
		__u32	Tv_WkIdx;
		__u32	Tv_Mask;
	// code --------------------
		Tv_Result		=	A_StrTg;
		Tv_IsAddCom	=	FALSE;

		Tv_Mask	=	1;
		for (Tv_WkIdx=0; Tv_WkIdx < E_GrDvrMaxChCnt; Tv_WkIdx++)
		{
			if (0 != (Tv_Mask & A_ChMap))
			{
				if (Tv_IsAddCom)
				{
					Tv_Result[0]	=	0x2C;
					Tv_Result[1]	=	0;
					Tv_Result ++;
				}
				// digit 10
				if (10 <= (Tv_WkIdx + 1))
				{
					Tv_Result[0]	=	0x31;
					Tv_Result[1]	=	(WCHAR)(0x30 + (Tv_WkIdx + 1 - 10));
					Tv_Result[2]	=	0;
					Tv_Result ++;
					Tv_Result ++;
				}
				else
				{
					Tv_Result[0]	=	(WCHAR)(0x30 + (Tv_WkIdx + 1));
					Tv_Result[1]	=	0;
					Tv_Result ++;
				}
				// next
				Tv_IsAddCom	=	TRUE;
			}
			// next
			Tv_Mask	=	Tv_Mask << 1;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void*	Cls_GrP4Mng::UiScptDecThis(Cls_GrP4DoBase* A_Obj, void* A_PtrScpt)
{
	// local -------------------
		void*	Tv_Result;
		__u32	Tv_Prgs;
		__u8*	Tv_PtrScpt;
		Ptr_GrP4ScptObjHd		Tv_PtrStx;
		Cls_GrP4DoBase*	Tv_Obj;
#ifdef GR_P4_EDITOR
		BOOL8	Tv_IsParaUpdt;
		__u16	Tv_ScptId;
#endif
		// code --------------------
		Tv_Result	=	NULL;

		Tv_Prgs		=	0;
		Tv_PtrStx	=	(Ptr_GrP4ScptObjHd)A_PtrScpt;
#ifdef GR_P4_EDITOR
		Tv_IsParaUpdt	=	TRUE;
#endif
		// check stx
		if (E_GrP4ScptObjStx == Tv_PtrStx->Stx)
		{
			Tv_Obj		=	A_Obj;
#ifdef GR_P4_EDITOR
			Tv_ScptId	=	A_Obj->ScptIdGet();
			if ((E_GrP4ScptIdPageBase <= Tv_ScptId) && ((E_GrP4ScptIdPageBase + m_PageCnt) > Tv_ScptId))
			{
				Tv_IsParaUpdt	=	FALSE;
			}
#endif

			// update para
#ifdef GR_P4_EDITOR
			if (Tv_IsParaUpdt)
			{
				Tv_Obj->RtlScptParaUpdt(Tv_PtrStx);
			}
#else
			Tv_Obj->RtlScptParaUpdt(Tv_PtrStx);
#endif
			// go next
			Tv_Prgs			=	sizeof(St_GrP4ScptObjHd)+ (__u32)Tv_PtrStx->ParaSize;
			Tv_PtrScpt	=	(__u8*)((__u32)Tv_PtrStx + sizeof(St_GrP4ScptObjHd)+ (__u32)Tv_PtrStx->ParaSize);

			// update result
			Tv_Result	=	(void*)Tv_PtrScpt;
		}
		else
		{
			DbgMsgPrint("Cls_GrP4Mng::UiScptDecThis - script ( %d ) stx error!\n", A_Obj->ScptIdGet());
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrP4Mng::UiScptDecRemain(Cls_GrP4DoBase* A_Obj, void* A_PtrScpt)
{
	// local -------------------
		__u32	Tv_Result;
		__u32	Tv_Deced;			// local decode
		__u8*	Tv_PtrScpt;
		Ptr_GrP4ScptObjEnd	Tv_PtrEdx;
		Cls_GrP4DoBase*	Tv_Obj;
	// code --------------------
		Tv_Result	=	0;

		if (NULL != A_PtrScpt)
		{
			Tv_Obj			=	A_Obj;
			Tv_PtrScpt	=	(__u8*)A_PtrScpt;

			while (E_GrP4ScptObjStx == (*Tv_PtrScpt))
			{
				// slave decode
				Tv_Deced	=	LcUiDecSlave(Tv_Obj, Tv_PtrScpt, FALSE);
				if (0 == Tv_Deced)
				{
					// error quit
					break;
				}
				// go next
				Tv_Result		=	Tv_Result + Tv_Deced;
				Tv_PtrScpt	=	(__u8*)((__u32)Tv_PtrScpt + Tv_Deced);
			}
			// check edx
			Tv_PtrEdx	=	(Ptr_GrP4ScptObjEnd)Tv_PtrScpt;
			if (E_GrP4ScptObjEdx != Tv_PtrEdx->Edx)
			{
				DbgMsgPrint("Cls_GrP4Mng::UiScptDecRemain - script ( %d ) edx error!  \n", A_Obj->ScptIdGet());
			}
			// edx add
			Tv_Result	=	Tv_Result + sizeof(St_GrP4ScptObjEnd);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
