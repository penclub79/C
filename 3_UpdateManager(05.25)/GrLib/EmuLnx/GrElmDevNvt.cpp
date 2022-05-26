/*
	emulation linux HiMPI device

*/

//====================================================================
//include

#include	<EmuLnx/GrElmDevNvt.h>
#include	<GrDumyTool.h>
#include	<GrBufTool.h>
#include	<GrGdibTool.h>
#include	<GrFileCtrl.h>
#include	<Dvr/GrDvrBase.h>


//====================================================================
//local const
#define E_GrElmDevNvtMaxObjCnt		2

//====================================================================
//local type


//====================================================================
//global var

DWORD	V_GrEmlDevHi3531CompCnt	=	0;	

Cls_GrElmDevNvt*	V_GrElmDevNvtObj[E_GrElmDevNvtMaxObjCnt]	=	
{	NULL, NULL };
file_operations			V_GrElmDevNvtFo;

int		V_GrEmlDevNvtFbNodeTbl[E_GrElmDevNvtFbCnt]	=	
{ -1, -1, -1, -1, -1, -1, -1 };

//====================================================================
//functions
//DWORD WINAPI GrElmDevSoloProcThrd( LPVOID A_Para );

int GrEmuDevNvt_ioctl (struct inode * A_PtrInode, struct file * A_PtrFile, DWORD A_Para1, DWORD A_Para2);
int GrEmuDevNvt_mmap (struct file * A_PtrFile, struct vm_area_struct * A_PtrVm);
int GrEmuDevNvt_open (struct inode * A_PtrInode, struct file * A_PtrFile);
int GrEmuDevNvt_release (struct inode * A_PtrInode, struct file * A_PtrFile);

//--------------------------------------------------------------------
Cls_GrElmDevNvt::Cls_GrElmDevNvt( DWORD A_DevIdx, BYTE A_FbIdx ):
Cls_GrElmDevBase( A_DevIdx )
{
	// local -------------------
		cdev	Tv_Cdev;
		BYTE	Tv_WkIdx;
	// code --------------------
		//init
		m_FbIdx	=	A_FbIdx;
		
		V_GrElmDevNvtFo.open			=	GrEmuDevNvt_open;
		V_GrElmDevNvtFo.release		=	GrEmuDevNvt_release;
		V_GrElmDevNvtFo.ioctl			=	GrEmuDevNvt_ioctl;
		V_GrElmDevNvtFo.mmap			=	GrEmuDevNvt_mmap;

		cdev_init( &Tv_Cdev, &V_GrElmDevNvtFo );
		cdev_add( &Tv_Cdev, m_Node, 1 );		// register ioctl

		m_PtrFb	=	malloc( E_GrElmDevNvtFbSize );

		V_GrElmDevNvtObj[m_FbIdx]	=	this;

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
Cls_GrElmDevNvt::~Cls_GrElmDevNvt()
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

		V_GrElmDevNvtObj[m_FbIdx]	=	NULL;
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
void*	Cls_GrElmDevNvt::Mmap( DWORD A_Offset, DWORD A_Size )
{
	// local -------------------
		void*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;

		// check allocated frame buffer
		if ( NULL != m_PtrFb )
		{
			if ( (0 != A_Size) && (E_GrElmDevNvtFbSize >= (A_Offset + A_Size)) )
			{
				Tv_Result	=	(void*)( (DWORD)m_PtrFb + A_Offset );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int GrEmuDevNvt_ioctl (struct inode * A_PtrInode, struct file * A_PtrFile, DWORD A_Para1, DWORD A_Para2)
{
	// local -------------------
		fb_var_screeninfo*	Tv_PtrVsInfo;
		fb_fix_screeninfo*	Tv_PtrFsInfo;
	// code --------------------
		switch(A_Para1)
		{
		case FBIOGET_VSCREENINFO:
			Tv_PtrVsInfo		= (fb_var_screeninfo*)A_Para2;
			Tv_PtrVsInfo->xres	= 1920;
			Tv_PtrVsInfo->yres	= 1080;
			break;
		case FBIOGET_FSCREENINFO:
			Tv_PtrFsInfo		= (fb_fix_screeninfo*)A_Para2;
			Tv_PtrFsInfo->line_length	= 1920 * GrDrawGetPixelByteByFcc(E_GrFccARGB1555);
			break;
		}
		return	0;
}
//--------------------------------------------------------------------
int GrEmuDevNvt_mmap (struct file * A_PtrFile, struct vm_area_struct * A_PtrVm)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	-1;

		if ( NULL != V_GrElmDevNvtObj[A_PtrFile->Handle] )
		{
			A_PtrVm->MapAdr	=	V_GrElmDevNvtObj[A_PtrFile->Handle]->Mmap( 
				A_PtrVm->vm_pgoff, A_PtrVm->vm_end );
			if ( NULL != A_PtrVm->MapAdr )
			{
				Tv_Result	=	0;
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
int GrEmuDevNvt_open (struct inode * A_PtrInode, struct file * A_PtrFile)
{
	// local -------------------
	// code --------------------

		return	0;
}
//--------------------------------------------------------------------
int GrEmuDevNvt_release (struct inode * A_PtrInode, struct file * A_PtrFile)
{
	// local -------------------
	// code --------------------
		return	0;
}
//--------------------------------------------------------------------
void	Cls_GrElmDevNvt::LcEmuAvLoad( void )
{
	// local -------------------
		DWORD		Tv_VdoSize;
		Cls_GrFileCtrl*	Tv_Fc;
		Ptr_GrElmDevHiMpiFrmHd	Tv_PtrFrm;
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
			Tv_Fc->Seek( 0x200000 );

			while ( TRUE )
			{
				//check data finished
				if ( (Tv_VdoSize + sizeof(St_GrDvrFrmHdVdo)) >= E_GrElmDevHiMpiAvBufSize )
				{
					//finished
					break;
				}
				//load frame
				Tv_PtrFrm		=	(Ptr_GrElmDevHiMpiFrmHd)((DWORD)m_EmuVdoBuf + Tv_VdoSize);
				Tv_LoadSize	=	Tv_Fc->Read(Tv_PtrFrm, sizeof(St_GrElmDevHiMpiFrmHd));
				if (Tv_LoadSize != sizeof(St_GrElmDevHiMpiFrmHd))
				{
					//data ended
					break;
				}
				//check valid frame
				if ( 0x5641444E == Tv_PtrFrm->Fcc )
				{
					//check video frame and channel
					if (	(0 == Tv_PtrFrm->Ch) && (1 >= Tv_PtrFrm->Type) && 
						( Tv_IsStart || ( 0 == Tv_PtrFrm->Type ) )
						)
					{
						//copy contents
						Tv_PtrData	=	(void*)((DWORD)Tv_PtrFrm + sizeof(St_GrElmDevHiMpiFrmHd));
						Tv_LoadSize	=	(Tv_PtrFrm->DataSize + 0x1F) & (~0x1F);
						if ((Tv_VdoSize + sizeof(St_GrElmDevHiMpiFrmHd)+ Tv_LoadSize) >= E_GrElmDevHiMpiAvBufSize)
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
						Tv_VdoSize	=	Tv_VdoSize + Tv_LoadSize + sizeof(St_GrElmDevHiMpiFrmHd);
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
			DbgMsgPrint( "Cls_GrElmDevHiMpi::LcEmuAvLoad - emulation av data open fail.\n" );
		}
		delete	Tv_Fc;
}
//--------------------------------------------------------------------
extern	"C"
{
	HD_RESULT hd_common_init(UINT32 sys_config_type)
	{
		// local -------------------
			HD_RESULT	Tv_Result;
		// code --------------------
			Tv_Result	=	HD_ERR_NOT_SUPPORT;

			// init FO table
			V_GrElmDevNvtFo.open			=	GrEmuDevNvt_open;
			V_GrElmDevNvtFo.release		=	GrEmuDevNvt_release;
			V_GrElmDevNvtFo.ioctl			=	GrEmuDevNvt_ioctl;
			V_GrElmDevNvtFo.mmap			=	GrEmuDevNvt_mmap;

			// register driver 1
			V_GrEmlDevNvtFbNodeTbl[1]	=	GrElaDrvAdd( "fb1", &V_GrElmDevNvtFo );
			if ( 0 <= V_GrEmlDevNvtFbNodeTbl[1] )
			{
				Tv_Result	=	HD_OK;
			}

			V_GrEmlDevNvtFbNodeTbl[2]	=	GrElaDrvAdd( "fb1", &V_GrElmDevNvtFo );
			if ( 0 <= V_GrEmlDevNvtFbNodeTbl[2] )
			{
				Tv_Result	=	HD_OK;
			}

			V_GrEmlDevNvtFbNodeTbl[3]	=	GrElaDrvAdd( "fb2", &V_GrElmDevNvtFo );
			if ( 0 <= V_GrEmlDevNvtFbNodeTbl[3] )
			{
				Tv_Result	=	HD_OK;
			}

			return	Tv_Result;
	}

	HD_RESULT hd_common_sysconfig(UINT32 sys_config_type1, UINT32 sys_config_type2, UINT32 type1_mask, UINT32 type2_mask)
	{
		return HD_OK;
	}

	HD_RESULT hd_common_get_sysconfig(UINT32 *p_sys_config_type1, UINT32 *p_sys_config_type2)
	{
		*p_sys_config_type1	= 0;
		*p_sys_config_type2	= 1;

		return HD_OK;
	}

	HD_RESULT hd_common_uninit(void)
	{
		return HD_OK;
	}

	HD_RESULT hd_common_mem_init(HD_COMMON_MEM_INIT_CONFIG *p_mem_config)
	{
		return HD_OK;
	}

	UINT32 hd_common_mem_calc_buf_size(void *p_hd_data)
	{
		return E_GrElmDevHiMpiAvBufSize;
	}

	void *hd_common_mem_mmap(HD_COMMON_MEM_MEM_TYPE mem_type, UINT32 phy_addr, UINT32 size)
	{
		return NULL;
	}

	HD_RESULT hd_common_mem_munmap(void *virt_addr, unsigned int size)
	{
		return HD_OK;
	}

	HD_RESULT hd_common_mem_flush_cache(void *virt_addr, unsigned int size)
	{
		return HD_OK;
	}

	HD_COMMON_MEM_VB_BLK hd_common_mem_get_block(HD_COMMON_MEM_POOL_TYPE pool_type, UINT32 blk_size, HD_COMMON_MEM_DDR_ID ddr)
	{
		return 8;
	}

	HD_RESULT hd_common_mem_release_block(HD_COMMON_MEM_VB_BLK blk)
	{
		return HD_OK;
	}

	UINT32 hd_common_mem_blk2pa(HD_COMMON_MEM_VB_BLK blk)
	{
		return 0x1000;
	}

	HD_RESULT hd_common_mem_alloc(CHAR *name, UINT32 *phy_addr, void **virt_addr, UINT32 size, HD_COMMON_MEM_DDR_ID ddr)
	{
		return HD_OK;
	}

	HD_RESULT hd_common_mem_free(UINT32 phy_addr, void *virt_addr)
	{
		return HD_OK;
	}

	HD_RESULT hd_common_mem_get(HD_COMMON_MEM_PARAM_ID id, VOID *p_param)
	{
		return HD_OK;
	}

	HD_RESULT hd_common_dmacopy(HD_COMMON_MEM_DDR_ID src_ddr, UINT32 src_phy_addr,
								HD_COMMON_MEM_DDR_ID dst_ddr, UINT32 dst_phy_addr, UINT32 len)
	{
		return HD_OK;
	}

	HD_RESULT hd_common_mem_uninit(void)
	{
		return HD_OK;
	}

	HD_RESULT hd_common_pcie_init(void)
	{
		return HD_OK;
	}

	HD_RESULT hd_common_pcie_open(char *name, HD_COMMON_PCIE_CHIP chip_id, int chan_id, int mode)
	{
		return HD_OK;
	}

	HD_RESULT hd_common_pcie_close(HD_COMMON_PCIE_CHIP chip_id, int chan_id)
	{
		return HD_OK;
	}

	INT hd_common_pcie_get_chip_num(VOID)
	{
		return 0;
	}

	HD_RESULT hd_common_pcie_get(HD_COMMON_PCIE_CHIP chip_id, int chan_id, HD_COMMON_PCIE_PARAM_ID id, void *parm)
	{
		return HD_OK;
	}

	HD_RESULT hd_common_pcie_set(HD_COMMON_PCIE_CHIP chip_id, int chan_id, HD_COMMON_PCIE_PARAM_ID id, void *parm)
	{
		return HD_OK;
	}

	int hd_common_pcie_recv(HD_COMMON_PCIE_CHIP chip_id, int chan_id, unsigned char *pbuf, unsigned int len)
	{
		return len;
	}

	int hd_common_pcie_send(HD_COMMON_PCIE_CHIP chip_id, int chan_id, unsigned char *pbuf, unsigned int len)
	{
		return len;
	}

	HD_RESULT hd_common_pcie_uninit(void)
	{
		return HD_OK;
	}
};

extern	"C" //hd_videocap
{
	HD_RESULT hd_videocap_init()
	{
		return HD_OK;
	}

	HD_RESULT hd_videocap_bind(__u32 out_id, __u32 dest_in_id)
	{
		return HD_OK;
	}

	HD_RESULT hd_videocap_unbind(__u32 out_id)
	{
		return HD_OK;
	}

	HD_RESULT hd_videocap_open(__u32 in_id, __u32 out_id, __u32 *p_path_id)
	{
		return HD_OK;
	}

	HD_RESULT hd_videocap_start(__u32 path_id)
	{
		return HD_OK;
	}

	HD_RESULT hd_videocap_stop(__u32 path_id)
	{
		return HD_OK;
	}

	HD_RESULT hd_videocap_start_list(__u32 *path_id, UINT num)
	{
		return HD_OK;
	}

	HD_RESULT hd_videocap_stop_list(__u32 *path_id, UINT num)
	{
		return HD_OK;
	}

	HD_RESULT hd_videocap_get(__u32 path_id, HD_VIDEOCAP_PARAM_ID id, VOID *p_param)
	{
		return HD_OK;
	}

	HD_RESULT hd_videocap_set(__u32 path_id, HD_VIDEOCAP_PARAM_ID id, VOID *p_param)
	{
		return HD_OK;
	}

	HD_RESULT hd_videocap_drv_get(HD_VIDEOCAP_DRV_PARAM_ID id, VOID *p_param)
	{
		return HD_OK;
	}

	HD_RESULT hd_videocap_drv_set(HD_VIDEOCAP_DRV_PARAM_ID id, VOID *p_param)
	{
		return HD_OK;
	}
	
	HD_RESULT hd_videocap_push_in_buf(__u32 path_id, HD_VIDEO_FRAME *p_video_frame, INT32 wait_ms)
	{
		return HD_OK;
	}

	HD_RESULT hd_videocap_pull_out_buf(__u32 path_id, HD_VIDEO_FRAME *p_video_frame, INT32 wait_ms)
	{
		return HD_OK;
	}

	HD_RESULT hd_videocap_release_out_buf(__u32 path_id, HD_VIDEO_FRAME *p_video_frame)
	{
		return HD_OK;
	}

	HD_RESULT hd_videocap_close(__u32 path_id)
	{
		return HD_OK;
	}

	HD_RESULT hd_videocap_uninit(VOID)
	{
		return HD_OK;
	}
};

extern	"C" //hd_videoenc
{
	HD_RESULT hd_videoenc_init(VOID)
	{
		return HD_OK;
	}

	HD_RESULT hd_videoenc_bind(__u32 out_id, __u32 dest_in_id)
	{
		return HD_OK;
	}

	HD_RESULT hd_videoenc_unbind(__u32 out_id)
	{
		return HD_OK;
	}

	HD_RESULT hd_videoenc_open(__u32 in_id, __u32 out_id, __u32 *p_path_id)
	{
		return HD_OK;
	}

	HD_RESULT hd_videoenc_start(__u32 path_id)
	{
		return HD_OK;
	}

	HD_RESULT hd_videoenc_stop(__u32 path_id)
	{
		return HD_OK;
	}

	HD_RESULT hd_videoenc_start_list(__u32 *path_id, UINT num)
	{
		return HD_OK;
	}

	HD_RESULT hd_videoenc_stop_list(__u32 *path_id, UINT num)
	{
		return HD_OK;
	}

	HD_RESULT hd_videoenc_close(__u32 path_id)
	{
		return HD_OK;
	}

	HD_RESULT hd_videoenc_get(__u32 path_id, HD_VIDEOENC_PARAM_ID id, VOID *p_param)
	{
		return HD_OK;
	}

	HD_RESULT hd_videoenc_set(__u32 path_id, HD_VIDEOENC_PARAM_ID id, VOID *p_param)
	{
		return HD_OK;
	}

	HD_RESULT hd_videoenc_push_in_buf(__u32 path_id, HD_VIDEO_FRAME *p_in_video_frame, HD_VIDEOENC_BS *p_user_out_videoenc_bs, INT32 wait_ms)
	{
		return HD_OK;
	}

	HD_RESULT hd_videoenc_pull_out_buf(__u32 path_id, HD_VIDEOENC_BS *p_videoenc_bs, INT32 wait_ms)
	{
		return HD_OK;
	}

	HD_RESULT hd_videoenc_release_out_buf(HD_PATH_ID path_id, HD_VIDEOENC_BS *p_videoenc_bs)
	{
		return HD_OK;
	}

	HD_RESULT hd_videoenc_poll_list(HD_VIDEOENC_POLL_LIST *p_poll, UINT32 num, INT32 wait_ms)
	{
		return HD_OK;
	}

	HD_RESULT hd_videoenc_recv_list(HD_VIDEOENC_RECV_LIST *p_videoenc_list, UINT32 num)
	{
		return HD_OK;
	}

	HD_RESULT hd_videoenc_uninit(VOID)
	{
		return HD_OK;
	}
};

extern	"C" //hd_videoenc
{
	HD_RESULT hd_videoproc_init(VOID)
	{
		return HD_OK;
	}

	HD_RESULT hd_videoproc_bind(__u32 out_id, __u32 dest_in_id)
	{
		return HD_OK;
	}

	HD_RESULT hd_videoproc_unbind(__u32 out_id)
	{
		return HD_OK;
	}

	HD_RESULT hd_videoproc_open(__u32 in_id, __u32 out_id, __u32 *p_path_id)
	{
		return HD_OK;
	}

	HD_RESULT hd_videoproc_start(__u32 path_id)
	{
		return HD_OK;
	}

	HD_RESULT hd_videoproc_stop(__u32 path_id)
	{
		return HD_OK;
	}

	HD_RESULT hd_videoproc_start_list(__u32 *path_id, UINT num)
	{
		return HD_OK;
	}

	HD_RESULT hd_videoproc_stop_list(__u32 *path_id, UINT num)
	{
		return HD_OK;
	}

	HD_RESULT hd_videoproc_get(__u32 path_id, HD_VIDEOPROC_PARAM_ID id, VOID *p_param)
	{
		return HD_OK;
	}

	HD_RESULT hd_videoproc_set(__u32 path_id, HD_VIDEOPROC_PARAM_ID id, VOID *p_param)
	{
		return HD_OK;
	}

	HD_RESULT hd_videoproc_push_in_buf(__u32 path_id, HD_VIDEO_FRAME *p_in_video_frame, HD_VIDEO_FRAME *p_user_out_video_frame, INT32 wait_ms)
	{
		return HD_OK;
	}

	HD_RESULT hd_videoproc_pull_out_buf(__u32 path_id, HD_VIDEO_FRAME *p_video_frame, INT32 wait_ms)
	{
		return HD_OK;
	}

	HD_RESULT hd_videoproc_release_out_buf(__u32 path_id, HD_VIDEO_FRAME *p_video_frame)
	{
		return HD_OK;
	}
	
	HD_RESULT hd_videoproc_poll_list(HD_VIDEOPROC_POLL_LIST *p_poll, UINT32 num, INT32 wait_ms)
	{
		return HD_OK;
	}

	HD_RESULT hd_videoproc_send_list(HD_VIDEOPROC_SEND_LIST *p_videoproc_list, UINT32 num, INT32 wait_ms)
	{
		return HD_OK;
	}

	HD_RESULT hd_videoproc_close(__u32 path_id)
	{
		return HD_OK;
	}

	HD_RESULT hd_videoproc_uninit(VOID)
	{
		return HD_OK;
	}

	HD_RESULT vendor_videoenc_get(HD_PATH_ID path_id, VENDOR_VIDEOENC_PARAM_ID id, void *p_param)
	{
		return HD_OK;
	}

	HD_RESULT vendor_video_get(HD_PATH_ID path_id, VENDOR_VIDEO_PARAM_ID id, void *p_param)
	{
		return HD_OK;
	}

	HD_RESULT vendor_video_set(HD_PATH_ID path_id, VENDOR_VIDEO_PARAM_ID id, void *p_param)
	{
		return HD_OK;
	}

	HD_RESULT vendor_video_init(HD_PATH_ID path_id)
	{
		return HD_OK;
	}

	HD_RESULT vendor_video_uninit(HD_PATH_ID path_id)
	{
		return HD_OK;
	}

	HD_RESULT hd_audiocap_init(void)
	{
		return HD_OK;
	}

	HD_RESULT hd_audiocap_bind(HD_OUT_ID out_id, HD_IN_ID dest_in_id)
	{
		return HD_OK;
	}

	HD_RESULT hd_audiocap_open(HD_IN_ID in_id, HD_OUT_ID out_id, HD_PATH_ID *p_path_id)
	{
		return HD_OK;
	}

	HD_RESULT hd_audiocap_start(HD_PATH_ID path_id)
	{
		return HD_OK;
	}

	HD_RESULT hd_audiocap_stop(HD_PATH_ID path_id)
	{
		return HD_OK;
	}

	HD_RESULT hd_audiocap_set(HD_PATH_ID path_id, HD_AUDIOCAP_PARAM_ID id, VOID *p_param)
	{
		return HD_OK;
	}

	HD_RESULT hd_audiocap_get(HD_PATH_ID path_id, HD_AUDIOCAP_PARAM_ID id, VOID *p_param)
	{
		return HD_OK;
	}

	HD_RESULT hd_audioenc_init(VOID)
	{
		return HD_OK;
	}

	HD_RESULT hd_audioenc_bind(HD_OUT_ID out_id, HD_IN_ID dest_in_id)
	{
		return HD_OK;
	}

	HD_RESULT hd_audioenc_unbind(HD_OUT_ID out_id)
	{
		return HD_OK;
	}

	HD_RESULT hd_audioenc_open(HD_IN_ID in_id, HD_OUT_ID out_id, HD_PATH_ID *p_path_id)
	{
		return HD_OK;
	}

	HD_RESULT hd_audioenc_start(HD_PATH_ID path_id)
	{
		return HD_OK;
	}

	HD_RESULT hd_audioenc_stop(HD_PATH_ID path_id)
	{
		return HD_OK;
	}

	HD_RESULT hd_audioenc_start_list(HD_PATH_ID *path_id, UINT num)
	{
		return HD_OK;
	}

	HD_RESULT hd_audioenc_stop_list(HD_PATH_ID *path_id, UINT num)
	{
		return HD_OK;
	}

	HD_RESULT hd_audioenc_get(HD_PATH_ID path_id, HD_AUDIOENC_PARAM_ID id, VOID *p_param)
	{
		return HD_OK;
	}

	HD_RESULT hd_audioenc_set(HD_PATH_ID path_id, HD_AUDIOENC_PARAM_ID id, VOID *p_param)
	{
		return HD_OK;
	}

	HD_RESULT hd_audioenc_push_in_buf(HD_PATH_ID path_id, HD_AUDIO_FRAME *p_in_audio_frame, 
		HD_AUDIO_BS *p_user_out_audio_bs, INT32 wait_ms)
	{
		return HD_OK;
	}

	HD_RESULT hd_audioenc_pull_out_buf(HD_PATH_ID path_id, HD_AUDIO_BS *p_audio_bs, INT32 wait_ms)
	{
		return HD_OK;
	}

	HD_RESULT hd_audioenc_release_out_buf(HD_PATH_ID path_id, HD_AUDIO_BS *p_audio_bs)
	{
		return HD_OK;
	}

	HD_RESULT hd_audioenc_poll_list(HD_AUDIOENC_POLL_LIST *p_poll, UINT32 num, INT32 wait_ms)
	{
		return HD_OK;
	}

	HD_RESULT hd_audioenc_recv_list(HD_AUDIOENC_RECV_LIST *p_audio_bs, UINT32 num)
	{
		return HD_OK;
	}

	HD_RESULT hd_audioenc_close(HD_PATH_ID path_id)
	{
		return HD_OK;
	}

	HD_RESULT hd_audioenc_uninit(VOID)
	{
		return HD_OK;
	}

};

//--------------------------------------------------------------------
/*HI_S32 HI_MPI_ADEC_CreateChn(ADEC_CHN AdChn, ADEC_CHN_ATTR_S *pstAttr)
{
	return	HI_SUCCESS;
}*/
//--------------------------------------------------------------------
