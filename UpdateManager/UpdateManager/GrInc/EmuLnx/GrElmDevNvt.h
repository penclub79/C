/*
	emulation linux module Hi3531
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrElmDevNvt
#define	_EPC_GrElmDevNvt

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

#include <EmuLnx/GrElaBase.h>

#include	<EmuLnx/GrElmDevBase.h>

#include	<GrProcess.h>
#include	<GrGdibObj.h>
#include	<Dvr/GrDvrBase.h>

#include "hd_type.h"
#include "hd_common.h"
#include "hd_gfx.h"
#include "hd_util.h"
#include "hd_logger.h"
#include "hd_debug.h"
#include "hd_audiocapture.h"
#include "hd_audiodec.h"
#include "hd_audioenc.h"
#include "hd_audioout.h"
#include "hd_videocapture.h"
#include "hd_videodec.h"
#include "hd_videoenc.h"
#include "hd_videoout.h"
#include "hd_videoprocess.h"
#include "hdal.h"
#include "vendor_videoenc.h"
#include "vendor_video.h"

//====================================================================
//constance

#define E_GrElmDevNvtFbSize				0x800000
#define E_GrElmDevNvtFbCnt				7

#define E_GrElmDevHiMpiAvBufSize	0x800000

#define E_GrElmDevNvtVdoGetWait		6
#define E_GrElmDevNvtVdoGetCnt		2

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

class	Cls_GrElmDevNvt	:	public Cls_GrElmDevBase
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
		Cls_GrElmDevNvt( DWORD A_DevIdx, BYTE A_FbIdx );
		virtual	~Cls_GrElmDevNvt();
		
		void*	Mmap( DWORD A_Offset, DWORD A_Size );


};


//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif
