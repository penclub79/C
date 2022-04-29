/*
	solo device control

*/

//====================================================================
//include
#include	<Hw/Solo/GrUiConSolo.h>
#include	<GrGdibObj.h>
#include	<Ui/GrUiMng.h>

//====================================================================
//local const

//====================================================================
//local type

typedef	struct St_GrUiConSoloLnPos 
{
	St_GrWordPos	St;
	St_GrWordPos	Ed;
}	*Ptr_GrUiConSoloLnPos;

//====================================================================
//global var
extern	Cls_GrUiMng*		V_GrUiMngObj;

St_GrWordRect	V_UiConSoloQdrcMd1[E_GrDvrVdoSigTypeCnt][E_GrDvrQuadVwCntMd1]	=	
{
	//ntsc
	{
		{	0, 0, 704, 480 }
	},
	//pal
	{
		{	0, 0, 704, 576 }
	}
};

St_GrWordRect	V_UiConSoloQdrcMd4[E_GrDvrVdoSigTypeCnt][E_GrDvrQuadVwCntMd4]	=	
{
	//ntsc
	{
		{	0, 0, 352, 240 } , {	352, 0, 704, 240 },	{	0, 240, 352, 480 } , {	352, 240, 704, 480 }
	},
		//pal
	{
		{	0, 0, 352, 288 } , {	352, 0, 704, 288 },	{	0, 288, 352, 576 } , {	352, 288, 704, 576 }
	}
};

St_GrWordRect	V_UiConSoloQdrcMd6[E_GrDvrVdoSigTypeCnt][E_GrDvrQuadVwCntMd6]	=	
{
	//ntsc
	/*
	{
		{	0, 0, 464, 320 } ,	{	464, 0, 696, 160 },		{	464, 160, 696, 320 }, 
		{	0, 320, 232, 480 },	{	232, 320, 464, 480 },	{	464, 320, 696, 480 }
	},
	*/
	{
		{	0, 0, 472, 320 } ,	{	472, 0, 704, 160 },		{	472, 160, 704, 320 }, 
		{	0, 320, 236, 480 },	{	236, 320, 472, 480 },	{	472, 320, 704, 480 }
	},
	//pal
	{
		{	0, 0, 472, 384 } ,	{	472, 0, 704, 192 },		{	472, 192, 704, 384 }, 
		{	0, 384, 236, 576 },	{	236, 384, 472, 576 },	{	472, 384, 704, 576 }
	}
};

St_GrWordRect	V_UiConSoloQdrcMd8[E_GrDvrVdoSigTypeCnt][E_GrDvrQuadVwCntMd8]	=	
{
	//ntsc
	{
		{	0, 0, 528, 360 } ,	{	528, 0, 704, 120 },		{	528, 120, 704, 240 }, {	528, 240, 704, 360 },
		{	0, 360, 176, 480 },	{	176, 360, 352, 480 },	{	352, 360, 528, 480 },	{	528, 360, 704, 480 },		
	},
	//pal
	{
		{	0, 0, 528, 432 } ,	{	528, 0, 704, 144 },		{	528, 144, 704, 288 }, {	528, 288, 704, 432 },
		{	0, 432, 176, 576 },	{	176, 432, 352, 576 },	{	352, 432, 528, 576 },	{	528, 432, 704, 576 },		
	}
};

St_GrWordRect	V_UiConSoloQdrcMd9[E_GrDvrVdoSigTypeCnt][E_GrDvrQuadVwCntMd9]	=	
{
	//ntsc
	{
		{	0, 0, 236, 160 } ,	{	236, 0, 472, 160 },		{	472, 0, 704, 160 }, 
		{	0, 160, 236, 320 },	{	236, 160, 472, 320 },	{	472, 160, 704, 320 }, 
		{	0, 320, 236, 480 },	{	236, 320, 472, 480 },	{	472, 320, 704, 480 }, 
	},
		//pal
	{
		{	0, 0, 236, 192 } ,	{	236, 0, 472, 192 },		{	472, 0, 704, 192 }, 
		{	0, 192, 236, 384 },	{	236, 192, 472, 384 },	{	472, 192, 704, 384 }, 
		{	0, 384, 236, 576 },	{	236, 384, 472, 576 },	{	472, 384, 704, 576 }, 
	}
};

St_GrWordRect	V_UiConSoloQdrcMd10[E_GrDvrVdoSigTypeCnt][E_GrDvrQuadVwCntMd10]	=	
{
	//ntsc
	{
		{	0, 0, 352, 240 } ,	{	352, 0, 704, 240 },		
		{	0, 240, 176, 360 },	{	176, 240, 352, 360 },	{	352, 240, 528, 360 },	{	528, 240, 704, 360 },	
		{	0, 360, 176, 480 },	{	176, 360, 352, 480 },	{	352, 360, 528, 480 },	{	528, 360, 704, 480 },	
	},
	//pal
	{
		{	0, 0, 352, 288 } ,	{	352, 0, 704, 288 },		
		{	0, 288, 176, 432 },	{	176, 288, 352, 432 },	{	352, 288, 528, 432 },	{	528, 288, 704, 432 },	
		{	0, 432, 176, 576 },	{	176, 432, 352, 576 },	{	352, 432, 528, 576 },	{	528, 432, 704, 576 },	
	}
};

St_GrWordRect	V_UiConSoloQdrcMd13[E_GrDvrVdoSigTypeCnt][E_GrDvrQuadVwCntMd13]	=	
{
	//ntsc
	{
		{	176, 120, 528, 360 } ,	
		{	0, 0, 176, 120 },		{	176, 0, 352, 120 },		{	352, 0, 528, 120 },		{	528, 0, 704, 120 },	
		{	0, 120, 176, 240 },	{	528, 120, 704, 240 },	
		{	0, 240, 176, 360 },	{	528, 240, 704, 360 },	
		{	0, 360, 176, 480 },	{	176, 360, 352, 480 },	{	352, 360, 528, 480 },	{	528, 360, 704, 480 },	
	},
	//pal
	{
		{	176, 144, 528, 432 } ,	
		{	0, 0, 176, 144 },		{	176, 0, 352, 144 },		{	352, 0, 528, 144 },		{	528, 0, 704, 144 },	
		{	0, 144, 176, 288 },	{	528, 144, 704, 288 },	
		{	0, 288, 176, 432 },	{	528, 288, 704, 432 },	
		{	0, 432, 176, 576 },	{	176, 432, 352, 576 },	{	352, 432, 528, 576 },	{	528, 432, 704, 576 },	
	}
};

St_GrWordRect	V_UiConSoloQdrcMd16[E_GrDvrVdoSigTypeCnt][E_GrDvrQuadVwCntMd16]	=	
{
	//ntsc
	{
		{	0, 0, 176, 120 },		{	176, 0, 352, 120 },		{	352, 0, 528, 120 },		{	528, 0, 704, 120 },	
		{	0, 120, 176, 240 },	{	176, 120, 352, 240 },	{	352, 120, 528, 240 },	{	528, 120, 704, 240 },	
		{	0, 240, 176, 360 },	{	176, 240, 352, 360 },	{	352, 240, 528, 360 },	{	528, 240, 704, 360 },	
		{	0, 360, 176, 480 },	{	176, 360, 352, 480 },	{	352, 360, 528, 480 },	{	528, 360, 704, 480 },	
	},
	//pal
	{
		{	0, 0, 176, 144 },		{	176, 0, 352, 144 },		{	352, 0, 528, 144 },		{	528, 0, 704, 144 },	
		{	0, 144, 176, 288 },	{	176, 144, 352, 288 },	{	352, 144, 528, 288 },	{	528, 144, 704, 288 },	
		{	0, 288, 176, 432 },	{	176, 288, 352, 432 },	{	352, 288, 528, 432 },	{	528, 288, 704, 432 },	
		{	0, 432, 176, 576 },	{	176, 432, 352, 576 },	{	352, 432, 528, 576 },	{	528, 432, 704, 576 },	
	}
};

Ptr_GrWordRect	V_UiConSoloQdRcTbl[E_GrDvrVdoSigTypeCnt][E_GrDvrQuadModeCnt]	=	
{
	//ntsc
	{
		V_UiConSoloQdrcMd1[E_GrDvrVdoNtsc],		V_UiConSoloQdrcMd4[E_GrDvrVdoNtsc],	
		V_UiConSoloQdrcMd6[E_GrDvrVdoNtsc],		V_UiConSoloQdrcMd8[E_GrDvrVdoNtsc],
		V_UiConSoloQdrcMd9[E_GrDvrVdoNtsc],		V_UiConSoloQdrcMd10[E_GrDvrVdoNtsc],
		V_UiConSoloQdrcMd13[E_GrDvrVdoNtsc],	V_UiConSoloQdrcMd16[E_GrDvrVdoNtsc]
	},
	//pal
	{
		V_UiConSoloQdrcMd1[E_GrDvrVdoPal],		V_UiConSoloQdrcMd4[E_GrDvrVdoPal],	
		V_UiConSoloQdrcMd6[E_GrDvrVdoPal],		V_UiConSoloQdrcMd8[E_GrDvrVdoPal],
		V_UiConSoloQdrcMd9[E_GrDvrVdoPal],		V_UiConSoloQdrcMd10[E_GrDvrVdoPal],
		V_UiConSoloQdrcMd13[E_GrDvrVdoPal],		V_UiConSoloQdrcMd16[E_GrDvrVdoPal]
	}
};

__u8	V_UiConSoloQdsclMd1[E_GrDvrQuadVwCntMd1]	=	
{
	E_SoloRv_VI_WIN_Scl1_1
};

__u8	V_UiConSoloQdsclMd4[E_GrDvrQuadVwCntMd4]	=	
{
	E_SoloRv_VI_WIN_Scl1_2,	E_SoloRv_VI_WIN_Scl1_2,	E_SoloRv_VI_WIN_Scl1_2,	E_SoloRv_VI_WIN_Scl1_2
};

__u8	V_UiConSoloQdsclMd6[E_GrDvrQuadVwCntMd6]	=	
{
	E_SoloRv_VI_WIN_Scl2_3,	E_SoloRv_VI_WIN_Scl1_3,	E_SoloRv_VI_WIN_Scl1_3,
	E_SoloRv_VI_WIN_Scl1_3,	E_SoloRv_VI_WIN_Scl1_3,	E_SoloRv_VI_WIN_Scl1_3
};

__u8	V_UiConSoloQdsclMd8[E_GrDvrQuadVwCntMd8]	=	
{
	E_SoloRv_VI_WIN_Scl3_4,	E_SoloRv_VI_WIN_Scl1_4,	E_SoloRv_VI_WIN_Scl1_4,	E_SoloRv_VI_WIN_Scl1_4,
	E_SoloRv_VI_WIN_Scl1_4,	E_SoloRv_VI_WIN_Scl1_4,	E_SoloRv_VI_WIN_Scl1_4,	E_SoloRv_VI_WIN_Scl1_4
};

__u8	V_UiConSoloQdsclMd9[E_GrDvrQuadVwCntMd9]	=	
{
	E_SoloRv_VI_WIN_Scl1_3,	E_SoloRv_VI_WIN_Scl1_3,	E_SoloRv_VI_WIN_Scl1_3,
	E_SoloRv_VI_WIN_Scl1_3,	E_SoloRv_VI_WIN_Scl1_3,	E_SoloRv_VI_WIN_Scl1_3,
	E_SoloRv_VI_WIN_Scl1_3,	E_SoloRv_VI_WIN_Scl1_3,	E_SoloRv_VI_WIN_Scl1_3
};

