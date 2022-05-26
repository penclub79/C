/*
Platform 5 skin type 2

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5SkinT2
#define	_EPC_GrP5SkinT2

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5SkinBase.h>

#include <GrGdibObj.h>

//====================================================================
//constance

#define	E_GrP5SkinT2PrgsSkinMaxCnt			4
	
#define	E_GrP5SkinT2TitleT0Height				28
#define	E_GrP5SkinT2TitleT0DivWidth			26

#define	E_GrP5SkinT2TabLblSepWidth			4

#define	E_GrP5SkinT2TabBtnFocusWidth		2

#define	E_GrP5SkinT2BtnWakuWidth				2
#define	E_GrP5SkinT2BtnFocusLineWidth		3

#define	E_GrP5SkinT2BtnCutSize					8

#define	E_GrP5SkinT2PrgsWakuWidth				5

#define	E_GrP5SkinT2ListCtrlMargin			1

#define	E_GrP5SkinT2TblMargin						1
#define	E_GrP5SkinT2TblItemMgnLeft			0
#define	E_GrP5SkinT2TblItemMgnTop				0
#define	E_GrP5SkinT2TblItemMgnRight			4
#define	E_GrP5SkinT2TblItemMgnDown			4
#define	E_GrP5SkinT2TblCusWidth					3

#define	E_GrP5SkinT2CbxMarkSize					10
#define	E_GrP5SkinT2CbxMarkGap					2

#define	E_GrP5SkinT2SpinMarginHz				3
#define	E_GrP5SkinT2SpinMarginVt				3
#define	E_GrP5SkinT2SpinCtlMargin				3
#define	E_GrP5SkinT2SpinCtlWidth				3

#define	E_GrP5SkinT2GridOutWidth				1
#define	E_GrP5SkinT2GridCusWidth				5
#define	E_GrP5SkinT2GridMarkWidth				3
#define	E_GrP5SkinT2GridDrageWidth			2

#define	E_GrP5SkinT2GraphHzMargin				3
#define	E_GrP5SkinT2GrpHzT1ContMgn			4

#define	E_GrP5SkinT2CalMargin						0
#define	E_GrP5SkinT2CalLineWidth				2
#define	E_GrP5SkinT2CalCusWidth					2

#define	E_GrP5SkinT2GridRscdItemHeight	8
#define	E_GrP5SkinT2GridRscdDragWidth		2
#define	E_GrP5SkinT2GridRscdDragRcCnt		4

#define	E_GrP5SkinT2WinPopListWakuWdt		1

#define	E_GrP5SkinT2GridentMaxCnt				7

#define	E_GrP5SkinT2AimCrossWidth				1
#define	E_GrP5SkinT2AimCrossSize				120

#define	E_GrP5SkinT2PnlDirIdcDfltSize		8

#define	E_GrP5SkinT2PopListWakuWidth		2
#define	E_GrP5SkinT2PopListMargin				4
#define	E_GrP5SkinT2PopListItmMgn				2
#define	E_GrP5SkinT2PopListSbarMgn			2

#define	E_GrP5SkinT2SbarVtWidth					22
#define	E_GrP5SkinT2SbarVtBtnHeight			14
#define	E_GrP5SkinT2SbarVtBtnMarkGap		2
#define	E_GrP5SkinT2SbarVtThumbSize			32
#define	E_GrP5SkinT2SbarVtLineWidth			2

#define	E_GrP5SkinT2PopMenuWakuWidth		2
#define	E_GrP5SkinT2PopMenuMargin				4
#define	E_GrP5SkinT2PopMenuItmMgn				2
#define	E_GrP5SkinT2PopMenuSubMark			6	

#define	E_GrP5SkinT2TabBorderWidth			4	

#define	E_GrP5SkinT2GrpHzT2GrpHeight		8
#define	E_GrP5SkinT2GrpHzT2RulLhHeight	6
#define	E_Grp5SkinT2GrpHzT2ZoomCnt			8			

#define	E_GrP5SkinT2TooltipWakuGap			2


// vector command
#define	E_GrP5SkinT2VctCmdEnd						0
#define	E_GrP5SkinT2VctCmdRect					1
#define	E_GrP5SkinT2VctCmdLine					2

//====================================================================
//global macro

//====================================================================
//global type


typedef struct St_GrP5SkinT2GradInfo
{
	__u8	ItemCnt;
	__u8	SizeTbl[E_GrP5SkinT2GridentMaxCnt];
	__u32	ColorTbl[E_GrP5SkinT2GridentMaxCnt];
}	*Ptr_GrP5SkinT2GradInfo;

typedef struct St_Grp5SkinT2GrpHzT2Rul
{
	__u32	PxPerSec;
	__u32	TxtPerSec;
	__u32	TxtPerPx;
	__u32	RulPerSec;
	__u32	RulPerPx;

}*Ptr_Grp5SkinT2GrpHzT2Rul;

typedef struct St_GrP5SkinT2VctCmd
{
	__u32	Cmd;
	__u32	ClrIdx;
	St_GrSwordRect	Rc;
}	*Ptr_GrP5SkinT2VctCmd;

//====================================================================
//global class

class Cls_GrP5SkinT2 : public Cls_GrP5SkinBase
{
	private:

#ifdef GR_P5_EDITOR
		__u32	m_DvlFccClr;
#endif

		// pallete
		__u32	m_ClrWinT0Bg0;
		__u32	m_ClrWinT0Bg1;
		__u32	m_ClrWinT0Bg2;

		__u32	m_ClrSbarBg;
		__u32	m_ClrSbarTrack;
		__u32	m_ClrSbarBtnNrm;
		__u32	m_ClrSbarBtnFcs;
		__u32	m_ClrSbarBtnPush;
		__u32	m_ClrSbarThmNrm;
		__u32	m_ClrSbarThmFcs;
		__u32	m_ClrSbarThmPush;

		__u32	m_ClrWinPopListWaku;
		__u32	m_ClrWinPopListBg;

		__u32	m_ClrCbxBg;

		__u32	m_ClrBtnT0PushLine;
		__u32	m_ClrBtnT0NrmBg;
		__u32	m_ClrBtnT0DisBg;
		__u32	m_ClrBtnT0FcsBg;

		__u32	m_ClrBtnT1SelMark;

		__u32	m_ClrBtnT2Inner;
		__u32	m_ClrBtnT2OutNrm;
		__u32	m_ClrBtnT2OutFcs;

		__u32	m_ClrBtnT3LnDark;
		__u32	m_ClrBtnT3LnLight;
		__u32	m_ClrBtnT3InNrm;
		__u32	m_ClrBtnT3InFcs;

		__u32	m_ClrTabLblSepLight;

		__u32	m_ClrTbtnBgLineDark;
		__u32	m_ClrTbtnBgLineLight;
		__u32	m_ClrTbtnBgNormal;
		__u32	m_ClrTbtnBgDisable;
		__u32	m_ClrTbtnBgPush;
		__u32	m_ClrTbtnBgFocus;
		__u32	m_ClrTbtnBgSelected;

		__u32	m_ClrPrgsBgLineLight[E_GrP5SkinT2PrgsSkinMaxCnt];
		__u32	m_ClrPrgsBgLineDark[E_GrP5SkinT2PrgsSkinMaxCnt];
		__u32	m_ClrPrgsBgFace[E_GrP5SkinT2PrgsSkinMaxCnt];
		__u32	m_ClrPrgsBgGraph[E_GrP5SkinT2PrgsSkinMaxCnt];

		__u32	m_ClrSpinBgNormal;
		__u32	m_ClrSpinBgDisable;
		__u32	m_ClrSpinBgFocus;
		__u32	m_ClrSpinBgPush;
		__u32	m_ClrSpinCtlMarkNormal;
		__u32	m_ClrSpinCtlMarkFocus;
		__u32	m_ClrSpinCtlMarkPush;

		__u32	m_ClrLblBgLineDark;
		__u32	m_ClrLblBgLineLight;
		__u32	m_ClrLblBg;

		__u32	m_ClrChkBgLineFocus;
		__u32	m_ClrChkMarkBgStd;
		__u32	m_ClrChkMarkBgChk;
		__u32	m_ClrChkMarkChar;

		__u32	m_ClrTitleOutLine;
		__u32	m_ClrTitleBg0;
		__u32	m_ClrTitleBg1;

		__u32	m_ClrBtmOutLine;
		__u32	m_ClrBtmBg0;
		__u32	m_ClrBtmBg1;

		__u32	m_ClrGridCellOutline;
		__u32	m_ClrGridCursor;
		__u32	m_ClrGridCheck;
		__u32	m_ClrGridDrag;
		__u32	m_ClrGridRscdDrag;

		__u32	m_ClrCalLine;
		__u32	m_ClrCalCusNrm;
		__u32	m_ClrCalMark;
		__u32	m_ClrCalSel;

		__u32	m_ClrRatbNone;
		__u32	m_ClrRatbCont;
		__u32	m_ClrRatbMntn;
		__u32	m_ClrRatbSens;

		__u32	m_ClrGrpHzCusHz;
		__u32	m_ClrGrpHzCusVt;

		__u32	m_ClrLblMark;

		__u32	m_ClrAimCross;

		__u32	m_ClrAglLight;

		__u32	m_ClrGrpHzT1IndBg;
		__u32	m_ClrGrpHzT1BgFocus;

		__u32	m_ClrGrpHzT1Non[5];
		__u32	m_ClrGrpHzT1Nrm[5];
		__u32	m_ClrGrpHzT1Mtn[5];
		__u32	m_ClrGrpHzT1Sen[5];
		__u32	m_ClrGrpHzT1Mns[5];
		__u32	m_ClrGrpHzT1Cne[5];

		__u32	m_ClrGridRscdItm[8];

		__u32	m_ClrArwBgFocus;
		__u32	m_ClrArwBgSelect;

		__u32	m_ClrArwBase[3];
		__u32	m_ClrArwLine[4];

		__u32	m_ClrPnlDirNor;
		__u32	m_ClrPnlDirPush;

		__u32	m_ClrArwDirClr;

		__u32	m_ClrWinPopListOnCus;

		__u32	m_ClrTablBorder;

		__u32	m_ClrTblNormal;
		__u32	m_ClrTblSelect;
		__u32	m_ClrTblBorder;

		__u32	m_ClrPnlDragCus;

		St_GrP5SkinT2GradInfo	m_GradLblBg2;

		__u32	m_ClrGrpHzT2RulLine;
		__u32	m_ClrGrpHzT2Cus;

		__u32	m_ClrTooltipBg;

		// control
		__u16		m_CtlClassId;
		St_GrRect	m_CtlRcScr;
		void*		m_CtlUo;
		void*		m_PtrCtlParaUoBase;
		Ptr_GrP5SkinPara	m_PtrCtlSkinPara;
		__u32		m_CtlStat;


	protected:
		void	LcPalleteInit(void);

		void	LcGradeintByTbl(Ptr_GrRect A_PtrRect, Ptr_GrP5SkinT2GradInfo A_PtrGrad);

		void	LcInitSbarVt(Ptr_Grp5SkinSbarCtl A_PtrCtl);

		void	LcDrawTriangleIsosLeft(Ptr_GrRect A_PtrRect, __u32 A_Color);
		void	LcDrawTriangleIsosRight(Ptr_GrRect A_PtrRect, __u32 A_Color);
		void	LcDrawTriangleIsosUp(Ptr_GrRect A_PtrRect, __u32 A_Color);
		void	LcDrawTriangleIsosDown(Ptr_GrRect A_PtrRect, __u32 A_Color);

		//void	LcDrawTriangleRangLu( Ptr_GrRect A_PtrRect, __u32 A_Color);
		//void	LcDrawTriangleRangLd( Ptr_GrRect A_PtrRect, __u32 A_Color);
		//void	LcDrawTriangleRangRu( Ptr_GrRect A_PtrRect, __u32 A_Color);
		void	LcDrawTriangleRangRd(Ptr_GrRect A_PtrRect, __u32 A_Color);

		void	LcDrawVmark(Ptr_GrRect A_PtrRect, __u32 A_Color);

		void	LcDrawAglBrkLt(Ptr_GrRect A_PtrRect, __u32 A_Clr, __u32 A_Width);
		void	LcDrawAglBrkRt(Ptr_GrRect A_PtrRect, __u32 A_Clr, __u32 A_Width);
		void	LcDrawAglBrkUp(Ptr_GrRect A_PtrRect, __u32 A_Clr, __u32 A_Width);
		void	LcDrawAglBrkDn(Ptr_GrRect A_PtrRect, __u32 A_Clr, __u32 A_Width);

		void	LcDrawPrgsBgT0(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect,
			__u8 A_Direction, __u32 A_ValTot, __u32 A_ValNow);
		void	LcDrawPrgsBgT1(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect,
			__u8 A_Direction, __u32 A_ValTot, __u32 A_ValNow);

		void	LcDrawBtnStd(__u32 A_Stat);

		void	LcDrawBtnSubType0(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat);
		void	LcDrawBtnSubType1(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat);
		void	LcDrawBtnSubType2(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat);
		void	LcDrawBtnSubType3(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat);
		void	LcDrawBtnSubType4(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat);

		void	LcDrawArwCrosshair(__s32 A_X, __s32 A_Y);
		void	LcDrawArwStart(__s32 A_X, __s32 A_Y);
		void	LcDrawArwCircle(__s32 A_StX, __s32 A_StY, __s32 A_EdX, __s32 A_EdY, __u32 A_Stat, __u32 A_Val);
		void	LcDrawArwHalfCir(__s32 A_X, __s32 A_Y, __u32 A_Angle, __u32 A_Quadrant, __u32 A_Color);

		void	LcDrawPerpAraw(__s32 A_Dx, __s32 A_Dy, __s32 A_Cx, __s32 A_Cy);
		void	LcDrawArwSideMark(__s32 A_StX, __s32 A_StY, __s32 A_EdX, __s32 A_EdY, __u32 A_DirMap);

		void	LcDrawVct(__s32 A_X, __s32 A_Y, Ptr_GrP5SkinT2VctCmd A_PtrCmd, __u32* A_PtrClrTbl, BOOL8 A_IsFlipX, BOOL8 A_IsFlipY);

		void	LcDrawArwItem(__s32 A_StX, __s32 A_StY, __s32 A_EdX, __s32 A_EdY, __u32 A_ColorType, __u32 A_DirMap);

		void	LcDrawChkBgTop(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat);
		void	LcDrawChkMark(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat);

		void	LcDrawGridSmdItem(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat);
		void	LcDrawGridDrag(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect);
		void	LcDrawGridRscdItem(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat, __u8 A_Val);
		void	LcDrawGridRscdDrag(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect);

		void	LcDrawGraphHzBg(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat);
		void	LcDrawGraphHzItem(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u8 A_Val);
		void	LcDrawGraphHzCursorVt(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat);

		void	LcDrawGrpHzT1BgInd(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat);
		void	LcDrawGrpHzT1BgCont(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat);
		void	LcDrawGrpHzT1Item(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat, __u8 A_Val);

		void	LcDrawAimBg(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat);

		void	LcDrawPrgsBg(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect,
			__u8 A_Direction, __u32 A_ValTot, __u32 A_ValNow);

		void	LcDrawSpinVtBg(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat);
		void	LcDrawSpinVtBtnDec(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat);
		void	LcDrawSpinVtBtnInc(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat);

		void	LcDrawSpinHzBg(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat);
		void	LcDrawSpinHzBtnDec(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat);
		void	LcDrawSpinHzBtnInc(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat);

		void	LcDrawTabLblBg(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat);

		void	LcDrawTitleDivT0(Ptr_GrRect A_PtrRect);

		void	LcDrawGridBox(Ptr_GrRect A_PtrRc, __u32 A_ClrIn, __u32 A_ClrBrd, __u8 A_WidLeft, __u8 A_WidTop, __u8 A_WidRight, __u8 A_WidBottom);

		void	LcDrawSbarVt(Ptr_Grp5SkinSbarCtl A_PtrCtl);

		void	LcDrawCtlGrpHzT1Ruler(void);
		void	LcDrawCtlGrpHzTiCont(void);
		void	LcDrawCtlPnlGrpBarLine(void);
		void	LcDrawCtlPnlGrpBarItemName(void);
		void	LcDrawCtlPnlGrpBarGridVal(void);
		void	LcDrawCtlPnlGrpBarBar(void);
		void	LcDrawCtlPnlGrpLineLine(void);
		void	LcDrawCtlPnlGrpLineColName(void);
		void	LcDrawCtlPnlGrpLineGridVal(void);
		void	LcDrawCtlPnlGrpLineGraph(void);
		void	LcDrawCtlQuadT1ViewArea(void);
		void	LcDrawCtlQuadBaseInnerLine(void);
		void	LcDrawCtlQuadBaseOutLine(void);
		void	LcDrawCtlTblBaseItems(void);
		void	LcDrawCtlGrpHzT2Cont(void);
		void	LcDrawCtlGrpHzT2Ruler(void);
		void	LcDrawCtlGrpHzT2RulTxt(void);
		void	LcDrawCtlGrpHzT2RulLine(void);
		void	LcDrawCtlGrpHzT2Cursor(void);

		void	LcDrawCtlWinT0(void);

		void	LcBtnBaseStatUpdate(void);
		void	LcChkBaseStatUpdate(void);
		void	LcGrpHzT2ContUpdate(void);

		// draw control
		void	LcCtlDrawArwBase(void);
		void	LcCtlDrawArwStd(void);
		void	LcCtlDrawCalMark(void);
		void	LcCtlDrawBtnImgBase(void);
		void	LcCtlDrawBtnImgTid(void);
		void	LcCtlDrawBtnLtxt(void);
		void	LcCtlDrawBtnSint(void);
		void	LcCtlDrawBtnStxt(void);
		void	LcCtlDrawBtnTabBase(void);
		void	LcCtlDrawBtnTabTid(void);
		void	LcCtlDrawBtnTid(void);
		void	LcCtlDrawBtnTime(void);
		void	LcCtlDrawBtnVkey(void);
		void	LcCtlDrawChkImgLtBase(void);
		void	LcCtlDrawChkImgTiBase(void);
		void	LcCtlDrawChkTiBase(void);
		void	LcCtlDrawGridSmd(void);
		void	LcCtlDrawGridSrscd(void);
		void	LcCtlDrawGrpHzDmap(void);
		void	LcCtlDrawGrpHzT1(void);
		void	LcCtlDrawLblBase(void);
		void	LcCtlDrawLblTimeReg(void);
		void	LcCtlDrawLblTimeView(void);
		void	LcCtlDrawPnlAim(void);
		void	LcCtlDrawPnlGrpBar(void);
		void	LcCtlDrawPnlGrpLine(void);
		void	LcCtlDrawPnlStd(void);
		void	LcCtlDrawPrgsBase(void);
		void	LcCtlDrawQuadT1(void);
		void	LcCtlDrawSpinBase(void);
		void	LcCtlDrawTabBase(void);
		void	LcCtlDrawTabTiRel(void);
		void	LcCtlDrawTblBase(void);
		void	LcCtlDrawWinBase(void);
		void	LcCtlDrawPnlDir(void);
		void	LcCtlDrawWinPopList(void);
		void	LcCtlDrawWinPopMenu(void);
		void	LcCtlDrawCbxBase(void);
		void	LcCtlDrawPnlDrag(void);
		void	LcCtlDrawGrpHzT2(void);

		// update control
		void	LcCtlUpdtArwBase(void);
		void	LcCtlUpdtCalBase(void);
		void	LcCtlUpdtGridBase(void);
		void	LcCtlUpdtGrpHzDmap(void);
		void	LcCtlUpdtGrpHzT1(void);
		void	LcCtlUpdtLblTid(void);
		void	LcCtlUpdtPnlGrpBar(void);
		void	LcCtlUpdtPnlGrpLine(void);
		void	LcCtlUpdtQuadT1(void);
		void	LcCtlUpdtSpinBase(void);
		void	LcCtlUpdtTabBase(void);
		void	LcCtlUpdtTabTiRel(void);
		void	LcCtlUpdtTblBase(void);
		void	LcCtlUpdtPnlDir(void);
		void	LcCtlUpdtWinPopMenu(void);
		void	LcCtlUpdtWinPopList(void);
		void	LcCtlUpdtGrpHzT2(void);

		// get item rect
		void	LcCtlItmRectArwBase(__s32 A_ItmStX, __s32 A_ItmStY, __s32 A_ItmEdX, __s32 A_ItmEdY, Ptr_GrRect A_PtrRtRect);
		void	LcCtlItmRectCalBase(__s32 A_ItmStX, __s32 A_ItmStY, __s32 A_ItmEdX, __s32 A_ItmEdY, Ptr_GrRect A_PtrRtRect);
		void	LcCtlItmRectGridBase(__s32 A_ItmStX, __s32 A_ItmStY, __s32 A_ItmEdX, __s32 A_ItmEdY, Ptr_GrRect A_PtrRtRect);
		void	LcCtlItmRectGrpHzDmap(__s32 A_ItmStX, __s32 A_ItmStY, __s32 A_ItmEdX, __s32 A_ItmEdY, Ptr_GrRect A_PtrRtRect);
		void	LcCtlItmRectGrpHzT1(__s32 A_ItmStX, __s32 A_ItmStY, __s32 A_ItmEdX, __s32 A_ItmEdY, Ptr_GrRect A_PtrRtRect);
		void	LcCtlItmRectTblBase(__s32 A_ItmStX, __s32 A_ItmStY, __s32 A_ItmEdX, __s32 A_ItmEdY, Ptr_GrRect A_PtrRtRect);
		void	LcCtlItmRectWinPopList(__s32 A_ItmStX, __s32 A_ItmStY, __s32 A_ItmEdX, __s32 A_ItmEdY, Ptr_GrRect A_PtrRtRect);
		void	LcCtlItmRectWinPopMenu(__s32 A_ItmStX, __s32 A_ItmStY, __s32 A_ItmEdX, __s32 A_ItmEdY, Ptr_GrRect A_PtrRtRect);

		// get item index
		void	LcCtlItemIdxArwBase(__s32 A_X, __s32 A_Y, __s32* A_PtrRtIdxX, __s32* A_PtrRtIdxY);
		void	LcCtlItemIdxCalBase(__s32 A_X, __s32 A_Y, __s32* A_PtrRtIdxX, __s32* A_PtrRtIdxY);
		void	LcCtlItemIdxGridBase(__s32 A_X, __s32 A_Y, __s32* A_PtrRtIdxX, __s32* A_PtrRtIdxY);
		void	LcCtlItemIdxGrpHzDmap(__s32 A_X, __s32 A_Y, __s32* A_PtrRtIdxX, __s32* A_PtrRtIdxY);
		void	LcCtlItemIdxGrpHzT1(__s32 A_X, __s32 A_Y, __s32* A_PtrRtIdxX, __s32* A_PtrRtIdxY);
		void	LcCtlItemIdxSpinBase(__s32 A_X, __s32 A_Y, __s32* A_PtrRtIdxX, __s32* A_PtrRtIdxY);
		void	LcCtlItemIdxTblBase(__s32 A_X, __s32 A_Y, __s32* A_PtrRtIdxX, __s32* A_PtrRtIdxY);
		void	LcCtlItemIdxWinPopList(__s32 A_X, __s32 A_Y, __s32* A_PtrRtIdxX, __s32* A_PtrRtIdxY);
		void	LcCtlItemIdxWinPopMenu(__s32 A_X, __s32 A_Y, __s32* A_PtrRtIdxX, __s32* A_PtrRtIdxY);

		// get size by item count
		void	LcCtlSizeWinPopList(__u32 A_ItemCnt, __u32* A_PtrRtSizeX, __u32* A_PtrRtSizeY);
		void	LcCtlSizeWinPopMenu(__u32 A_ItemCnt, __u32* A_PtrRtSizeX, __u32* A_PtrRtSizeY);

	public:
		Cls_GrP5SkinT2(__u8 A_SkinId);		// for window
		virtual	~Cls_GrP5SkinT2(void);

		// implement function

		// control
		virtual	void	CtlUpdateBase(void* A_ObjUi);

		virtual	void	CtlDrawBase(void* A_ObjUi);

		virtual	void	CtlGetItemRangeRect(void* A_ObjUi, __s32 A_ItmStX, __s32 A_ItmStY, __s32 A_ItmEdX, __s32 A_ItmEdY, Ptr_GrRect A_PtrRtRect);

		virtual	void	CtlGetItemIdxByPos(void* A_ObjUi, __s32 A_X, __s32 A_Y, __s32* A_PtrRtIdxX, __s32* A_PtrRtIdxY);

		virtual	void	CtlGetSizeByItemCnt(__u16 A_ClassId, __u32 A_ItemCnt, __u32* A_PtrRtSizeX, __u32* A_PtrRtSizeY);

		virtual	void	CtlCalcSbarVtByValue(void* A_ObjUi, Ptr_Grp5SkinSbarCtl A_PtrSbarCtl, __u32 A_Value);
		virtual	void	CtlCalcSbarVtByPos(void* A_ObjUi, Ptr_Grp5SkinSbarCtl A_PtrSbarCtl, __u32 A_Pos);

		// tooltip
		virtual	void	TooltipSetup(Ptr_GrRect A_PtrConRect, Ptr_GrRect A_PtrBaseRect, __u16 A_TxtId, Ptr_GrP5SkinTooltipCtl A_PtrRtCtl);	// A_TxtId = 0 - not display
		virtual	void	TooltipDraw(Ptr_GrP5SkinTooltipCtl A_PtrCtl);

};



//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

