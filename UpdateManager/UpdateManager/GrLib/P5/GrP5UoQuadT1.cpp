/*
Platform 5 UI object Quad type 1

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoQuadT1.h>

#include <GrDebug.h>
#include <GrStrTool.h>
#include <GrDumyTool.h>
#include <P5/GrP5ConBase.h>
#include <P5/GrP5SkinBase.h>
#include <P5/GrP5Mng.h>
#include <P5/GrP5Key.h>
#include <Dvr/GrDvrBase.h>

#ifdef GR_P5_EDITOR

#include <P5\Dvl\GrP5DvlBase.h>

#endif

//--------------------------------------------------------------------
// local constant

//--------------------------------------------------------------------
// local type

//--------------------------------------------------------------------
//local function

//--------------------------------------------------------------------
//global var

extern	St_GrP5Env	V_GrP5Env;


#ifdef GR_P5_EDITOR

#define	E_GrP5UoQuadT1ParaInfoCnt							10

St_GrP5DvlScptParaInfo	V_GrP5UoQuadT1ParaInfo[E_GrP5UoQuadT1ParaInfoCnt]	=
{
	{ L"Quad Type1", E_GrP5DvlValTypeSeparator, 0, NULL, FALSE, 0, NULL },
	{ L"NameFontId", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoQuadT1Para)0)->NameFontId, TRUE, 0, L"name text font id" },
	{ L"RecDoImg", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoQuadT1Para)0)->RecDoImg, TRUE, 0, L"do record image id" },
	{L"RmtnImg", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoQuadT1Para)0)->RmtnImg, TRUE, 0, L"do record motion image id"},
	{L"RsenImg", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoQuadT1Para)0)->RsenImg, TRUE, 0, L"do record sensor image id"},
	{ L"MtnImg", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoQuadT1Para)0)->MtnImg, TRUE, 0, L"motion detection image id"},
	{ L"SenImg", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoQuadT1Para)0)->SenImg, TRUE, 0, L"sensor detection image id"},
	{ L"PtzImg", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoQuadT1Para)0)->PtzImg, TRUE, 0, L"PTZ equipment image id" },
	{ L"AdoImg", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoQuadT1Para)0)->AdoImg, TRUE, 0, L"MIC equipment image id" },
	{L"IaImg", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoQuadT1Para)0)->IaImg, TRUE, 0, L"image analysis image id"},
};

#endif


//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoQuadT1::Cls_GrP5UoQuadT1(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoQuadBase(A_ScptId, A_ObjPrnt, A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdQuadT1, &m_ParaUoQuadT1, sizeof(m_ParaUoQuadT1), &m_SkinCtlQuadT1);
		
		// default setting
		GrDumyZeroMem(&m_ParaUoQuadT1, sizeof(m_ParaUoQuadT1));
		GrDumyZeroMem(&m_SkinCtlQuadT1, sizeof(m_SkinCtlQuadT1));
#ifdef GR_P5_EDITOR
		GrP5SkinDvlFontColorSet(&m_ParaUoQuadT1.NameTpara);
#endif
		m_ParaUoQuadT1.NameTpara.Atb				=	E_GrP5FontAtbDrawText;

		// reset
		m_SkinCtlQuadT1.IsShowName		=	TRUE;
		m_SkinCtlQuadT1.IsShowRec			=	TRUE;
		m_SkinCtlQuadT1.IsShowEvt			=	TRUE;
		m_SkinCtlQuadT1.IsShowPtz			=	TRUE;
		m_SkinCtlQuadT1.IsShowAudio		=	TRUE;
		m_SkinCtlQuadT1.IsRecEvtShow	=	FALSE;
		
		m_SkinCtlQuadT1.RecChMap			=	0;
		m_SkinCtlQuadT1.MtnChMap			=	0;
		m_SkinCtlQuadT1.SenChMap			=	0;
		m_SkinCtlQuadT1.AdoChMap			=	0;
		m_SkinCtlQuadT1.PtzChMap			=	0;
		m_SkinCtlQuadT1.IaChMap				=	0;

		GrDumyZeroMem(&m_DreqPrv, sizeof(m_DreqPrv));

		GrP5TimerRegist(this);
}
//--------------------------------------------------------------------
Cls_GrP5UoQuadT1::~Cls_GrP5UoQuadT1(void)
{
	// local -------------------
	// code --------------------
		

}
//--------------------------------------------------------------------
void	Cls_GrP5UoQuadT1::EvtScptDecDone(void)
{
		// inherited
		Cls_GrP5UoQuadBase::EvtScptDecDone();
}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoQuadT1::DvlParaInfoBuild(void)
{
	// local -------------------
		//St_GrP5DvlScptParaInfo	Tv_ParaInfo;
	// code --------------------
		// inherited
		Cls_GrP5UoQuadBase::DvlParaInfoBuild();

		DvlParaInfoAdd(V_GrP5UoQuadT1ParaInfo, E_GrP5UoQuadT1ParaInfoCnt, &m_ParaUoQuadT1);
		DvlParaInfoTxtParaAdd(L"NameTpara", &m_ParaUoQuadT1.NameTpara);
		DvlParaInfoRelPosAdd(L"NameRc", &m_ParaUoQuadT1.NameRc);
		DvlParaInfoRelPosAdd(L"RecDoRc", &m_ParaUoQuadT1.RecDoRc);
		DvlParaInfoRelPosAdd(L"MtnRc", &m_ParaUoQuadT1.MtnRc);
		DvlParaInfoRelPosAdd(L"SenRc", &m_ParaUoQuadT1.SenRc);
		DvlParaInfoRelPosAdd(L"PtzRc", &m_ParaUoQuadT1.PtzRc);
		DvlParaInfoRelPosAdd(L"AdoRc", &m_ParaUoQuadT1.AdoRc);
		DvlParaInfoRelPosAdd(L"IaRc", &m_ParaUoQuadT1.IaRc);

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoQuadT1::DvlParaInfoAction(__u32 A_Action)
{
		// inherited
		Cls_GrP5UoQuadBase::DvlParaInfoAction(A_Action);

		// custom action

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoQuadT1::DvlPreDelete(void)
{
	// local -------------------
	// code --------------------
		// delete prepare
		

		// inherited
		Cls_GrP5UoQuadBase::DvlPreDelete();
}
#endif
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoQuadT1::IsShowName(void)
{
	return	m_SkinCtlQuadT1.IsShowName;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoQuadT1::IsShowIconRec(void)
{
	return	m_SkinCtlQuadT1.IsShowRec;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoQuadT1::IsShowIconEvt(void)
{
	return	m_SkinCtlQuadT1.IsShowEvt;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoQuadT1::IsShowIconPtz(void)
{
	return	m_SkinCtlQuadT1.IsShowPtz;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoQuadT1::IsShowIconAudio(void)
{
	return	m_SkinCtlQuadT1.IsShowAudio;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoQuadT1::ShowNameSet(BOOL8 A_IsShow)
{
		m_SkinCtlQuadT1.IsShowName	=	A_IsShow;
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoQuadT1::ShowIconRec(BOOL8 A_IsShow)
{
		m_SkinCtlQuadT1.IsShowRec	=	A_IsShow;
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoQuadT1::ShowIconEvt(BOOL8 A_IsShow)
{
		m_SkinCtlQuadT1.IsShowEvt	=	A_IsShow;
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoQuadT1::ShowIconPtz(BOOL8 A_IsShow)
{
		m_SkinCtlQuadT1.IsShowPtz	=	A_IsShow;
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoQuadT1::ShowIconAudio(BOOL8 A_IsShow)
{
		m_SkinCtlQuadT1.IsShowAudio	=	A_IsShow;
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoQuadT1::EvtTimerTick(void)
{
	// local -------------------
		__u32	Tv_Val;
		__u32	Tv_WkCh;
		__u32	Tv_RecMask;
		__u32	Tv_MtnMask;
		__u32	Tv_SenMask;
		__u32	Tv_IaMask;
		__u32	Tv_AdoMask;
		__u32	Tv_PtzMask;
		__u32	Tv_ChMask;
		__u8	Tv_Ratb;
		__u8	Tv_VwIdx;
	// code --------------------

		// update record
		Tv_RecMask	=	0;
		Tv_Val	=	((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[m_ConIdx].ObjCon)->QuadRecChMapGet();
		if(m_SkinCtlQuadT1.IsShowRec)
		{
			Tv_RecMask	=	m_SkinCtlQuadT1.RecChMap ^ Tv_Val;

			if(m_SkinCtlQuadT1.IsRecEvtShow)
			{
				for(Tv_WkCh = 0; Tv_WkCh < E_GrDvrMaxChCnt; Tv_WkCh++)
				{
					Tv_Ratb	=	((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[m_ConIdx].ObjCon)->QuadChRecTypeGet(Tv_WkCh);
					if(Tv_Ratb != m_SkinCtlQuadT1.RatbTbl[Tv_WkCh])
					{
						m_SkinCtlQuadT1.RatbTbl[Tv_WkCh]	=	Tv_Ratb;
						Tv_RecMask	=	Tv_RecMask | (1 << Tv_WkCh);
					}
				}
			}
		}
		m_SkinCtlQuadT1.RecChMap	=	Tv_Val;

		// update motion
		Tv_MtnMask	=	0;
		Tv_Val	=	((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[m_ConIdx].ObjCon)->QuadMotionChMapGet();
		if(m_SkinCtlQuadT1.IsShowEvt)
		{
			Tv_MtnMask	=	m_SkinCtlQuadT1.MtnChMap ^ Tv_Val;
		}
		m_SkinCtlQuadT1.MtnChMap	=	Tv_Val;

		// update sensor
		Tv_SenMask	=	0;
		Tv_Val	=	((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[m_ConIdx].ObjCon)->QuadSensorChMapGet();
		if(m_SkinCtlQuadT1.IsShowEvt)
		{
			Tv_SenMask	=	m_SkinCtlQuadT1.SenChMap ^ Tv_Val;
		}
		m_SkinCtlQuadT1.SenChMap	=	Tv_Val;

		// update image analysis
		Tv_IaMask		=	0;
		Tv_Val	=	((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[m_ConIdx].ObjCon)->QuadIaChMapGet();
		if(m_SkinCtlQuadT1.IsShowEvt)
		{
			Tv_IaMask	=	m_SkinCtlQuadT1.IaChMap ^ Tv_Val;
		}
		m_SkinCtlQuadT1.IaChMap	=	Tv_Val;

		// update audio
		Tv_AdoMask		=	0;
		Tv_Val	=	((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[m_ConIdx].ObjCon)->QuadAdoChMapGet();
		if(m_SkinCtlQuadT1.IsShowAudio)
		{
			Tv_AdoMask	=	(m_SkinCtlQuadT1.AdoChMap ^ Tv_Val);
		}
		m_SkinCtlQuadT1.AdoChMap	=	Tv_Val;

		// update pantilt
		Tv_PtzMask	=	0;
		Tv_Val	=	((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[m_ConIdx].ObjCon)->QuadPtzChMapGet();
		if(m_SkinCtlQuadT1.IsShowPtz)
		{
			Tv_PtzMask	=	(m_SkinCtlQuadT1.PtzChMap ^ Tv_Val);
		}
		m_SkinCtlQuadT1.PtzChMap	=	Tv_Val;

		Tv_ChMask	=	1;
		for( Tv_WkCh = 0; Tv_WkCh < E_GrDvrMaxChCnt; Tv_WkCh++)
		{
			// convert view index
			Tv_VwIdx	=	m_SkinCtlQuadT1.ChToVwTbl[Tv_WkCh];
			// check current view index
			if((__s8)Tv_WkCh != m_SkinCtlQuadT1.Views[Tv_VwIdx].Ch)
			{
				continue;
			}
			// record icon
			if( 0 != (Tv_RecMask & Tv_ChMask) ) 
			{
				LcPntReqByRect(&m_SkinCtlQuadT1.Views[Tv_VwIdx].RcIconRec);
			}
			// motion icon
			if(0 != (Tv_MtnMask & Tv_ChMask))
			{
				LcPntReqByRect(&m_SkinCtlQuadT1.Views[Tv_VwIdx].RcIconMtn);
			}
			// sensor icon
			if(0 != (Tv_SenMask & Tv_ChMask))
			{
				LcPntReqByRect(&m_SkinCtlQuadT1.Views[Tv_VwIdx].RcIconSen);
			}
			// image analysis icon
			if(0 != (Tv_IaMask & Tv_ChMask))
			{
				LcPntReqByRect(&m_SkinCtlQuadT1.Views[Tv_VwIdx].RcIconIa);
			}
			// audio icon
			if(0 != (Tv_AdoMask & Tv_ChMask))
			{
				LcPntReqByRect(&m_SkinCtlQuadT1.Views[Tv_VwIdx].RcIconAdo);
			}
			// ptz icon
			if(0 != (Tv_PtzMask & Tv_ChMask))
			{
				LcPntReqByRect(&m_SkinCtlQuadT1.Views[Tv_VwIdx].RcIconPtz);
			}
			// next
			Tv_ChMask	=	Tv_ChMask << 1;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5UoQuadT1::EvtQuadChanged(void)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoQuadBase::EvtQuadChanged();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoQuadT1::IsRecIconEvtShow(void)
{
		return	m_SkinCtlQuadT1.IsRecEvtShow;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoQuadT1::RecIconEvtShowSet(BOOL8 A_IsOn)
{
		m_SkinCtlQuadT1.IsRecEvtShow	=	A_IsOn;
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoQuadT1::ReqDraw(BOOL8 A_IsForce)
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP5UoQuadBase::ReqDraw(A_IsForce);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoQuadT1::LcDrawReqPrev(void)
{
	// local -------------------
		__u32		Tv_ReqIdx;
	// code --------------------
		if(0 == m_DreqPrv.Cnt)
		{
			return;
		}

		for( Tv_ReqIdx = 0; Tv_ReqIdx < m_DreqPrv.Cnt; Tv_ReqIdx++)
		{
			LcPntReqByRect(&m_DreqPrv.RcTbl[Tv_ReqIdx]);
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5UoQuadT1::ReqDrawByChg(void)
{
	// local -------------------
	// code --------------------
		if(0 == m_DreqPrv.Cnt)
		{
			// reqbuild
			LcSkinCtlUpdate();

			// update new
			m_DreqPrv.Cnt		=	m_SkinCtlQuadT1.Dreq.Cnt;
			if(0 != m_DreqPrv.Cnt)
			{
				GrDumyCopyMem(m_DreqPrv.RcTbl, m_SkinCtlQuadT1.Dreq.RcTbl, sizeof(St_GrRect)* m_DreqPrv.Cnt);
			}

			ReqDraw(FALSE);
		}
		else
		{
			// draw previous
			LcDrawReqPrev();

			// reqbuild
			LcSkinCtlUpdate();

			// update new
			m_DreqPrv.Cnt		=	m_SkinCtlQuadT1.Dreq.Cnt;
			if(0 != m_DreqPrv.Cnt)
			{
				GrDumyCopyMem(m_DreqPrv.RcTbl, m_SkinCtlQuadT1.Dreq.RcTbl, sizeof(St_GrRect)* m_DreqPrv.Cnt);
			}

			// draw new
			LcDrawReqPrev();
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5UoQuadT1::RtlScrRectBld(void)
{
	// local -------------------
	// code --------------------
		Cls_GrP5UoQuadBase::RtlScrRectBld();
		m_DreqPrv.Cnt		=	0;
}
//--------------------------------------------------------------------