__u8	V_UiConSoloQdsclMd10[E_GrDvrQuadVwCntMd10]	=	
{
	E_SoloRv_VI_WIN_Scl1_2,	E_SoloRv_VI_WIN_Scl1_2,
	E_SoloRv_VI_WIN_Scl1_4,	E_SoloRv_VI_WIN_Scl1_4,	E_SoloRv_VI_WIN_Scl1_4,	E_SoloRv_VI_WIN_Scl1_4,
	E_SoloRv_VI_WIN_Scl1_4,	E_SoloRv_VI_WIN_Scl1_4,	E_SoloRv_VI_WIN_Scl1_4,	E_SoloRv_VI_WIN_Scl1_4
};

__u8	V_UiConSoloQdsclMd13[E_GrDvrQuadVwCntMd13]	=	
{
	E_SoloRv_VI_WIN_Scl1_2,	
	E_SoloRv_VI_WIN_Scl1_4,	E_SoloRv_VI_WIN_Scl1_4,	E_SoloRv_VI_WIN_Scl1_4,	E_SoloRv_VI_WIN_Scl1_4,
	E_SoloRv_VI_WIN_Scl1_4,	E_SoloRv_VI_WIN_Scl1_4,	E_SoloRv_VI_WIN_Scl1_4,	E_SoloRv_VI_WIN_Scl1_4,
	E_SoloRv_VI_WIN_Scl1_4,	E_SoloRv_VI_WIN_Scl1_4,	E_SoloRv_VI_WIN_Scl1_4,	E_SoloRv_VI_WIN_Scl1_4
};

__u8	V_UiConSoloQdsclMd16[E_GrDvrQuadVwCntMd16]	=	
{
	E_SoloRv_VI_WIN_Scl1_4,	E_SoloRv_VI_WIN_Scl1_4,	E_SoloRv_VI_WIN_Scl1_4,	E_SoloRv_VI_WIN_Scl1_4,
	E_SoloRv_VI_WIN_Scl1_4,	E_SoloRv_VI_WIN_Scl1_4,	E_SoloRv_VI_WIN_Scl1_4,	E_SoloRv_VI_WIN_Scl1_4,
	E_SoloRv_VI_WIN_Scl1_4,	E_SoloRv_VI_WIN_Scl1_4,	E_SoloRv_VI_WIN_Scl1_4,	E_SoloRv_VI_WIN_Scl1_4,
	E_SoloRv_VI_WIN_Scl1_4,	E_SoloRv_VI_WIN_Scl1_4,	E_SoloRv_VI_WIN_Scl1_4,	E_SoloRv_VI_WIN_Scl1_4
};

__u8*	V_UiConSoloQdSclTbl[E_GrDvrQuadModeCnt]	=	
{
	V_UiConSoloQdsclMd1,	V_UiConSoloQdsclMd4,	V_UiConSoloQdsclMd6,	V_UiConSoloQdsclMd8,
	V_UiConSoloQdsclMd9,	V_UiConSoloQdsclMd10,	V_UiConSoloQdsclMd13,	V_UiConSoloQdsclMd16
};

St_GrUiConSoloLnPos	V_UiConSoloQdLnPosMd1[E_GrDvrVdoSigTypeCnt][E_GrUiConSoloQdLnCntMd1]	=	
{
	//ntsc
	{
		{	{	0, 1 },		{	704, 1 }	},
		{	{	1, 0 },		{	1, 480 }	},
		{	{	703, 0 },	{	703, 480 }	},
		{	{	0, 479 },	{	704, 479 }	},
	},
	//pal
	{
		{	{	0, 1 },		{	704, 1 }	},
		{	{	1, 0 },		{	1, 576 }	},
		{	{	703, 0 },	{	703, 576 }	},
		{	{	0, 575 },	{	704, 575 }	},
	}
};

St_GrUiConSoloLnPos	V_UiConSoloQdLnPosMd4[E_GrDvrVdoSigTypeCnt][E_GrUiConSoloQdLnCntMd4]	=	
{
	//ntsc
	{
		{	{	0, 241 },	{	703, 241 }	},
		{	{	352, 0 },	{	352, 479 }	},

		{	{	0, 1 },		{	703, 1 }	},
		{	{	1, 0 },		{	1, 479 }	},
		{	{	703, 0 },	{	703, 479 }	},
		{	{	0, 479 },	{	704, 479 }	},
	},
	//pal
	{
		{	{	0, 288 },	{	703, 288 }	},
		{	{	352, 0 },	{	352, 575 }	},

		{	{	0, 1 },		{	703, 1 }	},
		{	{	1, 0 },		{	1, 575 }	},
		{	{	703, 0 },	{	703, 575 }	},
		{	{	0, 575 },	{	703, 575 }	},
	}
};

St_GrUiConSoloLnPos	V_UiConSoloQdLnPosMd6[E_GrDvrVdoSigTypeCnt][E_GrUiConSoloQdLnCntMd6]	=	
{
	//ntsc
	{
		/*

		{	{	0, 321 },	{	695, 321 }	},
		{	{	464, 161 },	{	695, 161 }	},
		{	{	464, 0 },	{	464, 479 }	},
		{	{	232, 320 },	{	232, 479 }	},

		{	{	0, 1 },		{	695, 1 }	},
		{	{	1, 0 },		{	1, 480 }	},
		{	{	695, 0 },	{	695, 479 }	},
		{	{	0, 479 },	{	695, 479 }	},

		*/
		{	{	0, 321 },	{	703, 321 }	},
		{	{	472, 161 },	{	703, 161 }	},
		{	{	472, 0 },	{	472, 479 }	},
		{	{	470, 0 },	{	470, 479 }	},
		{	{	236, 320 },	{	236, 479 }	},

		{	{	0, 1 },		{	703, 1 }	},
		{	{	1, 0 },		{	1, 480 }	},
		{	{	703, 0 },	{	703, 479 }	},
		{	{	0, 479 },	{	703, 479 }	},
	},
	//pal
	{
		{	{	0, 384 },	{	703, 384 }	},
		{	{	472, 192 },	{	703, 192 }	},
		{	{	472, 0 },	{	472, 576 }	},
		{	{	470, 0 },	{	470, 576 }	},
		{	{	236, 384 },	{	236, 576 }	},

		{	{	0, 1 },		{	703, 1 }	},
		{	{	1, 0 },		{	1, 575 }	},
		{	{	703, 0 },	{	703, 575 }	},
		{	{	0, 575 },	{	703, 575 }	},
	}
};

St_GrUiConSoloLnPos	V_UiConSoloQdLnPosMd8[E_GrDvrVdoSigTypeCnt][E_GrUiConSoloQdLnCntMd8]	=	
{
	//ntsc
	{
		{	{	0, 361 },	{	703, 361 }	},
		{	{	528, 121 },	{	703, 121 }	},
		{	{	528, 241 },	{	703, 241 }	},

		{	{	528, 0 },	{	528, 479 }	},
		{	{	176, 360 },	{	176, 479 }	},
		{	{	352, 360 },	{	352, 479 }	},

		{	{	0, 1 },		{	703, 1 }	},
		{	{	1, 0 },		{	1, 479 }	},
		{	{	703, 0 },	{	703, 479 }	},
		{	{	0, 479 },	{	703, 479 }	},
	},
	//pal
	{
		{	{	0, 432 },	{	703, 432 }	},
		{	{	528, 144 },	{	703, 144 }	},
		{	{	528, 288 },	{	703, 288 }	},

		{	{	528, 0 },	{	528, 575 }	},
		{	{	176, 432 },	{	176, 575 }	},
		{	{	352, 432 },	{	352, 575 }	},

		{	{	0, 1 },		{	703, 1 }	},
		{	{	1, 0 },		{	1, 575 }	},
		{	{	703, 0 },	{	703, 575 }	},
		{	{	0, 575 },	{	703, 575 }	},
	}
};

St_GrUiConSoloLnPos	V_UiConSoloQdLnPosMd9[E_GrDvrVdoSigTypeCnt][E_GrUiConSoloQdLnCntMd9]	=	
{
	//ntsc
	{
		{	{	0, 161 },	{	703, 161 }	},
		{	{	0, 321 },	{	703, 321 }	},
		{	{	236, 0 },	{	236, 479 }	},
		{	{	472, 0 },	{	472, 479 }	},

		{	{	0, 1 },		{	703, 1 }	},
		{	{	1, 0 },		{	1, 479 }	},
		{	{	703, 0 },	{	703, 479 }	},
		{	{	0, 479 },	{	703, 479 }	},
	},
	//pal
	{
		{	{	0, 192 },	{	703, 192 }	},
		{	{	0, 384 },	{	703, 384 }	},
		{	{	236, 0 },	{	236, 575 }	},
		{	{	472, 0 },	{	472, 575 }	},

		{	{	0, 1 },		{	703, 1 }	},
		{	{	1, 0 },		{	1, 575 }	},
		{	{	703, 0 },	{	703, 575 }	},
		{	{	0, 575 },	{	703, 575 }	},
	}
};

St_GrUiConSoloLnPos	V_UiConSoloQdLnPosMd10[E_GrDvrVdoSigTypeCnt][E_GrUiConSoloQdLnCntMd10]	=	
{
	// NTSC
	{
		{	{	0, 241 },	{	703, 241 }	},
		{	{	352, 0 },	{	352, 479 }	},

		{	{	0, 361 },	{	703, 361 }	},

		{	{	176, 240 },	{	176, 479 }	},
		{	{	528, 240 },	{	528, 479 }	},

		{	{	0, 1 },		{	703, 1 }	},
		{	{	1, 0 },		{	1, 479 }	},
		{	{	703, 0 },	{	703, 479 }	},
		{	{	0, 479 },	{	703, 479 }	},
	},
	// PAL
	{
		{	{	0, 288 },	{	703, 288 }	},
		{	{	352, 0 },	{	352, 575 }	},

		{	{	0, 432 },	{	703, 432 }	},

		{	{	176, 288 },	{	176, 575 }	},
		{	{	528, 288 },	{	528, 575 }	},

		{	{	0, 1 },		{	703, 1 }	},
		{	{	1, 0 },		{	1, 575 }	},
		{	{	703, 0 },	{	703, 575 }	},
		{	{	0, 575 },	{	703, 575 }	},
	}
};

St_GrUiConSoloLnPos	V_UiConSoloQdLnPosMd13[E_GrDvrVdoSigTypeCnt][E_GrUiConSoloQdLnCntMd13]	=	
{
	//ntsc
	{
		{	{	0, 121 },	{	703, 121 }	},
		{	{	0, 361 },	{	703, 361 }	},
		{	{	0, 241 },	{	176, 241 }	},
		{	{	528, 241 },	{	703, 241 }	},

		{	{	176, 0 },	{	176, 479 }	},
		{	{	528, 0 },	{	528, 479 }	},
		{	{	352, 0 },	{	352, 120 }	},
		{	{	352, 360 },	{	352, 479 }	},

		{	{	0, 1 },		{	703, 1 }	},
		{	{	1, 0 },		{	1, 479 }	},
		{	{	703, 0 },	{	703, 479 }	},
		{	{	0, 479 },	{	703, 479 }	},
	},
	//pal
	{
		{	{	0, 144 },	{	703, 144 }	},
		{	{	0, 432 },	{	703, 432 }	},
		{	{	0, 288 },	{	176, 288 }	},
		{	{	528, 288 },	{	703, 288 }	},

		{	{	176, 0 },	{	176, 575 }	},
		{	{	528, 0 },	{	528, 575 }	},
		{	{	352, 0 },	{	352, 144 }	},
		{	{	352, 432 },	{	352, 575 }	},

		{	{	0, 1 },		{	703, 1 }	},
		{	{	1, 0 },		{	1, 575 }	},
		{	{	703, 0 },	{	703, 575 }	},
		{	{	0, 575 },	{	703, 575 }	},
	}
};

St_GrUiConSoloLnPos	V_UiConSoloQdLnPosMd16[E_GrDvrVdoSigTypeCnt][E_GrUiConSoloQdLnCntMd16]	=	
{
	//ntsc
	{
		{	{	0, 121 },	{	704, 121}	},
		{	{	0, 241 },	{	704, 241 }	},
		{	{	0, 361 },	{	704, 361 }	},

		{	{	176, 0 },	{	176, 480 }	},
		{	{	352, 0 },	{	352, 480 }	},
		{	{	528, 0 },	{	528, 480 }	},

		{	{	0, 1 },		{	704, 1 }	},
		{	{	1, 0 },		{	1, 480 }	},
		{	{	703, 0 },	{	703, 480 }	},
		{	{	0, 479 },	{	704, 479 }	},
	},
	//pal
	{
		{	{	0, 144 },	{	704, 144 }	},
		{	{	0, 288 },	{	704, 288 }	},
		{	{	0, 432 },	{	704, 432 }	},

		{	{	176, 0 },	{	176, 576 }	},
		{	{	352, 0 },	{	352, 576 }	},
		{	{	528, 0 },	{	528, 576 }	},

		{	{	0, 1 },		{	704, 1 }	},
		{	{	1, 0 },		{	1, 576 }	},
		{	{	703, 0 },	{	703, 576 }	},
		{	{	0, 575 },	{	704, 575 }	},
	}
};

Ptr_GrUiConSoloLnPos	V_UiConSoloQdLnPosTbl[E_GrDvrVdoSigTypeCnt][E_GrDvrQuadModeCnt]	=	
{
	//ntsc
	{
		V_UiConSoloQdLnPosMd1[E_GrDvrVdoNtsc],	V_UiConSoloQdLnPosMd4[E_GrDvrVdoNtsc],	
		V_UiConSoloQdLnPosMd6[E_GrDvrVdoNtsc],	V_UiConSoloQdLnPosMd8[E_GrDvrVdoNtsc],
		V_UiConSoloQdLnPosMd9[E_GrDvrVdoNtsc],	V_UiConSoloQdLnPosMd10[E_GrDvrVdoNtsc],	
		V_UiConSoloQdLnPosMd13[E_GrDvrVdoNtsc],	V_UiConSoloQdLnPosMd16[E_GrDvrVdoNtsc]
	},
	//pal
	{
		V_UiConSoloQdLnPosMd1[E_GrDvrVdoPal],	V_UiConSoloQdLnPosMd4[E_GrDvrVdoPal],	
		V_UiConSoloQdLnPosMd6[E_GrDvrVdoPal],	V_UiConSoloQdLnPosMd8[E_GrDvrVdoPal],
		V_UiConSoloQdLnPosMd9[E_GrDvrVdoPal],	V_UiConSoloQdLnPosMd10[E_GrDvrVdoPal],	
		V_UiConSoloQdLnPosMd13[E_GrDvrVdoPal],	V_UiConSoloQdLnPosMd16[E_GrDvrVdoPal]
	}
};

__u8	V_UiConSoloQdLnCntTbl[E_GrDvrQuadModeCnt]	=	
{
	E_GrUiConSoloQdLnCntMd1,	E_GrUiConSoloQdLnCntMd4,	E_GrUiConSoloQdLnCntMd6,	E_GrUiConSoloQdLnCntMd8,
	E_GrUiConSoloQdLnCntMd9,	E_GrUiConSoloQdLnCntMd10,	E_GrUiConSoloQdLnCntMd13,	E_GrUiConSoloQdLnCntMd16
};

__u16	V_GrUiConSoloBarPosTbl[E_GrDvrVdoSigTypeCnt][E_GrDvrQuadModeCnt][2][E_SoloDispBarPosCnt]	=	
{
	//ntsc
	{
		{	/* x */{0, 702, 0, 0, 0}, /* y */{0, 478, 0, 0, 0} },		//mode 1
		{	/* x */{0, 350, 702, 0, 0}, /* y */{0, 238, 478, 0, 0} },		//mode 4
		{	/* x */{0, 234, 470, 702, 0}, /* y */{0, 158, 318, 478, 0} },		//mode 6
		{	/* x */{0, 174, 350, 526, 702}, /* y */{0, 118, 238, 358, 478} },		//mode 8
		{	/* x */{0, 234, 470, 702, 0}, /* y */{0, 158, 318, 478, 0} },		//mode 9
		{	/* x */{0, 174, 350, 526, 702}, /* y */{0, 118, 238, 358, 478} },		//mode 10
		{	/* x */{0, 174, 350, 526, 702}, /* y */{0, 118, 238, 358, 478} },		//mode 13
		{	/* x */{0, 174, 350, 526, 702}, /* y */{0, 118, 238, 358, 478} },		//mode 16
	},
	//pal
	{
		{	/* x */{0, 702, 0, 0, 0},				/* y */{0, 574, 0, 0, 0} },		//mode 1
		{	/* x */{0, 350, 702, 0, 0},			/* y */{0, 286, 574, 0, 0} },		//mode 4
		{	/* x */{0, 234, 470, 702, 0},		/* y */{0, 190, 382, 574, 0} },		//mode 6
		{	/* x */{0, 174, 350, 526, 702}, /* y */{0, 142, 286, 430, 574} },		//mode 8
		{	/* x */{0, 234, 470, 702, 0},		/* y */{0, 190, 382, 572, 0} },		//mode 9
		{	/* x */{0, 174, 350, 526, 702}, /* y */{0, 142, 286, 430, 574} },		//mode 10
		{	/* x */{0, 174, 350, 526, 702}, /* y */{0, 142, 286, 430, 574} },		//mode 13
		{	/* x */{0, 174, 350, 526, 702}, /* y */{0, 142, 286, 430, 574} },		//mode 16
	}
};

