/*
	emulation linux module Hi3531A
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrElmDevHiV3
#define	_EPC_GrElmDevHiV3

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

#include <EmuLnx/GrElaBase.h>

#include	<EmuLnx/GrElmDevBase.h>

#include	<GrProcess.h>
#include	<GrGdibObj.h>
#include	<Dvr/GrDvrBase.h>

#include "hi_common.h"
#include "hi_comm_sys.h"
#include "hi_comm_vb.h"
#include "hi_comm_vi.h"
#include "hi_comm_vo.h"
#include "hi_comm_venc.h"
#include "hi_comm_vpss.h"
#include "hi_comm_vdec.h"
#include "hi_comm_vda.h"
#include "hi_comm_region.h"
#include "hi_comm_adec.h"
#include "hi_comm_aenc.h"
#include "hi_comm_ai.h"
#include "hi_comm_ao.h"
#include "hi_comm_aio.h"
#include "hi_comm_hdmi.h"
#include "hi_comm_pciv.h"
#include "hi_defines.h"

#include "mpi_sys.h"
#include "mpi_vb.h"
#include "mpi_vi.h"
#include "mpi_vo.h"
#include "mpi_venc.h"
#include "mpi_vpss.h"
#include "mpi_vdec.h"
#include "mpi_vda.h"
#include "mpi_region.h"
#include "mpi_adec.h"
#include "mpi_aenc.h"
#include "mpi_ai.h"
#include "mpi_ao.h"
#include "mpi_hdmi.h"
#include "mpi_pciv.h"


//====================================================================
//constance

#define E_GrElmDevHiV3FbSize			0x800000
#define E_GrElmDevHiV3FbCnt				7

#define E_GrElmDevHiV3AvBufSize		0x800000

#define E_GrElmDevHiV3VdoGetWait	6
#define E_GrElmDevHiV3VdoGetCnt	2

//====================================================================
//global macro


//====================================================================
//global type

typedef	struct St_GrElmDevHiV3VencChItm
{
	BOOL8	IsOn;
	DWORD	Prgs;
	DWORD	Lpos;
}	*Ptr_GrElmDevHiV3VencChItm;

typedef struct St_GrElmDevHiV3FrmHd
{
	DWORD	Fcc;				// FCC code
	BYTE	Ch;
	BYTE	Type;
	BOOL8	IsSm;
	BYTE	Evt;				// record event
	__u32	TimeSec;		// time sec
	DWORD	DataSize;
	__u32	TimeMsec;		// time milliseconds - micro sec
	BYTE	Res;
	BYTE	SkipCnt;		// record speed - skip frame count, bit[7]  0:skiip count, 1:fps
	BYTE	VopId;			// H only
	BYTE	SeqId;			// frame sequence id
	DWORD	Wtmk;				// water mark - JPEG : QP index
	BYTE	Mode;				// menu mode
	BYTE	ReqId;			// request id
	BYTE	IsNosig;		// bit 0 = no signal - network live only
	BYTE	_rsvd1;			// reserved 1
}	*Ptr_GrElmDevHiV3FrmHd;

//====================================================================
//class

class	Cls_GrElmDevHiV3	:	public Cls_GrElmDevBase
{
	private:
		BYTE		m_FbIdx;
		int			m_Node;
		void*		m_PtrFb;		// frame buffer 

		BYTE		m_EmuVdoBuf[E_GrElmDevHiV3AvBufSize];	
		DWORD		m_EmuVdoCnt;
		DWORD		m_EmuVdoWait;

		St_GrElmDevHiV3VencChItm	m_VencTbl[E_GrDvrMaxChCnt];
		BYTE		m_VencCh;

	protected:
		void	LcEmuAvLoad( void );

	public:
		Cls_GrElmDevHiV3( DWORD A_DevIdx, BYTE A_FbIdx );
		virtual	~Cls_GrElmDevHiV3();
		
		void*	Mmap( DWORD A_Offset, DWORD A_Size );
};

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif
