/*
product regist server

*/

//====================================================================
//include

#include <Win/Dx/GrDxBase.h>

#include <GrDebug.h>
#include <GrError.h>

#include <GrDumyTool.h>


//====================================================================
//local const


//====================================================================
//local type

//====================================================================
//global var

St_GrDxEnv	V_GrDxEnv;

//====================================================================
//functions

//--------------------------------------------------------------------
Def_GrErrCode	GrDxInit(void)
{
	// local -------------------
		HRESULT	Tv_RtCode;
		__u32		Tv_DbgFlag;
	// code --------------------
		// initialize
		Tv_DbgFlag	=	0;
#ifdef _DEBUG
		Tv_DbgFlag	=	DXGI_CREATE_FACTORY_DEBUG;
#endif

		Tv_RtCode	=	CreateDXGIFactory2(Tv_DbgFlag,
			IID_PPV_ARGS();
}
//--------------------------------------------------------------------