__u16	V_GrUiConSoloBarVwMapTbl[E_GrDvrQuadModeCnt][E_SoloMaxChCnt]	=	
{
	{	0x0001,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },	// mode 1
	{	0x0001,0x0002,0x0010,0x0020,0,0,0,0,0,0,0,0,0,0,0,0 },	// mode 4
	{	0x0033,0x0004,0x0040,0x0100,0x0200,0x0400,0,0,0,0,0,0,0,0,0,0 },	// mode 6
	{	0x0777,0x0008,0x0080,0x0800,0x1000,0x2000,0x4000,0x8000,0,0,0,0,0,0,0,0 },	// mode 8
	{	0x0001,0x0002,0x0004,0x0010,0x0020,0x0040,0x0100,0x0200,0x0400,0,0,0,0,0,0,0 },	// mode 9
	{	0x0033,0x00CC,0x0100,0x0200,0x0400,0x0800,0x1000,0x2000,0x4000,0x8000,0,0,0,0,0,0 },	// mode 10
	{	0x0660,0x0001,0x0002,0x0004,0x0008,0x0010,0x0080,0x0100,0x0800,0x1000,0x2000,0x4000,0x8000,0,0,0 },	// mode 13
	{	0x0001,0x0002,0x0004,0x0008,0x0010,0x0020,0x0040,0x0080,0x0100,0x0200,0x0400,0x0800,0x1000,0x2000,0x4000,0x8000 }	// mode 16
};

__u8		V_GrUiConSolo4chQdPgCntTbl[E_GrDvrQuadModeCnt]	=	
{	4,	1,	0,
	0,	0,	0,
	0,	0	};

__u8		V_GrUiConSolo9chQdPgCntTbl[E_GrDvrQuadModeCnt]	=	
{	9,	3,	2,
	2,	1,	0,
	0,	0	};

__u8		V_GrUiConSolo16chQdPgCntTbl[E_GrDvrQuadModeCnt]	=	
{	E_GrDvrQuadPgCntMd1,	E_GrDvrQuadPgCntMd4,	E_GrDvrQuadPgCntMd6,
	E_GrDvrQuadPgCntMd8,	E_GrDvrQuadPgCntMd9,	E_GrDvrQuadPgCntMd10,
	E_GrDvrQuadPgCntMd13,	E_GrDvrQuadPgCntMd16	};

//====================================================================
//functions

//--------------------------------------------------------------------
Cls_GrUiConSolo::Cls_GrUiConSolo( Cls_GrDevSolo* A_DevSolo ):
Cls_GrUiConsole( NULL )
{
	// local -------------------
	// code --------------------
		m_ObjDev	=	A_DevSolo;

		m_NoSigMap		=	0;
		m_MaxVdoChCnt	=	(__u8)m_ObjDev->VdoChCnt();

		//override gdib
		m_GdibScr	=	m_ObjDev->OsgGdibObjGet();
		//update rect
		m_ScrRect.left		=	0;
		m_ScrRect.top			=	0;
		m_ScrRect.right		=	(__s32)m_GdibScr->GetResX();
		m_ScrRect.bottom	=	(__s32)m_GdibScr->GetResY();

		m_ChUseMap				=	0xFFFF;

		//page count table
		switch( m_MaxVdoChCnt )
		{
			case	9:
				m_QuadMaxMode		=	E_GrDvrQuadMode9;
				m_PtrPageCntTbl	=	V_GrUiConSolo9chQdPgCntTbl;
				break;
			case	16:
				m_QuadMaxMode		=	E_GrDvrQuadMode16;
				m_PtrPageCntTbl	=	V_GrUiConSolo16chQdPgCntTbl;
				break;
			default:
				m_QuadMaxMode		=	E_GrDvrQuadMode4;
				m_PtrPageCntTbl	=	V_GrUiConSolo4chQdPgCntTbl;
				break;
		}

		//init color
		m_IsHwGridShow	=	TRUE;
		LcInitColor();

		//mouse cursor off
		m_ObjDev->MseCusImgSet( 0 );

		//set default channel map
		GrDvrQuadMapReset( (void*)m_QuadChMap, m_MaxVdoChCnt );
}
//--------------------------------------------------------------------
Cls_GrUiConSolo::~Cls_GrUiConSolo()
{
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiConSolo::HalPntReqTransfer( Ptr_GrRect A_PtrRect )
{
	// local -------------------
	// code --------------------
		return	m_ObjDev->OsgUpdateRect( A_PtrRect );
}
//--------------------------------------------------------------------
void	Cls_GrUiConSolo::HalMouseCusOnOff( void )
{
	// local -------------------
	// code --------------------
		if ( m_IsMseCusOn )
		{
			m_ObjDev->MseCusImgSet( m_MseCusImg + 1 );
		}
		else
		{
			m_ObjDev->MseCusImgSet( 0 );
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiConSolo::MouseCusImgSet( __u32 A_ImgIdx )
{
		Cls_GrUiConsole::MouseCusImgSet( A_ImgIdx );
		if ( m_IsMseCusOn )
		{
			m_ObjDev->MseCusImgSet( m_MseCusImg + 1 );
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiConSolo::MouseCusPosSet( __s32 A_X, __s32 A_Y )
{
		m_ObjDev->MseCusPosSet( (__u16)A_X, (__u16)A_Y );
}
//--------------------------------------------------------------------
__u8	Cls_GrUiConSolo::QuadViewIdxByPos( __s32 A_X, __s32 A_Y )
{
	// local -------------------
		__u8	Tv_Result;
		__u8	Tv_ViewIdx;
		Ptr_GrWordRect	Tv_PtrRc;
	// code --------------------
		Tv_Result	=	0;
		//get view index
		if ( 0 != m_QuadViewCnt )
		{
			Tv_PtrRc	=	V_UiConSoloQdRcTbl[E_GrDvrVdoNtsc][m_QuadMode];
			if ( V_GrUiMngObj->IsOsdExp() )
			{
				Tv_PtrRc	=	V_UiConSoloQdRcTbl[E_GrDvrVdoPal][m_QuadMode];
			}

			for ( Tv_ViewIdx=0;Tv_ViewIdx < m_QuadViewCnt;Tv_ViewIdx++ )
			{
				//check rect
				if ( ( (__u16)A_X >= Tv_PtrRc->left ) && ( (__u16)A_X < Tv_PtrRc->right ) && ( (__u16)A_Y >= Tv_PtrRc->top ) &&  ( (__u16)A_Y < Tv_PtrRc->bottom ) )
				{
					//get view index
					Tv_Result	=	Tv_ViewIdx;
					break;
				}
				//next
				Tv_PtrRc ++;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrUiConSolo::QuadLineTblGet( Ptr_GrLinePos A_PtrRtBuf )
{
	// local -------------------
		__u8	Tv_Result;
		__u8	Tv_WkIdx;
		Ptr_GrUiConSoloLnPos	Tv_PtrLnPos;
		Ptr_GrLinePos	Tv_PtrRt;
	// code --------------------
		Tv_Result	=	V_UiConSoloQdLnCntTbl[m_QuadMode];

		if ( 0 != Tv_Result )
		{
			Tv_PtrLnPos	=	V_UiConSoloQdLnPosTbl[E_GrDvrVdoNtsc][m_QuadMode];
			if ( V_GrUiMngObj->IsOsdExp() )
			{
				Tv_PtrLnPos	=	V_UiConSoloQdLnPosTbl[E_GrDvrVdoPal][m_QuadMode];
			}
			Tv_PtrRt	=	A_PtrRtBuf;

			for ( Tv_WkIdx = 0;Tv_WkIdx < Tv_Result;Tv_WkIdx++ )
			{
				Tv_PtrRt->St.x		=	(__s32)Tv_PtrLnPos->St.x;
				Tv_PtrRt->St.y		=	(__s32)Tv_PtrLnPos->St.y;
				Tv_PtrRt->Ed.x		=	(__s32)Tv_PtrLnPos->Ed.x;
				Tv_PtrRt->Ed.y		=	(__s32)Tv_PtrLnPos->Ed.y;
				//next
				Tv_PtrLnPos ++;
				Tv_PtrRt ++;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiConSolo::QuadModeSet( __u8 A_Mode, __u8 A_Page, BOOL8 A_IsNotify )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_PlayMap;
		__u32	Tv_PipMap;
		__u8*	Tv_PtrChMap;
		__u8*	Tv_PtrSclTbl;
		Ptr_GrWordRect	Tv_PtrRcTbl;
		__u8	Tv_ViewCnt;
		__u8	Tv_SigIdx;
		__u32	Tv_BarMap;
		__u32	Tv_WkIdx;
		__u32	Tv_ChMask;
		__u32	Tv_NoSigMap;
		__u16*	Tv_PtrBarPosTbl;
	// code --------------------
		Tv_Result	=	FALSE;
		//check changeable
		//if ( (!m_IsPlayMode) || (m_ObjDev->IsPlaying()) )
		{
			Tv_Result	=	Cls_GrUiConsole::QuadModeSet( A_Mode, A_Page, FALSE );
			if ( Tv_Result )
			{
				//release quad
				if ( E_GrDvrQuadMode1 != A_Mode )
				{
					//release quad mode
					m_IsPipShow		=	FALSE;
				}
				//signal
				Tv_SigIdx		=	E_GrDvrVdoNtsc;
				if ( V_GrUiMngObj->IsOsdExp() )
				{
					Tv_SigIdx		=	E_GrDvrVdoPal;
				}

				//play map
				Tv_PlayMap	=	0;
				if ( m_IsPlayMode )
				{
					Tv_PlayMap	=	0xFFFF;
				}
				//pip map
				Tv_PipMap		=	0;
				if ( m_IsPipShow )
				{
					Tv_PipMap		=	1 << (__u32)m_PipCh;
				}
				//get channel map pointer
				Tv_PtrChMap		=	GrDvrQuadMapPtr( m_QuadChMap, m_QuadMode, m_QuadPage );
				//get rect pointer
				Tv_PtrRcTbl		=	V_UiConSoloQdRcTbl[Tv_SigIdx][m_QuadMode];
				Tv_PtrSclTbl	=	V_UiConSoloQdSclTbl[m_QuadMode];
				Tv_PtrChMap		=	QuadGetPageChMapPtr( m_QuadMode, m_QuadPage );
				Tv_ViewCnt		=	GrDvrQuadViewCount( m_QuadMode );
				m_ObjDev->PlayChMapSet( Tv_PlayMap );
				//get covert bar map
				Tv_BarMap			=	0;
				if ( !m_IsPlayMode )
				{
					//check covert channel
					Tv_NoSigMap	=	(__u32)NoSigMapGet();
					Tv_NoSigMap	=	Tv_NoSigMap | (__u32)m_CvtUsrMap;			// user no signal map
					Tv_NoSigMap	=	Tv_NoSigMap | (__u32)( ~m_ChUseMap);	// channel use map
					if ( m_IsAllCvt )
					{
						Tv_NoSigMap	=	0xFFFF;		//all covert
					}
					for ( Tv_WkIdx=0;Tv_WkIdx < Tv_ViewCnt;Tv_WkIdx++ )
					{
						//get channel
						Tv_ChMask	=	1 << ( (__u32)(Tv_PtrChMap[Tv_WkIdx]) );
						if ( ( 0 != (m_CorvertMap & Tv_ChMask) ) || ( 0 != (Tv_NoSigMap & Tv_ChMask) ) )
						{
							Tv_BarMap	=	Tv_BarMap | V_GrUiConSoloBarVwMapTbl[m_QuadMode][Tv_WkIdx];
						}
					}
				}
				Tv_PtrBarPosTbl	=	(__u16*)(V_GrUiConSoloBarPosTbl[Tv_SigIdx][m_QuadMode]);
				Tv_Result			=	m_ObjDev->QuadWinSet( Tv_PtrRcTbl, Tv_PtrSclTbl, Tv_PtrChMap, Tv_PipMap, Tv_ViewCnt, 
					Tv_PtrBarPosTbl, Tv_BarMap, 0, m_QuadMode, FALSE );
				// quad change notify
				if ( A_IsNotify )
				{
					SysNotifyQuadChg();
				}
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void*	Cls_GrUiConSolo::QuadGetChMapPtr( void )
{
		return	(void*)m_QuadChMap;
}
//--------------------------------------------------------------------
__u32	Cls_GrUiConSolo::QuadGetChMapSize( void )
{
		return	E_GrDvrQuadMapSize;
}
//--------------------------------------------------------------------
__u8*	Cls_GrUiConSolo::QuadGetPageChMapPtr( __u8 A_Mode, __u8 A_Page )
{
		return	GrDvrQuadMapPtr( (void*)m_QuadChMap, A_Mode, A_Page );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiConSolo::QuadRectGet( __u8 A_ViewIdx, Ptr_GrRect A_PtrRtRect )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u8		Tv_SigIdx;		
		Ptr_GrWordRect	Tv_PtrRc;
	// code --------------------
		Tv_Result		=	FALSE;
		//check view index
		if ( A_ViewIdx < m_QuadViewCnt )
		{
			//signal
			Tv_SigIdx		=	E_GrDvrVdoNtsc;
			if ( V_GrUiMngObj->IsOsdExp() )
			{
				Tv_SigIdx		=	E_GrDvrVdoPal;
			}
			//get rect
			Tv_PtrRc	=	V_UiConSoloQdRcTbl[Tv_SigIdx][m_QuadMode];
			A_PtrRtRect->left			=	(__s32)Tv_PtrRc[A_ViewIdx].left;
			A_PtrRtRect->top			=	(__s32)Tv_PtrRc[A_ViewIdx].top;
			A_PtrRtRect->right		=	(__s32)Tv_PtrRc[A_ViewIdx].right;
			A_PtrRtRect->bottom		=	(__s32)Tv_PtrRc[A_ViewIdx].bottom;

			//success
			Tv_Result		=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrUiConSolo::QuadGetChByView( __u8 A_View )
{
	// local -------------------
		__u8*		Tv_PtrChMap;
	// code --------------------
		Tv_PtrChMap	=	QuadGetPageChMapPtr( m_QuadMode, m_QuadPage );
		return	Tv_PtrChMap[A_View];
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiConSolo::IsQuadHwGrid( void )
{
		return	FALSE;		//wil change true
}
//--------------------------------------------------------------------
void	Cls_GrUiConSolo::QuadGridShow( BOOL8 A_IsShow )
{
		m_IsHwGridShow	=	A_IsShow;
		//draw hw grid

}
//--------------------------------------------------------------------
void	Cls_GrUiConSolo::CorvertMapSet( __u16 A_Map )
{
		//check change
		if ( m_CorvertMap != A_Map )
		{
			Cls_GrUiConsole::CorvertMapSet( A_Map );
			//update covert
			if ( !m_IsPlayMode )
			{
				QuadModeSet( m_QuadMode, m_QuadPage );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiConSolo::PipShow( __u8 A_Ch, __u8 A_Scale, __s32 A_X, __s32 A_Y )
{
		//mode change 1

		//build pip table
		
		//request quad window set

}
//--------------------------------------------------------------------
void	Cls_GrUiConSolo::PipHide( void )
{
		//pip hide

}
//--------------------------------------------------------------------
void	Cls_GrUiConSolo::PlayModeSet( BOOL8 A_IsPlay )
{
	// local -------------------
	// code --------------------
		Cls_GrUiConsole::PlayModeSet( A_IsPlay );
		QuadModeSet( m_QuadMode, m_QuadPage );		//reset quad win
		//notify playmode change
		NotifyGbStatChg( E_GrUiChgPlayMode );
}
//--------------------------------------------------------------------
__u16	Cls_GrUiConSolo::NoSigMapGet( void )
{
		//get value from dvr api 
		return	m_NoSigMap;
}
//--------------------------------------------------------------------
__u8	Cls_GrUiConSolo::RecAtbGet( __u8 A_Ch )
{
	// local -------------------
		__u8	Tv_Result;
		__u32	Tv_Map;
	// code --------------------
		Tv_Result	=	0;
		Tv_Map		=	m_ObjDev->RecAtbGet( A_Ch );
		if ( 0 != (0xFF & Tv_Map) )
		{
			Tv_Result	=	E_GrDvrRecAtbRecord;
		}
		Tv_Result	=	Tv_Result | (__u8)(0xFF & (Tv_Map >> 8));		//new type status
		/*
		Tv_Map	=	m_ObjDev->RecMotionMapGet();
		if ( 0 != (Tv_Map & (1 << A_Ch)) )
		{
			Tv_Result	=	Tv_Result | E_GrDvrRecAtbMotion;
		}
		Tv_Map	=	m_ObjDev->RecSensorMapGet();
		if ( 0 != (Tv_Map & (1 << A_Ch)) )
		{
			Tv_Result	=	Tv_Result | E_GrDvrRecAtbSensor;
		}
		*/

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrUiConSolo::PlayAtbGet( __u8 A_Ch )
{
		return	m_ObjDev->PlayAtbGet( A_Ch );
}
//--------------------------------------------------------------------
__u32	Cls_GrUiConSolo::GetResY( void )
{
	// local -------------------
		__u32	Tv_Result;
	// code --------------------
		Tv_Result		=	E_SoloNtscResY;
		if ( V_GrUiMngObj->IsOsdExp() )
		{
			Tv_Result		=	E_SoloPalResY;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiConSolo::NoSigMapSet( __u16 A_Map )
{
		if ( m_NoSigMap != A_Map )
		{
			m_NoSigMap	=	A_Map;
			if ( !m_IsPlayMode )
			{
				QuadModeSet( m_QuadMode, m_QuadPage );		//reupdate
			}
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrUiConSolo::QuadPageCntByMode( __u8 A_Mode )
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		if ( E_GrDvrQuadModeCnt > A_Mode )
		{
			Tv_Result	=	m_PtrPageCntTbl[A_Mode];
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrUiConSolo::QuadMaxModeGet( void )
{
		return	m_QuadMaxMode;
}
//--------------------------------------------------------------------
__u16	Cls_GrUiConSolo::ChUseMapGet( void )
{
		return	m_ChUseMap;
}
//--------------------------------------------------------------------
void	Cls_GrUiConSolo::ChUseMapSet( __u16 A_Map )
{
		m_ChUseMap	=	A_Map;
}
//--------------------------------------------------------------------


