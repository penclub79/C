/*
	web server


*/

#include	<Net/GrNetPlugWebSvr.h>
#include	<GrStrTool.h>
#include	<GrProcess.h>
#include	<GrDebug.h>
#include	<GrDumyTool.h>

//--------------------------------------------------------------------
//local const


//--------------------------------------------------------------------
//local type


//--------------------------------------------------------------------
//local function


//--------------------------------------------------------------------
//global var

__u8	V_GrNetPlugWebSvrHeadEoc[4]	=	
{	0x0D, 0x0A, 0x0D, 0x0A	};

char	V_GrNetPlugWebSvrRspGetHead[]	=	"HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Type: text/html\r\nContent-Length: ";

//====================================================================
//--------------------------------------------------------------------
Cls_GrNetPlugWebSvr::Cls_GrNetPlugWebSvr(Cls_GrTaskSvr* A_TaskSvr, Cls_GrTaskCli* A_NetSvs, __u8 A_DevIdx, __u8 A_PlugIdx) :
Cls_GrNetPlugBase(A_TaskSvr, A_NetSvs, A_DevIdx,A_PlugIdx )
{
	// local -------------------
		__u8	Tv_WkIdx;
	// code --------------------
		// init
		m_ConnCnt			=	0;

		GrStrClear( m_StrPath );
		GrStrCopy( m_StrIdxFn, "Default.htm" );
		m_SockListen	=	E_GrSockBadHandle;
		m_Port				=	E_GrNetPlugWebSvrDfltPort;

		GrDumyZeroMem( m_PtcTbl, sizeof(m_PtcTbl) );

		GrDumyZeroMem( m_ConnTbl, sizeof(m_ConnTbl) );
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrNetPlugWebSvrMaxConn;Tv_WkIdx++ )
		{
			m_ConnTbl[Tv_WkIdx].Sock	=	E_GrSockBadHandle;
		}

}
//--------------------------------------------------------------------
Cls_GrNetPlugWebSvr::~Cls_GrNetPlugWebSvr( void )
{
	// local -------------------
		__u8	Tv_WkIdx;
	// code --------------------

		//release all
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrNetPlugWebSvrMaxConn;Tv_WkIdx++ )
		{
			LcConnModeFinish( Tv_WkIdx );
		}

		//close listen 
		if ( E_GrSockBadHandle != m_SockListen )
		{
			GrSockV2Close( m_SockListen );
		}

}
//--------------------------------------------------------------------
void	Cls_GrNetPlugWebSvr::ProcCmdDevOff( void )
{

}
//--------------------------------------------------------------------
void	Cls_GrNetPlugWebSvr::ProcCmdDevOn( void )
{

}
//--------------------------------------------------------------------
void	Cls_GrNetPlugWebSvr::LcSetupScriptPath(char* A_StrPath)
{
	// local -------------------
	// code --------------------
		GrStrCopy( m_StrPath, A_StrPath );
}
//--------------------------------------------------------------------
void	Cls_GrNetPlugWebSvr::LcSetupPort( __u16 A_Port )
{
	// local -------------------
	// code --------------------
		//check port
		if ( m_Port != A_Port )
		{
			m_Port		=	A_Port;
			//close socket
			if ( E_GrSockBadHandle != m_SockListen )
			{
				GrSockV2Close( m_SockListen );
			}
			m_SockListen	=	E_GrSockBadHandle;
		}
}
//--------------------------------------------------------------------
void	Cls_GrNetPlugWebSvr::LcListenSockUpdt( void )
{
		if ( E_GrSockBadHandle == m_SockListen )
		{
			m_SockListen	=	GrSockV2Open( FALSE, TRUE, FALSE );
			if ( !GrSockV2SetListenMode( m_SockListen, m_Port, FALSE ) )
			{
				GrSockV2Close( m_SockListen );
				m_SockListen	=	E_GrSockBadHandle;
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrNetPlugWebSvr::LcConnChkCmdRecved( __u8 A_ConnIdx )
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrNetPlugWebSvrConn	Tv_PtrConn;
	// code --------------------
		Tv_Result	=	FALSE;
		// init
		Tv_PtrConn	=	(Ptr_GrNetPlugWebSvrConn)&m_ConnTbl[A_ConnIdx];

		// check finish
		while ( Tv_PtrConn->ParsPos < Tv_PtrConn->BufSize )
		{
			// check match
			if ( V_GrNetPlugWebSvrHeadEoc[Tv_PtrConn->FndCnt] == Tv_PtrConn->BufCont[Tv_PtrConn->ParsPos] )
			{
				// found one character
				Tv_PtrConn->FndCnt ++;
				// check found all
				if ( 4 <= Tv_PtrConn->FndCnt )
				{
					// found all character
					Tv_Result	=	TRUE;
					break;
				}
			}
			else
			{
				Tv_PtrConn->FndCnt	=	0;
			}
			// next
			Tv_PtrConn->ParsPos ++;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrNetPlugWebSvr::LcConnModeRecv( __u8 A_ConnIdx )
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrNetPlugWebSvrConn	Tv_PtrConn;
		void*	Tv_PtrBuf;
		__u32	Tv_Remain;
		__s32		Tv_Rslt;
	// code --------------------
		// init
		Tv_Result		=	FALSE;
		Tv_PtrConn	=	(Ptr_GrNetPlugWebSvrConn)&m_ConnTbl[A_ConnIdx];

		//receive data
		Tv_Remain		=	E_GrNetPlugWebSvrContBufSize - Tv_PtrConn->BufSize - 1;
		if ( 0 != Tv_Remain )
		{
			Tv_PtrBuf	=	(void*)( (__u32)Tv_PtrConn->BufCont + Tv_PtrConn->BufSize );
			Tv_Rslt	=	GrSockV2Recv( Tv_PtrConn->Sock, Tv_PtrBuf, Tv_Remain );
			if ( 0 < Tv_Rslt )
			{

				Tv_PtrConn->BufSize	=	Tv_PtrConn->BufSize + (__u32)Tv_Rslt;
				Tv_PtrConn->BufCont[Tv_PtrConn->BufSize]	=	0;		//mark zero
				//check command receive finish
				if ( LcConnChkCmdRecved( A_ConnIdx ) )
				{
					//parsing command
					if ( LcConnParsCmd( A_ConnIdx ) )
					{
						// prepare send mode
						Tv_PtrConn->Mode	=	E_GrNetPlugWebSvrModePrepSend;
					}
					else
					{
						// finish
						Tv_PtrConn->Mode	=	E_GrNetPlugWebSvrModeFinish;
					}
				}
				Tv_Result	=	TRUE;
			}
			else
			{
				if ( GrSockV2ChkCloseByRecv( Tv_Rslt ) )
				{
					DbgMsgPrint("Cls_GrNetPlugWebSvr::LcConnModeRecv - command recive error!\n");
					Tv_PtrConn->Mode	=	E_GrNetPlugWebSvrModeFinish;
				}
				else
				{
					if ( E_GrSockErrConnBroken == GrSockV2GetErrCode() )
					{
						DbgMsgPrint("Cls_GrNetPlugWebSvr::LcConnModeRecv - command recive error!\n");
						Tv_PtrConn->Mode	=	E_GrNetPlugWebSvrModeFinish;
					}
				}
			}
		}
		else
		{
			//overflow command size
			DbgMsgPrint("Cls_GrNetPlugWebSvr::LcConnModeRecv - command size overflow!\n");
			Tv_PtrConn->Mode	=	E_GrNetPlugWebSvrModeFinish;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrNetPlugWebSvr::LcConnGetFileName( __u8 A_ConnIdx, __u32 A_Pos )
{
	// local -------------------
		BOOL8	Tv_Reuslt;
		__u8	Tv_WkIdx;
		Ptr_GrNetPlugWebSvrConn	Tv_PtrConn;
		char*	Tv_StrSr;
		char*	Tv_StrTg;
		char*	Tv_StrFn;
		__u32	Tv_Pos;
		__u32	Tv_Len;
	// code --------------------
		// init
		Tv_Reuslt	=	FALSE;
		Tv_PtrConn	=	(Ptr_GrNetPlugWebSvrConn)&m_ConnTbl[A_ConnIdx];

		// build base path
		Tv_StrFn	=	GrStrCopy( Tv_PtrConn->StrFn, m_StrPath );

		// check position
		Tv_StrSr	=	(char*)&Tv_PtrConn->BufCont;
		Tv_StrTg	=	Tv_StrFn;
		Tv_Pos		=	A_Pos;

		Tv_Len		=	0;
		while ( Tv_Pos < Tv_PtrConn->BufSize )
		{
			if ( 0x20 == Tv_StrSr[Tv_Pos] )
			{
				// finish
				*Tv_StrTg	=	0;
				Tv_Reuslt	=	TRUE;
				break;
			}
			// copy
			*Tv_StrTg	=	Tv_StrSr[Tv_Pos];
			// next
			Tv_Pos ++;
			Tv_StrTg ++;
			Tv_Len ++;
			if ( E_GrNetPlugWebSvrFnLimit <= Tv_Len )
			{
				DbgMsgPrint( "Cls_GrNetPlugWebSvr::LcConnGetFileName - required file name is very long.\n" );
				break;
			}
		}

		// check found
		if ( Tv_Reuslt )
		{
			// init
			Tv_PtrConn->IsPtc	=	FALSE;
			// check default file
			if ( 0 == GrStrCmp( Tv_StrFn, "/", TRUE ) )
			{
				// default path
				GrStrCopy( Tv_StrFn, m_StrIdxFn );
			}
			else
			{
				// check patch item
				for ( Tv_WkIdx=0;Tv_WkIdx < E_GrNetPlugWebSvrPtcCnt;Tv_WkIdx++ )
				{
					// check exist item
					if ( m_PtcTbl[Tv_WkIdx].IsExist )
					{
						// check
						if ( 0 == GrStrCmp( Tv_StrFn, m_PtcTbl[Tv_WkIdx].StrFn, FALSE ) )
						{
							// found
							Tv_PtrConn->IsPtc		=	TRUE;
							Tv_PtrConn->PtcIdx	=	Tv_WkIdx;
							break;
						}
					}
				}
			}

		}
		
		return	Tv_Reuslt;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrNetPlugWebSvr::LcConnParsCmd( __u8 A_ConnIdx )
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrNetPlugWebSvrConn	Tv_PtrConn;
	// code --------------------
		// init
		Tv_Result	=	FALSE;
		Tv_PtrConn	=	(Ptr_GrNetPlugWebSvrConn)&m_ConnTbl[A_ConnIdx];

		// pars command
		if (0 == GrStrCmp((char*)Tv_PtrConn->BufCont, "GET ", FALSE, 4))
		{
			// update file name
			if ( LcConnGetFileName( A_ConnIdx, 4) )
			{
				// go prepare send mode
				Tv_Result	=	TRUE;
			}
			else
			{
				// finish
				Tv_PtrConn->Mode	=	E_GrNetPlugWebSvrModeFinish;
			}
		}
		else
		{
			// finish
			Tv_PtrConn->Mode	=	E_GrNetPlugWebSvrModeFinish;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrNetPlugWebSvr::LcAccept( void )
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
			if ( E_GrNetPlugWebSvrMaxConn > m_ConnCnt )
			{
				Tv_SockAcpt	=	GrSockV2Accept( m_SockListen, TRUE, &Tv_Ip, &Tv_Port, FALSE );
				if ( E_GrSockBadHandle != Tv_SockAcpt )
				{
					//find empty CONN
					for ( Tv_ConnIdx=0;E_GrNetPlugWebSvrMaxConn > Tv_ConnIdx;Tv_ConnIdx++ )
					{
						if ( E_GrNetPlugWebSvrModeNone == m_ConnTbl[Tv_ConnIdx].Mode )
						{
							//found
							m_ConnTbl[Tv_ConnIdx].Sock				=	Tv_SockAcpt;
							m_ConnTbl[Tv_ConnIdx].Mode				=	E_GrNetPlugWebSvrModeRecv;

							m_ConnTbl[Tv_ConnIdx].BufSize			=	0;
							m_ConnTbl[Tv_ConnIdx].BufPos			=	0;
							m_ConnTbl[Tv_ConnIdx].FlSize			=	0;
							m_ConnTbl[Tv_ConnIdx].Prgs				=	0;

							m_ConnTbl[Tv_ConnIdx].ParsPos			=	0;

							m_ConnTbl[Tv_ConnIdx].Fc					=	NULL;
							m_ConnTbl[Tv_ConnIdx].PtcIdx			=	0;
							m_ConnTbl[Tv_ConnIdx].IsPtc				=	FALSE;
							m_ConnTbl[Tv_ConnIdx].FndCnt			=	0;

							GrStrClear( m_ConnTbl[Tv_ConnIdx].StrFn );

							// connect mode
							m_ConnTbl[Tv_ConnIdx].Mode				=	E_GrNetPlugWebSvrModeRecv;

							m_ConnCnt ++;
							Tv_Result	=	TRUE;
							break;
						}
					}
					if ( !Tv_Result )
					{
						//not enough connection
						GrSockV2Close( Tv_SockAcpt );
						DbgMsgPrint("Cls_GrNetPlugWebSvr::LcAccept - broken connection count.\n");
					}
				}
			}
		}
		
		return	Tv_Result;		
}
//--------------------------------------------------------------------
BOOL8	Cls_GrNetPlugWebSvr::RtlBkgProc( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_ConnIdx;
	// code --------------------
		Tv_Result	=	FALSE;

		//accept
		if ( LcAccept() )
		{
			Tv_Result	=	TRUE;
		}

		//check exist client
		for ( Tv_ConnIdx=0;Tv_ConnIdx < E_GrNetPlugWebSvrMaxConn;Tv_ConnIdx++ )
		{
			switch ( m_ConnTbl[Tv_ConnIdx].Mode )
			{
				case E_GrNetPlugWebSvrModeRecv:
					if ( LcConnModeRecv( Tv_ConnIdx ) )
					{
						Tv_Result	=	TRUE;
					}
					break;
				case E_GrNetPlugWebSvrModePrepSend:
					if ( LcConnModePrepSend( Tv_ConnIdx ) )
					{
						Tv_Result	=	TRUE;
					}
					break;
				case E_GrNetPlugWebSvrModeSendHead:
					if ( LcConnModeSendHead( Tv_ConnIdx ) )
					{
						Tv_Result	=	TRUE;
					}
					break;
				case E_GrNetPlugWebSvrModePrepPtc:
					if ( LcConnModePrepPtc( Tv_ConnIdx ) )
					{
						Tv_Result	=	TRUE;
					}
					break;
				case E_GrNetPlugWebSvrModeSendPtc:
					if ( LcConnModeSendPtc( Tv_ConnIdx ) )
					{
						Tv_Result	=	TRUE;
					}
					break;
				case E_GrNetPlugWebSvrModePrepFile:
					if ( LcConnModePrepFile( Tv_ConnIdx ) )
					{
						Tv_Result	=	TRUE;
					}
					break;
				case E_GrNetPlugWebSvrModeSendFile:
					if ( LcConnModeSendFile( Tv_ConnIdx ) )
					{
						Tv_Result	=	TRUE;
					}
					break;
				case E_GrNetPlugWebSvrModeFinish:
					if ( LcConnModeFinish( Tv_ConnIdx ) )
					{
						Tv_Result	=	TRUE;
					}
					break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrNetPlugWebSvr::RtlTimer( void )
{
	// local -------------------
	// code --------------------
		LcListenSockUpdt();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrNetPlugWebSvr::LcConnModePrepPtc( __u8 A_ConnIdx )
{
	// local -------------------
		Ptr_GrNetPlugWebSvrConn	Tv_PtrConn;
	// code --------------------
		// init
		Tv_PtrConn	=	&m_ConnTbl[A_ConnIdx];

		// prepare
		Tv_PtrConn->Prgs	=	0;

		Tv_PtrConn->Mode	=	E_GrNetPlugWebSvrModeSendPtc;
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrNetPlugWebSvr::LcConnModeSendPtc( __u8 A_ConnIdx )
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrNetPlugWebSvrConn	Tv_PtrConn;
		__u32	Tv_Remain;
		void*	Tv_PtrBuf;
		__s32		Tv_Sended;
	// code --------------------
		// init
		Tv_Result	=	FALSE;
		Tv_PtrConn	=	&m_ConnTbl[A_ConnIdx];

		// check patch change
		if ( m_PtcTbl[Tv_PtrConn->PtcIdx].Size > Tv_PtrConn->Prgs )
		{
			// send
			Tv_Remain	=	m_PtcTbl[Tv_PtrConn->PtcIdx].Size - Tv_PtrConn->Prgs;
			Tv_PtrBuf	=	(void*)( (__u32)m_PtcTbl[Tv_PtrConn->PtcIdx].Buf + Tv_PtrConn->Prgs );
			Tv_Sended	=	GrSockV2Send( Tv_PtrConn->Sock, Tv_PtrBuf, Tv_Remain );
			if ( 0 < Tv_Sended )
			{
				Tv_PtrConn->Prgs	=	Tv_PtrConn->Prgs + (__u32)Tv_Sended;
				// check finish
				if ( Tv_PtrConn->Prgs >= m_PtcTbl[Tv_PtrConn->PtcIdx].Size )
				{
					// next
					Tv_PtrConn->Mode	=	E_GrNetPlugWebSvrModePrepFile;
					Tv_Result	=	TRUE;
				}
			}
			else
			{
				//check disconnected
				if ( E_GrSockErrWouldblock != GrSockV2GetErrCode() )
				{
					//socket release
					DbgMsgPrint( "Cls_GrNetPlugWebSvr::LcConnModeSendPtc - send fail\n" );
					Tv_PtrConn->Mode	=	E_GrNetPlugWebSvrModeFinish;
					Tv_Result	=	TRUE;
				}
			}

		}
		else
		{
			// patch bad finish
			DbgMsgPrint( "Cls_GrNetPlugWebSvr::LcConnModeSendPtc - invalid patch data.\n" );
			Tv_PtrConn->Mode	=	E_GrNetPlugWebSvrModeFinish;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrNetPlugWebSvr::LcConnModePrepFile( __u8 A_ConnIdx )
{
	// local -------------------
		Ptr_GrNetPlugWebSvrConn	Tv_PtrConn;
	// code --------------------
		// init
		Tv_PtrConn	=	&m_ConnTbl[A_ConnIdx];		
		
		// go send mode
		Tv_PtrConn->Fc->Seek( 0 );
		Tv_PtrConn->Prgs		=	0;
		Tv_PtrConn->BufSize	=	0;
		Tv_PtrConn->BufPos	=	0;
		Tv_PtrConn->Mode		=	E_GrNetPlugWebSvrModeSendFile;

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrNetPlugWebSvr::LcConnModeSendFile( __u8 A_ConnIdx )
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrNetPlugWebSvrConn	Tv_PtrConn;
		__u32	Tv_Remain;
		void*	Tv_PtrBuf;
		__s32		Tv_Sended;
	// code --------------------
		// init
		Tv_Result	=	FALSE;
		Tv_PtrConn	=	&m_ConnTbl[A_ConnIdx];

		// check need file read
		Tv_Remain		=	Tv_PtrConn->BufSize - Tv_PtrConn->BufPos;
		if ( 0 == Tv_Remain )
		{
			if ( Tv_PtrConn->Prgs < Tv_PtrConn->FlSize )
			{
				// read
				Tv_Remain	=	Tv_PtrConn->FlSize - Tv_PtrConn->Prgs;
				if ( E_GrNetPlugWebSvrContBufSize <= Tv_Remain )
				{
					Tv_Remain	=	E_GrNetPlugWebSvrContBufSize;
				}
				Tv_PtrConn->BufSize	=	Tv_PtrConn->Fc->Read( Tv_PtrConn->BufCont, Tv_Remain );
				Tv_PtrConn->BufPos	=	0;
				if ( 0 == Tv_PtrConn->BufSize )
				{
					// error
					DbgMsgPrint( "Cls_GrNetPlugWebSvr::LcConnModeSendFile - file %s read error.\n", Tv_PtrConn->StrFn );
					Tv_PtrConn->Mode	=	E_GrNetPlugWebSvrModeFinish;
					Tv_Result	=	TRUE;
				}
			}
			else
			{
				// finish
				Tv_PtrConn->Mode	=	E_GrNetPlugWebSvrModeFinish;
				Tv_Result	=	TRUE;
			}
		}
		else
		{
			// send
			Tv_PtrBuf		=	(void*)( (__u32)Tv_PtrConn->BufCont + Tv_PtrConn->BufPos );
			Tv_Sended		=	GrSockV2Send( Tv_PtrConn->Sock, Tv_PtrBuf, Tv_Remain );
			if ( 0 < Tv_Sended )
			{
				// send success
				Tv_PtrConn->Prgs		=	Tv_PtrConn->Prgs + (__u32)Tv_Sended;
				Tv_PtrConn->BufPos	=	Tv_PtrConn->BufPos + (__u32)Tv_Sended;
				Tv_Result	=	TRUE;
			}
			else
			{
				//check disconnected
				if ( E_GrSockErrWouldblock != GrSockV2GetErrCode() )
				{
					//socket release
					DbgMsgPrint( "Cls_GrNetPlugWebSvr::LcConnModeSendFile - send fail\n" );
					Tv_PtrConn->Mode	=	E_GrNetPlugWebSvrModeFinish;
					Tv_Result	=	TRUE;
				}
			}

		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrNetPlugWebSvr::LcConnModePrepSend( __u8 A_ConnIdx )
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrNetPlugWebSvrConn	Tv_PtrConn;
		__u32	Tv_ContSize;
		char*	Tv_StrWk;

	// code --------------------
		// init
		Tv_Result	=	FALSE;
		Tv_PtrConn	=	&m_ConnTbl[A_ConnIdx];		

		// try file open
		Tv_PtrConn->Fc	=	(Cls_GrFileCtrl*) new Cls_GrFileCtrl( Tv_PtrConn->StrFn, FALSE, FALSE );
		if ( NULL != Tv_PtrConn->Fc )
		{
			// check opened
			if ( Tv_PtrConn->Fc->IsOpened() )
			{
				// get file size
				Tv_PtrConn->FlSize	=	(__u32)Tv_PtrConn->Fc->GetSize();
				// check file size
				if ( 0 != Tv_PtrConn->FlSize )
				{
					// init
					Tv_ContSize	=	Tv_PtrConn->FlSize;
					// check PTC mode
					if ( Tv_PtrConn->IsPtc )
					{
						Tv_ContSize	=	Tv_ContSize + m_PtcTbl[Tv_PtrConn->PtcIdx].Size;
					}
					// build head
					Tv_StrWk	=	GrStrCopy((char*)Tv_PtrConn->BufCont, V_GrNetPlugWebSvrRspGetHead);
					GrStrIntToStr( Tv_StrWk, (__s32)Tv_ContSize );
					Tv_StrWk	=	GrStrCat( Tv_StrWk, "\r\n\r\n" );

					// go header send
					Tv_PtrConn->Prgs		=	0;
					Tv_PtrConn->BufSize	=	GrStrLen((char*)Tv_PtrConn->BufCont);
					Tv_PtrConn->Mode		=	E_GrNetPlugWebSvrModeSendHead;
					Tv_Result	=	TRUE;
				}
				else
				{
					// release file
					delete	Tv_PtrConn->Fc;
					Tv_PtrConn->Fc	=	NULL;

					DbgMsgPrint( "Cls_GrNetPlugWebSvr::LcConnModePrepSend - file %s empty.\n", Tv_PtrConn->StrFn );
					Tv_PtrConn->Mode	=	E_GrNetPlugWebSvrModeFinish;
					Tv_Result	=	TRUE;
				}
			}
			else
			{
				// release file
				delete	Tv_PtrConn->Fc;
				Tv_PtrConn->Fc	=	NULL;

				DbgMsgPrint( "Cls_GrNetPlugWebSvr::LcConnModePrepSend - file %s not found.\n", Tv_PtrConn->StrFn );
				Tv_PtrConn->Mode	=	E_GrNetPlugWebSvrModeFinish;
				Tv_Result	=	TRUE;
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrNetPlugWebSvr::LcConnModePrepSend - not enough memory.\n" );
			Tv_PtrConn->Mode	=	E_GrNetPlugWebSvrModeFinish;
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrNetPlugWebSvr::LcConnModeSendHead( __u8 A_ConnIdx )
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrNetPlugWebSvrConn	Tv_PtrConn;
		__u32	Tv_Remain;
		void*	Tv_PtrBuf;
		__s32		Tv_Sended;
	// code --------------------
		// init
		Tv_Result	=	FALSE;
		Tv_PtrConn	=	&m_ConnTbl[A_ConnIdx];		

		Tv_Remain		=	Tv_PtrConn->BufSize - Tv_PtrConn->Prgs;
		Tv_PtrBuf		=	(void*)( (__u32)Tv_PtrConn->BufCont + Tv_PtrConn->Prgs );

		Tv_Sended		=	GrSockV2Send( Tv_PtrConn->Sock, Tv_PtrBuf, Tv_Remain );
		if ( 0 < Tv_Sended )
		{
			Tv_PtrConn->Prgs	=	Tv_PtrConn->Prgs + (__u32)Tv_Sended;
			// check finish
			if ( Tv_PtrConn->Prgs >= Tv_PtrConn->BufSize )
			{
				// next
				Tv_PtrConn->Mode	=	E_GrNetPlugWebSvrModePrepFile;
				if ( Tv_PtrConn->IsPtc )
				{
					Tv_PtrConn->Mode	=	E_GrNetPlugWebSvrModePrepPtc;
				}
				Tv_Result	=	TRUE;
			}
		}
		else
		{
			//check disconnected
			if ( E_GrSockErrWouldblock != GrSockV2GetErrCode() )
			{
				//socket release
				DbgMsgPrint( "Cls_GrNetPlugWebSvr::LcConnModeSendHead - send fail\n" );
				Tv_PtrConn->Mode	=	E_GrNetPlugWebSvrModeFinish;
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrNetPlugWebSvr::LcConnModeFinish( __u8 A_ConnIdx )
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrNetPlugWebSvrConn	Tv_PtrConn;
	// code --------------------
		// init
		Tv_Result	=	FALSE;
		Tv_PtrConn	=	&m_ConnTbl[A_ConnIdx];		

		// release file control
		if ( NULL != Tv_PtrConn->Fc )
		{
			delete	Tv_PtrConn->Fc;
			Tv_PtrConn->Fc	=	NULL;
		}
		
		// close socket
		if ( E_GrSockBadHandle != Tv_PtrConn->Sock )
		{
			GrSockV2Close( Tv_PtrConn->Sock );
			Tv_PtrConn->Sock	=	E_GrSockBadHandle;
		}

		// mode none
		Tv_PtrConn->Mode	=	E_GrNetPlugWebSvrModeNone;
		m_ConnCnt --;

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrNetPlugWebSvr::RtlExclusive( __u32 A_Id, __u32 A_Cmd, __u32 A_Para1, __u32 A_Para2 )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( E_GrNetPlugWebSvrExclId == A_Id )
		{
			Tv_Result	=	TRUE;
			switch ( A_Cmd )
			{
				case E_GrNetPlugWebSvrExclCmdPath:
					LcSetupScriptPath((char*)A_Para1);
					break;
				case E_GrNetPlugWebSvrExclCmdDfltFn:
					LcSetupDfltFn((char*)A_Para1);
					break;
				case E_GrNetPlugWebSvrExclCmdRegPtc:
					LcSetupRegPtc( A_Para1, (Ptr_GrNetPlugWebSvrExclPtc)A_Para2 );
					break;
				case E_GrNetPlugWebSvrExclCmdPort:
					LcSetupPort( (__u16)A_Para1 );
					break;
				default:
					Tv_Result	=	FALSE;
					break;
			}
			
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrNetPlugWebSvr::LcSetupRegPtc( __u32 A_Idx, Ptr_GrNetPlugWebSvrExclPtc A_PtrPtc )
{
	// local -------------------
	// code --------------------
		if ( (E_GrNetPlugWebSvrPtcCnt > A_Idx) && (0 != A_PtrPtc->Size) && (E_GrNetPlugWebSvrPtcBufSize >= A_PtrPtc->Size) )
		{
			m_PtcTbl[A_Idx].IsExist	=	TRUE;
			m_PtcTbl[A_Idx].Size		=	A_PtrPtc->Size;
			GrStrCopy( m_PtcTbl[A_Idx].StrFn, A_PtrPtc->StrFn );
			GrDumyCopyMem( m_PtcTbl[A_Idx].Buf, A_PtrPtc->PtrBuf, A_PtrPtc->Size );
		}
}
//--------------------------------------------------------------------
void	Cls_GrNetPlugWebSvr::LcSetupDfltFn( char* A_StrFn )
{
	// local -------------------
	// code --------------------
		GrStrCopy( m_StrIdxFn, A_StrFn );
}
//--------------------------------------------------------------------
