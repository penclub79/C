/*
	Platform 5 render

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5Render.h>

#include <GrDebug.h>
#include <GrStrTool.h>
#include <P5/GrP5ConBase.h>
#include <P5/GrP5UoBase.h>

#include <P5/GrP5Mng.h>

#include <P5/GrP5Script.h>
#include <P5/GrP5SkinBase.h>

#include <GrDumyTool.h>
#include <GrLayerData.h>


//--------------------------------------------------------------------
// local constant

//--------------------------------------------------------------------
// local type

//--------------------------------------------------------------------
//local function

//--------------------------------------------------------------------
//global var

extern	St_GrP5Env	V_GrP5Env;

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5Render::Cls_GrP5Render(Cls_GrTaskSvr* A_TaskSvr) :
Cls_GrTaskCli()
{
	// local -------------------
	// code --------------------
		m_TskSvr	=	A_TaskSvr;

		m_MsgAloc	=	GrTaskSvrMsgCmdAlloc();

		m_IsOn		=	FALSE;

		m_TskSvr->RegistMsgClient(this);
		//m_TskSvr->RegistTimerClient(this, E_GrP5RenderTimerPeriod, &m_TmrMain);
		m_TskSvr->RegistBkgClient(this);
}
//--------------------------------------------------------------------
Cls_GrP5Render::~Cls_GrP5Render(void)
{
	// local -------------------
	// code --------------------
		m_TskSvr->UnregistBkgClient(this);
		//m_TskSvr->UnregistTimerClient(m_TmrMain);
		m_TskSvr->UnregistMsgClient(this);

		GrTaskSvrMsgCmdFree(m_MsgAloc);

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5Render::RtlMsgProc(Ptr_GrTaskMsgHd A_PtrMsg)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if(m_MsgAloc == A_PtrMsg->Cmd)
		{
			Tv_Result	=	TRUE;
			switch(((Ptr_GrP5RendCmdBase)A_PtrMsg)->Cmd)
			{
				case E_GrP5RendCmdRunModeSet:
					ImmRenderRunSet(((Ptr_GrP5RendCmdRunModeSet)A_PtrMsg)->IsOn);
					break;
				default:
					Tv_Result	=	FALSE;
					break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP5Render::RtlTimer(__u32 A_Hndl)
{
	// local -------------------
		
	// code --------------------

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5Render::LcTmrConPaint(void)
{
	// local -------------------
	// code --------------------

		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5Render::RtlBkgProc(void)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if(LcBkpRender())
		{
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5Render::LcBkpRender(void)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_ConIdx;
	// code --------------------
		Tv_Result	=	FALSE;

		if(m_IsOn)
		{
			for(Tv_ConIdx = 0; Tv_ConIdx < E_GrP5ConMaxCnt; Tv_ConIdx++)
			{
				if(LcConRender(Tv_ConIdx))
				{
					Tv_Result	=	TRUE;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5Render::LcConRender(__u8 A_ConIdx)
{
	// local -------------------
		
	// code --------------------
		// check need paint
		if(!V_GrP5Env.ConTbl[A_ConIdx].IsPntNeed)
		{
			return	FALSE;
		}

		if(NULL == V_GrP5Env.ConTbl[A_ConIdx].ObjCon)
		{
			return	FALSE;
		}

		// reqeust build paint code
		((Cls_GrP5Mng*)V_GrP5Env.ObjMng)->ReqPntCodeBld(A_ConIdx);

		// init
		m_ConIdx	=	A_ConIdx;
		m_SctIdx	=	0;
		m_PntIp		=	0;
		m_GdibScr	=	((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[A_ConIdx].ObjCon)->ScrGdibGet();
		m_ObjFont	=	NULL;
		GrDumyZeroMem(&m_FontPara, sizeof(m_FontPara));
		m_CusPos	=	-1;
		m_ClrCus	=	0;

#ifdef E_GrP5RenderGatherTrans
		m_RcGth.left		=	0;
		m_RcGth.top			=	0;
		m_RcGth.right		=	0;
		m_RcGth.bottom	=	0;
#endif	
		
		if(0 == V_GrP5Env.PntEnv.InCnt)
		{
			return	TRUE;
		}

		// run
		while(m_PntIp < V_GrP5Env.PntEnv.InCnt)
		{
			switch(V_GrP5Env.PntEnv.Items[m_PntIp].Cmd)
			{
				case E_GrP5PntCmdSection:
					LcCmdSection();
					break;
				case E_GrP5PntCmdClip:
					LcCmdClip();
					break;
				case E_GrP5PntCmdFillRect:
					LcCmdFillRect();
					break;
				case  E_GrP5PntCmdLine:
					LcCmdLine();
					break;
				case E_GrP5PntCmdPixel:
					LcCmdPixel();
					break;
				case E_GrP5PntCmdImage:
					LcCmdImage();
					break;
				case E_GrP5PntCmdTxtStyle0:
					LcCmdTxtStyle0();
					break;
				case E_GrP5PntCmdTxtStyle1:
					LcCmdTxtStyle1();
					break;
				case E_GrP5PntCmdTxtCus:
					LcCmdTxtCus();
					break;
				case E_GrP5PntCmdTxtDraw:
					LcCmdTxtDraw();
					break;
				case E_GrP5PntCmdDrawRect:
					LcCmdDrawRect();
					break;

				default:
					DbgMsgPrint("Cls_GrP5Render::LcConRender - unknown command %d input!\n", V_GrP5Env.PntEnv.Items[m_PntIp].Cmd);
					m_PntIp ++;
					break;
			}
		}

		// transfer last section
#ifndef E_GrP5RenderGatherTrans
		((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[m_ConIdx].ObjCon)->EvtPaintHwTransfer(&m_RcSct);
#else
		((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[m_ConIdx].ObjCon)->EvtPaintHwTransfer(&m_RcGth);
#endif

		return	TRUE;

}
//--------------------------------------------------------------------
void	Cls_GrP5Render::LcCmdSection(void)
{
	// local -------------------
		Ptr_GrP5PntCmdSection	Tv_PtrCmd;
	// code --------------------
		Tv_PtrCmd	=	(Ptr_GrP5PntCmdSection)&V_GrP5Env.PntEnv.Items[m_PntIp];

		// update rect
#ifndef E_GrP5RenderGatherTrans
		if(0 != Tv_PtrCmd->SctIdx)
		{
			((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[m_ConIdx].ObjCon)->EvtPaintHwTransfer(&m_RcSct);
		}
#endif

		// update section rect
		m_RcSct.left		=	(__s32)Tv_PtrCmd->Rect.left;
		m_RcSct.top			=	(__s32)Tv_PtrCmd->Rect.top;
		m_RcSct.right		=	(__s32)Tv_PtrCmd->Rect.right;
		m_RcSct.bottom	=	(__s32)Tv_PtrCmd->Rect.bottom;

		// prepare event
		((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[m_ConIdx].ObjCon)->EvtPaintPrepare(&m_RcSct);

#ifdef E_GrP5RenderGatherTrans
		if(0 == m_RcGth.right)
		{
			m_RcGth		=	m_RcSct;
		}
		else
		{
			if(m_RcGth.left >= m_RcSct.left)
			{
				m_RcGth.left	=	m_RcSct.left;
			}
			if(m_RcGth.top >= m_RcSct.top)
			{
				m_RcGth.top		=	m_RcSct.top;
			}
			if(m_RcGth.right <= m_RcSct.right)
			{
				m_RcGth.right	=	m_RcSct.right;
			}
			if(m_RcGth.bottom <= m_RcSct.bottom)
			{
				m_RcGth.bottom	=	m_RcSct.bottom;
			}
		}
#endif
		
		// update section index
		m_SctIdx	=	Tv_PtrCmd->SctIdx;
		// go next
		m_PntIp ++;
}
//--------------------------------------------------------------------
void	Cls_GrP5Render::LcCmdClip(void)
{
	// local -------------------
		Ptr_GrP5PntCmdClip	Tv_PtrCmd;
		St_GrRect	Tv_RcClip;
		__u32		Tv_ClpIdx;
	// code --------------------
		Tv_PtrCmd	=	(Ptr_GrP5PntCmdClip)&V_GrP5Env.PntEnv.Items[m_PntIp];

		// check able
		if(Tv_PtrCmd->IsValid)
		{
			// set clip
			Tv_RcClip.left		=	(__s32)Tv_PtrCmd->Rect.left;
			Tv_RcClip.top			=	(__s32)Tv_PtrCmd->Rect.top;
			Tv_RcClip.right		=	(__s32)Tv_PtrCmd->Rect.right;
			Tv_RcClip.bottom	=	(__s32)Tv_PtrCmd->Rect.bottom;
			m_GdibScr->ClipperSet(&Tv_RcClip);
			// go next
			m_PntIp ++;
		}
		else
		{
			// go next clip
			Tv_ClpIdx	=	Tv_PtrCmd->ClipIdx + 1;
			if(Tv_ClpIdx < V_GrP5Env.PntEnv.ClipTotal)
			{
				// check finish section
				if(m_SctIdx == V_GrP5Env.PntEnv.ClipBuf[Tv_ClpIdx].SctIdx)
				{
					// update next clip ip
					m_PntIp		=	 V_GrP5Env.PntEnv.ClipBuf[Tv_ClpIdx].Ip;
				}
				else
				{
					if((m_SctIdx + 1) < (__u8)V_GrP5Env.PntEnv.SctCnt)
					{
						// update next section ip
						m_PntIp		=	V_GrP5Env.PntEnv.SctBuf[m_SctIdx + 1].Ip;
					}
					else
					{
						// finish
						m_PntIp		=	V_GrP5Env.PntEnv.InCnt;
					}
				}
			}
			else
			{
				// finish
				m_PntIp		=	V_GrP5Env.PntEnv.InCnt;
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5Render::LcCmdFillRect(void)
{
	// local -------------------
		Ptr_GrP5PntCmdFillRect	Tv_PtrCmd;
		St_GrRect	Tv_Rc;
	// code --------------------
		Tv_PtrCmd	=	(Ptr_GrP5PntCmdFillRect)&V_GrP5Env.PntEnv.Items[m_PntIp];

		// make rect
		Tv_Rc.left		=	(__s32)Tv_PtrCmd->Rect.left;
		Tv_Rc.top			=	(__s32)Tv_PtrCmd->Rect.top;
		Tv_Rc.right		=	(__s32)Tv_PtrCmd->Rect.right;
		Tv_Rc.bottom	=	(__s32)Tv_PtrCmd->Rect.bottom;

		m_GdibScr->FillRect(&Tv_Rc, Tv_PtrCmd->Color);

		// next
		m_PntIp ++;

}
//--------------------------------------------------------------------
void	Cls_GrP5Render::LcCmdLine(void)
{
	// local -------------------
		Ptr_GrP5PntCmdLine	Tv_PtrCmd;
	// code --------------------
		Tv_PtrCmd	=	(Ptr_GrP5PntCmdLine)&V_GrP5Env.PntEnv.Items[m_PntIp];

		m_GdibScr->DrawLine((__s32)Tv_PtrCmd->St.x, (__s32)Tv_PtrCmd->St.y, (__s32)Tv_PtrCmd->Ed.x, (__s32)Tv_PtrCmd->Ed.y,
			(__s32)Tv_PtrCmd->Color);

		// next
		m_PntIp ++;

}
//--------------------------------------------------------------------
void	Cls_GrP5Render::LcCmdPixel(void)
{
	// local -------------------
		Ptr_GrP5PntCmdPixel	Tv_PtrCmd;
	// code --------------------
		Tv_PtrCmd	=	(Ptr_GrP5PntCmdPixel)&V_GrP5Env.PntEnv.Items[m_PntIp];

		m_GdibScr->SetPixelVal((__s32)Tv_PtrCmd->X, (__s32)Tv_PtrCmd->Y, Tv_PtrCmd->Color);

		// next
		m_PntIp ++;
}
//--------------------------------------------------------------------
void	Cls_GrP5Render::LcCmdImage(void)
{
	// local -------------------
		Ptr_GrP5PntCmdImage	Tv_PtrCmd;
	// code --------------------
		Tv_PtrCmd	=	(Ptr_GrP5PntCmdImage)&V_GrP5Env.PntEnv.Items[m_PntIp];

		m_GdibScr->SpriteModeSet(Tv_PtrCmd->IsSprite);

		m_GdibScr->BltGdib((__s32)Tv_PtrCmd->Pos.x, (__s32)Tv_PtrCmd->Pos.y, Tv_PtrCmd->PtrGdib);

		// next
		m_PntIp ++;
}
//--------------------------------------------------------------------
void	Cls_GrP5Render::LcCmdTxtStyle0(void)
{
	// local -------------------
		Ptr_GrP5PntCmdTxtStyle0	Tv_PtrCmd;
	// code --------------------
		Tv_PtrCmd	=	(Ptr_GrP5PntCmdTxtStyle0)&V_GrP5Env.PntEnv.Items[m_PntIp];

		m_FontPara.Atb						=	Tv_PtrCmd->Atb;
		m_FontPara.AlignHz				=	Tv_PtrCmd->AlignHz;
		m_FontPara.AlignVt				=	Tv_PtrCmd->AlignVt;
		m_FontPara.GapChar				=	Tv_PtrCmd->GapChar;
		m_FontPara.GapLine				=	Tv_PtrCmd->GapLine;
		m_FontPara.CodePw					=	Tv_PtrCmd->CodePw;
		m_FontPara.ColorShadow		=	Tv_PtrCmd->ClrShadow;
		m_FontPara.ColorTxtTbl[0]	=	Tv_PtrCmd->ClrTxt0;

		// next
		m_PntIp ++;
}
//--------------------------------------------------------------------
void	Cls_GrP5Render::LcCmdTxtStyle1(void)
{
	// local -------------------
		Ptr_GrP5PntCmdTxtStyle1	Tv_PtrCmd;
	// code --------------------
		Tv_PtrCmd	=	(Ptr_GrP5PntCmdTxtStyle1)&V_GrP5Env.PntEnv.Items[m_PntIp];

		m_FontPara.ColorTxtTbl[1]	=	Tv_PtrCmd->ClrTxt1;
		m_FontPara.ColorTxtTbl[2]	=	Tv_PtrCmd->ClrTxt2;
		m_FontPara.ColorTxtTbl[3]	=	Tv_PtrCmd->ClrTxt3;

		m_ObjFont	=	(Cls_GrP5Font*)GrP5FontGet(Tv_PtrCmd->FontId);

		// next
		m_PntIp ++;
}
//--------------------------------------------------------------------
void	Cls_GrP5Render::LcCmdTxtCus(void)
{
	// local -------------------
		Ptr_GrP5PntCmdTxtCus	Tv_PtrCmd;
	// code --------------------
		Tv_PtrCmd	=	(Ptr_GrP5PntCmdTxtCus)&V_GrP5Env.PntEnv.Items[m_PntIp];

		m_CusPos	=	Tv_PtrCmd->Pos;
		m_ClrCus	=	Tv_PtrCmd->Color;

		// next
		m_PntIp ++;
}
//--------------------------------------------------------------------
void	Cls_GrP5Render::LcCmdTxtDraw(void)
{
	// local -------------------
		Ptr_GrP5PntCmdTxtDraw	Tv_PtrCmd;
		St_GrRect	Tv_Rc;
	// code --------------------
		Tv_PtrCmd	=	(Ptr_GrP5PntCmdTxtDraw)&V_GrP5Env.PntEnv.Items[m_PntIp];

		if(NULL != m_ObjFont)
		{
			Tv_Rc.left		=	(__s32)Tv_PtrCmd->Rect.left;
			Tv_Rc.top			=	(__s32)Tv_PtrCmd->Rect.top;
			Tv_Rc.right		=	(__s32)Tv_PtrCmd->Rect.right;
			Tv_Rc.bottom	=	(__s32)Tv_PtrCmd->Rect.bottom;

			m_ObjFont->DrawString(Tv_PtrCmd->Str, m_GdibScr, &Tv_Rc, &m_FontPara, (__u32)Tv_PtrCmd->Stat, m_CusPos, m_ClrCus);
		}

		// next
		m_PntIp ++;
}
//--------------------------------------------------------------------
void	Cls_GrP5Render::LcCmdDrawRect(void)
{
	// local -------------------
		Ptr_GrP5PntCmdDrawRect	Tv_PtrCmd;
		St_GrRect	Tv_Rc;
	// code --------------------
		Tv_PtrCmd	=	(Ptr_GrP5PntCmdDrawRect)&V_GrP5Env.PntEnv.Items[m_PntIp];

		Tv_Rc.left		=	(__s32)Tv_PtrCmd->Rect.left;
		Tv_Rc.top			=	(__s32)Tv_PtrCmd->Rect.top;
		Tv_Rc.right		=	(__s32)Tv_PtrCmd->Rect.right;
		Tv_Rc.bottom	=	(__s32)Tv_PtrCmd->Rect.bottom;

		m_GdibScr->DrawRect(&Tv_Rc, Tv_PtrCmd->Color);

		// next
		m_PntIp ++;
}
//--------------------------------------------------------------------
void	Cls_GrP5Render::ImmRenderRunSet(BOOL8 A_IsRun)
{
	// local -------------------
	// code --------------------
		m_IsOn		=	A_IsRun;
}
//--------------------------------------------------------------------
void	Cls_GrP5Render::ReqRenderRunSet(BOOL8 A_IsRun)
{
	// local -------------------
		St_GrP5RendCmdRunModeSet	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP5RendCmdRunModeSet;
		Tv_Msg.IsOn					=	A_IsRun;

		m_TskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

}
//--------------------------------------------------------------------

