/*
	dvr io manager

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrDvrIoMng
#define	_EPC_GrDvrIoMng

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrTaskSvr.h>
#include <Dvr/GrDvrBase.h>
#include <GrTaskCli.h>
#include <Dvr/Io/GrDvrIoCliBase.h>
#include <DvrRtl.h>
	
//====================================================================
//constance
#ifdef E_DvrRtlTestVersion_PTZCtrl
#define E_GrDvrIoModelCnt		7
#else
#define E_GrDvrIoModelCnt		6
#endif

//====================================================================
//global macro

//====================================================================
//global type

//====================================================================
//class

class	Cls_GrDvrIoMng	:	public	Cls_GrTaskCli
{
	private:
		Cls_GrTaskSvr*	m_TaskSvr;
		__u32		m_MsgAloc;
		__u32		m_HndlTimer;

		Cls_GrDvrIoCliBase*	m_ObjIo;

	protected:

		//local
		void	LcSetup( __u8 A_Model, __u8 A_Uart, __u8 A_Id );
		void	LcUserAction( __u32 A_ActId );
		void	LcTimeSet( Def_GrTime A_Time );//%%
		void	LcCheckRtcBox( void );//^^%%

		//event
		virtual	BOOL8	RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg );		/* return = message processed */
		virtual	BOOL8	RtlBkgProc( void );
		virtual	void	RtlTimer( __u32 A_Hndl );

	public:
    Cls_GrDvrIoMng( Cls_GrTaskSvr* A_TaskSvr );
    virtual		~Cls_GrDvrIoMng();

		WCHAR*	ModelNameGet( __u8 A_Model );
		void	Setup( __u8 A_Model, __u8 A_Uart, __u8 A_Id );
		void	UserAction( __u32 A_ActId );
		void	TimeSet( Def_GrTime A_Time );//%%
		void	CheckRtcBox( void );
		BOOL8	RetrnRtcBox( void );
		BOOL8	RetrnGPS( void );

};
	
//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif


