/*
 DVR quad control

 */

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <GrDebug.h>
#include <Dvr/GrDvrQuadCtrl.h>
#include <GrStrTool.h>
#include <GrDumyTool.h>

//====================================================================
//local const


//====================================================================
//local type


//====================================================================
//local function

//====================================================================
//var

__u8		V_GrDvrQuadVwCntTbl[E_GrDvrQuadModeCnt]	=
{E_GrDvrQuadVwCntMd1, E_GrDvrQuadVwCntMd4, E_GrDvrQuadVwCntMd6,
E_GrDvrQuadVwCntMd8, E_GrDvrQuadVwCntMd9, E_GrDvrQuadVwCntMd10,
E_GrDvrQuadVwCntMd13, E_GrDvrQuadVwCntMd16, E_GrDvrQuadVwCntMd25,
E_GrDvrQuadVwCntMd32
};

St_GrByteRect	V_GrDvrQdRtMd1[E_GrDvrQuadVwCntMd1]	=
{
	{E_GrDvrQuadSclZero, E_GrDvrQuadSclZero, E_GrDvrQuadScl1_1, E_GrDvrQuadScl1_1}
};

St_GrByteRect	V_GrDvrQdRtMd4[E_GrDvrQuadVwCntMd4]	=
{
	{E_GrDvrQuadSclZero, E_GrDvrQuadSclZero, E_GrDvrQuadScl1_2, E_GrDvrQuadScl1_2},
	{E_GrDvrQuadScl1_2, E_GrDvrQuadSclZero, E_GrDvrQuadScl1_1, E_GrDvrQuadScl1_2},
	{E_GrDvrQuadSclZero, E_GrDvrQuadScl1_2, E_GrDvrQuadScl1_2, E_GrDvrQuadScl1_1},
	{E_GrDvrQuadScl1_2, E_GrDvrQuadScl1_2, E_GrDvrQuadScl1_1, E_GrDvrQuadScl1_1}
};

St_GrByteRect	V_GrDvrQdRtMd6[E_GrDvrQuadVwCntMd6]	=
{
	{E_GrDvrQuadSclZero, E_GrDvrQuadSclZero, E_GrDvrQuadScl2_3, E_GrDvrQuadScl2_3},
	{E_GrDvrQuadScl2_3, E_GrDvrQuadSclZero, E_GrDvrQuadScl1_1, E_GrDvrQuadScl1_3},
	{E_GrDvrQuadScl2_3, E_GrDvrQuadScl1_3, E_GrDvrQuadScl1_1, E_GrDvrQuadScl2_3},
	{E_GrDvrQuadSclZero, E_GrDvrQuadScl2_3, E_GrDvrQuadScl1_3, E_GrDvrQuadScl1_1},
	{E_GrDvrQuadScl1_3, E_GrDvrQuadScl2_3, E_GrDvrQuadScl2_3, E_GrDvrQuadScl1_1},
	{E_GrDvrQuadScl2_3, E_GrDvrQuadScl2_3, E_GrDvrQuadScl1_1, E_GrDvrQuadScl1_1}
};

St_GrByteRect	V_GrDvrQdRtMd8[E_GrDvrQuadVwCntMd8]	=
{
	{E_GrDvrQuadSclZero, E_GrDvrQuadSclZero, E_GrDvrQuadScl3_4, E_GrDvrQuadScl3_4},
	{E_GrDvrQuadScl3_4, E_GrDvrQuadSclZero, E_GrDvrQuadScl1_1, E_GrDvrQuadScl1_4},
	{E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_4, E_GrDvrQuadScl1_1, E_GrDvrQuadScl1_2},
	{E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_2, E_GrDvrQuadScl1_1, E_GrDvrQuadScl3_4},
	{E_GrDvrQuadSclZero, E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_4, E_GrDvrQuadScl1_1},
	{E_GrDvrQuadScl1_4, E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_2, E_GrDvrQuadScl1_1},
	{E_GrDvrQuadScl1_2, E_GrDvrQuadScl3_4, E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_1},
	{E_GrDvrQuadScl3_4, E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_1, E_GrDvrQuadScl1_1}
};

St_GrByteRect	V_GrDvrQdRtMd9[E_GrDvrQuadVwCntMd9]	=
{
	{E_GrDvrQuadSclZero, E_GrDvrQuadSclZero, E_GrDvrQuadScl1_3, E_GrDvrQuadScl1_3},
	{E_GrDvrQuadScl1_3, E_GrDvrQuadSclZero, E_GrDvrQuadScl2_3, E_GrDvrQuadScl1_3},
	{E_GrDvrQuadScl2_3, E_GrDvrQuadSclZero, E_GrDvrQuadScl1_1, E_GrDvrQuadScl1_3},
	{E_GrDvrQuadSclZero, E_GrDvrQuadScl1_3, E_GrDvrQuadScl1_3, E_GrDvrQuadScl2_3},
	{E_GrDvrQuadScl1_3, E_GrDvrQuadScl1_3, E_GrDvrQuadScl2_3, E_GrDvrQuadScl2_3},
	{E_GrDvrQuadScl2_3, E_GrDvrQuadScl1_3, E_GrDvrQuadScl1_1, E_GrDvrQuadScl2_3},
	{E_GrDvrQuadSclZero, E_GrDvrQuadScl2_3, E_GrDvrQuadScl1_3, E_GrDvrQuadScl1_1},
	{E_GrDvrQuadScl1_3, E_GrDvrQuadScl2_3, E_GrDvrQuadScl2_3, E_GrDvrQuadScl1_1},
	{E_GrDvrQuadScl2_3, E_GrDvrQuadScl2_3, E_GrDvrQuadScl1_1, E_GrDvrQuadScl1_1}
};

St_GrByteRect	V_GrDvrQdRtMd10[E_GrDvrQuadVwCntMd10]	=
{
	{E_GrDvrQuadSclZero, E_GrDvrQuadSclZero, E_GrDvrQuadScl1_2, E_GrDvrQuadScl1_2},
	{E_GrDvrQuadScl1_2, E_GrDvrQuadSclZero, E_GrDvrQuadScl1_1, E_GrDvrQuadScl1_2},
	{E_GrDvrQuadSclZero, E_GrDvrQuadScl1_2, E_GrDvrQuadScl1_4, E_GrDvrQuadScl3_4},
	{E_GrDvrQuadScl1_4, E_GrDvrQuadScl1_2, E_GrDvrQuadScl1_2, E_GrDvrQuadScl3_4},
	{E_GrDvrQuadScl1_2, E_GrDvrQuadScl1_2, E_GrDvrQuadScl3_4, E_GrDvrQuadScl3_4},
	{E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_2, E_GrDvrQuadScl1_1, E_GrDvrQuadScl3_4},
	{E_GrDvrQuadSclZero, E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_4, E_GrDvrQuadScl1_1},
	{E_GrDvrQuadScl1_4, E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_2, E_GrDvrQuadScl1_1},
	{E_GrDvrQuadScl1_2, E_GrDvrQuadScl3_4, E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_1},
	{E_GrDvrQuadScl3_4, E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_1, E_GrDvrQuadScl1_1}
};

