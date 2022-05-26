/*
	Layer file

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrLayerFile
#define	_EPC_GrLayerFile

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>

#include <GrError.h>

#include <GrLayerData.h>
#include <GrFileCtrl.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type


//====================================================================
//class

class	Cls_GrLayerFileSave
{
	protected:
		BOOL8	m_IsNoEmu;

		St_GrLayerDataHead	m_FlHead;

		Cls_GrFileCtrl*	m_Fc;		// file control

	public:
		Cls_GrLayerFileSave(BOOL8 A_IsNoEmu);
		virtual		~Cls_GrLayerFileSave();

		Def_GrErrCode	Open(char* A_StrFn,void* A_PtrUserInfo,__u32 A_UserInfoSize);
		Def_GrErrCode	Open(WCHAR* A_StrFn, void* A_PtrUserInfo, __u32 A_UserInfoSize);

		Def_GrErrCode	DataAdd(__u16 A_DataId, void* A_PtrBuf, __u32 A_Size, __u32 A_Para0, __u32 A_Para1);
};

class	Cls_GrLayerFileLoad
{
	protected:
		BOOL8	m_IsNoEmu;
		BOOL8	m_IsExistData;
		St_GrLayerDataLayerHead	m_LyrHead;

		Cls_GrFileCtrl*	m_Fc;		// file control

		Def_GrErrCode	LcLayerHeadRead(void);
		Def_GrErrCode	LcSkip(__u32 A_Size);

	public:
		Cls_GrLayerFileLoad(BOOL8 A_IsNoEmu);
		virtual		~Cls_GrLayerFileLoad();

		Def_GrErrCode	Open(char* A_StrFn,void* A_PtrUserInfo,__u32 A_MaxSize,__u32*A_PtrRtSize);
		Def_GrErrCode	Open(WCHAR* A_StrFn, void* A_PtrUserInfo, __u32 A_MaxSize, __u32*A_PtrRtSize);

		Def_GrErrCode	LayerInfoGet(__u16* A_PtrRtDataId,__u32* A_PtrRtSize,__u32* A_PtrRtPara0,__u32* A_PtrRtPara1);
		Def_GrErrCode	LayerDataLoad(void* A_PtrBuf,__u32 A_BufSize);
		Def_GrErrCode	LayerDataSkip(void);

};

	
//====================================================================
//global function

//====================================================================
//out Enviroment conditional
#endif

