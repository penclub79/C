/*
	gause SMTP client

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrSmtpCli
#define	_EPC_GrSmtpCli

//====================================================================
// uses

#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<GrTaskSvr.h>
#include	<GrTaskCli.h>
#include	<GrSocket.h>
#include	<GrBufTool.h>

//====================================================================
//const

#define E_GrSmtpCliMailContSize		0x1000
#define E_GrSmtpCliMailMaxSize		0x2000
#define E_GrSmtpCliMailQueSize		0x10000

//====================================================================
//type


//====================================================================
//class
class Cls_GrSmtpCli	:	public Cls_GrTaskCli
{
	private:
		Cls_GrTaskSvr*	m_TaskSvr;
		__u32			m_MsgAloc;
		__u32			m_TimerSec;


		__u8	m_Stage;			//stage
		__u32	m_TimeOutCnt;	//time out count

		char	m_MailQue[E_GrSmtpCliMailQueSize];
		char	m_StrTmp[E_GrSmtpCliMailContSize];
		char	m_StrMail[E_GrSmtpCliMailMaxSize];

	protected:
		//local
		BOOL8	LcSendMail( __u32 A_Ip, __u16 A_Port, WCHAR* A_StrFrom, WCHAR* A_StrTo, WCHAR* A_StrSubject,
			WCHAR* A_StrContents, WCHAR* A_StrId, WCHAR* A_StrPass );

		void	LcStageTryConn( void );
		void	LcStageWaitConn( void );
		void	LcStageSendRecv( void );
		void	LcStageFinish( void );

		//inherited
		virtual	BOOL8	RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg );		/* return = message processed */
		virtual	BOOL8	RtlBkgProc( void );												/* return = background processed */
		virtual	void	RtlTimer( __u32 A_Hndl );
	public:
		Cls_GrSmtpCli( Cls_GrTaskSvr* A_TaskSvr );
		virtual	~Cls_GrSmtpCli( void );

		BOOL8	SendMail( __u32 A_Ip, __u16 A_Port, WCHAR* A_StrFrom, WCHAR* A_StrTo, WCHAR* A_StrSubject,
			WCHAR* A_StrContents, WCHAR* A_StrId = NULL, WCHAR* A_StrPass = NULL );

};

//====================================================================
//function


//====================================================================
//out Enviroment conditional
#endif
