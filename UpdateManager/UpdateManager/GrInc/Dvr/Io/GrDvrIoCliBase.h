/*
	dvr io client base

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrDvrIoCliBase
#define	_EPC_GrDvrIoCliBase

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrTaskSvr.h>
#include <Dvr/GrDvrBase.h>
	
//====================================================================
//constance

//====================================================================
//global macro

//====================================================================
//global type


//====================================================================
//class

class	Cls_GrDvrIoCliBase
{
	protected:
		__u8	m_UartCh;
		__u8	m_Id;

	public:
    Cls_GrDvrIoCliBase( __u8 A_UartCh, __u8 A_Id );
    virtual		~Cls_GrDvrIoCliBase();

		virtual	void	UserAction( __u32 A_ActId );
		//virtual	__u8	EBCDIC( __u8 A_Code, BOOL8 A_IsEnc );
		virtual	BOOL8	RtlProc( void );
		virtual	void	RtlTimer( void );		// 1/10 sec timer

		virtual	void	TimeSet( Def_GrTime A_Time );//%%
		virtual	void	CheckRtcBox( void );//^^%%
		virtual	BOOL8	RetrnRtcBox( void );
		virtual	BOOL8	RetrnGPS( void );
};
	
//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif


