/*
	I2C


*/

#include	<GrI2c.h>

//--------------------------------------------------------------------
//local function


//--------------------------------------------------------------------
//global var

St_GrI2cChItm	V_GrI2cChTbl[E_GrI2cMaxChCnt]	=	
{
	{ FALSE, 0,	},	{ FALSE, 0,	},	{ FALSE, 0,	},	{ FALSE, 0,	}
};

//====================================================================
//--------------------------------------------------------------------
BOOL8	GrI2cIsRegisted( __u8 A_Ch )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrI2cMaxChCnt > A_Ch )
		{
			Tv_Result	=	V_GrI2cChTbl[A_Ch].IsExist;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrI2cRegist( __u8 A_Ch, Ptr_GrI2cOps A_PtrOps, __u32 A_Id )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrI2cMaxChCnt > A_Ch )
		{
			V_GrI2cChTbl[A_Ch].Id				=	A_Id;
			V_GrI2cChTbl[A_Ch].Ops			=	*A_PtrOps;
			V_GrI2cChTbl[A_Ch].IsExist	=	TRUE;
			Tv_Result	=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrI2cUnregist( __u8 A_Ch )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrI2cMaxChCnt > A_Ch )
		{
			V_GrI2cChTbl[A_Ch].IsExist		=	FALSE;
			Tv_Result	=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrI2cSlotRead( __u8 A_Ch, __u8 A_Slot, void* A_PtrData, BOOL8 A_IsWait )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrI2cMaxChCnt > A_Ch )
		{
			if ( V_GrI2cChTbl[A_Ch].IsExist )
			{
				if ( NULL != V_GrI2cChTbl[A_Ch].Ops.SlotRead )
				{
					Tv_Result	=	V_GrI2cChTbl[A_Ch].Ops.SlotRead( V_GrI2cChTbl[A_Ch].Id, A_Slot, A_PtrData, A_IsWait );
				}
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrI2cStdWrite( __u8 A_Ch, __u8 A_AdrDev, __u8 A_AdrSub, __u8* A_PtrData, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrI2cMaxChCnt > A_Ch )
		{
			if ( V_GrI2cChTbl[A_Ch].IsExist )
			{
				if ( NULL != V_GrI2cChTbl[A_Ch].Ops.StdWrite )
				{
					Tv_Result	=	V_GrI2cChTbl[A_Ch].Ops.StdWrite( V_GrI2cChTbl[A_Ch].Id, A_AdrDev, A_AdrSub, A_PtrData, A_Size );
				}
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrI2cStdRead( __u8 A_Ch, __u8 A_AdrDev, __u8 A_AdrSub, __u32 A_Size, __u8 A_Slot )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrI2cMaxChCnt > A_Ch )
		{
			if ( V_GrI2cChTbl[A_Ch].IsExist )
			{
				if ( NULL != V_GrI2cChTbl[A_Ch].Ops.StdRead )
				{
					Tv_Result	=	V_GrI2cChTbl[A_Ch].Ops.StdRead( V_GrI2cChTbl[A_Ch].Id, A_AdrDev, A_AdrSub, A_Size, A_Slot );
				}
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrI2cS16Write( __u8 A_Ch, __u8 A_AdrDev, __u16 A_AdrSub, __u8* A_PtrData, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrI2cMaxChCnt > A_Ch )
		{
			if ( V_GrI2cChTbl[A_Ch].IsExist )
			{
				if ( NULL != V_GrI2cChTbl[A_Ch].Ops.S16Write )
				{
					Tv_Result	=	V_GrI2cChTbl[A_Ch].Ops.S16Write( V_GrI2cChTbl[A_Ch].Id, A_AdrDev, A_AdrSub, A_PtrData, A_Size );
				}
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrI2cS16Read( __u8 A_Ch, __u8 A_AdrDev, __u16 A_AdrSub, __u32 A_Size, __u8 A_Slot )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrI2cMaxChCnt > A_Ch )
		{
			if ( V_GrI2cChTbl[A_Ch].IsExist )
			{
				if ( NULL != V_GrI2cChTbl[A_Ch].Ops.S16Read )
				{
					Tv_Result	=	V_GrI2cChTbl[A_Ch].Ops.S16Read( V_GrI2cChTbl[A_Ch].Id, A_AdrDev, A_AdrSub, A_Size, A_Slot );
				}
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------

