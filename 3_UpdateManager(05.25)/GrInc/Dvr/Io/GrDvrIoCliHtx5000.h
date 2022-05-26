/*
	dvr io client Htx5000 keyboard

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrDvrIoCliHtx5000
#define	_EPC_GrDvrIoCliHtx5000

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrTaskSvr.h>
#include <Dvr/GrDvrBase.h>
#include <Dvr/Io/GrDvrIoCliBase.h>
	
//====================================================================
//constance

#define E_GrDvrIoCliHtx5000RcvBufSize		8

//====================================================================
//global macro

//====================================================================
//global type


//====================================================================
//class

class	Cls_GrDvrIoCliHtx5000 : public Cls_GrDvrIoCliBase
{
	protected:
		__u8	m_RecvBuf[E_GrDvrIoCliHtx5000RcvBufSize];
		__u8	m_RecvIn;		//recive count
		__u8	m_RecvMax;	//packet size
		__u8	m_DirKeyIntv;

		//local function
		void	LcParsPrtc( void );
		void	LcProcQuadKey( __u8 A_Num );

	public:
    Cls_GrDvrIoCliHtx5000( __u8 A_UartCh, __u8 A_Id );
    virtual		~Cls_GrDvrIoCliHtx5000();

		virtual	BOOL8	RtlProc( void );
		virtual	void	RtlTimer( void );		// 1/10 sec timer
};
	
//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif


