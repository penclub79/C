/*
	protocol database

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrPrtcDb
#define	_EPC_GrPrtcDb

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>

#include <GrError.h>

//====================================================================
//constance

#define	E_GrPrtcDbFcc										Mac_GrMakeFcc('D','Q','R','Y')

#define	E_GrPrtcDbKeySize								64

// command
#define	E_GrPrtcDbCmdDataGetByOffset		1
#define	E_GrPrtcDbCmdRtnDataOne					2
#define	E_GrPrtcDbCmdDataSetByOffset		3
#define	E_GrPrtcDbCmdResult							4
#define	E_GrPrtcDbCmdKeyCreate					5
#define	E_GrPrtcDbCmdKeyDelete					6
#define	E_GrPrtcDbCmdKeyNewWithData			7
#define	E_GrPrtcDbCmdKeyAlocWithData		8

// result code
#define	E_GrPrtcDbResultOk							0
#define	E_GrPrtcDbResultBadPara					1
#define	E_GrPrtcDbResultNotFound				2
#define	E_GrPrtcDbResultNotEnoughSpace	3
#define	E_GrPrtcDbResultBusy						4


//====================================================================
//global macro

//====================================================================
//global type

#pragma	pack(1)

typedef struct St_GrPrtcDbBase
{
	__u32	Fcc;
	__u32	Cmd;
	__u32	Size;
}	*Ptr_GrPrtcDbBase;

typedef struct St_GrPrtcDbReqItem
{
	__u32	Ofs;
	__u32	Size;
}	*Ptr_GrPrtcDbReqItem;

typedef struct St_GrPrtcDbGetDataByOfs
{
	__u32	Fcc;
	__u32	Cmd;
	__u32	Size;
	__u32	GrpId;
	__u8	KeyVal[E_GrPrtcDbKeySize];
	__u32	UsrId;
	__u32	ReqCnt;
	
}	*Ptr_GrPrtcDbGetDataByOfs;

typedef struct St_GrPrtcDbRtnDataOne
{
	__u32	Fcc;
	__u32	Cmd;
	__u32	Size;
	__u32	GrpId;
	__u8	KeyVal[E_GrPrtcDbKeySize];
	__u32	UsrId;
	__u32	ErrCode;
}	*Ptr_GrPrtcDbRtnDataOne;

typedef struct St_GrPrtcDbPutDataByOfs
{
	__u32	Fcc;
	__u32	Cmd;
	__u32	Size;
	__u32	GrpId;
	__u8	KeyVal[E_GrPrtcDbKeySize];
	__u32	UsrId;
	__u32	Ofs;
}	*Ptr_GrPrtcDbPutDataByOfs;

typedef struct St_GrPrtcDbResult
{
	__u32	Fcc;
	__u32	Cmd;
	__u32	Size;
	__u32	GrpId;
	__u8	KeyVal[E_GrPrtcDbKeySize];
	__u32	UsrId;
	__u32	ErrCode;
}	*Ptr_GrPrtcDbResult;

typedef struct St_GrPrtcDbKeyCreate
{
	__u32	Fcc;
	__u32	Cmd;
	__u32	Size;
	__u32	GrpId;
	__u8	KeyVal[E_GrPrtcDbKeySize];
	__u32	UsrId;

}	*Ptr_GrPrtcDbKeyCreate;

typedef struct St_GrPrtcDbKeyDelete
{
	__u32	Fcc;
	__u32	Cmd;
	__u32	Size;
	__u32	GrpId;
	__u8	KeyVal[E_GrPrtcDbKeySize];
	__u32	UsrId;

}	*Ptr_GrPrtcDbKeyDelete;

typedef struct St_GrPrtcDbKeyNewWithData
{
	__u32	Fcc;
	__u32	Cmd;
	__u32	Size;
	__u32	GrpId;
	__u8	KeyVal[E_GrPrtcDbKeySize];
	__u32	UsrId;
	__u32	Ofs;
}	*Ptr_GrPrtcDbKeyNewWithData;

typedef struct St_GrPrtcDbKeyAlocWithData
{
	__u32	Fcc;
	__u32	Cmd;
	__u32	Size;
	__u32	GrpId;
	__u8	KeyVal[E_GrPrtcDbKeySize];
	__u32	UsrId;
	__u32	Ofs;
}	*Ptr_GrPrtcDbKeyAlocWithData;


#pragma	pack()

//====================================================================
//class

	
//====================================================================
//global function

//====================================================================
//out Enviroment conditional
#endif

