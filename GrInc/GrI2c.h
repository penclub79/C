/*
	gause i2c control


*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrI2c
#define	_EPC_GrI2c

//====================================================================
// uses

#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

//====================================================================
//const

#define E_GrI2cMaxChCnt			4


//====================================================================
//type

typedef	BOOL8	(*Fnc_GrI2cSlotRead)( __u32 A_Id, __u8 A_Slot, void* A_PtrData, BOOL8 A_IsWait );
typedef	BOOL8	(*Fnc_GrI2cStdWrite)( __u32 A_Id, __u8 A_AdrDev, __u8 A_AdrSub, __u8* A_PtrData, __u32 A_Size );
typedef	BOOL8	(*Fnc_GrI2cStdRead)( __u32 A_Id, __u8 A_AdrDev, __u8 A_AdrSub, __u32 A_Size, __u8 A_Slot );
typedef	BOOL8	(*Fnc_GrI2cS16Write)( __u32 A_Id, __u8 A_AdrDev, __u16 A_AdrSub, __u8* A_PtrData, __u32 A_Size );
typedef	BOOL8	(*Fnc_GrI2cS16Read)( __u32 A_Id, __u8 A_AdrDev, __u16 A_AdrSub, __u32 A_Size, __u8 A_Slot );


typedef	struct St_GrI2cOps 
{
	Fnc_GrI2cSlotRead	SlotRead;
	Fnc_GrI2cStdWrite	StdWrite;
	Fnc_GrI2cStdRead	StdRead;
	Fnc_GrI2cS16Write	S16Write;
	Fnc_GrI2cS16Read	S16Read;
}	*Ptr_GrI2cOps;

typedef	struct St_GrI2cChItm 
{
	BOOL8	IsExist;
	__u32	Id;					//regist id
	St_GrI2cOps	Ops;
}	*Ptr_GrI2cChItm;

//====================================================================
//class


//====================================================================
//function

BOOL8	GrI2cIsRegisted( __u8 A_Ch );
BOOL8	GrI2cRegist( __u8 A_Ch, Ptr_GrI2cOps A_PtrOps, __u32 A_Id );
BOOL8	GrI2cUnregist( __u8 A_Ch );

BOOL8	GrI2cSlotRead( __u8 A_Ch, __u8 A_Slot, void* A_PtrData, BOOL8 A_IsWait );
BOOL8	GrI2cStdWrite( __u8 A_Ch, __u8 A_AdrDev, __u8 A_AdrSub, __u8* A_PtrData, __u32 A_Size );
BOOL8	GrI2cStdRead( __u8 A_Ch, __u8 A_AdrDev, __u8 A_AdrSub, __u32 A_Size, __u8 A_Slot );
BOOL8	GrI2cS16Write( __u8 A_Ch, __u8 A_AdrDev, __u16 A_AdrSub, __u8* A_PtrData, __u32 A_Size );
BOOL8	GrI2cS16Read( __u8 A_Ch, __u8 A_AdrDev, __u16 A_AdrSub, __u32 A_Size, __u8 A_Slot );

//====================================================================
//out Enviroment conditional
#endif
