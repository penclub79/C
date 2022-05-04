/*
Platform 5 skin Type 1

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5SkinT1.h>

#include <GrDebug.h>
#include <GrStrTool.h>
#include <GrDumyTool.h>
#include <GrImgBase.h>
#include <GrMath.h>
#include <Dvr/GrDvrBase.h>

#include <P5/GrP5Paint.h>
#include <P5/GrP5ConBase.h>

#include <P5/GrP5Mng.h>

#include <P5/GrP5UoBase.h>

#include <P5/GrP5UoArwBase.h>
#include <P5/GrP5UoArwStd.h>
#include <P5/GrP5UoBtnBase.h>
#include <P5/GrP5UoBtnImgBase.h>
#include <P5/GrP5UoBtnImgTid.h>
#include <P5/GrP5UoBtnLtxt.h>
#include <P5/GrP5UoBtnSint.h>
#include <P5/GrP5UoBtnStxt.h>
#include <P5/GrP5UoBtnTabBase.h>
#include <P5/GrP5UoBtnTabTid.h>
#include <P5/GrP5UoBtnTid.h>
#include <P5/GrP5UoBtnTime.h>
#include <P5/GrP5UoBtnVkey.h>
#include <P5/GrP5UoCalBase.h>
#include <P5/GrP5UoCalMark.h>
#include <P5/GrP5UoChkImgLtBase.h>
#include <P5/GrP5UoChkImgTiBase.h>
#include <P5/GrP5UoChkTiBase.h>
#include <P5/GrP5UoGridBase.h>
#include <P5/GrP5UoGridSmd.h>
#include <P5/GrP5UoGridSrscd.h>
#include <P5/GrP5UoGrpHzBase.h>
#include <P5/GrP5UoGrpHzDmap.h>
#include <P5/GrP5UoGrpHzT1.h>
#include <P5/GrP5UoLblBase.h>
#include <P5/GrP5UoLblTid.h>
#include <P5/GrP5UoLblTimeReg.h>
#include <P5/GrP5UoLblTimeView.h>
#include <P5/GrP5UoPnlAim.h>
#include <P5/GrP5UoPnlGrpBar.h>
#include <P5/GrP5UoPnlGrpLine.h>
#include <P5/GrP5UoPnlStd.h>
#include <P5/GrP5UoPrgsBase.h>
#include <P5/GrP5UoQuadBase.h>
#include <P5/GrP5UoQuadT1.h>
#include <P5/GrP5UoSpinBase.h>
#include <P5/GrP5UoTabBase.h>
#include <P5/GrP5UoTabTiRel.h>
#include <P5/GrP5UoTblBase.h>
#include <P5/GrP5WinBase.h>
#include <P5/GrP5UoPnlDir.h>


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

extern	St_GrP5SkinCfg	V_GrP5SkinCfg;

#ifdef GR_P5_EDITOR
extern	WCHAR	V_Cls_GrP5UoTabTiRelStrLaebl[];
#endif

__u8	V_GrP5SkinT1Curb4x[4]	={4,2,1,1};

__u8	V_GrP5SkinT1Curb6x[6]	=	{6, 4, 3, 2, 1, 1};

St_GrSwordRect	V_GrP5SkinT1ArwCrhOutTbl[]	=	
{
	{0, -12, 1, -8}, {0, 9, 1, 13}, {-12, 0, -8, 1}, {9, 0, 13, 1},
	{-3, -8, 4, -7},
	{-5, -7, 6, -6},
	{-6, -6, -2, -5}, {3, -6, 7, -5},
	{-7, -5, -4, -4}, {5, -5, 8, -4},
	{-7, -4, -5, -3}, {6, -4, 8, -3},
	{-8, -3, -5, -2}, {6, -3, 9, -2},
	{-8, -2, -6, 3}, {7, -2, 9, 3},

	{-8, 3, -5, 4}, {6, 3, 9, 4},
	{-7, 4, -5, 5}, {6, 4, 8, 5},
	{-7, 5, -4, 6}, {5, 5, 8, 6},
	{-6, 6, -2, 7}, {3, 6, 7, 7},
	{-5, 7, 6, 8},
	{-3, 8, 4, 9},

	{0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF},
};

St_GrSwordRect	V_GrP5SkinT1ArwStartTbl[]	=
{
	{-2, -6, 3, -5},
	{-4, -5, 5, -4},
	{-5, -4, 6, -2},
	{-6, -2, 7, 3},
	{-5, 3, 6, 5},
	{-4, 5, 5, 6},
	{-2, 6, 3, 7},

	{0x7FFF, 0x7FFF, 0x7FFF, 0x7FFF},
};

St_GrSwordRect	V_GrP5SkinT1ArwCircle[16]	=
{
	{12, -1, 15, 1},		// 0
	{12, -2, 14, 0},		// 1
	{11, -3, 14, -1},		// 2
	{11, -4, 14, -2},		// 3
	{11, -5, 13, -3},		// 4
	{10, -6, 13, -4},		// 5
	{10, -7, 12, -5},		// 6
	{9, -8, 12, -6},		// 7
	{9, -9, 11, -6},		// 8
	{8, -10, 10, -7},		// 9
	{7, -11, 9, -8},		// 10
	{5, -12, 7, -9},		// 11
	{3, -13, 5, -10},		// 12
	{2, -13, 4, -10},		// 13
	{1, -13, 3, -11},		// 14
	{0, -14, 2, -11},		// 15
};

St_GrP5SkinT1VctCmd	V_GrP5SkinT1DirArwA0[]	=
{
	{E_GrP5SkinT1VctCmdRect, 0, {2, -1, 22, 3}},		//	0

	{E_GrP5SkinT1VctCmdLine, 0, {15, -4, 21, 2}},		//	1
	{E_GrP5SkinT1VctCmdLine, 0, {16, -4, 22, -1}},
	{E_GrP5SkinT1VctCmdLine, 0, {16, -5, 22, 1}},
	{E_GrP5SkinT1VctCmdLine, 0, {17, -5, 22, 0}},
	{E_GrP5SkinT1VctCmdLine, 0, {17, -6, 23, 0}},		//	3

	{E_GrP5SkinT1VctCmdLine, 0, {15, 5, 21, -1}},		//	4
	{E_GrP5SkinT1VctCmdLine, 0, {16, 5, 22, -1}},
	{E_GrP5SkinT1VctCmdLine, 0, {16, 6, 22, 0}},		//	5
	{E_GrP5SkinT1VctCmdLine, 0, {17, 6, 23, 0}},
	{E_GrP5SkinT1VctCmdLine, 0, {17, 7, 23, 1}},		//	6

	{E_GrP5SkinT1VctCmdEnd, },		//	6
};

St_GrP5SkinT1VctCmd	V_GrP5SkinT1DirArwA11[]	=
{
	{E_GrP5SkinT1VctCmdLine, 0, {2, -1, 21, -5}},	
	{E_GrP5SkinT1VctCmdLine, 0, {2, 0, 21, -4}},
	{E_GrP5SkinT1VctCmdLine, 0, {2, 1, 21, -3}},
	{E_GrP5SkinT1VctCmdLine, 0, {3, 1, 21, -2}},
	{E_GrP5SkinT1VctCmdLine, 0, {3, 2, 22, -2}},

	{E_GrP5SkinT1VctCmdLine, 0, {14, -7, 22, -2}},
	{E_GrP5SkinT1VctCmdLine, 0, {15, -8, 22, -3}},
	{E_GrP5SkinT1VctCmdLine, 0, {15, -7, 21, -3}},
	{E_GrP5SkinT1VctCmdLine, 0, {15, -9, 22, -4}},

	{E_GrP5SkinT1VctCmdLine, 0, {16, 3, 21, -5}},
	{E_GrP5SkinT1VctCmdLine, 0, {17, 3, 21, -4}},
	{E_GrP5SkinT1VctCmdLine, 0, {18, 3, 22, -4}},
	{E_GrP5SkinT1VctCmdLine, 0, {18, 4, 23, -4}},

	{E_GrP5SkinT1VctCmdEnd, },		
};

St_GrP5SkinT1VctCmd	V_GrP5SkinT1DirArwA23[]	=
{
	{E_GrP5SkinT1VctCmdLine, 0, {2, -1, 20, -9}},
	{E_GrP5SkinT1VctCmdLine, 0, {2, 0, 20, -8}},
	{E_GrP5SkinT1VctCmdLine, 0, {4, 0, 20, -7}},
	{E_GrP5SkinT1VctCmdLine, 0, {3, 1, 21, -7}},
	{E_GrP5SkinT1VctCmdLine, 0, {3, 2, 21, -6}},

	{E_GrP5SkinT1VctCmdLine, 0, {13, -9, 21, -6}},
	{E_GrP5SkinT1VctCmdLine, 0, {13, -10, 22, -7}},
	{E_GrP5SkinT1VctCmdLine, 0, {13, -11, 22, -8}},

	{E_GrP5SkinT1VctCmdLine, 0, {17, 1, 20, -8}},
	{E_GrP5SkinT1VctCmdLine, 0, {18, 1, 21, -8}},
	{E_GrP5SkinT1VctCmdLine, 0, {19, 1, 22, -7}},

	{E_GrP5SkinT1VctCmdEnd, },
};

St_GrP5SkinT1VctCmd	V_GrP5SkinT1DirArwA34[]	=
{
	{E_GrP5SkinT1VctCmdLine, 0, {1, -2, 17, -13}},
	{E_GrP5SkinT1VctCmdLine, 0, {2, -2, 10, -7}},
	{E_GrP5SkinT1VctCmdLine, 0, {7, -4, 17, -11}},
	{E_GrP5SkinT1VctCmdLine, 0, {2, -1, 17, -12}},
	{E_GrP5SkinT1VctCmdLine, 0, {2, 0, 18, -11}},
	{E_GrP5SkinT1VctCmdLine, 0, {3, 0, 18, -10}},
	{E_GrP5SkinT1VctCmdLine, 0, {3, 1, 19, -10}},

	{E_GrP5SkinT1VctCmdLine, 0, {10, -12, 19, -10}},
	{E_GrP5SkinT1VctCmdLine, 0, {10, -13, 19, -11}},
	{E_GrP5SkinT1VctCmdLine, 0, {10, -14, 19, -12}},

	{E_GrP5SkinT1VctCmdLine, 0, {15, -4, 17, -13}},
	{E_GrP5SkinT1VctCmdLine, 0, {16, -4, 18, -12}},
	{E_GrP5SkinT1VctCmdLine, 0, {17, -3, 19, -12}},

	{E_GrP5SkinT1VctCmdEnd, },
};

St_GrP5SkinT1VctCmd	V_GrP5SkinT1DirArwA45[]	=
{
	{E_GrP5SkinT1VctCmdLine, 0, {0, -1, 14, -15}},
	{E_GrP5SkinT1VctCmdLine, 0, {1, -1, 14, -14}},
	{E_GrP5SkinT1VctCmdLine, 0, {1, 0, 15, -14}},
	{E_GrP5SkinT1VctCmdLine, 0, {2, 0, 15, -13}},
	{E_GrP5SkinT1VctCmdLine, 0, {2, 1, 16, -13}},

	{E_GrP5SkinT1VctCmdRect, 0, {7, -15, 14, -12}},

	{E_GrP5SkinT1VctCmdRect, 0, {14, -15, 17, -5}},

	{E_GrP5SkinT1VctCmdEnd, },
};

St_GrP5SkinT1VctCmd	V_GrP5SkinT1DirArwA56[]	=
{
	{E_GrP5SkinT1VctCmdLine, 0, {0, -2, 11, -17}},
	{E_GrP5SkinT1VctCmdLine, 0, {1, -1, 12, -17}},
	{E_GrP5SkinT1VctCmdLine, 0, {2, -1, 13, -16}},
	{E_GrP5SkinT1VctCmdLine, 0, {3, 0, 13, -15}},

	{E_GrP5SkinT1VctCmdLine, 0, {1, -2, 10, -15}},
	{E_GrP5SkinT1VctCmdLine, 0, {6, -6, 11, -15}},
	{E_GrP5SkinT1VctCmdLine, 0, {6, -8, 7, -8}},
	{E_GrP5SkinT1VctCmdLine, 0, {3, -1, 12, -14}},

	{E_GrP5SkinT1VctCmdLine, 0, {4, -16, 13, -18}},
	{E_GrP5SkinT1VctCmdLine, 0, {4, -15, 13, -17}},
	{E_GrP5SkinT1VctCmdLine, 0, {5, -14, 14, -16}},

	{E_GrP5SkinT1VctCmdLine, 0, {11, -18, 13, -9}},
	{E_GrP5SkinT1VctCmdLine, 0, {12, -18, 14, -9}},
	{E_GrP5SkinT1VctCmdLine, 0, {13, -18, 15, -9}},

	{E_GrP5SkinT1VctCmdEnd, },

};

St_GrP5SkinT1VctCmd	V_GrP5SkinT1DirArwA68[]	=
{
	{E_GrP5SkinT1VctCmdLine, 0, {-1, -2, 6, -20}},
	{E_GrP5SkinT1VctCmdLine, 0, {0, -2, 7, -20}},
	{E_GrP5SkinT1VctCmdLine, 0, {1, -1, 8, -19}},
	{E_GrP5SkinT1VctCmdLine, 0, {2, -1, 9, -19}},

	{E_GrP5SkinT1VctCmdLine, 0, {1, -3, 7, -18}},

	{E_GrP5SkinT1VctCmdLine, 0, {0, -17, 8, -21}},
	{E_GrP5SkinT1VctCmdLine, 0, {1, -17, 9, -20}},
	{E_GrP5SkinT1VctCmdLine, 0, {1, -16, 9, -19}},

	{E_GrP5SkinT1VctCmdLine, 0, {7, -20, 10, -12}},
	{E_GrP5SkinT1VctCmdLine, 0, {8, -20, 11, -12}},
	{E_GrP5SkinT1VctCmdLine, 0, {8, -21, 12, -13}},

	{E_GrP5SkinT1VctCmdLine, 0, {11, -13, 10, -15}},

	{E_GrP5SkinT1VctCmdEnd, },

};

St_GrP5SkinT1VctCmd	V_GrP5SkinT1DirArwA79[]	=
{
	{E_GrP5SkinT1VctCmdLine, 0, {-1, -2, 3, -21}},
	{E_GrP5SkinT1VctCmdLine, 0, {0, -2, 4, -21}},
	{E_GrP5SkinT1VctCmdLine, 0, {1, -2, 5, -21}},
	{E_GrP5SkinT1VctCmdLine, 0, {2, -2, 6, -21}},

	{E_GrP5SkinT1VctCmdLine, 0, {-3, -18, 5, -23}},
	{E_GrP5SkinT1VctCmdLine, 0, {-2, -17, 5, -22}},
	{E_GrP5SkinT1VctCmdLine, 0, {-2, -16, 6, -21}},

	{E_GrP5SkinT1VctCmdLine, 0, {3, -22, 8, -14}},
	{E_GrP5SkinT1VctCmdLine, 0, {4, -22, 9, -15}},
	{E_GrP5SkinT1VctCmdLine, 0, {5, -23, 10, -15}},

	{E_GrP5SkinT1VctCmdLine, 0, {-1, -18, 7, -18}},
	{E_GrP5SkinT1VctCmdLine, 0, {8, -15, 7, -16}},

	{E_GrP5SkinT1VctCmdEnd, },

};

St_GrP5SkinT1VctCmd	V_GrP5SkinT1DirArwA90[]	=
{
	{E_GrP5SkinT1VctCmdRect, 0, {-1, -21, 3, -1}},

	{E_GrP5SkinT1VctCmdLine, 0, {-6, -16, 0, -22}},
	{E_GrP5SkinT1VctCmdLine, 0, {-5, -16, 1, -22}},
	{E_GrP5SkinT1VctCmdLine, 0, {-5, -15, 1, -21}},

	{E_GrP5SkinT1VctCmdLine, 0, {0, -21, 6, -15}},
	{E_GrP5SkinT1VctCmdLine, 0, {0, -22, 6, -16}},
	{E_GrP5SkinT1VctCmdLine, 0, {1, -22, 7, -16}},
	
	{E_GrP5SkinT1VctCmdEnd, },

};

WCHAR	V_GrP5SkinT1StrDayTbl[31][3]	=	
{{'1', 0}, {'2', 0}, {'3', 0}, {'4', 0}, {'5', 0}, {'6', 0}, {'7', 0}, {'8', 0}, {'9', 0}, {'1','0', 0},
{'1', '1', 0}, {'1', '2', 0}, {'1', '3', 0}, {'1', '4', 0}, {'1', '5', 0}, {'1', '6', 0}, {'1', '7', 0}, {'1', '8', 0}, {'1', '9', 0}, {'2', '0', 0},
{'2', '1', 0}, {'2', '2', 0}, {'2', '3', 0}, {'2', '4', 0}, {'2', '5', 0}, {'2', '6', 0}, {'2', '7', 0}, {'2', '8', 0}, {'2', '9', 0}, {'3', '0', 0},
{'3', '1', 0}
};

WCHAR	V_GrP5SkinT1StrRulerHrT1[5][3]	=	
{{'0', 0}, {'6', 0}, {'1', '2', 0}, {'1', '8', 0}, {'2', '3', 0},};

WCHAR	V_GrP5SkinT1StrRulerMinT1[7][3]	=
{{'0', 0}, {'1', '0', 0}, {'2', '0', 0}, {'3', '0', 0}, {'4', '0', 0}, {'5', '0', 0}, {'5', '9', 0}, };


//====================================================================
//--------------------------------------------------------------------
Cls_GrP5SkinT1::Cls_GrP5SkinT1(__u8 A_SkinId) :
Cls_GrP5SkinBase(A_SkinId)
{
	// local -------------------
	// code --------------------
		LcPalleteInit();
}
//--------------------------------------------------------------------
Cls_GrP5SkinT1::~Cls_GrP5SkinT1(void)
{
	// local -------------------
	// code --------------------
		

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcPalleteInit(void)
{
	// local -------------------
		__u32	Tv_PxFmt;
	// code --------------------
		Tv_PxFmt	=	((Cls_GrP5Mng*)V_GrP5Env.ObjMng)->PixelFormatGet();

		m_ClrWinFaceU0					=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 44, 113, 209), Tv_PxFmt);
		m_ClrWinFaceU1					=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 2, 2, 150), Tv_PxFmt);

		m_ClrWinLineDark				=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 1, 3, 8), Tv_PxFmt);
		m_ClrWinLineLight				=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 48, 72, 209), Tv_PxFmt);

		m_ClrWinTitleU1					=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 0, 2, 48), Tv_PxFmt);
		m_ClrWinLineU1					=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 245, 246, 255), Tv_PxFmt);

		m_ClrBtnT1SelMark				=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 0, 0, 0), Tv_PxFmt);

		m_ClrBtnT2Inner					=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 255, 255), Tv_PxFmt);
		m_ClrBtnT2OutNrm				=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 180, 180, 180), Tv_PxFmt);
		m_ClrBtnT2OutFcs				=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 0, 192, 255), Tv_PxFmt);

		m_ClrBtnT3LnDark				=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 0, 0, 0), Tv_PxFmt);
		m_ClrBtnT3LnLight				=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 184, 194, 188), Tv_PxFmt);
		m_ClrBtnT3InNrm					=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 252, 252, 252), Tv_PxFmt);
		m_ClrBtnT3InFcs					=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 225, 245, 196), Tv_PxFmt);

		m_ClrTabLblSepLight			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeRgb24(181, 181, 181) | 0xFF000000, Tv_PxFmt);

		m_ClrTbtnBgLineDark		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 181, 181, 181), Tv_PxFmt);
		m_ClrTbtnBgLineLight	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 181, 181, 181), Tv_PxFmt);
		m_ClrTbtnBgNormal			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 153, 153, 153), Tv_PxFmt);
		m_ClrTbtnBgDisable		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 40, 40, 40), Tv_PxFmt);
		m_ClrTbtnBgPush				=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 254, 205, 51), Tv_PxFmt);
		m_ClrTbtnBgFocus			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 254, 205, 51), Tv_PxFmt);
		m_ClrTbtnBgSelected		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 254, 205, 51), Tv_PxFmt);

		m_ClrPrgsBgLineDark[0]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 255, 255), Tv_PxFmt);
		m_ClrPrgsBgLineLight[0]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 255, 255), Tv_PxFmt);
		m_ClrPrgsBgFace[0]			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 90, 90, 90), Tv_PxFmt);
		m_ClrPrgsBgGraph[0]			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 204, 51), Tv_PxFmt);

		m_ClrPrgsBgLineDark[1]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 255, 255), Tv_PxFmt);
		m_ClrPrgsBgLineLight[1]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 255, 255), Tv_PxFmt);
		m_ClrPrgsBgFace[1]			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 237, 242, 253), Tv_PxFmt);
		m_ClrPrgsBgGraph[1]			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 18, 149, 241), Tv_PxFmt);

		m_ClrPrgsBgLineDark[2]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 255, 255), Tv_PxFmt);
		m_ClrPrgsBgLineLight[2]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 255, 255), Tv_PxFmt);
		m_ClrPrgsBgFace[2]			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 90, 90, 90), Tv_PxFmt);
		m_ClrPrgsBgGraph[2]			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 204, 51), Tv_PxFmt);

		m_ClrPrgsBgLineDark[3]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 255, 255), Tv_PxFmt);
		m_ClrPrgsBgLineLight[3]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 255, 255), Tv_PxFmt);
		m_ClrPrgsBgFace[3]			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 90, 90, 90), Tv_PxFmt);
		m_ClrPrgsBgGraph[3]			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 204, 51), Tv_PxFmt);

		m_ClrSbarBtnNormal		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 94, 94, 94), Tv_PxFmt);
		m_ClrSbarBtnFocus			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 94, 94, 94), Tv_PxFmt);
		m_ClrSbarBtnPush			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 4, 148, 197), Tv_PxFmt);
		m_ClrSbarBtnWakuDark	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 80, 80, 80), Tv_PxFmt);
		m_ClrSbarBtnWakuLight	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 80, 80, 80), Tv_PxFmt);
		m_ClrSbarBtnTriNormal	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 220, 220, 220), Tv_PxFmt);
		m_ClrSbarBtnTriFocus	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 90, 200, 251), Tv_PxFmt);
		m_ClrSbarBtnTriPush		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 5, 160, 235), Tv_PxFmt);
		m_ClrSbarBg						=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 51, 51, 51), Tv_PxFmt);
		m_ClrSbarWakuDark			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 80, 80, 80), Tv_PxFmt);
		m_ClrSbarWakuLight		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 80, 80, 80), Tv_PxFmt);
		m_ClrSbarPosNormal		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 94, 94, 94), Tv_PxFmt);
		m_ClrSbarPosFocus			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 94, 94, 94), Tv_PxFmt);
		m_ClrSbarPosPush			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 4, 148, 197), Tv_PxFmt);
		m_ClrSbarPosMarkNoraml	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 220, 220, 220), Tv_PxFmt);
		m_ClrSbarPosMarkFocus		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 90, 200, 251), Tv_PxFmt);
		m_ClrSbarPosMarkPush		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 5, 160, 235), Tv_PxFmt);

		m_ClrCbxBgNormal			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 63, 63, 63), Tv_PxFmt);
		m_ClrCbxBgDisable			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 55, 55, 55), Tv_PxFmt);
		m_ClrCbxBgFocus				=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 206, 49), Tv_PxFmt);
		m_ClrCbxBgPush				=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 206, 49), Tv_PxFmt);
		m_ClrCbxWakuDark			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 80, 80, 80), Tv_PxFmt);
		m_ClrCbxWakuLight			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 90, 90, 90), Tv_PxFmt);
		m_ClrCbxMarkNoraml		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 255, 255), Tv_PxFmt);
		m_ClrCbxMarkDisable		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 180, 180, 180), Tv_PxFmt);
		m_ClrCbxMarkFocus			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 90, 200, 251), Tv_PxFmt);
		m_ClrCbxMarkPush			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 5, 160, 235), Tv_PxFmt);

		m_ClrSpinBgNormal			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 252, 252, 252), Tv_PxFmt);
		m_ClrSpinBgDisable		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 63, 63, 63), Tv_PxFmt);
		m_ClrSpinBgFocus			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 228, 245, 196), Tv_PxFmt);
		m_ClrSpinBgPush				=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 228, 245, 196), Tv_PxFmt);
		m_ClrSpinWakuDark			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 5, 4, 2), Tv_PxFmt);
		m_ClrSpinWakuLight		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 171, 171, 171), Tv_PxFmt);
		m_ClrSpinCtlWakeDark		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 172, 172, 172), Tv_PxFmt);
		m_ClrSpinCtlWakeLight		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 172, 172, 172), Tv_PxFmt);
		m_ClrSpinCtlBgLnUp			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 224, 229), Tv_PxFmt);
		m_ClrSpinCtlBgLnRt			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 199, 73, 48), Tv_PxFmt);
		m_ClrSpinCtlBgLnDn			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 102, 28, 1), Tv_PxFmt);
		m_ClrSpinCtlBgLight			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 122, 125), Tv_PxFmt);
		m_ClrSpinCtlBgDark			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 245, 63, 7), Tv_PxFmt);
		m_ClrSpinCtlMarkNormal		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 5, 4, 2), Tv_PxFmt);
		m_ClrSpinCtlMarkDisable		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 80, 80, 80), Tv_PxFmt);
		m_ClrSpinCtlMarkFocus			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 5, 4, 2), Tv_PxFmt);
		m_ClrSpinCtlMarkPush			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 20, 20, 20), Tv_PxFmt);

		m_ClrLblBgLineDark		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 0, 0, 0), Tv_PxFmt);
		m_ClrLblBgLineLight		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 90, 90, 90), Tv_PxFmt);
		m_ClrLblBg						=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 255, 255), Tv_PxFmt);
		m_ClrLblMark					=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 0, 156, 206), Tv_PxFmt);

		m_ClrChkBgLineFocus		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 204, 51), Tv_PxFmt);
		m_ClrChkMarkBgStd			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 153, 153, 153), Tv_PxFmt);
		m_ClrChkMarkBgChk			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 204, 50), Tv_PxFmt);
		m_ClrChkMarkChar			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 51, 51, 51), Tv_PxFmt);

		m_ClrTitleOutLine			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 128, 128, 128), Tv_PxFmt);
		m_ClrTitleBg0					=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 112, 112, 112), Tv_PxFmt);
		m_ClrTitleBg1					=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 93, 93, 93), Tv_PxFmt);

		m_ClrBtmOutLine				=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 88, 88, 88), Tv_PxFmt);
		m_ClrBtmBg0						=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 61, 61, 61), Tv_PxFmt);
		m_ClrBtmBg1						=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 47, 47, 47), Tv_PxFmt);

		m_ClrGridCellOutline	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 120, 120, 120), Tv_PxFmt);
		m_ClrGridCursor				=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 0, 210, 255), Tv_PxFmt);
		m_ClrGridCheck				=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 0, 210, 255), Tv_PxFmt);
		m_ClrGridDrag					=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 210, 0), Tv_PxFmt);
		m_ClrGridRscdDrag			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 0, 0, 255), Tv_PxFmt);

		m_ClrCalBg						=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 63, 63, 63), Tv_PxFmt);
		m_ClrCalCusNrm				=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 254, 205, 51), Tv_PxFmt);
		m_ClrCalCusSel				=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 174, 255, 0), Tv_PxFmt);
		m_ClrCalMark					=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 236, 143, 68), Tv_PxFmt);
		m_ClrCalSel						=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 140, 140, 140), Tv_PxFmt);

		m_ClrRatbNone					=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 66, 66, 66), Tv_PxFmt);
		m_ClrRatbCont					=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 206, 255, 0), Tv_PxFmt);
		m_ClrRatbMntn					=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 57, 148, 255), Tv_PxFmt);
		m_ClrRatbSens					=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 74, 74), Tv_PxFmt);

		m_ClrGrpHzCusHz				=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 189, 0), Tv_PxFmt);
		m_ClrGrpHzCusVt				=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 189, 0), Tv_PxFmt);

		m_ClrTblBgNormal			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 16, 132, 172), Tv_PxFmt);
		m_ClrTblBgFocus				=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 204, 51), Tv_PxFmt);
		m_ClrTblItemBg				=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 63, 63, 63), Tv_PxFmt);
		m_ClrTblItemFocus			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 204, 51), Tv_PxFmt);

		m_ClrAimCross					=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 63, 63, 63), Tv_PxFmt);

		m_ClrAglLight					=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 250, 219, 192), Tv_PxFmt);

		m_ClrGrpHzT1IndBg			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 230, 230, 0), Tv_PxFmt);
		m_ClrGrpHzT1BgFocus		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 133, 252, 254), Tv_PxFmt);

		m_ClrGrpHzT1Non[0]		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 158, 155, 168), Tv_PxFmt);
		m_ClrGrpHzT1Non[1]		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 198, 199, 195), Tv_PxFmt);
		m_ClrGrpHzT1Non[2]		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 252, 252, 250), Tv_PxFmt);
		m_ClrGrpHzT1Non[3]		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 211, 212, 207), Tv_PxFmt);
		m_ClrGrpHzT1Non[4]		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 156, 155, 146), Tv_PxFmt);

		m_ClrGrpHzT1Nrm[0]		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 250, 0, 0), Tv_PxFmt);
		m_ClrGrpHzT1Nrm[1]		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 250, 40, 40), Tv_PxFmt);
		m_ClrGrpHzT1Nrm[2]		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 250, 80, 80), Tv_PxFmt);
		m_ClrGrpHzT1Nrm[3]		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 250, 40, 40), Tv_PxFmt);
		m_ClrGrpHzT1Nrm[4]		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 250, 0, 0), Tv_PxFmt);

		m_ClrGrpHzT1Mtn[0]		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 0, 250, 0), Tv_PxFmt);
		m_ClrGrpHzT1Mtn[1]		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 40, 250, 40), Tv_PxFmt);
		m_ClrGrpHzT1Mtn[2]		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 80, 250, 80), Tv_PxFmt);
		m_ClrGrpHzT1Mtn[3]		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 40, 250, 40), Tv_PxFmt);
		m_ClrGrpHzT1Mtn[4]		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 0, 250, 0), Tv_PxFmt);

		m_ClrGrpHzT1Sen[0]		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 250, 250, 0), Tv_PxFmt);
		m_ClrGrpHzT1Sen[1]		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 250, 250, 40), Tv_PxFmt);
		m_ClrGrpHzT1Sen[2]		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 250, 250, 80), Tv_PxFmt);
		m_ClrGrpHzT1Sen[3]		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 250, 250, 40), Tv_PxFmt);
		m_ClrGrpHzT1Sen[4]		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 250, 250, 0), Tv_PxFmt);

		m_ClrGrpHzT1Mns[0]		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 250, 0, 250), Tv_PxFmt);
		m_ClrGrpHzT1Mns[1]		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 250, 40, 250), Tv_PxFmt);
		m_ClrGrpHzT1Mns[2]		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 250, 80, 250), Tv_PxFmt);
		m_ClrGrpHzT1Mns[3]		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 250, 40, 250), Tv_PxFmt);
		m_ClrGrpHzT1Mns[4]		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 250, 0, 250), Tv_PxFmt);

		m_ClrGrpHzT1Cne[0]		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 0, 0, 250), Tv_PxFmt);
		m_ClrGrpHzT1Cne[1]		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 40, 40, 250), Tv_PxFmt);
		m_ClrGrpHzT1Cne[2]		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 80, 80, 250), Tv_PxFmt);
		m_ClrGrpHzT1Cne[3]		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 40, 40, 250), Tv_PxFmt);
		m_ClrGrpHzT1Cne[4]		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 0, 0, 250), Tv_PxFmt);

		m_ClrGridRscdItm[0][0]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 144, 144, 144), Tv_PxFmt);
		m_ClrGridRscdItm[0][1]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 144, 144, 144), Tv_PxFmt);
		m_ClrGridRscdItm[0][2]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 144, 144, 144), Tv_PxFmt);
		m_ClrGridRscdItm[0][3]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 144, 144, 144), Tv_PxFmt);
		m_ClrGridRscdItm[0][4]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 144, 144, 144), Tv_PxFmt);

		m_ClrGridRscdItm[1][0]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 248, 0, 0), Tv_PxFmt);
		m_ClrGridRscdItm[1][1]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 248, 0, 0), Tv_PxFmt);
		m_ClrGridRscdItm[1][2]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 248, 0, 0), Tv_PxFmt);
		m_ClrGridRscdItm[1][3]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 248, 0, 0), Tv_PxFmt);
		m_ClrGridRscdItm[1][4]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 248, 0, 0), Tv_PxFmt);

		m_ClrGridRscdItm[2][0]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 16, 248, 0), Tv_PxFmt);
		m_ClrGridRscdItm[2][1]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 16, 248, 0), Tv_PxFmt);
		m_ClrGridRscdItm[2][2]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 16, 248, 0), Tv_PxFmt);
		m_ClrGridRscdItm[2][3]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 16, 248, 0), Tv_PxFmt);
		m_ClrGridRscdItm[2][4]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 16, 248, 0), Tv_PxFmt);

		m_ClrGridRscdItm[3][0]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 209, 247, 0), Tv_PxFmt);
		m_ClrGridRscdItm[3][1]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 233, 255, 107), Tv_PxFmt);
		m_ClrGridRscdItm[3][2]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 255, 255), Tv_PxFmt);
		m_ClrGridRscdItm[3][3]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 200, 238, 0), Tv_PxFmt);
		m_ClrGridRscdItm[3][4]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 121, 194, 4), Tv_PxFmt);

		m_ClrGridRscdItm[4][0]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 230, 230, 0), Tv_PxFmt);
		m_ClrGridRscdItm[4][1]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 230, 230, 0), Tv_PxFmt);
		m_ClrGridRscdItm[4][2]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 230, 230, 0), Tv_PxFmt);
		m_ClrGridRscdItm[4][3]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 230, 230, 0), Tv_PxFmt);
		m_ClrGridRscdItm[4][4]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 230, 230, 0), Tv_PxFmt);

		m_ClrGridRscdItm[5][0]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 250, 60, 10), Tv_PxFmt);
		m_ClrGridRscdItm[5][1]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 250, 60, 10), Tv_PxFmt);
		m_ClrGridRscdItm[5][2]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 250, 60, 10), Tv_PxFmt);
		m_ClrGridRscdItm[5][3]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 250, 60, 10), Tv_PxFmt);
		m_ClrGridRscdItm[5][4]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 250, 60, 10), Tv_PxFmt);

		m_ClrGridRscdItm[6][0]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 248, 0, 250), Tv_PxFmt);
		m_ClrGridRscdItm[6][1]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 248, 0, 250), Tv_PxFmt);
		m_ClrGridRscdItm[6][2]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 248, 0, 250), Tv_PxFmt);
		m_ClrGridRscdItm[6][3]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 248, 0, 250), Tv_PxFmt);
		m_ClrGridRscdItm[6][4]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 248, 0, 250), Tv_PxFmt);

		m_ClrGridRscdItm[7][0]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 10, 50, 250), Tv_PxFmt);
		m_ClrGridRscdItm[7][1]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 10, 50, 250), Tv_PxFmt);
		m_ClrGridRscdItm[7][2]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 10, 50, 250), Tv_PxFmt);
		m_ClrGridRscdItm[7][3]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 10, 50, 250), Tv_PxFmt);
		m_ClrGridRscdItm[7][4]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 10, 50, 250), Tv_PxFmt);

		// type A	15 3 5 6
		m_GradA.ItemCnt		=	6;
		m_GradA.ColorTbl[0]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 187, 236, 252), Tv_PxFmt);
		m_GradA.SizeTbl[0]	=	15;
		m_GradA.ColorTbl[1]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 89, 187, 247), Tv_PxFmt);
		m_GradA.SizeTbl[1]	=	3;
		m_GradA.ColorTbl[2]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 50, 157, 250), Tv_PxFmt);
		m_GradA.SizeTbl[2]	=	3;
		m_GradA.ColorTbl[3]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 8, 128, 255), Tv_PxFmt);
		m_GradA.SizeTbl[3]	=	3;
		m_GradA.ColorTbl[4]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 0, 118, 252), Tv_PxFmt);
		m_GradA.SizeTbl[4]	=	3;
		m_GradA.ColorTbl[5]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 0, 123, 255), Tv_PxFmt);
		m_GradA.SizeTbl[5]	=	3;


		m_GradB.ItemCnt		=	6;
		m_GradB.ColorTbl[0]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 44, 104, 207), Tv_PxFmt);
		m_GradB.SizeTbl[0]	=	4;
		m_GradB.ColorTbl[1]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 44, 96, 212), Tv_PxFmt);
		m_GradB.SizeTbl[1]	=	4;
		m_GradB.ColorTbl[2]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 44, 88, 209), Tv_PxFmt);
		m_GradB.SizeTbl[2]	=	4;
		m_GradB.ColorTbl[3]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 44, 80, 207), Tv_PxFmt);
		m_GradB.SizeTbl[3]	=	4;
		m_GradB.ColorTbl[4]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 44, 72, 207), Tv_PxFmt);
		m_GradB.SizeTbl[4]	=	4;
		m_GradB.ColorTbl[5]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 44, 64, 207), Tv_PxFmt);
		m_GradB.SizeTbl[5]	=	8;

		m_GradC.ItemCnt		=	6;
		m_GradC.ColorTbl[0]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 0, 0, 136), Tv_PxFmt);
		m_GradC.SizeTbl[0]	=	8;
		m_GradC.ColorTbl[1]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 2, 2, 128), Tv_PxFmt);
		m_GradC.SizeTbl[1]	=	4;
		m_GradC.ColorTbl[2]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 5, 2, 112), Tv_PxFmt);
		m_GradC.SizeTbl[2]	=	4;
		m_GradC.ColorTbl[3]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 4, 1, 96), Tv_PxFmt);
		m_GradC.SizeTbl[3]	=	4;
		m_GradC.ColorTbl[4]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 4, 1, 80), Tv_PxFmt);
		m_GradC.SizeTbl[4]	=	4;
		m_GradC.ColorTbl[5]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 4, 1, 64), Tv_PxFmt);
		m_GradC.SizeTbl[5]	=	4;

		
		m_GradLblBg2.ItemCnt		=	7;
		m_GradLblBg2.ColorTbl[0]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 196, 209, 245), Tv_PxFmt);
		m_GradLblBg2.SizeTbl[0]	=	2;
		m_GradLblBg2.ColorTbl[1]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 161, 188, 247), Tv_PxFmt);
		m_GradLblBg2.SizeTbl[1]	=	3;
		m_GradLblBg2.ColorTbl[2]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 133, 165, 255), Tv_PxFmt);
		m_GradLblBg2.SizeTbl[2]	=	4;
		m_GradLblBg2.ColorTbl[3]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 43, 120, 252), Tv_PxFmt);
		m_GradLblBg2.SizeTbl[3]	=	4;
		m_GradLblBg2.ColorTbl[4]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 5, 66, 250), Tv_PxFmt);
		m_GradLblBg2.SizeTbl[4]	=	4;
		m_GradLblBg2.ColorTbl[5]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 2, 56, 217), Tv_PxFmt);
		m_GradLblBg2.SizeTbl[5]	=	4;
		m_GradLblBg2.ColorTbl[6]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 0, 41, 156), Tv_PxFmt);
		m_GradLblBg2.SizeTbl[6]	=	4;

		m_GradSlidBg.ItemCnt		=	7;
		m_GradSlidBg.ColorTbl[0]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 255, 253), Tv_PxFmt);
		m_GradSlidBg.SizeTbl[0]	=	2;
		m_GradSlidBg.ColorTbl[1]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 211, 245, 247), Tv_PxFmt);
		m_GradSlidBg.SizeTbl[1]	=	1;
		m_GradSlidBg.ColorTbl[2]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 161, 242, 245), Tv_PxFmt);
		m_GradSlidBg.SizeTbl[2]	=	7;
		m_GradSlidBg.ColorTbl[3]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 71, 187, 245), Tv_PxFmt);
		m_GradSlidBg.SizeTbl[3]	=	2;
		m_GradSlidBg.ColorTbl[4]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 25, 113, 245), Tv_PxFmt);
		m_GradSlidBg.SizeTbl[4]	=	4;
		m_GradSlidBg.ColorTbl[5]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 76, 184, 245), Tv_PxFmt);
		m_GradSlidBg.SizeTbl[5]	=	4;
		m_GradSlidBg.ColorTbl[6]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 118, 241, 245), Tv_PxFmt);
		m_GradSlidBg.SizeTbl[6]	=	4;

		m_GradSlidBtn.ItemCnt		=	7;
		m_GradSlidBtn.ColorTbl[0]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 244, 255, 0), Tv_PxFmt);
		m_GradSlidBtn.SizeTbl[0]	=	2;
		m_GradSlidBtn.ColorTbl[1]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 240, 0), Tv_PxFmt);
		m_GradSlidBtn.SizeTbl[1]	=	1;
		m_GradSlidBtn.ColorTbl[2]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 231, 0), Tv_PxFmt);
		m_GradSlidBtn.SizeTbl[2]	=	7;
		m_GradSlidBtn.ColorTbl[3]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 88, 0), Tv_PxFmt);
		m_GradSlidBtn.SizeTbl[3]	=	2;
		m_GradSlidBtn.ColorTbl[4]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 45, 0), Tv_PxFmt);
		m_GradSlidBtn.SizeTbl[4]	=	4;
		m_GradSlidBtn.ColorTbl[5]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 137, 0), Tv_PxFmt);
		m_GradSlidBtn.SizeTbl[5]	=	4;
		m_GradSlidBtn.ColorTbl[6]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 252, 224, 5), Tv_PxFmt);
		m_GradSlidBtn.SizeTbl[6]	=	4;

		m_ClrArwBgFocus		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 255, 0), Tv_PxFmt);
		m_ClrArwBgSelect	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 0, 255, 64), Tv_PxFmt);

		m_ClrArwBase[0]		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 0, 155, 255), Tv_PxFmt);
		m_ClrArwBase[1]		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 180, 0), Tv_PxFmt);
		m_ClrArwBase[2]		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 0, 0), Tv_PxFmt);

		m_ClrArwLine[0]		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 0, 255, 0), Tv_PxFmt);
		m_ClrArwLine[1]		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 0, 155, 255), Tv_PxFmt);
		m_ClrArwLine[2]		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 180, 0), Tv_PxFmt);
		m_ClrArwLine[3]		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 0, 0), Tv_PxFmt);

		m_ClrPnlDirNor		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 255, 255), Tv_PxFmt);
		m_ClrPnlDirPush		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 255, 0), Tv_PxFmt);
		
		m_ClrArwDirClr		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 0, 155, 255), Tv_PxFmt);
}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcGradeintByTbl( Ptr_GrRect A_PtrRect, Ptr_GrP5SkinT1GradInfo A_PtrGrad)
{
	// local -------------------
		St_GrRect	Tv_Rc;
		__u8	Tv_WkIdx;
	// code --------------------
		// init
		Tv_Rc.left		=	A_PtrRect->left;
		Tv_Rc.top			=	A_PtrRect->top;
		Tv_Rc.right		=	A_PtrRect->right;

		if(0 != A_PtrGrad->ItemCnt)
		{
			for( Tv_WkIdx = 0; Tv_WkIdx < A_PtrGrad->ItemCnt; Tv_WkIdx++)
			{
				Tv_Rc.bottom	=	Tv_Rc.top + (__s32)(A_PtrGrad->SizeTbl[Tv_WkIdx]);
				if(Tv_Rc.bottom > A_PtrRect->bottom)
				{
					break;
				}
				// draw
				GrP5PntReqFillRect(&Tv_Rc, A_PtrGrad->ColorTbl[Tv_WkIdx]);
				// next
				Tv_Rc.top		=	Tv_Rc.bottom;
			}

			if(Tv_Rc.top < A_PtrRect->bottom)
			{
				Tv_Rc.bottom	=	A_PtrRect->bottom;
				GrP5PntReqFillRect(&Tv_Rc, A_PtrGrad->ColorTbl[A_PtrGrad->ItemCnt - 1]);
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawTabLblBg(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat)
{
	// local -------------------
		St_GrRect	Tv_RcDraw;
	// code --------------------
		Tv_RcDraw.left		=	A_PtrRect->left;
		Tv_RcDraw.top			=	A_PtrRect->bottom - E_GrP5SkinT1TabLblSepWidth;
		Tv_RcDraw.right		=	A_PtrRect->right;
		Tv_RcDraw.bottom	=	A_PtrRect->bottom;

		GrP5PntArrangeByRect(&Tv_RcDraw);

		GrP5PntReqFillRect(&Tv_RcDraw, m_ClrTabLblSepLight);
}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawBtnSubType1(Ptr_GrP5SkinPara A_PtrPara,  Ptr_GrRect A_PtrRect, __u32 A_Stat)
{
	// local -------------------
		St_GrRect	Tv_Rc;
	// code --------------------
		if(0 != (E_GrP5StatOnSelected & A_Stat))
		{
			// draw mark
			// up
			Tv_Rc.left		=	A_PtrRect->left;
			Tv_Rc.top			=	A_PtrRect->top;
			Tv_Rc.right		=	A_PtrRect->right;
			Tv_Rc.bottom	=	A_PtrRect->top + 2;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrBtnT1SelMark);
			// bottom
			Tv_Rc.top			=	A_PtrRect->bottom - 2;
			Tv_Rc.bottom	=	A_PtrRect->bottom;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrBtnT1SelMark);
			// left
			Tv_Rc.top			=	A_PtrRect->top + 2;
			Tv_Rc.right		=	A_PtrRect->left + 2;
			Tv_Rc.bottom	=	A_PtrRect->bottom - 2;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrBtnT1SelMark);
			// right
			Tv_Rc.left		=	A_PtrRect->right - 2;
			Tv_Rc.right		=	A_PtrRect->right;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrBtnT1SelMark);
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawBtnSubType2(Ptr_GrP5SkinPara A_PtrPara,  Ptr_GrRect A_PtrRect, __u32 A_Stat)
{
	// local -------------------
		St_GrRect	Tv_Rc;
		__u32	Tv_ClrOut;
		St_GrRect	Tv_RcIn;
		__u32	Tv_WkIdx;
		__u32	Tv_Cidx;
	// code --------------------
		// virtual keyboard type
		if((18 < (A_PtrRect->right - A_PtrRect->left)) && (13 < (A_PtrRect->bottom - A_PtrRect->top)))
		{
			Tv_Rc.left		=	A_PtrRect->left;
			Tv_Rc.top			=	A_PtrRect->top + 6;
			Tv_Rc.right		=	A_PtrRect->right;
			Tv_Rc.bottom	=	A_PtrRect->bottom - 6;
			GrP5PntArrangeByRect(&Tv_Rc);

			Tv_ClrOut	=	m_ClrBtnT2OutNrm;
			if(0 != (E_GrP5StatOnFocus & A_Stat))
			{
				Tv_ClrOut	=	m_ClrBtnT2OutFcs;
			}

			// calculate inner
			Tv_RcIn.left		=	A_PtrRect->left + 5;
			Tv_RcIn.right		=	A_PtrRect->right - 5;
			Tv_RcIn.top			=	A_PtrRect->top + 2;
			Tv_RcIn.bottom	=	A_PtrRect->bottom - 5;

			Tv_Rc.bottom		=	A_PtrRect->top;
			// top
			Tv_Rc.top			=	Tv_Rc.bottom;
			Tv_Rc.bottom	=	Tv_Rc.top + 1;
			Tv_Rc.left		=	A_PtrRect->left + (__s32)(__u32)V_GrP5SkinT1Curb6x[0];
			Tv_Rc.right		=	A_PtrRect->right - + (__s32)(__u32)V_GrP5SkinT1Curb6x[0];
			GrP5PntReqFillRect(&Tv_Rc, Tv_ClrOut);

			Tv_Rc.top			=	Tv_Rc.bottom;
			Tv_Rc.bottom	=	Tv_Rc.top + 1;
			Tv_Rc.left		=	A_PtrRect->left + (__s32)(__u32)V_GrP5SkinT1Curb6x[1];
			Tv_Rc.right		=	A_PtrRect->right - + (__s32)(__u32)V_GrP5SkinT1Curb6x[1];
			GrP5PntReqFillRect(&Tv_Rc, Tv_ClrOut);

			// top out and inner
			for(Tv_WkIdx = 0; Tv_WkIdx < 4; Tv_WkIdx++)
			{
				Tv_Rc.top			=	Tv_Rc.bottom;
				Tv_Rc.bottom	=	Tv_Rc.top + 1;

				Tv_Rc.left		=	A_PtrRect->left + (__s32)(__u32)V_GrP5SkinT1Curb6x[Tv_WkIdx + 2];
				Tv_Rc.right		=	Tv_RcIn.left + (__s32)(__u32)V_GrP5SkinT1Curb4x[Tv_WkIdx];
				GrP5PntReqFillRect(&Tv_Rc, Tv_ClrOut);

				Tv_Rc.left		=	Tv_Rc.right;
				Tv_Rc.right		=	Tv_RcIn.right - (__s32)(__u32)V_GrP5SkinT1Curb4x[Tv_WkIdx];
				GrP5PntReqFillRect(&Tv_Rc, m_ClrBtnT2Inner);

				Tv_Rc.left		=	Tv_Rc.right;
				Tv_Rc.right		=	A_PtrRect->right - (__s32)(__u32)V_GrP5SkinT1Curb6x[Tv_WkIdx + 2];
				GrP5PntReqFillRect(&Tv_Rc, Tv_ClrOut);
			}

			// body
			Tv_Rc.top			=	Tv_Rc.bottom;
			Tv_Rc.bottom	=	A_PtrRect->bottom - 9;
			Tv_Rc.left		=	A_PtrRect->left;
			Tv_Rc.right		=	Tv_RcIn.left;
			GrP5PntReqFillRect(&Tv_Rc, Tv_ClrOut);
			Tv_Rc.left		=	Tv_Rc.right;
			Tv_Rc.right		=	Tv_RcIn.right;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrBtnT2Inner);
			Tv_Rc.left		=	Tv_Rc.right;
			Tv_Rc.right		=	A_PtrRect->right;
			GrP5PntReqFillRect(&Tv_Rc, Tv_ClrOut);

			// bottom with inner
			Tv_Cidx		=	3;
			for(Tv_WkIdx = 0; Tv_WkIdx < 3; Tv_WkIdx++)
			{
				Tv_Rc.top			=	Tv_Rc.bottom;
				Tv_Rc.bottom	=	Tv_Rc.top + 1;

				Tv_Rc.left		=	A_PtrRect->left;
				Tv_Rc.right		=	Tv_RcIn.left + (__s32)(__u32)V_GrP5SkinT1Curb4x[Tv_Cidx];
				GrP5PntReqFillRect(&Tv_Rc, Tv_ClrOut);

				Tv_Rc.left		=	Tv_Rc.right;
				Tv_Rc.right		=	Tv_RcIn.right - (__s32)(__u32)V_GrP5SkinT1Curb4x[Tv_Cidx];
				GrP5PntReqFillRect(&Tv_Rc, m_ClrBtnT2Inner);

				Tv_Rc.left		=	Tv_Rc.right;
				Tv_Rc.right		=	A_PtrRect->right;
				GrP5PntReqFillRect(&Tv_Rc, Tv_ClrOut);
				// next
				Tv_Cidx --;
			}

			// common
			Tv_Rc.top			=	Tv_Rc.bottom;
			Tv_Rc.bottom	=	Tv_Rc.top + 1;

			Tv_Rc.left		=	A_PtrRect->left + (__s32)(__u32)V_GrP5SkinT1Curb6x[5];
			Tv_Rc.right		=	Tv_RcIn.left + (__s32)(__u32)V_GrP5SkinT1Curb4x[0];
			GrP5PntReqFillRect(&Tv_Rc, Tv_ClrOut);

			Tv_Rc.left		=	Tv_Rc.right;
			Tv_Rc.right		=	Tv_RcIn.right - (__s32)(__u32)V_GrP5SkinT1Curb4x[0];
			GrP5PntReqFillRect(&Tv_Rc, m_ClrBtnT2Inner);

			Tv_Rc.left		=	Tv_Rc.right;
			Tv_Rc.right		=	A_PtrRect->right - (__s32)(__u32)V_GrP5SkinT1Curb6x[5];
			GrP5PntReqFillRect(&Tv_Rc, Tv_ClrOut);

			// out only
			Tv_Cidx		=	4;
			for(Tv_WkIdx = 0; Tv_WkIdx < 5; Tv_WkIdx++)
			{
				Tv_Rc.top			=	Tv_Rc.bottom;
				Tv_Rc.bottom	=	Tv_Rc.top + 1;

				Tv_Rc.left		=	A_PtrRect->left + (__s32)(__u32)V_GrP5SkinT1Curb6x[Tv_Cidx];
				Tv_Rc.right		=	A_PtrRect->right - (__s32)(__u32)V_GrP5SkinT1Curb6x[Tv_Cidx];
				GrP5PntReqFillRect(&Tv_Rc, Tv_ClrOut);
				// next
				Tv_Cidx --;
			}

		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawBtnSubType3(Ptr_GrP5SkinPara A_PtrPara,  Ptr_GrRect A_PtrRect, __u32 A_Stat)
{
	// local -------------------
		St_GrRect	Tv_Rc;
		__u32	Tv_Clr;
	// code --------------------

		GrP5PntArrangeByRect(A_PtrRect);

		// out line top
		Tv_Rc.left		=	A_PtrRect->left;
		Tv_Rc.top			=	A_PtrRect->top;
		Tv_Rc.right		=	A_PtrRect->right;
		Tv_Rc.bottom	=	A_PtrRect->top + 1;
		GrP5PntReqFillRect(&Tv_Rc, m_ClrBtnT3LnDark);
		Tv_Rc.left ++;
		Tv_Rc.top	++;
		Tv_Rc.right	--;
		Tv_Rc.bottom ++;
		GrP5PntReqFillRect(&Tv_Rc, m_ClrBtnT3LnDark);
		// out line left
		Tv_Rc.left		=	A_PtrRect->left;
		Tv_Rc.top			=	A_PtrRect->top + 1;
		Tv_Rc.right		=	A_PtrRect->left + 1;
		Tv_Rc.bottom	=	A_PtrRect->bottom;
		GrP5PntReqFillRect(&Tv_Rc, m_ClrBtnT3LnDark);
		Tv_Rc.left ++;
		Tv_Rc.top	++;
		Tv_Rc.right	++;
		Tv_Rc.bottom --;
		GrP5PntReqFillRect(&Tv_Rc, m_ClrBtnT3LnDark);
		// out line right
		Tv_Rc.left		=	A_PtrRect->right - 1;
		Tv_Rc.top			=	A_PtrRect->top + 1;
		Tv_Rc.right		=	A_PtrRect->right;
		Tv_Rc.bottom	=	A_PtrRect->bottom;
		GrP5PntReqFillRect(&Tv_Rc, m_ClrBtnT3LnDark);
		Tv_Rc.left --;
		Tv_Rc.top	++;
		Tv_Rc.right	--;
		Tv_Rc.bottom --;
		GrP5PntReqFillRect(&Tv_Rc, m_ClrBtnT3LnDark);
		// out line bottom
		Tv_Rc.left		=	A_PtrRect->left + 1;
		Tv_Rc.top			=	A_PtrRect->bottom - 1;
		Tv_Rc.right		=	A_PtrRect->right - 1;
		Tv_Rc.bottom	=	A_PtrRect->bottom;
		GrP5PntReqFillRect(&Tv_Rc, m_ClrBtnT3LnLight);
		Tv_Rc.left ++;
		Tv_Rc.top	--;
		Tv_Rc.right	--;
		Tv_Rc.bottom --;
		GrP5PntReqFillRect(&Tv_Rc, m_ClrBtnT3LnLight);

		// inner
		Tv_Clr		=	m_ClrBtnT3InNrm;
		if(0 != (E_GrP5StatOnFocus & A_Stat))
		{
			Tv_Clr		=	m_ClrBtnT3InFcs;
		}
		Tv_Rc.left		=	A_PtrRect->left + 2;
		Tv_Rc.top			=	A_PtrRect->top + 2;
		Tv_Rc.right		=	A_PtrRect->right - 2;
		Tv_Rc.bottom	=	A_PtrRect->bottom - 2;
		GrP5PntReqFillRect(&Tv_Rc, Tv_Clr);

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawBtnSubType4(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat)
{
	// local -------------------
	// code --------------------
		if(0 == (E_GrP5StatOnChecked & A_Stat))
		{
			GrP5PntReqFillRect(A_PtrRect, A_PtrPara->ParaDword);
		}
		else
		{
			GrP5PntReqFillRect(A_PtrRect, A_PtrPara->ParaColor);
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawPrgsBgT0(Ptr_GrP5SkinPara A_PtrPara,  Ptr_GrRect A_PtrRect,
	__u8 A_Direction, __u32 A_ValTot, __u32 A_ValNow)
{
	// local -------------------
		St_GrRect	Tv_Rc;
	// code --------------------

		GrP5PntArrangeByRect(A_PtrRect);

		switch(A_Direction)
		{
			case	E_GrWayUp:
				// graph
				Tv_Rc.left		=	A_PtrRect->left;
				Tv_Rc.top			=	A_PtrRect->top;
				Tv_Rc.right		=	A_PtrRect->right;
				Tv_Rc.bottom	=	A_PtrRect->bottom;
				if((0 != A_ValTot) && (A_ValTot >= A_ValNow))
				{
					Tv_Rc.top		=	A_PtrRect->bottom - (((A_PtrRect->bottom - A_PtrRect->top) * (__s32)A_ValNow) / (__s32)A_ValTot);
				}
				GrP5PntReqFillRect(&Tv_Rc, m_ClrPrgsBgGraph[0]);

				// background
				if(Tv_Rc.top > A_PtrRect->top)
				{
					Tv_Rc.bottom	=	Tv_Rc.top;
					Tv_Rc.top			=	A_PtrRect->top;
					GrP5PntReqFillRect(&Tv_Rc, m_ClrPrgsBgFace[0]);
				}
				break;
			case E_GrWayDown:
				// graph
				Tv_Rc.left		=	A_PtrRect->left;
				Tv_Rc.top			=	A_PtrRect->top;
				Tv_Rc.right		=	A_PtrRect->right;
				Tv_Rc.bottom	=	A_PtrRect->bottom;
				if((0 != A_ValTot) && (A_ValTot >= A_ValNow))
				{
					Tv_Rc.bottom	=	A_PtrRect->top + (((A_PtrRect->bottom - A_PtrRect->top) * (__s32)A_ValNow) / (__s32)A_ValTot);
				}
				GrP5PntReqFillRect(&Tv_Rc, m_ClrPrgsBgGraph[0]);

				// background
				if(Tv_Rc.bottom < A_PtrRect->bottom)
				{
					Tv_Rc.top			=	Tv_Rc.bottom;
					Tv_Rc.bottom	=	A_PtrRect->bottom;
					GrP5PntReqFillRect(&Tv_Rc, m_ClrPrgsBgFace[0]);
				}
				break;
			case E_GrWayLeft:
				// graph
				Tv_Rc.left		=	A_PtrRect->left;
				Tv_Rc.top			=	A_PtrRect->top;
				Tv_Rc.right		=	A_PtrRect->right;
				Tv_Rc.bottom	=	A_PtrRect->bottom;
				if((0 != A_ValTot) && (A_ValTot >= A_ValNow))
				{
					Tv_Rc.left	=	A_PtrRect->right - (((A_PtrRect->right - A_PtrRect->left) * (__s32)A_ValNow) / (__s32)A_ValTot);
				}
				GrP5PntReqFillRect(&Tv_Rc, m_ClrPrgsBgGraph[0]);

				// background
				if(Tv_Rc.left > A_PtrRect->left)
				{
					Tv_Rc.right		=	Tv_Rc.left;
					Tv_Rc.left		=	A_PtrRect->left;
					GrP5PntReqFillRect(&Tv_Rc, m_ClrPrgsBgFace[0]);
				}
				break;
			default:
				// graph
				Tv_Rc.left		=	A_PtrRect->left;
				Tv_Rc.top			=	A_PtrRect->top;
				Tv_Rc.right		=	A_PtrRect->right;
				Tv_Rc.bottom	=	A_PtrRect->bottom;
				if((0 != A_ValTot) && (A_ValTot >= A_ValNow))
				{
					Tv_Rc.right	=	A_PtrRect->left + (((A_PtrRect->right - A_PtrRect->left) * (__s32)A_ValNow) / (__s32)A_ValTot);
				}
				GrP5PntReqFillRect(&Tv_Rc, m_ClrPrgsBgGraph[0]);

				// background
				if(Tv_Rc.right < A_PtrRect->right)
				{
					Tv_Rc.left		=	Tv_Rc.right;
					Tv_Rc.right		=	A_PtrRect->right;
					GrP5PntReqFillRect(&Tv_Rc, m_ClrPrgsBgFace[0]);
				}
				break;
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawPrgsBgT1(Ptr_GrP5SkinPara A_PtrPara,  Ptr_GrRect A_PtrRect,
	__u8 A_Direction, __u32 A_ValTot, __u32 A_ValNow)
{
	// local -------------------
		St_GrRect	Tv_Rc;
	// code --------------------
		switch(A_Direction)
		{
			case	E_GrWayUp:
				// graph
				Tv_Rc.left		=	A_PtrRect->left;
				Tv_Rc.top			=	A_PtrRect->top;
				Tv_Rc.right		=	A_PtrRect->right;
				Tv_Rc.bottom	=	A_PtrRect->bottom;
				if((0 != A_ValTot) && (A_ValTot >= A_ValNow))
				{
					Tv_Rc.top		=	A_PtrRect->bottom - (((A_PtrRect->bottom - A_PtrRect->top) * (__s32)A_ValNow) / (__s32)A_ValTot);
				}
				GrP5PntReqFillRect(&Tv_Rc, m_ClrPrgsBgGraph[1]);

				// background
				if(Tv_Rc.top > A_PtrRect->top)
				{
					Tv_Rc.bottom	=	Tv_Rc.top;
					Tv_Rc.top			=	A_PtrRect->top;
					Tv_Rc.left		=	Tv_Rc.left + E_GrP5SkinT1PrgsWakuWidth;
					Tv_Rc.right		=	Tv_Rc.right - E_GrP5SkinT1PrgsWakuWidth;
					GrP5PntReqFillRect(&Tv_Rc, m_ClrPrgsBgFace[1]);
				}
				break;
			case E_GrWayDown:
				// graph
				Tv_Rc.left		=	A_PtrRect->left;
				Tv_Rc.top			=	A_PtrRect->top;
				Tv_Rc.right		=	A_PtrRect->right;
				Tv_Rc.bottom	=	A_PtrRect->bottom;
				if((0 != A_ValTot) && (A_ValTot >= A_ValNow))
				{
					Tv_Rc.bottom	=	A_PtrRect->top + (((A_PtrRect->bottom - A_PtrRect->top) * (__s32)A_ValNow) / (__s32)A_ValTot);
				}
				GrP5PntReqFillRect(&Tv_Rc, m_ClrPrgsBgGraph[1]);

				// background
				if(Tv_Rc.bottom < A_PtrRect->bottom)
				{
					Tv_Rc.top			=	Tv_Rc.bottom;
					Tv_Rc.bottom	=	A_PtrRect->bottom;
					Tv_Rc.left		=	Tv_Rc.left + E_GrP5SkinT1PrgsWakuWidth;
					Tv_Rc.right		=	Tv_Rc.right - E_GrP5SkinT1PrgsWakuWidth;
					GrP5PntReqFillRect(&Tv_Rc, m_ClrPrgsBgFace[1]);
				}
				break;
			case E_GrWayLeft:
				// graph
				Tv_Rc.left		=	A_PtrRect->left;
				Tv_Rc.top			=	A_PtrRect->top;
				Tv_Rc.right		=	A_PtrRect->right;
				Tv_Rc.bottom	=	A_PtrRect->bottom;
				if((0 != A_ValTot) && (A_ValTot >= A_ValNow))
				{
					Tv_Rc.left	=	A_PtrRect->right - (((A_PtrRect->right - A_PtrRect->left) * (__s32)A_ValNow) / (__s32)A_ValTot);
				}
				GrP5PntReqFillRect(&Tv_Rc, m_ClrPrgsBgGraph[1]);

				// background
				if(Tv_Rc.left > A_PtrRect->left)
				{
					Tv_Rc.right		=	Tv_Rc.left;
					Tv_Rc.left		=	A_PtrRect->left;
					Tv_Rc.top			=	Tv_Rc.top + E_GrP5SkinT1PrgsWakuWidth;
					Tv_Rc.bottom	=	Tv_Rc.bottom - E_GrP5SkinT1PrgsWakuWidth;
					GrP5PntReqFillRect(&Tv_Rc, m_ClrPrgsBgFace[1]);
				}
				break;
			default:
				// graph
				Tv_Rc.left		=	A_PtrRect->left;
				Tv_Rc.top			=	A_PtrRect->top;
				Tv_Rc.right		=	A_PtrRect->right;
				Tv_Rc.bottom	=	A_PtrRect->bottom;
				if((0 != A_ValTot) && (A_ValTot >= A_ValNow))
				{
					Tv_Rc.right	=	A_PtrRect->left + (((A_PtrRect->right - A_PtrRect->left) * (__s32)A_ValNow) / (__s32)A_ValTot);
				}
				GrP5PntReqFillRect(&Tv_Rc, m_ClrPrgsBgGraph[1]);

				// background
				if(Tv_Rc.right < A_PtrRect->right)
				{
					Tv_Rc.left		=	Tv_Rc.right;
					Tv_Rc.right		=	A_PtrRect->right;
					Tv_Rc.top			=	Tv_Rc.top + E_GrP5SkinT1PrgsWakuWidth;
					Tv_Rc.bottom	=	Tv_Rc.bottom - E_GrP5SkinT1PrgsWakuWidth;
					GrP5PntReqFillRect(&Tv_Rc, m_ClrPrgsBgFace[1]);
				}
				break;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawPrgsBg(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect,
	__u8 A_Direction, __u32 A_ValTot, __u32 A_ValNow)
{
	// local -------------------
		
	// code --------------------
		switch(A_PtrPara->SubType)
		{
			case 0:
				LcDrawPrgsBgT0(A_PtrPara, A_PtrRect, A_Direction, A_ValTot, A_ValNow);
				break;
			case 1:
				LcDrawPrgsBgT1(A_PtrPara, A_PtrRect, A_Direction, A_ValTot, A_ValNow);
				break;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawTriangleIsosUp( Ptr_GrRect A_PtrRect, __u32 A_Color)
{
	// local -------------------
		__s32	Tv_BtmSize;
		__s32	Tv_Height;
		__s32	Tv_X;
		__s32	Tv_Y;
	// code --------------------

		Tv_BtmSize	=	A_PtrRect->right - A_PtrRect->left;
		Tv_Height		=	A_PtrRect->bottom - A_PtrRect->top;
		if((0 < Tv_BtmSize) && (0 < Tv_Height) )
		{
			Tv_BtmSize	=	((Tv_BtmSize + 1) & 0xFFFFFFFE) - 1;
			if(((Tv_BtmSize + 1) / 2) > Tv_Height)
			{
				Tv_BtmSize	=	(Tv_Height * 2) - 1;
			}

			// draw
			Tv_X		=	A_PtrRect->left;
			Tv_Y		=	A_PtrRect->bottom - 1;

			while(0 < Tv_BtmSize)
			{
				GrP5PntReqLine(Tv_X, Tv_Y, Tv_X + Tv_BtmSize - 1, Tv_Y, A_Color);
				// next
				Tv_X ++;
				Tv_Y --;
				Tv_BtmSize	=	Tv_BtmSize - 2;
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawTriangleIsosDown( Ptr_GrRect A_PtrRect, __u32 A_Color)
{
	// local -------------------
		__s32	Tv_BtmSize;
		__s32	Tv_Height;
		__s32	Tv_X;
		__s32	Tv_Y;
	// code --------------------
		Tv_BtmSize	=	A_PtrRect->right - A_PtrRect->left;
		Tv_Height		=	A_PtrRect->bottom - A_PtrRect->top;
		if((0 < Tv_BtmSize) && (0 < Tv_Height))
		{
			Tv_BtmSize	=	((Tv_BtmSize + 1) & 0xFFFFFFFE) - 1;
			if(((Tv_BtmSize + 1) / 2) > Tv_Height)
			{
				Tv_BtmSize	=	(Tv_Height * 2) - 1;
			}

			// draw
			Tv_X		=	A_PtrRect->left;
			Tv_Y		=	A_PtrRect->top;

			while(0 < Tv_BtmSize)
			{
				GrP5PntReqLine(Tv_X, Tv_Y, Tv_X + Tv_BtmSize - 1, Tv_Y, A_Color);
				// next
				Tv_X ++;
				Tv_Y ++;
				Tv_BtmSize	=	Tv_BtmSize - 2;
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawTriangleIsosLeft( Ptr_GrRect A_PtrRect, __u32 A_Color)
{
	// local -------------------
		__s32	Tv_BtmSize;
		__s32	Tv_Height;
		__s32	Tv_X;
		__s32	Tv_Y;
	// code --------------------
		Tv_BtmSize	=	A_PtrRect->right - A_PtrRect->left;
		Tv_Height		=	A_PtrRect->bottom - A_PtrRect->top;
		if((0 < Tv_BtmSize) && (0 < Tv_Height))
		{
			Tv_Height	=	((Tv_Height + 1) & 0xFFFFFFFE) - 1;
			if(((Tv_Height + 1) / 2) > Tv_BtmSize)
			{
				Tv_Height	=	(Tv_BtmSize * 2) - 1;
			}

			// draw
			Tv_X		=	A_PtrRect->right - 1;
			Tv_Y		=	A_PtrRect->top + (((A_PtrRect->bottom - A_PtrRect->top) - Tv_Height) >> 1);

			while(0 < Tv_Height)
			{
				// draw light
				GrP5PntReqPixel(Tv_X, Tv_Y, m_ClrAglLight);
				// draw dark
				Tv_Y ++;
				Tv_Height --;
				if(0 < Tv_Height)
				{
					GrP5PntReqLine(Tv_X, Tv_Y, Tv_X, Tv_Y + Tv_Height, A_Color);
				}
				// next
				Tv_X --;
				Tv_Height --;
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawTriangleIsosRight( Ptr_GrRect A_PtrRect, __u32 A_Color)
{
	// local -------------------
		__s32	Tv_BtmSize;
		__s32	Tv_Height;
		__s32	Tv_X;
		__s32	Tv_Y;
	// code --------------------
		Tv_BtmSize	=	A_PtrRect->right - A_PtrRect->left;
		Tv_Height		=	A_PtrRect->bottom - A_PtrRect->top;
		if((0 < Tv_BtmSize) && (0 < Tv_Height))
		{
			Tv_Height	=	((Tv_Height + 1) & 0xFFFFFFFE) - 1;
			if(((Tv_Height + 1) / 2) > Tv_BtmSize)
			{
				Tv_Height	=	(Tv_BtmSize * 2) - 1;
			}

			// draw
			Tv_X		=	A_PtrRect->left;
			Tv_Y		=	A_PtrRect->top + (((A_PtrRect->bottom - A_PtrRect->top) - Tv_Height) >> 1);

			while(0 < Tv_Height)
			{
				// draw light
				GrP5PntReqPixel(Tv_X, Tv_Y, m_ClrAglLight);
				// draw dark
				Tv_Y ++;
				Tv_Height --;
				if(0 < Tv_Height)
				{
					GrP5PntReqLine(Tv_X, Tv_Y, Tv_X, Tv_Y + Tv_Height, A_Color);
				}
				// next
				Tv_X ++;
				Tv_Height	--;
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawTriangleRangRd( Ptr_GrRect A_PtrRect, __u32 A_Color)
{
	// local -------------------
		__s32	Tv_BtmSize;
		__s32	Tv_Height;
		__s32	Tv_X;
		__s32	Tv_Y;
	// code --------------------
		Tv_BtmSize	=	A_PtrRect->right - A_PtrRect->left;
		Tv_Height		=	A_PtrRect->bottom - A_PtrRect->top;
		if((0 < Tv_BtmSize) && (0 < Tv_Height))
		{
			if(Tv_BtmSize > Tv_Height)
			{
				Tv_BtmSize	=	Tv_Height;
			}

			// draw
			Tv_X		=	A_PtrRect->right - Tv_BtmSize;
			Tv_Y		=	A_PtrRect->bottom - 1;

			while(0 < Tv_BtmSize)
			{
				GrP5PntReqLine(Tv_X, Tv_Y, Tv_X + Tv_BtmSize, Tv_Y, A_Color);
				// next
				Tv_X ++;
				Tv_Y --;
				Tv_BtmSize --;
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawSpinHzBg(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat)
{
	// local -------------------
		St_GrRect	Tv_Rc;
		__u32	Tv_Color;
	// code --------------------

		GrP5PntArrangeByRect(A_PtrRect);

		// out line top
		Tv_Rc.left		=	A_PtrRect->left;
		Tv_Rc.top			=	A_PtrRect->top;
		Tv_Rc.right		=	A_PtrRect->right;
		Tv_Rc.bottom	=	A_PtrRect->top + 1;
		GrP5PntReqFillRect(&Tv_Rc, m_ClrSpinWakuDark);
		Tv_Rc.left ++;
		Tv_Rc.top	++;
		Tv_Rc.right	--;
		Tv_Rc.bottom ++;
		GrP5PntReqFillRect(&Tv_Rc, m_ClrSpinWakuDark);
		// out line left
		Tv_Rc.left		=	A_PtrRect->left;
		Tv_Rc.top			=	A_PtrRect->top + 1;
		Tv_Rc.right		=	A_PtrRect->left + 1;
		Tv_Rc.bottom	=	A_PtrRect->bottom;
		GrP5PntReqFillRect(&Tv_Rc, m_ClrSpinWakuDark);
		Tv_Rc.left ++;
		Tv_Rc.top	++;
		Tv_Rc.right	++;
		Tv_Rc.bottom --;
		GrP5PntReqFillRect(&Tv_Rc, m_ClrSpinWakuDark);
		// out line right
		Tv_Rc.left		=	A_PtrRect->right - 1;
		Tv_Rc.top			=	A_PtrRect->top + 1;
		Tv_Rc.right		=	A_PtrRect->right;
		Tv_Rc.bottom	=	A_PtrRect->bottom;
		GrP5PntReqFillRect(&Tv_Rc, m_ClrSpinWakuDark);
		Tv_Rc.left --;
		Tv_Rc.top	++;
		Tv_Rc.right	--;
		Tv_Rc.bottom --;
		GrP5PntReqFillRect(&Tv_Rc, m_ClrSpinWakuDark);
		// out line bottom
		Tv_Rc.left		=	A_PtrRect->left + 1;
		Tv_Rc.top			=	A_PtrRect->bottom - 1;
		Tv_Rc.right		=	A_PtrRect->right - 1;
		Tv_Rc.bottom	=	A_PtrRect->bottom;
		GrP5PntReqFillRect(&Tv_Rc, m_ClrSpinWakuLight);
		Tv_Rc.left ++;
		Tv_Rc.top	--;
		Tv_Rc.right	--;
		Tv_Rc.bottom --;
		GrP5PntReqFillRect(&Tv_Rc, m_ClrSpinWakuLight);

		// center
		Tv_Rc.left		=	A_PtrRect->left + 19;
		Tv_Rc.top			=	A_PtrRect->top + 2;
		Tv_Rc.right		=	A_PtrRect->right - 19;
		Tv_Rc.bottom	=	A_PtrRect->bottom - 2;

		Tv_Color			=	m_ClrSpinBgNormal;
		if(E_GrP5StatDisable == (E_GrP5StatConstMask & A_Stat))
		{
			Tv_Color	=	m_ClrSpinBgDisable;
		}
		else if(E_GrP5StatPush == (E_GrP5StatConstMask & A_Stat))
		{
			Tv_Color	=	m_ClrSpinBgPush;
		}
		else if(0 != (E_GrP5StatOnSelected & A_Stat))
		{
			Tv_Color	=	m_ClrSpinBgPush;
		}
		else if(0 != (E_GrP5StatOnFocus & A_Stat))
		{
			Tv_Color	=	m_ClrSpinBgPush;
		}

		GrP5PntReqFillRect(&Tv_Rc, Tv_Color);

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawSpinHzBtnDec(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat)
{
	// local -------------------
		__u32	Tv_Clr;
		St_GrRect	Tv_Rc;
	// code --------------------

		// background
		Tv_Rc.left		=	A_PtrRect->left;
		Tv_Rc.top			=	A_PtrRect->top;
		Tv_Rc.right		=	A_PtrRect->right - 1;
		Tv_Rc.bottom	=	Tv_Rc.top + 1;
		GrP5PntReqFillRect(&Tv_Rc, m_ClrSpinCtlBgLnUp);

		Tv_Rc.left		=	A_PtrRect->right - 1;
		Tv_Rc.top			=	A_PtrRect->top;
		Tv_Rc.right		=	A_PtrRect->right;
		Tv_Rc.bottom	=	A_PtrRect->bottom;
		GrP5PntReqFillRect(&Tv_Rc, m_ClrSpinCtlBgLnRt);

		Tv_Rc.left		=	A_PtrRect->left;
		Tv_Rc.top			=	A_PtrRect->bottom - 1;
		Tv_Rc.right		=	A_PtrRect->right - 1;
		Tv_Rc.bottom	=	A_PtrRect->bottom;
		GrP5PntReqFillRect(&Tv_Rc, m_ClrSpinCtlBgLnDn);

		Tv_Rc.left		=	A_PtrRect->left;
		Tv_Rc.top			=	A_PtrRect->top + 1;
		Tv_Rc.right		=	A_PtrRect->right - 1;
		Tv_Rc.bottom	=	A_PtrRect->top + ((A_PtrRect->bottom - A_PtrRect->top) >> 2);
		GrP5PntReqFillRect(&Tv_Rc, m_ClrSpinCtlBgLight);

		Tv_Rc.left		=	A_PtrRect->left;
		Tv_Rc.top			=	Tv_Rc.bottom;
		Tv_Rc.right		=	A_PtrRect->right - 1;
		Tv_Rc.bottom	=	A_PtrRect->bottom - 1;
		GrP5PntReqFillRect(&Tv_Rc, m_ClrSpinCtlBgDark);

		// draw mark
		Tv_Rc.left		=	A_PtrRect->left + 2;
		Tv_Rc.top			=	A_PtrRect->top	 + 4;
		Tv_Rc.right		=	A_PtrRect->right - 4;
		Tv_Rc.bottom	=	A_PtrRect->bottom - 4;

		Tv_Clr	=	m_ClrSpinCtlMarkNormal;
		if(E_GrP5StatDisable == (E_GrP5StatConstMask & A_Stat))
		{
			Tv_Clr	=	m_ClrSpinCtlMarkDisable;
		}
		else if(E_GrP5StatPush == (E_GrP5StatConstMask & A_Stat))
		{
			Tv_Clr	=	m_ClrSpinCtlMarkPush;
		}
		else if(0 != (E_GrP5StatOnFocus & A_Stat))
		{
			Tv_Clr	=	m_ClrSpinCtlMarkFocus;
		}

		LcDrawTriangleIsosLeft( &Tv_Rc, Tv_Clr);

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawSpinHzBtnInc(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat)
{
	// local -------------------
		__u32	Tv_Clr;
		St_GrRect	Tv_Rc;
	// code --------------------

		// background
		Tv_Rc.left		=	A_PtrRect->left;
		Tv_Rc.top			=	A_PtrRect->top;
		Tv_Rc.right		=	A_PtrRect->right - 1;
		Tv_Rc.bottom	=	Tv_Rc.top + 1;
		GrP5PntReqFillRect(&Tv_Rc, m_ClrSpinCtlBgLnUp);

		Tv_Rc.left		=	A_PtrRect->right - 1;
		Tv_Rc.top			=	A_PtrRect->top;
		Tv_Rc.right		=	A_PtrRect->right;
		Tv_Rc.bottom	=	A_PtrRect->bottom;
		GrP5PntReqFillRect(&Tv_Rc, m_ClrSpinCtlBgLnRt);

		Tv_Rc.left		=	A_PtrRect->left;
		Tv_Rc.top			=	A_PtrRect->bottom - 1;
		Tv_Rc.right		=	A_PtrRect->right - 1;
		Tv_Rc.bottom	=	A_PtrRect->bottom;
		GrP5PntReqFillRect(&Tv_Rc, m_ClrSpinCtlBgLnDn);

		Tv_Rc.left		=	A_PtrRect->left;
		Tv_Rc.top			=	A_PtrRect->top + 1;
		Tv_Rc.right		=	A_PtrRect->right - 1;
		Tv_Rc.bottom	=	A_PtrRect->top + ((A_PtrRect->bottom - A_PtrRect->top) >> 2);
		GrP5PntReqFillRect(&Tv_Rc, m_ClrSpinCtlBgLight);

		Tv_Rc.left		=	A_PtrRect->left;
		Tv_Rc.top			=	Tv_Rc.bottom;
		Tv_Rc.right		=	A_PtrRect->right - 1;
		Tv_Rc.bottom	=	A_PtrRect->bottom - 1;
		GrP5PntReqFillRect(&Tv_Rc, m_ClrSpinCtlBgDark);

		// draw mark
		Tv_Rc.left		=	A_PtrRect->left + 4;
		Tv_Rc.top			=	A_PtrRect->top	 + 4;
		Tv_Rc.right		=	A_PtrRect->right - 2;
		Tv_Rc.bottom	=	A_PtrRect->bottom - 4;

		Tv_Clr	=	m_ClrSpinCtlMarkNormal;
		if(E_GrP5StatDisable == (E_GrP5StatConstMask & A_Stat))
		{
			Tv_Clr	=	m_ClrSpinCtlMarkDisable;
		}
		else if(E_GrP5StatPush == (E_GrP5StatConstMask & A_Stat))
		{
			Tv_Clr	=	m_ClrSpinCtlMarkPush;
		}
		else if(0 != (E_GrP5StatOnFocus & A_Stat))
		{
			Tv_Clr	=	m_ClrSpinCtlMarkFocus;
		}

		LcDrawTriangleIsosRight( &Tv_Rc, Tv_Clr);

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawSpinVtBg(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat)
{
	// local -------------------
		St_GrRect	Tv_RcBg;
		__u32	Tv_Clr;
	// code --------------------
		GrP5PntArrangeByRect(A_PtrRect);

		// draw background out line
		GrP5PntReqDrawRect(A_PtrRect, m_ClrSpinWakuDark);

		// draw background color
		Tv_RcBg.left		=	A_PtrRect->left + 1;
		Tv_RcBg.top			=	A_PtrRect->top + 1;
		Tv_RcBg.right		=	A_PtrRect->right - 1;
		Tv_RcBg.bottom	=	A_PtrRect->bottom - 1;

		if((Tv_RcBg.left < Tv_RcBg.right) && (Tv_RcBg.top < Tv_RcBg.bottom))
		{
			Tv_Clr	=	m_ClrSpinBgNormal;
			if(E_GrP5StatDisable == (E_GrP5StatConstMask & A_Stat))
			{
				Tv_Clr	=	m_ClrSpinBgDisable;
			}
			else if(E_GrP5StatPush == (E_GrP5StatConstMask & A_Stat))
			{
				Tv_Clr	=	m_ClrSpinBgPush;
			}
			else if(0 != (E_GrP5StatOnFocus & A_Stat))
			{
				Tv_Clr	=	m_ClrSpinBgFocus;
			}

			if(0 != (E_GrP5StatOnSelected & A_Stat))
			{
				Tv_Clr	=	m_ClrSpinBgPush;
			}

			GrP5PntReqFillRect(&Tv_RcBg, Tv_Clr);
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawSpinVtBtnDec(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat)
{
	// local -------------------
		//St_GrRect	Tv_Rc;
		//__u32	Tv_Clr;
		//St_GrRect	Tv_RcCtl;
	// code --------------------
		GrP5PntReqDrawRect(A_PtrRect, m_ClrSpinCtlWakeDark);

#if 0
		Tv_Rc.left		=	A_PtrRect->left + 1;
		Tv_Rc.top			=	A_PtrRect->top + 1;
		Tv_Rcg.right		=	A_PtrRect->right - 1;
		Tv_RcBg.bottom	=	A_PtrRect->bottom - 1;

		if((Tv_RcBg.left < Tv_RcBg.right) && (Tv_RcBg.top < Tv_RcBg.bottom))
		{
			Tv_Clr	=	m_ClrSpinCtlBgNormal;
			if(E_GrP5StatDisable == (E_GrP5StatConstMask & A_Stat))
			{
				Tv_Clr	=	m_ClrSpinCtlBgDisable;
			}
			else if(E_GrP5StatPush == (E_GrP5StatConstMask & A_Stat))
			{
				Tv_Clr	=	m_ClrSpinCtlBgPush;
			}
			else if(0 != (E_GrP5StatOnFocus & A_Stat))
			{
				Tv_Clr	=	m_ClrSpinCtlBgFocus;
			}
			GrP5PntReqFillRect(&Tv_RcBg, Tv_Clr);

			// draw mark
			Tv_RcCtl.left		=	Tv_RcBg.left + E_GrP5SkinT1SpinCtlMargin;
			Tv_RcCtl.top		=	Tv_RcBg.top + E_GrP5SkinT1SpinCtlMargin;
			Tv_RcCtl.right	=	Tv_RcBg.right - E_GrP5SkinT1SpinCtlMargin;
			Tv_RcCtl.bottom	=	Tv_RcBg.bottom - E_GrP5SkinT1SpinCtlMargin;

			if((Tv_RcCtl.left < Tv_RcCtl.right) && (Tv_RcCtl.top < Tv_RcCtl.bottom))
			{
				Tv_Clr	=	m_ClrSpinCtlMarkNormal;
				if(E_GrP5StatDisable == (E_GrP5StatConstMask & A_Stat))
				{
					Tv_Clr	=	m_ClrSpinCtlMarkDisable;
				}
				else if(E_GrP5StatPush == (E_GrP5StatConstMask & A_Stat))
				{
					Tv_Clr	=	m_ClrSpinCtlMarkPush;
				}
				else if(0 != (E_GrP5StatOnFocus & A_Stat))
				{
					Tv_Clr	=	m_ClrSpinCtlMarkFocus;
				}
				LcDrawTriangleIsosDown( &Tv_RcCtl, Tv_Clr);
			}
		}
#endif

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawSpinVtBtnInc(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat)
{
	// local -------------------
		//St_GrRect	Tv_RcBg;
		//__u32	Tv_Clr;
		//St_GrRect	Tv_RcCtl;
	// code --------------------
		GrP5PntReqDrawRect(A_PtrRect, m_ClrSpinCtlWakeDark);

#if 0
		Tv_RcBg.left		=	A_PtrRect->left + 1;
		Tv_RcBg.top			=	A_PtrRect->top + 1;
		Tv_RcBg.right		=	A_PtrRect->right - 1;
		Tv_RcBg.bottom	=	A_PtrRect->bottom - 1;

		if((Tv_RcBg.left < Tv_RcBg.right) && (Tv_RcBg.top < Tv_RcBg.bottom))
		{
			Tv_Clr	=	m_ClrSpinCtlBgNormal;
			if(E_GrP5StatDisable == (E_GrP5StatConstMask & A_Stat))
			{
				Tv_Clr	=	m_ClrSpinCtlBgDisable;
			}
			else if(E_GrP5StatPush == (E_GrP5StatConstMask & A_Stat))
			{
				Tv_Clr	=	m_ClrSpinCtlBgPush;
			}
			else if(0 != (E_GrP5StatOnFocus & A_Stat))
			{
				Tv_Clr	=	m_ClrSpinCtlBgFocus;
			}
			GrP5PntReqFillRect(&Tv_RcBg, Tv_Clr);

			// draw mark
			Tv_RcCtl.left		=	Tv_RcBg.left + E_GrP5SkinT1SpinCtlMargin;
			Tv_RcCtl.top		=	Tv_RcBg.top + E_GrP5SkinT1SpinCtlMargin;
			Tv_RcCtl.right	=	Tv_RcBg.right - E_GrP5SkinT1SpinCtlMargin;
			Tv_RcCtl.bottom	=	Tv_RcBg.bottom - E_GrP5SkinT1SpinCtlMargin;

			if((Tv_RcCtl.left < Tv_RcCtl.right) && (Tv_RcCtl.top < Tv_RcCtl.bottom))
			{
				Tv_Clr	=	m_ClrSpinCtlMarkNormal;
				if(E_GrP5StatDisable == (E_GrP5StatConstMask & A_Stat))
				{
					Tv_Clr	=	m_ClrSpinCtlMarkDisable;
				}
				else if(E_GrP5StatPush == (E_GrP5StatConstMask & A_Stat))
				{
					Tv_Clr	=	m_ClrSpinCtlMarkPush;
				}
				else if(0 != (E_GrP5StatOnFocus & A_Stat))
				{
					Tv_Clr	=	m_ClrSpinCtlMarkFocus;
				}
				LcDrawTriangleIsosUp( &Tv_RcCtl, Tv_Clr);
			}
		}
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawGraphHzBg(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat)
{
	// local -------------------
		St_GrRect	Tv_Rc;
	// code --------------------
		if(0 != (E_GrP5StatOnFocus & A_Stat))
		{

			Tv_Rc.left		=	A_PtrRect->left;
			Tv_Rc.top			=	A_PtrRect->top;
			Tv_Rc.right		=	A_PtrRect->right;
			Tv_Rc.bottom	=	A_PtrRect->top + E_GrP5SkinT1GraphHzMargin;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrGrpHzCusHz);

			Tv_Rc.top			=	A_PtrRect->bottom - E_GrP5SkinT1GraphHzMargin;
			Tv_Rc.bottom	=	A_PtrRect->bottom;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrGrpHzCusHz);

			Tv_Rc.top			=	A_PtrRect->top + E_GrP5SkinT1GraphHzMargin;
			Tv_Rc.right		=	A_PtrRect->left + E_GrP5SkinT1GraphHzMargin;
			Tv_Rc.bottom	=	A_PtrRect->bottom - E_GrP5SkinT1GraphHzMargin;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrGrpHzCusHz);

			Tv_Rc.left		=	A_PtrRect->right - E_GrP5SkinT1GraphHzMargin;
			Tv_Rc.right		=	A_PtrRect->right;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrGrpHzCusHz);
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawGraphHzItem(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u8 A_Val)
{
	// local -------------------
		St_GrRect	Tv_Rc;
	// code --------------------
		Tv_Rc.left	=	A_PtrRect->left;
		Tv_Rc.top		=	A_PtrRect->top;
		Tv_Rc.right	=	A_PtrRect->right;

		// check value
		if(0 == ((E_GrDvrRecAtbRecord | E_GrDvrRecAtbMotion | E_GrDvrRecAtbSensor) & A_Val))
		{
			// no record
			Tv_Rc.bottom	=	A_PtrRect->top + ((A_PtrRect->bottom - A_PtrRect->top) >> 1);
			GrP5PntReqFillRect(&Tv_Rc, m_ClrRatbNone);
		}
		else if(0 == ((E_GrDvrRecAtbMotion | E_GrDvrRecAtbSensor) & A_Val))
		{
			// normal
			Tv_Rc.bottom	=	A_PtrRect->top + ((A_PtrRect->bottom - A_PtrRect->top) >> 1);
			GrP5PntReqFillRect(&Tv_Rc, m_ClrRatbCont);
		}
		else if(0 == (E_GrDvrRecAtbMotion & A_Val))
		{
			// sensor
			Tv_Rc.bottom	=	A_PtrRect->top + ((A_PtrRect->bottom - A_PtrRect->top) >> 1);
			GrP5PntReqFillRect(&Tv_Rc, m_ClrRatbSens);
		}
		else if(0 == (E_GrDvrRecAtbSensor & A_Val))
		{
			// motion
			Tv_Rc.bottom	=	A_PtrRect->top + ((A_PtrRect->bottom - A_PtrRect->top) >> 1);
			GrP5PntReqFillRect(&Tv_Rc, m_ClrRatbMntn);
		}
		else 
		{
			// motion and sensor
			Tv_Rc.bottom	=	A_PtrRect->top + ((A_PtrRect->bottom - A_PtrRect->top) >> 2);
			GrP5PntReqFillRect(&Tv_Rc, m_ClrRatbSens);
			Tv_Rc.top			=	Tv_Rc.bottom;
			Tv_Rc.bottom	=	A_PtrRect->top + ((A_PtrRect->bottom - A_PtrRect->top) >> 1);
			GrP5PntReqFillRect(&Tv_Rc, m_ClrRatbMntn);
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawGraphHzCursorVt(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat)
{
	// local -------------------
		St_GrRect	Tv_Rc;
	// code --------------------
		Tv_Rc.left		=	A_PtrRect->left + ((A_PtrRect->right - A_PtrRect->left) >> 1) - 1;
		Tv_Rc.top			=	A_PtrRect->top;
		Tv_Rc.right		=	Tv_Rc.left + 2;
		Tv_Rc.bottom	=	A_PtrRect->bottom;

		GrP5PntReqFillRect(&Tv_Rc, m_ClrGrpHzCusVt);
}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawChkBgTop(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat)
{
	// local -------------------
		St_GrRect	Tv_Rc;
	// code --------------------
		// check on foucs
		if(0 != (E_GrP5StatOnFocus & A_Stat))
		{
			// draw focus mark
			Tv_Rc.left		=	A_PtrRect->left;
			Tv_Rc.top			=	A_PtrRect->top;
			Tv_Rc.right		=	A_PtrRect->right;
			Tv_Rc.bottom	=	A_PtrRect->top + 3;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrChkBgLineFocus);

			Tv_Rc.top			=	A_PtrRect->top + 3;
			Tv_Rc.right		=	A_PtrRect->left + 3;
			Tv_Rc.bottom	=	A_PtrRect->bottom - 3;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrChkBgLineFocus);

			Tv_Rc.left		=	A_PtrRect->left;
			Tv_Rc.top			=	A_PtrRect->bottom - 3;
			Tv_Rc.right		=	A_PtrRect->right;
			Tv_Rc.bottom	=	A_PtrRect->bottom;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrChkBgLineFocus);

			Tv_Rc.top			=	A_PtrRect->top + 3;
			Tv_Rc.left		=	A_PtrRect->right - 3;
			Tv_Rc.bottom	=	A_PtrRect->bottom - 3;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrChkBgLineFocus);

		}
}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawChkMark(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat)
{
	// local -------------------
		St_GrRect	Tv_Rc;
	// code --------------------
		if(4 == A_PtrPara->SubType)
		{
			return;
		}
		if(0 == (E_GrP5StatOnChecked & A_Stat))
		{
			// unmark
			GrP5PntReqFillRect(A_PtrRect, m_ClrChkMarkBgStd);
		}
		else
		{
			// mark
			GrP5PntReqFillRect(A_PtrRect, m_ClrChkMarkBgChk);
			// draw v mark
			Tv_Rc.left		=	A_PtrRect->left + 3;
			Tv_Rc.top			=	A_PtrRect->top + 3;
			Tv_Rc.right		=	A_PtrRect->right - 3;
			Tv_Rc.bottom	=	A_PtrRect->bottom - 3;
			LcDrawVmark( &Tv_Rc, m_ClrChkMarkChar);
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawVmark( Ptr_GrRect A_PtrRect, __u32 A_Color)
{
	// local -------------------
		__s32	Tv_CtX;
	// code --------------------
		// check rect
		if((A_PtrRect->left < A_PtrRect->right) && (A_PtrRect->top < A_PtrRect->bottom))
		{
			// calculate centor
			Tv_CtX	=	A_PtrRect->left + ((A_PtrRect->right - A_PtrRect->left) / 2);
			// draw first
			GrP5PntReqLine(A_PtrRect->left, A_PtrRect->top, Tv_CtX, A_PtrRect->bottom - 1, A_Color);
			// draw next
			GrP5PntReqLine(Tv_CtX, A_PtrRect->bottom - 1, A_PtrRect->right -1, A_PtrRect->top, A_Color);
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawU0TitleBar( Ptr_GrRect A_PtrRect)
{
	// local -------------------
		St_GrRect	Tv_Rc;
	// code --------------------
		// check able
		if((72 <= (A_PtrRect->bottom - A_PtrRect->top)) && (72 <= (A_PtrRect->right - A_PtrRect->left)))
		{
			// title bar
			Tv_Rc.left		=	A_PtrRect->left + 2;
			Tv_Rc.top			=	A_PtrRect->top + 2;
			Tv_Rc.right		=	A_PtrRect->right - 2;
			Tv_Rc.bottom	=	Tv_Rc.top + 38;
			LcGradeintByTbl( &Tv_Rc, &m_GradA);

			// title background
			Tv_Rc.top			=	Tv_Rc.bottom;
			Tv_Rc.bottom	=	Tv_Rc.top + 2;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrWinLineLight);

		}
}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawU0BtmBar( Ptr_GrRect A_PtrRect)
{
	// local -------------------
		St_GrRect	Tv_Rc;
	// code --------------------
		if((64 <= (A_PtrRect->bottom - A_PtrRect->top)) && (64 <= (A_PtrRect->right - A_PtrRect->left)))
		{
			Tv_Rc.left		=	A_PtrRect->left + 2;
			Tv_Rc.top			=	A_PtrRect->bottom - 30;
			Tv_Rc.right		=	A_PtrRect->right - 2;
			Tv_Rc.bottom	=	A_PtrRect->bottom - 2;

			LcGradeintByTbl( &Tv_Rc, &m_GradB);
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawU1TitleBar( Ptr_GrRect A_PtrRect)
{
	// local -------------------
		St_GrRect	Tv_Rc;
	// code --------------------
		if((180 <= (A_PtrRect->bottom - A_PtrRect->top)) && (60 <= (A_PtrRect->right - A_PtrRect->left)))
		{
			// title
			Tv_Rc.left		=	A_PtrRect->left + 2;
			Tv_Rc.top			=	A_PtrRect->top + 2;
			Tv_Rc.right		=	A_PtrRect->right - 2;
			Tv_Rc.bottom	=	Tv_Rc.top + 28;
			LcGradeintByTbl( &Tv_Rc, &m_GradC);

			// title background
			Tv_Rc.top			=	Tv_Rc.bottom;
			Tv_Rc.bottom	=	Tv_Rc.top + 120;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrWinTitleU1);

		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawU1BtmBar( Ptr_GrRect A_PtrRect)
{
	// local -------------------
		St_GrRect	Tv_Rc;
	// code --------------------
		if((180 <= (A_PtrRect->bottom - A_PtrRect->top)) && (60 <= (A_PtrRect->right - A_PtrRect->left)))
		{
			Tv_Rc.left		=	A_PtrRect->left + 2;
			Tv_Rc.top			=	A_PtrRect->bottom - 30;
			Tv_Rc.right		=	A_PtrRect->right - 2;
			Tv_Rc.bottom	=	A_PtrRect->bottom - 2;
			LcGradeintByTbl( &Tv_Rc, &m_GradC);
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawGridDrag(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect)
{
	// local -------------------
		St_GrRect	Tv_Rc;
	// code --------------------
		// top
		Tv_Rc.left		=	A_PtrRect->left;
		Tv_Rc.top			=	A_PtrRect->top;
		Tv_Rc.right		=	A_PtrRect->right;
		Tv_Rc.bottom	=	Tv_Rc.top + E_GrP5SkinT1GridDrageWidth;
		GrP5PntReqFillRect(&Tv_Rc, m_ClrGridDrag);
		// bottom
		Tv_Rc.top			=	A_PtrRect->bottom - E_GrP5SkinT1GridDrageWidth;
		Tv_Rc.bottom	=	A_PtrRect->bottom;
		GrP5PntReqFillRect(&Tv_Rc, m_ClrGridDrag);
		// left
		Tv_Rc.top			=	A_PtrRect->top + E_GrP5SkinT1GridDrageWidth;
		Tv_Rc.right		=	Tv_Rc.left + E_GrP5SkinT1GridDrageWidth;
		Tv_Rc.bottom	=	A_PtrRect->bottom - E_GrP5SkinT1GridDrageWidth;
		GrP5PntReqFillRect(&Tv_Rc, m_ClrGridDrag);
		// right
		Tv_Rc.left		=	A_PtrRect->right - E_GrP5SkinT1GridDrageWidth;
		Tv_Rc.right		=	A_PtrRect->right;
		GrP5PntReqFillRect(&Tv_Rc, m_ClrGridDrag);

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawGridRscdDrag(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect)
{
	// local -------------------
		St_GrRect	Tv_Rc;
	// code --------------------

		// left
		Tv_Rc.left		=	A_PtrRect->left;
		Tv_Rc.top			=	A_PtrRect->top + E_GrP5SkinT1GridRscdDragWidth;
		Tv_Rc.right		=	A_PtrRect->left + E_GrP5SkinT1GridRscdDragWidth;
		Tv_Rc.bottom	=	A_PtrRect->bottom - E_GrP5SkinT1GridRscdDragWidth;
		GrP5PntReqFillRect(&Tv_Rc, m_ClrGridRscdDrag);

		// top
		Tv_Rc.left		=	A_PtrRect->left;
		Tv_Rc.top			=	A_PtrRect->top;
		Tv_Rc.right		=	A_PtrRect->right;
		Tv_Rc.bottom	=	A_PtrRect->top + E_GrP5SkinT1GridRscdDragWidth;
		GrP5PntReqFillRect(&Tv_Rc, m_ClrGridRscdDrag);

		// right
		Tv_Rc.left		=	A_PtrRect->right - E_GrP5SkinT1GridRscdDragWidth;
		Tv_Rc.top			=	A_PtrRect->top + E_GrP5SkinT1GridRscdDragWidth;
		Tv_Rc.right		=	A_PtrRect->right;
		Tv_Rc.bottom	=	A_PtrRect->bottom - E_GrP5SkinT1GridRscdDragWidth;
		GrP5PntReqFillRect(&Tv_Rc, m_ClrGridRscdDrag);

		// bottom
		Tv_Rc.left		=	A_PtrRect->left;
		Tv_Rc.top			=	A_PtrRect->bottom - E_GrP5SkinT1GridRscdDragWidth;
		Tv_Rc.right		=	A_PtrRect->right;
		Tv_Rc.bottom	=	A_PtrRect->bottom;
		GrP5PntReqFillRect(&Tv_Rc, m_ClrGridRscdDrag);
			

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawGridSmdItem(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat)
{
	// local -------------------
		St_GrRect	Tv_Rc;
		__s32	Tv_SzX;
		__s32	Tv_SzY;
		__u32	Tv_Clr;
	// code --------------------
		// check size
		Tv_SzX	=	A_PtrRect->right - A_PtrRect->left;
		Tv_SzY	=	A_PtrRect->bottom - A_PtrRect->top;
		if((((E_GrP5SkinT1GridOutWidth * 2) + (E_GrP5SkinT1GridCusWidth * 2) + (E_GrP5SkinT1GridMarkWidth * 2)) <= Tv_SzX) &&
			(((E_GrP5SkinT1GridOutWidth * 2) + (E_GrP5SkinT1GridCusWidth * 2) + (E_GrP5SkinT1GridMarkWidth * 2)) <= Tv_SzY)
			)
		{

			if((E_GrP5StatOnChecked | E_GrP5StatOnCtrlCus) == ((E_GrP5StatOnChecked | E_GrP5StatOnCtrlCus) & A_Stat))
			{
				// full draw
				Tv_Rc.left		=	A_PtrRect->left;
				Tv_Rc.top			=	A_PtrRect->top;
				Tv_Rc.right		=	A_PtrRect->right;
				Tv_Rc.bottom	=	Tv_Rc.top + (E_GrP5SkinT1GridMarkWidth + E_GrP5SkinT1GridOutWidth);
				GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCursor);
				Tv_Rc.top			=	Tv_Rc.bottom;
				Tv_Rc.right		=	Tv_Rc.left + (E_GrP5SkinT1GridMarkWidth + E_GrP5SkinT1GridOutWidth);
				Tv_Rc.bottom	=	A_PtrRect->bottom - E_GrP5SkinT1GridMarkWidth - E_GrP5SkinT1GridOutWidth;
				GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCursor);
				Tv_Rc.left		=	A_PtrRect->right - E_GrP5SkinT1GridMarkWidth - E_GrP5SkinT1GridOutWidth;
				Tv_Rc.right		=	Tv_Rc.left + E_GrP5SkinT1GridMarkWidth + E_GrP5SkinT1GridOutWidth;
				GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCursor);
				Tv_Rc.left		=	A_PtrRect->left;
				Tv_Rc.top			=	Tv_Rc.bottom;
				Tv_Rc.right		=	A_PtrRect->right;
				Tv_Rc.bottom	=	Tv_Rc.top + E_GrP5SkinT1GridMarkWidth + E_GrP5SkinT1GridOutWidth;
				GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCursor);
			}
			else if(0 == ((E_GrP5StatOnChecked | E_GrP5StatOnCtrlCus) & A_Stat))
			{
				// draw outline only
				Tv_Rc.left		=	A_PtrRect->left;
				Tv_Rc.top			=	A_PtrRect->top;
				Tv_Rc.right		=	A_PtrRect->right;
				Tv_Rc.bottom	=	A_PtrRect->top + E_GrP5SkinT1GridOutWidth;
				GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCellOutline);
				Tv_Rc.top			=	Tv_Rc.bottom;
				Tv_Rc.right		=	A_PtrRect->left + E_GrP5SkinT1GridOutWidth;
				Tv_Rc.bottom	=	A_PtrRect->bottom - E_GrP5SkinT1GridOutWidth;
				GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCellOutline);
				Tv_Rc.left		=	A_PtrRect->right - E_GrP5SkinT1GridOutWidth;
				Tv_Rc.right		=	A_PtrRect->right;
				GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCellOutline);
				Tv_Rc.left		=	A_PtrRect->left;
				Tv_Rc.top			=	A_PtrRect->bottom - E_GrP5SkinT1GridOutWidth;
				Tv_Rc.right		=	A_PtrRect->right;
				Tv_Rc.bottom	=	A_PtrRect->bottom;
				GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCellOutline);
			}
			else
			{
				// separate
				// mark
				Tv_Clr	=	m_ClrGridCellOutline;
				if(0 != (E_GrP5StatOnChecked & A_Stat))
				{
					Tv_Clr	=	m_ClrGridCursor;
				}
				Tv_Rc.left		=	A_PtrRect->left;
				Tv_Rc.top			=	A_PtrRect->top;
				Tv_Rc.right		=	A_PtrRect->right;
				Tv_Rc.bottom	=	A_PtrRect->top + E_GrP5SkinT1GridOutWidth;
				GrP5PntReqFillRect(&Tv_Rc, Tv_Clr);
				Tv_Rc.top			=	Tv_Rc.bottom;
				Tv_Rc.right		=	A_PtrRect->left + E_GrP5SkinT1GridOutWidth;
				Tv_Rc.bottom	=	A_PtrRect->bottom - E_GrP5SkinT1GridOutWidth;
				GrP5PntReqFillRect(&Tv_Rc, Tv_Clr);
				Tv_Rc.left		=	A_PtrRect->right - E_GrP5SkinT1GridOutWidth;
				Tv_Rc.right		=	A_PtrRect->right;
				GrP5PntReqFillRect(&Tv_Rc, Tv_Clr);
				Tv_Rc.left		=	A_PtrRect->left;
				Tv_Rc.top			=	A_PtrRect->bottom - E_GrP5SkinT1GridOutWidth;
				Tv_Rc.right		=	A_PtrRect->right;
				Tv_Rc.bottom	=	A_PtrRect->bottom;
				GrP5PntReqFillRect(&Tv_Rc, Tv_Clr);

				// cursor
				if(0 != (E_GrP5StatOnCtrlCus & A_Stat))
				{
					Tv_Rc.left		=	A_PtrRect->left + E_GrP5SkinT1GridOutWidth;
					Tv_Rc.top			=	A_PtrRect->top + E_GrP5SkinT1GridOutWidth;
					Tv_Rc.right		=	A_PtrRect->right - E_GrP5SkinT1GridOutWidth;
					Tv_Rc.bottom	=	Tv_Rc.top + E_GrP5SkinT1GridMarkWidth;
					GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCursor);
					Tv_Rc.top			=	Tv_Rc.bottom;
					Tv_Rc.right		=	Tv_Rc.left + E_GrP5SkinT1GridMarkWidth;
					Tv_Rc.bottom	=	A_PtrRect->bottom - E_GrP5SkinT1GridMarkWidth - E_GrP5SkinT1GridOutWidth;
					GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCursor);
					Tv_Rc.left		=	A_PtrRect->right - E_GrP5SkinT1GridMarkWidth - E_GrP5SkinT1GridOutWidth;
					Tv_Rc.right		=	Tv_Rc.left + E_GrP5SkinT1GridMarkWidth;
					GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCursor);
					Tv_Rc.left		=	A_PtrRect->left + E_GrP5SkinT1GridOutWidth;
					Tv_Rc.top			=	Tv_Rc.bottom;
					Tv_Rc.right		=	A_PtrRect->right - E_GrP5SkinT1GridOutWidth;
					Tv_Rc.bottom	=	Tv_Rc.top + E_GrP5SkinT1GridMarkWidth;
					GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCursor);
				}
			}



#if 0
			// draw outline
			Tv_Rc.left		=	A_PtrRect->left;
			Tv_Rc.top			=	A_PtrRect->top;
			Tv_Rc.right		=	A_PtrRect->right;
			Tv_Rc.bottom	=	A_PtrRect->top + E_GrP5SkinT1GridOutWidth;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCellOutline);
			Tv_Rc.top			=	Tv_Rc.bottom;
			Tv_Rc.right		=	A_PtrRect->left + E_GrP5SkinT1GridOutWidth;
			Tv_Rc.bottom	=	A_PtrRect->bottom - E_GrP5SkinT1GridOutWidth;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCellOutline);
			Tv_Rc.left		=	A_PtrRect->right - E_GrP5SkinT1GridOutWidth;
			Tv_Rc.right		=	A_PtrRect->right;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCellOutline);
			Tv_Rc.left		=	A_PtrRect->left;
			Tv_Rc.top			=	A_PtrRect->bottom - E_GrP5SkinT1GridOutWidth;
			Tv_Rc.right		=	A_PtrRect->right;
			Tv_Rc.bottom	=	A_PtrRect->bottom;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCellOutline);

			if((E_GrP5StatOnChecked | E_GrP5StatOnCtrlCus) == ((E_GrP5StatOnChecked | E_GrP5StatOnCtrlCus) & A_Stat))
			{
				// full draw
				Tv_Rc.left		=	A_PtrRect->left + E_GrP5SkinT1GridOutWidth;
				Tv_Rc.top			=	A_PtrRect->top + E_GrP5SkinT1GridOutWidth;
				Tv_Rc.right		=	A_PtrRect->right - E_GrP5SkinT1GridOutWidth;
				Tv_Rc.bottom	=	Tv_Rc.top + (E_GrP5SkinT1GridMarkWidth + E_GrP5SkinT1GridCusWidth);
				GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCursor);
				Tv_Rc.top			=	Tv_Rc.bottom;
				Tv_Rc.right		=	Tv_Rc.left + (E_GrP5SkinT1GridMarkWidth + E_GrP5SkinT1GridCusWidth);
				Tv_Rc.bottom	=	A_PtrRect->bottom - E_GrP5SkinT1GridCusWidth - E_GrP5SkinT1GridMarkWidth - E_GrP5SkinT1GridOutWidth;
				GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCursor);
				Tv_Rc.left		=	A_PtrRect->right - E_GrP5SkinT1GridCusWidth - E_GrP5SkinT1GridMarkWidth - E_GrP5SkinT1GridOutWidth;
				Tv_Rc.right		=	Tv_Rc.left + E_GrP5SkinT1GridMarkWidth + E_GrP5SkinT1GridCusWidth;
				GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCursor);
				Tv_Rc.left		=	A_PtrRect->left + E_GrP5SkinT1GridOutWidth;
				Tv_Rc.top			=	Tv_Rc.bottom;
				Tv_Rc.right		=	A_PtrRect->right - E_GrP5SkinT1GridOutWidth;
				Tv_Rc.bottom	=	Tv_Rc.top + E_GrP5SkinT1GridMarkWidth + E_GrP5SkinT1GridCusWidth;
				GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCursor);
			}
			else
			{
				// separate
				// mark
				if(0 != (E_GrP5StatOnChecked & A_Stat))
				{
					Tv_Rc.left		=	A_PtrRect->left + E_GrP5SkinT1GridOutWidth;
					Tv_Rc.top			=	A_PtrRect->top + E_GrP5SkinT1GridOutWidth;
					Tv_Rc.right		=	A_PtrRect->right - E_GrP5SkinT1GridOutWidth;
					Tv_Rc.bottom	=	Tv_Rc.top + E_GrP5SkinT1GridMarkWidth;
					GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCursor);
					Tv_Rc.top			=	Tv_Rc.bottom;
					Tv_Rc.right		=	Tv_Rc.left + E_GrP5SkinT1GridMarkWidth;
					Tv_Rc.bottom	=	A_PtrRect->bottom - E_GrP5SkinT1GridMarkWidth - E_GrP5SkinT1GridOutWidth;
					GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCursor);
					Tv_Rc.left		=	A_PtrRect->right - E_GrP5SkinT1GridMarkWidth - E_GrP5SkinT1GridOutWidth;
					Tv_Rc.right		=	Tv_Rc.left + E_GrP5SkinT1GridMarkWidth;
					GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCursor);
					Tv_Rc.left		=	A_PtrRect->left + E_GrP5SkinT1GridOutWidth;
					Tv_Rc.top			=	Tv_Rc.bottom;
					Tv_Rc.right		=	A_PtrRect->right - E_GrP5SkinT1GridOutWidth;
					Tv_Rc.bottom	=	Tv_Rc.top + E_GrP5SkinT1GridMarkWidth;
					GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCursor);
				}

				// cursor
				Tv_Clr	=	0;
				if(0 != (E_GrP5StatOnCtrlCus & A_Stat))
				{
					Tv_Rc.left		=	A_PtrRect->left + E_GrP5SkinT1GridOutWidth + E_GrP5SkinT1GridMarkWidth;
					Tv_Rc.top			=	A_PtrRect->top + E_GrP5SkinT1GridOutWidth + E_GrP5SkinT1GridMarkWidth;
					Tv_Rc.right		=	A_PtrRect->right - E_GrP5SkinT1GridOutWidth - E_GrP5SkinT1GridMarkWidth;
					Tv_Rc.bottom	=	Tv_Rc.top + E_GrP5SkinT1GridCusWidth;
					GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCursor);
					Tv_Rc.top			=	Tv_Rc.bottom;
					Tv_Rc.right		=	Tv_Rc.left + E_GrP5SkinT1GridCusWidth;
					Tv_Rc.bottom	=	A_PtrRect->bottom - E_GrP5SkinT1GridCusWidth - E_GrP5SkinT1GridOutWidth - E_GrP5SkinT1GridMarkWidth;
					GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCursor);
					Tv_Rc.left		=	A_PtrRect->right - E_GrP5SkinT1GridCusWidth - E_GrP5SkinT1GridOutWidth - E_GrP5SkinT1GridMarkWidth;
					Tv_Rc.right		=	Tv_Rc.left + E_GrP5SkinT1GridCusWidth;
					GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCursor);
					Tv_Rc.left		=	A_PtrRect->left + E_GrP5SkinT1GridOutWidth + E_GrP5SkinT1GridMarkWidth;
					Tv_Rc.top			=	Tv_Rc.bottom;
					Tv_Rc.right		=	A_PtrRect->right - E_GrP5SkinT1GridOutWidth - E_GrP5SkinT1GridMarkWidth;
					Tv_Rc.bottom	=	Tv_Rc.top + E_GrP5SkinT1GridCusWidth;
					GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCursor);
				}
			}
#endif

		}	// check size
}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawAglBrkLt( Ptr_GrRect A_PtrRect, __u32 A_Clr, __u32 A_Width)
{
	// local -------------------
		__s32	Tv_SzX;
		__s32	Tv_SzY;
		
		__s32	Tv_StX;
		__s32	Tv_StY;
		__s32	Tv_TgX;
		__s32	Tv_TgY;

		__s32	Tv_DtX;
		__s32	Tv_DtY;

		__s32	Tv_Etm;
		St_GrRect	Tv_Rc;
	// code --------------------
		// check size
		Tv_SzX	=	A_PtrRect->right - A_PtrRect->left;
		Tv_SzY	=	A_PtrRect->bottom - A_PtrRect->top;

		if((Tv_SzX > (__s32)A_Width) && (2 <= Tv_SzY))
		{
			// upside
			Tv_StX	=	A_PtrRect->right - A_Width;
			Tv_StY	=	A_PtrRect->top;
			Tv_TgX	=	A_PtrRect->left;
			Tv_TgY	=	A_PtrRect->top + (Tv_SzY >> 1);

			Tv_DtX	=	Tv_StX - Tv_TgX;
			Tv_DtY	=	Tv_TgY - Tv_StY;

			Tv_Etm	=	0;
			Tv_Rc.left	=	Tv_StX;
			Tv_Rc.top		=	Tv_StY;
			if(Tv_DtX > Tv_DtY)
			{
				while((Tv_Rc.left >= Tv_TgX) || (Tv_Rc.top < Tv_TgY))
				{
					// draw pixel
					Tv_Rc.right		=	Tv_Rc.left + (__s32)A_Width;
					Tv_Rc.bottom	=	Tv_Rc.top + 1;
					GrP5PntReqFillRect(&Tv_Rc, A_Clr);

					// next
					Tv_Rc.left --;
					Tv_Etm	=	Tv_Etm + Tv_DtY;
					if(Tv_Etm >= Tv_DtX)
					{
						Tv_Etm	=	Tv_Etm - Tv_DtX;
						Tv_Rc.top ++;
					}
				}
			}
			else
			{
				while((Tv_Rc.left >= Tv_TgX) || (Tv_Rc.top < Tv_TgY))
				{
					// draw pixel
					Tv_Rc.right		=	Tv_Rc.left + (__s32)A_Width;
					Tv_Rc.bottom	=	Tv_Rc.top + 1;
					GrP5PntReqFillRect(&Tv_Rc, A_Clr);

					// next
					Tv_Rc.top ++;
					Tv_Etm	=	Tv_Etm + Tv_DtX;
					if(Tv_Etm >= Tv_DtY)
					{
						Tv_Etm	=	Tv_Etm - Tv_DtY;
						Tv_Rc.left --;
					}
				}
			}

			// down side
			Tv_StX	=	A_PtrRect->left;
			Tv_StY	=	A_PtrRect->top + (Tv_SzY >> 1);
			Tv_TgX	=	A_PtrRect->right - (__s32)A_Width;
			Tv_TgY	=	A_PtrRect->bottom;

			Tv_DtX	=	Tv_TgX - Tv_StX;
			Tv_DtY	=	Tv_TgY - Tv_StY;

			Tv_Etm	=	0;
			Tv_Rc.left	=	Tv_StX;
			Tv_Rc.top		=	Tv_StY;
			if(Tv_DtX > Tv_DtY)
			{
				while((Tv_Rc.left <= Tv_TgX) || (Tv_Rc.top < Tv_TgY))
				{
					// draw pixel
					Tv_Rc.right		=	Tv_Rc.left + (__s32)A_Width;
					Tv_Rc.bottom	=	Tv_Rc.top + 1;
					GrP5PntReqFillRect(&Tv_Rc, A_Clr);

					// next
					Tv_Rc.left ++;
					Tv_Etm	=	Tv_Etm + Tv_DtY;
					if(Tv_Etm >= Tv_DtX)
					{
						Tv_Etm	=	Tv_Etm - Tv_DtX;
						Tv_Rc.top ++;
					}
				}
			}
			else
			{
				while((Tv_Rc.left <= Tv_TgX) || (Tv_Rc.top < Tv_TgY))
				{
					// draw pixel
					Tv_Rc.right		=	Tv_Rc.left + (__s32)A_Width;
					Tv_Rc.bottom	=	Tv_Rc.top + 1;
					GrP5PntReqFillRect(&Tv_Rc, A_Clr);

					// next
					Tv_Rc.top ++;
					Tv_Etm	=	Tv_Etm + Tv_DtX;
					if(Tv_Etm >= Tv_DtY)
					{
						Tv_Etm	=	Tv_Etm - Tv_DtY;
						Tv_Rc.left ++;
					}
				}
			}

		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawAglBrkRt( Ptr_GrRect A_PtrRect, __u32 A_Clr, __u32 A_Width)
{
	// local -------------------
		__s32	Tv_SzX;
		__s32	Tv_SzY;

		__s32	Tv_StX;
		__s32	Tv_StY;
		__s32	Tv_TgX;
		__s32	Tv_TgY;

		__s32	Tv_DtX;
		__s32	Tv_DtY;

		__s32	Tv_Etm;
		St_GrRect	Tv_Rc;
	// code --------------------
		// check size
		Tv_SzX	=	A_PtrRect->right - A_PtrRect->left;
		Tv_SzY	=	A_PtrRect->bottom - A_PtrRect->top;

		if((Tv_SzX > (__s32)A_Width) && (2 <= Tv_SzY))
		{
			// upside
			Tv_StX	=	A_PtrRect->left;
			Tv_StY	=	A_PtrRect->top;
			Tv_TgX	=	A_PtrRect->right - (__s32)A_Width;
			Tv_TgY	=	A_PtrRect->top + (Tv_SzY >> 1);

			Tv_DtX	=	Tv_TgX - Tv_StX;
			Tv_DtY	=	Tv_TgY - Tv_StY;

			Tv_Etm	=	0;
			Tv_Rc.left	=	Tv_StX;
			Tv_Rc.top		=	Tv_StY;
			if(Tv_DtX > Tv_DtY)
			{
				while((Tv_Rc.left <= Tv_TgX) || (Tv_Rc.top < Tv_TgY))
				{
					// draw pixel
					Tv_Rc.right		=	Tv_Rc.left + (__s32)A_Width;
					Tv_Rc.bottom	=	Tv_Rc.top + 1;
					GrP5PntReqFillRect(&Tv_Rc, A_Clr);

					// next
					Tv_Rc.left ++;
					Tv_Etm	=	Tv_Etm + Tv_DtY;
					if(Tv_Etm >= Tv_DtX)
					{
						Tv_Etm	=	Tv_Etm - Tv_DtX;
						Tv_Rc.top ++;
					}
				}
			}
			else
			{
				while((Tv_Rc.left <= Tv_TgX) || (Tv_Rc.top < Tv_TgY))
				{
					// draw pixel
					Tv_Rc.right		=	Tv_Rc.left + (__s32)A_Width;
					Tv_Rc.bottom	=	Tv_Rc.top + 1;
					GrP5PntReqFillRect(&Tv_Rc, A_Clr);

					// next
					Tv_Rc.top ++;
					Tv_Etm	=	Tv_Etm + Tv_DtX;
					if(Tv_Etm >= Tv_DtY)
					{
						Tv_Etm	=	Tv_Etm - Tv_DtY;
						Tv_Rc.left ++;
					}
				}
			}

			// down side
			Tv_StX	=	A_PtrRect->right - (__s32)A_Width;
			Tv_StY	=	A_PtrRect->top + (Tv_SzY >> 1);
			Tv_TgX	=	A_PtrRect->left;
			Tv_TgY	=	A_PtrRect->bottom;

			Tv_DtX	=	Tv_StX - Tv_TgX;
			Tv_DtY	=	Tv_TgY - Tv_StY;

			Tv_Etm	=	0;
			Tv_Rc.left	=	Tv_StX;
			Tv_Rc.top		=	Tv_StY;
			if(Tv_DtX > Tv_DtY)
			{
				while((Tv_Rc.left >= Tv_TgX) || (Tv_Rc.top < Tv_TgY))
				{
					// draw pixel
					Tv_Rc.right		=	Tv_Rc.left + (__s32)A_Width;
					Tv_Rc.bottom	=	Tv_Rc.top + 1;
					GrP5PntReqFillRect(&Tv_Rc, A_Clr);

					// next
					Tv_Rc.left --;
					Tv_Etm	=	Tv_Etm + Tv_DtY;
					if(Tv_Etm >= Tv_DtX)
					{
						Tv_Etm	=	Tv_Etm - Tv_DtX;
						Tv_Rc.top ++;
					}
				}
			}
			else
			{
				while((Tv_Rc.left >= Tv_TgX) || (Tv_Rc.top < Tv_TgY))
				{
					// draw pixel
					Tv_Rc.right		=	Tv_Rc.left + (__s32)A_Width;
					Tv_Rc.bottom	=	Tv_Rc.top + 1;
					GrP5PntReqFillRect(&Tv_Rc, A_Clr);

					// next
					Tv_Rc.top ++;
					Tv_Etm	=	Tv_Etm + Tv_DtX;
					if(Tv_Etm >= Tv_DtY)
					{
						Tv_Etm	=	Tv_Etm - Tv_DtY;
						Tv_Rc.left --;
					}
				}
			}

		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawAglBrkUp( Ptr_GrRect A_PtrRect, __u32 A_Clr, __u32 A_Width)
{
	// local -------------------
		__s32	Tv_SzX;
		__s32	Tv_SzY;

		__u32	Tv_L1Sx;
		__u32	Tv_L1Sy;
		__u32	Tv_L1Ex;
		__u32	Tv_L1Ey;

		__u32	Tv_L2Sx;
		__u32	Tv_L2Sy;
		__u32	Tv_L2Ex;
		__u32	Tv_L2Ey;

		__u32	Tv_WkIdx;

	// code --------------------
		// check size
		Tv_SzX	=	A_PtrRect->right - A_PtrRect->left;
		Tv_SzY	=	A_PtrRect->bottom - A_PtrRect->top;

		if((Tv_SzY > (__s32)A_Width) && (2 < Tv_SzX) && (0 != A_Width))
		{
			// calculate left line
			Tv_L1Sx		=	A_PtrRect->left + (Tv_SzX >> 1);
			if(0 == (1 & Tv_SzX))
			{
				Tv_L1Sx --;
			}
			Tv_L1Ex		=	A_PtrRect->left;
			Tv_L1Sy		=	A_PtrRect->top;
			Tv_L1Ey		=	A_PtrRect->bottom - (__s32)A_Width;

			// calculate right line
			Tv_L2Sx		=	A_PtrRect->left + (Tv_SzX >> 1);
			Tv_L2Ex		=	A_PtrRect->right - 1;
			Tv_L2Sy		=	A_PtrRect->top;
			Tv_L2Ey		=	A_PtrRect->bottom - (__s32)A_Width;

			// do draw
			for( Tv_WkIdx = 0; Tv_WkIdx < A_Width; Tv_WkIdx++)
			{
				GrP5PntReqLine(Tv_L1Sx, Tv_L1Sy, Tv_L1Ex, Tv_L1Ey, A_Clr);
				GrP5PntReqLine(Tv_L2Sx, Tv_L2Sy, Tv_L2Ex, Tv_L2Ey, A_Clr);
				// next
				Tv_L1Sy ++;
				Tv_L1Ey ++;
				Tv_L2Sy ++;
				Tv_L2Ey ++;
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawAglBrkDn( Ptr_GrRect A_PtrRect, __u32 A_Clr, __u32 A_Width)
{
	// local -------------------
		__s32	Tv_SzX;
		__s32	Tv_SzY;

		__u32	Tv_L1Sx;
		__u32	Tv_L1Sy;
		__u32	Tv_L1Ex;
		__u32	Tv_L1Ey;

		__u32	Tv_L2Sx;
		__u32	Tv_L2Sy;
		__u32	Tv_L2Ex;
		__u32	Tv_L2Ey;

		__u32	Tv_WkIdx;
	// code --------------------
		// check size
		Tv_SzX	=	A_PtrRect->right - A_PtrRect->left;
		Tv_SzY	=	A_PtrRect->bottom - A_PtrRect->top;

		if((Tv_SzY > (__s32)A_Width) && (2 < Tv_SzX) && (0 != A_Width))
		{
			// calculate left line
			Tv_L1Sx		=	A_PtrRect->left + (Tv_SzX >> 1);
			if(0 == (1 & Tv_SzX))
			{
				Tv_L1Sx --;
			}
			Tv_L1Ex		=	A_PtrRect->left;
			Tv_L1Sy		=	A_PtrRect->bottom - (__s32)A_Width;
			Tv_L1Ey		=	A_PtrRect->top;

			// calculate right line
			Tv_L2Sx		=	A_PtrRect->left + (Tv_SzX >> 1);
			Tv_L2Ex		=	A_PtrRect->right - 1;
			Tv_L2Sy		=	A_PtrRect->bottom - (__s32)A_Width;
			Tv_L2Ey		=	A_PtrRect->top;

			// do draw
			for(Tv_WkIdx = 0; Tv_WkIdx < A_Width; Tv_WkIdx++)
			{
				GrP5PntReqLine(Tv_L1Sx, Tv_L1Sy, Tv_L1Ex, Tv_L1Ey, A_Clr);
				GrP5PntReqLine(Tv_L2Sx, Tv_L2Sy, Tv_L2Ex, Tv_L2Ey, A_Clr);
				// next
				Tv_L1Sy ++;
				Tv_L1Ey ++;
				Tv_L2Sy ++;
				Tv_L2Ey ++;
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawGridRscdItem(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat, __u8 A_Val)
{
	// local -------------------
		St_GrRect	Tv_RcItm;
		St_GrRect	Tv_RcRun;
		__u32*	Tv_PtrCols;
		__s32		Tv_Height;
	// code --------------------
		Tv_RcItm.left		=	A_PtrRect->left;
		Tv_RcItm.top		=	A_PtrRect->top + (__s32)(__u32)A_PtrPara->ParaByte0;
		Tv_RcItm.bottom	=	A_PtrRect->bottom - (__s32)(__u32)A_PtrPara->ParaByte1;
		Tv_RcItm.right	=	A_PtrRect->right - (__s32)(__u32)A_PtrPara->ParaByte2;

		Tv_Height				=	Tv_RcItm.bottom - Tv_RcItm.top;

		if((Tv_RcItm.right > Tv_RcItm.left) && (5 <= Tv_Height))
		{
			Tv_PtrCols	=	m_ClrGridRscdItm[A_Val & 7];

			// draw item
			Tv_RcRun.left		=	Tv_RcItm.left;
			Tv_RcRun.bottom	=	Tv_RcItm.top;
			Tv_RcRun.right	=	Tv_RcItm.right;

			// 0
			Tv_RcRun.top		=	Tv_RcRun.bottom;
			Tv_RcRun.bottom	=	Tv_RcItm.top + ((Tv_Height * 2) >> 3);
			GrP5PntReqFillRect(&Tv_RcRun, Tv_PtrCols[0]);

			// 1
			Tv_RcRun.top		=	Tv_RcRun.bottom;
			Tv_RcRun.bottom	=	Tv_RcItm.top + ((Tv_Height * 3) >> 3);
			GrP5PntReqFillRect(&Tv_RcRun, Tv_PtrCols[1]);

			// 2
			Tv_RcRun.top		=	Tv_RcRun.bottom;
			Tv_RcRun.bottom	=	Tv_RcItm.top + ((Tv_Height * 5) >> 3);
			GrP5PntReqFillRect(&Tv_RcRun, Tv_PtrCols[2]);

			// 3
			Tv_RcRun.top		=	Tv_RcRun.bottom;
			Tv_RcRun.bottom	=	Tv_RcItm.top + ((Tv_Height * 6) >> 3);
			GrP5PntReqFillRect(&Tv_RcRun, Tv_PtrCols[3]);

			// 4
			Tv_RcRun.top		=	Tv_RcRun.bottom;
			Tv_RcRun.bottom	=	Tv_RcItm.bottom;
			GrP5PntReqFillRect(&Tv_RcRun, Tv_PtrCols[4]);
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawAimBg(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat)
{
	// local -------------------
		__s32	Tv_CtX;
		__s32	Tv_CtY;
		St_GrRect	Tv_Rc;
	// code --------------------
		Tv_CtX	=	A_PtrRect->left + ((A_PtrRect->right - A_PtrRect->left) >> 1);
		Tv_CtY	=	A_PtrRect->top + ((A_PtrRect->bottom - A_PtrRect->top) >> 1);
		// cross center
		Tv_Rc.left		=	Tv_CtX - (E_GrP5SkinT1AimCrossWidth >> 1);
		Tv_Rc.right		=	Tv_Rc.left + E_GrP5SkinT1AimCrossWidth;
		Tv_Rc.top			=	Tv_CtY - (E_GrP5SkinT1AimCrossSize >> 1);
		Tv_Rc.bottom	=	Tv_Rc.top + E_GrP5SkinT1AimCrossSize;
		GrP5PntReqFillRect(&Tv_Rc, m_ClrAimCross);

		// cross left
		Tv_Rc.left		=	Tv_CtX - (E_GrP5SkinT1AimCrossSize >> 1);
		Tv_Rc.right		=	Tv_CtX - (E_GrP5SkinT1AimCrossWidth >> 1);
		Tv_Rc.top			=	Tv_CtY - (E_GrP5SkinT1AimCrossWidth >> 1);
		Tv_Rc.bottom	=	Tv_Rc.top + E_GrP5SkinT1AimCrossWidth;
		GrP5PntReqFillRect(&Tv_Rc, m_ClrAimCross);

		// cross right
		Tv_Rc.left		=	Tv_Rc.right + E_GrP5SkinT1AimCrossWidth;
		Tv_Rc.right		=	Tv_CtX + (E_GrP5SkinT1AimCrossSize >> 1);
		GrP5PntReqFillRect(&Tv_Rc, m_ClrAimCross);
		
}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawGrpHzT1BgCont(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat)
{
	// local -------------------
		St_GrRect	Tv_Rc;
	// code --------------------
		// check able
		if(0 != (E_GrP5StatOnFocus & A_Stat))
		{
			// up
			Tv_Rc.left		=	A_PtrRect->left;
			Tv_Rc.top			=	A_PtrRect->top;
			Tv_Rc.right		=	A_PtrRect->right;
			Tv_Rc.bottom	=	Tv_Rc.top + E_GrP5SkinT1GrpHzT1ContMgn;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrGrpHzT1BgFocus);
			// down
			Tv_Rc.top			=	A_PtrRect->bottom - E_GrP5SkinT1GrpHzT1ContMgn;
			Tv_Rc.bottom	=	A_PtrRect->bottom;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrGrpHzT1BgFocus);
			// left
			Tv_Rc.top			=	A_PtrRect->top + E_GrP5SkinT1GrpHzT1ContMgn;
			Tv_Rc.right		=	Tv_Rc.left + E_GrP5SkinT1GrpHzT1ContMgn;
			Tv_Rc.bottom	=	A_PtrRect->bottom - E_GrP5SkinT1GrpHzT1ContMgn;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrGrpHzT1BgFocus);
			// right
			Tv_Rc.left		=	A_PtrRect->right - E_GrP5SkinT1GrpHzT1ContMgn;
			Tv_Rc.right		=	A_PtrRect->right;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrGrpHzT1BgFocus);
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawGrpHzT1Item(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat, __u8 A_Val)
{
	// local -------------------
		St_GrRect	Tv_RcItm;
		St_GrRect	Tv_RcRun;
		__u32*	Tv_PtrCols;
		__s32		Tv_Height;
	// code --------------------
		Tv_RcItm.left		=	A_PtrRect->left;
		Tv_RcItm.top		=	A_PtrRect->top + (__s32)(__u32)A_PtrPara->ParaByte0;
		Tv_RcItm.bottom	=	A_PtrRect->bottom - (__s32)(__u32)A_PtrPara->ParaByte1;
		Tv_RcItm.right	=	A_PtrRect->right - (__s32)(__u32)A_PtrPara->ParaByte2;

		Tv_Height				=	Tv_RcItm.bottom - Tv_RcItm.top;

		if((Tv_RcItm.right > Tv_RcItm.left) && (5 <= Tv_Height))
		{
			Tv_PtrCols	=	m_ClrGrpHzT1Cne;
			if(0 == A_Val)
			{
				Tv_PtrCols	=	m_ClrGrpHzT1Non;
			}
			else if(E_GrDvrRecAtbRecord == A_Val)
			{
				Tv_PtrCols	=	m_ClrGrpHzT1Nrm;
			}
			else if(E_GrDvrRecAtbMotion == A_Val)
			{
				Tv_PtrCols	=	m_ClrGrpHzT1Mtn;
			}
			else if(E_GrDvrRecAtbSensor == A_Val)
			{
				Tv_PtrCols	=	m_ClrGrpHzT1Sen;
			}
			else if((E_GrDvrRecAtbMotion | E_GrDvrRecAtbSensor) == A_Val)
			{
				Tv_PtrCols	=	m_ClrGrpHzT1Mns;
			}

			// draw item
			Tv_RcRun.left		=	Tv_RcItm.left;
			Tv_RcRun.bottom	=	Tv_RcItm.top;
			Tv_RcRun.right	=	Tv_RcItm.right;

			// 0
			Tv_RcRun.top		=	Tv_RcRun.bottom;
			Tv_RcRun.bottom	=	Tv_RcItm.top + ((Tv_Height * 1) >> 3);
			GrP5PntReqFillRect(&Tv_RcRun, Tv_PtrCols[0]);

			// 1
			Tv_RcRun.top		=	Tv_RcRun.bottom;
			Tv_RcRun.bottom	=	Tv_RcItm.top + ((Tv_Height * 3) >> 3);
			GrP5PntReqFillRect(&Tv_RcRun, Tv_PtrCols[1]);

			// 2
			Tv_RcRun.top		=	Tv_RcRun.bottom;
			Tv_RcRun.bottom	=	Tv_RcItm.top + ((Tv_Height * 5) >> 3);
			GrP5PntReqFillRect(&Tv_RcRun, Tv_PtrCols[2]);

			// 3
			Tv_RcRun.top		=	Tv_RcRun.bottom;
			Tv_RcRun.bottom	=	Tv_RcItm.top + ((Tv_Height * 7) >> 3);
			GrP5PntReqFillRect(&Tv_RcRun, Tv_PtrCols[3]);

			// 4
			Tv_RcRun.top		=	Tv_RcRun.bottom;
			Tv_RcRun.bottom	=	Tv_RcItm.bottom;
			GrP5PntReqFillRect(&Tv_RcRun, Tv_PtrCols[4]);

			// cursor
			if(0 != (E_GrP5StatOnCtrlCus & A_Stat))
			{
				Tv_RcRun.left		=	Tv_RcItm.left;
				Tv_RcRun.top		=	Tv_RcItm.top;
				Tv_RcRun.right	=	Tv_RcItm.right;
				Tv_RcRun.bottom	=	Tv_RcRun.top + 2;
				GrP5PntReqFillRect(&Tv_RcRun, m_ClrGrpHzT1BgFocus);
				Tv_RcRun.top		=	Tv_RcItm.bottom - 2;
				Tv_RcRun.bottom	=	Tv_RcItm.bottom;
				GrP5PntReqFillRect(&Tv_RcRun, m_ClrGrpHzT1BgFocus);
				Tv_RcRun.right	=	Tv_RcItm.left + 2;
				Tv_RcRun.top		=	Tv_RcItm.top + 2;
				Tv_RcRun.bottom	=	Tv_RcRun.bottom - 2;
				GrP5PntReqFillRect(&Tv_RcRun, m_ClrGrpHzT1BgFocus);
				Tv_RcRun.left		=	Tv_RcItm.right - 2;
				Tv_RcRun.right	=	Tv_RcItm.right;
				GrP5PntReqFillRect(&Tv_RcRun, m_ClrGrpHzT1BgFocus);
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawGrpHzT1BgInd(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat)
{
	// local -------------------
		St_GrRect	Tv_Rc;
		__u32	Tv_WkIdx;
	// code --------------------
		// check size over 8 pixel
		if((8 < (A_PtrRect->right - A_PtrRect->left)) && (8 < (A_PtrRect->bottom - A_PtrRect->top)))
		{
			// upper
			Tv_Rc.bottom	=	A_PtrRect->top;
			for( Tv_WkIdx = 0; Tv_WkIdx < 4; Tv_WkIdx++)
			{
				Tv_Rc.top			=	Tv_Rc.bottom;
				Tv_Rc.left		=	A_PtrRect->left + (__s32)(__u32)V_GrP5SkinT1Curb4x[Tv_WkIdx];
				Tv_Rc.right		=	A_PtrRect->right - (__s32)(__u32)V_GrP5SkinT1Curb4x[Tv_WkIdx];
				Tv_Rc.bottom	=	Tv_Rc.top + 1;

				GrP5PntReqFillRect(&Tv_Rc, m_ClrGrpHzT1IndBg);
			}
			// contents
			Tv_Rc.left		=	A_PtrRect->left;
			Tv_Rc.top			=	Tv_Rc.bottom;
			Tv_Rc.right		=	A_PtrRect->right;
			Tv_Rc.bottom	=	A_PtrRect->bottom - 4;

			GrP5PntReqFillRect(&Tv_Rc, m_ClrGrpHzT1IndBg);

			// bottom
			for(Tv_WkIdx = 0; Tv_WkIdx < 4; Tv_WkIdx++)
			{
				Tv_Rc.top			=	Tv_Rc.bottom;
				Tv_Rc.left		=	A_PtrRect->left + (__s32)(__u32)V_GrP5SkinT1Curb4x[3 - Tv_WkIdx];
				Tv_Rc.right		=	A_PtrRect->right - (__s32)(__u32)V_GrP5SkinT1Curb4x[3 - Tv_WkIdx];
				Tv_Rc.bottom	=	Tv_Rc.top + 1;

				GrP5PntReqFillRect(&Tv_Rc, m_ClrGrpHzT1IndBg);
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawArwCrosshair(__s32 A_X, __s32 A_Y)
{
	// local -------------------
		St_GrRect	Tv_RcDw;
		__u32	Tv_RcIdx;
	// code --------------------
		// draw out
		Tv_RcIdx	=	0;
		while(0x7FFF > V_GrP5SkinT1ArwCrhOutTbl[Tv_RcIdx].left)
		{
			Tv_RcDw.left		=	A_X + (__s32)V_GrP5SkinT1ArwCrhOutTbl[Tv_RcIdx].left;
			Tv_RcDw.top			=	A_Y + (__s32)V_GrP5SkinT1ArwCrhOutTbl[Tv_RcIdx].top;
			Tv_RcDw.right		=	A_X + (__s32)V_GrP5SkinT1ArwCrhOutTbl[Tv_RcIdx].right;
			Tv_RcDw.bottom	=	A_Y + (__s32)V_GrP5SkinT1ArwCrhOutTbl[Tv_RcIdx].bottom;

			GrP5PntReqFillRect(&Tv_RcDw, m_ClrArwBase[0]);

			// next
			Tv_RcIdx ++;
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawArwHalfCir(__s32 A_X, __s32 A_Y, __u32 A_Angle, __u32 A_Quadrant, __u32 A_Color)
{
	// local -------------------
		__u32	Tv_Angl;
		__u32	Tv_Qdrt;
		__u32	Tv_WkIdx;
		St_GrRect	Tv_Rc;
	// code --------------------
		// init
		Tv_Qdrt	=	A_Quadrant;
		Tv_Angl	=	A_Angle;
		for( Tv_WkIdx = 0; Tv_WkIdx < 32; Tv_WkIdx++)
		{
			// draw now
			if(0 == Tv_Qdrt)
			{
				Tv_Rc.left		=	A_X + V_GrP5SkinT1ArwCircle[Tv_Angl].left;
				Tv_Rc.top			=	A_Y + V_GrP5SkinT1ArwCircle[Tv_Angl].top;
				Tv_Rc.right		=	A_X + V_GrP5SkinT1ArwCircle[Tv_Angl].right;
				Tv_Rc.bottom	=	A_Y + V_GrP5SkinT1ArwCircle[Tv_Angl].bottom;
				GrP5PntReqFillRect(&Tv_Rc, A_Color);
				// next
				Tv_Angl ++;
				if(16 <= Tv_Angl)
				{
					Tv_Qdrt ++;
					Tv_Angl	=	15;
				}
			}
			else if(1 == Tv_Qdrt)
			{
				Tv_Rc.left		=	A_X - V_GrP5SkinT1ArwCircle[Tv_Angl].right - 1;
				Tv_Rc.top			=	A_Y + V_GrP5SkinT1ArwCircle[Tv_Angl].top;
				Tv_Rc.right		=	A_X - V_GrP5SkinT1ArwCircle[Tv_Angl].left;
				Tv_Rc.bottom	=	A_Y + V_GrP5SkinT1ArwCircle[Tv_Angl].bottom;
				GrP5PntReqFillRect(&Tv_Rc, A_Color);
				// next
				if(0 < Tv_Angl)
				{
					Tv_Angl --;
				}
				else
				{
					Tv_Qdrt ++;
					Tv_Angl	=	0;
				}
			}
			else if(2 == Tv_Qdrt)
			{
				Tv_Rc.left		=	A_X - V_GrP5SkinT1ArwCircle[Tv_Angl].right - 1;
				Tv_Rc.top			=	A_Y - V_GrP5SkinT1ArwCircle[Tv_Angl].bottom + 1;
				Tv_Rc.right		=	A_X - V_GrP5SkinT1ArwCircle[Tv_Angl].left;
				Tv_Rc.bottom	=	A_Y - V_GrP5SkinT1ArwCircle[Tv_Angl].top + 2;
				GrP5PntReqFillRect(&Tv_Rc, A_Color);
				// next
				Tv_Angl ++;
				if(16 <= Tv_Angl)
				{
					Tv_Qdrt ++;
					Tv_Angl	=	15;
				}
			}
			else
			{
				Tv_Rc.left		=	A_X + V_GrP5SkinT1ArwCircle[Tv_Angl].left;
				Tv_Rc.top			=	A_Y - V_GrP5SkinT1ArwCircle[Tv_Angl].bottom + 1;
				Tv_Rc.right		=	A_X + V_GrP5SkinT1ArwCircle[Tv_Angl].right;
				Tv_Rc.bottom	=	A_Y - V_GrP5SkinT1ArwCircle[Tv_Angl].top + 2;
				GrP5PntReqFillRect(&Tv_Rc, A_Color);
				// next
				if(0 < Tv_Angl)
				{
					Tv_Angl --;
				}
				else
				{
					Tv_Qdrt =	0;
					Tv_Angl	=	0;
				}
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawArwCircle(__s32 A_StX, __s32 A_StY, __s32 A_EdX, __s32 A_EdY, __u32 A_Stat, __u32 A_Val)
{
	// local -------------------
		__s32	Tv_CtX;
		__s32	Tv_CtY;
		__s32	Tv_DtX;
		__s32	Tv_DtY;
		__u32	Tv_Angle;
		__u32	Tv_Qdrt;
	// code --------------------
		Tv_CtX		=	(A_StX + A_EdX) >> 1;
		Tv_CtY		=	(A_StY + A_EdY) >> 1;

		Tv_DtX		=	GrAbs32(A_StX - A_EdX);
		Tv_DtY		=	GrAbs32(A_StY - A_EdY);

		if(Tv_DtX < Tv_DtY)
		{
			if(0 == Tv_DtX)
			{
				Tv_Angle	=	15;
			}
			else
			{
				Tv_Angle	=	16 - ((Tv_DtX << 3) / Tv_DtY);
			}
		}
		else
		{
			if(0 == Tv_DtY)
			{
				Tv_Angle	=	0;
			}
			else
			{
				Tv_Angle	=	((Tv_DtY << 3) / Tv_DtX);
			}
		}

		if(16 <= Tv_Angle)
		{
			Tv_Angle	=	15;
		}

		// calculate quadrant
		Tv_Qdrt	=	0;
		if((A_StX >= A_EdX) && (A_StY >= A_EdY))
		{
			Tv_Qdrt	=	1;
		}
		else if((A_StX >= A_EdX) && (A_StY < A_EdY))
		{
			Tv_Qdrt	=	2;
		}
		else if((A_StX < A_EdX) && (A_StY < A_EdY))
		{
			Tv_Qdrt	=	3;
		}

		// draw 
		if(0 != (1 & A_Val))
		{
			// draw left
			LcDrawArwHalfCir(Tv_CtX, Tv_CtY, Tv_Angle, Tv_Qdrt, m_ClrArwBase[0]);
		}

		// draw right
		if(0 != (2 & A_Val))
		{
			// draw left
			Tv_Qdrt	=	(Tv_Qdrt + 2) & 3;
			LcDrawArwHalfCir(Tv_CtX, Tv_CtY, Tv_Angle, Tv_Qdrt, m_ClrArwBase[2]);
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawArwStart(__s32 A_X, __s32 A_Y)
{
	// local -------------------
		St_GrRect	Tv_RcDw;
		__u32	Tv_RcIdx;
	// code --------------------
		// draw
		Tv_RcIdx	=	0;
		while(0x7FFF > V_GrP5SkinT1ArwStartTbl[Tv_RcIdx].left)
		{
			Tv_RcDw.left		=	A_X + (__s32)V_GrP5SkinT1ArwStartTbl[Tv_RcIdx].left;
			Tv_RcDw.top			=	A_Y + (__s32)V_GrP5SkinT1ArwStartTbl[Tv_RcIdx].top;
			Tv_RcDw.right		=	A_X + (__s32)V_GrP5SkinT1ArwStartTbl[Tv_RcIdx].right;
			Tv_RcDw.bottom	=	A_Y + (__s32)V_GrP5SkinT1ArwStartTbl[Tv_RcIdx].bottom;

			GrP5PntReqFillRect(&Tv_RcDw, m_ClrArwBase[1]);

			// next
			Tv_RcIdx ++;
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawArwItem(__s32 A_StX, __s32 A_StY, __s32 A_EdX, __s32 A_EdY, __u32 A_ColorType, __u32 A_DirMap)
{
	// local -------------------
	// code --------------------
		// check exist start position
		if((0 > A_StX) || (0 > A_StY) )
		{
			return;
		}

		// check start postion only
		if((0 > A_EdX) || (0 > A_EdY))
		{
			// draw line start
			LcDrawArwStart(A_StX, A_StY);
			return;
		}

		// darw circle
		//LcDrawArwCircle(A_StX, A_StY, A_EdX, A_EdY, A_Stat, A_Val);

		// draw perpendicularity arrow
		LcDrawArwSideMark(A_StX, A_StY, A_EdX, A_EdY, A_DirMap);

		// draw full line
		GrP5PntReqLine(A_StX, A_StY, A_EdX, A_EdY, m_ClrArwLine[A_ColorType]);
		GrP5PntReqLine(A_StX, A_StY + 1, A_EdX, A_EdY + 1, m_ClrArwLine[A_ColorType]);

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawArwSideMark(__s32 A_StX, __s32 A_StY, __s32 A_EdX, __s32 A_EdY, __u32 A_DirMap)
{
	// local -------------------
		__s32	Tv_CtX;
		__s32	Tv_CtY;
		__s32	Tv_DtBx;	// base
		__s32	Tv_DtBy;
		__s32	Tv_DtLx;	// base
		__s32	Tv_DtLy;
	// code --------------------
		Tv_CtX		=	(A_StX + A_EdX) >> 1;
		Tv_CtY		=	(A_StY + A_EdY) >> 1;

		Tv_DtBx		=	A_StX - A_EdX;
		Tv_DtBy		=	A_StY - A_EdY;

		// check right
		if(0 != (E_GrP5SkinDirMapRight & A_DirMap))
		{
			Tv_DtLx		=	-Tv_DtBy;
			Tv_DtLy		=	Tv_DtBx;

			LcDrawPerpAraw(Tv_DtLx, Tv_DtLy, Tv_CtX, Tv_CtY);
		}

		// check left
		if(0 != (E_GrP5SkinDirMapLeft & A_DirMap))
		{
			Tv_DtLx		=	Tv_DtBy;
			Tv_DtLy		=	-Tv_DtBx;

			LcDrawPerpAraw(Tv_DtLx, Tv_DtLy, Tv_CtX, Tv_CtY);
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawPerpAraw(__s32 A_Dx, __s32 A_Dy, __s32 A_Cx, __s32 A_Cy)
{
	// local -------------------
		__s32	Tv_Angle;
		BOOL8	Tv_IsFlipX;
		BOOL8	Tv_IsFlipY;
		Ptr_GrP5SkinT1VctCmd	Tv_PtrVct;
	// code --------------------
		Tv_IsFlipX	=	FALSE;
		Tv_IsFlipY	=	FALSE;

		// check quadrant
		if(0 == A_Dx)
		{
			Tv_Angle	=	460;
			if(0 < A_Dy)
			{
				Tv_IsFlipY	=	TRUE;
			}
		}
		else if((0 < A_Dx) && (0 >= A_Dy ))
		{
			Tv_Angle	=	((-A_Dy) << 4) / A_Dx;
		}
		else if((0 > A_Dx) && (0 >= A_Dy))
		{
			Tv_IsFlipX	=	TRUE;
			Tv_Angle	=	((-A_Dy) << 4) / (-A_Dx);
		}
		else if((0 > A_Dx) && (0 < A_Dy))
		{
			Tv_IsFlipX	=	TRUE;
			Tv_IsFlipY	=	TRUE;
			Tv_Angle	=	(A_Dy << 4) / (-A_Dx);
		}
		else
		{
			Tv_IsFlipY	=	TRUE;
			Tv_Angle	=	(A_Dy << 4) / A_Dx;
		}

		if(0 == Tv_Angle)
		{
			Tv_PtrVct	=	V_GrP5SkinT1DirArwA0;
		}
		else if(5 > Tv_Angle)
		{
			Tv_PtrVct	=	V_GrP5SkinT1DirArwA11;
		}
		else if(9 > Tv_Angle)
		{
			Tv_PtrVct	=	V_GrP5SkinT1DirArwA23;
		}
		else if(13 > Tv_Angle)
		{
			Tv_PtrVct	=	V_GrP5SkinT1DirArwA34;
		}
		else if(20 > Tv_Angle)
		{
			Tv_PtrVct	=	V_GrP5SkinT1DirArwA45;
		}
		else if(38 > Tv_Angle)
		{
			Tv_PtrVct	=	V_GrP5SkinT1DirArwA56;
		}
		else if(59 > Tv_Angle)
		{
			Tv_PtrVct	=	V_GrP5SkinT1DirArwA68;
		}
		else if(450 > Tv_Angle)
		{
			Tv_PtrVct	=	V_GrP5SkinT1DirArwA79;
		}
		else
		{
			Tv_PtrVct	=	V_GrP5SkinT1DirArwA90;
		}

		LcDrawVct(A_Cx, A_Cy, Tv_PtrVct, &m_ClrArwDirClr, Tv_IsFlipX, Tv_IsFlipY);

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawVct(__s32 A_X, __s32 A_Y, Ptr_GrP5SkinT1VctCmd A_PtrCmd, __u32* A_PtrClrTbl, 
	BOOL8 A_IsFlipX, BOOL8 A_IsFlipY)
{
	// local -------------------
		__u32	Tv_CmdIdx;
		St_GrRect	Tv_Rc;
	// code --------------------
		Tv_CmdIdx	=	0;

		while(E_GrP5SkinT1VctCmdEnd != A_PtrCmd[Tv_CmdIdx].Cmd)
		{
			switch(A_PtrCmd[Tv_CmdIdx].Cmd)
			{
				case E_GrP5SkinT1VctCmdRect:
					if(A_IsFlipX)
					{
						Tv_Rc.left	=	A_X - (__s32)A_PtrCmd[Tv_CmdIdx].Rc.right + 1;
						Tv_Rc.right	=	A_X - (__s32)A_PtrCmd[Tv_CmdIdx].Rc.left + 1;
					}
					else
					{
						Tv_Rc.left	=	A_X + (__s32)A_PtrCmd[Tv_CmdIdx].Rc.left;
						Tv_Rc.right	=	A_X + (__s32)A_PtrCmd[Tv_CmdIdx].Rc.right;
					}
					if(A_IsFlipY)
					{
						Tv_Rc.top			=	A_Y - (__s32)A_PtrCmd[Tv_CmdIdx].Rc.bottom + 1;
						Tv_Rc.bottom	=	A_Y - (__s32)A_PtrCmd[Tv_CmdIdx].Rc.top + 1;
					}
					else
					{
						Tv_Rc.top			=	A_Y + (__s32)A_PtrCmd[Tv_CmdIdx].Rc.top;
						Tv_Rc.bottom	=	A_Y + (__s32)A_PtrCmd[Tv_CmdIdx].Rc.bottom;
					}
					GrP5PntReqFillRect(&Tv_Rc, A_PtrClrTbl[A_PtrCmd[Tv_CmdIdx].ClrIdx]);
					break;
				case E_GrP5SkinT1VctCmdLine:
					if(A_IsFlipX)
					{
						Tv_Rc.left	=	A_X - (__s32)A_PtrCmd[Tv_CmdIdx].Rc.left;
						Tv_Rc.right	=	A_X - (__s32)A_PtrCmd[Tv_CmdIdx].Rc.right;
					}
					else
					{
						Tv_Rc.left	=	A_X + (__s32)A_PtrCmd[Tv_CmdIdx].Rc.left;
						Tv_Rc.right	=	A_X + (__s32)A_PtrCmd[Tv_CmdIdx].Rc.right;
					}
					if(A_IsFlipY)
					{
						Tv_Rc.top			=	A_Y - (__s32)A_PtrCmd[Tv_CmdIdx].Rc.top;
						Tv_Rc.bottom	=	A_Y - (__s32)A_PtrCmd[Tv_CmdIdx].Rc.bottom;
					}
					else
					{
						Tv_Rc.top			=	A_Y + (__s32)A_PtrCmd[Tv_CmdIdx].Rc.top;
						Tv_Rc.bottom	=	A_Y + (__s32)A_PtrCmd[Tv_CmdIdx].Rc.bottom;
					}
					GrP5PntReqLine(Tv_Rc.left, Tv_Rc.top, Tv_Rc.right, Tv_Rc.bottom, A_PtrClrTbl[A_PtrCmd[Tv_CmdIdx].ClrIdx]);
					break;
			}

			// next
			Tv_CmdIdx ++;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::CtlUpdateBase(void* A_ObjUi)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_ObjUi;
		Ptr_GrP5UoHierarchy	Tv_PtrHrcy;
	// code --------------------
		// init
		m_CtlUo							=	A_ObjUi;
		Tv_ObjUi						=	(Cls_GrP5UoBase*)m_CtlUo;
		m_CtlClassId				=	Tv_ObjUi->ClassIdGet();	// update base class id
		if(!Tv_ObjUi->ScrRectGet(&m_CtlRcScr))
		{
			return;
		}

		Tv_PtrHrcy					=	Tv_ObjUi->InheritClassInfoGet(E_GrP5ClassIdUoBase);
		m_PtrCtlParaUoBase	=	Tv_PtrHrcy->PtrPara;
		m_PtrCtlSkinPara		=	&((Ptr_GrP5UoBasePara)m_PtrCtlParaUoBase)->SkinPara;

		switch(m_CtlClassId)
		{
			case E_GrP5ClassIdArwStd:
				LcCtlUpdtArwBase();
				break;
			case E_GrP5ClassIdCalMark:
				LcCtlUpdtCalBase();
				break;
			case E_GrP5ClassIdGridSmd:
			case E_GrP5ClassIdGridSrscd:
				LcCtlUpdtGridBase();
				break;
			case E_GrP5ClassIdGrpHzDmap:
				LcCtlUpdtGrpHzDmap();
				break;
			case E_GrP5ClassIdGrpHzT1:
				LcCtlUpdtGrpHzT1();
				break;
			case E_GrP5ClassIdLblTid:
				LcCtlUpdtLblTid();
				break;
			case E_GrP5ClassIdPnlGrpBar:
				LcCtlUpdtPnlGrpBar();
				break;
			case E_GrP5ClassIdPnlGrpLine:
				LcCtlUpdtPnlGrpLine();
				break;
			case E_GrP5ClassIdQuadT1:
				LcCtlUpdtQuadT1();
				break;
			case E_GrP5ClassIdSpinLtLint:
			case E_GrP5ClassIdSpinNumLint:
			case E_GrP5ClassIdSpinNumSint:
				LcCtlUpdtSpinBase();
				break;
			case E_GrP5ClassIdTabStd:
				LcCtlUpdtTabBase();
				break;
			case E_GrP5ClassIdTabTiRel:
				LcCtlUpdtTabTiRel();
				break;
			case E_GrP5ClassIdTblStd:
				LcCtlUpdtTblBase();
				break;
			case E_GrP5ClassIdPnlDir:
				LcCtlUpdtPnlDir();
				break;

		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::CtlDrawBase(void* A_ObjUi)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_ObjUi;
		Ptr_GrP5UoHierarchy	Tv_PtrHrcy;
	// code --------------------
		// init
		m_CtlUo							=	A_ObjUi;
		Tv_ObjUi						=	(Cls_GrP5UoBase*)m_CtlUo;
		m_CtlClassId				=	Tv_ObjUi->ClassIdGet();	// update base class id
		if(!Tv_ObjUi->ScrRectGet(&m_CtlRcScr))
		{
			return;
		}
		
		Tv_PtrHrcy					=	Tv_ObjUi->InheritClassInfoGet(E_GrP5ClassIdUoBase);
		m_PtrCtlParaUoBase	=	Tv_PtrHrcy->PtrPara;
		m_PtrCtlSkinPara		=	&((Ptr_GrP5UoBasePara)m_PtrCtlParaUoBase)->SkinPara;

		switch(m_CtlClassId)
		{
			case E_GrP5ClassIdArwStd:
				LcCtlDrawArwStd();
				break;
			case E_GrP5ClassIdCalMark:
				LcCtlDrawCalMark();
				break;
			case E_GrP5ClassIdBtnImgTid:
				LcCtlDrawBtnImgTid();
				break;
			case E_GrP5ClassIdBtnLtxt:
				LcCtlDrawBtnLtxt();
				break;
			case E_GrP5ClassIdBtnSint:
				LcCtlDrawBtnSint();
				break;
			case E_GrP5ClassIdBtnStxt:
				LcCtlDrawBtnStxt();
				break;
			case E_GrP5ClassIdBtnTabTid:
				LcCtlDrawBtnTabTid();
				break;
			case E_GrP5ClassIdBtnTid:
				LcCtlDrawBtnTid();
				break;
			case E_GrP5ClassIdBtnTime:
				LcCtlDrawBtnTime();
				break;
			case E_GrP5ClassIdBtnVkey:
				LcCtlDrawBtnVkey();
				break;
			case E_GrP5ClassIdChkImgLtLc:
				LcCtlDrawChkImgLtBase();
				break;
			case E_GrP5ClassIdChkImgTiLc:
				LcCtlDrawChkImgTiBase();
				break;
			case E_GrP5ClassIdChkTiLc:
			case E_GrP5ClassIdChkTiReg:
				LcCtlDrawChkTiBase();
				break;
			case E_GrP5ClassIdGridSmd:
				LcCtlDrawGridSmd();
				break;
			case E_GrP5ClassIdGridSrscd:
				LcCtlDrawGridSrscd();
				break;
			case E_GrP5ClassIdGrpHzDmap:
				LcCtlDrawGrpHzDmap();
				break;
			case E_GrP5ClassIdGrpHzT1:
				LcCtlDrawGrpHzT1();
				break;
			case E_GrP5ClassIdLblLt:
			case E_GrP5ClassIdLblRef:
			case E_GrP5ClassIdLblTid:
				LcCtlDrawLblBase();
				break;
			case E_GrP5ClassIdLblTimeReg:
				LcCtlDrawLblTimeReg();
				break;
			case E_GrP5ClassIdLblTimeView:
				LcCtlDrawLblTimeView();
				break;
			case E_GrP5ClassIdPnlAim:
				LcCtlDrawPnlAim();
				break;
			case E_GrP5ClassIdPnlGrpBar:
				LcCtlDrawPnlGrpBar();
				break;
			case E_GrP5ClassIdPnlGrpLine:
				LcCtlDrawPnlGrpLine();
				break;
			case E_GrP5ClassIdPnlStd:
				LcCtlDrawPnlStd();
				break;
			case E_GrP5ClassIdPrgsRint:
				LcCtlDrawPrgsBase();
				break;
			case E_GrP5ClassIdQuadT1:
				LcCtlDrawQuadT1();
				break;
			case E_GrP5ClassIdSpinLtLint:
			case E_GrP5ClassIdSpinNumLint:
			case E_GrP5ClassIdSpinNumSint:
				LcCtlDrawSpinBase();
				break;
			case E_GrP5ClassIdTabStd:
				LcCtlDrawTabBase();
				break;
			case E_GrP5ClassIdTabTiRel:
				LcCtlDrawTabTiRel();
				break;
			case E_GrP5ClassIdTblStd:
				LcCtlDrawTblBase();
				break;
			case E_GrP5ClassIdWinChild:
				// not draw

				break;
			case E_GrP5ClassIdWinStd:
			case E_GrP5ClassIdWinVkbd:
				LcCtlDrawWinBase();
				break;

			case E_GrP5ClassIdPnlDir:
				LcCtlDrawPnlDir();
				break;

		}

		
}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlUpdtArwBase(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5SkinCtlArwBase	Tv_PtrSkinCtlArwBase;
		Ptr_GrP5UoArwBasePara		Tv_PtrParaArwBase;
	// code --------------------
		Tv_PtrHrcy	=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdArwBase);

		Tv_PtrParaArwBase			=	(Ptr_GrP5UoArwBasePara)Tv_PtrHrcy->PtrPara;
		Tv_PtrSkinCtlArwBase	=	(Ptr_GrP5SkinCtlArwBase)Tv_PtrHrcy->PtrSkinCtl;

		Tv_PtrSkinCtlArwBase->IsKeyCvrOn	=	FALSE;

		if((0 == Tv_PtrSkinCtlArwBase->CellxCnt) || (0 == Tv_PtrSkinCtlArwBase->CellyCnt))
		{
			Tv_PtrSkinCtlArwBase->CellSizeX	=	0;
			Tv_PtrSkinCtlArwBase->CellSizeY	=	0;

			return;
		}
		Tv_PtrSkinCtlArwBase->CellSizeX	=	(m_CtlRcScr.right - m_CtlRcScr.left) / Tv_PtrSkinCtlArwBase->CellxCnt;
		Tv_PtrSkinCtlArwBase->CellSizeY	=	(m_CtlRcScr.bottom - m_CtlRcScr.top) / Tv_PtrSkinCtlArwBase->CellyCnt;



}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlDrawArwBase(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy	Tv_PtrHrcy;
		Ptr_GrP5UoArwBasePara	Tv_PtrParaArwBase;
		Ptr_GrP5SkinCtlArwBase	Tv_PtrCtlArwBase;
		St_GrRect	Tv_Rc;
		__u32		Tv_ArwIdx;
		__s32		Tv_StX;
		__s32		Tv_StY;
		__s32		Tv_EdX;
		__s32		Tv_EdY;
	// code --------------------
		// init
		Tv_PtrHrcy	=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdArwBase);
		Tv_PtrCtlArwBase	=	(Ptr_GrP5SkinCtlArwBase)Tv_PtrHrcy->PtrSkinCtl;
		Tv_PtrParaArwBase	=	(Ptr_GrP5UoArwBasePara)Tv_PtrHrcy->PtrPara;

		// draw background
		if(1 == m_PtrCtlSkinPara->SubType)
		{
			Tv_Rc				=	m_CtlRcScr;
			// on focus
			if(((Cls_GrP5UoBase*)m_CtlUo)->IsOnFocuse())
			{
				GrP5PntReqDrawRect(&Tv_Rc, m_ClrArwBgFocus);
				Tv_Rc.left ++;
				Tv_Rc.top ++;
				Tv_Rc.right --;
				Tv_Rc.bottom --;
			}

			// select
			if(Tv_PtrCtlArwBase->IsKeyCvrOn && Tv_PtrParaArwBase->IsKeyCover)
			{
				GrP5PntReqDrawRect(&Tv_Rc, m_ClrArwBgSelect);
				Tv_Rc.left ++;
				Tv_Rc.top ++;
				Tv_Rc.right --;
				Tv_Rc.bottom --;
			}

			// fill background
			GrP5PntReqFillRect(&Tv_Rc, m_PtrCtlSkinPara->ParaColor);
		}

		// cursor
		LcCtlItmRectArwBase(Tv_PtrCtlArwBase->CusIdxX, Tv_PtrCtlArwBase->CusIdxY, Tv_PtrCtlArwBase->CusIdxX, Tv_PtrCtlArwBase->CusIdxY, &Tv_Rc);
		if(0 < Tv_Rc.right)
		{
			// draw cursor
			LcDrawArwCrosshair(Tv_Rc.left + (Tv_PtrCtlArwBase->CellSizeX >> 1), Tv_Rc.top + (Tv_PtrCtlArwBase->CellSizeY >> 1));
		}

		// draw arraw
		for(Tv_ArwIdx = 0; Tv_ArwIdx < E_GrP5SkinArwMaxCnt; Tv_ArwIdx++)
		{
			// check item exist
			if(((__s16)Tv_PtrParaArwBase->Margin > Tv_PtrCtlArwBase->ArwItms[Tv_ArwIdx].St.x) || ((__s16)(Tv_PtrCtlArwBase->CellxCnt - Tv_PtrParaArwBase->Margin) <= Tv_PtrCtlArwBase->ArwItms[Tv_ArwIdx].St.x) ||
				((__s16)Tv_PtrParaArwBase->Margin > Tv_PtrCtlArwBase->ArwItms[Tv_ArwIdx].St.y) || ((__s16)(Tv_PtrCtlArwBase->CellyCnt - Tv_PtrParaArwBase->Margin) <= Tv_PtrCtlArwBase->ArwItms[Tv_ArwIdx].St.y))
			{
				continue;
			}

			// calculate start position
			LcCtlItmRectArwBase((__s32)Tv_PtrCtlArwBase->ArwItms[Tv_ArwIdx].St.x, (__s32)Tv_PtrCtlArwBase->ArwItms[Tv_ArwIdx].St.y,
				(__s32)Tv_PtrCtlArwBase->ArwItms[Tv_ArwIdx].St.x, (__s32)Tv_PtrCtlArwBase->ArwItms[Tv_ArwIdx].St.y, &Tv_Rc);
			Tv_StX		=	Tv_Rc.left + (__s32)(Tv_PtrCtlArwBase->CellSizeX >> 1);
			Tv_StY		=	Tv_Rc.top + (__s32)(Tv_PtrCtlArwBase->CellSizeY >> 1);

			Tv_EdX		=	-1;
			Tv_EdY		=	-1;
			// check end position
			if(((__s16)Tv_PtrParaArwBase->Margin <= Tv_PtrCtlArwBase->ArwItms[Tv_ArwIdx].Ed.x) && 
				((__s16)(Tv_PtrCtlArwBase->CellxCnt - Tv_PtrParaArwBase->Margin) > Tv_PtrCtlArwBase->ArwItms[Tv_ArwIdx].Ed.x) &&
				((__s16)Tv_PtrParaArwBase->Margin <= Tv_PtrCtlArwBase->ArwItms[Tv_ArwIdx].Ed.y) &&
				((__s16)(Tv_PtrCtlArwBase->CellyCnt - Tv_PtrParaArwBase->Margin) > Tv_PtrCtlArwBase->ArwItms[Tv_ArwIdx].Ed.y)
				)
			{
				LcCtlItmRectArwBase((__s32)Tv_PtrCtlArwBase->ArwItms[Tv_ArwIdx].Ed.x, (__s32)Tv_PtrCtlArwBase->ArwItms[Tv_ArwIdx].Ed.y,
					(__s32)Tv_PtrCtlArwBase->ArwItms[Tv_ArwIdx].Ed.x, (__s32)Tv_PtrCtlArwBase->ArwItms[Tv_ArwIdx].Ed.y, &Tv_Rc);

				Tv_EdX		=	Tv_Rc.left + (__s32)(Tv_PtrCtlArwBase->CellSizeX >> 1);
				Tv_EdY		=	Tv_Rc.top + (__s32)(Tv_PtrCtlArwBase->CellSizeY >> 1);
			}

			// draw
			LcDrawArwItem(Tv_StX, Tv_StY, Tv_EdX, Tv_EdY, Tv_ArwIdx, Tv_PtrCtlArwBase->ArwItms[Tv_ArwIdx].DirMap);
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlDrawArwStd(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy	Tv_PtrHrcy;
		Ptr_GrP5UoArwStdPara	Tv_PtrParaArwStd;
		Ptr_GrP5SkinCtlArwStd	Tv_PtrCtlArwStd;
		__u32	Tv_RcIdx;
	// code --------------------
		// init
		Tv_PtrHrcy	=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdArwStd);
		Tv_PtrCtlArwStd		=	(Ptr_GrP5SkinCtlArwStd)Tv_PtrHrcy->PtrSkinCtl;
		Tv_PtrParaArwStd	=	(Ptr_GrP5UoArwStdPara)Tv_PtrHrcy->PtrPara;

		// draw base
		LcCtlDrawArwBase();

		// draw rect
		for(Tv_RcIdx = 0; Tv_RcIdx < E_GrP5SkinArwStdRectMaxCnt; Tv_RcIdx++)
		{
			if(0 < Tv_PtrCtlArwStd->RcTbl[Tv_RcIdx].right)
			{
				GrP5PntReqDrawRect(&Tv_PtrCtlArwStd->RcTbl[Tv_RcIdx], Tv_PtrParaArwStd->ClrRect[Tv_RcIdx]);
			}
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::CtlGetItemRangeRect(void* A_ObjUi, __s32 A_ItmStX, __s32 A_ItmStY, __s32 A_ItmEdX, __s32 A_ItmEdY, Ptr_GrRect A_PtrRtRect)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_ObjUi;
		Ptr_GrP5UoHierarchy	Tv_PtrHrcy;
	// code --------------------
		// init
		m_CtlUo							=	A_ObjUi;
		Tv_ObjUi						=	(Cls_GrP5UoBase*)m_CtlUo;
		m_CtlClassId				=	Tv_ObjUi->ClassIdGet();	// update base class id

		A_PtrRtRect->right	=	0;

		if(!Tv_ObjUi->ScrRectGet(&m_CtlRcScr))
		{
			return;
		}

		Tv_PtrHrcy					=	Tv_ObjUi->InheritClassInfoGet(E_GrP5ClassIdUoBase);
		m_PtrCtlParaUoBase	=	Tv_PtrHrcy->PtrPara;
		m_PtrCtlSkinPara		=	&((Ptr_GrP5UoBasePara)m_PtrCtlParaUoBase)->SkinPara;

		switch(m_CtlClassId)
		{
			case E_GrP5ClassIdArwStd:
				LcCtlItmRectArwBase(A_ItmStX, A_ItmStY, A_ItmEdX, A_ItmEdY, A_PtrRtRect);
				break;
			case E_GrP5ClassIdCalMark:
				LcCtlItmRectCalBase(A_ItmStX, A_ItmStY, A_ItmEdX, A_ItmEdY, A_PtrRtRect);
				break;
			case E_GrP5ClassIdGridSmd:
			case E_GrP5ClassIdGridSrscd:
				LcCtlItmRectGridBase(A_ItmStX, A_ItmStY, A_ItmEdX, A_ItmEdY, A_PtrRtRect);
				break;
			case E_GrP5ClassIdGrpHzDmap:
				LcCtlItmRectGrpHzDmap(A_ItmStX, A_ItmStY, A_ItmEdX, A_ItmEdY, A_PtrRtRect);
				break;
			case E_GrP5ClassIdGrpHzT1:
				LcCtlItmRectGrpHzT1(A_ItmStX, A_ItmStY, A_ItmEdX, A_ItmEdY, A_PtrRtRect);
				break;
			case E_GrP5ClassIdTblStd:
				LcCtlItmRectTblBase(A_ItmStX, A_ItmStY, A_ItmEdX, A_ItmEdY, A_PtrRtRect);
				break;
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlItmRectArwBase(__s32 A_ItmStX, __s32 A_ItmStY, __s32 A_ItmEdX, __s32 A_ItmEdY, Ptr_GrRect A_PtrRtRect)
{
	// local -------------------
		Ptr_GrP5UoHierarchy	Tv_PtrHrcy;
		Ptr_GrP5UoArwBasePara	Tv_PtrParaArwBase;
		Ptr_GrP5SkinCtlArwBase	Tv_PtrCtlArwBase;
	// code --------------------

		// init
		Tv_PtrHrcy	=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdArwBase);
		Tv_PtrCtlArwBase	=	(Ptr_GrP5SkinCtlArwBase)Tv_PtrHrcy->PtrSkinCtl;
		Tv_PtrParaArwBase	=	(Ptr_GrP5UoArwBasePara)Tv_PtrHrcy->PtrPara;

		A_PtrRtRect->right	=	0;

		// check parametter
		if((A_ItmStX < (__s32)Tv_PtrParaArwBase->Margin) || (A_ItmStX >= (__s32)(Tv_PtrCtlArwBase->CellxCnt - Tv_PtrParaArwBase->Margin)) ||
			(A_ItmStY < (__s32)Tv_PtrParaArwBase->Margin) || (A_ItmStY >= (__s32)(Tv_PtrCtlArwBase->CellyCnt - Tv_PtrParaArwBase->Margin)))
		{
			return;
		}
		
		A_PtrRtRect->left			=	(__s32)Tv_PtrCtlArwBase->CellSizeX * A_ItmStX;
		A_PtrRtRect->top			=	(__s32)Tv_PtrCtlArwBase->CellSizeY * A_ItmStY;

		if(A_ItmStX != A_ItmEdX)
		{
			if(A_ItmStX <= A_ItmEdX)
			{
				A_PtrRtRect->right	=	(__s32)Tv_PtrCtlArwBase->CellSizeX * A_ItmEdX;
			}
			else
			{
				A_PtrRtRect->right	=	A_PtrRtRect->left;
				A_PtrRtRect->left		=	(__s32)Tv_PtrCtlArwBase->CellSizeX * A_ItmEdX;
			}
			A_PtrRtRect->right	=	A_PtrRtRect->right + (__s32)Tv_PtrCtlArwBase->CellSizeX;
		}
		else
		{
			A_PtrRtRect->right	=	A_PtrRtRect->left + (__s32)Tv_PtrCtlArwBase->CellSizeX;
		}

		if(A_ItmStY != A_ItmEdY)
		{
			if(A_ItmStY <= A_ItmEdY)
			{
				A_PtrRtRect->bottom	=	(__s32)Tv_PtrCtlArwBase->CellSizeY * A_ItmEdY;
			}
			else
			{
				A_PtrRtRect->bottom	=	A_PtrRtRect->top;
				A_PtrRtRect->top		=	(__s32)Tv_PtrCtlArwBase->CellSizeY * A_ItmEdY;
			}
			A_PtrRtRect->bottom	=	A_PtrRtRect->bottom + (__s32)Tv_PtrCtlArwBase->CellSizeY;
		}
		else
		{
			A_PtrRtRect->bottom	=	A_PtrRtRect->top + (__s32)Tv_PtrCtlArwBase->CellSizeY;
		}

		// patch
		A_PtrRtRect->left		=	A_PtrRtRect->left + m_CtlRcScr.left;
		A_PtrRtRect->top		=	A_PtrRtRect->top + m_CtlRcScr.top;
		A_PtrRtRect->right	=	A_PtrRtRect->right + m_CtlRcScr.left;
		A_PtrRtRect->bottom	=	A_PtrRtRect->bottom + m_CtlRcScr.top;

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::CtlGetItemIdxByPos(void* A_ObjUi, __s32 A_X, __s32 A_Y, __s32* A_PtrRtIdxX, __s32* A_PtrRtIdxY)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_ObjUi;
		Ptr_GrP5UoHierarchy	Tv_PtrHrcy;
	// code --------------------
		// init
		m_CtlUo							=	A_ObjUi;
		Tv_ObjUi						=	(Cls_GrP5UoBase*)m_CtlUo;
		m_CtlClassId				=	Tv_ObjUi->ClassIdGet();	// update base class id

		*A_PtrRtIdxX				=	-1;
		*A_PtrRtIdxY				=	-1;

		if(!Tv_ObjUi->ScrRectGet(&m_CtlRcScr))
		{
			return;
		}

		Tv_PtrHrcy					=	Tv_ObjUi->InheritClassInfoGet(E_GrP5ClassIdUoBase);
		m_PtrCtlParaUoBase	=	Tv_PtrHrcy->PtrPara;
		m_PtrCtlSkinPara		=	&((Ptr_GrP5UoBasePara)m_PtrCtlParaUoBase)->SkinPara;

		switch(m_CtlClassId)
		{
			case E_GrP5ClassIdArwStd:
				LcCtlItemIdxArwBase(A_X, A_Y, A_PtrRtIdxX, A_PtrRtIdxY);
				break;
			case E_GrP5ClassIdCalMark:
				LcCtlItemIdxCalBase(A_X, A_Y, A_PtrRtIdxX, A_PtrRtIdxY);
				break;
			case E_GrP5ClassIdGridSmd:
			case E_GrP5ClassIdGridSrscd:
				LcCtlItemIdxGridBase(A_X, A_Y, A_PtrRtIdxX, A_PtrRtIdxY);
				break;
			case E_GrP5ClassIdGrpHzDmap:
				LcCtlItemIdxGrpHzDmap(A_X, A_Y, A_PtrRtIdxX, A_PtrRtIdxY);
				break;
			case E_GrP5ClassIdGrpHzT1:
				LcCtlItemIdxGrpHzT1(A_X, A_Y, A_PtrRtIdxX, A_PtrRtIdxY);
				break;
			case E_GrP5ClassIdSpinLtLint:
			case E_GrP5ClassIdSpinNumLint:
			case E_GrP5ClassIdSpinNumSint:
				LcCtlItemIdxSpinBase(A_X, A_Y, A_PtrRtIdxX, A_PtrRtIdxY);
				break;
			case E_GrP5ClassIdTblStd:
				LcCtlItemIdxTblBase(A_X, A_Y, A_PtrRtIdxX, A_PtrRtIdxY);
				break;
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlItemIdxArwBase(__s32 A_X, __s32 A_Y, __s32* A_PtrRtIdxX, __s32* A_PtrRtIdxY)
{
	// local -------------------
		Ptr_GrP5UoHierarchy	Tv_PtrHrcy;
		Ptr_GrP5UoArwBasePara	Tv_PtrParaArwBase;
		Ptr_GrP5SkinCtlArwBase	Tv_PtrCtlArwBase;
		__s32	Tv_IdxX;
		__s32	Tv_IdxY;
	// code --------------------

		// init
		Tv_PtrHrcy	=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdArwBase);
		Tv_PtrCtlArwBase	=	(Ptr_GrP5SkinCtlArwBase)Tv_PtrHrcy->PtrSkinCtl;
		Tv_PtrParaArwBase	=	(Ptr_GrP5UoArwBasePara)Tv_PtrHrcy->PtrPara;

		// check range
		if((A_X < m_CtlRcScr.left) || (A_X >= m_CtlRcScr.right) || (A_Y < m_CtlRcScr.top) || (A_Y >= m_CtlRcScr.bottom))
		{
			return;
		}

		// check status
		if((0 == Tv_PtrCtlArwBase->CellSizeX) || (0 == Tv_PtrCtlArwBase->CellSizeY ))
		{
			return;
		}

		Tv_IdxX		=	(A_X - m_CtlRcScr.left) / Tv_PtrCtlArwBase->CellSizeX;
		Tv_IdxY		=	(A_Y - m_CtlRcScr.top) / Tv_PtrCtlArwBase->CellSizeY;

		if((Tv_IdxX >= (__s32)Tv_PtrCtlArwBase->CellxCnt) || (Tv_IdxY >= (__s32)Tv_PtrCtlArwBase->CellyCnt))
		{
			Tv_IdxX		=	-1;
			Tv_IdxY		=	-1;
		}

		*A_PtrRtIdxX		=	Tv_IdxX;
		*A_PtrRtIdxY		=	Tv_IdxY;

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlDrawCalMark(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy	Tv_PtrHrcy;
		Ptr_GrP5UoCalBasePara	Tv_PtrParaCalBase;
		Ptr_GrP5SkinCtlCalBase Tv_PtrCtlCalBase;

		Ptr_GrP5UoCalMarkPara	Tv_PtrParaCalMark;

		__u32	Tv_IdxX;
		__u32	Tv_IdxY;

		BOOL8	Tv_IsSel;
		BOOL8	Tv_IsCus;

		__u32	Tv_Clr;
		St_GrRect	Tv_RcItm;
		St_GrRect	Tv_RcWork;
	// code --------------------
		// init
		Tv_PtrHrcy	=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdCalBase);
		Tv_PtrParaCalBase	=	(Ptr_GrP5UoCalBasePara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlCalBase		=	(Ptr_GrP5SkinCtlCalBase)Tv_PtrHrcy->PtrSkinCtl;

		Tv_PtrHrcy	=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdCalMark);
		Tv_PtrParaCalMark	=	(Ptr_GrP5UoCalMarkPara)Tv_PtrHrcy->PtrPara;

		// draw background

		// draw item
		for(Tv_IdxY = 0; Tv_IdxY < E_GrP5SkinCalDayLineCnt; Tv_IdxY++)
		{
			for(Tv_IdxX = 0; Tv_IdxX < E_GrTimeDayOfWeek; Tv_IdxX++)
			{
				// check draw able
				if(0 == Tv_PtrCtlCalBase->DayTbl[Tv_IdxY][Tv_IdxX].Day)
				{
					continue;
				}

				// get rect
				LcCtlItmRectCalBase((__s32)Tv_IdxX, (__s32)Tv_IdxY, (__s32)Tv_IdxX, (__s32)Tv_IdxY, &Tv_RcItm);

				// select day
				Tv_IsSel		=	FALSE;
				if((Tv_PtrCtlCalBase->SelYear == Tv_PtrCtlCalBase->ViewYear) && (Tv_PtrCtlCalBase->SelMonth == Tv_PtrCtlCalBase->ViewMonth) && 
					(Tv_PtrCtlCalBase->SelDay == Tv_PtrCtlCalBase->DayTbl[Tv_IdxY][Tv_IdxX].Day))
				{
					Tv_IsSel		=	TRUE;
				}
				// on cursor
				Tv_IsCus		=	FALSE;
				if(((__u32)Tv_PtrCtlCalBase->CusIdxX == Tv_IdxX) && ((__u32)Tv_PtrCtlCalBase->CusIdxY == Tv_IdxY))
				{
					Tv_IsCus		=	TRUE;
				}

				// draw cursor area
				if(Tv_IsCus || (0 != Tv_PtrCtlCalBase->DayTbl[Tv_IdxY][Tv_IdxX].MarkMap))
				{

					Tv_Clr	=	m_ClrCalCusNrm;
					if(0 != Tv_PtrCtlCalBase->DayTbl[Tv_IdxY][Tv_IdxX].MarkMap)
					{
						Tv_Clr	=	m_ClrCalMark;
						if(Tv_IsCus)
						{
							//Tv_Clr	=	m_ClrCalCusSel;
							Tv_Clr	=	m_ClrCalCusNrm;
						}
					}
					// draw 
					Tv_RcWork.left		=	Tv_RcItm.left;
					Tv_RcWork.top			=	Tv_RcItm.top;
					Tv_RcWork.right		=	Tv_RcItm.right;
					Tv_RcWork.bottom	=	Tv_RcItm.top + E_GrP5SkinT1CalCusWidth;
					GrP5PntReqFillRect(&Tv_RcWork, Tv_Clr);
					Tv_RcWork.top			=	Tv_RcItm.bottom - E_GrP5SkinT1CalCusWidth;
					Tv_RcWork.bottom	=	Tv_RcItm.bottom;
					GrP5PntReqFillRect(&Tv_RcWork, Tv_Clr);
					Tv_RcWork.top			=	Tv_RcItm.top + E_GrP5SkinT1CalCusWidth;
					Tv_RcWork.right		=	Tv_RcItm.left + E_GrP5SkinT1CalCusWidth;
					Tv_RcWork.bottom	=	Tv_RcItm.bottom - E_GrP5SkinT1CalCusWidth;
					GrP5PntReqFillRect(&Tv_RcWork, Tv_Clr);
					Tv_RcWork.left		=	Tv_RcItm.right - E_GrP5SkinT1CalCusWidth;
					Tv_RcWork.right		=	Tv_RcItm.right;
					GrP5PntReqFillRect(&Tv_RcWork, Tv_Clr);
				}

				// darw internerl
				// check exist item
				if(Tv_IsSel || (0 != Tv_PtrCtlCalBase->DayTbl[Tv_IdxY][Tv_IdxX].MarkMap))
				{
					Tv_Clr	=	m_ClrCalMark;
					if(Tv_IsSel)
					{
						Tv_Clr	=	m_ClrCalSel;
					}

					Tv_RcWork.left		=	Tv_RcItm.left + E_GrP5SkinT1CalCusWidth;
					Tv_RcWork.top			=	Tv_RcItm.top + E_GrP5SkinT1CalCusWidth;
					Tv_RcWork.right		=	Tv_RcItm.right - E_GrP5SkinT1CalCusWidth;
					Tv_RcWork.bottom	=	Tv_RcItm.bottom - E_GrP5SkinT1CalCusWidth;

					GrP5PntReqFillRect(&Tv_RcWork, Tv_Clr);
				}

				// draw text
				if(GrP5RelRectToAbsRect(&Tv_PtrParaCalBase->TxtRc, &Tv_RcItm, &Tv_RcWork))
				{
					GrP5PntReqTxtStyle(&Tv_PtrParaCalBase->TxtPara, Tv_PtrParaCalBase->FontId);
					GrP5PntReqTxtCus();
					GrP5PntReqTxtDraw(&Tv_RcWork, V_GrP5SkinT1StrDayTbl[Tv_PtrCtlCalBase->DayTbl[Tv_IdxY][Tv_IdxX].Day -1], 0);	// txt buf ok
				}

			}	//	for(Tv_IdxX = 0; Tv_IdxX < E_GrTimeDayOfWeek; Tv_IdxX++)
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlItmRectCalBase(__s32 A_ItmStX, __s32 A_ItmStY, __s32 A_ItmEdX, __s32 A_ItmEdY, Ptr_GrRect A_PtrRtRect)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoCalBasePara	Tv_PtrParaCalBase;
		Ptr_GrP5SkinCtlCalBase Tv_PtrCtlCalBase;
		__s32	Tv_Tmp;
	// code --------------------
		// init
		Tv_PtrHrcy	=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdCalBase);
		Tv_PtrParaCalBase	=	(Ptr_GrP5UoCalBasePara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlCalBase		=	(Ptr_GrP5SkinCtlCalBase)Tv_PtrHrcy->PtrSkinCtl;

		A_PtrRtRect->right	=	0;

		if(0 == Tv_PtrCtlCalBase->DayVwSizeX)
		{
			return;
		}

		if((0 > A_ItmStX) || (E_GrTimeDayOfWeek <= A_ItmStX) || (0 > A_ItmStY) || (E_GrP5SkinCalDayLineCnt <= A_ItmStY) ||
			(0 > A_ItmEdX) || (E_GrTimeDayOfWeek <= A_ItmEdX) || (0 > A_ItmEdY) || (E_GrP5SkinCalDayLineCnt <= A_ItmEdY) )
		{
			return;
		}

		A_PtrRtRect->left			=	((__s32)Tv_PtrCtlCalBase->DayVwSizeX * A_ItmStX) + E_GrP5SkinT1CalMargin;
		A_PtrRtRect->top			=	((__s32)Tv_PtrCtlCalBase->DayVwSizeY * A_ItmStY) + E_GrP5SkinT1CalMargin;
		A_PtrRtRect->right		=	A_PtrRtRect->left;
		A_PtrRtRect->bottom		=	A_PtrRtRect->top;
		if( A_ItmStX != A_ItmEdX )
		{
			A_PtrRtRect->right		=	((__s32)Tv_PtrCtlCalBase->DayVwSizeX * A_ItmEdX) + E_GrP5SkinT1CalMargin;
		}
		if(A_ItmStY != A_ItmEdY)
		{
			A_PtrRtRect->bottom		=	((__s32)Tv_PtrCtlCalBase->DayVwSizeY * A_ItmEdY) + E_GrP5SkinT1CalMargin;
		}

		if(A_PtrRtRect->left > A_PtrRtRect->right)
		{
			Tv_Tmp		=	A_PtrRtRect->left;
			A_PtrRtRect->left		=	A_PtrRtRect->right;
			A_PtrRtRect->right	=	Tv_Tmp;
		}

		if(A_PtrRtRect->top > A_PtrRtRect->bottom)
		{
			Tv_Tmp		=	A_PtrRtRect->top;
			A_PtrRtRect->top		=	A_PtrRtRect->bottom;
			A_PtrRtRect->bottom	=	Tv_Tmp;
		}

		// patch
		A_PtrRtRect->left			=	A_PtrRtRect->left + m_CtlRcScr.left;
		A_PtrRtRect->top			=	A_PtrRtRect->top + m_CtlRcScr.top;
		A_PtrRtRect->right		=	A_PtrRtRect->right + (__s32)Tv_PtrCtlCalBase->DayVwSizeX + m_CtlRcScr.left;
		A_PtrRtRect->bottom		=	A_PtrRtRect->bottom + (__s32)Tv_PtrCtlCalBase->DayVwSizeY + m_CtlRcScr.top;

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlItmRectGridBase(__s32 A_ItmStX, __s32 A_ItmStY, __s32 A_ItmEdX, __s32 A_ItmEdY, Ptr_GrRect A_PtrRtRect)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoGridBasePara	Tv_PtrParaGridBase;
		Ptr_GrP5SkinCtlGridBase	Tv_PtrCtlGridBase;
	// code --------------------
		// init
		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdGridBase);
		Tv_PtrParaGridBase		=	(Ptr_GrP5UoGridBasePara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlGridBase			=	(Ptr_GrP5SkinCtlGridBase)Tv_PtrHrcy->PtrSkinCtl;

		A_PtrRtRect->right	=	0;

		if((0 == Tv_PtrCtlGridBase->CellSizeX) || (0 == Tv_PtrCtlGridBase->CellSizeY))
		{
			return;
		}

		if((0 > A_ItmStX) || ((__s32)Tv_PtrCtlGridBase->CellCntX <= A_ItmStX) || (0 > A_ItmStY) || ((__s32)Tv_PtrCtlGridBase->CellCntY <= A_ItmStY) ||
			(0 > A_ItmEdX) || ((__s32)Tv_PtrCtlGridBase->CellCntX <= A_ItmEdX) || (0 > A_ItmEdY) || ((__s32)Tv_PtrCtlGridBase->CellCntY <= A_ItmEdY))
		{
			return;
		}

		// calculate x
		if(A_ItmStX < A_ItmEdX)
		{
			A_PtrRtRect->left			=	(A_ItmStX * (__s32)Tv_PtrCtlGridBase->CellSizeX) + m_CtlRcScr.left;
			A_PtrRtRect->right		=	(A_ItmEdX * (__s32)Tv_PtrCtlGridBase->CellSizeX) + m_CtlRcScr.left;
		}
		else if(A_ItmStX > A_ItmEdX)
		{
			A_PtrRtRect->left			=	(A_ItmEdX * (__s32)Tv_PtrCtlGridBase->CellSizeX) + m_CtlRcScr.left;
			A_PtrRtRect->right		=	(A_ItmStX * (__s32)Tv_PtrCtlGridBase->CellSizeX) + m_CtlRcScr.left;
		}
		else
		{
			A_PtrRtRect->left			=	(A_ItmStX * (__s32)Tv_PtrCtlGridBase->CellSizeX) + m_CtlRcScr.left;
			A_PtrRtRect->right		=	A_PtrRtRect->left;
		}
		A_PtrRtRect->right		=	A_PtrRtRect->right + (__s32)Tv_PtrCtlGridBase->CellSizeX;

		// calculate y
		if(A_ItmStY < A_ItmEdY)
		{
			A_PtrRtRect->top			=	(A_ItmStY * (__s32)Tv_PtrCtlGridBase->CellSizeY) + m_CtlRcScr.top;
			A_PtrRtRect->bottom		=	(A_ItmEdY * (__s32)Tv_PtrCtlGridBase->CellSizeY) + m_CtlRcScr.top;
		}
		else if(A_ItmStY > A_ItmEdY)
		{
			A_PtrRtRect->top			=	(A_ItmEdY * (__s32)Tv_PtrCtlGridBase->CellSizeY) + m_CtlRcScr.top;
			A_PtrRtRect->bottom		=	(A_ItmStY * (__s32)Tv_PtrCtlGridBase->CellSizeY) + m_CtlRcScr.top;
		}
		else
		{
			A_PtrRtRect->top			=	(A_ItmStY * (__s32)Tv_PtrCtlGridBase->CellSizeY) + m_CtlRcScr.top;
			A_PtrRtRect->bottom		=	A_PtrRtRect->top;
		}
		A_PtrRtRect->bottom		=	A_PtrRtRect->bottom + (__s32)Tv_PtrCtlGridBase->CellSizeY;

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlItmRectGrpHzDmap(__s32 A_ItmStX, __s32 A_ItmStY, __s32 A_ItmEdX, __s32 A_ItmEdY, Ptr_GrRect A_PtrRtRect)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoGrpHzBasePara		Tv_PtrParaGrpHzBase;
		Ptr_GrP5SkinCtlGrpHzBase	Tv_PtrCtlGrpHzBase;

		Ptr_GrP5UoGrpHzDmapPara		Tv_PtrParaGrpHzDmap;
		Ptr_GrP5SkinCtlGrpHzDmap	Tv_PtrCtlGrpHzDmap;

		__s32	Tv_IdxStY;
		__s32	Tv_IdxEdY;
	// code --------------------
		// init
		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdGrpHzBase);
		Tv_PtrParaGrpHzBase		=	(Ptr_GrP5UoGrpHzBasePara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlGrpHzBase		=	(Ptr_GrP5SkinCtlGrpHzBase)Tv_PtrHrcy->PtrSkinCtl;

		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdGrpHzDmap);
		Tv_PtrParaGrpHzDmap		=	(Ptr_GrP5UoGrpHzDmapPara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlGrpHzDmap		=	(Ptr_GrP5SkinCtlGrpHzDmap)Tv_PtrHrcy->PtrSkinCtl;

		A_PtrRtRect->right	=	0;

		if((0 == Tv_PtrCtlGrpHzDmap->CellSizeX) || (0 == Tv_PtrCtlGrpHzDmap->VwCntY))
		{
			return;
		}

		if((0 > A_ItmStX) || ((__s32)Tv_PtrCtlGrpHzBase->CellCntX <= A_ItmStX) || (0 > A_ItmStY) || ((__s32)Tv_PtrCtlGrpHzBase->CellCntY <= A_ItmStY) ||
			(0 > A_ItmEdX) || ((__s32)Tv_PtrCtlGrpHzBase->CellCntX <= A_ItmEdX) || (0 > A_ItmEdY) || ((__s32)Tv_PtrCtlGrpHzBase->CellCntY <= A_ItmEdY))
		{
			return;
		}

		// check view range
		if((A_ItmStY >= (__s32)Tv_PtrCtlGrpHzDmap->VwCntY) && (A_ItmEdY >= (__s32)Tv_PtrCtlGrpHzDmap->VwCntY))
		{
			return;
		}

		// calculate x
		if(A_ItmStX < A_ItmEdX)
		{
			A_PtrRtRect->left			=	A_ItmStX * (__s32)Tv_PtrCtlGrpHzDmap->CellSizeX;
			A_PtrRtRect->right		=	A_ItmEdX * (__s32)Tv_PtrCtlGrpHzDmap->CellSizeX;
		}
		else if(A_ItmStX > A_ItmEdX)
		{
			A_PtrRtRect->left			=	A_ItmEdX * (__s32)Tv_PtrCtlGrpHzDmap->CellSizeX;
			A_PtrRtRect->right		=	A_ItmStX * (__s32)Tv_PtrCtlGrpHzDmap->CellSizeX;
		}
		else
		{
			A_PtrRtRect->left			=	A_ItmStX * (__s32)Tv_PtrCtlGrpHzDmap->CellSizeX;
			A_PtrRtRect->right		=	A_PtrRtRect->left;
		}

		Tv_IdxStY		=	A_ItmStY;
		Tv_IdxEdY		=	A_ItmEdY;
		// patch
		if(Tv_IdxStY >= (__s32)Tv_PtrCtlGrpHzDmap->VwCntY)
		{
			Tv_IdxStY		=	(__s32)Tv_PtrCtlGrpHzDmap->VwCntY - 1;
		}
		if(Tv_IdxEdY >= (__s32)Tv_PtrCtlGrpHzDmap->VwCntY)
		{
			Tv_IdxEdY		=	(__s32)Tv_PtrCtlGrpHzDmap->VwCntY - 1;
		}

		// calculate y
		if(Tv_IdxStY < Tv_IdxEdY)
		{
			A_PtrRtRect->top			=	Tv_IdxStY * (__s32)Tv_PtrCtlGrpHzDmap->CellSizeY;
			A_PtrRtRect->bottom		=	Tv_IdxEdY * (__s32)Tv_PtrCtlGrpHzDmap->CellSizeY;
		}
		else if(Tv_IdxStY > Tv_IdxEdY)
		{
			A_PtrRtRect->top			=	Tv_IdxEdY * (__s32)Tv_PtrCtlGrpHzDmap->CellSizeY;
			A_PtrRtRect->bottom		=	Tv_IdxStY * (__s32)Tv_PtrCtlGrpHzDmap->CellSizeY;
		}
		else
		{
			A_PtrRtRect->top			=	Tv_IdxStY * (__s32)Tv_PtrCtlGrpHzDmap->CellSizeY;
			A_PtrRtRect->bottom		=	A_PtrRtRect->top;
		}

		// patch
		A_PtrRtRect->left			=	A_PtrRtRect->left + E_GrP5SkinT1GraphHzMargin;
		A_PtrRtRect->top			=	A_PtrRtRect->top + E_GrP5SkinT1GraphHzMargin;
		A_PtrRtRect->right		=	A_PtrRtRect->right + (__s32)Tv_PtrCtlGrpHzDmap->CellSizeX + E_GrP5SkinT1GraphHzMargin;
		A_PtrRtRect->bottom		=	A_PtrRtRect->bottom + (__s32)Tv_PtrCtlGrpHzDmap->CellSizeY + E_GrP5SkinT1GraphHzMargin;

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlItmRectGrpHzT1(__s32 A_ItmStX, __s32 A_ItmStY, __s32 A_ItmEdX, __s32 A_ItmEdY, Ptr_GrRect A_PtrRtRect)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoGrpHzBasePara		Tv_PtrParaGrpHzBase;
		Ptr_GrP5SkinCtlGrpHzBase	Tv_PtrCtlGrpHzBase;

		Ptr_GrP5UoGrpHzT1Para		Tv_PtrParaGrpHzT1;
		Ptr_GrP5SkinCtlGrpHzT1	Tv_PtrCtlGrpHzT1;

		__s32	Tv_StX;
		__s32	Tv_EdX;
		__s32	Tv_StY;
		__s32	Tv_EdY;

	// code --------------------
		// init
		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdGrpHzBase);
		Tv_PtrParaGrpHzBase		=	(Ptr_GrP5UoGrpHzBasePara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlGrpHzBase		=	(Ptr_GrP5SkinCtlGrpHzBase)Tv_PtrHrcy->PtrSkinCtl;

		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdGrpHzT1);
		Tv_PtrParaGrpHzT1		=	(Ptr_GrP5UoGrpHzT1Para)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlGrpHzT1		=	(Ptr_GrP5SkinCtlGrpHzT1)Tv_PtrHrcy->PtrSkinCtl;

		A_PtrRtRect->right	=	0;

		if((0 == Tv_PtrCtlGrpHzT1->VwCntX) || (0 == Tv_PtrCtlGrpHzT1->VwCntY))
		{
			return;
		}

		// check x range
		if((0 > A_ItmStX) || ((__s32)Tv_PtrCtlGrpHzBase->CellCntX <= A_ItmStX) || (0 > A_ItmEdX) || ((__s32)Tv_PtrCtlGrpHzT1->VwCntX <= A_ItmEdX))
		{
			return;
		}

		// check indicater
		if( (0 > A_ItmStY) && (0 > A_ItmEdY) )
		{
			// calculate indicater
			A_PtrRtRect->left			=	m_CtlRcScr.left + E_GrP5SkinT1GrpHzT1ContMgn + (Tv_PtrCtlGrpHzT1->CellSizeX * A_ItmStX);
			A_PtrRtRect->right		=	A_PtrRtRect->left + (__s32)(__u32)Tv_PtrParaGrpHzT1->IndWidth;
			A_PtrRtRect->top			=	m_CtlRcScr.top;
			A_PtrRtRect->bottom		=	A_PtrRtRect->top + (__s32)(__u32)Tv_PtrParaGrpHzT1->RulerHeight;

			if(A_PtrRtRect->right > m_CtlRcScr.right)
			{
				A_PtrRtRect->right	=	m_CtlRcScr.right;
				A_PtrRtRect->left		=	A_PtrRtRect->right - (__s32)(__u32)Tv_PtrParaGrpHzT1->IndWidth;
			}
			return;
		}

		// check y range
		if((0 > A_ItmStY) || (0 >  A_ItmEdY))
		{
			return;
		}

		Tv_StX		=	A_ItmStX;
		Tv_StY		=	A_ItmStY;
		Tv_EdX		=	A_ItmEdX;
		Tv_EdY		=	A_ItmEdY;

		if(Tv_StX > Tv_EdX)
		{
			Tv_StX		=	A_ItmEdX;
			Tv_EdX		=	A_ItmStX;
		}
		if(Tv_StY > Tv_EdY)
		{
			Tv_StY		=	A_ItmEdY;
			Tv_EdY		=	A_ItmStY;
		}

		// check patch y
		if(Tv_StY >= (__s32)Tv_PtrCtlGrpHzT1->VwCntY)
		{
			Tv_StY	=	(__s32)Tv_PtrCtlGrpHzT1->VwCntY -1;
		}
		if(Tv_EdY >= (__s32)Tv_PtrCtlGrpHzT1->VwCntY)
		{
			Tv_EdY	=	(__s32)Tv_PtrCtlGrpHzT1->VwCntY -1;
		}

		A_PtrRtRect->left			=	(Tv_StX * (__s32)Tv_PtrCtlGrpHzT1->CellSizeX) + m_CtlRcScr.left + E_GrP5SkinT1GrpHzT1ContMgn;
		A_PtrRtRect->top			=	(Tv_StY * (__s32)Tv_PtrCtlGrpHzT1->CellSizeY) + m_CtlRcScr.top + E_GrP5SkinT1GrpHzT1ContMgn + + (__s32)(__u32)Tv_PtrParaGrpHzT1->RulerHeight;
		if(Tv_StX == Tv_EdX)
		{
			A_PtrRtRect->right		=	A_PtrRtRect->left + (__s32)Tv_PtrCtlGrpHzT1->CellSizeX;
		}
		else
		{
			A_PtrRtRect->right		=	((Tv_EdX + 1) * (__s32)Tv_PtrCtlGrpHzT1->CellSizeX) + m_CtlRcScr.left + E_GrP5SkinT1GrpHzT1ContMgn;
		}
		if(Tv_StY == Tv_EdY)
		{
			A_PtrRtRect->bottom		=	A_PtrRtRect->top + (__s32)Tv_PtrCtlGrpHzT1->CellSizeY;
		}
		else
		{
			A_PtrRtRect->bottom		=	((Tv_EdY + 1) * (__s32)Tv_PtrCtlGrpHzT1->CellSizeY) + m_CtlRcScr.top + E_GrP5SkinT1GrpHzT1ContMgn + + (__s32)(__u32)Tv_PtrParaGrpHzT1->RulerHeight;
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlItmRectTblBase(__s32 A_ItmStX, __s32 A_ItmStY, __s32 A_ItmEdX, __s32 A_ItmEdY, Ptr_GrRect A_PtrRtRect)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoTblBasePara		Tv_PtrParaTblBase;
		Ptr_GrP5SkinCtlTblBase	Tv_PtrCtlTblBase;

		__s32	Tv_StX;
		__s32	Tv_StY;
		__s32	Tv_EdX;
		__s32	Tv_EdY;

	// code --------------------
		// init
		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdTblBase);
		Tv_PtrParaTblBase		=	(Ptr_GrP5UoTblBasePara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlTblBase		=	(Ptr_GrP5SkinCtlTblBase)Tv_PtrHrcy->PtrSkinCtl;

		A_PtrRtRect->right	=	0;

		if((0 == Tv_PtrCtlTblBase->ColumCnt) || (0 == Tv_PtrCtlTblBase->ViewLineCnt) || (0 == Tv_PtrParaTblBase->ItemHeight))
		{
			return;
		}

		if(((0 > A_ItmStX) && (0 > A_ItmEdX)) || (((__s32)Tv_PtrCtlTblBase->ColumCnt <= A_ItmStX) && ((__s32)Tv_PtrCtlTblBase->ColumCnt <= A_ItmEdX)) || 
			((0 > A_ItmStY) && (0 > A_ItmEdY)) || (((__s32)Tv_PtrCtlTblBase->ViewLineCnt <= A_ItmStY) && ((__s32)Tv_PtrCtlTblBase->ViewLineCnt <= A_ItmEdY))
			)
		{
			return;
		}

		Tv_StX		=	A_ItmStX;
		Tv_EdX		=	A_ItmEdX;
		if(A_ItmStX > A_ItmEdX)
		{
			Tv_StX		=	A_ItmEdX;
			Tv_EdX		=	A_ItmStX;
		}

		Tv_StY		=	A_ItmStY;
		Tv_EdY		=	A_ItmEdY;
		if(A_ItmStY > A_ItmEdY)
		{
			Tv_StY		=	A_ItmEdY;
			Tv_EdY		=	A_ItmStY;
		}

		// patch position
		if(0 > Tv_StX)
		{
			Tv_StX	=	0;
		}
		if(0 > Tv_StY)
		{
			Tv_StY	=	0;
		}
		if((__s32)Tv_PtrCtlTblBase->ColumCnt <= Tv_EdX )
		{
			Tv_EdX	=	(__s32)Tv_PtrCtlTblBase->ColumCnt - 1;
		}
		if((__s32)Tv_PtrCtlTblBase->ViewLineCnt <= Tv_EdY)
		{
			Tv_EdY	=	(__s32)Tv_PtrCtlTblBase->ViewLineCnt - 1;
		}

		// calculate
		A_PtrRtRect->left		=	Tv_PtrCtlTblBase->Cols[Tv_StX].Pos;
		A_PtrRtRect->right	=	Tv_PtrCtlTblBase->Cols[Tv_EdX].Pos + Tv_PtrCtlTblBase->Cols[Tv_EdX].Width;
		A_PtrRtRect->top		=	m_CtlRcScr.top + E_GrP5SkinT1TblMargin + ((__s32)Tv_PtrParaTblBase->ItemHeight * Tv_StY);
		if(Tv_StY == Tv_EdY)
		{
			A_PtrRtRect->bottom		=	A_PtrRtRect->top + (__s32)Tv_PtrParaTblBase->ItemHeight;
		}
		else
		{
			A_PtrRtRect->bottom		=	m_CtlRcScr.top + E_GrP5SkinT1TblMargin + ((__s32)Tv_PtrParaTblBase->ItemHeight * (Tv_EdY + 1));
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlUpdtCalBase(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoCalBasePara	Tv_PtrParaCalBase;
		Ptr_GrP5SkinCtlCalBase Tv_PtrCtlCalBase;

		__s32	Tv_Size;
	// code --------------------
		// init
		Tv_PtrHrcy	=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdCalBase);
		Tv_PtrParaCalBase	=	(Ptr_GrP5UoCalBasePara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlCalBase		=	(Ptr_GrP5SkinCtlCalBase)Tv_PtrHrcy->PtrSkinCtl;

		Tv_PtrCtlCalBase->DayVwSizeX	=	0;
		Tv_Size	=	(m_CtlRcScr.right - m_CtlRcScr.left - (E_GrP5SkinT1CalMargin << 1)) / E_GrTimeDayOfWeek;
		if(0 < Tv_Size)
		{
			Tv_PtrCtlCalBase->DayVwSizeX	=	(__u32)Tv_Size;
		}

		Tv_PtrCtlCalBase->DayVwSizeY	=	0;
		if(0 != Tv_PtrCtlCalBase->DayVwSizeX)
		{
			Tv_Size	=	(m_CtlRcScr.bottom - m_CtlRcScr.top - (E_GrP5SkinT1CalMargin << 1)) / E_GrP5SkinCalDayLineCnt;
			if(0 < Tv_Size)
			{
				Tv_PtrCtlCalBase->DayVwSizeY	=	(__u32)Tv_Size;
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlUpdtGridBase(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoGridBasePara	Tv_PtrParaGridBase;
		Ptr_GrP5SkinCtlGridBase	Tv_PtrCtlGridBase;
	// code --------------------
		// init
		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdGridBase);
		Tv_PtrParaGridBase		=	(Ptr_GrP5UoGridBasePara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlGridBase			=	(Ptr_GrP5SkinCtlGridBase)Tv_PtrHrcy->PtrSkinCtl;

		Tv_PtrCtlGridBase->CellSizeX	=	0;
		Tv_PtrCtlGridBase->CellSizeY	=	0;

		if((0 == Tv_PtrCtlGridBase->CellCntX) || (0 == Tv_PtrCtlGridBase->CellCntY))
		{
			return;
		}

		Tv_PtrCtlGridBase->CellSizeX	=	(__u32)(m_CtlRcScr.right - m_CtlRcScr.left) / Tv_PtrCtlGridBase->CellCntX;
		Tv_PtrCtlGridBase->CellSizeY	=	(__u32)(m_CtlRcScr.bottom - m_CtlRcScr.top) / Tv_PtrCtlGridBase->CellCntY;

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlUpdtGrpHzDmap(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoGrpHzBasePara		Tv_PtrParaGrpHzBase;
		Ptr_GrP5SkinCtlGrpHzBase	Tv_PtrCtlGrpHzBase;

		Ptr_GrP5UoGrpHzDmapPara		Tv_PtrParaGrpHzDmap;
		Ptr_GrP5SkinCtlGrpHzDmap	Tv_PtrCtlGrpHzDmap;

		__s32		Tv_ValX;
		__s32		Tv_ValY;
	// code --------------------
		// init
		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdGrpHzBase);
		Tv_PtrParaGrpHzBase		=	(Ptr_GrP5UoGrpHzBasePara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlGrpHzBase		=	(Ptr_GrP5SkinCtlGrpHzBase)Tv_PtrHrcy->PtrSkinCtl;

		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdGrpHzDmap);
		Tv_PtrParaGrpHzDmap		=	(Ptr_GrP5UoGrpHzDmapPara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlGrpHzDmap		=	(Ptr_GrP5SkinCtlGrpHzDmap)Tv_PtrHrcy->PtrSkinCtl;

		// reset
		Tv_PtrCtlGrpHzDmap->CellSizeX	=	0;
		Tv_PtrCtlGrpHzDmap->CellSizeY	=	(__u32)Tv_PtrParaGrpHzBase->LineHeight;
		Tv_PtrCtlGrpHzDmap->VwCntY		=	0;

		// check able
		if((0 == Tv_PtrCtlGrpHzBase->CellCntX) || (0 == Tv_PtrCtlGrpHzBase->CellCntY))
		{
			return;
		}

		// calculate
		Tv_ValX		=	(m_CtlRcScr.right - m_CtlRcScr.left - (E_GrP5SkinT1GraphHzMargin << 1)) / (__s32)Tv_PtrCtlGrpHzBase->CellCntX;
		Tv_ValY		=	(m_CtlRcScr.right - m_CtlRcScr.left - (E_GrP5SkinT1GraphHzMargin << 1)) / (__s32)Tv_PtrCtlGrpHzDmap->CellSizeY;
		if((0 < Tv_ValX) && (0 < Tv_ValY))
		{
			Tv_PtrCtlGrpHzDmap->CellSizeX	=	(__u32)Tv_ValX;
			if((__u32)Tv_ValY >= Tv_PtrCtlGrpHzBase->CellCntY)
			{
				Tv_ValY		=	(__s32)Tv_PtrCtlGrpHzBase->CellCntY;
			}
			Tv_PtrCtlGrpHzDmap->VwCntY		=	(__u32)Tv_ValY;
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlUpdtGrpHzT1(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoGrpHzBasePara		Tv_PtrParaGrpHzBase;
		Ptr_GrP5SkinCtlGrpHzBase	Tv_PtrCtlGrpHzBase;

		Ptr_GrP5UoGrpHzT1Para		Tv_PtrParaGrpHzT1;
		Ptr_GrP5SkinCtlGrpHzT1	Tv_PtrCtlGrpHzT1;

		__s32		Tv_ValX;
		__s32		Tv_ValY;

	// code --------------------
		// init
		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdGrpHzBase);
		Tv_PtrParaGrpHzBase		=	(Ptr_GrP5UoGrpHzBasePara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlGrpHzBase		=	(Ptr_GrP5SkinCtlGrpHzBase)Tv_PtrHrcy->PtrSkinCtl;

		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdGrpHzT1);
		Tv_PtrParaGrpHzT1			=	(Ptr_GrP5UoGrpHzT1Para)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlGrpHzT1			=	(Ptr_GrP5SkinCtlGrpHzT1)Tv_PtrHrcy->PtrSkinCtl;

		// reset
		Tv_PtrCtlGrpHzT1->CellSizeX	=	(__u32)Tv_PtrParaGrpHzT1->CellWidth;
		Tv_PtrCtlGrpHzT1->CellSizeY	=	(__u32)Tv_PtrParaGrpHzBase->LineHeight;
		Tv_PtrCtlGrpHzT1->VwCntX		=	0;
		Tv_PtrCtlGrpHzT1->VwCntY		=	0;

		// check able
		if((0 == Tv_PtrCtlGrpHzBase->CellCntX) || (0 == Tv_PtrCtlGrpHzBase->CellCntY) || 
			(0 == Tv_PtrCtlGrpHzT1->CellSizeX) || (0 == Tv_PtrCtlGrpHzT1->CellSizeY))
		{
			return;
		}

		Tv_PtrCtlGrpHzT1->VwCntX		=	Tv_PtrCtlGrpHzBase->CellCntX;
		Tv_PtrCtlGrpHzT1->VwCntY		=	Tv_PtrCtlGrpHzBase->CellCntY;

		// calculate
		Tv_ValX		=	(m_CtlRcScr.right - m_CtlRcScr.left - (E_GrP5SkinT1GrpHzT1ContMgn << 1)) / (__s32)Tv_PtrCtlGrpHzT1->CellSizeX;
		Tv_ValY		=	(m_CtlRcScr.bottom - m_CtlRcScr.top - (E_GrP5SkinT1GrpHzT1ContMgn << 1) - (__s32)(__u32)Tv_PtrParaGrpHzT1->RulerHeight) / (__s32)Tv_PtrCtlGrpHzT1->CellSizeY;
		if((__u32)Tv_ValX < Tv_PtrCtlGrpHzBase->CellCntX)
		{
			Tv_PtrCtlGrpHzBase->CellCntX	=	(__u32)Tv_ValX;
		}
		if((__u32)Tv_ValY < Tv_PtrCtlGrpHzBase->CellCntY)
		{
			Tv_PtrCtlGrpHzBase->CellCntY	=	(__u32)Tv_ValY;
		}

		// calcuate contents area
		Tv_PtrCtlGrpHzT1->RcContArea.left		=	m_CtlRcScr.left;
		Tv_PtrCtlGrpHzT1->RcContArea.top		=	m_CtlRcScr.top + (__s32)(__u32)Tv_PtrParaGrpHzT1->RulerHeight;
		Tv_PtrCtlGrpHzT1->RcContArea.right	=	m_CtlRcScr.right;
		Tv_PtrCtlGrpHzT1->RcContArea.bottom	=	Tv_PtrCtlGrpHzT1->RcContArea.top + 
			(__s32)(Tv_PtrCtlGrpHzBase->CellCntY * Tv_PtrCtlGrpHzT1->CellSizeY) + E_GrP5SkinT1GrpHzT1ContMgn;

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlItemIdxCalBase(__s32 A_X, __s32 A_Y, __s32* A_PtrRtIdxX, __s32* A_PtrRtIdxY)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoCalBasePara	Tv_PtrParaCalBase;
		Ptr_GrP5SkinCtlCalBase Tv_PtrCtlCalBase;
	// code --------------------
		// init
		Tv_PtrHrcy	=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdCalBase);
		Tv_PtrParaCalBase	=	(Ptr_GrP5UoCalBasePara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlCalBase		=	(Ptr_GrP5SkinCtlCalBase)Tv_PtrHrcy->PtrSkinCtl;

		// check able
		if((0 == Tv_PtrCtlCalBase->DayVwSizeX) || (0 == Tv_PtrCtlCalBase->DayVwSizeY))
		{
			return;
		}

		// check range
		if(((m_CtlRcScr.left + E_GrP5SkinT1CalMargin) > A_X) || ((m_CtlRcScr.right - E_GrP5SkinT1CalMargin) <= A_X) ||
			((m_CtlRcScr.top + E_GrP5SkinT1CalMargin) > A_Y) || ((m_CtlRcScr.bottom - E_GrP5SkinT1CalMargin) <= A_Y)			)
		{
			return;
		}

		// calculate
		*A_PtrRtIdxX	=	(A_X - (m_CtlRcScr.left + E_GrP5SkinT1CalMargin)) / (__s32)Tv_PtrCtlCalBase->DayVwSizeX;
		*A_PtrRtIdxY	=	(A_Y - (m_CtlRcScr.top + E_GrP5SkinT1CalMargin)) / (__s32)Tv_PtrCtlCalBase->DayVwSizeY;

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlItemIdxGridBase(__s32 A_X, __s32 A_Y, __s32* A_PtrRtIdxX, __s32* A_PtrRtIdxY)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoGridBasePara	Tv_PtrParaGridBase;
		Ptr_GrP5SkinCtlGridBase	Tv_PtrCtlGridBase;

		__s32	Tv_X;
		__s32	Tv_Y;
	// code --------------------
		// init
		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdGridBase);
		Tv_PtrParaGridBase		=	(Ptr_GrP5UoGridBasePara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlGridBase			=	(Ptr_GrP5SkinCtlGridBase)Tv_PtrHrcy->PtrSkinCtl;

		if((0 == Tv_PtrCtlGridBase->CellSizeX) || (0 == Tv_PtrCtlGridBase->CellSizeY))
		{
			return;
		}

		Tv_X		=	A_X;
		Tv_Y		=	A_Y;
		if(Tv_X < m_CtlRcScr.left)
		{
			Tv_X		=	m_CtlRcScr.left;
		}
		if(Tv_X >= m_CtlRcScr.right)
		{
			Tv_X		=	m_CtlRcScr.right - 1;
		}
		if(Tv_Y < m_CtlRcScr.top)
		{
			Tv_Y		=	m_CtlRcScr.top;
		}
		if(Tv_Y >= m_CtlRcScr.bottom)
		{
			Tv_Y		=	m_CtlRcScr.bottom - 1;
		}

		// calculate
		*A_PtrRtIdxX	=	(Tv_X - m_CtlRcScr.left) / (__s32)Tv_PtrCtlGridBase->CellSizeX;
		*A_PtrRtIdxY	=	(Tv_Y - m_CtlRcScr.top) / (__s32)Tv_PtrCtlGridBase->CellSizeY;

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlItemIdxGrpHzDmap(__s32 A_X, __s32 A_Y, __s32* A_PtrRtIdxX, __s32* A_PtrRtIdxY)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoGrpHzBasePara		Tv_PtrParaGrpHzBase;
		Ptr_GrP5SkinCtlGrpHzBase	Tv_PtrCtlGrpHzBase;

		Ptr_GrP5UoGrpHzDmapPara		Tv_PtrParaGrpHzDmap;
		Ptr_GrP5SkinCtlGrpHzDmap	Tv_PtrCtlGrpHzDmap;

	// code --------------------
		// init
		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdGrpHzBase);
		Tv_PtrParaGrpHzBase		=	(Ptr_GrP5UoGrpHzBasePara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlGrpHzBase		=	(Ptr_GrP5SkinCtlGrpHzBase)Tv_PtrHrcy->PtrSkinCtl;

		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdGrpHzDmap);
		Tv_PtrParaGrpHzDmap		=	(Ptr_GrP5UoGrpHzDmapPara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlGrpHzDmap		=	(Ptr_GrP5SkinCtlGrpHzDmap)Tv_PtrHrcy->PtrSkinCtl;

		// check able
		if((0 == Tv_PtrCtlGrpHzDmap->CellSizeX) || (0 == Tv_PtrCtlGrpHzDmap->CellSizeY) || (0 == Tv_PtrCtlGrpHzDmap->VwCntY))
		{
			return;
		}

		// check range
		if(((m_CtlRcScr.left + E_GrP5SkinT1GraphHzMargin) > A_X) || ((m_CtlRcScr.right - E_GrP5SkinT1GraphHzMargin) <= A_X) ||
			((m_CtlRcScr.top + E_GrP5SkinT1GraphHzMargin) > A_Y) || ((m_CtlRcScr.bottom - E_GrP5SkinT1GraphHzMargin) <= A_Y))
		{
			return;
		}

		// calculate
		*A_PtrRtIdxX	=	(A_X - (m_CtlRcScr.left + E_GrP5SkinT1GraphHzMargin)) / (__s32)Tv_PtrCtlGrpHzDmap->CellSizeX;
		*A_PtrRtIdxY	=	(A_Y - (m_CtlRcScr.top + E_GrP5SkinT1GraphHzMargin)) / (__s32)Tv_PtrCtlGrpHzDmap->CellSizeY;

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlItemIdxGrpHzT1(__s32 A_X, __s32 A_Y, __s32* A_PtrRtIdxX, __s32* A_PtrRtIdxY)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoGrpHzBasePara		Tv_PtrParaGrpHzBase;
		Ptr_GrP5SkinCtlGrpHzBase	Tv_PtrCtlGrpHzBase;

		Ptr_GrP5UoGrpHzT1Para		Tv_PtrParaGrpHzT1;
		Ptr_GrP5SkinCtlGrpHzT1	Tv_PtrCtlGrpHzT1;

		St_GrRect	Tv_RcCont;
		__s32	Tv_IdxX;
		__s32	Tv_IdxY;
	// code --------------------
		// init
		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdGrpHzBase);
		Tv_PtrParaGrpHzBase		=	(Ptr_GrP5UoGrpHzBasePara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlGrpHzBase		=	(Ptr_GrP5SkinCtlGrpHzBase)Tv_PtrHrcy->PtrSkinCtl;

		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdGrpHzT1);
		Tv_PtrParaGrpHzT1		=	(Ptr_GrP5UoGrpHzT1Para)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlGrpHzT1		=	(Ptr_GrP5SkinCtlGrpHzT1)Tv_PtrHrcy->PtrSkinCtl;

		// check able
		if((0 == Tv_PtrCtlGrpHzT1->CellSizeX) || (0 == Tv_PtrCtlGrpHzT1->CellSizeY) || 
			(0 == Tv_PtrCtlGrpHzT1->VwCntX) || (0 == Tv_PtrCtlGrpHzT1->VwCntY))
		{
			return;
		}

		Tv_RcCont.left		=	m_CtlRcScr.left + E_GrP5SkinT1GrpHzT1ContMgn;
		Tv_RcCont.top			=	m_CtlRcScr.top + (__s32)(__u32)Tv_PtrParaGrpHzT1->RulerHeight + E_GrP5SkinT1GrpHzT1ContMgn;

		// check range
		if((Tv_RcCont.left > A_X) ||(Tv_RcCont.top > A_Y))
		{
			return;
		}

		// calculate
		Tv_IdxX		=	(A_X - Tv_RcCont.left) / (__s32)Tv_PtrCtlGrpHzT1->CellSizeX;
		Tv_IdxY		=	(A_Y - Tv_RcCont.top) / (__s32)Tv_PtrCtlGrpHzT1->CellSizeY;

		if((Tv_IdxX < (__s32)Tv_PtrCtlGrpHzT1->VwCntX) && (Tv_IdxY < (__s32)Tv_PtrCtlGrpHzT1->VwCntY))
		{
			*A_PtrRtIdxX	=	Tv_IdxX;
			*A_PtrRtIdxY	=	Tv_IdxY;
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlItemIdxSpinBase(__s32 A_X, __s32 A_Y, __s32* A_PtrRtIdxX, __s32* A_PtrRtIdxY)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoSpinBasePara	Tv_PtrParaSpinBase;
		Ptr_GrP5SkinCtlSpinBase	Tv_PtrCtlSpinBase;

	// code --------------------
		// init
		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdSpinBase);
		Tv_PtrParaSpinBase	=	(Ptr_GrP5UoSpinBasePara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlSpinBase		=	(Ptr_GrP5SkinCtlSpinBase)Tv_PtrHrcy->PtrSkinCtl;

		*A_PtrRtIdxY				=	-1;
		*A_PtrRtIdxX				=	E_GrP5SkinSpinIdxCont;

		// check dec button
		if((0 <= Tv_PtrCtlSpinBase->RcItems[E_GrP5SkinSpinIdxBtnDec].right) && 
			(A_X >= Tv_PtrCtlSpinBase->RcItems[E_GrP5SkinSpinIdxBtnDec].left) && 
			(A_Y >= Tv_PtrCtlSpinBase->RcItems[E_GrP5SkinSpinIdxBtnDec].top) &&
			(A_X < Tv_PtrCtlSpinBase->RcItems[E_GrP5SkinSpinIdxBtnDec].right) &&
			(A_Y < Tv_PtrCtlSpinBase->RcItems[E_GrP5SkinSpinIdxBtnDec].bottom) )
		{
			*A_PtrRtIdxX				=	E_GrP5SkinSpinIdxBtnDec;
		}
		else if((0 <= Tv_PtrCtlSpinBase->RcItems[E_GrP5SkinSpinIdxBtnInc].right) &&
			(A_X >= Tv_PtrCtlSpinBase->RcItems[E_GrP5SkinSpinIdxBtnInc].left) &&
			(A_Y >= Tv_PtrCtlSpinBase->RcItems[E_GrP5SkinSpinIdxBtnInc].top) &&
			(A_X < Tv_PtrCtlSpinBase->RcItems[E_GrP5SkinSpinIdxBtnInc].right) &&
			(A_Y < Tv_PtrCtlSpinBase->RcItems[E_GrP5SkinSpinIdxBtnInc].bottom))
		{
			*A_PtrRtIdxX				=	E_GrP5SkinSpinIdxBtnInc;
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlItemIdxTblBase(__s32 A_X, __s32 A_Y, __s32* A_PtrRtIdxX, __s32* A_PtrRtIdxY)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoTblBasePara		Tv_PtrParaTblBase;
		Ptr_GrP5SkinCtlTblBase	Tv_PtrCtlTblBase;

		__u32	Tv_ColIdx;
	// code --------------------
		// init
		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdTblBase);
		Tv_PtrParaTblBase		=	(Ptr_GrP5UoTblBasePara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlTblBase		=	(Ptr_GrP5SkinCtlTblBase)Tv_PtrHrcy->PtrSkinCtl;

		// check able
		if((0 == Tv_PtrCtlTblBase->ColumCnt) || (0 == Tv_PtrCtlTblBase->ViewLineCnt) && (0 != Tv_PtrParaTblBase->ItemHeight))
		{
			return;
		}

		// check range
		if((A_X < (m_CtlRcScr.left + E_GrP5SkinT1TblMargin)) || (A_X >= (m_CtlRcScr.right - E_GrP5SkinT1TblMargin)) || 
			(A_Y < (m_CtlRcScr.top + E_GrP5SkinT1TblMargin)) || (A_Y >= (m_CtlRcScr.bottom - E_GrP5SkinT1TblMargin)))
		{
			return;
		}

		// calculate x
		for(Tv_ColIdx = 0; Tv_ColIdx < Tv_PtrCtlTblBase->ColumCnt; Tv_ColIdx++)
		{
			if(A_X >(Tv_PtrCtlTblBase->Cols[Tv_ColIdx].Pos + Tv_PtrCtlTblBase->Cols[Tv_ColIdx].Width))
			{
				// found
				*A_PtrRtIdxX		=	Tv_ColIdx;
				break;
			}
		}

		// calculate y
		*A_PtrRtIdxY	=	(A_Y - (m_CtlRcScr.top + E_GrP5SkinT1TblMargin)) / (__s32)(__u32)Tv_PtrParaTblBase->ItemHeight;
		// check over
		if((*A_PtrRtIdxY) >= (__s32)Tv_PtrCtlTblBase->ViewLineCnt)
		{
			*A_PtrRtIdxY	=	-1;
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawBtnStd(__u32 A_Stat)
{
	// local -------------------
	// code --------------------

		// draw background
		if(1 == m_PtrCtlSkinPara->SubType)
		{
			LcDrawBtnSubType1(m_PtrCtlSkinPara, &m_CtlRcScr, A_Stat);
		}
		else if(2 == m_PtrCtlSkinPara->SubType)
		{
			// virtual keyboard type
			LcDrawBtnSubType2(m_PtrCtlSkinPara, &m_CtlRcScr, A_Stat);
		}
		else if(3 == m_PtrCtlSkinPara->SubType)
		{
			LcDrawBtnSubType3(m_PtrCtlSkinPara, &m_CtlRcScr, A_Stat);
		}
		else if(4 == m_PtrCtlSkinPara->SubType)
		{
			LcDrawBtnSubType4(m_PtrCtlSkinPara, &m_CtlRcScr, A_Stat);
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcBtnBaseStatUpdate(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoBtnBasePara		Tv_PtrParaBtnBase;
		Ptr_GrP5SkinCtlBtnBase	Tv_PtrCtlBtnBase;
	// code --------------------
		// init
		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdBtnBase);
		Tv_PtrParaBtnBase		=	(Ptr_GrP5UoBtnBasePara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlBtnBase		=	(Ptr_GrP5SkinCtlBtnBase)Tv_PtrHrcy->PtrSkinCtl;

		// get status
		m_CtlStat		=	0;

		if(((Cls_GrP5UoBtnBase*)m_CtlUo)->IsCanEnable())
		{
			if(Tv_PtrCtlBtnBase->IsPush)
			{
				m_CtlStat		=	E_GrP5StatPush;
			}
			else if(((Cls_GrP5UoBtnBase*)m_CtlUo)->IsOnFocuse())
			{
				m_CtlStat	=	m_CtlStat | E_GrP5StatOnFocus;
			}
			if(Tv_PtrCtlBtnBase->IsSelect)
			{
				m_CtlStat	=	m_CtlStat | E_GrP5StatOnSelected;
			}
		}
		else
		{
			m_CtlStat	=	E_GrP5StatDisable;
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcChkBaseStatUpdate(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoBtnBasePara		Tv_PtrParaBtnBase;
		Ptr_GrP5SkinCtlBtnBase	Tv_PtrCtlBtnBase;
	// code --------------------
		// init
		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdBtnBase);
		Tv_PtrParaBtnBase		=	(Ptr_GrP5UoBtnBasePara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlBtnBase		=	(Ptr_GrP5SkinCtlBtnBase)Tv_PtrHrcy->PtrSkinCtl;

		// get status
		m_CtlStat		=	0;

		if(((Cls_GrP5UoBtnBase*)m_CtlUo)->IsCanEnable())
		{
			if(Tv_PtrCtlBtnBase->IsPush)
			{
				m_CtlStat		=	E_GrP5StatPush;
			}
			else if(((Cls_GrP5UoBtnBase*)m_CtlUo)->IsOnFocuse())
			{
				m_CtlStat	=	m_CtlStat | E_GrP5StatOnFocus;
			}
			if(Tv_PtrCtlBtnBase->IsSelect)
			{
				m_CtlStat	=	m_CtlStat | E_GrP5StatOnSelected;
			}
		}
		else
		{
			m_CtlStat	=	E_GrP5StatDisable;
		}

		if(Tv_PtrCtlBtnBase->IsChked)
		{
			m_CtlStat	=	m_CtlStat | E_GrP5StatOnChecked;
		}
#ifdef GR_P5_EDITOR
		if(((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[((Cls_GrP5UoBase*)m_CtlUo)->ConsoleIdxGet()].ObjCon)->DvlIsEdit())
		{
			m_CtlStat	=	m_CtlStat | E_GrP5StatOnChecked;
		}
#endif

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlDrawBtnImgBase(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoBtnImgBasePara	Tv_PtrParaBtnImgBase;

		__u16	Tv_ImgId;
		__u32	Tv_TmpStat;
		Ptr_GrGdib	Tv_PtrGdib;
		St_GrRect		Tv_Rc;
	// code --------------------
		// init
		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdBtnImgBase);
		Tv_PtrParaBtnImgBase		=	(Ptr_GrP5UoBtnImgBasePara)Tv_PtrHrcy->PtrPara;

		LcBtnBaseStatUpdate();

		// draw background
		if(Tv_PtrParaBtnImgBase->IsSkinDraw)
		{
			LcDrawBtnStd(m_CtlStat);
		}

		// image
		Tv_PtrGdib	=	NULL;
		Tv_ImgId		=	E_GrP5ImgIdNone;
		Tv_TmpStat	=	E_GrP5StatConstMask & m_CtlStat;
		if(E_GrP5StatPush == Tv_TmpStat)
		{
			Tv_ImgId		=	Tv_PtrParaBtnImgBase->ImgPush;
		}
		else if(E_GrP5StatDisable == Tv_TmpStat)
		{
			Tv_ImgId		=	Tv_PtrParaBtnImgBase->ImgDisable;
		}
		else
		{
			// normal
			Tv_ImgId		=	Tv_PtrParaBtnImgBase->ImgNormal;
			if(0 != (m_CtlStat & E_GrP5StatOnChecked))
			{
				Tv_ImgId		=	Tv_PtrParaBtnImgBase->ImgOnSelected;
			}
			else
			{
				if(0 != (m_CtlStat & E_GrP5StatOnFocus))
				{
					Tv_ImgId		=	Tv_PtrParaBtnImgBase->ImgOnFocus;
				}
			}
		}
		if(E_GrP5ImgIdNone != Tv_ImgId)
		{
			// draw image
			Tv_PtrGdib	=	GrP5ImageGet(Tv_ImgId);
			if(NULL != Tv_PtrGdib)
			{
				// draw
				// get rect
				if(GrP5RelRectToAbsRect(&Tv_PtrParaBtnImgBase->ImgRc, &m_CtlRcScr, &Tv_Rc))
				{
					// draw image
					ReqPntGdib(Tv_PtrGdib, &Tv_Rc, Tv_PtrParaBtnImgBase->ImgAlignHz, Tv_PtrParaBtnImgBase->ImgAlignVt, 
						Tv_PtrParaBtnImgBase->IsImgSprite);
				}
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlDrawLblBase(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoLblBasePara	Tv_PtrParaLblBase;
		Ptr_GrP5SkinCtlLblBase Tv_PtrCtlLblBase;
		St_GrRect		Tv_Rc;
	// code --------------------
		// init
		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdLblBase);
		Tv_PtrParaLblBase		=	(Ptr_GrP5UoLblBasePara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlLblBase		=	(Ptr_GrP5SkinCtlLblBase)Tv_PtrHrcy->PtrSkinCtl;

		// background
		if(0 == m_PtrCtlSkinPara->SubType)
		{
			// draw mark
			if(0 != m_PtrCtlSkinPara->ParaByte0)
			{
				Tv_Rc.left		=	m_CtlRcScr.left + (__s32)(__u32)m_PtrCtlSkinPara->ParaByte1;
				Tv_Rc.right		=	Tv_Rc.left + (__s32)(__u32)m_PtrCtlSkinPara->ParaByte0;
				Tv_Rc.bottom	=	m_CtlRcScr.bottom - (__s32)(__u32)m_PtrCtlSkinPara->ParaByte2;
				Tv_Rc.top			=	Tv_Rc.bottom - (__s32)(__u32)m_PtrCtlSkinPara->ParaByte0;

				GrP5PntArrangeByRect(&Tv_Rc);

				GrP5PntReqFillRect(&Tv_Rc, m_PtrCtlSkinPara->ParaColor);
			}
		}
		else if(1 == m_PtrCtlSkinPara->SubType)
		{
			GrP5PntArrangeByRect(&m_CtlRcScr);
			GrP5PntReqFillRect(&m_CtlRcScr, m_PtrCtlSkinPara->ParaColor);
		}
		else if(2 == m_PtrCtlSkinPara->SubType)
		{
			GrP5PntArrangeByRect(&m_CtlRcScr);
			LcGradeintByTbl(&m_CtlRcScr, &m_GradLblBg2);
		}

		// draw text
		if(NULL != Tv_PtrCtlLblBase->Str)
		{
			if(GrP5RelRectToAbsRect(&Tv_PtrParaLblBase->TxtRc, &m_CtlRcScr, &Tv_Rc))
			{
				GrP5PntReqTxtStyle(&Tv_PtrParaLblBase->TxtPara, Tv_PtrParaLblBase->FontId);
				GrP5PntReqTxtCus(Tv_PtrCtlLblBase->CusPos, Tv_PtrCtlLblBase->ClrCus);

				m_CtlStat	=	Tv_PtrCtlLblBase->RtlStat;
				if(!((Cls_GrP5UoBase*)m_CtlUo)->IsCanEnable())
				{
					m_CtlStat	=	E_GrP5FontStatDisable;
				}
				GrP5PntReqTxtDraw(&Tv_Rc, Tv_PtrCtlLblBase->Str, (__u16)m_CtlStat);		// txt buf ok
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlUpdtLblTid(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoLblBasePara		Tv_PtrParaLblBase;
		Ptr_GrP5SkinCtlLblBase	Tv_PtrCtlLblBase;
		Ptr_GrP5UoLblTidPara		Tv_PtrParaLblTid;

	// code --------------------
		// init
		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdLblBase);
		Tv_PtrParaLblBase		=	(Ptr_GrP5UoLblBasePara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlLblBase		=	(Ptr_GrP5SkinCtlLblBase)Tv_PtrHrcy->PtrSkinCtl;

		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdLblTid);
		Tv_PtrParaLblTid		=	(Ptr_GrP5UoLblTidPara)Tv_PtrHrcy->PtrPara;

		Tv_PtrCtlLblBase->Str	=	GrP5TextGet(Tv_PtrParaLblTid->TxtId);

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlUpdtPnlGrpBar(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoPnlGrpBarPara		Tv_PtrParaPnlGrpBar;
		Ptr_GrP5SkinCtlPnlGrpBar	Tv_PtrCtlPnlGrpBar;
	// code --------------------
		// init
		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdPnlGrpBar);
		Tv_PtrParaPnlGrpBar	=	(Ptr_GrP5UoPnlGrpBarPara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlPnlGrpBar	=	(Ptr_GrP5SkinCtlPnlGrpBar)Tv_PtrHrcy->PtrSkinCtl;

		Tv_PtrCtlPnlGrpBar->PntItmWidth	=	100;
		if(0 != Tv_PtrCtlPnlGrpBar->ItemCnt)
		{
			Tv_PtrCtlPnlGrpBar->PntItmWidth	=	((m_CtlRcScr.right - m_CtlRcScr.left) - (__s32)Tv_PtrParaPnlGrpBar->GridValWidth) / Tv_PtrCtlPnlGrpBar->ItemCnt;
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlUpdtPnlGrpLine(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoPnlGrpLinePara		Tv_PtrParaPnlGrpLine;
		Ptr_GrP5SkinCtlPnlGrpLine	Tv_PtrCtlPnlGrpLine;

		__s32	Tv_Base;
		__u32	Tv_ColIdx;
	// code --------------------
		// init
		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdPnlGrpLine);
		Tv_PtrParaPnlGrpLine	=	(Ptr_GrP5UoPnlGrpLinePara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlPnlGrpLine		=	(Ptr_GrP5SkinCtlPnlGrpLine)Tv_PtrHrcy->PtrSkinCtl;

		// paint item width
		Tv_PtrCtlPnlGrpLine->PntColWidth	=	100;
		if(0 != Tv_PtrCtlPnlGrpLine->ColCnt)
		{
			Tv_PtrCtlPnlGrpLine->PntColWidth	=	
				((m_CtlRcScr.right - m_CtlRcScr.left) - (__s32)Tv_PtrParaPnlGrpLine->GridValWidth) / Tv_PtrCtlPnlGrpLine->ColCnt;
		}

		// colum position
		if(0 != Tv_PtrCtlPnlGrpLine->ColCnt)
		{
			Tv_Base	=	m_CtlRcScr.left + (__s32)Tv_PtrParaPnlGrpLine->GridValWidth + (Tv_PtrCtlPnlGrpLine->PntColWidth >> 1);
			for(Tv_ColIdx = 0; Tv_ColIdx < Tv_PtrCtlPnlGrpLine->ColCnt; Tv_ColIdx++)
			{
				Tv_PtrCtlPnlGrpLine->Cols[Tv_ColIdx].Pos	=	Tv_Base;
				// next
				Tv_Base			=	Tv_Base + Tv_PtrCtlPnlGrpLine->PntColWidth;
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlUpdtQuadT1(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoQuadBasePara	Tv_PtrParaQuadBase;

		Ptr_GrP5UoQuadT1Para		Tv_PtrParaQuadT1;
		Ptr_GrP5SkinCtlQuadT1		Tv_PtrCtlQuadT1;

		__u32	Tv_LineWidth;
		__u32	Tv_VwIdx;
		__s8	Tv_Ch;
		__u8	Tv_ConIdx;
		Ptr_GrRect	Tv_PtrRc;
		__u32	Tv_LnCnt;
		__u32	Tv_LnIdx;
		//St_GrRect	Tv_RcLn;
	// code --------------------
		// init
		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdQuadBase);
		Tv_PtrParaQuadBase	=	(Ptr_GrP5UoQuadBasePara)Tv_PtrHrcy->PtrPara;

		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdQuadT1);
		Tv_PtrParaQuadT1		=	(Ptr_GrP5UoQuadT1Para)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlQuadT1			=	(Ptr_GrP5SkinCtlQuadT1)Tv_PtrHrcy->PtrSkinCtl;

		// reset draw request 
		Tv_PtrCtlQuadT1->Dreq.Cnt		=	0;

		// calculate rect
		Tv_ConIdx	=	((Cls_GrP5UoBase*)m_CtlUo)->ConsoleIdxGet();
		Tv_PtrRc	=	((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[Tv_ConIdx].ObjCon)->QuadViewRcPtrGet(&Tv_PtrCtlQuadT1->ViewCnt);
		if((NULL == Tv_PtrRc) || (0 == Tv_PtrCtlQuadT1->ViewCnt))
		{
			return;
		}

		Tv_LineWidth	=	((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[Tv_ConIdx].ObjCon)->QuadLineWidthGet();
		// channel rect
		for(Tv_VwIdx = 0; Tv_VwIdx < Tv_PtrCtlQuadT1->ViewCnt; Tv_VwIdx++)
		{
			// get view rect
			Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcView	=	Tv_PtrRc[Tv_VwIdx];
			// calculate in area
			Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcInArea.left	=	Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcView.left + (__s32)(Tv_LineWidth >> 1);
			if(0 == Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcView.left)
			{
				Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcInArea.left	=	Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcView.left + (__s32)Tv_LineWidth;
			}
			Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcInArea.top	=	Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcView.top + (__s32)(Tv_LineWidth >> 1);
			if(0 == Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcView.top)
			{
				Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcInArea.top	=	Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcView.top + (__s32)Tv_LineWidth;
			}
			Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcInArea.right	=	Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcView.right - (__s32)(Tv_LineWidth >> 1);
			if(m_CtlRcScr.right <= Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcView.right)
			{
				Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcInArea.right	=	Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcView.right - (__s32)Tv_LineWidth;
			}
			Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcInArea.bottom	=	Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcView.bottom - (__s32)(Tv_LineWidth >> 1);
			if(m_CtlRcScr.bottom <= Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcView.bottom)
			{
				Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcInArea.bottom	=	Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcView.bottom - (__s32)Tv_LineWidth;
			}

			Tv_Ch	=	((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[Tv_ConIdx].ObjCon)->QuadChGetByViewIdx((__u8)Tv_VwIdx);
			if(0 <= Tv_Ch)
			{
				// update channel to view table
				Tv_PtrCtlQuadT1->ChToVwTbl[Tv_Ch]	=	(__u8)Tv_VwIdx;
				// calculate name rect
				if(GrP5RelRectToAbsRect(&Tv_PtrParaQuadT1->NameRc, &Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcView, &Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcName))
				{
					Tv_PtrCtlQuadT1->Dreq.RcTbl[Tv_PtrCtlQuadT1->Dreq.Cnt]	=	Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcName;
					Tv_PtrCtlQuadT1->Dreq.Cnt ++;
				}
				// calculate record icon rect
				if(GrP5RelRectToAbsRect(&Tv_PtrParaQuadT1->RecDoRc, &Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcView, &Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcIconRec))
				{
					Tv_PtrCtlQuadT1->Dreq.RcTbl[Tv_PtrCtlQuadT1->Dreq.Cnt]	=	Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcIconRec;
					Tv_PtrCtlQuadT1->Dreq.Cnt ++;
				}
				// calculate motion icon rect
				if(GrP5RelRectToAbsRect(&Tv_PtrParaQuadT1->MtnRc, &Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcView, &Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcIconMtn))
				{
					Tv_PtrCtlQuadT1->Dreq.RcTbl[Tv_PtrCtlQuadT1->Dreq.Cnt]	=	Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcIconMtn;
					Tv_PtrCtlQuadT1->Dreq.Cnt ++;
				}
				// calculate sensor icon rect
				if(GrP5RelRectToAbsRect(&Tv_PtrParaQuadT1->SenRc, &Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcView, &Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcIconSen))
				{
					Tv_PtrCtlQuadT1->Dreq.RcTbl[Tv_PtrCtlQuadT1->Dreq.Cnt]	=	Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcIconSen;
					Tv_PtrCtlQuadT1->Dreq.Cnt ++;
				}
				// calculate image analysis
				if(GrP5RelRectToAbsRect(&Tv_PtrParaQuadT1->IaRc, &Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcView, &Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcIconIa))
				{
					Tv_PtrCtlQuadT1->Dreq.RcTbl[Tv_PtrCtlQuadT1->Dreq.Cnt]	=	Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcIconIa;
					Tv_PtrCtlQuadT1->Dreq.Cnt ++;
				}
				// calculate ptz rect
				if(GrP5RelRectToAbsRect(&Tv_PtrParaQuadT1->PtzRc, &Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcView, &Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcIconPtz))
				{
					Tv_PtrCtlQuadT1->Dreq.RcTbl[Tv_PtrCtlQuadT1->Dreq.Cnt]	=	Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcIconPtz;
					Tv_PtrCtlQuadT1->Dreq.Cnt ++;
				}
				// calculate audio
				if(GrP5RelRectToAbsRect(&Tv_PtrParaQuadT1->AdoRc, &Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcView, &Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcIconAdo))
				{
					Tv_PtrCtlQuadT1->Dreq.RcTbl[Tv_PtrCtlQuadT1->Dreq.Cnt]	=	Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcIconAdo;
					Tv_PtrCtlQuadT1->Dreq.Cnt ++;
				}

			}
			// upate channel
			Tv_PtrCtlQuadT1->Views[Tv_VwIdx].Ch		=	Tv_Ch;

		}

		// inner line
		if((0 != Tv_LineWidth) && Tv_PtrParaQuadBase->IsDrawLine)
		{
			Tv_PtrRc	=	((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[Tv_ConIdx].ObjCon)->QuadLineRcPtrGet(&Tv_LnCnt);
			if((NULL != Tv_PtrRc) && (0 != Tv_LnCnt))
			{
				for(Tv_LnIdx = 0; Tv_LnIdx < Tv_LnCnt; Tv_LnIdx++)
				{
					Tv_PtrCtlQuadT1->Dreq.RcTbl[Tv_PtrCtlQuadT1->Dreq.Cnt]	=	Tv_PtrRc[Tv_LnIdx];
					Tv_PtrCtlQuadT1->Dreq.Cnt ++;
				}
			}
		}

		// outter line
#if 0
		if(0 != Tv_LineWidth)
		{
			// top
			Tv_RcLn.left		=	m_CtlRcScr.left;
			Tv_RcLn.top			=	m_CtlRcScr.top;
			Tv_RcLn.right		=	m_CtlRcScr.right;
			Tv_RcLn.bottom	=	m_CtlRcScr.top + (__s32)Tv_LineWidth;
			Tv_PtrCtlQuadT1->Dreq.RcTbl[Tv_PtrCtlQuadT1->Dreq.Cnt]	=	Tv_RcLn;
			Tv_PtrCtlQuadT1->Dreq.Cnt ++;

			// left
			Tv_RcLn.left		=	m_CtlRcScr.left;
			Tv_RcLn.top			=	m_CtlRcScr.top +  (__s32)Tv_LineWidth;
			Tv_RcLn.right		=	m_CtlRcScr.left + (__s32)Tv_LineWidth;
			Tv_RcLn.bottom	=	m_CtlRcScr.bottom - (__s32)Tv_LineWidth;
			Tv_PtrCtlQuadT1->Dreq.RcTbl[Tv_PtrCtlQuadT1->Dreq.Cnt]	=	Tv_RcLn;
			Tv_PtrCtlQuadT1->Dreq.Cnt ++;

			// right
			Tv_RcLn.left		=	m_CtlRcScr.right - (__s32)Tv_LineWidth;
			Tv_RcLn.top			=	m_CtlRcScr.top + (__s32)Tv_LineWidth;
			Tv_RcLn.right		=	m_CtlRcScr.right;
			Tv_RcLn.bottom	=	m_CtlRcScr.bottom - (__s32)Tv_LineWidth;
			Tv_PtrCtlQuadT1->Dreq.RcTbl[Tv_PtrCtlQuadT1->Dreq.Cnt]	=	Tv_RcLn;
			Tv_PtrCtlQuadT1->Dreq.Cnt ++;

			// bottom
			Tv_RcLn.left		=	m_CtlRcScr.left;
			Tv_RcLn.top			=	m_CtlRcScr.bottom - (__s32)Tv_LineWidth;
			Tv_RcLn.right		=	m_CtlRcScr.right;
			Tv_RcLn.bottom	=	m_CtlRcScr.bottom;
			Tv_PtrCtlQuadT1->Dreq.RcTbl[Tv_PtrCtlQuadT1->Dreq.Cnt]	=	Tv_RcLn;
			Tv_PtrCtlQuadT1->Dreq.Cnt ++;
		}
#endif

		// check draw request count over
		if(E_GrP5SkinQuadDrawAreaMaxCnt < Tv_PtrCtlQuadT1->Dreq.Cnt)
		{
			DbgMsgPrint("[BUG!] quad draw request area count (%d) over!\n", Tv_PtrCtlQuadT1->Dreq.Cnt);
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlUpdtSpinBase(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoSpinBasePara	Tv_PtrParaSpinBase;
		Ptr_GrP5SkinCtlSpinBase	Tv_PtrCtlSpinBase;
	// code --------------------
		// init
		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdSpinBase);
		Tv_PtrParaSpinBase	=	(Ptr_GrP5UoSpinBasePara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlSpinBase		=	(Ptr_GrP5SkinCtlSpinBase)Tv_PtrHrcy->PtrSkinCtl;

		// check direction
		if(Tv_PtrParaSpinBase->IsVertical)
		{
			// vertical
			Tv_PtrCtlSpinBase->RcItems[E_GrP5SkinSpinIdxCont].right			=	0;
			Tv_PtrCtlSpinBase->RcItems[E_GrP5SkinSpinIdxBtnDec].right		=	0;
			Tv_PtrCtlSpinBase->RcItems[E_GrP5SkinSpinIdxBtnInc].right		=	0;
			Tv_PtrCtlSpinBase->RcText.right		=	0;
		}
		else
		{
			// horizontal
			// body rect
			Tv_PtrCtlSpinBase->RcItems[E_GrP5SkinSpinIdxCont]	=	m_CtlRcScr;

			// decrease button
			Tv_PtrCtlSpinBase->RcItems[E_GrP5SkinSpinIdxBtnDec].left		=	m_CtlRcScr.left + 2;
			Tv_PtrCtlSpinBase->RcItems[E_GrP5SkinSpinIdxBtnDec].top			=	m_CtlRcScr.top + 2;
			Tv_PtrCtlSpinBase->RcItems[E_GrP5SkinSpinIdxBtnDec].right		=	m_CtlRcScr.left + 19;
			Tv_PtrCtlSpinBase->RcItems[E_GrP5SkinSpinIdxBtnDec].bottom	=	m_CtlRcScr.bottom - 2;

			// increase button
			Tv_PtrCtlSpinBase->RcItems[E_GrP5SkinSpinIdxBtnInc].left		=	m_CtlRcScr.right - 19;
			Tv_PtrCtlSpinBase->RcItems[E_GrP5SkinSpinIdxBtnInc].right		=	m_CtlRcScr.right - 2;
			Tv_PtrCtlSpinBase->RcItems[E_GrP5SkinSpinIdxBtnInc].top			=	m_CtlRcScr.top + 2;
			Tv_PtrCtlSpinBase->RcItems[E_GrP5SkinSpinIdxBtnInc].bottom	=	m_CtlRcScr.bottom - 2;

			// text rect
			GrP5RelRectToAbsRect(&Tv_PtrParaSpinBase->TxtRc, &m_CtlRcScr, &Tv_PtrCtlSpinBase->RcText);
		}


}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlUpdtTabBase(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoTabBasePara		Tv_PtrParaTabBase;
		Ptr_GrP5SkinCtlTabBase	Tv_PtrCtlTabBase;

	// code --------------------
		// init
		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdTabBase);
		Tv_PtrParaTabBase		=	(Ptr_GrP5UoTabBasePara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlTabBase		=	(Ptr_GrP5SkinCtlTabBase)Tv_PtrHrcy->PtrSkinCtl;

		GrP5RelRectToAbsRect(&Tv_PtrParaTabBase->ContRc, &m_CtlRcScr, &Tv_PtrCtlTabBase->RcCont);

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlUpdtTabTiRel(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoTabTiRelPara	Tv_PtrParaTabTiRel;
		Ptr_GrP5SkinCtlTabTiRel	Tv_PtrCtlTabTiRel;

	// code --------------------
		// init
		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdTabTiRel);
		Tv_PtrParaTabTiRel	=	(Ptr_GrP5UoTabTiRelPara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlTabTiRel		=	(Ptr_GrP5SkinCtlTabTiRel)Tv_PtrHrcy->PtrSkinCtl;

		// base update
		LcCtlUpdtTabBase();

		// icon rect
		GrP5RelRectToAbsRect(&Tv_PtrParaTabTiRel->IconRc, &m_CtlRcScr, &Tv_PtrCtlTabTiRel->RcIcon);
		// label background
		GrP5RelRectToAbsRect(&Tv_PtrParaTabTiRel->LblBgRc, &m_CtlRcScr, &Tv_PtrCtlTabTiRel->RcLblBg);
		// label text
		GrP5RelRectToAbsRect(&Tv_PtrParaTabTiRel->LblTxtRc, &m_CtlRcScr, &Tv_PtrCtlTabTiRel->RcLblTxt);

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlUpdtTblBase(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoTblBasePara		Tv_PtrParaTblBase;
		Ptr_GrP5SkinCtlTblBase	Tv_PtrCtlTblBase;

		__u32	Tv_ColIdx;
		__s32	Tv_BaseX;
	// code --------------------
		// init
		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdTblBase);
		Tv_PtrParaTblBase		=	(Ptr_GrP5UoTblBasePara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlTblBase		=	(Ptr_GrP5SkinCtlTblBase)Tv_PtrHrcy->PtrSkinCtl;

		// calculate max line count
		Tv_PtrCtlTblBase->ViewLineMax	=	0;
		if(0 < Tv_PtrParaTblBase->ItemHeight)
		{
			Tv_PtrCtlTblBase->ViewLineMax	=	(__u32)(m_CtlRcScr.bottom - m_CtlRcScr.top - (E_GrP5SkinT1TblMargin << 1)) /
				Tv_PtrParaTblBase->ItemHeight;
		}

		// update view line count
		Tv_PtrCtlTblBase->ViewLineCnt	=	Tv_PtrCtlTblBase->ItemCnt;
		if(Tv_PtrCtlTblBase->ViewLineCnt >= Tv_PtrCtlTblBase->ViewLineMax)
		{
			Tv_PtrCtlTblBase->ViewLineCnt		=	Tv_PtrCtlTblBase->ViewLineMax;
		}
		
		// colum count
		Tv_PtrCtlTblBase->ColumCnt	=	(__u32)Tv_PtrParaTblBase->ColumCnt;
		if(E_GrP5SkinTblColumMaxCnt <= Tv_PtrCtlTblBase->ColumCnt)
		{
			Tv_PtrCtlTblBase->ColumCnt	=	E_GrP5SkinTblColumMaxCnt;
		}

		// colum position calculate
		if(0 != Tv_PtrCtlTblBase->ColumCnt)
		{
			Tv_BaseX	=	m_CtlRcScr.left + E_GrP5SkinT1TblMargin;
			for(Tv_ColIdx = 0; Tv_ColIdx < Tv_PtrCtlTblBase->ColumCnt; Tv_ColIdx++)
			{
				Tv_PtrCtlTblBase->Cols[Tv_ColIdx].Pos		=	Tv_BaseX;
				Tv_PtrCtlTblBase->Cols[Tv_ColIdx].Width	=	(__s32)Tv_PtrParaTblBase->ColSize[Tv_ColIdx];
				if((m_CtlRcScr.right - E_GrP5SkinT1TblMargin) < (Tv_BaseX + Tv_PtrCtlTblBase->Cols[Tv_ColIdx].Width))
				{
					// patch size
					Tv_PtrCtlTblBase->Cols[Tv_ColIdx].Width	=	(m_CtlRcScr.right - E_GrP5SkinT1TblMargin) - Tv_BaseX;
					if(0 == Tv_PtrCtlTblBase->Cols[Tv_ColIdx].Width)
					{
						Tv_PtrCtlTblBase->ColumCnt	=	Tv_ColIdx;
						break;
					}
					Tv_PtrCtlTblBase->ColumCnt	=	Tv_ColIdx + 1;
					break;
				}
				// next
				Tv_BaseX	=	Tv_PtrCtlTblBase->Cols[Tv_ColIdx].Pos + Tv_PtrCtlTblBase->Cols[Tv_ColIdx].Width;
			}
		}


}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlUpdtPnlDir(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoPnlDirPara		Tv_PtrParaPnlDir;
		Ptr_GrP5SkinCtlPnlDir		Tv_PtrCtlPnlDir;

		__s32	Tv_SizeX;
		__s32	Tv_SizeY;
		__s32	Tv_IdcSize;
	// code --------------------
		// init
		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdPnlDir);
		Tv_PtrParaPnlDir		=	(Ptr_GrP5UoPnlDirPara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlPnlDir			=	(Ptr_GrP5SkinCtlPnlDir)Tv_PtrHrcy->PtrSkinCtl;

		Tv_SizeX		=	m_CtlRcScr.right - m_CtlRcScr.left;
		Tv_SizeY		=	m_CtlRcScr.bottom - m_CtlRcScr.top;
		Tv_IdcSize	=	E_GrP5SkinT1PnlDirIdcDfltSize + (__s32)m_PtrCtlSkinPara->ParaByte0;

		// left
		Tv_PtrCtlPnlDir->RcLeft.left		=	m_CtlRcScr.left + (__s32)m_PtrCtlSkinPara->ParaByte1;
		Tv_PtrCtlPnlDir->RcLeft.top			=	m_CtlRcScr.top + ((Tv_SizeY - Tv_IdcSize) >> 1);
		Tv_PtrCtlPnlDir->RcLeft.right		=	Tv_PtrCtlPnlDir->RcLeft.left + Tv_IdcSize;
		Tv_PtrCtlPnlDir->RcLeft.bottom	=	Tv_PtrCtlPnlDir->RcLeft.top + Tv_IdcSize;

		// right
		Tv_PtrCtlPnlDir->RcRight.right	=	m_CtlRcScr.right - (__s32)m_PtrCtlSkinPara->ParaByte1;
		Tv_PtrCtlPnlDir->RcRight.top		=	m_CtlRcScr.top + ((Tv_SizeY - Tv_IdcSize) >> 1);
		Tv_PtrCtlPnlDir->RcRight.left		=	Tv_PtrCtlPnlDir->RcRight.right - Tv_IdcSize;
		Tv_PtrCtlPnlDir->RcRight.bottom	=	Tv_PtrCtlPnlDir->RcLeft.top + Tv_IdcSize;

		// top
		Tv_PtrCtlPnlDir->RcUp.left			=	m_CtlRcScr.left + ((Tv_SizeX - Tv_IdcSize) >> 1);
		Tv_PtrCtlPnlDir->RcUp.top				=	m_CtlRcScr.top + (__s32)m_PtrCtlSkinPara->ParaByte1;
		Tv_PtrCtlPnlDir->RcUp.right			=	Tv_PtrCtlPnlDir->RcUp.left + Tv_IdcSize;
		Tv_PtrCtlPnlDir->RcUp.bottom		=	Tv_PtrCtlPnlDir->RcUp.top + Tv_IdcSize;

		// bottom
		Tv_PtrCtlPnlDir->RcDown.left		=	m_CtlRcScr.left + ((Tv_SizeX - Tv_IdcSize) >> 1);
		Tv_PtrCtlPnlDir->RcDown.bottom	=	m_CtlRcScr.bottom - (__s32)m_PtrCtlSkinPara->ParaByte1;
		Tv_PtrCtlPnlDir->RcDown.right		=	Tv_PtrCtlPnlDir->RcDown.left + Tv_IdcSize;
		Tv_PtrCtlPnlDir->RcDown.top			=	Tv_PtrCtlPnlDir->RcDown.bottom - Tv_IdcSize;

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlDrawBtnImgTid(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoBtnImgTidPara	Tv_PtrParaBtnImgTid;
		St_GrRect	Tv_RcScr;
		WCHAR*	Tv_StrTxt;

	// code --------------------
		// init
		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdBtnImgTid);
		Tv_PtrParaBtnImgTid		=	(Ptr_GrP5UoBtnImgTidPara)Tv_PtrHrcy->PtrPara;

		// draw base
		LcCtlDrawBtnImgBase();

		// draw text
		Tv_StrTxt	=	GrP5TextGet(Tv_PtrParaBtnImgTid->TxtId);
		if(NULL != Tv_StrTxt)
		{
			// get rect
			if(GrP5RelRectToAbsRect(&Tv_PtrParaBtnImgTid->TxtRc, &m_CtlRcScr, &Tv_RcScr))
			{
				GrP5PntReqTxtStyle(&Tv_PtrParaBtnImgTid->TxtPara, Tv_PtrParaBtnImgTid->FontId);
				GrP5PntReqTxtCus();
				// display text
				GrP5PntReqTxtDraw(&Tv_RcScr, Tv_StrTxt, (__u16)m_CtlStat);		// txt buf ok
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlDrawBtnLtxt(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5SkinCtlBtnBase	Tv_PtrCtlBtnBase;

		Ptr_GrP5UoBtnLtxtPara		Tv_PtrParaBtnLtxt;

		St_GrRect	Tv_RcScr;
	// code --------------------
		// init
		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdBtnBase);
		Tv_PtrCtlBtnBase		=	(Ptr_GrP5SkinCtlBtnBase)Tv_PtrHrcy->PtrSkinCtl;

		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdBtnLtxt);
		Tv_PtrParaBtnLtxt		=	(Ptr_GrP5UoBtnLtxtPara)Tv_PtrHrcy->PtrPara;


		LcBtnBaseStatUpdate();

		// draw background
		LcDrawBtnStd(m_CtlStat);

		// draw text
		if(NULL != Tv_PtrCtlBtnBase->Str)
		{
			// get rect
			if(GrP5RelRectToAbsRect(&Tv_PtrParaBtnLtxt->TxtRc, &m_CtlRcScr, &Tv_RcScr))
			{
				GrP5PntReqTxtStyle(&Tv_PtrParaBtnLtxt->TxtPara, Tv_PtrParaBtnLtxt->FontId);
				GrP5PntReqTxtCus();
				// display text
				GrP5PntReqTxtDraw(&Tv_RcScr, Tv_PtrCtlBtnBase->Str, (__u16)m_CtlStat);	// txt buf ok
			}
		}


}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlDrawBtnSint(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5SkinCtlBtnBase	Tv_PtrCtlBtnBase;

		Ptr_GrP5UoBtnSintPara		Tv_PtrParaBtnSint;

		St_GrRect	Tv_RcScr;
	// code --------------------
		// init
		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdBtnBase);
		Tv_PtrCtlBtnBase		=	(Ptr_GrP5SkinCtlBtnBase)Tv_PtrHrcy->PtrSkinCtl;

		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdBtnSint);
		Tv_PtrParaBtnSint		=	(Ptr_GrP5UoBtnSintPara)Tv_PtrHrcy->PtrPara;

		LcBtnBaseStatUpdate();

		// draw background
		LcDrawBtnStd(m_CtlStat);

		// get rect
		if(NULL != Tv_PtrCtlBtnBase->Str)
		{
			if(GrP5RelRectToAbsRect(&Tv_PtrParaBtnSint->TxtRc, &m_CtlRcScr, &Tv_RcScr))
			{
				GrP5PntReqTxtStyle(&Tv_PtrParaBtnSint->TxtPara, Tv_PtrParaBtnSint->FontId);
				GrP5PntReqTxtCus();
				// display text
				GrP5PntReqTxtDraw(&Tv_RcScr, Tv_PtrCtlBtnBase->Str, (__u16)m_CtlStat);	// txt buf ok
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlDrawBtnStxt(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5SkinCtlBtnBase	Tv_PtrCtlBtnBase;

		Ptr_GrP5UoBtnStxtPara		Tv_PtrParaBtnStxt;

		St_GrRect	Tv_RcScr;
	// code --------------------
		// init
		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdBtnBase);
		Tv_PtrCtlBtnBase		=	(Ptr_GrP5SkinCtlBtnBase)Tv_PtrHrcy->PtrSkinCtl;

		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdBtnStxt);
		Tv_PtrParaBtnStxt		=	(Ptr_GrP5UoBtnStxtPara)Tv_PtrHrcy->PtrPara;

		LcBtnBaseStatUpdate();

		// draw background
		LcDrawBtnStd(m_CtlStat);

		// get rect
		if(NULL != Tv_PtrCtlBtnBase->Str)
		{
			// get rect
			if(GrP5RelRectToAbsRect(&Tv_PtrParaBtnStxt->TxtRc, &m_CtlRcScr, &Tv_RcScr))
			{
				GrP5PntReqTxtStyle(&Tv_PtrParaBtnStxt->TxtPara, Tv_PtrParaBtnStxt->FontId);
				GrP5PntReqTxtCus();
				// display text
				GrP5PntReqTxtDraw(&Tv_RcScr, Tv_PtrCtlBtnBase->Str, (__u16)m_CtlStat);	// text buf ok - setup
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlDrawBtnTabBase(void)
{
	// local -------------------
	// code --------------------

		LcBtnBaseStatUpdate();

		if(((Cls_GrP5UoBtnTabBase*)m_CtlUo)->IsTabSelected())
		{
			m_CtlStat	=	m_CtlStat | E_GrP5StatOnChecked;
		}

		// draw background
		LcDrawBtnStd(m_CtlStat);

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlDrawBtnTabTid(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5SkinCtlBtnBase	Tv_PtrSkinCtlBtnBase;

		Ptr_GrP5UoBtnTabTidPara	Tv_PtrParaBtnTabTid;

		St_GrRect	Tv_RcScr;
		__u32	Tv_Stat;
		__u16	Tv_ImgId;
		Ptr_GrGdib	Tv_PtrGdib;
	// code --------------------
		// init
		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdBtnBase);
		Tv_PtrSkinCtlBtnBase	=	(Ptr_GrP5SkinCtlBtnBase)Tv_PtrHrcy->PtrSkinCtl;

		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdBtnTabTid);
		Tv_PtrParaBtnTabTid		=	(Ptr_GrP5UoBtnTabTidPara)Tv_PtrHrcy->PtrPara;

		// darw background
		LcCtlDrawBtnTabBase();

		// image
		Tv_PtrGdib	=	NULL;
		Tv_ImgId		=	E_GrP5ImgIdNone;
		Tv_Stat			=	E_GrP5StatConstMask & m_CtlStat;
		if(E_GrP5StatPush == Tv_Stat)
		{
			Tv_ImgId		=	Tv_PtrParaBtnTabTid->ImgPush;
		}
		else if(E_GrP5StatDisable == Tv_Stat)
		{
			Tv_ImgId		=	Tv_PtrParaBtnTabTid->ImgDisable;
		}
		else
		{
			// normal
			Tv_ImgId		=	Tv_PtrParaBtnTabTid->ImgNormal;
			if(0 != (m_CtlStat & E_GrP5StatOnChecked))
			{
				Tv_ImgId		=	Tv_PtrParaBtnTabTid->ImgOnSelected;
			}
			else
			{
				if(0 != (m_CtlStat & E_GrP5StatOnFocus))
				{
					Tv_ImgId		=	Tv_PtrParaBtnTabTid->ImgOnFocus;
				}
			}
		}
		if(E_GrP5ImgIdNone != Tv_ImgId)
		{
			// draw image
			Tv_PtrGdib	=	GrP5ImageGet(Tv_ImgId);
			if(NULL != Tv_PtrGdib)
			{
				// draw
				// get rect
				if(GrP5RelRectToAbsRect(&Tv_PtrParaBtnTabTid->ImgRc, &m_CtlRcScr, &Tv_RcScr))
				{
					// draw image
					ReqPntGdib(Tv_PtrGdib, &Tv_RcScr, Tv_PtrParaBtnTabTid->ImgAlignHz, Tv_PtrParaBtnTabTid->ImgAlignVt, 
						Tv_PtrParaBtnTabTid->IsImgSprite);
				}
			}
		}

		// draw text
		// check exist text
		Tv_PtrSkinCtlBtnBase->Str	=	GrP5TextGet(Tv_PtrParaBtnTabTid->TxtId);
		if(NULL != Tv_PtrSkinCtlBtnBase->Str)
		{
			// get rect
			if(GrP5RelRectToAbsRect(&Tv_PtrParaBtnTabTid->TxtRc, &m_CtlRcScr, &Tv_RcScr))
			{
				GrP5PntReqTxtStyle(&Tv_PtrParaBtnTabTid->TxtPara, Tv_PtrParaBtnTabTid->FontId);
				GrP5PntReqTxtCus();
				// display text
				GrP5PntReqTxtDraw(&Tv_RcScr, Tv_PtrSkinCtlBtnBase->Str, (__u16)m_CtlStat);	// txt buf ok - scpt
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlDrawBtnTid(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoBtnTidPara		Tv_PtrParaBtnTid;

		St_GrRect		Tv_RcScr;
		WCHAR*			Tv_Str;
	// code --------------------
		// init
		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdBtnTid);
		Tv_PtrParaBtnTid			=	(Ptr_GrP5UoBtnTidPara)Tv_PtrHrcy->PtrPara;

		LcBtnBaseStatUpdate();

		// draw background
		LcDrawBtnStd(m_CtlStat);

		// draw text
		Tv_Str		=	GrP5TextGet(Tv_PtrParaBtnTid->TxtId);
		if(NULL != Tv_Str)
		{
			if(GrP5RelRectToAbsRect(&Tv_PtrParaBtnTid->TxtRc, &m_CtlRcScr, &Tv_RcScr))
			{
				GrP5PntReqTxtStyle(&Tv_PtrParaBtnTid->TxtPara, Tv_PtrParaBtnTid->FontId);
				GrP5PntReqTxtCus();
				GrP5PntReqTxtDraw(&Tv_RcScr, Tv_Str, (__u16)m_CtlStat);	// txt buf ok
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlDrawBtnTime(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5SkinCtlBtnBase	Tv_PtrCtlBtnBase;

		Ptr_GrP5UoBtnTimePara		Tv_PtrParaBtnTime;

		St_GrRect		Tv_RcScr;
	// code --------------------

		// init
		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdBtnBase);
		Tv_PtrCtlBtnBase		=	(Ptr_GrP5SkinCtlBtnBase)Tv_PtrHrcy->PtrSkinCtl;

		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdBtnTime);
		Tv_PtrParaBtnTime			=	(Ptr_GrP5UoBtnTimePara)Tv_PtrHrcy->PtrPara;

		LcBtnBaseStatUpdate();

		// draw background
		LcDrawBtnStd(m_CtlStat);

		if(NULL != Tv_PtrCtlBtnBase->Str)
		{
			// get rect
			if(GrP5RelRectToAbsRect(&Tv_PtrParaBtnTime->TxtRc, &m_CtlRcScr, &Tv_RcScr))
			{
				GrP5PntReqTxtStyle(&Tv_PtrParaBtnTime->TxtPara, Tv_PtrParaBtnTime->FontId);
				GrP5PntReqTxtCus();
				GrP5PntReqTxtDraw(&Tv_RcScr, Tv_PtrCtlBtnBase->Str, (__u16)m_CtlStat);	// txt buf ok
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlDrawBtnVkey(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5SkinCtlBtnBase	Tv_PtrCtlBtnBase;

		Ptr_GrP5UoBtnVkeyPara		Tv_PtrParaBtnVkey;
		Ptr_GrP5SkinCtlBtnVkey	Tv_PtrCtlBtnVkey;

		St_GrRect		Tv_RcScr;
	// code --------------------
		// init
		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdBtnBase);
		Tv_PtrCtlBtnBase		=	(Ptr_GrP5SkinCtlBtnBase)Tv_PtrHrcy->PtrSkinCtl;

		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdBtnVkey);
		Tv_PtrParaBtnVkey			=	(Ptr_GrP5UoBtnVkeyPara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlBtnVkey			=	(Ptr_GrP5SkinCtlBtnVkey)Tv_PtrHrcy->PtrSkinCtl;

		LcBtnBaseStatUpdate();

		// draw background
		LcDrawBtnStd(m_CtlStat);

		if(NULL != Tv_PtrCtlBtnBase->Str)
		{
			if(GrP5RelRectToAbsRect(&Tv_PtrParaBtnVkey->TxtRc, &m_CtlRcScr, &Tv_RcScr))
			{
				GrP5PntReqTxtStyle(&Tv_PtrParaBtnVkey->TxtPara, Tv_PtrParaBtnVkey->FontId);
				GrP5PntReqTxtCus();

				// display text
				Tv_PtrCtlBtnBase->Str[0]	=	Tv_PtrParaBtnVkey->Code[Tv_PtrCtlBtnVkey->LangIdx][Tv_PtrCtlBtnVkey->ShftIdx];
				Tv_PtrCtlBtnBase->Str[1]	=	0;
				GrP5PntReqTxtDraw(&Tv_RcScr, Tv_PtrCtlBtnBase->Str, (__u16)m_CtlStat);	// txt buf ok
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlDrawChkImgLtBase(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5SkinCtlBtnBase	Tv_PtrCtlBtnBase;

		Ptr_GrP5UoBtnLtxtPara		Tv_PtrParaBtnLtxt;

		Ptr_GrP5UoChkImgLtBasePara	Tv_PtrParaChkImgLtBase;

		St_GrRect	Tv_RcScr;
		Ptr_GrGdib	Tv_PtrImg;
	// code --------------------
		// init
		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdBtnBase);
		Tv_PtrCtlBtnBase		=	(Ptr_GrP5SkinCtlBtnBase)Tv_PtrHrcy->PtrSkinCtl;

		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdBtnLtxt);
		Tv_PtrParaBtnLtxt		=	(Ptr_GrP5UoBtnLtxtPara)Tv_PtrHrcy->PtrPara;

		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdChkImgLtBase);
		Tv_PtrParaChkImgLtBase	=	(Ptr_GrP5UoChkImgLtBasePara)Tv_PtrHrcy->PtrPara;

		LcChkBaseStatUpdate();

		// draw background
		LcDrawBtnStd(m_CtlStat);

		// draw image
		if(GrP5RelRectToAbsRect(&Tv_PtrParaChkImgLtBase->ImgRc, &m_CtlRcScr, &Tv_RcScr))
		{
			Tv_PtrImg		=	NULL;
			if(E_GrP5StatDisable == (E_GrP5StatConstMask & m_CtlStat))
			{
				Tv_PtrImg		=	GrP5ImageGet(Tv_PtrParaChkImgLtBase->ImgIdDisable);
			}
			else if(E_GrP5StatPush == (E_GrP5StatConstMask & m_CtlStat))
			{
				Tv_PtrImg		=	GrP5ImageGet(Tv_PtrParaChkImgLtBase->ImgIdPush);
			}
			else if(0 != (E_GrP5StatOnChecked & m_CtlStat))
			{
				Tv_PtrImg		=	GrP5ImageGet(Tv_PtrParaChkImgLtBase->ImgIdChecked);
			}
			else
			{
				Tv_PtrImg		=	GrP5ImageGet(Tv_PtrParaChkImgLtBase->ImgIdNormal);
			}

			if(NULL != Tv_PtrImg)
			{
				ReqPntGdib(Tv_PtrImg, &Tv_RcScr, Tv_PtrParaChkImgLtBase->ImgAlignHz, Tv_PtrParaChkImgLtBase->ImgAlignVt,
					Tv_PtrParaChkImgLtBase->IsImgSprite);
			}
		}

		// draw text
		if(NULL != Tv_PtrCtlBtnBase->Str)
		{
			// get rect
			if(GrP5RelRectToAbsRect(&Tv_PtrParaBtnLtxt->TxtRc, &m_CtlRcScr, &Tv_RcScr))
			{
				GrP5PntReqTxtStyle(&Tv_PtrParaBtnLtxt->TxtPara, Tv_PtrParaBtnLtxt->FontId);
				GrP5PntReqTxtCus();
				// display text
				GrP5PntReqTxtDraw(&Tv_RcScr, Tv_PtrCtlBtnBase->Str, (__u16)m_CtlStat);	// txt buf ok
			}
		}

		// draw skin background
		LcDrawChkBgTop(m_PtrCtlSkinPara, &m_CtlRcScr, m_CtlStat);

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlDrawChkImgTiBase(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5SkinCtlBtnBase	Tv_PtrCtlBtnBase;

		Ptr_GrP5UoBtnTidPara		Tv_PtrParaBtnTid;

		Ptr_GrP5UoChkImgTiBasePara	Tv_PtrParaChkImgTiBase;

		St_GrRect	Tv_RcScr;
		Ptr_GrGdib	Tv_PtrImg;
		WCHAR*			Tv_Str;
	// code --------------------
		// init
		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdBtnBase);
		Tv_PtrCtlBtnBase		=	(Ptr_GrP5SkinCtlBtnBase)Tv_PtrHrcy->PtrSkinCtl;

		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdBtnTid);
		Tv_PtrParaBtnTid		=	(Ptr_GrP5UoBtnTidPara)Tv_PtrHrcy->PtrPara;

		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdChkImgTiBase);
		Tv_PtrParaChkImgTiBase	=	(Ptr_GrP5UoChkImgTiBasePara)Tv_PtrHrcy->PtrPara;

		LcChkBaseStatUpdate();

		// draw background
		LcDrawBtnStd(m_CtlStat);

		// draw image
		if(GrP5RelRectToAbsRect(&Tv_PtrParaChkImgTiBase->ImgRc, &m_CtlRcScr, &Tv_RcScr))
		{
			Tv_PtrImg		=	NULL;
			if(E_GrP5StatDisable == (E_GrP5StatConstMask & m_CtlStat))
			{
				Tv_PtrImg		=	GrP5ImageGet(Tv_PtrParaChkImgTiBase->ImgIdDisable);
			}
			else if(E_GrP5StatPush == (E_GrP5StatConstMask & m_CtlStat))
			{
				Tv_PtrImg		=	GrP5ImageGet(Tv_PtrParaChkImgTiBase->ImgIdPush);
			}
			else if(0 != (E_GrP5StatOnChecked & m_CtlStat))
			{
				Tv_PtrImg		=	GrP5ImageGet(Tv_PtrParaChkImgTiBase->ImgIdChecked);
			}
			else
			{
				Tv_PtrImg		=	GrP5ImageGet(Tv_PtrParaChkImgTiBase->ImgIdNormal);
			}

			if(NULL != Tv_PtrImg)
			{
				ReqPntGdib(Tv_PtrImg, &Tv_RcScr, Tv_PtrParaChkImgTiBase->ImgAlignHz, Tv_PtrParaChkImgTiBase->ImgAlignVt,
					Tv_PtrParaChkImgTiBase->IsImgSprite);
			}
		}

		// draw text
		Tv_Str		=	GrP5TextGet(Tv_PtrParaBtnTid->TxtId);
		if(NULL != Tv_Str)
		{
			if(GrP5RelRectToAbsRect(&Tv_PtrParaBtnTid->TxtRc, &m_CtlRcScr, &Tv_RcScr))
			{
				GrP5PntReqTxtStyle(&Tv_PtrParaBtnTid->TxtPara, Tv_PtrParaBtnTid->FontId);
				GrP5PntReqTxtCus();
				GrP5PntReqTxtDraw(&Tv_RcScr, Tv_Str, (__u16)m_CtlStat);	// txt buf ok
			}
		}

		// draw skin background
		LcDrawChkBgTop(m_PtrCtlSkinPara, &m_CtlRcScr, m_CtlStat);

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlDrawChkTiBase(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5SkinCtlBtnBase	Tv_PtrCtlBtnBase;

		Ptr_GrP5UoBtnTidPara		Tv_PtrParaBtnTid;

		Ptr_GrP5UoChkTiBasePara	Tv_PtrParaChkTiBase;

		St_GrRect	Tv_RcScr;
		WCHAR*			Tv_Str;
		__u32				Tv_Stat;
	// code --------------------
		// init
		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdBtnBase);
		Tv_PtrCtlBtnBase		=	(Ptr_GrP5SkinCtlBtnBase)Tv_PtrHrcy->PtrSkinCtl;

		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdBtnTid);
		Tv_PtrParaBtnTid		=	(Ptr_GrP5UoBtnTidPara)Tv_PtrHrcy->PtrPara;

		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdChkTiBase);
		Tv_PtrParaChkTiBase	=	(Ptr_GrP5UoChkTiBasePara)Tv_PtrHrcy->PtrPara;

		LcChkBaseStatUpdate();

		// draw background
		LcDrawBtnStd(m_CtlStat);

		// get rect
		if(GrP5RelRectToAbsRect(&Tv_PtrParaChkTiBase->MarkRc, &m_CtlRcScr, &Tv_RcScr))
		{
			LcDrawChkMark(m_PtrCtlSkinPara, &Tv_RcScr, m_CtlStat);
		}

		// draw text
		Tv_Str		=	GrP5TextGet(Tv_PtrParaBtnTid->TxtId);
		if(NULL != Tv_Str)
		{
			if(GrP5RelRectToAbsRect(&Tv_PtrParaBtnTid->TxtRc, &m_CtlRcScr, &Tv_RcScr))
			{
				GrP5PntReqTxtStyle(&Tv_PtrParaBtnTid->TxtPara, Tv_PtrParaBtnTid->FontId);
				GrP5PntReqTxtCus();
				Tv_Stat	=	m_CtlStat & (~E_GrP5FontStatOnChecked);
				GrP5PntReqTxtDraw(&Tv_RcScr, Tv_Str, (__u16)Tv_Stat);	// txt buf ok
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlDrawGridSmd(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoGridBasePara	Tv_PtrParaGridBase;
		Ptr_GrP5SkinCtlGridBase	Tv_PtrCtlGridBase;
		
		__u32	Tv_IdxX;
		__u32	Tv_IdxY;
		__u32	Tv_Stat;
		St_GrRect	Tv_RcCell;
	// code --------------------
		// init
		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdGridBase);
		Tv_PtrParaGridBase		=	(Ptr_GrP5UoGridBasePara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlGridBase			=	(Ptr_GrP5SkinCtlGridBase)Tv_PtrHrcy->PtrSkinCtl;

		// check draw able
		if((0 == Tv_PtrCtlGridBase->CellSizeX) || (0 == Tv_PtrCtlGridBase->CellSizeY))
		{
			return;
		}

		// draw item
		for(Tv_IdxY = 0; Tv_IdxY < Tv_PtrCtlGridBase->CellCntY; Tv_IdxY++)
		{
			for(Tv_IdxX = 0; Tv_IdxX < Tv_PtrCtlGridBase->CellCntX; Tv_IdxX++)
			{
				// get rect
				LcCtlItmRectGridBase((__s32)Tv_IdxX, (__s32)Tv_IdxY, (__s32)Tv_IdxX, (__s32)Tv_IdxY, &Tv_RcCell);
				if(0 < Tv_RcCell.right)
				{
					Tv_Stat	=	0;
					if(((__u32)Tv_PtrCtlGridBase->CusIdxX == Tv_IdxX) && ((__u32)Tv_PtrCtlGridBase->CusIdxY == Tv_IdxY))
					{
						Tv_Stat	=	Tv_Stat | E_GrP5StatOnCtrlCus;
					}
					if(0 != ((Cls_GrP5UoGridBase*)m_CtlUo)->CellValueGet(Tv_IdxX, Tv_IdxY))
					{
						Tv_Stat	=	Tv_Stat | E_GrP5StatOnChecked;
					}
					LcDrawGridSmdItem(m_PtrCtlSkinPara, &Tv_RcCell, Tv_Stat);
				}
			}
		}

		// draw drag
		LcCtlItmRectGridBase(Tv_PtrCtlGridBase->DragStX, Tv_PtrCtlGridBase->DragStY, Tv_PtrCtlGridBase->DragEdX, Tv_PtrCtlGridBase->DragEdY, &Tv_RcCell);
		if(0 < Tv_RcCell.right)
		{
			LcDrawGridDrag(m_PtrCtlSkinPara, &Tv_RcCell);
		}


}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlDrawGridSrscd(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoGridBasePara	Tv_PtrParaGridBase;
		Ptr_GrP5SkinCtlGridBase	Tv_PtrCtlGridBase;

		__u32	Tv_IdxX;
		__u32	Tv_IdxY;
		__u32	Tv_Stat;
		__u32	Tv_Val;
		St_GrRect	Tv_RcCell;
	// code --------------------
		// init
		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdGridBase);
		Tv_PtrParaGridBase		=	(Ptr_GrP5UoGridBasePara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlGridBase			=	(Ptr_GrP5SkinCtlGridBase)Tv_PtrHrcy->PtrSkinCtl;

		// check draw able
		if((0 == Tv_PtrCtlGridBase->CellSizeX) || (0 == Tv_PtrCtlGridBase->CellSizeY) || 
			(0 == Tv_PtrCtlGridBase->CellCntX) || (0 == Tv_PtrCtlGridBase->CellCntY))
		{
			return;
		}

		// draw items
		for(Tv_IdxY = 0; Tv_IdxY < Tv_PtrCtlGridBase->CellCntY; Tv_IdxY++)
		{
			for(Tv_IdxX = 0; Tv_IdxX < Tv_PtrCtlGridBase->CellCntX; Tv_IdxX++)
			{
				// get rect
				LcCtlItmRectGridBase((__s32)Tv_IdxX, (__s32)Tv_IdxY, (__s32)Tv_IdxX, (__s32)Tv_IdxY, &Tv_RcCell);
				if(0 < Tv_RcCell.right)
				{
					Tv_Stat	=	0;
					if(((__u32)Tv_PtrCtlGridBase->CusIdxX == Tv_IdxX) && ((__u32)Tv_PtrCtlGridBase->CusIdxY == Tv_IdxY))
					{
						Tv_Stat	=	Tv_Stat | E_GrP5StatOnCtrlCus;
					}

					Tv_Val	=	((Cls_GrP5UoGridBase*)m_CtlUo)->CellValueGet(Tv_IdxX, Tv_IdxY);
					LcDrawGridRscdItem(m_PtrCtlSkinPara, &Tv_RcCell, Tv_Stat, (__u8)Tv_Val);
				}
			}
		}

		// draw drag
		LcCtlItmRectGridBase(Tv_PtrCtlGridBase->DragStX, Tv_PtrCtlGridBase->DragStY, Tv_PtrCtlGridBase->DragEdX, Tv_PtrCtlGridBase->DragEdY, &Tv_RcCell);
		if(0 < Tv_RcCell.right)
		{
			LcDrawGridRscdDrag(m_PtrCtlSkinPara, &Tv_RcCell);
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlDrawGrpHzDmap(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoGrpHzBasePara		Tv_PtrParaGrpHzBase;
		Ptr_GrP5SkinCtlGrpHzBase	Tv_PtrCtlGrpHzBase;

		Ptr_GrP5UoGrpHzDmapPara		Tv_PtrParaGrpHzDmap;
		Ptr_GrP5SkinCtlGrpHzDmap	Tv_PtrCtlGrpHzDmap;

		__u32	Tv_IdxX;
		__u32	Tv_IdxY;
		__u32	Tv_Val;

		St_GrRect	Tv_RcScr;
	// code --------------------
		// init
		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdGrpHzBase);
		Tv_PtrParaGrpHzBase		=	(Ptr_GrP5UoGrpHzBasePara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlGrpHzBase		=	(Ptr_GrP5SkinCtlGrpHzBase)Tv_PtrHrcy->PtrSkinCtl;

		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdGrpHzDmap);
		Tv_PtrParaGrpHzDmap		=	(Ptr_GrP5UoGrpHzDmapPara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlGrpHzDmap		=	(Ptr_GrP5SkinCtlGrpHzDmap)Tv_PtrHrcy->PtrSkinCtl;

		// update status
		m_CtlStat		=	0;
		if(((Cls_GrP5UoBase*)m_CtlUo)->IsOnFocuse())
		{
			m_CtlStat	=	E_GrP5StatOnFocus;
		}

		// draw background
		LcDrawGraphHzBg(m_PtrCtlSkinPara, &m_CtlRcScr, m_CtlStat);

		// paint contents able
		if((0 == Tv_PtrCtlGrpHzBase->CellCntX) || (0 == Tv_PtrCtlGrpHzDmap->VwCntY))
		{
			return;
		}

		if((0 == Tv_PtrCtlGrpHzDmap->CellSizeX) || (0 == Tv_PtrCtlGrpHzDmap->CellSizeY))
		{
			return;
		}

		// draw contents
		// check draw able
		Tv_RcScr.top		=	m_CtlRcScr.top + E_GrP5SkinT1GraphHzMargin;
		for(Tv_IdxY = 0; Tv_IdxY < Tv_PtrCtlGrpHzDmap->VwCntY; Tv_IdxY++)
		{
			Tv_RcScr.bottom	=	Tv_RcScr.top + (__s32)Tv_PtrCtlGrpHzDmap->CellSizeY;

			Tv_RcScr.left		=	m_CtlRcScr.left + E_GrP5SkinT1GraphHzMargin;
			for(Tv_IdxX = 0; Tv_IdxX < Tv_PtrCtlGrpHzBase->CellCntX; Tv_IdxX++)
			{
				Tv_RcScr.right	=	Tv_RcScr.left + (__s32)Tv_PtrCtlGrpHzDmap->CellSizeX;
				// draw mark
				Tv_Val	=	((Cls_GrP5UoGrpHzDmap*)m_CtlUo)->ItemValueGet(Tv_IdxX, Tv_IdxY) & 7;
				LcDrawGraphHzItem(m_PtrCtlSkinPara, &Tv_RcScr, Tv_Val);
				// next x
				Tv_RcScr.left		=	Tv_RcScr.right;
			}
			// next y
			Tv_RcScr.top		=	Tv_RcScr.bottom;
		}

		// draw cursor
		if((0 <= Tv_PtrCtlGrpHzBase->CusIdxX))
		{
			LcCtlItmRectGrpHzDmap(Tv_PtrCtlGrpHzBase->CusIdxX, 0, Tv_PtrCtlGrpHzBase->CusIdxX, Tv_PtrCtlGrpHzDmap->VwCntY - 1, &Tv_RcScr);
			if(0 < Tv_RcScr.right)
			{
				// draw
				LcDrawGraphHzCursorVt(m_PtrCtlSkinPara, &Tv_RcScr, 0);
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawCtlGrpHzT1Ruler(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoGrpHzBasePara		Tv_PtrParaGrpHzBase;
		Ptr_GrP5SkinCtlGrpHzBase	Tv_PtrCtlGrpHzBase;

		Ptr_GrP5UoGrpHzT1Para		Tv_PtrParaGrpHzT1;
		Ptr_GrP5SkinCtlGrpHzT1	Tv_PtrCtlGrpHzT1;

		St_GrRect	Tv_Rc;
		__u32			Tv_RulIdx;
		__s32			Tv_StepX;
		__s32			Tv_BaseX;

	// code --------------------
		// init
		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdGrpHzBase);
		Tv_PtrParaGrpHzBase		=	(Ptr_GrP5UoGrpHzBasePara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlGrpHzBase		=	(Ptr_GrP5SkinCtlGrpHzBase)Tv_PtrHrcy->PtrSkinCtl;

		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdGrpHzT1);
		Tv_PtrParaGrpHzT1		=	(Ptr_GrP5UoGrpHzT1Para)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlGrpHzT1		=	(Ptr_GrP5SkinCtlGrpHzT1)Tv_PtrHrcy->PtrSkinCtl;

		// ruler body
		if((0 != Tv_PtrParaGrpHzT1->RulerUnitWidth) && (0 != Tv_PtrParaGrpHzT1->RulerHeight))
		{
			GrP5PntReqTxtStyle(&Tv_PtrParaGrpHzT1->RulerTxtStyle, Tv_PtrParaGrpHzT1->RulerFontId);
			GrP5PntReqTxtCus();

			Tv_RulIdx		=	0;
			if(Tv_PtrCtlGrpHzT1->IsDayMode)
			{
				// day mode
				Tv_StepX			=	(__s32)Tv_PtrCtlGrpHzT1->CellSizeX * 6;

				// draw 0
				Tv_Rc.left		=	m_CtlRcScr.left + E_GrP5SkinT1GrpHzT1ContMgn;
				Tv_Rc.top			=	m_CtlRcScr.top;
				Tv_Rc.right		=	Tv_Rc.left + (__s32)(__u32)Tv_PtrParaGrpHzT1->RulerUnitWidth;
				Tv_Rc.bottom	=	Tv_Rc.top + (__s32)(__u32)Tv_PtrParaGrpHzT1->RulerHeight;
				GrP5PntReqTxtDraw(&Tv_Rc, V_GrP5SkinT1StrRulerHrT1[Tv_RulIdx], (__u16)m_CtlStat);		// txt buf ok
				Tv_RulIdx ++;
				// draw 6
				Tv_Rc.left		=	Tv_Rc.left + Tv_StepX;
				Tv_Rc.right		=	Tv_Rc.left + (__s32)(__u32)Tv_PtrParaGrpHzT1->RulerUnitWidth;

				GrP5PntReqTxtDraw(&Tv_Rc, V_GrP5SkinT1StrRulerHrT1[Tv_RulIdx], (__u16)m_CtlStat);		// txt buf ok
				Tv_RulIdx ++;

				// draw 12
				Tv_Rc.left		=	Tv_Rc.left + Tv_StepX;
				Tv_Rc.right		=	Tv_Rc.left + (__s32)(__u32)Tv_PtrParaGrpHzT1->RulerUnitWidth;

				GrP5PntReqTxtDraw(&Tv_Rc, V_GrP5SkinT1StrRulerHrT1[Tv_RulIdx], (__u16)m_CtlStat);		// txt buf ok
				Tv_RulIdx ++;

				// draw 18
				Tv_Rc.left		=	Tv_Rc.left + Tv_StepX;
				Tv_Rc.right		=	Tv_Rc.left + (__s32)(__u32)Tv_PtrParaGrpHzT1->RulerUnitWidth;

				GrP5PntReqTxtDraw(&Tv_Rc, V_GrP5SkinT1StrRulerHrT1[Tv_RulIdx], (__u16)m_CtlStat);		// txt buf ok
				Tv_RulIdx ++;

				// darw 23
				Tv_Rc.right		=	Tv_Rc.left + Tv_StepX;
				Tv_Rc.left		=	Tv_Rc.right - (__s32)(__u32)Tv_PtrParaGrpHzT1->RulerUnitWidth;

				GrP5PntReqTxtDraw(&Tv_Rc, V_GrP5SkinT1StrRulerHrT1[Tv_RulIdx], (__u16)m_CtlStat);		// txt buf ok
				Tv_RulIdx ++;

			}
			else
			{
				// minute mode
				Tv_StepX			=	(__s32)Tv_PtrCtlGrpHzT1->CellSizeX * 10;
				Tv_BaseX			=	m_CtlRcScr.left + E_GrP5SkinT1GrpHzT1ContMgn;

				// draw 0
				Tv_Rc.left		=	Tv_BaseX;
				Tv_Rc.top			=	m_CtlRcScr.top;
				Tv_Rc.right		=	Tv_Rc.left + (__s32)(__u32)Tv_PtrParaGrpHzT1->RulerUnitWidth;
				Tv_Rc.bottom	=	Tv_Rc.top + (__s32)(__u32)Tv_PtrParaGrpHzT1->RulerHeight;

				GrP5PntReqTxtDraw(&Tv_Rc, V_GrP5SkinT1StrRulerMinT1[Tv_RulIdx], (__u16)m_CtlStat);		// txt buf ok
				Tv_RulIdx ++;
				Tv_BaseX			=	Tv_BaseX + Tv_StepX;

				// draw 10
				Tv_Rc.left		=	Tv_BaseX - (((__s32)(__u32)Tv_PtrParaGrpHzT1->RulerUnitWidth - Tv_PtrCtlGrpHzT1->CellSizeX) >> 1);
				Tv_Rc.right		=	Tv_Rc.left + (__s32)(__u32)Tv_PtrParaGrpHzT1->RulerUnitWidth;

				GrP5PntReqTxtDraw(&Tv_Rc, V_GrP5SkinT1StrRulerMinT1[Tv_RulIdx], (__u16)m_CtlStat);		// txt buf ok
				Tv_RulIdx ++;
				Tv_BaseX			=	Tv_BaseX + Tv_StepX;

				// draw 20
				Tv_Rc.left		=	Tv_BaseX - (((__s32)(__u32)Tv_PtrParaGrpHzT1->RulerUnitWidth - Tv_PtrCtlGrpHzT1->CellSizeX) >> 1);
				Tv_Rc.right		=	Tv_Rc.left + (__s32)(__u32)Tv_PtrParaGrpHzT1->RulerUnitWidth;

				GrP5PntReqTxtDraw(&Tv_Rc, V_GrP5SkinT1StrRulerMinT1[Tv_RulIdx], (__u16)m_CtlStat);		// txt buf ok
				Tv_RulIdx ++;
				Tv_BaseX			=	Tv_BaseX + Tv_StepX;

				// draw 30
				Tv_Rc.left		=	Tv_BaseX - (((__s32)(__u32)Tv_PtrParaGrpHzT1->RulerUnitWidth - Tv_PtrCtlGrpHzT1->CellSizeX) >> 1);
				Tv_Rc.right		=	Tv_Rc.left + (__s32)(__u32)Tv_PtrParaGrpHzT1->RulerUnitWidth;

				GrP5PntReqTxtDraw(&Tv_Rc, V_GrP5SkinT1StrRulerMinT1[Tv_RulIdx], (__u16)m_CtlStat);		// txt buf ok
				Tv_RulIdx ++;
				Tv_BaseX			=	Tv_BaseX + Tv_StepX;

				// draw 40
				Tv_Rc.left		=	Tv_BaseX - (((__s32)(__u32)Tv_PtrParaGrpHzT1->RulerUnitWidth - Tv_PtrCtlGrpHzT1->CellSizeX) >> 1);
				Tv_Rc.right		=	Tv_Rc.left + (__s32)(__u32)Tv_PtrParaGrpHzT1->RulerUnitWidth;

				GrP5PntReqTxtDraw(&Tv_Rc, V_GrP5SkinT1StrRulerMinT1[Tv_RulIdx], (__u16)m_CtlStat);		// txt buf ok
				Tv_RulIdx ++;
				Tv_BaseX			=	Tv_BaseX + Tv_StepX;

				// draw 50
				Tv_Rc.left		=	Tv_BaseX - (((__s32)(__u32)Tv_PtrParaGrpHzT1->RulerUnitWidth - Tv_PtrCtlGrpHzT1->CellSizeX) >> 1);
				Tv_Rc.right		=	Tv_Rc.left + (__s32)(__u32)Tv_PtrParaGrpHzT1->RulerUnitWidth;

				GrP5PntReqTxtDraw(&Tv_Rc, V_GrP5SkinT1StrRulerMinT1[Tv_RulIdx], (__u16)m_CtlStat);		// txt buf ok
				Tv_RulIdx ++;
				Tv_BaseX			=	Tv_BaseX + Tv_StepX;

				// darw 59
				Tv_Rc.right		=	Tv_Rc.left + Tv_StepX;
				Tv_Rc.left		=	Tv_Rc.right - (__s32)(__u32)Tv_PtrParaGrpHzT1->RulerUnitWidth;

				GrP5PntReqTxtDraw(&Tv_Rc, V_GrP5SkinT1StrRulerMinT1[Tv_RulIdx], (__u16)m_CtlStat);		// txt buf ok

			}

		}	//	if(0 != Tv_PtrParaGrpHzT1->RulerUnitWidth)

		// ruler indicater
		if((NULL != Tv_PtrCtlGrpHzT1->StrInd) && (0 != Tv_PtrParaGrpHzT1->IndWidth))
		{
			if((0 <= Tv_PtrCtlGrpHzBase->CusIdxX) && ((__s32)Tv_PtrCtlGrpHzT1->VwCntX > Tv_PtrCtlGrpHzBase->CusIdxX))
			{
				Tv_Rc.left		=	m_CtlRcScr.left + E_GrP5SkinT1GrpHzT1ContMgn + (Tv_PtrCtlGrpHzT1->CellSizeX * Tv_PtrCtlGrpHzBase->CusIdxX);
				Tv_Rc.right		=	Tv_Rc.left + (__s32)(__u32)Tv_PtrParaGrpHzT1->IndWidth;
				Tv_Rc.top			=	m_CtlRcScr.top;
				Tv_Rc.bottom	=	Tv_Rc.top + (__s32)(__u32)Tv_PtrParaGrpHzT1->RulerHeight;

				if(Tv_Rc.right > m_CtlRcScr.right)
				{
					Tv_Rc.right		=	m_CtlRcScr.right;
					Tv_Rc.left		=	Tv_Rc.right - (__s32)(__u32)Tv_PtrParaGrpHzT1->IndWidth;
				}

				// draw background
				LcDrawGrpHzT1BgInd(m_PtrCtlSkinPara, &Tv_Rc, m_CtlStat);
				// draw text
				GrStrIntToWstr(Tv_PtrCtlGrpHzT1->StrInd, Tv_PtrCtlGrpHzBase->CusIdxX);

				GrP5PntReqTxtStyle(&Tv_PtrParaGrpHzT1->IndTxtStyle, Tv_PtrParaGrpHzT1->IndFontId);
				GrP5PntReqTxtCus();
				GrP5PntReqTxtDraw(&Tv_Rc, Tv_PtrCtlGrpHzT1->StrInd, (__u16)m_CtlStat);		// txt buf ok
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawCtlGrpHzTiCont(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoGrpHzBasePara		Tv_PtrParaGrpHzBase;
		Ptr_GrP5SkinCtlGrpHzBase	Tv_PtrCtlGrpHzBase;

		Ptr_GrP5UoGrpHzT1Para		Tv_PtrParaGrpHzT1;
		Ptr_GrP5SkinCtlGrpHzT1	Tv_PtrCtlGrpHzT1;

		__u32	Tv_IdxX;
		__u32	Tv_IdxY;
		__u32	Tv_Val;
		__u32	Tv_Stat;
		St_GrRect	Tv_Rc;
	// code --------------------
		// init
		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdGrpHzBase);
		Tv_PtrParaGrpHzBase		=	(Ptr_GrP5UoGrpHzBasePara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlGrpHzBase		=	(Ptr_GrP5SkinCtlGrpHzBase)Tv_PtrHrcy->PtrSkinCtl;

		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdGrpHzT1);
		Tv_PtrParaGrpHzT1		=	(Ptr_GrP5UoGrpHzT1Para)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlGrpHzT1		=	(Ptr_GrP5SkinCtlGrpHzT1)Tv_PtrHrcy->PtrSkinCtl;

		// check item count
		if((0 == Tv_PtrCtlGrpHzT1->VwCntX) || (0 == Tv_PtrCtlGrpHzT1->VwCntY))
		{
			return;
		}

		Tv_Rc.top			=	m_CtlRcScr.top + (__s32)(__u32)Tv_PtrParaGrpHzT1->RulerHeight + E_GrP5SkinT1GrpHzT1ContMgn;

		for(Tv_IdxY = 0; Tv_IdxY < Tv_PtrCtlGrpHzT1->VwCntY; Tv_IdxY++)
		{
			Tv_Rc.bottom	=	Tv_Rc.top + (__s32)Tv_PtrCtlGrpHzT1->CellSizeY;

			if(Tv_Rc.bottom > (m_CtlRcScr.bottom - E_GrP5SkinT1GrpHzT1ContMgn))
			{
				break;
			}

			Tv_Rc.left		=	m_CtlRcScr.left + E_GrP5SkinT1GrpHzT1ContMgn;
			for(Tv_IdxX = 0; Tv_IdxX < Tv_PtrCtlGrpHzT1->VwCntX; Tv_IdxX++)
			{
				Tv_Rc.right	=	Tv_Rc.left + (__s32)Tv_PtrCtlGrpHzT1->CellSizeX;
				if(Tv_Rc.right  > (m_CtlRcScr.right - E_GrP5SkinT1GrpHzT1ContMgn))
				{
					break;
				}

				// draw mark
				Tv_Val	=	((Cls_GrP5UoGrpHzT1*)m_CtlUo)->ItemValueGet(Tv_IdxX, Tv_IdxY) & 7;
				Tv_Stat	=	0;
				if((__s32)Tv_IdxX == Tv_PtrCtlGrpHzBase->CusIdxX)
				{
					Tv_Stat		=	E_GrP5StatOnCtrlCus;
				}
				LcDrawGrpHzT1Item(m_PtrCtlSkinPara, &Tv_Rc, Tv_Stat, Tv_Val);
				// next x
				Tv_Rc.left	=	Tv_Rc.right;
			}
			// next y
			Tv_Rc.top		=	Tv_Rc.bottom;
		}


}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlDrawGrpHzT1(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoGrpHzBasePara		Tv_PtrParaGrpHzBase;
		Ptr_GrP5SkinCtlGrpHzBase	Tv_PtrCtlGrpHzBase;

		Ptr_GrP5UoGrpHzT1Para		Tv_PtrParaGrpHzT1;
		Ptr_GrP5SkinCtlGrpHzT1	Tv_PtrCtlGrpHzT1;
	// code --------------------
		// init
		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdGrpHzBase);
		Tv_PtrParaGrpHzBase		=	(Ptr_GrP5UoGrpHzBasePara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlGrpHzBase		=	(Ptr_GrP5SkinCtlGrpHzBase)Tv_PtrHrcy->PtrSkinCtl;

		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdGrpHzT1);
		Tv_PtrParaGrpHzT1			=	(Ptr_GrP5UoGrpHzT1Para)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlGrpHzT1			=	(Ptr_GrP5SkinCtlGrpHzT1)Tv_PtrHrcy->PtrSkinCtl;

		// update status
		m_CtlStat		=	0;
		if(((Cls_GrP5UoBase*)m_CtlUo)->IsOnFocuse())
		{
			m_CtlStat	=	E_GrP5StatOnFocus;
		}

		// draw ruler
		LcDrawCtlGrpHzT1Ruler();

		// draw contents background
		LcDrawGrpHzT1BgCont(m_PtrCtlSkinPara, &Tv_PtrCtlGrpHzT1->RcContArea, m_CtlStat);

		// paint day item
		LcDrawCtlGrpHzTiCont();
		
}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlDrawLblTimeReg(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoLblTimeRegPara		Tv_PtrParaLblTimeReg;

		St_GrRect	Tv_Rc;
		Ptr_GrGdib	Tv_Gdib;
	// code --------------------
		// init
		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdLblTimeReg);
		Tv_PtrParaLblTimeReg	=	(Ptr_GrP5UoLblTimeRegPara)Tv_PtrHrcy->PtrPara;

		// draw base
		LcCtlDrawLblBase();

		// draw icon
		// calculate rect
		Tv_Gdib	=	GrP5ImageGet(Tv_PtrParaLblTimeReg->ImgDstId);
		if(NULL != Tv_Gdib)
		{
			if(GrP5RelRectToAbsRect(&Tv_PtrParaLblTimeReg->RcIconDst, &m_CtlRcScr, &Tv_Rc))
			{
				// draw icon
				GrP5PntReqImage(Tv_Gdib, Tv_Rc.left, Tv_Rc.top, Tv_PtrParaLblTimeReg->IsSpriteImg);
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlDrawLblTimeView(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoLblTimeViewPara		Tv_PtrParaLblTimeView;

		St_GrRect	Tv_Rc;
		Ptr_GrGdib	Tv_Gdib;
	// code --------------------
		// init
		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdLblTimeView);
		Tv_PtrParaLblTimeView	=	(Ptr_GrP5UoLblTimeViewPara)Tv_PtrHrcy->PtrPara;

		// draw base
		LcCtlDrawLblBase();

		// draw icon
		// calculate rect
		Tv_Gdib	=	GrP5ImageGet(Tv_PtrParaLblTimeView->ImgDst);
		if(NULL != Tv_Gdib)
		{
			if(GrP5RelRectToAbsRect(&Tv_PtrParaLblTimeView->RcIconDst, &m_CtlRcScr, &Tv_Rc))
			{
				// draw icon
				GrP5PntReqImage(Tv_Gdib, Tv_Rc.left, Tv_Rc.top, Tv_PtrParaLblTimeView->IsSpriteImg);
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlDrawPnlAim(void)
{
	// local -------------------
	// code --------------------
		LcDrawAimBg(m_PtrCtlSkinPara, &m_CtlRcScr, 0);
}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawCtlPnlGrpBarLine(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoPnlGrpBarPara		Tv_PtrParaPnlGrpBar;

		__u32	Tv_GridIdx;
		St_GrRect	Tv_Rc;
	// code --------------------
		// init
		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdPnlGrpBar);
		Tv_PtrParaPnlGrpBar	=	(Ptr_GrP5UoPnlGrpBarPara)Tv_PtrHrcy->PtrPara;

		// grid
		Tv_Rc.left		=	m_CtlRcScr.left + (__s32)Tv_PtrParaPnlGrpBar->GridValWidth + (__s32)Tv_PtrParaPnlGrpBar->BoarderWidth;;
		Tv_Rc.right		=	m_CtlRcScr.right;
		Tv_Rc.top			=	m_CtlRcScr.bottom - (__s32)Tv_PtrParaPnlGrpBar->ItemNameHeight - (__s32)Tv_PtrParaPnlGrpBar->GridHeight;

		Tv_GridIdx		=	1;

		while(Tv_GridIdx < (__u32)Tv_PtrParaPnlGrpBar->GridCnt)
		{
			Tv_Rc.bottom	=	Tv_Rc.top + (__s32)Tv_PtrParaPnlGrpBar->GridLineWidth;
			GrP5PntReqFillRect(&Tv_Rc, Tv_PtrParaPnlGrpBar->ClrGrid);

			// next
			Tv_GridIdx ++;
			Tv_Rc.top		=	Tv_Rc.top - (__s32)Tv_PtrParaPnlGrpBar->GridHeight;
			if(E_GrP5SkinPnlGrpBarGridMaxCnt <= Tv_GridIdx)
			{
				break;
			}
		}

		// boarder
		// colum
		Tv_Rc.left		=	m_CtlRcScr.left + (__s32)Tv_PtrParaPnlGrpBar->GridValWidth;
		Tv_Rc.bottom	=	m_CtlRcScr.bottom - (__s32)Tv_PtrParaPnlGrpBar->ItemNameHeight;
		Tv_Rc.right		=	Tv_Rc.left + (__s32)Tv_PtrParaPnlGrpBar->BoarderWidth;
		Tv_Rc.top			=	Tv_Rc.bottom - ((__s32)Tv_PtrParaPnlGrpBar->GridHeight * (__s32)Tv_PtrParaPnlGrpBar->GridCnt);
		GrP5PntReqFillRect(&Tv_Rc, Tv_PtrParaPnlGrpBar->ClrBoarder);
		// row
		Tv_Rc.left		=	m_CtlRcScr.left + (__s32)Tv_PtrParaPnlGrpBar->GridValWidth + (__s32)Tv_PtrParaPnlGrpBar->BoarderWidth;
		Tv_Rc.bottom	=	m_CtlRcScr.bottom - (__s32)Tv_PtrParaPnlGrpBar->ItemNameHeight;
		Tv_Rc.right		=	m_CtlRcScr.right;
		Tv_Rc.top			=	Tv_Rc.bottom - (__s32)Tv_PtrParaPnlGrpBar->BoarderWidth;
		GrP5PntReqFillRect(&Tv_Rc, Tv_PtrParaPnlGrpBar->ClrBoarder);

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawCtlPnlGrpBarItemName(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoPnlGrpBarPara		Tv_PtrParaPnlGrpBar;
		Ptr_GrP5SkinCtlPnlGrpBar	Tv_PtrCtlPnlGrpBar;

		St_GrRect	Tv_Rc;
		__u32			Tv_Idx;
	// code --------------------
		// init
		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdPnlGrpBar);
		Tv_PtrParaPnlGrpBar	=	(Ptr_GrP5UoPnlGrpBarPara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlPnlGrpBar	=	(Ptr_GrP5SkinCtlPnlGrpBar)Tv_PtrHrcy->PtrSkinCtl;

		// check grid count
		if(0 == Tv_PtrCtlPnlGrpBar->ItemCnt)
		{
			return;
		}

		// init text cursor
		GrP5PntReqTxtCus();
		GrP5PntReqTxtStyle(&Tv_PtrParaPnlGrpBar->TxtStyleItem, Tv_PtrParaPnlGrpBar->FontItem);

		// grid width
		Tv_Rc.bottom	=	m_CtlRcScr.bottom;
		Tv_Rc.top			=	Tv_Rc.bottom - (__s32)Tv_PtrParaPnlGrpBar->ItemNameHeight;
		Tv_Rc.left		=	m_CtlRcScr.left + (__s32)Tv_PtrParaPnlGrpBar->GridValWidth;

		for(Tv_Idx = 0; Tv_Idx < Tv_PtrCtlPnlGrpBar->ItemCnt; Tv_Idx++)
		{
			Tv_Rc.right		=	Tv_Rc.left + Tv_PtrCtlPnlGrpBar->PntItmWidth;

			// draw text
			if(NULL != Tv_PtrCtlPnlGrpBar->Items[Tv_Idx].StrName)
			{
				GrP5PntReqTxtDraw(&Tv_Rc, Tv_PtrCtlPnlGrpBar->Items[Tv_Idx].StrName, 0);
			}

			// next
			Tv_Rc.left	=	Tv_Rc.left + (__s32)Tv_PtrCtlPnlGrpBar->PntItmWidth;
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawCtlPnlGrpBarGridVal(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoPnlGrpBarPara		Tv_PtrParaPnlGrpBar;
		Ptr_GrP5SkinCtlPnlGrpBar	Tv_PtrCtlPnlGrpBar;

		St_GrRect	Tv_Rc;
		__s32			Tv_Base;
		__u32			Tv_Idx;
	// code --------------------
		// init
		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdPnlGrpBar);
		Tv_PtrParaPnlGrpBar	=	(Ptr_GrP5UoPnlGrpBarPara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlPnlGrpBar	=	(Ptr_GrP5SkinCtlPnlGrpBar)Tv_PtrHrcy->PtrSkinCtl;

		// check grid count
		if(0 == Tv_PtrParaPnlGrpBar->GridCnt)
		{
			return;
		}

		// init text cursor
		GrP5PntReqTxtCus();
		GrP5PntReqTxtStyle(&Tv_PtrParaPnlGrpBar->TxtStyleGrid, Tv_PtrParaPnlGrpBar->FontGrid);

		// grid width
		Tv_Rc.left		=	m_CtlRcScr.left;
		Tv_Rc.right		=	m_CtlRcScr.left + (__s32)Tv_PtrParaPnlGrpBar->GridValWidth;
		Tv_Base				=	m_CtlRcScr.bottom - (__s32)Tv_PtrParaPnlGrpBar->ItemNameHeight;

		for(Tv_Idx = 0; Tv_Idx < (__u32)Tv_PtrParaPnlGrpBar->GridCnt; Tv_Idx++)
		{
			Tv_Rc.bottom	=	Tv_Base + (__s32)(Tv_PtrParaPnlGrpBar->GridValHeight >> 1);
			if(Tv_Rc.bottom >= m_CtlRcScr.bottom)
			{
				Tv_Rc.bottom		=	m_CtlRcScr.bottom;	// patch gird value bottom
			}
			Tv_Rc.top			=	Tv_Rc.bottom - (__s32)Tv_PtrParaPnlGrpBar->GridValHeight;

			// draw text
			if(NULL != Tv_PtrCtlPnlGrpBar->Grids[Tv_Idx].StrVal)
			{
				GrP5PntReqTxtDraw(&Tv_Rc, Tv_PtrCtlPnlGrpBar->Grids[Tv_Idx].StrVal, 0);
			}

			// next
			Tv_Base	=	Tv_Base - (__s32)Tv_PtrParaPnlGrpBar->GridHeight;

			if(E_GrP5SkinPnlGrpBarGridMaxCnt <= Tv_Idx)
			{
				break;
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawCtlPnlGrpBarBar(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoPnlGrpBarPara		Tv_PtrParaPnlGrpBar;
		Ptr_GrP5SkinCtlPnlGrpBar	Tv_PtrCtlPnlGrpBar;

		__u32	Tv_ItmIdx;

		__s32	Tv_AreaX;
		__s32	Tv_BarRelPosX;
		St_GrRect	Tv_RcBar;
		St_GrRect	Tv_RcVal;

		St_GrRect	Tv_RcRow;
	// code --------------------
		// init
		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdPnlGrpBar);
		Tv_PtrParaPnlGrpBar	=	(Ptr_GrP5UoPnlGrpBarPara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlPnlGrpBar	=	(Ptr_GrP5SkinCtlPnlGrpBar)Tv_PtrHrcy->PtrSkinCtl;

		if(0 == Tv_PtrCtlPnlGrpBar->ItemCnt)
		{
			return;
		}

		// init text cursor
		if(Tv_PtrParaPnlGrpBar->IsBarValView && (0 != Tv_PtrParaPnlGrpBar->BarValHeight))
		{
			GrP5PntReqTxtCus();
			GrP5PntReqTxtStyle(&Tv_PtrParaPnlGrpBar->TxtStyleVal, Tv_PtrParaPnlGrpBar->FontVal);
		}

		Tv_RcRow.top		=	m_CtlRcScr.bottom - (__s32)Tv_PtrParaPnlGrpBar->ItemNameHeight - (__s32)Tv_PtrParaPnlGrpBar->BoarderWidth;

		Tv_RcBar.bottom	=	Tv_RcRow.top;
		Tv_AreaX				=	m_CtlRcScr.left + (__s32)Tv_PtrParaPnlGrpBar->GridValWidth;
		Tv_BarRelPosX		=	(Tv_PtrCtlPnlGrpBar->PntItmWidth >> 1) - (__s32)(Tv_PtrParaPnlGrpBar->BarWidth >> 1);

		for(Tv_ItmIdx = 0; Tv_ItmIdx < Tv_PtrCtlPnlGrpBar->ItemCnt; Tv_ItmIdx++)
		{
			Tv_RcBar.left			=	Tv_AreaX + Tv_BarRelPosX;
			Tv_RcBar.right		=	Tv_RcBar.left + (__s32)Tv_PtrParaPnlGrpBar->BarWidth;
			Tv_RcBar.top			=	Tv_RcBar.bottom - (__s32)Tv_PtrCtlPnlGrpBar->Items[Tv_ItmIdx].BarHeight - (__s32)Tv_PtrParaPnlGrpBar->BoarderWidth;
			if(Tv_RcBar.top < m_CtlRcScr.top)
			{
				Tv_RcBar.top		=	m_CtlRcScr.top;
			}
			if(Tv_RcBar.top >= Tv_RcBar.bottom)
			{
				Tv_RcBar.top	=	Tv_RcBar.bottom - 1;
			}

			GrP5PntReqFillRect(&Tv_RcBar, Tv_PtrCtlPnlGrpBar->Items[Tv_ItmIdx].Color);

			// draw bar value
			if(Tv_PtrParaPnlGrpBar->IsBarValView && (0 != Tv_PtrParaPnlGrpBar->BarValHeight))
			{
				Tv_RcVal.left			=	Tv_AreaX;
				Tv_RcVal.bottom		=	Tv_RcBar.top;
				Tv_RcVal.top			=	Tv_RcVal.bottom - (__s32)Tv_PtrParaPnlGrpBar->BarValHeight;
				if(Tv_RcVal.top < m_CtlRcScr.top)
				{
					Tv_RcVal.top		=	m_CtlRcScr.top;
					Tv_RcVal.bottom	=	Tv_RcVal.top + (__s32)Tv_PtrParaPnlGrpBar->BarValHeight;
					if(Tv_RcVal.bottom > Tv_RcRow.top)
					{
						Tv_RcVal.bottom	=	Tv_RcRow.top;
					}
				}
				Tv_RcVal.right		=	Tv_RcVal.left + Tv_PtrCtlPnlGrpBar->PntItmWidth;
				if(NULL != Tv_PtrCtlPnlGrpBar->Items[Tv_ItmIdx].StrVal)
				{
					GrP5PntReqTxtDraw(&Tv_RcVal, Tv_PtrCtlPnlGrpBar->Items[Tv_ItmIdx].StrVal, 0);
				}
			}

			// next
			Tv_AreaX				=	Tv_AreaX + Tv_PtrCtlPnlGrpBar->PntItmWidth;
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlDrawPnlGrpBar(void)
{
	// local -------------------
	// code --------------------

		LcDrawCtlPnlGrpBarLine();

		LcDrawCtlPnlGrpBarItemName();

		LcDrawCtlPnlGrpBarGridVal();

		LcDrawCtlPnlGrpBarBar();

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawCtlPnlGrpLineLine(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoPnlGrpLinePara		Tv_PtrParaPnlGrpLine;
		Ptr_GrP5SkinCtlPnlGrpLine	Tv_PtrCtlPnlGrpLine;

		__u32	Tv_GridIdx;
		St_GrRect	Tv_Rc;
		St_GrRect	Tv_RcCol;
		St_GrRect	Tv_RcRow;
	// code --------------------
		// init
		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdPnlGrpLine);
		Tv_PtrParaPnlGrpLine	=	(Ptr_GrP5UoPnlGrpLinePara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlPnlGrpLine		=	(Ptr_GrP5SkinCtlPnlGrpLine)Tv_PtrHrcy->PtrSkinCtl;

		// calculate rect
		// boarder colum
		Tv_RcCol.left			=	m_CtlRcScr.left + (__s32)Tv_PtrParaPnlGrpLine->GridValWidth;
		Tv_RcCol.bottom		=	m_CtlRcScr.bottom - (__s32)Tv_PtrParaPnlGrpLine->ColNameHeight;
		Tv_RcCol.right		=	Tv_RcCol.left + (__s32)Tv_PtrParaPnlGrpLine->BoarderWidth;
		Tv_RcCol.top			=	Tv_RcCol.bottom - ((__s32)Tv_PtrParaPnlGrpLine->GridHeight * (__s32)Tv_PtrParaPnlGrpLine->GridCnt);
		if(Tv_RcCol.top <= m_CtlRcScr.top)
		{
			Tv_RcCol.top	=	m_CtlRcScr.top;
		}

		// boarder row
		Tv_RcRow.left			=	m_CtlRcScr.left + (__s32)Tv_PtrParaPnlGrpLine->GridValWidth + (__s32)Tv_PtrParaPnlGrpLine->BoarderWidth;
		Tv_RcRow.bottom		=	m_CtlRcScr.bottom - (__s32)Tv_PtrParaPnlGrpLine->ColNameHeight;
		Tv_RcRow.right		=	m_CtlRcScr.right;
		Tv_RcRow.top			=	Tv_RcRow.bottom - (__s32)Tv_PtrParaPnlGrpLine->BoarderWidth;


		// grid
		Tv_Rc.left		=	Tv_RcCol.right;
		Tv_Rc.right		=	Tv_RcRow.right;
		Tv_Rc.top			=	m_CtlRcScr.bottom - (__s32)Tv_PtrParaPnlGrpLine->ColNameHeight - (__s32)Tv_PtrParaPnlGrpLine->GridHeight;

		Tv_GridIdx		=	1;

		while(Tv_GridIdx < (__u32)Tv_PtrParaPnlGrpLine->GridCnt)
		{
			Tv_Rc.bottom	=	Tv_Rc.top + (__s32)Tv_PtrParaPnlGrpLine->GridLineWidth;
			GrP5PntReqFillRect(&Tv_Rc, Tv_PtrParaPnlGrpLine->ClrGrid);

			// next
			Tv_GridIdx ++;
			Tv_Rc.top		=	Tv_Rc.top - (__s32)Tv_PtrParaPnlGrpLine->GridHeight;
			if(E_GrP5SkinPnlGrpLineGridMaxCnt <= Tv_GridIdx)
			{
				break;
			}
		}

		// boarder
		// colum
		GrP5PntReqFillRect(&Tv_RcCol, Tv_PtrParaPnlGrpLine->ClrBoarder);
		// row
		GrP5PntReqFillRect(&Tv_RcRow, Tv_PtrParaPnlGrpLine->ClrBoarder);

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawCtlPnlGrpLineColName(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoPnlGrpLinePara		Tv_PtrParaPnlGrpLine;
		Ptr_GrP5SkinCtlPnlGrpLine	Tv_PtrCtlPnlGrpLine;

		St_GrRect	Tv_Rc;
		__u32			Tv_Idx;

	// code --------------------
		// init
		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdPnlGrpLine);
		Tv_PtrParaPnlGrpLine	=	(Ptr_GrP5UoPnlGrpLinePara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlPnlGrpLine		=	(Ptr_GrP5SkinCtlPnlGrpLine)Tv_PtrHrcy->PtrSkinCtl;

		// check colum count
		if(0 == Tv_PtrCtlPnlGrpLine->ColCnt)
		{
			return;
		}

		// init text cursor
		GrP5PntReqTxtCus();
		GrP5PntReqTxtStyle(&Tv_PtrParaPnlGrpLine->TxtStyleCol, Tv_PtrParaPnlGrpLine->FontCol);

		// grid width
		Tv_Rc.bottom	=	m_CtlRcScr.bottom;
		Tv_Rc.top			=	Tv_Rc.bottom - (__s32)Tv_PtrParaPnlGrpLine->ColNameHeight;
		Tv_Rc.left		=	m_CtlRcScr.left + (__s32)Tv_PtrParaPnlGrpLine->GridValWidth;

		for(Tv_Idx = 0; Tv_Idx < Tv_PtrCtlPnlGrpLine->ColCnt; Tv_Idx++)
		{
			Tv_Rc.right		=	Tv_Rc.left + Tv_PtrCtlPnlGrpLine->PntColWidth;

			// draw text
			if(NULL != Tv_PtrCtlPnlGrpLine->Cols[Tv_Idx].StrName)
			{
				GrP5PntReqTxtDraw(&Tv_Rc, Tv_PtrCtlPnlGrpLine->Cols[Tv_Idx].StrName, 0);
			}

			// next
			Tv_Rc.left		=	Tv_Rc.left + (__s32)Tv_PtrCtlPnlGrpLine->PntColWidth;
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawCtlPnlGrpLineGridVal(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoPnlGrpLinePara		Tv_PtrParaPnlGrpLine;
		Ptr_GrP5SkinCtlPnlGrpLine	Tv_PtrCtlPnlGrpLine;

		St_GrRect	Tv_Rc;
		__s32			Tv_Base;
		__u32			Tv_Idx;

	// code --------------------
		// init
		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdPnlGrpLine);
		Tv_PtrParaPnlGrpLine	=	(Ptr_GrP5UoPnlGrpLinePara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlPnlGrpLine		=	(Ptr_GrP5SkinCtlPnlGrpLine)Tv_PtrHrcy->PtrSkinCtl;

		// check grid count
		if(0 == Tv_PtrParaPnlGrpLine->GridCnt)
		{
			return;
		}

		// init text cursor
		GrP5PntReqTxtCus();
		GrP5PntReqTxtStyle(&Tv_PtrParaPnlGrpLine->TxtStyleGrid, Tv_PtrParaPnlGrpLine->FontGrid);

		// grid width
		Tv_Rc.left		=	m_CtlRcScr.left;
		Tv_Rc.right		=	m_CtlRcScr.left + (__s32)Tv_PtrParaPnlGrpLine->GridValWidth;
		Tv_Base				=	m_CtlRcScr.bottom - (__s32)Tv_PtrParaPnlGrpLine->ColNameHeight;

		for(Tv_Idx = 0; Tv_Idx < (__u32)Tv_PtrParaPnlGrpLine->GridCnt; Tv_Idx++)
		{
			Tv_Rc.bottom	=	Tv_Base + (__s32)(Tv_PtrParaPnlGrpLine->GridValHeight >> 1);
			if(Tv_Rc.bottom >= m_CtlRcScr.bottom)
			{
				Tv_Rc.bottom		=	m_CtlRcScr.bottom;	// patch gird value bottom
			}
			Tv_Rc.top			=	Tv_Rc.bottom - (__s32)Tv_PtrParaPnlGrpLine->GridValHeight;

			// draw text
			if(NULL != Tv_PtrCtlPnlGrpLine->Grids[Tv_Idx].StrVal)
			{
				GrP5PntReqTxtDraw(&Tv_Rc, Tv_PtrCtlPnlGrpLine->Grids[Tv_Idx].StrVal, 0);
			}

			// next
			Tv_Base	=	Tv_Base - (__s32)Tv_PtrParaPnlGrpLine->GridHeight;

			if(E_GrP5SkinPnlGrpLineGridMaxCnt <= Tv_Idx)
			{
				break;
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawCtlPnlGrpLineGraph(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoPnlGrpLinePara		Tv_PtrParaPnlGrpLine;
		Ptr_GrP5SkinCtlPnlGrpLine	Tv_PtrCtlPnlGrpLine;

		__u32	Tv_GrpIdx;
		__u32	Tv_ColIdx;
		__u8	Tv_LinIdx;
		__s32	Tv_PosYst;
		__s32	Tv_PosYed;

		__s32	Tv_RowTop;
		__s32	Tv_RowBtm;
	// code --------------------
		// init
		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdPnlGrpLine);
		Tv_PtrParaPnlGrpLine	=	(Ptr_GrP5UoPnlGrpLinePara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlPnlGrpLine		=	(Ptr_GrP5SkinCtlPnlGrpLine)Tv_PtrHrcy->PtrSkinCtl;

		if((0 == Tv_PtrCtlPnlGrpLine->GrpCnt) || (0 == Tv_PtrCtlPnlGrpLine->ColCnt) || (0 == Tv_PtrParaPnlGrpLine->GrpWidth))
		{
			return;
		}

		Tv_RowBtm		=	m_CtlRcScr.bottom - (__s32)Tv_PtrParaPnlGrpLine->ColNameHeight - (__s32)Tv_PtrParaPnlGrpLine->GrpWidth;
		Tv_RowTop		=	m_CtlRcScr.top;

		for(Tv_GrpIdx = 0; Tv_GrpIdx < Tv_PtrCtlPnlGrpLine->GrpCnt; Tv_GrpIdx++)
		{
			// check draw able
			if(!Tv_PtrCtlPnlGrpLine->Grps[Tv_GrpIdx].IsDraw)
			{
				continue;
			}

			// draw graph

			for(Tv_ColIdx = 1; Tv_ColIdx < Tv_PtrCtlPnlGrpLine->ColCnt; Tv_ColIdx++)
			{
				Tv_PosYst		=	Tv_RowBtm - Tv_PtrCtlPnlGrpLine->Grps[Tv_GrpIdx].Cols[Tv_ColIdx - 1].Pos;
				Tv_PosYed		=	Tv_RowBtm - Tv_PtrCtlPnlGrpLine->Grps[Tv_GrpIdx].Cols[Tv_ColIdx].Pos;
				if(Tv_PosYst < Tv_RowTop)
				{
					Tv_PosYst		=	Tv_RowTop;
				}
				if(Tv_PosYed < Tv_RowTop)
				{
					Tv_PosYed		=	Tv_RowTop;
				}

				// draw line
				for(Tv_LinIdx = 0; Tv_LinIdx < Tv_PtrParaPnlGrpLine->GrpWidth; Tv_LinIdx++)
				{
					GrP5PntReqLine(Tv_PtrCtlPnlGrpLine->Cols[Tv_ColIdx - 1].Pos, Tv_PosYst,
						Tv_PtrCtlPnlGrpLine->Cols[Tv_ColIdx].Pos, Tv_PosYed, Tv_PtrParaPnlGrpLine->ClrGraph[Tv_GrpIdx]);
					// next
					Tv_PosYst ++;
					Tv_PosYed ++;
				}
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlDrawPnlGrpLine(void)
{
	// local -------------------
	// code --------------------

		LcDrawCtlPnlGrpLineLine();
		LcDrawCtlPnlGrpLineColName();
		LcDrawCtlPnlGrpLineGridVal();
		LcDrawCtlPnlGrpLineGraph();

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlDrawPnlStd(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoPnlStdPara		Tv_PtrParaPnlStd;

		St_GrRect	Tv_RcPnt;
		Ptr_GrGdib	Tv_PtrGdib;
	// code --------------------
		// init
		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdPnlStd);
		Tv_PtrParaPnlStd	=	(Ptr_GrP5UoPnlStdPara)Tv_PtrHrcy->PtrPara;

		// background 
		if(Tv_PtrParaPnlStd->IsDrawBg)
		{
			GrP5PntArrangeByRect(&m_CtlRcScr);
			GrP5PntReqFillRect(&m_CtlRcScr, Tv_PtrParaPnlStd->Color);
		}

		// draw image
		Tv_PtrGdib	=	GrP5ImageGet(Tv_PtrParaPnlStd->ImgId);
		// check exist text
		if(NULL != Tv_PtrGdib)
		{
			if(GrP5RelRectToAbsRect(&Tv_PtrParaPnlStd->ImgRc, &m_CtlRcScr, &Tv_RcPnt))
			{
				ReqPntGdib(Tv_PtrGdib, &Tv_RcPnt, Tv_PtrParaPnlStd->ImgAlignHz, Tv_PtrParaPnlStd->ImgAlignVt, Tv_PtrParaPnlStd->IsSprite);
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlDrawPrgsBase(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoPrgsBasePara	Tv_PtrParaPrgsBase;
		Ptr_GrP5SkinCtlPrgsBase	Tv_PtrCtlPrgsBase;

	// code --------------------
		// init
		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdPrgsBase);
		Tv_PtrParaPrgsBase	=	(Ptr_GrP5UoPrgsBasePara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlPrgsBase		=	(Ptr_GrP5SkinCtlPrgsBase)Tv_PtrHrcy->PtrSkinCtl;

		LcDrawPrgsBg(m_PtrCtlSkinPara, &m_CtlRcScr, Tv_PtrParaPrgsBase->Direction, Tv_PtrCtlPrgsBase->ValMax, Tv_PtrCtlPrgsBase->ValNow);

		// draw text
		if(Tv_PtrParaPrgsBase->IsDrawTxt && (NULL != Tv_PtrCtlPrgsBase->StrTxt))
		{
			// build text
			GrP5PntReqTxtStyle(&Tv_PtrParaPrgsBase->TxtPara, Tv_PtrParaPrgsBase->FontId);
			GrP5PntReqTxtCus();
			GrP5PntReqTxtDraw(&m_CtlRcScr, Tv_PtrCtlPrgsBase->StrTxt, 0);		// txt buf ok
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawCtlQuadT1ViewArea(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoQuadBasePara	Tv_PtrParaQuadBase;

		Ptr_GrP5UoQuadT1Para		Tv_PtrParaQuadT1;
		Ptr_GrP5SkinCtlQuadT1		Tv_PtrCtlQuadT1;

		__u32	Tv_VwIdx;
		__s8	Tv_Ch;
		WCHAR*	Tv_Str;
		Ptr_GrGdib	Tv_PtrGdib;
		__u32	Tv_ChMask;
		__u16	Tv_ImgId;
		__u8	Tv_Ratb;

		__u8	Tv_ConIdx;
	// code --------------------
		// init
		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdQuadBase);
		Tv_PtrParaQuadBase	=	(Ptr_GrP5UoQuadBasePara)Tv_PtrHrcy->PtrPara;

		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdQuadT1);
		Tv_PtrParaQuadT1		=	(Ptr_GrP5UoQuadT1Para)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlQuadT1			=	(Ptr_GrP5SkinCtlQuadT1)Tv_PtrHrcy->PtrSkinCtl;

		// get view count
		if(0 == Tv_PtrCtlQuadT1->ViewCnt)
		{
			return;
		}

		Tv_ConIdx		=	((Cls_GrP5UoBase*)m_CtlUo)->ConsoleIdxGet();

		for(Tv_VwIdx = 0; Tv_VwIdx < Tv_PtrCtlQuadT1->ViewCnt; Tv_VwIdx++)
		{
			// draw internal rect
			GrP5PntReqFillRect(&Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcInArea, 0);

			Tv_Ch	=	Tv_PtrCtlQuadT1->Views[Tv_VwIdx].Ch;
			if(0 > Tv_Ch)
			{
				continue;
			}
			// draw
			Tv_ChMask	=	1 << (__u32)Tv_Ch;

			// draw channel name
			if(Tv_PtrCtlQuadT1->IsShowName)
			{
				Tv_Str	=	((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[Tv_ConIdx].ObjCon)->QuadChNameGet((__u8)Tv_Ch);
				if(NULL != Tv_Str)
				{
					if(0 < Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcName.right)
					{
						// set font
						GrP5PntReqTxtStyle(&Tv_PtrParaQuadT1->NameTpara, Tv_PtrParaQuadT1->NameFontId);
						// draw text
						GrP5PntReqTxtCus();
						GrP5PntReqTxtDraw(&Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcName, Tv_Str, 0);		// txt buf ok
					}
				}
			}

			if(Tv_PtrCtlQuadT1->IsShowRec)
			{
				// draw record do
				
				if(0 < Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcIconRec.right)
				{
					if(0 != (Tv_ChMask & Tv_PtrCtlQuadT1->RecChMap))
					{
						Tv_Ratb			=	((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[Tv_ConIdx].ObjCon)->QuadChRecTypeGet((__u8)Tv_Ch);

						if(0 != Tv_Ratb)
						{
							Tv_ImgId		=	Tv_PtrParaQuadT1->RecDoImg;
							if(Tv_PtrCtlQuadT1->IsRecEvtShow)
							{
								if(0 != (E_GrDvrRecAtbMotion & Tv_Ratb))
								{
									Tv_ImgId		=	Tv_PtrParaQuadT1->RmtnImg;
								}
								if(0 != (E_GrDvrRecAtbSensor & Tv_Ratb))
								{
									Tv_ImgId		=	Tv_PtrParaQuadT1->RsenImg;
								}
							}
							Tv_PtrGdib	=	GrP5ImageGet(Tv_ImgId);
							if(NULL != Tv_PtrGdib)
							{
								GrP5PntReqImage(Tv_PtrGdib, Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcIconRec.left, Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcIconRec.top,
									TRUE);
							}
						}
					}
				}
			}

			if(Tv_PtrCtlQuadT1->IsShowEvt)
			{
				// motion
				if(0 < Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcIconMtn.right)
				{
					if(0 != (Tv_ChMask & Tv_PtrCtlQuadT1->MtnChMap))
					{
						Tv_PtrGdib	=	GrP5ImageGet(Tv_PtrParaQuadT1->MtnImg);
						if(NULL != Tv_PtrGdib)
						{
							GrP5PntReqImage(Tv_PtrGdib, Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcIconMtn.left, Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcIconMtn.top,
								TRUE);
						}
					}
				}

				// sensor
				if(0 < Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcIconSen.right)
				{
					if(0 != (Tv_ChMask & Tv_PtrCtlQuadT1->SenChMap))
					{
						Tv_PtrGdib	=	GrP5ImageGet(Tv_PtrParaQuadT1->SenImg);
						if(NULL != Tv_PtrGdib)
						{
							GrP5PntReqImage(Tv_PtrGdib, Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcIconSen.left, Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcIconSen.top,
								TRUE);
						}
					}
				}

				// image analysis
				if(0 < Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcIconIa.right)
				{
					if(0 != (Tv_ChMask & Tv_PtrCtlQuadT1->IaChMap))
					{
						Tv_PtrGdib	=	GrP5ImageGet(Tv_PtrParaQuadT1->IaImg);
						if(NULL != Tv_PtrGdib)
						{
							GrP5PntReqImage(Tv_PtrGdib, Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcIconIa.left, Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcIconIa.top,
								TRUE);
						}
					}
				}
			}

			// draw ptz
			if(Tv_PtrCtlQuadT1->IsShowPtz)
			{
				if(0 < Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcIconPtz.right)
				{
					if(0 != (Tv_ChMask & Tv_PtrCtlQuadT1->PtzChMap))
					{
						Tv_PtrGdib	=	GrP5ImageGet(Tv_PtrParaQuadT1->PtzImg);
						if(NULL != Tv_PtrGdib)
						{
							GrP5PntReqImage(Tv_PtrGdib, Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcIconPtz.left, Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcIconPtz.top,
								TRUE);
						}
					}
				}
			}

			// draw audio
			if(Tv_PtrCtlQuadT1->IsShowAudio)
			{
				if(0 < Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcIconAdo.right)
				{
					if(0 != (Tv_ChMask & Tv_PtrCtlQuadT1->AdoChMap))
					{
						Tv_PtrGdib	=	GrP5ImageGet(Tv_PtrParaQuadT1->AdoImg);
						if(NULL != Tv_PtrGdib)
						{
							GrP5PntReqImage(Tv_PtrGdib, Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcIconAdo.left, Tv_PtrCtlQuadT1->Views[Tv_VwIdx].RcIconAdo.top,
								TRUE);
						}
					}
				}
			}


		}	//	for(Tv_VwIdx = 0; Tv_VwIdx < Tv_PtrCtlQuadT1->ViewCnt; Tv_VwIdx++)
}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawCtlQuadBaseInnerLine(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoQuadBasePara	Tv_PtrParaQuadBase;

		Ptr_GrRect	Tv_PtrRc;
		__u32	Tv_LnCnt;
		__u32	Tv_LnIdx;
		__u8	Tv_ConIdx;
	// code --------------------
		// init
		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdQuadBase);
		Tv_PtrParaQuadBase	=	(Ptr_GrP5UoQuadBasePara)Tv_PtrHrcy->PtrPara;

		Tv_ConIdx		=	((Cls_GrP5UoBase*)m_CtlUo)->ConsoleIdxGet();
		Tv_PtrRc	=	((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[Tv_ConIdx].ObjCon)->QuadLineRcPtrGet(&Tv_LnCnt);
		if((NULL != Tv_PtrRc) && (0 != Tv_LnCnt))
		{
			for(Tv_LnIdx = 0; Tv_LnIdx < Tv_LnCnt; Tv_LnIdx++)
			{
				GrP5PntReqFillRect(&Tv_PtrRc[Tv_LnIdx], Tv_PtrParaQuadBase->ClrLine);
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawCtlQuadBaseOutLine(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoQuadBasePara	Tv_PtrParaQuadBase;

		__u8	Tv_ConIdx;
		__u32	Tv_Width;
		St_GrRect	Tv_RcLn;
	// code --------------------
		// init
		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdQuadBase);
		Tv_PtrParaQuadBase	=	(Ptr_GrP5UoQuadBasePara)Tv_PtrHrcy->PtrPara;

		Tv_ConIdx		=	((Cls_GrP5UoBase*)m_CtlUo)->ConsoleIdxGet();

		// get line width
		Tv_Width	=	((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[Tv_ConIdx].ObjCon)->QuadLineWidthGet();
		if(0 == Tv_Width)
		{
			return;
		}

		// top
		Tv_RcLn.left		=	m_CtlRcScr.left;
		Tv_RcLn.top			=	m_CtlRcScr.top;
		Tv_RcLn.right		=	m_CtlRcScr.right;
		Tv_RcLn.bottom	=	m_CtlRcScr.top + (__s32)Tv_Width;
		GrP5PntReqFillRect(&Tv_RcLn, Tv_PtrParaQuadBase->ClrLine);

		// left
		Tv_RcLn.left		=	m_CtlRcScr.left;
		Tv_RcLn.top			=	m_CtlRcScr.top +  (__s32)Tv_Width;
		Tv_RcLn.right		=	m_CtlRcScr.left + (__s32)Tv_Width;
		Tv_RcLn.bottom	=	m_CtlRcScr.bottom - (__s32)Tv_Width;
		GrP5PntReqFillRect(&Tv_RcLn, Tv_PtrParaQuadBase->ClrLine);

		// right
		Tv_RcLn.left		=	m_CtlRcScr.right - (__s32)Tv_Width;
		Tv_RcLn.top			=	m_CtlRcScr.top + (__s32)Tv_Width;
		Tv_RcLn.right		=	m_CtlRcScr.right;
		Tv_RcLn.bottom	=	m_CtlRcScr.bottom - (__s32)Tv_Width;
		GrP5PntReqFillRect(&Tv_RcLn, Tv_PtrParaQuadBase->ClrLine);

		// bottom
		Tv_RcLn.left		=	m_CtlRcScr.left;
		Tv_RcLn.top			=	m_CtlRcScr.bottom - (__s32)Tv_Width;
		Tv_RcLn.right		=	m_CtlRcScr.right;
		Tv_RcLn.bottom	=	m_CtlRcScr.bottom;
		GrP5PntReqFillRect(&Tv_RcLn, Tv_PtrParaQuadBase->ClrLine);

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlDrawQuadT1(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoQuadBasePara	Tv_PtrParaQuadBase;

		Ptr_GrP5UoQuadT1Para		Tv_PtrParaQuadT1;
		Ptr_GrP5SkinCtlQuadT1		Tv_PtrCtlQuadT1;
	// code --------------------
		// init
		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdQuadBase);
		Tv_PtrParaQuadBase	=	(Ptr_GrP5UoQuadBasePara)Tv_PtrHrcy->PtrPara;

		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdQuadT1);
		Tv_PtrParaQuadT1		=	(Ptr_GrP5UoQuadT1Para)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlQuadT1			=	(Ptr_GrP5SkinCtlQuadT1)Tv_PtrHrcy->PtrSkinCtl;

		// draw view area
		LcDrawCtlQuadT1ViewArea();

		// draw inner line
		LcDrawCtlQuadBaseInnerLine();

		// darw outter line
		LcDrawCtlQuadBaseOutLine();

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlDrawSpinBase(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoSpinBasePara	Tv_PtrParaSpinBase;
		Ptr_GrP5SkinCtlSpinBase	Tv_PtrCtlSpinBase;

		__u32	Tv_BtnDecStat;
		__u32	Tv_BtnIncStat;
	// code --------------------
		// init
		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdSpinBase);
		Tv_PtrParaSpinBase	=	(Ptr_GrP5UoSpinBasePara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlSpinBase		=	(Ptr_GrP5SkinCtlSpinBase)Tv_PtrHrcy->PtrSkinCtl;

		// status update
		m_CtlStat			=	E_GrP5StatDisable;
		Tv_BtnDecStat	=	E_GrP5StatDisable;
		Tv_BtnIncStat	=	E_GrP5StatDisable;

		if( ((Cls_GrP5UoBase*)m_CtlUo)->IsCanEnable())
		{
			m_CtlStat			=	E_GrP5StatNormal;
			Tv_BtnDecStat	=	E_GrP5StatNormal;
			Tv_BtnIncStat	=	E_GrP5StatNormal;

			if(Tv_PtrCtlSpinBase->IsPush)
			{
				if(E_GrP5SkinSpinIdxBtnDec == Tv_PtrCtlSpinBase->ItmIdx)
				{
					Tv_BtnDecStat	=	E_GrP5StatPush;
				}
				else if(E_GrP5SkinSpinIdxBtnInc == Tv_PtrCtlSpinBase->ItmIdx)
				{
					Tv_BtnIncStat	=	E_GrP5StatPush;
				}
			}

			// check focus
			if(((Cls_GrP5UoBase*)m_CtlUo)->IsOnFocuse())
			{
				m_CtlStat		=	m_CtlStat | E_GrP5StatOnFocus;
				if(!Tv_PtrCtlSpinBase->IsPush)
				{
					if(E_GrP5SkinSpinIdxBtnDec == Tv_PtrCtlSpinBase->ItmIdx)
					{
						Tv_BtnDecStat	=	Tv_BtnDecStat | E_GrP5StatOnFocus;
					}
					else if(E_GrP5SkinSpinIdxBtnInc == Tv_PtrCtlSpinBase->ItmIdx)
					{
						Tv_BtnIncStat	=	Tv_BtnIncStat | E_GrP5StatOnFocus;
					}
				}
			}

			// select
			if(Tv_PtrParaSpinBase->IsKeyCover && (Tv_PtrCtlSpinBase->IsKeyCoverOn))
			{
				m_CtlStat		=	m_CtlStat | E_GrP5StatOnSelected;
			}
		}

		// draw
		// draw background
		if(Tv_PtrParaSpinBase->IsVertical)
		{
			LcDrawSpinVtBg(m_PtrCtlSkinPara, &m_CtlRcScr, m_CtlStat);

			if(0 < Tv_PtrCtlSpinBase->RcItems[E_GrP5SkinSpinIdxBtnDec].right)
			{
				LcDrawSpinVtBtnDec(m_PtrCtlSkinPara, &Tv_PtrCtlSpinBase->RcItems[E_GrP5SkinSpinIdxBtnDec], Tv_BtnDecStat);
			}
			if(0 < Tv_PtrCtlSpinBase->RcItems[E_GrP5SkinSpinIdxBtnInc].right)
			{
				LcDrawSpinVtBtnInc(m_PtrCtlSkinPara, &Tv_PtrCtlSpinBase->RcItems[E_GrP5SkinSpinIdxBtnInc], Tv_BtnIncStat);
			}
		}
		else
		{
			LcDrawSpinHzBg(m_PtrCtlSkinPara, &m_CtlRcScr, m_CtlStat);

			if(0 < Tv_PtrCtlSpinBase->RcItems[E_GrP5SkinSpinIdxBtnDec].right)
			{
				LcDrawSpinHzBtnDec(m_PtrCtlSkinPara, &Tv_PtrCtlSpinBase->RcItems[E_GrP5SkinSpinIdxBtnDec], Tv_BtnDecStat);
			}
			if(0 < Tv_PtrCtlSpinBase->RcItems[E_GrP5SkinSpinIdxBtnInc].right)
			{
				LcDrawSpinHzBtnInc(m_PtrCtlSkinPara, &Tv_PtrCtlSpinBase->RcItems[E_GrP5SkinSpinIdxBtnInc], Tv_BtnIncStat);
			}
		}

		// text draw
		if(NULL != Tv_PtrCtlSpinBase->StrDisp)
		{
			if(0 < Tv_PtrCtlSpinBase->RcText.right)
			{
				GrP5PntReqTxtStyle(&Tv_PtrParaSpinBase->TxtPara, Tv_PtrParaSpinBase->FontId);
				GrP5PntReqTxtCus();
				GrP5PntReqTxtDraw(&Tv_PtrCtlSpinBase->RcText, Tv_PtrCtlSpinBase->StrDisp, (__u16)m_CtlStat);
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlDrawTabBase(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoTabBasePara		Tv_PtrParaTabBase;
		Ptr_GrP5SkinCtlTabBase	Tv_PtrCtlTabBase;

	// code --------------------
		// init
		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdTabBase);
		Tv_PtrParaTabBase	=	(Ptr_GrP5UoTabBasePara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlTabBase		=	(Ptr_GrP5SkinCtlTabBase)Tv_PtrHrcy->PtrSkinCtl;

		if(Tv_PtrParaTabBase->IsContNoSkin)
		{
			return;
		}
		
		if(0 >= Tv_PtrCtlTabBase->RcCont.right)
		{
			return;
		}

		GrP5PntReqFillRect(&Tv_PtrCtlTabBase->RcCont, Tv_PtrParaTabBase->ContClr);

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlDrawTabTiRel(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoTabTiRelPara	Tv_PtrParaTabTiRel;
		Ptr_GrP5SkinCtlTabTiRel	Tv_PtrCtlTabTiRel;

		__u16	Tv_ImgId;
		__u16	Tv_TxtId;
		WCHAR*	Tv_Str;
		Ptr_GrGdib	Tv_PtrImg;
		Cls_GrP5UoBase*	Tv_WinChild;
	// code --------------------
		// init
		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdTabTiRel);
		Tv_PtrParaTabTiRel	=	(Ptr_GrP5UoTabTiRelPara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlTabTiRel		=	(Ptr_GrP5SkinCtlTabTiRel)Tv_PtrHrcy->PtrSkinCtl;

		// base draw
		LcCtlDrawTabBase();

		// label background
		if(0 < Tv_PtrCtlTabTiRel->RcLblBg.right)
		{
			// draw skin
			LcDrawTabLblBg(m_PtrCtlSkinPara, &Tv_PtrCtlTabTiRel->RcLblBg, 0);
		}

#ifdef GR_P5_EDITOR
		{
			__u8	Tv_ConIdx;

			Tv_ConIdx	=	((Cls_GrP5UoBase*)m_CtlUo)->ConsoleIdxGet();

			if(((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[Tv_ConIdx].ObjCon)->DvlIsEdit())
			{
				// icon

				// label
				if(0 < Tv_PtrCtlTabTiRel->RcLblTxt.right)
				{
					// draw label  
					GrP5PntReqTxtStyle(&Tv_PtrParaTabTiRel->LblTxtPara, Tv_PtrParaTabTiRel->LblFontId);
					GrP5PntReqTxtCus();
					GrP5PntReqTxtDraw(&Tv_PtrCtlTabTiRel->RcLblTxt, V_Cls_GrP5UoTabTiRelStrLaebl, 0);
				}
				// child area

				return;
			}
		}
#endif

		Tv_WinChild	=	((Cls_GrP5UoTabBase*)m_CtlUo)->ChildWinGet();

		if(NULL != Tv_WinChild)
		{
			// icon
			Tv_ImgId	=	(__u16)Tv_WinChild->UserDataGet(2);
			if(0 < Tv_PtrCtlTabTiRel->RcIcon.right)
			{
				Tv_PtrImg	=	GrP5ImageGet(Tv_ImgId);
				if(NULL != Tv_PtrImg)
				{
					ReqPntGdib(Tv_PtrImg, &Tv_PtrCtlTabTiRel->RcIcon, Tv_PtrParaTabTiRel->IconAlignHz, Tv_PtrParaTabTiRel->IconAlignVt, 
						Tv_PtrParaTabTiRel->IsIconSprite);
				}
			}

			// text
			Tv_TxtId	=	(__u16)Tv_WinChild->UserDataGet(3);
			Tv_Str		=	GrP5TextGet(Tv_TxtId);
			if(NULL != Tv_Str)
			{
				// label
				if(0 < Tv_PtrCtlTabTiRel->RcLblTxt.right)
				{
					// draw label
					GrP5PntReqTxtStyle(&Tv_PtrParaTabTiRel->LblTxtPara, Tv_PtrParaTabTiRel->LblFontId);
					GrP5PntReqTxtCus();
					GrP5PntReqTxtDraw(&Tv_PtrCtlTabTiRel->RcLblTxt, Tv_Str, 0);
				}
			}

		}


}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcDrawCtlTblBaseItems(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoTblBasePara		Tv_PtrParaTblBase;
		Ptr_GrP5SkinCtlTblBase	Tv_PtrCtlTblBase;

		__u32	Tv_LineIdx;
		__u8	Tv_ColIdx;
		St_GrRect	Tv_RcItm;

		St_GrRect	Tv_RcDraw;
		St_GrRect	Tv_RcTxt;
		WCHAR*	Tv_StrTxt;
		__u32	Tv_Stat;		// item status
		St_GrP5FontDrawStyle	Tv_TxtPara;
		Ptr_GrGdib	Tv_PtrGdib;

	// code --------------------
		// init
		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdTblBase);
		Tv_PtrParaTblBase		=	(Ptr_GrP5UoTblBasePara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlTblBase		=	(Ptr_GrP5SkinCtlTblBase)Tv_PtrHrcy->PtrSkinCtl;

		Tv_RcItm.top		=	m_CtlRcScr.top + E_GrP5SkinT1TblMargin;

		for(Tv_LineIdx = 0; Tv_LineIdx < Tv_PtrCtlTblBase->ViewLineMax; Tv_LineIdx++)
		{
			// check able
			if(Tv_PtrCtlTblBase->ViewLineCnt <= Tv_LineIdx)
			{
				break;
			}

			// calculate bottom
			Tv_RcItm.bottom	=	Tv_RcItm.top + (__s32)(__u32)Tv_PtrParaTblBase->ItemHeight;

			// colum display
			for(Tv_ColIdx = 0; Tv_ColIdx < Tv_PtrCtlTblBase->ColumCnt; Tv_ColIdx++)
			{
				// check finish
				if(0 == Tv_PtrCtlTblBase->Cols[Tv_ColIdx].Width)
				{
					break;
				}

				// calculate rect
				Tv_RcItm.left			=	Tv_PtrCtlTblBase->Cols[Tv_ColIdx].Pos;
				Tv_RcItm.right		=	Tv_RcItm.left + Tv_PtrCtlTblBase->Cols[Tv_ColIdx].Width;

				// item back ground

				// paint item
				Tv_RcDraw.left		=	Tv_RcItm.left + E_GrP5SkinT1TblItemMgnLeft;
				Tv_RcDraw.top			=	Tv_RcItm.top + E_GrP5SkinT1TblItemMgnTop;
				Tv_RcDraw.right		=	Tv_RcItm.right - E_GrP5SkinT1TblItemMgnRight;
				Tv_RcDraw.bottom	=	Tv_RcItm.bottom - E_GrP5SkinT1TblItemMgnDown;

				if(Tv_RcDraw.left > Tv_RcDraw.right)
				{
					continue;
				}

				// get item status
				Tv_Stat			=	0;
				if((Tv_PtrCtlTblBase->CusY == (__s32)Tv_LineIdx) && (Tv_PtrParaTblBase->IsLineSel || (Tv_PtrCtlTblBase->CusX == (__s32)Tv_ColIdx)))
				{
					Tv_Stat	=	Tv_Stat | E_GrP5StatOnFocus;
				}
				if((Tv_PtrCtlTblBase->SelY == (__s32)Tv_LineIdx) && ((Tv_PtrParaTblBase->IsLineSel) || (Tv_PtrCtlTblBase->SelX == (__s32)Tv_ColIdx)))
				{
					Tv_Stat	=	Tv_Stat | E_GrP5StatOnSelected;
				}

				// draw image
				if(E_GrP5TblCtlImage == Tv_PtrCtlTblBase->Lines[Tv_LineIdx].Cols[Tv_ColIdx].Type)
				{
					Tv_PtrGdib	=	GrP5ImageGet(Tv_PtrCtlTblBase->Lines[Tv_LineIdx].Cols[Tv_ColIdx].RsrcId);
					if(NULL != Tv_PtrGdib)
					{
						ReqPntGdib(Tv_PtrGdib, &Tv_RcDraw, Tv_PtrCtlTblBase->Lines[Tv_LineIdx].Cols[Tv_ColIdx].AlignHz,
							Tv_PtrCtlTblBase->Lines[Tv_LineIdx].Cols[Tv_ColIdx].AlignVt, 
							(BOOL8)Tv_PtrCtlTblBase->Lines[Tv_LineIdx].Cols[Tv_ColIdx].Para);
					}
				}

				// draw text
				Tv_StrTxt	=	NULL;
				switch(Tv_PtrCtlTblBase->Lines[Tv_LineIdx].Cols[Tv_ColIdx].Type)
				{
					case E_GrP5TblCtlTextLocal:
					case E_GrP5TblCtlValue:
						Tv_StrTxt	=	Tv_PtrCtlTblBase->Lines[Tv_LineIdx].Cols[Tv_ColIdx].Str;
						break;
					case E_GrP5TblCtlTextId:
						Tv_StrTxt	=	GrP5TextGet(Tv_PtrCtlTblBase->Lines[Tv_LineIdx].Cols[Tv_ColIdx].RsrcId);
						break;
				}
#ifdef GR_P5_EDITOR
				Tv_StrTxt	=	L"TABLE";
#endif

				Tv_RcTxt.left		=	Tv_RcDraw.left + (__s32)(__u32)Tv_PtrParaTblBase->TxtMgnX[Tv_ColIdx];
				Tv_RcTxt.top		=	Tv_RcDraw.top + (__s32)(__u32)Tv_PtrParaTblBase->TxtMgnY[Tv_ColIdx];
				Tv_RcTxt.right	=	Tv_RcDraw.right - (__s32)(__u32)Tv_PtrParaTblBase->TxtMgnX[Tv_ColIdx];
				Tv_RcTxt.bottom	=	Tv_RcDraw.bottom - (__s32)(__u32)Tv_PtrParaTblBase->TxtMgnY[Tv_ColIdx];

				if((Tv_RcTxt.left >= Tv_RcTxt.right) || (Tv_RcTxt.top >= Tv_RcTxt.bottom))
				{
					Tv_StrTxt		=	NULL;
				}

				if(NULL != Tv_StrTxt)
				{
					GrDumyCopyMem(&Tv_TxtPara, &Tv_PtrParaTblBase->TxtPara, sizeof(Tv_TxtPara));
					Tv_TxtPara.AlignHz	=	Tv_PtrCtlTblBase->Lines[Tv_LineIdx].Cols[Tv_ColIdx].AlignHz;
					Tv_TxtPara.AlignVt	=	Tv_PtrCtlTblBase->Lines[Tv_LineIdx].Cols[Tv_ColIdx].AlignVt;
					Tv_TxtPara.GapChar	=	Tv_PtrParaTblBase->CharPitch[Tv_ColIdx];
					if(Tv_PtrParaTblBase->IsFixPitch[Tv_ColIdx])
					{
						Tv_TxtPara.Atb		=	Tv_TxtPara.Atb | E_GrP5FontAtbFixPitch;
					}
					else
					{
						Tv_TxtPara.Atb		=	Tv_TxtPara.Atb & (~E_GrP5FontAtbFixPitch);
					}
					GrP5PntReqTxtStyle(&Tv_TxtPara, Tv_PtrParaTblBase->FontId);
					GrP5PntReqTxtCus();
					GrP5PntReqTxtDraw(&Tv_RcTxt, Tv_StrTxt, (__u16)Tv_Stat);
				}
			}

			// draw cursor
#if 0
			// not use
			if(Tv_PtrParaTblBase->IsLineSel && (Tv_PtrCtlTblBase->CusY == (__s32)Tv_LineIdx))
			{
				// draw cursor
				Tv_RcDraw.left			=	m_CtlRcScr.left + E_GrP5SkinT1TblMargin;
				Tv_RcDraw.top				=	Tv_RcItm.top;
				Tv_RcDraw.right			=	m_CtlRcScr.right - E_GrP5SkinT1TblMargin;
				Tv_RcDraw.bottom		=	Tv_RcItm.bottom;

				// draw line cursor
			
			}
#endif

			// next
			Tv_RcItm.top			=	Tv_RcItm.bottom;
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlDrawTblBase(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoTblBasePara		Tv_PtrParaTblBase;
		Ptr_GrP5SkinCtlTblBase	Tv_PtrCtlTblBase;

	// code --------------------
		// init
		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdTblBase);
		Tv_PtrParaTblBase		=	(Ptr_GrP5UoTblBasePara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlTblBase		=	(Ptr_GrP5SkinCtlTblBase)Tv_PtrHrcy->PtrSkinCtl;

		// status
		m_CtlStat		=	0;
		if( ((Cls_GrP5UoBase*)m_CtlUo)->IsOnFocuse())
		{
			m_CtlStat=	m_CtlStat | E_GrP5StatOnFocus;
		}

		// background

		// contents
		if((0 == Tv_PtrCtlTblBase->ViewLineMax) || (0 == Tv_PtrCtlTblBase->ColumCnt))
		{
			return;
		}

		LcDrawCtlTblBaseItems();


}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlDrawWinBase(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5WinBasePara			Tv_PtrParaWinBase;

		St_GrRect	Tv_Rc;
	// code --------------------
		// init
		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdWinBase);
		Tv_PtrParaWinBase		=	(Ptr_GrP5WinBasePara)Tv_PtrHrcy->PtrPara;

		if(2 >= m_PtrCtlSkinPara->SubType)
		{
			GrP5PntArrangeByRect(&m_CtlRcScr);
		}

		if(0 == m_PtrCtlSkinPara->SubType)
		{
			// draw title bar
			LcDrawU0TitleBar(&m_CtlRcScr);

			// draw body
			Tv_Rc.left		=	m_CtlRcScr.left + 2;
			Tv_Rc.top			=	m_CtlRcScr.top	 + E_GrP5SkinT1WinTitleHeight;
			Tv_Rc.right		=	m_CtlRcScr.right - 2;
			Tv_Rc.bottom	=	m_CtlRcScr.bottom - E_GrP5SkinT1WinBtmHeight;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrWinFaceU0);

			// draw bottom
			LcDrawU0BtmBar(&m_CtlRcScr);

			// draw out line
			Tv_Rc.left		=	m_CtlRcScr.left;
			Tv_Rc.top			=	m_CtlRcScr.top;
			Tv_Rc.right		=	m_CtlRcScr.right;
			Tv_Rc.bottom	=	m_CtlRcScr.top + 2;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrWinLineDark);
			Tv_Rc.top			=	m_CtlRcScr.bottom - 2;
			Tv_Rc.bottom	=	m_CtlRcScr.bottom;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrWinLineDark);
			Tv_Rc.top			=	m_CtlRcScr.top + 2;
			Tv_Rc.right		=	m_CtlRcScr.left + 2;
			Tv_Rc.bottom	=	m_CtlRcScr.bottom - 2;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrWinLineDark);
			Tv_Rc.left		=	m_CtlRcScr.right - 2;
			Tv_Rc.right		=	m_CtlRcScr.right;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrWinLineDark);

		}
		else if(1 == m_PtrCtlSkinPara->SubType)
		{
			// draw title bar
			LcDrawU1TitleBar(&m_CtlRcScr);

			// draw body
			Tv_Rc.left		=	m_CtlRcScr.left + 2;
			Tv_Rc.top			=	m_CtlRcScr.top	 + 150;
			Tv_Rc.right		=	m_CtlRcScr.right - 2;
			Tv_Rc.bottom	=	m_CtlRcScr.bottom - 30;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrWinFaceU1);

			// draw bottom
			LcDrawU1BtmBar(&m_CtlRcScr);

			// draw out line
			Tv_Rc.left		=	m_CtlRcScr.left;
			Tv_Rc.top			=	m_CtlRcScr.top;
			Tv_Rc.right		=	m_CtlRcScr.right;
			Tv_Rc.bottom	=	m_CtlRcScr.top + 2;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrWinLineU1);
			Tv_Rc.top			=	m_CtlRcScr.bottom - 2;
			Tv_Rc.bottom	=	m_CtlRcScr.bottom;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrWinLineU1);
			Tv_Rc.top			=	m_CtlRcScr.top + 2;
			Tv_Rc.right		=	m_CtlRcScr.left + 2;
			Tv_Rc.bottom	=	m_CtlRcScr.bottom - 2;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrWinLineU1);
			Tv_Rc.left		=	m_CtlRcScr.right - 2;
			Tv_Rc.right		=	m_CtlRcScr.right;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrWinLineU1);
		}
		else if(2 == m_PtrCtlSkinPara->SubType)
		{
			GrP5PntReqFillRect(&m_CtlRcScr, m_PtrCtlSkinPara->ParaColor);
		}


}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT1::LcCtlDrawPnlDir(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoPnlDirPara		Tv_PtrParaPnlDir;
		Ptr_GrP5SkinCtlPnlDir		Tv_PtrCtlPnlDir;

		__u32		Tv_Clr;
	// code --------------------
		// init
		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdPnlDir);
		Tv_PtrParaPnlDir		=	(Ptr_GrP5UoPnlDirPara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlPnlDir			=	(Ptr_GrP5SkinCtlPnlDir)Tv_PtrHrcy->PtrSkinCtl;

#ifdef GR_P5_EDITOR
		{
			__u8	Tv_ConIdx;

			Tv_ConIdx	=	((Cls_GrP5UoBase*)m_CtlUo)->ConsoleIdxGet();
			if(((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[Tv_ConIdx].ObjCon)->DvlIsEdit())
			{
				Tv_PtrCtlPnlDir->StatLeft		=	E_GrP5StatOnFocus;
				Tv_PtrCtlPnlDir->StatUp			=	E_GrP5StatOnFocus;
				Tv_PtrCtlPnlDir->StatRight	=	E_GrP5StatOnFocus;
				Tv_PtrCtlPnlDir->StatDown		=	E_GrP5StatOnFocus;
			}
		}
#endif

		// left draw
		if(E_GrP5StatNormal != Tv_PtrCtlPnlDir->StatLeft)
		{
			Tv_Clr		=	m_ClrPnlDirNor;
			if(E_GrP5StatPush == Tv_PtrCtlPnlDir->StatLeft)
			{
				Tv_Clr		=	m_ClrPnlDirPush;
			}
			LcDrawTriangleIsosLeft(&Tv_PtrCtlPnlDir->RcLeft, Tv_Clr);
		}

		// right draw
		if(E_GrP5StatNormal != Tv_PtrCtlPnlDir->StatRight)
		{
			Tv_Clr		=	m_ClrPnlDirNor;
			if(E_GrP5StatPush == Tv_PtrCtlPnlDir->StatRight)
			{
				Tv_Clr		=	m_ClrPnlDirPush;
			}
			LcDrawTriangleIsosRight(&Tv_PtrCtlPnlDir->RcRight, Tv_Clr);
		}

		// up draw
		if(E_GrP5StatNormal != Tv_PtrCtlPnlDir->StatUp)
		{
			Tv_Clr		=	m_ClrPnlDirNor;
			if(E_GrP5StatPush == Tv_PtrCtlPnlDir->StatUp)
			{
				Tv_Clr		=	m_ClrPnlDirPush;
			}
			LcDrawTriangleIsosUp(&Tv_PtrCtlPnlDir->RcUp, Tv_Clr);
		}

		// down
		if(E_GrP5StatNormal != Tv_PtrCtlPnlDir->StatDown)
		{
			Tv_Clr		=	m_ClrPnlDirNor;
			if(E_GrP5StatPush == Tv_PtrCtlPnlDir->StatDown)
			{
				Tv_Clr		=	m_ClrPnlDirPush;
			}
			LcDrawTriangleIsosDown(&Tv_PtrCtlPnlDir->RcDown, Tv_Clr);
		}


}
//--------------------------------------------------------------------

