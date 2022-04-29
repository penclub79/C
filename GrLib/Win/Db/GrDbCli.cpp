/*
product regist server

*/

//====================================================================
//include

#include <Win/Db/GrDbCli.h>

#include <GrDebug.h>
#include <GrError.h>

#include <GrDumyTool.h>
#include <GrTimeTool.h>

#include <Protocol\GrPrtcDb.h>

//====================================================================
//local const


//====================================================================
//local type

//====================================================================
//global var


//====================================================================
//functions

//--------------------------------------------------------------------
Cls_GrDbCli::Cls_GrDbCli(void)
{
	// local -------------------
	// code --------------------
		// init

		m_Sock					=	E_GrSockBadHandle;

}
//--------------------------------------------------------------------
Cls_GrDbCli::~Cls_GrDbCli()
{
	// local -------------------
	// code --------------------


}
//--------------------------------------------------------------------
__u8	Cls_GrDbCli::LcConnect(void)
{
	// local -------------------
		__u8	Tv_RtCode;
		__u32	Tv_TimeOut;
	// code --------------------

		// create socket
		m_Sock	=	GrSockV2Open(FALSE, TRUE, FALSE);
		if(E_GrSockBadHandle == m_Sock)
		{
			return	E_GrErrObjectCreateFail;
		}

		// try connect
		Tv_RtCode	=	GrSockV2Connect(m_Sock, &m_SvIp, m_SvPort, FALSE);
		Tv_TimeOut	=	0;
		while(E_GrSockV2ConnWaiting == Tv_RtCode)
		{
			// check time out
			Tv_TimeOut ++;
			if(E_GrDbCliConnTimeOut <= Tv_TimeOut)
			{
				break;
			}
			// wait
			Tv_RtCode	=	GrSockV2WaitConnect(m_Sock, 10);
		}

		// check succes
		if(E_GrSockV2ConnOk != Tv_RtCode)
		{
			GrSockV2Close(m_Sock);
			m_Sock	=	E_GrSockBadHandle;
			return	E_GrErrFail;
		}

		// set blocking mode
		//GrSockV2SetBlockMode(m_Sock, FALSE);

		return	E_GrErrNone;

}
//--------------------------------------------------------------------
__u8	Cls_GrDbCli::LcSend(void* A_PtrBuf, __u32 A_Size)
{
	// local -------------------
		__u8	Tv_Result;
		__u32	Tv_ReqSize;
		__u32	Tv_SendTotal;
		__s32	Tv_Sended;
		void*	Tv_PtrSend;
		__u32	Tv_Retry;
	// code --------------------
		Tv_Result	=	E_GrErrFail;
		Tv_Retry	=	0;

		Tv_SendTotal	=	0;
		while (300 > Tv_Retry)
		{
			Tv_ReqSize	=	A_Size - Tv_SendTotal;
			Tv_PtrSend	=	(void*)((Def_GrCalPtr)A_PtrBuf + (Def_GrCalPtr)Tv_SendTotal);
			Tv_Sended		=	(__s32)GrSockV2Send(m_Sock, Tv_PtrSend, Tv_ReqSize);
			if (0 < Tv_Sended)
			{
				Tv_SendTotal	=	Tv_SendTotal + (__u32)Tv_Sended;
				if (A_Size <= Tv_SendTotal)
				{
					Tv_Result	=	E_GrErrNone;
					break;
				}
				// next
				continue;
			}
			// retry
			GrPrcSleep(10);
			Tv_Retry ++;
		}

		return	Tv_Result;

}
//--------------------------------------------------------------------
__u8	Cls_GrDbCli::LcRecv(void)
{
	// local -------------------
		__u8	Tv_Result;
		__u32	Tv_RecvTotal;
		__u32	Tv_Retry;
		__u32	Tv_ReqSize;
		__u32	Tv_PktSize;
		__s32	Tv_Recved;
		void*	Tv_PtrRecv;
		Ptr_GrPrtcDbBase	Tv_PtrPktBase;
	// code --------------------
		Tv_Result	=	E_GrErrFail;

		Tv_RecvTotal	=	0;
		Tv_PktSize		=	sizeof(St_GrPrtcDbBase);

		Tv_Retry			=	0;

		while(300 > Tv_Retry)
		{
			Tv_ReqSize	=	Tv_PktSize - Tv_RecvTotal;
			Tv_PtrRecv	=	(void*)((Def_GrCalPtr)m_Buf + (Def_GrCalPtr)Tv_RecvTotal);
			Tv_Recved	=	(__s32)GrSockV2Recv(m_Sock, Tv_PtrRecv, Tv_ReqSize);
			// check close
			if(GrSockV2ChkCloseByRecv(Tv_Recved))
			{
				// aloreay closed
				Tv_Result	=	E_GrSockErrConnBroken;
				break;
			}
			// check recevie total size
			if(0 > Tv_Recved)
			{
				// waiting
				GrPrcSleep(10);
				Tv_Retry ++;
				continue;
			}

			Tv_RecvTotal	=	Tv_RecvTotal + (__u32)Tv_Recved;
			if(Tv_PktSize <= Tv_RecvTotal)
			{
				// check header receive
				if(sizeof(St_GrPrtcDbBase) == Tv_PktSize)
				{
					Tv_PtrPktBase	=	(Ptr_GrPrtcDbBase)m_Buf;
					// check header
					if((E_GrPrtcDbFcc != Tv_PtrPktBase->Fcc) || (sizeof(St_GrPrtcDbBase) > Tv_PtrPktBase->Size))
					{
						Tv_Result	=	E_GrErrDataBroken;
						break;
					}
					// update packet size
					Tv_PktSize	=	Tv_PtrPktBase->Size;
					if(sizeof(St_GrPrtcDbBase) == Tv_PktSize)
					{
						// finish
						Tv_Result	=	E_GrErrNone;
						break;
					}
					// receive next
					Tv_Retry ++;
					continue;
				}

				// receive contents
				Tv_Result	=	E_GrErrNone;
				break;
			}

			// retry
			GrPrcSleep(10);
			Tv_Retry ++;
		}


		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDbCli::LcClose(void)
{
	// local -------------------
	// code --------------------
		if(E_GrSockBadHandle != m_Sock)
		{
			GrSockV2Close(m_Sock);
			m_Sock	=	E_GrSockBadHandle;
		}
}
//--------------------------------------------------------------------
void	Cls_GrDbCli::Setup(__u32 A_SvIp, __u16 A_Port)
{
	// local -------------------
	// code --------------------
		m_SvIp		=	A_SvIp;
		m_SvPort	=	A_Port;
}
//--------------------------------------------------------------------
__u8	Cls_GrDbCli::KeyCreate(__u32 A_Grpid, void* A_PtrKey, __u32 A_Ofs, void* A_PtrData, __u32 A_Size)
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		// connect
		Tv_Result	=	LcConnect();
		if(E_GrErrNone != Tv_Result	)
		{
			return	Tv_Result;
		}

		if((NULL == A_PtrData) || (0 == A_Size) )
		{
			// key create only
			Ptr_GrPrtcDbKeyCreate	Tv_PtrPktKco;
			// build packet
			Tv_PtrPktKco	=	(Ptr_GrPrtcDbKeyCreate)m_Buf;

			Tv_PtrPktKco->Fcc		=	E_GrPrtcDbFcc;
			Tv_PtrPktKco->Cmd		=	E_GrPrtcDbCmdKeyCreate;
			Tv_PtrPktKco->Size	=	sizeof(St_GrPrtcDbKeyCreate);
			Tv_PtrPktKco->GrpId	=	A_Grpid;
			GrDumyCopyMem(Tv_PtrPktKco->KeyVal, A_PtrKey, E_GrPrtcDbKeySize);
			Tv_PtrPktKco->UsrId	=	1;

			// send
			Tv_Result	=	LcSend(m_Buf, sizeof(St_GrPrtcDbKeyCreate));
			if(E_GrErrNone != Tv_Result )
			{
				LcClose();
				return	Tv_Result;
			}

		}
		else
		{
			Ptr_GrPrtcDbKeyNewWithData	Tv_PtrPktKnd;

			// check size and offset
			if(0x10000 < A_Size)
			{
				LcClose();
				return	E_GrErrBadParam;
			}

			Tv_PtrPktKnd	=	(Ptr_GrPrtcDbKeyNewWithData)m_Buf;

			// build packet
			Tv_PtrPktKnd->Fcc		=	E_GrPrtcDbFcc;
			Tv_PtrPktKnd->Cmd		=	E_GrPrtcDbCmdKeyNewWithData;
			Tv_PtrPktKnd->Size	=	sizeof(St_GrPrtcDbKeyNewWithData) + A_Size;
			Tv_PtrPktKnd->GrpId	=	A_Grpid;
			GrDumyCopyMem(Tv_PtrPktKnd->KeyVal, A_PtrKey, E_GrPrtcDbKeySize);
			Tv_PtrPktKnd->UsrId	=	1;
			Tv_PtrPktKnd->Ofs		=	A_Ofs;
			
			// send header
			Tv_Result	=	LcSend(m_Buf, sizeof(St_GrPrtcDbKeyNewWithData));
			if(E_GrErrNone != Tv_Result)
			{
				LcClose();
				return	Tv_Result;
			}

			// send data
			Tv_Result	=	LcSend(A_PtrData, A_Size);
			if(E_GrErrNone != Tv_Result)
			{
				LcClose();
				return	Tv_Result;
			}
		}

		// receive answer
		Tv_Result	=	LcRecv();
		if(E_GrErrNone != Tv_Result)
		{
			LcClose();
			return	Tv_Result;
		}

		// parsing data
		{
			Ptr_GrPrtcDbResult	Tv_PtrPktRsl;

			Tv_PtrPktRsl	=	(Ptr_GrPrtcDbResult)m_Buf;
			if(E_GrPrtcDbCmdResult != Tv_PtrPktRsl->Cmd)
			{
				Tv_Result	=	E_GrErrDataBroken;
			}
			else
			{
				switch(Tv_PtrPktRsl->ErrCode)
				{
					case E_GrPrtcDbResultOk:
						Tv_Result	=	E_GrErrNone;
						break;
					case E_GrPrtcDbResultBadPara:
						Tv_Result	=	E_GrErrBadParam;
						break;
					case E_GrPrtcDbResultNotFound:
						Tv_Result	=	E_GrErrNotFound;
						break;
					case E_GrPrtcDbResultNotEnoughSpace:
						Tv_Result	=	E_GrErrDiskFull;
						break;
					case E_GrPrtcDbResultBusy:
						Tv_Result	=	E_GrErrBusy;
						break;
					default:
						Tv_Result	=	E_GrErrFail;
						break;
				}
			}

		}

		// close
		LcClose();
		return	Tv_Result;

}
//--------------------------------------------------------------------
__u8	Cls_GrDbCli::KeyDelete(__u32 A_GrpId, void* A_PtrKey)
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		// connect
		Tv_Result	=	LcConnect();
		if(E_GrErrNone != Tv_Result)
		{
			return	Tv_Result;
		}

		// send contents
		{
			// key create only
			Ptr_GrPrtcDbKeyDelete	Tv_PtrPktKde;
			// build packet
			Tv_PtrPktKde	=	(Ptr_GrPrtcDbKeyDelete)m_Buf;

			Tv_PtrPktKde->Fcc		=	E_GrPrtcDbFcc;
			Tv_PtrPktKde->Cmd		=	E_GrPrtcDbCmdKeyDelete;
			Tv_PtrPktKde->Size	=	sizeof(St_GrPrtcDbKeyDelete);
			Tv_PtrPktKde->GrpId	=	A_GrpId;
			GrDumyCopyMem(Tv_PtrPktKde->KeyVal, A_PtrKey, E_GrPrtcDbKeySize);
			Tv_PtrPktKde->UsrId	=	1;

			// send
			Tv_Result	=	LcSend(m_Buf, sizeof(St_GrPrtcDbKeyDelete));
			if(E_GrErrNone != Tv_Result)
			{
				LcClose();
				return	Tv_Result;
			}
		}

		// receive answer
		Tv_Result	=	LcRecv();
		if(E_GrErrNone != Tv_Result)
		{
			LcClose();
			return	Tv_Result;
		}

		// parsing data
		{
			Ptr_GrPrtcDbResult	Tv_PtrPktRsl;

			Tv_PtrPktRsl	=	(Ptr_GrPrtcDbResult)m_Buf;
			if(E_GrPrtcDbCmdResult != Tv_PtrPktRsl->Cmd)
			{
				Tv_Result	=	E_GrErrDataBroken;
			}
			else
			{
				switch(Tv_PtrPktRsl->ErrCode)
				{
				case E_GrPrtcDbResultOk:
					Tv_Result	=	E_GrErrNone;
					break;
				case E_GrPrtcDbResultBadPara:
					Tv_Result	=	E_GrErrBadParam;
					break;
				case E_GrPrtcDbResultNotFound:
					Tv_Result	=	E_GrErrNotFound;
					break;
				case E_GrPrtcDbResultNotEnoughSpace:
					Tv_Result	=	E_GrErrDiskFull;
					break;
				case E_GrPrtcDbResultBusy:
					Tv_Result	=	E_GrErrBusy;
					break;
				default:
					Tv_Result	=	E_GrErrFail;
					break;
				}
			}

		}

		// close
		LcClose();
		return	Tv_Result;

}
//--------------------------------------------------------------------
__u8	Cls_GrDbCli::DataWrite(__u32 A_GrpId, void* A_PtrKey, __u32 A_Ofs, void* A_PtrData, __u32 A_Size)
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		// connect
		Tv_Result	=	LcConnect();
		if(E_GrErrNone != Tv_Result)
		{
			return	Tv_Result;
		}

		// send contents
		{
			Ptr_GrPrtcDbPutDataByOfs	Tv_PtrPktDwt;

			// check size and offset
			if(0x10000 < A_Size)
			{
				LcClose();
				return	E_GrErrBadParam;
			}

			Tv_PtrPktDwt		=	(Ptr_GrPrtcDbPutDataByOfs)m_Buf;

			// build packet
			Tv_PtrPktDwt->Fcc		=	E_GrPrtcDbFcc;
			Tv_PtrPktDwt->Cmd		=	E_GrPrtcDbCmdDataSetByOffset;
			Tv_PtrPktDwt->Size	=	sizeof(St_GrPrtcDbPutDataByOfs) + A_Size;
			Tv_PtrPktDwt->GrpId	=	A_GrpId;
			GrDumyCopyMem(Tv_PtrPktDwt->KeyVal, A_PtrKey, E_GrPrtcDbKeySize);
			Tv_PtrPktDwt->UsrId	=	1;
			Tv_PtrPktDwt->Ofs		=	A_Ofs;

			// send header
			Tv_Result	=	LcSend(m_Buf, sizeof(St_GrPrtcDbPutDataByOfs));
			if(E_GrErrNone != Tv_Result)
			{
				LcClose();
				return	Tv_Result;
			}

			// send data
			Tv_Result	=	LcSend(A_PtrData, A_Size);
			if(E_GrErrNone != Tv_Result)
			{
				LcClose();
				return	Tv_Result;
			}
		}

		// receive answer
		Tv_Result	=	LcRecv();
		if(E_GrErrNone != Tv_Result)
		{
			LcClose();
			return	Tv_Result;
		}

		// parsing data
		{
			Ptr_GrPrtcDbResult	Tv_PtrPktRsl;

			Tv_PtrPktRsl	=	(Ptr_GrPrtcDbResult)m_Buf;
			if(E_GrPrtcDbCmdResult != Tv_PtrPktRsl->Cmd)
			{
				Tv_Result	=	E_GrErrDataBroken;
			}
			else
			{
				switch(Tv_PtrPktRsl->ErrCode)
				{
				case E_GrPrtcDbResultOk:
					Tv_Result	=	E_GrErrNone;
					break;
				case E_GrPrtcDbResultBadPara:
					Tv_Result	=	E_GrErrBadParam;
					break;
				case E_GrPrtcDbResultNotFound:
					Tv_Result	=	E_GrErrNotFound;
					break;
				case E_GrPrtcDbResultNotEnoughSpace:
					Tv_Result	=	E_GrErrDiskFull;
					break;
				case E_GrPrtcDbResultBusy:
					Tv_Result	=	E_GrErrBusy;
					break;
				default:
					Tv_Result	=	E_GrErrFail;
					break;
				}
			}

		}

		// close
		LcClose();
		return	Tv_Result;

}
//--------------------------------------------------------------------
__u8	Cls_GrDbCli::DataRead(__u32 A_GrpId, void* A_PtrKey, __u32 A_Ofs, void* A_PtrData, __u32 A_Size)
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		// connect
		Tv_Result	=	LcConnect();
		if(E_GrErrNone != Tv_Result)
		{
			return	Tv_Result;
		}

		// send contents
		{
			Ptr_GrPrtcDbGetDataByOfs	Tv_PtrPktDrd;
			Ptr_GrPrtcDbReqItem	Tv_PtrPktReq;

			// check size and offset
			if(0x10000 < A_Size)
			{
				LcClose();
				return	E_GrErrBadParam;
			}

			Tv_PtrPktDrd					=	(Ptr_GrPrtcDbGetDataByOfs)m_Buf;

			// build packet
			Tv_PtrPktDrd->Fcc			=	E_GrPrtcDbFcc;
			Tv_PtrPktDrd->Cmd			=	E_GrPrtcDbCmdDataGetByOffset;
			Tv_PtrPktDrd->Size		=	sizeof(St_GrPrtcDbGetDataByOfs) + sizeof(St_GrPrtcDbReqItem) + A_Size;
			Tv_PtrPktDrd->GrpId		=	A_GrpId;
			GrDumyCopyMem(Tv_PtrPktDrd->KeyVal, A_PtrKey, E_GrPrtcDbKeySize);
			Tv_PtrPktDrd->UsrId		=	1;
			Tv_PtrPktDrd->ReqCnt	=	1;

			Tv_PtrPktReq					=	(Ptr_GrPrtcDbReqItem)((Def_GrCalPtr)m_Buf + sizeof(St_GrPrtcDbGetDataByOfs));
			Tv_PtrPktReq->Ofs			=	A_Ofs;
			Tv_PtrPktReq->Size		=	A_Size;

			// send header
			Tv_Result	=	LcSend(m_Buf, sizeof(St_GrPrtcDbGetDataByOfs) + sizeof(St_GrPrtcDbReqItem));
			if(E_GrErrNone != Tv_Result)
			{
				LcClose();
				return	Tv_Result;
			}

		}

		// receive answer
		Tv_Result	=	LcRecv();
		if(E_GrErrNone != Tv_Result)
		{
			LcClose();
			return	Tv_Result;
		}

		// parsing data
		{
			Ptr_GrPrtcDbRtnDataOne	Tv_PtrPktRsl;

			Tv_PtrPktRsl	=	(Ptr_GrPrtcDbRtnDataOne)m_Buf;
			if(E_GrPrtcDbCmdRtnDataOne != Tv_PtrPktRsl->Cmd)
			{
				Tv_Result	=	E_GrErrDataBroken;
			}
			else
			{
				switch(Tv_PtrPktRsl->ErrCode)
				{
					case E_GrPrtcDbResultOk:
						Tv_Result	=	E_GrErrNone;
						break;
					case E_GrPrtcDbResultBadPara:
						Tv_Result	=	E_GrErrBadParam;
						break;
					case E_GrPrtcDbResultNotFound:
						Tv_Result	=	E_GrErrNotFound;
						break;
					case E_GrPrtcDbResultNotEnoughSpace:
						Tv_Result	=	E_GrErrDiskFull;
						break;
					case E_GrPrtcDbResultBusy:
						Tv_Result	=	E_GrErrBusy;
						break;
					default:
						Tv_Result	=	E_GrErrFail;
						break;
				}

				// update data
				if(E_GrErrNone == Tv_Result)
				{
					// check size
					if((sizeof(St_GrPrtcDbRtnDataOne) + A_Size) != Tv_PtrPktRsl->Size)
					{
						Tv_Result	=	E_GrErrDataBroken;
					}
					else
					{
						void*	Tv_PtrData;

						// copy
						Tv_PtrData	=	(void*)((Def_GrCalPtr)m_Buf + sizeof(St_GrPrtcDbRtnDataOne));
						GrDumyCopyMem(A_PtrData, Tv_PtrData, A_Size);
					}
				}
			}
		}

		// close
		LcClose();
		return	Tv_Result;

}
//--------------------------------------------------------------------

