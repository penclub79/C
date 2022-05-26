/*
 Osd Manager

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

#include	<Ui/GrUiConsole.h>
#include	<GrStrTool.h>
#include	<GrFileTool.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<Ui/GrUiMng.h>
#include	<Ui/GrUiScptDec.h>
#include	<Ui/GrUiDoVkbdBase.h>
#include	<Ui/GrUiDoVkbdTxt.h>
#include	<Ui/GrUiDoVkbdNum.h>
#include	<Ui/GrUiDoMsgbox.h>
#include	<GrTaskSvr.h>
#include	<GrDebug.h>

#include	<stdlib.h>

//====================================================================
//local const
#define E_GrUiConsoleCursorTimeOut		50

//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var
extern	Cls_GrUiMng*		V_GrUiMngObj;

#ifdef GR_OSD_EDIT

extern	__u16	V_GrUiDvlQuadSizeX;
extern	__u16	V_GrUiDvlQuadSizeY;
extern	__u16	V_GrUiDvlQuadPalExt;

St_GrByteRect	V_GrUiConQdRtMd1[E_GrDvrQuadVwCntMd1]	=	
{
		{	E_GrDvrQuadSclZero,	E_GrDvrQuadSclZero,	E_GrDvrQuadScl1_1,	E_GrDvrQuadScl1_1	}
};

St_GrByteRect	V_GrUiConQdRtMd4[E_GrDvrQuadVwCntMd4]	=	
{
		{	E_GrDvrQuadSclZero,	E_GrDvrQuadSclZero,	E_GrDvrQuadScl1_2,	E_GrDvrQuadScl1_2	},	
		{	E_GrDvrQuadScl1_2,	E_GrDvrQuadSclZero,	E_GrDvrQuadScl1_1,	E_GrDvrQuadScl1_2	},	
		{	E_GrDvrQuadSclZero,	E_GrDvrQuadScl1_2,	E_GrDvrQuadScl1_2,	E_GrDvrQuadScl1_1	},	
		{	E_GrDvrQuadScl1_2,	E_GrDvrQuadScl1_2,	E_GrDvrQuadScl1_1,	E_GrDvrQuadScl1_1	}
};

St_GrByteRect	V_GrUiConQdRtMd6[E_GrDvrQuadVwCntMd6]	=	
{
		{	E_GrDvrQuadSclZero,	E_GrDvrQuadSclZero,	E_GrDvrQuadScl2_3,	E_GrDvrQuadScl2_3	},																
		{	E_GrDvrQuadScl2_3,	E_GrDvrQuadSclZero,	E_GrDvrQuadScl1_1,	E_GrDvrQuadScl1_3	},	
		{	E_GrDvrQuadScl2_3,	E_GrDvrQuadScl1_3,	E_GrDvrQuadScl1_1,	E_GrDvrQuadScl2_3	},	
		{	E_GrDvrQuadSclZero,	E_GrDvrQuadScl2_3,	E_GrDvrQuadScl1_3,	E_GrDvrQuadScl1_1	},	
		{	E_GrDvrQuadScl1_3,	E_GrDvrQuadScl2_3,	E_GrDvrQuadScl2_3,	E_GrDvrQuadScl1_1	},	
		{	E_GrDvrQuadScl2_3,	E_GrDvrQuadScl2_3,	E_GrDvrQuadScl1_1,	E_GrDvrQuadScl1_1	}
};

St_GrByteRect	V_GrUiConQdRtMd8[E_GrDvrQuadVwCntMd8]	=	
{
		{	E_GrDvrQuadSclZero, E_GrDvrQuadSclZero, E_GrDvrQuadScl3_4, E_GrDvrQuadScl3_4 },																											
		{	E_GrDvrQuadScl3_4, E_GrDvrQuadSclZero, E_GrDvrQuadScl1_1, E_GrDvrQuadScl1_4 },	
		{	E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_4,	E_GrDvrQuadScl1_1, E_GrDvrQuadScl1_2 },	
		{	E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_2,	E_GrDvrQuadScl1_1, E_GrDvrQuadScl3_4 },	
		{	E_GrDvrQuadSclZero, E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_4, E_GrDvrQuadScl1_1	},	
		{	E_GrDvrQuadScl1_4, E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_2, E_GrDvrQuadScl1_1 },	
		{	E_GrDvrQuadScl1_2, E_GrDvrQuadScl3_4, E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_1 },	
		{	E_GrDvrQuadScl3_4, E_GrDvrQuadScl3_4,	E_GrDvrQuadScl1_1, E_GrDvrQuadScl1_1 }
};

St_GrByteRect	V_GrUiConQdRtMd9[E_GrDvrQuadVwCntMd9]	=	
{
		{	E_GrDvrQuadSclZero, E_GrDvrQuadSclZero, E_GrDvrQuadScl1_3, E_GrDvrQuadScl1_3	},		
		{	E_GrDvrQuadScl1_3,	E_GrDvrQuadSclZero,	E_GrDvrQuadScl2_3, E_GrDvrQuadScl1_3	},	
		{	E_GrDvrQuadScl2_3,	E_GrDvrQuadSclZero,	E_GrDvrQuadScl1_1, E_GrDvrQuadScl1_3	},	
		{	E_GrDvrQuadSclZero, E_GrDvrQuadScl1_3, E_GrDvrQuadScl1_3, E_GrDvrQuadScl2_3	},	
		{	E_GrDvrQuadScl1_3, E_GrDvrQuadScl1_3,	E_GrDvrQuadScl2_3, E_GrDvrQuadScl2_3 },		
		{	E_GrDvrQuadScl2_3, E_GrDvrQuadScl1_3,	E_GrDvrQuadScl1_1, E_GrDvrQuadScl2_3	},	
		{	E_GrDvrQuadSclZero, E_GrDvrQuadScl2_3,	E_GrDvrQuadScl1_3, E_GrDvrQuadScl1_1	},	
		{	E_GrDvrQuadScl1_3, E_GrDvrQuadScl2_3, E_GrDvrQuadScl2_3, E_GrDvrQuadScl1_1 },		
		{	E_GrDvrQuadScl2_3, E_GrDvrQuadScl2_3, E_GrDvrQuadScl1_1, E_GrDvrQuadScl1_1	}
};

St_GrByteRect	V_GrUiConQdRtMd10[E_GrDvrQuadVwCntMd10]	=	
{
		{	E_GrDvrQuadSclZero, E_GrDvrQuadSclZero, E_GrDvrQuadScl1_2, E_GrDvrQuadScl1_2 },															
		{	E_GrDvrQuadScl1_2, E_GrDvrQuadSclZero, E_GrDvrQuadScl1_1, E_GrDvrQuadScl1_2 },	
		{	E_GrDvrQuadSclZero, E_GrDvrQuadScl1_2, E_GrDvrQuadScl1_4, E_GrDvrQuadScl3_4	},	
		{	E_GrDvrQuadScl1_4, E_GrDvrQuadScl1_2, E_GrDvrQuadScl1_2, E_GrDvrQuadScl3_4 },	
		{	E_GrDvrQuadScl1_2, E_GrDvrQuadScl1_2, E_GrDvrQuadScl3_4, E_GrDvrQuadScl3_4 },	
		{	E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_2,	E_GrDvrQuadScl1_1, E_GrDvrQuadScl3_4 },
		{	E_GrDvrQuadSclZero, E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_4, E_GrDvrQuadScl1_1	},	
		{	E_GrDvrQuadScl1_4, E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_2, E_GrDvrQuadScl1_1 },	
		{	E_GrDvrQuadScl1_2, E_GrDvrQuadScl3_4, E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_1 },	
		{	E_GrDvrQuadScl3_4, E_GrDvrQuadScl3_4,	E_GrDvrQuadScl1_1, E_GrDvrQuadScl1_1 }
};

St_GrByteRect	V_GrUiConQdRtMd13[E_GrDvrQuadVwCntMd13]	=	
{
		{	E_GrDvrQuadScl1_4, E_GrDvrQuadScl1_4, E_GrDvrQuadScl3_4, E_GrDvrQuadScl3_4 },
		{	E_GrDvrQuadSclZero, E_GrDvrQuadSclZero, E_GrDvrQuadScl1_4, E_GrDvrQuadScl1_4	},		
		{	E_GrDvrQuadScl1_4, E_GrDvrQuadSclZero, E_GrDvrQuadScl1_2, E_GrDvrQuadScl1_4 },		
		{	E_GrDvrQuadScl1_2, E_GrDvrQuadSclZero, E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_4 },		
		{	E_GrDvrQuadScl3_4, E_GrDvrQuadSclZero,	E_GrDvrQuadScl1_1, E_GrDvrQuadScl1_4 },
		{	E_GrDvrQuadSclZero, E_GrDvrQuadScl1_4, E_GrDvrQuadScl1_4, E_GrDvrQuadScl1_2	},																									
		{	E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_4,	E_GrDvrQuadScl1_1, E_GrDvrQuadScl1_2 },
		{	E_GrDvrQuadSclZero, E_GrDvrQuadScl1_2, E_GrDvrQuadScl1_4, E_GrDvrQuadScl3_4	},																									
		{	E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_2,	E_GrDvrQuadScl1_1, E_GrDvrQuadScl3_4 },
		{	E_GrDvrQuadSclZero, E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_4, E_GrDvrQuadScl1_1	},	
		{	E_GrDvrQuadScl1_4, E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_2, E_GrDvrQuadScl1_1 },	
		{	E_GrDvrQuadScl1_2, E_GrDvrQuadScl3_4, E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_1 },	
		{	E_GrDvrQuadScl3_4, E_GrDvrQuadScl3_4,	E_GrDvrQuadScl1_1, E_GrDvrQuadScl1_1 }
};

St_GrByteRect	V_GrUiConQdRtMd16[E_GrDvrQuadVwCntMd16]	=	
{
		{	E_GrDvrQuadSclZero, E_GrDvrQuadSclZero, E_GrDvrQuadScl1_4, E_GrDvrQuadScl1_4	},		
		{	E_GrDvrQuadScl1_4, E_GrDvrQuadSclZero, E_GrDvrQuadScl1_2, E_GrDvrQuadScl1_4 },		
		{	E_GrDvrQuadScl1_2, E_GrDvrQuadSclZero, E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_4 },		
		{	E_GrDvrQuadScl3_4, E_GrDvrQuadSclZero,	E_GrDvrQuadScl1_1, E_GrDvrQuadScl1_4 },
		{	E_GrDvrQuadSclZero, E_GrDvrQuadScl1_4, E_GrDvrQuadScl1_4, E_GrDvrQuadScl1_2	},	
		{	E_GrDvrQuadScl1_4, E_GrDvrQuadScl1_4, E_GrDvrQuadScl1_2, E_GrDvrQuadScl1_2 },	
		{	E_GrDvrQuadScl1_2, E_GrDvrQuadScl1_4, E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_2 },	
		{	E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_4,	E_GrDvrQuadScl1_1, E_GrDvrQuadScl1_2 },
		{	E_GrDvrQuadSclZero, E_GrDvrQuadScl1_2, E_GrDvrQuadScl1_4, E_GrDvrQuadScl3_4	},	
		{	E_GrDvrQuadScl1_4, E_GrDvrQuadScl1_2, E_GrDvrQuadScl1_2, E_GrDvrQuadScl3_4 },	
		{	E_GrDvrQuadScl1_2, E_GrDvrQuadScl1_2, E_GrDvrQuadScl3_4, E_GrDvrQuadScl3_4 },	
		{	E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_2,	E_GrDvrQuadScl1_1, E_GrDvrQuadScl3_4 },
		{	E_GrDvrQuadSclZero, E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_4, E_GrDvrQuadScl1_1	},	
		{	E_GrDvrQuadScl1_4, E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_2, E_GrDvrQuadScl1_1 },	
		{	E_GrDvrQuadScl1_2, E_GrDvrQuadScl3_4, E_GrDvrQuadScl3_4, E_GrDvrQuadScl1_1 },	
		{	E_GrDvrQuadScl3_4, E_GrDvrQuadScl3_4,	E_GrDvrQuadScl1_1, E_GrDvrQuadScl1_1 }
};

Ptr_GrByteRect	V_GrUiConQdRtTbl[E_GrDvrQuadModeCnt]	=	
{
	V_GrUiConQdRtMd1,		V_GrUiConQdRtMd4,		V_GrUiConQdRtMd6,		V_GrUiConQdRtMd8,
	V_GrUiConQdRtMd9,		V_GrUiConQdRtMd10,	V_GrUiConQdRtMd13,	V_GrUiConQdRtMd16
};

St_GrUiConQdLnRt	V_GrUiConQdLnTblMd4[2]	=	
{
	{	{	E_GrDvrQuadSclZero , E_GrDvrQuadScl1_2 },	{	E_GrDvrQuadScl1_1 , E_GrDvrQuadScl1_2 }	},
	{	{	E_GrDvrQuadScl1_2 , E_GrDvrQuadSclZero },	{	E_GrDvrQuadScl1_2 , E_GrDvrQuadScl1_1 }	}
};

St_GrUiConQdLnRt	V_GrUiConQdLnTblMd6[4]	=	
{
	{	{	E_GrDvrQuadSclZero , E_GrDvrQuadScl2_3 },	{	E_GrDvrQuadScl1_1 , E_GrDvrQuadScl2_3 }	},
	{	{	E_GrDvrQuadScl2_3 , E_GrDvrQuadScl1_3 },	{	E_GrDvrQuadScl1_1 , E_GrDvrQuadScl1_3 }	},
	{	{	E_GrDvrQuadScl2_3 , E_GrDvrQuadSclZero },	{	E_GrDvrQuadScl2_3 , E_GrDvrQuadScl1_1 }	},
	{	{	E_GrDvrQuadScl1_3 , E_GrDvrQuadScl2_3 },	{	E_GrDvrQuadScl1_3 , E_GrDvrQuadScl1_1 }	}
};

St_GrUiConQdLnRt	V_GrUiConQdLnTblMd8[6]	=	
{
	{	{	E_GrDvrQuadSclZero , E_GrDvrQuadScl3_4 },	{	E_GrDvrQuadScl1_1 , E_GrDvrQuadScl3_4 }	},
	{	{	E_GrDvrQuadScl3_4 , E_GrDvrQuadScl1_4 },	{	E_GrDvrQuadScl1_1 , E_GrDvrQuadScl1_4 }	},
	{	{	E_GrDvrQuadScl3_4 , E_GrDvrQuadScl1_2 },	{	E_GrDvrQuadScl1_1 , E_GrDvrQuadScl1_2 }	},

	{	{	E_GrDvrQuadScl3_4 , E_GrDvrQuadSclZero },	{	E_GrDvrQuadScl3_4 , E_GrDvrQuadScl1_1 }	},
	{	{	E_GrDvrQuadScl1_4 , E_GrDvrQuadScl3_4 },	{	E_GrDvrQuadScl1_4 , E_GrDvrQuadScl1_1 }	},
	{	{	E_GrDvrQuadScl1_2 , E_GrDvrQuadScl3_4 },	{	E_GrDvrQuadScl1_2 , E_GrDvrQuadScl1_1 }	}
};

St_GrUiConQdLnRt	V_GrUiConQdLnTblMd9[4]	=	
{
	{	{	E_GrDvrQuadSclZero , E_GrDvrQuadScl1_3 },	{	E_GrDvrQuadScl1_1 , E_GrDvrQuadScl1_3 }	},
	{	{	E_GrDvrQuadSclZero , E_GrDvrQuadScl2_3 },	{	E_GrDvrQuadScl1_1 , E_GrDvrQuadScl2_3 }	},
	{	{	E_GrDvrQuadScl1_3 , E_GrDvrQuadSclZero },	{	E_GrDvrQuadScl1_3 , E_GrDvrQuadScl1_1 }	},
	{	{	E_GrDvrQuadScl2_3 , E_GrDvrQuadSclZero },	{	E_GrDvrQuadScl2_3 , E_GrDvrQuadScl1_1 }	}
};

St_GrUiConQdLnRt	V_GrUiConQdLnTblMd10[5]	=	
{
	{	{	E_GrDvrQuadSclZero , E_GrDvrQuadScl1_2 },	{	E_GrDvrQuadScl1_1 , E_GrDvrQuadScl1_2 }	},
	{	{	E_GrDvrQuadScl1_2 , E_GrDvrQuadSclZero },	{	E_GrDvrQuadScl1_2 , E_GrDvrQuadScl1_1 }	},

	{	{	E_GrDvrQuadSclZero , E_GrDvrQuadScl3_4 },	{	E_GrDvrQuadScl1_1 , E_GrDvrQuadScl3_4 }	},

	{	{	E_GrDvrQuadScl1_4 , E_GrDvrQuadScl1_2 },	{	E_GrDvrQuadScl1_4 , E_GrDvrQuadScl1_1 }	},
	{	{	E_GrDvrQuadScl3_4 , E_GrDvrQuadScl1_2 },	{	E_GrDvrQuadScl3_4 , E_GrDvrQuadScl1_1 }	},
};

St_GrUiConQdLnRt	V_GrUiConQdLnTblMd13[8]	=	
{
	{	{	E_GrDvrQuadSclZero , E_GrDvrQuadScl1_4 },	{	E_GrDvrQuadScl1_1 , E_GrDvrQuadScl1_4 }	},
	{	{	E_GrDvrQuadSclZero , E_GrDvrQuadScl3_4 },	{	E_GrDvrQuadScl1_1 , E_GrDvrQuadScl3_4 }	},
	{	{	E_GrDvrQuadSclZero , E_GrDvrQuadScl1_2 },	{	E_GrDvrQuadScl1_4 , E_GrDvrQuadScl1_2 }	},
	{	{	E_GrDvrQuadScl3_4 , E_GrDvrQuadScl1_2 },	{	E_GrDvrQuadScl1_1 , E_GrDvrQuadScl1_2 }	},

	{	{	E_GrDvrQuadScl1_4 , E_GrDvrQuadSclZero },	{	E_GrDvrQuadScl1_4 , E_GrDvrQuadScl1_1 }	},
	{	{	E_GrDvrQuadScl3_4 , E_GrDvrQuadSclZero },	{	E_GrDvrQuadScl3_4 , E_GrDvrQuadScl1_1 }	},
	{	{	E_GrDvrQuadScl1_2 , E_GrDvrQuadSclZero },	{	E_GrDvrQuadScl1_2 , E_GrDvrQuadScl1_4 }	},
	{	{	E_GrDvrQuadScl1_2 , E_GrDvrQuadScl3_4 },	{	E_GrDvrQuadScl1_2 , E_GrDvrQuadScl1_1 }	},
};

St_GrUiConQdLnRt	V_GrUiConQdLnTblMd16[6]	=	
{
	{	{	E_GrDvrQuadSclZero , E_GrDvrQuadScl1_4 },	{	E_GrDvrQuadScl1_1 , E_GrDvrQuadScl1_4 }	},
	{	{	E_GrDvrQuadSclZero , E_GrDvrQuadScl1_2 },	{	E_GrDvrQuadScl1_1 , E_GrDvrQuadScl1_2 }	},
	{	{	E_GrDvrQuadSclZero , E_GrDvrQuadScl3_4 },	{	E_GrDvrQuadScl1_1 , E_GrDvrQuadScl3_4 }	},

	{	{	E_GrDvrQuadScl1_4 , E_GrDvrQuadSclZero },	{	E_GrDvrQuadScl1_4 , E_GrDvrQuadScl1_1 }	},
	{	{	E_GrDvrQuadScl1_2 , E_GrDvrQuadSclZero },	{	E_GrDvrQuadScl1_2 , E_GrDvrQuadScl1_1 }	},
	{	{	E_GrDvrQuadScl3_4 , E_GrDvrQuadSclZero },	{	E_GrDvrQuadScl3_4 , E_GrDvrQuadScl1_1 }	},
};

Ptr_GrUiConQdLnRt	V_GrUiConQdLineTbl[E_GrDvrQuadModeCnt]	=	
{
	NULL,	V_GrUiConQdLnTblMd4,	V_GrUiConQdLnTblMd6,	V_GrUiConQdLnTblMd8,
	V_GrUiConQdLnTblMd9,	V_GrUiConQdLnTblMd10,	V_GrUiConQdLnTblMd13,	V_GrUiConQdLnTblMd16
};	

__u8	V_GrUiConQdLineCntTbl[E_GrDvrQuadModeCnt]	=	
{
	0,	2,	4,	6,	4,	5,	8,	6
};

__u8		V_GrUiConQuadPgCntTbl[E_GrDvrQuadModeCnt]	=	
{	E_GrDvrQuadPgCntMd1,	E_GrDvrQuadPgCntMd4,	E_GrDvrQuadPgCntMd6,
	E_GrDvrQuadPgCntMd8,	E_GrDvrQuadPgCntMd9,	E_GrDvrQuadPgCntMd10,
	E_GrDvrQuadPgCntMd13,	E_GrDvrQuadPgCntMd16	};

#endif
//====================================================================
//--------------------------------------------------------------------
Cls_GrUiConsole::Cls_GrUiConsole( Cls_GrGdib* A_GdibScr )
{
	// local -------------------
	// code --------------------
		//init
		m_ConIdx			=	E_GrUiConIdxFocused;		//not allocated
		m_GdibScr			=	A_GdibScr;
		m_IsMseCusOn	=	FALSE;
		m_MseCusImg		=	E_GrUiMouseCusImgArrow;	//default cursor
		m_ObjFocus		=	NULL;
		m_IsPntMsg		=	FALSE;
		m_ObjOnMse		=	NULL;
		m_MseCusTime	=	0;

		m_IsFocusCrtReq	=	FALSE;

		m_IsPlayMode	=	FALSE;

		m_QuadMode		=	E_GrDvrQuadMode1;
		m_QuadPage		=	0;
		m_CorvertMap	=	0;
		m_IsAllCvt		=	FALSE;
		m_CvtUsrMap		=	0;
		GrDumyZeroMem( &m_PntSlot, sizeof(m_PntSlot) );
		GrDumyZeroMem( m_LayerTbl, sizeof(m_LayerTbl) );
		GrDumyZeroMem( m_IdAlocTbl, sizeof(m_IdAlocTbl) );

		if ( NULL != m_GdibScr )
		{
			//update rect
			m_ScrRect.left		=	0;
			m_ScrRect.top			=	0;
			m_ScrRect.right		=	(__s32)m_GdibScr->GetResX();
			m_ScrRect.bottom	=	(__s32)m_GdibScr->GetResY();
			LcInitColor();
		}

		m_QuadViewCnt		=	1;

		HalMouseCusOnOff();		//cursor off
}
//--------------------------------------------------------------------
Cls_GrUiConsole::~Cls_GrUiConsole()
{
		//release all object
		WindowAllClose( E_GrUiWinIdNone, TRUE );
}
//--------------------------------------------------------------------
void	Cls_GrUiConsole::LcInitColor( void )
{
		if ( m_GdibScr->IsPalleteUse() )
		{
			m_CursClr			=	E_GrUiConPltClrBlack;
			m_ClrWhite		=	E_GrUiConPltClrWhite;
			m_ClrBlack		=	E_GrUiConPltClrBlack;
		}
		else
		{
			m_CursClr		=	GrDrawCvtColorByFcc( E_GrFccRGB24, 0x0080FF , m_GdibScr->GetFcc() );
			m_ClrBlack	=	GrDrawGetColorBlack( m_GdibScr->GetFcc() );
			m_ClrWhite	=	GrDrawGetColorWhite( m_GdibScr->GetFcc() );
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrUiConsole::IndexGet( void )
{
		return	m_ConIdx;
}
//--------------------------------------------------------------------
void		Cls_GrUiConsole::IndexSet( __u8 A_Idx )
{
		m_ConIdx	=	A_Idx;
}
//--------------------------------------------------------------------
__u32		Cls_GrUiConsole::GetResX( void )
{
		return	m_ScrRect.right - m_ScrRect.left;
}
//--------------------------------------------------------------------
__u32		Cls_GrUiConsole::GetResY( void )
{
		return	m_ScrRect.bottom - m_ScrRect.top;
}
//--------------------------------------------------------------------
__u32		Cls_GrUiConsole::GetFcc( void )
{
		return	m_GdibScr->GetFcc();
}
//--------------------------------------------------------------------
Cls_GrGdib*	Cls_GrUiConsole::GetGdibScr( void )
{
		return	m_GdibScr;
}
//--------------------------------------------------------------------
__u32	Cls_GrUiConsole::CursColorGet( void )
{
		return	m_CursClr;
}
//--------------------------------------------------------------------
void	Cls_GrUiConsole::CursColorSet( __u32 A_Color )
{
		m_CursClr		=	A_Color;
}
//--------------------------------------------------------------------
Cls_GrUiDoBase*	Cls_GrUiConsole::FocusObjGet( void )
{
	// local -------------------
		Cls_GrUiDoBase*	Tv_Result;
	// code --------------------
		Tv_Result	=	m_ObjFocus;
		if ( m_IsMseCusOn )
		{
			Tv_Result	=	NULL;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiConsole::FocusObjSet( Cls_GrUiDoBase* A_ObjUiDo )
{
	// local -------------------
		BOOL8	Tv_IsAble;
	// code --------------------
		Tv_IsAble	=	FALSE;
		if ( m_ObjFocus != A_ObjUiDo )
		{
			Tv_IsAble	=	TRUE;
			if ( NULL != A_ObjUiDo )
			{
				Tv_IsAble	=	A_ObjUiDo->IsCanFocus();
			}
		}

		//check changed
		if ( Tv_IsAble )
		{
			//check old focus exist
			if ( NULL != m_ObjFocus )
			{
				//focus out event
				LcPostFocusLossMsg( m_ObjFocus );
			}
			//set new focus
			m_ObjFocus	=	A_ObjUiDo;
			//check new object
			if ( NULL != m_ObjFocus )
			{
				//focus on event
				LcPostFocusGetMsg( m_ObjFocus );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiConsole::FocusReset( void )
{
		m_ObjFocus		=	NULL;
}
//--------------------------------------------------------------------
void	Cls_GrUiConsole::FocusCorrect( void )
{
	// local -------------------
		St_GrUiMsgHd	Tv_Msg;
	// code --------------------
		//check requested
		if ( !m_IsFocusCrtReq )
		{
			Tv_Msg.Cmd		=	E_GrUiMsgCmdConFocusCrt;
			Tv_Msg.ConIdx	=	E_GrUiConIdxFocused;
			Tv_Msg.WinId	=	E_GrUiWinIdNone;
			Tv_Msg.ObjId	=	E_GrUiObjIdNone;
			m_IsFocusCrtReq	=	UiMsgPost( &Tv_Msg, sizeof(Tv_Msg) );	//request
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiConsole::IsExistWin( Cls_GrUiDoWin* A_ObjWin )
{
	// local -------------------
		BOOL8	Tv_Result;
		__s32		Tv_LayIdx;
		__s32		Tv_WinIdx;
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		//find
		for ( Tv_LayIdx=0;Tv_LayIdx < E_GrUiConLayerCnt;Tv_LayIdx++	)
		{
			for ( Tv_WinIdx=0;Tv_WinIdx < E_GrUiConWinMaxCntOfLayer;Tv_WinIdx++ )
			{
				//check match window
				if ( A_ObjWin == m_LayerTbl[Tv_LayIdx][Tv_WinIdx] )
				{
					//finded
					Tv_Result	=	TRUE;
					break;
				}
			}
			if ( Tv_Result )
			{
				break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiConsole::WindowClose( __u16 A_WinId, BOOL8 A_IsNow )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrUiMsgConWinClose	Tv_Msg;
	// code --------------------

	#ifndef LINUX_APP
		if ( A_WinId == 0xFFFF )
		{
			MessageBoxW( NULL, L"WindowClose : Bad window id!", L"error", MB_OK | MB_ICONERROR );
		}
	#endif

		Tv_Msg.Hd.Cmd			=	E_GrUiMsgCmdConWinClose;
		Tv_Msg.Hd.ConIdx	=	E_GrUiConIdxFocused;
		Tv_Msg.Hd.WinId		=	E_GrUiWinIdNone;				//console message mode
		Tv_Msg.Hd.ObjId		=	E_GrUiObjIdNone;
		Tv_Msg.Mode				=	E_GrUiWinCloseWin;			//close window mode
		Tv_Msg.Idx				=	A_WinId;
		Tv_Msg.Para				=	NULL;
		if ( A_IsNow )
		{
			Tv_Result	=	UiMsgSend( (Ptr_GrUiMsgHd)&Tv_Msg );
		}
		else
		{
			Tv_Result	=	UiMsgPost( (Ptr_GrUiMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiConsole::WindowSafeClose( Cls_GrUiDoWin* A_ObjWin, BOOL8 A_IsNow )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrUiMsgConWinClose	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.Cmd			=	E_GrUiMsgCmdConWinClose;
		Tv_Msg.Hd.ConIdx	=	E_GrUiConIdxFocused;
		Tv_Msg.Hd.WinId		=	E_GrUiWinIdNone;				//console message mode
		Tv_Msg.Hd.ObjId		=	E_GrUiObjIdNone;
		Tv_Msg.Mode				=	E_GrUiWinCloseWin;			//close window mode
		Tv_Msg.Idx				=	A_ObjWin->WinIdGet();
		Tv_Msg.Para				=	(__u32)A_ObjWin;
		if ( A_IsNow )
		{
			Tv_Result	=	UiMsgSend( (Ptr_GrUiMsgHd)&Tv_Msg );
		}
		else
		{
			Tv_Result	=	UiMsgPost( (Ptr_GrUiMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiConsole::WindowLayerClose( __u8 A_Layer, __u16 A_ExceptWinId, BOOL8 A_IsNow )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrUiMsgConWinClose	Tv_Msg;
	// code --------------------

		Tv_Msg.Hd.Cmd			=	E_GrUiMsgCmdConWinClose;
		Tv_Msg.Hd.ConIdx	=	E_GrUiConIdxFocused;
		Tv_Msg.Hd.WinId		=	E_GrUiWinIdNone;					//console message mode
		Tv_Msg.Hd.ObjId		=	E_GrUiObjIdNone;					//console message mode
		Tv_Msg.Mode				=	E_GrUiWinCloseLayer;			//close all window in layer mode
		Tv_Msg.Idx				=	A_Layer;
		Tv_Msg.Para				=	A_ExceptWinId;
		if ( A_IsNow )
		{
			Tv_Result	=	UiMsgSend( (Ptr_GrUiMsgHd)&Tv_Msg );
		}
		else
		{
			Tv_Result	=	UiMsgPost( (Ptr_GrUiMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiConsole::WindowAllClose( __u16 A_ExceptWinId, BOOL8 A_IsNow )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrUiMsgConWinClose	Tv_Msg;
	// code --------------------

		Tv_Msg.Hd.Cmd			=	E_GrUiMsgCmdConWinClose;
		Tv_Msg.Hd.ConIdx	=	E_GrUiConIdxFocused;
		Tv_Msg.Hd.WinId		=	E_GrUiWinIdNone;				//console message mode
		Tv_Msg.Hd.ObjId		=	E_GrUiObjIdNone;
		Tv_Msg.Mode				=	E_GrUiWinCloseAll;			//close all window mode
		Tv_Msg.Idx				=	0;
		Tv_Msg.Para				=	(__u32)A_ExceptWinId;
		if ( A_IsNow )
		{
			Tv_Result	=	UiMsgSend( (Ptr_GrUiMsgHd)&Tv_Msg );
		}
		else
		{
			Tv_Result	=	UiMsgPost( (Ptr_GrUiMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiConsole::PaintReq( Ptr_GrRect A_PtrRect )
{
	// local -------------------
		__u32		Tv_WkIdx;
		__u32		Tv_Cover;
		BOOL8		Tv_IsRpl;		//replace mode
		BOOL8		Tv_IsOk;
	// code --------------------	
		//init
		Tv_IsRpl	=	FALSE;
		//check area
		Tv_IsOk		=	TRUE;
#ifndef LINUX_APP
		if ( A_PtrRect->bottom <= m_ScrRect.bottom )
		{
#endif
			Tv_IsOk		=	FALSE;
			//check paint item exist
			if ( 0 != m_PntSlot.ItmCnt )
			{
				for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiConMaxPaintItemCnt;Tv_WkIdx++ )
				{
					//check exist item
					if ( m_PntSlot.Item[Tv_WkIdx].IsUse )
					{
						if ( Tv_IsRpl )
						{
							//check replace
							Tv_Cover	=	GrDrawCnChkCoverRect( A_PtrRect, &m_PntSlot.Item[Tv_WkIdx].Rect );
							if ( 0 != (E_GrDrawChkOvlpCoverPara1 & Tv_Cover) )
							{
								//remove
								m_PntSlot.Item[Tv_WkIdx].IsUse	=	FALSE;
								m_PntSlot.ItmCnt --;
							}
						}
						else
						{
							//check rect
							Tv_Cover	=	GrDrawCnChkCoverRect( A_PtrRect, &m_PntSlot.Item[Tv_WkIdx].Rect );
							if ( 0 != (E_GrDrawChkOvlpCoverPara2 & Tv_Cover) )
							{
								//covered rect
								Tv_IsOk	=	TRUE;
								break;
							}
							else if ( E_GrDrawChkOvlpCoverPara1 == Tv_Cover )
							{
								//cover rect, replace rect
								m_PntSlot.Item[Tv_WkIdx].Rect	=	*A_PtrRect;
								Tv_IsOk		=	TRUE;
								Tv_IsRpl	=	TRUE;
							}
						}
					}
				}
			}
#ifndef LINUX_APP
		}
		else
		{
			DbgMsgPrint("GrUiConsile - Paint area is so bad, this requirment will skip.\n");
		}
#endif
		
		//check new 
		if ( !Tv_IsOk )
		{
			//check full
			if ( E_GrUiConMaxPaintItemCnt == m_PntSlot.ItmCnt )
			{
				//do full item
				m_PntSlot.ItmCnt							=	1;
				m_PntSlot.Item[0].IsUse				=	TRUE;
				m_PntSlot.Item[0].Rect.left		=	0;
				m_PntSlot.Item[0].Rect.top		=	0;
				m_PntSlot.Item[0].Rect.right	=	m_ScrRect.right;
				m_PntSlot.Item[0].Rect.bottom	=	m_ScrRect.bottom;
				//delete other
				for ( Tv_WkIdx=1;Tv_WkIdx < E_GrUiConMaxPaintItemCnt;Tv_WkIdx++ )
				{
					m_PntSlot.Item[Tv_WkIdx].IsUse	=	FALSE;
				}
			}
			else
			{
				//insert new
				for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiConMaxPaintItemCnt;Tv_WkIdx++ )
				{
					if ( !m_PntSlot.Item[Tv_WkIdx].IsUse )
					{
						//insert
						m_PntSlot.Item[Tv_WkIdx].Rect		=	*A_PtrRect;
						m_PntSlot.Item[Tv_WkIdx].IsUse	=	TRUE;
						m_PntSlot.ItmCnt ++;
						break;
					}
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiConsole::PaintReset( void )
{
	// local -------------------
		__u32		Tv_WkIdx;
	// code --------------------
		m_PntSlot.ItmCnt	=	0;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiConMaxPaintItemCnt;Tv_WkIdx++ )
		{
			m_PntSlot.Item[Tv_WkIdx].IsUse	=	FALSE;
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiConsole::LcProcImpPaint( void )
{
	// local -------------------
		__u32		Tv_ItmIdx;
	// code --------------------
		//check paint able
		if ( 0 != m_PntSlot.ItmCnt )
		{
			//find prcessable item
			Tv_ItmIdx	=	0;
			while ( (E_GrUiConMaxPaintItemCnt > Tv_ItmIdx) && (0 != m_PntSlot.ItmCnt) )
			{
				//check paint
				if ( m_PntSlot.Item[Tv_ItmIdx].IsUse )
				{
					//do paint
					//background clear
	#ifdef GR_OSD_EDIT
					m_GdibScr->ClipperSet( &m_PntSlot.Item[Tv_ItmIdx].Rect );
					m_GdibScr->FillRect( &m_PntSlot.Item[Tv_ItmIdx].Rect , 0 );
	#endif
					//patch rect
					if ( m_ScrRect.left > m_PntSlot.Item[Tv_ItmIdx].Rect.left )
					{
						m_PntSlot.Item[Tv_ItmIdx].Rect.left	=	m_ScrRect.left;
					}
					if ( m_ScrRect.top > m_PntSlot.Item[Tv_ItmIdx].Rect.top )
					{
						m_PntSlot.Item[Tv_ItmIdx].Rect.top	=	m_ScrRect.top;	
					}
					if ( m_ScrRect.right < m_PntSlot.Item[Tv_ItmIdx].Rect.right )
					{
						m_PntSlot.Item[Tv_ItmIdx].Rect.right	=	m_ScrRect.right;
					}
					if ( m_ScrRect.bottom < m_PntSlot.Item[Tv_ItmIdx].Rect.bottom )
					{
						m_PntSlot.Item[Tv_ItmIdx].Rect.bottom	=	m_ScrRect.bottom;
					}
					LcPaintByRect( &m_PntSlot.Item[Tv_ItmIdx].Rect );
					HalPntReqTransfer( &m_PntSlot.Item[Tv_ItmIdx].Rect );
					m_PntSlot.Item[Tv_ItmIdx].IsUse	=	FALSE;
				}
				//next
				Tv_ItmIdx ++;
			}
			m_PntSlot.ItmCnt	=	0;
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiConsole::LcPaintByRect( Ptr_GrRect A_PtrRc )
{
	// local -------------------
		__s32		Tv_LayIdx;
		__s32		Tv_WinIdx;
		__s32		Tv_LayPnt;		//start layer
		St_GrRect	Tv_Rc;
	// code --------------------
		//find start position
		Tv_LayPnt	=	0;
		for ( Tv_LayIdx=E_GrUiConLayerCnt - 1;0 < Tv_LayIdx;Tv_LayIdx-- )
		{
			for ( Tv_WinIdx=0;Tv_WinIdx < E_GrUiConWinMaxCntOfLayer;Tv_WinIdx++ )
			{
				//check exist window
				if ( NULL != m_LayerTbl[Tv_LayIdx][Tv_WinIdx] )
				{
					//check full cover window
					if ( !m_LayerTbl[Tv_LayIdx][Tv_WinIdx]->IsBgSpriteMode() )
					{
						//get rect
						if ( m_LayerTbl[Tv_LayIdx][Tv_WinIdx]->ScrRectGet( &Tv_Rc ) )
						{
							//check cover
							if ( (Tv_Rc.left <= A_PtrRc->left) && (Tv_Rc.top <= A_PtrRc->top) && (Tv_Rc.right >= A_PtrRc->right) && (Tv_Rc.bottom >= A_PtrRc->bottom) )
							{
								//finded
								Tv_LayPnt	=	Tv_LayIdx;
								break;
							}
						}
					}
				}
			}
			if ( 0 != Tv_LayPnt )
			{
				break;
			}
		}

		//do paint
		while ( Tv_LayPnt < E_GrUiConLayerCnt )
		{
			for ( Tv_WinIdx=0;Tv_WinIdx < E_GrUiConWinMaxCntOfLayer;Tv_WinIdx++ )
			{
				//check exist window
				if ( NULL != m_LayerTbl[Tv_LayPnt][Tv_WinIdx] )
				{
					m_LayerTbl[Tv_LayPnt][Tv_WinIdx]->RtlPaint( A_PtrRc );
				}
			}
			//next
			Tv_LayPnt ++;
		}
}
//--------------------------------------------------------------------
Cls_GrUiDoWin*	Cls_GrUiConsole::WindowFindByScptId( __u16 A_Id, __u32 A_Idx )
{
	// local -------------------
		__u32	Tv_RemCnt;				//find remain count
		Cls_GrUiDoWin*	Tv_Result;
		__s32		Tv_LayIdx;
		__s32		Tv_WinIdx;
	// code --------------------
		//init
		Tv_Result		=	NULL;
		Tv_RemCnt		=	A_Idx;

		//find
		for ( Tv_LayIdx=0;Tv_LayIdx < E_GrUiConLayerCnt;Tv_LayIdx++ )
		{
			for ( Tv_WinIdx=0;Tv_WinIdx < E_GrUiConWinMaxCntOfLayer;Tv_WinIdx++ )
			{
				//check exist window
				if ( NULL != m_LayerTbl[Tv_LayIdx][Tv_WinIdx] )
				{
					//check user id
					if ( A_Id == m_LayerTbl[Tv_LayIdx][Tv_WinIdx]->ObjIdGet() )
					{
						//find id
						if ( 0 == Tv_RemCnt )
						{
							Tv_Result	=	m_LayerTbl[Tv_LayIdx][Tv_WinIdx];
							break;
						}
						//next
						Tv_RemCnt --;
					}
				}
			}
			//finded
			if ( NULL != Tv_Result )
			{
				break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiConsole::MouseCusOn( BOOL8 A_IsOn )
{
	// local -------------------
		BOOL8	Tv_IsOld;
		__u16	Tv_X;
		__u16	Tv_Y;
	// code --------------------
		Tv_IsOld	=	m_IsMseCusOn;		//backup old
		m_IsMseCusOn	=	A_IsOn;			//update
		//check change to on
		if ( m_IsMseCusOn )
		{
			//check on
			if ( !Tv_IsOld )
			{
				//redraw focused object
				if ( NULL != m_ObjFocus )
				{
					//focus leave message
					LcPostFocusLossMsg( m_ObjFocus );
				}
				//get mouse position
				V_GrUiMngObj->MousePosGet( &Tv_X, &Tv_Y );
				//update mouse object
				LcMseObjUpdt( Tv_X, Tv_Y );
			}
		}
		else
		{
			//check change
			if ( Tv_IsOld )
			{
				if ( NULL != m_ObjFocus )
				{
					LcPostFocusGetMsg( m_ObjFocus );
				}
			}
			//mouse object free
			LcMseObjFree();
		}
		//update hardware mouse curso
		if ( Tv_IsOld != m_IsMseCusOn )
		{
			HalMouseCusOnOff();
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiConsole::MouseCusPosSet( __s32 A_X, __s32 A_Y )
{
	//hardware set , not implemented
}
//--------------------------------------------------------------------
void	Cls_GrUiConsole::MouseCusImgSet( __u32 A_ImgIdx )
{
	m_MseCusImg		=	A_ImgIdx;
}
//--------------------------------------------------------------------
Cls_GrUiDoBase*	Cls_GrUiConsole::ObjFindByPos( __s32 A_X, __s32 A_Y, BOOL8 A_IsInpAbleOnly )
{
	// local -------------------
		Cls_GrUiDoBase*	Tv_Result;
		__s32		Tv_LayIdx;
		__s32		Tv_WinIdx;
		BOOL8	Tv_IsLayLock;
		Cls_GrUiDoWin*	Tv_Win;
	// code --------------------
		//init
		Tv_Result			=	NULL;
		Tv_IsLayLock	=	FALSE;

		//find layer
		for ( Tv_LayIdx=E_GrUiConLayerCnt-1;Tv_LayIdx >= 0;Tv_LayIdx-- )
		{
			//find window
			for ( Tv_WinIdx=0;Tv_WinIdx < E_GrUiConWinMaxCntOfLayer;Tv_WinIdx++ )
			{
				//check exist win
				if ( NULL != m_LayerTbl[Tv_LayIdx][Tv_WinIdx] )
				{
					//init
					Tv_Win	=	m_LayerTbl[Tv_LayIdx][Tv_WinIdx];
					//find position item
					Tv_Result	=	Tv_Win->FindObjByPos( A_X, A_Y, A_IsInpAbleOnly );
					if ( NULL != Tv_Result )
					{
						break;
					}
					//update focus lock
					if ( Tv_Win->IsLayerFcsLock() )
					{
						Tv_IsLayLock	=	TRUE;
						if ( NULL == Tv_Result )
						{
							Tv_Result	=	(Cls_GrUiDoWin*)Tv_Win;
							break;
						}
					}
				}
			}
			if ( Tv_IsLayLock || (NULL != Tv_Result) )
			{
				break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiConsole::LcMseObjUpdt( __s32 A_X, __s32 A_Y )
{
	// local -------------------
		Cls_GrUiDoBase*	Tv_Obj;
		St_GrUiMsgHd		Tv_Msg;
		Cls_GrUiDoWin*	Tv_Win;
	// code --------------------
		//get object
		Tv_Obj	=	ObjFindByPos( A_X, A_Y );
		if ( m_ObjOnMse != Tv_Obj )
		{
			LcMseObjFree();
			m_ObjOnMse		=	Tv_Obj;
			if ( NULL != m_ObjOnMse )
			{
				//mouse enter event
				Tv_Win				=	(Cls_GrUiDoWin*)m_ObjOnMse->WindowGet();
				Tv_Msg.Cmd		=	E_GrUiMsgCmdMouseEnter;
				Tv_Msg.ConIdx	=	E_GrUiConIdxFocused;
				Tv_Msg.WinId	=	Tv_Win->WinIdGet();
				Tv_Msg.ObjId	=	E_GrUiObjIdNone;
				if ( !m_ObjOnMse->IsWindow() )
				{
					Tv_Msg.ObjId	=	m_ObjOnMse->ObjIdGet();
				}
				UiMsgPost( &Tv_Msg, sizeof(Tv_Msg) );
			}
			//update cursor
			//LcMseCusImgUpdt();
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiConsole::LcMseObjFree( void )
{
	// local -------------------
		St_GrUiMsgHd		Tv_Msg;
		Cls_GrUiDoWin*	Tv_Win;
	// code --------------------
		if ( NULL != m_ObjOnMse )
		{
			Tv_Win		=	(Cls_GrUiDoWin*)m_ObjOnMse->WindowGet();
			//leave focus message
			Tv_Msg.Cmd		=	E_GrUiMsgCmdMouseLeave;
			Tv_Msg.ConIdx	=	E_GrUiConIdxFocused;
			Tv_Msg.WinId	=	Tv_Win->WinIdGet();
			Tv_Msg.ObjId	=	E_GrUiObjIdNone;
			if ( !m_ObjOnMse->IsWindow() )
			{
				Tv_Msg.ObjId	=	m_ObjOnMse->ObjIdGet();
			}
			UiMsgPost( &Tv_Msg, sizeof(Tv_Msg) );

			//release mouse object
			m_ObjOnMse	=	NULL;
		}
}
//--------------------------------------------------------------------
/*
void	Cls_GrUiConsole::LcMseCusImgUpdt( void )
{
	// local -------------------
	__u32		Tv_MseImg;
	// code --------------------
	//image index
	Tv_MseImg		=	E_GrUiMouseCusImgArrow;
	//check exist object
	if ( m_IsMseCusOn && ( NULL != m_ObjOnMse ) )
	{
		Tv_MseImg	=	m_ObjOnMse->MouseCusImgIdxGet();
	}
	//set mouse cursor
	MouseCusImgSet( Tv_MseImg );
}
*/
//--------------------------------------------------------------------
Cls_GrUiDoBase*	Cls_GrUiConsole::MouseOnObjGet( void )
{
		return	m_ObjOnMse;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiConsole::VirtKbdCreate( __u16 A_WinId, __u16 A_ObjId, BOOL8 A_IsNum, BOOL8 A_IsPasswd, __u32 A_VkId, WCHAR* A_Str, __u32 A_MaxLen, BOOL8 A_IsPassChk, BOOL8 A_IsLangLock )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u16		Tv_ScptId;
		Cls_GrUiDoVkbdBase*	Tv_VkWin;
	// code --------------------
		//init
		Tv_Result	=	FALSE;
		Tv_VkWin	=	NULL;

		//check exist discript
		Tv_ScptId		=	E_GrUiObjIdWinVkbdText;
		if ( A_IsNum )
		{
			Tv_ScptId		=	E_GrUiObjIdWinVkbdNum;
		}

		//check exist script
		if ( V_GrUiMngObj->IsScptExist( Tv_ScptId ) )
		{
			//create window
			if ( A_IsNum )
			{
				Tv_VkWin	=	(Cls_GrUiDoVkbdBase*) new Cls_GrUiDoVkbdNum( A_WinId,A_ObjId, A_IsPasswd, A_VkId, A_Str, A_MaxLen, m_ConIdx ); 
			}
			else
			{
				Tv_VkWin	=	(Cls_GrUiDoVkbdBase*) new Cls_GrUiDoVkbdTxt( A_WinId, A_ObjId, A_IsPasswd, A_VkId, A_Str, A_MaxLen, A_IsPassChk, A_IsLangLock, m_ConIdx );
			}
			if ( NULL != Tv_VkWin )
			{
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiConsole::VirtKbdAllClose( void )
{
	// local -------------------
		Cls_GrUiDoWin*	Tv_Win;
		__u32		Tv_Idx;
	// code --------------------
		//text
		Tv_Idx	=	0;
		Tv_Win	=	WindowFindByScptId( E_GrUiObjIdWinVkbdText, Tv_Idx );
		while ( NULL != Tv_Win )
		{
			//request close
			Tv_Win->Close();
			//next
			Tv_Idx ++;
			Tv_Win	=	WindowFindByScptId( E_GrUiObjIdWinVkbdText, Tv_Idx );
		}
		//number
		Tv_Idx	=	0;
		Tv_Win	=	WindowFindByScptId( E_GrUiObjIdWinVkbdNum, Tv_Idx );
		while ( NULL != Tv_Win )
		{
			//request close
			Tv_Win->Close();
			//next
			Tv_Idx ++;
			Tv_Win	=	WindowFindByScptId( E_GrUiObjIdWinVkbdNum, Tv_Idx );
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiConsole::MouseOnObjReset( void )
{
		m_ObjOnMse	=	NULL;
}
//--------------------------------------------------------------------
void	Cls_GrUiConsole::RtlUiMsgProc( Ptr_GrUiMsgHd A_PtrMsg )
{
	// local -------------------
		__u8		Tv_LayIdx;
		__u8		Tv_WinIdx;
	// code --------------------
		//time tick process
		if ( E_GrUiMsgCmdTickPeriod == A_PtrMsg->Cmd )
		{
			LcProcTimePeriod();
		}
		//check console message
		if ( E_GrUiWinIdNone == A_PtrMsg->WinId )
		{
			//console ui message 
			LcProcUiMsg( A_PtrMsg );	
		}
		else
		{
			//check broadcast
			if ( E_GrUiWinIdAll == A_PtrMsg->WinId )
			{
				//boradcast
				for ( Tv_LayIdx = 0;Tv_LayIdx < E_GrUiConLayerCnt;Tv_LayIdx++ )
				{
					for ( Tv_WinIdx=0;Tv_WinIdx < E_GrUiConWinMaxCntOfLayer;Tv_WinIdx++ )
					{
						//check exist object
						if ( NULL != m_LayerTbl[Tv_LayIdx][Tv_WinIdx] )
						{
							//message process
							if ( E_GrUiObjIdNone == A_PtrMsg->ObjId )
							{
								m_LayerTbl[Tv_LayIdx][Tv_WinIdx]->ProcUiMsg( A_PtrMsg );
							}
							else
							{
								m_LayerTbl[Tv_LayIdx][Tv_WinIdx]->RtlUiMsgProc( A_PtrMsg );
							}
						}
					}
				}
			}
			else
			{
				//get id
				Tv_LayIdx	=	GrUiDoWinGetLayerByWinId( A_PtrMsg->WinId, &Tv_WinIdx );
				//check exist
				if ( NULL != m_LayerTbl[Tv_LayIdx][Tv_WinIdx] )
				{
					//message process
					if ( E_GrUiObjIdNone == A_PtrMsg->ObjId )
					{
						m_LayerTbl[Tv_LayIdx][Tv_WinIdx]->ProcUiMsg( A_PtrMsg );
					}
					else
					{
						m_LayerTbl[Tv_LayIdx][Tv_WinIdx]->RtlUiMsgProc( A_PtrMsg );
					}
				}
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiConsole::UiMsgPost( Ptr_GrUiMsgHd A_PtrMsg, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		//init
		Tv_Result	=	FALSE;

		//patch console index
		if ( E_GrUiConIdxFocused == A_PtrMsg->ConIdx )
		{
			A_PtrMsg->ConIdx	=	m_ConIdx;
		}

		return	V_GrUiMngObj->UiMsgPost( A_PtrMsg, A_Size );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiConsole::UiMsgSend( Ptr_GrUiMsgHd A_PtrMsg )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		//init
		Tv_Result	=	FALSE;

		//patch console id
		if ( E_GrUiConIdxFocused == A_PtrMsg->ConIdx )
		{
			A_PtrMsg->ConIdx	=	m_ConIdx;
		}
		//check console id
		if ( A_PtrMsg->ConIdx == m_ConIdx )
		{
			//do runtime
			RtlUiMsgProc( A_PtrMsg );
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiConsole::LcProcUiMsg( Ptr_GrUiMsgHd A_PtrMsg )
{
	// local -------------------
	// code --------------------
	switch( A_PtrMsg->Cmd )
	{
		case	E_GrUiMsgCmdConWinReg:
			LcProcMsgWinReg( (Ptr_GrUiMsgConWinReg)A_PtrMsg );
			break;
		case	E_GrUiMsgCmdConWinClose:
			LcProcMsgWinClose( (Ptr_GrUiMsgConWinClose)A_PtrMsg );
			break;
		case	E_GrUiMsgCmdConFocusCrt:
			LcProcMsgFocusCrt();
			break;
		case	E_GrUiMsgCmdConInpKey:
			LcProcMsgInpKey( (Ptr_GrUiMsgKey)A_PtrMsg );
			break;
		case	E_GrUiMsgCmdConInpMouse:
			LcProcMsgInpMouse( (Ptr_GrUiMsgMouse)A_PtrMsg );
			break;
		case	E_GrUiMsgCmdConInpWheel:
			LcProcMsgInpWheel( (Ptr_GrUiMsgWheel)A_PtrMsg );
			break;
		case	E_GrUiMsgCmdConChgConFcs:
			LcProcMsgConChgFcs( (Ptr_GrUiMsgConFcsChg)A_PtrMsg );
			break;
		case	E_GrUiMsgCmdConPaint:
			//LcProcMsgPaint();
			break;
	}
}
//--------------------------------------------------------------------
void	Cls_GrUiConsole::ProcDoWinFree( __u8 A_Layer, __u8 A_WinIdx )
{
		delete	m_LayerTbl[A_Layer][A_WinIdx];
		m_LayerTbl[A_Layer][A_WinIdx]	=	NULL;
}
//--------------------------------------------------------------------
void	Cls_GrUiConsole::LcProcMsgWinClose( Ptr_GrUiMsgConWinClose A_PtrMsg )
{
	// local -------------------
		__u8		Tv_Layer;
		__u8		Tv_SubIdx;
	// code --------------------
		switch( A_PtrMsg->Mode )
		{
			case	E_GrUiWinCloseWin:
				Tv_Layer	=	GrUiDoWinGetLayerByWinId( A_PtrMsg->Idx, &Tv_SubIdx );
				if ( NULL != m_LayerTbl[Tv_Layer][Tv_SubIdx] )
				{
					//safe check
					if ( (NULL == A_PtrMsg->Para) || ( (Cls_GrUiDoWin*)A_PtrMsg->Para == m_LayerTbl[Tv_Layer][Tv_SubIdx] ) )
					{
						//unregist
						ProcDoWinFree( Tv_Layer, Tv_SubIdx );
					}
				}
				break;
			case	E_GrUiWinCloseLayer:
				for ( Tv_SubIdx=0;Tv_SubIdx < E_GrUiConWinMaxCntOfLayer;Tv_SubIdx++ )
				{
					//check exist window
					if ( NULL != m_LayerTbl[A_PtrMsg->Idx][Tv_SubIdx] )
					{
						//check except window
						if ( (__u16)A_PtrMsg->Para != m_LayerTbl[A_PtrMsg->Idx][Tv_SubIdx]->WinIdGet() )
						{
							//unregist
							ProcDoWinFree( (__u8)A_PtrMsg->Idx, Tv_SubIdx );
						}
					}
				}
				break;
			case	E_GrUiWinCloseAll:
				for ( Tv_Layer=0;Tv_Layer < E_GrUiConLayerCnt;Tv_Layer++ )
				{
					for ( Tv_SubIdx=0;Tv_SubIdx < E_GrUiConWinMaxCntOfLayer;Tv_SubIdx++ )
					{
						//check exist window
						if ( NULL != m_LayerTbl[Tv_Layer][Tv_SubIdx] )
						{
							//check except window
							if ( (__u16)A_PtrMsg->Para != m_LayerTbl[Tv_Layer][Tv_SubIdx]->WinIdGet() )
							{
								//unregist
								ProcDoWinFree( Tv_Layer, Tv_SubIdx );
							}
						}
					}
				}
				break;
		}

		//focus correct
		if ( NULL == m_ObjFocus )
		{
			FocusCorrect();
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiConsole::LcProcMsgWinReg( Ptr_GrUiMsgConWinReg A_PtrMsg )
{
	// local -------------------
		St_GrUiMsgHd	Tv_Msg;
	// code --------------------
		if ( LcWinRegist( (Cls_GrUiDoWin*)A_PtrMsg->ObjWin ) )
		{
			//do create event
			Tv_Msg.Cmd		=	E_GrUiMsgCmdWinCreate;
			Tv_Msg.ConIdx	=	E_GrUiConIdxFocused;
			Tv_Msg.WinId	=	((Cls_GrUiDoWin*)A_PtrMsg->ObjWin)->WinIdGet();
			Tv_Msg.ObjId	=	E_GrUiObjIdNone;
			UiMsgSend( &Tv_Msg );		//do message
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiConsole::LcProcMsgFocusCrt( void )
{
	// local -------------------
		__s32	Tv_LayIdx;
		__s32	Tv_WinIdx;
		Cls_GrUiDoBase*	Tv_ObjFocus;
	// code --------------------
		//check focus is null
		if ( NULL == m_ObjFocus )
		{
			Tv_ObjFocus	=	NULL;
			for ( Tv_LayIdx = E_GrUiConLayerCnt - 1;Tv_LayIdx >= 0;Tv_LayIdx -- )
			{
				for ( Tv_WinIdx=0;Tv_WinIdx < E_GrUiConWinMaxCntOfLayer;Tv_WinIdx++ )
				{
					//check exist win
					if ( NULL != m_LayerTbl[Tv_LayIdx][Tv_WinIdx] )
					{
						//check can focus
						if ( m_LayerTbl[Tv_LayIdx][Tv_WinIdx]->IsCanFocus() )
						{
							//exist last focus object
							Tv_ObjFocus	=	m_LayerTbl[Tv_LayIdx][Tv_WinIdx]->LastFocusObjGet();
							if ( NULL == Tv_ObjFocus )
							{
								//find slave focus object
								Tv_ObjFocus	=	m_LayerTbl[Tv_LayIdx][Tv_WinIdx]->FindFocusableSlave();
								if ( NULL == Tv_ObjFocus )
								{
									//set window focus
									Tv_ObjFocus	=	(Cls_GrUiDoBase*)m_LayerTbl[Tv_LayIdx][Tv_WinIdx];
									break;
								}
							}
						}
					}
				}
				if ( NULL != Tv_ObjFocus )
				{
					break;
				}
			}
			//update
			FocusObjSet( Tv_ObjFocus );
		}
		//reset requested
		m_IsFocusCrtReq		=	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiConsole::WindowRegist( Cls_GrUiDoWin* A_ObjWin )
{
	// local -------------------
		St_GrUiMsgConWinReg	Tv_Msg;
	// code --------------------

		Tv_Msg.Hd.Cmd			=	E_GrUiMsgCmdConWinReg;
		Tv_Msg.Hd.ConIdx	=	E_GrUiConIdxFocused;
		Tv_Msg.Hd.WinId		=	E_GrUiWinIdNone;					//console message mode
		Tv_Msg.Hd.ObjId		=	E_GrUiObjIdNone;					//console message mode
		Tv_Msg.ObjWin			=	A_ObjWin;
		return	UiMsgPost( (Ptr_GrUiMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrUiConsole::LcProcMsgInpKey( Ptr_GrUiMsgKey A_PtrMsg )
{
	// local -------------------
		Cls_GrUiDoWin*	Tv_ObjWin;
	// code --------------------
		//mouse cursor off
		MouseCusOn( FALSE );

		//send key to current object
		if ( NULL != m_ObjFocus )
		{
			//check can focus
			if ( m_ObjFocus->IsCanFocus() )
			{
				//patch message
				Tv_ObjWin	=	(Cls_GrUiDoWin*)m_ObjFocus->WindowGet();
				A_PtrMsg->Hd.Cmd			=	E_GrUiMsgCmdKey;
				A_PtrMsg->Hd.WinId		=	Tv_ObjWin->WinIdGet();
				A_PtrMsg->Hd.ObjId		=	E_GrUiObjIdNone;
				if ( !m_ObjFocus->IsWindow() )
				{
					A_PtrMsg->Hd.ObjId		=	m_ObjFocus->ObjIdGet();
				}
				RtlUiMsgProc( (Ptr_GrUiMsgHd)A_PtrMsg );			//run process
			}
			else
			{
				FocusReset();
				FocusCorrect();
			}
		}
		else
		{
			FocusCorrect();
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiConsole::LcProcMsgInpMouse( Ptr_GrUiMsgMouse A_PtrMsg )
{
	// local -------------------
		Cls_GrUiDoWin*	Tv_ObjWin;
	// code --------------------
		//reset mouse time
		m_MseCusTime	=	0;
		//mouse cursor on
		MouseCusOn( TRUE );
		//update mouse cursor
		MouseCusPosSet( A_PtrMsg->X, A_PtrMsg->Y );
		//update mouse object
		LcMseObjUpdt( A_PtrMsg->X, A_PtrMsg->Y );
		//send mouse to current object
		if ( NULL != m_ObjOnMse )
		{
			Tv_ObjWin	=	(Cls_GrUiDoWin*)m_ObjOnMse->WindowGet();
			A_PtrMsg->Hd.Cmd		=	E_GrUiMsgCmdMouse;
			A_PtrMsg->Hd.WinId	=	Tv_ObjWin->WinIdGet();
			A_PtrMsg->Hd.ObjId	=	E_GrUiObjIdNone;
			if ( !m_ObjOnMse->IsWindow() )
			{
				A_PtrMsg->Hd.ObjId	=	m_ObjOnMse->ObjIdGet();
			}
			RtlUiMsgProc( (Ptr_GrUiMsgHd)A_PtrMsg );
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiConsole::LcProcMsgInpWheel( Ptr_GrUiMsgWheel A_PtrMsg )
{
	// local -------------------
		Cls_GrUiDoWin*	Tv_ObjWin;
	// code --------------------
		//reset mouse time
		m_MseCusTime	=	0;
		//mouse cursor on
		MouseCusOn( TRUE );
		//send mouse to current object
		if ( NULL != m_ObjOnMse )
		{
			Tv_ObjWin	=	(Cls_GrUiDoWin*)m_ObjOnMse->WindowGet();
			A_PtrMsg->Hd.Cmd		=	E_GrUiMsgCmdWeel;
			A_PtrMsg->Hd.WinId	=	Tv_ObjWin->WinIdGet();
			A_PtrMsg->Hd.ObjId	=	E_GrUiObjIdNone;
			if ( !m_ObjOnMse->IsWindow() )
			{
				A_PtrMsg->Hd.ObjId	=	m_ObjOnMse->ObjIdGet();
			}
			RtlUiMsgProc( (Ptr_GrUiMsgHd)A_PtrMsg );
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiConsole::LcProcMsgConChgFcs( Ptr_GrUiMsgConFcsChg A_PtrMsg )
{
	// local -------------------
	// code --------------------
		//check lose focus
		if ( !A_PtrMsg->IsFocus )
		{
			MouseCusOn( FALSE );
		}
		//broadcast all window
		A_PtrMsg->Hd.WinId		=	E_GrUiWinIdAll;
		A_PtrMsg->Hd.ObjId		=	E_GrUiObjIdNone;
		UiMsgPost( (Ptr_GrUiMsgHd)A_PtrMsg, sizeof(St_GrUiMsgConFcsChg) );
}
//--------------------------------------------------------------------
Cls_GrUiDoWin*	Cls_GrUiConsole::WindowFindByWinId( __u16 A_WinId )
{
	// local -------------------
		Cls_GrUiDoWin*	Tv_Result;
		__u8		Tv_LayIdx;
		__u8		Tv_SubIdx;
	// code --------------------
		Tv_Result		=	NULL;
		Tv_LayIdx		=	GrUiDoWinGetLayerByWinId( A_WinId, &Tv_SubIdx );
		//check match range
		if ( (E_GrUiConLayerCnt > Tv_LayIdx) && (E_GrUiConWinMaxCntOfLayer > Tv_SubIdx) )
		{
			Tv_Result		=	m_LayerTbl[Tv_LayIdx][Tv_SubIdx];
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiConsole::IsConFocused( void )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result	=	FALSE;

		if ( this == V_GrUiMngObj->GetConsoleObj() )
		{
			Tv_Result		=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32		Cls_GrUiConsole::ClrBlackGet( void )
{
	return	m_ClrBlack;
}
//--------------------------------------------------------------------
__u32		Cls_GrUiConsole::ClrWhiteGet( void )
{
	return	m_ClrWhite;
}
//--------------------------------------------------------------------
__u16	Cls_GrUiConsole::ObjIdAlloc( void )
{
	// local -------------------
	__u16		Tv_Result;
	__u16		Tv_ByteIdx;
	__u16		Tv_BitIdx;
	__u8		Tv_BitMask;
	// code --------------------
	//init
	Tv_Result		=	E_GrUiObjIdNone;

	for ( Tv_ByteIdx=0;Tv_ByteIdx < E_GrUiConIdAllocTblSize;Tv_ByteIdx++ )
	{
		if ( 0xFF != m_IdAlocTbl[Tv_ByteIdx] )
		{
			Tv_BitMask	=	1;
			for ( Tv_BitIdx=0;Tv_BitIdx < 8;Tv_BitIdx++ )
			{
				if ( 0 == (Tv_BitMask & m_IdAlocTbl[Tv_ByteIdx]) )
				{
					//finded
					m_IdAlocTbl[Tv_ByteIdx]	=	m_IdAlocTbl[Tv_ByteIdx] | Tv_BitMask;		//allocate id
					//update result
					Tv_Result	=	E_GrUiObjIdConsole + (Tv_ByteIdx << 3) + Tv_BitIdx;
					break;
				}
				//next
				Tv_BitMask	=	Tv_BitMask << 1;
			}
		}
		//check finished
		if ( E_GrUiObjIdNone != Tv_Result )
		{
			break;
		}
	}

	return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiConsole::ObjIdFree( __u16 A_Id )
{
	// local -------------------
	__u16		Tv_ByteIdx;
	__u16		Tv_BitIdx;
	__u8		Tv_BitMask;
	// code --------------------
	//check range
	if ( (E_GrUiObjIdConsole <= A_Id) && ((E_GrUiObjIdConsole + E_GrUiConMaxIdAllocCnt) > A_Id ) )
	{
		//free
		Tv_ByteIdx	=	(A_Id - E_GrUiObjIdConsole) >> 3;
		Tv_BitIdx		=	(A_Id - E_GrUiObjIdConsole) & 0x07;
		Tv_BitMask	=	1 << Tv_BitIdx;
		m_IdAlocTbl[Tv_ByteIdx]	=	m_IdAlocTbl[Tv_ByteIdx] & ~Tv_BitMask;
	}
}
//--------------------------------------------------------------------
void	Cls_GrUiConsole::LcProcTimePeriod( void )
{
	// local -------------------
	// code --------------------
		//check async paint
		LcProcImpPaint();
		//update mouse auto cursor off
		LcMseCusAutoHide();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiConsole::HalPntReqTransfer( Ptr_GrRect A_PtrRect )
{
	return	TRUE;
}
//--------------------------------------------------------------------
__u32	Cls_GrUiConsole::UiMessageBox(	__u16 A_ScptId, WCHAR* A_StrTitle, WCHAR* A_StrCont, 
																			__u8 A_Layer )
{
	// local -------------------
		__u32	Tv_Result;
		Cls_GrUiDoMsgbox*	Tv_MsgBox;
	// code --------------------
		//init
		Tv_Result	=	E_GrUiDoWinModalStanby;		//not work
		//create message box
		Tv_MsgBox	=	(Cls_GrUiDoMsgbox*)new Cls_GrUiDoMsgbox(	A_ScptId, A_StrTitle, A_StrCont, 
																													E_GrUiDoWinDfltLayer, m_ConIdx );
		Tv_MsgBox->RtlModalResultSet( &Tv_Result );
		//do message pump
		while ( E_GrUiDoWinModalStanby == Tv_Result )
		{
			if ( !V_GrUiMngObj->UiMsgPump() )
			{
				V_GrUiMngObj->UiMsgRest();
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiConsole::MsgBoxAllClose( void )
{
	// local -------------------
		Cls_GrUiDoWin*	Tv_Win;
		__u32	Tv_Idx;
	// code --------------------
		//message box yes
		Tv_Idx	=	0;
		Tv_Win	=	WindowFindByScptId( E_GrUiObjIdWinMsgBoxType0, Tv_Idx );
		while ( NULL != Tv_Win )
		{
			//request close
			Tv_Win->Close();
			//next
			Tv_Idx ++;
			Tv_Win	=	WindowFindByScptId( E_GrUiObjIdWinMsgBoxType0, Tv_Idx );
		}
		//message box yes no
		Tv_Idx	=	0;
		Tv_Win	=	WindowFindByScptId( E_GrUiObjIdWinMsgBoxType1, Tv_Idx );
		while ( NULL != Tv_Win )
		{
			//request close
			Tv_Win->Close();
			//next
			Tv_Idx ++;
			Tv_Win	=	WindowFindByScptId( E_GrUiObjIdWinMsgBoxType1, Tv_Idx );
		}
		//message box yes no cancel
		Tv_Idx ++;
		Tv_Win	=	WindowFindByScptId( E_GrUiObjIdWinMsgBoxType2, Tv_Idx );
		if ( NULL != Tv_Win )
		{
			//request close
			Tv_Win->Close();
			//next
			Tv_Idx ++;
			Tv_Win	=	WindowFindByScptId( E_GrUiObjIdWinMsgBoxType2, Tv_Idx );
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiConsole::LcWinRegist( Cls_GrUiDoWin* A_Win )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8		Tv_WkIdx;
		__u8		Tv_Layer;
		__u16		Tv_WinId;
	// code --------------------
		Tv_Result	=	FALSE;

		//init
		Tv_Layer	=	A_Win->LayerGet();
		//find empty slot of layer
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiConWinMaxCntOfLayer;Tv_WkIdx++ )
		{
			//check empty
			if ( NULL == m_LayerTbl[Tv_Layer][Tv_WkIdx] )
			{
				//finded
				m_LayerTbl[Tv_Layer][Tv_WkIdx]	=	A_Win;
				//set window id
				Tv_WinId	=	GrUiDoWinMakeWinId( Tv_Layer, Tv_WkIdx );
				m_LayerTbl[Tv_Layer][Tv_WkIdx]->RtlWinIdSet( Tv_WinId );
				//success
				Tv_Result	=	TRUE;
				break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiConsole::RtlWinRegist( Cls_GrUiDoWin* A_Win )
{
	// local -------------------
	// code --------------------
		//lock - implement later
		return	LcWinRegist( A_Win );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiConsole::QuadModeSet( __u8 A_Mode, __u8 A_Page, BOOL8 A_IsNotify )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		//check mode
		if ( E_GrDvrQuadModeCnt > A_Mode )
		{
			//check able quad
			if ( IsQuadAble( A_Mode ) )
			{
				//check page
				if ( A_Page < QuadPageCntByMode( A_Mode ) )
				{
					//update parameter
					m_QuadMode		=	A_Mode;
					m_QuadPage		=	A_Page;
					m_QuadViewCnt	=	GrDvrQuadViewCount( A_Mode );
					Tv_Result			=	TRUE;

					//notify quad change
					if ( A_IsNotify )
					{
						SysNotifyQuadChg();
					}
				}
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiConsole::SysNotifyQuadChg( void )
{
	// local -------------------
		St_GrTaskMsgQuadChgReq	Tv_SysMsg;
	// code --------------------
		//notify system message
		Tv_SysMsg.MsgHd.Cmd			=	E_GrTaskMsgQuadChgReq;
		Tv_SysMsg.MsgHd.PtrRtl	=	NULL;
		Tv_SysMsg.ConIdx				=	m_ConIdx;
		GrTaskSvrMsgBroadCast( (Ptr_GrTaskMsgHd)&Tv_SysMsg, sizeof(Tv_SysMsg) );
}
//--------------------------------------------------------------------
__u8	Cls_GrUiConsole::QuadMaxModeGet( void )
{
	// local -------------------
		__u8	Tv_Result;
		__u8	Tv_WkIdx;
	// code --------------------
		Tv_Result	=	E_GrDvrQuadMode1;
		for ( Tv_WkIdx=E_GrDvrQuadModeCnt;0 != Tv_WkIdx;Tv_WkIdx-- )
		{
			if ( 0 != QuadPageCntByMode( Tv_WkIdx-1 ) )
			{
				Tv_Result	=	Tv_WkIdx - 1;
				break;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiConsole::QuadMaxModeSet( void )
{
		return	QuadModeSet( QuadMaxModeGet(), 0 );
}
//--------------------------------------------------------------------
__u8	Cls_GrUiConsole::QuadViewIdxByPos( __s32 A_X, __s32 A_Y )
{
#ifdef GR_OSD_EDIT
	// local -------------------
		__u8	Tv_Result;
		__u8	Tv_ViewIdx;
		St_GrRect	Tv_Rc;
	// code --------------------
		Tv_Result	=	0;
		//get view index
		if ( 0 != m_QuadViewCnt )
		{
			for ( Tv_ViewIdx=0;Tv_ViewIdx < m_QuadViewCnt;Tv_ViewIdx++ )
			{
				//check rect
				QuadRectGet( Tv_ViewIdx, &Tv_Rc );
				if ( ( A_X >= Tv_Rc.left ) && ( A_X < Tv_Rc.right ) && ( A_Y >= Tv_Rc.top ) &&  ( A_Y < Tv_Rc.bottom ) )
				{
					//finded
					Tv_Result	=	Tv_ViewIdx;
					break;
				}
			}
		}

		return	Tv_Result;
#else
	return	0;
#endif	//GR_OSD_EDIT
}
//--------------------------------------------------------------------
__u8	Cls_GrUiConsole::QuadChByPos( __s32 A_X, __s32 A_Y )
{
	// local -------------------
		__u8	Tv_ViewIdx;
	// code --------------------
		Tv_ViewIdx	=	QuadViewIdxByPos(A_X, A_Y);
		return	QuadGetChByView( Tv_ViewIdx );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiConsole::QuadRectGet( __u8 A_ViewIdx, Ptr_GrRect A_PtrRtRect )
{
#ifdef GR_OSD_EDIT
	// local -------------------
		__u16		Tv_ResX;
		__u16		Tv_ResY;
		Ptr_GrByteRect	Tv_PtrSclTbl;
	// code --------------------
		LcQdGetRes( &Tv_ResX, &Tv_ResY );
		Tv_PtrSclTbl	=	V_GrUiConQdRtTbl[m_QuadMode];
		A_PtrRtRect->left		=	(__s32)LcQdCalcScl( Tv_PtrSclTbl[A_ViewIdx].left, Tv_ResX );
		A_PtrRtRect->top		=	(__s32)LcQdCalcScl( Tv_PtrSclTbl[A_ViewIdx].top, Tv_ResY );
		A_PtrRtRect->right	=	(__s32)LcQdCalcScl( Tv_PtrSclTbl[A_ViewIdx].right, Tv_ResX );
		A_PtrRtRect->bottom	=	(__s32)LcQdCalcScl( Tv_PtrSclTbl[A_ViewIdx].bottom, Tv_ResY );

		return	TRUE;
#else
		return	FALSE;
#endif
}
//--------------------------------------------------------------------
void	Cls_GrUiConsole::CorvertMapSet( __u16 A_Map )
{
		m_CorvertMap	=	A_Map;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiConsole::IsPipShow( void )
{
		return	m_IsPipShow;
}
//--------------------------------------------------------------------
void	Cls_GrUiConsole::PipShow( __u8 A_Ch, __u8 A_Scale, __s32 A_X, __s32 A_Y )
{
}
//--------------------------------------------------------------------
void	Cls_GrUiConsole::PipHide( void )
{
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiConsole::IsPlayMode( void )
{
		return	m_IsPlayMode;
}
//--------------------------------------------------------------------
void	Cls_GrUiConsole::PlayModeSet( BOOL8 A_IsPlay )
{
		m_IsPlayMode	=	A_IsPlay;
}
//--------------------------------------------------------------------
void	Cls_GrUiConsole::QuadPageNext( void )
{
	// local -------------------
		__u8		Tv_PageCnt;
		__u8		Tv_ReqPage;
	// code --------------------
		Tv_PageCnt	=	QuadPageCntByMode( m_QuadMode );
		if ( 0 != Tv_PageCnt )
		{
			Tv_ReqPage	=	m_QuadPage + 1;
			if ( Tv_ReqPage >= Tv_PageCnt )
			{
				Tv_ReqPage	=	0;
			}
			QuadModeSet( m_QuadMode, Tv_ReqPage );
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiConsole::QuadPagePrev( void )
{
	// local -------------------
		__u8		Tv_PageCnt;
		__u8		Tv_ReqPage;
	// code --------------------
		Tv_PageCnt	=	QuadPageCntByMode( m_QuadMode );
		if ( 0 != Tv_PageCnt )
		{
			Tv_ReqPage	=	m_QuadPage;
			if ( 0 == Tv_ReqPage )
			{
				Tv_ReqPage	=	Tv_PageCnt - 1;
			}
			else
			{
				Tv_ReqPage	--;
			}
			QuadModeSet( m_QuadMode, Tv_ReqPage );
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiConsole::QuadModeNext( void )
{
	// local -------------------
		__u8	Tv_WkIdx;
		__u8	Tv_Mode;
	// code --------------------
		Tv_Mode	=	m_QuadMode;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrQuadModeCnt;Tv_WkIdx++ )
		{
			Tv_Mode ++;
			if ( Tv_Mode >= E_GrDvrQuadModeCnt )
			{
				Tv_Mode	=	0;
			}
			if ( IsQuadAble( Tv_Mode ) )
			{
				//finded
				QuadModeSet( Tv_Mode, 0 );
				break;
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiConsole::QuadModePrev( void )
{
	// local -------------------
		__u8	Tv_WkIdx;
		__u8	Tv_Mode;
	// code --------------------
		Tv_Mode	=	m_QuadMode;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrQuadModeCnt;Tv_WkIdx++ )
		{
			if ( 0 == Tv_Mode )
			{
				Tv_Mode	=	E_GrDvrQuadModeCnt - 1;
			}
			else
			{
				Tv_Mode --;
			}
			if ( IsQuadAble( Tv_Mode ) )
			{
				//finded
				QuadModeSet( Tv_Mode, 0 );
				break;
			}
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrUiConsole::QuadModeGet( void )
{
		return	m_QuadMode;
}
//--------------------------------------------------------------------
__u8	Cls_GrUiConsole::QuadPageGet( void )
{
		return	m_QuadPage;
}
//--------------------------------------------------------------------
__u8	Cls_GrUiConsole::QuadViewCntGet( void )
{
		return	m_QuadViewCnt;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiConsole::IsQuadAble( BOOL8 A_Mode )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u8		Tv_Mode;
	// code --------------------
		Tv_Result	=	FALSE;
		Tv_Mode		=	A_Mode;
		if ( E_GrDvrQuadModeNow == Tv_Mode )
		{
			Tv_Mode	=	m_QuadMode;
		}
		if ( 0 != QuadPageCntByMode( A_Mode ) )
		{
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrUiConsole::CorvertMapGet( void )
{
		return	m_CorvertMap;
}
//--------------------------------------------------------------------
__u8	Cls_GrUiConsole::QuadLineTblGet( Ptr_GrLinePos A_PtrRtBuf )
{
#ifdef GR_OSD_EDIT
	// local -------------------
		__u8		Tv_Result;
		__u16		Tv_ResX;
		__u16		Tv_ResY;
		Ptr_GrUiConQdLnRt	Tv_PtrLnRt;
		__u8		Tv_WkIdx;
		Ptr_GrLinePos	Tv_PtrRt;
	// code --------------------
		Tv_Result	=	V_GrUiConQdLineCntTbl[m_QuadMode];
		if ( 0 != Tv_Result )
		{
			Tv_PtrRt	=	A_PtrRtBuf;
			LcQdGetRes( &Tv_ResX, &Tv_ResY );
			Tv_PtrLnRt	=	V_GrUiConQdLineTbl[m_QuadMode];
			for ( Tv_WkIdx=0;Tv_WkIdx < Tv_Result;Tv_WkIdx++ )
			{
				Tv_PtrRt->St.x	=	(__s32)LcQdCalcScl( Tv_PtrLnRt->St.x, Tv_ResX );
				Tv_PtrRt->St.y	=	(__s32)LcQdCalcScl( Tv_PtrLnRt->St.y, Tv_ResY );
				Tv_PtrRt->Ed.x	=	(__s32)LcQdCalcScl( Tv_PtrLnRt->Ed.x, Tv_ResX );
				Tv_PtrRt->Ed.y	=	(__s32)LcQdCalcScl( Tv_PtrLnRt->Ed.y, Tv_ResY );
				//next
				Tv_PtrRt ++;
				Tv_PtrLnRt ++;
			}
		}
		return	Tv_Result;		
#else
	return	0;
#endif
}
//--------------------------------------------------------------------
__u16	Cls_GrUiConsole::NoSigMapGet( void )
{
		return	0;
}
//--------------------------------------------------------------------
void	Cls_GrUiConsole::NoSigMapSet( __u16 A_Map )
{
		//not implemented
}
//--------------------------------------------------------------------
void	Cls_GrUiConsole::HalMouseCusOnOff( void )
{
}
//--------------------------------------------------------------------
__u8	Cls_GrUiConsole::RecAtbGet( __u8 A_Ch )
{
#ifdef GR_OSD_EDIT
	return	0xFF;
#else
	return	0;
#endif	//GR_OSD_EDIT
}
//--------------------------------------------------------------------
__u8	Cls_GrUiConsole::PlayAtbGet( __u8 A_Ch )
{
#ifdef GR_OSD_EDIT
		return	0xFF;
#else
		return	0;
#endif	//GR_OSD_EDIT
}
//--------------------------------------------------------------------
__u8	Cls_GrUiConsole::QuadGetChByView( __u8 A_View )
{
#ifdef GR_OSD_EDIT
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	0;

		if ( A_View < m_QuadViewCnt )
		{
			Tv_Result	=	(m_QuadPage * m_QuadViewCnt) + A_View;
			Tv_Result	=	0x0F & Tv_Result;
		}

		return	Tv_Result;
#else
	return	0;
#endif
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiConsole::NotifyGbStatChg( __u8 A_Type )
{
	// local -------------------
		St_GrUiMsgStatChg	Tv_UiMsg;
	// code --------------------
		Tv_UiMsg.Hd.Cmd			=	E_GrUiMsgCmdStatChg;
		Tv_UiMsg.Hd.ConIdx	=	E_GrUiConIdxFocused;
		Tv_UiMsg.Hd.WinId		=	E_GrUiWinIdAll;
		Tv_UiMsg.Hd.ObjId		=	E_GrUiObjIdBroadcast;
		Tv_UiMsg.Type				=	A_Type;
		return	UiMsgPost( (Ptr_GrUiMsgHd)&Tv_UiMsg, sizeof(Tv_UiMsg) );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiConsole::IsQuadHwGrid( void )
{
		return	FALSE;
}
//--------------------------------------------------------------------
void	Cls_GrUiConsole::QuadGridShow( BOOL8 A_IsShow )
{
}
//--------------------------------------------------------------------
#ifdef GR_OSD_EDIT
__u16	Cls_GrUiConsole::LcQdCalcScl( __u8 A_Scale, __u16 A_Val )
{
	// local -------------------
		__u16		Tv_Result;
	// code --------------------
		Tv_Result	=	0;

		switch( A_Scale )
		{
			case	E_GrDvrQuadScl1_1:
				Tv_Result	=	A_Val;
				break;
			case	E_GrDvrQuadScl1_2:
				Tv_Result	=	A_Val >> 1;
				break;
			case	E_GrDvrQuadScl1_3:
				Tv_Result	=	A_Val / 3;
				break;
			case	E_GrDvrQuadScl1_4:
				Tv_Result	=	A_Val >> 2;
				break;
			case	E_GrDvrQuadScl2_3:
				Tv_Result	=	(A_Val << 1)/3;
				break;
			case	E_GrDvrQuadScl3_4:
				Tv_Result	=	(A_Val * 3)>>2;
				break;
		}
		
		return	Tv_Result;
}
#endif	//GR_OSD_EDIT
//--------------------------------------------------------------------
#ifdef GR_OSD_EDIT
void		Cls_GrUiConsole::LcQdGetRes( __u16* A_PtrRtX, __u16* A_PtrRtY )
{
		*A_PtrRtX		=	V_GrUiDvlQuadSizeX;
		*A_PtrRtY		=	V_GrUiDvlQuadSizeY;
		if ( V_GrUiMngObj->IsOsdExp() )
		{
			*A_PtrRtY		=	V_GrUiDvlQuadSizeY + V_GrUiDvlQuadPalExt;
		}
}
#endif	//GR_OSD_EDIT
//--------------------------------------------------------------------
void	Cls_GrUiConsole::LcPostFocusGetMsg( Cls_GrUiDoBase* A_ObjUiDo )
{
	// local -------------------
		St_GrUiMsgHd		Tv_Msg;
		Cls_GrUiDoWin*	Tv_ObjWin;
	// code --------------------
		Tv_ObjWin			=	(Cls_GrUiDoWin*)A_ObjUiDo->WindowGet();
		Tv_Msg.Cmd		=	E_GrUiMsgCmdFocusGet;
		Tv_Msg.ConIdx	=	E_GrUiConIdxFocused;
		Tv_Msg.WinId	=	Tv_ObjWin->WinIdGet();
		Tv_Msg.ObjId	=	E_GrUiObjIdNone;
		if ( !m_ObjFocus->IsWindow() )
		{
			Tv_Msg.ObjId	=	m_ObjFocus->ObjIdGet();
		}
		UiMsgPost( (Ptr_GrUiMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrUiConsole::LcPostFocusLossMsg( Cls_GrUiDoBase* A_ObjUiDo )
{
	// local -------------------
		St_GrUiMsgHd		Tv_Msg;
		Cls_GrUiDoWin*	Tv_ObjWin;
	// code --------------------
		//focus out event
		Tv_ObjWin			=	(Cls_GrUiDoWin*)A_ObjUiDo->WindowGet();
		Tv_Msg.Cmd		=	E_GrUiMsgCmdFocusLoss;
		Tv_Msg.ConIdx	=	E_GrUiConIdxFocused;
		Tv_Msg.WinId	=	Tv_ObjWin->WinIdGet();
		Tv_Msg.ObjId	=	E_GrUiObjIdNone;
		if ( !m_ObjFocus->IsWindow() )
		{
			Tv_Msg.ObjId	=	m_ObjFocus->ObjIdGet();
		}
		UiMsgPost( (Ptr_GrUiMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
Cls_GrUiDoBase*	Cls_GrUiConsole::FocusKeyObjGet( void )
{
		return	m_ObjFocus;
}
//--------------------------------------------------------------------
__u8	Cls_GrUiConsole::QuadPageCntByMode( __u8 A_Mode )
{
#ifdef GR_OSD_EDIT
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	0;

		if ( E_GrDvrQuadModeCnt > A_Mode )
		{
			Tv_Result	=	V_GrUiConQuadPgCntTbl[A_Mode];
		}

		return	Tv_Result;
#else
		return	0;
#endif
}
//--------------------------------------------------------------------
void	Cls_GrUiConsole::LcMseCusAutoHide( void )
{
		if ( m_IsMseCusOn )
		{
			if ( 0 == V_GrUiMngObj->MouseBtnMapGet() )
			{
				m_MseCusTime ++;
				if ( E_GrUiConsoleCursorTimeOut <= m_MseCusTime )
				{
					MouseCusOn( FALSE );
					m_MseCusTime	=	0;
				}
			}
			else
			{
				m_MseCusTime	=	0;
			}
		}
		else
		{
			m_MseCusTime	=	0;
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiConsole::IsAllCovertMode( void )
{
		return	m_IsAllCvt;
}
//--------------------------------------------------------------------
void	Cls_GrUiConsole::AllCovertModeSet( BOOL8 A_IsSet )
{
		m_IsAllCvt	=	A_IsSet;
}
//--------------------------------------------------------------------
__u16	Cls_GrUiConsole::CovertUserMapGet( void )
{
		return	m_CvtUsrMap;
}
//--------------------------------------------------------------------
void	Cls_GrUiConsole::CovertUserMapSet( __u16 A_Map )
{
		m_CvtUsrMap	=	A_Map;
}
//--------------------------------------------------------------------
__u16	Cls_GrUiConsole::ChUseMapGet( void )
{
		return	0xFFFF;
}
//--------------------------------------------------------------------
void	Cls_GrUiConsole::ChUseMapSet( __u16 A_Map )
{

}
//--------------------------------------------------------------------

