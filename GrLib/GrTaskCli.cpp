/*
	task server

*/

//====================================================================
//include
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <GrTaskCli.h>

//====================================================================
//local const

//====================================================================
//local type

//====================================================================
//global var

//====================================================================
//functions

//--------------------------------------------------------------------
Cls_GrTaskCli::Cls_GrTaskCli()
{
}
//--------------------------------------------------------------------
Cls_GrTaskCli::~Cls_GrTaskCli()
{
}
//--------------------------------------------------------------------
BOOL8	Cls_GrTaskCli::RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg )
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrTaskCli::RtlBkgProc( void )
{
		return	FALSE;
}
//--------------------------------------------------------------------
void	Cls_GrTaskCli::RtlTimer( __u32 A_Hndl )
{
}
//--------------------------------------------------------------------
BOOL8	Cls_GrTaskCli::RtlCallback( Ptr_GrTaskMsgHd A_PtrMsg )
{
		return	FALSE;
}
//--------------------------------------------------------------------

