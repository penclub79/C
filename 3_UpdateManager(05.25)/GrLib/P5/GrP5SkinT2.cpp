/*
Platform 5 skin Type 2

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5SkinT2.h>

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
#include <P5/GrP5UoCbxBase.h>
#include <P5/GrP5UoCbxLtBase.h>
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
#include <P5/GrP5WinPopItemBase.h>
#include <P5/GrP5WinPopMenu.h>
#include <P5/GrP5UoGrpHzT2.h>


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

__u8	V_GrP5SkinT2Curb4x[4]	={4, 2, 1, 1};

__u8	V_GrP5SkinT2Curb6x[6]	={6, 4, 3, 2, 1, 1};

St_GrSwordRect	V_GrP5SkinT2ArwCrhOutTbl[]	=
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

St_GrSwordRect	V_GrP5SkinT2ArwStartTbl[]	=
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

St_GrSwordRect	V_GrP5SkinT2ArwCircle[16]	=
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

St_GrP5SkinT2VctCmd	V_GrP5SkinT2DirArwA0[]	=
{
	{E_GrP5SkinT2VctCmdRect, 0, {2, -1, 22, 3}},		//	0

	{E_GrP5SkinT2VctCmdLine, 0, {15, -4, 21, 2}},		//	1
	{E_GrP5SkinT2VctCmdLine, 0, {16, -4, 22, -1}},
	{E_GrP5SkinT2VctCmdLine, 0, {16, -5, 22, 1}},
	{E_GrP5SkinT2VctCmdLine, 0, {17, -5, 22, 0}},
	{E_GrP5SkinT2VctCmdLine, 0, {17, -6, 23, 0}},		//	3

	{E_GrP5SkinT2VctCmdLine, 0, {15, 5, 21, -1}},		//	4
	{E_GrP5SkinT2VctCmdLine, 0, {16, 5, 22, -1}},
	{E_GrP5SkinT2VctCmdLine, 0, {16, 6, 22, 0}},		//	5
	{E_GrP5SkinT2VctCmdLine, 0, {17, 6, 23, 0}},
	{E_GrP5SkinT2VctCmdLine, 0, {17, 7, 23, 1}},		//	6

	{E_GrP5SkinT2VctCmdEnd, },		//	6
};

St_GrP5SkinT2VctCmd	V_GrP5SkinT2DirArwA11[]	=
{
	{E_GrP5SkinT2VctCmdLine, 0, {2, -1, 21, -5}},
	{E_GrP5SkinT2VctCmdLine, 0, {2, 0, 21, -4}},
	{E_GrP5SkinT2VctCmdLine, 0, {2, 1, 21, -3}},
	{E_GrP5SkinT2VctCmdLine, 0, {3, 1, 21, -2}},
	{E_GrP5SkinT2VctCmdLine, 0, {3, 2, 22, -2}},

	{E_GrP5SkinT2VctCmdLine, 0, {14, -7, 22, -2}},
	{E_GrP5SkinT2VctCmdLine, 0, {15, -8, 22, -3}},
	{E_GrP5SkinT2VctCmdLine, 0, {15, -7, 21, -3}},
	{E_GrP5SkinT2VctCmdLine, 0, {15, -9, 22, -4}},

	{E_GrP5SkinT2VctCmdLine, 0, {16, 3, 21, -5}},
	{E_GrP5SkinT2VctCmdLine, 0, {17, 3, 21, -4}},
	{E_GrP5SkinT2VctCmdLine, 0, {18, 3, 22, -4}},
	{E_GrP5SkinT2VctCmdLine, 0, {18, 4, 23, -4}},

	{E_GrP5SkinT2VctCmdEnd, },
};

St_GrP5SkinT2VctCmd	V_GrP5SkinT2DirArwA23[]	=
{
	{E_GrP5SkinT2VctCmdLine, 0, {2, -1, 20, -9}},
	{E_GrP5SkinT2VctCmdLine, 0, {2, 0, 20, -8}},
	{E_GrP5SkinT2VctCmdLine, 0, {4, 0, 20, -7}},
	{E_GrP5SkinT2VctCmdLine, 0, {3, 1, 21, -7}},
	{E_GrP5SkinT2VctCmdLine, 0, {3, 2, 21, -6}},

	{E_GrP5SkinT2VctCmdLine, 0, {13, -9, 21, -6}},
	{E_GrP5SkinT2VctCmdLine, 0, {13, -10, 22, -7}},
	{E_GrP5SkinT2VctCmdLine, 0, {13, -11, 22, -8}},

	{E_GrP5SkinT2VctCmdLine, 0, {17, 1, 20, -8}},
	{E_GrP5SkinT2VctCmdLine, 0, {18, 1, 21, -8}},
	{E_GrP5SkinT2VctCmdLine, 0, {19, 1, 22, -7}},

	{E_GrP5SkinT2VctCmdEnd, },
};

St_GrP5SkinT2VctCmd	V_GrP5SkinT2DirArwA34[]	=
{
	{E_GrP5SkinT2VctCmdLine, 0, {1, -2, 17, -13}},
	{E_GrP5SkinT2VctCmdLine, 0, {2, -2, 10, -7}},
	{E_GrP5SkinT2VctCmdLine, 0, {7, -4, 17, -11}},
	{E_GrP5SkinT2VctCmdLine, 0, {2, -1, 17, -12}},
	{E_GrP5SkinT2VctCmdLine, 0, {2, 0, 18, -11}},
	{E_GrP5SkinT2VctCmdLine, 0, {3, 0, 18, -10}},
	{E_GrP5SkinT2VctCmdLine, 0, {3, 1, 19, -10}},

	{E_GrP5SkinT2VctCmdLine, 0, {10, -12, 19, -10}},
	{E_GrP5SkinT2VctCmdLine, 0, {10, -13, 19, -11}},
	{E_GrP5SkinT2VctCmdLine, 0, {10, -14, 19, -12}},

	{E_GrP5SkinT2VctCmdLine, 0, {15, -4, 17, -13}},
	{E_GrP5SkinT2VctCmdLine, 0, {16, -4, 18, -12}},
	{E_GrP5SkinT2VctCmdLine, 0, {17, -3, 19, -12}},

	{E_GrP5SkinT2VctCmdEnd, },
};

St_GrP5SkinT2VctCmd	V_GrP5SkinT2DirArwA45[]	=
{
	{E_GrP5SkinT2VctCmdLine, 0, {0, -1, 14, -15}},
	{E_GrP5SkinT2VctCmdLine, 0, {1, -1, 14, -14}},
	{E_GrP5SkinT2VctCmdLine, 0, {1, 0, 15, -14}},
	{E_GrP5SkinT2VctCmdLine, 0, {2, 0, 15, -13}},
	{E_GrP5SkinT2VctCmdLine, 0, {2, 1, 16, -13}},

	{E_GrP5SkinT2VctCmdRect, 0, {7, -15, 14, -12}},

	{E_GrP5SkinT2VctCmdRect, 0, {14, -15, 17, -5}},

	{E_GrP5SkinT2VctCmdEnd, },
};

St_GrP5SkinT2VctCmd	V_GrP5SkinT2DirArwA56[]	=
{
	{E_GrP5SkinT2VctCmdLine, 0, {0, -2, 11, -17}},
	{E_GrP5SkinT2VctCmdLine, 0, {1, -1, 12, -17}},
	{E_GrP5SkinT2VctCmdLine, 0, {2, -1, 13, -16}},
	{E_GrP5SkinT2VctCmdLine, 0, {3, 0, 13, -15}},

	{E_GrP5SkinT2VctCmdLine, 0, {1, -2, 10, -15}},
	{E_GrP5SkinT2VctCmdLine, 0, {6, -6, 11, -15}},
	{E_GrP5SkinT2VctCmdLine, 0, {6, -8, 7, -8}},
	{E_GrP5SkinT2VctCmdLine, 0, {3, -1, 12, -14}},

	{E_GrP5SkinT2VctCmdLine, 0, {4, -16, 13, -18}},
	{E_GrP5SkinT2VctCmdLine, 0, {4, -15, 13, -17}},
	{E_GrP5SkinT2VctCmdLine, 0, {5, -14, 14, -16}},

	{E_GrP5SkinT2VctCmdLine, 0, {11, -18, 13, -9}},
	{E_GrP5SkinT2VctCmdLine, 0, {12, -18, 14, -9}},
	{E_GrP5SkinT2VctCmdLine, 0, {13, -18, 15, -9}},

	{E_GrP5SkinT2VctCmdEnd, },

};

St_GrP5SkinT2VctCmd	V_GrP5SkinT2DirArwA68[]	=
{
	{E_GrP5SkinT2VctCmdLine, 0, {-1, -2, 6, -20}},
	{E_GrP5SkinT2VctCmdLine, 0, {0, -2, 7, -20}},
	{E_GrP5SkinT2VctCmdLine, 0, {1, -1, 8, -19}},
	{E_GrP5SkinT2VctCmdLine, 0, {2, -1, 9, -19}},

	{E_GrP5SkinT2VctCmdLine, 0, {1, -3, 7, -18}},

	{E_GrP5SkinT2VctCmdLine, 0, {0, -17, 8, -21}},
	{E_GrP5SkinT2VctCmdLine, 0, {1, -17, 9, -20}},
	{E_GrP5SkinT2VctCmdLine, 0, {1, -16, 9, -19}},

	{E_GrP5SkinT2VctCmdLine, 0, {7, -20, 10, -12}},
	{E_GrP5SkinT2VctCmdLine, 0, {8, -20, 11, -12}},
	{E_GrP5SkinT2VctCmdLine, 0, {8, -21, 12, -13}},

	{E_GrP5SkinT2VctCmdLine, 0, {11, -13, 10, -15}},

	{E_GrP5SkinT2VctCmdEnd, },

};

St_GrP5SkinT2VctCmd	V_GrP5SkinT2DirArwA79[]	=
{
	{E_GrP5SkinT2VctCmdLine, 0, {-1, -2, 3, -21}},
	{E_GrP5SkinT2VctCmdLine, 0, {0, -2, 4, -21}},
	{E_GrP5SkinT2VctCmdLine, 0, {1, -2, 5, -21}},
	{E_GrP5SkinT2VctCmdLine, 0, {2, -2, 6, -21}},

	{E_GrP5SkinT2VctCmdLine, 0, {-3, -18, 5, -23}},
	{E_GrP5SkinT2VctCmdLine, 0, {-2, -17, 5, -22}},
	{E_GrP5SkinT2VctCmdLine, 0, {-2, -16, 6, -21}},

	{E_GrP5SkinT2VctCmdLine, 0, {3, -22, 8, -14}},
	{E_GrP5SkinT2VctCmdLine, 0, {4, -22, 9, -15}},
	{E_GrP5SkinT2VctCmdLine, 0, {5, -23, 10, -15}},

	{E_GrP5SkinT2VctCmdLine, 0, {-1, -18, 7, -18}},
	{E_GrP5SkinT2VctCmdLine, 0, {8, -15, 7, -16}},

	{E_GrP5SkinT2VctCmdEnd, },

};

St_GrP5SkinT2VctCmd	V_GrP5SkinT2DirArwA90[]	=
{
	{E_GrP5SkinT2VctCmdRect, 0, {-1, -21, 3, -1}},

	{E_GrP5SkinT2VctCmdLine, 0, {-6, -16, 0, -22}},
	{E_GrP5SkinT2VctCmdLine, 0, {-5, -16, 1, -22}},
	{E_GrP5SkinT2VctCmdLine, 0, {-5, -15, 1, -21}},

	{E_GrP5SkinT2VctCmdLine, 0, {0, -21, 6, -15}},
	{E_GrP5SkinT2VctCmdLine, 0, {0, -22, 6, -16}},
	{E_GrP5SkinT2VctCmdLine, 0, {1, -22, 7, -16}},

	{E_GrP5SkinT2VctCmdEnd, },

};

WCHAR	V_GrP5SkinT2StrDayTbl[31][3]	=
{{'1', 0}, {'2', 0}, {'3', 0}, {'4', 0}, {'5', 0}, {'6', 0}, {'7', 0}, {'8', 0}, {'9', 0}, {'1', '0', 0},
{'1', '1', 0}, {'1', '2', 0}, {'1', '3', 0}, {'1', '4', 0}, {'1', '5', 0}, {'1', '6', 0}, {'1', '7', 0}, {'1', '8', 0}, {'1', '9', 0}, {'2', '0', 0},
{'2', '1', 0}, {'2', '2', 0}, {'2', '3', 0}, {'2', '4', 0}, {'2', '5', 0}, {'2', '6', 0}, {'2', '7', 0}, {'2', '8', 0}, {'2', '9', 0}, {'3', '0', 0},
{'3', '1', 0}
};

WCHAR	V_GrP5SkinT2StrRulerHrT1[5][3]	=
{{'0', 0}, {'6', 0}, {'1', '2', 0}, {'1', '8', 0}, {'2', '3', 0}, };

WCHAR	V_GrP5SkinT2StrRulerMinT1[7][3]	=
{{'0', 0}, {'1', '0', 0}, {'2', '0', 0}, {'3', '0', 0}, {'4', '0', 0}, {'5', '0', 0}, {'5', '9', 0}, };

__s32	V_GrP5SkinT2TitleT0DivVct[E_GrP5SkinT2TitleT0Height]	=
{0, 2, 2, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 2};

St_Grp5SkinT2GrpHzT2Rul	V_GrP5SkinT2GrpHzT2Rul[E_Grp5SkinT2GrpHzT2ZoomCnt]	=	
{{120,7200,60, 3600, 30},
{60, 3600, 60,3600, 30},
{30, 1800, 60,1800,60},
{10, 1800, 180,600,60},
{5, 600, 120,60,12},
{4, 600, 150,60,15},
{2, 300, 150,60,30},
{1, 120, 120,60,60},
};

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5SkinT2::Cls_GrP5SkinT2(__u8 A_SkinId) :
Cls_GrP5SkinBase(A_SkinId)
{
	// local -------------------
	// code --------------------
		LcPalleteInit();

}
//--------------------------------------------------------------------
Cls_GrP5SkinT2::~Cls_GrP5SkinT2(void)
{
	// local -------------------
	// code --------------------


}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcPalleteInit(void)
{
	// local -------------------
	__u32	Tv_PxFmt;
	// code --------------------
	Tv_PxFmt	=	((Cls_GrP5Mng*)V_GrP5Env.ObjMng)->PixelFormatGet();

	m_ClrWinT0Bg0						=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeRgb24(120, 120, 120) | 0xFF000000, Tv_PxFmt);
	m_ClrWinT0Bg1						=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeRgb24(80, 80, 80) | 0xFF000000, Tv_PxFmt);
	m_ClrWinT0Bg2						=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeRgb24(0, 0, 0) | 0xFF000000, Tv_PxFmt);


	m_ClrSbarBg							=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeRgb24(120, 120, 120) | 0xFF000000, Tv_PxFmt);
	m_ClrSbarTrack					=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeRgb24(80, 80, 80) | 0xFF000000, Tv_PxFmt);
	m_ClrSbarBtnNrm					=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeRgb24(80, 80, 80) | 0xFF000000, Tv_PxFmt);
	m_ClrSbarBtnFcs					=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeRgb24(90, 206, 255) | 0xFF000000, Tv_PxFmt);
	m_ClrSbarBtnPush				=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeRgb24(255, 255, 255) | 0xFF000000, Tv_PxFmt);

	m_ClrSbarThmNrm					=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeRgb24(120, 120, 120) | 0xFF000000, Tv_PxFmt);
	m_ClrSbarThmFcs					=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeRgb24(90, 206, 255) | 0xFF000000, Tv_PxFmt);
	m_ClrSbarThmPush				=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeRgb24(255, 255, 255) | 0xFF000000, Tv_PxFmt);
	
	m_ClrWinPopListWaku			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeRgb24(150, 150, 150) | 0xFF000000, Tv_PxFmt);
	m_ClrWinPopListBg				=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeRgb24(80, 80, 80) | 0xFF000000, Tv_PxFmt);
		
	m_ClrCbxBg							=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeRgb24(63, 63, 63) | 0xFF000000, Tv_PxFmt);

	m_ClrBtnT0PushLine			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 90, 206, 255), Tv_PxFmt);
	m_ClrBtnT0NrmBg					=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 63, 63, 63), Tv_PxFmt);
	m_ClrBtnT0DisBg					=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 45, 45, 45), Tv_PxFmt);
	m_ClrBtnT0FcsBg					=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 90, 206, 255), Tv_PxFmt);

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

	m_ClrSpinBgNormal			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 63, 63, 63), Tv_PxFmt);
	m_ClrSpinBgDisable		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 63, 63, 63), Tv_PxFmt);
	m_ClrSpinBgFocus			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 90, 206, 255), Tv_PxFmt);
	m_ClrSpinBgPush				=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 90, 206, 255), Tv_PxFmt);
	m_ClrSpinCtlMarkNormal		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 90, 90, 90), Tv_PxFmt);
	m_ClrSpinCtlMarkFocus			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 255, 255), Tv_PxFmt);
	m_ClrSpinCtlMarkPush			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 90, 90, 90), Tv_PxFmt);

	m_ClrLblBgLineDark		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 0, 0, 0), Tv_PxFmt);
	m_ClrLblBgLineLight		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 90, 90, 90), Tv_PxFmt);
	m_ClrLblBg						=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 255, 255), Tv_PxFmt);
	m_ClrLblMark					=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 0, 156, 206), Tv_PxFmt);

	m_ClrChkBgLineFocus		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 204, 51), Tv_PxFmt);
	m_ClrChkMarkBgStd			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 153, 153, 153), Tv_PxFmt);
	m_ClrChkMarkBgChk			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 159, 227, 225), Tv_PxFmt);
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

	m_ClrCalLine					=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 155, 155, 155), Tv_PxFmt);
	m_ClrCalCusNrm				=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 254, 205, 51), Tv_PxFmt);
	m_ClrCalMark					=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 236, 143, 68), Tv_PxFmt);
	m_ClrCalSel						=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 91, 250, 255), Tv_PxFmt);

	m_ClrRatbNone					=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 66, 66, 66), Tv_PxFmt);
	m_ClrRatbCont					=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 206, 255, 0), Tv_PxFmt);
	m_ClrRatbMntn					=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 57, 148, 255), Tv_PxFmt);
	m_ClrRatbSens					=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 74, 74), Tv_PxFmt);

	m_ClrGrpHzCusHz				=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 189, 0), Tv_PxFmt);
	m_ClrGrpHzCusVt				=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 189, 0), Tv_PxFmt);

	m_ClrAimCross					=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 63, 63, 63), Tv_PxFmt);

	m_ClrAglLight					=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 250, 219, 192), Tv_PxFmt);

	m_ClrGrpHzT1IndBg			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 230, 230, 0), Tv_PxFmt);
	m_ClrGrpHzT1BgFocus		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 133, 252, 254), Tv_PxFmt);

	m_ClrWinPopListOnCus	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 90, 206, 255), Tv_PxFmt);

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

	m_ClrGridRscdItm[0]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 60, 60, 60), Tv_PxFmt);		// none
	m_ClrGridRscdItm[1]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 0, 0, 255), Tv_PxFmt);			// continous
	m_ClrGridRscdItm[2]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 100, 255), Tv_PxFmt);	// motion
	m_ClrGridRscdItm[3]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 139, 69, 18), Tv_PxFmt);		// continouse + motion
	m_ClrGridRscdItm[4]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 255, 0), Tv_PxFmt);		// sensor
	m_ClrGridRscdItm[5]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 139, 69, 18), Tv_PxFmt);		// continouse + sensor
	m_ClrGridRscdItm[6]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 186, 85, 211), Tv_PxFmt);	// motion + sensor
	m_ClrGridRscdItm[7]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 139, 69, 18), Tv_PxFmt);		// continouse + motion + sensor
	
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

	m_ClrTablBorder		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 150, 150, 150), Tv_PxFmt);

	m_ClrTblNormal		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 120, 120, 120), Tv_PxFmt);
	m_ClrTblSelect		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 159, 227, 255), Tv_PxFmt);
	m_ClrTblBorder		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 45, 45, 45), Tv_PxFmt);

	m_ClrPnlDragCus		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 249, 137, 53), Tv_PxFmt);

	m_ClrGrpHzT2RulLine	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 62, 56, 60), Tv_PxFmt);
	m_ClrGrpHzT2Cus		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 249, 137, 53), Tv_PxFmt);

	m_ClrTooltipBg		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 91, 250, 255), Tv_PxFmt);

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcGradeintByTbl(Ptr_GrRect A_PtrRect, Ptr_GrP5SkinT2GradInfo A_PtrGrad)
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
		for(Tv_WkIdx = 0; Tv_WkIdx < A_PtrGrad->ItemCnt; Tv_WkIdx++)
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
void	Cls_GrP5SkinT2::LcDrawTabLblBg(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat)
{
	// local -------------------
	St_GrRect	Tv_RcDraw;
	// code --------------------
	Tv_RcDraw.left		=	A_PtrRect->left;
	Tv_RcDraw.top			=	A_PtrRect->bottom - E_GrP5SkinT2TabLblSepWidth;
	Tv_RcDraw.right		=	A_PtrRect->right;
	Tv_RcDraw.bottom	=	A_PtrRect->bottom;

	GrP5PntArrangeByRect(&Tv_RcDraw);

	GrP5PntReqFillRect(&Tv_RcDraw, m_ClrTabLblSepLight);
}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcDrawBtnSubType0(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat)
{
	// local -------------------
		St_GrRect	Tv_RcBg;
		St_GrRect	Tv_RcDraw;
		__u32			Tv_Clr;
		__s32			Tv_LimLeft;
		__s32			Tv_LimRight;
	// code --------------------

		// check able
		if((E_GrP5SkinT2BtnCutSize >= (A_PtrRect->right - A_PtrRect->left)) || (E_GrP5SkinT2BtnCutSize >= (A_PtrRect->bottom - A_PtrRect->top)))
		{
			return;
		}

		Tv_RcBg	=	*A_PtrRect;

		// check push
		if(E_GrP5StatPush == (E_GrP5StatConstMask & A_Stat))
		{
			// push mode
			// draw out line
			GrP5PntReqLine(A_PtrRect->left, A_PtrRect->top, A_PtrRect->right - E_GrP5SkinT2BtnCutSize, A_PtrRect->top, m_ClrBtnT0PushLine);
			GrP5PntReqLine(A_PtrRect->left + E_GrP5SkinT2BtnCutSize - 1, A_PtrRect->bottom - 1, A_PtrRect->right - 1, A_PtrRect->bottom - 1, m_ClrBtnT0PushLine);
			GrP5PntReqLine(A_PtrRect->left, A_PtrRect->top + 1, A_PtrRect->left, A_PtrRect->bottom - E_GrP5SkinT2BtnCutSize, m_ClrBtnT0PushLine);
			GrP5PntReqLine(A_PtrRect->right - 1, A_PtrRect->top + E_GrP5SkinT2BtnCutSize - 1, A_PtrRect->right - 1 , A_PtrRect->bottom - 2, m_ClrBtnT0PushLine);

			GrP5PntReqLine(A_PtrRect->left + 1, A_PtrRect->bottom - E_GrP5SkinT2BtnCutSize + 1, A_PtrRect->left + E_GrP5SkinT2BtnCutSize - 2, A_PtrRect->bottom - 2, m_ClrBtnT0PushLine);
			GrP5PntReqLine(A_PtrRect->right - E_GrP5SkinT2BtnCutSize + 1, A_PtrRect->top + 1, A_PtrRect->right - 2, A_PtrRect->top +E_GrP5SkinT2BtnCutSize - 2, m_ClrBtnT0PushLine);

			// patch bg
			Tv_RcBg.left ++;
			Tv_RcBg.top ++;
			Tv_RcBg.right --;
			Tv_RcBg.bottom --;
		}

		// check able
		if((Tv_RcBg.left >= Tv_RcBg.right) || (Tv_RcBg.top >= Tv_RcBg.bottom))
		{
			return;
		}

		// get background color
		Tv_Clr		=	m_ClrBtnT0DisBg;
		if(E_GrP5StatDisable != (E_GrP5StatConstMask & A_Stat))
		{
			Tv_Clr		=	m_ClrBtnT0NrmBg;
			if(0 != (E_GrP5StatOnFocus & A_Stat))
			{
				Tv_Clr		=	m_ClrBtnT0FcsBg;
			}
		}

		// calculate limite
		Tv_LimLeft	=	A_PtrRect->bottom - E_GrP5SkinT2BtnCutSize;
		Tv_LimRight	=	A_PtrRect->top + E_GrP5SkinT2BtnCutSize - 1;

		// draw 
		Tv_RcDraw.left		=	Tv_RcBg.left;
		Tv_RcDraw.top			=	Tv_RcBg.top;
		Tv_RcDraw.right		=	Tv_RcBg.right - E_GrP5SkinT2BtnCutSize + 1;
		Tv_RcDraw.bottom	=	Tv_RcDraw.top + 1;

		while(Tv_RcDraw.top < Tv_RcBg.bottom)
		{
			GrP5PntReqFillRect(&Tv_RcDraw, Tv_Clr);
			
			// next
			if(Tv_RcDraw.top >= Tv_LimLeft)
			{
				Tv_RcDraw.left ++;
			}

			if(Tv_RcDraw.top < Tv_LimRight)
			{
				Tv_RcDraw.right ++;
			}

			Tv_RcDraw.top ++;
			Tv_RcDraw.bottom ++;
		}
		

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcDrawBtnSubType1(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat)
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
void	Cls_GrP5SkinT2::LcDrawBtnSubType2(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat)
{
	// local -------------------
	// code --------------------
		if((0 != ((E_GrP5StatOnSelected | E_GrP5StatOnFocus | E_GrP5StatOnChecked) & A_Stat)) || (E_GrP5StatPush == (E_GrP5StatConstMask & A_Stat)))
		{
			GrP5PntReqFillRect(&m_CtlRcScr, m_PtrCtlSkinPara->ParaColor);
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcDrawBtnSubType3(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat)
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
void	Cls_GrP5SkinT2::LcDrawBtnSubType4(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat)
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
void	Cls_GrP5SkinT2::LcDrawPrgsBgT0(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect,
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
void	Cls_GrP5SkinT2::LcDrawPrgsBgT1(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect,
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
			Tv_Rc.left		=	Tv_Rc.left + E_GrP5SkinT2PrgsWakuWidth;
			Tv_Rc.right		=	Tv_Rc.right - E_GrP5SkinT2PrgsWakuWidth;
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
			Tv_Rc.left		=	Tv_Rc.left + E_GrP5SkinT2PrgsWakuWidth;
			Tv_Rc.right		=	Tv_Rc.right - E_GrP5SkinT2PrgsWakuWidth;
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
			Tv_Rc.top			=	Tv_Rc.top + E_GrP5SkinT2PrgsWakuWidth;
			Tv_Rc.bottom	=	Tv_Rc.bottom - E_GrP5SkinT2PrgsWakuWidth;
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
			Tv_Rc.top			=	Tv_Rc.top + E_GrP5SkinT2PrgsWakuWidth;
			Tv_Rc.bottom	=	Tv_Rc.bottom - E_GrP5SkinT2PrgsWakuWidth;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrPrgsBgFace[1]);
		}
		break;
	}
}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcDrawPrgsBg(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect,
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
void	Cls_GrP5SkinT2::LcDrawTriangleIsosUp(Ptr_GrRect A_PtrRect, __u32 A_Color)
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
void	Cls_GrP5SkinT2::LcDrawTriangleIsosDown(Ptr_GrRect A_PtrRect, __u32 A_Color)
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
void	Cls_GrP5SkinT2::LcDrawTriangleIsosLeft(Ptr_GrRect A_PtrRect, __u32 A_Color)
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
void	Cls_GrP5SkinT2::LcDrawTriangleIsosRight(Ptr_GrRect A_PtrRect, __u32 A_Color)
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
void	Cls_GrP5SkinT2::LcDrawTriangleRangRd(Ptr_GrRect A_PtrRect, __u32 A_Color)
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
void	Cls_GrP5SkinT2::LcDrawSpinHzBg(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat)
{
	// local -------------------
		St_GrRect	Tv_Rc;
		__u32	Tv_Color;
	// code --------------------

		GrP5PntArrangeByRect(A_PtrRect);

		if(E_GrP5StatPush == (E_GrP5StatConstMask & A_Stat))
		{
			// draw push background
			GrP5PntReqDrawRect(A_PtrRect, m_ClrSpinBgPush);
			Tv_Rc.left		=	A_PtrRect->left + 1;
			Tv_Rc.top			=	A_PtrRect->top + 1;
			Tv_Rc.right		=	A_PtrRect->right - 1;
			Tv_Rc.bottom	=	A_PtrRect->bottom - 1;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrSpinBgNormal);

			return;
		}

		Tv_Color			=	m_ClrSpinBgNormal;
		if(E_GrP5StatDisable == (E_GrP5StatConstMask & A_Stat))
		{
			Tv_Color	=	m_ClrSpinBgDisable;
		}
		else if(E_GrP5StatPush == (E_GrP5StatConstMask & A_Stat))
		{
			Tv_Color	=	m_ClrSpinBgPush;
		}
		else if(0 != (E_GrP5StatOnFocus & A_Stat))
		{
			Tv_Color	=	m_ClrSpinBgFocus;
		}

		GrP5PntReqFillRect(A_PtrRect, Tv_Color);

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcDrawSpinHzBtnDec(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat)
{
	// local -------------------
		__u32	Tv_Clr;
		St_GrRect	Tv_Rc;
	// code --------------------

		if(E_GrP5StatDisable == (E_GrP5StatConstMask & A_Stat))
		{
			return;
		}

		// draw mark
		Tv_Rc.left		=	A_PtrRect->left + 2;
		Tv_Rc.top			=	A_PtrRect->top	 + 4;
		Tv_Rc.right		=	A_PtrRect->right - 4;
		Tv_Rc.bottom	=	A_PtrRect->bottom - 4;

		Tv_Clr	=	m_ClrSpinCtlMarkNormal;
		if(E_GrP5StatPush == (E_GrP5StatConstMask & A_Stat))
		{
			Tv_Clr	=	m_ClrSpinCtlMarkPush;
		}
		else if(0 != (E_GrP5StatOnFocus & A_Stat))
		{
			Tv_Clr	=	m_ClrSpinCtlMarkFocus;
		}

		LcDrawTriangleIsosLeft(&Tv_Rc, Tv_Clr);

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcDrawSpinHzBtnInc(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat)
{
	// local -------------------
		__u32	Tv_Clr;
		St_GrRect	Tv_Rc;
	// code --------------------

		if(E_GrP5StatDisable == (E_GrP5StatConstMask & A_Stat))
		{
			return;
		}

		// draw mark
		Tv_Rc.left		=	A_PtrRect->left + 4;
		Tv_Rc.top			=	A_PtrRect->top	 + 4;
		Tv_Rc.right		=	A_PtrRect->right - 2;
		Tv_Rc.bottom	=	A_PtrRect->bottom - 4;

		Tv_Clr	=	m_ClrSpinCtlMarkNormal;
		if(E_GrP5StatPush == (E_GrP5StatConstMask & A_Stat))
		{
			Tv_Clr	=	m_ClrSpinCtlMarkPush;
		}
		else if(0 != (E_GrP5StatOnFocus & A_Stat))
		{
			Tv_Clr	=	m_ClrSpinCtlMarkFocus;
		}

		LcDrawTriangleIsosRight(&Tv_Rc, Tv_Clr);

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcDrawSpinVtBg(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat)
{
	// local -------------------
		__u32	Tv_Clr;
	// code --------------------
		GrP5PntArrangeByRect(A_PtrRect);

		// draw background color
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

		GrP5PntReqFillRect(A_PtrRect, Tv_Clr);

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcDrawSpinVtBtnDec(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat)
{
	// local -------------------
	//St_GrRect	Tv_Rc;
	//__u32	Tv_Clr;
	//St_GrRect	Tv_RcCtl;
	// code --------------------
#if 0

	GrP5PntReqDrawRect(A_PtrRect, m_ClrSpinCtlWakeDark);

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
		Tv_RcCtl.left		=	Tv_RcBg.left + E_GrP5SkinT2SpinCtlMargin;
		Tv_RcCtl.top		=	Tv_RcBg.top + E_GrP5SkinT2SpinCtlMargin;
		Tv_RcCtl.right	=	Tv_RcBg.right - E_GrP5SkinT2SpinCtlMargin;
		Tv_RcCtl.bottom	=	Tv_RcBg.bottom - E_GrP5SkinT2SpinCtlMargin;

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
			LcDrawTriangleIsosDown(&Tv_RcCtl, Tv_Clr);
		}
	}
#endif

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcDrawSpinVtBtnInc(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat)
{
	// local -------------------
	//St_GrRect	Tv_RcBg;
	//__u32	Tv_Clr;
	//St_GrRect	Tv_RcCtl;
	// code --------------------
#if 0

	GrP5PntReqDrawRect(A_PtrRect, m_ClrSpinCtlWakeDark);

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
		Tv_RcCtl.left		=	Tv_RcBg.left + E_GrP5SkinT2SpinCtlMargin;
		Tv_RcCtl.top		=	Tv_RcBg.top + E_GrP5SkinT2SpinCtlMargin;
		Tv_RcCtl.right	=	Tv_RcBg.right - E_GrP5SkinT2SpinCtlMargin;
		Tv_RcCtl.bottom	=	Tv_RcBg.bottom - E_GrP5SkinT2SpinCtlMargin;

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
			LcDrawTriangleIsosUp(&Tv_RcCtl, Tv_Clr);
		}
	}
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcDrawGraphHzBg(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat)
{
	// local -------------------
	St_GrRect	Tv_Rc;
	// code --------------------
	if(0 != (E_GrP5StatOnFocus & A_Stat))
	{

		Tv_Rc.left		=	A_PtrRect->left;
		Tv_Rc.top			=	A_PtrRect->top;
		Tv_Rc.right		=	A_PtrRect->right;
		Tv_Rc.bottom	=	A_PtrRect->top + E_GrP5SkinT2GraphHzMargin;
		GrP5PntReqFillRect(&Tv_Rc, m_ClrGrpHzCusHz);

		Tv_Rc.top			=	A_PtrRect->bottom - E_GrP5SkinT2GraphHzMargin;
		Tv_Rc.bottom	=	A_PtrRect->bottom;
		GrP5PntReqFillRect(&Tv_Rc, m_ClrGrpHzCusHz);

		Tv_Rc.top			=	A_PtrRect->top + E_GrP5SkinT2GraphHzMargin;
		Tv_Rc.right		=	A_PtrRect->left + E_GrP5SkinT2GraphHzMargin;
		Tv_Rc.bottom	=	A_PtrRect->bottom - E_GrP5SkinT2GraphHzMargin;
		GrP5PntReqFillRect(&Tv_Rc, m_ClrGrpHzCusHz);

		Tv_Rc.left		=	A_PtrRect->right - E_GrP5SkinT2GraphHzMargin;
		Tv_Rc.right		=	A_PtrRect->right;
		GrP5PntReqFillRect(&Tv_Rc, m_ClrGrpHzCusHz);
	}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcDrawGraphHzItem(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u8 A_Val)
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
void	Cls_GrP5SkinT2::LcDrawGraphHzCursorVt(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat)
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
void	Cls_GrP5SkinT2::LcDrawChkBgTop(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat)
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
void	Cls_GrP5SkinT2::LcDrawChkMark(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat)
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
		LcDrawVmark(&Tv_Rc, m_ClrChkMarkChar);
	}
}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcDrawVmark(Ptr_GrRect A_PtrRect, __u32 A_Color)
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
void	Cls_GrP5SkinT2::LcDrawGridDrag(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect)
{
	// local -------------------
	St_GrRect	Tv_Rc;
	// code --------------------
	// top
	Tv_Rc.left		=	A_PtrRect->left;
	Tv_Rc.top			=	A_PtrRect->top;
	Tv_Rc.right		=	A_PtrRect->right;
	Tv_Rc.bottom	=	Tv_Rc.top + E_GrP5SkinT2GridDrageWidth;
	GrP5PntReqFillRect(&Tv_Rc, m_ClrGridDrag);
	// bottom
	Tv_Rc.top			=	A_PtrRect->bottom - E_GrP5SkinT2GridDrageWidth;
	Tv_Rc.bottom	=	A_PtrRect->bottom;
	GrP5PntReqFillRect(&Tv_Rc, m_ClrGridDrag);
	// left
	Tv_Rc.top			=	A_PtrRect->top + E_GrP5SkinT2GridDrageWidth;
	Tv_Rc.right		=	Tv_Rc.left + E_GrP5SkinT2GridDrageWidth;
	Tv_Rc.bottom	=	A_PtrRect->bottom - E_GrP5SkinT2GridDrageWidth;
	GrP5PntReqFillRect(&Tv_Rc, m_ClrGridDrag);
	// right
	Tv_Rc.left		=	A_PtrRect->right - E_GrP5SkinT2GridDrageWidth;
	Tv_Rc.right		=	A_PtrRect->right;
	GrP5PntReqFillRect(&Tv_Rc, m_ClrGridDrag);

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcDrawGridRscdDrag(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect)
{
	// local -------------------
	St_GrRect	Tv_Rc;
	// code --------------------

	// left
	Tv_Rc.left		=	A_PtrRect->left;
	Tv_Rc.top			=	A_PtrRect->top + E_GrP5SkinT2GridRscdDragWidth;
	Tv_Rc.right		=	A_PtrRect->left + E_GrP5SkinT2GridRscdDragWidth;
	Tv_Rc.bottom	=	A_PtrRect->bottom - E_GrP5SkinT2GridRscdDragWidth;
	GrP5PntReqFillRect(&Tv_Rc, m_ClrGridRscdDrag);

	// top
	Tv_Rc.left		=	A_PtrRect->left;
	Tv_Rc.top			=	A_PtrRect->top;
	Tv_Rc.right		=	A_PtrRect->right;
	Tv_Rc.bottom	=	A_PtrRect->top + E_GrP5SkinT2GridRscdDragWidth;
	GrP5PntReqFillRect(&Tv_Rc, m_ClrGridRscdDrag);

	// right
	Tv_Rc.left		=	A_PtrRect->right - E_GrP5SkinT2GridRscdDragWidth;
	Tv_Rc.top			=	A_PtrRect->top + E_GrP5SkinT2GridRscdDragWidth;
	Tv_Rc.right		=	A_PtrRect->right;
	Tv_Rc.bottom	=	A_PtrRect->bottom - E_GrP5SkinT2GridRscdDragWidth;
	GrP5PntReqFillRect(&Tv_Rc, m_ClrGridRscdDrag);

	// bottom
	Tv_Rc.left		=	A_PtrRect->left;
	Tv_Rc.top			=	A_PtrRect->bottom - E_GrP5SkinT2GridRscdDragWidth;
	Tv_Rc.right		=	A_PtrRect->right;
	Tv_Rc.bottom	=	A_PtrRect->bottom;
	GrP5PntReqFillRect(&Tv_Rc, m_ClrGridRscdDrag);


}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcDrawGridSmdItem(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat)
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
	if((((E_GrP5SkinT2GridOutWidth * 2) + (E_GrP5SkinT2GridCusWidth * 2) + (E_GrP5SkinT2GridMarkWidth * 2)) <= Tv_SzX) &&
		(((E_GrP5SkinT2GridOutWidth * 2) + (E_GrP5SkinT2GridCusWidth * 2) + (E_GrP5SkinT2GridMarkWidth * 2)) <= Tv_SzY)
		)
	{

		if((E_GrP5StatOnChecked | E_GrP5StatOnCtrlCus) == ((E_GrP5StatOnChecked | E_GrP5StatOnCtrlCus) & A_Stat))
		{
			// full draw
			Tv_Rc.left		=	A_PtrRect->left;
			Tv_Rc.top			=	A_PtrRect->top;
			Tv_Rc.right		=	A_PtrRect->right;
			Tv_Rc.bottom	=	Tv_Rc.top + (E_GrP5SkinT2GridMarkWidth + E_GrP5SkinT2GridOutWidth);
			GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCursor);
			Tv_Rc.top			=	Tv_Rc.bottom;
			Tv_Rc.right		=	Tv_Rc.left + (E_GrP5SkinT2GridMarkWidth + E_GrP5SkinT2GridOutWidth);
			Tv_Rc.bottom	=	A_PtrRect->bottom - E_GrP5SkinT2GridMarkWidth - E_GrP5SkinT2GridOutWidth;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCursor);
			Tv_Rc.left		=	A_PtrRect->right - E_GrP5SkinT2GridMarkWidth - E_GrP5SkinT2GridOutWidth;
			Tv_Rc.right		=	Tv_Rc.left + E_GrP5SkinT2GridMarkWidth + E_GrP5SkinT2GridOutWidth;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCursor);
			Tv_Rc.left		=	A_PtrRect->left;
			Tv_Rc.top			=	Tv_Rc.bottom;
			Tv_Rc.right		=	A_PtrRect->right;
			Tv_Rc.bottom	=	Tv_Rc.top + E_GrP5SkinT2GridMarkWidth + E_GrP5SkinT2GridOutWidth;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCursor);
		}
		else if(0 == ((E_GrP5StatOnChecked | E_GrP5StatOnCtrlCus) & A_Stat))
		{
			// draw outline only
			Tv_Rc.left		=	A_PtrRect->left;
			Tv_Rc.top			=	A_PtrRect->top;
			Tv_Rc.right		=	A_PtrRect->right;
			Tv_Rc.bottom	=	A_PtrRect->top + E_GrP5SkinT2GridOutWidth;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCellOutline);
			Tv_Rc.top			=	Tv_Rc.bottom;
			Tv_Rc.right		=	A_PtrRect->left + E_GrP5SkinT2GridOutWidth;
			Tv_Rc.bottom	=	A_PtrRect->bottom - E_GrP5SkinT2GridOutWidth;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCellOutline);
			Tv_Rc.left		=	A_PtrRect->right - E_GrP5SkinT2GridOutWidth;
			Tv_Rc.right		=	A_PtrRect->right;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCellOutline);
			Tv_Rc.left		=	A_PtrRect->left;
			Tv_Rc.top			=	A_PtrRect->bottom - E_GrP5SkinT2GridOutWidth;
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
			Tv_Rc.bottom	=	A_PtrRect->top + E_GrP5SkinT2GridOutWidth;
			GrP5PntReqFillRect(&Tv_Rc, Tv_Clr);
			Tv_Rc.top			=	Tv_Rc.bottom;
			Tv_Rc.right		=	A_PtrRect->left + E_GrP5SkinT2GridOutWidth;
			Tv_Rc.bottom	=	A_PtrRect->bottom - E_GrP5SkinT2GridOutWidth;
			GrP5PntReqFillRect(&Tv_Rc, Tv_Clr);
			Tv_Rc.left		=	A_PtrRect->right - E_GrP5SkinT2GridOutWidth;
			Tv_Rc.right		=	A_PtrRect->right;
			GrP5PntReqFillRect(&Tv_Rc, Tv_Clr);
			Tv_Rc.left		=	A_PtrRect->left;
			Tv_Rc.top			=	A_PtrRect->bottom - E_GrP5SkinT2GridOutWidth;
			Tv_Rc.right		=	A_PtrRect->right;
			Tv_Rc.bottom	=	A_PtrRect->bottom;
			GrP5PntReqFillRect(&Tv_Rc, Tv_Clr);

			// cursor
			if(0 != (E_GrP5StatOnCtrlCus & A_Stat))
			{
				Tv_Rc.left		=	A_PtrRect->left + E_GrP5SkinT2GridOutWidth;
				Tv_Rc.top			=	A_PtrRect->top + E_GrP5SkinT2GridOutWidth;
				Tv_Rc.right		=	A_PtrRect->right - E_GrP5SkinT2GridOutWidth;
				Tv_Rc.bottom	=	Tv_Rc.top + E_GrP5SkinT2GridMarkWidth;
				GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCursor);
				Tv_Rc.top			=	Tv_Rc.bottom;
				Tv_Rc.right		=	Tv_Rc.left + E_GrP5SkinT2GridMarkWidth;
				Tv_Rc.bottom	=	A_PtrRect->bottom - E_GrP5SkinT2GridMarkWidth - E_GrP5SkinT2GridOutWidth;
				GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCursor);
				Tv_Rc.left		=	A_PtrRect->right - E_GrP5SkinT2GridMarkWidth - E_GrP5SkinT2GridOutWidth;
				Tv_Rc.right		=	Tv_Rc.left + E_GrP5SkinT2GridMarkWidth;
				GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCursor);
				Tv_Rc.left		=	A_PtrRect->left + E_GrP5SkinT2GridOutWidth;
				Tv_Rc.top			=	Tv_Rc.bottom;
				Tv_Rc.right		=	A_PtrRect->right - E_GrP5SkinT2GridOutWidth;
				Tv_Rc.bottom	=	Tv_Rc.top + E_GrP5SkinT2GridMarkWidth;
				GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCursor);
			}
		}



#if 0
		// draw outline
		Tv_Rc.left		=	A_PtrRect->left;
		Tv_Rc.top			=	A_PtrRect->top;
		Tv_Rc.right		=	A_PtrRect->right;
		Tv_Rc.bottom	=	A_PtrRect->top + E_GrP5SkinT2GridOutWidth;
		GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCellOutline);
		Tv_Rc.top			=	Tv_Rc.bottom;
		Tv_Rc.right		=	A_PtrRect->left + E_GrP5SkinT2GridOutWidth;
		Tv_Rc.bottom	=	A_PtrRect->bottom - E_GrP5SkinT2GridOutWidth;
		GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCellOutline);
		Tv_Rc.left		=	A_PtrRect->right - E_GrP5SkinT2GridOutWidth;
		Tv_Rc.right		=	A_PtrRect->right;
		GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCellOutline);
		Tv_Rc.left		=	A_PtrRect->left;
		Tv_Rc.top			=	A_PtrRect->bottom - E_GrP5SkinT2GridOutWidth;
		Tv_Rc.right		=	A_PtrRect->right;
		Tv_Rc.bottom	=	A_PtrRect->bottom;
		GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCellOutline);

		if((E_GrP5StatOnChecked | E_GrP5StatOnCtrlCus) == ((E_GrP5StatOnChecked | E_GrP5StatOnCtrlCus) & A_Stat))
		{
			// full draw
			Tv_Rc.left		=	A_PtrRect->left + E_GrP5SkinT2GridOutWidth;
			Tv_Rc.top			=	A_PtrRect->top + E_GrP5SkinT2GridOutWidth;
			Tv_Rc.right		=	A_PtrRect->right - E_GrP5SkinT2GridOutWidth;
			Tv_Rc.bottom	=	Tv_Rc.top + (E_GrP5SkinT2GridMarkWidth + E_GrP5SkinT2GridCusWidth);
			GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCursor);
			Tv_Rc.top			=	Tv_Rc.bottom;
			Tv_Rc.right		=	Tv_Rc.left + (E_GrP5SkinT2GridMarkWidth + E_GrP5SkinT2GridCusWidth);
			Tv_Rc.bottom	=	A_PtrRect->bottom - E_GrP5SkinT2GridCusWidth - E_GrP5SkinT2GridMarkWidth - E_GrP5SkinT2GridOutWidth;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCursor);
			Tv_Rc.left		=	A_PtrRect->right - E_GrP5SkinT2GridCusWidth - E_GrP5SkinT2GridMarkWidth - E_GrP5SkinT2GridOutWidth;
			Tv_Rc.right		=	Tv_Rc.left + E_GrP5SkinT2GridMarkWidth + E_GrP5SkinT2GridCusWidth;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCursor);
			Tv_Rc.left		=	A_PtrRect->left + E_GrP5SkinT2GridOutWidth;
			Tv_Rc.top			=	Tv_Rc.bottom;
			Tv_Rc.right		=	A_PtrRect->right - E_GrP5SkinT2GridOutWidth;
			Tv_Rc.bottom	=	Tv_Rc.top + E_GrP5SkinT2GridMarkWidth + E_GrP5SkinT2GridCusWidth;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCursor);
		}
		else
		{
			// separate
			// mark
			if(0 != (E_GrP5StatOnChecked & A_Stat))
			{
				Tv_Rc.left		=	A_PtrRect->left + E_GrP5SkinT2GridOutWidth;
				Tv_Rc.top			=	A_PtrRect->top + E_GrP5SkinT2GridOutWidth;
				Tv_Rc.right		=	A_PtrRect->right - E_GrP5SkinT2GridOutWidth;
				Tv_Rc.bottom	=	Tv_Rc.top + E_GrP5SkinT2GridMarkWidth;
				GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCursor);
				Tv_Rc.top			=	Tv_Rc.bottom;
				Tv_Rc.right		=	Tv_Rc.left + E_GrP5SkinT2GridMarkWidth;
				Tv_Rc.bottom	=	A_PtrRect->bottom - E_GrP5SkinT2GridMarkWidth - E_GrP5SkinT2GridOutWidth;
				GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCursor);
				Tv_Rc.left		=	A_PtrRect->right - E_GrP5SkinT2GridMarkWidth - E_GrP5SkinT2GridOutWidth;
				Tv_Rc.right		=	Tv_Rc.left + E_GrP5SkinT2GridMarkWidth;
				GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCursor);
				Tv_Rc.left		=	A_PtrRect->left + E_GrP5SkinT2GridOutWidth;
				Tv_Rc.top			=	Tv_Rc.bottom;
				Tv_Rc.right		=	A_PtrRect->right - E_GrP5SkinT2GridOutWidth;
				Tv_Rc.bottom	=	Tv_Rc.top + E_GrP5SkinT2GridMarkWidth;
				GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCursor);
			}

			// cursor
			Tv_Clr	=	0;
			if(0 != (E_GrP5StatOnCtrlCus & A_Stat))
			{
				Tv_Rc.left		=	A_PtrRect->left + E_GrP5SkinT2GridOutWidth + E_GrP5SkinT2GridMarkWidth;
				Tv_Rc.top			=	A_PtrRect->top + E_GrP5SkinT2GridOutWidth + E_GrP5SkinT2GridMarkWidth;
				Tv_Rc.right		=	A_PtrRect->right - E_GrP5SkinT2GridOutWidth - E_GrP5SkinT2GridMarkWidth;
				Tv_Rc.bottom	=	Tv_Rc.top + E_GrP5SkinT2GridCusWidth;
				GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCursor);
				Tv_Rc.top			=	Tv_Rc.bottom;
				Tv_Rc.right		=	Tv_Rc.left + E_GrP5SkinT2GridCusWidth;
				Tv_Rc.bottom	=	A_PtrRect->bottom - E_GrP5SkinT2GridCusWidth - E_GrP5SkinT2GridOutWidth - E_GrP5SkinT2GridMarkWidth;
				GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCursor);
				Tv_Rc.left		=	A_PtrRect->right - E_GrP5SkinT2GridCusWidth - E_GrP5SkinT2GridOutWidth - E_GrP5SkinT2GridMarkWidth;
				Tv_Rc.right		=	Tv_Rc.left + E_GrP5SkinT2GridCusWidth;
				GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCursor);
				Tv_Rc.left		=	A_PtrRect->left + E_GrP5SkinT2GridOutWidth + E_GrP5SkinT2GridMarkWidth;
				Tv_Rc.top			=	Tv_Rc.bottom;
				Tv_Rc.right		=	A_PtrRect->right - E_GrP5SkinT2GridOutWidth - E_GrP5SkinT2GridMarkWidth;
				Tv_Rc.bottom	=	Tv_Rc.top + E_GrP5SkinT2GridCusWidth;
				GrP5PntReqFillRect(&Tv_Rc, m_ClrGridCursor);
			}
		}
#endif

	}	// check size
}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcDrawAglBrkLt(Ptr_GrRect A_PtrRect, __u32 A_Clr, __u32 A_Width)
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
void	Cls_GrP5SkinT2::LcDrawAglBrkRt(Ptr_GrRect A_PtrRect, __u32 A_Clr, __u32 A_Width)
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
void	Cls_GrP5SkinT2::LcDrawAglBrkUp(Ptr_GrRect A_PtrRect, __u32 A_Clr, __u32 A_Width)
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
void	Cls_GrP5SkinT2::LcDrawAglBrkDn(Ptr_GrRect A_PtrRect, __u32 A_Clr, __u32 A_Width)
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
void	Cls_GrP5SkinT2::LcDrawGridRscdItem(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat, __u8 A_Val)
{
	// local -------------------
		St_GrRect	Tv_RcItm;
		__u32			Tv_Clr;
	// code --------------------
		Tv_RcItm.left		=	A_PtrRect->left;
		Tv_RcItm.top		=	A_PtrRect->top + (__s32)(__u32)A_PtrPara->ParaByte0;
		Tv_RcItm.right	=	A_PtrRect->right;
		Tv_RcItm.bottom	=	Tv_RcItm.top + E_GrP5SkinT2GridRscdItemHeight;
		if(Tv_RcItm.bottom >= A_PtrRect->bottom)
		{
			Tv_RcItm.bottom	=	A_PtrRect->bottom;
		}

		if((Tv_RcItm.right <= Tv_RcItm.left) || (Tv_RcItm.bottom <= Tv_RcItm.top))
		{
			return;
		}

		Tv_Clr					=	m_ClrGridRscdItm[A_Val & 7];

		GrP5PntReqFillRect(&Tv_RcItm, Tv_Clr);

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcDrawAimBg(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat)
{
	// local -------------------
	__s32	Tv_CtX;
	__s32	Tv_CtY;
	St_GrRect	Tv_Rc;
	// code --------------------
	Tv_CtX	=	A_PtrRect->left + ((A_PtrRect->right - A_PtrRect->left) >> 1);
	Tv_CtY	=	A_PtrRect->top + ((A_PtrRect->bottom - A_PtrRect->top) >> 1);
	// cross center
	Tv_Rc.left		=	Tv_CtX - (E_GrP5SkinT2AimCrossWidth >> 1);
	Tv_Rc.right		=	Tv_Rc.left + E_GrP5SkinT2AimCrossWidth;
	Tv_Rc.top			=	Tv_CtY - (E_GrP5SkinT2AimCrossSize >> 1);
	Tv_Rc.bottom	=	Tv_Rc.top + E_GrP5SkinT2AimCrossSize;
	GrP5PntReqFillRect(&Tv_Rc, m_ClrAimCross);

	// cross left
	Tv_Rc.left		=	Tv_CtX - (E_GrP5SkinT2AimCrossSize >> 1);
	Tv_Rc.right		=	Tv_CtX - (E_GrP5SkinT2AimCrossWidth >> 1);
	Tv_Rc.top			=	Tv_CtY - (E_GrP5SkinT2AimCrossWidth >> 1);
	Tv_Rc.bottom	=	Tv_Rc.top + E_GrP5SkinT2AimCrossWidth;
	GrP5PntReqFillRect(&Tv_Rc, m_ClrAimCross);

	// cross right
	Tv_Rc.left		=	Tv_Rc.right + E_GrP5SkinT2AimCrossWidth;
	Tv_Rc.right		=	Tv_CtX + (E_GrP5SkinT2AimCrossSize >> 1);
	GrP5PntReqFillRect(&Tv_Rc, m_ClrAimCross);

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcDrawGrpHzT1BgCont(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat)
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
		Tv_Rc.bottom	=	Tv_Rc.top + E_GrP5SkinT2GrpHzT1ContMgn;
		GrP5PntReqFillRect(&Tv_Rc, m_ClrGrpHzT1BgFocus);
		// down
		Tv_Rc.top			=	A_PtrRect->bottom - E_GrP5SkinT2GrpHzT1ContMgn;
		Tv_Rc.bottom	=	A_PtrRect->bottom;
		GrP5PntReqFillRect(&Tv_Rc, m_ClrGrpHzT1BgFocus);
		// left
		Tv_Rc.top			=	A_PtrRect->top + E_GrP5SkinT2GrpHzT1ContMgn;
		Tv_Rc.right		=	Tv_Rc.left + E_GrP5SkinT2GrpHzT1ContMgn;
		Tv_Rc.bottom	=	A_PtrRect->bottom - E_GrP5SkinT2GrpHzT1ContMgn;
		GrP5PntReqFillRect(&Tv_Rc, m_ClrGrpHzT1BgFocus);
		// right
		Tv_Rc.left		=	A_PtrRect->right - E_GrP5SkinT2GrpHzT1ContMgn;
		Tv_Rc.right		=	A_PtrRect->right;
		GrP5PntReqFillRect(&Tv_Rc, m_ClrGrpHzT1BgFocus);
	}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcDrawGrpHzT1Item(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat, __u8 A_Val)
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
void	Cls_GrP5SkinT2::LcDrawGrpHzT1BgInd(Ptr_GrP5SkinPara A_PtrPara, Ptr_GrRect A_PtrRect, __u32 A_Stat)
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
		for(Tv_WkIdx = 0; Tv_WkIdx < 4; Tv_WkIdx++)
		{
			Tv_Rc.top			=	Tv_Rc.bottom;
			Tv_Rc.left		=	A_PtrRect->left + (__s32)(__u32)V_GrP5SkinT2Curb4x[Tv_WkIdx];
			Tv_Rc.right		=	A_PtrRect->right - (__s32)(__u32)V_GrP5SkinT2Curb4x[Tv_WkIdx];
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
			Tv_Rc.left		=	A_PtrRect->left + (__s32)(__u32)V_GrP5SkinT2Curb4x[3 - Tv_WkIdx];
			Tv_Rc.right		=	A_PtrRect->right - (__s32)(__u32)V_GrP5SkinT2Curb4x[3 - Tv_WkIdx];
			Tv_Rc.bottom	=	Tv_Rc.top + 1;

			GrP5PntReqFillRect(&Tv_Rc, m_ClrGrpHzT1IndBg);
		}
	}
}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcDrawArwCrosshair(__s32 A_X, __s32 A_Y)
{
	// local -------------------
	St_GrRect	Tv_RcDw;
	__u32	Tv_RcIdx;
	// code --------------------
	// draw out
	Tv_RcIdx	=	0;
	while(0x7FFF > V_GrP5SkinT2ArwCrhOutTbl[Tv_RcIdx].left)
	{
		Tv_RcDw.left		=	A_X + (__s32)V_GrP5SkinT2ArwCrhOutTbl[Tv_RcIdx].left;
		Tv_RcDw.top			=	A_Y + (__s32)V_GrP5SkinT2ArwCrhOutTbl[Tv_RcIdx].top;
		Tv_RcDw.right		=	A_X + (__s32)V_GrP5SkinT2ArwCrhOutTbl[Tv_RcIdx].right;
		Tv_RcDw.bottom	=	A_Y + (__s32)V_GrP5SkinT2ArwCrhOutTbl[Tv_RcIdx].bottom;

		GrP5PntReqFillRect(&Tv_RcDw, m_ClrArwBase[0]);

		// next
		Tv_RcIdx ++;
	}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcDrawArwHalfCir(__s32 A_X, __s32 A_Y, __u32 A_Angle, __u32 A_Quadrant, __u32 A_Color)
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
	for(Tv_WkIdx = 0; Tv_WkIdx < 32; Tv_WkIdx++)
	{
		// draw now
		if(0 == Tv_Qdrt)
		{
			Tv_Rc.left		=	A_X + V_GrP5SkinT2ArwCircle[Tv_Angl].left;
			Tv_Rc.top			=	A_Y + V_GrP5SkinT2ArwCircle[Tv_Angl].top;
			Tv_Rc.right		=	A_X + V_GrP5SkinT2ArwCircle[Tv_Angl].right;
			Tv_Rc.bottom	=	A_Y + V_GrP5SkinT2ArwCircle[Tv_Angl].bottom;
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
			Tv_Rc.left		=	A_X - V_GrP5SkinT2ArwCircle[Tv_Angl].right - 1;
			Tv_Rc.top			=	A_Y + V_GrP5SkinT2ArwCircle[Tv_Angl].top;
			Tv_Rc.right		=	A_X - V_GrP5SkinT2ArwCircle[Tv_Angl].left;
			Tv_Rc.bottom	=	A_Y + V_GrP5SkinT2ArwCircle[Tv_Angl].bottom;
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
			Tv_Rc.left		=	A_X - V_GrP5SkinT2ArwCircle[Tv_Angl].right - 1;
			Tv_Rc.top			=	A_Y - V_GrP5SkinT2ArwCircle[Tv_Angl].bottom + 1;
			Tv_Rc.right		=	A_X - V_GrP5SkinT2ArwCircle[Tv_Angl].left;
			Tv_Rc.bottom	=	A_Y - V_GrP5SkinT2ArwCircle[Tv_Angl].top + 2;
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
			Tv_Rc.left		=	A_X + V_GrP5SkinT2ArwCircle[Tv_Angl].left;
			Tv_Rc.top			=	A_Y - V_GrP5SkinT2ArwCircle[Tv_Angl].bottom + 1;
			Tv_Rc.right		=	A_X + V_GrP5SkinT2ArwCircle[Tv_Angl].right;
			Tv_Rc.bottom	=	A_Y - V_GrP5SkinT2ArwCircle[Tv_Angl].top + 2;
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
void	Cls_GrP5SkinT2::LcDrawArwCircle(__s32 A_StX, __s32 A_StY, __s32 A_EdX, __s32 A_EdY, __u32 A_Stat, __u32 A_Val)
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
void	Cls_GrP5SkinT2::LcDrawArwStart(__s32 A_X, __s32 A_Y)
{
	// local -------------------
	St_GrRect	Tv_RcDw;
	__u32	Tv_RcIdx;
	// code --------------------
	// draw
	Tv_RcIdx	=	0;
	while(0x7FFF > V_GrP5SkinT2ArwStartTbl[Tv_RcIdx].left)
	{
		Tv_RcDw.left		=	A_X + (__s32)V_GrP5SkinT2ArwStartTbl[Tv_RcIdx].left;
		Tv_RcDw.top			=	A_Y + (__s32)V_GrP5SkinT2ArwStartTbl[Tv_RcIdx].top;
		Tv_RcDw.right		=	A_X + (__s32)V_GrP5SkinT2ArwStartTbl[Tv_RcIdx].right;
		Tv_RcDw.bottom	=	A_Y + (__s32)V_GrP5SkinT2ArwStartTbl[Tv_RcIdx].bottom;

		GrP5PntReqFillRect(&Tv_RcDw, m_ClrArwBase[1]);

		// next
		Tv_RcIdx ++;
	}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcDrawArwItem(__s32 A_StX, __s32 A_StY, __s32 A_EdX, __s32 A_EdY, __u32 A_ColorType, __u32 A_DirMap)
{
	// local -------------------
	// code --------------------
	// check exist start position
	if((0 > A_StX) || (0 > A_StY))
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
void	Cls_GrP5SkinT2::LcDrawArwSideMark(__s32 A_StX, __s32 A_StY, __s32 A_EdX, __s32 A_EdY, __u32 A_DirMap)
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
void	Cls_GrP5SkinT2::LcDrawPerpAraw(__s32 A_Dx, __s32 A_Dy, __s32 A_Cx, __s32 A_Cy)
{
	// local -------------------
	__s32	Tv_Angle;
	BOOL8	Tv_IsFlipX;
	BOOL8	Tv_IsFlipY;
	Ptr_GrP5SkinT2VctCmd	Tv_PtrVct;
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
	else if((0 < A_Dx) && (0 >= A_Dy))
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
		Tv_PtrVct	=	V_GrP5SkinT2DirArwA0;
	}
	else if(5 > Tv_Angle)
	{
		Tv_PtrVct	=	V_GrP5SkinT2DirArwA11;
	}
	else if(9 > Tv_Angle)
	{
		Tv_PtrVct	=	V_GrP5SkinT2DirArwA23;
	}
	else if(13 > Tv_Angle)
	{
		Tv_PtrVct	=	V_GrP5SkinT2DirArwA34;
	}
	else if(20 > Tv_Angle)
	{
		Tv_PtrVct	=	V_GrP5SkinT2DirArwA45;
	}
	else if(38 > Tv_Angle)
	{
		Tv_PtrVct	=	V_GrP5SkinT2DirArwA56;
	}
	else if(59 > Tv_Angle)
	{
		Tv_PtrVct	=	V_GrP5SkinT2DirArwA68;
	}
	else if(450 > Tv_Angle)
	{
		Tv_PtrVct	=	V_GrP5SkinT2DirArwA79;
	}
	else
	{
		Tv_PtrVct	=	V_GrP5SkinT2DirArwA90;
	}

	LcDrawVct(A_Cx, A_Cy, Tv_PtrVct, &m_ClrArwDirClr, Tv_IsFlipX, Tv_IsFlipY);

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcDrawVct(__s32 A_X, __s32 A_Y, Ptr_GrP5SkinT2VctCmd A_PtrCmd, __u32* A_PtrClrTbl,
	BOOL8 A_IsFlipX, BOOL8 A_IsFlipY)
{
	// local -------------------
	__u32	Tv_CmdIdx;
	St_GrRect	Tv_Rc;
	// code --------------------
	Tv_CmdIdx	=	0;

	while(E_GrP5SkinT2VctCmdEnd != A_PtrCmd[Tv_CmdIdx].Cmd)
	{
		switch(A_PtrCmd[Tv_CmdIdx].Cmd)
		{
		case E_GrP5SkinT2VctCmdRect:
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
		case E_GrP5SkinT2VctCmdLine:
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
void	Cls_GrP5SkinT2::CtlUpdateBase(void* A_ObjUi)
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
			case E_GrP5ClassIdWinPopList:
				LcCtlUpdtWinPopList();
				break;
			case E_GrP5ClassIdWinPopMenu:
				LcCtlUpdtWinPopMenu();
				break;
			case E_GrP5ClassIdGrpHzT2:
				LcCtlUpdtGrpHzT2();
				break;
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::CtlDrawBase(void* A_ObjUi)
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

			case E_GrP5ClassIdWinPopList:
				LcCtlDrawWinPopList();
				break;
			case E_GrP5ClassIdWinPopMenu:
				LcCtlDrawWinPopMenu();
				break;
			case E_GrP5ClassIdCbxLtLint:
			case E_GrP5ClassIdCbxLtSint:
			case E_GrP5ClassIdCbxTiSint:
				LcCtlDrawCbxBase();
				break;
			case E_GrP5ClassIdPnlDrag:
				LcCtlDrawPnlDrag();
				break;
			case E_GrP5ClassIdGrpHzT2:
				LcCtlDrawGrpHzT2();
				break;
		}


}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcCtlUpdtArwBase(void)
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
void	Cls_GrP5SkinT2::LcCtlDrawArwBase(void)
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
void	Cls_GrP5SkinT2::LcCtlDrawArwStd(void)
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
void	Cls_GrP5SkinT2::CtlGetItemRangeRect(void* A_ObjUi, __s32 A_ItmStX, __s32 A_ItmStY, __s32 A_ItmEdX, __s32 A_ItmEdY, Ptr_GrRect A_PtrRtRect)
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
			case E_GrP5ClassIdWinPopList:
				LcCtlItmRectWinPopList(A_ItmStX, A_ItmStY, A_ItmEdX, A_ItmEdY, A_PtrRtRect);
				break;
			case E_GrP5ClassIdWinPopMenu:
				LcCtlItmRectWinPopMenu(A_ItmStX, A_ItmStY, A_ItmEdX, A_ItmEdY, A_PtrRtRect);
				break;
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcCtlItmRectArwBase(__s32 A_ItmStX, __s32 A_ItmStY, __s32 A_ItmEdX, __s32 A_ItmEdY, Ptr_GrRect A_PtrRtRect)
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
void	Cls_GrP5SkinT2::CtlGetItemIdxByPos(void* A_ObjUi, __s32 A_X, __s32 A_Y, __s32* A_PtrRtIdxX, __s32* A_PtrRtIdxY)
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
			case E_GrP5ClassIdWinPopList:
				LcCtlItemIdxWinPopList(A_X, A_Y, A_PtrRtIdxX, A_PtrRtIdxY);
				break;
			case E_GrP5ClassIdWinPopMenu:
				LcCtlItemIdxWinPopMenu(A_X, A_Y, A_PtrRtIdxX, A_PtrRtIdxY);
				break;
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcCtlItemIdxArwBase(__s32 A_X, __s32 A_Y, __s32* A_PtrRtIdxX, __s32* A_PtrRtIdxY)
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
	if((0 == Tv_PtrCtlArwBase->CellSizeX) || (0 == Tv_PtrCtlArwBase->CellSizeY))
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
void	Cls_GrP5SkinT2::LcCtlDrawCalMark(void)
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

		St_GrRect	Tv_RcItm;
		St_GrRect	Tv_RcWork;
		St_GrRect	Tv_RcCont;

		__s32	Tv_LineWdLeft;
		__s32	Tv_LineWdTop;
		__s32	Tv_LineWdRight;
		__s32	Tv_LineWdBottom;

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
			// calculate line widht
			Tv_LineWdTop		=	E_GrP5SkinT2CalLineWidth;
			Tv_LineWdBottom	=	E_GrP5SkinT2CalLineWidth;
			if(0 != Tv_IdxY)
			{
				Tv_LineWdTop		=	(E_GrP5SkinT2CalLineWidth >> 1);
			}
			if((Tv_IdxY + 1) < E_GrP5SkinCalDayLineCnt)
			{
				Tv_LineWdBottom		=	(E_GrP5SkinT2CalLineWidth >> 1);
			}
			for(Tv_IdxX = 0; Tv_IdxX < E_GrTimeDayOfWeek; Tv_IdxX++)
			{
				// calculate line width
				Tv_LineWdLeft		=	E_GrP5SkinT2CalLineWidth;
				Tv_LineWdRight	=	E_GrP5SkinT2CalLineWidth;
				if(0 != Tv_IdxX)
				{
					Tv_LineWdLeft		=	(E_GrP5SkinT2CalLineWidth >> 1);
				}
				if((Tv_IdxX + 1) < E_GrTimeDayOfWeek)
				{
					Tv_LineWdRight		=	(E_GrP5SkinT2CalLineWidth >> 1);
				}

				// get rect
				LcCtlItmRectCalBase((__s32)Tv_IdxX, (__s32)Tv_IdxY, (__s32)Tv_IdxX, (__s32)Tv_IdxY, &Tv_RcItm);

				// draw item waku
				Tv_RcWork.left		=	Tv_RcItm.left;
				Tv_RcWork.top			=	Tv_RcItm.top;
				Tv_RcWork.right		=	Tv_RcItm.right;
				Tv_RcWork.bottom	=	Tv_RcItm.top + Tv_LineWdTop;
				GrP5PntReqFillRect(&Tv_RcWork, m_ClrCalLine);		// top
				Tv_RcWork.top			=	Tv_RcItm.bottom - Tv_LineWdBottom;
				Tv_RcWork.bottom	=	Tv_RcItm.bottom;
				GrP5PntReqFillRect(&Tv_RcWork, m_ClrCalLine); // bottom
				Tv_RcWork.top			=	Tv_RcItm.top + Tv_LineWdTop;
				Tv_RcWork.right		=	Tv_RcItm.left + Tv_LineWdLeft;
				Tv_RcWork.bottom	=	Tv_RcItm.bottom - Tv_LineWdBottom;
				GrP5PntReqFillRect(&Tv_RcWork, m_ClrCalLine);	// left
				Tv_RcWork.left		=	Tv_RcItm.right - Tv_LineWdRight;
				Tv_RcWork.right		=	Tv_RcItm.right;
				GrP5PntReqFillRect(&Tv_RcWork, m_ClrCalLine);	// right

				// check draw able
				if(0 == Tv_PtrCtlCalBase->DayTbl[Tv_IdxY][Tv_IdxX].Day)
				{
					continue;
				}

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

				// contents rect set
				//Tv_RcCont	=	Tv_RcItm;
				Tv_RcCont.left		=	Tv_RcItm.left + Tv_LineWdLeft;
				Tv_RcCont.top			=	Tv_RcItm.top + Tv_LineWdTop;
				Tv_RcCont.right		=	Tv_RcItm.right - Tv_LineWdRight;
				Tv_RcCont.bottom	=	Tv_RcItm.bottom - Tv_LineWdBottom;

				// draw cursor area
				if(Tv_IsCus)
				{
					ReqPntRectangle(&Tv_RcCont, E_GrP5SkinT2CalCusWidth, m_ClrCalCusNrm);
					// patch rect
					Tv_RcCont.left		=	Tv_RcCont.left + E_GrP5SkinT2CalCusWidth;
					Tv_RcCont.top			=	Tv_RcCont.top + E_GrP5SkinT2CalCusWidth;
					Tv_RcCont.right		=	Tv_RcCont.right - E_GrP5SkinT2CalCusWidth;
					Tv_RcCont.bottom	=	Tv_RcCont.bottom - E_GrP5SkinT2CalCusWidth;
				}

				if(Tv_IsSel)
				{
					ReqPntRectangle(&Tv_RcCont, E_GrP5SkinT2CalCusWidth, m_ClrCalSel);
					// patch rect
					Tv_RcCont.left		=	Tv_RcCont.left + E_GrP5SkinT2CalCusWidth;
					Tv_RcCont.top			=	Tv_RcCont.top + E_GrP5SkinT2CalCusWidth;
					Tv_RcCont.right		=	Tv_RcCont.right - E_GrP5SkinT2CalCusWidth;
					Tv_RcCont.bottom	=	Tv_RcCont.bottom - E_GrP5SkinT2CalCusWidth;
				}

				// draw contents
				if(0 != Tv_PtrCtlCalBase->DayTbl[Tv_IdxY][Tv_IdxX].MarkMap)
				{
					GrP5PntReqFillRect(&Tv_RcCont, m_ClrCalMark);
				}

				// draw text
				if(GrP5RelRectToAbsRect(&Tv_PtrParaCalBase->TxtRc, &Tv_RcItm, &Tv_RcWork))
				{
					GrP5PntReqTxtStyle(&Tv_PtrParaCalBase->TxtPara, Tv_PtrParaCalBase->FontId);
					GrP5PntReqTxtCus();
					GrP5PntReqTxtDraw(&Tv_RcWork, V_GrP5SkinT2StrDayTbl[Tv_PtrCtlCalBase->DayTbl[Tv_IdxY][Tv_IdxX].Day -1], 0);	// txt buf ok
				}

			}	//	for(Tv_IdxX = 0; Tv_IdxX < E_GrTimeDayOfWeek; Tv_IdxX++)
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcCtlItmRectCalBase(__s32 A_ItmStX, __s32 A_ItmStY, __s32 A_ItmEdX, __s32 A_ItmEdY, Ptr_GrRect A_PtrRtRect)
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
			(0 > A_ItmEdX) || (E_GrTimeDayOfWeek <= A_ItmEdX) || (0 > A_ItmEdY) || (E_GrP5SkinCalDayLineCnt <= A_ItmEdY))
		{
			return;
		}

		A_PtrRtRect->left			=	((__s32)Tv_PtrCtlCalBase->DayVwSizeX * A_ItmStX) + E_GrP5SkinT2CalMargin;
		A_PtrRtRect->top			=	((__s32)Tv_PtrCtlCalBase->DayVwSizeY * A_ItmStY) + E_GrP5SkinT2CalMargin;
		A_PtrRtRect->right		=	A_PtrRtRect->left;
		A_PtrRtRect->bottom		=	A_PtrRtRect->top;
		if(A_ItmStX != A_ItmEdX)
		{
			A_PtrRtRect->right		=	((__s32)Tv_PtrCtlCalBase->DayVwSizeX * A_ItmEdX) + E_GrP5SkinT2CalMargin;
		}
		if(A_ItmStY != A_ItmEdY)
		{
			A_PtrRtRect->bottom		=	((__s32)Tv_PtrCtlCalBase->DayVwSizeY * A_ItmEdY) + E_GrP5SkinT2CalMargin;
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
void	Cls_GrP5SkinT2::LcCtlItmRectGridBase(__s32 A_ItmStX, __s32 A_ItmStY, __s32 A_ItmEdX, __s32 A_ItmEdY, Ptr_GrRect A_PtrRtRect)
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
void	Cls_GrP5SkinT2::LcCtlItmRectGrpHzDmap(__s32 A_ItmStX, __s32 A_ItmStY, __s32 A_ItmEdX, __s32 A_ItmEdY, Ptr_GrRect A_PtrRtRect)
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
	A_PtrRtRect->left			=	A_PtrRtRect->left + E_GrP5SkinT2GraphHzMargin;
	A_PtrRtRect->top			=	A_PtrRtRect->top + E_GrP5SkinT2GraphHzMargin;
	A_PtrRtRect->right		=	A_PtrRtRect->right + (__s32)Tv_PtrCtlGrpHzDmap->CellSizeX + E_GrP5SkinT2GraphHzMargin;
	A_PtrRtRect->bottom		=	A_PtrRtRect->bottom + (__s32)Tv_PtrCtlGrpHzDmap->CellSizeY + E_GrP5SkinT2GraphHzMargin;

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcCtlItmRectGrpHzT1(__s32 A_ItmStX, __s32 A_ItmStY, __s32 A_ItmEdX, __s32 A_ItmEdY, Ptr_GrRect A_PtrRtRect)
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
	if((0 > A_ItmStY) && (0 > A_ItmEdY))
	{
		// calculate indicater
		A_PtrRtRect->left			=	m_CtlRcScr.left + E_GrP5SkinT2GrpHzT1ContMgn + (Tv_PtrCtlGrpHzT1->CellSizeX * A_ItmStX);
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

	A_PtrRtRect->left			=	(Tv_StX * (__s32)Tv_PtrCtlGrpHzT1->CellSizeX) + m_CtlRcScr.left + E_GrP5SkinT2GrpHzT1ContMgn;
	A_PtrRtRect->top			=	(Tv_StY * (__s32)Tv_PtrCtlGrpHzT1->CellSizeY) + m_CtlRcScr.top + E_GrP5SkinT2GrpHzT1ContMgn + + (__s32)(__u32)Tv_PtrParaGrpHzT1->RulerHeight;
	if(Tv_StX == Tv_EdX)
	{
		A_PtrRtRect->right		=	A_PtrRtRect->left + (__s32)Tv_PtrCtlGrpHzT1->CellSizeX;
	}
	else
	{
		A_PtrRtRect->right		=	((Tv_EdX + 1) * (__s32)Tv_PtrCtlGrpHzT1->CellSizeX) + m_CtlRcScr.left + E_GrP5SkinT2GrpHzT1ContMgn;
	}
	if(Tv_StY == Tv_EdY)
	{
		A_PtrRtRect->bottom		=	A_PtrRtRect->top + (__s32)Tv_PtrCtlGrpHzT1->CellSizeY;
	}
	else
	{
		A_PtrRtRect->bottom		=	((Tv_EdY + 1) * (__s32)Tv_PtrCtlGrpHzT1->CellSizeY) + m_CtlRcScr.top + E_GrP5SkinT2GrpHzT1ContMgn + + (__s32)(__u32)Tv_PtrParaGrpHzT1->RulerHeight;
	}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcCtlItmRectTblBase(__s32 A_ItmStX, __s32 A_ItmStY, __s32 A_ItmEdX, __s32 A_ItmEdY, Ptr_GrRect A_PtrRtRect)
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
	if((__s32)Tv_PtrCtlTblBase->ColumCnt <= Tv_EdX)
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
	A_PtrRtRect->top		=	m_CtlRcScr.top + E_GrP5SkinT2TblMargin + ((__s32)Tv_PtrParaTblBase->ItemHeight * Tv_StY);
	if(Tv_StY == Tv_EdY)
	{
		A_PtrRtRect->bottom		=	A_PtrRtRect->top + (__s32)Tv_PtrParaTblBase->ItemHeight;
	}
	else
	{
		A_PtrRtRect->bottom		=	m_CtlRcScr.top + E_GrP5SkinT2TblMargin + ((__s32)Tv_PtrParaTblBase->ItemHeight * (Tv_EdY + 1));
	}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcCtlItmRectWinPopList(__s32 A_ItmStX, __s32 A_ItmStY, __s32 A_ItmEdX, __s32 A_ItmEdY, Ptr_GrRect A_PtrRtRect)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5SkinCtlWinPopItemBase		Tv_PtrCtlWinPopItemBase;

		__s32		Tv_ViewLimit;

		__s32		Tv_StY;
		__s32		Tv_EdY;
	// code --------------------

		// init
		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdWinPopItemBase);
		Tv_PtrCtlWinPopItemBase	=	(Ptr_GrP5SkinCtlWinPopItemBase)Tv_PtrHrcy->PtrSkinCtl;

		A_PtrRtRect->right	=	0;

		// check able
		if((0 == Tv_PtrCtlWinPopItemBase->ItemHeight) || (0 == Tv_PtrCtlWinPopItemBase->ItemWidth) ||
			(0 == Tv_PtrCtlWinPopItemBase->ViewCnt) || (0 == V_GrP5Env.ItemCnt))
		{
			return;
		}

		Tv_ViewLimit	=	Tv_PtrCtlWinPopItemBase->Sbar.Val + Tv_PtrCtlWinPopItemBase->ViewCnt - 1;
		if(Tv_ViewLimit >= (__s32)V_GrP5Env.ItemCnt)
		{
			Tv_ViewLimit		=	(__s32)V_GrP5Env.ItemCnt - 1;
		}

		if(((0 != A_ItmStX) && (0 != A_ItmEdX)) ||
			((Tv_PtrCtlWinPopItemBase->Sbar.Val > A_ItmStY) && (Tv_PtrCtlWinPopItemBase->Sbar.Val > A_ItmEdY)) || 
			((Tv_ViewLimit < A_ItmStY) && (Tv_ViewLimit < A_ItmEdY))
			)
		{
			return;
		}

		// check position
		Tv_StY		=	A_ItmStY;
		Tv_EdY		=	A_ItmEdY;
		if(A_ItmStY > A_ItmEdY)
		{
			Tv_StY		=	A_ItmEdY;
			Tv_EdY		=	A_ItmStY;
		}

		// patch position
		if(Tv_PtrCtlWinPopItemBase->Sbar.Val > Tv_StY)
		{
			Tv_StY	=	Tv_PtrCtlWinPopItemBase->Sbar.Val;
		}
		if(Tv_ViewLimit < Tv_EdY)
		{
			Tv_EdY	=	Tv_ViewLimit;
		}

		// calculate
		A_PtrRtRect->left		=	m_CtlRcScr.left + E_GrP5SkinT2PopListMargin;
		A_PtrRtRect->right	=	A_PtrRtRect->left + Tv_PtrCtlWinPopItemBase->ItemWidth;
		A_PtrRtRect->top		=	m_CtlRcScr.top + E_GrP5SkinT2PopListMargin + (Tv_PtrCtlWinPopItemBase->ItemHeight * (Tv_StY - Tv_PtrCtlWinPopItemBase->Sbar.Val));
		if(Tv_StY == Tv_EdY)
		{
			A_PtrRtRect->bottom	=	A_PtrRtRect->top + Tv_PtrCtlWinPopItemBase->ItemHeight;
		}
		else
		{
			A_PtrRtRect->bottom	=	m_CtlRcScr.top + E_GrP5SkinT2PopListMargin + (Tv_PtrCtlWinPopItemBase->ItemHeight * (Tv_EdY - Tv_PtrCtlWinPopItemBase->Sbar.Val + 1));
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcCtlItmRectWinPopMenu(__s32 A_ItmStX, __s32 A_ItmStY, __s32 A_ItmEdX, __s32 A_ItmEdY, Ptr_GrRect A_PtrRtRect)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5SkinCtlWinPopItemBase		Tv_PtrCtlWinPopItemBase;

		Ptr_GrP5SkinCtlWinPopMenu	Tv_PtrCtlWinPopMenu;

		__s32		Tv_StY;
		__s32		Tv_EdY;

	// code --------------------
		// init
		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdWinPopItemBase);
		Tv_PtrCtlWinPopItemBase	=	(Ptr_GrP5SkinCtlWinPopItemBase)Tv_PtrHrcy->PtrSkinCtl;

		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdWinPopMenu);
		Tv_PtrCtlWinPopMenu	=	(Ptr_GrP5SkinCtlWinPopMenu)Tv_PtrHrcy->PtrSkinCtl;

		A_PtrRtRect->right	=	0;

		// check able
		if((0 == Tv_PtrCtlWinPopItemBase->ItemHeight) || (0 == Tv_PtrCtlWinPopItemBase->ItemWidth) ||
			(0 == Tv_PtrCtlWinPopItemBase->ViewCnt) || (0 == Tv_PtrCtlWinPopMenu->ItemCnt))
		{
			return;
		}

		if(((0 != A_ItmStX) && (0 != A_ItmEdX)) ||
			((0 > A_ItmStY) && (0 > A_ItmEdY)) || (((__s32)Tv_PtrCtlWinPopMenu->ItemCnt <= A_ItmStY) && ((__s32)Tv_PtrCtlWinPopMenu->ItemCnt <= A_ItmEdY))
			)
		{
			return;
		}

		// check position
		Tv_StY		=	A_ItmStY;
		Tv_EdY		=	A_ItmEdY;
		if(A_ItmStY > A_ItmEdY)
		{
			Tv_StY		=	A_ItmEdY;
			Tv_EdY		=	A_ItmStY;
		}

		// patch position
		if(0 > Tv_StY)
		{
			Tv_StY	=	0;
		}
		if((__s32)Tv_PtrCtlWinPopMenu->ItemCnt <= Tv_EdY)
		{
			Tv_EdY	=	(__s32)Tv_PtrCtlWinPopMenu->ItemCnt - 1;
		}

		// calculate
		A_PtrRtRect->left		=	m_CtlRcScr.left + E_GrP5SkinT2PopMenuMargin;
		A_PtrRtRect->right	=	A_PtrRtRect->left + Tv_PtrCtlWinPopItemBase->ItemWidth;
		A_PtrRtRect->top		=	m_CtlRcScr.top + E_GrP5SkinT2PopMenuMargin + (Tv_PtrCtlWinPopItemBase->ItemHeight * Tv_StY);
		if(Tv_StY == Tv_EdY)
		{
			A_PtrRtRect->bottom	=	A_PtrRtRect->top + Tv_PtrCtlWinPopItemBase->ItemHeight;
		}
		else
		{
			A_PtrRtRect->bottom	=	m_CtlRcScr.top + E_GrP5SkinT2PopMenuMargin + (Tv_PtrCtlWinPopItemBase->ItemHeight * (Tv_EdY + 1));
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcCtlUpdtCalBase(void)
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
		Tv_Size	=	(m_CtlRcScr.right - m_CtlRcScr.left - (E_GrP5SkinT2CalMargin << 1)) / E_GrTimeDayOfWeek;
		if(0 < Tv_Size)
		{
			Tv_PtrCtlCalBase->DayVwSizeX	=	(__u32)Tv_Size;
		}

		Tv_PtrCtlCalBase->DayVwSizeY	=	0;
		if(0 != Tv_PtrCtlCalBase->DayVwSizeX)
		{
			Tv_Size	=	(m_CtlRcScr.bottom - m_CtlRcScr.top - (E_GrP5SkinT2CalMargin << 1)) / E_GrP5SkinCalDayLineCnt;
			if(0 < Tv_Size)
			{
				Tv_PtrCtlCalBase->DayVwSizeY	=	(__u32)Tv_Size;
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcCtlUpdtGridBase(void)
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
void	Cls_GrP5SkinT2::LcCtlUpdtGrpHzDmap(void)
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
	Tv_ValX		=	(m_CtlRcScr.right - m_CtlRcScr.left - (E_GrP5SkinT2GraphHzMargin << 1)) / (__s32)Tv_PtrCtlGrpHzBase->CellCntX;
	Tv_ValY		=	(m_CtlRcScr.right - m_CtlRcScr.left - (E_GrP5SkinT2GraphHzMargin << 1)) / (__s32)Tv_PtrCtlGrpHzDmap->CellSizeY;
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
void	Cls_GrP5SkinT2::LcCtlUpdtGrpHzT1(void)
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
	Tv_ValX		=	(m_CtlRcScr.right - m_CtlRcScr.left - (E_GrP5SkinT2GrpHzT1ContMgn << 1)) / (__s32)Tv_PtrCtlGrpHzT1->CellSizeX;
	Tv_ValY		=	(m_CtlRcScr.bottom - m_CtlRcScr.top - (E_GrP5SkinT2GrpHzT1ContMgn << 1) - (__s32)(__u32)Tv_PtrParaGrpHzT1->RulerHeight) / (__s32)Tv_PtrCtlGrpHzT1->CellSizeY;
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
		(__s32)(Tv_PtrCtlGrpHzBase->CellCntY * Tv_PtrCtlGrpHzT1->CellSizeY) + E_GrP5SkinT2GrpHzT1ContMgn;

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcCtlItemIdxCalBase(__s32 A_X, __s32 A_Y, __s32* A_PtrRtIdxX, __s32* A_PtrRtIdxY)
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
		if(((m_CtlRcScr.left + E_GrP5SkinT2CalMargin) > A_X) || ((m_CtlRcScr.right - E_GrP5SkinT2CalMargin) <= A_X) ||
			((m_CtlRcScr.top + E_GrP5SkinT2CalMargin) > A_Y) || ((m_CtlRcScr.bottom - E_GrP5SkinT2CalMargin) <= A_Y))
		{
			return;
		}

		// calculate
		*A_PtrRtIdxX	=	(A_X - (m_CtlRcScr.left + E_GrP5SkinT2CalMargin)) / (__s32)Tv_PtrCtlCalBase->DayVwSizeX;
		*A_PtrRtIdxY	=	(A_Y - (m_CtlRcScr.top + E_GrP5SkinT2CalMargin)) / (__s32)Tv_PtrCtlCalBase->DayVwSizeY;

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcCtlItemIdxGridBase(__s32 A_X, __s32 A_Y, __s32* A_PtrRtIdxX, __s32* A_PtrRtIdxY)
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
void	Cls_GrP5SkinT2::LcCtlItemIdxGrpHzDmap(__s32 A_X, __s32 A_Y, __s32* A_PtrRtIdxX, __s32* A_PtrRtIdxY)
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
	if(((m_CtlRcScr.left + E_GrP5SkinT2GraphHzMargin) > A_X) || ((m_CtlRcScr.right - E_GrP5SkinT2GraphHzMargin) <= A_X) ||
		((m_CtlRcScr.top + E_GrP5SkinT2GraphHzMargin) > A_Y) || ((m_CtlRcScr.bottom - E_GrP5SkinT2GraphHzMargin) <= A_Y))
	{
		return;
	}

	// calculate
	*A_PtrRtIdxX	=	(A_X - (m_CtlRcScr.left + E_GrP5SkinT2GraphHzMargin)) / (__s32)Tv_PtrCtlGrpHzDmap->CellSizeX;
	*A_PtrRtIdxY	=	(A_Y - (m_CtlRcScr.top + E_GrP5SkinT2GraphHzMargin)) / (__s32)Tv_PtrCtlGrpHzDmap->CellSizeY;

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcCtlItemIdxGrpHzT1(__s32 A_X, __s32 A_Y, __s32* A_PtrRtIdxX, __s32* A_PtrRtIdxY)
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

	Tv_RcCont.left		=	m_CtlRcScr.left + E_GrP5SkinT2GrpHzT1ContMgn;
	Tv_RcCont.top			=	m_CtlRcScr.top + (__s32)(__u32)Tv_PtrParaGrpHzT1->RulerHeight + E_GrP5SkinT2GrpHzT1ContMgn;

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
void	Cls_GrP5SkinT2::LcCtlItemIdxSpinBase(__s32 A_X, __s32 A_Y, __s32* A_PtrRtIdxX, __s32* A_PtrRtIdxY)
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
		(A_Y < Tv_PtrCtlSpinBase->RcItems[E_GrP5SkinSpinIdxBtnDec].bottom))
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
void	Cls_GrP5SkinT2::LcCtlItemIdxTblBase(__s32 A_X, __s32 A_Y, __s32* A_PtrRtIdxX, __s32* A_PtrRtIdxY)
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
	if((A_X < (m_CtlRcScr.left + E_GrP5SkinT2TblMargin)) || (A_X >= (m_CtlRcScr.right - E_GrP5SkinT2TblMargin)) ||
		(A_Y < (m_CtlRcScr.top + E_GrP5SkinT2TblMargin)) || (A_Y >= (m_CtlRcScr.bottom - E_GrP5SkinT2TblMargin)))
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
	*A_PtrRtIdxY	=	(A_Y - (m_CtlRcScr.top + E_GrP5SkinT2TblMargin)) / (__s32)(__u32)Tv_PtrParaTblBase->ItemHeight;
	// check over
	if((*A_PtrRtIdxY) >= (__s32)Tv_PtrCtlTblBase->ViewLineCnt)
	{
		*A_PtrRtIdxY	=	-1;
	}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcCtlItemIdxWinPopList(__s32 A_X, __s32 A_Y, __s32* A_PtrRtIdxX, __s32* A_PtrRtIdxY)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5SkinCtlWinPopItemBase		Tv_PtrCtlWinPopItemBase;

		__s32	Tv_IdxY;
	// code --------------------
		// init
		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdWinPopItemBase);
		Tv_PtrCtlWinPopItemBase	=	(Ptr_GrP5SkinCtlWinPopItemBase)Tv_PtrHrcy->PtrSkinCtl;

		// check able
		if((0 == Tv_PtrCtlWinPopItemBase->ItemHeight) || (0 == Tv_PtrCtlWinPopItemBase->ItemWidth) ||
			(0 == Tv_PtrCtlWinPopItemBase->ViewCnt) || (0 == V_GrP5Env.ItemCnt))
		{
			return;
		}

		// check contents range
		if((A_X < (m_CtlRcScr.left + E_GrP5SkinT2PopListMargin)) || (A_X >= (m_CtlRcScr.left + E_GrP5SkinT2PopListMargin + (__s32)Tv_PtrCtlWinPopItemBase->ItemWidth)) ||
			(A_Y < m_CtlRcScr.top + E_GrP5SkinT2PopListMargin) )
		{
			return;
		}

		// calculate y
		Tv_IdxY	=	((A_Y - (m_CtlRcScr.top + E_GrP5SkinT2PopListMargin)) / Tv_PtrCtlWinPopItemBase->ItemHeight) + Tv_PtrCtlWinPopItemBase->Sbar.Val;
		
		if(Tv_IdxY < (__s32)V_GrP5Env.ItemCnt)
		{
			*A_PtrRtIdxX	=	0;
			*A_PtrRtIdxY	=	Tv_IdxY;
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcCtlItemIdxWinPopMenu(__s32 A_X, __s32 A_Y, __s32* A_PtrRtIdxX, __s32* A_PtrRtIdxY)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5SkinCtlWinPopItemBase		Tv_PtrCtlWinPopItemBase;

		Ptr_GrP5SkinCtlWinPopMenu	Tv_PtrCtlWinPopMenu;

		__s32	Tv_IdxY;
	// code --------------------
		// init
		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdWinPopItemBase);
		Tv_PtrCtlWinPopItemBase	=	(Ptr_GrP5SkinCtlWinPopItemBase)Tv_PtrHrcy->PtrSkinCtl;

		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdWinPopMenu);
		Tv_PtrCtlWinPopMenu	=	(Ptr_GrP5SkinCtlWinPopMenu)Tv_PtrHrcy->PtrSkinCtl;

		// check able
		if((0 == Tv_PtrCtlWinPopItemBase->ItemHeight) || (0 == Tv_PtrCtlWinPopItemBase->ItemWidth) ||
			(0 == Tv_PtrCtlWinPopItemBase->ViewCnt) || (0 == Tv_PtrCtlWinPopMenu->ItemCnt))
		{
			return;
		}

		// check contents range
		if((A_X < (m_CtlRcScr.left + E_GrP5SkinT2PopMenuMargin)) || (A_X >= (m_CtlRcScr.left + E_GrP5SkinT2PopMenuMargin + (__s32)Tv_PtrCtlWinPopItemBase->ItemWidth)) ||
			(A_Y < m_CtlRcScr.top + E_GrP5SkinT2PopMenuMargin))
		{
			return;
		}

		// calculate y
		Tv_IdxY	=	((A_Y - (m_CtlRcScr.top + E_GrP5SkinT2PopMenuMargin)) / Tv_PtrCtlWinPopItemBase->ItemHeight);

		if(Tv_IdxY < (__s32)Tv_PtrCtlWinPopMenu->ItemCnt)
		{
			*A_PtrRtIdxX	=	0;
			*A_PtrRtIdxY	=	Tv_IdxY;
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcDrawBtnStd(__u32 A_Stat)
{
	// local -------------------
	// code --------------------

		// draw background
		if(0 == m_PtrCtlSkinPara->SubType)
		{
			// standard button
			LcDrawBtnSubType0(m_PtrCtlSkinPara, &m_CtlRcScr, A_Stat);
		}
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
void	Cls_GrP5SkinT2::LcBtnBaseStatUpdate(void)
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
void	Cls_GrP5SkinT2::LcChkBaseStatUpdate(void)
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
void	Cls_GrP5SkinT2::LcCtlDrawBtnImgBase(void)
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
void	Cls_GrP5SkinT2::LcCtlDrawLblBase(void)
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
void	Cls_GrP5SkinT2::LcCtlUpdtLblTid(void)
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
void	Cls_GrP5SkinT2::LcCtlUpdtPnlGrpBar(void)
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
void	Cls_GrP5SkinT2::LcCtlUpdtPnlGrpLine(void)
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
void	Cls_GrP5SkinT2::LcCtlUpdtQuadT1(void)
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
void	Cls_GrP5SkinT2::LcCtlUpdtSpinBase(void)
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
void	Cls_GrP5SkinT2::LcCtlUpdtTabBase(void)
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
void	Cls_GrP5SkinT2::LcCtlUpdtTabTiRel(void)
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
void	Cls_GrP5SkinT2::LcCtlUpdtTblBase(void)
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
		Tv_PtrCtlTblBase->ViewLineMax	=	(__u32)(m_CtlRcScr.bottom - m_CtlRcScr.top - (E_GrP5SkinT2TblMargin << 1)) /
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
		Tv_BaseX	=	m_CtlRcScr.left + E_GrP5SkinT2TblMargin;
		for(Tv_ColIdx = 0; Tv_ColIdx < Tv_PtrCtlTblBase->ColumCnt; Tv_ColIdx++)
		{
			Tv_PtrCtlTblBase->Cols[Tv_ColIdx].Pos		=	Tv_BaseX;
			Tv_PtrCtlTblBase->Cols[Tv_ColIdx].Width	=	(__s32)Tv_PtrParaTblBase->ColSize[Tv_ColIdx];
			if((m_CtlRcScr.right - E_GrP5SkinT2TblMargin) < (Tv_BaseX + Tv_PtrCtlTblBase->Cols[Tv_ColIdx].Width))
			{
				// patch size
				Tv_PtrCtlTblBase->Cols[Tv_ColIdx].Width	=	(m_CtlRcScr.right - E_GrP5SkinT2TblMargin) - Tv_BaseX;
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
void	Cls_GrP5SkinT2::LcCtlUpdtPnlDir(void)
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
	Tv_IdcSize	=	E_GrP5SkinT2PnlDirIdcDfltSize + (__s32)m_PtrCtlSkinPara->ParaByte0;

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
void	Cls_GrP5SkinT2::LcCtlUpdtWinPopMenu(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5SkinCtlWinPopItemBase		Tv_PtrCtlWinPopItemBase;

		Ptr_GrP5SkinCtlWinPopMenu		Tv_PtrCtlWinPopMenu;

		Cls_GrP5Font*	Tv_Font;
		__u32	Tv_FontHeight;
	// code --------------------
		// init
		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdWinPopItemBase);
		Tv_PtrCtlWinPopItemBase	=	(Ptr_GrP5SkinCtlWinPopItemBase)Tv_PtrHrcy->PtrSkinCtl;

		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdWinPopMenu);
		Tv_PtrCtlWinPopMenu	=	(Ptr_GrP5SkinCtlWinPopMenu)Tv_PtrHrcy->PtrSkinCtl;

		// not use scroll bar
		Tv_PtrCtlWinPopItemBase->Sbar.RcArea.right		=	0;

		Tv_PtrCtlWinPopItemBase->ItemWidth	=	0;
		Tv_PtrCtlWinPopItemBase->ItemHeight	=	0;

		// get font
		Tv_Font		=	(Cls_GrP5Font*)GrP5FontGet(V_GrP5Env.PopCtl.FontId);
		if(NULL != Tv_Font)
		{
			Tv_FontHeight	=	(__u32)Tv_Font->HeightGet();

			// calculate item height
			Tv_PtrCtlWinPopItemBase->ItemHeight	=	(__s32)Tv_FontHeight + (E_GrP5SkinT2PopMenuItmMgn << 1);
			Tv_PtrCtlWinPopItemBase->ItemWidth	=	m_CtlRcScr.right - m_CtlRcScr.left - (E_GrP5SkinT2PopMenuMargin << 1);
			if(0 >= Tv_PtrCtlWinPopItemBase->ItemWidth)
			{
				Tv_PtrCtlWinPopItemBase->ItemHeight	=	0;
			}
		}

		// calculate item count
		Tv_PtrCtlWinPopItemBase->ViewCnt	=	0;
		if(0 != Tv_PtrCtlWinPopItemBase->ItemHeight)
		{
			Tv_PtrCtlWinPopItemBase->ViewCnt	=	(m_CtlRcScr.bottom - m_CtlRcScr.top - (E_GrP5SkinT2PopMenuMargin << 1)) /
				Tv_PtrCtlWinPopItemBase->ItemHeight;
			if(0 >= Tv_PtrCtlWinPopItemBase->ViewCnt)
			{
				Tv_PtrCtlWinPopItemBase->ViewCnt	=	0;
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::CtlCalcSbarVtByValue(void* A_ObjUi, Ptr_Grp5SkinSbarCtl A_PtrSbarCtl, __u32 A_Value)
{
	// local -------------------
	// code --------------------
		// check able
		if((0 >= A_PtrSbarCtl->RcArea.right) || (0 >= A_PtrSbarCtl->RcTrack.right))
		{
			A_PtrSbarCtl->Pos		=	0;
			return;
		}

		// check max value
		if(0 == A_PtrSbarCtl->Max)
		{
			A_PtrSbarCtl->Pos		=	0;
			return;
		}

		// patch value
		A_PtrSbarCtl->Val		=	(__s32)A_Value;

		if(A_PtrSbarCtl->Val >= A_PtrSbarCtl->Max)
		{
			A_PtrSbarCtl->Val	=	A_PtrSbarCtl->Max;
		}
		else if(0 > A_PtrSbarCtl->Val)
		{
			A_PtrSbarCtl->Val	=	0;
		}

		// calculate position
		A_PtrSbarCtl->Pos		=	(A_PtrSbarCtl->Val * A_PtrSbarCtl->End) / A_PtrSbarCtl->Max;

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::CtlCalcSbarVtByPos(void* A_ObjUi, Ptr_Grp5SkinSbarCtl A_PtrSbarCtl, __u32 A_Pos)
{
	// local -------------------
	// code --------------------
		// check able
		if((0 >= A_PtrSbarCtl->RcArea.right) || (0 >= A_PtrSbarCtl->RcTrack.right))
		{
			A_PtrSbarCtl->Pos		=	0;
			return;
		}

		// check max value
		if((0 == A_PtrSbarCtl->Max) || (0 == A_PtrSbarCtl->End))
		{
			A_PtrSbarCtl->Pos		=	0;
			return;
		}

		// get position
		A_PtrSbarCtl->Pos		=	(__s32)A_Pos;
		if(A_PtrSbarCtl->Pos >= A_PtrSbarCtl->End)
		{
			A_PtrSbarCtl->Pos	=	A_PtrSbarCtl->End;
		}
		else if(0 > A_PtrSbarCtl->Pos)
		{
			A_PtrSbarCtl->Pos	=	0;
		}

		// calculate value
		A_PtrSbarCtl->Val		=	(A_PtrSbarCtl->Pos * A_PtrSbarCtl->Max) / A_PtrSbarCtl->End;

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcInitSbarVt(Ptr_Grp5SkinSbarCtl A_PtrCtl)
{
	// local -------------------
	// code --------------------
		// calculate rect dec
		A_PtrCtl->RcBtnDec.left		=	A_PtrCtl->RcArea.left;
		A_PtrCtl->RcBtnDec.top		=	A_PtrCtl->RcArea.top;
		A_PtrCtl->RcBtnDec.right	=	A_PtrCtl->RcArea.right;
		A_PtrCtl->RcBtnDec.bottom	=	A_PtrCtl->RcArea.top + E_GrP5SkinT2SbarVtBtnHeight;

		// calculate rect inc
		A_PtrCtl->RcBtnInc.left		=	A_PtrCtl->RcArea.left;
		A_PtrCtl->RcBtnInc.top		=	A_PtrCtl->RcArea.bottom - E_GrP5SkinT2SbarVtBtnHeight;
		A_PtrCtl->RcBtnInc.right	=	A_PtrCtl->RcArea.right;
		A_PtrCtl->RcBtnInc.bottom	=	A_PtrCtl->RcArea.bottom;

		// check able
		if(A_PtrCtl->RcBtnDec.bottom > A_PtrCtl->RcBtnInc.top)
		{
			A_PtrCtl->RcArea.right		=	0;
			return;
		}

		// check thumb able
		A_PtrCtl->RcTrack.right		=	0;
		if(E_GrP5SkinT2SbarVtThumbSize < (A_PtrCtl->RcBtnInc.top - A_PtrCtl->RcBtnDec.bottom))
		{
			// calculate track
			A_PtrCtl->RcTrack.left		=	A_PtrCtl->RcArea.left;
			A_PtrCtl->RcTrack.top			=	A_PtrCtl->RcBtnDec.bottom;
			A_PtrCtl->RcTrack.right		=	A_PtrCtl->RcArea.right;
			A_PtrCtl->RcTrack.bottom	=	A_PtrCtl->RcBtnInc.top;

			A_PtrCtl->ThumbSize				=	E_GrP5SkinT2SbarVtThumbSize;

			// calculate end
			A_PtrCtl->End					=	A_PtrCtl->RcTrack.bottom - A_PtrCtl->RcTrack.top - E_GrP5SkinT2SbarVtThumbSize;
			if(0 >= A_PtrCtl->End)
			{
				A_PtrCtl->RcTrack.right		=	0;
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcCtlUpdtWinPopList(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5SkinCtlWinPopItemBase		Tv_PtrCtlWinPopItemBase;

		Cls_GrP5Font*	Tv_Font;
		__u32	Tv_FontHeight;
	// code --------------------
		// init
		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdWinPopItemBase);
		Tv_PtrCtlWinPopItemBase	=	(Ptr_GrP5SkinCtlWinPopItemBase)Tv_PtrHrcy->PtrSkinCtl;

		Tv_PtrCtlWinPopItemBase->ItemWidth	=	0;
		Tv_PtrCtlWinPopItemBase->ItemHeight	=	0;

		// get font
		Tv_Font		=	(Cls_GrP5Font*)GrP5FontGet(V_GrP5Env.PopCtl.FontId);
		if(NULL != Tv_Font)
		{
			Tv_FontHeight	=	(__u32)Tv_Font->HeightGet();

			// calculate item height
			Tv_PtrCtlWinPopItemBase->ItemHeight	=	(__s32)Tv_FontHeight + (E_GrP5SkinT2PopListItmMgn << 1);
			Tv_PtrCtlWinPopItemBase->ItemWidth	=	m_CtlRcScr.right - m_CtlRcScr.left - (E_GrP5SkinT2PopListMargin << 1);
			if(0 >= Tv_PtrCtlWinPopItemBase->ItemWidth)
			{
				Tv_PtrCtlWinPopItemBase->ItemHeight	=	0;
			}
		}

		// calculate item count
		Tv_PtrCtlWinPopItemBase->ViewCnt	=	0;
		if(0 != Tv_PtrCtlWinPopItemBase->ItemHeight)
		{
			Tv_PtrCtlWinPopItemBase->ViewCnt	=	(m_CtlRcScr.bottom - m_CtlRcScr.top - (E_GrP5SkinT2PopListMargin << 1)) /
				Tv_PtrCtlWinPopItemBase->ItemHeight;
			if(0 >= Tv_PtrCtlWinPopItemBase->ViewCnt)
			{
				Tv_PtrCtlWinPopItemBase->ViewCnt	=	0;
			}
		}
		
		// scroll bar
		Tv_PtrCtlWinPopItemBase->Sbar.RcArea.right		=	0;
		if((0 != Tv_PtrCtlWinPopItemBase->ViewCnt) && (Tv_PtrCtlWinPopItemBase->ViewCnt < (__s32)V_GrP5Env.ItemCnt))
		{
			// calculate area
			Tv_PtrCtlWinPopItemBase->Sbar.RcArea.left		=	m_CtlRcScr.right - E_GrP5SkinT2PopListMargin - E_GrP5SkinT2SbarVtWidth;
			Tv_PtrCtlWinPopItemBase->Sbar.RcArea.right	=	m_CtlRcScr.right - E_GrP5SkinT2PopListMargin;
			Tv_PtrCtlWinPopItemBase->Sbar.RcArea.top		=	m_CtlRcScr.top + E_GrP5SkinT2PopListMargin;
			Tv_PtrCtlWinPopItemBase->Sbar.RcArea.bottom	=	m_CtlRcScr.bottom - E_GrP5SkinT2PopListMargin;

			LcInitSbarVt(&Tv_PtrCtlWinPopItemBase->Sbar);

			// calculate max value
			Tv_PtrCtlWinPopItemBase->Sbar.Max		=	(__s32)V_GrP5Env.ItemCnt - Tv_PtrCtlWinPopItemBase->ViewCnt;
		}

		// patch item width
		if(0 < Tv_PtrCtlWinPopItemBase->Sbar.RcArea.right)
		{
			Tv_PtrCtlWinPopItemBase->ItemWidth	=	Tv_PtrCtlWinPopItemBase->Sbar.RcArea.left - m_CtlRcScr.left - E_GrP5SkinT2PopListSbarMgn;
		}
		if(0 >= Tv_PtrCtlWinPopItemBase->ItemWidth)
		{
			Tv_PtrCtlWinPopItemBase->ItemWidth	=	0;
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcCtlUpdtGrpHzT2(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoGrpHzBasePara		Tv_PtrParaGrpHzBase;
		Ptr_GrP5SkinCtlGrpHzBase	Tv_PtrCtlGrpHzBase;

		Ptr_GrP5UoGrpHzT2Para		Tv_PtrParaGrpHzT2;
		Ptr_GrP5SkinCtlGrpHzT2	Tv_PtrCtlGrpHzT2;

		Ptr_Grp5SkinT2GrpHzT2Rul	Tv_PtrRul;
		__u32			Tv_TimeWk;
		__u32			Tv_TimeRm;

		__u32			Tv_TimeBas;

		__u32			Tv_Hour;
		__u32			Tv_Min;

		__u32			Tv_PxCnt;
		__u32			Tv_SecCnt;

		WCHAR*		Tv_StrWk;

		St_GrRect	Tv_RcWork;

	// code --------------------
		// init
		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdGrpHzBase);
		Tv_PtrParaGrpHzBase		=	(Ptr_GrP5UoGrpHzBasePara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlGrpHzBase		=	(Ptr_GrP5SkinCtlGrpHzBase)Tv_PtrHrcy->PtrSkinCtl;

		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdGrpHzT2);
		Tv_PtrParaGrpHzT2			=	(Ptr_GrP5UoGrpHzT2Para)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlGrpHzT2			=	(Ptr_GrP5SkinCtlGrpHzT2)Tv_PtrHrcy->PtrSkinCtl;

		// init
		Tv_PtrCtlGrpHzBase->CellCntX	=	0;
		Tv_PtrCtlGrpHzBase->CellCntY	=	0;

		Tv_PtrCtlGrpHzT2->PxPerSec		=	0;
		Tv_PtrCtlGrpHzT2->RcContArea.right	=	0;
		Tv_PtrCtlGrpHzT2->RcRule.right			=	0;
		Tv_PtrCtlGrpHzT2->RulTxtCnt					=	0;

		// calculate ruler text area
		if((0 == Tv_PtrParaGrpHzT2->ViewLineCnt) || (0 == Tv_PtrParaGrpHzBase->LineHeight) || (E_GrP5SkinT2GrpHzT2RulLhHeight >= Tv_PtrParaGrpHzT2->RulerHeight) || (0 == Tv_PtrParaGrpHzT2->RulerTxtWidth))
		{
			return;
		}

		if((__s32)(__u32)(Tv_PtrParaGrpHzT2->RulerHeight) >= (m_CtlRcScr.bottom - m_CtlRcScr.top))
		{
			return;
		}

		Tv_PtrCtlGrpHzT2->VwCntY	=	(__u32)Tv_PtrParaGrpHzT2->ViewLineCnt;
		if(Tv_PtrParaGrpHzT2->IsMergeCh)
		{
			Tv_PtrCtlGrpHzT2->VwCntY	=	1;
		}
		else
		{
			if(E_GrDvrMaxChCnt <= Tv_PtrCtlGrpHzT2->StartCh)
			{
				Tv_PtrCtlGrpHzT2->VwCntY	=	0;
			}
			else
			{
				if((Tv_PtrCtlGrpHzT2->StartCh + Tv_PtrParaGrpHzT2->ViewLineCnt) > E_GrDvrMaxChCnt)
				{
					Tv_PtrCtlGrpHzT2->VwCntY	=	E_GrDvrMaxChCnt - (__u32)Tv_PtrCtlGrpHzT2->StartCh;
				}
			}
		}

		if(E_Grp5SkinT2GrpHzT2ZoomCnt <= Tv_PtrCtlGrpHzT2->ZoomLvl)
		{
			Tv_PtrCtlGrpHzT2->ZoomLvl	=	E_Grp5SkinT2GrpHzT2ZoomCnt - 1;
		}

		Tv_PtrRul	=	&V_GrP5SkinT2GrpHzT2Rul[Tv_PtrCtlGrpHzT2->ZoomLvl];

		// set
		Tv_PtrCtlGrpHzT2->RcRule.left			=	m_CtlRcScr.left;
		Tv_PtrCtlGrpHzT2->RcRule.top			=	m_CtlRcScr.top;
		Tv_PtrCtlGrpHzT2->RcRule.right		=	m_CtlRcScr.right;
		Tv_PtrCtlGrpHzT2->RcRule.bottom		=	Tv_PtrCtlGrpHzT2->RcRule.top + (__s32)(__u32)(Tv_PtrParaGrpHzT2->RulerHeight);

		Tv_PtrCtlGrpHzT2->RcContArea.left		=	m_CtlRcScr.left;
		Tv_PtrCtlGrpHzT2->RcContArea.top		=	Tv_PtrCtlGrpHzT2->RcRule.bottom;
		Tv_PtrCtlGrpHzT2->RcContArea.right	=	m_CtlRcScr.right;
		Tv_PtrCtlGrpHzT2->RcContArea.bottom	=	m_CtlRcScr.bottom;

		Tv_PtrCtlGrpHzT2->PxPerSec	=	Tv_PtrRul->PxPerSec;
		Tv_PtrCtlGrpHzT2->RulPerPx	=	Tv_PtrRul->RulPerPx;
		Tv_PtrCtlGrpHzT2->RulPerSec	=	Tv_PtrRul->RulPerSec;

		// calculate time limite
		Tv_PxCnt		=	(__u32)(Tv_PtrCtlGrpHzT2->RcContArea.right - Tv_PtrCtlGrpHzT2->RcContArea.left);
		Tv_SecCnt		=	Tv_PxCnt * Tv_PtrCtlGrpHzT2->PxPerSec;
		Tv_PtrCtlGrpHzT2->VwCntX	=	Tv_PxCnt;
		Tv_PtrCtlGrpHzT2->TimeLen	=	Tv_SecCnt;

		// patch start time
		Tv_PtrCtlGrpHzT2->TimeScl	=	Tv_PtrCtlGrpHzT2->TimeScl - (Tv_PtrCtlGrpHzT2->TimeScl % Tv_PtrRul->RulPerSec);

		if((Tv_PtrCtlGrpHzT2->TimeLoad + E_GrTimeSecOfDay) <= (Tv_PtrCtlGrpHzT2->TimeScl + Tv_PtrCtlGrpHzT2->TimeLen))
		{
			Tv_PtrCtlGrpHzT2->TimeScl	=	Tv_PtrCtlGrpHzT2->TimeLoad + E_GrTimeSecOfDay - Tv_PtrCtlGrpHzT2->TimeLen;
		}

		if(Tv_PtrCtlGrpHzT2->TimeLoad > Tv_PtrCtlGrpHzT2->TimeScl)
		{
			Tv_PtrCtlGrpHzT2->TimeScl	=	Tv_PtrCtlGrpHzT2->TimeLoad;
		}

		// calculate ruler text
		Tv_TimeWk		=	Tv_PtrCtlGrpHzT2->TimeScl;
		Tv_TimeRm		=	Tv_PtrCtlGrpHzT2->TimeScl % Tv_PtrRul->TxtPerSec;
		if(0 != Tv_TimeRm)
		{
			Tv_TimeWk		=	Tv_PtrCtlGrpHzT2->TimeScl - Tv_TimeRm + Tv_PtrRul->TxtPerSec;
		}

		Tv_TimeBas		=	Tv_TimeWk % E_GrTimeSecOfDay;

		Tv_RcWork.left		=	m_CtlRcScr.left + ((Tv_TimeWk - Tv_PtrCtlGrpHzT2->TimeScl) / Tv_PtrRul->PxPerSec);
		Tv_RcWork.top			=	m_CtlRcScr.top;
		Tv_RcWork.bottom	=	Tv_RcWork.top + (__s32)(__u32)(Tv_PtrParaGrpHzT2->RulerHeight - E_GrP5SkinT2GrpHzT2RulLhHeight);

		while((Tv_RcWork.left < m_CtlRcScr.right) && (Tv_TimeWk < (Tv_PtrCtlGrpHzT2->TimeLoad + E_GrTimeSecOfDay)))
		{
			Tv_RcWork.right		=	Tv_RcWork.left + (__s32)(__u32)Tv_PtrParaGrpHzT2->RulerTxtWidth;
			if(m_CtlRcScr.right < Tv_RcWork.right )
			{
				break;
			}

			// get hour and minute
			Tv_Hour		=	Tv_TimeBas / E_GrTimeSecOfHour;
			Tv_Min		=	(Tv_TimeBas % E_GrTimeSecOfHour) / E_GrTimeSecOfMin;

			// make text
			GrStrIntToWstr(Tv_PtrCtlGrpHzT2->RulTxtTbl[Tv_PtrCtlGrpHzT2->RulTxtCnt].Str, (__s32)Tv_Hour, 2, TRUE, FALSE);
			Tv_StrWk	=	GrStrAddWchar(Tv_PtrCtlGrpHzT2->RulTxtTbl[Tv_PtrCtlGrpHzT2->RulTxtCnt].Str, ':');
			GrStrIntToWstr(Tv_StrWk, (__s32)Tv_Min, 2, TRUE, FALSE);

			// set rect
			Tv_PtrCtlGrpHzT2->RulTxtTbl[Tv_PtrCtlGrpHzT2->RulTxtCnt].Rc		=	Tv_RcWork;

			// next
			Tv_TimeWk		=	Tv_TimeWk + Tv_PtrRul->TxtPerSec;
			Tv_TimeBas	=	Tv_TimeBas + Tv_PtrRul->TxtPerSec;
			Tv_RcWork.left	=	Tv_RcWork.left + (__s32)Tv_PtrRul->TxtPerPx;
			Tv_PtrCtlGrpHzT2->RulTxtCnt ++;
			if(E_GrP5SkinGrpHzT2RulTxtMaxCnt <= Tv_PtrCtlGrpHzT2->RulTxtCnt)
			{
				break;
			}
		}

		// build contents table
		LcGrpHzT2ContUpdate();
}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcCtlDrawBtnImgTid(void)
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
void	Cls_GrP5SkinT2::LcCtlDrawBtnLtxt(void)
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
void	Cls_GrP5SkinT2::LcCtlDrawBtnSint(void)
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
void	Cls_GrP5SkinT2::LcCtlDrawBtnStxt(void)
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
void	Cls_GrP5SkinT2::LcCtlDrawBtnTabBase(void)
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
void	Cls_GrP5SkinT2::LcCtlDrawBtnTabTid(void)
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
void	Cls_GrP5SkinT2::LcCtlDrawBtnTid(void)
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
void	Cls_GrP5SkinT2::LcCtlDrawBtnTime(void)
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
void	Cls_GrP5SkinT2::LcCtlDrawBtnVkey(void)
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
void	Cls_GrP5SkinT2::LcCtlDrawChkImgLtBase(void)
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
void	Cls_GrP5SkinT2::LcCtlDrawChkImgTiBase(void)
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
void	Cls_GrP5SkinT2::LcCtlDrawChkTiBase(void)
{
	// local -------------------
	Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
	Ptr_GrP5SkinCtlBtnBase	Tv_PtrCtlBtnBase;

	Ptr_GrP5UoBtnTidPara		Tv_PtrParaBtnTid;

	Ptr_GrP5UoChkTiBasePara	Tv_PtrParaChkTiBase;

	St_GrRect	Tv_RcScr;
	WCHAR*			Tv_Str;
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
			GrP5PntReqTxtDraw(&Tv_RcScr, Tv_Str, (__u16)m_CtlStat);	// txt buf ok
		}
	}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcCtlDrawGridSmd(void)
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
void	Cls_GrP5SkinT2::LcCtlDrawGridSrscd(void)
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
	if((0 == Tv_PtrCtlGridBase->CellSizeX) || (0 == Tv_PtrCtlGridBase->CellSizeY))
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
void	Cls_GrP5SkinT2::LcCtlDrawGrpHzDmap(void)
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
	Tv_RcScr.top		=	m_CtlRcScr.top + E_GrP5SkinT2GraphHzMargin;
	for(Tv_IdxY = 0; Tv_IdxY < Tv_PtrCtlGrpHzDmap->VwCntY; Tv_IdxY++)
	{
		Tv_RcScr.bottom	=	Tv_RcScr.top + (__s32)Tv_PtrCtlGrpHzDmap->CellSizeY;

		Tv_RcScr.left		=	m_CtlRcScr.left + E_GrP5SkinT2GraphHzMargin;
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
void	Cls_GrP5SkinT2::LcDrawCtlGrpHzT1Ruler(void)
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
			Tv_Rc.left		=	m_CtlRcScr.left + E_GrP5SkinT2GrpHzT1ContMgn;
			Tv_Rc.top			=	m_CtlRcScr.top;
			Tv_Rc.right		=	Tv_Rc.left + (__s32)(__u32)Tv_PtrParaGrpHzT1->RulerUnitWidth;
			Tv_Rc.bottom	=	Tv_Rc.top + (__s32)(__u32)Tv_PtrParaGrpHzT1->RulerHeight;
			GrP5PntReqTxtDraw(&Tv_Rc, V_GrP5SkinT2StrRulerHrT1[Tv_RulIdx], (__u16)m_CtlStat);		// txt buf ok
			Tv_RulIdx ++;
			// draw 6
			Tv_Rc.left		=	Tv_Rc.left + Tv_StepX;
			Tv_Rc.right		=	Tv_Rc.left + (__s32)(__u32)Tv_PtrParaGrpHzT1->RulerUnitWidth;

			GrP5PntReqTxtDraw(&Tv_Rc, V_GrP5SkinT2StrRulerHrT1[Tv_RulIdx], (__u16)m_CtlStat);		// txt buf ok
			Tv_RulIdx ++;

			// draw 12
			Tv_Rc.left		=	Tv_Rc.left + Tv_StepX;
			Tv_Rc.right		=	Tv_Rc.left + (__s32)(__u32)Tv_PtrParaGrpHzT1->RulerUnitWidth;

			GrP5PntReqTxtDraw(&Tv_Rc, V_GrP5SkinT2StrRulerHrT1[Tv_RulIdx], (__u16)m_CtlStat);		// txt buf ok
			Tv_RulIdx ++;

			// draw 18
			Tv_Rc.left		=	Tv_Rc.left + Tv_StepX;
			Tv_Rc.right		=	Tv_Rc.left + (__s32)(__u32)Tv_PtrParaGrpHzT1->RulerUnitWidth;

			GrP5PntReqTxtDraw(&Tv_Rc, V_GrP5SkinT2StrRulerHrT1[Tv_RulIdx], (__u16)m_CtlStat);		// txt buf ok
			Tv_RulIdx ++;

			// darw 23
			Tv_Rc.right		=	Tv_Rc.left + Tv_StepX;
			Tv_Rc.left		=	Tv_Rc.right - (__s32)(__u32)Tv_PtrParaGrpHzT1->RulerUnitWidth;

			GrP5PntReqTxtDraw(&Tv_Rc, V_GrP5SkinT2StrRulerHrT1[Tv_RulIdx], (__u16)m_CtlStat);		// txt buf ok
			Tv_RulIdx ++;

		}
		else
		{
			// minute mode
			Tv_StepX			=	(__s32)Tv_PtrCtlGrpHzT1->CellSizeX * 10;
			Tv_BaseX			=	m_CtlRcScr.left + E_GrP5SkinT2GrpHzT1ContMgn;

			// draw 0
			Tv_Rc.left		=	Tv_BaseX;
			Tv_Rc.top			=	m_CtlRcScr.top;
			Tv_Rc.right		=	Tv_Rc.left + (__s32)(__u32)Tv_PtrParaGrpHzT1->RulerUnitWidth;
			Tv_Rc.bottom	=	Tv_Rc.top + (__s32)(__u32)Tv_PtrParaGrpHzT1->RulerHeight;

			GrP5PntReqTxtDraw(&Tv_Rc, V_GrP5SkinT2StrRulerMinT1[Tv_RulIdx], (__u16)m_CtlStat);		// txt buf ok
			Tv_RulIdx ++;
			Tv_BaseX			=	Tv_BaseX + Tv_StepX;

			// draw 10
			Tv_Rc.left		=	Tv_BaseX - (((__s32)(__u32)Tv_PtrParaGrpHzT1->RulerUnitWidth - Tv_PtrCtlGrpHzT1->CellSizeX) >> 1);
			Tv_Rc.right		=	Tv_Rc.left + (__s32)(__u32)Tv_PtrParaGrpHzT1->RulerUnitWidth;

			GrP5PntReqTxtDraw(&Tv_Rc, V_GrP5SkinT2StrRulerMinT1[Tv_RulIdx], (__u16)m_CtlStat);		// txt buf ok
			Tv_RulIdx ++;
			Tv_BaseX			=	Tv_BaseX + Tv_StepX;

			// draw 20
			Tv_Rc.left		=	Tv_BaseX - (((__s32)(__u32)Tv_PtrParaGrpHzT1->RulerUnitWidth - Tv_PtrCtlGrpHzT1->CellSizeX) >> 1);
			Tv_Rc.right		=	Tv_Rc.left + (__s32)(__u32)Tv_PtrParaGrpHzT1->RulerUnitWidth;

			GrP5PntReqTxtDraw(&Tv_Rc, V_GrP5SkinT2StrRulerMinT1[Tv_RulIdx], (__u16)m_CtlStat);		// txt buf ok
			Tv_RulIdx ++;
			Tv_BaseX			=	Tv_BaseX + Tv_StepX;

			// draw 30
			Tv_Rc.left		=	Tv_BaseX - (((__s32)(__u32)Tv_PtrParaGrpHzT1->RulerUnitWidth - Tv_PtrCtlGrpHzT1->CellSizeX) >> 1);
			Tv_Rc.right		=	Tv_Rc.left + (__s32)(__u32)Tv_PtrParaGrpHzT1->RulerUnitWidth;

			GrP5PntReqTxtDraw(&Tv_Rc, V_GrP5SkinT2StrRulerMinT1[Tv_RulIdx], (__u16)m_CtlStat);		// txt buf ok
			Tv_RulIdx ++;
			Tv_BaseX			=	Tv_BaseX + Tv_StepX;

			// draw 40
			Tv_Rc.left		=	Tv_BaseX - (((__s32)(__u32)Tv_PtrParaGrpHzT1->RulerUnitWidth - Tv_PtrCtlGrpHzT1->CellSizeX) >> 1);
			Tv_Rc.right		=	Tv_Rc.left + (__s32)(__u32)Tv_PtrParaGrpHzT1->RulerUnitWidth;

			GrP5PntReqTxtDraw(&Tv_Rc, V_GrP5SkinT2StrRulerMinT1[Tv_RulIdx], (__u16)m_CtlStat);		// txt buf ok
			Tv_RulIdx ++;
			Tv_BaseX			=	Tv_BaseX + Tv_StepX;

			// draw 50
			Tv_Rc.left		=	Tv_BaseX - (((__s32)(__u32)Tv_PtrParaGrpHzT1->RulerUnitWidth - Tv_PtrCtlGrpHzT1->CellSizeX) >> 1);
			Tv_Rc.right		=	Tv_Rc.left + (__s32)(__u32)Tv_PtrParaGrpHzT1->RulerUnitWidth;

			GrP5PntReqTxtDraw(&Tv_Rc, V_GrP5SkinT2StrRulerMinT1[Tv_RulIdx], (__u16)m_CtlStat);		// txt buf ok
			Tv_RulIdx ++;
			Tv_BaseX			=	Tv_BaseX + Tv_StepX;

			// darw 59
			Tv_Rc.right		=	Tv_Rc.left + Tv_StepX;
			Tv_Rc.left		=	Tv_Rc.right - (__s32)(__u32)Tv_PtrParaGrpHzT1->RulerUnitWidth;

			GrP5PntReqTxtDraw(&Tv_Rc, V_GrP5SkinT2StrRulerMinT1[Tv_RulIdx], (__u16)m_CtlStat);		// txt buf ok

		}

	}	//	if(0 != Tv_PtrParaGrpHzT1->RulerUnitWidth)

	// ruler indicater
	if((NULL != Tv_PtrCtlGrpHzT1->StrInd) && (0 != Tv_PtrParaGrpHzT1->IndWidth))
	{
		if((0 <= Tv_PtrCtlGrpHzBase->CusIdxX) && ((__s32)Tv_PtrCtlGrpHzT1->VwCntX > Tv_PtrCtlGrpHzBase->CusIdxX))
		{
			Tv_Rc.left		=	m_CtlRcScr.left + E_GrP5SkinT2GrpHzT1ContMgn + (Tv_PtrCtlGrpHzT1->CellSizeX * Tv_PtrCtlGrpHzBase->CusIdxX);
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
void	Cls_GrP5SkinT2::LcDrawCtlGrpHzTiCont(void)
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

	Tv_Rc.top			=	m_CtlRcScr.top + (__s32)(__u32)Tv_PtrParaGrpHzT1->RulerHeight + E_GrP5SkinT2GrpHzT1ContMgn;

	for(Tv_IdxY = 0; Tv_IdxY < Tv_PtrCtlGrpHzT1->VwCntY; Tv_IdxY++)
	{
		Tv_Rc.bottom	=	Tv_Rc.top + (__s32)Tv_PtrCtlGrpHzT1->CellSizeY;

		if(Tv_Rc.bottom >(m_CtlRcScr.bottom - E_GrP5SkinT2GrpHzT1ContMgn))
		{
			break;
		}

		Tv_Rc.left		=	m_CtlRcScr.left + E_GrP5SkinT2GrpHzT1ContMgn;
		for(Tv_IdxX = 0; Tv_IdxX < Tv_PtrCtlGrpHzT1->VwCntX; Tv_IdxX++)
		{
			Tv_Rc.right	=	Tv_Rc.left + (__s32)Tv_PtrCtlGrpHzT1->CellSizeX;
			if(Tv_Rc.right  >(m_CtlRcScr.right - E_GrP5SkinT2GrpHzT1ContMgn))
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
void	Cls_GrP5SkinT2::LcCtlDrawGrpHzT1(void)
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
void	Cls_GrP5SkinT2::LcCtlDrawLblTimeReg(void)
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
void	Cls_GrP5SkinT2::LcCtlDrawLblTimeView(void)
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
void	Cls_GrP5SkinT2::LcCtlDrawPnlAim(void)
{
	// local -------------------
	// code --------------------
	LcDrawAimBg(m_PtrCtlSkinPara, &m_CtlRcScr, 0);
}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcDrawCtlPnlGrpBarLine(void)
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
void	Cls_GrP5SkinT2::LcDrawCtlPnlGrpBarItemName(void)
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
void	Cls_GrP5SkinT2::LcDrawCtlPnlGrpBarGridVal(void)
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
void	Cls_GrP5SkinT2::LcDrawCtlPnlGrpBarBar(void)
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
void	Cls_GrP5SkinT2::LcCtlDrawPnlGrpBar(void)
{
	// local -------------------
	// code --------------------

	LcDrawCtlPnlGrpBarLine();

	LcDrawCtlPnlGrpBarItemName();

	LcDrawCtlPnlGrpBarGridVal();

	LcDrawCtlPnlGrpBarBar();

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcDrawCtlPnlGrpLineLine(void)
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
void	Cls_GrP5SkinT2::LcDrawCtlPnlGrpLineColName(void)
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
void	Cls_GrP5SkinT2::LcDrawCtlPnlGrpLineGridVal(void)
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
void	Cls_GrP5SkinT2::LcDrawCtlPnlGrpLineGraph(void)
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
void	Cls_GrP5SkinT2::LcCtlDrawPnlGrpLine(void)
{
	// local -------------------
	// code --------------------

	LcDrawCtlPnlGrpLineLine();
	LcDrawCtlPnlGrpLineColName();
	LcDrawCtlPnlGrpLineGridVal();
	LcDrawCtlPnlGrpLineGraph();

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcCtlDrawPnlStd(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoPnlStdPara		Tv_PtrParaPnlStd;

		St_GrRect	Tv_RcCenter;
		St_GrRect	Tv_RcPnt;
		Ptr_GrGdib	Tv_PtrGdib;
	// code --------------------
		// init
		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdPnlStd);
		Tv_PtrParaPnlStd	=	(Ptr_GrP5UoPnlStdPara)Tv_PtrHrcy->PtrPara;

		// background 
		if(Tv_PtrParaPnlStd->IsDrawBg)
		{
			if(0 == m_PtrCtlSkinPara->SubType)
			{
				// subtype 0
				GrP5PntArrangeByRect(&m_CtlRcScr);

				Tv_RcCenter	=	m_CtlRcScr;
				if(0 != m_PtrCtlSkinPara->ParaByte0)
				{
					// draw boarder line
					// calculate center area
					Tv_RcCenter.left		=	m_CtlRcScr.left + (__s32)m_PtrCtlSkinPara->ParaByte0;
					Tv_RcCenter.top			=	m_CtlRcScr.top + (__s32)m_PtrCtlSkinPara->ParaByte0;
					Tv_RcCenter.right		=	m_CtlRcScr.right - (__s32)m_PtrCtlSkinPara->ParaByte0;
					Tv_RcCenter.bottom	=	m_CtlRcScr.bottom - (__s32)m_PtrCtlSkinPara->ParaByte0;

					ReqPntRectangle(&m_CtlRcScr, (__u32)m_PtrCtlSkinPara->ParaByte0, m_PtrCtlSkinPara->ParaColor);
				}

				GrP5PntReqFillRect(&Tv_RcCenter, Tv_PtrParaPnlStd->Color);

			}
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
void	Cls_GrP5SkinT2::LcCtlDrawPrgsBase(void)
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
void	Cls_GrP5SkinT2::LcDrawCtlQuadT1ViewArea(void)
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
void	Cls_GrP5SkinT2::LcDrawCtlQuadBaseInnerLine(void)
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
void	Cls_GrP5SkinT2::LcDrawCtlQuadBaseOutLine(void)
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
void	Cls_GrP5SkinT2::LcCtlDrawQuadT1(void)
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
void	Cls_GrP5SkinT2::LcCtlDrawSpinBase(void)
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

		if(((Cls_GrP5UoBase*)m_CtlUo)->IsCanEnable())
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
				else
				{
					m_CtlStat	=	E_GrP5StatPush;
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
void	Cls_GrP5SkinT2::LcCtlDrawTabBase(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoTabBasePara		Tv_PtrParaTabBase;
		Ptr_GrP5SkinCtlTabBase	Tv_PtrCtlTabBase;
		St_GrRect	Tv_RcDraw;
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

		// type zero
		if(0 == m_PtrCtlSkinPara->SubType)
		{
			ReqPntRectangle(&Tv_PtrCtlTabBase->RcCont, E_GrP5SkinT2TabBorderWidth, m_ClrTablBorder);
			Tv_RcDraw.left			=	Tv_PtrCtlTabBase->RcCont.left + E_GrP5SkinT2TabBorderWidth;
			Tv_RcDraw.top				=	Tv_PtrCtlTabBase->RcCont.top + E_GrP5SkinT2TabBorderWidth;
			Tv_RcDraw.right			=	Tv_PtrCtlTabBase->RcCont.right - E_GrP5SkinT2TabBorderWidth;
			Tv_RcDraw.bottom		=	Tv_PtrCtlTabBase->RcCont.bottom - E_GrP5SkinT2TabBorderWidth;

			GrP5PntReqFillRect(&Tv_RcDraw, Tv_PtrParaTabBase->ContClr);
		}
		else if(1 == m_PtrCtlSkinPara->SubType)
		{
			GrP5PntReqFillRect(&Tv_PtrCtlTabBase->RcCont, Tv_PtrParaTabBase->ContClr);
		}


		// draw top area
		if(m_CtlRcScr.top < Tv_PtrCtlTabBase->RcCont.top)
		{
			Tv_RcDraw.left			=	m_CtlRcScr.left;
			Tv_RcDraw.top				=	m_CtlRcScr.top;
			Tv_RcDraw.right			=	m_CtlRcScr.right;
			Tv_RcDraw.bottom		=	Tv_PtrCtlTabBase->RcCont.top;
			
			GrP5PntReqFillRect(&Tv_RcDraw, m_PtrCtlSkinPara->ParaColor);
		}

		// draw left area
		if(m_CtlRcScr.left < Tv_PtrCtlTabBase->RcCont.left)
		{
			Tv_RcDraw.left			=	m_CtlRcScr.left;
			Tv_RcDraw.top				=	Tv_PtrCtlTabBase->RcCont.top;
			Tv_RcDraw.right			=	Tv_PtrCtlTabBase->RcCont.left;
			Tv_RcDraw.bottom		=	Tv_PtrCtlTabBase->RcCont.bottom;

			GrP5PntReqFillRect(&Tv_RcDraw, m_PtrCtlSkinPara->ParaColor);
		}

		// draw right area
		if(m_CtlRcScr.right > Tv_PtrCtlTabBase->RcCont.right)
		{
			Tv_RcDraw.left			=	Tv_PtrCtlTabBase->RcCont.right;
			Tv_RcDraw.top				=	Tv_PtrCtlTabBase->RcCont.top;
			Tv_RcDraw.right			=	m_CtlRcScr.right;
			Tv_RcDraw.bottom		=	Tv_PtrCtlTabBase->RcCont.bottom;

			GrP5PntReqFillRect(&Tv_RcDraw, m_PtrCtlSkinPara->ParaColor);
		}

		// draw bottom area
		if(m_CtlRcScr.bottom > Tv_PtrCtlTabBase->RcCont.bottom)
		{
			Tv_RcDraw.left			=	m_CtlRcScr.left;
			Tv_RcDraw.top				=	Tv_PtrCtlTabBase->RcCont.bottom;
			Tv_RcDraw.right			=	m_CtlRcScr.right;
			Tv_RcDraw.bottom		=	m_CtlRcScr.bottom;

			GrP5PntReqFillRect(&Tv_RcDraw, m_PtrCtlSkinPara->ParaColor);
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcCtlDrawTabTiRel(void)
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
void	Cls_GrP5SkinT2::LcDrawCtlTblBaseItems(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoTblBasePara		Tv_PtrParaTblBase;
		Ptr_GrP5SkinCtlTblBase	Tv_PtrCtlTblBase;

		__u32	Tv_LineIdx;
		__u8	Tv_ColIdx;

		__u8	Tv_BrdTop;
		__u8	Tv_BrdLeft;
		__u8	Tv_BrdRight;
		__u8	Tv_BrdBottom;

		__u32	Tv_Clr;

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

		Tv_RcItm.top		=	m_CtlRcScr.top + E_GrP5SkinT2TblMargin;

		Tv_BrdTop				=	2;
		Tv_BrdBottom		=	1;

		for(Tv_LineIdx = 0; Tv_LineIdx < Tv_PtrCtlTblBase->ViewLineMax; Tv_LineIdx++)
		{
			// check able
			if(Tv_PtrCtlTblBase->ViewLineCnt <= Tv_LineIdx)
			{
				break;
			}

			// check last line
			if((Tv_LineIdx + 1) >= Tv_PtrCtlTblBase->ViewLineCnt)
			{
				Tv_BrdBottom		=	2;
			}

			// calculate bottom
			Tv_RcItm.bottom	=	Tv_RcItm.top + (__s32)(__u32)Tv_PtrParaTblBase->ItemHeight;

			// colum display
			Tv_BrdLeft			=	2;
			Tv_BrdRight			=	1;
			for(Tv_ColIdx = 0; Tv_ColIdx < (__u8)Tv_PtrCtlTblBase->ColumCnt; Tv_ColIdx++)
			{
				// check finish
				if(0 == Tv_PtrCtlTblBase->Cols[Tv_ColIdx].Width)
				{
					break;
				}

				if(((__u32)Tv_ColIdx + 1) >= Tv_PtrCtlTblBase->ColumCnt)
				{
					Tv_BrdRight			=	2;
				}

				// calculate rect
				Tv_RcItm.left			=	Tv_PtrCtlTblBase->Cols[Tv_ColIdx].Pos;
				Tv_RcItm.right		=	Tv_RcItm.left + Tv_PtrCtlTblBase->Cols[Tv_ColIdx].Width;

				// paint item
				Tv_RcDraw.left		=	Tv_RcItm.left + E_GrP5SkinT2TblItemMgnLeft;
				Tv_RcDraw.top			=	Tv_RcItm.top + E_GrP5SkinT2TblItemMgnTop;
				Tv_RcDraw.right		=	Tv_RcItm.right - E_GrP5SkinT2TblItemMgnRight;
				Tv_RcDraw.bottom	=	Tv_RcItm.bottom - E_GrP5SkinT2TblItemMgnDown;

				if(Tv_RcDraw.left > Tv_RcDraw.right)
				{
					continue;
				}

				// get item status
				Tv_Clr			=	m_ClrTblNormal;
				Tv_Stat			=	0;
#if 0
				if((Tv_PtrCtlTblBase->CusY == (__s32)Tv_LineIdx) && (Tv_PtrParaTblBase->IsLineSel || (Tv_PtrCtlTblBase->CusX == (__s32)Tv_ColIdx)))
				{
					Tv_Stat	=	Tv_Stat | E_GrP5StatOnFocus;
					Tv_Clr			=	m_ClrTblFocus;
				}
#endif
				if((Tv_PtrCtlTblBase->SelY == (__s32)Tv_LineIdx) && ((Tv_PtrParaTblBase->IsLineSel) || (Tv_PtrCtlTblBase->SelX == (__s32)Tv_ColIdx)))
				{
					Tv_Stat	=	Tv_Stat | E_GrP5StatOnSelected;
					Tv_Clr			=	m_ClrTblSelect;
				}

				// item background

				LcDrawGridBox(&Tv_RcItm, Tv_Clr, m_ClrTblBorder, Tv_BrdLeft, Tv_BrdTop, Tv_BrdRight,Tv_BrdBottom);
				
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

				// next
				Tv_BrdLeft		=	1;
			}

			// draw cursor
#if 0
			// not use
			if(Tv_PtrParaTblBase->IsLineSel && (Tv_PtrCtlTblBase->CusY == (__s32)Tv_LineIdx))
			{
				// draw cursor
				Tv_RcDraw.left			=	m_CtlRcScr.left + E_GrP5SkinT2TblMargin;
				Tv_RcDraw.top				=	Tv_RcItm.top;
				Tv_RcDraw.right			=	m_CtlRcScr.right - E_GrP5SkinT2TblMargin;
				Tv_RcDraw.bottom		=	Tv_RcItm.bottom;

				// draw line cursor

			}
#endif

			// next
			Tv_RcItm.top			=	Tv_RcItm.bottom;
			Tv_BrdTop					=	1;
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcDrawCtlGrpHzT2Cont(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoGrpHzBasePara		Tv_PtrParaGrpHzBase;
		Ptr_GrP5SkinCtlGrpHzBase	Tv_PtrCtlGrpHzBase;

		Ptr_GrP5UoGrpHzT2Para		Tv_PtrParaGrpHzT2;
		Ptr_GrP5SkinCtlGrpHzT2	Tv_PtrCtlGrpHzT2;

		__u32	Tv_LinIdx;
		__u32	Tv_ItmIdx;

		__u8	Tv_Val;

		St_GrRect	Tv_RcDraw;

	// code --------------------
		// init
		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdGrpHzBase);
		Tv_PtrParaGrpHzBase		=	(Ptr_GrP5UoGrpHzBasePara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlGrpHzBase		=	(Ptr_GrP5SkinCtlGrpHzBase)Tv_PtrHrcy->PtrSkinCtl;

		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdGrpHzT2);
		Tv_PtrParaGrpHzT2			=	(Ptr_GrP5UoGrpHzT2Para)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlGrpHzT2			=	(Ptr_GrP5SkinCtlGrpHzT2)Tv_PtrHrcy->PtrSkinCtl;

		// check able
		if((0 == Tv_PtrCtlGrpHzT2->VwCntY) || (0 == Tv_PtrParaGrpHzBase->LineHeight))
		{
			return;
		}

		if(Tv_PtrParaGrpHzBase->LineHeight < (m_PtrCtlSkinPara->ParaByte0 + E_GrP5SkinT2GrpHzT2GrpHeight))
		{
			return;
		}

		if(0 >= Tv_PtrCtlGrpHzT2->RcContArea.right)
		{
			return;
		}

		Tv_RcDraw.top		=	Tv_PtrCtlGrpHzT2->RcContArea.top + (__s32)(__u32)m_PtrCtlSkinPara->ParaByte0;

		for(Tv_LinIdx = 0; Tv_LinIdx < Tv_PtrCtlGrpHzT2->VwCntY; Tv_LinIdx++)
		{
			Tv_RcDraw.bottom		=	Tv_RcDraw.top + E_GrP5SkinT2GrpHzT2GrpHeight;
			Tv_RcDraw.left			=	Tv_PtrCtlGrpHzT2->RcContArea.left;

			if(Tv_RcDraw.bottom >= Tv_PtrCtlGrpHzT2->RcContArea.bottom)
			{
				break;
			}

			Tv_ItmIdx			=	0;
			Tv_Val				=	0;
			while(Tv_ItmIdx < Tv_PtrCtlGrpHzT2->ContTbl[Tv_LinIdx].Cnt)
			{
				Tv_Val	=	Tv_Val | Tv_PtrCtlGrpHzT2->ContTbl[Tv_LinIdx].Itms[Tv_ItmIdx].Val;
				Tv_RcDraw.right	=	Tv_PtrCtlGrpHzT2->RcContArea.left + (__s32)Tv_PtrCtlGrpHzT2->ContTbl[Tv_LinIdx].Itms[Tv_ItmIdx].EndPos;
				if(Tv_RcDraw.left != Tv_RcDraw.right)
				{
					// draw
					Tv_Val	=	Tv_Val & 7;
					GrP5PntReqFillRect(&Tv_RcDraw, m_ClrGridRscdItm[Tv_Val]);
					Tv_Val	=	0;
					Tv_RcDraw.left	=	Tv_RcDraw.right;
				}

				// next
				Tv_ItmIdx ++;
			}
			// next
			Tv_RcDraw.top	=	Tv_RcDraw.top + (__s32)(__u32)Tv_PtrParaGrpHzBase->LineHeight;
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcDrawCtlGrpHzT2Ruler(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoGrpHzBasePara		Tv_PtrParaGrpHzBase;
		Ptr_GrP5SkinCtlGrpHzBase	Tv_PtrCtlGrpHzBase;

		Ptr_GrP5UoGrpHzT2Para		Tv_PtrParaGrpHzT2;
		Ptr_GrP5SkinCtlGrpHzT2	Tv_PtrCtlGrpHzT2;

	// code --------------------
		// init
		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdGrpHzBase);
		Tv_PtrParaGrpHzBase		=	(Ptr_GrP5UoGrpHzBasePara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlGrpHzBase		=	(Ptr_GrP5SkinCtlGrpHzBase)Tv_PtrHrcy->PtrSkinCtl;

		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdGrpHzT2);
		Tv_PtrParaGrpHzT2			=	(Ptr_GrP5UoGrpHzT2Para)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlGrpHzT2			=	(Ptr_GrP5SkinCtlGrpHzT2)Tv_PtrHrcy->PtrSkinCtl;

		// check able
		if((0 == Tv_PtrCtlGrpHzT2->RulTxtCnt) || (0 == Tv_PtrCtlGrpHzT2->RcRule.right))
		{
			return;
		}

		// ruler text draw
		LcDrawCtlGrpHzT2RulTxt();

		// ruler line draw
		LcDrawCtlGrpHzT2RulLine();


}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcDrawCtlGrpHzT2RulLine(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoGrpHzBasePara		Tv_PtrParaGrpHzBase;
		Ptr_GrP5SkinCtlGrpHzBase	Tv_PtrCtlGrpHzBase;

		Ptr_GrP5UoGrpHzT2Para		Tv_PtrParaGrpHzT2;
		Ptr_GrP5SkinCtlGrpHzT2	Tv_PtrCtlGrpHzT2;

		St_GrRect	Tv_RcWk;
		__u32	Tv_TimeWk;
		__u32	Tv_TimeChk;

	// code --------------------

		// init
		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdGrpHzBase);
		Tv_PtrParaGrpHzBase		=	(Ptr_GrP5UoGrpHzBasePara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlGrpHzBase		=	(Ptr_GrP5SkinCtlGrpHzBase)Tv_PtrHrcy->PtrSkinCtl;

		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdGrpHzT2);
		Tv_PtrParaGrpHzT2			=	(Ptr_GrP5UoGrpHzT2Para)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlGrpHzT2			=	(Ptr_GrP5SkinCtlGrpHzT2)Tv_PtrHrcy->PtrSkinCtl;

		Tv_RcWk.left		=	Tv_PtrCtlGrpHzT2->RcRule.left;
		Tv_RcWk.top			=	Tv_PtrCtlGrpHzT2->RcRule.bottom - E_GrP5SkinT2GrpHzT2RulLhHeight;
		Tv_RcWk.right		=	Tv_PtrCtlGrpHzT2->RcRule.right;
		Tv_RcWk.bottom	=	Tv_RcWk.top + 1;
		GrP5PntReqFillRect(&Tv_RcWk, m_ClrGrpHzT2RulLine);

		Tv_RcWk.left		=	Tv_PtrCtlGrpHzT2->RcRule.left;
		Tv_RcWk.top			=	Tv_PtrCtlGrpHzT2->RcRule.bottom - E_GrP5SkinT2GrpHzT2RulLhHeight + 1;

		Tv_TimeChk			=	Tv_PtrCtlGrpHzT2->TimeScl;
		Tv_TimeWk				=	Tv_PtrCtlGrpHzT2->TimeScl % E_GrTimeSecOfHour;
		
		while((Tv_RcWk.left < Tv_PtrCtlGrpHzT2->RcRule.right) && (Tv_TimeChk < (Tv_PtrCtlGrpHzT2->TimeLoad + E_GrTimeSecOfDay)))
		{
			Tv_RcWk.right		=	Tv_RcWk.left + 1;
			Tv_RcWk.bottom	=	Tv_RcWk.top + 2;
			if(0 == Tv_TimeWk)
			{
				Tv_RcWk.bottom	=	Tv_RcWk.top + 5;
			}

			GrP5PntReqFillRect(&Tv_RcWk, m_ClrGrpHzT2RulLine);

			// next
			Tv_TimeChk		=	Tv_TimeChk + Tv_PtrCtlGrpHzT2->RulPerSec;
			Tv_TimeWk			=	Tv_TimeWk + Tv_PtrCtlGrpHzT2->RulPerSec;
			if(E_GrTimeSecOfHour <= Tv_TimeWk)
			{
				Tv_TimeWk	=	Tv_TimeWk - E_GrTimeSecOfHour;
			}
			Tv_RcWk.left	=	Tv_RcWk.left + (__s32)Tv_PtrCtlGrpHzT2->RulPerPx;
		}
		

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcDrawCtlGrpHzT2RulTxt(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoGrpHzBasePara		Tv_PtrParaGrpHzBase;
		Ptr_GrP5SkinCtlGrpHzBase	Tv_PtrCtlGrpHzBase;

		Ptr_GrP5UoGrpHzT2Para		Tv_PtrParaGrpHzT2;
		Ptr_GrP5SkinCtlGrpHzT2	Tv_PtrCtlGrpHzT2;

		__u32	Tv_ItmIdx;

	// code --------------------
		// init
		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdGrpHzBase);
		Tv_PtrParaGrpHzBase		=	(Ptr_GrP5UoGrpHzBasePara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlGrpHzBase		=	(Ptr_GrP5SkinCtlGrpHzBase)Tv_PtrHrcy->PtrSkinCtl;

		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdGrpHzT2);
		Tv_PtrParaGrpHzT2			=	(Ptr_GrP5UoGrpHzT2Para)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlGrpHzT2			=	(Ptr_GrP5SkinCtlGrpHzT2)Tv_PtrHrcy->PtrSkinCtl;

		GrP5PntReqTxtStyle(&Tv_PtrParaGrpHzT2->RulerTxtStyle, Tv_PtrParaGrpHzT2->RulerFontId);
		GrP5PntReqTxtCus();

		for(Tv_ItmIdx = 0; Tv_ItmIdx < Tv_PtrCtlGrpHzT2->RulTxtCnt; Tv_ItmIdx++)
		{
			if(NULL == Tv_PtrCtlGrpHzT2->RulTxtTbl[Tv_ItmIdx].Str)
			{
				break;
			}
			
			GrP5PntReqTxtDraw(&Tv_PtrCtlGrpHzT2->RulTxtTbl[Tv_ItmIdx].Rc, Tv_PtrCtlGrpHzT2->RulTxtTbl[Tv_ItmIdx].Str, 0);
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcDrawCtlGrpHzT2Cursor(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoGrpHzBasePara		Tv_PtrParaGrpHzBase;
		Ptr_GrP5SkinCtlGrpHzBase	Tv_PtrCtlGrpHzBase;

		Ptr_GrP5UoGrpHzT2Para		Tv_PtrParaGrpHzT2;
		Ptr_GrP5SkinCtlGrpHzT2	Tv_PtrCtlGrpHzT2;

		__u32	Tv_TimeCus;
		__u32	Tv_PxCus;

		St_GrRect	Tv_RcDraw;
	// code --------------------
		// init
		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdGrpHzBase);
		Tv_PtrParaGrpHzBase		=	(Ptr_GrP5UoGrpHzBasePara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlGrpHzBase		=	(Ptr_GrP5SkinCtlGrpHzBase)Tv_PtrHrcy->PtrSkinCtl;

		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdGrpHzT2);
		Tv_PtrParaGrpHzT2			=	(Ptr_GrP5UoGrpHzT2Para)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlGrpHzT2			=	(Ptr_GrP5SkinCtlGrpHzT2)Tv_PtrHrcy->PtrSkinCtl;

		// get cursor position
		Tv_TimeCus	=	Tv_PtrCtlGrpHzT2->TimeCus;
		
		if((Tv_TimeCus < Tv_PtrCtlGrpHzT2->TimeScl) || (Tv_TimeCus >= (Tv_PtrCtlGrpHzT2->TimeScl + Tv_PtrCtlGrpHzT2->TimeLen)))
		{
			return;
		}

		// draw cursor
		Tv_PxCus	=	(Tv_TimeCus - Tv_PtrCtlGrpHzT2->TimeScl) / Tv_PtrCtlGrpHzT2->PxPerSec;
		Tv_RcDraw.left		=	Tv_PtrCtlGrpHzT2->RcContArea.left + (__s32)Tv_PxCus;
		Tv_RcDraw.right		=	Tv_RcDraw.left + 1;
		Tv_RcDraw.top			=	Tv_PtrCtlGrpHzT2->RcContArea.top - E_GrP5SkinT2GrpHzT2RulLhHeight;
		Tv_RcDraw.bottom	=	Tv_PtrCtlGrpHzT2->RcContArea.bottom;

		GrP5PntReqFillRect(&Tv_RcDraw, m_ClrGrpHzT2Cus);

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcCtlDrawTblBase(void)
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
		if(((Cls_GrP5UoBase*)m_CtlUo)->IsOnFocuse())
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
void	Cls_GrP5SkinT2::LcDrawTitleDivT0(Ptr_GrRect A_PtrRect)
{
	// local -------------------
		St_GrRect	Tv_RcLbl;
		St_GrRect	Tv_RcWbg;
		__s32	Tv_WkIdx;

	// code --------------------
		Tv_RcLbl.left			=	A_PtrRect->left;
		Tv_RcLbl.top			=	A_PtrRect->top;
		Tv_RcLbl.bottom		=	Tv_RcLbl.top + 1;

		Tv_RcWbg.top			=	A_PtrRect->top;
		Tv_RcWbg.right		=	A_PtrRect->right;
		Tv_RcWbg.bottom		=	Tv_RcWbg.top + 1;

		Tv_RcLbl.right		=	Tv_RcLbl.left;

		for(Tv_WkIdx = 0; Tv_WkIdx < E_GrP5SkinT2TitleT0Height; Tv_WkIdx++)
		{
			// check finish
			if(A_PtrRect->bottom <= Tv_RcLbl.top)
			{
				break;
			}

			Tv_RcLbl.right	=	Tv_RcLbl.right + V_GrP5SkinT2TitleT0DivVct[Tv_WkIdx];
			Tv_RcWbg.left		=	Tv_RcLbl.right;

			// check label drawable
			if(Tv_RcLbl.left < Tv_RcLbl.right)
			{
				GrP5PntReqFillRect(&Tv_RcLbl, m_ClrWinT0Bg0);
			}
			// check window background drawable
			if(Tv_RcWbg.left < Tv_RcWbg.right)
			{
				GrP5PntReqFillRect(&Tv_RcWbg, m_ClrWinT0Bg1);
			}
			// go next
			Tv_RcLbl.top ++;
			Tv_RcLbl.bottom ++;
			Tv_RcWbg.top ++;
			Tv_RcWbg.bottom ++;
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcDrawGridBox(Ptr_GrRect A_PtrRc, __u32 A_ClrIn, __u32 A_ClrBrd, __u8 A_WidLeft, __u8 A_WidTop, __u8 A_WidRight, __u8 A_WidBottom)
{
	// local -------------------
		St_GrRect	Tv_RcDraw;
	// code --------------------
		// border
		// top
		Tv_RcDraw.left		=	A_PtrRc->left;
		Tv_RcDraw.top			=	A_PtrRc->top;
		Tv_RcDraw.right		=	A_PtrRc->right;
		Tv_RcDraw.bottom	=	A_PtrRc->top + (__s32)A_WidTop;
		GrP5PntReqFillRect(&Tv_RcDraw, A_ClrBrd);

		// left
		Tv_RcDraw.top			=	Tv_RcDraw.bottom;
		Tv_RcDraw.right		=	A_PtrRc->left + (__s32)A_WidLeft;
		Tv_RcDraw.bottom	=	A_PtrRc->bottom - (__s32)A_WidBottom;
		GrP5PntReqFillRect(&Tv_RcDraw, A_ClrBrd);

		// right
		Tv_RcDraw.left		=	A_PtrRc->right - (__s32)A_WidRight;
		Tv_RcDraw.right		=	A_PtrRc->right;
		GrP5PntReqFillRect(&Tv_RcDraw, A_ClrBrd);

		// bottom
		Tv_RcDraw.left		=	A_PtrRc->left;
		Tv_RcDraw.top			=	A_PtrRc->bottom - (__s32)A_WidBottom;
		Tv_RcDraw.bottom	=	A_PtrRc->bottom;
		GrP5PntReqFillRect(&Tv_RcDraw, A_ClrBrd);

		// fill contents
		Tv_RcDraw.left		=	A_PtrRc->left + (__s32)A_WidLeft;
		Tv_RcDraw.top			=	A_PtrRc->top + (__s32)A_WidTop;
		Tv_RcDraw.right		=	A_PtrRc->right - (__s32)A_WidRight;
		Tv_RcDraw.bottom	=	A_PtrRc->bottom - (__s32)A_WidBottom;
		GrP5PntReqFillRect(&Tv_RcDraw, A_ClrIn);

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcDrawCtlWinT0(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5WinBasePara			Tv_PtrParaWinBase;

		St_GrRect	Tv_RcLbl;
		St_GrRect	Tv_RcDiv;
		St_GrRect	Tv_RcRmn;

		St_GrRect	Tv_RcBody;
	// code --------------------
		// init
		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdWinBase);
		Tv_PtrParaWinBase		=	(Ptr_GrP5WinBasePara)Tv_PtrHrcy->PtrPara;

		// calculate rect area
		Tv_RcLbl.top		=	m_CtlRcScr.top + 1;
		Tv_RcLbl.left		=	m_CtlRcScr.left + 1;
		Tv_RcLbl.right	=	m_CtlRcScr.left + (__s32)m_PtrCtlSkinPara->ParaWord0;
		Tv_RcLbl.bottom	=	Tv_RcLbl.top + E_GrP5SkinT2TitleT0Height;

		Tv_RcDiv.top		=	Tv_RcLbl.top;
		Tv_RcDiv.left		=	Tv_RcLbl.right;
		Tv_RcDiv.right	=	Tv_RcDiv.left + E_GrP5SkinT2TitleT0DivWidth;
		Tv_RcDiv.bottom	=	Tv_RcLbl.bottom;

		Tv_RcRmn.top		=	Tv_RcLbl.top;
		Tv_RcRmn.left		=	Tv_RcDiv.right;
		Tv_RcRmn.right	=	m_CtlRcScr.right - 1;
		Tv_RcRmn.bottom	=	Tv_RcDiv.bottom;

		Tv_RcBody.left		=	Tv_RcLbl.left;
		Tv_RcBody.top			=	Tv_RcDiv.bottom;
		Tv_RcBody.right		=	Tv_RcRmn.right;
		Tv_RcBody.bottom	=	m_CtlRcScr.bottom - 1;

		// draw label
		GrP5PntReqFillRect(&Tv_RcLbl, m_ClrWinT0Bg0);

		// draw divid
		LcDrawTitleDivT0(&Tv_RcDiv);

		// draw title remain
		GrP5PntReqFillRect(&Tv_RcRmn, m_ClrWinT0Bg1);

		// draw window background
		GrP5PntReqFillRect(&Tv_RcBody, m_ClrWinT0Bg0);

		// draw waku
		GrP5PntReqDrawRect(&m_CtlRcScr, m_ClrWinT0Bg2);

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcCtlDrawWinBase(void)
{
	// local -------------------
	// code --------------------

		if(0 == m_PtrCtlSkinPara->SubType)
		{
			LcDrawCtlWinT0();
		}
		else if(1 == m_PtrCtlSkinPara->SubType)
		{
			GrP5PntReqFillRect(&m_CtlRcScr, m_PtrCtlSkinPara->ParaColor);
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcCtlDrawPnlDir(void)
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
void	Cls_GrP5SkinT2::LcDrawSbarVt(Ptr_Grp5SkinSbarCtl A_PtrCtl)
{
	// local -------------------
		__u32		Tv_Clr;
		St_GrRect	Tv_Rc;
	// code --------------------
		// check able
		if(0 >= A_PtrCtl->RcArea.right)
		{
			return;
		}
		// draw dec button
		GrP5PntReqFillRect(&A_PtrCtl->RcBtnDec, m_ClrSbarBg);

		Tv_Rc.left		=	A_PtrCtl->RcBtnDec.left + E_GrP5SkinT2SbarVtBtnMarkGap;
		Tv_Rc.top			=	A_PtrCtl->RcBtnDec.top + E_GrP5SkinT2SbarVtBtnMarkGap;
		Tv_Rc.right		=	A_PtrCtl->RcBtnDec.right - E_GrP5SkinT2SbarVtBtnMarkGap;
		Tv_Rc.bottom	=	A_PtrCtl->RcBtnDec.bottom - E_GrP5SkinT2SbarVtBtnMarkGap;

		if((Tv_Rc.left < Tv_Rc.right) && (Tv_Rc.top < Tv_Rc.bottom))
		{
			Tv_Clr		=	m_ClrSbarBtnNrm;
			if(E_GrP5StatPush == (E_GrP5StatConstMask & A_PtrCtl->BtnDecStat))
			{
				Tv_Clr		=	m_ClrSbarBtnPush;
			}
			else if(0 != (E_GrP5StatOnFocus & A_PtrCtl->BtnDecStat))
			{
				Tv_Clr		=	m_ClrSbarBtnFcs;
			}

			LcDrawTriangleIsosUp(&Tv_Rc, Tv_Clr);
		}
		
		// draw inc button
		GrP5PntReqFillRect(&A_PtrCtl->RcBtnInc, m_ClrSbarBg);

		Tv_Rc.left		=	A_PtrCtl->RcBtnInc.left + E_GrP5SkinT2SbarVtBtnMarkGap;
		Tv_Rc.top			=	A_PtrCtl->RcBtnInc.top + E_GrP5SkinT2SbarVtBtnMarkGap;
		Tv_Rc.right		=	A_PtrCtl->RcBtnInc.right - E_GrP5SkinT2SbarVtBtnMarkGap;
		Tv_Rc.bottom	=	A_PtrCtl->RcBtnInc.bottom - E_GrP5SkinT2SbarVtBtnMarkGap;

		if((Tv_Rc.left < Tv_Rc.right) && (Tv_Rc.top < Tv_Rc.bottom))
		{
			Tv_Clr		=	m_ClrSbarBtnNrm;
			if(E_GrP5StatPush == (E_GrP5StatConstMask & A_PtrCtl->BtnIncStat))
			{
				Tv_Clr		=	m_ClrSbarBtnPush;
			}
			else if(0 != (E_GrP5StatOnFocus & A_PtrCtl->BtnIncStat))
			{
				Tv_Clr		=	m_ClrSbarBtnFcs;
			}
			LcDrawTriangleIsosDown(&Tv_Rc, Tv_Clr);
		}

		// check track able
		if(0 >= A_PtrCtl->RcTrack.right)
		{
			return;
		}

		// draw none bg
		if(0 == A_PtrCtl->End)
		{
			// fill track

			Tv_Rc.left		=	A_PtrCtl->RcTrack.left;
			Tv_Rc.top			=	A_PtrCtl->RcTrack.top;
			Tv_Rc.right		=	A_PtrCtl->RcTrack.left + E_GrP5SkinT2SbarVtLineWidth;
			Tv_Rc.bottom	=	A_PtrCtl->RcTrack.bottom;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrSbarBg);

			Tv_Rc.left		=	Tv_Rc.right;
			Tv_Rc.right		=	A_PtrCtl->RcTrack.right - E_GrP5SkinT2SbarVtLineWidth;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrSbarTrack);

			Tv_Rc.left		=	Tv_Rc.right;
			Tv_Rc.right		=	A_PtrCtl->RcTrack.right;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrSbarBg);

			return;
		}

		// draw upper track
		if(0 < A_PtrCtl->Pos )
		{
			Tv_Rc.left		=	A_PtrCtl->RcTrack.left;
			Tv_Rc.top			=	A_PtrCtl->RcTrack.top;
			Tv_Rc.right		=	A_PtrCtl->RcTrack.left + E_GrP5SkinT2SbarVtLineWidth;
			Tv_Rc.bottom	=	A_PtrCtl->RcTrack.top + A_PtrCtl->Pos;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrSbarBg);

			Tv_Rc.left		=	Tv_Rc.right;
			Tv_Rc.right		=	A_PtrCtl->RcTrack.right - E_GrP5SkinT2SbarVtLineWidth;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrSbarTrack);

			Tv_Rc.left		=	Tv_Rc.right;
			Tv_Rc.right		=	A_PtrCtl->RcTrack.right;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrSbarBg);
		}

		// draw thumb
		Tv_Rc.top				=	A_PtrCtl->RcTrack.top + A_PtrCtl->Pos;
		Tv_Rc.left			=	A_PtrCtl->RcTrack.left;
		Tv_Rc.right			=	A_PtrCtl->RcTrack.right;
		Tv_Rc.bottom		=	Tv_Rc.top + E_GrP5SkinT2SbarVtThumbSize;

		Tv_Clr		=	m_ClrSbarThmNrm;
		if(E_GrP5StatPush == (E_GrP5StatConstMask & A_PtrCtl->ThumbStat))
		{
			Tv_Clr		=	m_ClrSbarThmPush;
		}
		else if(0 != (E_GrP5StatOnFocus & A_PtrCtl->ThumbStat))
		{
			Tv_Clr		=	m_ClrSbarThmFcs;
		}
		GrP5PntReqFillRect(&Tv_Rc, Tv_Clr);

		// draw down track
		if(A_PtrCtl->End > A_PtrCtl->Pos)
		{
			Tv_Rc.left		=	A_PtrCtl->RcTrack.left;
			Tv_Rc.top			=	Tv_Rc.bottom;
			Tv_Rc.right		=	A_PtrCtl->RcTrack.left + E_GrP5SkinT2SbarVtLineWidth;
			Tv_Rc.bottom	=	A_PtrCtl->RcTrack.bottom;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrSbarBg);

			Tv_Rc.left		=	Tv_Rc.right;
			Tv_Rc.right		=	A_PtrCtl->RcTrack.right - E_GrP5SkinT2SbarVtLineWidth;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrSbarTrack);

			Tv_Rc.left		=	Tv_Rc.right;
			Tv_Rc.right		=	A_PtrCtl->RcTrack.right;
			GrP5PntReqFillRect(&Tv_Rc, m_ClrSbarBg);
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcCtlDrawWinPopList(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5SkinCtlWinPopItemBase		Tv_PtrCtlWinPopItemBase;

		St_GrRect	Tv_RcItem;
		St_GrRect	Tv_RcDraw;
		__s32	Tv_VwIdx;
		__u32	Tv_ItmIdx;
		WCHAR*	Tv_Str;
	// code --------------------
		// init
		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdWinPopItemBase);
		Tv_PtrCtlWinPopItemBase	=	(Ptr_GrP5SkinCtlWinPopItemBase)Tv_PtrHrcy->PtrSkinCtl;

		// draw background
		ReqPntRectangle(&m_CtlRcScr, E_GrP5SkinT2PopListWakuWidth,m_ClrWinPopListWaku);
		Tv_RcDraw.left		=	m_CtlRcScr.left + E_GrP5SkinT2PopListWakuWidth;
		Tv_RcDraw.top			=	m_CtlRcScr.top + E_GrP5SkinT2PopListWakuWidth;
		Tv_RcDraw.right		=	m_CtlRcScr.right - E_GrP5SkinT2PopListWakuWidth;
		Tv_RcDraw.bottom	=	m_CtlRcScr.bottom - E_GrP5SkinT2PopListWakuWidth;
		GrP5PntReqFillRect(&Tv_RcDraw, m_ClrWinPopListBg);

		// draw item able
		if((0 == Tv_PtrCtlWinPopItemBase->ItemHeight) || (0 == Tv_PtrCtlWinPopItemBase->ItemWidth) || 
			(0 == Tv_PtrCtlWinPopItemBase->ViewCnt) || (0 == V_GrP5Env.ItemCnt))
		{
			return;
		}

		// set text style
		GrP5PntReqTxtCus();
		GrP5PntReqTxtStyle(&V_GrP5Env.PopCtl.TxtStyle, V_GrP5Env.PopCtl.FontId);

		// draw item
		Tv_ItmIdx				=	Tv_PtrCtlWinPopItemBase->Sbar.Val;

		Tv_RcItem.top		=	m_CtlRcScr.top + E_GrP5SkinT2PopListMargin;
		Tv_RcItem.left	=	m_CtlRcScr.left + E_GrP5SkinT2PopListMargin;
		Tv_RcItem.right	=	Tv_RcItem.left + Tv_PtrCtlWinPopItemBase->ItemWidth;
		for( Tv_VwIdx = 0; Tv_VwIdx < Tv_PtrCtlWinPopItemBase->ViewCnt; Tv_VwIdx++)
		{
			// check able
			if(Tv_ItmIdx >= V_GrP5Env.ItemCnt)
			{
				break;
			}

			// calculate bottom
			Tv_RcItem.bottom		=	Tv_RcItem.top + Tv_PtrCtlWinPopItemBase->ItemHeight;

			// check item exist
			if(E_GrP5ItemSquadIdNone != V_GrP5Env.ItemTbl[Tv_ItmIdx].Squad)
			{
				// check item on cursor
				if((Tv_PtrCtlWinPopItemBase->CusMseIdx == (__s32)Tv_ItmIdx) || 
					((0 > Tv_PtrCtlWinPopItemBase->CusMseIdx) && (Tv_PtrCtlWinPopItemBase->CusKeyIdx == (__s32)Tv_ItmIdx)))
				{
					// fill background
					GrP5PntReqFillRect(&Tv_RcItem, m_ClrWinPopListOnCus);
				}

				// calculate draw rect
				Tv_RcDraw.left		=	Tv_RcItem.left + E_GrP5SkinT2PopListItmMgn;
				Tv_RcDraw.top			=	Tv_RcItem.top + E_GrP5SkinT2PopListItmMgn;
				Tv_RcDraw.right		=	Tv_RcItem.right - E_GrP5SkinT2PopListItmMgn;
				Tv_RcDraw.bottom	=	Tv_RcItem.bottom - E_GrP5SkinT2PopListItmMgn;

				// draw text
				Tv_Str	=	NULL;
				if(E_GrP5TxtIdNone != V_GrP5Env.ItemTbl[Tv_ItmIdx].TxtId)
				{
					Tv_Str	=	GrP5TextGet(V_GrP5Env.ItemTbl[Tv_ItmIdx].TxtId);
				}
				else
				{
					Tv_Str	=	V_GrP5Env.ItemTbl[Tv_ItmIdx].StrBuf;
				}
				if(NULL != Tv_Str)
				{
					// draw text
					GrP5PntReqTxtDraw(&Tv_RcDraw, Tv_Str, (__u16)V_GrP5Env.ItemTbl[Tv_ItmIdx].Stat);
				}
			}

			// next
			Tv_ItmIdx ++;
			Tv_RcItem.top		=	Tv_RcItem.bottom;
		}

		// draw scroll bar
		if(0 < Tv_PtrCtlWinPopItemBase->Sbar.RcArea.right)
		{
			LcDrawSbarVt(&Tv_PtrCtlWinPopItemBase->Sbar);
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::CtlGetSizeByItemCnt(__u16 A_ClassId, __u32 A_ItemCnt, __u32* A_PtrRtSizeX, __u32* A_PtrRtSizeY)
{
	// local -------------------

	// code --------------------
		// init
		*A_PtrRtSizeX	=	0;
		*A_PtrRtSizeY	=	0;

		// check count
		if(0 == A_ItemCnt)
		{
			return;
		}

		switch(A_ClassId)
		{
			case E_GrP5ClassIdWinPopList:
				LcCtlSizeWinPopList(A_ItemCnt, A_PtrRtSizeX, A_PtrRtSizeY);
				break;
			case E_GrP5ClassIdWinPopMenu:
				LcCtlSizeWinPopMenu(A_ItemCnt, A_PtrRtSizeX, A_PtrRtSizeY);
				break;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcCtlSizeWinPopList(__u32 A_ItemCnt, __u32* A_PtrRtSizeX, __u32* A_PtrRtSizeY)
{
	// local -------------------
		__u32		Tv_ItemHeight;

		__u32		Tv_FontHeight;

		Cls_GrP5Font*	Tv_Font;
	// code --------------------

		if(0 == V_GrP5Env.PopCtl.WinWidth)
		{
			return;
		}

		Tv_ItemHeight		=	0;

		// get font
		Tv_Font		=	(Cls_GrP5Font*)GrP5FontGet(V_GrP5Env.PopCtl.FontId);
		if(NULL == Tv_Font)
		{
			return;
		}

		Tv_FontHeight	=	(__u32)Tv_Font->HeightGet();

		// calculate item height
		Tv_ItemHeight	=	Tv_FontHeight + (E_GrP5SkinT2PopListItmMgn << 1);

		// calculate size
		*A_PtrRtSizeX	=	V_GrP5Env.PopCtl.WinWidth;
		*A_PtrRtSizeY	=	(Tv_ItemHeight * A_ItemCnt) + (E_GrP5SkinT2PopListMargin << 1);

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcCtlSizeWinPopMenu(__u32 A_ItemCnt, __u32* A_PtrRtSizeX, __u32* A_PtrRtSizeY)
{
	// local -------------------
		__u32		Tv_ItemHeight;

		__u32		Tv_FontHeight;

		Cls_GrP5Font*	Tv_Font;
	// code --------------------

		if(0 == V_GrP5Env.PopCtl.WinWidth)
		{
			return;
		}

		Tv_ItemHeight		=	0;

		// get font
		Tv_Font		=	(Cls_GrP5Font*)GrP5FontGet(V_GrP5Env.PopCtl.FontId);
		if(NULL == Tv_Font)
		{
			return;
		}

		Tv_FontHeight	=	(__u32)Tv_Font->HeightGet();

		// calculate item height
		Tv_ItemHeight	=	Tv_FontHeight + (E_GrP5SkinT2PopMenuItmMgn << 1);

		// calculate size
		*A_PtrRtSizeX	=	V_GrP5Env.PopCtl.WinWidth;
		*A_PtrRtSizeY	=	(Tv_ItemHeight * A_ItemCnt) + (E_GrP5SkinT2PopMenuMargin << 1);
}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcCtlDrawWinPopMenu(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5SkinCtlWinPopItemBase		Tv_PtrCtlWinPopItemBase;

		Ptr_GrP5SkinCtlWinPopMenu		Tv_PtrCtlWinPopMenu;

		St_GrRect	Tv_RcItem;
		St_GrRect	Tv_RcDraw;

		BOOL8	Tv_IsFocused;

		__u32	Tv_ItmStat;
		__u32	Tv_ItmIdx;
		__u32	Tv_ClrIdx;
		WCHAR*		Tv_Str;
	// code --------------------
		// init
		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdWinPopItemBase);
		Tv_PtrCtlWinPopItemBase	=	(Ptr_GrP5SkinCtlWinPopItemBase)Tv_PtrHrcy->PtrSkinCtl;

		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdWinPopMenu);
		Tv_PtrCtlWinPopMenu	=	(Ptr_GrP5SkinCtlWinPopMenu)Tv_PtrHrcy->PtrSkinCtl;

		// draw background
		ReqPntRectangle(&m_CtlRcScr, E_GrP5SkinT2PopMenuWakuWidth, m_ClrWinPopListWaku);
		Tv_RcDraw.left		=	m_CtlRcScr.left + E_GrP5SkinT2PopMenuWakuWidth;
		Tv_RcDraw.top			=	m_CtlRcScr.top + E_GrP5SkinT2PopMenuWakuWidth;
		Tv_RcDraw.right		=	m_CtlRcScr.right - E_GrP5SkinT2PopMenuWakuWidth;
		Tv_RcDraw.bottom	=	m_CtlRcScr.bottom - E_GrP5SkinT2PopMenuWakuWidth;
		GrP5PntReqFillRect(&Tv_RcDraw, m_ClrWinPopListBg);

		// draw item able
		if((0 == Tv_PtrCtlWinPopItemBase->ItemHeight) || (0 == Tv_PtrCtlWinPopItemBase->ItemWidth) ||
			(0 == Tv_PtrCtlWinPopItemBase->ViewCnt) || (0 == V_GrP5Env.ItemCnt))
		{
			return;
		}

		Tv_IsFocused	=	((Cls_GrP5UoBase*)m_CtlUo)->IsOnFocuse();

		// set text style
		GrP5PntReqTxtCus();
		GrP5PntReqTxtStyle(&V_GrP5Env.PopCtl.TxtStyle, V_GrP5Env.PopCtl.FontId);

		// draw item
		Tv_RcItem.top		=	m_CtlRcScr.top + E_GrP5SkinT2PopMenuMargin;
		Tv_RcItem.left	=	m_CtlRcScr.left + E_GrP5SkinT2PopMenuMargin;
		Tv_RcItem.right	=	Tv_RcItem.left + Tv_PtrCtlWinPopItemBase->ItemWidth;
		for(Tv_ItmIdx = 0; Tv_ItmIdx < (__u32)Tv_PtrCtlWinPopItemBase->ViewCnt; Tv_ItmIdx++)
		{
			// check able
			
			if(Tv_ItmIdx >= Tv_PtrCtlWinPopMenu->ItemCnt)
			{
				break;
			}

			// calculate bottom
			Tv_RcItem.bottom		=	Tv_RcItem.top + Tv_PtrCtlWinPopItemBase->ItemHeight;

			// check item exist
			if(E_GrP5ItemSquadIdNone != Tv_PtrCtlWinPopMenu->Items[Tv_ItmIdx]->Squad)
			{
				// get item stat
				Tv_ItmStat		=	Tv_PtrCtlWinPopMenu->Items[Tv_ItmIdx]->Stat;
				if(0 <= Tv_PtrCtlWinPopItemBase->CusMseIdx)
				{
					if((__u32)Tv_PtrCtlWinPopItemBase->CusMseIdx == Tv_ItmIdx)
					{
						Tv_ItmStat		=	Tv_ItmStat | E_GrP5StatOnFocus;
					}
				}
				else
				{
					if(Tv_PtrCtlWinPopItemBase->CusKeyIdx == Tv_ItmIdx)
					{
						Tv_ItmStat		=	Tv_ItmStat | E_GrP5StatOnFocus;
					}
				}

				// fill focus rect
				if(Tv_IsFocused && (0 != (E_GrP5StatOnFocus & Tv_ItmStat)))
				{
					GrP5PntReqFillRect(&Tv_RcItem, m_ClrWinPopListOnCus);
				}

				// calculate text rect
				Tv_RcDraw.left		=	Tv_RcItem.left + E_GrP5SkinT2PopMenuItmMgn;
				Tv_RcDraw.top			=	Tv_RcItem.top + E_GrP5SkinT2PopMenuItmMgn;
				Tv_RcDraw.right		=	Tv_RcItem.right - E_GrP5SkinT2PopMenuItmMgn - E_GrP5SkinT2PopMenuSubMark;
				Tv_RcDraw.bottom	=	Tv_RcItem.bottom - E_GrP5SkinT2PopMenuItmMgn;

				// draw text
				Tv_Str	=	NULL;
				if(E_GrP5TxtIdNone != Tv_PtrCtlWinPopMenu->Items[Tv_ItmIdx]->TxtId)
				{
					Tv_Str	=	GrP5TextGet(Tv_PtrCtlWinPopMenu->Items[Tv_ItmIdx]->TxtId);
				}
				else
				{
					Tv_Str	=	Tv_PtrCtlWinPopMenu->Items[Tv_ItmIdx]->StrBuf;
				}
				if(NULL != Tv_Str)
				{
					// draw text
					GrP5PntReqTxtDraw(&Tv_RcDraw, Tv_Str, (__u16)Tv_ItmStat);
				}

				// draw sub tree mark
				if(E_GrP5ItemSquadIdNone != Tv_PtrCtlWinPopMenu->Items[Tv_ItmIdx]->ChildSquad)
				{
					Tv_RcDraw.left		=	Tv_RcItem.right - E_GrP5SkinT2PopMenuItmMgn - E_GrP5SkinT2PopMenuSubMark;
					Tv_RcDraw.right		=	Tv_RcItem.right - E_GrP5SkinT2PopMenuItmMgn;

					Tv_ClrIdx					=	E_GrP5FontTxtClrNormal;
					if(0 != (E_GrP5StatOnFocus & Tv_ItmStat))
					{
						Tv_ClrIdx					=	E_GrP5FontTxtClrFocus;
					}
					LcDrawTriangleIsosRight(&Tv_RcDraw, V_GrP5Env.PopCtl.TxtStyle.ColorTxtTbl[Tv_ClrIdx]);
				}

			}

			// next
			Tv_RcItem.top		=	Tv_RcItem.bottom;
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcCtlDrawCbxBase(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoCbxBasePara		Tv_PtrParaCbxBase;
		Ptr_GrP5SkinCtlCbxBase	Tv_PtrCtlCbxBase;
		
		St_GrRect		Tv_RcItm;
		St_GrRect		Tv_RcMark;
		St_GrRect		Tv_RcDraw;

		__u32		Tv_Stat;
		__u32		Tv_ClrIdx;
	// code --------------------
		// init
		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdCbxBase);
		Tv_PtrParaCbxBase		=	(Ptr_GrP5UoCbxBasePara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlCbxBase		=	(Ptr_GrP5SkinCtlCbxBase)Tv_PtrHrcy->PtrSkinCtl;

		// draw background
		GrP5PntReqFillRect(&m_CtlRcScr, m_ClrCbxBg);

		// check draw able
		if(E_GrP5SkinT2CbxMarkSize >= (m_CtlRcScr.right - m_CtlRcScr.left))
		{
			return;
		}

		// calculate status
		Tv_ClrIdx	=	0;
		Tv_Stat		=	E_GrP5StatNormal;
		if(Tv_PtrCtlCbxBase->IsPush)
		{
			Tv_Stat		=	E_GrP5StatPush;
			Tv_ClrIdx	=	E_GrP5FontTxtClrSelect;
		}
		else if(((Cls_GrP5UoBase*)m_CtlUo)->IsOnFocuse())
		{
			Tv_Stat		=	E_GrP5StatOnFocus;
			Tv_ClrIdx	=	E_GrP5FontTxtClrFocus;
		}

		// calculate area
		Tv_RcItm.left			=	m_CtlRcScr.left;
		Tv_RcItm.top			=	m_CtlRcScr.top;
		Tv_RcItm.right		=	m_CtlRcScr.right - E_GrP5SkinT2CbxMarkSize - E_GrP5SkinT2CbxMarkGap;
		Tv_RcItm.bottom		=	m_CtlRcScr.bottom;

		Tv_RcMark.left		=	Tv_RcItm.right;
		Tv_RcMark.right		=	m_CtlRcScr.right - E_GrP5SkinT2CbxMarkGap;
		Tv_RcMark.top			=	m_CtlRcScr.top + ((m_CtlRcScr.bottom - m_CtlRcScr.top) >> 1) - (E_GrP5SkinT2CbxMarkSize >> 1);
		Tv_RcMark.bottom	=	Tv_RcMark.top + E_GrP5SkinT2CbxMarkSize;
		
		// draw text
		if(NULL != Tv_PtrCtlCbxBase->StrDisp)
		{
			if(GrP5RelRectToAbsRect(&Tv_PtrParaCbxBase->CbxTxtRc, &Tv_RcItm, &Tv_RcDraw))
			{
				GrP5PntReqTxtCus();
				GrP5PntReqTxtStyle(&Tv_PtrParaCbxBase->CbxTpara, Tv_PtrParaCbxBase->CbxFontId);

				GrP5PntReqTxtDraw(&Tv_RcDraw, Tv_PtrCtlCbxBase->StrDisp, (__u16)Tv_Stat);
			}
		}

		// draw mark
		LcDrawTriangleIsosDown(&Tv_RcMark, Tv_PtrParaCbxBase->CbxTpara.ColorTxtTbl[Tv_ClrIdx]);

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcCtlDrawPnlDrag(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5SkinCtlPnlDrag	Tv_PtrCtlPnlDrag;

	// code --------------------
		// init
		Tv_PtrHrcy					=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdPnlDrag);
		Tv_PtrCtlPnlDrag		=	(Ptr_GrP5SkinCtlPnlDrag)Tv_PtrHrcy->PtrSkinCtl;

		// draw drag
		if(0 < Tv_PtrCtlPnlDrag->RcDrag.right)
		{
			ReqPntRectangle(&Tv_PtrCtlPnlDrag->RcDrag, 1, m_ClrPnlDragCus);
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcCtlDrawGrpHzT2(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoGrpHzBasePara		Tv_PtrParaGrpHzBase;
		Ptr_GrP5SkinCtlGrpHzBase	Tv_PtrCtlGrpHzBase;

		Ptr_GrP5UoGrpHzT2Para		Tv_PtrParaGrpHzT2;
		Ptr_GrP5SkinCtlGrpHzT2	Tv_PtrCtlGrpHzT2;
	// code --------------------
		// init
		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdGrpHzBase);
		Tv_PtrParaGrpHzBase		=	(Ptr_GrP5UoGrpHzBasePara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlGrpHzBase		=	(Ptr_GrP5SkinCtlGrpHzBase)Tv_PtrHrcy->PtrSkinCtl;

		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdGrpHzT2);
		Tv_PtrParaGrpHzT2			=	(Ptr_GrP5UoGrpHzT2Para)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlGrpHzT2			=	(Ptr_GrP5SkinCtlGrpHzT2)Tv_PtrHrcy->PtrSkinCtl;

		if(0 == Tv_PtrCtlGrpHzT2->PxPerSec)
		{
			return;
		}

		// draw contents
		LcDrawCtlGrpHzT2Cont();

		// draw ruler
		LcDrawCtlGrpHzT2Ruler();

		// draw cursor
		LcDrawCtlGrpHzT2Cursor();

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::TooltipSetup(Ptr_GrRect A_PtrConRect, Ptr_GrRect A_PtrBaseRect, __u16 A_TxtId, Ptr_GrP5SkinTooltipCtl A_PtrRtCtl)
{
	// local -------------------
		WCHAR*	Tv_StrTxt;
		Cls_GrP5Font*	Tv_ObjFont;
		__u32	Tv_Width;
		__u32	Tv_Height;
		__u32	Tv_WinSizeX;
		__u32	Tv_WinSizeY;
	// code --------------------
		A_PtrRtCtl->TxtId	=	A_TxtId;
		if(E_GrP5TxtIdNone == A_TxtId)
		{
			// off
			return;
		}

		// get text size
		Tv_StrTxt	=	GrP5TextGet(A_TxtId);
		if(NULL == Tv_StrTxt)
		{
			A_PtrRtCtl->TxtId	=	E_GrP5TxtIdNone;
			return;
		}

		Tv_ObjFont	=	(Cls_GrP5Font*)GrP5FontGet(V_GrP5SkinCfg.TooltipFontId);
		if(NULL == Tv_ObjFont)
		{
			A_PtrRtCtl->TxtId	=	E_GrP5TxtIdNone;
			return;
		}

		Tv_ObjFont->CalcStringDimension(Tv_StrTxt, A_PtrConRect, &V_GrP5SkinCfg.TooltipTxtStyle, 0, &Tv_Width, &Tv_Height);
		
		if((0 == Tv_Width) || (0 == Tv_Height))
		{
			A_PtrRtCtl->TxtId	=	E_GrP5TxtIdNone;
			return;
		}

		Tv_WinSizeX	=	Tv_Width + (E_GrP5SkinT2TooltipWakuGap << 1);
		Tv_WinSizeY	=	Tv_Height + (E_GrP5SkinT2TooltipWakuGap << 1);

		// calculate rect
		A_PtrRtCtl->RcDraw.left		=	A_PtrBaseRect->left + ((A_PtrBaseRect->right - A_PtrBaseRect->left) >> 1) - (__s32)(Tv_WinSizeX >> 1);
		A_PtrRtCtl->RcDraw.right	=	A_PtrRtCtl->RcDraw.left + (__s32)Tv_WinSizeX;
		A_PtrRtCtl->RcDraw.top		=	A_PtrBaseRect->top - (__s32)Tv_WinSizeY;
		A_PtrRtCtl->RcDraw.bottom	=	A_PtrBaseRect->top;

		// patch horizontal
		if(A_PtrRtCtl->RcDraw.left < A_PtrConRect->left)
		{
			A_PtrRtCtl->RcDraw.left		=	A_PtrConRect->left;
			A_PtrRtCtl->RcDraw.right	=	A_PtrRtCtl->RcDraw.left + (__s32)Tv_WinSizeX;
		}
		if(A_PtrRtCtl->RcDraw.right > A_PtrConRect->right)
		{
			A_PtrRtCtl->RcDraw.right	=	A_PtrConRect->right;
			A_PtrRtCtl->RcDraw.left		=	A_PtrRtCtl->RcDraw.right - (__s32)Tv_WinSizeX;
		}
		// patch vertical
		if(A_PtrRtCtl->RcDraw.top < A_PtrConRect->top)
		{
			A_PtrRtCtl->RcDraw.top		=	A_PtrBaseRect->bottom;
			A_PtrRtCtl->RcDraw.bottom	=	A_PtrRtCtl->RcDraw.top + (__s32)Tv_WinSizeY;
		}
		if(A_PtrRtCtl->RcDraw.bottom > A_PtrConRect->bottom)
		{
			A_PtrRtCtl->RcDraw.bottom		=	A_PtrBaseRect->top;
			A_PtrRtCtl->RcDraw.top			=	A_PtrRtCtl->RcDraw.bottom - (__s32)Tv_WinSizeY;
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::TooltipDraw(Ptr_GrP5SkinTooltipCtl A_PtrCtl)
{
	// local -------------------
		St_GrRect	Tv_RcDraw;
	// code --------------------
		if(E_GrP5TxtIdNone == A_PtrCtl->TxtId)
		{
			return;
		}

		// draw background
		GrP5PntReqFillRect(&A_PtrCtl->RcDraw,m_ClrTooltipBg);

		// draw text
		GrP5PntReqTxtCus();
		GrP5PntReqTxtStyle(&V_GrP5SkinCfg.TooltipTxtStyle, V_GrP5SkinCfg.TooltipFontId);
		Tv_RcDraw.left		=	A_PtrCtl->RcDraw.left + E_GrP5SkinT2TooltipWakuGap;
		Tv_RcDraw.top			=	A_PtrCtl->RcDraw.top + E_GrP5SkinT2TooltipWakuGap;
		Tv_RcDraw.right		=	A_PtrCtl->RcDraw.right - E_GrP5SkinT2TooltipWakuGap;
		Tv_RcDraw.bottom	=	A_PtrCtl->RcDraw.bottom - E_GrP5SkinT2TooltipWakuGap;
		GrP5PntReqTxtDraw(&Tv_RcDraw, GrP5TextGet(A_PtrCtl->TxtId), 0);

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinT2::LcGrpHzT2ContUpdate(void)
{
	// local -------------------
		Ptr_GrP5UoHierarchy			Tv_PtrHrcy;
		Ptr_GrP5UoGrpHzBasePara		Tv_PtrParaGrpHzBase;
		Ptr_GrP5SkinCtlGrpHzBase	Tv_PtrCtlGrpHzBase;

		Ptr_GrP5UoGrpHzT2Para		Tv_PtrParaGrpHzT2;
		Ptr_GrP5SkinCtlGrpHzT2	Tv_PtrCtlGrpHzT2;

		__u32	Tv_MinCnt;

		__u32	Tv_WkCh;
		__u32	Tv_LnIdx;
		__u32	Tv_ItmIdx;

		__u32	Tv_MapSt;
		__u32	Tv_MapCnt;
		__u32	Tv_WkMap;
		__u32	Tv_MapIdx;

		__u8	Tv_ValPrv;
		__u8	Tv_ValNow;
		__u32	Tv_TimeLen;
		Ptr_GrDvrRecMinMap	Tv_PtrMmap;

	// code --------------------
		// init
		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdGrpHzBase);
		Tv_PtrParaGrpHzBase		=	(Ptr_GrP5UoGrpHzBasePara)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlGrpHzBase		=	(Ptr_GrP5SkinCtlGrpHzBase)Tv_PtrHrcy->PtrSkinCtl;

		Tv_PtrHrcy						=	((Cls_GrP5UoBase*)m_CtlUo)->InheritClassInfoGet(E_GrP5ClassIdGrpHzT2);
		Tv_PtrParaGrpHzT2			=	(Ptr_GrP5UoGrpHzT2Para)Tv_PtrHrcy->PtrPara;
		Tv_PtrCtlGrpHzT2			=	(Ptr_GrP5SkinCtlGrpHzT2)Tv_PtrHrcy->PtrSkinCtl;

		// reset
		for( Tv_WkCh = 0; Tv_WkCh < E_GrDvrMaxChCnt; Tv_WkCh++)
		{
			Tv_PtrCtlGrpHzT2->ContTbl[Tv_WkCh].Cnt		=	0;
		}

		// check able
		if((NULL == Tv_PtrCtlGrpHzT2->PtrAtbBuf) || (0 == Tv_PtrCtlGrpHzT2->VwCntY))
		{
			return;
		}

		Tv_MapSt		=	(Tv_PtrCtlGrpHzT2->TimeScl % E_GrTimeSecOfDay) / E_GrTimeSecOfMin;
		Tv_MapCnt		=	E_GrTimeMinOfDay - Tv_MapSt;
		if(0 == Tv_MapCnt)
		{
			return;
		}

		// patch map count
		Tv_MinCnt	=	Tv_PtrCtlGrpHzT2->TimeLen / E_GrTimeSecOfMin;
		if(Tv_MapCnt >= Tv_MinCnt)
		{
			Tv_MapCnt	=	Tv_MinCnt;
		}

		Tv_PtrMmap	=	(Ptr_GrDvrRecMinMap)Tv_PtrCtlGrpHzT2->PtrAtbBuf;

		// check merge
		if(Tv_PtrParaGrpHzT2->IsMergeCh)
		{
			// init
			Tv_ValPrv		=	0xFF;
			Tv_MapIdx		=	Tv_MapSt;
			Tv_TimeLen	=	0;
			Tv_LnIdx		=	0;

			for(Tv_WkMap = 0; Tv_WkMap < Tv_MapCnt; Tv_WkMap++)
			{
				// get value
				Tv_ValNow		=	0;
				for( Tv_WkCh = 0; Tv_WkCh < E_GrDvrMaxChCnt; Tv_WkCh++)
				{
					Tv_ValNow	=	Tv_ValNow | Tv_PtrMmap[Tv_MapIdx].ChAtb[Tv_WkCh];
				}
				
				if(Tv_ValNow != Tv_ValPrv)
				{
					// close previous
					if(0 != Tv_PtrCtlGrpHzT2->ContTbl[Tv_LnIdx].Cnt)
					{
						Tv_ItmIdx		=	Tv_PtrCtlGrpHzT2->ContTbl[Tv_LnIdx].Cnt -1;
						Tv_PtrCtlGrpHzT2->ContTbl[Tv_LnIdx].Itms[Tv_ItmIdx].EndPos	=	(__u16)(Tv_TimeLen / Tv_PtrCtlGrpHzT2->PxPerSec);
					}
					// create new
					Tv_ItmIdx		=	Tv_PtrCtlGrpHzT2->ContTbl[Tv_LnIdx].Cnt;
					Tv_PtrCtlGrpHzT2->ContTbl[Tv_LnIdx].Itms[Tv_ItmIdx].Val	=	Tv_ValNow;
					Tv_PtrCtlGrpHzT2->ContTbl[Tv_LnIdx].Cnt ++;
				}

				// next
				Tv_TimeLen	=	Tv_TimeLen + E_GrTimeSecOfMin;
				Tv_ValPrv	=	Tv_ValNow;
				Tv_MapIdx ++;
			}
			// mark last
			Tv_ItmIdx		=	Tv_PtrCtlGrpHzT2->ContTbl[Tv_LnIdx].Cnt -1;
			Tv_PtrCtlGrpHzT2->ContTbl[Tv_LnIdx].Itms[Tv_ItmIdx].EndPos	=	(__u16)(Tv_TimeLen / Tv_PtrCtlGrpHzT2->PxPerSec);

		}
		else
		{
			Tv_WkCh	=	(__u32)Tv_PtrCtlGrpHzT2->StartCh;

			for(Tv_LnIdx = 0; Tv_LnIdx < Tv_PtrCtlGrpHzT2->VwCntY; Tv_LnIdx++)
			{
				// init
				Tv_ValPrv		=	0xFF;
				Tv_MapIdx		=	Tv_MapSt;
				Tv_TimeLen	=	0;

				for( Tv_WkMap = 0; Tv_WkMap < Tv_MapCnt; Tv_WkMap++)
				{
					// get value
					Tv_ValNow	=	Tv_PtrMmap[Tv_MapIdx].ChAtb[Tv_WkCh];
					if(Tv_ValNow != Tv_ValPrv)
					{
						// close previous
						if(0 != Tv_PtrCtlGrpHzT2->ContTbl[Tv_LnIdx].Cnt)
						{
							Tv_ItmIdx		=	Tv_PtrCtlGrpHzT2->ContTbl[Tv_LnIdx].Cnt -1;
							Tv_PtrCtlGrpHzT2->ContTbl[Tv_LnIdx].Itms[Tv_ItmIdx].EndPos	=	(__u16)(Tv_TimeLen / Tv_PtrCtlGrpHzT2->PxPerSec);
						}
						// create new
						Tv_ItmIdx		=	Tv_PtrCtlGrpHzT2->ContTbl[Tv_LnIdx].Cnt;
						Tv_PtrCtlGrpHzT2->ContTbl[Tv_LnIdx].Itms[Tv_ItmIdx].Val	=	Tv_ValNow;
						Tv_PtrCtlGrpHzT2->ContTbl[Tv_LnIdx].Cnt ++;
					}

					// next
					Tv_TimeLen	=	Tv_TimeLen + E_GrTimeSecOfMin;
					Tv_ValPrv	=	Tv_ValNow;
					Tv_MapIdx ++;
				}
				// mark last
				Tv_ItmIdx		=	Tv_PtrCtlGrpHzT2->ContTbl[Tv_LnIdx].Cnt -1;
				Tv_PtrCtlGrpHzT2->ContTbl[Tv_LnIdx].Itms[Tv_ItmIdx].EndPos	=	(__u16)(Tv_TimeLen / Tv_PtrCtlGrpHzT2->PxPerSec);

				// next 
				Tv_WkCh ++;
			}
		}

}
//--------------------------------------------------------------------
