/*
	emulation linux solo device 

*/

//====================================================================
//include

#include	<EmuLnx/GrElmDevNetra.h>
#include	<GrDumyTool.h>
#include	<GrBufTool.h>
#include	<GrGdibTool.h>
#include	<GrFileCtrl.h>
#include	<Dvr/GrDvrBase.h>


//====================================================================
//local const
#define E_GrElmDevNetraMaxObjCnt		2

//====================================================================
//local type


//====================================================================
//global var

DWORD	V_GrEmlDevNetraCompCnt	=	0;	

Cls_GrElmDevNetra*	V_GrElmDevNetraObj[E_GrElmDevNetraMaxObjCnt]	=	
{	NULL, NULL };
file_operations			V_GrElmDevNetraFo;

VENC_CALLBACK_NEW_DATA	V_GrElmDevNetraCbEnc	=	NULL;

//====================================================================
//functions
//DWORD WINAPI GrElmDevSoloProcThrd( LPVOID A_Para );

int GrEmuDevNetra_ioctl (struct inode * A_PtrInode, struct file * A_PtrFile, DWORD A_Para1, DWORD A_Para2);
int GrEmuDevNetra_mmap (struct file * A_PtrFile, struct vm_area_struct * A_PtrVm);
int GrEmuDevNetra_open (struct inode * A_PtrInode, struct file * A_PtrFile);
int GrEmuDevNetra_release (struct inode * A_PtrInode, struct file * A_PtrFile);

//--------------------------------------------------------------------
Cls_GrElmDevNetra::Cls_GrElmDevNetra( DWORD A_DevIdx, BYTE A_FbIdx ):
Cls_GrElmDevBase( A_DevIdx )
{
	// local -------------------
		cdev	Tv_Cdev;
		BYTE	Tv_WkIdx;
	// code --------------------
		//init
		m_FbIdx	=	A_FbIdx;
		// register driver
		switch(m_FbIdx)
		{
			case 0:
				alloc_chrdev_region( (dev_t*)&m_Node, 0, 1, "fb0" );
				break;
			case 1:
				alloc_chrdev_region( (dev_t*)&m_Node, 0, 2, "fb2" );
				break;
		}
		
		V_GrElmDevNetraFo.open			=	GrEmuDevNetra_open;
		V_GrElmDevNetraFo.release		=	GrEmuDevNetra_release;
		V_GrElmDevNetraFo.ioctl			=	GrEmuDevNetra_ioctl;
		V_GrElmDevNetraFo.mmap			=	GrEmuDevNetra_mmap;

		cdev_init( &Tv_Cdev, &V_GrElmDevNetraFo );
		cdev_add( &Tv_Cdev, m_Node, 1 );		// register ioctl

		m_PtrFb	=	malloc( E_GrElmDevNetraFbSize );

		V_GrElmDevNetraObj[m_FbIdx]	=	this;

		if ( 0 == m_FbIdx )
		{

			// emulate video
			m_EmuVdoCnt		=	0;
			m_EmuVdoWait	=	0;
			m_VencCh			=	0;
			GrDumyZeroMem( m_VencTbl, sizeof(m_VencTbl) );
			for (Tv_WkIdx=0;Tv_WkIdx < E_GrDvrMaxChCnt;Tv_WkIdx++)
			{
				m_VencTbl[Tv_WkIdx].IsOn	=	TRUE;
			}
			LcEmuAvLoad();
		}

		//thread create
		//GrPrcThrdCreate( &m_HndlThrd, GrElmDevSoloProcThrd, (void*)this );
}
//--------------------------------------------------------------------
Cls_GrElmDevNetra::~Cls_GrElmDevNetra()
{
	// local -------------------
	// code --------------------
		//release thread
		//GrPrcThrdRelease( &m_HndlThrd );

		if ( NULL != m_PtrFb )
		{
			free( m_PtrFb );
			m_PtrFb	=	NULL;
		}

		V_GrElmDevNetraObj[m_FbIdx]	=	NULL;
}
//--------------------------------------------------------------------
/*
DWORD WINAPI GrElmDevSoloProcThrd( LPVOID A_Para )
{
	// local -------------------
		Cls_GrElmDevSolo*	Tv_ObjSolo;
	// code --------------------
		Tv_ObjSolo	=	(Cls_GrElmDevSolo*)A_Para;
		Tv_ObjSolo->RtlThrdProc();

		return	0;
}
*/
//--------------------------------------------------------------------
void*	Cls_GrElmDevNetra::Mmap( DWORD A_Offset, DWORD A_Size )
{
	// local -------------------
		void*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;

		// check allocated frame buffer
		if ( NULL != m_PtrFb )
		{
			if ( (0 != A_Size) && (E_GrElmDevNetraFbSize >= (A_Offset + A_Size)) )
			{
				Tv_Result	=	(void*)( (DWORD)m_PtrFb + A_Offset );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int GrEmuDevNetra_ioctl (struct inode * A_PtrInode, struct file * A_PtrFile, DWORD A_Para1, DWORD A_Para2)
{
	// local -------------------
	// code --------------------
		return	0;
}
//--------------------------------------------------------------------
int GrEmuDevNetra_mmap (struct file * A_PtrFile, struct vm_area_struct * A_PtrVm)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	-1;

		if ( NULL != V_GrElmDevNetraObj[A_PtrFile->Handle] )
		{
			A_PtrVm->MapAdr	=	V_GrElmDevNetraObj[A_PtrFile->Handle]->Mmap( 
				A_PtrVm->vm_pgoff, A_PtrVm->vm_end );
			if ( NULL != A_PtrVm->MapAdr )
			{
				Tv_Result	=	0;
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
int GrEmuDevNetra_open (struct inode * A_PtrInode, struct file * A_PtrFile)
{
	// local -------------------
	// code --------------------

		return	0;
}
//--------------------------------------------------------------------
int GrEmuDevNetra_release (struct inode * A_PtrInode, struct file * A_PtrFile)
{
	// local -------------------
	// code --------------------
		return	0;
}
//--------------------------------------------------------------------
extern	"C" 
{
	Int32 Vsys_create(unsigned char A_Is16ChBd)
	{
		return	ERROR_NONE;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	Int32 Vsys_init  ( VSYS_PARAMS_S *  pContext  ) 
	{
		return	ERROR_NONE;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	Int32 Vsys_params_init  ( VSYS_PARAMS_S *  pContext  )  
	{
		return	ERROR_NONE;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	Int32 Vdec_init  ( VDEC_PARAMS_S *  pContext  )   
	{
		return	ERROR_NONE;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	Void Vdec_params_init  ( VDEC_PARAMS_S *  pContext  ) 
	{

	}
};
//--------------------------------------------------------------------
extern	"C"
{
	Int32 Venc_init  ( VENC_PARAMS_S *  pContext  )   
	{
		return	ERROR_NONE;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	Void Venc_params_init  ( VENC_PARAMS_S *  pContext  )   
	{

	}
};
//--------------------------------------------------------------------
extern	"C"
{
	Int32 Vcap_init  ( VCAP_PARAMS_S *  pContext , Bool A_IsPalMode )   
	{
		return	ERROR_NONE;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	Void Vcap_params_init  ( VCAP_PARAMS_S *  pContext  )  
	{

	}
};
//--------------------------------------------------------------------
extern	"C"
{
	Int32  Vdis_setMosaicParams (VDIS_DEV vdDevId, VDIS_MOSAIC_S *psVdMosaicParam) 
	{
		return	ERROR_NONE;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	Int32 Vcap_start  ( void ) 
	{
		return	ERROR_NONE;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	Int32 Venc_start  ( void )
	{
		return	ERROR_NONE;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	Int32 Vdec_start  ( void )
	{
		return	ERROR_NONE;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	Int32  Vdis_start () 
	{
		return	ERROR_NONE;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	Int32 Vsys_exit  ( void )   
	{
		return	ERROR_NONE;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	Void  Vdis_exit () 
	{

	}
};
//--------------------------------------------------------------------
extern	"C"
{
	Void Vdec_exit  ( void )   
	{

	}
};
//--------------------------------------------------------------------
extern	"C"
{
	Void Venc_exit  ( void )   
	{

	}
};
//--------------------------------------------------------------------
extern	"C"
{
	Void Vcap_exit  ( void )   
	{

	}
};
//--------------------------------------------------------------------
extern	"C"
{
	Int32 Vsys_delete  ( void )   
	{
		return	ERROR_NONE;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	Int32  Vdis_stop ( void )
	{
		return	ERROR_NONE;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	Int32 Vdec_stop  (void)   
	{
		return	ERROR_NONE;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	Int32 Venc_stop  (void)   
	{
		return	ERROR_NONE;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	Int32 Vcap_stop  (void)   
	{
		return	ERROR_NONE;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	Int32  Vdis_init (VDIS_PARAMS_S *pContext) 
	{
		return	ERROR_NONE;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	Void  Vdis_params_init (VDIS_PARAMS_S *pContext) 
	{

	}
};
//--------------------------------------------------------------------
extern	"C"
{
	Int32 Venc_getBitstreamBuffer(VCODEC_BITSBUF_LIST_S *pBitsBufList, UInt32 timeout)
	{
		// local -------------------
			Int32	Tv_Result;
		// code --------------------
			Tv_Result	=	ERROR_FAIL;
			if ( NULL != V_GrElmDevNetraObj[0] )
			{
				if ( V_GrElmDevNetraObj[0]->StreamGet( pBitsBufList ) )
				{
					Tv_Result	=	ERROR_NONE;
				}
			}
			
			return	Tv_Result;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	Int32 Venc_releaseBitstreamBuffer(VCODEC_BITSBUF_LIST_S *pBitsBufList)
	{
		// local -------------------
		// code --------------------
		return	ERROR_NONE;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	Int32 Vdec_requestBitstreamBuffer(VDEC_BUF_REQUEST_S *bufReq, VCODEC_BITSBUF_LIST_S *pBitsBufList, UInt32 timeout)
	{
		// local -------------------
			int		Tv_Result;
			DWORD	Tv_WkIdx;
		// code --------------------
			Tv_Result	=	ERROR_FAIL;
			pBitsBufList->numBufs	=	0;
			if ( 0 != bufReq->numBufs )
			{
				for( Tv_WkIdx=0;Tv_WkIdx < bufReq->numBufs;Tv_WkIdx++ )
				{
					pBitsBufList->bitsBuf[Tv_WkIdx].bufVirtAddr	=	malloc( (size_t)bufReq->u[Tv_WkIdx].minBufSize );
					if ( NULL != pBitsBufList->bitsBuf[Tv_WkIdx].bufVirtAddr )
					{
						pBitsBufList->bitsBuf[Tv_WkIdx].bufSize	=	bufReq->u[Tv_WkIdx].minBufSize;
						pBitsBufList->numBufs ++;
						Tv_Result	=	ERROR_NONE;
					}
					else
					{
						break;
					}
				}
			}

			return	Tv_Result;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	Int32 Vdec_putBitstreamBuffer(VCODEC_BITSBUF_LIST_S *pBitsBufList)
	{
		// local -------------------
			int		Tv_Result;
			DWORD	Tv_WkIdx;
			DWORD*	Tv_PtrHd;
		// code --------------------
			Tv_Result	=	ERROR_FAIL;

			if ( 0 != pBitsBufList->numBufs )
			{
				Tv_Result	=	ERROR_NONE;
				for( Tv_WkIdx=0;Tv_WkIdx < pBitsBufList->numBufs;Tv_WkIdx++ )
				{
					Tv_PtrHd	=	(DWORD*)pBitsBufList->bitsBuf[Tv_WkIdx].bufVirtAddr;
					if ( 0x01000000 != *Tv_PtrHd )
					{
						DbgMsgPrint( "Vdec_putBitstreamBuffer - bad stream detected!\n" );
						Tv_Result	=	ERROR_FAIL;
					}
					// release buffer
					free( pBitsBufList->bitsBuf[Tv_WkIdx].bufVirtAddr );
				}
			}

			return	Tv_Result;
	}
};
//--------------------------------------------------------------------
void	Cls_GrElmDevNetra::LcEmuAvLoad( void )
{
	// local -------------------
		DWORD		Tv_VdoSize;
		Cls_GrFileCtrl*	Tv_Fc;
		Ptr_GrDvrAvHd		Tv_PtrFrm;
		DWORD		Tv_LoadSize;
		DWORD		Tv_Fptr;
		BOOL		Tv_IsStart;
		void*		Tv_PtrData;
	// code --------------------
		//init
		Tv_VdoSize		=	0;
		m_EmuVdoCnt		=	0;
		Tv_IsStart		=	FALSE;

		Tv_Fc	=	(Cls_GrFileCtrl*)new Cls_GrFileCtrl( L"e:\\EmuD1\\avdata.dat", FALSE, FALSE, FALSE, TRUE );
		if ( Tv_Fc->IsOpened() )
		{
			//go first frame
			Tv_Fc->Seek( 0x300 );

			while ( TRUE )
			{
				//check data finished
				if ( (Tv_VdoSize + sizeof(St_GrDvrAvHd)) >= E_GrElmDevNetraAvBufSize )
				{
					//finished
					break;
				}
				//load frame
				Tv_PtrFrm		=	(Ptr_GrDvrAvHd)( (DWORD)m_EmuVdoBuf + Tv_VdoSize );
				Tv_LoadSize	=	Tv_Fc->Read( Tv_PtrFrm, sizeof(St_GrDvrAvHd) );
				if ( Tv_LoadSize != sizeof(St_GrDvrAvHd) )
				{
					//data ended
					break;
				}
				//check valid frame
				if ( 0x7641444E == Tv_PtrFrm->Fcc )
				{
					//check video frame and channel
					if (	(1 == Tv_PtrFrm->Ch) && ((E_GrDvrAvTypVdoP >= Tv_PtrFrm->Type) || (E_GrDvrAvTypVdoB == Tv_PtrFrm->Type)) && 
						( Tv_IsStart || ( E_GrDvrAvTypVdoI == Tv_PtrFrm->Type ) )
						)
					{
						//copy contents
						Tv_PtrData	=	(void*)( (DWORD)Tv_PtrFrm + sizeof(St_GrDvrAvHd) );
						Tv_LoadSize	=	(Tv_PtrFrm->DataSize + 0x1F) & (~0x1F);
						if ( (Tv_VdoSize + sizeof(St_GrDvrAvHd) + Tv_LoadSize ) >= E_GrElmDevNetraAvBufSize )
						{
							//finished
							break;
						}
						if ( Tv_LoadSize != Tv_Fc->Read( Tv_PtrData, Tv_LoadSize ) )
						{
							//data finished
							break;
						}
						Tv_IsStart	=	TRUE;
						m_EmuVdoCnt ++;
						Tv_VdoSize	=	Tv_VdoSize + Tv_LoadSize + sizeof(St_GrDvrAvHd);
					}
					else
					{
						//skip contents
						Tv_Fptr	=	(DWORD)Tv_Fc->GetPos() + Tv_PtrFrm->DataSize;
						Tv_Fptr	=	(Tv_Fptr + 0x1F) & (~0x1F);
						if ( Tv_Fptr != (DWORD)Tv_Fc->Seek( Tv_Fptr ) )
						{
							//data end, finished
							break;
						}
					}
				}
				else
				{
					//bad stream
					break;
				}

				//next

			}

		}
		else
		{
			DbgMsgPrint( "Cls_GrElmDevNetra::LcEmuAvLoad - emulation av data open fail.\n" );
		}
		delete	Tv_Fc;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrElmDevNetra::LcVencStrmIn( BYTE A_Ch, VCODEC_BITSBUF_S* A_PtrItm )
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrDvrAvHd	Tv_PtrHd;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( m_VencTbl[A_Ch].IsOn )
		{
			// init
			Tv_PtrHd	=	(Ptr_GrDvrAvHd)( (DWORD)m_EmuVdoBuf + m_VencTbl[A_Ch].Lpos );
			// check video data
			if ( (E_GrDvrAvTypVdoP >= Tv_PtrHd->Type) || (E_GrDvrAvTypVdoB == Tv_PtrHd->Type) )
			{
				// copy
				A_PtrItm->chnId			=	A_Ch;
				A_PtrItm->strmId		=	0;
				A_PtrItm->codecType	=	VCODEC_TYPE_H264;
				A_PtrItm->frameType	=	(VCODEC_FRAME_TYPE_E)Tv_PtrHd->Type;
				A_PtrItm->bufSize		=	Tv_PtrHd->DataSize;
				A_PtrItm->filledBufSize	=	Tv_PtrHd->DataSize;
				A_PtrItm->bufVirtAddr	=	(void*)( (DWORD)Tv_PtrHd + sizeof(St_GrDvrAvHd) );
				A_PtrItm->timestamp	=	GrTimeGetTick();

				switch ( Tv_PtrHd->Res )
				{
					case E_GrDvrVdoLowRes:
						A_PtrItm->frameWidth	=	352;
						A_PtrItm->frameHeight	=	240;
						break;
					case E_GrDvrVdoMidRes:
						A_PtrItm->frameWidth	=	704;
						A_PtrItm->frameHeight	=	240;
						break;
					default:
						A_PtrItm->frameWidth	=	704;
						A_PtrItm->frameHeight	=	480;
						break;
				}
				
				Tv_Result	=	TRUE;
			}
			// next
			m_VencTbl[A_Ch].Lpos	=	m_VencTbl[A_Ch].Lpos + sizeof(St_GrDvrAvHd) + ((Tv_PtrHd->DataSize + 0x1F) & (~0x1F));
			m_VencTbl[A_Ch].Prgs ++;
			if ( m_VencTbl[A_Ch].Prgs >= m_EmuVdoCnt )
			{
				m_VencTbl[A_Ch].Prgs	=	0;
				m_VencTbl[A_Ch].Lpos	=	0;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrElmDevNetra::StreamGet( VCODEC_BITSBUF_LIST_S* A_PtrBlst )
{
	// local -------------------
		BOOL8	Tv_Result;
		BYTE	Tv_WkIdx;
		BYTE	Tv_InCnt;
	// code --------------------
		Tv_Result	=	FALSE;
		Tv_InCnt	=	0;
		// check able
		if ( 0 != m_EmuVdoCnt )
		{
			if ( 0 == m_EmuVdoWait )
			{
				// get stream
				for ( Tv_WkIdx=0;Tv_WkIdx < E_GrElmDevNetraVdoGetCnt;Tv_WkIdx++ )
				{
					// get list
					if ( LcVencStrmIn( m_VencCh, &A_PtrBlst->bitsBuf[Tv_InCnt] ) )
					{
						Tv_InCnt ++;
						Tv_Result	=	TRUE;
					}
					// next
					m_VencCh ++;
					m_VencCh	=	m_VencCh & 0x0F;
				}
				// reset wait
				m_EmuVdoWait	=	E_GrElmDevNetraVdoGetWait;
			}
			// next
			m_EmuVdoWait --;
		}

		// update result
		A_PtrBlst->numBufs	=	Tv_InCnt;

		return	Tv_Result;
}
//--------------------------------------------------------------------
extern	"C"
{
	Int32 Venc_setDynamicParam(VENC_CHN vencChnId, VENC_STRM vencStrmID, VENC_CHN_DYNAMIC_PARAM_S *ptEncDynamicParam, VENC_PARAM_E veParamId)
	{
		return	ERROR_NONE;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	Int32 Vcap_setFrameRate(VCAP_CHN vcChnId, VCAP_STRM vStrmId, Int32 inputFrameRate, Int32 outputFrameRate)
	{
		return	ERROR_NONE;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	Int32 Venc_setInputFrameRate(VENC_CHN vencChnId, Int32 veFrameRate)
	{
		return	ERROR_NONE;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	Int32 Vcap_getVideoSourceStatus(VCAP_VIDEO_SOURCE_STATUS_S *pStatus)
	{
		return	ERROR_NONE;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	Int32 Vsys_registerEventHandler(VSYS_EVENT_HANDLER_CALLBACK callback, Ptr appData)
	{
		return	ERROR_NONE;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	Int32 Vdis_enableChn(VDIS_DEV vdDevId, VDIS_CHN vdChId)
	{
		return	ERROR_NONE;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	Int32 Vcap_enableChn(VCAP_CHN vcChnId, VCAP_STRM vcStrmId)
	{
		return	ERROR_NONE;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	Int32 Vcap_setDynamicParamChn(VCAP_CHN vcChnId, VCAP_CHN_DYNAMIC_PARAM_S *psCapChnDynaParam, VCAP_PARAMS_E paramId)
	{
		return	ERROR_NONE;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	Int32 Vcap_getNumChannels()
	{
		return	ERROR_NONE;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	Int32 Vcap_getBitstreamBuffer(VCODEC_BITSBUF_LIST_S *pBitsBufList, UInt32 timeout)
	{
		return	ERROR_NONE;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	Int32 Vcap_releaseBitstreamBuffer(VCODEC_BITSBUF_LIST_S *pBitsBufList)
	{
		return	ERROR_NONE;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	Int32 Vcap_registerBitsCallback(VCAP_CALLBACK_S * callback, Ptr arg)
	{
		return	ERROR_NONE;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	Int32 Venc_registerCallback(VENC_CALLBACK_S * callback, Ptr arg)
	{
		V_GrElmDevNetraCbEnc	=	callback->newDataAvailableCb;
		return	ERROR_NONE;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	Int32 Vdis_disableChn(VDIS_DEV vdDevId, VDIS_CHN vdChId)
	{
		return	ERROR_NONE;
	}
};
//--------------------------------------------------------------------
extern	"C"
{
	Int32 Vsys_printBufferStatistics()
	{
		return	ERROR_NONE;
	}
}
//--------------------------------------------------------------------
extern	"C"
{
	int Vcap_SetBright(unsigned char A_Ch, unsigned char A_Bright)
	{
		return	ERROR_NONE;
	}
}
//--------------------------------------------------------------------
extern	"C"
{
	int Vcap_SetContrast(unsigned char A_Ch, unsigned char A_Bright)
	{
		return	ERROR_NONE;
	}
}
//--------------------------------------------------------------------
extern	"C"
{
	int Vcap_SetHue(unsigned char A_Ch, unsigned char A_Hue)
	{
		return	ERROR_NONE;
	}
}
//--------------------------------------------------------------------
extern	"C"
{
	int Vcap_SetGain(unsigned char A_Ch, unsigned char A_Hue)
	{
		return	ERROR_NONE;
	}
}
//--------------------------------------------------------------------
extern	"C"
{
	int Vcap_SetAutoGain(unsigned char A_Ch, unsigned char A_Hue)
	{
		return	ERROR_NONE;
	}
}
//--------------------------------------------------------------------
extern	"C"
{
	unsigned short Vcap_GetVdoSig()
	{
		return	0xFFFF;
	}
}
//--------------------------------------------------------------------
extern	"C"
{
	Bool Vcap_isPalMode()
	{
		return	FALSE;
	}
}
//--------------------------------------------------------------------
extern	"C"
{
	int Vcap_MtnInit()
	{
		return	ERROR_NONE;
	}
}
//--------------------------------------------------------------------
extern	"C"
{
	int Vcap_MtnBlkVisi(unsigned char A_Ch, unsigned char A_IsVisi)
	{
		return	ERROR_NONE;
	}
}
//--------------------------------------------------------------------
extern	"C"
{
	int Vcap_MtnSetBlk(unsigned char A_Ch, unsigned char* A_PtrBlk)
	{
		return	ERROR_NONE;
	}
}
//--------------------------------------------------------------------
extern	"C"
{
	unsigned short Vcap_MtnGetEvtCh()
	{
		return	ERROR_NONE;
	}
}
//--------------------------------------------------------------------
extern	"C"
{
	int Vcap_MtnGetEvtBlk(unsigned char A_Ch, unsigned char* A_PtrBlk)
	{
		return	ERROR_NONE;
	}
}
//--------------------------------------------------------------------
extern	"C"
{
	int Vcap_MtnCfgSet(unsigned char A_Ch, unsigned char A_Spd, unsigned char A_LvSen, unsigned char A_SpSen, unsigned char A_TmpSen)
	{
		return	ERROR_NONE;
	}
}
//--------------------------------------------------------------------
extern	"C"
{
	int Vcap_AdoChSel(unsigned char A_Ch)
	{
		return	ERROR_NONE;
	}
}
//--------------------------------------------------------------------
extern	"C"
{
	int Vcap_AdoVolume(unsigned char A_Value)
	{
		return	ERROR_NONE;
	}
}
//--------------------------------------------------------------------
extern	"C"
{
	int Vcap_BlkageSet(unsigned char A_Ch, unsigned char* A_PtrBlk)
	{
		return	ERROR_NONE;
	}
}
//--------------------------------------------------------------------
extern	"C"
{
	int Vcap_BlkageVisi(unsigned char A_IsVisi)
	{
		return	ERROR_NONE;
	}
}
//--------------------------------------------------------------------
extern	"C"
{
	int Vdis_flushSwMs(VDIS_DEV vdDevId,VDIS_CHN vdispChnId,UInt32 holdLastFrame)
	{
		return	ERROR_NONE;
	}
}
//--------------------------------------------------------------------
