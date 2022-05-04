/*
	emulation linux module solo
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrElmDevSolo
#define	_EPC_GrElmDevSolo

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

#include	<Hw/Solo/RegSolo.h>

#include	<EmuLnx/GrElmDevBase.h>

#include	<GrProcess.h>
#include	<GrGdibObj.h>
#include	<Dvr/GrDvrBase.h>

//====================================================================
//constance
#define E_GrElmDevSoloMaxDevCnt			2		/* solo device max count */
#define E_GrElmDevSoloPntQueSize		0x4000

#define E_GrElmDevSoloEmuVdoSize		0x800000

//====================================================================
//global macro


//====================================================================
//global type

//====================================================================
//class

class	Cls_GrElmDevSolo	:	public Cls_GrElmDevBase
{
	private:
		DWORD		m_DevIdx;			//device index
		BOOL		m_Is6110;			//solo 6110
		void*		m_PtrReg;			//register area
		void*		m_PtrLmem;		//local memory
		DWORD		m_LdevIdx;		//local device index
		St_GrPrcThrd	m_HndlThrd;		//thread handle
		BOOL		m_IsRun;			//runngin driver
		BOOL		m_IsIrqReq;		//interrupt request

		DWORD		m_I2cIrqed;		//i2c interrupted

		//osg
		Cls_GrGdib*	m_GdibOsg;	//osg gdib
		Cls_GrGdib*	m_GdibDisp;	//display gdib

		//mpeg encode
		DWORD		m_EncBufSize;
		void*		m_PtrEncBuf;
		DWORD		m_EncAddPos;
		BYTE		m_EncGop[E_SoloMaxChCnt];
		BYTE		m_EncScl[E_SoloMaxChCnt];
		DWORD		m_FrmIdx[E_SoloMaxChCnt];
		Ptr_GrDvrAvHd	m_PtrFrm[E_SoloMaxChCnt];
		DWORD		m_EncCh;
		DWORD		m_EncQueIdx;
		DWORD		m_EncPeriod;

		DWORD		m_G723Period;

		//emubuf
		BYTE		m_EmuVdoBuf[E_GrElmDevSoloEmuVdoSize];	
		DWORD		m_EmuVdoCnt;

	protected:
		//local 
		void		LcRegInit( void );		//register init
		void		LcLmemInit( void );		//local memory init
		void		LcPaintReq( Ptr_GrRect A_PtrRect );		//request paint
		void		LcP2mWkOsg( DWORD A_OsgOfs, DWORD A_TrnsSize );
		void		LcEmuAvLoad( void );

		//emulate write register
		void		LcEwrP2mCtrl( DWORD A_Reg, DWORD A_Val );		//P2mControl register
		void		LcEwrVdec( void );

		//thread work
		BOOL		LcThrdP2m( DWORD A_P2mCh );
		BOOL		LcThrdIrq( void );
		BOOL		LcThrdI2c( void );
		BOOL		LcThrdAdo( void );
		BOOL		LcThrdVenc( void );

	public:
		Cls_GrElmDevSolo( DWORD A_DevIdx, BOOL A_Is6110 );
		virtual	~Cls_GrElmDevSolo();
		
		//misc
		void*		LmemPtrGet( DWORD A_Pos );			//local memory pointer get

		//inherited
		virtual	BOOL	MioWriteDword( void* A_Ptr, DWORD A_Val );
		virtual	BOOL	MioReadDword( void* A_Ptr, DWORD* A_PtrRtVal );

		//runtime
		void		RtlThrdProc( void );
		
};


//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif
