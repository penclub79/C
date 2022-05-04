/*
	dvr ptz protocol lvc c201n

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrDvrPtzPtC201n
#define	_EPC_GrDvrPtzPtC201n

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

class	Cls_GrDvrPtzPtC201n	:	public	Cls_GrDvrPtzPtBase
{
	private:
		BOOL8		m_IsAtFcs;
	protected:
		//local function
		void	LcSendPkt( __u8 A_CodeCnt, __u8 A_Code1, __u8 A_Code2, __u8 A_Code3, __u8 A_Code4 );
	public:
    Cls_GrDvrPtzPtC201n( __u8 A_UartCh, __u8 A_Id, __u8 A_Ch );
    virtual		~Cls_GrDvrPtzPtC201n();

		virtual	__u8	DoZoom( BOOL8 A_IsWide, __u8 A_Speed, __u8 A_AutoStop );
		virtual	__u8	DoFocus( BOOL8 A_IsFar, __u8 A_Speed, __u8 A_AutoStop );
		virtual	void	DoStop( void );
		virtual	void	DoAutoFocus( void );

		virtual	BOOL8	IsMenuAble( void );
		virtual	void	DoMenuCall( void );
		virtual	void	DoMenuExit( void );
		virtual	void	DoMenuOk( void );
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


