/*
Platform 5 skin UDR serise ver 5

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5SkinT1
#define	_EPC_GrP5SkinT1

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5SkinBase.h>

#include <GrGdibObj.h>

//====================================================================
//constance

#define	E_GrP5SkinT1PrgsSkinMaxCnt		4

#define	E_GrP5SkinT1WinBgInnerGap			6
#define	E_GrP5SkinT1WinBgInnerWidth		2

#define	E_GrP5SkinT1WinTitleHeight		42
#define	E_GrP5SkinT1WinBtmHeight			30

#define	E_GrP5SkinT1TabLblSepWidth		4

#define	E_GrP5SkinT1TabBtnFocusWidth	2

#define	E_GrP5SkinT1BtnWakuWidth			2
#define	E_GrP5SkinT1BtnFocusLineWidth	3

#define	E_GrP5SkinT1PrgsWakuWidth			5

#define	E_GrP5SkinT1ListCtrlMargin		1

#define	E_GrP5SkinT1TblMargin					1
#define	E_GrP5SkinT1TblItemMgnLeft		0
#define	E_GrP5SkinT1TblItemMgnTop			0
#define	E_GrP5SkinT1TblItemMgnRight		4
#define	E_GrP5SkinT1TblItemMgnDown		4
#define	E_GrP5SkinT1TblCusWidth				3

#define	E_GrP5SkinT1WinPopListMargin	1

#define	E_GrP5SkinT1CbxMarkGapRt			6
#define	E_GrP5SkinT1CbxMarkGapDn			3
#define	E_GrP5SkinT1CbxMarkSize				10

#define	E_GrP5SkinT1SpinMarginHz			3
#define	E_GrP5SkinT1SpinMarginVt			3
#define	E_GrP5SkinT1SpinCtlMargin			3
#define	E_GrP5SkinT1SpinCtlWidth			3

#define	E_GrP5SkinT1GridOutWidth			1
#define	E_GrP5SkinT1GridCusWidth			5
#define	E_GrP5SkinT1GridMarkWidth			3
#define	E_GrP5SkinT1GridDrageWidth		2

#define	E_GrP5SkinT1GraphHzMargin			3
#define	E_GrP5SkinT1GrpHzT1ContMgn		4

#define	E_GrP5SkinT1CalMargin					3
#define	E_GrP5SkinT1CalCusWidth				3

#define	E_GrP5SkinT1GridRscdAtbHeight	4
#define	E_GrP5SkinT1GridRscdTopMgn		8
#define	E_GrP5SkinT1GridRscdDragWidth	2
#define	E_GrP5SkinT1GridRscdDragRcCnt	4

#define	E_GrP5SkinT1WinPopListWakuWdt	1

#define	E_GrP5SkinT1GridentMaxCnt			7

#define	E_GrP5SkinT1AimCrossWidth			1
#define	E_GrP5SkinT1AimCrossSize			120

#define	E_GrP5SkinT1PnlDirIdcDfltSize	8

// vector command
#define	E_GrP5SkinT1VctCmdEnd					0
#define	E_GrP5SkinT1VctCmdRect				1
#define	E_GrP5SkinT1VctCmdLine				2

//====================================================================
//global macro

//====================================================================
//global type


typedef struct St_GrP5SkinT1GradInfo
{
	__u8	ItemCnt;
	__u8	SizeTbl[E_GrP5SkinT1GridentMaxCnt];
	__u32	ColorTbl[E_GrP5SkinT1GridentMaxCnt];
}	*Ptr_GrP5SkinT1GradInfo;

typedef struct St_GrP5SkinT1VctCmd
{
	__u32	Cmd;
	__u32	ClrIdx;
	St_GrSwordRect	Rc;
}	*Ptr_GrP5SkinT1VctCmd;

//====================================================================
//global class

class Cls_GrP5SkinT1	:	public Cls_GrP5SkinBase
{
	private:

#ifdef GR_P5_EDITOR
		__u32	m_DvlFccClr;
#endif

		// pallete
		__u32	m_ClrWinFaceU0;		// window standard background color
		__u32	m_ClrWinFaceU1;
		__u32	m_ClrWinLineDark;
		__u32	m_ClrWinLineLight;
		__u32	m_ClrWinTitleU1;
		__u32	m_ClrWinLineU1;

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

		__u32	m_ClrPrgsBgLineLight[E_GrP5SkinT1PrgsSkinMaxCnt];
		__u32	m_ClrPrgsBgLineDark[E_GrP5SkinT1PrgsSkinMaxCnt];
		__u32	m_ClrPrgsBgFace[E_GrP5SkinT1PrgsSkinMaxCnt];
		__u32	m_ClrPrgsBgGraph[E_GrP5SkinT1PrgsSkinMaxCnt];

		__u32	m_ClrSbarBtnNormal;
		__u32	m_ClrSbarBtnFocus;
		__u32	m_ClrSbarBtnPush;
		__u32	m_ClrSbarBtnWakuDark;
		__u32	m_ClrSbarBtnWakuLight;
		__u32	m_ClrSbarBtnTriNormal;
		__u32	m_ClrSbarBtnTriFocus;
		__u32	m_ClrSbarBtnTriPush;
		__u32	m_ClrSbarBg;
		__u32	m_ClrSbarWakuDark;
		__u32	m_ClrSbarWakuLight;
		__u32	m_ClrSbarPosWakuDark;
		__u32	m_ClrSbarPosWakuLight;
		__u32	m_ClrSbarPosNormal;
		__u32	m_ClrSbarPosFocus;
		__u32	m_ClrSbarPosPush;
		__u32	m_ClrSbarPosMarkNoraml;
		__u32	m_ClrSbarPosMarkFocus;
		__u32	m_ClrSbarPosMarkPush;

		__u32	m_ClrCbxBgNormal;
		__u32	m_ClrCbxBgDisable;
		__u32	m_ClrCbxBgFocus;
		__u32	m_ClrCbxBgPush;
		__u32	m_ClrCbxWakuDark;
		__u32	m_ClrCbxWakuLight;
		__u32	m_ClrCbxMarkNoraml;
		__u32	m_ClrCbxMarkDisable;
		__u32	m_ClrCbxMarkFocus;
		__u32	m_ClrCbxMarkPush;

		__u32	m_ClrSpinBgNormal;
		__u32	m_ClrSpinBgDisable;
		__u32	m_ClrSpinBgFocus;
		__u32	m_ClrSpinBgPush;
		__u32	m_ClrSpinWakuDark;
		__u32	m_ClrSpinWakuLight;
		__u32	m_ClrSpinCtlWakeDark;
		__u32	m_ClrSpinCtlWakeLight;
		__u32	m_ClrSpinCtlBgLnUp;
		__u32	m_ClrSpinCtlBgLnRt;
		__u32	m_ClrSpinCtlBgLnDn;
		__u32	m_ClrSpinCtlBgLight;
		__u32	m_ClrSpinCtlBgDark;
		__u32	m_ClrSpinCtlMarkNormal;
		__u32	m_ClrSpinCtlMarkDisable;
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

		__u32	m_ClrCalBg;
		__u32	m_ClrCalCusNrm;
		__u32	m_ClrCalCusSel;
		__u32	m_ClrCalMark;
		__u32	m_ClrCalSel;

		__u32	m_ClrRatbNone;
		__u32	m_ClrRatbCont;
		__u32	m_ClrRatbMntn;
		__u32	m_ClrRatbSens;

		__u32	m_ClrGrpHzCusHz;
		__u32	m_ClrGrpHzCusVt;

		__u32	m_ClrLblMark;

		__u32	m_ClrTblBgNormal;
		__u32	m_ClrTblBgFocus;
		__u32	m_ClrTblItemBg;
		__u32	m_ClrTblItemFocus;

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

		__u32	m_ClrGridRscdItm[8][5];

		__u32	m_ClrArwBgFocus;
		__u32	m_ClrArwBgSelect;

		__u32	m_ClrArwBase[3];
		__u32	m_ClrArwLine[4];

		__u32	m_ClrPnlDirNor;
		__u32	m_ClrPnlDirPush;

		__u32	m_ClrArwDirClr;

		St_GrP5SkinT1GradInfo	m_GradA;
		St_GrP5SkinT1GradInfo	m_GradB;
		St_GrP5SkinT1GradInfo	m_GradC;

		St_GrP5SkinT1GradInfo	m_GradLblBg2;

		St_GrP5SkinT1GradInfo	m_GradSlidBg;
		St_GrP5SkinT1GradInfo	m_GradSlidBtn;


		// control
		__u16		m_CtlClassId;
		St_GrRect	m_CtlRcScr;
		void*		m_CtlUo;
		void*		m_PtrCtlParaUoBase;
		Ptr_GrP5SkinPara	m_PtrCtlSkinPara;
		__u32		m_CtlStat;


	protected:
		void	LcPalleteInit(void);

		void	LcGradeintByTbl( Ptr_GrRect A_PtrRect, Ptr_GrP5SkinT1GradInfo A_PtrGrad);

		void	LcDrawTriangleIsosLeft( Ptr_GrRect A_PtrRect, __u32 A_Color);
		void	LcDrawTriangleIsosRight( Ptr_GrRect A_PtrRect, __u32 A_Color);
		void	LcDrawTriangleIsosUp( Ptr_GrRect A_PtrRect, __u32 A_Color);
		void	LcDrawTriangleIsosDown( Ptr_GrRect A_PtrRect, __u32 A_Color);

		//void	LcDrawTriangleRangLu( Ptr_GrRect A_PtrRect, __u32 A_Color);
		//void	LcDrawTriangleRangLd( Ptr_GrRect A_PtrRect, __u32 A_Color);
		//void	LcDrawTriangleRangRu( Ptr_GrRect A_PtrRect, __u32 A_Color);
		void	LcDrawTriangleRangRd( Ptr_GrRect A_PtrRect, __u32 A_Color);

		void	LcDrawVmark( Ptr_GrRect A_PtrRect, __u32 A_Color);

		void	LcDrawU0TitleBar( Ptr_GrRect A_PtrRect);
		void	LcDrawU0BtmBar( Ptr_GrRect A_PtrRect);

		void	LcDrawU1TitleBar( Ptr_GrRect A_PtrRect);
		void	LcDrawU1BtmBar( Ptr_GrRect A_PtrRect);

		void	LcDrawAglBrkLt( Ptr_GrRect A_PtrRect, __u32 A_Clr, __u32 A_Width);
		void	LcDrawAglBrkRt( Ptr_GrRect A_PtrRect, __u32 A_Clr, __u32 A_Width);
		void	LcDrawAglBrkUp( Ptr_GrRect A_PtrRect, __u32 A_Clr, __u32 A_Width);
		void	LcDrawAglBrkDn( Ptr_GrRect A_PtrRect, __u32 A_Clr, __u32 A_Width);

		void	LcDrawPrgsBgT0(Ptr_GrP5SkinPara A_PtrPara,  Ptr_GrRect A_PtrRect,
			__u8 A_Direction, __u32 A_ValTot, __u32 A_ValNow);
		void	LcDrawPrgsBgT1(Ptr_GrP5SkinPara A_PtrPara,  Ptr_GrRect A_PtrRect,
			__u8 A_Direction, __u32 A_ValTot, __u32 A_ValNow);

		void	LcDrawBtnStd(__u32 A_Stat);

		void	LcDrawBtnSubType1(Ptr_GrP5SkinPara A_PtrPara,  Ptr_GrRect A_PtrRect, __u32 A_Stat);
		void	LcDrawBtnSubType2(Ptr_GrP5SkinPara A_PtrPara,  Ptr_GrRect A_PtrRect, __u32 A_Stat);
		void	LcDrawBtnSubType3(Ptr_GrP5SkinPara A_PtrPara,  Ptr_GrRect A_PtrRect, __u32 A_Stat);
		void	LcDrawBtnSubType4(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat);

		void	LcDrawArwCrosshair(__s32 A_X, __s32 A_Y);
		void	LcDrawArwStart(__s32 A_X, __s32 A_Y);
		void	LcDrawArwCircle(__s32 A_StX, __s32 A_StY, __s32 A_EdX, __s32 A_EdY, __u32 A_Stat, __u32 A_Val);
		void	LcDrawArwHalfCir(__s32 A_X, __s32 A_Y, __u32 A_Angle, __u32 A_Quadrant ,__u32 A_Color);

		void	LcDrawPerpAraw(__s32 A_Dx, __s32 A_Dy, __s32 A_Cx, __s32 A_Cy);
		void	LcDrawArwSideMark(__s32 A_StX, __s32 A_StY, __s32 A_EdX, __s32 A_EdY, __u32 A_DirMap);

		void	LcDrawVct(__s32 A_X, __s32 A_Y, Ptr_GrP5SkinT1VctCmd A_PtrCmd, __u32* A_PtrClrTbl, BOOL8 A_IsFlipX, BOOL8 A_IsFlipY);

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

		void	LcBtnBaseStatUpdate(void);
		void	LcChkBaseStatUpdate(void);


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

		// get item rect
		void	LcCtlItmRectArwBase(__s32 A_ItmStX, __s32 A_ItmStY, __s32 A_ItmEdX, __s32 A_ItmEdY, Ptr_GrRect A_PtrRtRect);
		void	LcCtlItmRectCalBase(__s32 A_ItmStX, __s32 A_ItmStY, __s32 A_ItmEdX, __s32 A_ItmEdY, Ptr_GrRect A_PtrRtRect);
		void	LcCtlItmRectGridBase(__s32 A_ItmStX, __s32 A_ItmStY, __s32 A_ItmEdX, __s32 A_ItmEdY, Ptr_GrRect A_PtrRtRect);
		void	LcCtlItmRectGrpHzDmap(__s32 A_ItmStX, __s32 A_ItmStY, __s32 A_ItmEdX, __s32 A_ItmEdY, Ptr_GrRect A_PtrRtRect);
		void	LcCtlItmRectGrpHzT1(__s32 A_ItmStX, __s32 A_ItmStY, __s32 A_ItmEdX, __s32 A_ItmEdY, Ptr_GrRect A_PtrRtRect);
		void	LcCtlItmRectTblBase(__s32 A_ItmStX, __s32 A_ItmStY, __s32 A_ItmEdX, __s32 A_ItmEdY, Ptr_GrRect A_PtrRtRect);

		// get item index
		void	LcCtlItemIdxArwBase(__s32 A_X, __s32 A_Y, __s32* A_PtrRtIdxX, __s32* A_PtrRtIdxY);
		void	LcCtlItemIdxCalBase(__s32 A_X, __s32 A_Y, __s32* A_PtrRtIdxX, __s32* A_PtrRtIdxY);
		void	LcCtlItemIdxGridBase(__s32 A_X, __s32 A_Y, __s32* A_PtrRtIdxX, __s32* A_PtrRtIdxY);
		void	LcCtlItemIdxGrpHzDmap(__s32 A_X, __s32 A_Y, __s32* A_PtrRtIdxX, __s32* A_PtrRtIdxY);
		void	LcCtlItemIdxGrpHzT1(__s32 A_X, __s32 A_Y, __s32* A_PtrRtIdxX, __s32* A_PtrRtIdxY);
		void	LcCtlItemIdxSpinBase(__s32 A_X, __s32 A_Y, __s32* A_PtrRtIdxX, __s32* A_PtrRtIdxY);
		void	LcCtlItemIdxTblBase(__s32 A_X, __s32 A_Y, __s32* A_PtrRtIdxX, __s32* A_PtrRtIdxY);


	public:
		Cls_GrP5SkinT1(__u8 A_SkinId);		// for window
		virtual	~Cls_GrP5SkinT1(void);

		// implement function

		// control
		virtual	void	CtlUpdateBase(void* A_ObjUi);

		virtual	void	CtlDrawBase(void* A_ObjUi);

		virtual	void	CtlGetItemRangeRect(void* A_ObjUi, __s32 A_ItmStX, __s32 A_ItmStY, __s32 A_ItmEdX, __s32 A_ItmEdY, Ptr_GrRect A_PtrRtRect);

		virtual	void	CtlGetItemIdxByPos(void* A_ObjUi, __s32 A_X, __s32 A_Y, __s32* A_PtrRtIdxX, __s32* A_PtrRtIdxY);
};



//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

