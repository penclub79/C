/*
 dvr external enviroment sensor

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<GrDebug.h>
#include	<Dvr/Ext/GrDvrExtEsen.h>
#include	<GrDumyTool.h>
#include	<GrTimeTool.h>
#include	<GrStrTool.h>
#include	<GrUart.h>

#include	<stdlib.h>


//====================================================================
//local const

#define E_GrDvrExtEsenTimerSecPriod		1000
#define E_GrDvrExtEsenReqPriod				30

#define E_GrDvrExtEsenTimeOut					10
#define E_GrDvrExtEsenTimeOver				30

#define E_GrDvrExtEsenStx							0x44585453
#define E_GrDvrExtEsenEtx							0x44585445

#define E_GrDvrExtEsenMsgSetup				0				/* setup : St_GrDvrExtEsenMsgSetup */


//====================================================================
//local type

typedef	struct  St_GrDvrExtEsenPktReq
{
	__u32	Stx;
	__u8	Ch[4];
	__u32	Etx;
}	*Ptr_GrDvrExtEsenPktReq;

typedef	struct St_GrDvrExtEsenPktData 
{
	__u32	Stx;
	__u8	Ch[2];
	__u8	Int0;
	__u8	Int1;
	__u8	Int2;
	__u8	Int3;
	__u8	Minor;
	__u8	Err;
	__u32	Etx;
}	*Ptr_GrDvrExtEsenPktData;

typedef	struct St_GrDvrExtEsenMsgHd 
{
	St_GrTaskMsgHd	MsgHd;
	__u32	Cmd;
}	*Ptr_GrDvrExtEsenMsgHd;

typedef	struct St_GrDvrExtEsenMsgSetup 
{
	St_GrDvrExtEsenMsgHd	Hd;
	__u8	UartCh;
	__u16	ChMap;
}	*Ptr_GrDvrExtEsenMsgSetup;



//====================================================================
//local function

//====================================================================
//var


//====================================================================

//--------------------------------------------------------------------
Cls_GrDvrExtEsen::Cls_GrDvrExtEsen( Cls_GrTaskSvr* A_TaskSvr ):
Cls_GrTaskCli()
{
	// local -------------------
	// code --------------------
		m_TaskSvr	=	A_TaskSvr;
		m_MsgAloc	=	GrTaskSvrMsgCmdAlloc();

		//init
		GrDumyZeroMem( m_ChTbl, sizeof(m_ChTbl) );
		m_UartCh		=	0;
		m_RecvSize	=	0;
		m_RecvMax		=	4;
		m_IsOn			=	FALSE;
		m_ChkCh			=	0;
		m_IsReqed		=	FALSE;
		m_TimeOut		=	0;
		m_IsChged		=	FALSE;

		//regist task
		m_TaskSvr->RegistMsgClient( this );
		m_TaskSvr->RegistTimerClient( this, E_GrDvrExtEsenTimerSecPriod, &m_HndlTimer );
		m_TaskSvr->RegistBkgClient( this );

}
//--------------------------------------------------------------------
Cls_GrDvrExtEsen::~Cls_GrDvrExtEsen()
{
	// local -------------------
	// code --------------------
		//unregister timer
		m_TaskSvr->UnregistBkgClient( this );
		m_TaskSvr->UnregistTimerClient( m_HndlTimer );
		m_TaskSvr->UnregistMsgClient( this );

		//release protocol object

		GrTaskSvrMsgCmdFree( m_MsgAloc );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrExtEsen::RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( m_MsgAloc == A_PtrMsg->Cmd )
		{
			Tv_Result	=	TRUE;
			switch( ((Ptr_GrDvrExtEsenMsgHd)A_PtrMsg)->Cmd )
			{
				case	E_GrDvrExtEsenMsgSetup:
					LcSetup( ((Ptr_GrDvrExtEsenMsgSetup)A_PtrMsg)->UartCh,((Ptr_GrDvrExtEsenMsgSetup)A_PtrMsg)->ChMap );
					break;

				default:
					Tv_Result	=	FALSE;
					break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrExtEsen::LcSetup( __u8 A_UartCh, __u16 A_ChMap )
{
	// local -------------------
		__u8	Tv_Ch;
		__u16	Tv_MaskCh;
		BOOL8	Tv_IsOn;
	// code --------------------
		m_UartCh	=	A_UartCh;
		Tv_IsOn		=	FALSE;
		Tv_MaskCh	=	1;
		for ( Tv_Ch=0;Tv_Ch < E_GrDvrMaxChCnt;Tv_Ch++ )
		{
			//channel mask
			if ( 0 != (Tv_MaskCh & A_ChMap) )
			{
				m_ChTbl[Tv_Ch].IsUse		=	TRUE;
				m_ChTbl[Tv_Ch].IsRcv		=	FALSE;
				m_ChTbl[Tv_Ch].IsErr		=	FALSE;
				m_ChTbl[Tv_Ch].ValInt		=	0;
				m_ChTbl[Tv_Ch].ValMin		=	0;
				//able
				Tv_IsOn	=	TRUE;
			}
			else
			{
				//off
				m_ChTbl[Tv_Ch].IsUse		=	FALSE;
				m_ChTbl[Tv_Ch].IsRcv		=	FALSE;
				m_ChTbl[Tv_Ch].IsErr		=	FALSE;
				m_ChTbl[Tv_Ch].ValInt		=	0;
				m_ChTbl[Tv_Ch].ValMin		=	0;
			}
			//next
			Tv_MaskCh	=	Tv_MaskCh << 1;
		}

		//update useable
		m_IsReqed		=	FALSE;
		m_TimeOut		=	0;
		m_ChkCh			=	0;
		m_IsOn			=	Tv_IsOn;
}
//--------------------------------------------------------------------
void	Cls_GrDvrExtEsen::Setup( __u8 A_UartCh, __u16 A_ChMap )
{
	// local -------------------
		St_GrDvrExtEsenMsgSetup	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrDvrExtEsenMsgSetup;
		Tv_Msg.UartCh				=	A_UartCh;
		Tv_Msg.ChMap				=	A_ChMap;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrDvrExtEsen::LcRequest( __u8 A_Ch )
{
	// local -------------------
		St_GrDvrExtEsenPktReq	Tv_Pkt;		
		__u8	Tv_Ch;
	// code --------------------
		if ( m_ChTbl[A_Ch].IsUse )
		{
			Tv_Ch	=	A_Ch + 1;
			Tv_Pkt.Stx		=	E_GrDvrExtEsenStx;
			Tv_Pkt.Ch[0]	=	'0';
			Tv_Pkt.Ch[1]	=	'0';
			Tv_Pkt.Ch[2]	=	'0';
			Tv_Pkt.Ch[3]	=	'0';

			if ( 10 <= Tv_Ch )
			{
				Tv_Pkt.Ch[2]	=	'1';
				Tv_Ch	=	Tv_Ch - 10;
			}

			Tv_Pkt.Ch[3]	=	Tv_Pkt.Ch[3] + Tv_Ch;
			Tv_Pkt.Etx		=	E_GrDvrExtEsenEtx;
			GrUartSend( m_UartCh, &Tv_Pkt, sizeof(Tv_Pkt), TRUE );
			//set sended
			m_IsReqed	=	TRUE;
			m_TimeOut	=	0;
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrExtEsen::LcProcRecv( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_ReqSize;
		__u8	Tv_Recved;
		void*	Tv_PtrBuf;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( m_IsOn )
		{
			//get request ptr
			Tv_ReqSize	=	m_RecvMax - m_RecvSize;
			Tv_PtrBuf		=	(void*)( (__u32)m_RecvBuf + (__u32)m_RecvSize );
			Tv_Recved		=	(__u8)GrUartRecv( m_UartCh, Tv_PtrBuf, Tv_ReqSize );
			if ( 0 != Tv_Recved )
			{
				//update recived
				m_RecvSize	=	m_RecvSize + Tv_Recved;
				if ( m_RecvMax == m_RecvSize )
				{
					//stage completed
					if ( 4 == m_RecvSize )
					{
						//check stx
						if ( E_GrDvrExtEsenStx ==  (*((__u32*)m_RecvBuf)) )
						{
							//match stx
							m_RecvMax	=	16;
						}
						else
						{
							//bad stx, shift buffer
							(*((__u32*)m_RecvBuf))	=	(*((__u32*)m_RecvBuf)) >> 8;
							m_RecvSize	=	3;
						}
					}
					else if ( 16 == m_RecvSize )
					{
						//check etx
						if ( E_GrDvrExtEsenEtx == ((Ptr_GrDvrExtEsenPktData)m_RecvBuf)->Etx )
						{
							//analysis value
							LcUpdtValue();
						}
						//reset value
						m_RecvSize	=	0;
						m_RecvMax		=	4;
					}
					else
					{
						DbgMsgPrint( "Cls_GrDvrExtEsen::LcProcRecv - bad size recived - %d\n", m_RecvSize );
					}
				}
				//recived
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrExtEsen::LcUpdtValue( void )
{
	// local -------------------
		Ptr_GrDvrExtEsenPktData	Tv_PtrPkt;
		__u8	Tv_Ch;
		__u16	Tv_Val;
	// code --------------------
		Tv_PtrPkt	=	(Ptr_GrDvrExtEsenPktData)m_RecvBuf;

		//get channel
		Tv_Ch		=	0;
		if ( '1' == Tv_PtrPkt->Ch[0] )
		{
			Tv_Ch	=	10;
		}
		Tv_Ch		=	Tv_Ch + (Tv_PtrPkt->Ch[1] - '0') - 1;

		//check channel
		if ( E_GrDvrMaxChCnt > Tv_Ch )
		{
			//check exist channel
			if ( m_ChTbl[Tv_Ch].IsUse )
			{
				//check error
				if ( 'B' == Tv_PtrPkt->Err )
				{
					m_ChTbl[Tv_Ch].IsErr	=	TRUE;
				}
				else
				{
					//integer part
					Tv_Val	=	0;
					Tv_Val	=	Tv_Val + ((__u16)(Tv_PtrPkt->Int0 - '0') * 1000);
					Tv_Val	=	Tv_Val + ((__u16)(Tv_PtrPkt->Int1 - '0') * 100);
					Tv_Val	=	Tv_Val + ((__u16)(Tv_PtrPkt->Int2 - '0') * 10);
					Tv_Val	=	Tv_Val + (__u16)(Tv_PtrPkt->Int3 - '0');

					m_ChTbl[Tv_Ch].ValInt	=	Tv_Val;
					//minor part
					m_ChTbl[Tv_Ch].ValMin	=	( Tv_PtrPkt->Minor - 0x30 );
					//noerror
					m_ChTbl[Tv_Ch].IsErr	=	FALSE;
				}
				//update recived
				m_ChTbl[Tv_Ch].IsRcv		=	TRUE;
				//check match requested channel
				if ( Tv_Ch == m_ChkCh )
				{
					m_IsReqed	=	FALSE;
					m_TimeOut	=	0;
					m_ChkCh ++;
				}
				// changed
				m_IsChged		=	TRUE;
			}
			else
			{
				DbgMsgPrint( "Cls_GrDvrExtEsen::LcUpdtValue - not used channel - %d\n", Tv_Ch );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrDvrExtEsen::LcUpdtValue - bad channel number - %d\n", Tv_Ch );
		}

}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrExtEsen::RtlBkgProc( void )
{
		return	LcProcRecv();
}
//--------------------------------------------------------------------
void	Cls_GrDvrExtEsen::RtlTimer( __u32 A_Hndl )
{
	// local -------------------
	// code --------------------
		if ( m_HndlTimer == A_Hndl )
		{
			if ( m_IsOn )
			{
				if ( E_GrDvrMaxChCnt > m_ChkCh )
				{
					//check requested
					if ( m_IsReqed )
					{
						//check timeout
						m_TimeOut ++;
						if ( E_GrDvrExtEsenTimeOut < m_TimeOut )
						{
							m_ChTbl[m_ChkCh].IsErr	=	TRUE;
							m_ChTbl[m_ChkCh].IsRcv	=	FALSE;
							m_IsChged	=	TRUE;
							//reest time out
							m_IsReqed	=	FALSE;
							m_TimeOut	=	0;
							m_ChkCh ++;
						}
					}
					else
					{
						//find request
						while ( E_GrDvrMaxChCnt > m_ChkCh )
						{
							if ( m_ChTbl[m_ChkCh].IsUse )
							{
								LcRequest( m_ChkCh );
								break;
							}
							//next
							m_ChkCh ++;
						}
					}
				}
				else
				{
					// wait next priod
					m_TimeOut ++;
					if ( E_GrDvrExtEsenReqPriod <= m_TimeOut )
					{
						//reqeust
						m_ChkCh		=	0;
						m_TimeOut	=	0;
					}
				}
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrExtEsen::ValueGet( __u8 A_Ch, __u16* A_PtrRtInt, __u8* A_PtrRtMin, BOOL8* A_PtrRtErr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( m_IsOn && (E_GrDvrMaxChCnt > A_Ch) )
		{
			if ( m_ChTbl[A_Ch].IsUse && m_ChTbl[A_Ch].IsRcv )
			{
				*A_PtrRtInt	=	m_ChTbl[A_Ch].ValInt;
				*A_PtrRtMin	=	m_ChTbl[A_Ch].ValMin;
				*A_PtrRtErr	=	m_ChTbl[A_Ch].IsErr;
				Tv_Result	=	TRUE;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrExtEsen::IsChanged( void )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		Tv_Result	=	m_IsChged;
		m_IsChged	=	FALSE;
		return	Tv_Result;
}
//--------------------------------------------------------------------
