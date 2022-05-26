/*
	SMTP Client


*/

#include	<GrSmtpCli.h>
#include	<GrStrTool.h>
#include	<GrProcess.h>
#include	<GrDumyTool.h>
#include	<GrTimeTool.h>
#include	<GrDebug.h>

#ifdef LINUX_APP

#include	<arpa/inet.h>

#endif

//--------------------------------------------------------------------
//local const
#define E_GrSmtpCliTimerPriod					1000

#define E_GrSmtpCliStageNone					0		/* stage none */
#define E_GrSmtpCliStageTryConn				1		/* stage try connect */
#define E_GrSmtpCliStageWaitConn			2		/* stage wait connect */
#define E_GrSmtpCliStageSendRecv			3		/* stage send */
#define E_GrSmtpCliStageFinish				4		/* stage finish */

#define E_GrSmtpCliMsgCmdSendMail			0		/* request send mail : St_GrSmtpCliMsgSendMail */

//--------------------------------------------------------------------
//local type

typedef	struct St_GrSmtpCliQueItm 
{
	Def_GrSock	Hsock;
	__u32	Sended;
	__u32	Size;
	__u32	Ip;
	__u16	Port;
	__u16	_rsvd;
}	*Ptr_GrSmtpCliQueItm;


typedef	struct St_GrSmtpCliMsgSendMail 
{
	St_GrTaskMsgHd	MsgHd;
	__u32	Cmd;

	__u32		Ip;
	__u16		Port;
	WCHAR*	StrFrom;
	WCHAR*	StrTo;
	WCHAR*	StrSubject;
	WCHAR*	StrContents;
	WCHAR*	StrId;
	WCHAR*	StrPass;

	BOOL8*		PtrRslt;

}	*Ptr_GrSmtpCliMsgSendMail;


//--------------------------------------------------------------------
//local macro

//--------------------------------------------------------------------
//local function

//--------------------------------------------------------------------
//global var

//====================================================================
//--------------------------------------------------------------------
Cls_GrSmtpCli::Cls_GrSmtpCli( Cls_GrTaskSvr* A_TaskSvr ):
Cls_GrTaskCli()
{
		m_TaskSvr		=	A_TaskSvr;
		m_MsgAloc		=	GrTaskSvrMsgCmdAlloc();

		GrSockInit();

		m_Stage				=	E_GrSmtpCliStageNone;
		m_TimeOutCnt	=	0;
		//init mail queue
		GrBufVqInit( (Ptr_GrBufVarQue)m_MailQue, E_GrSmtpCliMailQueSize, 4 );

		m_TaskSvr->RegistBkgClient( this );
		m_TaskSvr->RegistMsgClient( this );
		m_TaskSvr->RegistTimerClient( this, E_GrSmtpCliTimerPriod, &m_TimerSec );
}
//--------------------------------------------------------------------
Cls_GrSmtpCli::~Cls_GrSmtpCli( void )
{
	// local -------------------
		Ptr_GrSmtpCliQueItm	Tv_PtrItm;
	// code --------------------
		m_TaskSvr->UnregistBkgClient( this );
		m_TaskSvr->UnregistMsgClient( this );
		m_TaskSvr->UnregistTimerClient( m_TimerSec );

		GrTaskSvrMsgCmdFree( m_MsgAloc );

		//release socket
		Tv_PtrItm	=	(Ptr_GrSmtpCliQueItm)GrBufVqGetOutPtr( (Ptr_GrBufVarQue)m_MailQue );
		while ( NULL != Tv_PtrItm )
		{
			//close socket
			GrSockClose( Tv_PtrItm->Hsock ) ;
			//remove queue
			GrBufVqOut( (Ptr_GrBufVarQue)m_MailQue );
			//get next
			Tv_PtrItm	=	(Ptr_GrSmtpCliQueItm)GrBufVqGetOutPtr( (Ptr_GrBufVarQue)m_MailQue );
		}

		GrSockFinish();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrSmtpCli::RtlBkgProc( void )
{
	// local -------------------
	// code --------------------
		switch ( m_Stage )
		{
			case E_GrSmtpCliStageTryConn:
				LcStageTryConn();
				break;
			case E_GrSmtpCliStageWaitConn:
				LcStageWaitConn();
				break;
			case E_GrSmtpCliStageSendRecv:
				LcStageSendRecv();
				break;
			case E_GrSmtpCliStageFinish:
				LcStageFinish();
				break;
		}

		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrSmtpCli::RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( m_MsgAloc == A_PtrMsg->Cmd )
		{
			//check command
			if ( E_GrSmtpCliMsgCmdSendMail == ((Ptr_GrSmtpCliMsgSendMail)A_PtrMsg)->Cmd )
			{
				*(((Ptr_GrSmtpCliMsgSendMail)A_PtrMsg)->PtrRslt)	=	LcSendMail( ((Ptr_GrSmtpCliMsgSendMail)A_PtrMsg)->Ip,
					((Ptr_GrSmtpCliMsgSendMail)A_PtrMsg)->Port, ((Ptr_GrSmtpCliMsgSendMail)A_PtrMsg)->StrFrom,
					((Ptr_GrSmtpCliMsgSendMail)A_PtrMsg)->StrTo, ((Ptr_GrSmtpCliMsgSendMail)A_PtrMsg)->StrSubject,
					((Ptr_GrSmtpCliMsgSendMail)A_PtrMsg)->StrContents, ((Ptr_GrSmtpCliMsgSendMail)A_PtrMsg)->StrId,
					((Ptr_GrSmtpCliMsgSendMail)A_PtrMsg)->StrPass );
			}

			Tv_Result	=	TRUE;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrSmtpCli::LcSendMail( __u32 A_Ip, __u16 A_Port, WCHAR* A_StrFrom, WCHAR* A_StrTo, WCHAR* A_StrSubject,
								 WCHAR* A_StrContents, WCHAR* A_StrId, WCHAR* A_StrPass )
{
	// local -------------------
		BOOL8	Tv_Result;
		BOOL8	Tv_IsAuth;
		char*	Tv_StrWk;
		__u32	Tv_MailSize;
		Ptr_GrSmtpCliQueItm	Tv_PtrItm;
		void*	Tv_PtrSend;
	// code --------------------
		Tv_Result	=	FALSE;

		//make mail data
		//check password exist
		Tv_IsAuth		=	FALSE;
		if ( (NULL != A_StrId) && (NULL != A_StrPass) )
		{
			if ( GrStrIsExist(A_StrId) && GrStrIsExist(A_StrPass) )
			{
				Tv_IsAuth	=	TRUE;
			}
		}
		//make header
		if ( Tv_IsAuth )
		{
			Tv_StrWk	=	GrStrCopy( m_StrMail, "ehlo GAUSE.RESERCH\r\n" );
			Tv_StrWk	=	GrStrCopy( Tv_StrWk, "auth LOGIN\r\n" );
			//Tv_StrWk	=	GrStrCopy( Tv_StrWk, "auth LOGIN\r\n" );
			//id
			GrStrWstrToStr( m_StrTmp, A_StrId );
			GrStrBase64Enc( m_StrTmp, Tv_StrWk );
			Tv_StrWk	=	GrStrCat( Tv_StrWk, "\r\n" );
			//pass
			GrStrWstrToStr( m_StrTmp, A_StrPass );
			GrStrBase64Enc( m_StrTmp, Tv_StrWk );
			Tv_StrWk	=	GrStrCat( Tv_StrWk, "\r\n" );
		}
		else
		{
			Tv_StrWk	=	GrStrCopy( m_StrMail, "HELO gause.com\r\n" );
		}

		//from address
		Tv_StrWk	=	GrStrCopy( Tv_StrWk, "MAIL FROM: <" );
		GrStrWstrToStr( Tv_StrWk, A_StrFrom );
		Tv_StrWk	=	GrStrCat( Tv_StrWk, ">\r\n" );
		//to address
		Tv_StrWk	=	GrStrCopy( Tv_StrWk, "RCPT TO: <" );
		GrStrWstrToStr( Tv_StrWk, A_StrTo );
		Tv_StrWk	=	GrStrCat( Tv_StrWk, ">\r\n" );
		//start data
		Tv_StrWk	=	GrStrCopy( Tv_StrWk, "DATA\r\n" );
		//subject
		Tv_StrWk	=	GrStrCopy( Tv_StrWk, "Subject: " );
		if ( GrStrIsExist(A_StrSubject) )
		{
			Tv_StrWk	=	GrStrCopy( Tv_StrWk, "=?utf-8?B?" );
			GrStrWstrToUtf8( m_StrTmp, A_StrSubject );
			GrStrBase64Enc( m_StrTmp, Tv_StrWk );
			Tv_StrWk	=	GrStrCat( Tv_StrWk, "?= " );
		}
		Tv_StrWk	=	GrStrCopy( Tv_StrWk, "\r\n" );
		//date from address
		Tv_StrWk	=	GrStrCopy( Tv_StrWk, "FROM: " );
		GrStrWstrToStr( Tv_StrWk, A_StrFrom );
		Tv_StrWk	=	GrStrCat( Tv_StrWk, "\r\n" );
		//data to address
		Tv_StrWk	=	GrStrCopy( Tv_StrWk, "TO: " );
		GrStrWstrToStr( Tv_StrWk, A_StrTo );
		Tv_StrWk	=	GrStrCat( Tv_StrWk, "\r\n" );
		//MIME mark
		Tv_StrWk	=	GrStrCopy( Tv_StrWk, "MIME-Version: 1.0\r\n" );
		Tv_StrWk	=	GrStrCopy( Tv_StrWk, "Content-Type: text/plain; charset=utf-8\r\n" );
		Tv_StrWk	=	GrStrCopy( Tv_StrWk, "Content-Transfer-Encoding: base64\r\n" );
		//contents
		Tv_StrWk	=	GrStrCopy( Tv_StrWk, "\r\n" );
		GrStrWstrToUtf8( m_StrTmp, A_StrContents );
		GrStrBase64Enc( m_StrTmp, Tv_StrWk );
		//quit
		Tv_StrWk	=	GrStrCat( Tv_StrWk, "\r\n" );
		Tv_StrWk	=	GrStrCopy( Tv_StrWk, ".\r\n" );
		Tv_StrWk	=	GrStrCopy( Tv_StrWk, "QUIT\r\n" );

		//get data size
		Tv_MailSize	=	GrStrLen( m_StrMail );

		//allocate mail buffer
		Tv_PtrItm	=	(Ptr_GrSmtpCliQueItm)GrBufVqGetInPtr( (Ptr_GrBufVarQue)m_MailQue, Tv_MailSize + sizeof(St_GrSmtpCliQueItm) );
		if ( NULL != Tv_PtrItm )
		{
			//init
			Tv_PtrItm->Sended	=	0;
			Tv_PtrItm->Size		=	Tv_MailSize;
			Tv_PtrItm->Ip			=	A_Ip;
			Tv_PtrItm->Port		=	A_Port;
			//make socket
			Tv_PtrItm->Hsock	=	GrSockOpen( FALSE, TRUE );
			if ( E_GrSockBadHandle != Tv_PtrItm->Hsock )
			{
				//make contents
				Tv_PtrSend	=	(void*)( (__u32)Tv_PtrItm + sizeof(St_GrSmtpCliQueItm) );
				GrDumyCopyMem( Tv_PtrSend, m_StrMail, Tv_MailSize );
				//allocate buffer
				GrBufVqIn( (Ptr_GrBufVarQue)m_MailQue );
				//set stage
				if ( E_GrSmtpCliStageNone == m_Stage )
				{
					//set try connect stage
					m_Stage				=	E_GrSmtpCliStageTryConn;
					m_TimeOutCnt	=	10;		//connection timeout
					Tv_Result			=	TRUE;
				}
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrSmtpCli::LcSendMail - not enough vq!\n");
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrSmtpCli::SendMail( __u32 A_Ip, __u16 A_Port, WCHAR* A_StrFrom, WCHAR* A_StrTo, WCHAR* A_StrSubject,
							 WCHAR* A_StrContents, WCHAR* A_StrId, WCHAR* A_StrPass )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrSmtpCliMsgSendMail	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.MsgHd.Cmd	=	m_MsgAloc;	
		Tv_Msg.Cmd				=	E_GrSmtpCliMsgCmdSendMail;
		Tv_Msg.Ip					=	A_Ip;
		Tv_Msg.Port				=	A_Port;
		Tv_Msg.StrFrom		=	A_StrFrom;
		Tv_Msg.StrTo			=	A_StrTo;
		Tv_Msg.StrSubject	=	A_StrSubject;
		Tv_Msg.StrContents	=	A_StrContents;
		Tv_Msg.StrId			=	A_StrId;
		Tv_Msg.StrPass		=	A_StrPass;
		Tv_Msg.PtrRslt		=	&Tv_Result;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrSmtpCli::LcStageTryConn( void )
{
	// local -------------------
		Ptr_GrSmtpCliQueItm	Tv_PtrItm;
		__u8	Tv_Rval;
	// code --------------------
		//check exist item 
		Tv_PtrItm	=	(Ptr_GrSmtpCliQueItm)GrBufVqGetOutPtr( (Ptr_GrBufVarQue)m_MailQue );
		if ( NULL != Tv_PtrItm )
		{
			Tv_Rval	=	GrSockConnect( Tv_PtrItm->Hsock, Tv_PtrItm->Ip, Tv_PtrItm->Port );
			if ( E_GrSockConnOk == Tv_Rval )
			{
				m_Stage				=	E_GrSmtpCliStageSendRecv;
				m_TimeOutCnt	=	120;		//2minute
			}
			else if ( E_GrSockConnWaiting != Tv_Rval )
			{
				//mail error
				DbgMsgPrint( "Cls_GrSmtpCli::LcStageTryConn - smtp server connect error.\n" );
				//go finish
				m_Stage				=	E_GrSmtpCliStageFinish;
				m_TimeOutCnt	=	10;			//default timeout
			}
			else
			{
				// connect wait
				m_Stage				=	E_GrSmtpCliStageWaitConn;
				m_TimeOutCnt	=	20;		// connect wait tome 20 sec
			}
		}
		else
		{
			// no item
			m_Stage	=	E_GrSmtpCliStageNone;
		}
}
//--------------------------------------------------------------------
void	Cls_GrSmtpCli::LcStageWaitConn( void )
{
	// local -------------------
		Ptr_GrSmtpCliQueItm	Tv_PtrItm;
		__u8	Tv_Rval;
	// code --------------------
		//check exist item 
		Tv_PtrItm	=	(Ptr_GrSmtpCliQueItm)GrBufVqGetOutPtr( (Ptr_GrBufVarQue)m_MailQue );
		if ( NULL != Tv_PtrItm )
		{
			Tv_Rval	=	GrSockWaitConnect( Tv_PtrItm->Hsock, 0 );
			if ( E_GrSockConnOk == Tv_Rval )
			{
				// go next
				m_Stage				=	E_GrSmtpCliStageSendRecv;
				m_TimeOutCnt	=	120;		//2minute
			}
			else if ( E_GrSockConnWaiting != Tv_Rval )
			{
				//mail error
				DbgMsgPrint( "Cls_GrSmtpCli::LcStageWaitConn - smtp server connect error.\n" );
				//go finish
				m_Stage				=	E_GrSmtpCliStageFinish;
				m_TimeOutCnt	=	10;			//default timeout
			}
		}
		else
		{
			// no item
			m_Stage	=	E_GrSmtpCliStageNone;
		}
}
//--------------------------------------------------------------------
void	Cls_GrSmtpCli::LcStageSendRecv( void )
{
	// local -------------------
		Ptr_GrSmtpCliQueItm	Tv_PtrItm;
		void*	Tv_PtrSend;
		__s32		Tv_Sended;
		__s32		Tv_Recved;
		Def_StrName	Tv_StrRecv;
	// code --------------------
		//check able out 
		Tv_PtrItm	=	(Ptr_GrSmtpCliQueItm)GrBufVqGetOutPtr( (Ptr_GrBufVarQue)m_MailQue );
		if ( NULL != Tv_PtrItm )
		{
			//check exist send able
			if ( Tv_PtrItm->Sended < Tv_PtrItm->Size )
			{
				//try send
				Tv_PtrSend	=	(void*)( (__u32)Tv_PtrItm + sizeof(St_GrSmtpCliQueItm) + Tv_PtrItm->Sended );
				Tv_Sended	=	GrSockSend( Tv_PtrItm->Hsock, Tv_PtrSend, Tv_PtrItm->Size - Tv_PtrItm->Sended );
				if ( 0 < Tv_Sended )
				{
					Tv_PtrItm->Sended	=	Tv_PtrItm->Sended + Tv_Sended;
				}
				else
				{
					//failed, quit
					if ( E_GrSockErrWouldblock != GrSockGetErrCode() )
					{
						//socket fail
						//go finish
						m_Stage				=	E_GrSmtpCliStageFinish;
						m_TimeOutCnt	=	10;			//default timeout
					}
				}
			}

			//try recv
			Tv_Recved	=	GrSockRecv( Tv_PtrItm->Hsock, Tv_StrRecv, GrStrMaxLen(Tv_StrRecv) );
			if ( 0 >= Tv_Recved )
			{
				if ( GrSockChkCloseByRecv( Tv_Recved ) )
				{
					// go to finish
					m_Stage				=	E_GrSmtpCliStageFinish;
					m_TimeOutCnt	=	10;			//default timeout
				}
			}
#ifndef LINUX_APP
			else
			{
				Tv_StrRecv[Tv_Recved]	=	0;
				DbgMsgPrint( "MailMsg - %s\n", Tv_StrRecv );
			}
#endif
		}
		else
		{
			// no item
			m_Stage	=	E_GrSmtpCliStageNone;
		}
}
//--------------------------------------------------------------------
void	Cls_GrSmtpCli::LcStageFinish( void )
{
	// local -------------------
		Ptr_GrSmtpCliQueItm	Tv_PtrItm;
	// code --------------------
		Tv_PtrItm	=	(Ptr_GrSmtpCliQueItm)GrBufVqGetOutPtr( (Ptr_GrBufVarQue)m_MailQue );
		if ( NULL != Tv_PtrItm )
		{
			//close socket
			GrSockClose( Tv_PtrItm->Hsock );
			//release buffer
			GrBufVqOut( (Ptr_GrBufVarQue)m_MailQue );
			//go next stage
			m_Stage				=	E_GrSmtpCliStageTryConn;
			m_TimeOutCnt	=	10;
		}
		else
		{
			// no item
			m_Stage	=	E_GrSmtpCliStageNone;
		}
}
//--------------------------------------------------------------------
void	Cls_GrSmtpCli::RtlTimer( __u32 A_Hndl )
{
	// local -------------------
	// code --------------------
		if ( E_GrSmtpCliStageNone != m_Stage )
		{
			if ( 0 == m_TimeOutCnt )
			{
				//timeout
				DbgMsgPrint( "Cls_GrSmtpCli::RtlTimer - timeout. stage = %d.\n", m_Stage );
				//go finish
				m_Stage				=	E_GrSmtpCliStageFinish;
				m_TimeOutCnt	=	10;			//default timeout
			}
			else
			{
				m_TimeOutCnt --;
			}
		}
}
//--------------------------------------------------------------------

