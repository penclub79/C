/*
 GAUSE Network SMTP plug-in

 */

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <Net/GrNetPlugSmtp.h>
#include <Net/GrNetSvs.h>

#include <GrDebug.h>
#include <GrDumyTool.h>
#include <GrTimeTool.h>
#include <GrStrTool.h>
#include <GrBufTool.h>

#include	<stdlib.h>

#include <GrSockV2.h>

//====================================================================
//local const

//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var

//====================================================================
//--------------------------------------------------------------------
Cls_GrNetPlugSmtp::Cls_GrNetPlugSmtp(Cls_GrTaskSvr* A_TaskSvr, Cls_GrTaskCli* A_NetSvs, __u8 A_DevIdx, __u8 A_PlugIdx) :
Cls_GrNetPlugBase(A_TaskSvr, A_NetSvs, A_DevIdx, A_PlugIdx)
{
	// local -------------------
	// code --------------------
		// init
		m_SvIp		=	0;
		m_DnsRstTimer	=	E_GrNetPlugSmtpDnsUpdtTime;
		m_HndlDns	=	-1;
		m_IsUseSSL	= FALSE;
		GrStrClear( m_StrSvName );
#ifndef E_GrNetPlugSmtpSslNot
#ifdef	LINUX_APP
		m_PtrCtx		= NULL;
		m_PtrSsl		= NULL;
#endif
#endif
		GrDumyZeroMem(m_StrMail, sizeof(m_StrMail));

		GrBufVqInit( (Ptr_GrBufVarQue)m_MailQue, E_GrNetPlugSmtpQueSize, 4 );

		m_TaskSvr->RegistTimerClient(this, E_GrNetPlugSmtpTimerPeriod, &m_HndlTimer);
		m_TaskSvr->RegistBkgClient(this);
}
//--------------------------------------------------------------------
Cls_GrNetPlugSmtp::~Cls_GrNetPlugSmtp()
{
	// local -------------------
	// code --------------------

		m_TaskSvr->UnregistBkgClient(this);
		m_TaskSvr->UnregistTimerClient(m_HndlTimer);

}
//--------------------------------------------------------------------
BOOL8	Cls_GrNetPlugSmtp::RtlBkgProc( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		// DNS
		if ( LcBgpDns() )
		{
			Tv_Result	=	TRUE;
		}
		
		// send and receive mail
		if ( LcBgpItem() )
		{
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrNetPlugSmtp::RtlTimer(__u32 A_Hndl)
{
	// local -------------------
		Ptr_GrNetPlugSmtpItemHd	Tv_PtrItem;
	// code --------------------
		if ( 0 == m_SvIp )
		{
			// check server IP
			if ( (GrStrIsExist(m_StrSvName)) && (0 > m_HndlDns) )
			{
				// request DNS
				m_HndlDns	=	((Cls_GrNetSvs*)m_NetSvs)->ImmDnsReq( m_StrSvName, 1, FALSE );
			}
		}
		else
		{
			if ( 0 == m_DnsRstTimer )
			{
				m_DnsRstTimer	=	E_GrNetPlugSmtpDnsUpdtTime;
				m_SvIp	=	0;		// server ip reset
			}
			else
			{
				m_DnsRstTimer --;
			}

			Tv_PtrItem	=	(Ptr_GrNetPlugSmtpItemHd)GrBufVqGetOutPtr( (Ptr_GrBufVarQue)m_MailQue );

			if(NULL != Tv_PtrItem)
			{
				if(0 == Tv_PtrItem->TimeOut)
				{
					DbgMsgPrint("Cls_GrNetPlugSmtp::RtlTimer - smtp timeout\n");
					Tv_PtrItem->Stage = E_GrNetPlugSmtpStageFinish;
					Tv_PtrItem->TimeOut		= 10;
				}
				else
				{
					Tv_PtrItem->TimeOut--;
				}
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrNetPlugSmtp::SmtpRequest(WCHAR* A_StrFrom, WCHAR* A_StrTo,
	WCHAR* A_StrSubject, WCHAR*	A_StrCont, void* A_PtrImg, __u32 A_ImgSize)
{
	// local -------------------
		BOOL8	Tv_Result;
		char*	Tv_StrWk;
		__u32	Tv_MailSize;
		Ptr_GrNetPlugSmtpItemHd	Tv_PtrItm;
		void*	Tv_PtrData;
		char*	Tv_PtrStr;
		__u32	Tv_WkCnt;
		St_GrNetPlugSmtpStageInfo Tv_Info[E_GrNetPlugSmtpMailTblCnt];
	// code --------------------
		Tv_Result	=	FALSE;
		GrDumyZeroMem(Tv_Info, sizeof(St_GrNetPlugSmtpStageInfo) * E_GrNetPlugSmtpMailTblCnt);
		Tv_WkCnt = 0;

		// check able
		if ( 0 != m_SvIp )
		{
			// make head
			if ( m_IsAuth )
			{
				Tv_StrWk	=	GrStrCopy( m_StrMail, "ehlo GAUSE.RESERCH\r\n" );
				Tv_Info[Tv_WkCnt].Size		= (__u32)(Tv_StrWk - m_StrMail);//0
				Tv_Info[Tv_WkCnt].Pos			= 0;
				Tv_PtrStr = Tv_StrWk;
				Tv_WkCnt++;

				//login
				Tv_StrWk	=	GrStrCopy( Tv_StrWk, "auth LOGIN\r\n" );
				Tv_Info[Tv_WkCnt].Size		= (__u32)(Tv_StrWk - Tv_PtrStr);//1
				Tv_Info[Tv_WkCnt].Pos			= Tv_Info[Tv_WkCnt - 1].Pos + Tv_Info[Tv_WkCnt - 1].Size;
				Tv_PtrStr = Tv_StrWk;
				Tv_WkCnt++;

				//id
				Tv_StrWk	=	GrStrCopy( Tv_StrWk, m_StrSvId );
				Tv_StrWk	=	GrStrCopy( Tv_StrWk, "\r\n" );
				Tv_Info[Tv_WkCnt].Size		= (__u32)(Tv_StrWk - Tv_PtrStr);//1
				Tv_Info[Tv_WkCnt].Pos			= Tv_Info[Tv_WkCnt - 1].Pos + Tv_Info[Tv_WkCnt - 1].Size;
				Tv_PtrStr = Tv_StrWk;
				Tv_WkCnt++;

				//pass
				Tv_StrWk	=	GrStrCopy( Tv_StrWk, m_StrSvPass );
				Tv_StrWk	=	GrStrCopy( Tv_StrWk, "\r\n" );
				Tv_Info[Tv_WkCnt].Size		= (__u32)(Tv_StrWk - Tv_PtrStr);//2
				Tv_Info[Tv_WkCnt].Pos			= Tv_Info[Tv_WkCnt - 1].Pos + Tv_Info[Tv_WkCnt - 1].Size;
				Tv_PtrStr = Tv_StrWk;
				Tv_WkCnt++;
			}
			else
			{
				Tv_StrWk	=	GrStrCopy( m_StrMail, "HELO gause.com\r\n" );
			}
			//from address
			Tv_StrWk	=	GrStrCopy( Tv_StrWk, "MAIL FROM: <" );
			GrStrWstrToStr( Tv_StrWk, A_StrFrom );
			Tv_StrWk	=	GrStrCat( Tv_StrWk, ">\r\n" );

			Tv_Info[Tv_WkCnt].Size		= GrStrLen(Tv_PtrStr);
			Tv_Info[Tv_WkCnt].Pos			= Tv_Info[Tv_WkCnt - 1].Pos + Tv_Info[Tv_WkCnt - 1].Size;
			Tv_PtrStr = Tv_StrWk;
			Tv_WkCnt++;
			//to address
			Tv_StrWk	=	GrStrCopy( Tv_StrWk, "RCPT TO: <" );
			GrStrWstrToStr( Tv_StrWk, A_StrTo );
			Tv_StrWk	=	GrStrCat( Tv_StrWk, ">\r\n" );

			Tv_Info[Tv_WkCnt].Size		= GrStrLen(Tv_PtrStr);
			Tv_Info[Tv_WkCnt].Pos			= Tv_Info[Tv_WkCnt - 1].Pos + Tv_Info[Tv_WkCnt - 1].Size;
			Tv_PtrStr = Tv_StrWk;
			Tv_WkCnt++;
			//start data
			Tv_StrWk	=	GrStrCopy( Tv_StrWk, "DATA\r\n" );
			Tv_Info[Tv_WkCnt].Size		= (__u32)(Tv_StrWk - Tv_PtrStr);//5
			Tv_Info[Tv_WkCnt].Pos			= Tv_Info[Tv_WkCnt - 1].Pos + Tv_Info[Tv_WkCnt - 1].Size;
			Tv_PtrStr = Tv_StrWk;
			Tv_WkCnt++;
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
			GrStrWstrToUtf8( m_StrTmp, A_StrCont );
			GrStrBase64Enc( m_StrTmp, Tv_StrWk );
			//quit
			Tv_StrWk	=	GrStrCat( Tv_StrWk, "\r\n" );
			Tv_StrWk	=	GrStrCopy( Tv_StrWk, ".\r\n" );
			Tv_Info[Tv_WkCnt].Size		= (__u32)(Tv_StrWk - Tv_PtrStr);//6
			Tv_Info[Tv_WkCnt].Pos			= Tv_Info[Tv_WkCnt - 1].Pos + Tv_Info[Tv_WkCnt - 1].Size;
			Tv_PtrStr = Tv_StrWk;
			Tv_WkCnt++;

			Tv_StrWk	=	GrStrCopy( Tv_StrWk, "QUIT\r\n" );
			Tv_Info[Tv_WkCnt].Size		= (__u32)(Tv_StrWk - Tv_PtrStr);//7
			Tv_Info[Tv_WkCnt].Pos			= Tv_Info[Tv_WkCnt - 1].Pos + Tv_Info[Tv_WkCnt - 1].Size;
			Tv_PtrStr = Tv_StrWk;
			Tv_WkCnt++;

			//get data size
			Tv_MailSize	=	GrStrLen( m_StrMail );

			//allocate mail buffer
			Tv_PtrItm	=	(Ptr_GrNetPlugSmtpItemHd)GrBufVqGetInPtr( (Ptr_GrBufVarQue)m_MailQue, Tv_MailSize + sizeof(St_GrNetPlugSmtpItemHd) );
			if ( NULL != Tv_PtrItm )
			{
				//init
				Tv_PtrItm->Sent		=	0;
				Tv_PtrItm->Size		=	Tv_MailSize;
				//make socket
				Tv_PtrItm->HndlSock	=	GrSockV2Open( FALSE, TRUE, FALSE );
				if ( E_GrSockBadHandle != Tv_PtrItm->HndlSock )
				{
					//stage info
					GrDumyCopyMem(Tv_PtrItm->Info, Tv_Info, sizeof(St_GrNetPlugSmtpStageInfo) * E_GrNetPlugSmtpMailTblCnt);
					//make contents
					Tv_PtrData	=	(void*)( (__u32)Tv_PtrItm + sizeof(St_GrNetPlugSmtpItemHd) );
					GrDumyCopyMem( Tv_PtrData, m_StrMail, Tv_MailSize );
					//set stage
					Tv_PtrItm->Stage		=	E_GrNetPlugSmtpStageTryConn;
					Tv_PtrItm->TimeOut	=	10;
					Tv_Result	=	TRUE;
					//allocate buffer
					GrBufVqIn( (Ptr_GrBufVarQue)m_MailQue );
				}
				else
				{
					DbgMsgPrint("Cls_GrSmtpCli::SmtpRequest - socket open fail!\n");
				}
			}
			else
			{
				DbgMsgPrint("Cls_GrSmtpCli::SmtpRequest - not enough vq!\n");
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrNetPlugSmtp::SmtpRequest - mail que ptr get fail!\n");
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrNetPlugSmtp::LcBgpItem( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_Rval;
		Ptr_GrNetPlugSmtpItemHd	Tv_PtrItem;
		void*	Tv_PtrSend;
		__s32		Tv_Sent;
		__s32		Tv_Recved;
		Def_StrName	Tv_StrRecv;	
	// code --------------------
		Tv_Result	=	FALSE;
		
		if(!m_IsUseSSL)
		{
			// check exist item
			Tv_PtrItem	=	(Ptr_GrNetPlugSmtpItemHd)GrBufVqGetOutPtr( (Ptr_GrBufVarQue)m_MailQue );
			if ( NULL != Tv_PtrItem )
			{
				// check stage
				Tv_Result	=	TRUE;
				switch ( Tv_PtrItem->Stage )
				{
					case E_GrNetPlugSmtpStageTryConn:
						// try connect
						Tv_Rval	=	GrSockV2Connect( Tv_PtrItem->HndlSock, &m_SvIp, m_SvPort, FALSE );
						if ( E_GrSockV2ConnOk == Tv_Rval )
						{
							Tv_PtrItem->Stage		=	E_GrNetPlugSmtpStageIo;
							Tv_PtrItem->TimeOut	=	90;
						}
						else if ( E_GrSockV2ConnWaiting != Tv_Rval )
						{
							DbgMsgPrint( "Cls_GrNetPlugSmtp::LcBgpItem - SMTP server connect fail!\n" );
							Tv_PtrItem->Stage		=	E_GrNetPlugSmtpStageFinish;
							Tv_PtrItem->TimeOut	=	10;
						}
						else
						{
							Tv_PtrItem->Stage		=	E_GrNetPlugSmtpStageWaitConn;
							Tv_PtrItem->TimeOut	=	20;
							Tv_Result	=	FALSE;
						}
						break;
					case E_GrNetPlugSmtpStageWaitConn:
						Tv_Rval	=	GrSockV2WaitConnect( Tv_PtrItem->HndlSock, 0 );
						if ( E_GrSockV2ConnOk == Tv_Rval )
						{
							// go next
							Tv_PtrItem->Stage		=	E_GrNetPlugSmtpStageIo;
							Tv_PtrItem->TimeOut	=	90;
						}
						else if ( E_GrSockV2ConnWaiting != Tv_Rval )
						{
							//mail error
							DbgMsgPrint( "Cls_GrNetPlugSmtp::LcBgpItem - smtp server connect error.\n" );
							//go finish
							Tv_PtrItem->Stage		=	E_GrNetPlugSmtpStageFinish;
							Tv_PtrItem->TimeOut	=	10;
						}
						else
						{
							Tv_Result	=	FALSE;
						}
						break;
					case E_GrNetPlugSmtpStageIo:
						Tv_Result	=	FALSE;
						//check exist send able
						if ( Tv_PtrItem->Sent < Tv_PtrItem->Size )
						{
							//try send
							Tv_PtrSend	=	(void*)( (__u32)Tv_PtrItem + sizeof(St_GrNetPlugSmtpItemHd) + Tv_PtrItem->Sent );
							Tv_Sent			=	GrSockV2Send( Tv_PtrItem->HndlSock, Tv_PtrSend, Tv_PtrItem->Size - Tv_PtrItem->Sent );
							if ( 0 < Tv_Sent )
							{
								Tv_PtrItem->Sent	=	Tv_PtrItem->Sent + Tv_Sent;
							}
							else
							{
								//failed, quit
								if ( E_GrSockErrWouldblock != GrSockV2GetErrCode() )
								{
									//socket fail
									//go finish
									Tv_PtrItem->Stage		=	E_GrNetPlugSmtpStageFinish;
									Tv_PtrItem->TimeOut	=	10;
									Tv_Result	=	TRUE;
								}
							}
						}

						//try recv
						Tv_Recved	=	GrSockV2Recv( Tv_PtrItem->HndlSock, Tv_StrRecv, GrStrMaxLen(Tv_StrRecv) );
						if ( 0 >= Tv_Recved )
						{
							if ( GrSockV2ChkCloseByRecv( Tv_Recved ) )
							{
								// go to finish
								Tv_PtrItem->Stage		=	E_GrNetPlugSmtpStageFinish;
								Tv_PtrItem->TimeOut	=	10;
								Tv_Result	=	TRUE;
							}
						}
						break;
					case E_GrNetPlugSmtpStageFinish:
						//close socket
						GrSockV2Close( Tv_PtrItem->HndlSock );
						//release buffer
						GrBufVqOut( (Ptr_GrBufVarQue)m_MailQue );
						break;
					default:
						// bad stage
						DbgMsgPrint( "Cls_GrNetPlugSmtp::LcBgpItem - bad stage!\n" );
						GrBufVqOut( (Ptr_GrBufVarQue)m_MailQue );
						break;
				}
			}
		}
		else
		{
			Tv_Result = LcProcSsl();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrNetPlugSmtp::SmtpSetup(WCHAR* A_StrSvName, __u16 A_SvPort, WCHAR* A_StrId, WCHAR* A_StrPass, BOOL8 A_IsUseSSL)
{
	// local -------------------
		Def_StrTag	Tv_StrTxt;
	// code --------------------
		// copy server name
		GrStrWstrToStr(m_StrSvName, A_StrSvName);
		m_SvIp		=	0;
		m_SvPort	=	A_SvPort;

		m_IsAuth	=	FALSE;
		GrStrClear( m_StrSvId );
		GrStrClear( m_StrSvPass );
		if (GrStrIsExist(A_StrId) || GrStrIsExist(A_StrPass))
		{
			m_IsAuth	=	TRUE;
			GrStrWstrToStr(Tv_StrTxt, A_StrId);
			GrStrBase64Enc( Tv_StrTxt, m_StrSvId );
			GrStrWstrToStr(Tv_StrTxt, A_StrPass);
			GrStrBase64Enc(Tv_StrTxt, m_StrSvPass);
		}

		m_IsUseSSL = A_IsUseSSL;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrNetPlugSmtp::LcBgpDns( void )
{
	// local -------------------
		__u8	Tv_GetCnt;
		__u32	Tv_Ip;
	// code --------------------
		// DNS
		if ( 0 <= m_HndlDns )
		{
			// get result
			Tv_Ip	=	0;
			if ( ((Cls_GrNetSvs*)m_NetSvs)->ImmDnsResult( m_HndlDns, &Tv_Ip, 1, &Tv_GetCnt, FALSE ) )
			{
				// update IP
				m_SvIp	=	Tv_Ip;
				m_DnsRstTimer	=	E_GrNetPlugSmtpDnsUpdtTime;
				// release DNS
				((Cls_GrNetSvs*)m_NetSvs)->ImmDnsFree( m_HndlDns, FALSE );
				m_HndlDns	=	-1;
			}
		}

		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrNetPlugSmtp::LcProcSsl()
{
	// local -------------------
		BOOL8		Tv_Result;
		__u8	Tv_Rval;
		Ptr_GrNetPlugSmtpItemHd	Tv_PtrItem;
#ifndef E_GrNetPlugSmtpSslNot
		void*	Tv_PtrSend;
		__s32		Tv_Recved;
		Def_StrLong	Tv_StrRecv;	
		__s32		Tv_Sent;
		fd_set Tv_FdWrite;
		__s32 Tv_ErrVal;
		__s32		Tv_RtnVal;
		fd_set Tv_FdRead;
		__s32 Tv_Sslval;
		timeval	Tv_Time;
#endif
	// code --------------------
		Tv_Result		= FALSE;

		// check exist item
			Tv_PtrItem	=	(Ptr_GrNetPlugSmtpItemHd)GrBufVqGetOutPtr( (Ptr_GrBufVarQue)m_MailQue );
			if ( NULL != Tv_PtrItem )
			{
				// check stage
				Tv_Result	=	TRUE;
				switch ( Tv_PtrItem->Stage )
				{
					case E_GrNetPlugSmtpStageTryConn:
						// try connect
						Tv_Rval	=	GrSockV2Connect( Tv_PtrItem->HndlSock, &m_SvIp, m_SvPort, FALSE );
						if ( E_GrSockV2ConnOk == Tv_Rval )
						{
							Tv_PtrItem->Stage		=	E_GrNetPlugSmtpStageSsl;
							Tv_PtrItem->TimeOut	=	10;
						}
						else if ( E_GrSockV2ConnWaiting != Tv_Rval )
						{
							DbgMsgPrint( "Cls_GrNetPlugSmtp::LcBgpItem - SMTP server connect fail!\n" );
							Tv_PtrItem->Stage		=	E_GrNetPlugSmtpStageFinish;
							Tv_PtrItem->TimeOut	=	10;
						}
						else
						{
							Tv_PtrItem->Stage		=	E_GrNetPlugSmtpStageWaitConn;
							Tv_PtrItem->TimeOut	=	20;
							Tv_Result	=	FALSE;
						}
						break;
					case E_GrNetPlugSmtpStageWaitConn:
						Tv_Rval	=	GrSockV2WaitConnect( Tv_PtrItem->HndlSock, 0 );
						if ( E_GrSockV2ConnOk == Tv_Rval )
						{
							// go next
							Tv_PtrItem->Stage		=	E_GrNetPlugSmtpStageSsl;
							Tv_PtrItem->TimeOut	=	10;
						}
						else if ( E_GrSockV2ConnWaiting != Tv_Rval )
						{
							//mail error
							DbgMsgPrint( "Cls_GrNetPlugSmtp::LcBgpItem - smtp server connect error.\n" );
							//go finish
							Tv_PtrItem->Stage		=	E_GrNetPlugSmtpStageFinish;
							Tv_PtrItem->TimeOut	=	10;
						}
						else
						{
							Tv_Result	=	FALSE;
						}
						break;
					case E_GrNetPlugSmtpStageFinish:
#ifndef E_GrNetPlugSmtpSslNot
#ifdef	LINUX_APP
						if(NULL != m_PtrSsl)
						{
							SSL_shutdown (m_PtrSsl);  /* send SSL/TLS close_notify */
							SSL_free (m_PtrSsl);
							m_PtrSsl = NULL;
						}

						if(NULL != m_PtrCtx)
						{
							SSL_CTX_free(m_PtrCtx);	
							m_PtrCtx = NULL;
						}
#endif
#endif
						//close socket
						GrSockV2Close( Tv_PtrItem->HndlSock );
						//release buffer
						GrBufVqOut( (Ptr_GrBufVarQue)m_MailQue );
						break;
					case E_GrNetPlugSmtpStageSsl:
#ifndef E_GrNetPlugSmtpSslNot						
#ifdef	LINUX_APP
						SSL_library_init();
						SSL_load_error_strings();
						m_PtrCtx = SSL_CTX_new(SSLv23_client_method());

						if(NULL == m_PtrCtx)
						{
							DbgMsgPrint( "Cls_GrNetPlugSmtp::LcProcSsl - E_GrNetPlugSmtpStageTryConn : ssl ctx fail\n" );
						}
						SSL_CTX_set_options(m_PtrCtx, SSL_OP_NO_SSLv2);
						m_PtrSsl	= SSL_new(m_PtrCtx);
						if(NULL != m_PtrSsl)
						{
							SSL_set_fd(m_PtrSsl, Tv_PtrItem->HndlSock);
							SSL_set_mode(m_PtrSsl, SSL_MODE_AUTO_RETRY);

							Tv_Time.tv_sec = 180;
							Tv_Time.tv_usec	= 0;
							
							Tv_ErrVal = 0;
							for(__s32 Tv_WkCnt = 0;Tv_WkCnt < 10;Tv_WkCnt++)
							{
								FD_ZERO(&Tv_FdWrite);
								FD_ZERO(&Tv_FdRead);

								if(SSL_ERROR_WANT_READ == Tv_ErrVal)
								{
									FD_SET(Tv_PtrItem->HndlSock, &Tv_FdRead);
								}
								if(SSL_ERROR_WANT_WRITE == Tv_ErrVal)
								{
									FD_SET(Tv_PtrItem->HndlSock, &Tv_FdWrite);
								}

								if((SSL_ERROR_WANT_WRITE == Tv_ErrVal) || (SSL_ERROR_WANT_READ == Tv_ErrVal))
								{
									if(-1 == select(Tv_PtrItem->HndlSock + 1, &Tv_FdRead, &Tv_FdWrite, NULL, &Tv_Time))
									{
										DbgMsgPrint("Cls_GrNetPlugSmtp::LcProcSsl() - select socket error\n");
									}
								}

								Tv_Sslval = SSL_connect(m_PtrSsl);

								if(0 < Tv_Sslval)
								{
									break;
								}
								else
								{
									Tv_ErrVal = SSL_get_error(m_PtrSsl, Tv_Sslval);

									switch(Tv_ErrVal)
									{
									case SSL_ERROR_NONE:
										DbgMsgPrint("Cls_GrNetPlugSmtp::LcProcSsl() - SSL_get_error = SSL_ERROR_NONE\n");
										break;
									case SSL_ERROR_ZERO_RETURN:
										DbgMsgPrint("Cls_GrNetPlugSmtp::LcProcSsl() SSL_get_error = SSL_ERROR_ZERO_RETURN\n");
										break;
									case SSL_ERROR_WANT_READ:
										DbgMsgPrint("Cls_GrNetPlugSmtp::LcProcSsl() - SSL_get_error = SSL_ERROR_WANT_READ %d\n", Tv_WkCnt);
										break;
									case SSL_ERROR_WANT_WRITE:
										DbgMsgPrint("Cls_GrNetPlugSmtp::LcProcSsl() - SSL_get_error = SSL_ERROR_WANT_WRITE %d\n", Tv_WkCnt);
										break;
									case SSL_ERROR_WANT_CONNECT:
										DbgMsgPrint("Cls_GrNetPlugSmtp::LcProcSsl() - SSL_get_error = SSL_ERROR_WANT_CONNECT\n");
										break;
									case SSL_ERROR_WANT_ACCEPT:
										DbgMsgPrint("Cls_GrNetPlugSmtp::LcProcSsl() - SSL_get_error = SSL_ERROR_WANT_ACCEPT\n");
										break;
									case SSL_ERROR_WANT_X509_LOOKUP:
										DbgMsgPrint("Cls_GrNetPlugSmtp::LcProcSsl() - SSL_get_error = SSL_ERROR_WANT_X509_LOOKUP\n");
										break;
									case SSL_ERROR_SYSCALL:
										DbgMsgPrint("Cls_GrNetPlugSmtp::LcProcSsl() - SSL_get_error = SSL_ERROR_SYSCALL\n");
										break;
									case SSL_ERROR_SSL:
										Tv_PtrItem->Stage		=	E_GrNetPlugSmtpStageFinish;
										Tv_PtrItem->TimeOut	=	10;
										Tv_Result = TRUE;
										DbgMsgPrint("Cls_GrNetPlugSmtp::LcProcSsl() - SSL_get_error = SSL_ERROR_SSL\n");
										break;
									}
								}
							}

							if(LcSmtpRecv(Tv_PtrItem, "250"))
							{
								Tv_PtrItem->Stage		=	E_GrNetPlugSmtpStageSslHello;
								Tv_PtrItem->TimeOut	=	10;
								Tv_PtrItem->Sent	= 0;
								Tv_PtrItem->Size	= Tv_PtrItem->Info[0].Size;
								Tv_Result = TRUE;
							}
						}
						else
						{
							DbgMsgPrint( "Cls_GrNetPlugSmtp::LcProcSsl - E_GrNetPlugSmtpStageTryConn : new ssl fail\n" );
						}
#endif
#endif
						break;
					case E_GrNetPlugSmtpStageSslHello:
						Tv_Result	=	FALSE;
						if(LcSmtpSend(Tv_PtrItem, Tv_PtrItem->Info[0].Pos))
						{
							if(LcSmtpRecv(Tv_PtrItem, "220"))
							{
								Tv_PtrItem->Stage		=	E_GrNetPlugSmtpStageSslLogin;
								Tv_PtrItem->TimeOut	=	10;
								Tv_PtrItem->Sent	= 0;
								Tv_PtrItem->Size	= Tv_PtrItem->Info[1].Size;
								Tv_Result = TRUE;
							}
						}
						break;
					case E_GrNetPlugSmtpStageSslLogin:
						Tv_Result	=	FALSE;
						if(LcSmtpSend(Tv_PtrItem, Tv_PtrItem->Info[1].Pos))
						{
							if(LcSmtpRecv(Tv_PtrItem, "334"))
							{
								Tv_PtrItem->Stage		=	E_GrNetPlugSmtpStageSslId;
								Tv_PtrItem->TimeOut	=	10;
								Tv_PtrItem->Sent	= 0;
								Tv_PtrItem->Size	= Tv_PtrItem->Info[2].Size;
								Tv_Result = TRUE;
							}
						}
						break;
					case E_GrNetPlugSmtpStageSslId:
						
						Tv_Result	=	FALSE;
						if(LcSmtpSend(Tv_PtrItem, Tv_PtrItem->Info[2].Pos))
						{
							if(LcSmtpRecv(Tv_PtrItem, "334"))
							{
								Tv_PtrItem->Stage		=	E_GrNetPlugSmtpStageSslPass;
								Tv_PtrItem->TimeOut	=	10;
								Tv_PtrItem->Sent	= 0;
								Tv_PtrItem->Size	= Tv_PtrItem->Info[3].Size;
								Tv_Result = TRUE;
							}
						}
						break;
					case E_GrNetPlugSmtpStageSslPass:
						
						Tv_Result	=	FALSE;
						if(LcSmtpSend(Tv_PtrItem, Tv_PtrItem->Info[3].Pos))
						{
							if(LcSmtpRecv(Tv_PtrItem, "235"))
							{
								Tv_PtrItem->Stage		=	E_GrNetPlugSmtpStageSslFrom;
								Tv_PtrItem->TimeOut	=	10;
								Tv_PtrItem->Sent	= 0;
								Tv_PtrItem->Size	= Tv_PtrItem->Info[4].Size;
								Tv_Result = TRUE;
							}
						}
						break;
					case E_GrNetPlugSmtpStageSslFrom:
						Tv_Result	=	FALSE;
						if(LcSmtpSend(Tv_PtrItem, Tv_PtrItem->Info[4].Pos))
						{
							if(LcSmtpRecv(Tv_PtrItem, "250"))
							{
								Tv_PtrItem->Stage		=	E_GrNetPlugSmtpStageSslTo;
								Tv_PtrItem->TimeOut	=	10;
								Tv_PtrItem->Sent	= 0;
								Tv_PtrItem->Size	= Tv_PtrItem->Info[5].Size;
								Tv_Result = TRUE;
							}
						}
						break;
					case E_GrNetPlugSmtpStageSslTo:						
						Tv_Result	=	FALSE;
						if(LcSmtpSend(Tv_PtrItem, Tv_PtrItem->Info[5].Pos))
						{
							if(LcSmtpRecv(Tv_PtrItem, "250"))
							{
								Tv_PtrItem->Stage		=	E_GrNetPlugSmtpStageSslData;
								Tv_PtrItem->TimeOut	=	10;
								Tv_PtrItem->Sent	= 0;
								Tv_PtrItem->Size	= Tv_PtrItem->Info[6].Size;
								Tv_Result = TRUE;
							}
						}
						break;
					case E_GrNetPlugSmtpStageSslData:
						Tv_Result	=	FALSE;
						if(LcSmtpSend(Tv_PtrItem, Tv_PtrItem->Info[6].Pos))
						{
							if(LcSmtpRecv(Tv_PtrItem, "354"))
							{
								Tv_PtrItem->Stage		=	E_GrNetPlugSmtpStageSslMail;
								Tv_PtrItem->TimeOut	=	10;
								Tv_PtrItem->Sent	= 0;
								Tv_PtrItem->Size	= Tv_PtrItem->Info[7].Size;
								Tv_Result = TRUE;
							}
						}
						break;
					case E_GrNetPlugSmtpStageSslMail:
						Tv_Result	=	FALSE;
						if(LcSmtpSend(Tv_PtrItem, Tv_PtrItem->Info[7].Pos))
						{
							if(LcSmtpRecv(Tv_PtrItem, "250"))
							{
								Tv_PtrItem->Stage		=	E_GrNetPlugSmtpStageSslEnd;
								Tv_PtrItem->TimeOut	=	10;
								Tv_PtrItem->Sent	= 0;
								Tv_PtrItem->Size	= Tv_PtrItem->Info[8].Size;
								Tv_Result = TRUE;
							}
						}
						break;
					case E_GrNetPlugSmtpStageSslEnd:
						Tv_Result	=	FALSE;
						if(LcSmtpSend(Tv_PtrItem, Tv_PtrItem->Info[8].Pos))
						{
							if(LcSmtpRecv(Tv_PtrItem, "250"))
							{
								Tv_PtrItem->Stage		=	E_GrNetPlugSmtpStageFinish;
								Tv_PtrItem->TimeOut	=	10;
								Tv_Result = TRUE;
							}
						}
						break;
					default:
						// bad stage
						DbgMsgPrint( "Cls_GrNetPlugSmtp::LcBgpItem - bad stage!\n" );
						GrBufVqOut( (Ptr_GrBufVarQue)m_MailQue );
						break;
				}
			}

		return Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrNetPlugSmtp::LcSmtpSend(Ptr_GrNetPlugSmtpItemHd A_PtrSmtpItm, __u32 A_SmtpPos)
{
	// local -------------------
		BOOL8		Tv_Result;
#ifndef E_GrNetPlugSmtpSslNot
		__s32			Tv_Sent;
		void*		Tv_PtrSend;
		fd_set Tv_FdWrite;
		fd_set Tv_FdRead;
		timeval	Tv_Time;
#endif
	// code --------------------
		Tv_Result		= TRUE;

#ifndef E_GrNetPlugSmtpSslNot
#ifdef	LINUX_APP
		if(NULL != A_PtrSmtpItm)
		{
			if(A_PtrSmtpItm->Sent < A_PtrSmtpItm->Size)
			{
				FD_ZERO(&Tv_FdRead);
				FD_ZERO(&Tv_FdWrite);

				FD_SET(A_PtrSmtpItm->HndlSock,&Tv_FdWrite);
				
				if(FD_ISSET(A_PtrSmtpItm->HndlSock,&Tv_FdWrite))
				{
					Tv_Time.tv_sec = 10;
					Tv_Time.tv_usec	= 0;
					if(-1 == select(A_PtrSmtpItm->HndlSock + 1, &Tv_FdRead, &Tv_FdWrite, NULL, &Tv_Time))
					{
						DbgMsgPrint("Cls_GrNetPlugSmtp::LcSmtpSend - select socket error\n");
					}

					Tv_PtrSend	= (void*)((__u32)A_PtrSmtpItm + sizeof(St_GrNetPlugSmtpItemHd) + A_PtrSmtpItm->Sent + A_SmtpPos);
					Tv_Sent = SSL_write(m_PtrSsl, Tv_PtrSend, (A_PtrSmtpItm->Size - A_PtrSmtpItm->Sent));
				
					if(0 < Tv_Sent)
					{
						A_PtrSmtpItm->Sent	=	A_PtrSmtpItm->Sent + Tv_Sent;
					}
					else
					{
						switch(SSL_get_error(m_PtrSsl, Tv_Sent))
						{
							Tv_Result = FALSE;
							/* We wrote something*/
							case SSL_ERROR_NONE:
								DbgMsgPrint( "Cls_GrNetPlugSmtp::LcSmtpSend - SSL_write = SSL_ERROR_NONE\n" );
								break;
							/* We would have blocked */
							case SSL_ERROR_WANT_WRITE:
								DbgMsgPrint( "Cls_GrNetPlugSmtp::LcSmtpSend - SSL_write = SSL_ERROR_WANT_WRITE\n" );
								break;
							case SSL_ERROR_WANT_READ:
								DbgMsgPrint( "Cls_GrNetPlugSmtp::LcSmtpSend - SSL_write = SSL_ERROR_WANT_READ\n" );
								break;
							default:
								DbgMsgPrint("Cls_GrNetPlugSmtp::LcSmtpSend - SSL_get_error : go finish\n");
								A_PtrSmtpItm->Stage		=	E_GrNetPlugSmtpStageFinish;
								A_PtrSmtpItm->TimeOut	=	10;
						}
					}
				}
			}
		}
#endif
#endif

		return Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrNetPlugSmtp::LcSmtpRecv(Ptr_GrNetPlugSmtpItemHd A_PtrSmtpItm, char* A_PtrStr)
{
	// local -------------------
		BOOL8		Tv_Result;
#ifndef E_GrNetPlugSmtpSslNot
		Def_StrLong	Tv_StrRecv;
		fd_set Tv_FdWrite;
		__s32		Tv_ErrVal;
		__s32		Tv_Recved;
		fd_set Tv_FdRead;
		timeval	Tv_Time;
#endif
		
	// code --------------------
		Tv_Result		= FALSE;

#ifndef E_GrNetPlugSmtpSslNot
#ifdef	LINUX_APP
		FD_ZERO(&Tv_FdRead);
		FD_ZERO(&Tv_FdWrite);

		Tv_Time.tv_sec = 10;
		Tv_Time.tv_usec	= 0;
		FD_SET(A_PtrSmtpItm->HndlSock,&Tv_FdRead);

		if(-1 == select(A_PtrSmtpItm->HndlSock + 1, &Tv_FdRead, &Tv_FdWrite, NULL, &Tv_Time))
		{
			DbgMsgPrint("Cls_GrNetPlugSmtp::LcSmtpRecv - select socket error\n");
		}

		if(FD_ISSET(A_PtrSmtpItm->HndlSock, &Tv_FdRead))
		{
			Tv_Recved = SSL_read(m_PtrSsl, Tv_StrRecv, 1024);
			if(0 < Tv_Recved)
			{
				Tv_Result = TRUE;
			}
			else
			{
				if(0 > Tv_Recved)
				{
					DbgMsgPrint("Cls_GrNetPlugSmtp::LcSmtpRecv Recv go finish\n");
					// go to finish
					A_PtrSmtpItm->Stage		=	E_GrNetPlugSmtpStageFinish;
					A_PtrSmtpItm->TimeOut	=	10;
				}

				Tv_ErrVal = SSL_get_error(m_PtrSsl, Tv_Recved);

				switch(Tv_ErrVal)
				{
				case SSL_ERROR_WANT_READ:
					DbgMsgPrint("Cls_GrNetPlugSmtp::LcSmtpRecv - SSL_get_error = SSL_ERROR_WANT_READ %d\n");
					break;
				case SSL_ERROR_WANT_WRITE:
					DbgMsgPrint("Cls_GrNetPlugSmtp::LcSmtpRecv - SSL_get_error = SSL_ERROR_WANT_WRITE %d\n");
					//FD_SET(A_PtrSmtpItm->HndlSock,&Tv_FdWrite);
					break;
				default:
					DbgMsgPrint("Cls_GrNetPlugSmtp::LcSmtpRecv - SSL_get_error : go finish\n");
					A_PtrSmtpItm->Stage		=	E_GrNetPlugSmtpStageFinish;
					A_PtrSmtpItm->TimeOut	=	10;
					break;
				}
			}
		}

#endif
#endif
		return Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrNetPlugSmtp::LcChkRecvStr(char* A_PtrStr, char* A_PtrCmpStr)
{
	// local -------------------
		BOOL8		Tv_Result;
		__u32		Tv_StrSrCnt;
		__u32		Tv_StrTgCnt;
		__u32		Tv_WkCnt;
		char*		Tv_PtrSr;
		char*		Tv_PtrTg;
	// code --------------------
		Tv_Result		= FALSE;

		Tv_StrSrCnt = GrStrLen(A_PtrCmpStr);
		Tv_StrTgCnt = GrStrLen(A_PtrCmpStr);

		Tv_PtrSr		= A_PtrStr;
		Tv_PtrTg		= A_PtrCmpStr;
		for(Tv_WkCnt = 0;Tv_WkCnt < (Tv_StrSrCnt - Tv_StrTgCnt);Tv_WkCnt++)
		{
			if(0 == GrStrCmp(Tv_PtrSr, Tv_PtrTg, FALSE, Tv_StrTgCnt))
			{
				Tv_Result		= TRUE;
				break;
			}
			Tv_PtrSr++;
		}

		return Tv_Result;
}
//--------------------------------------------------------------------