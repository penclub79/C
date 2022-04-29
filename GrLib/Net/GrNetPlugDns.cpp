/*
 GAUSE Network DNS plug-in

 */

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <Net/GrNetPlugDns.h>
#include <Net/GrNetSvs.h>

#include <GrDebug.h>
#include <GrDumyTool.h>
#include <GrTimeTool.h>
#include <GrStrTool.h>

#include <GrError.h>

#include	<stdlib.h>


//====================================================================
//local const

#define E_GrNetPlugDnsHostAddrField		1					/* host address field */
#define E_GrNetPlugDnsArpaIntnField		1					/* ARPA internet field */

//====================================================================
//local type


//====================================================================
//local function

//====================================================================
//var

//====================================================================

//--------------------------------------------------------------------
Cls_GrNetPlugDns::Cls_GrNetPlugDns(Cls_GrTaskSvr* A_TaskSvr, Cls_GrTaskCli* A_NetSvs, __u8 A_DevIdx, __u8 A_PlugIdx) :
Cls_GrNetPlugBase(A_TaskSvr, A_NetSvs, A_DevIdx,A_PlugIdx)
{
	// local -------------------
	// code --------------------
		// init
		m_IsRecursion	=	TRUE;

		// default DNS
		m_SvIpTbl[1]	=	0;	// setup only
		m_SvIpTbl[1]	=	Mac_GrSockMakeIp(219,250,36,130);	// SK broad band
		m_SvIpTbl[2]	=	Mac_GrSockMakeIp(168,126,63,1);		// KT
		m_SvIpTbl[3]	=	Mac_GrSockMakeIp(164,124,101,2);	// LG POWERCOM
		m_SvIpTbl[4]	=	Mac_GrSockMakeIp(8,8,8,8);				// GOOGLE

		GrDumyZeroMem( m_Slot, sizeof(m_Slot) );
		// create socket
		m_Sock	=	GrSockV2Open( TRUE, TRUE, FALSE );
		if ( E_GrSockBadHandle != m_Sock )
		{
			if ( !GrSockV2UdpPortAssign( m_Sock, E_GrNetPlugDnsPort ) )
			{
				DbgMsgPrint( "Cls_GrNetPlugDns::Cls_GrNetPlugDns - port assign error!\n" );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrNetPlugDns::Cls_GrNetPlugDns - socket create error!\n" );
		}

		
		m_TaskSvr->RegistTimerClient(this, E_GrNetPlugDnsTimerPeriod, &m_HndlTimer);
		m_TaskSvr->RegistBkgClient(this);

}
//--------------------------------------------------------------------
Cls_GrNetPlugDns::~Cls_GrNetPlugDns()
{
	// local -------------------
	// code --------------------
		m_TaskSvr->UnregistBkgClient(this);
		m_TaskSvr->UnregistTimerClient(m_HndlTimer);

		//release socket
		if ( E_GrSockBadHandle != m_Sock )
		{
			GrSockV2Close( m_Sock );
		}

}
//--------------------------------------------------------------------
BOOL8	Cls_GrNetPlugDns::RtlBkgProc( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_Slot;
	// code --------------------
		Tv_Result	=	FALSE;

		// try receive
		for ( Tv_Slot=0;Tv_Slot < E_GrNetPlugDnsMaxReqCnt;Tv_Slot++ )
		{
			if ( E_GrNetPlugDnsStageTry == m_Slot[Tv_Slot].Stage )
			{
				// try request
				if ( LcStageRequest( Tv_Slot ) )
				{
					Tv_Result	=	TRUE;
				}
				if ( LcProcUdpRecv() )
				{
					Tv_Result	=	TRUE;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrNetPlugDns::RtlTimer(__u32 A_Hndl)
{
	// local -------------------
		__u8	Tv_Slot;
	// code --------------------
		for ( Tv_Slot=0;Tv_Slot < E_GrNetPlugDnsMaxReqCnt;Tv_Slot++ )
		{
			if ( E_GrNetPlugDnsStageTry == m_Slot[Tv_Slot].Stage )
			{
				m_Slot[Tv_Slot].TimeOut	++;
				if ( E_GrNetPlugDnsTimeOut <= m_Slot[Tv_Slot].TimeOut )
				{
					m_Slot[Tv_Slot].Stage	=	E_GrNetPlugDnsStageDone;
				}
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrNetPlugDns::LcProcUdpRecv( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__s32		Tv_Readed;
		__u32	Tv_Ip;
		__u16	Tv_Port;
		Ptr_GrNetPlugDnsV4Head	Tv_PtrHd;
		__u16	Tv_Slot;
		__u16	Tv_SeqId;
		Def_GrErrCode	Tv_ErrCode;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Readed		=	GrSockV2UdpRead(m_Sock, &Tv_Ip, &Tv_Port, m_RecvBuf, E_GrNetPlugDnsRecvPktSize, FALSE, &Tv_ErrCode);
		if ( E_GrNetPlugDnsPktMinSize <= Tv_Readed )
		{
			Tv_PtrHd	=	(Ptr_GrNetPlugDnsV4Head)m_RecvBuf;
			// check id
			Tv_Slot		=	(Tv_PtrHd->Id >> E_GrNetPlugDnsSlotBitPos) & E_GrNetPlugDnsSlotValMask;
			Tv_SeqId	=	Tv_PtrHd->Id & E_GrNetPlugDnsSeqIdMask;
			if ( E_GrNetPlugDnsMaxReqCnt > Tv_Slot )
			{
				// check able slot
				if ( (E_GrNetPlugDnsStageTry == m_Slot[Tv_Slot].Stage) && (Tv_SeqId == m_Slot[Tv_Slot].SeqId) )
				{
					// check flag
					if (	(0 != (E_GrNetPlugDnsFlagResponse & Tv_PtrHd->Flag)) && 
						(E_GrNetPlugDnsOpcodeQuery == (E_GrNetPlugDnsOpcodeMask & Tv_PtrHd->Flag)) && 
						(E_GrNetPlugDnsAnsNoError == (E_GrNetPlugDnsAnsCodeMask & Tv_PtrHd->Flag))
						)
					{
						// check need TCP mode
						if ( 0 != (E_GrNetPlugDnsFlagTurncate & Tv_PtrHd->Flag) )
						{
							DbgMsgPrint( "Cls_GrNetPlugDns::LcProcUdpRecv - answer turncated! need TCP requirement.!\n" );
						}
						// check answer count
						if ( 0 < Tv_PtrHd->AnsCnt )
						{
							// skip question
							m_RecvSize	=	(__u32)Tv_Readed;
							m_ParsPos		=	sizeof(St_GrNetPlugDnsV4Head);
							// skip query
							LcParsSkipQuery();
							// pars answer
							LcParsAnswer( (__u8)Tv_Slot );																
						}
						// thread active
						Tv_Result	=	TRUE;
					}
				}
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrNetPlugDns::LcParsSkipQuery( void )
{
	// local -------------------
		Ptr_GrNetPlugDnsV4Head	Tv_PtrHd;
		__u16	Tv_Remain;
	// code --------------------
		// init
		Tv_PtrHd	=	(Ptr_GrNetPlugDnsV4Head)m_RecvBuf;
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
void	Cls_GrNetPlugDns::LcParsSkipName( void )
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
void	Cls_GrNetPlugDns::LcParsAnswer( __u8 A_Slot )
{
	// local -------------------
		Ptr_GrNetPlugDnsV4Head	Tv_PtrHd;
		__u32	Tv_ParsCnt;
		__u16	Tv_Remain;
		__u8	Tv_Type[2];
		__u8	Tv_Class[2];
		__u16	Tv_AdrLen;
		__u32	Tv_Ip;
		__u8*	Tv_PtrBt;
	// code --------------------
		Tv_ParsCnt	=	0;
		Tv_PtrHd		=	(Ptr_GrNetPlugDnsV4Head)m_RecvBuf;
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
			if (	(E_GrNetPlugDnsTypeHostLb == Tv_Type[0]) && (E_GrNetPlugDnsTypeHostHb == Tv_Type[1]) &&
						(E_GrNetPlugDnsClassInLb == Tv_Class[0]) && (E_GrNetPlugDnsClassInHb == Tv_Class[1]) &&
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
void	Cls_GrNetPlugDns::LcSlotAddIp( __u8 A_Slot, __u32 A_Ip )
{
	// local -------------------
		__u8	Tv_WkIdx;
		BOOL8	Tv_IsAdd;
	// code --------------------
		if ( E_GrNetPlugDnsRsltCnt > m_Slot[A_Slot].IpCnt )
		{
			Tv_IsAdd	=	TRUE;
			// check equal ip
			if ( 0 != m_Slot[A_Slot].IpCnt )
			{
				for ( Tv_WkIdx=0;Tv_WkIdx < m_Slot[A_Slot].IpCnt;Tv_WkIdx++ )
				{
					if ( A_Ip == m_Slot[A_Slot].RsltTbl[Tv_WkIdx] )
					{
						Tv_IsAdd	=	FALSE;
						break;
					}
				}
			}
			// add IP
			if ( Tv_IsAdd )
			{
				m_Slot[A_Slot].RsltTbl[m_Slot[A_Slot].IpCnt]	=	A_Ip;
				m_Slot[A_Slot].IpCnt ++;
				if ( E_GrNetPlugDnsRsltCnt <= m_Slot[A_Slot].IpCnt )
				{
					// full
					m_Slot[A_Slot].Stage	=	E_GrNetPlugDnsStageDone;
				}
			}
		}
}
//--------------------------------------------------------------------
__s32	Cls_GrNetPlugDns::DnsQueryRequest(char* A_StrName)
{
	// local -------------------
		__s32		Tv_Result;
		__u8	Tv_WkIdx;
		__u32	Tv_StrLen;
	// code --------------------
		Tv_Result	=	-1;

		// check name length
		Tv_StrLen	=	GrStrLen( A_StrName );	
		if ( (0 != Tv_StrLen) && (100 > Tv_StrLen) )
		{
			// find empty socket
			for ( Tv_WkIdx=0;Tv_WkIdx < E_GrNetPlugDnsMaxReqCnt;Tv_WkIdx++ )
			{
				if ( E_GrNetPlugDnsStageNone == m_Slot[Tv_WkIdx].Stage )
				{
					// found
					Tv_Result	=	Tv_WkIdx;
					break;
				}
			}
			// check found
			if ( 0 <= Tv_Result )
			{
				// init slot
				m_Slot[Tv_Result].TimeOut		=	0;
				m_Slot[Tv_Result].IpCnt			=	0;
				m_Slot[Tv_Result].SendSize	=	0;
				m_Slot[Tv_Result].ReqMap		=	(1 << E_GrNetPlugDnsServerCnt) - 1;

				// check numeric ip
				if ( GrStrStripToIpV4( A_StrName, &m_Slot[Tv_Result].RsltTbl[0] ) )
				{
					m_Slot[Tv_Result].IpCnt	=	1;
					m_Slot[Tv_Result].Stage	=	E_GrNetPlugDnsStageDone;
				}
				else
				{
					// make send packet
					if ( !LcBldReqPacket( (__u8)Tv_Result, A_StrName ) )
					{
						Tv_Result	=	-1;
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrNetPlugDns::LcBldReqPacket( __u8 A_Slot, char* A_StrName )
{
	// local -------------------	
		BOOL8	Tv_Result;
		Ptr_GrNetPlugDnsV4Head	Tv_PtrHd;
		__u32	Tv_Len;
		void*	Tv_PtrBuf;
		__u32	Tv_PktSize;
	// code --------------------
		Tv_Result	=	TRUE;

		// build header
		Tv_PtrHd	=	(Ptr_GrNetPlugDnsV4Head)m_Slot[A_Slot].SendBuf;
		// make ID
		Tv_PtrHd->Id	=	m_Slot[A_Slot].SeqId | ( ((__u16)A_Slot) << E_GrNetPlugDnsSlotBitPos );
		// make flag
		Tv_PtrHd->Flag	=	E_GrNetPlugDnsFlagRcsDesire;
		// make query count
		Tv_PtrHd->QstCnt	=	0x0100;
		// other reset
		Tv_PtrHd->AnsCnt	=	0;
		Tv_PtrHd->AuthRrs	=	0;
		Tv_PtrHd->AddRrs	=	0;
		Tv_PktSize	=	sizeof(St_GrNetPlugDnsV4Head);
		// build name
		Tv_PtrBuf		=	(void*)( (__u32)m_Slot[A_Slot].SendBuf + Tv_PktSize );
		Tv_Len			=	LcBldNameStr( Tv_PtrBuf, A_StrName );
		Tv_PktSize	=	Tv_PktSize + Tv_Len;
		// build type
		m_Slot[A_Slot].SendBuf[Tv_PktSize]		=	E_GrNetPlugDnsTypeHostLb;
		m_Slot[A_Slot].SendBuf[Tv_PktSize+1]	=	E_GrNetPlugDnsTypeHostHb;
		Tv_PktSize	=	Tv_PktSize + 2;
		// build class
		m_Slot[A_Slot].SendBuf[Tv_PktSize]		=	E_GrNetPlugDnsClassInLb;
		m_Slot[A_Slot].SendBuf[Tv_PktSize+1]	=	E_GrNetPlugDnsClassInHb;
		Tv_PktSize	=	Tv_PktSize + 2;
		// request send
		if ( E_GrNetPlugDnsSendPktSize >= Tv_PktSize )
		{
			m_Slot[A_Slot].SendSize	=	(__u8)Tv_PktSize;
			m_Slot[A_Slot].Stage		=	E_GrNetPlugDnsStageTry;
		}
		else
		{
			Tv_Result	=	FALSE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrNetPlugDns::LcBldNameStr( void* A_PtrBuf, char* A_StrName )
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
BOOL8	Cls_GrNetPlugDns::LcStageRequest( __u8 A_Slot )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_Mask;
		__u8	Tv_SvIdx;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( 0 != m_Slot[A_Slot].ReqMap )
		{
			Tv_Mask	=	1;
			for ( Tv_SvIdx=0;Tv_SvIdx < E_GrNetPlugDnsServerCnt;Tv_SvIdx++ )
			{
				if ( 0 != m_SvIpTbl[Tv_SvIdx] )
				{
					if ( (__u32)m_Slot[A_Slot].SendSize == GrSockV2UdpSend( m_Sock, &m_SvIpTbl[Tv_SvIdx], 
						E_GrNetPlugDnsPort, m_Slot[A_Slot].SendBuf, (__u32)m_Slot[A_Slot].SendSize, FALSE ) )
					{
						// remove requirement
						m_Slot[A_Slot].ReqMap	=	m_Slot[A_Slot].ReqMap & ~(Tv_Mask);
						// item left
						Tv_Result	=	TRUE;
					}
				}
				else
				{
					// remove requirement
					m_Slot[A_Slot].ReqMap	=	m_Slot[A_Slot].ReqMap & ~(Tv_Mask);
					// item left
					Tv_Result	=	TRUE;
				}
				// next
				Tv_Mask	=	Tv_Mask << 1;
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrNetPlugDns::DnsQueryGetResult(__s32 A_Hndl, void* A_PtrRtIp, __u8 A_ReqCnt, __u8* A_PtrRtCnt)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_ReqCnt;
		__u8	Tv_WkIdx;
		__u32*	Tv_PtrTg;
	// code --------------------
		Tv_Result		=	TRUE;
		*A_PtrRtCnt	=	0;
		if ((0 < A_ReqCnt) && (E_GrNetPlugDnsMaxReqCnt > (__u32)A_Hndl))
		{
			Tv_Result		=	FALSE;
			if (E_GrNetPlugDnsStageDone == m_Slot[A_Hndl].Stage)
			{
				Tv_Result	=	TRUE;
				if (0 != m_Slot[A_Hndl].IpCnt)
				{
					// get ip list
					Tv_ReqCnt	=	A_ReqCnt;
					if (Tv_ReqCnt > m_Slot[A_Hndl].IpCnt)
					{
						Tv_ReqCnt	=	m_Slot[A_Hndl].IpCnt;
					}
					Tv_PtrTg	=	(__u32*)A_PtrRtIp;
					for ( Tv_WkIdx=0;Tv_WkIdx < Tv_ReqCnt;Tv_WkIdx++ )
					{
						Tv_PtrTg[Tv_WkIdx]	=	m_Slot[A_Hndl].RsltTbl[Tv_WkIdx];
					}
					*A_PtrRtCnt	=	Tv_ReqCnt;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrNetPlugDns::DnsFree(__s32 A_Hndl)
{
	// local -------------------
	// code --------------------
		if (E_GrNetPlugDnsMaxReqCnt > (__u32)A_Hndl)
		{
			if (E_GrNetPlugDnsStageNone != m_Slot[A_Hndl].Stage)
			{
				m_Slot[A_Hndl].Stage		=	E_GrNetPlugDnsStageNone;
				m_Slot[A_Hndl].SeqId ++;
				m_Slot[A_Hndl].SeqId		=	m_Slot[A_Hndl].SeqId & E_GrNetPlugDnsSeqIdMask;
				m_Slot[A_Hndl].TimeOut	=	0;
				m_Slot[A_Hndl].ReqMap		=	0;
				m_Slot[A_Hndl].SendSize	=	0;

			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrNetPlugDns::DnsIpSet(__u8 A_Idx, __u32 A_IpVal)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if (E_GrNetPlugDnsServerCnt > A_Idx)
		{
			m_SvIpTbl[A_Idx]	=	A_IpVal;
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
