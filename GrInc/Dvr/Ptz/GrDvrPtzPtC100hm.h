/*
	dvr ptz protocol lvc c100hm

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrDvrPtzPtC100hm
#define	_EPC_GrDvrPtzPtC100hm

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrTaskSvr.h>
#include <Dvr/GrDvrBase.h>
#include <Dvr/Ptz/GrDvrPtzPtBase.h>
	
//====================================================================
//constance

//====================================================================
//global macro

//====================================================================
//global type

//====================================================================
//class

class	Cls_GrDvrPtzPtC100hm	:	public	Cls_GrDvrPtzPtBase
{
	private:

	protected:
		//local function
		void	LcSendPkt( __u8 A_Code1, __u8 A_Code2, __u8 A_Code3 );
	public:
    Cls_GrDvrPtzPtC100hm( __u8 A_UartCh, __u8 A_Id, __u8 A_Ch );
    virtual		~Cls_GrDvrPtzPtC100hm();

		virtual	__u8	DoZoom( BOOL8 A_IsWide, __u8 A_Speed, __u8 A_AutoStop );
		virtual	__u8	DoFocus( BOOL8 A_IsFar, __u8 A_Speed, __u8 A_AutoStop );
		virtual	__u8	DoIris( BOOL8 A_IsOpen, __u8 A_Speed, __u8 A_AutoStop );
		virtual	void	DoStop( void );
		virtual	void	DoAutoFocus( void );

		virtual	BOOL8	IsMenuAble( void );
		virtual	void	DoMenuCall( void );
		virtual	void	DoMenuExit( void );
		virtual	void	DoMenuLeft( void );
		virtual	void	DoMenuRight( void );
		virtual	void	DoMenuUp( void );
		virtual	void	DoMenuDown( void );

		virtual	void	ExIdView( BOOL8 A_IsOn );
		virtual	void	ExIdSet( __u8 A_IdNew ); 
};
	
//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif


