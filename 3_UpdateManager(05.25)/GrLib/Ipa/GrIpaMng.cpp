/*
		image process manager

 */

//====================================================================
// uses
#include <Ipa/GrIpaMng.h>
#include <GrDebug.h>
#include <GrDrawTool.h>
#include <GrDumyTool.h>
#include <GrStrTool.h>

#include <GrBufTool.h>

#include <Ipa/GrIpaObjSrcIn.h>
#include <Ipa/GrIpaObjMonOut.h>
#include <Ipa/GrIpaObjSplitYuv.h>
#include <Ipa/GrIpaObjBindYuv.h>
#include <Ipa/GrIpaObjBinary.h>
#include <Ipa/GrIpaObjWdr.h>

//====================================================================
//local const

//====================================================================
//local type

//====================================================================
//local macro

//====================================================================
//local function

//====================================================================
//var

Cls_GrIpaMng*	V_GrIpaMng	=	NULL;

//====================================================================
//--------------------------------------------------------------------
Cls_GrIpaMng::Cls_GrIpaMng( void )
{
	// local -------------------
	// code --------------------
		//init
		GrDumyZeroMem( m_ObjTbl, sizeof(m_ObjTbl) );

		GrBufVqInit( (Ptr_GrBufVarQue)m_CmdBuf, sizeof(m_CmdBuf), 4 );		// command queue initialize

		// update manager
		V_GrIpaMng	=	this;
}
//--------------------------------------------------------------------
Cls_GrIpaMng::~Cls_GrIpaMng( void )
{
	// local -------------------
	// code --------------------
		Reset();

		V_GrIpaMng	=	NULL;
}
//--------------------------------------------------------------------
void	Cls_GrIpaMng::Reset( void )
{
	// local -------------------
		__u32	Tv_WkIdx;
	// code --------------------
		for (Tv_WkIdx=0;Tv_WkIdx < E_GrIpaMngMaxObjCnt;Tv_WkIdx++)
		{
			if ( NULL != m_ObjTbl[Tv_WkIdx] )
			{
				delete	m_ObjTbl[Tv_WkIdx];
				m_ObjTbl[Tv_WkIdx]	=	NULL;
			}
		}
}
//--------------------------------------------------------------------
Cls_GrIpaObjBase*	Cls_GrIpaMng::LcObjCreate( __u16 A_Type )
{
	// local -------------------
		Cls_GrIpaObjBase*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;

		switch ( A_Type )
		{
			case E_GrIpaAdptTypeSrcIn:
				Tv_Result	=	(Cls_GrIpaObjBase*) new Cls_GrIpaObjSrcIn();
				break;
			case E_GrIpaAdptTypeMonOut:
				Tv_Result	=	(Cls_GrIpaObjBase*) new Cls_GrIpaObjMonOut();
				break;
			case E_GrIpaAdptTypeSplitYuv:
				Tv_Result	=	(Cls_GrIpaObjBase*) new Cls_GrIpaObjSplitYuv();
				break;
			case E_GrIpaAdptTypeBindYuv:
				Tv_Result	=	(Cls_GrIpaObjBase*) new Cls_GrIpaObjBindYuv();
				break;
			case E_GrIpaAdptTypeBinary:
				Tv_Result	=	(Cls_GrIpaObjBase*) new Cls_GrIpaObjBinary();
				break;
			case E_GrIpaAdptTypeWdr:
				Tv_Result	=	(Cls_GrIpaObjBase*) new Cls_GrIpaObjWdr();
				break;
			default:
				DbgMsgPrint( "Cls_GrIpaMng::LcObjCreate - unknown adoptor type ( %d )\n", A_Type );
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Cls_GrIpaObjBase*	Cls_GrIpaMng::ObjCreate( __u16 A_Type, __u16 A_ScptId )
{
	// local -------------------
		Cls_GrIpaObjBase*	Tv_Result;
		__u16	Tv_WkIdx;
		__u16	Tv_ScptId;
	// code --------------------
		Tv_Result	=	NULL;

		Tv_ScptId	=	A_ScptId;

		if ( E_GrIpaScptIdNone == Tv_ScptId )
		{
			// find empty slot
			for (Tv_WkIdx = 0;Tv_WkIdx < E_GrIpaMngMaxObjCnt;Tv_WkIdx++)
			{
				if ( NULL == m_ObjTbl[Tv_WkIdx] )
				{
					// found ID
					Tv_ScptId	=	Tv_WkIdx + E_GrIpaScptIdBase;
					break;
				}
			}
		}
		else
		{
			Tv_WkIdx	=	A_ScptId - E_GrIpaScptIdBase;
			if ( E_GrIpaMngMaxObjCnt <= Tv_WkIdx )
			{
				Tv_ScptId	=	E_GrIpaScptIdNone;
			}
			else if ( NULL != m_ObjTbl[Tv_WkIdx] )
			{
				Tv_ScptId	=	E_GrIpaScptIdNone;
			}
		}

		// check able
		if ( E_GrIpaScptIdNone != Tv_ScptId )
		{
			Tv_WkIdx	=	Tv_ScptId - E_GrIpaScptIdBase;
			Tv_Result	=	LcObjCreate( A_Type );
			if ( NULL != Tv_Result )
			{
				// update info
				Tv_Result->ScptIdSet( Tv_ScptId );		// update script id
				m_ObjTbl[Tv_WkIdx]	=	Tv_Result;			// register object table
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void*	Cls_GrIpaMng::SourceInfoGet( __u32 A_SourceId, __u32* A_PtrRtStrmType, __u32* A_PtrRtInfoSize )
{
	// local -------------------
	// code --------------------
		return	NULL;
}
//--------------------------------------------------------------------
void*	Cls_GrIpaMng::SourceStrmGet( __u32 A_SourceId, __u32* A_PtrRtStrmSize )
{
	// local -------------------
	// code --------------------
		return	NULL;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrIpaMng::MonitorSetup( __u32 A_MonId, __u32 A_StrmTYpe, void* A_PtrInfo, __u32 A_InfoSize )
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrIpaMng::MonitorOut( __u32 A_MonId, void* A_PtrStrm, __u32 A_StrmSize )
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrIpaMng::CommandInput( Ptr_GrIpaMngCmdBase A_PtrCmd, __u32 A_CmdSize )
{
	// local -------------------
		BOOL8	Tv_Result;
		void*	Tv_PtrBuf;
	// code --------------------
		Tv_Result	=	FALSE;

		// get message buffer
		Tv_PtrBuf	=	GrBufVqGetInPtr( (Ptr_GrBufVarQue)m_CmdBuf, A_CmdSize );
		if ( NULL != Tv_PtrBuf )
		{
			// input
			GrDumyCopyMem( Tv_PtrBuf, A_PtrCmd, A_CmdSize );
			GrBufVqIn( (Ptr_GrBufVarQue)m_CmdBuf );
			// success
			Tv_Result	=	TRUE;
		}
		else
		{
			DbgMsgPrint( "Cls_GrIpaMng::CommandInput - command buffer full!\n" );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrIpaMng::CommandOnePump( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrIpaMngCmdBase	Tv_PtrCmd;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_PtrCmd	=	(Ptr_GrIpaMngCmdBase)GrBufVqGetOutPtr( (Ptr_GrBufVarQue)m_CmdBuf );
		if ( NULL != Tv_PtrCmd )
		{
			LcDoCmd( Tv_PtrCmd );
			// release command
			GrBufVqOut( (Ptr_GrBufVarQue)m_CmdBuf );
			
			// success
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Cls_GrIpaObjBase*	Cls_GrIpaMng::LcObjByScptId( __u16 A_Id )
{
	// local -------------------
		Cls_GrIpaObjBase*	Tv_Result;
		__u16	Tv_Idx;
	// code --------------------
		Tv_Result	=	NULL;

		if ( (E_GrIpaScptIdBase <= A_Id) && ( (E_GrIpaScptIdBase + E_GrIpaMngMaxObjCnt) > A_Id ) )
		{
			Tv_Idx	=	A_Id - E_GrIpaScptIdBase;
			Tv_Result	=	m_ObjTbl[Tv_Idx];
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrIpaMng::LcDoCmd( Ptr_GrIpaMngCmdBase A_PtrCmd )
{
	// local -------------------
		BOOL8	Tv_Result;

	// code --------------------
		Tv_Result	=	TRUE;

		switch ( A_PtrCmd->Cmd )
		{
			case E_GrIpaMngCmdSetup:
				LcPcmdSetup( (Ptr_GrIpaMngCmdSetup)A_PtrCmd );
				break;
			case E_GrIpaMngCmdProcess:
				LcPcmdProcess( (Ptr_GrIpaMngCmdProcess)A_PtrCmd );
				break;
			default:
				DbgMsgPrint( "Cls_GrIpaMng::LcDoCmd - unknown command %d \n", A_PtrCmd->Cmd );
				Tv_Result	=	FALSE;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrIpaMng::LcPcmdSetup( Ptr_GrIpaMngCmdSetup A_PtrCmd )
{
	// local -------------------
		Cls_GrIpaObjBase*	Tv_Adaptor;
	// code --------------------
		Tv_Adaptor	=	LcObjByScptId( A_PtrCmd->ScptId );

		if ( NULL != Tv_Adaptor )
		{
			if ( !Tv_Adaptor->RtlSetupPlug( A_PtrCmd->PlugIdx, A_PtrCmd->StrmType, A_PtrCmd->PtrStrmInfo, A_PtrCmd->StrmInfoSize ) )
			{
				DbgMsgPrint( "Cls_GrIpaMng::LcPcmdSetup - adaptor ( %d ) plug ( %d ) setup fail.\n", A_PtrCmd->ScptId, A_PtrCmd->PlugIdx );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrIpaMng::LcPcmdSetup - source adaptor ( %d ) not found\n", A_PtrCmd->ScptId );
		}

}
//--------------------------------------------------------------------
BOOL8	Cls_GrIpaMng::LcSetupAdaptor( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_WkIdx;
	// code --------------------
		Tv_Result	=	TRUE;

		// source setup
		for (Tv_WkIdx=0;Tv_WkIdx < E_GrIpaMngMaxObjCnt;Tv_WkIdx++)
		{
			if ( NULL != m_ObjTbl[Tv_WkIdx] )
			{
				m_ObjTbl[Tv_WkIdx]->RtlSetupSource();
			}
		}

		// pump command
		while ( CommandOnePump() )
		{
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrIpaMng::LcProcess( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_WkIdx;
	// code --------------------
		Tv_Result	=	FALSE;

		// source process
		for (Tv_WkIdx=0;Tv_WkIdx < E_GrIpaMngMaxObjCnt;Tv_WkIdx++)
		{
			if ( NULL != m_ObjTbl[Tv_WkIdx] )
			{
				if ( m_ObjTbl[Tv_WkIdx]->RtlProcessSource() )
				{
					Tv_Result	=	TRUE;
				}
			}
		}

		// pump command
		while ( CommandOnePump() )
		{
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrIpaMng::LcPcmdProcess( Ptr_GrIpaMngCmdProcess A_PtrCmd )
{
	// local -------------------
		Cls_GrIpaObjBase*	Tv_Adaptor;
	// code --------------------
		Tv_Adaptor	=	LcObjByScptId( A_PtrCmd->ScptId );
		if ( NULL != Tv_Adaptor )
		{
			if ( !Tv_Adaptor->RtlPlugSend( A_PtrCmd->PlugIdx, A_PtrCmd->PtrStrm, A_PtrCmd->StrmSize ) )
			{
				DbgMsgPrint( "Cls_GrIpaMng::LcPcmdProcess - adaptor ( %d ) plug ( %d ) send fail.\n", A_PtrCmd->ScptId, A_PtrCmd->PlugIdx );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrIpaMng::LcPcmdProcess - source adaptor ( %d ) not found\n", A_PtrCmd->ScptId );
		}
}
//--------------------------------------------------------------------
Cls_GrIpaObjBase*	Cls_GrIpaMng::ObjGet( __u16 A_ScptId )
{
	// local -------------------
		Cls_GrIpaObjBase*	Tv_Result;
		__u16	Tv_ObjIdx;
	// code --------------------
		Tv_Result	=	NULL;

		if ( (E_GrIpaScptIdBase <= A_ScptId) && ( (E_GrIpaScptIdBase + E_GrIpaMngMaxObjCnt) > A_ScptId ) )
		{
			Tv_ObjIdx	=	A_ScptId - E_GrIpaScptIdBase;
			Tv_Result	=	m_ObjTbl[Tv_ObjIdx];			
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrIpaMng::Run( void )
{
	// local -------------------
	// code --------------------
		LcSetupAdaptor();
		return	LcProcess( );
}
//--------------------------------------------------------------------
#ifdef GR_IPA_EDIT
void	Cls_GrIpaMng::LcScptEncObjPara( void )
{
	// local -------------------
		Ptr_GrIpaScptSctHead	Tv_PtrSctHd;
		__u32	Tv_WkIdx;
		void*	Tv_PtrScpt;
		__u32	Tv_ScptSize;
	// code --------------------
		// create section header
		Tv_PtrSctHd	=	(Ptr_GrIpaScptSctHead)( (__u32)m_PtrScptEnc + m_ScptEncPrgs );
		Tv_PtrSctHd->IdSection	=	E_GrIpaScptSctFcc;
		Tv_PtrSctHd->Type				=	E_GrIpaSectObjPara;
		Tv_PtrSctHd->Para				=	0;
		Tv_PtrSctHd->Size				=	sizeof(St_GrIpaScptSctHead);

		m_ScptEncPrgs	=	m_ScptEncPrgs + sizeof(St_GrIpaScptSctHead);

		// dump object script
		for (Tv_WkIdx=0;Tv_WkIdx < E_GrIpaMngMaxObjCnt;Tv_WkIdx++)
		{
			if ( NULL != m_ObjTbl[Tv_WkIdx] )
			{
				Tv_PtrScpt	=	(void*)( (__u32)m_PtrScptEnc + m_ScptEncPrgs );
				Tv_ScptSize	=	m_ObjTbl[Tv_WkIdx]->ParaEnc( Tv_PtrScpt );
				// update size
				Tv_PtrSctHd->Size	=	Tv_PtrSctHd->Size + Tv_ScptSize;
				m_ScptEncPrgs	=	m_ScptEncPrgs + Tv_ScptSize;
			}
		}

}
#endif
//--------------------------------------------------------------------
#ifdef GR_IPA_EDIT
void	Cls_GrIpaMng::LcScptEncObjChain( void )
{
	// local -------------------
		Ptr_GrIpaScptSctHead	Tv_PtrSctHd;
		__u32	Tv_WkIdx;
		void*	Tv_PtrScpt;
		__u32	Tv_ScptSize;
	// code --------------------
		// create section header
		Tv_PtrSctHd	=	(Ptr_GrIpaScptSctHead)( (__u32)m_PtrScptEnc + m_ScptEncPrgs );
		Tv_PtrSctHd->IdSection	=	E_GrIpaScptSctFcc;
		Tv_PtrSctHd->Type				=	E_GrIpaSectObjChain;
		Tv_PtrSctHd->Para				=	0;
		Tv_PtrSctHd->Size				=	sizeof(St_GrIpaScptSctHead);
		
		m_ScptEncPrgs	=	m_ScptEncPrgs + sizeof(St_GrIpaScptSctHead);

		// dump object chain
		for (Tv_WkIdx=0;Tv_WkIdx < E_GrIpaMngMaxObjCnt;Tv_WkIdx++)
		{
			if ( NULL != m_ObjTbl[Tv_WkIdx] )
			{
				Tv_PtrScpt	=	(void*)( (__u32)m_PtrScptEnc + m_ScptEncPrgs );
				Tv_ScptSize	=	m_ObjTbl[Tv_WkIdx]->ChainEnc( Tv_PtrScpt );
				// update size
				Tv_PtrSctHd->Size	=	Tv_PtrSctHd->Size + Tv_ScptSize;
				m_ScptEncPrgs	=	m_ScptEncPrgs + Tv_ScptSize;
			}
		}

}
#endif
//--------------------------------------------------------------------
#ifdef GR_IPA_EDIT
__u32	Cls_GrIpaMng::ScriptEncode( void* A_PtrBuf )
{
	// local -------------------
	// code --------------------
		// init
		m_PtrScptEnc	=	A_PtrBuf;
		m_ScptEncPrgs	=	0;

		// build object script
		LcScptEncObjPara();

		// build object chain
		LcScptEncObjChain();

		return	m_ScptEncPrgs;
}
#endif
//--------------------------------------------------------------------
BOOL8	Cls_GrIpaMng::ScriptDecode( void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrIpaScptSctHead	Tv_PtrScHd;
		__u32	Tv_Prgs;
		void*	Tv_PtrPayload;
		__u32	Tv_PayloadSize;
	// code --------------------
		Tv_Result	=	TRUE;

		// release all object
		Reset();

		Tv_Prgs		=	0;

		// section decode
		while ( Tv_Prgs < A_Size )
		{
			// get section header
			Tv_PtrScHd	=	(Ptr_GrIpaScptSctHead)( (__u32)A_PtrBuf + Tv_Prgs );
			// check identify
			if ( E_GrIpaScptSctFcc == Tv_PtrScHd->IdSection )
			{
				// check section size
				if ( (Tv_Prgs + (__u32)Tv_PtrScHd->Size) <= A_Size )
				{
					// setup section decode
					Tv_PtrPayload		=	(void*)( (__u32)Tv_PtrScHd + sizeof(St_GrIpaScptSctHead) );
					Tv_PayloadSize	=	(__u32)Tv_PtrScHd->Size - sizeof(St_GrIpaScptSctHead);

					if ( !LcSectionDecode( Tv_PtrScHd->Type, Tv_PtrScHd->Para, Tv_PtrPayload, Tv_PayloadSize ) )
					{
						DbgMsgPrint( "Cls_GrIpaMng::ScriptDecode - (%08X) section not decode.\n" , Tv_Prgs );
						Tv_Result	=	FALSE;
						break;
					}
					// next
					Tv_Prgs	=	Tv_Prgs + (__u32)Tv_PtrScHd->Size;
				}
				else
				{
					DbgMsgPrint( "Cls_GrIpaMng::ScriptDecode - (%08X) bad section size.\n" , Tv_Prgs );
					Tv_Result	=	FALSE;
					break;
				}
			}
			else
			{
				DbgMsgPrint( "Cls_GrIpaMng::ScriptDecode - (%08X) bad section identify.\n" , Tv_Prgs );
				Tv_Result	=	FALSE;
				break;
			}
		}	//	while ( Tv_Prgs < A_Size )

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrIpaMng::LcSectionDecode( __u8 A_Type, __u8 A_Para, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;

		switch ( A_Type )
		{
			case E_GrIpaSectObjPara:
				Tv_Result	=	LcScptParaDec( A_PtrBuf, A_Size );
				break;
			case E_GrIpaSectObjChain:
				Tv_Result	=	LcScptChainDec( A_PtrBuf, A_Size );
				break;
			default:
				Tv_Result	=	FALSE;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrIpaMng::LcScptParaDec( void* A_PtrScpt, __u32 A_ScptSize )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_Prgs;
		Ptr_GrIpaScptObjHead	Tv_PtrObjHd;
		Cls_GrIpaObjBase*	Tv_Obj;
		void*	Tv_PtrPara;
		__u32	Tv_ParaSize;
	// code --------------------
		Tv_Result	=	TRUE;

		Tv_Prgs		=	0;

		while ( Tv_Prgs < A_ScptSize )
		{
			Tv_PtrObjHd	=	(Ptr_GrIpaScptObjHead)( (__u32)A_PtrScpt + Tv_Prgs );
			if ( E_GrIpaScptObjFcc == Tv_PtrObjHd->Identify )
			{
				if ( (Tv_Prgs + (__u32)Tv_PtrObjHd->Size) <= A_ScptSize )
				{
					// check current script id
					if ( E_GrIpaScptIdNone != Tv_PtrObjHd->ScptId )
					{
						Tv_Obj	=	ObjCreate( Tv_PtrObjHd->Type, Tv_PtrObjHd->ScptId );
						if ( NULL != Tv_Obj )
						{
							// parameter decode
							Tv_PtrPara	=	(void*)( (__u32)Tv_PtrObjHd + sizeof(St_GrIpaScptObjHead) );
							Tv_ParaSize	=	(__u32)Tv_PtrObjHd->Size - sizeof(St_GrIpaScptObjHead);
							if ( 0 != Tv_ParaSize )
							{
								if ( !Tv_Obj->RtlParaDec( Tv_PtrPara, Tv_ParaSize ) )
								{
									Tv_Result	=	FALSE;
									DbgMsgPrint( "Cls_GrIpaMng::LcScptParaDec - (%08X) bad parameter.\n", Tv_Prgs );
									break;
								}
							}
							// next
							Tv_Prgs	=	Tv_Prgs + (__u32)Tv_PtrObjHd->Size;
						}
						else
						{
							Tv_Result	=	FALSE;
							DbgMsgPrint( "Cls_GrIpaMng::LcScptParaDec - (%08X) bad object create fail.\n", Tv_Prgs );
							break;
						}
					}
					else
					{
						Tv_Result	=	FALSE;
						DbgMsgPrint( "Cls_GrIpaMng::LcScptParaDec - (%08X) bad object script ID.\n", Tv_Prgs );
						break;
					}
				}
				else
				{
					Tv_Result	=	FALSE;
					DbgMsgPrint( "Cls_GrIpaMng::LcScptParaDec - (%08X) bad object script size.\n", Tv_Prgs );
					break;
				}
			}
			else
			{
				Tv_Result	=	FALSE;
				DbgMsgPrint( "Cls_GrIpaMng::LcScptParaDec - (%08X) bad object header identify.\n", Tv_Prgs );
				break;
			}
		}	//	while ( Tv_Prgs < A_ScptSize )

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrIpaMng::LcScptChainDec( void* A_PtrScpt, __u32 A_ScptSize )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_Prgs;
		Ptr_GrIpaScptChainHead	Tv_PtrChHd;		// chain head
		Cls_GrIpaObjBase*	Tv_Obj;
		void*	Tv_PtrData;
		__u32	Tv_DataSize;
	// code --------------------
		Tv_Result	=	TRUE;

		Tv_Prgs		=	0;

		while ( Tv_Prgs < A_ScptSize )
		{
			Tv_PtrChHd	=	(Ptr_GrIpaScptChainHead)( (__u32)A_PtrScpt + Tv_Prgs );
			if ( E_GrIpaScptChainFcc == Tv_PtrChHd->Identify )
			{
				if ( (Tv_Prgs + (__u32)Tv_PtrChHd->Size) <= A_ScptSize )
				{
					Tv_Obj	=	ObjGet( Tv_PtrChHd->ScptId );
					if ( NULL != Tv_Obj	)
					{
						// try chain
						Tv_PtrData	=	(void*)( (__u32)Tv_PtrChHd + sizeof(St_GrIpaScptChainHead) );
						Tv_DataSize	=	(__u32)Tv_PtrChHd->Size - sizeof(St_GrIpaScptChainHead);
						Tv_Obj->RtlChainDec( Tv_PtrData, Tv_DataSize );
						// next
						Tv_Prgs	=	Tv_Prgs + (__u32)Tv_PtrChHd->Size;
					}
					else
					{
						Tv_Result	=	FALSE;
						DbgMsgPrint( "Cls_GrIpaMng::LcScptChainDec - (%08X) object (%d) not found.\n", Tv_Prgs, Tv_PtrChHd->ScptId );
						break;
					}
				}
				else
				{
					Tv_Result	=	FALSE;
					DbgMsgPrint( "Cls_GrIpaMng::LcScptChainDec - (%08X) bad chain script size.\n", Tv_Prgs );
					break;
				}
			}
			else
			{
				Tv_Result	=	FALSE;
				DbgMsgPrint( "Cls_GrIpaMng::LcScptChainDec - (%08X) bad chain header identify.\n", Tv_Prgs );
				break;
			}

		}	//	while ( Tv_Prgs < A_ScptSize )

		return	Tv_Result;
}
//--------------------------------------------------------------------

