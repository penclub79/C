/*
 configuration - Fds1640

 */

//====================================================================
// uses
#include	<GrLnxApp.h>

#include	<GrTypeBase.h>
#include	<GrTimeTool.h>
#include	<GrDumyTool.h>
#include	<GrStrTool.h>

#include	<GrDebug.h>

//====================================================================
//local const

//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var
Def_StrShort	V_DbgStrDumpMsg;

//====================================================================
//--------------------------------------------------------------------
void	DbgMemDump( void* A_Ptr, unsigned int A_Size, unsigned int A_Unit )
{
	// local -------------------
		__u32		Tv_LastSize;
		void*		Tv_PtrWk;
		__u32		Tv_WkIdx;
		__u32		Tv_WkVal;
	// code --------------------
		//print head
		DbgMsgPrint( "Dump - Addr : 0x" );
		GrStrDwordToHexStr( V_DbgStrDumpMsg, (__u32)(Def_GrCalPtr)A_Ptr, 8, TRUE );
		DbgMsgPrint( V_DbgStrDumpMsg );
		DbgMsgPrint( "\n" );

		Tv_LastSize		=	A_Size;
		Tv_PtrWk			=	A_Ptr;
		while ( 0 != Tv_LastSize )
		{
			//print address
			GrStrDwordToHexStr(V_DbgStrDumpMsg, (__u32)(Def_GrCalPtr)Tv_PtrWk, 8, TRUE);
			DbgMsgPrint( V_DbgStrDumpMsg );
			DbgMsgPrint( " : " );

			//prite contents
			switch ( A_Unit )
			{
				case E_DbgDumpUnitWord:
					for (Tv_WkIdx = 0;Tv_WkIdx < 8;Tv_WkIdx++)
					{
						if ( 2 <= Tv_LastSize )
						{
							Tv_WkVal	=	*((__u16*)Tv_PtrWk);
							GrStrDwordToHexStr( V_DbgStrDumpMsg, Tv_WkVal, 4, TRUE );
							DbgMsgPrint( V_DbgStrDumpMsg );
						}
						else
						{
							Tv_LastSize		=	0;
							break;
						}
						//next
						Tv_LastSize		=	Tv_LastSize - 2;
						Tv_PtrWk			=	(void*)((Def_GrCalPtr)Tv_PtrWk + 2);
						DbgMsgPrint( " " );
					}
					break;
				case E_DbgDumpUnitDword:
					for (Tv_WkIdx = 0;Tv_WkIdx < 4;Tv_WkIdx++)
					{
						if ( 4 <= Tv_LastSize )
						{
							Tv_WkVal	=	*((__u32*)Tv_PtrWk);
							GrStrDwordToHexStr( V_DbgStrDumpMsg, Tv_WkVal, 8, TRUE );
							DbgMsgPrint( V_DbgStrDumpMsg );
						}
						else
						{
							Tv_LastSize		=	0;
							break;
						}
						//next
						Tv_LastSize		=	Tv_LastSize - 4;
						Tv_PtrWk			=	(void*)( (Def_GrCalPtr)Tv_PtrWk + 4 );
						DbgMsgPrint( " " );
					}
					break;
				default:
					for (Tv_WkIdx = 0;Tv_WkIdx < 16;Tv_WkIdx++)
					{
						if ( 1 <= Tv_LastSize )
						{
							Tv_WkVal	=	*((__u8*)Tv_PtrWk);
							GrStrDwordToHexStr( V_DbgStrDumpMsg, Tv_WkVal, 2, TRUE );
							DbgMsgPrint( V_DbgStrDumpMsg );
						}
						else
						{
							Tv_LastSize		=	0;
							break;
						}
						//next
						Tv_LastSize		=	Tv_LastSize - 1;
						Tv_PtrWk			=	(void*)( (Def_GrCalPtr)Tv_PtrWk + 1 );
						DbgMsgPrint( " " );
					}
			    break;
			}

			//go next line
			DbgMsgPrint( "\n" );
		}

}
//--------------------------------------------------------------------