St_GrByteRect	V_GrDvrQdRtMd13[E_GrDvrQuadVwCntMd13]	=
{
	{E_GrDvrQuadScl1_4, E_GrDvrQuadScl1_4, E_GrDvrQuadScl3_4, E_GrDvrQuadScl3_4},
	{E_GrDvrQuadSclZero, E_GrDvrQuadSclZero, E_GrDvrQuadScl1_4, E_GrDvrQuadScl1_4},
	{E_GrDvrQuadScl1_4, E_GrDvrQuadSclZero, E_GrDvrQuadScl1_2, E_GrDvrQuadScl1_4},
	{E_GrDvrQuadScl1_2, E_GrDvrQuadSclZero, E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_4},
	{E_GrDvrQuadScl3_4, E_GrDvrQuadSclZero, E_GrDvrQuadScl1_1, E_GrDvrQuadScl1_4},
	{E_GrDvrQuadSclZero, E_GrDvrQuadScl1_4, E_GrDvrQuadScl1_4, E_GrDvrQuadScl1_2},
	{E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_4, E_GrDvrQuadScl1_1, E_GrDvrQuadScl1_2},
	{E_GrDvrQuadSclZero, E_GrDvrQuadScl1_2, E_GrDvrQuadScl1_4, E_GrDvrQuadScl3_4},
	{E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_2, E_GrDvrQuadScl1_1, E_GrDvrQuadScl3_4},
	{E_GrDvrQuadSclZero, E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_4, E_GrDvrQuadScl1_1},
	{E_GrDvrQuadScl1_4, E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_2, E_GrDvrQuadScl1_1},
	{E_GrDvrQuadScl1_2, E_GrDvrQuadScl3_4, E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_1},
	{E_GrDvrQuadScl3_4, E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_1, E_GrDvrQuadScl1_1}
};

St_GrByteRect	V_GrDvrQdRtMd16[E_GrDvrQuadVwCntMd16]	=
{
	{E_GrDvrQuadSclZero, E_GrDvrQuadSclZero, E_GrDvrQuadScl1_4, E_GrDvrQuadScl1_4},
	{E_GrDvrQuadScl1_4, E_GrDvrQuadSclZero, E_GrDvrQuadScl2_4, E_GrDvrQuadScl1_4},
	{E_GrDvrQuadScl2_4, E_GrDvrQuadSclZero, E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_4},
	{E_GrDvrQuadScl3_4, E_GrDvrQuadSclZero, E_GrDvrQuadScl1_1, E_GrDvrQuadScl1_4},
	{E_GrDvrQuadSclZero, E_GrDvrQuadScl1_4, E_GrDvrQuadScl1_4, E_GrDvrQuadScl2_4},
	{E_GrDvrQuadScl1_4, E_GrDvrQuadScl1_4, E_GrDvrQuadScl2_4, E_GrDvrQuadScl2_4},
	{E_GrDvrQuadScl2_4, E_GrDvrQuadScl1_4, E_GrDvrQuadScl3_4, E_GrDvrQuadScl2_4},
	{E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_4, E_GrDvrQuadScl1_1, E_GrDvrQuadScl2_4},
	{E_GrDvrQuadSclZero, E_GrDvrQuadScl2_4, E_GrDvrQuadScl1_4, E_GrDvrQuadScl3_4},
	{E_GrDvrQuadScl1_4, E_GrDvrQuadScl2_4, E_GrDvrQuadScl2_4, E_GrDvrQuadScl3_4},
	{E_GrDvrQuadScl2_4, E_GrDvrQuadScl2_4, E_GrDvrQuadScl3_4, E_GrDvrQuadScl3_4},
	{E_GrDvrQuadScl3_4, E_GrDvrQuadScl2_4, E_GrDvrQuadScl1_1, E_GrDvrQuadScl3_4},
	{E_GrDvrQuadSclZero, E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_4, E_GrDvrQuadScl1_1},
	{E_GrDvrQuadScl1_4, E_GrDvrQuadScl3_4, E_GrDvrQuadScl2_4, E_GrDvrQuadScl1_1},
	{E_GrDvrQuadScl2_4, E_GrDvrQuadScl3_4, E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_1},
	{E_GrDvrQuadScl3_4, E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_1, E_GrDvrQuadScl1_1}
};

St_GrByteRect	V_GrDvrQdRtMd25[E_GrDvrQuadVwCntMd25]	=
{
	{E_GrDvrQuadSclZero, E_GrDvrQuadSclZero, E_GrDvrQuadScl1_5, E_GrDvrQuadScl1_5},
	{E_GrDvrQuadScl1_5, E_GrDvrQuadSclZero, E_GrDvrQuadScl2_5, E_GrDvrQuadScl1_5},
	{E_GrDvrQuadScl2_5, E_GrDvrQuadSclZero, E_GrDvrQuadScl3_5, E_GrDvrQuadScl1_5},
	{E_GrDvrQuadScl3_5, E_GrDvrQuadSclZero, E_GrDvrQuadScl4_5, E_GrDvrQuadScl1_5},
	{E_GrDvrQuadScl4_5, E_GrDvrQuadSclZero, E_GrDvrQuadScl1_1, E_GrDvrQuadScl1_5},

	{E_GrDvrQuadSclZero, E_GrDvrQuadScl1_5, E_GrDvrQuadScl1_5, E_GrDvrQuadScl2_5},
	{E_GrDvrQuadScl1_5, E_GrDvrQuadScl1_5, E_GrDvrQuadScl2_5, E_GrDvrQuadScl2_5},
	{E_GrDvrQuadScl2_5, E_GrDvrQuadScl1_5, E_GrDvrQuadScl3_5, E_GrDvrQuadScl2_5},
	{E_GrDvrQuadScl3_5, E_GrDvrQuadScl1_5, E_GrDvrQuadScl4_5, E_GrDvrQuadScl2_5},
	{E_GrDvrQuadScl4_5, E_GrDvrQuadScl1_5, E_GrDvrQuadScl1_1, E_GrDvrQuadScl2_5},

	{E_GrDvrQuadSclZero, E_GrDvrQuadScl2_5, E_GrDvrQuadScl1_5, E_GrDvrQuadScl3_5},
	{E_GrDvrQuadScl1_5, E_GrDvrQuadScl2_5, E_GrDvrQuadScl2_5, E_GrDvrQuadScl3_5},
	{E_GrDvrQuadScl2_5, E_GrDvrQuadScl2_5, E_GrDvrQuadScl3_5, E_GrDvrQuadScl3_5},
	{E_GrDvrQuadScl3_5, E_GrDvrQuadScl2_5, E_GrDvrQuadScl4_5, E_GrDvrQuadScl3_5},
	{E_GrDvrQuadScl4_5, E_GrDvrQuadScl2_5, E_GrDvrQuadScl1_1, E_GrDvrQuadScl3_5},

	{E_GrDvrQuadSclZero, E_GrDvrQuadScl3_5, E_GrDvrQuadScl1_5, E_GrDvrQuadScl4_5},
	{E_GrDvrQuadScl1_5, E_GrDvrQuadScl3_5, E_GrDvrQuadScl2_5, E_GrDvrQuadScl4_5},
	{E_GrDvrQuadScl2_5, E_GrDvrQuadScl3_5, E_GrDvrQuadScl3_5, E_GrDvrQuadScl4_5},
	{E_GrDvrQuadScl3_5, E_GrDvrQuadScl3_5, E_GrDvrQuadScl4_5, E_GrDvrQuadScl4_5},
	{E_GrDvrQuadScl4_5, E_GrDvrQuadScl3_5, E_GrDvrQuadScl1_1, E_GrDvrQuadScl4_5},

	{E_GrDvrQuadSclZero, E_GrDvrQuadScl4_5, E_GrDvrQuadScl1_5, E_GrDvrQuadScl1_1},
	{E_GrDvrQuadScl1_5, E_GrDvrQuadScl4_5, E_GrDvrQuadScl2_5, E_GrDvrQuadScl1_1},
	{E_GrDvrQuadScl2_5, E_GrDvrQuadScl4_5, E_GrDvrQuadScl3_5, E_GrDvrQuadScl1_1},
	{E_GrDvrQuadScl3_5, E_GrDvrQuadScl4_5, E_GrDvrQuadScl4_5, E_GrDvrQuadScl1_1},
	{E_GrDvrQuadScl4_5, E_GrDvrQuadScl4_5, E_GrDvrQuadScl1_1, E_GrDvrQuadScl1_1},
};

St_GrByteRect	V_GrDvrQdRtMd32[E_GrDvrQuadVwCntMd32]	=
{
	{E_GrDvrQuadScl2_6, E_GrDvrQuadScl2_6, E_GrDvrQuadScl4_6, E_GrDvrQuadScl4_6},

	{E_GrDvrQuadSclZero, E_GrDvrQuadSclZero, E_GrDvrQuadScl1_6, E_GrDvrQuadScl1_6},
	{E_GrDvrQuadScl1_6, E_GrDvrQuadSclZero, E_GrDvrQuadScl2_6, E_GrDvrQuadScl1_6},
	{E_GrDvrQuadScl2_6, E_GrDvrQuadSclZero, E_GrDvrQuadScl3_6, E_GrDvrQuadScl1_6},
	{E_GrDvrQuadScl3_6, E_GrDvrQuadSclZero, E_GrDvrQuadScl4_6, E_GrDvrQuadScl1_6},
	{E_GrDvrQuadScl4_6, E_GrDvrQuadSclZero, E_GrDvrQuadScl5_6, E_GrDvrQuadScl1_6},
	{E_GrDvrQuadScl5_6, E_GrDvrQuadSclZero, E_GrDvrQuadScl1_1, E_GrDvrQuadScl1_6},

	{E_GrDvrQuadSclZero, E_GrDvrQuadScl1_6, E_GrDvrQuadScl1_6, E_GrDvrQuadScl2_6},
	{E_GrDvrQuadScl1_6, E_GrDvrQuadScl1_6, E_GrDvrQuadScl2_6, E_GrDvrQuadScl2_6},
	{E_GrDvrQuadScl2_6, E_GrDvrQuadScl1_6, E_GrDvrQuadScl3_6, E_GrDvrQuadScl2_6},
	{E_GrDvrQuadScl3_6, E_GrDvrQuadScl1_6, E_GrDvrQuadScl4_6, E_GrDvrQuadScl2_6},
	{E_GrDvrQuadScl4_6, E_GrDvrQuadScl1_6, E_GrDvrQuadScl5_6, E_GrDvrQuadScl2_6},
	{E_GrDvrQuadScl5_6, E_GrDvrQuadScl1_6, E_GrDvrQuadScl1_1, E_GrDvrQuadScl2_6},

	{E_GrDvrQuadSclZero, E_GrDvrQuadScl2_6, E_GrDvrQuadScl1_6, E_GrDvrQuadScl3_6},
	{E_GrDvrQuadScl1_6, E_GrDvrQuadScl2_6, E_GrDvrQuadScl2_6, E_GrDvrQuadScl3_6},
	{E_GrDvrQuadScl4_6, E_GrDvrQuadScl2_6, E_GrDvrQuadScl5_6, E_GrDvrQuadScl3_6},
	{E_GrDvrQuadScl5_6, E_GrDvrQuadScl2_6, E_GrDvrQuadScl1_1, E_GrDvrQuadScl3_6},

	{E_GrDvrQuadSclZero, E_GrDvrQuadScl3_6, E_GrDvrQuadScl1_6, E_GrDvrQuadScl4_6},
	{E_GrDvrQuadScl1_6, E_GrDvrQuadScl3_6, E_GrDvrQuadScl2_6, E_GrDvrQuadScl4_6},
	{E_GrDvrQuadScl4_6, E_GrDvrQuadScl3_6, E_GrDvrQuadScl5_6, E_GrDvrQuadScl4_6},
	{E_GrDvrQuadScl5_6, E_GrDvrQuadScl3_6, E_GrDvrQuadScl1_1, E_GrDvrQuadScl4_6},

	{E_GrDvrQuadSclZero, E_GrDvrQuadScl4_6, E_GrDvrQuadScl1_6, E_GrDvrQuadScl5_6},
	{E_GrDvrQuadScl1_6, E_GrDvrQuadScl4_6, E_GrDvrQuadScl2_6, E_GrDvrQuadScl5_6},
	{E_GrDvrQuadScl2_6, E_GrDvrQuadScl4_6, E_GrDvrQuadScl3_6, E_GrDvrQuadScl5_6},
	{E_GrDvrQuadScl3_6, E_GrDvrQuadScl4_6, E_GrDvrQuadScl4_6, E_GrDvrQuadScl5_6},
	{E_GrDvrQuadScl4_6, E_GrDvrQuadScl4_6, E_GrDvrQuadScl5_6, E_GrDvrQuadScl5_6},
	{E_GrDvrQuadScl5_6, E_GrDvrQuadScl4_6, E_GrDvrQuadScl1_1, E_GrDvrQuadScl5_6},

	{E_GrDvrQuadSclZero, E_GrDvrQuadScl5_6, E_GrDvrQuadScl1_6, E_GrDvrQuadScl1_1},
	{E_GrDvrQuadScl1_6, E_GrDvrQuadScl5_6, E_GrDvrQuadScl2_6, E_GrDvrQuadScl1_1},
	{E_GrDvrQuadScl2_6, E_GrDvrQuadScl5_6, E_GrDvrQuadScl3_6, E_GrDvrQuadScl1_1},
	{E_GrDvrQuadScl3_6, E_GrDvrQuadScl5_6, E_GrDvrQuadScl4_6, E_GrDvrQuadScl1_1},
	{E_GrDvrQuadScl4_6, E_GrDvrQuadScl5_6, E_GrDvrQuadScl5_6, E_GrDvrQuadScl1_1},
	{E_GrDvrQuadScl5_6, E_GrDvrQuadScl5_6, E_GrDvrQuadScl1_1, E_GrDvrQuadScl1_1},
};

Ptr_GrByteRect	V_GrDvrQdRtTbl[E_GrDvrQuadModeCnt]	=
{
	V_GrDvrQdRtMd1, V_GrDvrQdRtMd4, V_GrDvrQdRtMd6, V_GrDvrQdRtMd8,
	V_GrDvrQdRtMd9, V_GrDvrQdRtMd10, V_GrDvrQdRtMd13, V_GrDvrQdRtMd16,
	V_GrDvrQdRtMd25, V_GrDvrQdRtMd32
};

#if 0
St_GrByteRect	V_GrDvrQdLnTblMd1[E_GrDvrQdLnMode1Cnt]	=
{
	{E_GrDvrQuadSclZero, E_GrDvrQuadSclZero, E_GrDvrQuadSclZero, E_GrDvrQuadScl1_1},	// left
	{E_GrDvrQuadSclZero, E_GrDvrQuadSclZero, E_GrDvrQuadScl1_1, E_GrDvrQuadSclZero},	// top
	{E_GrDvrQuadScl1_1, E_GrDvrQuadSclZero, E_GrDvrQuadScl1_1, E_GrDvrQuadScl1_1},	// right
	{E_GrDvrQuadSclZero, E_GrDvrQuadScl1_1, E_GrDvrQuadScl1_1, E_GrDvrQuadScl1_1},	// bottom

};
#endif

St_GrByteRect	V_GrDvrQdLnTblMd4[E_GrDvrQdLnMode4Cnt]	=
{
	{E_GrDvrQuadSclZero, E_GrDvrQuadScl1_2, E_GrDvrQuadScl1_1, E_GrDvrQuadScl1_2},
	{E_GrDvrQuadScl1_2, E_GrDvrQuadSclZero, E_GrDvrQuadScl1_2, E_GrDvrQuadScl1_1}
};

St_GrByteRect	V_GrDvrQdLnTblMd6[E_GrDvrQdLnMode6Cnt]	=
{
	{E_GrDvrQuadSclZero, E_GrDvrQuadScl2_3, E_GrDvrQuadScl1_1, E_GrDvrQuadScl2_3},
	{E_GrDvrQuadScl2_3, E_GrDvrQuadScl1_3, E_GrDvrQuadScl1_1, E_GrDvrQuadScl1_3},
	{E_GrDvrQuadScl2_3, E_GrDvrQuadSclZero, E_GrDvrQuadScl2_3, E_GrDvrQuadScl1_1},
	{E_GrDvrQuadScl1_3, E_GrDvrQuadScl2_3, E_GrDvrQuadScl1_3, E_GrDvrQuadScl1_1}
};

St_GrByteRect	V_GrDvrQdLnTblMd8[E_GrDvrQdLnMode8Cnt]	=
{
	{E_GrDvrQuadSclZero, E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_1, E_GrDvrQuadScl3_4},
	{E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_4, E_GrDvrQuadScl1_1, E_GrDvrQuadScl1_4},
	{E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_2, E_GrDvrQuadScl1_1, E_GrDvrQuadScl1_2},

	{E_GrDvrQuadScl3_4, E_GrDvrQuadSclZero, E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_1},
	{E_GrDvrQuadScl1_4, E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_4, E_GrDvrQuadScl1_1},
	{E_GrDvrQuadScl1_2, E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_2, E_GrDvrQuadScl1_1}
};

St_GrByteRect	V_GrDvrQdLnTblMd9[E_GrDvrQdLnMode9Cnt]	=
{
	{E_GrDvrQuadSclZero, E_GrDvrQuadScl1_3, E_GrDvrQuadScl1_1, E_GrDvrQuadScl1_3},
	{E_GrDvrQuadSclZero, E_GrDvrQuadScl2_3, E_GrDvrQuadScl1_1, E_GrDvrQuadScl2_3},
	{E_GrDvrQuadScl1_3, E_GrDvrQuadSclZero, E_GrDvrQuadScl1_3, E_GrDvrQuadScl1_1},
	{E_GrDvrQuadScl2_3, E_GrDvrQuadSclZero, E_GrDvrQuadScl2_3, E_GrDvrQuadScl1_1}
};

St_GrByteRect	V_GrDvrQdLnTblMd10[E_GrDvrQdLnMode10Cnt]	=
{
	{E_GrDvrQuadSclZero, E_GrDvrQuadScl1_2, E_GrDvrQuadScl1_1, E_GrDvrQuadScl1_2},
	{E_GrDvrQuadScl1_2, E_GrDvrQuadSclZero, E_GrDvrQuadScl1_2, E_GrDvrQuadScl1_1},

	{E_GrDvrQuadSclZero, E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_1, E_GrDvrQuadScl3_4},

	{E_GrDvrQuadScl1_4, E_GrDvrQuadScl1_2, E_GrDvrQuadScl1_4, E_GrDvrQuadScl1_1},
	{E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_2, E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_1},
};

St_GrByteRect	V_GrDvrQdLnTblMd13[E_GrDvrQdLnMode13Cnt]	=
{
	{E_GrDvrQuadSclZero, E_GrDvrQuadScl1_4, E_GrDvrQuadScl1_1, E_GrDvrQuadScl1_4},
	{E_GrDvrQuadSclZero, E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_1, E_GrDvrQuadScl3_4},
	{E_GrDvrQuadSclZero, E_GrDvrQuadScl1_2, E_GrDvrQuadScl1_4, E_GrDvrQuadScl1_2},
	{E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_2, E_GrDvrQuadScl1_1, E_GrDvrQuadScl1_2},

	{E_GrDvrQuadScl1_4, E_GrDvrQuadSclZero, E_GrDvrQuadScl1_4, E_GrDvrQuadScl1_1},
	{E_GrDvrQuadScl3_4, E_GrDvrQuadSclZero, E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_1},
	{E_GrDvrQuadScl1_2, E_GrDvrQuadSclZero, E_GrDvrQuadScl1_2, E_GrDvrQuadScl1_4},
	{E_GrDvrQuadScl1_2, E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_2, E_GrDvrQuadScl1_1},
};

St_GrByteRect	V_GrDvrQdLnTblMd16[E_GrDvrQdLnMode16Cnt]	=
{
	{E_GrDvrQuadSclZero, E_GrDvrQuadScl1_4, E_GrDvrQuadScl1_1, E_GrDvrQuadScl1_4},
	{E_GrDvrQuadSclZero, E_GrDvrQuadScl1_2, E_GrDvrQuadScl1_1, E_GrDvrQuadScl1_2},
	{E_GrDvrQuadSclZero, E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_1, E_GrDvrQuadScl3_4},

	{E_GrDvrQuadScl1_4, E_GrDvrQuadSclZero, E_GrDvrQuadScl1_4, E_GrDvrQuadScl1_1},
	{E_GrDvrQuadScl1_2, E_GrDvrQuadSclZero, E_GrDvrQuadScl1_2, E_GrDvrQuadScl1_1},
	{E_GrDvrQuadScl3_4, E_GrDvrQuadSclZero, E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_1},
};

St_GrByteRect	V_GrDvrQdLnTblMd25[E_GrDvrQdLnMode25Cnt]	=
{
	{E_GrDvrQuadSclZero, E_GrDvrQuadScl1_5, E_GrDvrQuadScl1_1, E_GrDvrQuadScl1_5},
	{E_GrDvrQuadSclZero, E_GrDvrQuadScl2_5, E_GrDvrQuadScl1_1, E_GrDvrQuadScl2_5},
	{E_GrDvrQuadSclZero, E_GrDvrQuadScl3_5, E_GrDvrQuadScl1_1, E_GrDvrQuadScl3_5},
	{E_GrDvrQuadSclZero, E_GrDvrQuadScl4_5, E_GrDvrQuadScl1_1, E_GrDvrQuadScl4_5},

	{E_GrDvrQuadScl1_5, E_GrDvrQuadSclZero, E_GrDvrQuadScl1_5, E_GrDvrQuadScl1_1},
	{E_GrDvrQuadScl2_5, E_GrDvrQuadSclZero, E_GrDvrQuadScl2_5, E_GrDvrQuadScl1_1},
	{E_GrDvrQuadScl3_5, E_GrDvrQuadSclZero, E_GrDvrQuadScl3_5, E_GrDvrQuadScl1_1},
	{E_GrDvrQuadScl4_5, E_GrDvrQuadSclZero, E_GrDvrQuadScl4_5, E_GrDvrQuadScl1_1},
};

St_GrByteRect	V_GrDvrQdLnTblMd32[E_GrDvrQdLnMode32Cnt]	=
{
	{E_GrDvrQuadSclZero, E_GrDvrQuadScl1_6, E_GrDvrQuadScl1_1, E_GrDvrQuadScl1_6},
	{E_GrDvrQuadSclZero, E_GrDvrQuadScl2_6, E_GrDvrQuadScl1_1, E_GrDvrQuadScl2_6},
	{E_GrDvrQuadSclZero, E_GrDvrQuadScl3_6, E_GrDvrQuadScl2_6, E_GrDvrQuadScl3_6},
	{E_GrDvrQuadScl4_6, E_GrDvrQuadScl3_6, E_GrDvrQuadScl1_1, E_GrDvrQuadScl3_6},
	{E_GrDvrQuadSclZero, E_GrDvrQuadScl4_6, E_GrDvrQuadScl1_1, E_GrDvrQuadScl4_6},
	{E_GrDvrQuadSclZero, E_GrDvrQuadScl5_6, E_GrDvrQuadScl1_1, E_GrDvrQuadScl5_6},

	{E_GrDvrQuadScl1_6, E_GrDvrQuadSclZero, E_GrDvrQuadScl1_6, E_GrDvrQuadScl1_1},
	{E_GrDvrQuadScl2_6, E_GrDvrQuadSclZero, E_GrDvrQuadScl2_6, E_GrDvrQuadScl1_1},
	{E_GrDvrQuadScl3_6, E_GrDvrQuadSclZero, E_GrDvrQuadScl3_6, E_GrDvrQuadScl2_6},
	{E_GrDvrQuadScl3_6, E_GrDvrQuadScl4_6, E_GrDvrQuadScl3_6, E_GrDvrQuadScl1_1},
	{E_GrDvrQuadScl4_6, E_GrDvrQuadSclZero, E_GrDvrQuadScl4_6, E_GrDvrQuadScl1_1},
	{E_GrDvrQuadScl5_6, E_GrDvrQuadSclZero, E_GrDvrQuadScl5_6, E_GrDvrQuadScl1_1},
};

Ptr_GrByteRect	V_GrDvrQdLineTbl[E_GrDvrQuadModeCnt]	=
{
	NULL, V_GrDvrQdLnTblMd4, V_GrDvrQdLnTblMd6, V_GrDvrQdLnTblMd8,
	V_GrDvrQdLnTblMd9, V_GrDvrQdLnTblMd10, V_GrDvrQdLnTblMd13, V_GrDvrQdLnTblMd16,
	V_GrDvrQdLnTblMd25, V_GrDvrQdLnTblMd32
};

__u8	V_GrDvrQdLineCntTbl[E_GrDvrQuadModeCnt]	=
{
	E_GrDvrQdLnMode1Cnt, E_GrDvrQdLnMode4Cnt, E_GrDvrQdLnMode6Cnt, E_GrDvrQdLnMode8Cnt,
	E_GrDvrQdLnMode9Cnt, E_GrDvrQdLnMode10Cnt, E_GrDvrQdLnMode13Cnt, E_GrDvrQdLnMode16Cnt,
	E_GrDvrQdLnMode25Cnt, E_GrDvrQdLnMode32Cnt
};

//====================================================================

//--------------------------------------------------------------------
Cls_GrDvrQuadCtrl::Cls_GrDvrQuadCtrl(__u32 A_ModeMap, __u32 A_MaxChCnt, Ptr_GrRect A_PtrRcScr, __u32 A_AlignX, __u32 A_AlignY)
{
	// local -------------------
		__u32	Tv_WkIdx;
		__u32	Tv_MdMask;
		__u32	Tv_VwCnt;
		__u32	Tv_RcCnt;
		__u32	Tv_LnCnt;
	// code --------------------
		m_ModeMap		=	A_ModeMap;
		m_ChMaxCnt	=	A_MaxChCnt;

		m_XalgAdd		=	(__s32)A_AlignX - 1;
		m_XalgMask	=	~m_XalgAdd;

		m_YalgAdd		=	(__s32)A_AlignY - 1;
		m_YalgMask	=	~m_YalgAdd;

		m_LineWidth	=	E_GrDvrQuadLineDfltWidth;

		m_ModeNow		=	0;
		m_PageNow		=	0;

		// build mode table
		GrDumyZeroMem(m_ModeTbl, sizeof(m_ModeTbl));
		m_CfgSize		=	0;
		Tv_MdMask		=	1;
		Tv_RcCnt		=	0;
		Tv_LnCnt		=	0;
		for(Tv_WkIdx = 0; Tv_WkIdx < E_GrDvrQuadModeCnt; Tv_WkIdx++)
		{
			if(0 != (Tv_MdMask & m_ModeMap))
			{
				// update page count
				Tv_VwCnt	=	(__u32)GrDvrQuadViewCountGetByMode((__u8)Tv_WkIdx);
				m_ModeTbl[Tv_WkIdx].PageCnt	=	m_ChMaxCnt / Tv_VwCnt;
				if(0 != (m_ChMaxCnt % Tv_VwCnt))
				{
					m_ModeTbl[Tv_WkIdx].PageCnt ++;
				}

				// update map pointer
				m_ModeTbl[Tv_WkIdx].PtrMap	=	&m_CfgBuf[m_CfgSize];

				// update view rect pointer
				m_ModeTbl[Tv_WkIdx].PtrVwRc	=	&m_VwRcTbl[Tv_RcCnt];
				Tv_RcCnt	=	Tv_RcCnt + Tv_VwCnt;

				// update line rect pointer
				if(0 != V_GrDvrQdLineCntTbl[Tv_WkIdx])
				{
					m_ModeTbl[Tv_WkIdx].PtrLnRc	=	&m_LnRcTbl[Tv_LnCnt];
					Tv_LnCnt	=	Tv_LnCnt + (__u32)V_GrDvrQdLineCntTbl[Tv_WkIdx];
				}

				// update configuration size
				m_CfgSize	=	m_CfgSize + (m_ModeTbl[Tv_WkIdx].PageCnt * Tv_VwCnt);
			}
			// next
			Tv_MdMask	=	Tv_MdMask << 1;
		}

		CfgReset();

		ModeMaxUpdate();

		m_ModeMtLast	=	m_ModeMax;

		ScrRectSet(A_PtrRcScr);
}
//--------------------------------------------------------------------
Cls_GrDvrQuadCtrl::~Cls_GrDvrQuadCtrl()
{

}
//--------------------------------------------------------------------
__s32		Cls_GrDvrQuadCtrl::CalcScale(__s32 A_Val, __u8 A_Scale)
{
	// local -------------------
		__s32		Tv_Result;
	// code --------------------
		switch(A_Scale)
		{
			case E_GrDvrQuadScl1_1:
				Tv_Result	=	A_Val;
				break;
			case E_GrDvrQuadScl1_2:
				Tv_Result	=	A_Val >> 1;
				break;
			case E_GrDvrQuadScl1_3:
				Tv_Result	=	A_Val / 3;
				break;
			case E_GrDvrQuadScl1_4:
				Tv_Result	=	A_Val >> 2;
				break;
			case E_GrDvrQuadScl2_3:
				Tv_Result	=	(A_Val << 1) / 3;
				break;
			case E_GrDvrQuadScl3_4:
				Tv_Result	=	(A_Val * 3) >> 2;
				break;
			case E_GrDvrQuadScl1_5:
				Tv_Result	=	A_Val / 5;
				break;
			case E_GrDvrQuadScl2_5:
				Tv_Result	=	(A_Val / 5) << 1;
				break;
			case E_GrDvrQuadScl3_5:
				Tv_Result	=	(A_Val / 5) * 3;
				break;
			case E_GrDvrQuadScl4_5:
				Tv_Result	=	(A_Val / 5) << 2;
				break;
			case E_GrDvrQuadScl1_6:
				Tv_Result	=	A_Val / 6;
				break;
			case E_GrDvrQuadScl2_6:
				Tv_Result	=	(A_Val / 6) << 1;
				break;
			case E_GrDvrQuadScl4_6:
				Tv_Result	=	(A_Val / 6) << 2;
				break;
			case E_GrDvrQuadScl5_6:
				Tv_Result	=	(A_Val / 6) * 5;
				break;
			default:
				Tv_Result	=	0;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	GrDvrQuadViewCountGetByMode(__u8 A_Mode)
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	0;

		if(E_GrDvrQuadModeCnt > A_Mode)
		{
			Tv_Result		=	V_GrDvrQuadVwCntTbl[A_Mode];
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrQuadCtrl::ViewNowCountGet(void)
{
	// local -------------------
	// code --------------------
		return	GrDvrQuadViewCountGetByMode(m_ModeNow);
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrQuadCtrl::CfgSizeGet(void)
{
		return	m_CfgSize;
}
//--------------------------------------------------------------------
void	Cls_GrDvrQuadCtrl::CfgDataGet(void* A_PtrBuf)
{
	// local -------------------
	// code --------------------
		GrDumyCopyMem(A_PtrBuf, m_CfgBuf, m_CfgSize);
}
//--------------------------------------------------------------------
void	Cls_GrDvrQuadCtrl::CfgDataSet(void* A_PtrBuf)
{
		GrDumyCopyMem(m_CfgBuf, A_PtrBuf, m_CfgSize);
		// assign
}
//--------------------------------------------------------------------
void	Cls_GrDvrQuadCtrl::CfgReset(void)
{
	// local -------------------
		__u32	Tv_MdIdx;
		__u32	Tv_WkIdx;
		__u32	Tv_WkCnt;
		__u32	Tv_VwCnt;
		__s8	Tv_ChVal;
		__s8*	Tv_PtrMap;
	// code --------------------
		for(Tv_MdIdx = 0; Tv_MdIdx < E_GrDvrQuadModeCnt; Tv_MdIdx++)
		{
			if(0 != m_ModeTbl[Tv_MdIdx].PageCnt)
			{
				Tv_VwCnt	=	(__u32)GrDvrQuadViewCountGetByMode(Tv_MdIdx);
				Tv_WkCnt	=	m_ModeTbl[Tv_MdIdx].PageCnt * Tv_VwCnt;
				Tv_PtrMap	=	m_ModeTbl[Tv_MdIdx].PtrMap;
				Tv_ChVal	=	0;
				for( Tv_WkIdx = 0; Tv_WkIdx < Tv_WkCnt; Tv_WkIdx++)
				{
					Tv_PtrMap[Tv_WkIdx]	=	Tv_ChVal;
					// next
					if(0 <= Tv_ChVal)
					{
						Tv_ChVal ++;
						if(Tv_ChVal >= (__s8)m_ChMaxCnt)
						{
							if(1 < m_ModeTbl[Tv_MdIdx].PageCnt)
							{
								Tv_ChVal	=	0;
							}
							else
							{
								Tv_ChVal	=	-1;
							}
						}
					}
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrQuadCtrl::ScrRectSet(Ptr_GrRect A_PtrRc)
{
	// local -------------------
		__u32	Tv_MdIdx;
		__u32	Tv_VwCnt;
		__u32	Tv_WkIdx;
		Ptr_GrByteRect	Tv_SclTbl;
		Ptr_GrRect	Tv_RcTbl;
		__s32	Tv_ScrSzX;
		__s32	Tv_ScrSzY;
	// code --------------------
		// update rect
		m_RcScr.left		=	(A_PtrRc->left + m_XalgAdd) & m_XalgMask;
		m_RcScr.top			=	(A_PtrRc->top + m_YalgAdd) & m_YalgMask;
		m_RcScr.right		=	A_PtrRc->right & m_XalgMask;
		m_RcScr.bottom	=	A_PtrRc->bottom	& m_YalgMask;

		Tv_ScrSzX				=	m_RcScr.right - m_RcScr.left;
		Tv_ScrSzY				=	m_RcScr.bottom - m_RcScr.top;
		// calculate
		for(Tv_MdIdx = 0; Tv_MdIdx < E_GrDvrQuadModeCnt; Tv_MdIdx++)
		{
			if(0 != m_ModeTbl[Tv_MdIdx].PageCnt)
			{
				Tv_VwCnt	=	(__u32)GrDvrQuadViewCountGetByMode(Tv_MdIdx);
				Tv_SclTbl	=	V_GrDvrQdRtTbl[Tv_MdIdx];
				Tv_RcTbl	=	m_ModeTbl[Tv_MdIdx].PtrVwRc;
				for( Tv_WkIdx = 0; Tv_WkIdx < Tv_VwCnt; Tv_WkIdx++)
				{
					Tv_RcTbl[Tv_WkIdx].left		=	m_RcScr.left + (CalcScale(Tv_ScrSzX, Tv_SclTbl[Tv_WkIdx].left) & m_XalgMask);
					Tv_RcTbl[Tv_WkIdx].top		=	m_RcScr.top + (CalcScale(Tv_ScrSzY, Tv_SclTbl[Tv_WkIdx].top) & m_YalgMask);
					Tv_RcTbl[Tv_WkIdx].right	=	m_RcScr.left + (CalcScale(Tv_ScrSzX, Tv_SclTbl[Tv_WkIdx].right) & m_XalgMask);
					Tv_RcTbl[Tv_WkIdx].bottom	=	m_RcScr.top + (CalcScale(Tv_ScrSzY, Tv_SclTbl[Tv_WkIdx].bottom) & m_YalgMask);
				}
			}
		}

		// rebuild line area
		LineTableBuild();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrQuadCtrl::QuadModeSet(__u8 A_Mode, __u8 A_Page)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		// check mode
		if(E_GrDvrQuadModeCnt > A_Mode)
		{
			if(0 != m_ModeTbl[A_Mode].PageCnt)
			{
				if(A_Page < (__u8)m_ModeTbl[A_Mode].PageCnt)
				{
					// backup last multi mode
					if (E_GrDvrQuadMode1 != m_ModeNow)
					{
						// update
						m_ModeMtLast	=	m_ModeNow;
					}
					// update
					m_ModeNow		=	A_Mode;
					m_PageNow		=	A_Page;
					Tv_Result		=	TRUE;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrQuadCtrl::QuadNowModeGet(__u8* A_PtrRtMode, __u8* A_PtrRtPage)
{
		*A_PtrRtMode	=	m_ModeNow;
		*A_PtrRtPage	=	m_PageNow;
}
//--------------------------------------------------------------------
__s8	Cls_GrDvrQuadCtrl::ChGetByViewIdx(__u8 A_ViewIdx)
{
	// local -------------------
		__s8	Tv_Result;
		__u8	Tv_VwCnt;
		__u32	Tv_Pos;
	// code --------------------
		Tv_Result	=	-1;

		Tv_VwCnt	=	ViewNowCountGet();
		if(A_ViewIdx < Tv_VwCnt)
		{
			Tv_Pos		=	((__u32)m_PageNow * (__u32)Tv_VwCnt) + (__u32)A_ViewIdx;
			Tv_Result	=	m_ModeTbl[m_ModeNow].PtrMap[Tv_Pos];
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrQuadCtrl::ViewRectGetByViewIdx(__u8 A_ViewIdx, Ptr_GrRect A_PtrRtRect)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_VwCnt;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_VwCnt	=	ViewNowCountGet();
		if(A_ViewIdx < Tv_VwCnt)
		{
			*A_PtrRtRect	=	m_ModeTbl[m_ModeNow].PtrVwRc[A_ViewIdx];
			Tv_Result			=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Ptr_GrRect	Cls_GrDvrQuadCtrl::ViewTblPtrGetNowMode(__u32* A_PtrRtCnt)
{
	// local -------------------
		Ptr_GrRect	Tv_Result;
		__u32		Tv_VwCnt;
	// code --------------------
		Tv_Result		=	NULL;
		*A_PtrRtCnt	=	0;

		Tv_VwCnt	=	(__u32)ViewNowCountGet();
		if(0 != Tv_VwCnt)
		{
			*A_PtrRtCnt	=	Tv_VwCnt;
			Tv_Result		=	m_ModeTbl[m_ModeNow].PtrVwRc;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void		Cls_GrDvrQuadCtrl::ModeMaxUpdate(void)
{
	// local -------------------
		__u8	Tv_WkIdx;
	// code --------------------
		// find max quad
		Tv_WkIdx	=	E_GrDvrQuadModeCnt;
		while (0 != Tv_WkIdx)
		{
			Tv_WkIdx --;

			if (0 != m_ModeTbl[Tv_WkIdx].PageCnt)
			{
				// found
				m_ModeMax	=	Tv_WkIdx;
				break;
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrQuadCtrl::QuadModeMaxSet(void)
{
	// local -------------------
	// code --------------------
		QuadModeSet(m_ModeMax, 0);	// try change
}
//--------------------------------------------------------------------
void	Cls_GrDvrQuadCtrl::QuadModeNextSet(void)
{
	// local -------------------
		__u8	Tv_Mode;
		__u8	Tv_WkCnt;
	// code --------------------
		Tv_Mode		=	m_ModeNow + 1;
		Tv_WkCnt	=	E_GrDvrQuadModeCnt;

		while(0 != Tv_WkCnt)
		{
			// patch mode
			if(E_GrDvrQuadModeCnt <= Tv_Mode)
			{
				Tv_Mode	=	0;
			}

			if(0 != m_ModeTbl[Tv_Mode].PageCnt)
			{
				// found
				QuadModeSet(Tv_Mode, 0);	// try change
				break;
			}

			// next
			Tv_WkCnt --;
			Tv_Mode ++;
		}

}
//--------------------------------------------------------------------
void	Cls_GrDvrQuadCtrl::QuadModePrevSet(void)
{
	// local -------------------
		__u8	Tv_Mode;
		__u8	Tv_WkCnt;
	// code --------------------
		Tv_Mode		=	m_ModeNow;
		Tv_WkCnt	=	E_GrDvrQuadModeCnt;

		while(0 != Tv_WkCnt)
		{
			// patch mode
			if(0 == Tv_Mode)
			{
				Tv_Mode	=	E_GrDvrQuadModeCnt - 1;
			}
			else
			{
				Tv_Mode --;
			}

			if(0 != m_ModeTbl[Tv_Mode].PageCnt)
			{
				// found
				QuadModeSet(Tv_Mode, 0);	// try change
				break;
			}

			// next
			Tv_WkCnt --;
		}

}
//--------------------------------------------------------------------
void	Cls_GrDvrQuadCtrl::QuadPageNextSet(void)
{
	// local -------------------
		__u8	Tv_Page;
	// code --------------------
		Tv_Page	=	m_PageNow + 1;
		if(Tv_Page >= (__u8)m_ModeTbl[m_ModeNow].PageCnt)
		{
			Tv_Page	=	0;
		}

		QuadModeSet(m_ModeNow, Tv_Page);	// try change
}
//--------------------------------------------------------------------
void	Cls_GrDvrQuadCtrl::QuadPagePrevSet(void)
{
	// local -------------------
		__u8	Tv_Page;
	// code --------------------
		Tv_Page	=	m_PageNow;
		if(0 == Tv_Page)
		{
			Tv_Page	=	(__u8)m_ModeTbl[m_ModeNow].PageCnt - 1;
		}
		else
		{
			Tv_Page --;
		}

		QuadModeSet(m_ModeNow, Tv_Page);	// try change
}
//--------------------------------------------------------------------
__s8	Cls_GrDvrQuadCtrl::ViewIdxGetByPos(__s32 A_X, __s32 A_Y)
{
	// local -------------------
		__s8	Tv_Result;
		__u8	Tv_VwCnt;
		__u8	Tv_VwIdx;
		Ptr_GrRect	Tv_PtrRc;
	// code --------------------
		Tv_Result	=	-1;

		// check able mode
		if(0 != m_ModeTbl[m_ModeNow].PageCnt)
		{
			// get now page count
			Tv_VwCnt	=	ViewNowCountGet();
			if(0 != Tv_VwCnt)
			{
				Tv_PtrRc	=	m_ModeTbl[m_ModeNow].PtrVwRc;
				for( Tv_VwIdx = 0; Tv_VwIdx < Tv_VwCnt; Tv_VwIdx++)
				{
					if((Tv_PtrRc[Tv_VwIdx].left <= A_X) && (Tv_PtrRc[Tv_VwIdx].right > A_X) && 
						(Tv_PtrRc[Tv_VwIdx].top <= A_Y) && (Tv_PtrRc[Tv_VwIdx].bottom > A_Y))
					{
						// found
						Tv_Result	=	(__s8)Tv_VwIdx;
						break;
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__s8	Cls_GrDvrQuadCtrl::ViewIdxGetByCh(__u8 A_Ch)
{
	// local -------------------
		__s8	Tv_Result;
		__u8	Tv_VwCnt;
		__u8	Tv_WkIdx;
		__u32	Tv_Pos;
	// code --------------------
		Tv_Result	=	-1;

		// check able mode
		if(0 != m_ModeTbl[m_ModeNow].PageCnt)
		{
			// get now page count
			Tv_VwCnt	=	ViewNowCountGet();
			if(0 != Tv_VwCnt)
			{
				Tv_Pos		=	((__u32)m_PageNow * (__u32)Tv_VwCnt);
				
				for(Tv_WkIdx = 0; Tv_WkIdx < Tv_VwCnt; Tv_WkIdx++)
				{
					if(A_Ch == m_ModeTbl[m_ModeNow].PtrMap[Tv_Pos])
					{
						// found
						Tv_Result	=	(__s8)Tv_WkIdx;
						break;
					}
					// next
					Tv_Pos ++;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__s8	Cls_GrDvrQuadCtrl::PageGetByCh(__u8 A_Mode, __u8 A_Ch)
{
	// local -------------------
		__s8	Tv_Result;
		__u8	Tv_VwCnt;
		__u8	Tv_VwIdx;
		__u32	Tv_PgIdx;
		__s8*	Tv_PtrMap;
	// code --------------------
		Tv_Result	=	-1;

		if ((m_ModeMax >= A_Mode) && (m_ChMaxCnt > A_Ch))
		{
			if (0 != m_ModeTbl[A_Mode].PageCnt)
			{
				Tv_VwCnt	=	GrDvrQuadViewCountGetByMode(A_Mode);
				Tv_PtrMap	=	m_ModeTbl[A_Mode].PtrMap;

				for ( Tv_PgIdx = 0; Tv_PgIdx < m_ModeTbl[A_Mode].PageCnt; Tv_PgIdx++)
				{
					for ( Tv_VwIdx = 0; Tv_VwIdx < Tv_VwCnt; Tv_VwIdx++)
					{
						// check compare
						if (A_Ch == (__u8)(*Tv_PtrMap))
						{
							// found
							Tv_Result	=	(__s8)((__s32)Tv_PgIdx);
							break;
						}
						//next
						Tv_PtrMap ++;
					}
					if(0 <= Tv_Result)
					{
						break;
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrQuadCtrl::QuadModeExchange(__u8 A_Ch)
{
	// local -------------------
		__s8	Tv_Page;
	// code --------------------
		// check exist last mode
		if (E_GrDvrQuadMode1 != m_ModeMtLast)
		{
			// check now mode
			if(E_GrDvrQuadMode1 == m_ModeNow)
			{
				// get page
				Tv_Page		=	PageGetByCh(m_ModeMtLast, A_Ch);
				if(0 <= Tv_Page)
				{
					// change mode
					QuadModeSet(m_ModeMtLast, (__u8)Tv_Page);
				}
			}
			else
			{
				Tv_Page	=	PageGetByCh(E_GrDvrQuadMode1, A_Ch);
				QuadModeSet(E_GrDvrQuadMode1, (__u8)Tv_Page);
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrQuadCtrl::LineWidthSet(__u32 A_Width)
{
	// local -------------------
	// code --------------------
		if(0 != A_Width)
		{
			m_LineWidth	=	A_Width;

			// build line
			LineTableBuild();
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrQuadCtrl::LineWidthGet(void)
{
		return	m_LineWidth;
}
//--------------------------------------------------------------------
void	Cls_GrDvrQuadCtrl::LineTableBuild(void)
{
	// local -------------------
		__u32	Tv_MdIdx;
		__u32	Tv_LnCnt;
		__u32	Tv_LnIdx;
		Ptr_GrByteRect	Tv_PtrRcScl;
		St_GrRect	Tv_Rc;
		__s32	Tv_SzX;
		__s32	Tv_SzY;
	// code --------------------
		Tv_SzX	=	m_RcScr.right - m_RcScr.left;
		Tv_SzY	=	m_RcScr.bottom - m_RcScr.top;

		for( Tv_MdIdx = 0; Tv_MdIdx < E_GrDvrQuadModeCnt; Tv_MdIdx++)
		{
			if(0 != m_ModeTbl[Tv_MdIdx].PageCnt)
			{
				// check exist line
				Tv_LnCnt	=	(__u32)V_GrDvrQdLineCntTbl[Tv_MdIdx];
				if(0 != Tv_LnCnt)
				{
					// build 
					Tv_PtrRcScl	=	V_GrDvrQdLineTbl[Tv_MdIdx];

					for(Tv_LnIdx = 0; Tv_LnIdx < Tv_LnCnt; Tv_LnIdx ++)
					{
						Tv_Rc.left		=	m_RcScr.left + (CalcScale(Tv_SzX, Tv_PtrRcScl[Tv_LnIdx].left) & m_XalgMask);
						Tv_Rc.top			=	m_RcScr.top + (CalcScale(Tv_SzY, Tv_PtrRcScl[Tv_LnIdx].top) & m_YalgMask);
						Tv_Rc.right		=	m_RcScr.left + (CalcScale(Tv_SzX, Tv_PtrRcScl[Tv_LnIdx].right) & m_XalgMask);
						Tv_Rc.bottom	=	m_RcScr.top + (CalcScale(Tv_SzY, Tv_PtrRcScl[Tv_LnIdx].bottom) & m_YalgMask);
						// check horizontal or vertical
						if(Tv_Rc.top == Tv_Rc.bottom)
						{
							// horizontal
							Tv_Rc.top			=	Tv_Rc.top - (__s32)(m_LineWidth >> 1);
							Tv_Rc.bottom	=	Tv_Rc.top + (__s32)m_LineWidth;
						}
						else
						{
							// vertical
							Tv_Rc.left	=	Tv_Rc.left - (__s32)(m_LineWidth >> 1);
							Tv_Rc.right	=	Tv_Rc.left + (__s32)m_LineWidth;
						}

						// update
						m_ModeTbl[Tv_MdIdx].PtrLnRc[Tv_LnIdx]	=	Tv_Rc;
					}
				}
			}
		}
}
//--------------------------------------------------------------------
Ptr_GrRect	Cls_GrDvrQuadCtrl::LineTblPtrGetByMode(__u8 A_Mode, __u32* A_PtrRtCnt)
{
	// local -------------------
		Ptr_GrRect	Tv_Result;
	// code --------------------
		Tv_Result		=	NULL;
		*A_PtrRtCnt	=	0;

		if(E_GrDvrQuadModeCnt > A_Mode)
		{
			if(0 != m_ModeTbl[A_Mode].PageCnt)
			{
				if(0 != V_GrDvrQdLineCntTbl[A_Mode])
				{
					*A_PtrRtCnt	=	(__u32)V_GrDvrQdLineCntTbl[A_Mode];
					Tv_Result	=	m_ModeTbl[A_Mode].PtrLnRc;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Ptr_GrRect	Cls_GrDvrQuadCtrl::LineTblPtrGetNowMode(__u32* A_PtrRtCnt)
{
	// local -------------------
	// code --------------------
		return	LineTblPtrGetByMode(m_ModeNow, A_PtrRtCnt);
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrQuadCtrl::ViewChMapGet(void)
{
	// local -------------------
		__u32	Tv_Result;
		__u8	Tv_VwCnt;
		__u8	Tv_VwIdx;
		__s8	Tv_ChVal;
		__u32	Tv_Pos;
	// code --------------------
		Tv_Result	=	0;

		Tv_VwCnt	=	ViewNowCountGet();
		if(0 != Tv_VwCnt)
		{
			Tv_Pos	=	((__u32)m_PageNow * (__u32)Tv_VwCnt);
			for( Tv_VwIdx = 0; Tv_VwIdx < Tv_VwCnt; Tv_VwIdx++)
			{
				Tv_ChVal	=	m_ModeTbl[m_ModeNow].PtrMap[Tv_Pos];
				if((0 <= Tv_ChVal) && (E_GrDvrMaxChCnt > Tv_ChVal))
				{
					Tv_Result	=	Tv_Result | (1 << Tv_ChVal);
				}
				// next
				Tv_Pos ++;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
