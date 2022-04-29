/*
	DVR serial I/O device base

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrP4SioBase
#define	_EPC_GrP4SioBase

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrTaskSvr.h>
#include <Dvr/GrDvrBase.h>
	
//====================================================================
//constance

// plug-in command
#define E_GrP4SioPiCmdPtzMode				1			/* PTZ mode : para 0 = PTZ mode TRUE/FALSE, para 1 = channel */
#define E_GrP4SioPiCmdQuadMode			2			/* QUAD mode : para 0 = quad mode , para 1 = quad page */

//====================================================================
//global macro

//====================================================================
//global type


//====================================================================
//class

class	Cls_GrP4SioPiBase
{
	protected:
		void*	m_SioMng;
		__u8	m_UartCh;
		__u8	m_Id;

		// API

	public:
    Cls_GrP4SioPiBase( void* A_SioMng, __u8 A_Uart, __u8 A_Id );
    virtual		~Cls_GrP4SioPiBase();

		virtual	void	RtlPiCmd( __u32 A_Cmd, __u32 A_Para0, __u32 A_Para1 );

		virtual	BOOL8	RtlBkgProc( void );
		virtual	void	RtlTimer( void );			// 1/10 second period

};
	
//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif


