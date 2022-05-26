/*
	dvr ptz protocol powertech

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrDvrPtzPtPwtech
#define	_EPC_GrDvrPtzPtPwtech

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrTaskSvr.h>
#include <Dvr/GrDvrBase.h>
#include <Dvr/Ptz/GrDvrPtzPtPelcoD.h>
	
//====================================================================
//constance

//====================================================================
//global macro

//====================================================================
//global type


//====================================================================
//class

class	Cls_GrDvrPtzPtPwtech	: public Cls_GrDvrPtzPtPelcoD
{
	protected:
		void	LcSendSpcPkt( __u8 A_Val3, __u8 A_Val4, __u8 A_Val5, __u8 A_Val6, __u8 A_Val7, __u8 A_Val8, 
			__u8 A_Val9, __u8 A_Val10 ); 

	public:
    Cls_GrDvrPtzPtPwtech( __u8 A_UartCh, __u8 A_Id, __u8 A_Ch );
    virtual		~Cls_GrDvrPtzPtPwtech();

		//status
		virtual	BOOL8	IsMenuAble( void );

		//command
		virtual	void	DoMenuCall( void );
		virtual	void	DoMenuExit( void );
		virtual	void	DoMenuOk( void );
		virtual	void	DoMenuCancel( void );
		virtual	void	TourSet( Ptr_GrDvrPtzTourItm A_PtrTbl, __u8 A_Cnt );
		virtual	void	TourRun( void );
		virtual	void	TourStop( void );
};
	
//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif


