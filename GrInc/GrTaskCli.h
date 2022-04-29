/*
	Task client
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrTaskCli
#define	_EPC_GrTaskCli

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<GrProcess.h>
#include	<GrTaskBase.h>

//====================================================================
//constance

//====================================================================
//global macro


//====================================================================
//global type


//====================================================================
//class
class	Cls_GrTaskCli
{
	protected:
		//api

	public:
		Cls_GrTaskCli( void );
		virtual	~Cls_GrTaskCli();

		//runtime
		virtual	BOOL8	RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg );		/* return = message processed */
		virtual	BOOL8	RtlBkgProc( void );											/* return = background processed */
		virtual	void	RtlTimer( __u32 A_Hndl );

		//callback
		virtual	BOOL8	RtlCallback( Ptr_GrTaskMsgHd A_PtrMsg );	


};

//====================================================================
//global function
	
//====================================================================
//out Enviroment conditional
#endif
