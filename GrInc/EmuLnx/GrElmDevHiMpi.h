/*
	emulation linux module Hi3531
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrElmDevHiMpi
#define	_EPC_GrElmDevHiMpi

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



//====================================================================
//constance

#define E_GrElmDevHiMpiFbSize			0x800000
#define E_GrElmDevHiMpiFbCnt				7

#define E_GrElmDevHiMpiAvBufSize		0x800000

#define E_GrElmDevHiMpiVdoGetWait	6
#define E_GrElmDevHiMpiVdoGetCnt	2

//====================================================================
//global macro


//====================================================================
//global type

typedef	struct St_GrElmDevHiMpiVencChItm
{
	BOOL8	IsOn;
	DWORD	Prgs;
	DWORD	Lpos;
}	*Ptr_GrElmDevHiMpiVencChItm;

typedef struct St_GrElmDevHiMpiFrmHd
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
}	*Ptr_GrElmDevHiMpiFrmHd;

//====================================================================
//class

class	Cls_GrElmDevHiMpi	:	public Cls_GrElmDevBase
{
	private:
		BYTE		m_FbIdx;
		int			m_Node;
		void*		m_PtrFb;		// frame buffer 

		BYTE		m_EmuVdoBuf[E_GrElmDevHiMpiAvBufSize];	
		DWORD		m_EmuVdoCnt;
		DWORD		m_EmuVdoWait;

		St_GrElmDevHiMpiVencChItm	m_VencTbl[E_GrDvrMaxChCnt];
		BYTE		m_VencCh;

	protected:
		void	LcEmuAvLoad( void );

	public:
		Cls_GrElmDevHiMpi( DWORD A_DevIdx, BYTE A_FbIdx );
		virtual	~Cls_GrElmDevHiMpi();
		
		void*	Mmap( DWORD A_Offset, DWORD A_Size );


};


//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif
