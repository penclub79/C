/*

GL base

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_GrGlBase
#define	_GrGlBase

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


typedef struct St_GrGlEnv
{

}	*Ptr_GrGlEnv;

//====================================================================
//class

//====================================================================
//global function

Def_GrErrCode	GrGlInit(void);

//====================================================================
//global variable

//====================================================================

//out Enviroment conditional
#endif


