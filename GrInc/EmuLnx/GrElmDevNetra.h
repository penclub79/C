/*
	emulation linux module Netra
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrElmDevNetra
#define	_EPC_GrElmDevNetra

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

#include	<Hw/Solo/RegSolo.h>

#include	<EmuLnx/GrElmDevBase.h>

#include	<GrProcess.h>
#include	<GrGdibObj.h>
#include	<Dvr/GrDvrBase.h>

extern	"C"
{
#include	<ti_media_std.h>
#include	<ti_media_error_def.h>
#include	<ti_vsys.h>
#include 	<ti_vcap.h>
#include 	<ti_venc.h>
#include 	<ti_vdec.h>
#include 	<ti_vdis.h>
};

//====================================================================
//constance

#define E_GrElmDevNetraFbSize			0x800000

#define E_GrElmDevNetraAvBufSize	0x800000

#define E_GrElmDevNetraVdoGetWait	2
#define E_GrElmDevNetraVdoGetCnt	32
//====================================================================
//global macro


//====================================================================
//global type

typedef	struct St_GrElmDevNetraVencChItm
{
	BOOL8	IsOn;
	DWORD	Prgs;
	DWORD	Lpos;
}	*Ptr_GrElmDevNetraVencChItm;


//====================================================================
//class

class	Cls_GrElmDevNetra	:	public Cls_GrElmDevBase
{
	private:
		BYTE		m_FbIdx;
		int			m_Node;
		void*		m_PtrFb;		// frame buffer 

		BYTE		m_EmuVdoBuf[E_GrElmDevNetraAvBufSize];	
		DWORD		m_EmuVdoCnt;
		DWORD		m_EmuVdoWait;

		St_GrElmDevNetraVencChItm	m_VencTbl[E_GrDvrMaxChCnt];
		BYTE		m_VencCh;

	protected:
		void	LcEmuAvLoad( void );
		BOOL8	LcVencStrmIn( BYTE A_Ch, VCODEC_BITSBUF_S* A_PtrItm );

	public:
		Cls_GrElmDevNetra( DWORD A_DevIdx, BYTE A_FbIdx );
		virtual	~Cls_GrElmDevNetra();
		
		void*	Mmap( DWORD A_Offset, DWORD A_Size );

		BOOL8	StreamGet( VCODEC_BITSBUF_LIST_S* A_PtrBlst );

};


//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif
