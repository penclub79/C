/*
	emulation iden 1200

*/

//====================================================================
//include

#include	<EmuLnx/GrElaIden1200.h>
#include	<GrDumyTool.h>
#include	<GrBufTool.h>
#include	<GrGdibTool.h>
#include	<GrFileCtrl.h>
#include <GrStrTool.h>
#include	<Dvr/GrDvrBase.h>

//====================================================================
//local const
#define E_GrElaIden1200MaxChCnt			16

//====================================================================
//local type


//====================================================================
//global var

Cls_GrElaIden1200*	V_GrElaIden1200Mng =	NULL;
IDEN_VI_CHN_ATTR_S	V_GrElaIden_ViChAttrTbl[E_GrDvrMaxChCnt];
IDEN_HENC_STREAM_S	V_GrElaIden_HencMainStrm;
IDEN_HENC_STREAM_S	V_GrElaIden_HencSubStrm;
St_GrElaIden1200HencItem	V_GrElaIden_HencMainChTbl[16];
St_GrElaIden1200HencItem	V_GrElaIden_HencSubChTbl[16];
__u32	V_GrElaIden_HencMainChIdx = 0;
__u32	V_GrElaIden_HencSubChIdx	= 0;

char	V_GrElaIden_HencVdoMain[0x8000]	=	"VIDEO-MAIN";
char	V_GrElaIden_HencVdoSub[0x8000]	=	"VIDEO-SUB";


//====================================================================
//functions

//--------------------------------------------------------------------
Cls_GrElaIden1200::Cls_GrElaIden1200( void ):
	Cls_GrTaskCli()
{
	// local -------------------
	// code --------------------
		GrDumyZeroMem(V_GrElaIden_ViChAttrTbl, sizeof(IDEN_VI_CHN_ATTR_S) * E_GrDvrMaxChCnt);
}
//--------------------------------------------------------------------
Cls_GrElaIden1200::~Cls_GrElaIden1200( void )
{
	// local -------------------
	// code --------------------
}
//--------------------------------------------------------------------
extern	"C"
{
//--------------------------------------------------------------------
IDEN_HANDLE IDEN_SYS_CreateHandle(void)
{
	// local -------------------
		IDEN_HANDLE	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;

		if ( NULL == V_GrElaIden1200Mng )
		{
			V_GrElaIden1200Mng	=	(Cls_GrElaIden1200*) new Cls_GrElaIden1200();
			// update result
			Tv_Result	=	(IDEN_HANDLE)V_GrElaIden1200Mng;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_SYS_DestroyHandle(const IDEN_HANDLE hIden)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			// release emulate manager
			delete	V_GrElaIden1200Mng;
			V_GrElaIden1200Mng	=	NULL;

			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_SYS_SetConf(IDEN_HANDLE hIden, const char *pVideoNtPalStr)
{
	// local -------------------
		int		Tv_Result;
		BOOL8	Tv_IsPal;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_IsPal	=	FALSE;
			if ( 0 == GrStrCmp( (PCHAR)pVideoNtPalStr , "ntsc", TRUE ) )
			{
				Tv_IsPal	=	FALSE;
			}
			else if ( 0 == GrStrCmp( (PCHAR)pVideoNtPalStr , "pal", TRUE ) )
			{
				Tv_IsPal	=	TRUE;
			}
			else
			{
				DbgMsgPrint( "IDEN_SYS_SetConf - bad signal type %s \n", pVideoNtPalStr );
			}

			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_SYS_Init(const IDEN_HANDLE hIden)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_SYS_Exit(const IDEN_HANDLE hIden)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_SYS_InitBufferPool(const IDEN_HANDLE hIden)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_SYS_InitPtsBase(const IDEN_HANDLE hIden, const __u64 u64Pts)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_SYS_GetConf(const IDEN_HANDLE hIden, IDEN_SYS_CONF_S *pstSysConf)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			pstSysConf->devChnCnt = E_GrElaIden1200MaxChCnt;
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_VI_DisableChn(const IDEN_HANDLE hIden, const __u8 u8Chn)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			if ( u8Chn < (E_GrElaIden1200DevEmuCnt * E_GrElaIden1200DevChCnt) )
			{
				Tv_Result	=	IDEN_SUCCESS;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_VI_SetChnAttr(const IDEN_HANDLE hIden, const __u16 u16ChnBit, const IDEN_VI_CHN_ATTR_S stViChnAttr[IDEN_MAX_VI_PATH])
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result = IDEN_SUCCESS;
		}

		return Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_VI_SetMultiChnAttr(const IDEN_HANDLE hIden, const __u16 u16ChnBit, const IDEN_VI_CHN_ATTR_S *pstViChnAttr)
{
	// local -------------------
		__u8	Tv_WkCnt;
		__u16	Tv_WkMsk;
		IDEN_VI_CHN_ATTR_S*	Tv_PtrViAChttr;
	// code --------------------
		Tv_PtrViAChttr = (IDEN_VI_CHN_ATTR_S*) pstViChnAttr;

		if ((NULL != V_GrElaIden1200Mng) && ((Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng))
		{
			for (Tv_WkCnt = 0;Tv_WkCnt < 16;Tv_WkCnt++)
			{
				if (NULL != Tv_PtrViAChttr)
				{
					Tv_WkMsk = (1 << Tv_WkCnt);

					if (0 != (Tv_WkMsk & u16ChnBit))
					{
						GrDumyCopyMem(&V_GrElaIden_ViChAttrTbl[Tv_WkCnt], Tv_PtrViAChttr, sizeof(IDEN_VI_CHN_ATTR_S));
					}
				}
				else
				{
					return IDEN_FAILURE;
				}
				//next
				Tv_PtrViAChttr++;
			}
		}

		return	IDEN_SUCCESS;
}
//--------------------------------------------------------------------
int IDEN_VI_EnableChn(const IDEN_HANDLE hIden, const __u8 u8Chn)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			if ( u8Chn < (E_GrElaIden1200DevEmuCnt * E_GrElaIden1200DevChCnt) )
			{
				Tv_Result	=	IDEN_SUCCESS;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_VI_PZ_SetHDAttr(const IDEN_HANDLE hIden, const IDEN_VI_PZ_ATTR_S *pstViPzHdAttr)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_VO_HD_Enable(const IDEN_HANDLE hIden)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_VO_HD_Disable(const IDEN_HANDLE hIden)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_VO_HD_SetAttr(const IDEN_HANDLE hIden, const IDEN_VO_HD_ATTR_S *pstVoHdAttr)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_VO_SPOT_Enable(const IDEN_HANDLE hIden)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_VO_SPOT_Disable(const IDEN_HANDLE hIden)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_VO_SPOT_SetAttr(const IDEN_HANDLE hIden, const IDEN_VO_SPOT_ATTR_S *pstVoSpotAttr)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_SYS_InitCpuCfg(const IDEN_HANDLE hIden)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_VO_HD_WIN_EnableChn(const IDEN_HANDLE hIden, const __u8 u8Chn)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_VO_HD_WIN_DisableChn(const IDEN_HANDLE hIden, const __u8 u8Chn)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_VO_HD_WIN_SetChnAttr(const IDEN_HANDLE hIden, const __u8 u8Chn, const IDEN_VO_HD_WIN_CHN_ATTR_S *pstVoHdWinChnAttr)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_VO_HD_WIN_MakeSplit(const IDEN_HANDLE hIden, const IDEN_VO_HD_SPLIT_SCR *pstVoHdWinSplitScr)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_VO_HD_WIN_Display(const IDEN_HANDLE hIden, int camera)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_VO_SPOT_WIN_EnableChn(const IDEN_HANDLE hIden, const __u8 u8Chn)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_VO_SPOT_WIN_DisableChn(const IDEN_HANDLE hIden, const __u8 u8Chn)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_VO_SPOT_WIN_SetChnAttr(const IDEN_HANDLE hIden, const __u8 u8Chn, const IDEN_VO_SPOT_WIN_CHN_ATTR_S *pstVoSpotWinChnAttr)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			if (u8Chn < (E_GrElaIden1200DevEmuCnt * E_GrElaIden1200DevChCnt))
			{
				Tv_Result	=	IDEN_SUCCESS;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_VO_SPOT_WIN_MakeSplit(const IDEN_HANDLE hIden, const IDEN_VO_SPOT_SPLIT_SCR *pstVoSpotWinSplitScr)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_VO_SPOT_WIN_Display(const IDEN_HANDLE hIden, int camera)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_AI_SetAttr(const IDEN_HANDLE hIden, const int devNum, const IDEN_AI_ATTR_S *pstAiAttr)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_AI_SetInputGain(const IDEN_HANDLE hIden, const __u8 u8Chn, const __u8 u8Gain)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_AI_SetMuxAttr(const IDEN_HANDLE hIden, const __u8 u8Chn, const IDEN_AI_MUX_ATTR_S *pstAiMuxAttr)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_AI_SetI2SChnAttr(const IDEN_HANDLE hIden, const int devNum, const __u8 u8Chn, const IDEN_AI_CHN_ATTR_S *pstAiChnAttr)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_AI_SetI2SPbChnAttr(const IDEN_HANDLE hIden, const IDEN_AI_PBCHN_ATTR_S *pstAiPbChnAttr)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_AI_SetMixRatioAttr(const IDEN_HANDLE hIden, const __u8 u8Chn, const IDEN_AI_MIXRATIO_ATTR_S *pstAiMixRatioAttr)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_AI_SetMixMode(const IDEN_HANDLE hIden, const IDEN_AI_MIX_MODE_E eAiMixMode)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_AO_SetRecAttr(const IDEN_HANDLE hIden, const IDEN_AO_REC_ATTR_S *pstAoRecAttr)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_AO_SetMonAttr(const IDEN_HANDLE hIden, const IDEN_AO_MON_ATTR_S *pstAoMonAttr)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_AO_SetOutputMonChn(const IDEN_HANDLE hIden, const __u8 u8LeftCh, const __u8 u8RightChn)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_AO_SetDacAttr(const IDEN_HANDLE hIden, const IDEN_AO_DAC_ATTR_S *pstAoDacAttr)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_AO_SetOutputDacChn(const IDEN_HANDLE hIden, const __u8 u8Chn)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_AO_EnableMuteMon(const IDEN_HANDLE hIden, const __u8 u8LRCh, const IDEN_ENABLE_E eEnable)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_AO_EnableMuteRec(const IDEN_HANDLE hIden, const __u8 u8Chn, const IDEN_ENABLE_E eEnable)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_AO_EnableMuteDac(const IDEN_HANDLE hIden, const IDEN_ENABLE_E eEnable)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_AO_EnableMuteMix(const IDEN_HANDLE hIden, const __u8 u8Chn, const IDEN_ENABLE_E eEnable)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_NOVID_GetCurStatus(const IDEN_HANDLE hIden, __u16 *pStatusHD, __u16 *pStatusSD)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		*pStatusHD	=	0;
		*pStatusSD	=	0;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_VFD_GetDetChnFormat(const IDEN_HANDLE hIden, const __u8 u8Chn, __u8 *pu8DetFormat)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		*pu8DetFormat	=	CAMERA_RESOL_1920x1080_30p;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_NOVID_EnableChn(const IDEN_HANDLE hIden, const __u8 u8Chn)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_NOVID_DisableChn(const IDEN_HANDLE hIden, const __u8 u8Chn)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_NOVID_SetChnAttr(const IDEN_HANDLE hIden, const __u8 u8Chn, const IDEN_EVT_NOVID_CHN_ATTR_S *pstEvtNovidChnAttr)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_NOVID_GetChnAttr(const IDEN_HANDLE hIden, const __u8 u8Chn, IDEN_EVT_NOVID_CHN_ATTR_S *pstEvtNovidChnAttr)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_NOVID_Start(const IDEN_HANDLE hIden)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_VFD_EnableChn(const IDEN_HANDLE hIden, const __u8 u8Chn)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_VFD_DisableChn(const IDEN_HANDLE hIden, const __u8 u8Chn)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_VFD_SetChnAttr(const IDEN_HANDLE hIden, const __u8 u8Chn, const IDEN_EVT_VFD_CHN_ATTR_S *pstEvtVfdChnAttr)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_VFD_GetChnAttr(const IDEN_HANDLE hIden, const __u8 u8Chn, IDEN_EVT_VFD_CHN_ATTR_S *pstEvtVfdChnAttr)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_VFD_Start(const IDEN_HANDLE hIden)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_VI_SetChnParam(const IDEN_HANDLE hIden, const __u8 u8Chn, const IDEN_VI_CHN_PARAM_S *pstViChnParam)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_VI_PZ_SetSDAttr(const IDEN_HANDLE hIden, const IDEN_VI_PZ_ATTR_S *pstViPzSdAttr)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_VO_HD_WIN_SetChnParam(const IDEN_HANDLE hIden, const __u8 u8Chn, const IDEN_VO_HD_WIN_CHN_PARAM_S *pstVoHdWinChnParam)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_SetAttr(const IDEN_HANDLE hIden, const IDEN_EVT_ATTR_S *pstEvtAttr)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_GetAttr(const IDEN_HANDLE hIden, IDEN_EVT_ATTR_S *pstEvtAttr)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_GetIntStatus(const IDEN_HANDLE hIden, IDEN_EVT_STAT_S *pstEvtStat)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_WaitEvent(const IDEN_HANDLE hIden, int timeOutMsec)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_HENC_SetAttr(const IDEN_HANDLE hIden, const IDEN_HENC_ATTR_S *pstHencAttr)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_HENC_Enable(const IDEN_HANDLE hIden)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_HENC_Disable(const IDEN_HANDLE hIden)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_HENC_EnableChn(const IDEN_HANDLE hIden, const __u8 u8Chn)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_HENC_DisableChn(const IDEN_HANDLE hIden, const __u8 u8Chn)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_HENC_SetChnAttr(const IDEN_HANDLE hIden, const __u8 u8Chn, const IDEN_HENC_CHN_ATTR_S *pstHencChnAttr)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_HENC_SetNovidEncStop(const IDEN_HANDLE hIden, const __u8 u8Chn, const IDEN_HENC_NOVID_ENC_STOP_E eNovidEncStop)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_HENC_SetChromFilter(const IDEN_HANDLE hIden, const __u8 u8Chn, const IDEN_HENC_CHROM_FILTER_S *pstChromFilter)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_HENC_SetTemplFilter(const IDEN_HANDLE hIden, const __u8 u8Chn, const IDEN_HENC_TEMPL_FILTER_S *pstTemplFilter)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_HENC_SetSpatlFilter(const IDEN_HANDLE hIden, const __u8 u8Chn, const IDEN_HENC_SPATL_FILTER_S *pstSpatlFilter)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_HENC_SetFrameReferenceMode(const IDEN_HANDLE hIden, const __u8 u8Chn, const IDEN_HENC_FRAME_REFERENCEMODE_S *pstFrameEncMode)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_HENC_SetEncrypt(const IDEN_HANDLE hIden, const __u8 u8Chn, const IDEN_HENC_ENCRYPT_S *pstEncrypt)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_HENC_SetVui(const IDEN_HANDLE hIden, const __u8 u8Chn, const IDEN_HENC_VUI_S *pstVui)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_HENC_SetRoiCfg(const IDEN_HANDLE hIden, const __u8 u8Chn, const IDEN_HENC_ROI_S *pstRoi)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_HENC_SetDeint(const IDEN_HANDLE hIden, const __u8 u8Chn, const IDEN_HENC_DEINT_S *pstDeint)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_HENC_SetWatermark(const IDEN_HANDLE hIden, const __u8 u8Chn, const IDEN_HENC_WATERMARK_S *pstWatermark)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_HENC_SetNonuniQpCfg(const IDEN_HANDLE hIden, const __u8 u8Chn, const IDEN_HENC_NONUNIQ_S *pstNonuniQ)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_HENC_SetMdStreamAttr(const IDEN_HANDLE hIden, const __u8 u8Chn, const IDEN_HENC_MDSTREAM_ATTR_S *pstMdStreamAttr)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_HENC_SetEncModeCfg(const IDEN_HANDLE hIden, const __u8 u8Chn, const IDEN_HENC_ENCMODE_S *pstEncMode)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_JENC_SetAttr(const IDEN_HANDLE hIden, const IDEN_JENC_ATTR_S *pstJencAttr)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_JENC_Enable(const IDEN_HANDLE hIden)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_JENC_Disable(const IDEN_HANDLE hIden)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_JENC_DisableChn(const IDEN_HANDLE hIden, const __u8 u8Chn)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_MD_EnableChn(const IDEN_HANDLE hIden, const __u8 u8Chn)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_MD_DisableChn(const IDEN_HANDLE hIden, const __u8 u8Chn)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_MD_SetChnAttr(const IDEN_HANDLE hIden, const __u8 u8Chn, const IDEN_EVT_MD_CHN_ATTR_S *pstEvtMdChnAttr)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_MD_GetChnAttr(const IDEN_HANDLE hIden, const __u8 u8Chn, IDEN_EVT_MD_CHN_ATTR_S *pstEvtMdChnAttr)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_MD_SetMaskCellChn(const IDEN_HANDLE hIden, const __u8 u8Chn, const IDEN_EVT_MASKCELL_S *pstEvtMaskCell)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_MD_SetMaskCellBitChn(const IDEN_HANDLE hIden, const __u8 u8Chn, const int size, const __u8 *pEvtMaskCellBit)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_MD_GetMaskCellChn(const IDEN_HANDLE hIden, const __u8 u8Chn, IDEN_EVT_MASKCELL_S *pstEvtMaskCell)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_MD_GetMaskCellBitChn(const IDEN_HANDLE hIden, const __u8 u8Chn, const int size, __u8 *pEvtMaskCellBit)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_MD_Start(const IDEN_HANDLE hIden)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_MD_Stop(const IDEN_HANDLE hIden)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_MD_GetDetChnData(const IDEN_HANDLE hIden, const __u8 u8Chn, IDEN_EVT_MASKCELL_S *pstEvtMaskCell)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_MD_GetDetBitChnData(const IDEN_HANDLE hIden, const __u8 u8Chn, const int size, __u8 *pEvtMaskCellBit)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_MD_GetDetChnAccData(const IDEN_HANDLE hIden, const __u8 u8Chn, IDEN_EVT_MASKCELL_S *pstEvtMaskCell)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_MD_GetDetBitChnAccData(const IDEN_HANDLE hIden, const __u8 u8Chn, const int size, __u8 *pEvtMaskCellBit)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_MD_Query(const IDEN_HANDLE hIden, IDEN_EVT_MD_STAT_S *pstEvtMdStat)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_RD_EnableChn(const IDEN_HANDLE hIden, const __u8 u8Chn)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_RD_DisableChn(const IDEN_HANDLE hIden, const __u8 u8Chn)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_RD_SetChnAttr(const IDEN_HANDLE hIden, const __u8 u8Chn, const IDEN_EVT_RD_CHN_ATTR_S *pstEvtRdChnAttr)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_RD_GetChnAttr(const IDEN_HANDLE hIden, const __u8 u8Chn, IDEN_EVT_RD_CHN_ATTR_S *pstEvtRdChnAttr)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_RD_SetMaskCellChn(const IDEN_HANDLE hIden, const __u8 u8Chn, const IDEN_EVT_MASKCELL_S *pstEvtMaskCell)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_RD_SetMaskCellBitChn(const IDEN_HANDLE hIden, const __u8 u8Chn, const int size, const __u8 *pEvtMaskCellBit)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_RD_GetMaskCellChn(const IDEN_HANDLE hIden, const __u8 u8Chn, IDEN_EVT_MASKCELL_S *pstEvtMaskCell)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_RD_GetMaskCellBitChn(const IDEN_HANDLE hIden, const __u8 u8Chn, const int size, __u8 *pEvtMaskCellBit)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_RD_Start(const IDEN_HANDLE hIden)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_RD_Stop(const IDEN_HANDLE hIden)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_RD_GetDetChnData(const IDEN_HANDLE hIden, const __u8 u8Chn, IDEN_EVT_MASKCELL_S *pstEvtMaskCell)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_RD_GetDetBitChnData(const IDEN_HANDLE hIden, const __u8 u8Chn, const int size, __u8 *pEvtMaskCellBit)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_RD_GetDetChnAccData(const IDEN_HANDLE hIden, const __u8 u8Chn, IDEN_EVT_MASKCELL_S *pstEvtMaskCell)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_RD_GetDetBitChnAccData(const IDEN_HANDLE hIden, const __u8 u8Chn, const int size, __u8 *pEvtMaskCellBit)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_RD_Query(const IDEN_HANDLE hIden, IDEN_EVT_RD_STAT_S *pstEvtRdStat)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_BD_EnableChn(const IDEN_HANDLE hIden, const __u8 u8Chn)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_BD_DisableChn(const IDEN_HANDLE hIden, const __u8 u8Chn)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_BD_SetChnAttr(const IDEN_HANDLE hIden, const __u8 u8Chn, const IDEN_EVT_BD_CHN_ATTR_S *pstEvtBdChnAttr)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_BD_GetChnAttr(const IDEN_HANDLE hIden, const __u8 u8Chn, IDEN_EVT_BD_CHN_ATTR_S *pstEvtBdChnAttr)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_BD_SetMaskCellChn(const IDEN_HANDLE hIden, const __u8 u8Chn, const IDEN_EVT_MASKCELL_S *pstEvtMaskCell)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_BD_SetMaskCellBitChn(const IDEN_HANDLE hIden, const __u8 u8Chn, const int size, const __u8 *pEvtMaskCellBit)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_BD_GetMaskCellChn(const IDEN_HANDLE hIden, const __u8 u8Chn, IDEN_EVT_MASKCELL_S *pstEvtMaskCell)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_BD_GetMaskCellBitChn(const IDEN_HANDLE hIden, const __u8 u8Chn, const int size, __u8 *pEvtMaskCellBit)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_BD_Start(const IDEN_HANDLE hIden)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_BD_Stop(const IDEN_HANDLE hIden)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_BD_GetDetChnData(const IDEN_HANDLE hIden, const __u8 u8Chn, IDEN_EVT_MASKCELL_S *pstEvtMaskCell)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_BD_GetDetBitChnData(const IDEN_HANDLE hIden, const __u8 u8Chn, const int size, __u8 *pEvtMaskCellBit)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_BD_Query(const IDEN_HANDLE hIden, IDEN_EVT_BD_STAT_S *pstEvtBdStat)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_ND_EnableChn(const IDEN_HANDLE hIden, const __u8 u8Chn)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_ND_DisableChn(const IDEN_HANDLE hIden, const __u8 u8Chn)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_ND_SetChnAttr(const IDEN_HANDLE hIden, const __u8 u8Chn, const IDEN_EVT_ND_CHN_ATTR_S *pstEvtNdChnAttr)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_ND_GetChnAttr(const IDEN_HANDLE hIden, const __u8 u8Chn, IDEN_EVT_ND_CHN_ATTR_S *pstEvtNdChnAttr)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_ND_Start(const IDEN_HANDLE hIden)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_ND_Stop(const IDEN_HANDLE hIden)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_ND_GetDetChnData(const IDEN_HANDLE hIden, const __u8 u8Chn, IDEN_EVT_MASKCELL_S *pstEvtMaskCell)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_ND_GetDetBitChnData(const IDEN_HANDLE hIden, const __u8 u8Chn, const int size, __u8 *pEvtMaskCellBit)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_ND_Query(const IDEN_HANDLE hIden, IDEN_EVT_ND_STAT_S *pstEvtNdStat)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_DFD_EnableChn(const IDEN_HANDLE hIden, const __u8 u8Chn)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_DFD_DisableChn(const IDEN_HANDLE hIden, const __u8 u8Chn)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_DFD_SetChnAttr(const IDEN_HANDLE hIden, const __u8 u8Chn, const IDEN_EVT_DFD_CHN_ATTR_S *pstEvtDfdChnAttr)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_DFD_GetChnAttr(const IDEN_HANDLE hIden, const __u8 u8Chn, IDEN_EVT_DFD_CHN_ATTR_S *pstEvtDfdChnAttr)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_DFD_Start(const IDEN_HANDLE hIden)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_DFD_Stop(const IDEN_HANDLE hIden)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_DFD_Query(const IDEN_HANDLE hIden, IDEN_EVT_DFD_STAT_S *pstEvtDfdStat)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_NOVID_Stop(const IDEN_HANDLE hIden)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_NOVID_Query(const IDEN_HANDLE hIden, IDEN_EVT_NOVID_STAT_S *pstEvtNovidStat)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_VFD_Stop(const IDEN_HANDLE hIden)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_VFD_Query(const IDEN_HANDLE hIden, IDEN_EVT_VFD_STAT_S *pstEvtVfdStat)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_BOX_SetAttr(const IDEN_HANDLE hIden, const IDEN_EVT_BOX_ATTR_S *pstEvtBoxAttr)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_BOX_GetAttr(const IDEN_HANDLE hIden, IDEN_EVT_BOX_ATTR_S *pstEvtBoxAttr)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_BOX_SetCellParam(const IDEN_HANDLE hIden, const __u8 u8Chn, const IDEN_EVT_BOX_CELL_PARAM_S *pstEvtBoxCellParam)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_BOX_Display(const IDEN_HANDLE hIden, const __u8 u8Chn, const IDEN_EVT_BOX_DISPLAY_ATTR_S *pstEvtBoxDisplayAttr)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_BOX_GetStatusDisplay(const IDEN_HANDLE hIden, const __u8 u8Chn, IDEN_EVT_BOX_DISPLAY_ATTR_S *pstEvtBoxDisplayAttr)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_AUDABS_EnableChn(const IDEN_HANDLE hIden, const __u8 u8Chn)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_AUDABS_DisableChn(const IDEN_HANDLE hIden, const __u8 u8Chn)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_AUDABS_SetChnAttr(const IDEN_HANDLE hIden, const __u8 u8Chn, const IDEN_EVT_AUDABS_CHN_ATTR_S *pstEvtAudAbsChnAttr)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_AUDABS_GetChnAttr(const IDEN_HANDLE hIden, const __u8 u8Chn, IDEN_EVT_AUDABS_CHN_ATTR_S *pstEvtAudAbsChnAttr)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_AUDABS_Start(const IDEN_HANDLE hIden)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_AUDABS_Stop(const IDEN_HANDLE hIden)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_AUDABS_Query(const IDEN_HANDLE hIden, IDEN_EVT_AUDABS_STAT_S *pstEvtAudAbsStat)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_AUDDIFF_EnableChn(const IDEN_HANDLE hIden, const __u8 u8Chn)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_AUDDIFF_DisableChn(const IDEN_HANDLE hIden, const __u8 u8Chn)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_AUDDIFF_SetChnAttr(const IDEN_HANDLE hIden, const __u8 u8Chn, const IDEN_EVT_AUDDIFF_CHN_ATTR_S *pstEvtAudDiffChnAttr)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_AUDDIFF_GetChnAttr(const IDEN_HANDLE hIden, const __u8 u8Chn, IDEN_EVT_AUDDIFF_CHN_ATTR_S *pstEvtAudDiffChnAttr)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_AUDDIFF_Start(const IDEN_HANDLE hIden)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_AUDDIFF_Stop(const IDEN_HANDLE hIden)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_AUDDIFF_Query(const IDEN_HANDLE hIden, IDEN_EVT_AUDDIFF_STAT_S *pstEvtAudDiffStat)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_NOAUD_SetAttr(const IDEN_HANDLE hIden, const IDEN_EVT_NOAUD_ATTR_S *pstEvtNoaudAttr)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_NOAUD_GetAttr(const IDEN_HANDLE hIden, IDEN_EVT_NOAUD_ATTR_S *pstEvtNoaudAttr)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_NOAUD_EnableChn(const IDEN_HANDLE hIden, const __u8 u8Chn)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_NOAUD_DisableChn(const IDEN_HANDLE hIden, const __u8 u8Chn)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_NOAUD_SetChnAttr(const IDEN_HANDLE hIden, const __u8 u8Chn, const IDEN_EVT_NOAUD_CHN_ATTR_S *pstEvtNoaudChnAttr)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_NOAUD_GetChnAttr(const IDEN_HANDLE hIden, const __u8 u8Chn, IDEN_EVT_NOAUD_CHN_ATTR_S *pstEvtNoaudChnAttr)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_NOAUD_Start(const IDEN_HANDLE hIden)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_NOAUD_Stop(const IDEN_HANDLE hIden)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_EVT_NOAUD_Query(const IDEN_HANDLE hIden, IDEN_EVT_NOAUD_STAT_S *pstEvtNoaudStat)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_VI_GetChnAttr(const IDEN_HANDLE hIden, const __u8 u8Chn, IDEN_VI_CHN_ATTR_S *pstViChnAttr)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			if ( u8Chn < (E_GrElaIden1200DevEmuCnt * E_GrElaIden1200DevChCnt) )
			{
				if ((0 != V_GrElaIden_ViChAttrTbl[u8Chn].cameraResolHeight) && (0 != V_GrElaIden_ViChAttrTbl[u8Chn].cameraResolWidth))
				GrDumyCopyMem(pstViChnAttr, &V_GrElaIden_ViChAttrTbl[u8Chn], sizeof(IDEN_VI_CHN_ATTR_S));
				Tv_Result	=	IDEN_SUCCESS;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_VO_HD_SetParam(const IDEN_HANDLE hIden, const IDEN_VO_HD_PARAM_S *pstVoHdParam)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			Tv_Result	=	IDEN_SUCCESS;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_VO_SPOT_WIN_SetChnParam(const IDEN_HANDLE hIden, const __u8 u8Chn, const IDEN_VO_SPOT_WIN_CHN_PARAM_S *pstVoSpotWinChnParam)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	IDEN_FAILURE;

		if ( (NULL != V_GrElaIden1200Mng) && ( (Cls_GrElaIden1200*)hIden == V_GrElaIden1200Mng ) )
		{
			if (u8Chn < (E_GrElaIden1200DevEmuCnt * E_GrElaIden1200DevChCnt))
			{
				Tv_Result	=	IDEN_SUCCESS;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int IDEN_HENC_WaitStream(const IDEN_HANDLE hIden, int timeOutMsec)
{
		return	1;
}
//--------------------------------------------------------------------
int IDEN_HENC_Query(const IDEN_HANDLE hIden, IDEN_HENC_STAT_S *pstHencStat)
{
		pstHencStat->leftPics		=	E_GrElaIden1200EncStrmCnt;
		return	IDEN_SUCCESS;
}
//--------------------------------------------------------------------
int IDEN_HENC_GetStream(const IDEN_HANDLE hIden, const IDEN_HENC_STAT_S *pstHencStat, IDEN_HENC_STREAM_S **ppstHencStream)
{
	// local -------------------
		__u32	Tv_WkCnt;
		__u32	Tv_TmMili;
		Def_GrTime	Tv_TmSec;
		BOOL8	Tv_IsSm;
	// code --------------------
		V_GrElaIden_HencMainStrm.picCount	=	pstHencStat->leftPics;

		Tv_WkCnt	=	0;
		Tv_TmSec	=	GrTimeGet(&Tv_TmMili, &Tv_IsSm);

		// update time
		for (Tv_WkCnt=0; Tv_WkCnt < 16;Tv_WkCnt++)
		{
			V_GrElaIden_HencMainChTbl[Tv_WkCnt].TmSec		=	Tv_TmSec;
			V_GrElaIden_HencMainChTbl[Tv_WkCnt].TmMili	=	Tv_TmMili;
		}

		Tv_WkCnt	=	0;
		while (E_GrElaIden1200EncStrmCnt > Tv_WkCnt)
		{
			// update 
			GrDumyCopyMem(&V_GrElaIden_HencMainStrm.stStreamInfo[Tv_WkCnt].stFrameHeader, &V_GrElaIden_HencMainChTbl[V_GrElaIden_HencMainChIdx].Head, 
				sizeof(VIDEO_FRAME_HEADER_S));
			// update other info
			V_GrElaIden_HencMainStrm.stStreamInfo[Tv_WkCnt].stFrameHeader.u.bits.sub_chid					=	V_GrElaIden_HencMainChIdx;
			V_GrElaIden_HencMainStrm.stStreamInfo[Tv_WkCnt].stFrameHeader.u.bits.framerate				=	15;
			V_GrElaIden_HencMainStrm.stStreamInfo[Tv_WkCnt].stFrameHeader.u.bits.input_framerate	=	30;
			V_GrElaIden_HencMainStrm.stStreamInfo[Tv_WkCnt].u32VirtAddr	=	(__u32)V_GrElaIden_HencVdoMain;
			V_GrElaIden_HencMainStrm.stStreamInfo[Tv_WkCnt].u32Len			=	12300;
			// update gop
			V_GrElaIden_HencMainStrm.stStreamInfo[Tv_WkCnt].stFrameHeader.u.bits.type							=	1;
			if (0 != V_GrElaIden_HencMainChTbl[V_GrElaIden_HencMainChIdx].GopPrgs)
			{
				V_GrElaIden_HencMainStrm.stStreamInfo[Tv_WkCnt].stFrameHeader.u.bits.type							=	0;
			}
			V_GrElaIden_HencMainChTbl[V_GrElaIden_HencMainChIdx].GopPrgs ++;
			if (10 <= V_GrElaIden_HencMainChTbl[V_GrElaIden_HencMainChIdx].GopPrgs)
			{
				V_GrElaIden_HencMainChTbl[V_GrElaIden_HencMainChIdx].GopPrgs	=	0;
			}
			// update time
			V_GrElaIden_HencMainStrm.stStreamInfo[Tv_WkCnt].stFrameHeader.u.bits.timestamp_sec	=	V_GrElaIden_HencMainChTbl[V_GrElaIden_HencMainChIdx].TmSec;
			V_GrElaIden_HencMainStrm.stStreamInfo[Tv_WkCnt].stFrameHeader.u.bits.timestamp_usec	=	V_GrElaIden_HencMainChTbl[V_GrElaIden_HencMainChIdx].TmMili << 10;
			V_GrElaIden_HencMainChTbl[V_GrElaIden_HencMainChIdx].TmMili	=	V_GrElaIden_HencMainChTbl[V_GrElaIden_HencMainChIdx].TmMili + 66;
			if (1000 <= V_GrElaIden_HencMainChTbl[V_GrElaIden_HencMainChIdx].TmMili)
			{
				V_GrElaIden_HencMainChTbl[V_GrElaIden_HencMainChIdx].TmMili	=	V_GrElaIden_HencMainChTbl[V_GrElaIden_HencMainChIdx].TmMili - 1000;
				V_GrElaIden_HencMainChTbl[V_GrElaIden_HencMainChIdx].TmSec ++;
			}
			// next
			V_GrElaIden_HencMainChIdx ++;
			if (16 <= V_GrElaIden_HencMainChIdx)
			{
				V_GrElaIden_HencMainChIdx	=	0;
			}
			Tv_WkCnt ++;
		}
		
		*ppstHencStream	=	&V_GrElaIden_HencMainStrm;
		return	IDEN_SUCCESS;
}
//--------------------------------------------------------------------
int IDEN_HENC_ReleaseStream(const IDEN_HANDLE hIden, IDEN_HENC_STREAM_S **ppstHencStream)
{
		return	IDEN_SUCCESS;
}
//--------------------------------------------------------------------
int IDEN_HENCSUB_Query(const IDEN_HANDLE hIden, IDEN_HENC_STAT_S *pstHencStat)
{
		pstHencStat->leftPics		=	E_GrElaIden1200EncStrmCnt;
		return	IDEN_SUCCESS;
}
//--------------------------------------------------------------------
int IDEN_HENCSUB_GetStream(const IDEN_HANDLE hIden, const IDEN_HENC_STAT_S *pstHencStat, IDEN_HENC_STREAM_S **ppstHencStream)
{
	// local -------------------
		__u32	Tv_WkCnt;
		__u32	Tv_TmMili;
		Def_GrTime	Tv_TmSec;
		BOOL8	Tv_IsSm;
	// code --------------------
		V_GrElaIden_HencSubStrm.picCount	=	pstHencStat->leftPics;

		Tv_WkCnt	=	0;
		Tv_TmSec	=	GrTimeGet(&Tv_TmMili, &Tv_IsSm);

		// update time
		for (Tv_WkCnt=0; Tv_WkCnt < 16; Tv_WkCnt++)
		{
			V_GrElaIden_HencSubChTbl[Tv_WkCnt].TmSec		=	Tv_TmSec;
			V_GrElaIden_HencSubChTbl[Tv_WkCnt].TmMili	=	Tv_TmMili;
		}

		while (E_GrElaIden1200EncStrmCnt > Tv_WkCnt)
		{
			// update 
			GrDumyCopyMem(&V_GrElaIden_HencSubStrm.stStreamInfo[Tv_WkCnt].stFrameHeader, &V_GrElaIden_HencSubChTbl[V_GrElaIden_HencSubChIdx].Head,
				sizeof(VIDEO_FRAME_HEADER_S));
			// update other info
			V_GrElaIden_HencSubStrm.stStreamInfo[Tv_WkCnt].stFrameHeader.u.bits.sub_chid					=	V_GrElaIden_HencSubChIdx;
			V_GrElaIden_HencSubStrm.stStreamInfo[Tv_WkCnt].stFrameHeader.u.bits.framerate				=	15;
			V_GrElaIden_HencSubStrm.stStreamInfo[Tv_WkCnt].stFrameHeader.u.bits.input_framerate	=	30;
			V_GrElaIden_HencSubStrm.stStreamInfo[Tv_WkCnt].u32VirtAddr	=	(__u32)V_GrElaIden_HencVdoSub;
			V_GrElaIden_HencSubStrm.stStreamInfo[Tv_WkCnt].u32Len			=	11200;
			// update gop
			V_GrElaIden_HencSubStrm.stStreamInfo[Tv_WkCnt].stFrameHeader.u.bits.type							=	1;
			if (0 != V_GrElaIden_HencSubChTbl[V_GrElaIden_HencSubChIdx].GopPrgs)
			{
				V_GrElaIden_HencSubStrm.stStreamInfo[Tv_WkCnt].stFrameHeader.u.bits.type							=	0;
			}
			V_GrElaIden_HencSubChTbl[V_GrElaIden_HencSubChIdx].GopPrgs ++;
			if (10 <= V_GrElaIden_HencSubChTbl[V_GrElaIden_HencSubChIdx].GopPrgs)
			{
				V_GrElaIden_HencSubChTbl[V_GrElaIden_HencSubChIdx].GopPrgs	=	0;
			}
			// update time
			V_GrElaIden_HencSubStrm.stStreamInfo[Tv_WkCnt].stFrameHeader.u.bits.timestamp_sec	=	V_GrElaIden_HencSubChTbl[V_GrElaIden_HencSubChIdx].TmSec;
			V_GrElaIden_HencSubStrm.stStreamInfo[Tv_WkCnt].stFrameHeader.u.bits.timestamp_usec	=	V_GrElaIden_HencSubChTbl[V_GrElaIden_HencSubChIdx].TmMili << 10;
			V_GrElaIden_HencSubChTbl[V_GrElaIden_HencSubChIdx].TmMili	=	V_GrElaIden_HencSubChTbl[V_GrElaIden_HencSubChIdx].TmMili + 66;
			if (1000 <= V_GrElaIden_HencSubChTbl[V_GrElaIden_HencSubChIdx].TmMili)
			{
				V_GrElaIden_HencSubChTbl[V_GrElaIden_HencSubChIdx].TmMili	=	V_GrElaIden_HencSubChTbl[V_GrElaIden_HencSubChIdx].TmMili - 1000;
				V_GrElaIden_HencSubChTbl[V_GrElaIden_HencSubChIdx].TmSec ++;
			}
			// next
			V_GrElaIden_HencSubChIdx ++;
			if (16 <= V_GrElaIden_HencSubChIdx)
			{
				V_GrElaIden_HencSubChIdx	=	0;
			}
			Tv_WkCnt ++;
		}

		*ppstHencStream	=	&V_GrElaIden_HencSubStrm;
		return	IDEN_SUCCESS;
}
//--------------------------------------------------------------------
int IDEN_HENCSUB_ReleaseStream(const IDEN_HANDLE hIden, IDEN_HENC_STREAM_S **ppstHencStream)
{
	return	IDEN_SUCCESS;
}
//--------------------------------------------------------------------
int IDEN_HENCSUB_WaitStream(const IDEN_HANDLE hIden, int timeOutMsec)
{
	return	1;
}
//--------------------------------------------------------------------
int IDEN_VO_HD_WIN_GetStatusDisplay(const IDEN_HANDLE hIden, IDEN_VO_HD_STATUS_DISPLAY_S *pstVoHdStatusDisplay)
{
	return	IDEN_SUCCESS;
}
//--------------------------------------------------------------------
int IDEN_VO_SPOT_WIN_GetStatusDisplay(const IDEN_HANDLE hIden, IDEN_VO_SPOT_STATUS_DISPLAY_S *pstVoSpotStatusDisplay)
{
	return	IDEN_SUCCESS;
}
//--------------------------------------------------------------------
int IDEN_HENC_StartRecvPic(const IDEN_HANDLE hIden, const __u8 u8Chn)
{
	return	IDEN_SUCCESS;
}
//--------------------------------------------------------------------
int IDEN_HENC_RequestNewGopStart(const IDEN_HANDLE hIden, const __u8 u8Chn)
{
	return	IDEN_SUCCESS;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
}	// extern C
//--------------------------------------------------------------------

