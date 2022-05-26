/*
		image process adaptor base

 */

//====================================================================
// uses
#include	<Ipa/GrIpaBase.h>
#include	<GrDebug.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<GrStrTool.h>


//====================================================================
//local const

//====================================================================
//local type

//====================================================================
//local macro

//====================================================================
//local function

//====================================================================
//var

#ifdef GR_IPA_EDIT

St_GrIpaAdptInfo	V_GrIpaAdptInfoTbl[E_GrIpaAdptInfoCnt]	=	
{
	{ E_GrIpaAdptTypeSrcIn,			L"Source In"	},
	{ E_GrIpaAdptTypeMonOut,		L"Monitor out"	},
	{ E_GrIpaAdptTypeSplitYuv,	L"Split YUV"	},
	{ E_GrIpaAdptTypeBindYuv,		L"Bind YUV"	},
	{ E_GrIpaAdptTypeBinary,		L"2D Binary"	},
	{ E_GrIpaAdptTypeWdr,				L"WDR" },
};

#endif

//====================================================================
//--------------------------------------------------------------------
#ifdef GR_IPA_EDIT
WCHAR*	GrIpaAdoptNameGet( __u16 A_Type )
{
	// local -------------------
		WCHAR*	Tv_Result;
		__u16		Tv_WkIdx;
	// code --------------------
		Tv_Result	=	NULL;

		for (Tv_WkIdx=0;Tv_WkIdx <E_GrIpaAdptInfoCnt;Tv_WkIdx++ )
		{
			if ( A_Type == V_GrIpaAdptInfoTbl[Tv_WkIdx].TypeId )
			{
				Tv_Result	=	V_GrIpaAdptInfoTbl[Tv_WkIdx].StrName;
			}
		}

		return	Tv_Result;
}
#endif
//--------------------------------------------------------------------
