/*

DB base

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_GrDbBase
#define	_GrDbBase

//====================================================================
// uses
#include <afxwin.h>

#include <GrTypeBase.h>
#include <GrTaskBase.h>
#include <GrTaskCli.h>
#include <GrTaskSvr.h>


#include <GrError.h>
#include <GrStrTool.h>

#include <dxgi1_3.h>

//====================================================================
//constance



//====================================================================
//global macro

//====================================================================
//global type


typedef struct St_GrDxEnv
{

}	*Ptr_GrDxEnv;

//====================================================================
//class

//====================================================================
//global function

Def_GrErrCode	GrDxInit(void);

//====================================================================
//global variable

//====================================================================

//out Enviroment conditional
#endif


