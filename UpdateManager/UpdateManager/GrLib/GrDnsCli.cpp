/*
	dns client


*/

#include <GrDnsCli.h>
#include <GrSockV2.h>
#include <GrStrTool.h>
#include <GrProcess.h>
#include <GrDebug.h>
#include <GrDumyTool.h>
#include <GrError.h>

#ifdef LINUX_APP

#include <errno.h>

#endif

//--------------------------------------------------------------------
//local const

//--------------------------------------------------------------------
//local type


//--------------------------------------------------------------------
//local function


//--------------------------------------------------------------------
//global var


//====================================================================
//--------------------------------------------------------------------
Cls_GrDnsCli::Cls_GrDnsCli( Cls_GrTaskSvr* A_TaskSvr ):
Cls_GrTaskCli()
{
	// local -------------------		
	// code --------------------
		m_TaskSvr		=	A_TaskSvr;
		m_MsgAloc		=	GrTaskSvrMsgCmdAlloc();

		//init
		m_IsRecursion	=	TRUE;

		// default DNS
		m_SvIpV4		=	Mac_GrSockMakeIp(8, 8, 8, 8);				// GOOGLE
		m_Sock			=	E_GrSockBadHandle;

		m_ItemCnt		=	0;
		m_RunCnt		=	0;
		GrDumyZeroMem(m_ItemTbl, sizeof(m_ItemTbl));

		//regist
		m_TaskSvr->RegistMsgClient( this );
		m_TaskSvr->RegistTimerClient( this, E_GrDnsCliTimerPriod, &m_HndlTime );
		m_TaskSvr->RegistBkgClient( this );
}
//--------------------------------------------------------------------
Cls_GrDnsCli::~Cls_GrDnsCli()
{
	// local -------------------
	// code --------------------

		ReqFinish();

		//release 
		m_TaskSvr->UnregistBkgClient( this );
		m_TaskSvr->UnregistTimerClient( m_HndlTime );
		m_TaskSvr->UnregistMsgClient( this );

}
//--------------------------------------------------------------------
void	Cls_GrDnsCli::ImmSetup( __u32 A_DnsIp )
{
	// local -------------------
	// code --------------------
		
		m_SvIpV4	=	A_DnsIp;

}
//--------------------------------------------------------------------
BOOL8	Cls_GrDnsCli::RtlBkgProc( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if((E_GrSockBadHandle != m_Sock) && (0 != m_RunCnt))
		{
			// receive
			if(LcProcUdpRecv())
			{
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDnsCli::RtlTimer( __u32 A_Hndl )
{
	// local -------------------
		__u8	Tv_Slot;
	// code --------------------
		if(0 != m_RunCnt)
		{
			for(Tv_Slot=0; Tv_Slot < E_GrDnsCliMaxItemCnt; Tv_Slot++)
			{
				if(E_GrDnsCliStageTry == m_ItemTbl[Tv_Slot].Stage)
				{
					m_ItemTbl[Tv_Slot].TimeOut	++;
					if(E_GrDnsCliTimeOut <= m_ItemTbl[Tv_Slot].TimeOut)
					{
						// do time out
						m_ItemTbl[Tv_Slot].Stage	=	E_GrDnsCliStageDone;
						m_RunCnt --;
					}
				}
			}
		}

}
//--------------------------------------------------------------------
BOOL8	Cls_GrDnsCli::RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( m_MsgAloc == A_PtrMsg->Cmd )
		{
			Tv_Result	=	TRUE;
			switch( ((Ptr_GrDnsCliMsgHd)A_PtrMsg)->Cmd )
			{
				case  E_GrDnsCliMsgInit:
					ImmInit();
					break;
				case E_GrDnsCliMsgFinish:
					ImmFinish();
					break;
				case	E_GrDnsCliMsgSetup:
					ImmSetup( ((Ptr_GrDnsCliMsgSetup)A_PtrMsg)->DnsIp );
					break;
				case	E_GrDnsCliMsgOpen:
					*(((Ptr_GrDnsCliMsgOpen)A_PtrMsg)->PtrRt)	=	
						ImmOpen(((Ptr_GrDnsCliMsgOpen)A_PtrMsg)->StrName);
					break;
				case E_GrDnsCliMsgRead:
					*(((Ptr_GrDnsCliMsgRead)A_PtrMsg)->PtrRtRslt)	=	
						ImmRead(((Ptr_GrDnsCliMsgRead)A_PtrMsg)->Hndl,
						((Ptr_GrDnsCliMsgRead)A_PtrMsg)->PtrRtIp, ((Ptr_GrDnsCliMsgRead)A_PtrMsg)->MaxCnt,
						((Ptr_GrDnsCliMsgRead)A_PtrMsg)->PtrRtCnt);
					break;
				case	E_GrDnsCliMsgClose:
					*(((Ptr_GrDnsCliMsgClose)A_PtrMsg)->PtrRtRslt)		=
						ImmClose(((Ptr_GrDnsCliMsgClose)A_PtrMsg)->Hndl);
					break;
				default:
					Tv_Result	=	FALSE;
					break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDnsCli::ReqSetup(__u32 A_DnsIp)
{
	// local -------------------
		St_GrDnsCliMsgSetup	Tv_Msg;		
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrDnsCliMsgSetup;
		Tv_Msg.DnsIp				=	A_DnsIp;
		m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
__s32		Cls_GrDnsCli::ReqOpen(char* A_StrDns)
{
	// local -------------------
		__s32		Tv_Result;
		St_GrDnsCliMsgOpen	Tv_Msg;
	// code --------------------
		Tv_Result						=	-1;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrDnsCliMsgOpen;
		Tv_Msg.StrName			=	A_StrDns;
		Tv_Msg.PtrRt				=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDnsCli::ReqClose(__s32 A_Hndl)
{
	// local -------------------
		__u8	Tv_Result;
		St_GrDnsCliMsgClose	Tv_Msg;
	// code --------------------
		Tv_Result		=	E_GrErrReqSendFail;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrDnsCliMsgClose;
		Tv_Msg.PtrRtRslt		=	&Tv_Result;
		Tv_Msg.Hndl					=	A_Hndl;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDnsCli::ReqRead(__s32 A_Hndl, void* A_PtrRtIp, __u8 A_MaxCnt, __u8* A_PtrRtCnt)
{
	// local -------------------
		__u8	Tv_Result;
		St_GrDnsCliMsgRead	Tv_Msg;
	// code --------------------
		Tv_Result		=	E_GrErrReqSendFail;
		*A_PtrRtCnt	=	0;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrDnsCliMsgRead;
		Tv_Msg.PtrRtRslt		=	&Tv_Result;
		Tv_Msg.Hndl					=	A_Hndl;
		Tv_Msg.MaxCnt				=	A_MaxCnt;
		Tv_Msg.PtrRtIp			=	A_PtrRtIp;
		Tv_Msg.PtrRtCnt			=	A_PtrRtCnt;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDnsCli::LcProcUdpRecv( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		Def_GrErrCode	Tv_ErrCode;
		__s32		Tv_Readed;
		__u32	Tv_Ip;
		Ptr_GrDnsCliV4Head	Tv_PtrHd;
		__u16	Tv_Slot;
		__u16	Tv_SeqId;
		__u16	Tv_Port;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Readed		=	GrSockV2UdpRead(m_Sock, &Tv_Ip, &Tv_Port, m_RecvBuf, E_GrDnsCliRecvPktSize, FALSE, &Tv_ErrCode);
		if ( E_GrDnsCliPktMinSize <= Tv_Readed )
		{
			Tv_PtrHd	=	(Ptr_GrDnsCliV4Head)m_RecvBuf;
			// check id
			Tv_Slot		=	(Tv_PtrHd->Id >> E_GrDnsCliSlotBitPos) & E_GrDnsCliSlotValMask;
			Tv_SeqId	=	Tv_PtrHd->Id & E_GrDnsCliSeqIdMask;
			if(E_GrDnsCliMaxItemCnt > Tv_Slot)
			{
				// check able slot
				if ( (E_GrDnsCliStageTry == m_ItemTbl[Tv_Slot].Stage) && (Tv_SeqId == m_ItemTbl[Tv_Slot].SeqId) )
				{
					// check flag
					if (	(0 != (E_GrDnsCliFlagResponse & Tv_PtrHd->Flag)) && 
						(E_GrDnsCliOpcodeQuery == (E_GrDnsCliOpcodeMask & Tv_PtrHd->Flag)) && 
						(E_GrDnsCliAnsNoError == (E_GrDnsCliAnsCodeMask & Tv_PtrHd->Flag))
						)
					{
						// check need TCP mode
						if ( 0 != (E_GrDnsCliFlagTurncate & Tv_PtrHd->Flag) )
						{
							DbgMsgPrint( "Cls_GrDnsCli::LcProcUdpRecv - answer turncated! need TCP requirement.!\n" );
						}
						// check answer count
						if ( 0 < Tv_PtrHd->AnsCnt )
						{
							// skip question
							m_RecvSize	=	(__u32)Tv_Readed;
							m_ParsPos		=	sizeof(St_GrDnsCliV4Head);
							// skip query
							LcParsSkipQuery();
							// pars answer
							LcParsAnswer( (__u8)Tv_Slot );

							// done
							m_ItemTbl[Tv_Slot].Stage	=	E_GrDnsCliStageDone;
							m_RunCnt --;
						}
					}
				}
			}
			// thread active
			Tv_Result	=	TRUE;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDnsCli::LcSlotAddIp( __u8 A_Slot, __u32 A_Ip )
{
	// local -------------------
		__u8	Tv_WkIdx;
		BOOL8	Tv_IsAdd;
	// code --------------------
		if ( E_GrDnsCliRsltCnt > m_ItemTbl[A_Slot].IpCnt )
		{
			Tv_IsAdd	=	TRUE;
			// check equal ip
			if ( 0 != m_ItemTbl[A_Slot].IpCnt )
			{
				for ( Tv_WkIdx=0;Tv_WkIdx < m_ItemTbl[A_Slot].IpCnt;Tv_WkIdx++ )
				{
					if ( A_Ip == m_ItemTbl[A_Slot].RsltTbl[Tv_WkIdx] )
					{
						Tv_IsAdd	=	FALSE;
						break;
					}
				}
			}
			// add IP
			if ( Tv_IsAdd )
			{
				m_ItemTbl[A_Slot].RsltTbl[m_ItemTbl[A_Slot].IpCnt]	=	A_Ip;
				m_ItemTbl[A_Slot].IpCnt ++;
				if ( E_GrDnsCliRsltCnt <= m_ItemTbl[A_Slot].IpCnt )
				{
					// full
					m_ItemTbl[A_Slot].Stage	=	E_GrDnsCliStageDone;
					m_RunCnt --;
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDnsCli::LcParsSkipQuery( void )
{
	// local -------------------
		Ptr_GrDnsCliV4Head	Tv_PtrHd;
		__u16	Tv_Remain;
	// code --------------------
		// init
		Tv_PtrHd	=	(Ptr_GrDnsCliV4Head)m_RecvBuf;
		Tv_Remain	=	GrDumyEndian2byteGet( &Tv_PtrHd->QstCnt );
		// parse name
		if ( 0 != Tv_Remain )
		{
			while ( m_RecvSize > m_ParsPos )
			{
				// skip name
				LcParsSkipName();
				m_ParsPos	=	m_ParsPos + 4;		// type and class skip
				Tv_Remain --;
				if ( 0 == Tv_Remain )
				{
					break;
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDnsCli::LcParsSkipName( void )
{
	// local -------------------
		__u32	Tv_JmpSize;
	// code --------------------
		while ( m_RecvSize > m_ParsPos )
		{
			if ( 0 == m_RecvBuf[m_ParsPos] )
			{
				m_ParsPos ++;
				break;
			}
			else if ( 0x3F >= m_RecvBuf[m_ParsPos] )
			{
				Tv_JmpSize	=	(__u32)m_RecvBuf[m_ParsPos] + 1;
				m_ParsPos		=	m_ParsPos + Tv_JmpSize;
			}
			else
			{
				m_ParsPos	=	m_ParsPos + 2;
				break;
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDnsCli::LcParsAnswer( __u8 A_Slot )
{
	// local -------------------
		Ptr_GrDnsCliV4Head	Tv_PtrHd;
		__u32	Tv_ParsCnt;
		__u16	Tv_Remain;
		__u8	Tv_Type[2];
		__u8	Tv_Class[2];
		__u16	Tv_AdrLen;
		__u32	Tv_Ip;
		__u8*	Tv_PtrBt;
	// code --------------------
		Tv_ParsCnt	=	0;
		Tv_PtrHd		=	(Ptr_GrDnsCliV4Head)m_RecvBuf;
		Tv_Remain		=	GrDumyEndian2byteGet( &Tv_PtrHd->AnsCnt );
		while ( m_RecvSize > m_ParsPos )
		{
			LcParsSkipName();		// name skip
			// get type
			Tv_Type[0]	=	m_RecvBuf[m_ParsPos];
			Tv_Type[1]	=	m_RecvBuf[m_ParsPos + 1];
			m_ParsPos	=	m_ParsPos + 2;
			// get class
			Tv_Class[0]	=	m_RecvBuf[m_ParsPos];
			Tv_Class[1]	=	m_RecvBuf[m_ParsPos + 1];
			m_ParsPos	=	m_ParsPos + 2;
			// skip TTL
			m_ParsPos	=	m_ParsPos + 4;
			// get address length
			Tv_AdrLen	=	(__u16)m_RecvBuf[m_ParsPos + 1] | (((__u16)m_RecvBuf[m_ParsPos]) << 8);
			m_ParsPos	=	m_ParsPos + 2;
			// get address
			if (	(E_GrDnsCliTypeHostLb == Tv_Type[0]) && (E_GrDnsCliTypeHostHb == Tv_Type[1]) &&
						(E_GrDnsCliClassInLb == Tv_Class[0]) && (E_GrDnsCliClassInHb == Tv_Class[1]) &&
						(4 == Tv_AdrLen) )
			{
				// update IP address
				Tv_PtrBt	=	(__u8*)&Tv_Ip;
				Tv_PtrBt[0]	=	m_RecvBuf[m_ParsPos];
				Tv_PtrBt[1]	=	m_RecvBuf[m_ParsPos + 1];
				Tv_PtrBt[2]	=	m_RecvBuf[m_ParsPos + 2];
				Tv_PtrBt[3]	=	m_RecvBuf[m_ParsPos + 3];
				// add ip
				LcSlotAddIp( A_Slot, Tv_Ip );
			}
			m_ParsPos	=	m_ParsPos + Tv_AdrLen;
			// next
			Tv_Remain --;
			if ( 0 == Tv_Remain )
			{
				break;
			}
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrDnsCli::LcBldNameStr(void* A_PtrBuf, char* A_StrName)
{
	// local -------------------
		__u32	Tv_Result;
		__u8*	Tv_PtrTg;
		__u8*	Tv_PtrCnt;
		char*	Tv_PtrSr;
	// code --------------------
		Tv_Result	=	1;
		Tv_PtrSr		=	A_StrName;
		Tv_PtrCnt		=	(__u8*)A_PtrBuf;
		Tv_PtrTg		=	(__u8*)( (__u32)Tv_PtrCnt + 1 );
		*Tv_PtrCnt	=	0;
		while ( 0 != *Tv_PtrSr )
		{
			if ( '.' == *Tv_PtrSr )
			{
				Tv_PtrCnt		=	Tv_PtrTg;
				*Tv_PtrCnt	=	0;
			}
			else
			{
				(*Tv_PtrCnt) ++;
				*Tv_PtrTg	=	(__u8)*Tv_PtrSr;
			}
			// next
			Tv_PtrSr ++;
			Tv_PtrTg ++;
			Tv_Result ++;
		}
		// add last mark
		*Tv_PtrTg	=	0;
		Tv_Result ++;

		return	Tv_Result;
}
//--------------------------------------------------------------------
__s32	Cls_GrDnsCli::ImmOpen(char* A_StrName)
{
	// local -------------------
		__s32		Tv_Result;
		__u8		Tv_WkIdx;
		__u16		Tv_SeqId;
		__u32		Tv_StrLen;
		__u32		Tv_PktSize;
	// code --------------------
		Tv_Result	=	-1;

		// check socket exist
		if( (E_GrSockBadHandle != m_Sock) && (0 != m_SvIpV4))
		{
			// check name length
			Tv_StrLen	=	GrStrLen(A_StrName);
			if((0 != Tv_StrLen) && (100 > Tv_StrLen))
			{
				// find empty socket
				for(Tv_WkIdx=0; Tv_WkIdx < E_GrDnsCliMaxItemCnt; Tv_WkIdx++)
				{
					if(E_GrDnsCliStageNone == m_ItemTbl[Tv_WkIdx].Stage)
					{
						// found
						Tv_Result	=	Tv_WkIdx;
						break;
					}
				}
				// check found
				if(0 <= Tv_Result)
				{
					// init slot
					m_ItemTbl[Tv_Result].TimeOut		=	0;
					m_ItemTbl[Tv_Result].IpCnt			=	0;

					// check numeric ip
					if(GrStrStripToIpV4(A_StrName, &m_ItemTbl[Tv_Result].RsltTbl[0]))
					{
						m_ItemTbl[Tv_Result].IpCnt	=	1;
						m_ItemTbl[Tv_Result].Stage	=	E_GrDnsCliStageDone;
						m_ItemCnt ++;
					}
					else
					{
						// build backup
						Tv_SeqId		=	m_ItemTbl[Tv_Result].SeqId | ((__u16)Tv_Result << E_GrDnsCliSlotBitPos);
						Tv_PktSize	=	LcBldReqPacket(A_StrName, Tv_SeqId);

						__u32		Tv_SendSize;

						Tv_SendSize		= GrSockV2UdpSend(m_Sock, &m_SvIpV4, E_GrDnsCliPort, m_SendBuf, Tv_PktSize,FALSE);
						// send packet
						if(Tv_PktSize == Tv_SendSize)
						{
							// ok
							m_ItemTbl[Tv_Result].Stage	=	E_GrDnsCliStageTry;
							m_ItemCnt ++;
							m_RunCnt ++;
						}
						else
						{
							DbgMsgPrint("Cls_GrDnsCli::ImmOpen() - send failed. size = %d\n", Tv_SendSize);
							// fail
#ifdef LINUX_APP
							DbgMsgPrint("Cls_GrDnsCli::ImmRequest - errorno = %d\n",errno);
#endif
							m_ItemTbl[Tv_Result].Stage	=	E_GrDnsCliStageNone;
							Tv_Result	=	-1;
							DbgMsgPrint("Cls_GrDnsCli::ImmRequest - packet send fail(ip:%d.%d.%d.%d)\n", 
								(m_SvIpV4 & 0xFF), ((m_SvIpV4 >> 8) & 0xFF), ((m_SvIpV4 >> 16) & 0xFF), ((m_SvIpV4 >> 24) & 0xFF));
						}
					}
				}
				else
				{
					DbgMsgPrint("Cls_GrDnsCli::LcRequest - slot full!\n");
				}
			}
			else
			{
				DbgMsgPrint("Cls_GrDnsCli::LcRequest - bad URL length!\n");
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrDnsCli::LcRequest - not initialized!\n");
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrDnsCli::LcBldReqPacket(char* A_StrName, __u16 A_SeqId)
{
	// local -------------------	
		__u32	Tv_Result;
		Ptr_GrDnsCliV4Head	Tv_PtrHd;
		__u32	Tv_Len;
		void*	Tv_PtrBuf;
	// code --------------------
		Tv_Result	=	0;

		// build header
		Tv_PtrHd	=	(Ptr_GrDnsCliV4Head)m_SendBuf;
		// make ID
		Tv_PtrHd->Id		=	A_SeqId;
		// make flag
		Tv_PtrHd->Flag		=	E_GrDnsCliFlagRcsDesire;
		// make query count
		GrDumyEndian2byteSet(&Tv_PtrHd->QstCnt, 1);		// question count
		// other reset
		Tv_PtrHd->AnsCnt		=	0;
		Tv_PtrHd->AuthRrs	=	0;
		Tv_PtrHd->AddRrs		=	0;
		Tv_Result				=	sizeof(St_GrDnsCliV4Head);
		// build name
		Tv_PtrBuf		=	(void*)((Def_GrCalPtr)m_SendBuf + (Def_GrCalPtr)Tv_Result);
		Tv_Len			=	LcBldNameStr( Tv_PtrBuf, A_StrName );
		Tv_Result	=	Tv_Result + Tv_Len;
		// build type
		m_SendBuf[Tv_Result]		=	E_GrDnsCliTypeHostLb;
		m_SendBuf[Tv_Result+1]	=	E_GrDnsCliTypeHostHb;
		Tv_Result	=	Tv_Result + 2;
		// build class
		m_SendBuf[Tv_Result]		=	E_GrDnsCliClassInLb;
		m_SendBuf[Tv_Result+1]	=	E_GrDnsCliClassInHb;
		Tv_Result	=	Tv_Result + 2;

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDnsCli::ImmClose(__s32 A_Hndl)
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrErrBadParam;

		if((0 <= A_Hndl) && (E_GrDnsCliMaxItemCnt > A_Hndl))
		{
			if(E_GrDnsCliStageNone != m_ItemTbl[A_Hndl].Stage)
			{
				if(E_GrDnsCliStageTry == m_ItemTbl[A_Hndl].Stage)
				{
					m_RunCnt --;
				}
				m_ItemCnt --;

				// reset
				m_ItemTbl[A_Hndl].Stage		=	E_GrDnsCliStageNone;
				m_ItemTbl[A_Hndl].SeqId ++;
				m_ItemTbl[A_Hndl].SeqId		=	m_ItemTbl[A_Hndl].SeqId & E_GrDnsCliSeqIdMask;
				m_ItemTbl[A_Hndl].TimeOut	=	0;

				// success
				Tv_Result	=	E_GrErrNone;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDnsCli::ImmRead(__s32 A_Hndl, void* A_PtrRtIp, __u8 A_MaxCnt, __u8* A_PtrRtCnt)
{
	// local -------------------
		__u8	Tv_Result;
		__u8	Tv_ReqCnt;
		__u8	Tv_WkIdx;
		__u32*	Tv_PtrTg;
	// code --------------------
		Tv_Result		=	E_GrErrBadParam;
		*A_PtrRtCnt	=	0;
		if((0 < A_MaxCnt) && (0 <= A_Hndl) && (E_GrDnsCliMaxItemCnt > A_Hndl))
		{
			if(E_GrDnsCliStageDone == m_ItemTbl[A_Hndl].Stage)
			{
				if(0 != m_ItemTbl[A_Hndl].IpCnt)
				{
					// get ip list
					Tv_ReqCnt	=	A_MaxCnt;
					if(Tv_ReqCnt > m_ItemTbl[A_Hndl].IpCnt)
					{
						Tv_ReqCnt	=	m_ItemTbl[A_Hndl].IpCnt;
					}
					Tv_PtrTg	=	(__u32*)A_PtrRtIp;
					for ( Tv_WkIdx=0;Tv_WkIdx < Tv_ReqCnt;Tv_WkIdx++ )
					{
						Tv_PtrTg[Tv_WkIdx]	=	m_ItemTbl[A_Hndl].RsltTbl[Tv_WkIdx];
					}
					*A_PtrRtCnt	=	Tv_ReqCnt;
				}
				Tv_Result	=	E_GrErrNone;
				m_RunCnt--;
			}
			else if(E_GrDnsCliStageTry == m_ItemTbl[A_Hndl].Stage)
			{
				Tv_Result	=	E_GrErrBusy;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDnsCli::ImmInit(void)
{
	// local -------------------
	// code --------------------
		GrSockV2Init();

		// create socket
		m_Sock	=	GrSockV2Open(TRUE, TRUE, FALSE);
		if(E_GrSockBadHandle != m_Sock)
		{
			if(GrSockV2UdpPortAssign(m_Sock, E_GrDnsCliPort))
			{
				DbgMsgPrint("Cls_GrDnsCli::ImmInit - initialize succes.\n");
			}
			else
			{
				DbgMsgPrint("Cls_GrDnsCli::ImmInit - port assign error!\n");
				GrSockV2Close(m_Sock);
				m_Sock	=	E_GrSockBadHandle;
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrDnsCli::ImmInit - socket create error!\n");
		}
}
//--------------------------------------------------------------------
void	Cls_GrDnsCli::ImmFinish(void)
{
	// local -------------------
	// code --------------------
		if(E_GrSockBadHandle != m_Sock)
		{
			GrSockV2Close(m_Sock);
			m_Sock	=	E_GrSockBadHandle;
		}

		GrSockV2Finish();
}
//--------------------------------------------------------------------
void	Cls_GrDnsCli::ReqInit(void)
{
	// local -------------------
		St_GrDnsCliMsgHd	Tv_Msg;
	// code --------------------
		Tv_Msg.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Cmd				=	E_GrDnsCliMsgInit;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));
}
//--------------------------------------------------------------------
void	Cls_GrDnsCli::ReqFinish(void)
{
	// local -------------------
		St_GrDnsCliMsgHd	Tv_Msg;
	// code --------------------
		Tv_Msg.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Cmd				=	E_GrDnsCliMsgFinish;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));
}
//--------------------------------------------------------------------
__u32	Cls_GrDnsCli::ReqDnsIpGet(void)
{
	return	m_SvIpV4;
}
//--------------------------------------------------------------------
