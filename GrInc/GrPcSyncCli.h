/*
	gause NTP client

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrPcSyncCli
#define	_EPC_GrPcSyncCli

//====================================================================
// uses

#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<GrTaskSvr.h>
#include	<GrTaskCli.h>
#include	<GrSocket.h>

//====================================================================
//const
#define E_GrPcSyncCliCbCmdTimeGet	Mac_GrMakeFcc('P','C','S','G')

#define E_GrPcSyncCliProtsCnt	10			//protocol count
//====================================================================
//type
typedef	struct St_GrPcSyncCliCbMsgTimeGet
{
	St_GrTaskMsgHd	Hd;
	__u32	Para;
	Def_GrTime	SysTime;
	BOOL8	IsSuccess;
}	*Ptr_GrPcSyncCliCbMsgTimeGet;

typedef struct St_GrPcSyncCliProt
{
	__s32			ProtType;			//protocol type
	char*		ProtName;			//protocol name
} *Ptr_Prots;

//====================================================================
//class
class Cls_GrPcSyncCli	:	public Cls_GrTaskCli
{
	private:
		Cls_GrTaskSvr*	m_TaskSvr;
		__u32			m_MsgAloc;
		Cls_GrTaskCli*	m_CbCli;		//event client

		__u32			m_CbPara;					//event parameter
		Def_GrSock	m_HsockSend;
		__u32			m_Ip;
		__u32			m_TickTmOut;
		__u32			m_TickLast;
		__u8			m_Buf[1024];
		__u32			m_Sended;
		St_GrPcSyncCliProt			m_Prots[E_GrPcSyncCliProtsCnt];

	protected:
		//local
		BOOL8	LcProcMsgReqTime( __u32 A_Ip, Cls_GrTaskCli* A_CbObj, __u32 A_Para, __u32 A_TickTimeOut );
		BOOL8	LcBgpTimeRecv( void );
		void	LcMakePacket( PBYTE A_Pkt );
		Def_GrTime	LcGetPacketToTime( PBYTE A_PtrBuf );

		//inherited
		virtual	BOOL8	RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg );		/* return = message processed */
		virtual	BOOL8	RtlBkgProc( void );												/* return = background processed */

	public:
		Cls_GrPcSyncCli( Cls_GrTaskSvr* A_TaskSvr );
		virtual	~Cls_GrPcSyncCli( void );

		BOOL8	ReqTimeGet( __u32 A_Ip, Cls_GrTaskCli* A_CbObj, __u32 A_Para, __u32 A_TimeOutMs = 8000, __u16 A_Port = 123 );
};

//====================================================================
//function


//====================================================================
//out Enviroment conditional
#endif
