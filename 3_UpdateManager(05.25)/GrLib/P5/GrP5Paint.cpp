/*
	Platform 5 render

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5Paint.h>

#include <GrDebug.h>
#include <GrStrTool.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5ConBase.h>
#include <P5/GrP5Script.h>

#include <GrDumyTool.h>

//--------------------------------------------------------------------
// local constant

//--------------------------------------------------------------------
// local type

//--------------------------------------------------------------------
//local function

void	GrP5PntClipArrange(Ptr_GrSwordRect A_PtrRect);

//--------------------------------------------------------------------
//global var

extern	St_GrP5Env	V_GrP5Env;

//====================================================================
//--------------------------------------------------------------------
void	GrP5PntReset(void)
{
		V_GrP5Env.PntEnv.InCnt			=	0;
		V_GrP5Env.PntEnv.SctCnt			=	0;
		V_GrP5Env.PntEnv.ClipTotal	=	0;

}
//--------------------------------------------------------------------
BOOL8	GrP5PntSectionStart(Ptr_GrRect A_PtrRect)
{
	// local -------------------
		Ptr_GrP5PntCmdSection	Tv_PtrCmdSct;
	// code --------------------
		if(E_GrP5PntStackMaxCnt <= V_GrP5Env.PntEnv.InCnt )
		{
#ifdef E_GrP5DbgMsgWarnning
			DbgMsgPrint("GrP5PntSectionStart - stack is full!\n");
#endif
			return	FALSE;
		}

		if(E_GrP5PntSctMaxCnt <= V_GrP5Env.PntEnv.SctCnt)
		{
#ifdef E_GrP5DbgMsgWarnning
			DbgMsgPrint("GrP5PntSectionStart - section is full!\n");
#endif
			return	FALSE;
		}

#ifdef E_GrP5DbgMsgWarnning
		if ((0 > A_PtrRect->left) || (0 > A_PtrRect->top))
		{
			DbgMsgPrint("GrP5PntSectionStart - bad rect (l:%d t:%d r:%d b:%d) input!\n", 
				A_PtrRect->left,A_PtrRect->top,A_PtrRect->right,A_PtrRect->bottom);
		}
#endif

		// insert section command
		Tv_PtrCmdSct	=	(Ptr_GrP5PntCmdSection)&V_GrP5Env.PntEnv.Items[V_GrP5Env.PntEnv.InCnt];

		Tv_PtrCmdSct->Cmd	=	E_GrP5PntCmdSection;
		Tv_PtrCmdSct->Rect.left			=	(__s16)A_PtrRect->left;
		Tv_PtrCmdSct->Rect.right		=	(__s16)A_PtrRect->right;
		Tv_PtrCmdSct->Rect.top			=	(__s16)A_PtrRect->top;
		Tv_PtrCmdSct->Rect.bottom		=	(__s16)A_PtrRect->bottom;
		Tv_PtrCmdSct->SctIdx				=	(__u8)V_GrP5Env.PntEnv.SctCnt;

		// section info
		V_GrP5Env.PntEnv.SctBuf[V_GrP5Env.PntEnv.SctCnt].Ip				=	V_GrP5Env.PntEnv.InCnt;
		V_GrP5Env.PntEnv.SctBuf[V_GrP5Env.PntEnv.SctCnt].Rc				=	Tv_PtrCmdSct->Rect;
		V_GrP5Env.PntEnv.SctBuf[V_GrP5Env.PntEnv.SctCnt].ClipIdx	=	-1;

		// clip info
		V_GrP5Env.PntEnv.RcClip		=	*A_PtrRect;

		// info update
		V_GrP5Env.PntEnv.InCnt ++;
		V_GrP5Env.PntEnv.SctCnt ++;

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	GrP5PntClipSet(Ptr_GrRect A_PtrRect, Ptr_GrRect A_PtrRtRcOrg)
{
	// local -------------------
		Ptr_GrP5PntCmdClip	Tv_PtrCmdClip;
		St_GrRect	Tv_RcNew;
	// code --------------------
		if (E_GrP5PntStackMaxCnt <= V_GrP5Env.PntEnv.InCnt)
		{
#ifdef E_GrP5DbgMsgWarnning
			DbgMsgPrint("GrP5PntClipSet - stack is full!\n");
#endif
			return	FALSE;
		}

		// chek clip insertable
		if (E_GrP5PntClipMaxCnt <= V_GrP5Env.PntEnv.ClipTotal)
		{
#ifdef E_GrP5DbgMsgWarnning
			DbgMsgPrint("GrP5PntClipSet - clip buffer is full!\n");
#endif
			return	FALSE;
		}

		// return orignal clip rect
		*A_PtrRtRcOrg	=	V_GrP5Env.PntEnv.RcClip;

		// check able rect
		if (!GrDrawCnGetComRect(&Tv_RcNew, &V_GrP5Env.PntEnv.RcClip, A_PtrRect))
		{
			return	FALSE;
		}

		// insert clip command
		Tv_PtrCmdClip	=	(Ptr_GrP5PntCmdClip)&V_GrP5Env.PntEnv.Items[V_GrP5Env.PntEnv.InCnt];

		Tv_PtrCmdClip->Cmd	=	E_GrP5PntCmdClip;
		Tv_PtrCmdClip->Rect.left			=	(__s16)Tv_RcNew.left;
		Tv_PtrCmdClip->Rect.right			=	(__s16)Tv_RcNew.right;
		Tv_PtrCmdClip->Rect.top				=	(__s16)Tv_RcNew.top;
		Tv_PtrCmdClip->Rect.bottom		=	(__s16)Tv_RcNew.bottom;
		Tv_PtrCmdClip->IsValid				=	TRUE;
		Tv_PtrCmdClip->ClipIdx				=	V_GrP5Env.PntEnv.ClipTotal;

		// insert clip buffer
		V_GrP5Env.PntEnv.ClipBuf[V_GrP5Env.PntEnv.ClipTotal].Ip			=	V_GrP5Env.PntEnv.InCnt;
		V_GrP5Env.PntEnv.ClipBuf[V_GrP5Env.PntEnv.ClipTotal].IsAct	=	TRUE;
		V_GrP5Env.PntEnv.ClipBuf[V_GrP5Env.PntEnv.ClipTotal].Rc			=	Tv_PtrCmdClip->Rect;
		V_GrP5Env.PntEnv.ClipBuf[V_GrP5Env.PntEnv.ClipTotal].SctIdx	=	(__u8)V_GrP5Env.PntEnv.SctCnt - 1;

		// update section
		if(0 > V_GrP5Env.PntEnv.SctBuf[V_GrP5Env.PntEnv.SctCnt - 1].ClipIdx)
		{
			V_GrP5Env.PntEnv.SctBuf[V_GrP5Env.PntEnv.SctCnt - 1].ClipIdx	=	(__s32)V_GrP5Env.PntEnv.ClipTotal;
		}

		// info update
		V_GrP5Env.PntEnv.InCnt ++;
		V_GrP5Env.PntEnv.ClipTotal ++;
		V_GrP5Env.PntEnv.RcClip		=	Tv_RcNew;
		
		return	TRUE;

}
//--------------------------------------------------------------------
void	GrP5PntClipRestore(Ptr_GrRect A_PtrRect)
{
	// local -------------------
	// code --------------------
		V_GrP5Env.PntEnv.RcClip	=	*A_PtrRect;
}
//--------------------------------------------------------------------
void	GrP5PntArrangeByRect(Ptr_GrRect A_PtrRect)
{
	// local -------------------
		St_GrRect	Tv_RcClp;
		St_GrSwordRect	Tv_RcswClp;
	// code --------------------
		// get clipped rect
		if(!GrDrawCnGetComRect(&Tv_RcClp, &V_GrP5Env.PntEnv.RcClip, A_PtrRect))
		{
			return;
		}
		
		// convert to sword rect
		Tv_RcswClp.left			=	(__s16)Tv_RcClp.left;
		Tv_RcswClp.top			=	(__s16)Tv_RcClp.top;
		Tv_RcswClp.right		=	(__s16)Tv_RcClp.right;
		Tv_RcswClp.bottom		=	(__s16)Tv_RcClp.bottom;

		// do arrange
		GrP5PntClipArrange(&Tv_RcswClp);
		
}
//--------------------------------------------------------------------
BOOL8	GrP5PntReqFillRect(Ptr_GrRect A_PtrRect, __u32 A_Color)
{
	// local -------------------
		Ptr_GrP5PntCmdFillRect	Tv_PtrCmd;
	// code --------------------
		if(E_GrP5PntStackMaxCnt <= V_GrP5Env.PntEnv.InCnt)
		{
#ifdef E_GrP5DbgMsgWarnning
			DbgMsgPrint("GrP5PntReqFillRect - stack is full!\n");
#endif
			return	FALSE;
		}

		// insert command
		Tv_PtrCmd	=	(Ptr_GrP5PntCmdFillRect)&V_GrP5Env.PntEnv.Items[V_GrP5Env.PntEnv.InCnt];

		Tv_PtrCmd->Cmd		=	E_GrP5PntCmdFillRect;
		Tv_PtrCmd->Color	=	A_Color;
		Tv_PtrCmd->Rect.left			=	(__s16)A_PtrRect->left;
		Tv_PtrCmd->Rect.right			=	(__s16)A_PtrRect->right;
		Tv_PtrCmd->Rect.top				=	(__s16)A_PtrRect->top;
		Tv_PtrCmd->Rect.bottom		=	(__s16)A_PtrRect->bottom;

		// info update
		V_GrP5Env.PntEnv.InCnt ++;

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	GrP5PntReqLine(__s32 A_StX, __s32 A_StY, __s32 A_EdX, __s32 A_EdY, __u32 A_Color)
{
	// local -------------------
		Ptr_GrP5PntCmdLine	Tv_PtrCmd;
	// code --------------------
		if(E_GrP5PntStackMaxCnt <= V_GrP5Env.PntEnv.InCnt)
		{
#ifdef E_GrP5DbgMsgWarnning
			DbgMsgPrint("GrP5PntReqLine - stack is full!\n");
#endif
			return	FALSE;
		}

		Tv_PtrCmd	=	(Ptr_GrP5PntCmdLine)&V_GrP5Env.PntEnv.Items[V_GrP5Env.PntEnv.InCnt];

		Tv_PtrCmd->Cmd		=	E_GrP5PntCmdLine;
		Tv_PtrCmd->Color	=	A_Color;

		Tv_PtrCmd->St.x		=	(__s16)A_StX;
		Tv_PtrCmd->St.y		=	(__s16)A_StY;

		Tv_PtrCmd->Ed.x		=	(__s16)A_EdX;
		Tv_PtrCmd->Ed.y		=	(__s16)A_EdY;

		// info update
		V_GrP5Env.PntEnv.InCnt ++;

		return	TRUE;

}
//--------------------------------------------------------------------
BOOL8	GrP5PntReqPixel(__s32 A_X, __s32 A_Y, __u32 A_Color)
{
	// local -------------------
		Ptr_GrP5PntCmdPixel	Tv_PtrCmd;
	// code --------------------
		if(E_GrP5PntStackMaxCnt <= V_GrP5Env.PntEnv.InCnt)
		{
#ifdef E_GrP5DbgMsgWarnning
			DbgMsgPrint("GrP5PntReqPixel - stack is full!\n");
#endif
			return	FALSE;
		}

		Tv_PtrCmd	=	(Ptr_GrP5PntCmdPixel)&V_GrP5Env.PntEnv.Items[V_GrP5Env.PntEnv.InCnt];

		Tv_PtrCmd->Cmd		=	E_GrP5PntCmdPixel;
		Tv_PtrCmd->Color	=	A_Color;

		Tv_PtrCmd->X			=	A_X;
		Tv_PtrCmd->Y			=	A_Y;

		// info update
		V_GrP5Env.PntEnv.InCnt ++;

		return	TRUE;

}
//--------------------------------------------------------------------
BOOL8	GrP5PntReqImage(Ptr_GrGdib A_PtrGdib, __s32 A_X, __s32 A_Y, BOOL8 A_IsSprite)
{
	// local -------------------
		Ptr_GrP5PntCmdImage	Tv_PtrCmd;
	// code --------------------
		if(E_GrP5PntStackMaxCnt <= V_GrP5Env.PntEnv.InCnt)
		{
#ifdef E_GrP5DbgMsgWarnning
			DbgMsgPrint("GrP5PntReqImage - stack is full!\n");
#endif
			return	FALSE;
		}

		if(NULL == A_PtrGdib)
		{
			return FALSE;
		}

		Tv_PtrCmd	=	(Ptr_GrP5PntCmdImage)&V_GrP5Env.PntEnv.Items[V_GrP5Env.PntEnv.InCnt];

		Tv_PtrCmd->Cmd			=	E_GrP5PntCmdImage;
		Tv_PtrCmd->IsSprite	=	A_IsSprite;
		Tv_PtrCmd->PtrGdib	=	A_PtrGdib;

		Tv_PtrCmd->Pos.x		=	(__s16)A_X;
		Tv_PtrCmd->Pos.y		=	(__s16)A_Y;

		// info update
		V_GrP5Env.PntEnv.InCnt ++;

		return	TRUE;

}
//--------------------------------------------------------------------
BOOL8	GrP5PntReqTxtStyle(void* A_PtrFontDrawStyle, __u8 A_FontId)
{
	// local -------------------
		Ptr_GrP5FontDrawStyle	Tv_PtrStyle;
		Ptr_GrP5PntCmdTxtStyle0	Tv_PtrCmd0;
		Ptr_GrP5PntCmdTxtStyle1	Tv_PtrCmd1;
	// code --------------------
		if(E_GrP5PntStackMaxCnt <= (V_GrP5Env.PntEnv.InCnt + 1))
		{
#ifdef E_GrP5DbgMsgWarnning
			DbgMsgPrint("GrP5PntReqTxtStyle - stack is full!\n");
#endif
			return	FALSE;
		}

		Tv_PtrStyle	=	(Ptr_GrP5FontDrawStyle)A_PtrFontDrawStyle;

		Tv_PtrCmd0	=	(Ptr_GrP5PntCmdTxtStyle0)&V_GrP5Env.PntEnv.Items[V_GrP5Env.PntEnv.InCnt];
		Tv_PtrCmd1	=	(Ptr_GrP5PntCmdTxtStyle1)&V_GrP5Env.PntEnv.Items[V_GrP5Env.PntEnv.InCnt + 1];

		Tv_PtrCmd0->Cmd				=	E_GrP5PntCmdTxtStyle0;
		Tv_PtrCmd0->Atb				=	Tv_PtrStyle->Atb;
		Tv_PtrCmd0->AlignHz		=	Tv_PtrStyle->AlignHz;
		Tv_PtrCmd0->AlignVt		=	Tv_PtrStyle->AlignVt;
		Tv_PtrCmd0->GapChar		=	Tv_PtrStyle->GapChar;
		Tv_PtrCmd0->GapLine		=	Tv_PtrStyle->GapLine;
		Tv_PtrCmd0->CodePw		=	Tv_PtrStyle->CodePw;
		Tv_PtrCmd0->ClrShadow	=	Tv_PtrStyle->ColorShadow;
		Tv_PtrCmd0->ClrTxt0		=	Tv_PtrStyle->ColorTxtTbl[0];

		Tv_PtrCmd1->Cmd				=	E_GrP5PntCmdTxtStyle1;
		Tv_PtrCmd1->FontId		=	A_FontId;
		Tv_PtrCmd1->ClrTxt1		=	Tv_PtrStyle->ColorTxtTbl[1];
		Tv_PtrCmd1->ClrTxt2		=	Tv_PtrStyle->ColorTxtTbl[2];
		Tv_PtrCmd1->ClrTxt3		=	Tv_PtrStyle->ColorTxtTbl[3];

		// info update
		V_GrP5Env.PntEnv.InCnt	=	V_GrP5Env.PntEnv.InCnt + 2;

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	GrP5PntReqTxtCus(__s32 A_CusPos, __u32 A_ClrCus)
{
	// local -------------------
		Ptr_GrP5PntCmdTxtCus	Tv_PtrCmd;
	// code --------------------
		if(E_GrP5PntStackMaxCnt <= V_GrP5Env.PntEnv.InCnt)
		{
#ifdef E_GrP5DbgMsgWarnning
			DbgMsgPrint("GrP5PntReqTxtCus - stack is full!\n");
#endif
			return	FALSE;
		}

		Tv_PtrCmd	=	(Ptr_GrP5PntCmdTxtCus)&V_GrP5Env.PntEnv.Items[V_GrP5Env.PntEnv.InCnt];

		Tv_PtrCmd->Cmd			=	E_GrP5PntCmdTxtCus;
		Tv_PtrCmd->Pos			=	A_CusPos;
		Tv_PtrCmd->Color		=	A_ClrCus;

		// info update
		V_GrP5Env.PntEnv.InCnt ++;

		return	TRUE;

}
//--------------------------------------------------------------------
BOOL8	GrP5PntReqTxtDraw(Ptr_GrRect A_PtrRect, WCHAR* A_Str, __u16 A_Stat)
{
	// local -------------------
		Ptr_GrP5PntCmdTxtDraw	Tv_PtrCmd;
	// code --------------------
		if(E_GrP5PntStackMaxCnt <= V_GrP5Env.PntEnv.InCnt)
		{
#ifdef E_GrP5DbgMsgWarnning
			DbgMsgPrint("GrP5PntReqTxtDraw - stack is full!\n");
#endif
			return	FALSE;
		}

		Tv_PtrCmd	=	(Ptr_GrP5PntCmdTxtDraw)&V_GrP5Env.PntEnv.Items[V_GrP5Env.PntEnv.InCnt];

		Tv_PtrCmd->Cmd	=	E_GrP5PntCmdTxtDraw;
		Tv_PtrCmd->Stat	=	A_Stat;

		Tv_PtrCmd->Str	=	A_Str;

		Tv_PtrCmd->Rect.left			=	(__s16)A_PtrRect->left;
		Tv_PtrCmd->Rect.right			=	(__s16)A_PtrRect->right;
		Tv_PtrCmd->Rect.top				=	(__s16)A_PtrRect->top;
		Tv_PtrCmd->Rect.bottom		=	(__s16)A_PtrRect->bottom;

		// info update
		V_GrP5Env.PntEnv.InCnt ++;

		return	TRUE;

}
//--------------------------------------------------------------------
void	GrP5PntClipArrange(Ptr_GrSwordRect A_PtrRect)
{
	// local -------------------
		BOOL8	Tv_IsRmvClp;
		__u32	Tv_ClpIdx;
		__u32	Tv_PntIp;
		__u32	Tv_ClpMin;
		St_GrSwordRect	Tv_RcOrgClp;
		Ptr_GrP5PntClipItem	Tv_PtrClp;
	// code --------------------
		// init
		Tv_IsRmvClp	=	FALSE;
		// check now clip
		Tv_PtrClp	=	&V_GrP5Env.PntEnv.ClipBuf[V_GrP5Env.PntEnv.ClipTotal - 1];
		if((A_PtrRect->left <= Tv_PtrClp->Rc.left) && (A_PtrRect->right >= Tv_PtrClp->Rc.right) && 
			(A_PtrRect->top <= Tv_PtrClp->Rc.top) && (A_PtrRect->bottom >= Tv_PtrClp->Rc.bottom)
			)
		{
			// remove stacked script
			V_GrP5Env.PntEnv.InCnt	=	Tv_PtrClp->Ip + 1;
			// remove previous clip able
			Tv_IsRmvClp	=	TRUE;
			Tv_RcOrgClp	=	Tv_PtrClp->Rc;	// backup orignal clip rect
		}

		// check previous clip
		Tv_ClpIdx	=	V_GrP5Env.PntEnv.ClipTotal - 1;
		Tv_ClpMin	=	(__u32)V_GrP5Env.PntEnv.SctBuf[V_GrP5Env.PntEnv.SctCnt - 1].ClipIdx;
		while(Tv_ClpMin < Tv_ClpIdx)
		{
			// decrease index
			Tv_ClpIdx --;
			Tv_PtrClp	=	&V_GrP5Env.PntEnv.ClipBuf[Tv_ClpIdx];

			// check active
			if(Tv_PtrClp->IsAct)
			{
				if((A_PtrRect->left <= Tv_PtrClp->Rc.left) && (A_PtrRect->right >= Tv_PtrClp->Rc.right) &&
					(A_PtrRect->top <= Tv_PtrClp->Rc.top) && (A_PtrRect->bottom >= Tv_PtrClp->Rc.bottom)
					)
				{
					// deactivate
					Tv_PtrClp->IsAct	=	FALSE;
					// update clip code status
					((Ptr_GrP5PntCmdClip)&V_GrP5Env.PntEnv.Items[Tv_PtrClp->Ip])->IsValid	=	FALSE;
				}
				else
				{
					Tv_IsRmvClp	=	FALSE;
				}
			}

			// do remove clip
			if(Tv_IsRmvClp)
			{
				// move clip positon
				V_GrP5Env.PntEnv.ClipBuf[Tv_ClpIdx].Rc		=	Tv_RcOrgClp;
				V_GrP5Env.PntEnv.ClipBuf[Tv_ClpIdx].IsAct	=	TRUE;
				// patch clip command
				Tv_PntIp	=	V_GrP5Env.PntEnv.ClipBuf[Tv_ClpIdx].Ip;
				((Ptr_GrP5PntCmdClip)&V_GrP5Env.PntEnv.Items[Tv_PntIp])->IsValid	=	TRUE;
				((Ptr_GrP5PntCmdClip)&V_GrP5Env.PntEnv.Items[Tv_PntIp])->ClipIdx	=	Tv_ClpIdx;
				((Ptr_GrP5PntCmdClip)&V_GrP5Env.PntEnv.Items[Tv_PntIp])->Rect			=	*A_PtrRect;
				// update count
				V_GrP5Env.PntEnv.ClipTotal	=	Tv_ClpIdx + 1;
				V_GrP5Env.PntEnv.InCnt			=	V_GrP5Env.PntEnv.ClipBuf[Tv_ClpIdx].Ip + 1;
			}
		}
}
//--------------------------------------------------------------------
BOOL8	GrP5PntReqDrawRect(Ptr_GrRect A_PtrRect, __u32 A_Color)
{
	// local -------------------
		Ptr_GrP5PntCmdDrawRect	Tv_PtrCmd;
	// code --------------------
		if(E_GrP5PntStackMaxCnt <= V_GrP5Env.PntEnv.InCnt)
		{
#ifdef E_GrP5DbgMsgWarnning
			DbgMsgPrint("GrP5PntReqDrawRect - stack is full!\n");
#endif
			return	FALSE;
		}

		// insert command
		Tv_PtrCmd	=	(Ptr_GrP5PntCmdDrawRect)&V_GrP5Env.PntEnv.Items[V_GrP5Env.PntEnv.InCnt];

		Tv_PtrCmd->Cmd		=	E_GrP5PntCmdDrawRect;
		Tv_PtrCmd->Color	=	A_Color;
		Tv_PtrCmd->Rect.left			=	(__s16)A_PtrRect->left;
		Tv_PtrCmd->Rect.right			=	(__s16)A_PtrRect->right;
		Tv_PtrCmd->Rect.top				=	(__s16)A_PtrRect->top;
		Tv_PtrCmd->Rect.bottom		=	(__s16)A_PtrRect->bottom;

		// info update
		V_GrP5Env.PntEnv.InCnt ++;

		return	TRUE;
}
//--------------------------------------------------------------------
