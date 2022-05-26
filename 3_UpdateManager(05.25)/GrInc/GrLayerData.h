/*
	Layer data

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrLayerData
#define	_EPC_GrLayerData

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>

#include <GrError.h>

#include <GrFileCtrl.h>

//====================================================================
//constance

#define	E_GrLayerDataFcc							Mac_GrMakeFcc('L','F','V','1')
#define	E_GrLayerDataSeparatorId			0x504C

#define	E_GrLayerDataIdEnd						0		/* data id end */
#define	E_GrLayerDataIdBase						1		/* data id base */

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrLayerDataHead
{
	__u32	Fcc;
	__u32	UserInfoSize;
}	*Ptr_GrLayerDataHead;

typedef struct St_GrLayerDataLayerHead
{
	__u16	SeparatorId;
	__u16	DataId;
	__u32	Size;					// data size, without layer header size
	__u32	Para0;
	__u32	Para1;
}	*Ptr_GrLayerDataLayerHead;

typedef struct St_GrLayerDataParsRtl
{
	Ptr_GrLayerDataHead PtrBuf;
	void*	PtrUserInfo;
	void*	PtrCont;
	__u32	Prgs;
	BOOL8	IsFinish;
}	*Ptr_GrLayerDataParsRtl;

//====================================================================
//class

	
//====================================================================
//global function

BOOL8	GrLayerDataParsInit(Ptr_GrLayerDataParsRtl A_PtrRtl, void* A_PtrBuf);
void*	GrLayerDataParsGetUserInfo(Ptr_GrLayerDataParsRtl A_PtrRtl, __u32* A_PtrRtSize);
void*	GrLayerDataParsDo(Ptr_GrLayerDataParsRtl A_PtrRtl, __u16* A_PtrRtDataId, __u32* A_PtrRtSize, __u32* A_PtrRtPara0, __u32* A_PtrRtPara1);


//====================================================================
//out Enviroment conditional
#endif

