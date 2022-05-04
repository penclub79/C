/*
	DvrCtrlPrtc
*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrDvrIoCliCtrl
#define	_EPC_GrDvrIoCliCtrl

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrTaskSvr.h>
#include <Dvr/GrDvrBase.h>
#include <Dvr/Io/GrDvrIoCliBase.h>
	
//====================================================================
//constance

#define E_GrDvrCtrlBufSize					8
#define E_GrDvrCtrlPrtcStx					0x47
#define E_GrDvrCtrlPrtcEtx					0x56

//====================================================================
//global macro

//====================================================================
//global type

//====================================================================
//class

class	Cls_GrDvrIoCliCtrl : public Cls_GrDvrIoCliBase
{
	private:
		__u8	m_RecvBuf[E_GrDvrCtrlBufSize];
		__u8	m_RecvIn;
		__u8	m_UartCh;
		__u8	m_PkData[E_GrDvrCtrlBufSize];
		__u8	m_PkDidx;
		__u8	m_PkCmd;
		__u8	m_PkData1;
		__u8	m_PkDataSz;
		BOOL8	m_IsRevd;
		__u32	m_HndlTimer;	
	
	protected:		
		
	public:
    Cls_GrDvrIoCliCtrl( __u8 A_UartCh, __u8 A_Id );
    virtual		~Cls_GrDvrIoCliCtrl();
		virtual	void	RtlTimer( void );
		virtual	BOOL8	RtlProc( void );
};	

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif


