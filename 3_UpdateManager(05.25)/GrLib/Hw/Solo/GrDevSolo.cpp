/*
	solo device control

*/

//====================================================================
//include
#include	<Hw/Solo/GrDevSolo.h>
#include	<GrDebug.h>
#include	<GrDumyTool.h>
#include	<Hw/Solo/RegSolo.h>
#include	<Hw/Solo/GrDrvSolo.h>
#include	<GrGdibTool.h>
#include	<GrBufTool.h>
#include	<GrTimeTool.h>
#include	<GrProcess.h>

#include	<stdlib.h>

#ifdef LINUX_APP

#include	<sys/types.h>
#include	<sys/stat.h>
#include	<sys/ioctl.h>
#include	<sys/time.h>
#include	<time.h>
#include	<pthread.h>
#include	<fcntl.h>
#include	<unistd.h>
#include	<sys/mman.h>

#else

#include <EmuLnx/GrElaBase.h>

#endif

//====================================================================
//local const

#define E_GrDevSoloI2cTimeOut			2000
#define E_GrDevSoloUartWait				20
#define E_GrDevSoloMtnThrdLine		64
#define E_GrDevSoloMtnThrdMaxX		44
#define E_GrDevSoloMtnThrdMaxY		36
#define E_GrDevSoloMtnCfgSize			0x2000

//====================================================================
//local type

//====================================================================
//global var

__u32	V_GrDevSoloDecIdx1[11]	=	
{
	//ntsc
	E_SoloRv_VD_IDX_V_BLOCK(15) | E_SoloRv_VD_IDX_H_BLOCK(22) | E_SoloRv_VD_IDX_SRC_SCALE( E_SoloRv_CAP_SCL_H1_2V1 ) ,
	E_SoloRv_VD_IDX_V_BLOCK(15) | E_SoloRv_VD_IDX_H_BLOCK(44) | E_SoloRv_VD_IDX_SRC_SCALE( E_SoloRv_CAP_SCL_H1_V1 ) ,
	E_SoloRv_VD_IDX_V_BLOCK(30) | E_SoloRv_VD_IDX_H_BLOCK(44) | E_SoloRv_VD_IDX_SRC_SCALE( E_SoloRv_CAP_SCL_H1_V1 | E_SoloRv_CAP_SCL_FRAMEMODE ) ,

	//reserved
	0 | 0 | 0 ,		//3
	0 | 0 | 0 ,		//4
	0 | 0 | 0 ,		//5
	0 | 0 | 0 ,		//6
	0 | 0 | 0 ,		//7

	//pal
	E_SoloRv_VD_IDX_V_BLOCK(18) | E_SoloRv_VD_IDX_H_BLOCK(22) | E_SoloRv_VD_IDX_SRC_SCALE( E_SoloRv_CAP_SCL_H1_2V1 ) ,
	E_SoloRv_VD_IDX_V_BLOCK(18) | E_SoloRv_VD_IDX_H_BLOCK(44) | E_SoloRv_VD_IDX_SRC_SCALE( E_SoloRv_CAP_SCL_H1_V1 ) ,
	E_SoloRv_VD_IDX_V_BLOCK(36) | E_SoloRv_VD_IDX_H_BLOCK(44) | E_SoloRv_VD_IDX_SRC_SCALE( E_SoloRv_CAP_SCL_H1_V1 | E_SoloRv_CAP_SCL_FRAMEMODE ) ,
};

//====================================================================
//functions

//--------------------------------------------------------------------
Cls_GrDevSolo::Cls_GrDevSolo( __s32 A_HndlDrv, __u8 A_DevIdx, __u32 A_PhyDrvMem, void* A_PtrDrvMem, __u32 A_PllMhz )
{
	// local -------------------
		St_GrDrvSoloDevInfo	Tv_DevInfo;
		__u32	Tv_WkIdx;
	// code --------------------
		//init
		m_HndlDrv			=	A_HndlDrv;
		m_DevIdx			=	A_DevIdx;
		m_PhysDrvMem	=	A_PhyDrvMem;
		m_PllMhz			=	A_PllMhz;
		m_PtrDrvMem		=	A_PtrDrvMem;
		m_PtrDrvMng		=	(Ptr_GrDrvSoloMng)A_PtrDrvMem;
		m_IsIrqPump		=	TRUE;
		m_AdoChCnt		=	0;
		m_IsMtnMap		=	FALSE;

		m_PtrCtrl			=	&m_PtrDrvMng->DevTbl[A_DevIdx];

		//init
		m_GdibOsg		=	NULL;
		m_VdoResX		=	0;
		m_VdoResY		=	0;
		m_VdoFps		=	0;

		m_CovertMap	=	0;
		m_PlayChMap	=	0;

		m_DecVreqId	=	0;
		m_DecAreqId	=	0;

		m_RecMtnMap	=	0;
		m_RecDiMap	=	0;

		m_PlayDintlVal	=	0;
		m_IsPlaying	=	FALSE;

		m_PtrBufMtnCfg	=	NULL;
		m_PtrBufAdoDec	=	NULL;
		m_PtrBufAdoSlc	=	NULL;
		m_PtrBufEncOsd	=	NULL;
		m_PtrBufNetAdo	=	NULL;
		m_PtrBufJpegCap	=	NULL;
		m_PtrBufImgCap	=	NULL;
		m_PtrBufOsg			=	NULL;
		m_PtrBufPlay		=	NULL;
		m_PtrBufNetMpg 	=	NULL;
		m_PtrBufNetJpg	=	NULL;
		m_PtrBufRec			=	NULL;

		GrPrcCritInit( &m_I2cCrit );
		m_I2cTimeOut	=	GrTimeMsToTick( E_GrDevSoloI2cTimeOut );

		for ( Tv_WkIdx=0;Tv_WkIdx < E_SoloUartChCnt;Tv_WkIdx++ )
		{
			GrPrcCritInit( &m_UartChTbl[Tv_WkIdx].CritRead );
			GrPrcCritInit( &m_UartChTbl[Tv_WkIdx].CritWrite );
		}

		GrDumyZeroMem( m_PlayAtb, sizeof(m_PlayAtb) );

		//get device info
		Tv_DevInfo.DevIdx		=	m_DevIdx;
		if ( 0 <=	ioctl( m_HndlDrv, E_GrDrvSoloIoctlDevInfo, &Tv_DevInfo ) )
		{
			//update parameter
			m_VdoChCnt	=	Tv_DevInfo.VdoChCnt;
			m_CodecPwr	=	Tv_DevInfo.CodecPwr;
			m_VdoChMask	=	(1 << m_VdoChCnt) - 1;
		}
		
}
//--------------------------------------------------------------------
Cls_GrDevSolo::~Cls_GrDevSolo()
{
	// local -------------------
		__u32	Tv_WkIdx;
	// code --------------------

		LcDevFinish();

		for ( Tv_WkIdx=0;Tv_WkIdx < E_SoloUartChCnt;Tv_WkIdx++ )
		{
			GrPrcCritDel( &m_UartChTbl[Tv_WkIdx].CritRead );
			GrPrcCritDel( &m_UartChTbl[Tv_WkIdx].CritWrite );
		}

		GrPrcCritDel( &m_I2cCrit );

}
//--------------------------------------------------------------------
BOOL8	Cls_GrDevSolo::DevConfig( Ptr_GrDrvSoloBufCfg A_PtrCfgBuf, Ptr_GrDrvSoloGpioCfg A_PtrCfgGpio )
{
	// local -------------------
		BOOL8	Tv_Result;
		__s32		Tv_Rslt;
		__u32	Tv_Ioc;
	// code --------------------
		//init
		Tv_Result	=	FALSE;

		m_DecVreqId	=	0;
		m_DecAreqId	=	0;
		
		//finish device
		LcDevFinish();

		//set parameter
		GrDumyCopyMem( &m_PtrCtrl->CfgBuf, A_PtrCfgBuf, sizeof(St_GrDrvSoloBufCfg) );
		GrDumyCopyMem( &m_PtrCtrl->CfgGpio, A_PtrCfgGpio, sizeof(St_GrDrvSoloGpioCfg) );
		m_PtrCtrl->DecVprsId	=	0;		//reset decode process id
		m_PtrCtrl->DecAprsId	=	0;		// reset decode audio processed id
		m_PtrCtrl->EncMpgWtc	=	1;		// init watchdog counter

		//try config
		Tv_Ioc		=	m_DevIdx;
		Tv_Rslt	=	ioctl( m_HndlDrv, E_GrDrvSoloIoctlDevCfg, &Tv_Ioc );
		if ( 0 <= Tv_Rslt )
		{
			//init buffer pointer
			m_PtrBufMtnCfg	=	(void*)( (__u32)m_PtrDrvMem + A_PtrCfgBuf->MtnCfg.Pos );
			m_PtrBufAdoDec	=	(void*)( (__u32)m_PtrDrvMem + A_PtrCfgBuf->AdoDec.Pos );
			m_PtrBufAdoSlc	=	(void*)( (__u32)m_PtrDrvMem + A_PtrCfgBuf->AdoSlc.Pos );
			m_PtrBufEncOsd	=	(void*)( (__u32)m_PtrDrvMem + A_PtrCfgBuf->EncOsd.Pos );
			m_PtrBufNetAdo	=	(void*)( (__u32)m_PtrDrvMem + A_PtrCfgBuf->NetAdo.Pos );
			m_PtrBufJpegCap	=	(void*)( (__u32)m_PtrDrvMem + A_PtrCfgBuf->JpegCap.Pos );
			m_PtrBufImgCap	=	(void*)( (__u32)m_PtrDrvMem + A_PtrCfgBuf->ImgCap.Pos );
			m_PtrBufOsg			=	(void*)( (__u32)m_PtrDrvMem + A_PtrCfgBuf->Osg.Pos );
			m_PtrBufNetJpg	=	(void*)( (__u32)m_PtrDrvMem + A_PtrCfgBuf->NetJpg.Pos );
			m_PtrBufPlay		=	(void*)( (__u32)m_PtrDrvMem + A_PtrCfgBuf->Play.Pos );
			m_PtrBufNetMpg	=	(void*)( (__u32)m_PtrDrvMem + A_PtrCfgBuf->NetMpg.Pos );
			m_PtrBufRec			=	(void*)( (__u32)m_PtrDrvMem + A_PtrCfgBuf->Rec.Pos );

			//init silence queue
			GrDumyZeroMem( m_PtrBufAdoSlc, A_PtrCfgBuf->AdoSlc.Size );

			//init buffer queue
			GrBufRqInit( (Ptr_GrBufRefQue)m_PtrBufRec, A_PtrCfgBuf->Rec.Size, E_GrDrvSoloVencP2mAlignUnit );		//record buffer
			GrBufMaInit( (Ptr_GrBufMaMng)m_PtrBufNetMpg, A_PtrCfgBuf->NetMpg.Size );
			GrBufMaInit( (Ptr_GrBufMaMng)m_PtrBufNetJpg, A_PtrCfgBuf->NetJpg.Size );
			GrBufQueInit( (Ptr_GrBufQue)m_PtrBufAdoDec, E_GrDrvSoloAdoClustSize, A_PtrCfgBuf->AdoDec.Size );	//audio oob
			GrBufQueInit( (Ptr_GrBufQue)m_PtrCtrl->AdecPlayQue, sizeof(St_GrDrvSoloAdoPlayItm), E_GrDrvSoloAdoPlayQueSize );	//audio play queue

			//create osg - maximum buffer
			m_GdibOsg	=	GrGdibCreate( E_GrFccUYVYF, E_SoloBaseResX, E_SoloPalResY, E_GrGdibPitchAuto, m_PtrBufOsg );

			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDevSolo::LcDevFinish( void )
{
	// local -------------------
	// code --------------------
		//finish av
		LcAvFinish();

		//reset osg
		if ( NULL != m_GdibOsg )
		{
			delete	m_GdibOsg;
			m_GdibOsg	=	NULL;
		}

		//reset buffer pointer
		m_PtrBufMtnCfg	=	NULL;
		m_PtrBufAdoDec	=	NULL;
		m_PtrBufAdoSlc	=	NULL;
		m_PtrBufEncOsd	=	NULL;
		m_PtrBufNetAdo	=	NULL;
		m_PtrBufJpegCap	=	NULL;
		m_PtrBufImgCap	=	NULL;
		m_PtrBufOsg			=	NULL;
		m_PtrBufNetJpg	=	NULL;
		m_PtrBufPlay		=	NULL;
		m_PtrBufNetMpg 	=	NULL;
		m_PtrBufRec			=	NULL;
 
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDevSolo::AvConfig( Ptr_GrDrvSoloAveCfg A_PtrCfgAv )
{
	// local -------------------
		BOOL8	Tv_Result;
		__s32		Tv_Rslt;
		__u32	Tv_Ioc;
		__u32	Tv_WkIdx;
	// code --------------------
		//init
		Tv_Result	=	FALSE;

		//stop av config
		LcAvFinish();

		//update configuration
		GrDumyCopyMem( &m_PtrCtrl->CfgAv, A_PtrCfgAv, sizeof(St_GrDrvSoloAveCfg) );

		//patch index
		Tv_Ioc	=	m_DevIdx;
		Tv_Rslt	=	ioctl( m_HndlDrv, E_GrDrvSoloIoctlAvCfg, &Tv_Ioc );
		if ( 0 <= Tv_Rslt )
		{
			//update info
			m_AdoChCnt	=	A_PtrCfgAv->Ado.ChCnt;
			m_VdoResX		=	E_SoloBaseResX;
			m_VdoResY		=	E_SoloNtscResY;
			m_VdoFps		=	E_SoloNtscFps;
			if ( A_PtrCfgAv->IsVdoPal )
			{
				m_VdoResY		=	E_SoloPalResY;
				m_VdoFps		=	E_SoloPalFps;
			}
			m_IsMtnMap	=	A_PtrCfgAv->IsMtnMap;		//update motion map
			//motion map initialize
			GrDumyFillD( m_PtrBufMtnCfg, m_PtrCtrl->CfgBuf.MtnCfg.Size >> 2, 0xFFFFFFFF );
			for ( Tv_WkIdx=0;Tv_WkIdx < E_SoloMaxChCnt;Tv_WkIdx++ )
			{
				m_MtnSenTbl[Tv_WkIdx]	=	0xFFFF;		//sensortive init
			}
			//nosignal map clear
			m_PtrCtrl->NosigMap			=	0;
			//success
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDevSolo::LcAvFinish( void )
{
	// local -------------------
	// code --------------------
		//encode stop

		//reset info
		m_VdoResX		=	0;
		m_VdoResY		=	0;
		m_VdoFps		=	0;
}
//--------------------------------------------------------------------
void	Cls_GrDevSolo::RegWrite( __u32 A_Adr, __u32 A_Val )
{
	// local -------------------
		St_GrDrvSoloRegCtrl	Tv_RegCtrl;
	// code --------------------
		Tv_RegCtrl.DevIdx	=	m_DevIdx;
		Tv_RegCtrl.Adr		=	A_Adr;
		Tv_RegCtrl.Val		=	A_Val;
		ioctl( m_HndlDrv, E_GrDrvSoloIoctlWriteReg, &Tv_RegCtrl );
}
//--------------------------------------------------------------------
__u32	Cls_GrDevSolo::RegRead( __u32 A_Adr )
{
	// local -------------------
		St_GrDrvSoloRegCtrl	Tv_RegCtrl;
	// code --------------------
		Tv_RegCtrl.DevIdx	=	m_DevIdx;
		Tv_RegCtrl.Adr		=	A_Adr;
		ioctl( m_HndlDrv, E_GrDrvSoloIoctlReadReg, &Tv_RegCtrl );
		return	Tv_RegCtrl.Val;
}
//--------------------------------------------------------------------
Cls_GrGdib*	Cls_GrDevSolo::OsgGdibObjGet( void )
{
		return	m_GdibOsg;
}
//--------------------------------------------------------------------
__u32	Cls_GrDevSolo::VdoChCnt( void )
{
		return	m_VdoChCnt;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDevSolo::P2mReqCopy(	__u32 A_Ch, void* A_PtrSys, __u32 A_PosLmem, __u32 A_Size, BOOL8 A_SysToLmem,
																	__u8 A_ParaType, __u32 A_Para0, __u32 A_Para1, __u32 A_Para2 )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_PosSys;
		Ptr_GrBufQue	Tv_PtrQue;
		Ptr_GrDrvSoloP2mReq	Tv_PtrP2mItm;
	// code --------------------
		Tv_Result	=	FALSE;
		Tv_PosSys	=	(__u32)A_PtrSys - (__u32)m_PtrDrvMem;
		Tv_PtrQue	=	(Ptr_GrBufQue)m_PtrCtrl->P2mQue[A_Ch];
		Tv_PtrP2mItm	=	(Ptr_GrDrvSoloP2mReq)GrBufQueGetInPtr( Tv_PtrQue );
		if ( NULL != Tv_PtrP2mItm )
		{
			//make register
			Tv_PtrP2mItm->RegTarAdr	=	Tv_PosSys + m_PhysDrvMem;
			Tv_PtrP2mItm->RegExtAdr	=	A_PosLmem;
			Tv_PtrP2mItm->RegExtCfg	=	E_SoloRv_P2M_COPY_SIZE((A_Size >> 2));
			Tv_PtrP2mItm->RegCtrl		=	E_SoloRv_P2M_BURST_SIZE(E_SoloRv_P2M_BURST_256) | E_SoloRv_P2M_TRANS_ON;
			if ( A_SysToLmem )
			{
				Tv_PtrP2mItm->RegCtrl		=	Tv_PtrP2mItm->RegCtrl | E_SoloRv_P2M_WRITE;
			}

			//udpate status
			Tv_PtrP2mItm->Stat			=	E_GrDrvSoloP2mStatNone;
			//update parameter
			Tv_PtrP2mItm->ParaType	=	A_ParaType;
			Tv_PtrP2mItm->Para[0]		=	A_Para0;			
			Tv_PtrP2mItm->Para[1]		=	A_Para1;			
			Tv_PtrP2mItm->Para[2]		=	A_Para2;

			//queue input
			GrBufQueIn( Tv_PtrQue );
			//success
			Tv_Result	=	TRUE;
			//irq pump
			LcPumpIrq();

		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDevSolo::P2mReqBlt(	__u32 A_Ch, void* A_PtrSys, __u32 A_PosLmem, __u32 A_LineSize, __u32 A_LineCount, 
																BOOL8 A_SysToLmem, __u32 A_PitchSys, __u32 A_PitchLmem , 
																__u8 A_ParaType, __u32 A_Para0, __u32 A_Para1, __u32 A_Para2 )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_PosSys;
		Ptr_GrBufQue	Tv_PtrQue;
		Ptr_GrDrvSoloP2mReq	Tv_PtrP2mItm;
	// code --------------------
		Tv_Result	=	FALSE;
		Tv_PtrQue	=	(Ptr_GrBufQue)m_PtrCtrl->P2mQue[A_Ch];
		Tv_PosSys	=	(__u32)A_PtrSys - (__u32)m_PtrDrvMem;
		Tv_PtrP2mItm	=	(Ptr_GrDrvSoloP2mReq)GrBufQueGetInPtr( Tv_PtrQue );
		if ( NULL != Tv_PtrP2mItm )
		{
			//make register
			Tv_PtrP2mItm->RegTarAdr	=	Tv_PosSys + m_PhysDrvMem;
			Tv_PtrP2mItm->RegExtAdr	=	A_PosLmem;
			Tv_PtrP2mItm->RegExtCfg	=	E_SoloRv_P2M_COPY_SIZE((A_LineSize >> 2))  | E_SoloRv_P2M_EXT_INC( (A_PitchLmem >> 2) );
			Tv_PtrP2mItm->RegCtrl		=	E_SoloRv_P2M_BURST_SIZE(E_SoloRv_P2M_BURST_256) | E_SoloRv_P2M_TRANS_ON |
				E_SoloRv_P2M_PCI_INC((A_PitchSys >> 2)) | E_SoloRv_P2M_REPEAT( (A_LineCount - 1) );
			if ( A_SysToLmem )
			{
				Tv_PtrP2mItm->RegCtrl		=	Tv_PtrP2mItm->RegCtrl | E_SoloRv_P2M_WRITE;
			}

			//udpate status
			Tv_PtrP2mItm->Stat			=	E_GrDrvSoloP2mStatNone;
			//update parameter
			Tv_PtrP2mItm->ParaType	=	A_ParaType;
			Tv_PtrP2mItm->Para[0]		=	A_Para0;			
			Tv_PtrP2mItm->Para[1]		=	A_Para1;			
			Tv_PtrP2mItm->Para[2]		=	A_Para2;			

			//queue input
			GrBufQueIn( Tv_PtrQue );
			//success
			Tv_Result	=	TRUE;
			//irq pump
			LcPumpIrq();
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDevSolo::OsgUpdateRect( Ptr_GrRect A_PtrRect )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrRect	Tv_Rc;
		void*	Tv_PtrSys;
		__u32	Tv_LmemPos;
		__u32	Tv_LineSize;
		__u32	Tv_LineCnt;
#ifndef LINUX_APP
		__u32	Tv_EndAdr;
#endif
	// code --------------------

		Tv_Rc.left		=	0;
		Tv_Rc.top			=	0;
		Tv_Rc.right		=	m_GdibOsg->GetResX();
		Tv_Rc.bottom	=	m_GdibOsg->GetResY();
		if ( NULL != A_PtrRect )
		{
			Tv_Rc	=	*A_PtrRect;
		}
		//calculate address
		Tv_Rc.left	=	Tv_Rc.left & 0xFFFFFFF8;
		Tv_Rc.right	=	(Tv_Rc.right + 7) & 0xFFFFFFF8;
		Tv_PtrSys		=	(void*)( (__u32)m_GdibOsg->GetDibPtr() + (m_GdibOsg->GetDibPitch() * Tv_Rc.top) + (Tv_Rc.left << 1) );
		Tv_LmemPos	=	E_GrDrvSoloLmemOsgPos + ( Tv_Rc.top * E_SoloOsgLmemPitch ) + (Tv_Rc.left << 1);
		Tv_LineSize	=	(Tv_Rc.right - Tv_Rc.left) << 1;
		Tv_LineCnt	=	Tv_Rc.bottom - Tv_Rc.top;
#ifndef LINUX_APP
		if ( Tv_LineSize > E_SoloOsgLmemPitch )
		{
			DbgMsgPrint( "GrDevSolo - [Emergency] - OSG line size overflow!!!!\n" );
		}
		Tv_EndAdr		=	Tv_LmemPos + (E_SoloOsgLmemPitch * Tv_LineCnt);
		if ( (Tv_EndAdr > (E_GrDrvSoloLmemOsgPos + E_GrDrvSoloLmemOsgSize)) || (E_GrDrvSoloLmemOsgPos > Tv_LmemPos) )
		{
			DbgMsgPrint( "GrDevSolo - [Emergency] - OSG line count overflow at %08X!!!!\n", Tv_EndAdr );
		}
#endif
		//completion event
		Tv_Result	=	P2mReqBlt(	E_GrDrvSoloP2mChOsg, Tv_PtrSys, Tv_LmemPos, Tv_LineSize, Tv_LineCnt, TRUE, 
														m_GdibOsg->GetDibPitch(), E_SoloOsgLmemPitch, 
														E_GrDrvSoloP2mParaModeNone, 0, 0, 0 );

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDevSolo::I2cSlotRead( __u8 A_Ch, __u8 A_Slot, void* A_PtrData, BOOL8 A_IsWait )
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrDrvSoloI2cRtSlot	Tv_PtrRtSlot;
		__u32	Tv_Tick;
	// code --------------------
		Tv_Result	=	FALSE;

		//not need critical section
		//check channel range
		if ( E_SoloI2cChCnt > A_Ch )
		{
			if ( E_GrDrvSoloI2cRtSlotCnt > A_Slot )
			{
				Tv_PtrRtSlot	=	&m_PtrCtrl->I2cRtSlot[A_Slot];
				//check wait
				if ( A_IsWait )
				{
					Tv_Tick	=	GrTimeGetTick();
					while ( m_I2cTimeOut > (GrTimeGetTick() - Tv_Tick) )
					{
						if ( Tv_PtrRtSlot->IsCpl )
						{
							//completed
							break;
						}
						//wait
						GrPrcSleep( 0 );
					}
				}
				//check finished
				if ( Tv_PtrRtSlot->IsCpl && (!Tv_PtrRtSlot->IsErr) )
				{
					//get read value
					if ( 0 != Tv_PtrRtSlot->ReadSize )
					{
						GrDumyCopyMem( A_PtrData, Tv_PtrRtSlot->ReadBuf, Tv_PtrRtSlot->ReadSize );
						Tv_Result	=	TRUE;
					}
				}
				else
				{
					if ( Tv_PtrRtSlot->IsErr )
					{
						DbgMsgPrint( "I2c Error. ch %x , dev %x , subadr %x \n", Tv_PtrRtSlot->Ch, Tv_PtrRtSlot->DevAdr, Tv_PtrRtSlot->SubAdr );
					}
					else
					{
						if ( A_IsWait )
						{
							DbgMsgPrint( "I2c time out. ch %x , dev %x , subadr %x \n", Tv_PtrRtSlot->Ch, Tv_PtrRtSlot->DevAdr, Tv_PtrRtSlot->SubAdr );
						}
					}
				}
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDevSolo::I2cStdWrite( __u8 A_Ch, __u8 A_AdrDev, __u8 A_AdrSub, __u8* A_PtrData, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrBufQue	Tv_PtrQueReq;
		Ptr_GrDrvSoloI2cReq	Tv_PtrItmReq;
		__u32		Tv_WkIdx;
		__u8*		Tv_PtrWk;
	// code --------------------
		Tv_Result	=	FALSE;

		//critical section
		GrPrcCritEnter( &m_I2cCrit );

		//check channel range
		if ( E_SoloI2cChCnt > A_Ch )
		{
			Tv_PtrQueReq	=	(Ptr_GrBufQue)m_PtrCtrl->I2cQueReq;
			Tv_PtrItmReq	=	(Ptr_GrDrvSoloI2cReq)GrBufQueGetInPtr( Tv_PtrQueReq );
			while ( NULL == Tv_PtrItmReq )
			{
				GrPrcSleep( 0 );
				Tv_PtrItmReq	=	(Ptr_GrDrvSoloI2cReq)GrBufQueGetInPtr( Tv_PtrQueReq );
			}
			Tv_PtrItmReq->Ch			=	A_Ch;
			Tv_PtrItmReq->IsRun		=	FALSE;
			Tv_PtrItmReq->DevAdr	=	A_AdrDev;
			Tv_PtrItmReq->SubAdr	=	A_AdrSub;
			Tv_PtrItmReq->ReadCnt	=	0;
			Tv_PtrItmReq->Prgs		=	0;
			Tv_PtrItmReq->Slot		=	E_GrDrvSoloI2cRtSlotIdxNone;
			Tv_PtrItmReq->CmdCnt	=	0;

			//send address
			Tv_PtrItmReq->ValTbl[Tv_PtrItmReq->CmdCnt]	=	A_AdrDev;
			Tv_PtrItmReq->CmdTbl[Tv_PtrItmReq->CmdCnt]	=	E_SoloRv_IIC_CH(A_Ch) | E_SoloRv_IIC_START | E_SoloRv_IIC_WRITE;
			Tv_PtrItmReq->CmdCnt ++;

			//send subaddress
			Tv_PtrItmReq->ValTbl[Tv_PtrItmReq->CmdCnt]	=	A_AdrSub;
			Tv_PtrItmReq->CmdTbl[Tv_PtrItmReq->CmdCnt]	=	E_SoloRv_IIC_CH(A_Ch) | E_SoloRv_IIC_WRITE;
			Tv_PtrItmReq->CmdCnt ++;

			//write data
			Tv_PtrWk	=	A_PtrData;
			Tv_WkIdx	=	A_Size - 1;
			while ( 0 != Tv_WkIdx )
			{
				//send data
				Tv_PtrItmReq->ValTbl[Tv_PtrItmReq->CmdCnt]	=	*Tv_PtrWk;
				Tv_PtrItmReq->CmdTbl[Tv_PtrItmReq->CmdCnt]	=	E_SoloRv_IIC_CH(A_Ch) | E_SoloRv_IIC_WRITE;
				Tv_PtrItmReq->CmdCnt ++;
				//next
				Tv_PtrWk ++;
				Tv_WkIdx --;
			}
			//send last byte
			Tv_PtrItmReq->ValTbl[Tv_PtrItmReq->CmdCnt]	=	*Tv_PtrWk;
			Tv_PtrItmReq->CmdTbl[Tv_PtrItmReq->CmdCnt]	=	E_SoloRv_IIC_CH(A_Ch) | E_SoloRv_IIC_WRITE | E_SoloRv_IIC_STOP;
			Tv_PtrItmReq->CmdCnt ++;

			//send request
			GrBufQueIn( Tv_PtrQueReq );
			Tv_Result		=	TRUE;
			//pump irq
			LcPumpIrq();
		}

		GrPrcCritLeave( &m_I2cCrit );

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDevSolo::I2cStdRead( __u8 A_Ch, __u8 A_AdrDev, __u8 A_AdrSub, __u32 A_Size, __u8 A_Slot )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32		Tv_WkIdx;
		Ptr_GrDrvSoloI2cRtSlot	Tv_PtrRtSlot;
		Ptr_GrBufQue	Tv_PtrQueReq;
		Ptr_GrDrvSoloI2cReq	Tv_PtrItmReq;
	// code --------------------
		Tv_Result	=	FALSE;

		//critical section
		GrPrcCritEnter( &m_I2cCrit );

		//check channel range
		if ( (E_SoloI2cChCnt > A_Ch) && (E_GrDrvSoloI2cRtSlotCnt > A_Slot) )
		{
			Tv_PtrQueReq	=	(Ptr_GrBufQue)m_PtrCtrl->I2cQueReq;
			Tv_PtrRtSlot	=	&m_PtrCtrl->I2cRtSlot[A_Slot];
			Tv_PtrItmReq	=	(Ptr_GrDrvSoloI2cReq)GrBufQueGetInPtr( Tv_PtrQueReq );
			while ( NULL == Tv_PtrItmReq )
			{
				GrPrcSleep( 0 );
				Tv_PtrItmReq	=	(Ptr_GrDrvSoloI2cReq)GrBufQueGetInPtr( Tv_PtrQueReq );
			}
			Tv_PtrItmReq->Ch			=	A_Ch;
			Tv_PtrItmReq->IsRun		=	FALSE;
			Tv_PtrItmReq->DevAdr	=	A_AdrDev;
			Tv_PtrItmReq->SubAdr	=	A_AdrSub;
			Tv_PtrItmReq->ReadCnt	=	0;
			Tv_PtrItmReq->Prgs		=	0;
			Tv_PtrItmReq->Slot		=	A_Slot;
			Tv_PtrItmReq->CmdCnt	=	0;

			//reset complete mark
			Tv_PtrRtSlot->IsCpl		=	FALSE;
			Tv_PtrRtSlot->IsErr		=	FALSE;

			//send address
			Tv_PtrItmReq->ValTbl[Tv_PtrItmReq->CmdCnt]	=	A_AdrDev;
			Tv_PtrItmReq->CmdTbl[Tv_PtrItmReq->CmdCnt]	=	E_SoloRv_IIC_CH(A_Ch) | E_SoloRv_IIC_WRITE | E_SoloRv_IIC_START;
			Tv_PtrItmReq->CmdCnt ++;

			//send subaddress
			Tv_PtrItmReq->ValTbl[Tv_PtrItmReq->CmdCnt]	=	A_AdrSub;
			Tv_PtrItmReq->CmdTbl[Tv_PtrItmReq->CmdCnt]	=	E_SoloRv_IIC_CH(A_Ch) | E_SoloRv_IIC_WRITE;
			Tv_PtrItmReq->CmdCnt ++;

			//send read address
			Tv_PtrItmReq->ValTbl[Tv_PtrItmReq->CmdCnt]	=	A_AdrDev | 1;
			Tv_PtrItmReq->CmdTbl[Tv_PtrItmReq->CmdCnt]	=	E_SoloRv_IIC_CH(A_Ch) | E_SoloRv_IIC_WRITE | E_SoloRv_IIC_START;
			Tv_PtrItmReq->CmdCnt ++;

			//read work
			Tv_WkIdx	=	A_Size - 1;
			while ( 0 != Tv_WkIdx )
			{
				//send command
				Tv_PtrItmReq->CmdTbl[Tv_PtrItmReq->CmdCnt]	=	E_SoloRv_IIC_CH(A_Ch) | E_SoloRv_IIC_READ;
				Tv_PtrItmReq->CmdCnt ++;

				//next
				Tv_WkIdx --;
			}
			//send last byte
			Tv_PtrItmReq->CmdTbl[Tv_PtrItmReq->CmdCnt]	=	E_SoloRv_IIC_CH(A_Ch) | E_SoloRv_IIC_READ | E_SoloRv_IIC_STOP | E_SoloRv_IIC_ACK_EN;
			Tv_PtrItmReq->CmdCnt ++;

			//send request
			GrBufQueIn( Tv_PtrQueReq );
			Tv_Result	=	TRUE;
			//pump irq
			LcPumpIrq();
		}

		GrPrcCritLeave( &m_I2cCrit );

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDevSolo::I2cS16Write( __u8 A_Ch, __u8 A_AdrDev, __u16 A_AdrSub, __u8* A_PtrData, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrBufQue	Tv_PtrQueReq;
		Ptr_GrDrvSoloI2cReq	Tv_PtrItmReq;
		__u32		Tv_WkIdx;
		__u8*		Tv_PtrWk;
	// code --------------------
		Tv_Result	=	FALSE;

		//critical section
		GrPrcCritEnter( &m_I2cCrit );

		//check channel range
		if ( E_SoloI2cChCnt > A_Ch )
		{
			Tv_PtrQueReq	=	(Ptr_GrBufQue)m_PtrCtrl->I2cQueReq;
			Tv_PtrItmReq	=	(Ptr_GrDrvSoloI2cReq)GrBufQueGetInPtr( Tv_PtrQueReq );
			while ( NULL == Tv_PtrItmReq )
			{
				GrPrcSleep( 0 );
				Tv_PtrItmReq	=	(Ptr_GrDrvSoloI2cReq)GrBufQueGetInPtr( Tv_PtrQueReq );
			}
			Tv_PtrItmReq->Ch			=	A_Ch;
			Tv_PtrItmReq->IsRun		=	FALSE;
			Tv_PtrItmReq->DevAdr	=	A_AdrDev;
			Tv_PtrItmReq->SubAdr	=	A_AdrSub;
			Tv_PtrItmReq->ReadCnt	=	0;
			Tv_PtrItmReq->Prgs		=	0;
			Tv_PtrItmReq->Slot		=	E_GrDrvSoloI2cRtSlotIdxNone;
			Tv_PtrItmReq->CmdCnt	=	0;

			//send address
			Tv_PtrItmReq->ValTbl[Tv_PtrItmReq->CmdCnt]	=	A_AdrDev;
			Tv_PtrItmReq->CmdTbl[Tv_PtrItmReq->CmdCnt]	=	E_SoloRv_IIC_CH(A_Ch) | E_SoloRv_IIC_START | E_SoloRv_IIC_WRITE;
			Tv_PtrItmReq->CmdCnt ++;

			//send subaddress - 1
			Tv_PtrItmReq->ValTbl[Tv_PtrItmReq->CmdCnt]	=	(__u8)((A_AdrSub >> 8) & 0xFF);
			Tv_PtrItmReq->CmdTbl[Tv_PtrItmReq->CmdCnt]	=	E_SoloRv_IIC_CH(A_Ch) | E_SoloRv_IIC_WRITE;
			Tv_PtrItmReq->CmdCnt ++;

			//send subaddress - 2
			Tv_PtrItmReq->ValTbl[Tv_PtrItmReq->CmdCnt]	=	(__u8)(A_AdrSub & 0xFF);
			Tv_PtrItmReq->CmdTbl[Tv_PtrItmReq->CmdCnt]	=	E_SoloRv_IIC_CH(A_Ch) | E_SoloRv_IIC_WRITE;
			Tv_PtrItmReq->CmdCnt ++;

			//send address
			Tv_PtrItmReq->ValTbl[Tv_PtrItmReq->CmdCnt]	=	A_AdrDev;
			Tv_PtrItmReq->CmdTbl[Tv_PtrItmReq->CmdCnt]	=	E_SoloRv_IIC_CH(A_Ch) | E_SoloRv_IIC_START | E_SoloRv_IIC_WRITE;
			Tv_PtrItmReq->CmdCnt ++;

			//write data
			Tv_PtrWk	=	A_PtrData;
			Tv_WkIdx	=	A_Size - 1;
			while ( 0 != Tv_WkIdx )
			{
				//send data
				Tv_PtrItmReq->ValTbl[Tv_PtrItmReq->CmdCnt]	=	*Tv_PtrWk;
				Tv_PtrItmReq->CmdTbl[Tv_PtrItmReq->CmdCnt]	=	E_SoloRv_IIC_CH(A_Ch) | E_SoloRv_IIC_WRITE;
				Tv_PtrItmReq->CmdCnt ++;
				//next
				Tv_PtrWk ++;
				Tv_WkIdx --;
			}
			//send last byte
			Tv_PtrItmReq->ValTbl[Tv_PtrItmReq->CmdCnt]	=	*Tv_PtrWk;
			Tv_PtrItmReq->CmdTbl[Tv_PtrItmReq->CmdCnt]	=	E_SoloRv_IIC_CH(A_Ch) | E_SoloRv_IIC_WRITE | E_SoloRv_IIC_STOP;
			Tv_PtrItmReq->CmdCnt ++;

			//send request
			GrBufQueIn( Tv_PtrQueReq );
			Tv_Result		=	TRUE;
			//pump irq
			LcPumpIrq();
		}

		GrPrcCritLeave( &m_I2cCrit );

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDevSolo::I2cS16Read( __u8 A_Ch, __u8 A_AdrDev, __u16 A_AdrSub, __u32 A_Size, __u8 A_Slot )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32		Tv_WkIdx;
		Ptr_GrDrvSoloI2cRtSlot	Tv_PtrRtSlot;
		Ptr_GrBufQue	Tv_PtrQueReq;
		Ptr_GrDrvSoloI2cReq	Tv_PtrItmReq;
	// code --------------------
		Tv_Result	=	FALSE;

		//critical section
		GrPrcCritEnter( &m_I2cCrit );

		//check channel range
		if ( (E_SoloI2cChCnt > A_Ch) && (E_GrDrvSoloI2cRtSlotCnt > A_Slot) )
		{
			Tv_PtrQueReq	=	(Ptr_GrBufQue)m_PtrCtrl->I2cQueReq;
			Tv_PtrRtSlot	=	&m_PtrCtrl->I2cRtSlot[A_Slot];
			Tv_PtrItmReq	=	(Ptr_GrDrvSoloI2cReq)GrBufQueGetInPtr( Tv_PtrQueReq );
			while ( NULL == Tv_PtrItmReq )
			{
				GrPrcSleep( 0 );
				Tv_PtrItmReq	=	(Ptr_GrDrvSoloI2cReq)GrBufQueGetInPtr( Tv_PtrQueReq );
			}
			Tv_PtrItmReq->Ch			=	A_Ch;
			Tv_PtrItmReq->IsRun		=	FALSE;
			Tv_PtrItmReq->DevAdr	=	A_AdrDev;
			Tv_PtrItmReq->SubAdr	=	A_AdrSub;
			Tv_PtrItmReq->ReadCnt	=	0;
			Tv_PtrItmReq->Prgs		=	0;
			Tv_PtrItmReq->Slot		=	A_Slot;
			Tv_PtrItmReq->CmdCnt	=	0;

			//reset complete mark
			Tv_PtrRtSlot->IsCpl		=	FALSE;
			Tv_PtrRtSlot->IsErr		=	FALSE;

			//send address
			Tv_PtrItmReq->ValTbl[Tv_PtrItmReq->CmdCnt]	=	A_AdrDev;
			Tv_PtrItmReq->CmdTbl[Tv_PtrItmReq->CmdCnt]	=	E_SoloRv_IIC_CH(A_Ch) | E_SoloRv_IIC_WRITE | E_SoloRv_IIC_START;
			Tv_PtrItmReq->CmdCnt ++;

			//send subaddress - 1
			Tv_PtrItmReq->ValTbl[Tv_PtrItmReq->CmdCnt]	=	(__u8)((A_AdrSub >> 8) & 0xFF);
			Tv_PtrItmReq->CmdTbl[Tv_PtrItmReq->CmdCnt]	=	E_SoloRv_IIC_CH(A_Ch) | E_SoloRv_IIC_WRITE;
			Tv_PtrItmReq->CmdCnt ++;

			//send subaddress - 2
			Tv_PtrItmReq->ValTbl[Tv_PtrItmReq->CmdCnt]	=	(__u8)(A_AdrSub & 0xFF);
			Tv_PtrItmReq->CmdTbl[Tv_PtrItmReq->CmdCnt]	=	E_SoloRv_IIC_CH(A_Ch) | E_SoloRv_IIC_WRITE;
			Tv_PtrItmReq->CmdCnt ++;

			//send read address
			Tv_PtrItmReq->ValTbl[Tv_PtrItmReq->CmdCnt]	=	A_AdrDev | 1;
			Tv_PtrItmReq->CmdTbl[Tv_PtrItmReq->CmdCnt]	=	E_SoloRv_IIC_CH(A_Ch) | E_SoloRv_IIC_WRITE | E_SoloRv_IIC_START;
			Tv_PtrItmReq->CmdCnt ++;

			//read work
			Tv_WkIdx	=	A_Size - 1;
			while ( 0 != Tv_WkIdx )
			{
				//send command
				Tv_PtrItmReq->CmdTbl[Tv_PtrItmReq->CmdCnt]	=	E_SoloRv_IIC_CH(A_Ch) | E_SoloRv_IIC_READ;
				Tv_PtrItmReq->CmdCnt ++;

				//next
				Tv_WkIdx --;
			}
			//send last byte
			Tv_PtrItmReq->CmdTbl[Tv_PtrItmReq->CmdCnt]	=	E_SoloRv_IIC_CH(A_Ch) | E_SoloRv_IIC_READ | E_SoloRv_IIC_STOP | E_SoloRv_IIC_ACK_EN;
			Tv_PtrItmReq->CmdCnt ++;

			//send request
			GrBufQueIn( Tv_PtrQueReq );
			Tv_Result	=	TRUE;
			//pump irq
			LcPumpIrq();
		}

		//critical section
		GrPrcCritLeave( &m_I2cCrit );

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDevSolo::GpioOutSet( __u32 A_Map )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDrvSoloGpioOut	Tv_Ioc;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Ioc.DevIdx		=	m_DevIdx;
		Tv_Ioc.Map			=	A_Map;
		if ( 0 <= ioctl( m_HndlDrv, E_GrDrvSoloIoctlGpioOutSet, &Tv_Ioc ) )
		{
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDevSolo::GpioOutClear( __u32 A_Map )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDrvSoloGpioOut	Tv_Ioc;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Ioc.DevIdx		=	m_DevIdx;
		Tv_Ioc.Map			=	A_Map;
		if ( 0 <= ioctl( m_HndlDrv, E_GrDrvSoloIoctlGpioOutClear, &Tv_Ioc ) )
		{
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDevSolo::GpioWrite( __u32 A_Set, __u32 A_Clear )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDrvSoloGpioWrite	Tv_Ioc;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Ioc.DevIdx		=	m_DevIdx;
		Tv_Ioc.Set			=	A_Set;
		Tv_Ioc.Clear		=	A_Clear;
		if ( 0 <= ioctl( m_HndlDrv, E_GrDrvSoloIoctlGpioWrite, &Tv_Ioc ) )
		{
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrDevSolo::GpioRead( void )
{
	// local -------------------
		__u32	Tv_Result;
		__u32	Tv_Ioc;
	// code --------------------
		Tv_Result	=	0;

		Tv_Ioc		=	m_DevIdx;
		if ( 0 <= ioctl( m_HndlDrv, E_GrDrvSoloIoctlGpioRead, &Tv_Ioc ) )
		{
			Tv_Result	=	Tv_Ioc;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDevSolo::LcPumpIrq( void )
{
	// local -------------------
		__u32	Tv_Ioc;
	// code --------------------
		//check av initialized
		if ( m_IsIrqPump )
		{
			Tv_Ioc		=	m_DevIdx;
			ioctl( m_HndlDrv, E_GrDrvSoloIoctlPumpIrq, &Tv_Ioc );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDevSolo::IrqPumpMode( BOOL8 A_IsRun )
{
		m_IsIrqPump	=	A_IsRun;
}
//--------------------------------------------------------------------
void	Cls_GrDevSolo::MseCusImgSet( __u32 A_Idx )
{
	// local -------------------
		St_GrDrvSoloMseCusImg	Tv_Ioc;
	// code --------------------
		Tv_Ioc.DevIdx		=	m_DevIdx;
		if ( E_GrDrvSoloMseCusCnt >= A_Idx )
		{
			Tv_Ioc.CusIdx		=	A_Idx;
			ioctl( m_HndlDrv, E_GrDrvSoloIoctlMseCusImg, &Tv_Ioc );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDevSolo::MseCusPosSet( __u16 A_X, __u16 A_Y )
{
	// local -------------------
		St_GrDrvSoloMseCusPos	Tv_Ioc;
	// code --------------------
		Tv_Ioc.DevIdx		=	m_DevIdx;
		Tv_Ioc.X				=	A_X;
		Tv_Ioc.Y				=	A_Y;
		ioctl( m_HndlDrv, E_GrDrvSoloIoctlMseCusPos, &Tv_Ioc );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDevSolo::QuadWinSet(	Ptr_GrWordRect A_PtrRcTbl, __u8* A_PtrSclTbl, __u8* A_PtrChTbl, 
																__u32 A_PipMap, __u8 A_Cnt, __u16* A_PtrBarPotTbl, __u32 A_BarMap, 
																__u32 A_LineMap, __u8 A_QuadMode, BOOL8 A_IsExp )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_Ioc;
		__u8	Tv_WkIdx;
		__u32	Tv_ChMask;
		__u32	Tv_Ch;
		__u32	Tv_Rval;
	// code --------------------
		Tv_Result	=	FALSE;

		//reset table
		for ( Tv_Ch=0;Tv_Ch < E_SoloMaxChCnt;Tv_Ch++ )
		{
			//reset value
			m_PtrCtrl->QuadRegTbl[Tv_Ch].WinCtrl0	=	E_SoloRv_VI_WIN_CHANNEL( Tv_Ch );
			m_PtrCtrl->QuadRegTbl[Tv_Ch].WinCtrl1	=	0;
			m_PtrCtrl->QuadRegTbl[Tv_Ch].LiveOn		=	0;
		}

		//build
		Tv_WkIdx	=	0;
		while ( Tv_WkIdx < A_Cnt )
		{
			Tv_Ch			=	(__u32)A_PtrChTbl[Tv_WkIdx];
			Tv_ChMask	=	1 << Tv_Ch;

			Tv_Rval	=	E_SoloRv_VI_WIN_CHANNEL( Tv_Ch )	|
								E_SoloRv_VI_WIN_SCALE( (__u32)A_PtrSclTbl[Tv_WkIdx] ) |
								E_SoloRv_VI_WIN_SX( ((__u32)A_PtrRcTbl[Tv_WkIdx].left) ) |
								E_SoloRv_VI_WIN_EX( ((__u32)A_PtrRcTbl[Tv_WkIdx].right) );
			if ( 0 != ( A_PipMap & Tv_ChMask ) )
			{
				Tv_Rval	=	Tv_Rval | E_SoloRv_VI_WIN_PIP;
			}
			m_PtrCtrl->QuadRegTbl[Tv_Ch].WinCtrl0	=	Tv_Rval;

			Tv_Rval		=	E_SoloRv_VI_WIN_SY( (__u32)A_PtrRcTbl[Tv_WkIdx].top ) |	
									E_SoloRv_VI_WIN_EY( (__u32)A_PtrRcTbl[Tv_WkIdx].bottom );
			m_PtrCtrl->QuadRegTbl[Tv_Ch].WinCtrl1	=	Tv_Rval;

			Tv_Rval		=	0;
			if ( E_SoloRv_VI_WIN_SclOff != A_PtrSclTbl[Tv_WkIdx] )
			{
				Tv_Rval	=	E_SoloRv_VI_LIVE_LIVE;
				if ( 0 != (m_PlayChMap & Tv_ChMask) )
				{
					Tv_Rval		=	E_SoloRv_VI_LIVE_PLAY;
				}
			}
			m_PtrCtrl->QuadRegTbl[Tv_Ch].LiveOn	=	Tv_Rval;

			//next
			Tv_WkIdx ++;
		}

		//bar position table copy
		m_PtrCtrl->QuadMode		=	A_QuadMode;
		m_PtrCtrl->IsQuadExp	=	A_IsExp;
		GrDumyCopyMem( m_PtrCtrl->QuadBarPosTbl, A_PtrBarPotTbl, E_SoloDispBarPosCnt << 2 );

		//update line bar map
		m_PtrCtrl->QuadLineMap	=	A_LineMap;
		m_PtrCtrl->QuadBarMap		=	A_BarMap;
		//DbgMsgPrint( "DAMGI - bar map = %08X\n", A_BarMap );

		//run ioctl
		Tv_Result	=	FALSE;
		Tv_Ioc	=	m_DevIdx;
		if ( 0 <= ioctl( m_HndlDrv, E_GrDrvSoloIoctlQuadSet, &Tv_Ioc ) )
		{
			Tv_Result	=	TRUE;
		}

		return	TRUE;
}
//--------------------------------------------------------------------
void*	Cls_GrDevSolo::RecBufPtrGet( void )
{
		return	m_PtrBufRec;
}
//--------------------------------------------------------------------
void	Cls_GrDevSolo::RecAtbSet( __u8 A_Ch, __u32 A_Atb )
{
		m_PtrCtrl->RecSchd[A_Ch]	=	A_Atb;
}
//--------------------------------------------------------------------
__u32	Cls_GrDevSolo::RecAtbGet( __u8 A_Ch )
{
		return	m_PtrCtrl->RecSchd[A_Ch];
}
//--------------------------------------------------------------------
__u32	Cls_GrDevSolo::AdoChCnt( void )
{
		return	m_AdoChCnt;
}
//--------------------------------------------------------------------
void	Cls_GrDevSolo::LcRecFinish( void )
{
		//not implemented
}
//--------------------------------------------------------------------
void	Cls_GrDevSolo::LcNetFinish( void )
{

}
//--------------------------------------------------------------------
BOOL8	Cls_GrDevSolo::RecChgPara( Ptr_GrDrvSoloRecCfg A_PtrCfg )
{
	// local -------------------
		BOOL8	Tv_Result;
		__s32		Tv_Rslt;
		__u32	Tv_Ioc;
	// code --------------------
		//init
		Tv_Result	=	FALSE;

		//update configuration
		GrDumyCopyMem( &m_PtrCtrl->CfgRec, A_PtrCfg, sizeof(St_GrDrvSoloRecCfg) );

		//patch index
		Tv_Ioc	=	m_DevIdx;
		Tv_Rslt	=	ioctl( m_HndlDrv, E_GrDrvSoloIoctlRecPara, &Tv_Ioc );
		if ( 0 <= Tv_Rslt )
		{
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDevSolo::RecConfig( Ptr_GrDrvSoloRecCfg A_PtrCfg )
{
	// local -------------------
		BOOL8	Tv_Result;
		__s32		Tv_Rslt;
		__u32	Tv_Ioc;
	// code --------------------
		//init
		Tv_Result	=	FALSE;

		//stop vrec config
		LcRecFinish();

		//update configuration
		GrDumyCopyMem( &m_PtrCtrl->CfgRec, A_PtrCfg, sizeof(St_GrDrvSoloRecCfg) );

		//patch index
		Tv_Ioc	=	m_DevIdx;
		Tv_Rslt	=	ioctl( m_HndlDrv, E_GrDrvSoloIoctlRecCfg, &Tv_Ioc );
		if ( 0 <= Tv_Rslt )
		{
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDevSolo::NetConfig( Ptr_GrDrvSoloNetCfg A_PtrCfg )
{
	// local -------------------
		BOOL8	Tv_Result;
		__s32		Tv_Rslt;
		__u32	Tv_Ioc;
	// code --------------------
		Tv_Result	=	FALSE;

		//stop net work config
		LcNetFinish();

		//update configuration
		GrDumyCopyMem( &m_PtrCtrl->CfgNet, A_PtrCfg, sizeof(St_GrDrvSoloNetCfg) );

		//request
		Tv_Ioc		=	m_DevIdx;
		Tv_Rslt		=	ioctl( m_HndlDrv, E_GrDrvSoloIoctlNetCfg, &Tv_Ioc );
		if ( 0 <= Tv_Rslt )
		{
			Tv_Result	=	TRUE;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDevSolo::TimeUpdt( void )
{
	// local -------------------
		__u32	Tv_Ioc;
	// code --------------------
		Tv_Ioc		=	m_DevIdx;
		ioctl( m_HndlDrv, E_GrDrvSoloIoctlCapTimeUpdt, &Tv_Ioc );
}
//--------------------------------------------------------------------
void	Cls_GrDevSolo::TimeSync( void )
{
	// local -------------------
		__u32	Tv_Ioc;
	// code --------------------
		Tv_Ioc		=	m_DevIdx;
		ioctl( m_HndlDrv, E_GrDrvSoloIoctlCapTimeSync, &Tv_Ioc );
}
//--------------------------------------------------------------------
void*	Cls_GrDevSolo::PlayBufPtrGet( void )
{
		return	m_PtrBufPlay;
}
//--------------------------------------------------------------------
__u32	Cls_GrDevSolo::PlayBufSizeGet( void )
{
		return	m_PtrCtrl->CfgBuf.Play.Size;
}
//--------------------------------------------------------------------
__u32	Cls_GrDevSolo::CovertMapGet( void )
{
		return	m_CovertMap;
}
//--------------------------------------------------------------------
void	Cls_GrDevSolo::CovertMapSet( __u32 A_Map )
{
		m_CovertMap	=	A_Map;
}
//--------------------------------------------------------------------
__u32	Cls_GrDevSolo::QuadViewChMapGet( void )
{
	// local -------------------
		__u32	Tv_Result;
		__u32	Tv_ChMask;
		__u32	Tv_WinIdx;
	// code --------------------
		Tv_Result	=	0;

		Tv_ChMask	=	1;
		for ( Tv_WinIdx=0;Tv_WinIdx < E_SoloMaxChCnt;Tv_WinIdx++ )
		{
			if ( 0 != m_PtrCtrl->QuadRegTbl[Tv_WinIdx].LiveOn )
			{
				Tv_Result	=	Tv_Result | Tv_ChMask;
			}
			Tv_ChMask	=	Tv_ChMask << 1;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDevSolo::PlayVdoData( Ptr_GrDvrAvHd A_PtrFrm, BOOL8 A_IsNoDisp )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_LbPosOut;
		__u32	Tv_LbPosIn;
		__u32	Tv_Size;
		void*	Tv_PtrCode;
		__u32	Tv_Idx0;
		__u32	Tv_Idx1;
		__u32	Tv_Idx2;
		__u32	Tv_VopType;
		__u32	Tv_Ch;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Size		=	(A_PtrFrm->DataSize + E_SoloLmemAlignUnit - 1) & E_SoloLmemAlignMask;

		Tv_LbPosIn	=	m_PtrCtrl->VdecLbPosIn;
		Tv_LbPosOut	=	m_PtrCtrl->VdecLbPosOut;		//backup pos out
		if ( Tv_LbPosIn < Tv_LbPosOut	)
		{
			if ( (Tv_Size + E_GrDevSoloVdecGap) < (Tv_LbPosOut - Tv_LbPosIn) )
			{
				Tv_Result	=	TRUE;
			}
		}
		else
		{
			if ( (Tv_Size + E_GrDevSoloVdecGap) < (E_GrDrvSoloLmemDecStrmSize - Tv_LbPosIn) )
			{
				Tv_Result	=	TRUE;
			}
			else if ( ( (Tv_Size + E_GrDevSoloVdecGap) == (E_GrDrvSoloLmemDecStrmSize - Tv_LbPosIn)) && (0 != Tv_LbPosOut) )
			{
				Tv_Result	=	TRUE;
			}
			else if ( (Tv_Size + E_GrDevSoloVdecGap) < Tv_LbPosOut )
			{
				Tv_LbPosIn	=	0;
				Tv_Result		=	TRUE;
			}
		}

		//check able
		if ( Tv_Result )
		{
			//request p2m
			Tv_VopType	=	(__u32)A_PtrFrm->Type;
			Tv_Ch				=	(__u32)A_PtrFrm->Ch;
			Tv_Idx0	=	E_SoloRv_VD_IDX_SIZE(A_PtrFrm->DataSize) | E_SoloRv_VD_IDX_VOP(Tv_VopType) |
								E_SoloRv_VD_IDX_CHANNEL(Tv_Ch);
			Tv_Idx1	=	V_GrDevSoloDecIdx1[A_PtrFrm->Res] | E_SoloRv_VD_IDX_WINDOW(Tv_Ch);
			Tv_Idx2	=	(E_GrDrvSoloLmemDecRefPos + (E_SoloDecRefUnit * Tv_Ch)) >> 16;
			//patch value
			if ( E_GrDvrVdoHighRes == (0x03 & A_PtrFrm->Res) )
			{
				Tv_Idx0	=	Tv_Idx0 | E_SoloRv_VD_IDX_INTERLACE;
				Tv_Idx1	=	Tv_Idx1 | m_PlayDintlVal;
			}
			if ( A_IsNoDisp )
			{
				Tv_Idx0	=	Tv_Idx0 | E_SoloRv_VD_IDX_NODISP;
			}

			Tv_Result		=	FALSE;
			Tv_PtrCode	=	(void*)( (__u32)A_PtrFrm + sizeof(St_GrDvrAvHd) );
			if ( P2mReqCopy(	E_GrDrvSoloP2mChPlayVdo, Tv_PtrCode, Tv_LbPosIn + E_GrDrvSoloLmemDecStrmPos, Tv_Size, 
												TRUE, E_GrDrvSoloP2mParaModeVdec, Tv_Idx0, Tv_Idx1, Tv_Idx2	) )
			{
				//update request count
				m_DecVreqId ++;
				//update decode position
				Tv_LbPosIn	=	Tv_LbPosIn + Tv_Size;
				if ( E_GrDrvSoloLmemDecStrmSize <= Tv_LbPosIn )
				{
					Tv_LbPosIn	=	0;
				}
				m_PtrCtrl->VdecLbPosIn	=	Tv_LbPosIn;

				//update
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDevSolo::IsVdoPal( void )
{
		return	m_PtrCtrl->CfgAv.IsVdoPal;
}
//--------------------------------------------------------------------
__u32	Cls_GrDevSolo::EncPowerGet( void )
{
	// local -------------------
		__u32	Tv_Result;
	// code --------------------
		Tv_Result	=	m_PtrCtrl->CfgAv.EncPwr;
		if ( m_Is6110 )
		{
			Tv_Result	=	E_Solo6110EncPwr;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrDevSolo::DecPowerGet( void )
{
		return	m_CodecPwr - EncPowerGet();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDevSolo::PlayAdoData( Ptr_GrDvrAvHd A_PtrFrm )
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrDrvSoloAdoPlayItm	Tv_PtrItm;
	// code --------------------
		Tv_Result	=	FALSE;

		//check exist buffer
		Tv_PtrItm	=	(Ptr_GrDrvSoloAdoPlayItm)GrBufQueGetInPtr( (Ptr_GrBufQue)m_PtrCtrl->AdecPlayQue );
		if ( NULL != Tv_PtrItm )
		{
			//calculate buffer position
			Tv_PtrItm->BufPos	=	(__u32)A_PtrFrm + sizeof(St_GrDvrAvHd) - (__u32)m_PtrDrvMem;
			//request count
			m_DecAreqId ++;

			//buffer in
			GrBufQueIn( (Ptr_GrBufQue)m_PtrCtrl->AdecPlayQue );
			//success
			Tv_Result	=	TRUE;
		}
		else
		{
			DbgMsgPrint( "Cls_GrDevSolo::PlayAdoData - request queue full\n" );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDevSolo::PlayAdoOob( void* A_PtrData )
{
	// local -------------------
		BOOL8	Tv_Result;
		void*	Tv_PtrBuf;
	// code --------------------
		Tv_Result	=	FALSE;

		//check exist buffer
		Tv_PtrBuf	=	(void*)GrBufQueGetInPtr( (Ptr_GrBufQue)m_PtrBufAdoDec );
		if ( NULL != Tv_PtrBuf )
		{
			//copy data
			GrDumyCopyMem( Tv_PtrBuf, A_PtrData, E_GrDrvSoloAdoClustSize );
			//buffer in
			GrBufQueIn( (Ptr_GrBufQue)m_PtrBufAdoDec );
			//success
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrDevSolo::PlayChMapGet( void )
{
		return	m_PlayChMap;
}
//--------------------------------------------------------------------
void	Cls_GrDevSolo::PlayChMapSet( __u32 A_ChMap )
{
		m_PlayChMap	=	A_ChMap;
}
//--------------------------------------------------------------------
void	Cls_GrDevSolo::PlayAtbSet( __u8 A_Ch, __u8 A_Atb )
{
		m_PlayAtb[A_Ch]	=	A_Atb;
}
//--------------------------------------------------------------------
__u8	Cls_GrDevSolo::PlayAtbGet( __u8 A_Ch )
{
		return	m_PlayAtb[A_Ch];
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDevSolo::UartSetup( __u8 A_Ch, __u32 A_BaudRate,	__u8 A_DataBit, __u8 A_StopBitMode, __u8 A_ParityMode )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_Radr;
		__u32	Tv_Rval;
		__u32	Tv_Div;
	// code --------------------
		Tv_Result	=	FALSE;

		//check channel
		if ( E_SoloUartChCnt > A_Ch )
		{
			//init
			Tv_Result		=	TRUE;
			Tv_Radr			=	E_SoloRa_UART_CTRL(A_Ch);
			//update baud rate
			switch( A_BaudRate )
			{
				case 1200:
					Tv_Rval		=	E_SoloRv_UART_CTRL_BAUD0(6) | E_SoloRv_UART_CTRL_BAUD1(3);
					Tv_Div		=	((((((m_PllMhz * 1000000) / 4) /337500 ) + 5) / 10) -1);
					break;
				case 2400:
					Tv_Rval		=	E_SoloRv_UART_CTRL_BAUD0(6) | E_SoloRv_UART_CTRL_BAUD1(2);
					Tv_Div		=	((((((m_PllMhz * 1000000) / 2) /337500 ) + 5) / 10) -1);
					break;
				case 4800:
					Tv_Rval		=	E_SoloRv_UART_CTRL_BAUD0(6) | E_SoloRv_UART_CTRL_BAUD1(1);
					Tv_Div		=	((((((m_PllMhz * 1000000) / 2) /337500 ) + 5) / 10) -1);
					break;
				case 9600:
					Tv_Rval		=	E_SoloRv_UART_CTRL_BAUD0(6) | E_SoloRv_UART_CTRL_BAUD1(0);
					Tv_Div		=	((((((m_PllMhz * 1000000) / 2) /337500 ) + 5) / 10) -1);
					break;
				case 19200:
					Tv_Rval		=	E_SoloRv_UART_CTRL_BAUD0(5) | E_SoloRv_UART_CTRL_BAUD1(0);
					Tv_Div		=	((((((m_PllMhz * 1000000) / 2) /337500 ) + 5) / 10) -1);
					break;
				case 38400:
					Tv_Rval		=	E_SoloRv_UART_CTRL_BAUD0(4) | E_SoloRv_UART_CTRL_BAUD1(0);
					Tv_Div		=	((((((m_PllMhz * 1000000) / 2) /337500 ) + 5) / 10) -1);
					break;
				case 57600:
					Tv_Rval		=	E_SoloRv_UART_CTRL_BAUD0(4) | E_SoloRv_UART_CTRL_BAUD1(0);
					Tv_Div		=	((((((m_PllMhz * 1000000) / 3) /337500 ) + 5) / 10) -1);
					break;
				case 115200:
					Tv_Rval		=	E_SoloRv_UART_CTRL_BAUD0(2) | E_SoloRv_UART_CTRL_BAUD1(0);
					Tv_Div		=	((((((m_PllMhz * 1000000) / 2) /337500 ) + 5) / 10) -1);
					break;
				case 230400:
					Tv_Rval		=	E_SoloRv_UART_CTRL_BAUD0(1) | E_SoloRv_UART_CTRL_BAUD1(0);
					Tv_Div		=	((((((m_PllMhz * 1000000) / 2) /337500 ) + 5) / 10) -1);
					break;
				default:
					Tv_Rval		=	E_SoloRv_UART_CTRL_BAUD0(5) | E_SoloRv_UART_CTRL_BAUD1(0);		//default = 9600 bps
					Tv_Div		=	((((((m_PllMhz * 1000000) / 2) /337500 ) + 5) / 10) -1);
					Tv_Result		=	FALSE;
					break;
			}
			//date bit
			switch( A_DataBit )
			{
				case 5:
					Tv_Rval	=	Tv_Rval | E_SoloRv_UART_CTRL_DATA(0);
					break;
				case 6:
					Tv_Rval	=	Tv_Rval | E_SoloRv_UART_CTRL_DATA(1);
					break;
				case 7:
					Tv_Rval	=	Tv_Rval | E_SoloRv_UART_CTRL_DATA(2);
					break;
				case 8:
					Tv_Rval	=	Tv_Rval | E_SoloRv_UART_CTRL_DATA(3);
					break;
				default:
					Tv_Rval	=	Tv_Rval | E_SoloRv_UART_CTRL_DATA(3);	//default = 8 bit
					Tv_Result	=	FALSE;
					break;
			}
			//stop bit
			Tv_Rval	=	Tv_Rval | E_SoloRv_UART_CTRL_STOP( (__u32)A_StopBitMode);
			//parity bit
			Tv_Rval	=	Tv_Rval | E_SoloRv_UART_CTRL_PARITY( (__u32)A_ParityMode);

			//system clock divider
			if ( !m_Is6110 )
			{
				Tv_Div	=	(((((m_PllMhz * 1000000) /337500 ) + 5) / 10) -1) / 2;
			}
			Tv_Rval	=	Tv_Rval | E_SoloRv_UART_CTRL_DIV(Tv_Div);		
			//enable module and interrupt
			Tv_Rval	=	Tv_Rval | E_SoloRv_UART_CTRL_TX_EN | E_SoloRv_UART_CTRL_RX_EN | E_SoloRv_UART_CTRL_RX_IRQ;
			//write register
			RegWrite( Tv_Radr, Tv_Rval );
			//success
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrDevSolo::UartWrite( __u8 A_Ch, void* A_PtrBuf, __u32 A_Size, BOOL8 A_IsWait )
{
	// local -------------------
		__u32	Tv_Result;
		__u8*	Tv_PtrBuf;
		__u8*	Tv_PtrQval;
	// code --------------------
		Tv_Result	=	0;

		//check channel
		if ( E_SoloUartChCnt > A_Ch )
		{
			//crit
			GrPrcCritEnter( &m_UartChTbl[A_Ch].CritWrite );

			Tv_PtrBuf		=	(__u8*)A_PtrBuf;
			while ( A_Size > Tv_Result )
			{
				Tv_PtrQval	=	(__u8*)GrBufQueGetInPtr( (Ptr_GrBufQue)m_PtrCtrl->UartQueSend[A_Ch] );
				if ( NULL == Tv_PtrQval )
				{
					if ( !A_IsWait )
					{
						break;
					}
					//wait data
					GrPrcSleep( E_GrDevSoloUartWait );
				}
				else
				{
					//write data
					*Tv_PtrQval	=	*Tv_PtrBuf;
					//go next
					Tv_PtrBuf ++;
					GrBufQueIn( (Ptr_GrBufQue)m_PtrCtrl->UartQueSend[A_Ch] );
					Tv_Result ++;
				}
			}

			//crit
			GrPrcCritLeave( &m_UartChTbl[A_Ch].CritWrite );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrDevSolo::UartRead( __u8 A_Ch, void* A_PtrBuf, __u32 A_Size, __u32 A_TimeOut )
{
	// local -------------------
		__u32	Tv_Result;
		__u8*	Tv_PtrBuf;
		__u8*	Tv_PtrQval;
		__u32	Tv_PrsTime;
	// code --------------------
		Tv_Result	=	0;

		//check channel
		if ( E_SoloUartChCnt > A_Ch )
		{
			//crit
			GrPrcCritEnter( &m_UartChTbl[A_Ch].CritRead );

			Tv_PrsTime	=	0;
			Tv_PtrBuf		=	(__u8*)A_PtrBuf;
			while ( A_Size > Tv_Result )
			{
				Tv_PtrQval	=	(__u8*)GrBufQueGetOutPtr( (Ptr_GrBufQue)m_PtrCtrl->UartQueRecv[A_Ch] );
				if ( NULL == Tv_PtrQval )
				{
					if ( Tv_PrsTime >= A_TimeOut )
					{
						break;
					}
					//wait data
					GrPrcSleep( E_GrDevSoloUartWait );
					Tv_PrsTime	=	Tv_PrsTime + E_GrDevSoloUartWait;
				}
				else
				{
					//read data
					*Tv_PtrBuf	=	*Tv_PtrQval;
					//go next
					Tv_PtrBuf ++;
					GrBufQueOut( (Ptr_GrBufQue)m_PtrCtrl->UartQueRecv[A_Ch] );
					Tv_Result ++;
				}
			}

			//crit
			GrPrcCritLeave( &m_UartChTbl[A_Ch].CritRead );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDevSolo::OsgAlphaStrengthSet( __u32 A_Strength )
{
	// local -------------------
		__u32	Tv_Rval;
	// code --------------------
		if ( 64 > A_Strength )
		{
			Tv_Rval	=	E_SoloRv_VO_OSG_ON | E_SoloRv_VO_OSG_BASE(E_GrDrvSoloLmemOsgPos) | 
								E_SoloRv_VO_OSG_ALPHA_RATE( A_Strength ) | E_SoloRv_VO_OSG_ALPHA_BG_RATE( 64 - A_Strength );
			RegWrite( E_SoloRa_OSG_CONFIG, Tv_Rval );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDevSolo::MotionInit( void )
{
	// local -------------------
	// code --------------------
		// flag buffer clear
		GrDumyZeroMem( m_PtrBufMtnCfg, 0x20000 );
		P2mReqCopy( E_GrDrvSoloP2mChMtnCfg, m_PtrBufMtnCfg, E_GrDrvSoloLmemMotionPos, 0x20000, 
			TRUE, E_GrDrvSoloP2mParaModeNone, 0, 0, 0 );
		P2mReqCopy( E_GrDrvSoloP2mChMtnCfg, m_PtrBufMtnCfg, E_GrDrvSoloLmemMotionPos + 0x20000, 0x20000, 
			TRUE, E_GrDrvSoloP2mParaModeNone, 0, 0, 0 );
		P2mReqCopy( E_GrDrvSoloP2mChMtnCfg, m_PtrBufMtnCfg, E_GrDrvSoloLmemMotionPos + 0x40000, 0x04000, 
			TRUE, E_GrDrvSoloP2mParaModeNone, 0, 0, 0 );
}
//--------------------------------------------------------------------
void	Cls_GrDevSolo::MotionStart( void )
{
	// local -------------------
		__u32		Tv_Rval;
	// code --------------------
		Tv_Rval		=	E_SoloRv_VI_MOTION_EN(0xFFFF) | (E_GrDrvSoloLmemMotionPos >> 16);
		RegWrite( E_SoloRa_VI_MOT_ADR, Tv_Rval );
		/*
		Tv_Rval		=	E_SoloRv_VI_MOTION_FRAME_COUNT(3) | E_SoloRv_VI_MOTION_SAMPLE_LENGTH(E_SoloBaseResX >> 4) |
			E_SoloRv_VI_MOTION_SAMPLE_COUNT(1);
		RegWrite( E_SoloRa_VI_MOT_CTRL, Tv_Rval );
		*/
		//motion display off
		// RegWrite( A_DevIdx, E_SoloRa_VI_MOTION_BORDER, 0 );
		// RegWrite( A_DevIdx, E_SoloRa_VI_MOTION_BAR, 0 );

		
}
//--------------------------------------------------------------------
__u32 Cls_GrDevSolo::MotionOcuMapGet( void )
{
	// var --------------------
		__u32			Tv_Result;
	// code -------------------
		Tv_Result	=	m_DevIdx;
		if ( 0 > ioctl( m_HndlDrv, E_GrDrvSoloIoctlMtnStatGet, &Tv_Result ) )
		{
			Tv_Result	=	0;
		}
		Tv_Result	=	Tv_Result & m_VdoChMask;
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDevSolo::MotionSenseSet( __u8 A_Ch, __u16 A_Sense )
{
	// local -------------------
		__u16*	Tv_PtrTrhd;
		__u32	Tv_LmemAdr;
		__u32	Tv_WkX;
		__u32	Tv_WkY;
		__u32	Tv_TrhdIdx;
	// code --------------------
		//check change sensitive
		if ( A_Sense != m_MtnSenTbl[A_Ch] )
		{
			Tv_LmemAdr	=	E_GrDrvSoloLmemMotionPos + ( ((__u32)A_Ch + 1) << 14 );
			Tv_PtrTrhd	=	(__u16*)( (__u32)m_PtrBufMtnCfg + ((__u32)A_Ch << 13) );

			//change sense
			m_MtnSenTbl[A_Ch]	=	A_Sense;

			Tv_WkY	=	0;
			while ( E_GrDevSoloMtnThrdMaxY > Tv_WkY )
			{
				Tv_WkX	=	0;
				while ( E_GrDevSoloMtnThrdMaxX > Tv_WkX )
				{
					//patch value
					//write value
					Tv_TrhdIdx	=	(Tv_WkY << 6) + Tv_WkX;
					if ( 0xFFFF != Tv_PtrTrhd[Tv_TrhdIdx] )
					{
						Tv_PtrTrhd[Tv_TrhdIdx]	=	A_Sense;
					}
					//next
					Tv_WkX	=	Tv_WkX + 1;
				}
				//next
				Tv_WkY	=	Tv_WkY + 1;
			}
			//write area
			P2mReqCopy( E_GrDrvSoloP2mChMtnCfg, Tv_PtrTrhd, Tv_LmemAdr, E_GrDevSoloMtnCfgSize, 
				TRUE, E_GrDrvSoloP2mParaModeNone, 0, 0, 0 );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDevSolo::MotionChEnableSet( __u32 A_ChMap )
{
	// local -------------------
		__u32		Tv_Rval;
	// code --------------------
		Tv_Rval		=	E_SoloRv_VI_MOTION_EN(A_ChMap) | (E_GrDrvSoloLmemMotionPos >> 16);
		RegWrite( E_SoloRa_VI_MOT_ADR, Tv_Rval );
}
//--------------------------------------------------------------------
void	Cls_GrDevSolo::MotionSenseMapSet( __u8 A_Ch, void* A_PtrMap )
{
	// local -------------------
		__u8*	Tv_PtrMap;
		__u8	Tv_BitMask;
		__u16*	Tv_PtrTrhd;
		__u32	Tv_LmemAdr;
		__u32	Tv_WkX;
		__u32	Tv_WkY;
		__u16	Tv_Val;
		__u32	Tv_TrhdIdx;
	// code --------------------
		Tv_PtrMap		=	(__u8*)A_PtrMap;
		Tv_BitMask	=	1;
		Tv_LmemAdr	=	E_GrDrvSoloLmemMotionPos + ( ((__u32)A_Ch + 1) << 14 );
		Tv_PtrTrhd	=	(__u16*)( (__u32)m_PtrBufMtnCfg + ((__u32)A_Ch << 13) );

		Tv_WkY	=	0;
		while ( E_GrDevSoloMtnThrdMaxY > Tv_WkY )
		{
			Tv_WkX	=	0;
			while ( E_GrDevSoloMtnThrdMaxX > Tv_WkX )
			{
				//get value
				Tv_Val	=	0xFFFF;
				if ( 0 != ( Tv_BitMask & (*Tv_PtrMap) ) )
				{
					Tv_Val	=	m_MtnSenTbl[A_Ch];
				}
				//write value
				Tv_TrhdIdx	=	(Tv_WkY << 6) + Tv_WkX;
				Tv_PtrTrhd[Tv_TrhdIdx]	=	Tv_Val;
				Tv_PtrTrhd[Tv_TrhdIdx + 1]	=	Tv_Val;
				Tv_PtrTrhd[Tv_TrhdIdx + E_GrDevSoloMtnThrdLine]	=	Tv_Val;
				Tv_PtrTrhd[Tv_TrhdIdx + E_GrDevSoloMtnThrdLine + 1]	=	Tv_Val;

				//next
				Tv_WkX	=	Tv_WkX + 2;
				if ( 0x80 == Tv_BitMask )
				{
					Tv_BitMask	=	0x01;
					Tv_PtrMap ++;
				}
				else
				{
					Tv_BitMask	=	Tv_BitMask << 1;
				}
			}
			//next
			Tv_WkY	=	Tv_WkY + 2;
		}
		//write area
		P2mReqCopy( E_GrDrvSoloP2mChMtnCfg, Tv_PtrTrhd, Tv_LmemAdr, E_GrDevSoloMtnCfgSize, 
			TRUE, E_GrDrvSoloP2mParaModeNone, 0, 0, 0 );
}
//--------------------------------------------------------------------
void	Cls_GrDevSolo::MotionSenseMapAndSenseSet( __u8 A_Ch, void* A_PtrMap, __u16 A_Sense )
{
		m_MtnSenTbl[A_Ch]	=	A_Sense;
		MotionSenseMapSet( A_Ch, A_PtrMap );
}
//--------------------------------------------------------------------
void	Cls_GrDevSolo::MotionDisplaySet( BOOL8 A_IsOn )
{
	// local -------------------
		__u32	Tv_Rval;
	// code --------------------
		Tv_Rval	=	0x00000040;
		if ( A_IsOn )
		{
			Tv_Rval	=	Tv_Rval | (1 << 24);
		}
		RegWrite( E_SoloRa_VI_MOTION_BAR, Tv_Rval );
}
//--------------------------------------------------------------------
__u32	Cls_GrDevSolo::PlayVdoRemainCntGet( void )
{
		return	(m_DecVreqId - m_PtrCtrl->DecVprsId);
}
//--------------------------------------------------------------------
__u32	Cls_GrDevSolo::PlayAdoRemainCntGet( void )
{
		return	(m_DecAreqId - m_PtrCtrl->DecAprsId);
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDevSolo::IsMtnMapGet( void )
{
		return	m_IsMtnMap;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDevSolo::Is6110( void )
{
		return	m_Is6110;
}
//--------------------------------------------------------------------
void	Cls_GrDevSolo::EncMtnMapSensSet( __u32 A_Sense )
{
	// local -------------------
		__u32	Tv_Rval;
		__u32	Tv_WkIdx;
	// code --------------------
		if ( m_Is6110 )
		{
			Tv_Rval	=	(0xFF & A_Sense) | E_SoloRv_VE_MTNMAPCFG_Base;
			RegWrite( E_SoloRa_VE_MTNMAPCFG, Tv_Rval );
		}
		else
		{
			Tv_Rval	=	(0xFF & A_Sense) << 4;
			for ( Tv_WkIdx=0;Tv_WkIdx < m_VdoChCnt;Tv_WkIdx++ )
			{
				RegWrite( E_SoloRa_VE_CH_MOT(Tv_WkIdx), Tv_Rval );
			}
		}
}
//--------------------------------------------------------------------
/*
__u32	Cls_GrDevSolo::RecMotionMapGet( void )
{
		return	m_RecMtnMap;
}
//--------------------------------------------------------------------
void	Cls_GrDevSolo::RecMotionMapSet( __u32 A_Map )
{
		m_RecMtnMap	=	A_Map;
}
//--------------------------------------------------------------------
__u32	Cls_GrDevSolo::RecSensorMapGet( void )
{
		return	m_RecDiMap;
}
//--------------------------------------------------------------------
void	Cls_GrDevSolo::RecSensorMapSet( __u32 A_Map )
{
		m_RecDiMap	=	A_Map;
}
*/
//--------------------------------------------------------------------
__u32	Cls_GrDevSolo::IrqOcuCountGet( void )
{
		//((Ptr_GrBufQue)m_PtrCtrl->P2mQue[0])->InPos
		return	m_PtrCtrl->IrqOcuCnt;
}
//--------------------------------------------------------------------
void	Cls_GrDevSolo::DispErase( BOOL8 A_IsErase )
{
	// local -------------------
		__u32	Tv_Rval;
	// code --------------------
		Tv_Rval	=	0;
		if ( A_IsErase )
		{
			Tv_Rval	=	E_SoloRv_VO_DISP_ERASE_ON;
		}
		RegWrite( E_SoloRa_VO_DISP_ERASE, Tv_Rval );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDevSolo::IsPlayDeinterlace( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( 0 != m_PlayDintlVal )
		{
			Tv_Result	=	TRUE;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDevSolo::PlayDeinterlaceSet( BOOL8 A_IsSet )
{
		if ( A_IsSet )
		{
			m_PlayDintlVal	=	E_SoloRv_VD_IDX_DEINTERLACE;
		}
		else
		{
			m_PlayDintlVal	=	0;
		}
}
//--------------------------------------------------------------------
void*	Cls_GrDevSolo::NetManagerPtrGet( void )
{
		return	(void*)m_PtrCtrl->NetMng;
}
//--------------------------------------------------------------------
void*	Cls_GrDevSolo::NetLiveAdoBufPtrGet( void )
{
		return	m_PtrBufNetAdo;
}
//--------------------------------------------------------------------
void*	Cls_GrDevSolo::NetLiveMpgBufPtrGet( void )
{
		return	m_PtrBufNetMpg;
}
//--------------------------------------------------------------------
void*	Cls_GrDevSolo::PosSysToPtr( __u32 A_PosSys )
{
		return	(void*)( (__u32)m_PtrDrvMng + A_PosSys );
}
//--------------------------------------------------------------------
void	Cls_GrDevSolo::NosigMapSet( __u16 A_Map )
{
		m_PtrCtrl->NosigMap	=	A_Map;
}
//--------------------------------------------------------------------
void*	Cls_GrDevSolo::NetLiveJpgBufPtrGet( void )
{
		return	m_PtrBufNetJpg;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDevSolo::IsPlaying( void )
{
		return	m_IsPlaying;
}
//--------------------------------------------------------------------
void	Cls_GrDevSolo::PlayingSet( BOOL8 A_IsSet )
{
		m_IsPlaying	=	A_IsSet;
}
//--------------------------------------------------------------------
void	Cls_GrDevSolo::CapBtwReset( void )
{
	// local -------------------
		__u32	Tv_Ioc;
	// code --------------------
		Tv_Ioc	=	m_DevIdx;
		if ( 0 > ioctl( m_HndlDrv, E_GrDrvSoloIoctlCapBtwRst, &Tv_Ioc ) )
		{
			DbgMsgPrint("Cls_GrDevSolo::CapBtwReset - ioctl failure!\n");
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDevSolo::IsEncChAble( __u8 A_Ch )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( m_PtrCtrl->CfgRec.IsRec && (0 != m_PtrCtrl->CfgRec.Vdo.ChTbl[A_Ch].Scl) )
		{
			Tv_Result	=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrDevSolo::VencWatchdogCount( void )
{
		return	m_PtrCtrl->EncMpgWtc;
}
//--------------------------------------------------------------------
void	Cls_GrDevSolo::VencWatchdogReset( void )
{
		m_PtrCtrl->EncMpgWtc	=	0;	
}
//--------------------------------------------------------------------
void	Cls_GrDevSolo::WaterMarkSet( BOOL8 A_IsWmrk )
{
	// local -------------------
		St_GrDrvSoloIsWmrk	Tv_Ioc;
	// code --------------------
		Tv_Ioc.DevIdx		=	m_DevIdx;
		Tv_Ioc.IsWmrk		=	A_IsWmrk;
		ioctl( m_HndlDrv, E_GrDrvSoloIoctlWmrkSet, &Tv_Ioc );
}
//--------------------------------------------------------------------
void	Cls_GrDevSolo::DigitZoomMove( __u16 A_X, __u16 A_Y )
{
	// local -------------------
		St_GrDrvSoloZoomPos	Tv_Ioc;
	// code --------------------
		Tv_Ioc.DevIdx		=	m_DevIdx;
		Tv_Ioc.ZoomX		=	A_X;
		Tv_Ioc.ZoomY		=	A_Y;
		ioctl( m_HndlDrv, E_GrDrvSoloIoctlZoomMove, &Tv_Ioc );
}
//--------------------------------------------------------------------
void	Cls_GrDevSolo::DigitZoomOff()
{
	// local -------------------
		__u32		Tv_Ioc;
	// code --------------------
		Tv_Ioc		=	m_DevIdx;
		ioctl( m_HndlDrv, E_GrDrvSoloIoctlZoomOff, &Tv_Ioc );
}
//--------------------------------------------------------------------
void	Cls_GrDevSolo::MosaicValueSet( __u32 A_MosIdx, __u8 A_Ch, __u8 A_Brgt )
{
	// local -------------------
	// code --------------------
		RegWrite( E_SoloRa_VI_MOSAIC(A_Ch), A_MosIdx );
		RegWrite( E_SoloRa_VI_FMT_CFG, (A_Brgt << 22) );
}//new-120905~mosaic
////--------------------------------------------------------------------
__u32	Cls_GrDevSolo::MosaicValueGet( __u8 A_Ch )
{
	// local -------------------
		__u32				Tv_MosIdx;
	// code --------------------
		Tv_MosIdx = RegRead( E_SoloRa_VI_MOSAIC(A_Ch) );
		return Tv_MosIdx;
}//new-120905~mosaic
////--------------------------------------------------------------------
__u32	Cls_GrDevSolo::MosaicBrightValueGet( __u8 A_Ch )
{
	// local -------------------
		__u32				Tv_Brgt;
	// code --------------------
		Tv_Brgt = RegRead( E_SoloRa_VI_FMT_CFG );
		return Tv_Brgt;
}//new-120905~mosaic
//--------------------------------------------------------------------
void*	Cls_GrDevSolo::DevCtrlPtrGet( void )
{
		return	(void*)m_PtrCtrl;
}
//--------------------------------------------------------------------
void*	Cls_GrDevSolo::DrvMemPtrGet( void )
{
		return	m_PtrDrvMem;
}
//--------------------------------------------------------------------
