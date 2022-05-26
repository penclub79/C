/*
	UART


*/

#include	<GrUart.h>
#include	<GrStrTool.h>
#include	<GrProcess.h>
#include	<GrDebug.h>

//--------------------------------------------------------------------
//local const
#define E_GrUartOsWaitTime		20

//--------------------------------------------------------------------
//local function


//--------------------------------------------------------------------
//global var

St_GrUartChItm	V_GrUartChTbl[E_GrUartMaxCh]	=	
{
	{ FALSE, FALSE, },	{ FALSE, FALSE, },	{ FALSE, FALSE, },	{ FALSE, FALSE, }
};

//====================================================================
//--------------------------------------------------------------------
BOOL8	GrUartIsRegisted( __u8 A_Ch )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrUartMaxCh > A_Ch )
		{
			Tv_Result	=	V_GrUartChTbl[A_Ch].IsExist;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------

#if defined(_WIN64)
BOOL8	GrUartRegist(__u8 A_Ch, Ptr_GrUartOps A_PtrOps, __u64 A_Id)
#elif defined(_WIN32)
BOOL8	GrUartRegist( __u8 A_Ch, Ptr_GrUartOps A_PtrOps, __u32 A_Id )
#else
BOOL8	GrUartRegist(__u8 A_Ch, Ptr_GrUartOps A_PtrOps, __u32 A_Id)
#endif
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrUartMaxCh > A_Ch )
		{
#ifdef LINUX_APP
			V_GrUartChTbl[A_Ch].Id				=	A_Id;
#else
			V_GrUartChTbl[A_Ch].Id				=	(HANDLE)A_Id;
#endif
			V_GrUartChTbl[A_Ch].Ops				=	*A_PtrOps;
			V_GrUartChTbl[A_Ch].IsOsPort	=	FALSE;
			V_GrUartChTbl[A_Ch].IsExist		=	TRUE;
			V_GrUartChTbl[A_Ch].Buad			=	0;
			Tv_Result	=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrUartOsOpsRegist( __u8 A_Ch, __u8 A_Port )
{
	// local -------------------
		BOOL8	Tv_Result;
#ifndef LINUX_APP
		Def_WstrName	Tv_StrName;
		DCB		Tv_Dcb;
		COMMTIMEOUTS	Tv_TmOut;
#endif
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrUartMaxCh > A_Ch )
		{
#ifndef LINUX_APP
			GrStrWcopy( Tv_StrName, L"COM1" );
			Tv_StrName[3]	=	Tv_StrName[3] + A_Port;

			V_GrUartChTbl[A_Ch].Id	=	CreateFileW( Tv_StrName ,GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING,
																0, 0 );
			if ( INVALID_HANDLE_VALUE != (HANDLE)V_GrUartChTbl[A_Ch].Id )
			{
				//set time out
				GetCommTimeouts( (HANDLE)V_GrUartChTbl[A_Ch].Id, &Tv_TmOut );
				Tv_TmOut.ReadIntervalTimeout	=	10;
				Tv_TmOut.ReadTotalTimeoutConstant	=	5;
				Tv_TmOut.ReadTotalTimeoutMultiplier	=	2;
				SetCommTimeouts( (HANDLE)V_GrUartChTbl[A_Ch].Id, &Tv_TmOut );

				GetCommState( (HANDLE)V_GrUartChTbl[A_Ch].Id, &Tv_Dcb );
				Tv_Dcb.BaudRate		=	CBR_9600;
				Tv_Dcb.Parity			=	NOPARITY;
				Tv_Dcb.StopBits		=	ONESTOPBIT;
				Tv_Dcb.ByteSize		=	8;
				SetCommState( (HANDLE)V_GrUartChTbl[A_Ch].Id, &Tv_Dcb );

				V_GrUartChTbl[A_Ch].IsOsPort	=	TRUE;
				V_GrUartChTbl[A_Ch].IsExist		=	TRUE;
			}
			
			Tv_Result	=	TRUE;
#else
			Tv_Result	=	FALSE;
#endif
			// reset
			V_GrUartChTbl[A_Ch].Buad	=	0;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrUartUnregist( __u8 A_Ch )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------

		Tv_Result	=	FALSE;
		if ( E_GrUartMaxCh > A_Ch )
		{
			//close OS port
			if ( V_GrUartChTbl[A_Ch].IsExist && V_GrUartChTbl[A_Ch].IsOsPort )
			{
#ifndef LINUX_APP
				CloseHandle( (HANDLE)V_GrUartChTbl[A_Ch].Id );
#endif
			}
			
			V_GrUartChTbl[A_Ch].IsExist		=	FALSE;
			Tv_Result	=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrUartSetup( __u8 A_Ch, __u32 A_Speed, __u8 A_DataBit, __u8 A_ParityBit, __u8 A_StopBit	)
{
	// local -------------------
		BOOL8	Tv_Result;
#ifndef LINUX_APP
		DCB		Tv_Dcb;
#endif
	// code --------------------

		Tv_Result	=	FALSE;
		if ( E_GrUartMaxCh > A_Ch )
		{
			//check exist
			if ( V_GrUartChTbl[A_Ch].IsExist )
			{
				// check option change
				if (	(A_Speed != V_GrUartChTbl[A_Ch].Buad) || (A_DataBit != V_GrUartChTbl[A_Ch].DataBit) || 
							(A_ParityBit != V_GrUartChTbl[A_Ch].Parity) || (A_StopBit != V_GrUartChTbl[A_Ch].StopBit) )
				{
					//DbgMsgPrint( "UART %d chg : spd %d data %d stop %d parity %d \n", A_Ch, A_Speed, A_DataBit, A_StopBit, A_ParityBit );
					//check OS port
					if ( V_GrUartChTbl[A_Ch].IsOsPort )
					{
						//OS function
#ifndef LINUX_APP

						//set com status
						GetCommState( (HANDLE)V_GrUartChTbl[A_Ch].Id, &Tv_Dcb );
						Tv_Dcb.BaudRate		=	A_Speed;
						switch( A_ParityBit )
						{
							case	E_GrUartCfgParityBitNone:
								Tv_Dcb.Parity		=	NOPARITY;
								break;
							case	E_GrUartCfgParityBitEven:
								Tv_Dcb.Parity		=	EVENPARITY;
								break;
							case	E_GrUartCfgParityBitOdd:
								Tv_Dcb.Parity		=	ODDPARITY;
								break;
						}
						switch( A_StopBit )
						{
							case	E_GrUartCfgStopBit1:
								Tv_Dcb.StopBits		=	ONESTOPBIT;
								break;
							case	E_GrUartCfgStopBit1_5:
								Tv_Dcb.StopBits		=	ONE5STOPBITS;
								break;
							case	E_GrUartCfgStopBit2:
								Tv_Dcb.StopBits		=	TWOSTOPBITS;
								break;
						}
						Tv_Dcb.ByteSize		=	A_DataBit;
						Tv_Result	=	SetCommState( (HANDLE)V_GrUartChTbl[A_Ch].Id, &Tv_Dcb );
#endif
					}
					else
					{
						//register function
						Tv_Result	=	(*V_GrUartChTbl[A_Ch].Ops.Setup)( V_GrUartChTbl[A_Ch].Id, A_Speed, A_DataBit, A_ParityBit, A_StopBit );
					}
					// update configuration
					if ( Tv_Result )
					{
						V_GrUartChTbl[A_Ch].Buad		=	A_Speed;
						V_GrUartChTbl[A_Ch].DataBit	=	A_DataBit;
						V_GrUartChTbl[A_Ch].StopBit	=	A_StopBit;
						V_GrUartChTbl[A_Ch].Parity	=	A_ParityBit;
					}
				}
			}
			else
			{
				DbgMsgPrint( "GrUartSetup - channel %d is not exist.\n", A_Ch );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32		GrUartSend( __u8 A_Ch, void* A_PtrBuf, __u32 A_Size, BOOL8 A_IsWait )
{
	// local -------------------
		__u32	Tv_Result;
#ifndef LINUX_APP
		__u32	Tv_Writed;
		void*	Tv_PtrBuf;
		__u32	Tv_TryCnt;
#endif
	// code --------------------
		Tv_Result	=	0;
		if ( E_GrUartMaxCh > A_Ch )
		{
			//check exist
			if ( V_GrUartChTbl[A_Ch].IsExist )
			{
				//check os port
				if ( V_GrUartChTbl[A_Ch].IsOsPort )
				{
					DbgMsgPrint("GrUartSend() - ch = %d, size = %d test1\n", A_Ch, A_Size);
#ifndef LINUX_APP
					Tv_TryCnt	=	0;
					while ( Tv_Result < A_Size )
					{
						Tv_PtrBuf	=	(void*)( (__u32)A_PtrBuf + Tv_Result);
						if ( WriteFile( (HANDLE)V_GrUartChTbl[A_Ch].Id, Tv_PtrBuf, A_Size - Tv_Result, (LPDWORD)&Tv_Writed, NULL ) )
						{
							Tv_Result	=	Tv_Result +	Tv_Writed;
						}
						//check wait
						if ( !A_IsWait )
						{
							break;
						}
						Tv_TryCnt ++;
						if ( 100 < Tv_TryCnt )
						{
							DbgMsgPrint("UartSendFail\n");
							break;
						}
						GrPrcSleep( 10 );
					}
#endif
				}
				else
				{
					Tv_Result	=	(*V_GrUartChTbl[A_Ch].Ops.Send)( V_GrUartChTbl[A_Ch].Id, A_PtrBuf, A_Size, A_IsWait );
				}
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32		GrUartRecv( __u8 A_Ch, void* A_PtrBuf, __u32 A_Size, __u32 A_TimeOut )
{
	// local -------------------
		__u32	Tv_Result;
#ifndef LINUX_APP
		__u32	Tv_Readed;
		__u32	Tv_PrcTime;
		void*	Tv_PtrBuf;
#endif
	// code --------------------
		Tv_Result	=	0;

		if ( E_GrUartMaxCh > A_Ch )
		{
			//check exist
			if ( V_GrUartChTbl[A_Ch].IsExist )
			{
				//check OS port
				if ( V_GrUartChTbl[A_Ch].IsOsPort )
				{
#ifndef LINUX_APP
					Tv_PrcTime	=	0;
					while ( Tv_Result < A_Size )
					{
						Tv_PtrBuf	=	(void*)( (__u32)A_PtrBuf + Tv_Result );
						if ( ReadFile( V_GrUartChTbl[A_Ch].Id, Tv_PtrBuf, A_Size - Tv_Result, (LPDWORD)&Tv_Readed, NULL ) )
						{
							if ( 0 != Tv_Readed )
							{
								Tv_Result	=	Tv_Result + Tv_Readed;
							}
						}
						//check time out
						if ( Tv_PrcTime >= A_TimeOut )
						{
							break;
						}
						GrPrcSleep( E_GrUartOsWaitTime );
						Tv_PrcTime	=	Tv_PrcTime + E_GrUartOsWaitTime;
					}
#endif
				}
				else
				{
					Tv_Result	=	(*V_GrUartChTbl[A_Ch].Ops.Recv)( V_GrUartChTbl[A_Ch].Id, A_PtrBuf, A_Size, A_TimeOut );
					if(0 < Tv_Result)
					{
						DbgMsgPrint("GrUartRecv() - ch = %d, size = %d test1\n", A_Ch, A_Size);
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
