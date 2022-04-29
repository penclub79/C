/*
	UPnP Client
*/

#include	<GrUpnpCli.h>
//#include	<DvrRtl.h>
#include	<GrStrTool.h>
#include	<GrProcess.h>
#include	<GrDumyTool.h>
#include	<GrTimeTool.h>
#include	<GrDebug.h>
#include	<GrStrTool.h>
#include	<stdio.h>

#include <GrNetBase.h>

#ifdef LINUX_APP
#include	<arpa/inet.h>
#endif

//--------------------------------------------------------------------
//local const
#define E_GrUpnpCliTimerPriod		1000
#define E_GrUpnpCliMaxFindCnt		64
#define E_GrUpnpCliMaxErr				3
#define E_GrUpnpCliModelCnt			3

//UPNP mode
#define E_GrUpnpCliStageNone					0		/* stage none */
#define E_GrUpnpCliStageUrlSend				1		/* UDP UNICAST send */
#define E_GrUpnpCliStageUrlRecv				2		/* UDP UNICAST receive */
#define E_GrUpnpCliStageDnsUpdt				3		/* server ip get DNS */
#define E_GrUpnpCliStageAddPort				4		/* port forward apply */
#define E_GrUpnpCliStageAddPktSend		5		/* port forward message send */
#define E_GrUpnpCliStageAddPktRecv		6		/* port forward message receive */
#define E_GrUpnpCliStageDelPort				7		/* delete port */
#define E_GrUpnpCliStageDelPktSend		8		/* delete port message send */
#define E_GrUpnpCliStageDelPktRecv		9		/* delete port message receive */
#define E_GrUpnpCliStageGetPort				10
#define E_GrUpnpCliStageGetPktSend		11
#define E_GrUpnpCliStageGetPktRecv		12
#define E_GrUpnpCliStageModel					13
#define E_GrUpnpCliStageModelPktSend	14
#define E_GrUpnpCliStageModelPktRecv	15

//stage state
#define	E_GrUpnpCliStateNone		0
#define	E_GrUpnpCliStateFindSv	1
#define	E_GrUpnpCliStateRegist	2
#define	E_GrUpnpCliStateFail		3
#define	E_GrUpnpCliStateOk			4
#define	E_GrUpnpCliStateDel			5
#define	E_GrUpnpCliStateGet			6

//HTTP find string mode
#define	E_GrUpnpCliFindDev			0
#define	E_GrUpnpCliFindAddr			1
#define	E_GrUpnpCliParseAddr		2
#define	E_GrUpnpCliParsePort		3
#define	E_GrUpnpCliFindUrl			4
#define	E_GrUpnpCliParseUrl			5

//HTTP find string mode
#define E_GrUpnpCliParseAck			0				//acknowledge
#define E_GrUpnpCliParsePortSt	1				//external port
#define E_GrUpnpCliParsePortEd	2				//external port
#define E_GrUpnpCliParseProtSt	3				//external port
#define E_GrUpnpCliParseProtEd	4				//external port
#define E_GrUpnpCliParseNameSt	5				//description
#define E_GrUpnpCliParseNameEd	6				//description
#define E_GrUpnpCliParseCmpCode	7				//compare webcode

//http find model string mode
#define E_GrUpnpCliParseNone		0
#define E_GrUpnpCliParseMdAck		1				//acknowledge
#define E_GrUpnpCliParseNameHd	2				//model description header
#define E_GrUpnpCliParseNdName	3				//model description name
#define E_GrUpnpCliParseSvcType	4				//service type name(urn:schemas-upnp-org:service:WANIPConnection:1)
#define E_GrUpnpCliParseCtrlHd	5				//control URL header
#define E_GrUpnpCliParseCtrlUrl	6				//control URL name

#define E_GrUpnpCliMsgCmdReqApply		0				/* request upnp apply : St_GrUpnpCliMsgReqApply */
#define E_GrUpnpCliMsgCmdReqCancel	1				/* request upnp cancel : St_GrUpnpCliMsgReqCancel */

//--------------------------------------------------------------------
//local type
typedef	struct St_GrUpnpCliMsgReqApply
{
	St_GrTaskMsgHd		MsgHd;
	__u32		Cmd;
	BOOL8*		PtrRslt;
}	*Ptr_GrUpnpCliMsgReqApply;

typedef	struct St_GrUpnpCliMsgReqDel
{
	St_GrTaskMsgHd		MsgHd;
	Cls_GrTaskCli*		CbObj;
	__u32		CliPort;
	__u32		Cmd;
	BOOL8		IsUdp;
	BOOL8*		PtrRslt;
}	*Ptr_GrUpnpCliMsgReqDel;

//--------------------------------------------------------------------
//local macro

//--------------------------------------------------------------------
//local function

//--------------------------------------------------------------------
//global var


//====================================================================
//--------------------------------------------------------------------
Cls_GrUpnpCli::Cls_GrUpnpCli( Cls_GrTaskSvr* A_TaskSvr ):
Cls_GrTaskCli()
{
	// local -------------------
		__u32	Tv_WkCnt;
	// code --------------------
		m_TaskSvr		=	A_TaskSvr;
		m_MsgAloc		=	GrTaskSvrMsgCmdAlloc();

		m_Sock		=	E_GrSockBadHandle;
		m_IntfIdx	=	0;
		m_ErrCnt	=	0;
		m_CliIdx	=	0;
		m_ListCnt	=	0;
		m_PtrSv		=	NULL;
		m_Stage		=	E_GrUpnpCliStageNone;
		m_TimeOutCnt	=	10;

		m_DevType	=	E_GrNetUpnpDevTypeGateway;

		GrDumyZeroMem( m_SvTbl, sizeof(m_SvTbl) );
		for ( Tv_WkCnt = 0; Tv_WkCnt < E_GrUpnpCliMaxSvCnt; Tv_WkCnt++ )
		{
			m_SvTbl[Tv_WkCnt].Dns	=	-1;
		}

		/*GrStrCopy( m_DevList[0], "urn:schemas-upnp-org:device:InternetGatewayDevice:1" );
		GrStrCopy( m_DevList[1], "urn:schemas-upnp-org:service:WANIPConnection:1" );
		GrStrCopy( m_DevList[2], "urn:schemas-upnp-org:service:WANPPPConnection:1" );
		GrStrCopy( m_DevList[3], "upnp:rootdevice" );*/

		m_TaskSvr->RegistBkgClient( this );
		m_TaskSvr->RegistMsgClient( this );
		m_TaskSvr->RegistTimerClient( this, E_GrUpnpCliTimerPriod, &m_TimerSec );
}
//--------------------------------------------------------------------
Cls_GrUpnpCli::~Cls_GrUpnpCli( void )
{
	// local -------------------
		__u32	Tv_WkCnt;
	// code --------------------

		ReqFinish();

		m_TaskSvr->UnregistBkgClient( this );
		m_TaskSvr->UnregistMsgClient( this );
		m_TaskSvr->UnregistTimerClient( m_TimerSec );

}
//--------------------------------------------------------------------
BOOL8	Cls_GrUpnpCli::RtlBkgProc( void )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		Tv_Result		=	FALSE;

		if(LcBkgStage())
		{
			Tv_Result	=	TRUE;
		}
		
		return	Tv_Result;

}
//--------------------------------------------------------------------
BOOL8	Cls_GrUpnpCli::LcBkgStage(void)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		switch(m_Stage)
		{
			case E_GrUpnpCliStageDiscverV4:
				Tv_Result	=	LcStageDiscoverV4();
				break;
			case E_GrUpnpCliStageReadDiscvV4:
				Tv_Result	=	LcStageReadDiscvV4();
				break;

			case E_GrUpnpCliStageFinish:
				Tv_Result	=	LcStageFinish();
				break;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrUpnpCli::LcMakePortFwListMsgV4(__u32 A_DevIdx, __u32 A_EntryIdx)
{
	// local -------------------
		PBYTE	Tv_PtrSv;
		__s32		Tv_UrlLen;
		Def_StrHuge	Tv_StrBuf;

		char*		Tv_StrWk;
	// code --------------------
		
#if 0
		// cgi contents
		Tv_StrWk	=	GrStrCopy(m_Buf,)



		Tv_PtrSv	=	(PBYTE)&A_PtrSv->Ip;
		sprintf(A_SendBuf,
			"<?xml version=\"1.0\"?>\r\n"
			"<SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://schemas.xmlsoap.org/soap/envelope/\" "
			"SOAP-ENV:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\"><SOAP-ENV:Body>"
			"<m:GetGenericPortMappingEntry xmlns:m=\"urn:schemas-upnp-org:service:WANIPConnection:1\">"
			"<NewPortMappingIndex xmlns:dt=\"urn:schemas-microsoft-com:datatypes\" dt:dt=\"ui2\">%d</NewPortMappingIndex>"
			"</m:GetGenericPortMappingEntry></SOAP-ENV:Body></SOAP-ENV:Envelope>\r\n",
			m_FindIdx);
		Tv_UrlLen	=	GrStrLen(A_SendBuf);



		// cgi head 
		Tv_StrWk	=	GrStrCopy(m_CgiHdBuf, "POST ");
		Tv_StrWk	=	GrStrCopy(Tv_StrWk, m_DevTbl[A_DevIdx].StrCgi);
		Tv_StrWk	=	GrStrCopy(Tv_StrWk, " HTTP/1.1\r\n"
			"Content-Type: text/xml; charset=\"utf-8\"\r\n"
			"SOAPAction: \"urn:schemas-upnp-org:service:WANIPConnection:1#GetGenericPortMappingEntry\"\r\n"
			"User-Agent: Mozilla/4.0 (compatible; UPnP/1.0; Windows 9x)\r\n"
			"Host: ");
		GrStrIpToStr(Tv_StrWk, m_DevTbl[A_DevIdx].IpV4);
		Tv_StrWk	=	GrStrCat(Tv_StrWk, ":");
		GrStrIntToStr(Tv_StrWk, (__s32)m_DevTbl[A_DevIdx].Port);
		Tv_StrWk	=	GrStrCat(Tv_StrWk, "\r\n"
			"Content-Length: "
			
			
			%d\r\n"
			"Connection: Keep-Alive\r\n"
			"Cache-Control: no-cache\r\n"
			"Pragma: no-cache\r\n"
			"\r\n");
			
			

		Tv_StrWk	=	GrStrCopy(m_Buf,"POST %s HTTP/1.1\r\n"
			"Content-Type: text/xml; charset=\"utf-8\"\r\n"
			"SOAPAction: \"urn:schemas-upnp-org:service:WANIPConnection:1#GetGenericPortMappingEntry\"\r\n"
			"User-Agent: Mozilla/4.0 (compatible; UPnP/1.0; Windows 9x)\r\n"
			"Host: %d.%d.%d.%d:%d\r\n"
			"Content-Length: %d\r\n"
			"Connection: Keep-Alive\r\n"
			"Cache-Control: no-cache\r\n"
			"Pragma: no-cache\r\n"
			"\r\n",



		Tv_PtrSv	=	(PBYTE)&A_PtrSv->Ip;
		sprintf( A_SendBuf,
			"<?xml version=\"1.0\"?>\r\n"
			"<SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://schemas.xmlsoap.org/soap/envelope/\" "
			"SOAP-ENV:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\"><SOAP-ENV:Body>"
			"<m:GetGenericPortMappingEntry xmlns:m=\"urn:schemas-upnp-org:service:WANIPConnection:1\">"
			"<NewPortMappingIndex xmlns:dt=\"urn:schemas-microsoft-com:datatypes\" dt:dt=\"ui2\">%d</NewPortMappingIndex>"
			"</m:GetGenericPortMappingEntry></SOAP-ENV:Body></SOAP-ENV:Envelope>\r\n",
			m_FindIdx );
		Tv_UrlLen	=	GrStrLen( A_SendBuf );

		sprintf( Tv_StrBuf,
			"POST %s HTTP/1.1\r\n"
			"Content-Type: text/xml; charset=\"utf-8\"\r\n"
			"SOAPAction: \"urn:schemas-upnp-org:service:WANIPConnection:1#GetGenericPortMappingEntry\"\r\n"
			"User-Agent: Mozilla/4.0 (compatible; UPnP/1.0; Windows 9x)\r\n"
			"Host: %d.%d.%d.%d:%d\r\n"
			"Content-Length: %d\r\n"
			"Connection: Keep-Alive\r\n"
			"Cache-Control: no-cache\r\n"
			"Pragma: no-cache\r\n"
			"\r\n",
			A_PtrSv->CtrlUrl, Tv_PtrSv[0], Tv_PtrSv[1], Tv_PtrSv[2], Tv_PtrSv[3], A_PtrSv->Port, Tv_UrlLen );

		GrStrIns( A_SendBuf, Tv_StrBuf, 0 );
#endif

		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUpnpCli::LcStageGetPortPktSend()
{
	// var ---------------------
		BOOL8	Tv_Result;
		__s32		Tv_MsgLen;
		__s32		Tv_SendLen;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		if ( E_GrSockBadHandle != m_Sock )
		{
			Tv_MsgLen		=	GrStrLen( m_Buf );
			Tv_SendLen	=	GrSockV2Send( m_Sock, m_Buf + m_SendLen, Tv_MsgLen - m_SendLen );
			if ( 0 <= Tv_SendLen )
			{
				Tv_Result		=	TRUE;
				m_SendLen		= m_SendLen + Tv_SendLen;
				if ( m_SendLen >= Tv_MsgLen )
				{
					m_Stage				=	E_GrUpnpCliStageGetPktRecv;
					m_RecvLen			=	0;
					m_TimeOutCnt	=	10;
				}
			}
			else
			{
				DbgMsgPrint("Cls_GrUpnpCli::LcStageGetPortPktSend - tcp get port information message send fail\n");
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrUpnpCli::LcParseIsResigtPort( char* A_SrBuf )
{
	// var ---------------------
		__u8		Tv_Result;
		__u32		Tv_Mode;
		__s32			Tv_WkCnt;
		__s32			Tv_MaxLen;
		char*		Tv_StrBuf;

		char*		Tv_StrType;
		__s32			Tv_TypeLen;

		__s32			Tv_Port;
		__u32		Tv_NameIdx;
		__u8		Tv_ProtType;
		Def_StrHuge		Tv_StrName;
	// code --------------------
		Tv_Result	=	E_GrUpnpCliStageDelPort;
		Tv_StrBuf	=	A_SrBuf;
		Tv_Mode		=	E_GrUpnpCliParseAck;
		Tv_MaxLen	=	GrStrLen( A_SrBuf );
		if ( E_GrUpnpCliRecvBufLen < Tv_MaxLen )
		{
			Tv_MaxLen	=	E_GrUpnpCliRecvBufLen - 1;
		}
		Tv_WkCnt	=	0;

		Tv_StrType	=	E_GrUpnpCliAckMsg;
		Tv_TypeLen	=	GrStrLen( Tv_StrType );
		while( Tv_WkCnt < Tv_MaxLen )
		{
			switch( Tv_Mode )
			{
			case E_GrUpnpCliParseAck:
				if ( 0 == GrStrCmp(Tv_StrBuf + Tv_WkCnt, Tv_StrType, FALSE, Tv_TypeLen) )
				{
					Tv_WkCnt	=	Tv_WkCnt + Tv_TypeLen;

					//next
					Tv_StrType	=	"<NewExternalPort>";
					Tv_TypeLen	=	GrStrLen( Tv_StrType );
					Tv_Mode			=	E_GrUpnpCliParsePortSt;
				}
				else if ( 0 == GrStrCmp(Tv_StrBuf + Tv_WkCnt, E_GrUpnpCliNakMsg, FALSE, GrStrLen(E_GrUpnpCliNakMsg)) )
				{
					m_CliIdx	=	0;
					Tv_Result	=	E_GrUpnpCliStageAddPort;
					if ( 0 == m_CliIdx )
					{
						//m_CliInfo[0].ExtrPort		=	DvrRtlCfgNetCmsPortGet();
						//m_CliInfo[0].InterPort	=	DvrRtlCfgNetCmsPortGet();
						//m_CliInfo[1].ExtrPort		=	DvrRtlCfgNetCmsPortGet();
						//m_CliInfo[1].InterPort	=	DvrRtlCfgNetCmsPortGet();
					}
					else
					{
						//m_CliInfo[2].ExtrPort		=	DvrRtlCfgWebSvrPortGet();
						//m_CliInfo[2].InterPort	=	DvrRtlCfgWebSvrPortGet();
					}
					//exit
					Tv_WkCnt	=	Tv_MaxLen;
				}
				break;
			case E_GrUpnpCliParsePortSt:
				if ( 0 == GrStrCmp(Tv_StrBuf + Tv_WkCnt, Tv_StrType, FALSE, Tv_TypeLen) )
				{
					//next
					Tv_WkCnt		=	Tv_WkCnt + Tv_TypeLen - 1;
					Tv_StrType	=	"</NewExternalPort>";
					Tv_TypeLen	=	GrStrLen( Tv_StrType );
					Tv_Mode			=	E_GrUpnpCliParsePortEd;
					Tv_NameIdx	=	0;
				}
				break;
			case E_GrUpnpCliParsePortEd:
				if ( 0 == GrStrCmp(Tv_StrBuf + Tv_WkCnt, Tv_StrType, FALSE, Tv_TypeLen) )
				{
					Tv_StrName[Tv_NameIdx]	=	0;
					
					GrStrStrToInt( Tv_StrName, &Tv_Port );
					Tv_WkCnt	=	Tv_WkCnt + Tv_TypeLen - 1;
					Tv_Mode		=	E_GrUpnpCliParseProtSt;
					Tv_StrType	=	"<NewProtocol>";
					Tv_TypeLen	=	GrStrLen( Tv_StrType );
				}
				else
				{
					Tv_StrName[Tv_NameIdx]	=	Tv_StrBuf[Tv_WkCnt];
					Tv_NameIdx ++;
				}
				break;
			case E_GrUpnpCliParseProtSt:
				if ( 0 == GrStrCmp(Tv_StrBuf + Tv_WkCnt, Tv_StrType, FALSE, Tv_TypeLen) )
				{
					//next
					Tv_WkCnt		=	Tv_WkCnt + Tv_TypeLen - 1;
					Tv_StrType	=	"</NewProtocol>";
					Tv_TypeLen	=	GrStrLen( Tv_StrType );
					Tv_Mode			=	E_GrUpnpCliParseProtEd;
					Tv_NameIdx	=	0;
				}
				break;
			case E_GrUpnpCliParseProtEd:
				if ( 0 == GrStrCmp(Tv_StrBuf + Tv_WkCnt, Tv_StrType, FALSE, Tv_TypeLen) )
				{
					Tv_StrName[Tv_NameIdx]	=	0;
					Tv_ProtType	=	1;		//udp
					if ( 0 == GrStrCmp(Tv_StrName, "TCP", FALSE) )
					{
						Tv_ProtType	=	0;			//tcp
					}

					Tv_WkCnt	=	Tv_WkCnt + Tv_TypeLen;
					Tv_Mode		=	E_GrUpnpCliParseNameSt;
					Tv_StrType	=	"<NewPortMappingDescription>";
					Tv_TypeLen	=	GrStrLen( Tv_StrType );
				}
				else
				{
					Tv_StrName[Tv_NameIdx]	=	Tv_StrBuf[Tv_WkCnt];
					Tv_NameIdx ++;
				}
				break;
			case E_GrUpnpCliParseNameSt:
				if ( 0 == GrStrCmp(Tv_StrBuf + Tv_WkCnt, Tv_StrType, FALSE, Tv_TypeLen) )
				{
					//next
					Tv_WkCnt		=	Tv_WkCnt + Tv_TypeLen - 1;
					Tv_StrType	=	"</NewPortMappingDescription>";
					Tv_TypeLen	=	GrStrLen( Tv_StrType );
					Tv_Mode			=	E_GrUpnpCliParseNameEd;

					Tv_NameIdx	=	0;
				}
				break;
			case E_GrUpnpCliParseNameEd:
				if ( 0 == GrStrCmp(Tv_StrBuf + Tv_WkCnt, Tv_StrType, FALSE, Tv_TypeLen) )
				{
					//exit
					Tv_Mode		=	E_GrUpnpCliParseCmpCode;
					Tv_StrName[Tv_NameIdx]	=	0;
				}
				else
				{
					Tv_StrName[Tv_NameIdx]	=	Tv_StrBuf[Tv_WkCnt];
					Tv_NameIdx ++;
				}
				break;
			case E_GrUpnpCliParseCmpCode:
				//compare
				__u8		Tv_CliCnt;
				BOOL8		Tv_IsDel;
				Tv_IsDel	=	FALSE;
				Tv_Result	=	E_GrUpnpCliStageGetPort;
				for ( Tv_CliCnt = 0; Tv_CliCnt < E_GrUpnpCliApplyCilCnt; Tv_CliCnt++ )
				{
					if ( 0 == GrStrCmp(Tv_StrName, m_CliInfo[Tv_CliCnt].Tiltle, FALSE) )
					{
						Tv_IsDel	=	TRUE;
						Tv_Result	=	E_GrUpnpCliStageDelPort;
						m_CliInfo[Tv_CliCnt].ExtrPort		=	Tv_Port;
						m_CliInfo[Tv_CliCnt].InterPort	=	Tv_Port;
						m_CliInfo[Tv_CliCnt].Protocol		=	Tv_ProtType;
						m_CliIdx		=	Tv_CliCnt;
						m_IsAllDel	=	FALSE;
						break;
					}
					else
					{
#if 0
						__u16		Tv_Num;
						if ( Tv_Port == m_CliInfo[Tv_CliCnt].ExtrPort )
						{
							Tv_Num	=	Tv_Port + 1;
							m_CliInfo[Tv_CliCnt].ExtrPort		=	Tv_Num;
							m_CliInfo[Tv_CliCnt].InterPort	=	Tv_Num;
							if ( 0 == Tv_CliCnt )
							{
								if ( Tv_Num == DvrRtlCfgWebSvrPortGet() )
									Tv_Num ++;

								DvrRtlCfgNetCmsPortSet( Tv_Num );

							}
							else
							{
								if ( Tv_Num == DvrRtlCfgNetCmsPortGet() )
									Tv_Num ++;

								DvrRtlCfgWebSvrPortSet( Tv_Num );
							}
							DvrRtlCfgSave();
						}
#endif
					}
				}

				if ( !Tv_IsDel )
				{
					m_FindIdx ++;
				}
					
				//exit
				Tv_WkCnt	=	Tv_MaxLen;
				break;
			}
			Tv_WkCnt ++;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUpnpCli::LcStageGetPortPktRecv()
{
	// var ---------------------
		BOOL8	Tv_Result;
		__s32		Tv_RecvLen;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( E_GrSockBadHandle != m_Sock )
		{
			Tv_RecvLen	=	GrSockV2Recv( m_Sock, m_Buf + m_RecvLen, E_GrUpnpCliRecvBufLen - m_RecvLen );
			if ( 0 >= Tv_RecvLen )
			{
				if ( GrSockV2ChkCloseByRecv( Tv_RecvLen ) )
				{
					DbgMsgPrint("Cls_GrUpnpCli::LcStageGetPortPktRecv - get information success\n");
					m_Stage			=	LcParseIsResigtPort( m_Buf );

					//next
					m_TimeOutCnt	=	10;

					//release
					if ( E_GrSockBadHandle != m_Sock )
					{
						GrSockV2Close( m_Sock );
						m_Sock	=	E_GrSockBadHandle;
					}
				}
			}
			else
			{
				Tv_Result		=	TRUE;
				m_RecvLen		=	m_RecvLen + Tv_RecvLen;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUpnpCli::LcStageFinish( void )
{
	// local -------------------
		__u32		Tv_WkCnt;
	// code --------------------
		//release
		if ( E_GrSockBadHandle != m_Sock )
		{
			GrSockV2Close( m_Sock );
			m_Sock	=	E_GrSockBadHandle;
		}

		m_Stage		=	E_GrUpnpCliStageNone;
		m_TimeOut	=	0;
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUpnpCli::ReqUpnpApply( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrUpnpCliMsgReqApply	Tv_Msg;
		// code --------------------
		Tv_Result			=	FALSE;

		Tv_Msg.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Cmd				=	E_GrUpnpCliMsgCmdReqApply;
		Tv_Msg.PtrRslt		=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUpnpCli::ReqUpnpCancel()
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrUpnpCliMsgReqApply	Tv_Msg;
	// code --------------------
		Tv_Result			=	FALSE;

		Tv_Msg.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Cmd				=	E_GrUpnpCliMsgCmdReqCancel;
		Tv_Msg.PtrRslt		=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUpnpCli::RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( m_MsgAloc == A_PtrMsg->Cmd )
		{
			Tv_Result	=	TRUE;
			switch(((Ptr_GrUpnpCliMsgHd)A_PtrMsg)->Cmd)
			{
				case E_GrUpnpCliCmdInit:
					ImmInit();
					break;
				case E_GrUpnpCliCmdFinish:
					ImmFinish();
					break;
				default:
					DbgMsgPrint("Cls_GrUpnpCli::RtlMsgProc - unknown message command %d\n", ((Ptr_GrUpnpCliMsgHd)A_PtrMsg)->Cmd);
					break;
			}

		}
		else
		{
			// inherited
			Tv_Result	=	Cls_GrTaskCli::RtlMsgProc(A_PtrMsg);
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUpnpCli::LcReqUpnpApply( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_IpBrd;
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		if ( E_GrSockBadHandle != m_Sock )
		{
			GrSockV2Close( m_Sock );
			m_Sock	=	E_GrSockBadHandle;
		}
		
		//make socket
		m_Sock	=	GrSockV2Open( TRUE, TRUE ,FALSE);
		if ( E_GrSockBadHandle != m_Sock )
		{
			Tv_IpBrd	=	E_GrUpnpCliBroadAddr;
			if(GrSockV2UdpBroad(m_Sock, &Tv_IpBrd, m_ReqDevIdx ,FALSE))
			{
				m_SendReq			=	LcMakeDiscvMsgV4( m_Buf );
				Tv_Result			=	TRUE;

				//next
				m_ListCnt			=	0;
				m_Stage				=	E_GrUpnpCliStageUrlSend;
				m_SendLen			=	0;
				m_TimeOutCnt	=	10;						//connection timeout
				m_CliIdx			=	0;
				m_FindIdx			=	0;
				m_IsAllDel		=	TRUE;

				LcUpdtClientTable();
			}
			else
			{
				//fail
				DbgMsgPrint("Cls_GrUpnpCli::LcReqUpnpApply - not option!\n");
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrUpnpCli::LcReqUpnpApply - not open socket!\n");
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUpnpCli::LcStageAddPortApply( void )
{
	// local ------------------
		BOOL8		Tv_Result;
	// code -------------------
		//init
		Tv_Result		=	FALSE;
		if ( m_PtrSv->IsUse )
		{
			if ( LcTcpOpen(m_PtrSv) )
			{
				//make get external ip message
				LcMakeAddPortMsg( m_Buf, m_PtrSv, &m_CliInfo[m_CliIdx] );

				//next
				m_Stage		=	E_GrUpnpCliStageAddPktSend;
				m_SendLen	=	0;
				Tv_Result	=	TRUE;
				m_TimeOutCnt	=	10;
			}
			else
			{
				DbgMsgPrint("Cls_GrUpnpCli::LcStageAddPortApply - tcp open fail\n");
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUpnpCli::LcTcpOpen( Ptr_NetCtrlSvInfo A_PtrSv )
{
	// local ------------------
		BOOL8		Tv_Result;
	// code -------------------
		Tv_Result	=	FALSE;

		//release
		if ( E_GrSockBadHandle != m_Sock )
		{
			GrSockV2Close( m_Sock );
			m_Sock	=	E_GrSockBadHandle;
		}

		//open new socket
		m_Sock	=	GrSockV2Open( FALSE, TRUE,FALSE );
		if ( 0 <= GrSockV2Connect(m_Sock, &A_PtrSv->Ip, (__u16)A_PtrSv->Port,FALSE) )
		{
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUpnpCli::LcMakeAddPortMsg( char*	A_SendBuf, Ptr_NetCtrlSvInfo A_PtrSv, 
	Ptr_GrUpnpCliCliInfo A_CliInfo )
{
	// var ---------------------
		char*		Tv_StrMsg;
		PBYTE		Tv_PtrSv, Tv_PtrCli;
		__s32			Tv_StrLen;
		Def_StrHuge			Tv_StrArgument;
	// code --------------------
		//init
		Tv_StrMsg		=	A_SendBuf;

		//make event message
		Tv_PtrSv	=	(PBYTE)&A_PtrSv->Ip;
		Tv_PtrCli	=	(PBYTE)&A_CliInfo->ClientIp;
		sprintf( Tv_StrArgument,
			"<?xml version=\"1.0\"?>\r\n"
			"<SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://schemas.xmlsoap.org/soap/envelope/\" "
			"SOAP-ENV:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">"
			"<SOAP-ENV:Body><m:AddPortMapping xmlns:m=\"urn:schemas-upnp-org:service:WANIPConnection:1\">"
			//"<NewRemoteHost xmlns:dt=\"urn:schemas-microsoft-com:datatypes\" dt:dt=\"string\">%d.%d.%d.%d</NewRemoteHost>"
			"<NewRemoteHost xmlns:dt=\"urn:schemas-microsoft-com:datatypes\" dt:dt=\"string\"></NewRemoteHost>"
			"<NewExternalPort xmlns:dt=\"urn:schemas-microsoft-com:datatypes\" dt:dt=\"ui2\">%d</NewExternalPort>"
			"<NewProtocol xmlns:dt=\"urn:schemas-microsoft-com:datatypes\" dt:dt=\"string\">%s</NewProtocol>"
			"<NewInternalPort xmlns:dt=\"urn:schemas-microsoft-com:datatypes\" dt:dt=\"ui2\">%d</NewInternalPort>"
			"<NewInternalClient xmlns:dt=\"urn:schemas-microsoft-com:datatypes\" dt:dt=\"string\">%d.%d.%d.%d</NewInternalClient>"
			"<NewEnabled xmlns:dt=\"urn:schemas-microsoft-com:datatypes\" dt:dt=\"boolean\">1</NewEnabled>"
			"<NewPortMappingDescription xmlns:dt=\"urn:schemas-microsoft-com:datatypes\" dt:dt=\"string\">%s</NewPortMappingDescription>"
			"<NewLeaseDuration xmlns:dt=\"urn:schemas-microsoft-com:datatypes\" dt:dt=\"ui4\">%d</NewLeaseDuration>"
			"</m:AddPortMapping></SOAP-ENV:Body></SOAP-ENV:Envelope>\r\n",
			//Tv_PtrSv[0], Tv_PtrSv[1], Tv_PtrSv[2], Tv_PtrSv[3],
			A_CliInfo->ExtrPort, (A_CliInfo->Protocol)?"UDP":"TCP", A_CliInfo->InterPort,
			Tv_PtrCli[0], Tv_PtrCli[1], Tv_PtrCli[2], Tv_PtrCli[3],
			A_CliInfo->Tiltle, A_CliInfo->LeaseTime );

		__s32	 Tv_Len;
		Tv_Len	=	GrStrLen( Tv_StrArgument );

		//make http message
		Tv_StrLen		=	GrStrLen( Tv_StrArgument );
		sprintf( Tv_StrMsg,
			"POST %s HTTP/1.1\r\n"
			"Content-Type: text/xml; charset=\"utf-8\"\r\n"
			"SOAPAction: \"urn:schemas-upnp-org:service:WANIPConnection:1#AddPortMapping\"\r\n"
			"User-Agent: Mozilla/4.0 (compatible; UPnP/1.0; Windows 9x)\r\n"
			"Host: %d.%d.%d.%d:%d\r\n"
			"Content-Length: %d\r\n"
			"Connection: Keep-Alive\r\n"
			"Cache-Control: no-cache\r\n"
			"Pragma: no-cache\r\n"
			"\r\n"
			"%s",
			A_PtrSv->CtrlUrl, Tv_PtrSv[0], Tv_PtrSv[1], Tv_PtrSv[2], Tv_PtrSv[3],
			A_PtrSv->Port, Tv_StrLen, Tv_StrArgument );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUpnpCli::LcStageAddPortPktSend()
{
	// var ---------------------
		BOOL8		Tv_Result;
		__s32			Tv_SendLen;
		__s32			Tv_MsgLen;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		if ( E_GrSockBadHandle != m_Sock )
		{
			Tv_MsgLen		=	GrStrLen( m_Buf );
			Tv_SendLen	=	GrSockV2Send( m_Sock, m_Buf + m_SendLen, Tv_MsgLen - m_SendLen );
			if ( 0 <= Tv_SendLen )
			{
				Tv_Result		=	TRUE;
				m_SendLen		= m_SendLen + Tv_SendLen;
				if ( m_SendLen >= Tv_MsgLen )
				{
					m_Stage		=	E_GrUpnpCliStageAddPktRecv;
					m_RecvLen	=	0;
					m_TimeOutCnt	=	10;
				}
			}
			else
			{
				DbgMsgPrint("Cls_GrUpnpCli::LcStageAddPortPktSend - tcp add port packet send fail\n");
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUpnpCli::LcParseGetAnswer(char* A_SrBuf, BOOL8* A_PtrIsAck)
{
	// var ---------------------
		__s32			Tv_MaxLen;
		__s32			Tv_WkCnt;
		__s32			Tv_AckLen;
		__s32			Tv_NakLen;
		BOOL8		Tv_Result;
		char*		Tv_StrBuf;

		__u32		Tv_Mode;
	// code --------------------
		Tv_StrBuf	=	A_SrBuf;
		Tv_MaxLen	=	GrStrLen( Tv_StrBuf );
		if ( E_GrUpnpCliRecvBufLen < Tv_MaxLen )
		{
			Tv_MaxLen	=	E_GrUpnpCliRecvBufLen - 1;
		}
		
		Tv_Result	=	FALSE;

		Tv_WkCnt	=	0;
		Tv_Mode		=	E_GrUpnpCliParseAck;
		Tv_AckLen	=	GrStrLen( E_GrUpnpCliAckMsg );
		Tv_NakLen	=	GrStrLen( E_GrUpnpCliNakMsg );
		while( !Tv_Result && Tv_WkCnt < Tv_MaxLen )
		{
			switch( Tv_Mode )
			{
			case E_GrUpnpCliParseAck:
				if ( 0 == GrStrCmp(Tv_StrBuf + Tv_WkCnt, E_GrUpnpCliAckMsg, FALSE, Tv_AckLen) )
				{
					Tv_Result		=	TRUE;
					*A_PtrIsAck	=	TRUE;
				}
				else if ( 0 == GrStrCmp(Tv_StrBuf + Tv_WkCnt, E_GrUpnpCliNakMsg, FALSE, Tv_NakLen) )
				{
					Tv_Result		=	TRUE;
					*A_PtrIsAck	=	FALSE;
				}
				break;
			}

			//next
			Tv_WkCnt++;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUpnpCli::LcStageAddPortPktRecv()
{
	// var ---------------------
		BOOL8		Tv_Result;
		__s32			Tv_RecvLen;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		if ( E_GrSockBadHandle != m_Sock )
		{
			Tv_RecvLen	=	GrSockV2Recv( m_Sock, m_Buf + m_RecvLen, E_GrUpnpCliRecvBufLen - m_RecvLen );
			if ( 0 >= Tv_RecvLen )
			{
				if ( GrSockV2ChkCloseByRecv( Tv_RecvLen ) )
				{
					BOOL8	Tv_IsAck;
					// go to finish
					if ( LcParseGetAnswer(m_Buf, &Tv_IsAck) )
					{
						if ( Tv_IsAck )
						{
							//next
							m_CliIdx ++;
							m_TimeOutCnt	=	10;
							if ( E_GrUpnpCliApplyCilCnt == m_CliIdx )
							{
								m_Stage		=	E_GrUpnpCliStageNone;
								m_ErrCnt	=	0;
							}
							else
							{
								m_Stage	=	E_GrUpnpCliStageAddPort;
							}
						}
						else
						{
							m_Stage		=	E_GrUpnpCliStageGetPort;
						}
						
						//release
						if ( E_GrSockBadHandle != m_Sock )
						{
							GrSockV2Close( m_Sock );
							m_Sock	=	E_GrSockBadHandle;
						}
						DbgMsgPrint("Cls_GrUpnpCli::LcStageAddPortPktRecv - add port success\n");
					}
					else
					{
						DbgMsgPrint("Cls_GrUpnpCli::LcStageAddPortPktRecv - add port fail\n");
					}
				}
			}
			else
			{
				Tv_Result		=	TRUE;
				m_RecvLen		=	m_RecvLen + Tv_RecvLen;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUpnpCli::LcParseHttpUrl( char* A_SrBuf, Ptr_NetCtrlSvInfo A_PtrSv )
{
	// var ---------------------
		BOOL8		Tv_Result;
		__u32		Tv_WkCnt;
		__u32		Tv_MaxLen;
		Def_StrName		Tv_StrAddr;
		Def_StrNum		Tv_StrPort;
		__u32		Tv_AddrIdx;
		__u32		Tv_Mode;

		Ptr_NetCtrlSvInfo		Tv_PtrSv;
		char*		Tv_StrType;
		__u32		Tv_TypeLen;
	// code --------------------
		//init
		Tv_Result	=	FALSE;
		Tv_MaxLen	=	GrStrLen( A_SrBuf );
		if ( E_GrUpnpCliRecvBufLen < Tv_MaxLen )
		{
			Tv_MaxLen	=	E_GrUpnpCliRecvBufLen - 1;
		}
		Tv_WkCnt	=	0;
		Tv_PtrSv	=	A_PtrSv;
		Tv_PtrSv->IsUse	=	FALSE;
		m_ListCnt		=	0;

		Tv_StrType	=	E_GrUpnpCliAckMsg;
		Tv_TypeLen	=	GrStrLen( Tv_StrType );

		Tv_Mode			=	E_GrUpnpCliFindDev;
		Tv_AddrIdx	=	0;
		while( Tv_WkCnt < Tv_MaxLen )
		{
			switch( Tv_Mode )
			{
				case	E_GrUpnpCliFindDev:
					if ( 0 == GrStrCmp(A_SrBuf + Tv_WkCnt, Tv_StrType, FALSE, Tv_TypeLen) )
					{
						Tv_WkCnt	=	Tv_WkCnt + Tv_TypeLen;		//jump

						//next find
						Tv_Mode			=	E_GrUpnpCliFindAddr;
						Tv_StrType	=	"http://";
						Tv_TypeLen	=	GrStrLen( Tv_StrType );
					}
					break;
				case	E_GrUpnpCliFindAddr:
					if ( 0 == GrStrCmp(A_SrBuf + Tv_WkCnt, Tv_StrType, FALSE, Tv_TypeLen) )
					{
						Tv_WkCnt	=	Tv_WkCnt + Tv_TypeLen - 1;		//jump

						//next find
						Tv_Mode			=	E_GrUpnpCliParseAddr;
						Tv_AddrIdx	=	0;
					}
					break;
				case	E_GrUpnpCliParseAddr:
					if ( ':' == A_SrBuf[Tv_WkCnt] )
					{
						Tv_StrAddr[Tv_AddrIdx]	=	0;
						GrStrCopy( Tv_PtrSv->StrIp, Tv_StrAddr );
						//Tv_PtrSv->Dns	=	GrSockDnsReq( Tv_PtrSv->StrIp );
						//next
						Tv_Mode			=	E_GrUpnpCliParsePort;
						Tv_AddrIdx	=	0;
					}
					else
					{
						Tv_StrAddr[Tv_AddrIdx]	=	A_SrBuf[Tv_WkCnt];
						Tv_AddrIdx ++;
					}
					break;
				case	E_GrUpnpCliParsePort:
					if ( ('0' > A_SrBuf[Tv_WkCnt]) || ('9' < A_SrBuf[Tv_WkCnt]) || (30 < Tv_AddrIdx) )
					{
						Tv_StrPort[Tv_AddrIdx]	=	0;
						GrStrStrToInt( Tv_StrPort, (__s32*)&Tv_PtrSv->Port );
						//next
						Tv_Mode			=	E_GrUpnpCliParseUrl;
						Tv_AddrIdx	=	0;
						Tv_WkCnt	--;
					}
					else
					{
						Tv_StrPort[Tv_AddrIdx]	=	A_SrBuf[Tv_WkCnt];
						Tv_AddrIdx ++;
					}
					break;
				case	E_GrUpnpCliParseUrl:
					//get URL
					if ( (0x0D == A_SrBuf[Tv_WkCnt]) || (0x0A == A_SrBuf[Tv_WkCnt]) )
					{
						//find success
						Tv_PtrSv->Url[Tv_AddrIdx]	=	0;
						Tv_PtrSv->IsUse	=	TRUE;
						Tv_Result				=	TRUE;
						m_ListCnt	++;

						//next server info
						Tv_PtrSv++;
						Tv_PtrSv->IsUse	=	FALSE;
						Tv_Mode	=	E_GrUpnpCliStageNone;
					}
					else
					{
						Tv_PtrSv->Url[Tv_AddrIdx]	=	A_SrBuf[Tv_WkCnt];
						Tv_AddrIdx ++;
					}
					break;
			}

			//next
			Tv_WkCnt++;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUpnpCli::LcStageDnsUpdt( void )
{
	// local -------------------
		__u8		Tv_IpCnt;
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	TRUE;
		if ( NULL != m_PtrSv )
		{
			if ( m_PtrSv->IsUse )
			{
				if ( 0 > m_PtrSv->Dns )
				{
					DbgMsgPrint("Cls_GrUpnpCli::LcStageDnsUpdt - %s\n", m_PtrSv->StrIp);
					if ( GrStrIsExist(m_PtrSv->StrIp) )
					{
						//request DNS
						//m_PtrSv->Dns	=	GrSockDnsReq( m_PtrSv->StrIp );
					}
				}
				else
				{
					//if ( GrSockDnsGetResult( m_PtrSv->Dns, &m_PtrSv->Ip, 1, &Tv_IpCnt ) )
					{
						if ( 0 != m_PtrSv->Ip )
						{
							//next stage
							m_Stage				=	E_GrUpnpCliStageModel;
							m_TimeOutCnt	=	10;
							m_CliIdx			=	0;
							DbgMsgPrint("Cls_GrUpnpCli::LcStageDnsUpdt - dns update\n");
						}
						else
						{
							Tv_Result		=	FALSE;
							DbgMsgPrint("Cls_GrUpnpCli::LcStageDnsUpdt - upnp %d server dns error\n", m_SvIdx);
						}

						//GrSockDnsFree( m_PtrSv->Dns );
						m_PtrSv->Dns	=	-1;
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUpnpCli::RtlTimer( __u32 A_Hndl )
{
	// local -------------------
		__u32	Tv_WkCnt;
	// code --------------------
		if ( E_GrUpnpCliStageNone != m_Stage )
		{
			if(0 == m_TimeOut)
			{
				return;
			}

			m_TimeOut --;
			if ( 0 == m_TimeOutCnt )
			{
				// check now stage
				if(E_GrUpnpCliStageReadDiscvV4 == m_Stage)
				{
					// go next stage
#ifdef E_GrUpnpCliDebug
					DbgMsgPrint("[UPNP] type %d dev %d found!\n", m_DevType,m_DevCnt);
#endif
					// check command
					switch(m_ReqCmd)
					{
						case E_GrUpnpCliReqPortForward:
							m_Stage	=	E_GrUpnpCliStagePortFwPrepare;
							break;
						default:
							DbgMsgPrint("Cls_GrUpnpCli::RtlTimer - unknown request %d!\n", m_ReqCmd);
							m_Stage	=	E_GrUpnpCliStageFinish;
							break;
					}
				}
				else
				{
					// timeout
					DbgMsgPrint("Cls_GrUpnpCli::RtlTimer - timeout. stage = %d.\n", m_Stage);
					m_Stage	=	E_GrUpnpCliStageFinish;
				}
			}
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrUpnpCli::LcMakeDiscvMsgV4(void*	A_PtrBuf)
{
	// local -------------------
		__s8*	Tv_StrBuf;
		char*	Tv_StrWk;
	// code --------------------
		// init
		Tv_StrBuf	=	(__s8*)A_PtrBuf;
		GrStrClear( Tv_StrBuf );

		Tv_StrWk	=	GrStrCopy(Tv_StrBuf, "M-SEARCH * HTTP/1.1\r\n"
			"HOST: 239.255.255.250:1900\r\n");

		if(E_GrNetUpnpDevTypeGateway == m_DevType)
		{
			Tv_StrWk	=	GrStrCopy(Tv_StrWk, "ST:urn:schemas-upnp-org:device:InternetGatewayDevice:1\r\n");
		}

		Tv_StrWk	=	GrStrCopy(Tv_StrWk, "MAN: \"ssdp:discover\"\r\n"
			"MX:3\r\n"
			"\r\n");

		return	GrStrLen( Tv_StrBuf );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUpnpCli::LcStageDelPortApply( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//make socket
		if ( 0 != m_ListCnt )
		{
			m_PtrSv	=	m_SvTbl;
			if ( m_PtrSv->IsUse )
			{
				if ( LcTcpOpen(m_PtrSv) )
				{
					LcMakeDelPortMsg( m_Buf, m_PtrSv, &m_CliInfo[m_CliIdx] );
					Tv_Result			=	TRUE;

					//next
					m_Stage				=	E_GrUpnpCliStageDelPktSend;
					m_SendLen			=	0;
					m_TimeOutCnt	=	10;						//connection timeout
				}
				else
				{
					//fail
					DbgMsgPrint("Cls_GrUpnpCli::LcReqUpnpDelete - not open!\n");
				}
			}
		}
		else
		{
			//fail
			DbgMsgPrint("Cls_GrUpnpCli::LcReqUpnpDelete - not exist server list!\n");
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUpnpCli::LcMakeDelPortMsg( char* A_SendBuf, Ptr_NetCtrlSvInfo A_PtrSv, Ptr_GrUpnpCliCliInfo A_CliInfo )
{
	// local -------------------
		PBYTE	Tv_PtrSv;
		__s32		Tv_UrlLen;
		Def_StrHuge	Tv_StrBuf;
	// code --------------------
		Tv_PtrSv	=	(PBYTE)&A_PtrSv->Ip;
		sprintf( A_SendBuf,
			"<?xml version=\"1.0\"?>\r\n"
			"<SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://schemas.xmlsoap.org/soap/envelope/\" "
			"SOAP-ENV:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">"
			"<SOAP-ENV:Body><m:DeletePortMapping xmlns:m=\"urn:schemas-upnp-org:service:WANIPConnection:1\">"
			//"<NewRemoteHost xmlns:dt=\"urn:schemas-microsoft-com:datatypes\" dt:dt=\"string\">%d.%d.%d.%d</NewRemoteHost>"
			"<NewRemoteHost xmlns:dt=\"urn:schemas-microsoft-com:datatypes\" dt:dt=\"string\"></NewRemoteHost>"
			"<NewExternalPort xmlns:dt=\"urn:schemas-microsoft-com:datatypes\" dt:dt=\"ui2\">%d</NewExternalPort>"
			"<NewProtocol xmlns:dt=\"urn:schemas-microsoft-com:datatypes\" dt:dt=\"string\">%s</NewProtocol>"
			"</m:DeletePortMapping></SOAP-ENV:Body></SOAP-ENV:Envelope>\r\n",
			/*Tv_PtrSv[0], Tv_PtrSv[1], Tv_PtrSv[2], Tv_PtrSv[3], */A_CliInfo->ExtrPort, (A_CliInfo->Protocol)?"UDP":"TCP" );
		Tv_UrlLen	=	GrStrLen( A_SendBuf );

		sprintf( Tv_StrBuf,
			"POST %s HTTP/1.1\r\n"
			"Content-Type: text/xml; charset=\"utf-8\"\r\n"
			"SOAPAction: \"urn:schemas-upnp-org:service:WANIPConnection:1#DeletePortMapping\"\r\n"
			"User-Agent: Mozilla/4.0 (compatible; UPnP/1.0; Windows 9x)\r\n"
			"Host: %d.%d.%d.%d:%d\r\n"
			"Content-Length: %d\r\nConnection: Keep-Alive\r\n"
			"Cache-Control: no-cache\r\n"
			"Pragma: no-cache\r\n"
			"\r\n",
			A_PtrSv->CtrlUrl, Tv_PtrSv[0], Tv_PtrSv[1], Tv_PtrSv[2], Tv_PtrSv[3], A_PtrSv->Port, Tv_UrlLen );

		GrStrIns( A_SendBuf, Tv_StrBuf, 0 );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUpnpCli::LcStageDelPortPktSend()
{
	// var ---------------------
		BOOL8		Tv_Result;
		__s32			Tv_SendLen;
		__s32			Tv_MsgLen;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		if ( E_GrSockBadHandle != m_Sock )
		{
			Tv_MsgLen		=	GrStrLen( m_Buf );
			Tv_SendLen	=	GrSockV2Send( m_Sock, m_Buf + m_SendLen, Tv_MsgLen - m_SendLen );
			if ( 0 <= Tv_SendLen )
			{
				Tv_Result		=	TRUE;
				m_SendLen		= m_SendLen + Tv_SendLen;
				if ( m_SendLen >= Tv_MsgLen )
				{
					m_Stage		=	E_GrUpnpCliStageDelPktRecv;
					m_RecvLen	=	0;
					m_TimeOutCnt	=	10;
				}
			}
			else
			{
				DbgMsgPrint("Cls_GrUpnpCli::tcp del port message send fail\n");
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUpnpCli::LcStageDelPortPktRecv()
{
	// var ---------------------
		BOOL8		Tv_Result;
		__s32			Tv_RecvLen;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		if ( E_GrSockBadHandle != m_Sock )
		{
			Tv_RecvLen	=	GrSockV2Recv( m_Sock, m_Buf + m_RecvLen, E_GrUpnpCliRecvBufLen - m_RecvLen );
			if ( 0 >= Tv_RecvLen )
			{
				if ( GrSockV2ChkCloseByRecv( Tv_RecvLen ) )
				{
					BOOL8	Tv_IsAck;
					if ( LcParseGetAnswer(m_Buf, &Tv_IsAck) )
					{
						DbgMsgPrint("Cls_GrUpnpCli::LcStageDelPortPktRecv - delete success\n");
					}
					else
					{
						DbgMsgPrint("Cls_GrUpnpCli::LcStageDelPortPktRecv - del port fail\n");
					}

					//next
					m_TimeOutCnt	=	10;
					if ( m_IsAllDel )
					{
						m_CliIdx ++;
						if ( E_GrUpnpCliApplyCilCnt == m_CliIdx )
						{
							m_Stage		=	E_GrUpnpCliStageGetPort;
							m_CliIdx	=	0;
							LcUpdtClientTable();
						}
						else
						{
							m_Stage	=	E_GrUpnpCliStageDelPort;
						}
					}
					else
					{
						m_Stage		=	E_GrUpnpCliStageGetPort;
						m_CliIdx	=	0;
						LcUpdtClientTable();
					}

					//release
					if ( E_GrSockBadHandle != m_Sock )
					{
						GrSockV2Close( m_Sock );
						m_Sock	=	E_GrSockBadHandle;
					}
				}
			}
			else
			{
				Tv_Result		=	TRUE;
				m_RecvLen		=	m_RecvLen + Tv_RecvLen;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUpnpCli::LcUpdtClientTable()
{
		//set client info - CMS TCP
		//m_CliInfo[0].ExtrPort		=	DvrRtlCfgNetCmsPortGet();
		m_CliInfo[0].Protocol		=	0;								//TCP:0, UDP:1
		//m_CliInfo[0].InterPort	=	DvrRtlCfgNetCmsPortGet();
		GrSockV2GetMyIp(0, &m_CliInfo[0].ClientIp,FALSE);
		m_CliInfo[0].LeaseTime	=	0;								//max time
		GrStrCopy( m_CliInfo[0].Tiltle, "CMS_" );
		//GrStrCat( m_CliInfo[0].Tiltle, (__s8*)DvrRtlNetWebcodeGet() );

		//set client info - CMS UDP
		//m_CliInfo[1].ExtrPort		=	DvrRtlCfgNetCmsPortGet();
		m_CliInfo[1].Protocol		=	1;								//TCP:0, UDP:1
		//m_CliInfo[1].InterPort	=	DvrRtlCfgNetCmsPortGet();
		GrSockV2GetMyIp(0, &m_CliInfo[1].ClientIp,FALSE);
		m_CliInfo[1].LeaseTime	=	0;								//max time
		GrStrCopy( m_CliInfo[1].Tiltle, "CMS_" );
		//GrStrCat( m_CliInfo[1].Tiltle, (__s8*)DvrRtlNetWebcodeGet() );

		//set client info - WEB TCP
		if ( m_CliInfo[0].InterPort == m_CliInfo[2].InterPort )
		{
			//DvrRtlCfgWebSvrPortSet( m_CliInfo[2].InterPort + 1 );
			//DvrRtlCfgSave();
		}

		//m_CliInfo[2].ExtrPort		=	DvrRtlCfgWebSvrPortGet();
		m_CliInfo[2].Protocol		=	0;								//TCP:0, UDP:1
		//m_CliInfo[2].InterPort	=	DvrRtlCfgWebSvrPortGet();
		GrSockV2GetMyIp(0, &m_CliInfo[2].ClientIp,FALSE);
		m_CliInfo[2].LeaseTime	=	0;								//max time
		GrStrCopy( m_CliInfo[2].Tiltle, "WEB_" );
		//GrStrCat( m_CliInfo[2].Tiltle, (__s8*)DvrRtlNetWebcodeGet() );
}
//--------------------------------------------------------------------
__u32	Cls_GrUpnpCli::GetUpnpStage( void )
{
		return	m_Stage;
}
//--------------------------------------------------------------------
__u32	Cls_GrUpnpCli::LcMakeGetModelMsg( void* A_PtrBuf, Ptr_NetCtrlSvInfo A_PtrSv )
{
	// var ---------------------
		__u32	Tv_Result;
		char*	Tv_StrBuf;
		char*	Tv_StrWk;
	// code --------------------
		// init
		Tv_StrBuf	=	(char*)A_PtrBuf;
		GrStrClear( Tv_StrBuf );
		Tv_StrWk	=	GrStrCopy( Tv_StrBuf, "GET " );
		Tv_StrWk	=	GrStrCopy( Tv_StrWk, A_PtrSv->Url );
		Tv_StrWk	=	GrStrCopy( Tv_StrWk, " HTTP/1.1\r\n"
			"Accept: text/xml, application/xml\r\n"
			"User-Agent: Mozilla/4.0 (compatible; UPnP/1.0; Windows NT/5.1)\r\n"
			"Host: " );
		GrStrIntToStr( Tv_StrWk, (__s32)(A_PtrSv->Ip & 0xFF) );
		Tv_StrWk	=	GrStrCat( Tv_StrWk, "." );
		GrStrIntToStr( Tv_StrWk, (__s32)((A_PtrSv->Ip >> 8) & 0xFF) );
		Tv_StrWk	=	GrStrCat( Tv_StrWk, "." );
		GrStrIntToStr( Tv_StrWk, (__s32)((A_PtrSv->Ip >> 16) & 0xFF) );
		Tv_StrWk	=	GrStrCat( Tv_StrWk, "." );
		GrStrIntToStr( Tv_StrWk, (__s32)((A_PtrSv->Ip >> 24) & 0xFF) );
		Tv_StrWk	=	GrStrCat( Tv_StrWk, ":" );
		GrStrIntToStr( Tv_StrWk, (__s32)A_PtrSv->Port );
		GrStrCat( Tv_StrWk, "\r\n"
			"Connection: Keep-Alive\r\n"
			"Cache-Control: no-cache\r\n"
			"Pragma: no-cache\r\n\r\n");

		Tv_Result	=	GrStrLen( Tv_StrBuf );
		if ( E_GrUpnpCliRecvBufLen <= Tv_Result )
		{
			DbgMsgPrint("Cls_GrUpnpCli::LcMakeGetModelMsg - buffer size over.\n", Tv_Result);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUpnpCli::LcStageGetModelInfo()
{
	// var ---------------------
		BOOL8		Tv_Result;
	// code --------------------
		Tv_Result		=	FALSE;
		if ( NULL != m_PtrSv )
		{
			if ( m_PtrSv->IsUse )
			{
				if ( LcTcpOpen(m_PtrSv) )
				{
					//make get model message
					m_SendReq	=	LcMakeGetModelMsg( m_Buf, m_PtrSv );

					//next
					m_Stage		=	E_GrUpnpCliStageModelPktSend;
					m_SendLen	=	0;
					Tv_Result	=	TRUE;
					m_TimeOutCnt	=	10;
				}
				else
				{
					DbgMsgPrint("Cls_GrUpnpCli::LcStageGetModelInfo - tcp open fail\n");
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUpnpCli::LcStageGetModelPktSend()
{
	// var ---------------------
		BOOL8	Tv_Result;
		__s32		Tv_SendLen;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( E_GrSockBadHandle != m_Sock )
		{
			Tv_SendLen	=	GrSockV2Send( m_Sock, m_Buf + m_SendLen, (__s32)m_SendReq - m_SendLen );
			if ( 0 <= Tv_SendLen )
			{
				Tv_Result		=	TRUE;
				m_SendLen		= m_SendLen + Tv_SendLen;
				if ( m_SendLen >= (__s32)m_SendReq )
				{
					m_Stage				=	E_GrUpnpCliStageModelPktRecv;
					m_RecvLen			=	0;
					m_TimeOutCnt	=	10;
					DbgMsgPrint("Cls_GrUpnpCli::LcStageGetModelPktSend - send success.\n");
				}
			}
			else
			{
				DbgMsgPrint("Cls_GrUpnpCli::LcStageGetModelPktSend - tcp get port information message send fail\n");
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUpnpCli::LcParseGetModel( char* A_SrBuf, Ptr_NetCtrlSvInfo A_PtrSv )
{
	// var ---------------------
		BOOL8		Tv_Result;
		__u32		Tv_WkCnt;
		__u32		Tv_MaxLen;
		Def_StrTag	Tv_StrAddr;
		__u32		Tv_AddrIdx;
		__u32		Tv_Mode;

		char*		Tv_StrType;
		__u32		Tv_TypeLen;
	// code --------------------
		//init
		Tv_Result	=	FALSE;
		Tv_MaxLen	=	GrStrLen( A_SrBuf );
		if ( E_GrUpnpCliRecvBufLen < Tv_MaxLen )
		{
			Tv_MaxLen	=	E_GrUpnpCliRecvBufLen - 1;
		}
		Tv_WkCnt	=	0;

		Tv_StrType	=	E_GrUpnpCliAckMsg;
		Tv_TypeLen	=	GrStrLen( Tv_StrType );

		Tv_Mode			=	E_GrUpnpCliParseMdAck;
		Tv_AddrIdx	=	0;
		while( Tv_WkCnt < Tv_MaxLen )
		{
			switch( Tv_Mode )
			{
			case	E_GrUpnpCliParseMdAck:
				if ( 0 == GrStrCmp(A_SrBuf + Tv_WkCnt, Tv_StrType, FALSE, Tv_TypeLen) )
				{
					Tv_WkCnt	=	Tv_WkCnt + Tv_TypeLen;		//jump

					//next find
					Tv_Mode			=	E_GrUpnpCliParseNameHd;
					Tv_StrType	=	"<friendlyName>";
					Tv_TypeLen	=	GrStrLen( Tv_StrType );
				}
				break;
			case E_GrUpnpCliParseNameHd:
				if ( 0 == GrStrCmp(A_SrBuf + Tv_WkCnt, Tv_StrType, FALSE, Tv_TypeLen) )
				{
					Tv_WkCnt	=	Tv_WkCnt + Tv_TypeLen;		//jump

					//next find
					Tv_Mode		=	E_GrUpnpCliParseNdName;
					Tv_WkCnt --;
				}
				break;
			case E_GrUpnpCliParseNdName:
				if ( '<' == A_SrBuf[Tv_WkCnt] )
				{
					Tv_StrAddr[Tv_AddrIdx]	=	0;
					//next
					Tv_Mode			=	E_GrUpnpCliParseSvcType;
					Tv_StrType	=	"urn:schemas-upnp-org:service:WANIPConnection:1";
					Tv_TypeLen	=	GrStrLen( Tv_StrType );
				}
				else
				{
					Tv_StrAddr[Tv_AddrIdx]	=	A_SrBuf[Tv_WkCnt];
					Tv_AddrIdx ++;
				}
				break;
			case E_GrUpnpCliParseSvcType:
				if ( 0 == GrStrCmp(A_SrBuf + Tv_WkCnt, Tv_StrType, FALSE, Tv_TypeLen) )
				{
					Tv_WkCnt	=	Tv_WkCnt + Tv_TypeLen;		//jump

					//next find
					Tv_Mode			=	E_GrUpnpCliParseCtrlHd;
					Tv_StrType	=	"<controlURL>";
					Tv_TypeLen	=	GrStrLen( Tv_StrType );
				}
				break;
			case E_GrUpnpCliParseCtrlHd:
				if ( 0 == GrStrCmp(A_SrBuf + Tv_WkCnt, Tv_StrType, FALSE, Tv_TypeLen) )
				{
					Tv_WkCnt	=	Tv_WkCnt + Tv_TypeLen;		//jump

					//next find
					Tv_Mode			=	E_GrUpnpCliParseCtrlUrl;
					Tv_AddrIdx	=	0;
					Tv_WkCnt --;
				}
				break;
			case E_GrUpnpCliParseCtrlUrl:
				if ( '<' == A_SrBuf[Tv_WkCnt] )
				{
					A_PtrSv->CtrlUrl[Tv_AddrIdx]	=	0;
					Tv_Result	=	TRUE;
					//next
					Tv_Mode		=	E_GrUpnpCliParseNone;
				}
				else
				{
					A_PtrSv->CtrlUrl[Tv_AddrIdx]	=	A_SrBuf[Tv_WkCnt];
					Tv_AddrIdx ++;
				}
				break;
			}

			//next
			Tv_WkCnt++;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUpnpCli::LcStageGetModelPktRecv()
{
	// var ---------------------
		BOOL8	Tv_Result;
		__s32		Tv_RecvLen;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( E_GrSockBadHandle != m_Sock )
		{
			Tv_RecvLen	=	GrSockV2Recv( m_Sock, m_Buf + m_RecvLen, E_GrUpnpCliRecvBufLen - m_RecvLen );
			if ( 0 >= Tv_RecvLen )
			{
				// check parse able
				if ( 0 != m_RecvLen )
				{
					DbgMsgPrint( "Cls_GrUpnpCli::LcStageGetModelPktRecv - %d byte recived ok.\n", m_RecvLen );
					// go to finish
					m_Buf[m_RecvLen]	=	0;
					//DbgMsgPrint( "[DATA]\n%s\n\n", (__s8*)m_Buf );

					// LcDbgDump( m_Buf, m_RecvLen );
					if ( LcParseGetModel( m_Buf, m_PtrSv ) )
					{
						m_Stage	=	E_GrUpnpCliStageGetPort;
						DbgMsgPrint("Cls_GrUpnpCli::LcStageGetModelPktRecv - add port success\n");
					}
					else
					{
						DbgMsgPrint("Cls_GrUpnpCli::LcStageGetModelPktRecv - add port fail\n");
					}
				}
			}
			else
			{
				Tv_Result		=	TRUE;
				m_RecvLen		=	m_RecvLen + Tv_RecvLen;
				// DbgMsgPrint( "Cls_GrUpnpCli::LcStageGetModelPktRecv - %d byte recived.\n", m_RecvLen );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUpnpCli::LcDbgDump( void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		__u32	Tv_Col;
		__u32	Tv_Remain;
		__u8*	Tv_PtrBuf;
	// code --------------------
		Tv_Remain	=	A_Size;
		Tv_PtrBuf	=	(__u8*)A_PtrBuf;

		while ( 0 != Tv_Remain )
		{
			// column
			Tv_Col	=	0;
			while ( (16 > Tv_Col) && (0 != Tv_Remain) )
			{
				DbgMsgPrint( "0x%02X,", *Tv_PtrBuf );
				Tv_PtrBuf ++;
				Tv_Col ++;
				Tv_Remain --;
			}
			DbgMsgPrint( "\n" );
		}
}
//--------------------------------------------------------------------
void	Cls_GrUpnpCli::ImmInit(void)
{
	// local -------------------
	// code --------------------
		GrSockV2Init();

		m_Stage	=	E_GrUpnpCliStageDiscverV4;
}
//--------------------------------------------------------------------
void	Cls_GrUpnpCli::ImmFinish(void)
{
	// local -------------------
	// code --------------------
		GrSockV2Finish();
}
//--------------------------------------------------------------------
void	Cls_GrUpnpCli::ReqInit(void)
{
	// local -------------------
		St_GrUpnpCliMsgHd	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.Cmd	=	m_MsgAloc;
		Tv_Msg.Cmd		=	E_GrUpnpCliCmdInit;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

}
//--------------------------------------------------------------------
void	Cls_GrUpnpCli::ReqFinish(void)
{
	// local -------------------
		St_GrUpnpCliMsgHd	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.Cmd	=	m_MsgAloc;
		Tv_Msg.Cmd		=	E_GrUpnpCliCmdFinish;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

}
//--------------------------------------------------------------------
BOOL8	Cls_GrUpnpCli::LcStageDiscoverV4(void)
{
	// local -------------------
		__u32	Tv_IpBrd;
		__u32	Tv_WkIdx;
	// code --------------------
		// init device list
		GrDumyZeroMem(m_DevTbl, sizeof(m_DevTbl));
		m_DevCnt		=	0;

		// make braodcast socket
		m_Sock	=	GrSockV2Open(TRUE, TRUE, FALSE);
		if(E_GrSockBadHandle == m_Sock)
		{
			m_RtCode	=	E_GrUpnpCliRtNotEnoughRsrc;
			m_Stage		=	E_GrUpnpCliStageFinish;
			return		FALSE;
		}

		Tv_IpBrd	=	E_GrUpnpCliBroadAddr;
		if(!GrSockV2UdpBroad(m_Sock, &Tv_IpBrd, m_IntfIdx,FALSE))
		{
			m_RtCode	=	E_GrUpnpCliRtSocketProblem;
			m_Stage		=	E_GrUpnpCliStageFinish;
			return		FALSE;
		}

		// make discver packet
		m_SendReq	=	LcMakeDiscvMsgV4(m_Buf);

		// do broadcast
		for( Tv_WkIdx = 0; Tv_WkIdx < 3; Tv_WkIdx++)
		{
			Tv_IpBrd	=	E_GrUpnpCliBroadAddr;
			if(0 > GrSockV2UdpSend(m_Sock, &Tv_IpBrd, E_GrUpnpCliBroadPort, m_Buf, m_SendReq, FALSE))
			{
				// send fail
				DbgMsgPrint("Cls_GrUpnpCli::LcStageDiscoverV4 - send fail %d\n", GrSockV2GetErrCode());
				// go finish
				m_RtCode	=	E_GrUpnpCliRtSocketProblem;
				m_Stage		=	E_GrUpnpCliStageFinish;
				return	FALSE;
			}
			GrPrcSleep(10);
		}

		m_Stage		=	E_GrUpnpCliStageReadDiscvV4;
		m_TimeOut	=	4;
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUpnpCli::LcStageReadDiscvV4(void)
{
	// local -------------------
		__u32	Tv_Ip;
		__u16	Tv_Port;
		Def_GrErrCode	Tv_RtCode;
	// code --------------------
		//receive packet
		Tv_Port			=	0;
		Tv_Ip				=	INADDR_ANY;
		m_RecvLen		=	(__s32)GrSockV2UdpRead(m_Sock, &Tv_Ip, &Tv_Port, m_Buf, E_GrUpnpCliRecvBufLen - 1, FALSE,
			&Tv_RtCode);

		if((0 < m_RecvLen) && (E_GrUpnpCliRecvBufLen > m_RecvLen))
		{
			m_Buf[m_RecvLen]	=	0;
			m_PrsPos					=	0;

			// do parsing
			LcParsDiscv();

			return	TRUE;
		}

		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUpnpCli::LcSubParsGetOneLine(void)
{
	// local -------------------
		
	// code --------------------
		// check already finished
		if((m_PrsPos + 1) >= m_RecvLen)
		{
			return	FALSE;
		}

		// init
		m_StrLine		=	&m_Buf[m_PrsPos];
		m_LineLen		=	0;
		m_LinePrgs	=	0;

		while((m_PrsPos + 1) < m_RecvLen)
		{
			// check finish
			if((0x0D == m_Buf[m_PrsPos]) && (0x0A == m_Buf[m_PrsPos + 1]))
			{
				// set end line
				m_Buf[m_PrsPos]				=	0;
				m_Buf[m_PrsPos + 1]		=	0;
				// move next position
				m_PrsPos	=	m_PrsPos + 2;
				break;
			}

			// next
			m_PrsPos ++;
			m_LineLen	++;
		}

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUpnpCli::LcSubParsWordSkipSpace(void)
{
	// local -------------------
	// code --------------------

		while(m_LinePrgs < m_LineLen)
		{
			if(0x20 < m_StrLine[m_LinePrgs] )
			{
				// found word
				return	TRUE;
			}
			// next
			m_LinePrgs ++;
		}

		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUpnpCli::LcSubParsWordSkipHttp(void)
{
	// local -------------------
	// code --------------------
		if(!LcSubParsWordSkipSpace())
		{
			return	FALSE;
		}

		if((m_LinePrgs + 7) >= m_LineLen)
		{
			return	FALSE;
		}

		if((('h' == m_StrLine[m_LinePrgs]) || ('H' == m_StrLine[m_LinePrgs])) && 
			(('t' == m_StrLine[m_LinePrgs + 1]) || ('T' == m_StrLine[m_LinePrgs + 1])) &&
			(('t' == m_StrLine[m_LinePrgs + 2]) || ('T' == m_StrLine[m_LinePrgs + 2])) &&
			(('p' == m_StrLine[m_LinePrgs + 3]) || ('P' == m_StrLine[m_LinePrgs + 3])) &&
			(':' == m_StrLine[m_LinePrgs + 4])  && ('/' == m_StrLine[m_LinePrgs + 5]) && ('/' == m_StrLine[m_LinePrgs + 6])
			)
		{
			m_LinePrgs	=	m_LinePrgs + 7;
		}

		if(m_LinePrgs >= m_LineLen)
		{
			return	FALSE;
		}
		
		return	TRUE;

}
//--------------------------------------------------------------------
BOOL8	Cls_GrUpnpCli::LcSubParsWordLabel(void)
{
	// local -------------------
	// code --------------------
		// skip space
		if(!LcSubParsWordSkipSpace())
		{
			return	FALSE;
		}

		// check label len
		m_StrWord	=	&m_StrLine[m_LinePrgs];
		m_WordLen	=	0;
		while(m_LinePrgs < m_LineLen)
		{
			if(':' == m_StrLine[m_LinePrgs])
			{
				// found
				m_StrLine[m_LinePrgs]	=	0;
				m_LinePrgs ++;
				if(0 != m_WordLen)
				{
					return	TRUE;
				}
				return	FALSE;
			}
			// next
			m_WordLen ++;
			m_LinePrgs ++;
		}

		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUpnpCli::LcSubParsWordAll(void)
{
	// local -------------------
	// code --------------------
		// skip space
		if(!LcSubParsWordSkipSpace())
		{
			return	FALSE;
		}

		// check label len
		m_StrWord	=	&m_StrLine[m_LinePrgs];
		m_WordLen	=	m_LineLen - m_LinePrgs;
		
		m_LinePrgs	=	m_LineLen;

		if(0 == m_WordLen)
		{
			return	FALSE;
		}

		return	TRUE;

}
//--------------------------------------------------------------------
BOOL8	Cls_GrUpnpCli::LcSubParsWordDblColon(void)
{
	// local -------------------
	// code --------------------
		// skip space
		if(!LcSubParsWordSkipSpace())
		{
			return	FALSE;
		}

		// check label len
		m_StrWord	=	&m_StrLine[m_LinePrgs];
		m_WordLen	=	0;
		while(m_LinePrgs < m_LineLen)
		{
			if((m_LinePrgs + 1) < m_LineLen)
			{
				if((':' == m_StrLine[m_LinePrgs]) && (':' == m_StrLine[m_LinePrgs + 1]))
				{
					// found
					m_StrLine[m_LinePrgs]	=	0;
					m_StrLine[m_LinePrgs + 1]	=	0;
					m_LinePrgs	=	m_LinePrgs + 2;
					if(0 != m_WordLen)
					{
						return	TRUE;
					}
					return	FALSE;
				}
			}
			// next
			m_WordLen ++;
			m_LinePrgs ++;
		}

		return	FALSE;

}
//--------------------------------------------------------------------
BOOL8	Cls_GrUpnpCli::LcSubParsWordIpAndPort(__u32* A_PtrRtIp, __u16* A_PtrRtPort)
{
	// local -------------------
		char	Tv_StrBuf[64];
		__u32	Tv_InPos;
		__s32	Tv_Port;
	// code --------------------
		// init
		*A_PtrRtPort	=	80;
		Tv_InPos	=	0;
		// get ip
		while(m_LinePrgs < m_LineLen)
		{
			if((('0' <= m_StrLine[m_LinePrgs]) && ('9' >= m_StrLine[m_LinePrgs])) ||
				('.' == m_StrLine[m_LinePrgs]) )
			{
				// correct
				Tv_StrBuf[Tv_InPos]	=	m_StrLine[m_LinePrgs];
				Tv_InPos ++;
				if(63 <= Tv_InPos)
				{
					break;
				}
			}
			else
			{
				// end
				break;
			}
			// next
			m_LinePrgs ++;
		}

		if(0 == Tv_InPos)
		{
			return	FALSE;
		}

		Tv_StrBuf[Tv_InPos]	=	0;
		if(!GrStrStripToIpV4(Tv_StrBuf, A_PtrRtIp))
		{
			return	FALSE;
		}
		
		// check port exist
		if(m_LinePrgs >= m_LineLen)
		{
			return	TRUE;
		}

		if(':' != m_StrLine[m_LinePrgs])
		{
			return	TRUE;
		}
		m_LinePrgs ++;

		// get port
		Tv_InPos		=	0;
		while(m_LinePrgs < m_LineLen)
		{
			if(('0' <= m_StrLine[m_LinePrgs]) && ('9' >= m_StrLine[m_LinePrgs]))
			{
				// correct
				Tv_StrBuf[Tv_InPos]	=	m_StrLine[m_LinePrgs];
				Tv_InPos ++;
				if(63 <= Tv_InPos)
				{
					break;
				}
			}
			else
			{
				// end
				break;
			}
			// next
			m_LinePrgs ++;
		}

		if(0 == Tv_InPos)
		{
			return	TRUE;
		}

		Tv_StrBuf[Tv_InPos]	=	0;
		if(GrStrStrToInt(Tv_StrBuf, &Tv_Port))
		{
			*A_PtrRtPort	=	(__u16)Tv_Port;
		}
		
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrUpnpCli::LcSubDiscCmdSt(void)
{
	// local -------------------
	// code --------------------
		// get label
		if(!LcSubParsWordLabel())
		{
			return;
		}

		if(0 != GrStrCmp(m_StrWord,"urn",FALSE))
		{
#ifdef E_GrUpnpCliDebug
			DbgMsgPrint("Cls_GrUpnpCli::LcSubDiscCmdSt - unknown sub command %s!\n", m_StrWord);
#endif
			return;
		}

		if(LcSubParsWordAll())
		{
			GrStrCopy(m_DevTbl[m_DevCnt].StrUrn, m_StrWord);
		}

}
//--------------------------------------------------------------------
void	Cls_GrUpnpCli::LcSubDiscCmdUsn(void)
{
	// local -------------------
	// code --------------------
		// get label
		if(!LcSubParsWordLabel())
		{
			return;
		}

		if(0 != GrStrCmp(m_StrWord, "uuid", FALSE))
		{
#ifdef E_GrUpnpCliDebug
			DbgMsgPrint("Cls_GrUpnpCli::LcSubDiscCmdUsn - unknown sub command %s!\n", m_StrWord);
#endif
			return;
		}

		if(!LcSubParsWordDblColon())
		{
#ifdef E_GrUpnpCliDebug
			DbgMsgPrint("Cls_GrUpnpCli::LcSubDiscCmdUsn - uuid value not found!\n");
#endif
			return;
		}
		
		GrStrCopy(m_DevTbl[m_DevCnt].StrUuid, m_StrWord);
}
//--------------------------------------------------------------------
void	Cls_GrUpnpCli::LcSubDiscCmdLocation(void)
{
	// local -------------------
	// code --------------------
		// skip space
		if(!LcSubParsWordSkipSpace())
		{
			return;
		}

		if(!LcSubParsWordSkipHttp())
		{
			return;
		}

		// get ip v4 and port
		if(!LcSubParsWordIpAndPort(&m_DevTbl[m_DevCnt].IpV4, &m_DevTbl[m_DevCnt].Port))
		{
#ifdef E_GrUpnpCliDebug
			DbgMsgPrint("Cls_GrUpnpCli::LcSubDiscCmdLocation - ip not found!\n");
#endif
			return;
		}
		
		// get cgi file name
		if(LcSubParsWordAll())
		{
			GrStrCopy(m_DevTbl[m_DevCnt].StrCgi, m_StrWord, E_GrUpnpCliDevCgiMaxLen);
		}

}
//--------------------------------------------------------------------
__s32	Cls_GrUpnpCli::LcDevFindByUuid(char* A_StrUuid)
{
	// local -------------------
		__s32	Tv_WkIdx;
	// code --------------------
		if(0 == m_DevCnt)
		{
			return	-1;
		}

		for(Tv_WkIdx = 0; Tv_WkIdx < m_DevCnt; Tv_WkIdx++)
		{
			if(0 == GrStrCmp(m_DevTbl[Tv_WkIdx].StrUuid, A_StrUuid,FALSE))
			{
				return	Tv_WkIdx;
			}
		}

		return	-1;
}
//--------------------------------------------------------------------
__s32	Cls_GrUpnpCli::LcDevFindByIpV4(__u32 A_IpV4)
{
	// local -------------------
		__s32	Tv_WkIdx;
	// code --------------------
		if(0 == m_DevCnt)
		{
			return	-1;
		}

		for(Tv_WkIdx = 0; Tv_WkIdx < m_DevCnt; Tv_WkIdx++)
		{
			if(m_DevTbl[Tv_WkIdx].IpV4 == A_IpV4)
			{
				return	Tv_WkIdx;
			}
		}

		return	-1;
}
//--------------------------------------------------------------------
void	Cls_GrUpnpCli::LcParsDiscv(void)
{
	// local -------------------
		BOOL8	Tv_IsErase;
	// code --------------------
		// check device count
		if(E_GrUpnpCliDevMaxCnt <= m_DevCnt)
		{
			return;
		}

		// get first line
		if(!LcSubParsGetOneLine())
		{
#ifdef E_GrUpnpCliDebug
			DbgMsgPrint("Cls_GrUpnpCli::LcParsDiscv - ACK not exist!\n");
#endif
			return;
		}

		// check first line
		if(0 != GrStrCmp(m_StrLine, E_GrUpnpCliAckMsg,FALSE))
		{
			// bad command
#ifdef E_GrUpnpCliDebug
			DbgMsgPrint("Cls_GrUpnpCli::LcParsDiscv - not ACK!\n");
#endif
			return;
		}

		// do contentes parsing
		while(LcSubParsGetOneLine())
		{
			// check end line
			if(0 == m_LineLen)
			{
				break;
			}

			// get command
			if(!LcSubParsWordLabel())
			{
				continue;
			}

			// parsing command
			if(0 == GrStrCmp(m_StrWord, "ST",FALSE))
			{
				LcSubDiscCmdSt();
			}
			else if(0 == GrStrCmp(m_StrWord, "USN", FALSE))
			{
				LcSubDiscCmdUsn();
			}
			else if(0 == GrStrCmp(m_StrWord, "LOCATION", FALSE))
			{
				LcSubDiscCmdLocation();
			}

		}

		// check device found
		Tv_IsErase	=	TRUE;
		if((0 != m_DevTbl[m_DevCnt].IpV4) && GrStrIsExist(m_DevTbl[m_DevCnt].StrCgi) && GrStrIsExist(m_DevTbl[m_DevCnt].StrUuid))
		{
			// check exist uuid
			if(0 <= LcDevFindByUuid(m_DevTbl[m_DevCnt].StrUuid))
			{
				m_DevCnt ++;
				Tv_IsErase	=	FALSE;
			}
		}

		if(Tv_IsErase)
		{
			GrDumyZeroMem(&m_DevTbl[m_DevCnt], sizeof(St_GrUpnpCliDevCtl));
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUpnpCli::LcStagePortFwPrepare(void)
{
	// local -------------------
	// code --------------------
		// close socket
		if(E_GrSockBadHandle != m_Sock)
		{
			GrSockV2Close(m_Sock);
			m_Sock	=	E_GrSockBadHandle;
		}

		// test
		m_ReqDevIdx	=	0;

		// find device
		if(0 > m_ReqDevIdx)
		{
			// fail
			m_RtCode	=	E_GrUpnpCliRtNotFound;
			m_Stage		=	E_GrUpnpCliStageFinish;
			return	FALSE;
		}
		
		// reset runtime
		GrDumyZeroMem(&m_Rtl, sizeof(m_Rtl));

		// go request entry
		m_Stage	=	E_GrUpnpCliStagePortFwReqEntry;

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUpnpCli::LcStagePortFwReqEntry(void)
{
	// local -------------------
	// code --------------------
		// make cgi message

	return	FALSE;
}
//--------------------------------------------------------------------

