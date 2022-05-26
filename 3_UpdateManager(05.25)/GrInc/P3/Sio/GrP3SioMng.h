/*
	DVR Serial communication I/O device manager

	Timer period is 1/10 second
*/

//--------------------------------------------------------------------
// Environment conditional
#ifndef	_EPC_GrP3SioMng
#define	_EPC_GrP3SioMng

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrTaskSvr.h>
#include <Dvr/GrDvrBase.h>
#include <GrTaskCli.h>
#include <P3/Sio/GrP3SioBase.h>
	
//====================================================================
//constance
#define E_GrP3SioModelMaxCnt					3

#define E_GrP3SioMngTimerPeriod				100

// message command
#define E_GrP3SioMsgCmdSetup					1		/* setup : St_GrP3SioMsgSetup */
#define E_GrP3SioMsgCmdPiCmd					2		/* plug in command : St_GrP3SioMsgPiCmd */

//====================================================================
//global macro

//====================================================================
//global type

typedef	struct St_GrP3SioMsgHd 
{
	St_GrTaskMsgHd	MsgHd;
	__u32	Cmd;
}	*Ptr_GrP3SioMsgHd;

typedef	struct St_GrP3SioMsgSetup 
{
	St_GrP3SioMsgHd	Hd;
	__u8	Model;
	__u8	Uart;
	__u8	Id;
	__u8	_rsvd0;
}	*Ptr_GrP3SioMsgSetup;

typedef struct St_GrP3SioMsgPiCmd 
{
	St_GrP3SioMsgHd	Hd;
	__u32	PiCmd;
	__u32	Para0;
	__u32	Para1;
}	*Ptr_GrP3SioMsgPiCmd;



//====================================================================
//class

class	Cls_GrP3SioMng	:	public	Cls_GrTaskCli
{
	private:
		Cls_GrTaskSvr*	m_TaskSvr;
		__u32		m_MsgAloc;
		__u32		m_HndlTimer;

		Cls_GrP3SioPiBase*	m_ObjSio;


	protected:
		//local function


		//event
		virtual	BOOL8	RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg );		/* return = message processed */
		virtual	void	RtlTimer( __u32 A_Hndl );
		virtual	BOOL8	RtlBkgProc( void );											/* return = background processed */

	public:
    Cls_GrP3SioMng( Cls_GrTaskSvr* A_TaskSvr );
    virtual		~Cls_GrP3SioMng();


		void	ModelNameGet( __u8 A_Model, WCHAR* A_StrRt );
		__u8	ModelCntGet( void );

		void	ImmSetup( __u8 A_Model, __u8 A_Uart, __u8 A_Id );
		void	ImmPiCmd( __u32 A_Cmd, __u32 A_Para0, __u32 A_Para1 );

		void	SafeSetup( __u8 A_Model, __u8 A_Uart, __u8 A_Id );
		void	SafePiCmd( __u32 A_Cmd, __u32 A_Para0, __u32 A_Para1 );

		// runtime
		virtual	void	RtlExtCmd( __u32 A_Cmd, void* A_PtrPara );

};
	
//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif


