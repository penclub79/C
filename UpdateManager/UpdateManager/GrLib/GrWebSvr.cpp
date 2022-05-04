/*
	web server


*/

#include	<GrWebSvr.h>
#include	<GrStrTool.h>
#include	<GrProcess.h>
#include	<GrDebug.h>
#include	<GrDumyTool.h>

//--------------------------------------------------------------------
//local const
#define E_GrWebSvrDfltPort			80

#define E_GrWebSvrParsCmdNone		0
#define E_GrWebSvrParsCmdGet		1
#define E_GrWebSvrParsCmdHead		2
#define E_GrWebSvrParsCmdPost		3

#define E_GrWebSvrCmdGetHead		"HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Type: text/html\r\nContent-Length: "

#define E_GrWebSvrMsgCmdSetupPort			0		/* setup listen port : St_GrWebSvrMsgSetupPort */

//--------------------------------------------------------------------
//local type

typedef	struct St_GrWebSvrMsgBase 
{
	St_GrTaskMsgHd	MsgHd;
	__u32		Cmd;
}	*Ptr_GrWebSvrMsgBase;

typedef	struct St_GrWebSvrMsgSetupPort 
{
	St_GrWebSvrMsgBase	Hd;
	__u16	Port;
}	*Ptr_GrWebSvrMsgSetupPort;


//--------------------------------------------------------------------
//local function


//--------------------------------------------------------------------
//global var


//====================================================================
//--------------------------------------------------------------------
Cls_GrWebSvr::Cls_GrWebSvr(Cls_GrTaskSvr* A_TaskSvr, char* A_StrPath)
{
	// local -------------------
		__u8	Tv_WkIdx;
	// code --------------------
		m_TaskSvr	=	A_TaskSvr;
		m_MsgAloc	=	GrTaskSvrMsgCmdAlloc();

		m_ConnCnt			=	0;
		GrStrCopy( m_StrPath, A_StrPath );		//copy base path
		m_SockListen	=	E_GrSockBadHandle;
		m_Port				=	E_GrWebSvrDfltPort;

		GrDumyZeroMem( m_ConnTbl, sizeof(m_ConnTbl) );
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrWebSvrMaxConn;Tv_WkIdx++ )
		{
			m_ConnTbl[Tv_WkIdx].Sock	=	E_GrSockBadHandle;
		}

		//
		m_TaskSvr->RegistMsgClient( this );
		m_TaskSvr->RegistBkgClient( this );
}
//--------------------------------------------------------------------
Cls_GrWebSvr::~Cls_GrWebSvr( void )
{
	// local -------------------
		__u8	Tv_WkIdx;
	// code --------------------
		//release task 
		m_TaskSvr->UnregistBkgClient( this );
		m_TaskSvr->UnregistMsgClient( this );

		//release all
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrWebSvrMaxConn;Tv_WkIdx++ )
		{
			LcConnClose( Tv_WkIdx );
		}

		//close listen 
		if ( E_GrSockBadHandle != m_SockListen )
		{
			GrSockClose( m_SockListen );
		}

		//release command
		GrTaskSvrMsgCmdFree( m_MsgAloc );
}
//--------------------------------------------------------------------
void	Cls_GrWebSvr::LcSetupPort( __u16 A_Port )
{
		//check port
		if ( m_Port != A_Port )
		{
			m_Port		=	A_Port;
			//close socket
			if ( E_GrSockBadHandle != m_SockListen )
			{
				GrSockClose( m_SockListen );
			}
			m_SockListen	=	E_GrSockBadHandle;
		}
}
//--------------------------------------------------------------------
void	Cls_GrWebSvr::LcListenSockUpdt( void )
{
		if ( E_GrSockBadHandle == m_SockListen )
		{
			m_SockListen	=	GrSockOpen( FALSE, TRUE );
			GrSockSetListenMode( m_SockListen, m_Port );
		}
}
//--------------------------------------------------------------------
void	Cls_GrWebSvr::LcConnClose( __u8 A_ConnIdx )
{
		//check exist
		if ( E_GrSockBadHandle != m_ConnTbl[A_ConnIdx].Sock )
		{
			//release file control
			if ( NULL != m_ConnTbl[A_ConnIdx].Fc )
			{
				delete	m_ConnTbl[A_ConnIdx].Fc;
				m_ConnTbl[A_ConnIdx].Fc	=	NULL;
			}
			//close socket
			GrSockClose( m_ConnTbl[A_ConnIdx].Sock );
			m_ConnTbl[A_ConnIdx].Sock	=	E_GrSockBadHandle;
			m_ConnCnt --;
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrWebSvr::LcConnChkCmdRecved( __u8 A_ConnIdx )
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrWebSvrConn	Tv_PtrConn;
		__u32	Tv_WkIdx;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_PtrConn	=	(Ptr_GrWebSvrConn)&m_ConnTbl[A_ConnIdx];
		if ( 3 < Tv_PtrConn->BufSize )
		{
			Tv_WkIdx	=	Tv_PtrConn->BufSize - 1;
			while ( 3 <= Tv_WkIdx )
			{
				//check command
				if (	(0x0D == Tv_PtrConn->BufCont[Tv_WkIdx - 3]) && 
							(0x0A == Tv_PtrConn->BufCont[Tv_WkIdx - 2]) &&
							(0x0D == Tv_PtrConn->BufCont[Tv_WkIdx - 1]) &&
							(0x0A == Tv_PtrConn->BufCont[Tv_WkIdx]) 
						)
				{
					//finded
					Tv_PtrConn->BufSize	=	Tv_WkIdx;
					Tv_PtrConn->BufCont[Tv_WkIdx]	=	0;		//mark last position
					Tv_Result	=	TRUE;
					break;
				}
				//next
				Tv_WkIdx --;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrWebSvr::LcConnRecv( __u8 A_ConnIdx )
{
	// local -------------------
		Ptr_GrWebSvrConn	Tv_PtrConn;
		void*	Tv_PtrBuf;
		__u32	Tv_Resid;
		__s32		Tv_Rslt;
		__u8	Tv_CmdType;
		char*	Tv_StrNext;
	// code --------------------
		Tv_PtrConn	=	(Ptr_GrWebSvrConn)&m_ConnTbl[A_ConnIdx];
		//revice data
		Tv_Resid	=	E_GrWebSvrContBufSize - Tv_PtrConn->BufSize - 1;
		if ( 0 != Tv_Resid )
		{
			Tv_PtrBuf	=	(void*)( (__u32)Tv_PtrConn->BufCont + Tv_PtrConn->BufSize );
			Tv_Rslt	=	GrSockRecv( Tv_PtrConn->Sock, Tv_PtrBuf, Tv_Resid );
			if ( 0 < Tv_Rslt )
			{
				Tv_PtrConn->BufSize	=	Tv_PtrConn->BufSize + (__u32)Tv_Rslt;
				Tv_PtrConn->BufCont[Tv_PtrConn->BufSize]	=	0;		//mark zero
				//check command recive finish
				if ( LcConnChkCmdRecved( A_ConnIdx ) )
				{
					//parsing command
					Tv_CmdType	=	LcConnParsCmd( A_ConnIdx, &Tv_StrNext );
					if ( (NULL != Tv_StrNext) && (E_GrWebSvrParsCmdGet == Tv_CmdType) )
					{ 
						//get file name
						GrStrFindWordChar( Tv_StrNext, " ", Tv_PtrConn->StrPars );
						LcConnParsStrToFn( A_ConnIdx );		//make file name
						//connection get send mode
						if ( LcConnModeSetCmdGet( A_ConnIdx ) )
						{
							//do send mode
							Tv_PtrConn->BufSended		=	0;
							Tv_PtrConn->IsSendMode	=	TRUE;
						}
						else
						{
							LcConnClose( A_ConnIdx );
						}
					}
					else
					{
						LcConnClose( A_ConnIdx );
						DbgMsgPrint("Cls_GrWebSvr::LcConnRecv - bad command.\n");
					}
				}
			}
			else
			{
				if ( GrSockChkCloseByRecv( Tv_Rslt ) )
				{
					LcConnClose( A_ConnIdx );
					DbgMsgPrint("Cls_GrWebSvr::LcConnRecv - command recive error!\n");
				}
			}
		}
		else
		{
			//overflow command size
			DbgMsgPrint("Cls_GrWebSvr::LcConnRecv - command size overflow!\n");
			LcConnClose( A_ConnIdx );
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrWebSvr::LcConnParsCmd(__u8 A_ConnIdx, char** A_PtrNext)
{
	// local -------------------
		__u8	Tv_Result;
		Ptr_GrWebSvrConn	Tv_PtrConn;
		char*	Tv_StrNxt;
	// code --------------------
		Tv_Result	=	E_GrWebSvrParsCmdNone;

		Tv_PtrConn	=	(Ptr_GrWebSvrConn)&m_ConnTbl[A_ConnIdx];
		Tv_StrNxt		=	(char*)Tv_PtrConn->BufCont;
		Tv_StrNxt		=	GrStrFindWordChar( Tv_StrNxt, " " , Tv_PtrConn->StrPars );
		//check command
		if ( 0 == GrStrCmp( Tv_PtrConn->StrPars, "GET", FALSE ) )
		{
			Tv_Result	=	E_GrWebSvrParsCmdGet;
		}
		else if ( 0 == GrStrCmp( Tv_PtrConn->StrPars, "HEAD", FALSE ) )
		{
			Tv_Result	=	E_GrWebSvrParsCmdHead;
		}
		else if ( 0 == GrStrCmp( Tv_PtrConn->StrPars, "POST", FALSE ) )
		{
			Tv_Result	=	E_GrWebSvrParsCmdPost;
		}

		//update next
		*A_PtrNext	=	Tv_StrNxt;

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrWebSvr::LcConnParsStrToFn( __u8 A_ConnIdx )
{
	// local -------------------
		Ptr_GrWebSvrConn	Tv_PtrConn;
		BOOL8	Tv_IsDfltFn;
		char*	Tv_StrWk;
	// code --------------------
		Tv_PtrConn	=	(Ptr_GrWebSvrConn)&m_ConnTbl[A_ConnIdx];

		Tv_IsDfltFn	=	FALSE;

		//check file name
		if ( 0 == GrStrCmp( Tv_PtrConn->StrPars, "/" ,TRUE ) )
		{
			Tv_IsDfltFn	=	TRUE;
		}
		//build file name
		Tv_StrWk	=	GrStrCopy( Tv_PtrConn->StrFn, m_StrPath );
		if ( Tv_IsDfltFn )
		{
			GrStrCopy( Tv_StrWk, "/default.htm" );
		}
		else
		{
			GrStrCopy( Tv_StrWk, Tv_PtrConn->StrPars );
		}
		//case change to low
		GrStrChgStrSenseLow( Tv_PtrConn->StrFn );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrWebSvr::LcConnModeSetCmdGet( __u8 A_ConnIdx )
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrWebSvrConn	Tv_PtrConn;
		Cls_GrFileCtrl*	Tv_Fc;
		char*	Tv_StrWk;
	// code --------------------
		Tv_Result		=	FALSE;
		Tv_PtrConn	=	(Ptr_GrWebSvrConn)&m_ConnTbl[A_ConnIdx];

		//open file
		Tv_Fc	=	(Cls_GrFileCtrl*)new Cls_GrFileCtrl( Tv_PtrConn->StrFn, FALSE, FALSE, 
			FALSE, TRUE );
		Tv_PtrConn->Fc	=	Tv_Fc;
		if ( Tv_Fc->IsOpened() )
		{
			//get file size
			Tv_PtrConn->FlReside	=	(__u32)Tv_Fc->GetSize();
			//get first position
			Tv_Fc->Seek( 0 );
			//make default command
			Tv_StrWk	=	GrStrCopy((char*)Tv_PtrConn->BufCont, E_GrWebSvrCmdGetHead);
			GrStrIntToStr( Tv_StrWk, (__s32)Tv_PtrConn->FlReside );
			Tv_StrWk	=	GrStrCat( Tv_StrWk, "\r\n\r\n" );
			//calculate
			Tv_PtrConn->BufSize	=	(__u32)Tv_StrWk - (__u32)Tv_PtrConn->BufCont;
			//success
			Tv_Result	=	TRUE;
		}
		else
		{
			DbgMsgPrint("Cls_GrWebSvr::LcConnModeSetCmdGet - bad file - %s \n", Tv_PtrConn->StrFn );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrWebSvr::LcConnSend( __u8 A_ConnIdx )
{
	// local -------------------
		Ptr_GrWebSvrConn	Tv_PtrConn;
		void*	Tv_PtrBuf;
		__u32	Tv_SendSize;
		__s32		Tv_Rslt;
	// code --------------------
		Tv_PtrConn	=	(Ptr_GrWebSvrConn)&m_ConnTbl[A_ConnIdx];

		//check reside buffer
		if ( Tv_PtrConn->BufSended < Tv_PtrConn->BufSize )
		{
			Tv_PtrBuf	=	(void*)( (__u32)Tv_PtrConn->BufCont + Tv_PtrConn->BufSended );
			Tv_SendSize	=	Tv_PtrConn->BufSize - Tv_PtrConn->BufSended;
			Tv_Rslt	=	GrSockSend( Tv_PtrConn->Sock, Tv_PtrBuf, Tv_SendSize );
			if ( 0 < Tv_Rslt )
			{
				Tv_PtrConn->BufSended	=	Tv_PtrConn->BufSended + (__u32)Tv_Rslt;
			}
			else
			{
				//check disconnected
				if ( E_GrSockErrWouldblock != GrSockGetErrCode() )
				{
					//socket release
					LcConnClose( A_ConnIdx );
					DbgMsgPrint( "Cls_GrWebSvr::LcConnSend - send fail\n" );
				}
			}
		}
		else
		{
			//check file opened
			if ( NULL != Tv_PtrConn->Fc )
			{
				//reset 
				Tv_PtrConn->BufSended	=	0;
				Tv_SendSize	=	Tv_PtrConn->FlReside;
				if ( E_GrWebSvrContBufSize < Tv_SendSize )
				{
					Tv_SendSize	=	E_GrWebSvrContBufSize;
				}
				if ( Tv_SendSize == (__u32)Tv_PtrConn->Fc->Read( Tv_PtrConn->BufCont, Tv_SendSize ) )
				{
					Tv_PtrConn->BufSize	=	Tv_SendSize;
					Tv_PtrConn->FlReside	=	Tv_PtrConn->FlReside - Tv_SendSize;
					if ( 0 == Tv_PtrConn->FlReside )
					{
						//release file control
						delete	Tv_PtrConn->Fc;
						Tv_PtrConn->Fc	=	NULL;
					}
				}
				else
				{
					LcConnClose( A_ConnIdx );
					DbgMsgPrint( "Cls_GrWebSvr::LcConnSend - file read error\n" );
				}
			}
			else
			{
				//send finished
				LcConnClose( A_ConnIdx );
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrWebSvr::LcAccept( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_Ip;
		__u16	Tv_Port;
		Def_GrSock	Tv_SockAcpt;
		__u8	Tv_ConnIdx;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( E_GrSockBadHandle != m_SockListen )
		{
			//check accept able
			if ( E_GrWebSvrMaxConn > m_ConnCnt )
			{
				Tv_SockAcpt	=	GrSockAccept( m_SockListen, TRUE, &Tv_Ip, &Tv_Port );
				if ( E_GrSockBadHandle != Tv_SockAcpt )
				{
					//find empty connn
					for ( Tv_ConnIdx=0;E_GrWebSvrMaxConn > Tv_ConnIdx;Tv_ConnIdx++ )
					{
						if ( E_GrSockBadHandle == m_ConnTbl[Tv_ConnIdx].Sock )
						{
							//finded
							m_ConnTbl[Tv_ConnIdx].Sock		=	Tv_SockAcpt;
							m_ConnTbl[Tv_ConnIdx].BufSize	=	0;
							m_ConnTbl[Tv_ConnIdx].BufSended	=	0;
							m_ConnTbl[Tv_ConnIdx].FlReside	=	0;
							m_ConnTbl[Tv_ConnIdx].IsSendMode	=	FALSE;
							GrStrClear( m_ConnTbl[Tv_ConnIdx].StrFn );
							if ( NULL != m_ConnTbl[Tv_ConnIdx].Fc )
							{
								delete	m_ConnTbl[Tv_ConnIdx].Fc;
								m_ConnTbl[Tv_ConnIdx].Fc	=	NULL;
								DbgMsgPrint("Cls_GrWebSvr::LcAccept - Find garbage file ctrl.\n");
							}
							m_ConnCnt ++;
							Tv_Result	=	TRUE;
							break;
						}
					}
					if ( !Tv_Result )
					{
						//not enough connection
						GrSockClose( Tv_SockAcpt );
						DbgMsgPrint("Cls_GrWebSvr::LcAccept - broken connection count.\n");
					}
				}
			}
		}
		
		return	Tv_Result;		
}
//--------------------------------------------------------------------
BOOL8	Cls_GrWebSvr::RtlBkgProc( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_ConnIdx;
	// code --------------------
		Tv_Result	=	FALSE;

		//listen socket update
		LcListenSockUpdt();
		//accept
		if ( LcAccept() )
		{
			Tv_Result	=	TRUE;
		}

		//check exist client
		for ( Tv_ConnIdx=0;Tv_ConnIdx < E_GrWebSvrMaxConn;Tv_ConnIdx++ )
		{
			if ( E_GrSockBadHandle != m_ConnTbl[Tv_ConnIdx].Sock )
			{
				if ( m_ConnTbl[Tv_ConnIdx].IsSendMode )
				{
					LcConnSend( Tv_ConnIdx );
				}
				else
				{
					LcConnRecv( Tv_ConnIdx );
				}
				//processed
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrWebSvr::RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		//check message
		if ( m_MsgAloc == A_PtrMsg->Cmd )
		{
			if ( E_GrWebSvrMsgCmdSetupPort == ((Ptr_GrWebSvrMsgBase)A_PtrMsg)->Cmd )
			{
				LcSetupPort( ((Ptr_GrWebSvrMsgSetupPort)A_PtrMsg)->Port );
			}

			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrWebSvr::SetupPort( __u16 A_Port, BOOL8 A_IsNow )
{
	// local -------------------
		St_GrWebSvrMsgSetupPort	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Hd.Cmd					=	E_GrWebSvrMsgCmdSetupPort;
		Tv_Msg.Port						=	A_Port;
		if ( A_IsNow )
		{
			m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
		else
		{
			m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
