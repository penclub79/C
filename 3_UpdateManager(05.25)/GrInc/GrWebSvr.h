/*
	web server

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrWebSvr
#define	_EPC_GrWebSvr

//====================================================================
// uses

#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<GrTaskBase.h>
#include	<GrTaskSvr.h>
#include	<GrSocket.h>
#include	<GrFileCtrl.h>

//====================================================================
//const

#define E_GrWebSvrMaxConn				32
#define E_GrWebSvrContBufSize		1024

//====================================================================
//type

typedef	struct St_GrWebSvrConn 
{
	Def_GrSock	Sock;		//socket
	__u32				BufSize;
	__u32				BufSended;
	BOOL8				IsSendMode;
	__u8	BufCont[E_GrWebSvrContBufSize];		//contents buffer
	char	StrPars[E_GrWebSvrContBufSize];		//parsing charactor
	char	StrFn[E_GrWebSvrContBufSize];			//file name
	Cls_GrFileCtrl*	Fc;
	__u32	FlReside;			//file reside
}	*Ptr_GrWebSvrConn;

//====================================================================
//class
class Cls_GrWebSvr	:	public	Cls_GrTaskCli
{
	private:
		Cls_GrTaskSvr*	m_TaskSvr;
		__u32			m_MsgAloc;

		Def_StrGrFnPathShort	m_StrPath;

		__u16			m_Port;
		Def_GrSock			m_SockListen;

		__u32			m_ConnCnt;
		St_GrWebSvrConn	m_ConnTbl[E_GrWebSvrMaxConn];
	protected:
		//local function
		void	LcSetupPort( __u16 A_Port );
		void	LcListenSockUpdt( void );
		void	LcConnRecv( __u8 A_ConnIdx );
		void	LcConnClose( __u8 A_ConnIdx );
		BOOL8	LcConnChkCmdRecved( __u8 A_ConnIdx );
		__u8	LcConnParsCmd(__u8 A_ConnIdx, char** A_PtrNext);
		void	LcConnParsStrToFn( __u8 A_ConnIdx );
		BOOL8	LcConnModeSetCmdGet( __u8 A_ConnIdx );
		void	LcConnSend( __u8 A_ConnIdx );
		BOOL8	LcAccept( void );

		//inherited
		virtual	BOOL8	RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg );		/* return = message processed */
		virtual	BOOL8	RtlBkgProc( void );											/* return = background processed */

	public:
		Cls_GrWebSvr(Cls_GrTaskSvr* A_TaskSvr, char* A_StrPath);
		virtual	~Cls_GrWebSvr( void );

		void	SetupPort( __u16 A_Port, BOOL8 A_IsNow = TRUE );			//change port


};


//====================================================================
//function


//====================================================================
//out Enviroment conditional
#endif
